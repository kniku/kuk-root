#include "About.h"

DlgAbout::DlgAbout(QWidget *parent) : QDialog(parent) {
	Options *options;
	options=getGlobalOptionsObj();
	//DlgAbout *dlgAbout = new DlgAbout(this);
	setupUi(this);
	okButton->setAutoDefault(false);
	connect(okButton,SIGNAL(clicked()),this,SLOT(on_DlgAbout_okButton()));
	connect(refreshButton,SIGNAL(clicked()),this,SLOT(on_DlgAbout_refreshButton()));
	//connect(pushButtonFont,SIGNAL(clicked()),this,SLOT(on_DlgAbout_pushButtonFont()));

	if (options) {
        label_6->setPixmap(*options->pixmapCacheIcons()->getPixmap("companylog.png"));
	}
	serverInfo=new ServerInfo();
	if (serverInfo) {
		connect(serverInfo, SIGNAL(serverRequestIsFinish()), this, SLOT(on_DlgAbout_ServerInfoFinished()));
		serverInfo->requestCurrentServerInfo();
	}
}

void DlgAbout::on_DlgAbout_ServerInfoFinished() {
	if (serverInfo) {
		version->setText("Version: "+serverInfo->getVersion());
        uptime->setText("Uptime: "+serverInfo->getUptime()+" hours");
        requests->setText("Requests: "+serverInfo->getRequests());
        threads->setText("Open threads: "+serverInfo->getThreads());
	}
}

void DlgAbout::on_DlgAbout_okButton() {
	close();
}

void DlgAbout::on_DlgAbout_refreshButton() {
	if (serverInfo) {
		serverInfo->requestCurrentServerInfo();
	}
}

DlgAbout::~DlgAbout() {
	//okButton->setAutoDefault=false;
	fprintf(stderr,"DlgAbout destructor called\n");
	if (serverInfo) {
		delete serverInfo;
	}
}
