/********************************************************************************************************
* PROGRAM      : companylog
* DATE - TIME  : Sonntag 14 Mai 2006 - 10h12
* AUTHOR       : Martin Paar
* FILENAME     : RequestObject.cpp
* LICENSE      :
* COMMENTARY   :
********************************************************************************************************/
#include "RequestObject.h"
#include <QDir>

RequestObject::RequestObject() {
	requestThread=0;
	resultBuf="";
	requestThread=new RequestThread();
	if (requestThread) {
		connect(requestThread, SIGNAL(requestFinish(QString)), this, SLOT(requestIsFinish(QString)));
		connect(requestThread, SIGNAL(socketError(QAbstractSocket::SocketError,QString)), this, SLOT(displayError(QAbstractSocket::SocketError,QString)));
	}
}

RequestObject::~RequestObject() {
	if (requestThread) {
		requestThread->exit();
		delete requestThread;
	}
}


void RequestObject::stopRequestCompanylogServer() {
	if (requestThread && requestThread->isRunning()) {
		requestThread->exit();
	}
}

//serverCallString="es3.5:PZE:000000000000000000000000000pam:0000000000";
void RequestObject::requestCompanylogServer(char *requestString) {
	//fprintf(stderr,"call start()");
	if (requestThread && requestThread->isRunning()==false) {
		requestThread->setRequestString(requestString);
		requestThread->start();
	}
}

// call from thread
void RequestObject::requestIsFinish(QString threadResultBuf) {
	fprintf(stderr,"RequestObject::requestIsFinish called...\n");
	if(requestThread) {
		resultBuf=threadResultBuf;
	} else {
		resultBuf="";
	}
	emit requestFinish();
}
// call from thread
void RequestObject::displayError(QAbstractSocket::SocketError socketError,QString errorString)
{
	switch (socketError) {
		case QAbstractSocket::RemoteHostClosedError:
			break;
		case QAbstractSocket::HostNotFoundError:
			QMessageBox::information(0, tr("Companylog"),
						tr("Hostname wurde nicht gefunden. Bitte berprfen sie den Hostnamen und Port."));
			break;
		case QAbstractSocket::ConnectionRefusedError:
			QMessageBox::information(0, tr("Companylog"),
						tr("Die Verbindung wurde vom Server zurckgewiesen. Bitte berprfen sie den Hostnamen und Port."));
			break;
		default:
			QMessageBox::information(0, tr("Companylog"),
						tr("Folgender Verbindungsfehler ist aufgetreten: %1.")
						.arg(errorString));
	}
}


/* THREAD */
/* THREAD */
/* THREAD */
/* THREAD */
/* THREAD */
/* THREAD */

RequestThread::RequestThread(QObject *parent) : QThread(parent) {
}
RequestThread::~RequestThread() {
	wait();
}

void RequestThread::run() {
	fprintf(stderr,"Thread started ...(%d)\n",(int)currentThreadId());

#if 0
/* TEST */
	QFile testfile("testlogs.txt");
	testfile.open(QIODevice::ReadOnly);
	resultBuf=testfile.readAll();
	testfile.close();
	//isReadyRead()
	//isDisconnected();
	emit requestFinish(resultBuf);
	return;
/* TEST */
#endif

	ecoSocket=new QTcpSocket(0);
/*	connect(ecoSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
	connect(ecoSocket, SIGNAL(connected()), this, SLOT(isConnected()));
	connect(ecoSocket, SIGNAL(disconnected()), this, SLOT(isDisconnected()));
	
	connect(ecoSocket, SIGNAL(hostFound()), this, SLOT(isFoundHost()),Qt::DirectConnection);
	connect(ecoSocket, SIGNAL(readyRead()), this, SLOT(isReadyRead()));
	connect(ecoSocket, SIGNAL(aboutToClose()), this, SLOT(isAboutToClose()));
*/
	resultBuf="";
	connectingToServer();
	if (ecoSocket->waitForConnected (5000)) {
		sendRequestStringToServer();
		if (ecoSocket->waitForReadyRead (50000)) {
			fprintf(stderr,"ok, have data from Server...\n");
			isReadyRead();
			while (ecoSocket->waitForReadyRead (2000)) {
				isReadyRead();
			}
		}
		disconnectFromServer();
		//fprintf(stderr,"String from Server: %s\n",resultBuf.toAscii().data());
		emit requestFinish(resultBuf);
		//exec();
	} else {
		emit socketError(ecoSocket->error(),ecoSocket->errorString());	
	}
	delete ecoSocket;
	fprintf(stderr,"Thread ended ...(%d)\n",(int)currentThreadId());
}

void RequestThread::setRequestString(QString xrequestString) {
	requestString=xrequestString;
}

void RequestThread::connectingToServer() {
	Options *options=getGlobalOptionsObj();
	if (ecoSocket && options) {
		fprintf(stderr,"Connecting to: %s (Port %ld)\n",options->serverName().toAscii().data(),options->serverPort());
		ecoSocket->connectToHost(options->serverName(),options->serverPort()/*,QIODevice::ReadWrite*/);
	}
}

void RequestThread::disconnectFromServer() {
	if (ecoSocket && ecoSocket->isValid()) {
		ecoSocket->close();
	}
}

void RequestThread::sendRequestStringToServer() {
#if 0
	QString xpath=QDir::homePath();
	if (xpath.size()<=0) {
		xpath=QApplication::applicationDirPath();
	}	

	QString user_name;
	Options *options;
	options=getGlobalOptionsObj();
	user_name=options->getUsername();
	
	QFile file(xpath+"/.companylog_"+user_name+".req");

	if (file.open(QIODevice::WriteOnly)) {
		file.write("REQUEST",strlen("REQUEST"));
		file.close();
	}
#endif

	resultBuf="";

	if (requestString.size()>0 && ecoSocket && ecoSocket->isValid()) {
		int count=ecoSocket->write(requestString.toAscii().data(),requestString.size()+1);
		ecoSocket->flush();
		fprintf(stderr,"%s\n%d Bytes send to server\n",requestString.toAscii().data(),count);
	}
}
#if 0
void RequestThread::isConnected() {
	fprintf(stderr,"connected...\n");
	/*if (ecoSocket->waitForConnected(5000)) {*/
		//sendRequestStringToServer();
	/*}*/
}

void RequestThread::isDisconnected() {
	fprintf(stderr,"disconnected...\n");
	fprintf(stderr,"refresh gui now...\n");
	emit requestFinish();
	exit();
}

void RequestThread::isFoundHost() {
	fprintf(stderr,"host found...\n");
}


void RequestThread::isAboutToClose() {
	fprintf(stderr,"isAboutToClose called\n");
	//emit requestFinish();
}
#endif
void RequestThread::isReadyRead() {
	if (ecoSocket) {
		//fprintf(stderr,"ready to read...\n");
		//ecoSocket->waitForReadyRead(5000);
		
		QByteArray arr;
		while (ecoSocket->bytesAvailable()) {
			arr = ecoSocket->readAll();
			resultBuf+=arr;
			//fprintf(stderr,resultBuf.toAscii().data());
		}
		//fprintf(stderr,resultBuf.toLatin1());
		//ecoSocket->waitForReadyRead(1000);
		//if (!ecoSocket->bytesAvailable()) {
			//isAboutToClose();
		//}
		
		/*while(ecoSocket->readLine(buf,1024)>0) {
			//arr = ecoSocket->readAll();
			//resultBuf+=arr;
			resultBuf+=buf;
		}*/
		
		/*		char buf[1024];
		int	len;
		do {
			if (ecoSocket->canReadLine()) {
				len=ecoSocket->readLine(buf,1024);
				if (len>-1) {
					QString mess(buf);
					actRequestForListWidget->mergeResult(mess);
				}
			} else len=-1;
		} while (len>-1);
		*/
		//fprintf(stderr,mess.toLatin1());
	}
}



