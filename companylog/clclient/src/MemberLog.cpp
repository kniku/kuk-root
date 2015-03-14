/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h21
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : MemberLog.cpp
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "MemberLog.h"

MemberLog::MemberLog() {
	memset(&date,0,sizeof(QDate));
	memset(&time,0,sizeof(QTime));
	in=false;
	diffIn=false;
}
MemberLog::~MemberLog() {
}

bool MemberLog::isLogFromToday() {
    QDate	now=QDate::currentDate();

    if (now>date) {
        return false;
    }
    return true;
}
bool MemberLog::isIn() {
    return in;
}
QString MemberLog::message() {
    return anm;
}
