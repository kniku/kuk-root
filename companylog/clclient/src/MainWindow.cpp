/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h18
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : MainWindow.cpp
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "MainWindow.h"
#include <QDesktopWidget>


//#define SERVERLESS_MODE 1
#define POKERLESS_MODE 1

/*
  QDesktopWidget* d = new QDesktopWidget();
> QWidget* w = d->screen();
> QPainter p(w);
> // painting ...
> p.end();
*/

#ifdef Q_WS_WIN
#include<windows.h>
#else
//#include <QX11Info>
//#include <X11/Xlib.h>
#endif

MainWindow::MainWindow(QApplication *qapp,QWidget *parent) : QMainWindow(parent) {
   chatManager=0;
   incomingPort=0;
   lastSelectedTel=0;
#ifdef Q_WS_WIN
    popupWindow=new PopupWindow(0,/*Qt::Tool*//*Qt::SplashScreen*/ Qt::Tool|Qt::FramelessWindowHint);
#else
    //Fukus kann das Fenster mit Qt::Tooltip nie bekommen, aber zumindest erscheint es dann am Vordergrund!
    //popupWindow=new PopupWindow(0,/*Qt::Tool*//*Qt::SplashScreen*/ Qt::ToolTip|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    popupWindow=new PopupWindow(0,/*Qt::Tool*//*Qt::SplashScreen*/ Qt::ToolTip|Qt::FramelessWindowHint);
    //	popupWindow->setFocusPolicy(Qt::StrongFocus);
    //	popupWindow->setEnabled(true);
    //popupWindow->setWindowModality(Qt::NonModal);
    //popupWindow=new PopupWindow(0,/*Qt::Tool*//*Qt::SplashScreen*/ Qt::Tool|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
#endif
    connect(popupWindow,SIGNAL(sendMessage (QString, QString)),this,SLOT(on_ChatWidget_sendMessage(QString, QString)));

    qApplication=qapp;
    setDockOptions(QMainWindow::AllowTabbedDocks);
    lastWarnIfLogOutTimer=-1;
    lastDelayRefreshTimer=-1;
    lastChatServerRegistrationTimer=-1;
    isInStartup=true;
    isInShutdown=false;
    setupGlobalFont();
    //prepareWorkspace();
    prepareDockWidget();
    prepareActions();
    prepareMenu();
    prepareToolbar();
    prepareSystemTrayIcon();

    updateUserdefinedActions();
    updateUserdefinedToolbar();
    updateUserdefinedMenu();
    updateUserdefinedTrayMenu();
    updateMainLogToolbar();

    prepareStatusBar();
    prepareEcoMiniReg();
#ifndef SERVERLESS_MODE
    registerChatManager();// nach ecominireg!
    startAutoRefreshCurrentUserState();
#endif
    prepareTabWidget();

#ifndef SERVERLESS_MODE
    startChatServerRegistrationTimer(); // am server alle 15 Min neu registrieren
#endif
    restorWorkspaceFromOptions();
    setupEndSessionManagement(qapp);
}

void MainWindow::registerChatManager() {
    if (!chatManager) {
        chatManager=new ChatManager();
        incomingPort=chatManager->getIncomingPort();
        connect(chatManager,SIGNAL(incomingChatMessage(QString)),this,SLOT(on_MainWindow_IncomingChatMessage(QString)));
    }
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();
    if(ecoMiniReg && incomingPort>0) {
        ecoMiniReg->RegisterChatPort(incomingPort);
    }
}
void MainWindow::on_MainWindow_IncomingChatMessage(QString message) {
  //QMessageBox::information(this, "Message from Server",message);
  Options *options=getGlobalOptionsObj();

#if 0
  message="<ecm t=\"THOLDM\"> <msg t=\"game_list\"> <game id=\"$GAMEID1\" game_name=\"$GAMENAME1\"> <player l=\"$USERa\" n=\"$USER_LONGNAME\"/> <player l=\"$USERb\" n=\"$USER_LONGNAME\"/> <player l=\"$USERc\" n=\"$USER_LONGNAME\"/> <player l=\"$USERd\" n=\"$USER_LONGNAME\"/> </game>"
          "<game id=\"$GAMEID2\" game_name=\"$GAMENAME2\">"
                        "<player l=\"$USER1\" n=\"$USER_LONGNAME1\"/>"
                        "<player l=\"$USER2\" n=\"$USER_LONGNAME2\"/>"
                        "<player l=\"$USER3\" n=\"$USER_LONGNAME3\"/>"
                        "<player l=\"$USER4\" n=\"$USER_LONGNAME4\"/>"
                "</game>"
        "</msg>"
"</ecm>";
#endif
  if (chatWidget) {
	  QDomDocument doc( "AdBookML" );	  
	  doc.setContent(message);
	  QDomElement root = doc.documentElement();

      QString t=root.attribute("t", "" );
      bool handled=false;
      if (t=="THOLDM") {
#ifndef POKERLESS_MODE
        /* eine Pokerliste? */
        handled=pokerGames->updateGameList(message);
        if (!handled) {
            /* vermutlich PokerStatus */
            handled=updateGamestatus(message);
        }
#endif
      } else {
          /* default = chat message */
          QString fromUser = root.attribute( "l", "" );
          QString fromMessage=root.attribute( "m", "" );
          QString fromUserLong=root.attribute( "ln", "" );
          QString fromPopup = root.attribute( "popup", "1" );
          if (fromUser.size()>0 && fromMessage.size()>0) {
              if (fromUser!="Server") {
                 chatWidget->addPersonToEditfield(fromUser);
              } else {
                    fromUserLong="[Server]";
                    fromUser="[Server]";
              }
              if (fromUserLong.size()<=0) {
                    fromUserLong=fromUser;
              }
              chatWidget->setMessage(true,fromUser, fromUserLong, fromMessage, (fromPopup == "0" && fromUser == "[Server]") ? false : true);
    //
              if (popupWindow &&            /*QApplication::activeWindow()==0 &&*/
                  fromPopup == "1" &&
                  options->showNotifierDlg()) {
                    QWidget *actWindow=QApplication::activeWindow();
                    QWidget *focusW=QApplication::focusWidget();

                    popupWindow->setMessage(fromUser, fromUserLong, fromMessage);
                    popupWindow->setXYFromOption();
                                popupWindow->show();
                    WId winId=popupWindow->winId();
                    //BringWindowToTop(winId);
#ifdef Q_WS_WIN
                SetWindowPos(
                    winId,
                    HWND_TOPMOST,
                    0,
                    0,
                    100,
                    50,
                    SWP_NOSIZE|SWP_NOMOVE);
#else

                //QX11Info x11info=popupWindow->x11Info();
                //XRaiseWindow(x11info.display(), popupWindow->winId());
                //XMapWindow(x11info.display(), popupWindow->winId());
                popupWindow->raise();
#endif
                this->setFocus(Qt::OtherFocusReason);
                // setzt fokus wieder auf das MainWindow (meistens halt :)
                if (actWindow) {
                    QApplication::setActiveWindow(actWindow);
                }
                // setzt den Fokus wieder auf das lezte Widget des MainWindow
                if (focusW) {
                    focusW->setFocus(Qt::OtherFocusReason);
                }
            }
#if 0	
            // Standard Balloon Tip Message
            if ((fromPopup == "1") && systemTrayIcon) {
                Options *options=getGlobalOptionsObj();
                long notifierTimer=options->notifierTimer();
                if (notifierTimer<=0) {
                    notifierTimer=500;
                }
                systemTrayIcon->showMessage("companylog Mitteilung von "+fromUserLong,fromMessage,QSystemTrayIcon::Information,1000*notifierTimer);
            }
#endif
		  
#if 0
              popupWindow->activateWindow();
              popupWindow->raise();
              Options *options;

            options=getGlobalOptionsObj();
            QWidget *screen=QApplication::desktop()->screen( 0 );
            //QWidget *screen=QDesktopWidget().screen( 0 );
            screen->setBackgroundRole(QPalette::Dark);
            QPainter paint;
            paint.begin(screen);
            paint.drawPixmap(QPoint(10,10),*options->loadedBatPixmap());
       //      paint.drawImage(0,0,*options->loadedUnknownPersonPixmap());
            paint.end();
#endif
          }
      }
  }
}


MainWindow::~MainWindow() {
    destroyGlobalOptionsObj();
    destroyGlobalEcoMiniReg();
    deleteUserdefinedActionLists();
    if (chatManager) delete chatManager;
    if (popupWindow) delete popupWindow;
}

void MainWindow::restorWorkspaceFromOptions() {
	Options *options;
	QByteArray array;

	options=getGlobalOptionsObj();
	if (options) {
		array=options->dockState();
		if (array.size()>0) {
			restoreState(array);
		}
        array=options->mainWindowState();
        if (array.size()>0) {
            restoreGeometry(array);
        } else {
            setGeometry(10,20,1014,790);
        }
	}
}

void MainWindow::prepareWorkspace() {
	workSpace = new QWorkspace(this);
	if (workSpace) {
		workSpace->setContentsMargins(0,0,0,0);

		//workSpace->setBackground(QBrush(Qt::yellow));
		
		//workSpace->setBackgroundRole(QPalette::Base);
		//workSpace->setForegroundRole(QPalette::Base);
		//workSpace->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
		//workSpace->setGeometry(0,0,100,100);
		//workSpace->updateGeometry();
		setCentralWidget(workSpace);
	}
}

void MainWindow::prepareEcoMiniReg() {
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();
	if(ecoMiniReg) {
		connect(ecoMiniReg,SIGNAL(currentUserStateChanged(MemberLog*)),this,SLOT(on_MainWindow_CurrentUserState_Changed(MemberLog*)));
	}
}


void MainWindow::startAutoRefreshCurrentUserState() {
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();
    if(ecoMiniReg) {
        //setCurrentUserState();

		ecoMiniReg->startStateRefreshTimer();
	}
}

void MainWindow::setCurrentUserState() {
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();
    if(ecoMiniReg) {
        ecoMiniReg->requestCurrentUserStateFromServer();
	}
}

void MainWindow::prepareDockWidget() {
	dockWidget1 = new DockWidget(this);
	dockWidget1->setObjectName("dockWidget1");
	dockWidget1->setWindowTitle("Information");
	addDockWidget(Qt::RightDockWidgetArea, dockWidget1/*,Qt::Vertical*/);

	waitForWidget = new WaitForWidget(this);
	waitForWidget->setObjectName("waitForWidget");
	waitForWidget->setWindowTitle("Person erwarten");
	addDockWidget(Qt::RightDockWidgetArea, waitForWidget/*,Qt::Vertical*/);

	connect(waitForWidget,SIGNAL(onPersonAdded (QString)),this,SLOT(on_WaitForWidget_onPersonAdded(QString)));
#ifndef POKERLESS_MODE
    pokerGames = new PokerGames(this);
    pokerGames->setObjectName("pokerGames");
    pokerGames->setWindowTitle("Poker");
    addDockWidget(Qt::RightDockWidgetArea, pokerGames/*,Qt::Vertical*/);
#endif
	chatWidget = new ChatWidget(this);
	chatWidget->setObjectName("chatWidget");
	chatWidget->setWindowTitle("Nachrichten");
	addDockWidget(Qt::RightDockWidgetArea, chatWidget/*,Qt::Vertical*/);
    //chatWidget->hide();
	
	connect(chatWidget,SIGNAL(sendMessage (QString, QString)),this,SLOT(on_ChatWidget_sendMessage(QString, QString)));

	/*dockWidget2 = new DockWidget(this);
	dockWidget2->setWindowTitle("Zuletzt");
	addDockWidget(Qt::RightDockWidgetArea, dockWidget2);
	*/
	/*listWidget1 = new QWidget(dockWidget1);
	listWidget2 = new QListWidget(dockWidget2);
	
	dockWidget1->setWidget(listWidget1);
	dockWidget2->setWidget(listWidget2);*/

}



void MainWindow::prepareActions() {
    QString appPath=QApplication::applicationDirPath();
    Options *options;
    options=getGlobalOptionsObj();

    newAction = new QAction(*options->pixmapCacheIcons()->getIcon("filenew.png"),tr("&Neu"), this);
    if (newAction) {
        newAction->setShortcut(tr("Ctrl+N"));
        newAction->setStatusTip(tr("Neue Auswertung"));
        connect(newAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_New()));
    }
    infoAction = new QAction(*options->pixmapCacheIcons()->getIcon("messagebox_info.png"),tr("Info ..."), this);
    if (infoAction) {
        infoAction->setShortcut(tr("Ctrl+U"));
        infoAction->setStatusTip(tr("Informationen anzeigen"));
        connect(infoAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_Info()));
    }
    auswertungAction = new QAction(*options->pixmapCacheIcons()->getIcon("auswertung.png"),tr("&Auswertung"), this);
    if (auswertungAction) {
        auswertungAction->setShortcut(tr("Ctrl+A"));
        auswertungAction->setStatusTip(tr("Auswertung"));
        connect(auswertungAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_NewAuswertung()));
    }
    exitAction = new QAction(*options->pixmapCacheIcons()->getIcon("exit.png"),tr("&Beenden"), this);
    if (exitAction) {
        exitAction->setShortcut(tr("Ctrl+Q"));
        exitAction->setStatusTip(tr("Programm beenden"));
        connect(exitAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_Exit()));
    }
    optionsAction = new QAction(*options->pixmapCacheIcons()->getIcon("configure.png"),tr("O&ptionen"), this);
    if (optionsAction) {
        optionsAction->setShortcut(tr("Ctrl+P"));
        optionsAction->setStatusTip(tr("Optionen"));
        connect(optionsAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_Options()));
    }
    logConfigAction = new QAction(*options->pixmapCacheIcons()->getIcon("logbuttonconfig.png"),tr("&Logbuttons konfigurieren"), this);
    if (logConfigAction) {
        logConfigAction->setShortcut(tr("Ctrl+L"));
        logConfigAction->setStatusTip(tr("Logbuttons konfigurieren"));
        connect(logConfigAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_LogConfigDialog()));
    }
    logIdentifierAction = new QAction(*options->pixmapCacheIcons()->getIcon("logtextconfig.png"),tr("Logtexterkennung konfigurieren"), this);
    if (logIdentifierAction) {
        logIdentifierAction->setShortcut(tr("Ctrl+E"));
        logIdentifierAction->setStatusTip(tr("Logtexterkennung konfigurieren"));
        connect(logIdentifierAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_LogIdentifierDialog()));
    }

    refreshAction = new QAction(*options->pixmapCacheIcons()->getIcon("reload.png"),tr("Neu &laden"), this);
    if (refreshAction) {
        refreshAction->setShortcut(tr("F5"));
        refreshAction->setStatusTip(tr("Neu laden"));
        connect(refreshAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_Refresh()));
    }


#ifndef POKERLESS_MODE
    testPokerAction = new QAction(*options->pixmapCacheIcons()->getIcon("poker.png"),tr("Poker Test"), this);
    if (testPokerAction) {
        connect(testPokerAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_testPoker()));
    }
#endif
    testCiscoAction = new QAction(*options->pixmapCacheIcons()->getIcon("gnomemeeting.png"),tr("Cisco call"), this);
    if (testCiscoAction) {
        connect(testCiscoAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_testCisco()));
    }
    sqlWorksheetAction = new QAction(*options->pixmapCacheIcons()->getIcon("sqlworksheet.png"),tr("Sql-Worksheet"), this);
    if (sqlWorksheetAction) {
        connect(sqlWorksheetAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_sqlWorksheet()));
    }
    wiederherstellenAction = new QAction(*options->pixmapCacheIcons()->getIcon("companylog.png"),tr("Wiederherstellen"), this);
    if (wiederherstellenAction) {
        //wiederherstellenAction->setShortcut(tr("F5"));
        wiederherstellenAction->setStatusTip(tr("Wiederherstellen"));
        connect(wiederherstellenAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_Wiederherstellen()));
    }

    setWindowIcon(*options->pixmapCacheIcons()->getIcon("companylog.png"));
}
void MainWindow::prepareMenu() {
    fileMenu=menuBar()->addMenu(tr("&Datei"));
    if (fileMenu) {
        fileMenu->addAction(newAction);
        fileMenu->addAction(auswertungAction);
        fileMenu->addAction(refreshAction);
        fileMenu->addAction(exitAction);
    }
    logMenu=menuBar()->addMenu(tr("&Log"));

    einstellungenMenu=menuBar()->addMenu(tr("&Einstellungen"));
    if (einstellungenMenu) {
        einstellungenMenu->addAction(optionsAction);
        einstellungenMenu->addAction(logConfigAction);
        einstellungenMenu->addAction(logIdentifierAction);
        einstellungenMenu->addAction(sqlWorksheetAction);
    }

    helpMenu=menuBar()->addMenu(tr("&Hilfe"));
    if (helpMenu) {
        helpMenu->addAction(infoAction);
    }
}

void MainWindow::prepareSystemTrayIcon() {
    Options *options;
    options=getGlobalOptionsObj();
    trayMenu=new QMenu(this);
    systemTrayIcon=new QSystemTrayIcon(*options->pixmapCacheIcons()->getIcon("companylog.png"));
    if (systemTrayIcon && trayMenu) {
        connect(systemTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_MainWindow_SysTrayIcon_activated(QSystemTrayIcon::ActivationReason)));
        if (wiederherstellenAction) {
                trayMenu->addAction(wiederherstellenAction);
        }
        if (exitAction) {
                trayMenu->addAction(exitAction);
        }
        systemTrayIcon->setContextMenu(trayMenu);
        systemTrayIcon->setToolTip("companylog - a Companyserv (c) client");
        systemTrayIcon->show();
    }
}


void MainWindow::deleteUserdefinedActionLists() {
    int count=userdefinedActionList.count();
    for(int i=0;i<count;i++) {
        QAction* userdefinedAction=userdefinedActionList.value(i);
        delete userdefinedAction;
    }
    count=userdefinedActionListMixed.count();
    for(int i=0;i<count;i++) {
        QAction* userdefinedAction=userdefinedActionListMixed.value(i);
        delete userdefinedAction;
    }
    userdefinedActionList.clear();
    userdefinedActionListMixed.clear();
}

void MainWindow::updateUserdefinedActions() {
    Options *options=getGlobalOptionsObj();
    deleteUserdefinedActionLists();
    int count=options->logManager()->logActionCount();
    for (int i=0;i<count;i++) {
        LogAction * logAction=options->logManager()->logAction(i);
        if (logAction) {
            QIcon *icon=options->logManager()->getIcon(logAction->iconName());
            if (icon) {
                QAction *newAction=new QAction(*icon,logAction->text(),0);
                if (newAction) {
                    newAction->setStatusTip(logAction->text());
                    connect(newAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_UserdefinedActionClicked()));
                    userdefinedActionList.append(newAction);
                }
                QPixmap *pixmapMixed=options->logManager()->getMixedPixmap(options->pixmapCacheIcons()->getPixmap("companylog.png"),"companylog.png",logAction->iconName());
                if (pixmapMixed) {
                    icon=options->logManager()->getIcon("companylog.png"+logAction->iconName());
                    if (icon) {
                        QAction *newAction=new QAction(*icon,logAction->text(),0);
                        if (newAction) {
                            newAction->setStatusTip(logAction->text());
                            connect(newAction,SIGNAL(triggered()),this,SLOT(on_MainWindow_UserdefinedActionClicked()));
                            userdefinedActionListMixed.append(newAction);
                        }
                    }
                }
            }
        }
    }
}



void MainWindow::updateUserdefinedToolbar() {
    if (toolBar3) {
        toolBar3->clear();
        int count=userdefinedActionList.count();
        for(int i=0;i<count;i++) {
            QAction* userdefinedAction=userdefinedActionList.value(i);
            if (userdefinedAction) {
                if (userdefinedAction->statusTip()==LOGIN ||
                    userdefinedAction->statusTip()==LOGOUT ||
                    userdefinedAction->statusTip()==LOGIN_SONST ||
                    userdefinedAction->statusTip()==LOGOUT_SONST) {
                    /* skip */
                } else {
                    toolBar3->addAction(userdefinedAction);
                }
            }
        }
        if (count>0) {
            toolBar3->addSeparator();
        }
        if (logConfigAction) {
            toolBar3->addAction(logConfigAction);
        }
    }
}

void MainWindow::updateMainLogToolbar() {
    if (toolBar2) {
        toolBar2->clear();
        int count=userdefinedActionList.count();
        for(int i=0;i<count;i++) {
            QAction* userdefinedAction=userdefinedActionList.value(i);
            if (userdefinedAction) {
                if (userdefinedAction->statusTip()==LOGIN ||
                    userdefinedAction->statusTip()==LOGOUT) {
                    toolBar2->addAction(userdefinedAction);
                }
            }
        }
        sonstigerEintragLabel=new QLabel("sonstiger Eintrag",toolBar2);
        sonstigerEintragEdit=new QLineEdit(toolBar2);
        if(sonstigerEintragEdit) {
            sonstigerEintragEdit->setMaxLength(30);
            sonstigerEintragEdit->setMaximumWidth(250);
            sonstigerEintragLabel->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
            //sonstigerEintragEdit->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
        }

        if(sonstigerEintragLabel) toolBar2->addWidget(sonstigerEintragLabel);
        if(sonstigerEintragEdit) toolBar2->addWidget(sonstigerEintragEdit);

        for(int i=0;i<count;i++) {
            QAction* userdefinedAction=userdefinedActionList.value(i);
            if (userdefinedAction) {
                if (userdefinedAction->statusTip()==LOGIN_SONST ||
                    userdefinedAction->statusTip()==LOGOUT_SONST) {
                    toolBar2->addAction(userdefinedAction);
                }
            }
        }
    }
}

void MainWindow::updateUserdefinedMenu() {
    if (logMenu) {
        logMenu->clear();
        int count=userdefinedActionList.count();
        for(int i=0;i<count;i++) {
            QAction* userdefinedAction=userdefinedActionList.value(i);
             if (userdefinedAction && (userdefinedAction->statusTip()==LOGIN ||userdefinedAction->statusTip()==LOGOUT)) {
                logMenu->addAction(userdefinedAction);
            }
        }

        logMenu->addSeparator();
        for(int i=0;i<count;i++) {
            QAction* userdefinedAction=userdefinedActionList.value(i);
             if (userdefinedAction) {
                if (userdefinedAction->statusTip()==LOGIN ||
                    userdefinedAction->statusTip()==LOGOUT ||
                    userdefinedAction->statusTip()==LOGIN_SONST ||
                    userdefinedAction->statusTip()==LOGOUT_SONST) {
                    /* skip */
                } else {
                    logMenu->addAction(userdefinedAction);
                }
            }
        }
    }
}

void MainWindow::updateUserdefinedTrayMenu() {
    if (trayMenu) {
        trayMenu->clear();


        int count=userdefinedActionList.count();

        for(int i=0;i<count;i++) {
            QAction* userdefinedAction=userdefinedActionList.value(i);
            if (userdefinedAction) {
                if (userdefinedAction->statusTip()==LOGIN ||userdefinedAction->statusTip()==LOGOUT) {
                    trayMenu->addAction(userdefinedAction);
                }
            }
        }
        trayMenu->addSeparator();

        for(int i=0;i<count;i++) {
            QAction* userdefinedAction=userdefinedActionList.value(i);
            if (userdefinedAction) {
                if (userdefinedAction->statusTip()==LOGIN ||
                    userdefinedAction->statusTip()==LOGOUT ||
                    userdefinedAction->statusTip()==LOGIN_SONST ||
                    userdefinedAction->statusTip()==LOGOUT_SONST) {
                    /* skip */
                } else {
                    trayMenu->addAction(userdefinedAction);
                }
            }
        }
        trayMenu->addSeparator();
        if (wiederherstellenAction) {
                trayMenu->addAction(wiederherstellenAction);
        }
        if (exitAction) {
                trayMenu->addAction(exitAction);
        }

    }
}


void MainWindow::on_MainWindow_UserdefinedActionClicked() {
    Options *options=getGlobalOptionsObj();
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();
    QObject *xsender=sender();
    QAction *xaction=0;
    if (xsender && options && ecoMiniReg) {
        xaction=qobject_cast<QAction *>(xsender);
        if (xaction) {
            QString xtext=xaction->text();
            LogAction* logAction=options->logManager()->logActionFromText(xtext);
            if (logAction) {
                logUserdefined(logAction);
            }
        }
    }
}


void MainWindow::prepareToolbar() {
    toolBar = new QToolBar(tr("Haupt-Werkzeugleiste"),this);
    toolBar2 = new QToolBar(tr("Logging-Werkzeugleiste"),this);
    toolBar3 = new QToolBar(tr("Benutzerdefinierte Werkzeugleiste"),this);

    if (toolBar) {
        toolBar->setObjectName("toolBar");

        //toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        addToolBar(toolBar);
        if (newAction) {
                toolBar->addAction(newAction);
        }
        if (auswertungAction) {
                toolBar->addAction(auswertungAction);
        }
        if (refreshAction) {
                toolBar->addSeparator();
                toolBar->addAction(refreshAction);
        }
#ifndef POKERLESS_MODE
        if (testPokerAction) {
                toolBar->addAction(testPokerAction);
        }
#endif
        if (testCiscoAction) {
                toolBar->addAction(testCiscoAction);
        }

    }
    if (toolBar2) {
        toolBar2->setObjectName("toolBar2");
        //toolBar2->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        addToolBar(toolBar2);
    }
    if (toolBar3) {
        toolBar3->setObjectName("toolBar3");
        //toolBar2->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        addToolBar(toolBar3);
    }
}

void MainWindow::prepareStatusBar() {
	statusBar()->showMessage(tr("Bereit"));
	//currentStatePresenter=new UserStatusWidget(statusBar());
	//if (currentStatePresenter) {
	//statusBar()->addPermanentWidget(currentStatePresenter);
		
		/* For Test ONLY!!!!!!!!!!!!!!! */
		/* For Test ONLY!!!!!!!!!!!!!!! */
		/* For Test ONLY!!!!!!!!!!!!!!! */
		/* For Test ONLY!!!!!!!!!!!!!!! */
		/* For Test ONLY!!!!!!!!!!!!!!! */
		/* For Test ONLY!!!!!!!!!!!!!!! */
		/*on_MainWindow_CurrentUserState_Changed(currentUserStateNout);*/
		/* For Test ONLY!!!!!!!!!!!!!!! */
		/* For Test ONLY!!!!!!!!!!!!!!! */
		/* For Test ONLY!!!!!!!!!!!!!!! */
		/* For Test ONLY!!!!!!!!!!!!!!! */
		/* For Test ONLY!!!!!!!!!!!!!!! */
	//}
}

void MainWindow::prepareTabWidget() {
	tabWidget = new TabWidget(this);
#if 0
  	QHBoxLayout *vlay = new QHBoxLayout;
	
	workSpace->setLayout(vlay);
	if (tabWidget) {
		vlay->addWidget(tabWidget);
	}
#endif
	setCentralWidget(tabWidget);

#ifndef SERVERLESS_MODE
        on_MainWindow_New();
#endif
}

/*void MainWindow::onListWidget_clicked ( const QModelIndex & index ) {
	
}*/

void MainWindow::on_MainWindow_MemberSelected(MemberIdentifier* memberIdentifier) {
	Options *options;
	options=getGlobalOptionsObj();

	if (memberIdentifier && options) {
		QString xxx=memberIdentifier->zusatz;

        while (!xxx[0].isNumber() && !xxx.isEmpty()) {
            xxx=xxx.remove(0,1);
        }
        if (xxx.size()>0) {
			lastSelectedTel=xxx.toInt();
        }
        lastSelectedShortName=memberIdentifier->shortname;
        lastSelectedfullName=memberIdentifier->fullName;

        dockWidget1->requestPersonPicture(memberIdentifier);
		if (memberIdentifier->shortname!=options->getUsername()) {
			 waitForWidget->addPersonToEditfield(memberIdentifier->shortname);
			 chatWidget->addPersonToEditfield(memberIdentifier->shortname);
			 sonstigerEintragEdit->setText("bei "+memberIdentifier->fullName);
		} else {
			 waitForWidget->addPersonToEditfield("");
			 chatWidget->addPersonToEditfield("");
			 sonstigerEintragEdit->setText("");
		}
  }
}

// Button New
void MainWindow::on_MainWindow_New() {
	TabPanel* tabPanel=0;
	if (tabWidget) {
		tabPanel=tabWidget->createNewStandardTab();
		if (tabPanel && tabPanel->listWidget) {
			connect(tabPanel->listWidget,SIGNAL(onMemberSelected(MemberIdentifier*)),this,SLOT(on_MainWindow_MemberSelected(MemberIdentifier*)));
			connect(tabPanel->listWidget,SIGNAL(onCurrentMemberStateUpdated(MemberLog *)),this,SLOT(on_MainWindow_CurrentUserState_Changed(MemberLog *)));
			connect(tabPanel->listWidget,SIGNAL(onCurrentListRefreshed()),this,SLOT(on_MainWindow_CurrentList_Refreshed()));
		}
	}
}

// Button neue Auswertung
void MainWindow::on_MainWindow_NewAuswertung() {
    Auswertung *dlgAuswertung=new Auswertung(this);
    if (dlgAuswertung) {
        connect(dlgAuswertung,SIGNAL(auswertungOk()),this,SLOT(on_Auswertung_auswertungOk()));
        dlgAuswertung->show();
    }
	//if (tabWidget) {
	//	tabWidget->createNewStandardTab("pam",2006,24,1, false); /* todo: von Auswertungsdialog */
	//}	
}
//es3.5:AUS:Paar                          :200601*.pze
//es3.5:AUS:tea                           :200601*.pze
void MainWindow::on_Auswertung_auswertungOk() {
    Options *options;
    options=getGlobalOptionsObj();
    if (options) {
        fprintf(stderr,"Auswertung OK pushed! \n");
        fprintf(stderr,options->name().toAscii());
        fprintf(stderr,options->vName().toAscii());

        tabWidget->createNewAuswertungTab(options->name().simplified(),
                                            options->vName().simplified(),
                                            options->year().simplified(),
                                            options->month().simplified(),
                                            options->day().simplified());


    }
}

void MainWindow::on_MainWindow_LogConfigDialog() {
    LogConfigDialog *dlg=new LogConfigDialog(this);
    if (dlg) {
        connect(dlg,SIGNAL(LogConfigChanged()),this,SLOT(on_MainWindow_LogConfigChanged()));
        dlg->show();
    }
}
void MainWindow::on_MainWindow_LogIdentifierDialog() {
    LogIdentifierView *dlg=new LogIdentifierView(this);
    if (dlg) {
        //connect(dlg,SIGNAL(LogConfigChanged()),this,SLOT(on_MainWindow_LogConfigChanged()));
        dlg->show();
    }
}

void MainWindow::on_MainWindow_LogConfigChanged() {
    updateUserdefinedActions();
    updateUserdefinedToolbar();
    updateUserdefinedMenu();
    updateUserdefinedTrayMenu();
    updateMainLogToolbar();
}

void MainWindow::on_MainWindow_Options() {
	dlgOptions=new DlgOptions(this);
	Options *options;
	options=getGlobalOptionsObj();
	if (options) {
		dlgOptions->setServer(options->serverName());
        dlgOptions->setCiscoPass(options->ciscoPass());
        dlgOptions->setPort(QString::number(options->serverPort()));
		dlgOptions->setServer_2(options->serverName_2());
		dlgOptions->setServerPath_2(options->serverPath_2());
		dlgOptions->setPort_2(QString::number(options->serverPort_2()));
		dlgOptions->setTimer(QString::number(options->timerSequence()));
		dlgOptions->setFontName(options->fontName());
		
		dlgOptions->setAutoLogout(options->autoLogout());
		dlgOptions->setInaktive(options->inaktive());
		dlgOptions->setAutoLogin(options->autoLogin());
		dlgOptions->setWanIfLogOut(options->wanIfLogOut());
		dlgOptions->setWanIfLogOutTimer(QString::number(options->wanIfLogOutTimer()));

		dlgOptions->setNotifierTimer(QString::number(options->notifierTimer()));
		dlgOptions->setNotifierX(QString::number(options->notifierX()));
		dlgOptions->setNotifierY(QString::number(options->notifierY()));
        dlgOptions->setNotifierDlg(options->showNotifierDlg());
		dlgOptions->setShowPersonPicture(options->showPersonPicture());
	}

	connect(dlgOptions,SIGNAL(dlgOptionsOk()),this,SLOT(on_DlgOptionsOk_dlgOptionsOk()));

	dlgOptions->show();	
}

void MainWindow::on_MainWindow_Refresh() {
    TabPanel	*tabPanel;
    if (tabWidget) {
        if (tabWidget->currentIndex()>=0) {
            QWidget *tmp=tabWidget->currentWidget();
            tabPanel = qobject_cast<TabPanel *>(tmp);
            if (tabPanel) {
                if (tabPanel->listWidget && !tabPanel->listWidget->isAuswertung) {
                    tabPanel->listWidget->updateData(0);
                    waitForUsers();
                }
            } else {
                /* todo: send refresh-request for this Poker Client */
            }
        }
    }
}

MemberData* MainWindow::getMemberDataFromFirstList() {
	TabPanel	*tabPanel;
	int			count;
	if (tabWidget) {
		count=tabWidget->count();
		for (int i=0;i<count;i++) {
                        QWidget *tmp=tabWidget->widget(i);
                        tabPanel = qobject_cast<TabPanel *>(tmp);
			if (tabPanel && tabPanel->listWidget && tabPanel->listWidget->memberData && !tabPanel->listWidget->isAuswertung) {
				return tabPanel->listWidget->memberData;
			}
		}
	}
	return 0;
}

void MainWindow::waitForUsers() {
	TabPanel	*tabPanel;
        int		count;
	QString		message="";
	bool		have_message=false;

	if (tabWidget  && waitForWidget->getPersonCount()>0) {
		count=tabWidget->count();
		for (int i=0;i<count;i++) {
            QWidget *tmp=tabWidget->widget(i);
            tabPanel = qobject_cast<TabPanel *>(tmp);
            if (tabPanel && tabPanel->listWidget && tabPanel->listWidget->memberData && !tabPanel->listWidget->isAuswertung) {
				int p_count=waitForWidget->getPersonCount();
				for (int p=p_count-1;p>=0;p--) {
					QString shortname=waitForWidget->getPersonAtIndex(p);
					if (shortname.size()==3) {
						MemberIdentifier *memberIdentifier=tabPanel->listWidget->memberData->searchMemberFromShortname(shortname);
						if (memberIdentifier) {
							MemberLog* memberLog=memberIdentifier->getlastLogFromToday();
							if (memberLog) {
								waitForWidget->setMemberLog(shortname, memberLog);
                                if (memberLog->in && memberLog->anm.trimmed().size()==0) {
									if (message.size()>0) {
										message=message+"\n";
									}
                                    message=message+memberIdentifier->vName+" "+memberIdentifier->name.trimmed()+" ist im Büro\n("+memberLog->zusatz+")";
									have_message=true;
									waitForWidget->removePerson(shortname);
								}
							}
						}
					}
				}	
			}
			break; /* Daten aus einem Tab reichen */
		}
        if (have_message) {
            QDomDocument doc;
            QDomElement msg=doc.createElement("msg");

            msg.setAttribute("l","Server");
            msg.setAttribute("ln","Server");
            msg.setAttribute("m",message);
            msg.setAttribute("popup","1");

            doc.appendChild(msg);

            QString qmessage=doc.toString();
            on_MainWindow_IncomingChatMessage(qmessage);
        }
#if 0
		if (have_message && systemTrayIcon) {
			systemTrayIcon->showMessage("companylog",message,QSystemTrayIcon::Information,1000*500);
		}
#endif
	}
}

void MainWindow::on_WaitForWidget_onPersonAdded(QString shortName) {
	waitForUsers();
}

void MainWindow::on_ChatWidget_sendMessage(QString to, QString message) {
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();
    if(ecoMiniReg && incomingPort>0) {
		ecoMiniReg->SendChatMessage(to,message);
		if (chatWidget) {
			 chatWidget->setMessage(false,to/*""*//*fromUser*/, "---"/*fromUserLong*/, message, true);
		}
  }
}

void MainWindow::on_MainWindow_CurrentList_Refreshed() {
	MemberData* memberData=getMemberDataFromFirstList();
	if (memberData) {
		m_currentMemberLog=*memberData->getLastLogForCurrentUser();
	}

	if (isInStartup==true) {
		/* AutoLogin on Application-Startup */
		isInStartup=false;
		autoLoginOnStartup();
	}

	waitForUsers();
}

void MainWindow::setupGlobalFont() {
	Options *options;
	options=getGlobalOptionsObj();
	if (options) {
		if (options->fontName().size()>0) {
			QFont newFont;
			if (newFont.fromString(options->fontName())) {
				if (qApplication) {
					qApplication->setFont(newFont);	
				} else {
					setFont(newFont);	
				}
			}
		}
	}
}

void MainWindow::on_DlgOptionsOk_dlgOptionsOk() {
	fprintf(stderr,"DlgOptionsOK pushed! \n");
	Options *options;
	options=getGlobalOptionsObj();
	if (options) {
		options->setServerName(dlgOptions->server());
        options->setCiscoPass(dlgOptions->ciscoPass());
        options->setServerPort(dlgOptions->port().toInt());
		options->setServerName_2(dlgOptions->server_2());
		options->setServerPath_2(dlgOptions->serverPath_2());
		options->setServerPort_2(dlgOptions->port_2().toInt());
		options->setTimerSequence(dlgOptions->timer().toInt());
		options->setFontName(dlgOptions->fontName());
		options->setAutoLogout(dlgOptions->autoLogout());
		options->setInaktive(dlgOptions->inaktive());
		options->setAutoLogin(dlgOptions->autoLogin());
		options->setWanIfLogOut(dlgOptions->wanIfLogOut());
		options->setWanIfLogOutTimer(dlgOptions->wanIfLogOutTimer().toInt());

		options->setNotifierTimer(dlgOptions->notifierTimer().toInt());
		options->setNotifierX(dlgOptions->notifierX().toInt());
		options->setNotifierY(dlgOptions->notifierY().toInt());
                options->setNotifierDlg(dlgOptions->showNotifierDlg());
		options->setShowPersonPicture(dlgOptions->showPersonPicture());

		options->saveOptionsToFile(options->getFilenameFromUser());
		
		setupGlobalFont();
	}

	delete dlgOptions;
	dlgOptions=0;
}

// Button Info
void MainWindow::on_MainWindow_Info() {
	DlgAbout *dlgAbout = new DlgAbout(this);
	if (dlgAbout) {
		//dlgAbout->setModal(false);
		dlgAbout->exec();
		delete dlgAbout;
	}
}

void MainWindow::beforeShutdownHandler() {
    Options *options;
    QByteArray array;

    options=getGlobalOptionsObj();
    // save Windowstates
    if (options) {
            array=saveState();
            options->setDockState(array);

            array=saveGeometry();
            options->setMainWindowState(array);

            options->saveOptionsToFile(options->getFilenameFromUser());
            options->saveLogOptions();
    }

#ifndef SERVERLESS_MODE
    // chat horchport beim Server deregistrieren
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();
    if(ecoMiniReg && incomingPort>0) {
        ecoMiniReg->UnregisterChatPort(incomingPort);
    }
    // logout the user
    autoLogoutOnShutdown();
#endif
}

// Button Beenden
void MainWindow::on_MainWindow_Exit() {
	beforeShutdownHandler();
	qApplication->quit();
/*	if (systemTrayIcon) { 
		systemTrayIcon->hide();
		delete systemTrayIcon;
		systemTrayIcon=0;
	}
	close();*/
}

void MainWindow::autoLoginOnStartup() {
    Options *options;
    options=getGlobalOptionsObj();
    if (options) {
        if (options && options->autoLogin() && m_currentMemberLog.in==false) {
            /* do Login */
            on_MainWindow_LogIn();
        }
    }
}

void MainWindow::autoLogoutOnShutdown() {
	Options *options;
	options=getGlobalOptionsObj();
	if (options) {
		if (options && options->autoLogout() && m_currentMemberLog.in==true) {
			/* do Logout */
			on_MainWindow_LogOut();
		}
	}
}


void MainWindow::setupEndSessionManagement(QApplication *qapp) {
	if (qapp) {
		connect(qapp, SIGNAL(commitDataRequest(QSessionManager&)),this, SLOT(on_MainWindow_CommitData(QSessionManager&)), Qt::DirectConnection);
	}
}

void MainWindow::on_MainWindow_CommitData(QSessionManager& qSessionManager) {
	// session goes down, autologout user
	isInShutdown=true;
	beforeShutdownHandler();
	qSessionManager.release();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (!isInShutdown && systemTrayIcon && systemTrayIcon->isVisible()) {
        hide();
        e->ignore();
    }
}


void MainWindow::on_MainWindow_SysTrayIcon_activated(QSystemTrayIcon::ActivationReason reason) {
	if (reason==QSystemTrayIcon::Trigger) {
		on_MainWindow_Wiederherstellen();
	}
}

void MainWindow::on_MainWindow_Wiederherstellen() {
	activateWindow();
	raise();
	show(); 
}

void MainWindow::on_MainWindow_LogIn() {
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();
    if(ecoMiniReg) {
        startWarnIfLogOutTimer(false);
		ecoMiniReg->LogIn();
		setTrayIconFromLastLoginAction();
		startDelayRefreshTimer();
	}
}
void MainWindow::on_MainWindow_LogOut() {
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();
    if(ecoMiniReg) {
        ecoMiniReg->LogOut();
		setTrayIconFromLastLoginAction();
		startWarnIfLogOutTimer(true);
		startDelayRefreshTimer();
	}
}

void MainWindow::on_MainWindow_LogSonstigIn() {
	bool have_text=false;
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();
    if(ecoMiniReg) {
        ecoMiniReg->LogSonstigIn(sonstigerEintragEdit->text());
		if (sonstigerEintragEdit->text().size()>0) {
			 have_text=true;
		}
		setTrayIconFromLastLoginAction();
		sonstigerEintragEdit->setText("");
		if (have_text) {
			 startWarnIfLogOutTimer(true);// obwohl in, aber hoechstwarscheinlich nicht im Buero
		} else {
			 startWarnIfLogOutTimer(false);
		}
		startDelayRefreshTimer();
	}
}
void MainWindow::on_MainWindow_LogSonstigOut() {
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();
    if(ecoMiniReg) {
        ecoMiniReg->LogSonstigOut(sonstigerEintragEdit->text());
		setTrayIconFromLastLoginAction();
		sonstigerEintragEdit->setText("");
		startWarnIfLogOutTimer(true);
		startDelayRefreshTimer();
	}
}


void MainWindow::logUserdefined(LogAction *logAction) {
    bool have_text=false;
    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();
    if(ecoMiniReg && logAction) {
        if (logAction->locked()=="1") {
            if (logAction->text()==LOGIN) {
                on_MainWindow_LogIn();
            } else if (logAction->text()==LOGOUT) {
                on_MainWindow_LogOut();
            } else if (logAction->text()==LOGIN_SONST) {
                on_MainWindow_LogSonstigIn();
            } else if (logAction->text()==LOGOUT_SONST) {
                on_MainWindow_LogSonstigOut();
            }
        } else {
            if (logAction->in()=="IN") {
                ecoMiniReg->LogSonstigIn(logAction->text());
            } else {
                ecoMiniReg->LogSonstigOut(logAction->text());
            }
            setTrayIconFromLastLoginAction();
            if (logAction->in()!="IN") {
                /* out */
                startWarnIfLogOutTimer(true);
            } else {
                /* in */
                if (logAction->text().size()==0) {
                     startWarnIfLogOutTimer(false);
                } else {
                    startWarnIfLogOutTimer(true);// obwohl in, aber hoechstwarscheinlich nicht im Buero
                }
            }
            startDelayRefreshTimer();
        }
    }
}


void MainWindow::timerEvent(QTimerEvent *event) {
	if (event->timerId()==lastWarnIfLogOutTimer) {
		killTimer(event->timerId());
		lastWarnIfLogOutTimer=-1;
		on_MainWindow_Wiederherstellen();
		/* Show Request Message!!! */
		int ret = QMessageBox::question(this, tr("companylog"),
                   tr("Sie sind derzeit nicht als 'im Büro' eingeloggt. Wieder einloggen?"),
                   QMessageBox::Yes | QMessageBox::No);

		if (ret==QMessageBox::Yes) {
			on_MainWindow_LogIn();
		}
	} else if (event->timerId()==lastDelayRefreshTimer) {
		killTimer(event->timerId());
		lastDelayRefreshTimer=-1;
		on_MainWindow_Refresh();
	} else if (event->timerId()==lastChatServerRegistrationTimer) {
		killTimer(event->timerId());
		lastChatServerRegistrationTimer=-1;
		
		registerChatManager();

		startChatServerRegistrationTimer();
	}
}

void MainWindow::startWarnIfLogOutTimer(bool start) {
	Options *options;
	options=getGlobalOptionsObj();

	if (lastWarnIfLogOutTimer!=-1) {
		killTimer(lastWarnIfLogOutTimer);
        lastWarnIfLogOutTimer=-1;
	}
	if (options && start && options->wanIfLogOut()) {
		if (options) {
			long min=options->wanIfLogOutTimer();

			lastWarnIfLogOutTimer=startTimer(min*60*1000);
		}
	}
}


void MainWindow::startDelayRefreshTimer() {
	Options *options;
	options=getGlobalOptionsObj();

	if (lastDelayRefreshTimer!=-1) {
		killTimer(lastDelayRefreshTimer);
        lastDelayRefreshTimer=-1;
	}
	lastDelayRefreshTimer=startTimer(500);/* eine halbe Sekunde Warten bis zum Refresh der Liste, damit kann die Log-Aktion zuvor ausgefuehrt werden */
}

void MainWindow::startChatServerRegistrationTimer() {
    Options *options;
    options=getGlobalOptionsObj();

    if (lastChatServerRegistrationTimer!=-1) {
            killTimer(lastChatServerRegistrationTimer);
            lastChatServerRegistrationTimer=-1;
    }
    lastChatServerRegistrationTimer=startTimer(1000*60*15);/* Horchport alle 15 Min neu registrieren */
}

void MainWindow::setTrayIconFromLastLoginAction()
{
    Options *options;
    options=getGlobalOptionsObj();

    EcoMiniReg* ecoMiniReg=getGlobalEcoMiniReg();
    if(ecoMiniReg) {
        MemberLog m_currentMemberLogFromLastLogAction=ecoMiniReg->currentUserStateFromLastLogAction();

        QIcon* trayIcon=options->logManager()->getMixedIconFromLog(options->pixmapCacheIcons()->getPixmap("companylog.png"), "companylog.png", m_currentMemberLogFromLastLogAction.message(), m_currentMemberLogFromLastLogAction.isIn());
        if (trayIcon) {
            if (systemTrayIcon) {
                systemTrayIcon->setIcon(*trayIcon);
            }
        }
        m_currentMemberLog=m_currentMemberLogFromLastLogAction;
    }
}


void MainWindow::on_MainWindow_CurrentUserState_Changed(MemberLog *currentMemberLog) {
    Options *options;
    options=getGlobalOptionsObj();

    if (currentMemberLog) {
        if (m_currentMemberLog.anm!=currentMemberLog->anm || m_currentMemberLog.in!=currentMemberLog->in) {
            QIcon* trayIcon=options->logManager()->getMixedIconFromLog(options->pixmapCacheIcons()->getPixmap("companylog.png"), "companylog.png", currentMemberLog->message(), currentMemberLog->isIn());
            if (trayIcon) {
                if (systemTrayIcon) {
                    systemTrayIcon->setIcon(*trayIcon);
                }
            }
        }
        m_currentMemberLog=(*currentMemberLog);
    }

//	if (currentStatePresenter) {
//		currentStatePresenter->setStatusFromMemberLog(currentMemberLog);
//	}
}

bool MainWindow::updateGamestatus(QString message) {
    bool        handled=false;
#ifndef POKERLESS_MODE
    PokerView   *pokerView=0;
    int         count=tabWidget->count();
    QString     id;

    QDomDocument doc( "AdBookML" );
    doc.setContent(message);
    QDomElement root = doc.documentElement();

    QDomNodeList msg=root.elementsByTagName("msg");
    if (msg.count()>0) {
        QDomNode msg0_node=msg.item(0);
        if (msg0_node.isElement()) {
            QDomElement msg0 = msg0_node.toElement();
            QString msg_t=msg0.attribute("t", "" );
            if (msg_t=="game_state") {
                id = msg0.attribute( "game_id", "" );
            }
        }
    }
    if (id.size()>0) {
        for (int i=0;i<count && !handled;i++) {
            QWidget *tmp=tabWidget->widget(i);
            pokerView = qobject_cast<PokerView *>(tmp);
            if (pokerView) {
                handled=pokerView->updateGamestatus(message);
            }
        }
        if (!handled) {
            pokerView=tabWidget->createNewPokerTab();
            if (pokerView) {
                handled=pokerView->updateGamestatus(message);
            }
        }
    }
#endif
    return handled;
}

void MainWindow::on_MainWindow_testPoker() {
#ifndef POKERLESS_MODE
    //tabWidget->createNewPokerTab();

    QString    message="<ecm t=\"THOLDM\">"
           "<msg t=\"game_status\" "
                        "game_id=\"1234\""
                        "count=\"0\""
                        "status=\"ROUND_STARTED\""
                        "pot=\"0\""
                        "round=\"0\""
                        "min_bigblind=\"50\""
                        "dealer=\"hre\""
                        "smallblind=\"kuk\""
                        "bigblind=\"pam\""
                        "nextplayer=\"ret\""
                        "can_bet=\"TRUE\""
                        "can_call=\"TRUE\""
                        "can_raise=\"TRUE\""
                        "can_check=\"TRUE\""
                        "can_fold=\"TRUE\""
                        "can_deal=\"TRUE\""
                        "min_call=\"100\""
                        "card_1=\"1\""
                        "card_2=\"2\""
                        "card_3=\"3\""
                        "card_4=\"40\""
                        "card_5=\"30\""
                        ">"
                        "<player "
                                "l=\"hre\""
                                "in_game=\"TRUE\""
                                "money=\"1\""
                                "act_bet=\"0\""
                                "card_1=\"10\""
                                "card_2=\"11\""
                        "/>"
                        "<player "
                                "l=\"kuk\""
                                "in_game=\"TRUE\""
                                "money=\"2\""
                                "act_bet=\"25\""
                                "card_1=\"12\""
                                "card_2=\"13\""
                        "/>"
                        "<player "
                                "l=\"pam\""
                                "in_game=\"TRUE\""
                                "money=\"3\""
                                "act_bet=\"50\""
                                "card_1=\"14\""
                                "card_2=\"15\""
                                "last_action=\"CALL\""
                        "/>"
                        "<player "
                                "l=\"ret\""
                                "in_game=\"TRUE\""
                                "money=\"4\""
                                "act_bet=\"0\""
                                "card_1=\"16\""
                                "card_2=\"17\""
                        "/>"
                        "<player "
                                "l=\"drw\""
                                "in_game=\"TRUE\""
                                "money=\"5\""
                                "act_bet=\"0\""
                                "card_1=\"18\""
                                "card_2=\"19\""
                        "/>"
                        "<player "
                                "l=\"muc\""
                                "in_game=\"TRUE\""
                                "money=\"6\""
                                "act_bet=\"0\""
                                "card_1=\"20\""
                                "card_2=\"21\""
                        "/>"
                        "<player "
                                "l=\"kli\""
                                "in_game=\"TRUE\""
                                "money=\"7\""
                                "act_bet=\"0\""
                                "card_1=\"22\""
                                "card_2=\"23\""
                        "/>"
                        "<player "
                                "l=\"goe\""
                                "in_game=\"TRUE\""
                                "money=\"8\""
                                "act_bet=\"0\""
                                "card_1=\"24\""
                                "card_2=\"25\""
                        "/>"
                "</msg>"
        "</ecm>";

    updateGamestatus(message);
#endif
}

void MainWindow::on_MainWindow_testCisco() {
    Options *options;
    options=getGlobalOptionsObj();
    MemberData* memberData=getMemberDataFromFirstList();


    if (options->ciscoPass().size()>0) {
        if (lastSelectedTel>0 ) {
            if (memberData) {
                MemberIdentifier *me=memberData->searchMemberFromShortname(options->getUsername());
                if (me) {
					int myTel=0;
					QString xxx=me->zusatz;

					while (!xxx[0].isNumber()) {
						xxx=xxx.remove(0,1);
					}
					if (xxx.size()>0) {
						myTel=xxx.toInt();
					}

                    if (myTel>0) {
                        int rv=ciscoDealer.deal(QString::number(lastSelectedTel),options->ciscoPass(),me->shortname,QString::number(myTel));
                        if (rv==CiscoRequestRv_OK) {
                            statusBar()->showMessage("Cisco call to "+lastSelectedfullName+" ("+QString::number(lastSelectedTel)+") successfull...",5*1000);
                        } else if (rv==CiscoRequestRv_ConnectionFailed) {
                            QMessageBox::information(this, "companylog - Cisco dialup", "Verbindung konnte nicht aufgebaut werden!");
                        } else if (rv==CiscoRequestRv_AuthenticationFailed) {
                            QMessageBox::information(this, "companylog - Cisco dialup", "Authentifizierung fehlgeschlagen. Bitte das eingetragene Passwort in den Optionen überprüfen!");
                        } else if (rv==CiscoRequestRv_DeviceRegisterFailed) {
                            QMessageBox::information(this, "companylog - Cisco dialup", "Device konnte nicht registriert werden!");
                        }
                    } else {
                        QMessageBox::information(this, "companylog - Cisco dialup", "Die eigene Durchwahl konnte aus dem Logeintrag nicht ermittelt werden!");
                    }
                } else {
                    QMessageBox::information(this, "companylog - Cisco dialup", "Der eigene Benutzer ist in der Logliste nicht vorhanden. Eigene Durchwahl konnte nicht ermittelt werden!");
                }
            } else {
                QMessageBox::information(this, "companylog - Cisco dialup", "Es wurde kein Logliste geöffnet!");
            }
        } else {
            QMessageBox::information(this, "companylog - Cisco dialup", "Es wurde kein Benutzer selektiert, oder die Durchwahl konnte aus dem selektierten Logeintrag nicht ermittelt werden!");
        }
    } else {
        QMessageBox::information(this, "companylog - Cisco dialup", "Es wurde noch kein Passwort für die Telefonanlage eingetragen. Das Passwort kann in den Optionen gesetzt werden!");
    }
}

void MainWindow::on_MainWindow_sqlWorksheet() {
    Options *options;
    options=getGlobalOptionsObj();

    /* request */
    /*
    <esm t="SQL">
        <stmt>select * from XXX</stmt>
    </esm>
     */

    if (tabWidget) {
        tabWidget->createNewSqlWorksheetTab();
    }
}
