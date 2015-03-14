/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 3 Mar 2009 - 17h15
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : PixmapRequester.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef PIXMAP_REQUESTER_H
#define PIXMAP_REQUESTER_H

#include <QDockWidget>
#include <QPixmap>
#include <QIODevice>
#include <QHttp>
#include <QBuffer>
#include "Options.h"
#include <QHash>

class PixmapRequester: public QObject {
	Q_OBJECT
	public:
	QHttp	*qhttp;
	QPixmap	*qPixmapPerson;
	QHash<QString, QPixmap*> hash;
	QString	shortname;
    //QBuffer *qBufferPerson;

	PixmapRequester(QObject *parent=0);
	~PixmapRequester();
	void requestPixmap(QString name);
	void setXScaling(int width);
	bool havePixmap(QString name);
	void clearPixmaps();
	QPixmap* getLoadedPixmap(QString name);

	public slots:
	void requestFinished(int id, bool error);
    void requestStarted(int id);
    //void requestError(QAbstractSocket::SocketError socketError, QString errorString);
protected:
	int havePixmapFromServer;
	int	xScaling;
    bool unknown_inserted;
	void requestPixmapIntern(QString name);
    void insertUnknownPictureIfNeeded();
    QHash<int, QString>      waitForPicturehash;
    QHash<QString, QBuffer*> waitForPictureBuffer;
    signals:
	void pixmapFound(QString name, QPixmap *pixmap);

};



#endif
