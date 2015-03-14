/********************************************************************************************************
 * PROGRAM      : src
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h11
 * AUTHOR       :  Martin Paar
 * FILENAME     : MemberData.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef MEMBER_DATA_H
#define MEMBER_DATA_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QMap>
#include <QStringList>
#include <QStandardItemModel>
#include <QColor>
#include <QIcon>

#include "MemberIdentifier.h"
#include "MemberLog.h"
#include "Options.h"

#define COL1_LEN 2
#define COL2_LEN 10
#define TIME_LEN 8
#define DATE_LEN 8
#define STATE_LEN 2
#define COL6_LEN 2
#define NAME_LEN 16
#define VNAME_LEN 16
#define ANM_LEN 30 /* normal 20 */
#define ZUSATZ_LEN 30

#define COL1_OFFSET 0
#define COL2_OFFSET 3
#define TIME_OFFSET 14
#define DATE_OFFSET 23
#define STATE_OFFSET 32
#define COL6_OFFSET 35
#define NAME_OFFSET 38
#define VNAME_OFFSET 55
#define ANM_OFFSET 72
// optionaler Rest
#define OPTREST_OFFSET ANM_OFFSET+ANM_LEN
#define MAX_LEN	72


#define VIS_INDEX 0
#define NAME_INDEX 1
#define VNAME_INDEX 2
#define WEEKDAY_INDEX 3
#define DATE_INDEX 4
#define ANM_INDEX 5
#define INOUT_INDEX 6
#define TOTALNOWIN_INDEX 7
#define FIRSTIN_INDEX 8
#define OUTTOTAL_INDEX 9
#define LASTOUT_INDEX 10
#define INTOTAL_INDEX 11
#define ZUSATZ_INDEX 12
#define DATENATIVE_INDEX 13
#define OPTREST_INDEX 14



class MemberData:public QObject{
	public:
	MemberData();
	~MemberData();
	bool updateAllMembers(QString *updateResult);
	void calcAllDiffs();
	void removeOlderLogsFromAllMembers(QDate date, bool removeEmptyMembers);
	bool fillModel(QStandardItemModel *model,bool isAuswertung);
	QString	lastRequest;
	QString createHashKey(QDate date, QString name, QString vName, bool useDate);
	MemberIdentifier* searchMember(QString fullName);
	MemberIdentifier* searchMemberFromShortname(QString shortname);

	
	void setLastLogForCurrentUser(MemberLog log);
	MemberLog *getLastLogForCurrentUser();
	
	MemberLog lastLogForCurrentUser;
	

private:
	QString	actResult;
	
	
	
	bool addMember(MemberIdentifier memberIdentifier);
	QString createFullName(QString name, QString vname);
	bool removeMember(QString memberName);
	QString getSubstringFromLine(QString &line, int begin_index, int length);
	QString getReststringFromLine(QString &line, int begin_index);
	QMap<QString, MemberIdentifier> memberIdentifiers; // hash: <Username>
};


#endif
