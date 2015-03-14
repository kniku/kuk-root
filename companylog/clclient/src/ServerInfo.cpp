/********************************************************************************************************
* PROGRAM      : companylog
* DATE - TIME  : Sonntag 29 Mar 2008 - 10h12
* AUTHOR       : Martin Paar
* FILENAME     : ServerInfo.cpp
* LICENSE      :
* COMMENTARY   :
********************************************************************************************************/
#include "ServerInfo.h"



ServerInfo::ServerInfo() {
	requestObject=new RequestObject();
	if (requestObject) {
		connect(requestObject, SIGNAL(requestFinish()), this, SLOT(requestIsFinish()));
	}
}
ServerInfo::~ServerInfo() {
	if (requestObject) {
		delete requestObject;
	}
}

void ServerInfo::requestCurrentServerInfo() {
	char 	command[1000]={0};
	//int		count=0;
	Options *options;
	options=getGlobalOptionsObj();
	if (options) {
		if (requestObject) {
			//#INF:uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu:
			sprintf(command,"es3.5:INF:%-30s:",options->getUsername().toAscii().data());
			requestObject->requestCompanylogServer(command);
		}
	}
}
void ServerInfo::requestIsFinish() {
	QRegExp rx;
	QString resultBuf;
	/*rx.setMinimal(true);
	rx.setPatternSyntax(QRegExp::RegExp);*/
 //rx("^([I])\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)$");
	rx.setPattern("~*:(.*):(.*):(.*):(.*)"); /* [\r]*\n$ */

	//fprintf(stderr,"ServerInfo::onRequestFinish called (user State)...\n");
	//fprintf(stderr,"User State: %s<<\n",requestObject->resultBuf.toAscii().data());
	
	/*if (!rx.isValid()) {
		fprintf(stderr,"%s\n",rx.errorString().toAscii().data());
	}*/
	
	if (requestObject->resultBuf.size()>0) {
		resultBuf=requestObject->resultBuf;
	    if (rx.indexIn(resultBuf) != -1) {
			/*for (int i=1;i<15;i++) {
			fprintf(stderr,"%d: %s>>\n",i,rx.cap(i).toAscii().data());
			}*/

			version=rx.cap(1);
			uptime=rx.cap(2);
			requests=rx.cap(3);
			threads=rx.cap(4);

			fprintf(stderr,"Server Info [Version: %s, Uptime: %s, Requests: %s, Threads: %s]",version.toAscii().data(),uptime.toAscii().data(),requests.toAscii().data(),threads.toAscii().data());
			emit serverRequestIsFinish();
	    }
	}
}

QString ServerInfo::getVersion() {
	return version;
}
QString ServerInfo::getUptime() {
	return uptime;
}
QString ServerInfo::getRequests() {
	return requests;
}
QString ServerInfo::getThreads() {
	return threads;
}
