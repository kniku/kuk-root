/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h21
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : Options.cpp
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "Options.h"

Options::Options() {
	QString appPath=QApplication::applicationDirPath();
    companylogUserHomePath="";
    logManager_intern=0;
    m_ciscoPass="";
    m_serverName="ereg.ecofinance.com";
	m_serverName_2="is.ecofinance.com";
	m_serverPort=7881;
	m_serverPort_2=80;
	m_serverPath_2="/bib/uimages/";
	m_timerSequence=300;
	m_autoLogout=false;
	m_inaktive=true;
	m_autoLogin=false;
	m_wanIfLogOut=false;
	m_wanIfLogOutTimer=1;	

    m_notifierTimer=0;
    m_notifierX=-1;
    m_notifierY=-1;
    m_notifierShow=true;
    m_showPersonPicture=true;

    pixmapCacheIcons()->setPixmapPath(getIconPath());
    pixmapCacheEmoticons()->setPixmapPath(getEmoticonPath());
    loadLogOptions();

#if 0
    pixmapCacheIcons()->insertMergePixmap(pixmapCacheIcons()->getPixmap("companylogsmal.png"),
                                          pixmapCacheIcons()->getPixmap("out.png"), "out_mix.png", 0, 1.4);
    pixmapCacheIcons()->insertMergePixmap(pixmapCacheIcons()->getPixmap("companylogsmal.png"),
                                          pixmapCacheIcons()->getPixmap("in.png"), "in_mix.png", 0, 1.4);
    pixmapCacheIcons()->insertMergePixmap(pixmapCacheIcons()->getPixmap("companylogsmal.png"),
                                          pixmapCacheIcons()->getPixmap("www.png"), "www_mix.png", 0, 1.4);
    pixmapCacheIcons()->insertMergePixmap(pixmapCacheIcons()->getPixmap("companylogsmal.png"),
                                          pixmapCacheIcons()->getPixmap("mittag.png"), "mittag_mix.png", 0, 1.4);
    pixmapCacheIcons()->insertMergePixmap(pixmapCacheIcons()->getPixmap("companylogsmal.png"),
                                          pixmapCacheIcons()->getPixmap("rauch.png"), "rauch_mix.png", 0, 1.4);
    pixmapCacheIcons()->insertMergePixmap(pixmapCacheIcons()->getPixmap("companylogsmal.png"),
                                          pixmapCacheIcons()->getPixmap("binbei.png"), "binbei_mix.png", 0, 1.4);
    pixmapCacheIcons()->insertMergePixmap(pixmapCacheIcons()->getPixmap("companylogsmal.png"),
                                          pixmapCacheIcons()->getPixmap("besprechung.png"), "besprechung_mix.png", 0, 1.4);
    pixmapCacheIcons()->insertMergePixmap(pixmapCacheIcons()->getPixmap("companylogsmal.png"),
                                          pixmapCacheIcons()->getPixmap("klbesprechung.png"), "klbesprechung_mix.png", 0, 1.4);
    pixmapCacheIcons()->insertMergePixmap(pixmapCacheIcons()->getPixmap("companylogsmal.png"),
                                          pixmapCacheIcons()->getPixmap("wartung.png"), "wartung_mix.png", 0, 1.4);
    pixmapCacheIcons()->insertMergePixmap(pixmapCacheIcons()->getPixmap("companylogsmal.png"),
                                          pixmapCacheIcons()->getPixmap("jourfix.png"), "jourfix_mix.png", 0, 1.4);
    pixmapCacheIcons()->insertMergePixmap(pixmapCacheIcons()->getPixmap("companylogsmal.png"),
                                          pixmapCacheIcons()->getPixmap("sonstout.png"), "sonstout_mix.png", 0, 1.4);
    pixmapCacheIcons()->insertMergePixmap(pixmapCacheIcons()->getPixmap("companylogsmal.png"),
                                          pixmapCacheIcons()->getPixmap("sonstin.png"), "sonstin_mix.png", 0, 1.4);
    pixmapCacheIcons()->insertMergePixmap(pixmapCacheIcons()->getPixmap("companylogsmal.png"),
                                          pixmapCacheIcons()->getPixmap("home.png"), "home_mix.png", 0, 1.4);
#endif
}


Options::~Options() {
}

void Options::loadLogOptions() {
    bool    loaded=false;
    QString fn=getLogOptionFilenameFromUser();
    if (fn.size()>0) {
        QFile file(fn);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray f=file.readAll();
            if (f.size()>0) {
                loaded=true;
                logManager()->setLogActionsFromXml(f);
                logManager()->setLogIdentifiersFromXml(f);
            }
        }
    }
    if (!loaded) {
       QFile file(QApplication::applicationDirPath()+"/defaultlogsettings.xml");
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray f=file.readAll();
            if (f.size()>0) {
                loaded=true;
                logManager()->setLogActionsFromXml(f);
                logManager()->setLogIdentifiersFromXml(f);
            }
        }
    }
}

void Options::saveLogOptions() {
    /* TODO request logManager! */
    QString xmlLogOptions=logManager()->createXmlString();

    QString fn=getLogOptionFilenameFromUser();
    if (fn.size()>0) {
        QFile file(fn);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(xmlLogOptions.toAscii());
            file.close();
        }
    }
}

void Options::setOption(QString line) {
	QStringList qStringList=line.split("=");

	if (qStringList.count()==2) {
		if (qStringList[0]=="Server" && qStringList[1].size()>1) {
			setServerName(qStringList[1].simplified());
        } else if (qStringList[0]=="Server2" && qStringList[1].size()>1) {
			setServerName_2(qStringList[1].simplified());
		} else if (qStringList[0]=="ServerPath2" && qStringList[1].size()>1) {
			setServerPath_2(qStringList[1].simplified());
		} else if (qStringList[0]=="Port") {
			setServerPort(qStringList[1].simplified().toInt());
		} else if (qStringList[0]=="Port2") {
			setServerPort_2(qStringList[1].simplified().toInt());
		} else if (qStringList[0]=="Timer") {
			setTimerSequence(qStringList[1].simplified().toInt());
		} else if (qStringList[0]=="Name") {
			setName(qStringList[1].simplified());
		} else if (qStringList[0]=="VName") {
			setVName(qStringList[1].simplified());
		} else if (qStringList[0]=="Year") {
			setYear(qStringList[1].simplified());
		} else if (qStringList[0]=="Month") {
			setMonth(qStringList[1].simplified());
		} else if (qStringList[0]=="Day") {
			setDay(qStringList[1].simplified());
		} else if (qStringList[0]=="Font") {
			setFontName(qStringList[1].simplified());
		} else if (qStringList[0]=="WarnIfLogOutTimer") {
			setWanIfLogOutTimer(qStringList[1].simplified().toInt());
		} else if (qStringList[0]=="AutoLogout") {
			setAutoLogout(qStringList[1].simplified().toInt());
		} else if (qStringList[0]=="Inaktive") {
			setInaktive(qStringList[1].simplified().toInt());
		} else if (qStringList[0]=="AutoLogin") {
			setAutoLogin(qStringList[1].simplified().toInt());
		} else if (qStringList[0]=="WarnIfLogOut") {
			setWanIfLogOut(qStringList[1].simplified().toInt());
		} else if (qStringList[0]=="NotifierTimer") {
			setNotifierTimer(qStringList[1].simplified().toInt());
		} else if (qStringList[0]=="NotifierX") {
			setNotifierX(qStringList[1].simplified().toInt());
		} else if (qStringList[0]=="NotifierY") {
			setNotifierY(qStringList[1].simplified().toInt());
                } else if (qStringList[0]=="ShowNotifierDlg") {
                        setNotifierDlg(qStringList[1].simplified().toInt());
                } else if (qStringList[0]=="ShowPersonPicture") {
			setShowPersonPicture(qStringList[1].simplified().toInt());
		} else if (qStringList[0]=="ListState") {
			QByteArray tmp(qStringList[1].toAscii().data());
			tmp=tmp.replace("@","=");
			setListState(tmp.fromBase64(tmp));
        } else if (qStringList[0]=="CiscoPass") {
            QByteArray tmp(qStringList[1].toAscii().data());
            tmp=tmp.replace("@","=");
            setCiscoPass(tmp.fromBase64(tmp));
        } else if (qStringList[0]=="DockState1.5") {
			QByteArray tmp(qStringList[1].toAscii().data());
			tmp=tmp.replace("@","=");
			setDockState(tmp.fromBase64(tmp));
        } else if (qStringList[0]=="MainWindowState") {
            QByteArray tmp(qStringList[1].toAscii().data());
            tmp=tmp.replace("@","=");
            setMainWindowState(tmp.fromBase64(tmp));
        }
	}
}

PixmapCache *Options::pixmapCacheIcons() {
    return &pixmapCache_Icons;
}
PixmapCache *Options::pixmapCacheEmoticons() {
    return &pixmapCache_Emoticons;
}
LogManager *Options::logManager() {
    if (!logManager_intern) {
        logManager_intern=new LogManager();
        logManager_intern->setupIconPaths(getLogIconPath(), getLogIconPrivatePath());
    }
    return logManager_intern;
}

void Options::readOptionsFromFile() {
	QString	filename;
	filename=getFilenameFromUser();	
	
	QFile file(filename);
	char buf[1024];
	if (file.open(QIODevice::ReadOnly)) {
		while (file.readLine(buf, sizeof(buf))!=-1) {
			// the line is available in buf
			QString qbuf(buf);
			setOption(qbuf);
		}
		file.close();
	}
}
void Options::saveOptionsToFile(QString filename) {
    QFile file(filename);
    char buf[1024];
    if (file.open(QIODevice::WriteOnly)) {
        sprintf(buf,"Server=%s\n",serverName().simplified().toAscii().data());
        file.write(buf,strlen(buf));
        sprintf(buf,"Port=%ld\n",serverPort());
        file.write(buf,strlen(buf));
        sprintf(buf,"Timer=%ld\n",timerSequence());
        file.write(buf,strlen(buf));
        sprintf(buf,"Name=%s\n",name().simplified().toAscii().data());
        file.write(buf,strlen(buf));
        sprintf(buf,"VName=%s\n",vName().simplified().toAscii().data());
        file.write(buf,strlen(buf));
        sprintf(buf,"Year=%s\n",year().simplified().toAscii().data());
        file.write(buf,strlen(buf));
        sprintf(buf,"Month=%s\n",month().simplified().toAscii().data());
        file.write(buf,strlen(buf));
        sprintf(buf,"Day=%s\n",day().simplified().toAscii().data());
        file.write(buf,strlen(buf));
        sprintf(buf,"Font=%s\n",fontName().simplified().toAscii().data());
        file.write(buf,strlen(buf));
        sprintf(buf,"WarnIfLogOutTimer=%ld\n",wanIfLogOutTimer());
        file.write(buf,strlen(buf));
        sprintf(buf,"AutoLogout=%d\n",autoLogout());
        file.write(buf,strlen(buf));
        sprintf(buf,"Inaktive=%d\n",inaktive());
        file.write(buf,strlen(buf));
        sprintf(buf,"AutoLogin=%d\n",autoLogin());
        file.write(buf,strlen(buf));
        sprintf(buf,"WarnIfLogOut=%d\n",wanIfLogOut());
        file.write(buf,strlen(buf));
        sprintf(buf,"NotifierTimer=%ld\n",notifierTimer());
        file.write(buf,strlen(buf));
        sprintf(buf,"NotifierX=%ld\n",notifierX());
        file.write(buf,strlen(buf));
        sprintf(buf,"NotifierY=%ld\n",notifierY());
        file.write(buf,strlen(buf));
        sprintf(buf,"ShowNotifierDlg=%d\n",showNotifierDlg());
        file.write(buf,strlen(buf));
        sprintf(buf,"ShowPersonPicture=%d\n",showPersonPicture());
        file.write(buf,strlen(buf));
        sprintf(buf,"Server2=%s\n",serverName_2().simplified().toAscii().data());
        file.write(buf,strlen(buf));
        sprintf(buf,"ServerPath2=%s\n",serverPath_2().simplified().toAscii().data());
        file.write(buf,strlen(buf));
        sprintf(buf,"Port2=%ld\n",serverPort_2());
        file.write(buf,strlen(buf));

        QByteArray tmp(listState().toBase64());
        tmp=tmp.replace("=","@");
        sprintf(buf,"ListState=%s\n",tmp.data());
        file.write(buf,strlen(buf));

        QByteArray tmp11(QByteArray(ciscoPass().toAscii()).toBase64());
        tmp11=tmp11.replace("=","@");
        sprintf(buf,"CiscoPass=%s\n",tmp11.data());
        file.write(buf,strlen(buf));

        QByteArray tmp2(dockState().toBase64());
        tmp2=tmp2.replace("=","@");
        sprintf(buf,"DockState1.5=%s\n",tmp2.data());
        file.write(buf,strlen(buf));

        QByteArray tmp3(mainWindowState().toBase64());
        tmp3=tmp3.replace("=","@");
        sprintf(buf,"MainWindowState=%s\n",tmp3.data());
        file.write(buf,strlen(buf));


        file.close();
    }
}


QString Options::getUsername() {
    char *user_name;
    user_name=getenv("USER");
    if (!user_name || (user_name && user_name[0]==0)) {
        user_name=getenv("USERNAME");
    }
    if (user_name) {
        return QString(user_name);
    }
    return QString("");
}

QString Options::getFilenameFromUser() {
    return getCompanylogUserHomePath()+"/companylog.opt";
}

QString Options::getLogOptionFilenameFromUser() {
    return getCompanylogUserHomePath()+"/logoptions.xml";
}

QString Options::getCompanylogUserHomePath() {
    if (companylogUserHomePath.size()==0) {
        QString homePath=QDir::homePath();
        QDir homeDir(homePath+"/.companylog");
        if (!homeDir.exists()) {
            //homeDir.mkpath(homePath+"/.companylog");
            //homeDir.mkpath(homePath+"/.companylog/icons");
            homeDir.mkpath(homePath+"/.companylog/icons/logs");
            /* migrate the old Options-File */
            if (QFile::exists(homePath+"/.companylog.opt")) {
                QFile::copy(homePath+"/.companylog.opt",homePath+"/.companylog/companylog.opt");
                QFile::remove(homePath+"/.companylog.opt");
            }
        }
        companylogUserHomePath=homePath+"/.companylog";
    }
    return companylogUserHomePath;
}
QString Options::getLogIconPath() {
    return QApplication::applicationDirPath()+"/icons/logs";
}
QString Options::getLogIconPrivatePath() {
    return getCompanylogUserHomePath()+"/icons/logs";
}
QString Options::getIconPath() {
    return QApplication::applicationDirPath()+"/icons";
}
QString Options::getEmoticonPath() {
    return QApplication::applicationDirPath()+"/emoticons";
}



QString Options::serverName() {return m_serverName;}
QString Options::ciscoPass() {return m_ciscoPass;}
long	Options::serverPort() {return m_serverPort;}
QString Options::serverName_2() {return m_serverName_2;}
QString Options::serverPath_2() {return m_serverPath_2;}
long	Options::serverPort_2() {return m_serverPort_2;}
long	Options::timerSequence() {return m_timerSequence;}
QString Options::fontName(){return m_font;}
QByteArray Options::listState() {return byteArrayListState;}
QByteArray Options::dockState() {return byteArrayDockState;}
QByteArray Options::mainWindowState() {return byteArrayMainWindowState;}

long	Options::notifierTimer() {return m_notifierTimer;}
long	Options::notifierX() {return m_notifierX;}
long	Options::notifierY() {return m_notifierY;}
bool    Options::showNotifierDlg() {return m_notifierShow;}
bool 	Options::showPersonPicture() {return m_showPersonPicture;}


void Options::setServerName(QString server_name) {m_serverName=server_name;}
void Options::setCiscoPass(QString cisco_pass) {m_ciscoPass=cisco_pass;}
void Options::setServerPort(long server_port) {m_serverPort=server_port;}
void Options::setServerName_2(QString server_name) {m_serverName_2=server_name;}
void Options::setServerPath_2(QString server_name) {m_serverPath_2=server_name;}
void Options::setServerPort_2(long server_port) {m_serverPort_2=server_port;}
void Options::setTimerSequence(long timer_seq) {m_timerSequence=timer_seq;}
void Options::setFontName(QString s) {m_font=s;}

void Options::setListState(QByteArray s) {byteArrayListState=s;}
void Options::setDockState(QByteArray s) {byteArrayDockState=s;}
void Options::setMainWindowState(QByteArray s) {byteArrayMainWindowState=s;}

void Options::setNotifierTimer(long notifier_timer) {m_notifierTimer=notifier_timer;}
void Options::setNotifierX(long notifierX) {m_notifierX=notifierX;}
void Options::setNotifierY(long notifierY) {m_notifierY=notifierY;}
void Options::setNotifierDlg(bool showNotifier) { m_notifierShow=showNotifier; }
void Options::setShowPersonPicture(bool showPersonPicture) {m_showPersonPicture=showPersonPicture;}


QString Options::vName() {return m_vName;}
QString Options::name(){return m_name;}
QString Options::year(){return m_year;}
QString Options::month(){return m_month;}
QString Options::day(){return m_day;}
void Options::setVName(QString s) {m_vName=s;}
void Options::setName(QString s) {m_name=s;}
void Options::setYear(QString s) {m_year=s;}
void Options::setMonth(QString s) {m_month=s;}
void Options::setDay(QString s) {m_day=s;}


bool	Options::autoLogout() {return m_autoLogout;}
bool	Options::inaktive() {return m_inaktive;}
bool	Options::autoLogin() {return m_autoLogin;}
bool	Options::wanIfLogOut() {return m_wanIfLogOut;}
long	Options::wanIfLogOutTimer() {return m_wanIfLogOutTimer;}

void	Options::setAutoLogout(bool b) {m_autoLogout=b;}
void	Options::setInaktive(bool b) {m_inaktive=b;}
void	Options::setAutoLogin(bool b) {m_autoLogin=b;}
void	Options::setWanIfLogOut(bool b) {m_wanIfLogOut=b;}
void	Options::setWanIfLogOutTimer(long b) {
	if (b<1) {
		b=1;
	}
	m_wanIfLogOutTimer=b;
}

DlgOptions::DlgOptions(QWidget *parent) : QDialog(parent) {
	setupUi(this);
	okButton->setAutoDefault(false);
	connect(okButton,SIGNAL(clicked()),this,SLOT(on_DlgOptions_okButton()));
	connect(pushButtonFont,SIGNAL(clicked()),this,SLOT(on_DlgOptions_pushButtonFont()));
    lineCiscoPass->setEchoMode(QLineEdit::Password);
}

DlgOptions::~DlgOptions() {
	//okButton->setAutoDefault=false;
	fprintf(stderr,"DlgOptions destructor called\n");
}

void DlgOptions::on_DlgOptions_okButton() {
	emit(dlgOptionsOk());
}

void DlgOptions::on_DlgOptions_pushButtonFont() {
	bool ok;
    QFont font = QFontDialog::getFont(
                    &ok, QFont("Helvetica [Cronyx]", 10), this);
    if (ok) {
        setFontName(font.toString());
        // the user clicked OK and font is set to the font the user selected
    } else {
        // the user canceled the dialog; font is set to the initial
        // value, in this case Helvetica [Cronyx], 10
    }
}

QString DlgOptions::server() {
	return lineServer->text();
}
QString DlgOptions::ciscoPass() {
    return lineCiscoPass->text();
}

QString DlgOptions::server_2() {
	return lineServer_2->text();
}
QString DlgOptions::serverPath_2() {
	return lineServerPath_2->text();
}

QString DlgOptions::port() {
	return linePort->text();
}
QString DlgOptions::port_2() {
	return linePort_2->text();
}
QString DlgOptions::timer() {
	return lineTimer->text();
}
QString DlgOptions::fontName() {
	return lineFont->text();
}
void DlgOptions::setServer(QString s) {
	lineServer->setText(s);
}
void DlgOptions::setCiscoPass(QString s) {
    lineCiscoPass->setText(s);
}
QString DlgOptions::notifierTimer() {
	return lineNotifierTimer->text();
}
QString DlgOptions::notifierX() {
	return lineNotifierX->text();
}
QString DlgOptions::notifierY() {
	return lineNotifierY->text();
}
bool DlgOptions::showNotifierDlg() {
        return checkShowNotifier->checkState()==Qt::Checked?true:false;
}
bool DlgOptions::showPersonPicture() {
	return checkShowPersonPicture->checkState()==Qt::Checked?true:false;
}

void DlgOptions::setNotifierTimer(QString s) {
	lineNotifierTimer->setText(s);
}
void DlgOptions::setNotifierX(QString s) {
	lineNotifierX->setText(s);
}
void DlgOptions::setNotifierY(QString s) {
	lineNotifierY->setText(s);
}
void DlgOptions::setNotifierDlg(bool b) {
        checkShowNotifier->setCheckState(b?Qt::Checked:Qt::Unchecked);
}

void DlgOptions::setShowPersonPicture(bool b) {
	checkShowPersonPicture->setCheckState(b?Qt::Checked:Qt::Unchecked);
}

void DlgOptions::setPort(QString s) {
	linePort->setText(s);
}
void DlgOptions::setServer_2(QString s) {
	lineServer_2->setText(s);
}
void DlgOptions::setServerPath_2(QString s) {
	lineServerPath_2->setText(s);
}
void DlgOptions::setPort_2(QString s) {
	linePort_2->setText(s);
}

void DlgOptions::setTimer(QString s) {
	lineTimer->setText(s);
}
void DlgOptions::setFontName(QString s) {
	lineFont->setText(s);
}

void DlgOptions::setAutoLogout(bool b) {
	checkAutoLogout->setCheckState(b?Qt::Checked:Qt::Unchecked);
}
void DlgOptions::setInaktive(bool b) {
	checkInaktive->setCheckState(b?Qt::Checked:Qt::Unchecked);
}
void DlgOptions::setAutoLogin(bool b) {
	checkAutoLogin->setCheckState(b?Qt::Checked:Qt::Unchecked);
}
void DlgOptions::setWanIfLogOut(bool b) {
	checkWanIfLogOut->setCheckState(b?Qt::Checked:Qt::Unchecked);
}
void DlgOptions::setWanIfLogOutTimer(QString b) {
	lineWarnIfLogoutTime->setText(b);
}
bool DlgOptions::autoLogout() {
	return checkAutoLogout->checkState()==Qt::Checked?true:false;
}
bool DlgOptions::inaktive() {
	return checkInaktive->checkState()==Qt::Checked?true:false;
}
bool DlgOptions::autoLogin() {
	return checkAutoLogin->checkState()==Qt::Checked?true:false;
}
bool DlgOptions::wanIfLogOut() {
	return checkWanIfLogOut->checkState()==Qt::Checked?true:false;
}
QString DlgOptions::wanIfLogOutTimer() {
	return lineWarnIfLogoutTime->text();
}


Options *gg_options=0;

Options* getGlobalOptionsObj() {
	if (!gg_options) {
		gg_options=new Options();
		gg_options->readOptionsFromFile();
	}
	return gg_options;
}
void destroyGlobalOptionsObj() {
	if(gg_options)  {
		delete gg_options;	
	}
}
#include "EcoMiniReg.h"

EcoMiniReg *gg_ecoMiniReg=0;

EcoMiniReg* getGlobalEcoMiniReg() {
    if (!gg_ecoMiniReg) {
        gg_ecoMiniReg=new EcoMiniReg();
    }
    return gg_ecoMiniReg;
}
void destroyGlobalEcoMiniReg() {
    if(gg_ecoMiniReg)  {
        delete gg_ecoMiniReg;
    }
}
