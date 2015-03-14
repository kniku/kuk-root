/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h20
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : MemberIdentifier.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef MEMBER_IDENTIFIER_H
#define MEMBER_IDENTIFIER_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QMap>

#include "MemberLog.h"

class MemberIdentifier {
	public:
	MemberIdentifier();
	~MemberIdentifier();
	QString fullName;
	QString name;
	QString vName;
	QString shortname;
	QString	zusatz;
	QString	optrest;
	QTime	totalIn;
	QTime	totalOut;
	QTime	firstIn;
	QTime	lastOut;
	QTime	totalNowIn;
	QString	hashKey;
	bool addLogToMember(QDate date, QTime time, bool in, QString anm, QString _zusatz, QString shortname, QString _optrest);
	void removeOlderLogsFromMember(QDate date);
	void removeAllLogsButNotLast();
	MemberLog* getlastLogFromToday();
	bool haveLogsFromToday();
	void calcDiffs();
	QTime diffTime(QTime a, QTime b);	

	QString createMemberLogKey(QDate date, QTime time,bool in);
	QMap<QString, MemberLog> memberLogs; // hash: 01022006121212
};



#endif
