#ifndef PIXMAPCACHE_H
#define PIXMAPCACHE_H

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QHash>
#include <QFile>
#include <QIcon>
#include <QApplication>
#include <QPainter>

class PixmapCache : public QObject
{
Q_OBJECT
public:
    PixmapCache();
    ~PixmapCache();
    void setPixmapSubpath(QString subpath);
    void setPixmapPath(QString path);
    void setPixmapAlternativePath(QString path);
    bool loadPixmap(QString FullName, QString key="");
    void setAutoload(bool autol);
    QPixmap * getPixmap(QString fullName);
    QIcon * getIcon(QString fullName);
    void insertPixmap(QPixmap *tmp, QString key);
    void insertMergePixmap(QPixmap *back, QPixmap *front, QString key, int size_back, double scalefactor_front);
    void clear();
    bool contains(QString key);
private:
    QHash<QString, QPixmap*> hash;
    QHash<QString, QIcon*> hashI;
    QString actPath;
    QString actAlternativePath;
    QPixmap * getPixmap_intern(QString key);

    bool autoload;
};

#endif // PIXMAPCACHE_H


