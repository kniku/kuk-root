/********************************************************************************************************
 * PROGRAM      : src
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h12
 * AUTHOR       :  Martin Paar
 * FILENAME     : RequestObject.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef REQUEST_OBJECT_H
#define REQUEST_OBJECT_H

#include <QThread>
#include "EcoSocket.h"
#include "Options.h"
#include <QString>
#include <QFile>

class RequestThread : public QThread {
	Q_OBJECT
public:
	RequestThread(QObject *parent=0);
	~RequestThread();
	QString	resultBuf;
	QString requestString;
	void setRequestString(QString xrequestString);
protected:
	void run();
	void connectingToServer();
	void disconnectFromServer();
	void sendRequestStringToServer();
	QTcpSocket *ecoSocket;
	void isReadyRead();
/*public slots:
	void isConnected();
	void isDisconnected();
	void isFoundHost();
	void isAboutToClose();*/
signals:
	void requestFinish(QString threadResultBuf);
	void socketError(QAbstractSocket::SocketError,QString errorString);

};

class RequestObject:public QObject {
	Q_OBJECT
	public:

    RequestObject();
	~RequestObject();
	void 	stopRequestCompanylogServer();
	void requestCompanylogServer(char *requestString);
	QString	resultBuf;

	private:
	RequestThread *requestThread;
	public slots:
	void requestIsFinish(QString threadResultBuf);
	void displayError(QAbstractSocket::SocketError socketError, QString errorString);
	signals:
	void requestFinish();
};




#endif
