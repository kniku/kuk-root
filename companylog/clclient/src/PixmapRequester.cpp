/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h15
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : DockWidget.cpp
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "PixmapRequester.h"


PixmapRequester::PixmapRequester(QObject *parent) : QObject(parent) {
	Options *options=getGlobalOptionsObj();

	xScaling=0;
    unknown_inserted=false;
    if (options) {
		fprintf(stderr,"Picture Server: %s\n",options->serverName_2().toAscii().data());
		fprintf(stderr,"Picture Server Path: %s\n",options->serverPath_2().toAscii().data());
		
		qhttp=new QHttp(options->serverName_2(),options->serverPort_2());
		connect(qhttp,SIGNAL(requestFinished (int,bool)),this,SLOT(requestFinished(int,bool)));
        connect(qhttp,SIGNAL(requestStarted (int)),this,SLOT(requestStarted(int)));

        //connect(qhttp, SIGNAL(socketError(QAbstractSocket::SocketError,QString)), this, SLOT(displayError(QAbstractSocket::SocketError,QString)));

        //qBufferPerson=new QBuffer();
		qPixmapPerson=new QPixmap();
	}
}

PixmapRequester::~PixmapRequester() {
	if (qPixmapPerson) delete qPixmapPerson;
	if (qhttp) delete qhttp;
    //if (qBufferPerson) delete qBufferPerson;
    waitForPicturehash.clear();
	clearPixmaps();
}
void PixmapRequester::clearPixmaps() {
	foreach (QPixmap *valuePixmap, hash) {
		delete valuePixmap;
	}
	hash.clear();
}

void PixmapRequester::setXScaling(int width) {
	xScaling=width;
}

void PixmapRequester::requestStarted(int id) {
    //waitForPicturehash.insert(id,shortname);
}

void PixmapRequester::requestFinished(int id, bool error) {
	havePixmapFromServer=1;
    QString id_shortname=shortname;
    QBuffer *buf=0;

    if (waitForPicturehash.contains(id)) {
        id_shortname=waitForPicturehash.value(id);
        waitForPicturehash.remove(id);
        if (waitForPictureBuffer.contains(id_shortname)) {
            buf=waitForPictureBuffer.value(id_shortname);
            waitForPictureBuffer.remove(id_shortname);
        }
    }

    if (!error && buf) {
        fprintf(stderr,"Picture responsed! %d\n",(int)buf->size());
        if (buf->size()>1000) {
            qPixmapPerson->loadFromData(buf->buffer());
            fprintf(stderr,"Picture inserted %s\n",id_shortname.toAscii().data());
			if (xScaling>0) {
                hash.insert(id_shortname,new QPixmap(qPixmapPerson->scaledToWidth(xScaling,Qt::SmoothTransformation)));
			} else {
                hash.insert(id_shortname,new QPixmap(*qPixmapPerson));
			}
            emit pixmapFound(id_shortname, (QPixmap*)hash.value(id_shortname));
		} else {
            /* to small for a picture */
            emit pixmapFound(id_shortname, getLoadedPixmap("unknown"));
        }
	} else {
        emit pixmapFound(id_shortname, getLoadedPixmap("unknown"));
    }
    if (buf) delete buf;
}

void PixmapRequester::requestPixmapIntern(QString name) {
	Options *options=getGlobalOptionsObj();

    if (options && !waitForPictureBuffer.contains(name)) {
		shortname=name;

		havePixmapFromServer=0;
		fprintf(stderr,"short name: %s\n",shortname.toAscii().data());

        QBuffer *buf=new QBuffer();
        waitForPictureBuffer.insert(name,buf);
        int id=qhttp->get(options->serverPath_2()+shortname+".jpg",buf);
        waitForPicturehash.insert(id,shortname);

		//while(havePixmapFromServer==0) {
		
		//}
		fprintf(stderr,"leave RequestPixmapIntern %s State: %d\n",shortname.toAscii().data(),qhttp->state());
	}
}

bool PixmapRequester::havePixmap(QString name) {
	if (hash.contains(name)) {
		return true;
	} else {
		return false;
	}
}

void PixmapRequester::insertUnknownPictureIfNeeded() {
    if (!unknown_inserted) {
        Options *options=getGlobalOptionsObj();
        QPixmap *tmp_unknownPersonPixmap=options->pixmapCacheIcons()->getPixmap("unknown.jpg");
        if (tmp_unknownPersonPixmap) {
            QPixmap *qPixmapPersonScaledUnknown;
            if (xScaling>0) {
                qPixmapPersonScaledUnknown= new QPixmap(tmp_unknownPersonPixmap->scaledToWidth(xScaling,Qt::SmoothTransformation));
            } else {
                qPixmapPersonScaledUnknown= new QPixmap(*tmp_unknownPersonPixmap);
            }
            if (qPixmapPersonScaledUnknown) {
                hash.insert("unknown",qPixmapPersonScaledUnknown);
                unknown_inserted=true;
            }
        }
    }
}


QPixmap* PixmapRequester::getLoadedPixmap(QString name) {
    insertUnknownPictureIfNeeded();
    if (hash.contains(name)) {
		return (QPixmap*)hash.value(name);
	}
	return 0;
}

void PixmapRequester::requestPixmap(QString name) {
    insertUnknownPictureIfNeeded();

    if (!hash.contains(name)) {
		requestPixmapIntern(name);
		return;
    } else {
        fprintf(stderr,"now search in hash %s\n",name.toAscii().data());
        fprintf(stderr,"hash contains %s\n",name.toAscii().data());
        emit pixmapFound(name, (QPixmap*)hash.value(name));
	}
    return;
}
