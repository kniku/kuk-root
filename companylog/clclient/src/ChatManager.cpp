/********************************************************************************************************
* PROGRAM      : companylog
* DATE - TIME  : Sonntag 02 Jul 2006 - 10h12
* AUTHOR       : Martin Paar
* FILENAME     : EcoMiniReg.cpp
* LICENSE      :
* COMMENTARY   :
********************************************************************************************************/
#include "ChatManager.h"




ChatManager::ChatManager() {
  incomingPort=-1;
  chatThread=0;

#if THREAD_BASED	
  chatThread=new ChatThread();
  if (chatThread) {
		 //connect(chatThread, SIGNAL(threadedChatMessage(QString)), this, SLOT(onChatMessage(QString)));
  }
#endif
  tcpServer=new QTcpServer();
	if (tcpServer) {
		connect(tcpServer, SIGNAL(newConnection()), this, SLOT(haveNewConnection()));
		if (!tcpServer->listen()) {
        /*QMessageBox::critical(this, tr("Chat Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));*/
		} else {
			incomingPort=tcpServer->serverPort();
			fprintf(stderr,"Chat Server horcht auf %ld\n",incomingPort);
		}
	}
}

ChatManager::~ChatManager() {
#if THREAD_BASED	
	if (chatThread) {
		chatThread->exit();
		delete chatThread;
	}
#endif
	if (tcpServer) {
		delete tcpServer;
	}
}


long ChatManager::getIncomingPort() {
  return incomingPort;
}

void ChatManager::haveNewConnection()
{
   if (tcpServer) {
	 QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
	 if (clientConnection) {
		fprintf(stderr,"have incoming connection on Port %ld\n",incomingPort);
		connect(clientConnection, SIGNAL(disconnected()),clientConnection, SLOT(deleteLater()));
#if THREAD_BASED	
		 if (chatThread) {
			 if (chatThread->isRunning()==false) {
				chatThread->setTcpSocket(clientConnection);
				chatThread->start();

				chatThread->wait();
			 } else {
				chatThread->wait();
				chatThread->setTcpSocket(clientConnection);
				chatThread->start();

				chatThread->wait();
			 }
			 if (chatThread->resultBuf.size()>0) {
				emit incomingChatMessage(chatThread->resultBuf);
			 }
		} else {
#endif
			/* nicht thread-basiert */
			resultBuf="";
			if (clientConnection->waitForReadyRead (5000)) {
				fprintf(stderr,"ok, have data from Chat Server...\n");
				isReadyRead(clientConnection);
				while (clientConnection->waitForReadyRead (2000)) {
					isReadyRead(clientConnection);
				}
			}
			clientConnection->disconnectFromHost();			
			if (resultBuf.size()>0) {
				emit incomingChatMessage(resultBuf);
			}
#if THREAD_BASED	
		 }
#endif
	 }
  }
}

void ChatManager::isReadyRead(QTcpSocket *clientConnection) {
	if (clientConnection) {
		//fprintf(stderr,"ready to read...\n");
		//ecoSocket->waitForReadyRead(5000);
		
		QByteArray arr;
		while (clientConnection->bytesAvailable()) {
			arr = clientConnection->readAll();
			resultBuf+=arr;
			//fprintf(stderr,resultBuf.toAscii().data());
		}
	}
}


/* derzeit unbenutzt */
void ChatManager::onChatMessage(QString message) {
  emit incomingChatMessage(message);  
}


/* THREAD */
/* THREAD */
/* THREAD */
/* THREAD */

ChatThread::ChatThread(QObject *parent) : QThread(parent) {
}
ChatThread::~ChatThread() {
	wait();
}

void ChatThread::setTcpSocket(QTcpSocket *_clientConnection) {
	clientConnection=_clientConnection;
}


void ChatThread::run() {
	fprintf(stderr,"Chat Thread started (incoming connection)...(%d)\n",(int)currentThreadId());
   if (clientConnection) {
		resultBuf="";
	   if (clientConnection->waitForReadyRead (5000)) {
			fprintf(stderr,"ok, have data from Chat Server...\n");
			 isReadyRead(clientConnection);
			 /*while (clientConnection->waitForReadyRead (1000)) {
				isReadyRead(clientConnection);
			 }*/
		}
		/* clientConnection->disconnectFromHost();*/
	 }
	 if (resultBuf.size()>0) {
		 fprintf(stderr,"Message:%s thread: %d\n",resultBuf.toAscii().data(),(int)currentThreadId());
		 //emit threadedChatMessage(resultBuf);
	 }
	 clientConnection=0;
	fprintf(stderr,"Chat Thread ended ...(%d)\n",(int)currentThreadId());
}

void ChatThread::isReadyRead(QTcpSocket *clientConnection) {
	if (clientConnection) {
		//fprintf(stderr,"ready to read...\n");
		//ecoSocket->waitForReadyRead(5000);
		
		QByteArray arr;
		while (clientConnection->bytesAvailable()) {
			arr = clientConnection->readAll();
			resultBuf+=arr;
			//fprintf(stderr,resultBuf.toAscii().data());
		}
	}
}



