#include "ciscodealer.h"

CiscoDealer::CiscoDealer()
{
    buf=0;
    readyToCall=false;
    actual_request_id=-1;
    baseurl="https://ecor02.ecofinance.com:8443/webdialer";
    jsession="";
    cookie="";
    myNumber="";
    device="SEP0024C40D0F85";
    sess="";

    qhttp=new QHttp("ecor02.ecofinance.com",QHttp::ConnectionModeHttps,8443);
    connect(qhttp,SIGNAL(requestFinished (int,bool)),this,SLOT(requestFinished(int,bool)));
    connect(qhttp,SIGNAL(requestStarted (int)),this,SLOT(requestStarted(int)));

    connect(qhttp, SIGNAL(sslErrors(const QList<QSslError> &)),
             this, SLOT(sslErrors(const QList<QSslError> &)));
    buf=new QBuffer();
    connect(qhttp, SIGNAL(done(bool)), &loop, SLOT(quit())); // you can use a different signal here
}
CiscoDealer::~CiscoDealer()
{
    if (qhttp) {
        delete qhttp;
        qhttp=0;
    }
    if (buf) delete buf;
}

void CiscoDealer::sendRequest(QString url, int requestType) {
    if (actual_request_id==-1 && url.size()>0) {
        //buf->clear();
        actual_request_type=requestType;

        //qhttp->ignoreSslErrors();
        //qhttp->setHost("ecor02.ecofinance.com", QHttp::ConnectionModeHttps, 8443);
        //actual_request_id=qhttp->get(url,buf);


        QHttpRequestHeader header("POST", "/webdialer/Webdialer");
        header.setValue("Host", "ecor02.ecofinance.com:8443");
        //QhttpClient->setHost(domain);
        header.setValue( "User-Agent", "Mozilla/5.0 (X11; U; Linux i686; de; rv:1.9.0.12) Gecko/2009070610 Firefox/3.0.12");
        //header.setContentType("application/x-www-form-urlencoded");
        //QString searchString = "postparam=value";
        //QhttpClient->request(header,searchString.toUtf8());

        actual_request_id=qhttp->request(header,0,buf);
    }
}

void CiscoDealer::registerMe() {
    if (actual_request_id==-1) {
        buf->buffer().clear();
        actual_request_type=RequestType_RegisterMe;
        QHttpRequestHeader header("POST", "/webdialer/Webdialer");
        header.setValue("Host", "ecor02.ecofinance.com:8443");

		header.setValue("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
		header.setValue("Accept-Language","de-de,de;q=0.8,en-us;q=0.5,en;q=0.3");
		header.setValue("Accept-Encoding","gzip,deflate");
		header.setValue("Accept-Charset","ISO-8859-1,utf-8;q=0.7,*;q=0.7");
		header.setValue("Keep-Alive","300");
		header.setValue("Connection","keep-alive");
		
		
		//header.setValue( "User-Agent", "Mozilla/5.0 (Windows; U; Windows NT 5.2; de; rv:1.9.0.12) Gecko/2009070611 Firefox/3.0.12");
		header.setValue( "User-Agent", "Mozilla/5.0 (X11; U; Linux i686; de; rv:1.9.0.12) Gecko/2009070610 Firefox/3.0.12");
        QByteArray searchString = "";
       // actual_request_id=qhttp->request(header,searchString,buf);
        actual_request_id=qhttp->request(header,0,buf);
    }
}


void CiscoDealer::sendPassword() {
    if (actual_request_id==-1) {
        buf->buffer().clear();
        actual_request_type=RequestType_SendPassword;
        QHttpRequestHeader header("POST", "/webdialer/Webdialer;jsessionid="+jsession+"?cmd=doAuth&red=null");
        header.setValue("Host", "ecor02.ecofinance.com:8443");
        header.setValue( "User-Agent", "Mozilla/5.0 (X11; U; Linux i686; de; rv:1.9.0.12) Gecko/2009070610 Firefox/3.0.12");
        header.setValue( "Cookie", "JSESSIONID="+jsession);
        //header.setValue( "Content-Type", "multipart/form-data; boundary=\"--1234\"");
        //header.setValue( "Content-Type", "multipart/form-data; boundary=\"--1234\"");

        header.setContentType("application/x-www-form-urlencoded");
 //       QByteArray searchString = "sub=false&uid="+user.toAscii()+"&pwd="+passwd.toAscii();

        QByteArray pass_toPercent=QUrl::toPercentEncoding(passwd,"@");

        QByteArray searchString = "sub=false&uid="+user.toAscii()+"&pwd="+pass_toPercent;
        header.setValue( "Content-Length", QString::number(searchString.length()));

        actual_request_id=qhttp->request(header,searchString,buf);


    }
}


void CiscoDealer::sendDevice() {
    if (actual_request_id==-1) {
        buf->buffer().clear();
        actual_request_type=RequestType_SendDevice;
        QHttpRequestHeader header("POST", "/webdialer/Webdialer?cmd=doSetProfile&destination=&loc=de-de&red=null");
        header.setValue("Host", "ecor02.ecofinance.com:8443");
        

		header.setValue( "User-Agent", "Mozilla/5.0 (X11; U; Linux i686; de; rv:1.9.0.12) Gecko/2009070610 Firefox/3.0.12");
        header.setValue( "Cookie", "JSESSIONID="+jsession+"; sess="+jsession+"; uid="+user);
        //header.setValue( "Content-Type", "multipart/form-data; boundary=\"--1234\"");
        //header.setValue( "Content-Type", "multipart/form-data; boundary=\"--1234\"");

        header.setContentType("application/x-www-form-urlencoded");
        QString sepplus=sep.replace(" ","+");//SEP0024C40D0F85+;+Cisco+7911
        QByteArray searchString = "sub=true&deviceMode=permanent&permDeviceSelect="+sepplus.toAscii()+"&lineNumber="+myNumber.toAscii()+";+AT_ECO_Phones&fButton=ok&lang=null&failReason=&prevPage=";

        header.setValue( "Content-Length", QString::number(searchString.length()));

        actual_request_id=qhttp->request(header,searchString,buf);
    }
}



void CiscoDealer::callDevice() {
    if (actual_request_id==-1) {
        buf->buffer().clear();
        actual_request_type=RequestType_SendDevice;
        QHttpRequestHeader header("POST", "/webdialer/Webdialer");//SEP0024C40D0F1E ; Cisco 7911
        header.setValue("Host", "ecor02.ecofinance.com:8443");
        header.setValue( "User-Agent", "Mozilla/5.0 (X11; U; Linux i686; de; rv:1.9.0.12) Gecko/2009070610 Firefox/3.0.12");

       int index_end=sep.indexOf(" ");
       QString sepleer=sep.left(index_end);//SEP0024C40D0F85


        header.setValue( "Cookie", "JSESSIONID="+jsession+"; sess="+jsession+"; uid="+user+";device="+sepleer+"; line="+myNumber+"; lang=null; em=false; sconf=false; noclose=false");
        //header.setValue( "Content-Type", "multipart/form-data; boundary=\"--1234\"");
        //header.setValue( "Content-Type", "multipart/form-data; boundary=\"--1234\"");

        header.setContentType("application/x-www-form-urlencoded");
        QByteArray searchString = "sub=true&red=null&destination="+number.toAscii()+"&cmd=doMakeCall";

        header.setValue( "Content-Length", QString::number(searchString.length()));

        actual_request_id=qhttp->request(header,searchString,buf);
    }
}


int CiscoDealer::deal(QString xnumber, QString xpasswd, QString xuser, QString xmyNumber) {
    int rv=CiscoRequestRv_OK;
    if (xpasswd!=passwd) {
        /* passwort hat sich geaendert! -> auf jeden fall neu authentizieren */
        jsession="";
        sess="";
        readyToCall=false;
    }

    number=xnumber;
    passwd=xpasswd;
    user=xuser;
    myNumber=xmyNumber;

    if (jsession=="") {
        registerMe();
        loop.exec(QEventLoop::AllEvents|QEventLoop::WaitForMoreEvents);
        if (jsession!="") {
            sendPassword();
            loop.exec(QEventLoop::AllEvents|QEventLoop::WaitForMoreEvents);
            if (sess!="") {
                sendDevice();
                loop.exec(QEventLoop::AllEvents|QEventLoop::WaitForMoreEvents);
                if (readyToCall) {
                    callDevice();
                } else {
                    rv=CiscoRequestRv_DeviceRegisterFailed;
                }
            } else {
                rv=CiscoRequestRv_AuthenticationFailed;
            }
        } else {
            rv=CiscoRequestRv_ConnectionFailed;
        }
    } else if (sess=="") {
        sendPassword();
        loop.exec(QEventLoop::AllEvents|QEventLoop::WaitForMoreEvents);
        if (sess!="") {
            sendDevice();
            loop.exec(QEventLoop::AllEvents|QEventLoop::WaitForMoreEvents);
            if (readyToCall) {
                callDevice();
            } else {
                rv=CiscoRequestRv_DeviceRegisterFailed;
            }
        } else {
            rv=CiscoRequestRv_AuthenticationFailed;
        }
    } else if(!readyToCall) {
        sendDevice();
        loop.exec(QEventLoop::AllEvents|QEventLoop::WaitForMoreEvents);
        if (readyToCall) {
            callDevice();
        } else {
            rv=CiscoRequestRv_DeviceRegisterFailed;
        }
    } else {
        callDevice();
    }
    return rv;
}

void CiscoDealer::requestStarted(int id) {
    //waitForPicturehash.insert(id,shortname);
}

void CiscoDealer::requestFinished(int id, bool error) {
    int errid=0;
    if (!error && id==actual_request_id) {
        if (buf->size()>0) {
            fprintf(stderr,"Http responsed! %s\n",buf->buffer().data());
        }
        if (actual_request_type==RequestType_RegisterMe) {
            QHttpResponseHeader resp=qhttp->lastResponse ();
            jsession="";
            sess="";
            readyToCall=false;
            cookie=resp.value("Set-Cookie");
            QByteArray cookieArray(cookie.toAscii());
            QList<QNetworkCookie>nc=QNetworkCookie::parseCookies (cookieArray);
            for (int i=0;i<nc.count();i++) {
                QNetworkCookie nnx=nc[i];
                if (nnx.name()=="JSESSIONID") {
                    jsession=nnx.value();
                }
            }
            if (jsession=="") {
                jsession="";
                sess="";
                readyToCall=false;
            }
        }
		QByteArray xxxx;
        if (actual_request_type==RequestType_SendPassword) {
            sess="";
            readyToCall=false;
            if (buf->size()>0) {
                if (buf->buffer().contains("Use permanent device") || buf->buffer().contains("Permanentes") ) {
					sess=jsession;
                }
				xxxx=buf->buffer();
                if (buf->buffer().contains("SEP")) {
                    int indexx=buf->buffer().indexOf("SEP");
                    if (indexx>0) {
                       QString tmp=buf->buffer().mid(indexx,50);
                       int index_end=tmp.indexOf("\n");
                       sep=tmp.left(index_end);
                    }
                }
            }
            if (sess=="") {
                jsession="";
                sess="";
                readyToCall=false;
            }
        }
        if (actual_request_type==RequestType_SendDevice) {
            readyToCall=false;
            if (buf->size()>0) {
                if (buf->buffer().contains("Phone number")) {
                    readyToCall=true;
                }
            }
            if (!readyToCall) {
                jsession="";
                sess="";
                readyToCall=false;
            }
        }
    }
    if (id==actual_request_id) {
        actual_request_id=-1;
        actual_request_type=-1;
    }
#if 1
    QHttpRequestHeader req=qhttp->currentRequest();
    QString abcc=req.value("User-Agent");
    QString xxx=req.toString();

    QHttpResponseHeader resp=qhttp->lastResponse ();
    QString yyy=resp.toString();
    QString reas=resp.reasonPhrase();

    if (id==actual_request_id) {
        if (!error) {
            if (actual_request_type==RequestType_RegisterMe) {
                if (jsession.size()>0) {
                    actual_request_id=-1;
                    actual_request_type=-1;
                    //sendPassword();
                }
            }
        }
        actual_request_id=-1;
        actual_request_type=-1;
    }
#endif
    if(error) {
        errid=qhttp->error();
    }
#if 0
    if (!error && buf) {
        fprintf(stderr,"Http responsed! %d\n",(int)buf->size());
        if (buf->size()>1000) {
            qPixmapPerson->loadFromData(buf->buffer());
            fprintf(stderr,"Picture inserted %s\n",id_shortname.toAscii().data());
            if (xScaling>0) {
                hash.insert(id_shortname,new QPixmap(qPixmapPerson->scaledToWidth(xScaling,Qt::SmoothTransformation)));
            } else {
                hash.insert(id_shortname,new QPixmap(*qPixmapPerson));
            }
            emit pixmapFound(id_shortname, (QPixmap*)hash.value(id_shortname));
        } else {
            /* to small for a picture */
            emit pixmapFound(id_shortname, getLoadedPixmap("unknown"));
        }
    } else {
        emit pixmapFound(id_shortname, getLoadedPixmap("unknown"));
    }
#endif
}
void CiscoDealer::sslErrors(const QList<QSslError> &errors)
 {
     QString errorString;
     foreach (const QSslError &error, errors) {
         if (!errorString.isEmpty())
             errorString += ", ";
         errorString += error.errorString();
     }
     qhttp->ignoreSslErrors();

#if 0
     if (QMessageBox::warning(this, tr("HTTP Example"),
                              tr("One or more SSL errors has occurred: %1").arg(errorString),
                              QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
     qhttp->ignoreSslErrors();
    QIODevice *sslx=qhttp->currentSourceDevice ();

    if (sslx) {
        QSslSocket *abc=static_cast<QSslSocket*>(sslx);
        if (abc) {
            //abc->ignoreSslErrors();
            if (abc->supportsSsl()) {
                int x=1;
                x++;
            }
        }
    }
#endif

 }

