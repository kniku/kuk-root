/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h21
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : Options.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef OPTIONS_H
#define OPTIONS_H

#include <QObject>
#include <QString>
#include <QDialog>
#include <QStringList>
#include <stdlib.h>
#include <QFile>
#include <QFontDialog>
#include "ui_DlgOptions.h"
#include <QPixmap>
#include "pixmapcache.h"
#include "logmanager.h"
#include <QWidget>
#include "ui_Auswertung.h"
#include <QDir>
#include <QPainter>
#include <QPixmap>
#include <QDir>
//#include "EcoMiniReg.h"


typedef enum {
	SQL_BASETYPE_STRING=1,
	SQL_BASETYPE_INT,
	SQL_BASETYPE_DOUBLE,
	SQL_BASETYPE_DATETIME,
	SQL_BASETYPE_BLOB
}SQL_BASETYPE;

#define SQL_BASETYPE_STRING_V	"string"
#define SQL_BASETYPE_INT_V		"int"
#define SQL_BASETYPE_DOUBLE_V	"double"
#define SQL_BASETYPE_DATETIME_V "datetime"
#define SQL_BASETYPE_BLOB_V		"blob"

class EcoMiniReg;

typedef struct _EmoticonsRec {
	bool	used;
	QString substring;
	QString picture;
}EmoticonsRec, *Emoticons;


#define EMOTICONS_REC EmoticonsRec emoticons[]={ \
	{1,":)","smile"}, \
	{1,":-)","smile"}, \
	{1,":o)","smile"}, \
	{1,";)","wink"}, \
	{1,";-)","wink"}, \
	{1,";o)","wink"}, \
	{1,":(","sad"}, \
	{1,":-(","sad"}, \
	{1,":o(","sad"}, \
	{1,";(","cry"}, \
	{1,";-(","cry"}, \
	{1,":o","omg"}, \
	{1,":-o","omg"}, \
	{1,":p","tongue"}, \
	{1,":-p","tongue"}, \
	{1,";p","tongue"}, \
	{1,";-p","tongue"}, \
	{1,":[","bat"}, \
	{1,":-[","bat"}, \
	{1,";[","bat"}, \
	{1,";-[","bat"}, \
    {0,"",""} \
};

class Options:public QObject {
    QString	m_serverName;
    QString	m_ciscoPass;
    long	m_serverPort;
    QString	m_serverName_2;
    QString	m_serverPath_2;
    long	m_serverPort_2;
    long	m_timerSequence;

    long	m_notifierTimer;
    bool    m_notifierShow;
    long	m_notifierX;
    long	m_notifierY;
    bool 	m_showPersonPicture;


    QString	m_vName;
    QString	m_name;
    QString	m_year;
    QString	m_month;
    QString	m_day;
    QString	m_font;

    bool m_autoLogout;
    bool m_inaktive;
    bool m_autoLogin;
    bool m_wanIfLogOut;
    long m_wanIfLogOutTimer;
	
private:
    void setOption(QString line);
    QByteArray byteArrayListState;
    QByteArray byteArrayDockState;
    QByteArray byteArrayMainWindowState;

    PixmapCache pixmapCache_Icons;
    PixmapCache pixmapCache_Emoticons;
    LogManager  *logManager_intern;

    QString companylogUserHomePath;
public:
    Options();
    ~Options();


    PixmapCache *pixmapCacheIcons();
    PixmapCache *pixmapCacheEmoticons();
    LogManager *logManager();
    QString getLogOptionFilenameFromUser();
    void loadLogOptions();
    void saveLogOptions();

    void readOptionsFromFile();
    void saveOptionsToFile(QString filename);
    QString getFilenameFromUser();
    QString getCompanylogUserHomePath();
    QString getIconPath();
    QString getEmoticonPath();
    QString getLogIconPath();
    QString getLogIconPrivatePath();

    QString getUsername();

    QByteArray	listState();
    QByteArray	dockState();
    QByteArray	mainWindowState();

    QString serverName();
    QString ciscoPass();
    QString serverName_2();
    QString serverPath_2();
    long	serverPort();
    long	serverPort_2();
    long	timerSequence();
    QString	fontName();
    void 	setServerName(QString server_name);
    void 	setCiscoPass(QString cisco_pass);
    void	setServerPort(long server_port);
    void 	setServerName_2(QString server_name);
    void 	setServerPath_2(QString server_name);
    void	setServerPort_2(long server_port);
    void	setTimerSequence(long timer_seq);
    void	setFontName(QString font_name);

    void	setListState(QByteArray);
    void	setDockState(QByteArray);
    void    setMainWindowState(QByteArray);

    long	notifierTimer();
    long	notifierX();
    long	notifierY();
    bool    showNotifierDlg();
    bool 	showPersonPicture();

    void	setNotifierTimer(long notifier_timer);
    void	setNotifierX(long notifierX);
    void	setNotifierY(long notifierY);
    void    setNotifierDlg(bool showNotifier);
    void 	setShowPersonPicture(bool showPersonPicture);


    QString vName();
    QString name();
    QString year();
    QString month();
    QString day();

    void setVName(QString s);
    void setName(QString s);
    void setYear(QString s);
    void setMonth(QString s);
    void setDay(QString s);
    void setAutoLogout(bool b);
    void setInaktive(bool b);
    void setAutoLogin(bool b);
    void setWanIfLogOut(bool b);
    void setWanIfLogOutTimer(long b);
    bool autoLogout();
    bool inaktive();
    bool autoLogin();
    bool wanIfLogOut();
    long wanIfLogOutTimer();
};




class DlgOptions:public QDialog, private Ui::DlgOptions {
	Q_OBJECT
	public:
	DlgOptions(QWidget *parent=0);
	~DlgOptions();
	QString server();
    QString ciscoPass();
    QString server_2();
	QString serverPath_2();
	QString port();
	QString port_2();
	QString timer();
	QString fontName();

	void setServer(QString);
    void setCiscoPass(QString);
	void setServer_2(QString);
	void setServerPath_2(QString);
	void setPort(QString);
	void setPort_2(QString);
	void setTimer(QString);
	void setFontName(QString);
	void setAutoLogout(bool b);
	void setInaktive(bool b);
	void setAutoLogin(bool b);
	void setWanIfLogOut(bool b);
	void setWanIfLogOutTimer(QString b);
	bool autoLogout();
	bool inaktive();
	bool autoLogin();
	bool wanIfLogOut();
	QString wanIfLogOutTimer();

	QString	notifierTimer();
	QString notifierX();
	QString	notifierY();
        bool    showNotifierDlg();
	bool 	showPersonPicture();

	void	setNotifierTimer(QString notifier_timer);
	void	setNotifierX(QString notifierX);
	void	setNotifierY(QString notifierY);
        void    setNotifierDlg(bool showNotifier);
	void 	setShowPersonPicture(bool showPersonPicture);


	public slots:
	void on_DlgOptions_okButton();
	void on_DlgOptions_pushButtonFont();
	signals:
	void dlgOptionsOk();
};




Options* getGlobalOptionsObj();
void destroyGlobalOptionsObj();
EcoMiniReg* getGlobalEcoMiniReg();
void destroyGlobalEcoMiniReg();




#endif
