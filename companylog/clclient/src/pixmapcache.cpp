#include "pixmapcache.h"

PixmapCache::PixmapCache()
{
    setPixmapSubpath("");
    autoload=true;
}

void PixmapCache::clear() {
    foreach (QIcon *valuePixmap, hashI) {
        delete valuePixmap;
    }

    foreach (QPixmap *valuePixmap, hash) {
        delete valuePixmap;
    }
    hashI.clear();
    hash.clear();
}

PixmapCache::~PixmapCache()
{
    clear();
}

void PixmapCache::setAutoload(bool autol) {
    autoload=autol;
}

void PixmapCache::setPixmapSubpath(QString subpath) {
    actPath=QApplication::applicationDirPath();
    if (subpath.size()>0) {
        actPath=actPath+"/"+subpath;
    }
}
void PixmapCache::setPixmapPath(QString path) {
    actPath=path;
}
void PixmapCache::setPixmapAlternativePath(QString path) {
    actAlternativePath=path;
}

bool PixmapCache::loadPixmap(QString fullName, QString key) {
    if (key.size()<=0)
        key=fullName;

    if (key.size()<=0) {
        return false;
    }
    if (hash.contains(key)) {
        return true;
    }
    QPixmap *tmp=new QPixmap(actPath+"/"+fullName);
    if (tmp && !tmp->isNull()) {
        insertPixmap(tmp,key);
        return true;
    }
    delete tmp;
    tmp=new QPixmap(actAlternativePath+"/"+fullName);
    if (tmp && !tmp->isNull()) {
        insertPixmap(tmp,key);
        return true;
    }
    return false;
}

void PixmapCache::insertPixmap(QPixmap *tmp, QString key) {
    if (tmp && key.size()>0) {
        hash.insert(key,tmp);
    }
}

void PixmapCache::insertMergePixmap(QPixmap *back, QPixmap *front, QString key, int size_back, double scalefactor_front) {
    QPixmap *realBack=0;
    QPixmap *realFront=0;
    if (back && front) {
        if (size_back>0.0) {
             realBack=new QPixmap(back->scaledToWidth(size_back,Qt::SmoothTransformation));
        } else {
             realBack=new QPixmap(*back);
        }
        if (realBack) {
            realFront=new QPixmap(front->scaledToWidth((int)(realBack->width()/scalefactor_front)));
        }
        if (realFront) {
            QPainter *painter=new QPainter (realBack);
            painter->drawPixmap(QPoint((int)(realBack->width()-realBack->width()/scalefactor_front),0),*realFront);
            delete painter;
            delete realFront;
            insertPixmap(realBack, key);
        }
    }
}


QPixmap * PixmapCache::getPixmap_intern(QString key) {
   if (hash.contains(key)) {
        return (QPixmap*)hash.value(key);
   } else if (autoload) {
        if (loadPixmap(key)) {
           if (hash.contains(key)) {
                return (QPixmap*)hash.value(key);
            }
        }
    }
    return 0;
}

QPixmap * PixmapCache::getPixmap(QString key) {
    QPixmap* tmp=getPixmap_intern(key);
    if (!tmp && autoload) {
        /* vordefiniertes unknown pixmap laden */
        tmp=getPixmap_intern("unknown.jpg");
        if (!tmp) {
            /* auch das unknown pixmap ist nicht vorhanden, sebst ein leeres Pixmap erzeugen */
            insertPixmap(new QPixmap(10,10), "unknown.jpg");
            tmp=getPixmap_intern("unknown.jpg");
        }
    }
    return tmp;
}

QIcon* PixmapCache::getIcon(QString key) {
    if (hashI.contains(key)) {
        return (QIcon*)hashI.value(key);
    }
    QPixmap *tmp=getPixmap(key);
    if (tmp) {
        QIcon *tmpI=new QIcon(*tmp);
        hashI.insert(key,tmpI);
        if (hashI.contains(key)) {
            return (QIcon*)hashI.value(key);
        }
    }
    return 0;
}

bool PixmapCache::contains(QString key) {
   return hash.contains(key);
}
