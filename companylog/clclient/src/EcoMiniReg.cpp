/********************************************************************************************************
* PROGRAM      : companylog
* DATE - TIME  : Sonntag 02 Jul 2006 - 10h12
* AUTHOR       : Martin Paar
* FILENAME     : EcoMiniReg.cpp
* LICENSE      :
* COMMENTARY   :
********************************************************************************************************/
#include "EcoMiniReg.h"



EcoMiniReg::EcoMiniReg() {
	notfirstRequest=false;
	lastStateRefreshTimerId=-1;
	requestObject=new RequestObject();
	if (requestObject) {
		connect(requestObject, SIGNAL(requestFinish()), this, SLOT(requestIsFinish()));
	}
	//m_currentState=currentUserStateNout;
}

EcoMiniReg::~EcoMiniReg() {
	if (requestObject) {
		delete requestObject;
	}
}

int EcoMiniReg::sendCommandToServer(QString command, int serverPort) {
    Options *options=getGlobalOptionsObj();
    int count=0;
    if (options && command.size()>0) {
        EcoSocket ecoSocket;
        fprintf(stderr,"%s:%d\n",options->serverName().toAscii().data(),serverPort);
        ecoSocket.connectToHost(options->serverName(),serverPort,QIODevice::ReadWrite);
        if (!ecoSocket.waitForConnected(5000)) {
            QMessageBox::information(0, tr("Companylog"),
                    tr("The host was not found. Please check the "
                    "host name and port settings."));
        } else {
            count=ecoSocket.write(command.toAscii().data(),command.size()+1/*strlen(command)+1*/);
            fprintf(stderr,"%s\n%d Bytes send to server\n",command.toAscii().data(),count);
        }
        ecoSocket.flush();
        ecoSocket.close();
    }
    return count;
}


int EcoMiniReg::sendCommandToServer(QString command) {
    int count=0;
    Options *options=getGlobalOptionsObj();
    count=sendCommandToServer(command, options->serverPort());

	return count;
}


int EcoMiniReg::LogWithMessage(QString message, int in) {
	Options *options;
	int		count=0;
	char 	command[1000]={0};
	options=getGlobalOptionsObj();
	if (options) {
		m_currentMemberLogFromLastLogAction.anm=message;
		m_currentMemberLogFromLastLogAction.in=in;
		m_currentMemberLogFromLastLogAction.date=QDate::currentDate();

		sprintf(command,"es3.5:ENT:%-30s:%s:K:%-30.30s",options->getUsername().toAscii().data(),in==1?"I":"O",message.toAscii().data());
		count=sendCommandToServer(command);
	}
	return count;
}

int EcoMiniReg::LogIn() {
	return LogWithMessage(" ", 1);
}
int EcoMiniReg::LogOut() {
	return LogWithMessage(" ", 0);
}
int EcoMiniReg::LogOutMittag() {
	return LogWithMessage("bin beim Mittagessen", 0);
}
int EcoMiniReg::LogOutWWW() {
	return LogWithMessage("www", 0);
}
int EcoMiniReg::LogInKlBspr() {
	return LogWithMessage("kl. Besprechungsraum", 1);
}
int EcoMiniReg::LogInBspr() {
	return LogWithMessage("gr. Besprechungsraum", 1);
}
int EcoMiniReg::LogInWartung() {
	return LogWithMessage("Wartungsraum", 1);
}
int EcoMiniReg::LogInJourfix() {
	return LogWithMessage("Jourfix", 1);
}
int EcoMiniReg::LogOutRauch() {
	return LogWithMessage("Pause", 0);
}

int EcoMiniReg::LogSonstigIn(QString text) {
	return LogWithMessage(text, 1);
}
int EcoMiniReg::LogSonstigOut(QString text) {
	return LogWithMessage(text, 0);
}

int EcoMiniReg::SendChatMessage(QString to, QString message) {
  /* char xmessage[1000]={0};*/
  Options *options=getGlobalOptionsObj();
  if (options && message.size()>0) {
	  fprintf(stderr,"Chat Plain Message:%s\n",message.toAscii().data());	  
	  QDomDocument doc;
	  QDomElement root=doc.createElement("msg");
	  doc.appendChild(root);
	  root.setAttribute("l",options->getUsername());
	  root.setAttribute("to",to);
	  root.setAttribute("m",message.replace(">","SP9PS-SP8PS"));
		
	  QString qmessage=doc.toString();
	  qmessage=qmessage.replace("SP9PS-SP8PS","&gt;");

	  fprintf(stderr,"Chat XML-String:%s\n",qmessage.toAscii().data());	  
	  
	  //sprintf(xmessage,"<msg l=\"%s\" to=\"%s\" m=\"%s\"/>",options->getUsername().toAscii().data(),to,message);
	 if (sendCommandToServer(qmessage)>0) {
		return 1;
	 }
  } else {
	 return 1;
  }
  return 0;
}


int EcoMiniReg::RegisterChatPort(long port) {
  char message[100]={0};
  Options *options=getGlobalOptionsObj();
  
  if (options) {
	 sprintf(message,"<reg l=\"%s\" port=\"%ld\"/>",options->getUsername().toAscii().data(),port);
         //fprintf(stderr,message);
	 if (sendCommandToServer(message)>0) {
		return 1;
	 }
  }
  return 0;
}

int EcoMiniReg::UnregisterChatPort(long port) {
  char message[100]={0};
  Options *options=getGlobalOptionsObj();
  
  if (options) {
	 sprintf(message,"<unreg l=\"%s\" port=\"%ld\"/>",options->getUsername().toAscii().data(),port);
	 fprintf(stderr,message);
	 if (sendCommandToServer(message)>0) {
		return 1;
	 }
  }
  return 0;
}


void EcoMiniReg::requestCurrentUserStateFromServer() {
	char 	command[1000]={0};
	//int		count=0;
	Options *options;
	options=getGlobalOptionsObj();
	if (options) {
		if (requestObject) {
			//#QMI:uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu:I|F|K:Pattern(30):Firma(30)
			sprintf(command,"es3.5:QMI:%-30s:K:%-30s:%-30s",options->getUsername().toAscii().data(),options->getUsername().toAscii().data(),"?");
			requestObject->requestCompanylogServer(command);
		}
	}
}


void EcoMiniReg::requestIsFinish() {
	QRegExp rx;
	/*rx.setMinimal(true);
	rx.setPatternSyntax(QRegExp::RegExp);*/
 //rx("^([I])\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)\\x02(.*)$");
	rx.setPattern("^([I])\\x2(.*)\\x2(.*)\\x2(.*)\\x2(.*)\\x2(.*)\\x2(.*)\\x2(.*)\\x2(.*)\\x2(.*)\\x2(.*)\\x2(.*)\\x2(.*)\\x2(.*)\\x2(.*)[\r]*\n$");
	QString inout;
	bool	in=false;
	QString info;
	QString	resultBuf;
	QString vname;
	QString name;	
		
	//fprintf(stderr,"EcoMiniReg::onRequestFinish called (user State)...\n");
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
		
	        vname = rx.cap(2);
	        name = rx.cap(3);
	        inout = rx.cap(13);
	        info = rx.cap(15);
			fprintf(stderr,"current user [Vorname: %s, Name: %s] is [%s] Log: [%s]\n",vname.toAscii().data(),name.toAscii().data(),inout.toAscii().data(),info.toAscii().data());
			in=inout=="IN"?true:false;
			if (notfirstRequest==false || m_currentMemberLog.anm!=info || in!=m_currentMemberLog.in) {
	    		notfirstRequest=true;
	    		m_currentMemberLog.anm=info;
	    		m_currentMemberLog.in=in;
				m_currentMemberLog.date=QDate::currentDate();
				emit currentUserStateChanged(&m_currentMemberLog);
			}
	    }
	}
}


MemberLog EcoMiniReg::currentUserState() {
	return m_currentMemberLog;
}

/* letzter Login/out versuch ist mit diesen Daten erfolgt, heisst aber noch nicht dass das auch geklappt hat */
MemberLog EcoMiniReg::currentUserStateFromLastLogAction() {
	return m_currentMemberLogFromLastLogAction;
}

void EcoMiniReg::timerEvent(QTimerEvent *event) {
	if (event->timerId()==lastStateRefreshTimerId) {
		killTimer(event->timerId());
        lastStateRefreshTimerId=-1;
		requestCurrentUserStateFromServer();
		startStateRefreshTimer();
	}
}

void EcoMiniReg::startStateRefreshTimer() {
	/*Options *options;
	options=getGlobalOptionsObj();
	if (options) {
		long sec=options->timerSequence();
		if (sec<=10) sec=60;
		lastTimerId=startTimer(sec*1000);
	} else {*/
	lastStateRefreshTimerId=startTimer(180*1000);
	/*}*/	
}

