#include "requestobjectwait.h"

RequestObjectWait::RequestObjectWait(QObject *parent) : QObject(parent)
{
    ecoSocket=new QTcpSocket(0);
	requestString="";
	resultBuf="";
	lastSocketError="";
	lastSocketErrorCode=0;
}
RequestObjectWait::~RequestObjectWait()
{
    if (ecoSocket) {
        delete ecoSocket;
    }
}

bool RequestObjectWait::connectingToServer() {
    Options *options=getGlobalOptionsObj();
    if (ecoSocket && options) {
        if (ecoSocket->isOpen()) {
			/* Socket is already open */
			return true;
        }
		fprintf(stderr,"Connecting to: %s (Port %ld)\n",options->serverName().toAscii().data(),options->serverPort()+1);
		ecoSocket->connectToHost(options->serverName(),options->serverPort()+1/*,QIODevice::ReadWrite*/);
        if (ecoSocket->waitForConnected (5000)) {
			/* juhu connected! */
			return true;
        } else {
			/* cannot connect! */
			lastSocketError=ecoSocket->errorString();
			lastSocketErrorCode=ecoSocket->error();
        }
    }
    return false;
}

void RequestObjectWait::disconnectFromServer() {
    if (ecoSocket && ecoSocket->isValid()) {
        ecoSocket->close();
    }
}

bool RequestObjectWait::sendRequestStringToServer(QString requestStringTmp, bool keepConnectionOpen) {
    requestString=requestStringTmp;
    resultBuf="";
    lastSocketError="";
    lastSocketErrorCode=0;
	bool rv=false;

    if (ecoSocket) {
		if (requestString.size()>0) {
			if (connectingToServer()) {
                                int count=ecoSocket->write(requestString.toAscii().data(),requestString.size()/*+1*/);
                ecoSocket->flush();
                fprintf(stderr,"%s\n%d Bytes send to server\n",requestString.toAscii().data(),count);

                if (ecoSocket->waitForReadyRead (50000)) {
                    fprintf(stderr,"ok, have data from Server...\n");
                    readStreamFromSocket();
                    while (ecoSocket->waitForReadyRead (2000)) {
                        readStreamFromSocket();
                    }
                }
				if (!keepConnectionOpen) {
					disconnectFromServer();
				}
				rv=true;
            }
        }
    }
	return rv;
}

void RequestObjectWait::readStreamFromSocket() {
    if (ecoSocket) {
        QByteArray arr;
        while (ecoSocket->bytesAvailable()) {
            arr = ecoSocket->readAll();
            resultBuf+=arr;
        }
    }
}

QString *RequestObjectWait::getResultBuffer() {
	return &resultBuf;
}
QString RequestObjectWait::getLastErrorString() {
	return lastSocketError;
}
