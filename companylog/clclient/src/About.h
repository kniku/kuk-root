#include <QObject>
#include <QString>
#include <QDialog>
#include <QWidget>
#include "ui_About.h"
#include "ServerInfo.h"
#include "Options.h"
class DlgAbout:public QDialog, private Ui::About {
	Q_OBJECT
	public:
	DlgAbout(QWidget *parent=0);
	~DlgAbout();
	ServerInfo *serverInfo;
	
	public slots:
	void on_DlgAbout_ServerInfoFinished();
	void on_DlgAbout_okButton();
	void on_DlgAbout_refreshButton();
//	signals:
//	void DlgAboutOk();
};
