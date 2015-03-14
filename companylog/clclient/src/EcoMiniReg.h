#ifndef ECOMINIREG_H
#define ECOMINIREG_H


#include <QString>
#include <QObject>
#include <QDomElement>
#include <QDomDocument>

#include "Options.h"
#include "RequestObject.h"
#include "EcoSocket.h"
#include "MemberLog.h"

//requestObject* requestObject;



class EcoMiniReg : public QObject {
	Q_OBJECT
public:
    RequestObject *requestObject;
    MemberLog currentUserState();
    MemberLog currentUserStateFromLastLogAction();
    MemberLog m_currentMemberLog;
    MemberLog m_currentMemberLogFromLastLogAction;
    EcoMiniReg();
	~EcoMiniReg();
    int sendCommandToServer(QString command);
    int sendCommandToServer(QString command, int serverPort);

	int LogIn();
	int LogOut();
	int LogOutMittag();
	int LogOutWWW();
	int LogInKlBspr();
	int LogInBspr();
	int LogInWartung();
	int LogInJourfix();
	int LogOutRauch();
	int LogSonstigIn(QString text);
	int LogSonstigOut(QString text);
	void requestCurrentUserStateFromServer();
	void startStateRefreshTimer();
	int	lastStateRefreshTimerId;
	int SendChatMessage(QString to, QString message);
	int RegisterChatPort(long port);
	int UnregisterChatPort(long port);
	
	bool	notfirstRequest;
private:
	int LogWithMessage(QString message, int in);
	QString lastAnmText;


public slots:
	void requestIsFinish();
signals:
    void currentUserStateChanged(MemberLog *currentMemberLog);
protected:
	void timerEvent(QTimerEvent *event);
};

#endif
