/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h18
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : MainWindow.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QWorkspace>
#include <QStatusBar>
#include <QTabWidget>
#include <QSizePolicy>
#include <QBrush>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QSpacerItem>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QSessionManager>

#include "DockWidget.h"
#include "WaitForWidget.h"
#include "ChatWidget.h"
#include "TabWidget.h"
#include "Auswertung.h"
#include "ListWidget.h"
#include "Options.h"
#include <QDate>
#include "UserStatusWidget.h"
#include "About.h"
#include "ChatManager.h"
#include <QMessageBox>
//#include <QDomElement>
#include <QDomDocument>
#include "popupwindow.h"
#include <QDesktopWidget>
#include "EcoMiniReg.h"
#include "pokergames.h"
#include "logconfigdialog.h"
#include "logidentifierview.h"
#include "ciscodealer.h"
#include "sqlworksheet.h"

#define ECO_VERSION "1.0 - RC 1"

class MainWindow : public QMainWindow {
	Q_OBJECT
	public:
	MainWindow(QApplication *qapp, QWidget *parent=0);
	~MainWindow();
	QApplication	*qApplication;
	private:
	long	chatPort;
	int	lastWarnIfLogOutTimer;
	int	lastDelayRefreshTimer;
        int	lastChatServerRegistrationTimer;
	bool isInStartup;
	bool isInShutdown;
	QAction	*newAction;
	QAction *infoAction;
	QAction *exitAction;
	QAction *auswertungAction;
	QAction *optionsAction;
        QAction *logConfigAction;
        QAction *logIdentifierAction;
        QAction *refreshAction;
	QAction *wiederherstellenAction;

        QAction *testPokerAction;
        QAction *testCiscoAction;
        QAction *sqlWorksheetAction;


        QList<QAction*> userdefinedActionList;
        QList<QAction*> userdefinedActionListMixed;
        QLabel		*sonstigerEintragLabel;
	QLineEdit	*sonstigerEintragEdit;

	QMenu *fileMenu;
	QMenu *logMenu;
	QMenu *einstellungenMenu;
	QMenu *helpMenu;
	QMenu *trayMenu;

	QWorkspace	*workSpace;
	QToolBar	*toolBar;
	QToolBar	*toolBar2;
        QToolBar	*toolBar3;

	TabWidget	*tabWidget;
	DockWidget	*dockWidget1;
	WaitForWidget *waitForWidget;
	ChatWidget *chatWidget;
        PokerGames *pokerGames;
	QWidget *listWidget1;
	QListWidget *listWidget2;

	QSystemTrayIcon	*systemTrayIcon;
	DlgOptions	*dlgOptions;	

	UserStatusWidget *currentStatePresenter;
	MemberLog		m_currentMemberLog;

    PopupWindow *popupWindow;
    CiscoDealer ciscoDealer;

	QIcon *act_icon;

    int lastSelectedTel;
    QString lastSelectedShortName;
    QString lastSelectedfullName;
    ChatManager *chatManager;
	long	incomingPort;
	void closeEvent(QCloseEvent *e);
        bool updateGamestatus(QString message);
	void prepareWorkspace();
	void restorWorkspaceFromOptions();
	void prepareDockWidget();
	void prepareActions();
	void prepareMenu();
	void prepareSystemTrayIcon();
	void prepareToolbar();
	void prepareStatusBar();
	void prepareTabWidget();
	void prepareEcoMiniReg();
	void setCurrentUserState();
	void setTrayIconFromLastLoginAction();
	void autoLoginOnStartup();
	void autoLogoutOnShutdown();
	void startAutoRefreshCurrentUserState();
	void setupGlobalFont();
	void startWarnIfLogOutTimer(bool start);
	void startDelayRefreshTimer();
	void setupEndSessionManagement(QApplication *qapp);
	void beforeShutdownHandler();
	void registerChatManager();
    void startChatServerRegistrationTimer();

	MemberData* getMemberDataFromFirstList();
	
	void waitForUsers();
    void logUserdefined(LogAction *logAction);
    void updateUserdefinedActions();
    void updateUserdefinedToolbar();
    void updateUserdefinedMenu();
    void updateUserdefinedTrayMenu();
    void updateMainLogToolbar();

    void deleteUserdefinedActionLists();

	public slots:
    void on_MainWindow_UserdefinedActionClicked();
	void on_MainWindow_New();
	void on_MainWindow_Info();
	void on_MainWindow_Exit();
	void on_MainWindow_NewAuswertung();
	void on_MainWindow_Options();
    void on_MainWindow_LogConfigDialog();
    void on_MainWindow_LogConfigChanged();
    void on_MainWindow_LogIdentifierDialog();
    void on_MainWindow_Refresh();
    void on_MainWindow_testCisco();
    void on_MainWindow_sqlWorksheet();
    void on_MainWindow_Wiederherstellen();
	void on_MainWindow_SysTrayIcon_activated(QSystemTrayIcon::ActivationReason reason);

	void on_MainWindow_LogIn();
	void on_MainWindow_LogOut();
	void on_MainWindow_CurrentUserState_Changed(MemberLog *currentMemberLog);
	void on_MainWindow_LogSonstigIn();
	void on_MainWindow_LogSonstigOut();
	void on_MainWindow_MemberSelected(MemberIdentifier* memberIdentifier);
	void on_MainWindow_CurrentList_Refreshed();
    void on_MainWindow_IncomingChatMessage(QString);
	void on_ChatWidget_sendMessage(QString to, QString message);

	void on_Auswertung_auswertungOk();
	void on_DlgOptionsOk_dlgOptionsOk();
	void on_WaitForWidget_onPersonAdded(QString);
	void on_MainWindow_CommitData(QSessionManager&);

    void on_MainWindow_testPoker();

	protected:
	void timerEvent(QTimerEvent *event);
	void timerEventDelayRefresh(QTimerEvent *event);

};


#endif
