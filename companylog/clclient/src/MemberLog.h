/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h21
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : MemberLog.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef MEMBER_LOG_H
#define MEMBER_LOG_H

#include <QString>
#include <QDate>
#include <QTime>
#include "Options.h"

typedef enum {
	memberStatusNhome,
	memberStatusNin,
	memberStatusNout,
	memberStatusNmittag,
	memberStatusNbinbei,
	memberStatusNrauch,
	memberStatusNklbesprechung,
	memberStatusNbesprechung,
	memberStatusNwartung,
	memberStatusNjourfix,
	memberStatusNwww
}MemberStatus;

class MemberLog {
	public:
	MemberLog();
	~MemberLog();
        bool isLogFromToday();
        bool isIn();
        QString message();

	QDate	date;
	QTime	time;
	bool	in;
	QString	anm;
	QString zusatz;
    QString shortname;

	QTime	diff;
	bool	diffIn;
	private:
};



#endif
