#ifndef SERVERINFO_H
#define SERVERINFO_H

#include <QString>
#include "Options.h"
#include "RequestObject.h"
#include <QObject>


//Q_DECLARE_TR_FUNCTIONS(EcoMiniReg);
class ServerInfo:public QObject {
	Q_OBJECT
public:
	ServerInfo();
	~ServerInfo();

	void requestCurrentServerInfo();
	RequestObject 	*requestObject;
	QString getVersion();
	QString getUptime();
	QString getRequests();
	QString getThreads();
private:
	QString version;
	QString uptime;
	QString requests;
	QString threads;

public slots:
	void requestIsFinish();
signals:
	void serverRequestIsFinish();
};

#endif
