/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h20
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : MemberIdentifier.cpp
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "MemberIdentifier.h"

QString MemberIdentifier::createMemberLogKey(QDate date, QTime time,bool in) {
	QString rv=date.toString("yyyyMMdd");
	rv+=time.toString("hhmmss");
	rv+=in?"1":"0";
	return rv;
}
void MemberIdentifier::removeOlderLogsFromMember(QDate date) {
	QMutableMapIterator<QString, MemberLog> it=memberLogs;
	while(it.hasNext()) {
		MemberLog log=it.next().value();
		if (date>log.date) {
			it.remove();	
		}
	}
}

void MemberIdentifier::removeAllLogsButNotLast() {
	//int count=0;
	QMutableMapIterator<QString, MemberLog> it=memberLogs;
	while(it.hasNext()) {
		MemberLog log=it.next().value();
		if (it.hasNext()) {
			it.remove();	
		}
		//count++;
	}
}

MemberLog* MemberIdentifier::getlastLogFromToday() {
	if (haveLogsFromToday()) {
		QMutableMapIterator<QString, MemberLog> it=memberLogs;
		it.toBack();
		if (it.hasPrevious()) {
			return &it.previous().value();
		}
	}
	return 0;
}

bool MemberIdentifier::haveLogsFromToday() {
	QDate	now=QDate::currentDate();

	QMutableMapIterator<QString, MemberLog> it=memberLogs;
	while(it.hasNext()) {
		MemberLog log=it.next().value();
		if (log.date>=now) {
			return true;
		}
	}
	return false;
}


QTime MemberIdentifier::diffTime(QTime a, QTime b) {
	int sec_a;	
	int min_a;	
	int std_a;	

	int seconds_a;
	int seconds_b;

	seconds_a=a.second()+(a.minute()*60)+(a.hour()*60*60);
	seconds_b=b.second()+(b.minute()*60)+(b.hour()*60*60);

	seconds_a=seconds_a-seconds_b;
	min_a=seconds_a/60;
	sec_a=seconds_a%60;
	std_a=min_a/60;
	min_a=(min_a%60);	
	
	return QTime(std_a,min_a,sec_a);
}


void MemberIdentifier::calcDiffs() {
	QMutableMapIterator<QString, MemberLog> it=memberLogs;
	int	i=0;
	QTime	now=QTime::currentTime();	
	QDate	dateNow=QDate::currentDate();	
	
	//now.setHMS(16,00,00);// DEBUG only!!!!!

	QTime	tmp;

	totalIn.setHMS(0,0,0);	
	totalOut.setHMS(0,0,0);	
	lastOut.setHMS(0,0,0);	
	firstIn.setHMS(0,0,0);
	totalNowIn.setHMS(0,0,0);	
	while(it.hasNext()) {
		MemberLog &log=it.next().value();
		
		log.diff.setHMS(0,0,0);
		
		if (log.in && firstIn.second()==0 && firstIn.minute()==0 && firstIn.hour()==0) {
			firstIn=log.time;
		}
		if (!log.in) {
			lastOut=log.time;
		}
		
		if (log.in) {
			if (log.date==dateNow) {
				totalNowIn=diffTime(now,log.time);
			}
		} else {
			totalNowIn.setHMS(0,0,0);	
		}
		
		if (!it.hasNext()) {
		} else {
			MemberLog &log_next=it.peekNext().value();
			log.diff=diffTime(log_next.time,log.time);
			log.diffIn=log.in;
			if (log.diffIn) {
				totalIn=totalIn.addSecs(log.diff.second()+(log.diff.minute()*60)+(log.diff.hour()*60*60));
			} else {
				totalOut=totalOut.addSecs(log.diff.second()+(log.diff.minute()*60)+(log.diff.hour()*60*60));
			}
		}
		
		//log.diff=now;
		i++;
	}
	totalNowIn=totalNowIn.addSecs(totalIn.second()+(totalIn.minute()*60)+(totalIn.hour()*60*60));
	
	
}

bool MemberIdentifier::addLogToMember(QDate date, QTime time, bool in, QString anm, QString _zusatz, QString _shortname, QString _optrest) {
	MemberLog memberLog;
	memberLog.date=date;
	memberLog.time=time;
	memberLog.in=in;
	memberLog.anm=anm;
	memberLog.zusatz=_zusatz;
	memberLog.shortname=_shortname;
	//memberLog.optrest=optrest;
	
	shortname=_shortname;
	optrest=_optrest;
	zusatz=_zusatz;

	QString newKey=createMemberLogKey(date, time,in);

	if (!memberLogs.contains(newKey)) {
		memberLogs.insert(newKey,memberLog);
		return true;
	} else {
		// log existiert bereits ... mh, eventuell in der gleichen Sekunde entstanden?
		// TODO: überschreiben!
		return true;
	}
}


MemberIdentifier::MemberIdentifier() {
}
MemberIdentifier::~MemberIdentifier() {
}
