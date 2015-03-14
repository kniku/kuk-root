#ifndef REQUESTOBJECTWAIT_H
#define REQUESTOBJECTWAIT_H

#include "EcoSocket.h"
#include "Options.h"
#include <QString>


class RequestObjectWait : public QObject
{
Q_OBJECT
public:
    RequestObjectWait(QObject *parent=0);
	~RequestObjectWait();
	bool         sendRequestStringToServer(QString requestStringTmp,bool keepConnectionOpen);
	QString		*getResultBuffer();
	QString		getLastErrorString();
private:
	QString		resultBuf;
    QString     requestString;
    QTcpSocket  *ecoSocket;
    //QEventLoop  loop;
    QString     lastSocketError;
    int         lastSocketErrorCode;

	bool        connectingToServer();
    void        disconnectFromServer();
    void        readStreamFromSocket();

    //public slots:
    //    void displayError(QAbstractSocket::SocketError socketError, QString errorString);
};

#endif // REQUESTOBJECTWAIT_H
