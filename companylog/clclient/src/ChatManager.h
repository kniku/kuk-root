#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "EcoSocket.h"
#include <QString>
#include "Options.h"
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QThread>





class ChatThread : public QThread {
	Q_OBJECT
public:
	ChatThread(QObject *parent=0);
	~ChatThread();
	QString	resultBuf;
	void setTcpSocket(QTcpSocket *_clientConnection);
protected:
	void run();
	QTcpSocket *clientConnection;
	void isReadyRead(QTcpSocket *clientConnection);
/*public slots:
	void isConnected();
	void isDisconnected();
	void isFoundHost();
	void isAboutToClose();*/
  //signals:
	//void threadedChatMessage(QString threadResultBuf);
	//void socketError(QAbstractSocket::SocketError,QString errorString);

};
	//Q_DECLARE_TR_FUNCTIONS(EcoMiniReg);
class ChatManager:public QObject {
	Q_OBJECT
	public:
	ChatManager();
	~ChatManager();
	 long getIncomingPort();


	private:
	QTcpServer *tcpServer;
	void isReadyRead(QTcpSocket *clientConnection);
   long			incomingPort;
	ChatThread *chatThread;
	QString	resultBuf;

	public slots:
	 void haveNewConnection();
	 void onChatMessage(QString message);
	signals:
	void incomingChatMessage(QString message);
	protected:
};

#endif
