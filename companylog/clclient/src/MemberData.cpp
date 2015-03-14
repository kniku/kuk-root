/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h20
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : MemberData.cpp
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#include <MemberData.h>

MemberData::MemberData() {
}
MemberData::~MemberData() {

}


bool MemberData::addMember(MemberIdentifier memberIdentifier) {
	if (!memberIdentifiers.contains(memberIdentifier.hashKey)) {
		memberIdentifiers.insert(memberIdentifier.hashKey,memberIdentifier);
		return true;
	} else {
		return true;
	}
}

void MemberData::removeOlderLogsFromAllMembers(QDate date, bool removeEmptyMembers) {
	Options *options;
	options=getGlobalOptionsObj();
	QMutableMapIterator<QString, MemberIdentifier> it=memberIdentifiers;
	while(it.hasNext()) {
		it.next();
		MemberIdentifier &memberIdentifier=(MemberIdentifier&)it.value();
		if (memberIdentifier.haveLogsFromToday() || options->inaktive()) {
			/* alle Logs vom Vortag/Vergangenheit entfernen */
			memberIdentifier.removeOlderLogsFromMember(date);
		} else {
			/* alle Logs vom Vortag entfernen, jedoch nicht den letzten (neuesten) Logeintrag */
			memberIdentifier.removeAllLogsButNotLast();
		}
		if (removeEmptyMembers && memberIdentifier.memberLogs.size()<1) {
			// remove the member itself
			it.remove();
		}
	}
}
void MemberData::calcAllDiffs() {
	QMutableMapIterator<QString, MemberIdentifier> it=memberIdentifiers;
	while(it.hasNext()) {
		it.next();
		MemberIdentifier &memberIdentifier=(MemberIdentifier&)it.value();
		memberIdentifier.calcDiffs();
	}
}

void MemberData::setLastLogForCurrentUser(MemberLog log) {
	lastLogForCurrentUser=log;
}

MemberLog* MemberData::getLastLogForCurrentUser() {
	return &lastLogForCurrentUser;
}


bool MemberData::fillModel(QStandardItemModel *model, bool isAuswertung) {
	Options *options;
	options=getGlobalOptionsObj();
	if (!model) return true;
	
	model->clear();
    model->setColumnCount(15);
	//model->setHorizontalHeaderLabels(QStringList()<<"ABC"<<"xyz");
	model->setHeaderData(VIS_INDEX, Qt::Horizontal, tr(""));
	model->setHeaderData(NAME_INDEX, Qt::Horizontal, tr("Name"));
	model->setHeaderData(VNAME_INDEX, Qt::Horizontal, tr("Vorname"));
    model->setHeaderData(WEEKDAY_INDEX, Qt::Horizontal, tr("Tag"));
    model->setHeaderData(DATE_INDEX, Qt::Horizontal, tr("Datum"));
	model->setHeaderData(INOUT_INDEX, Qt::Horizontal, tr("In/Out"));
	model->setHeaderData(ANM_INDEX, Qt::Horizontal, tr("Anmerkung"));
	model->setHeaderData(TOTALNOWIN_INDEX, Qt::Horizontal, tr("Gesamt AZ"));
	model->setHeaderData(FIRSTIN_INDEX, Qt::Horizontal, tr("Arbeitsbeginn"));
	model->setHeaderData(OUTTOTAL_INDEX, Qt::Horizontal, tr("Pause"));
	model->setHeaderData(LASTOUT_INDEX, Qt::Horizontal, tr("Arbeitsende"));
	model->setHeaderData(INTOTAL_INDEX, Qt::Horizontal, tr("IN"));
	model->setHeaderData(ZUSATZ_INDEX, Qt::Horizontal, tr("S-Name/Durchwahl"));
	model->setHeaderData(DATENATIVE_INDEX, Qt::Horizontal, tr("Datum"));
	model->setHeaderData(OPTREST_INDEX, Qt::Horizontal, tr("Zusatz"));

	//model->setHeaderData(9, Qt::Horizontal, tr("EMPTY"));


	QMutableMapIterator<QString, MemberIdentifier> it=memberIdentifiers;
	int count=0;


	QColor cBack(247,242,247);
	QColor cRed(164,0,38);
	QColor cGreen(2,92,17);
	QColor cRedHome(184,80,80);
	QColor cGreenHome(80,120,80);
	QColor cBlue(10,12,230);
	QColor cLightYellow(255,255,225);
	QColor cDarkYellow(255,255,198);
	QColor cLightBlue(230,245,255);
	QColor cDarkBlue(114,133,155);
	QColor cLightRed(255,0,38);
	QColor cDarkGreen(20,70,30);

	QColor cBLightGreen(230,255,224);
	QColor cBDarkGreen(214,255,188);
	
	QColor cBLightRed(255,240,240);
	QColor cBDarkRed(255,235,225);
	//QColor cBDarkRed(255,210,182);
	
	QFont	appFont=QApplication::font();
	if (appFont.bold()) {
		if (appFont.pointSize()>0) {
			appFont.setPointSize(appFont.pointSize()+1);
		}	
	} else {
		appFont.setBold(true);
		if (appFont.pointSize()>0) {
			appFont.setPointSize(appFont.pointSize()+1);
		}	
	}

	while(it.hasNext()) {
		it.next();
		MemberIdentifier &memberIdentifier=(MemberIdentifier&)it.value();
		//fprintf(stderr,"Name: %s\n",memberIdentifier.fullName.toAscii().data());

		model->insertRows (count, 1);
		model->setData(model->index(count,NAME_INDEX),memberIdentifier.name/*,Qt::DisplayRole*/);
		model->setData(model->index(count,VNAME_INDEX),memberIdentifier.vName/*,Qt::DisplayRole*/);
		model->setData(model->index(count,OPTREST_INDEX),memberIdentifier.optrest); /* Restzeile */

		QModelIndex name_parent=model->index(count,VIS_INDEX);
		model->insertColumns (0, 13, name_parent);

		QMutableMapIterator<QString, MemberLog> it2=memberIdentifier.memberLogs;
		int count2=0;

		//QString inOutString="";
		if (!(memberIdentifier.totalIn.second()==0 && memberIdentifier.totalIn.minute()==0 && memberIdentifier.totalIn.hour()==0)) {
			//inOutString+="IN: "+memberIdentifier.totalIn.toString("hh:mm:ss")+" ";
			model->setData(model->index(count,INTOTAL_INDEX),memberIdentifier.totalIn);
		}
		if (!(memberIdentifier.totalOut.second()==0 && memberIdentifier.totalOut.minute()==0 && memberIdentifier.totalOut.hour()==0)) {
			//inOutString+="PAUSE: "+memberIdentifier.totalOut.toString("hh:mm:ss");
			model->setData(model->index(count,OUTTOTAL_INDEX),memberIdentifier.totalOut);
		}
		//if (inOutString.size()>0) {
			//model->setData(model->index(count,INOUTTOTAL_INDEX),inOutString);
		//}


		while(it2.hasNext()) {
			MemberLog log=it2.next().value();
			//fprintf(stderr,"Log: %s\n",log.date.toString("yyMMdd").toAscii().data());
			//fprintf(stderr,"Log: %s\n",it2.key().toAscii().data());
			model->insertRow (/*count2*/0, name_parent);
			//DATUM!! model->setData(model->index(/*count2*/0,2,name_parent),log.date);
			if (isAuswertung) {
                model->setData(model->index(count,DATE_INDEX),log.date.toString("dd.MM.yyyy"));
                model->setData(model->index(count,WEEKDAY_INDEX),log.date.toString("ddd"));
            } else {
				model->setData(model->index(count,DATE_INDEX),log.date.toString("ddd dd.MM"));
                model->setData(model->index(count,WEEKDAY_INDEX),log.date.toString("ddd"));
            }
			model->setData(model->index(count,DATENATIVE_INDEX),log.date /*log.date.toString("dd.MM.yyyy")*/);
			model->setData(model->index(count,ANM_INDEX),log.anm);

			if (!it2.hasNext()) {
#if 0
				if (count%2) {
					/* Gerade Zeilen 2. usw */
					for (int x=0;x<14;x++) {
						if (x==TOTALNOWIN_INDEX) {
							model->setData(model->index(count,TOTALNOWIN_INDEX),cBDarkGreen,Qt::BackgroundColorRole);
						}/* else if (x==FIRSTIN_INDEX) {
							model->setData(model->index(count,FIRSTIN_INDEX),cDarkYellow,Qt::BackgroundColorRole);
						} else if (x==LASTOUT_INDEX) {
							model->setData(model->index(count,LASTOUT_INDEX),cDarkYellow,Qt::BackgroundColorRole);
						} else if (x==OUTTOTAL_INDEX) {
							model->setData(model->index(count,OUTTOTAL_INDEX),cBDarkRed,Qt::BackgroundColorRole);
						}*/ else {
							model->setData(model->index(count,x),cBack,Qt::BackgroundColorRole);
						}
					}
				} else {
					for (int x=0;x<14;x++) {
						if (x==TOTALNOWIN_INDEX) {
							model->setData(model->index(count,TOTALNOWIN_INDEX),cBLightGreen,Qt::BackgroundColorRole);
						}/* else if (x==FIRSTIN_INDEX) {
							model->setData(model->index(count,FIRSTIN_INDEX),cLightYellow,Qt::BackgroundColorRole);
						} else if (x==LASTOUT_INDEX) {
							model->setData(model->index(count,LASTOUT_INDEX),cLightYellow,Qt::BackgroundColorRole);
						} else if (x==OUTTOTAL_INDEX) {
							model->setData(model->index(count,OUTTOTAL_INDEX),cBLightRed,Qt::BackgroundColorRole);
						} else {
						
						}*/
					}
				}
#endif
                for (int x=0;x<15;x++) {
					if (x==INTOTAL_INDEX) {
						//model->setData(model->index(count,x),cDarkGreen,Qt::TextColorRole);
					} else if (x==OUTTOTAL_INDEX) {
						model->setData(model->index(count,x),cLightRed,Qt::TextColorRole);
					} else if (x==FIRSTIN_INDEX || x==LASTOUT_INDEX) {
					} else if (x==TOTALNOWIN_INDEX) {
							model->setData(model->index(count,x),cGreen,Qt::TextColorRole);
					} else {
						if (log.in) {
                                                        if (!log.isLogFromToday()) {
								model->setData(model->index(count,x),cGreenHome,Qt::TextColorRole);
							} else {
								//model->setData(model->index(count,x),cGreen,Qt::TextColorRole);
							}
						} else {
                                                        if (!log.isLogFromToday()) {
								model->setData(model->index(count,x),cRedHome,Qt::TextColorRole);
							} else {
								model->setData(model->index(count,x),cRed,Qt::TextColorRole);
							}
						}
					}
				}
				/* Gesamtarbeitszeit */
				if (!(memberIdentifier.totalNowIn.second()==0 && memberIdentifier.totalNowIn.minute()==0 && memberIdentifier.totalNowIn.hour()==0)) {
					model->setData(model->index(count,TOTALNOWIN_INDEX),memberIdentifier.totalNowIn);
				}
				/* Arbeitsbeginn */
				if (!(memberIdentifier.firstIn.second()==0 && memberIdentifier.firstIn.minute()==0 && memberIdentifier.firstIn.hour()==0)) {
					model->setData(model->index(count,FIRSTIN_INDEX),memberIdentifier.firstIn);
					//model->setData(model->index(count,FIRSTIN_INDEX),cDarkGreen,Qt::TextColorRole);
				}
				/* Arbeitsende */
				if (!(memberIdentifier.lastOut.second()==0 && memberIdentifier.lastOut.minute()==0 && memberIdentifier.lastOut.hour()==0)) {
					model->setData(model->index(count,LASTOUT_INDEX),memberIdentifier.lastOut);
					model->setData(model->index(count,LASTOUT_INDEX),cLightRed,Qt::TextColorRole);
				}
				
				
				/* icon setzen */
                QIcon* icon=options->logManager()->getIconFromLog(log.message(), log.isIn());
                if (icon) {
                    model->setData(model->index(count,VIS_INDEX),*icon,Qt::DecorationRole);
                }
				model->setData(model->index(count,ZUSATZ_INDEX),log.zusatz);

				if(!isAuswertung && log.shortname==options->getUsername()) {
					model->setData(model->index(count,VNAME_INDEX),appFont,Qt::FontRole);
					model->setData(model->index(count,NAME_INDEX),appFont,Qt::FontRole);

					model->setData(model->index(count,DATE_INDEX),appFont,Qt::FontRole);
                    model->setData(model->index(count,WEEKDAY_INDEX),appFont,Qt::FontRole);
                    model->setData(model->index(count,INOUT_INDEX),appFont,Qt::FontRole);
					model->setData(model->index(count,ANM_INDEX),appFont,Qt::FontRole);
					model->setData(model->index(count,TOTALNOWIN_INDEX),appFont,Qt::FontRole);
					model->setData(model->index(count,FIRSTIN_INDEX),appFont,Qt::FontRole);
					model->setData(model->index(count,OUTTOTAL_INDEX),appFont,Qt::FontRole);
					model->setData(model->index(count,LASTOUT_INDEX),appFont,Qt::FontRole);
					model->setData(model->index(count,INTOTAL_INDEX),appFont,Qt::FontRole);
					model->setData(model->index(count,ZUSATZ_INDEX),appFont,Qt::FontRole);
					
					setLastLogForCurrentUser(log);
				}
			}


			if (log.in) {
				model->setData(model->index(/*count2*/0,INOUT_INDEX,name_parent),log.time.toString("hh:mm:ss")+" IN");
				model->setData(model->index(count,INOUT_INDEX),"in seit "+log.time.toString("hh:mm:ss"));
				model->setData(model->index(/*count2*/0,INOUT_INDEX,name_parent),cGreen,Qt::TextColorRole);
				model->setData(model->index(/*count2*/0,ANM_INDEX,name_parent),cGreen,Qt::TextColorRole);
				//model->setData(model->index(/*count2*/0,INOUTTOTAL_INDEX,name_parent),cGreen,Qt::TextColorRole);
			} else {
				model->setData(model->index(/*count2*/0,INOUT_INDEX,name_parent),log.time.toString("hh:mm:ss")+" OUT");
				model->setData(model->index(count,INOUT_INDEX),"out seit "+log.time.toString("hh:mm:ss"));
				model->setData(model->index(/*count2*/0,INOUT_INDEX,name_parent),cRed,Qt::TextColorRole);
				model->setData(model->index(/*count2*/0,ANM_INDEX,name_parent),cRed,Qt::TextColorRole);
				//model->setData(model->index(/*count2*/0,INOUTTOTAL_INDEX,name_parent),cRed,Qt::TextColorRole);
			}	
			model->setData(model->index(/*count2*/0,ANM_INDEX,name_parent),log.anm);

			//model->setData(model->index(/*count2*/0,INOUTTOTAL_INDEX,name_parent),log.diff);
			if (log.diffIn) {
				model->setData(model->index(/*count2*/0,INTOTAL_INDEX,name_parent),log.diff);
				model->setData(model->index(/*count2*/0,INTOTAL_INDEX,name_parent),cGreen,Qt::TextColorRole);
			} else {
				model->setData(model->index(/*count2*/0,OUTTOTAL_INDEX,name_parent),log.diff);
				model->setData(model->index(/*count2*/0,OUTTOTAL_INDEX,name_parent),cRed,Qt::TextColorRole);
			}

		
			count2++;
		}
		count++;
	}
	return true;
	
}
/*	QString rv=date.toString("yyMMdd");
	rv+=time.toString("hhmmss");*/

QString MemberData::getSubstringFromLine(QString &line, int begin_index, int length) {
	QString result="";
	if (line.size()>begin_index) {
		result=line.mid(begin_index,length);
	}
	return result;
}

QString MemberData::getReststringFromLine(QString &line, int begin_index) {
	QString result="";
	if (line.size()>begin_index) {
		result=line.mid(begin_index,line.size());
	}
	return result;
}


QString MemberData::createFullName(QString name, QString vName) {
	return name.trimmed()+" "+vName.trimmed();
}

QString MemberData::createHashKey(QDate date, QString name, QString vName, bool useDate) {
	//fprintf(stderr,"Hash: %s\n",(name+vName+(useDate?date.toString("yyyyMMdd"):"")).toAscii().data());


	return name+vName+(useDate?date.toString("yyyyMMdd"):"");
}

MemberIdentifier* MemberData::searchMember(QString hashKey) {
	MemberIdentifier *memberIdentifierExist;
	if (memberIdentifiers.contains(hashKey)) {
		memberIdentifierExist=&memberIdentifiers[hashKey];
		return memberIdentifierExist;
	}
	return 0;
}

MemberIdentifier* MemberData::searchMemberFromShortname(QString shortname) {
	QMutableMapIterator<QString, MemberIdentifier> it=memberIdentifiers;
	while(it.hasNext()) {
		it.next();
		MemberIdentifier &memberIdentifier=(MemberIdentifier&)it.value();
		if (memberIdentifier.shortname.compare(shortname)==0) {
			return &memberIdentifier;
		}
	}
	return 0;
}

bool MemberData::updateAllMembers(QString *updateResult) {
	bool rv=true;
	QStringList	stringList;
	QString	sDate;
	MemberIdentifier *memberIdentifierExist=NULL;
	Options *options;
	options=getGlobalOptionsObj();
	QString n=options->name();
	QString v=options->vName();
	bool	isAuswertung=false;
	QString	tel="";

	QDate	now=QDate::currentDate();



	if (updateResult->size()>0) {
		//fprintf(stderr,"Parser: Full result String: %s\n",updateResult->toAscii().data());

		stringList=updateResult->split("\n",QString::KeepEmptyParts,Qt::CaseInsensitive);
		for (int i=0;i<stringList.size();i++) {
			if (i==0) {
				fprintf(stderr,"First Line: %s\n", stringList[i].toAscii().data());
				//lastRequest.fromString(stringList[i].right(10),"hhmmss");
				if (stringList[0].right(10)=="----------") {
					/* nothing todo */
					break;
				}
				lastRequest=stringList[i].right(10);
				if (stringList[0].contains("AUA:")) {
					isAuswertung=true;
				}
				continue;
			}

			/*fprintf(stdout,"Zeile:\n");
			fprintf(stdout,stringList[i].toAscii().data());*/
			
			
			MemberIdentifier memberIdentifier;
			memberIdentifier.name=getSubstringFromLine(stringList[i], NAME_OFFSET, NAME_LEN);
			memberIdentifier.vName=getSubstringFromLine(stringList[i], VNAME_OFFSET, VNAME_LEN);


			tel="";

			//fprintf(stderr,"Before: %s\n", memberIdentifier.vName.toAscii().data());

			memberIdentifier.vName=memberIdentifier.vName.simplified();
			if (memberIdentifier.vName.size()>6 && memberIdentifier.vName.endsWith(")")) {
				if (memberIdentifier.vName[memberIdentifier.vName.size()-1-4]=='(') {
					tel=memberIdentifier.vName.mid(memberIdentifier.vName.size()-1-3,3);
					memberIdentifier.vName=memberIdentifier.vName.remove(memberIdentifier.vName.size()-1-4,5);
				} else if (memberIdentifier.vName[memberIdentifier.vName.size()-1-5]=='(') {
					tel=memberIdentifier.vName.mid(memberIdentifier.vName.size()-1-4,4);
					memberIdentifier.vName=memberIdentifier.vName.remove(memberIdentifier.vName.size()-1-5,6);
				}
			}

			//fprintf(stderr,"After: %s\n", memberIdentifier.vName.toAscii().data());

			memberIdentifier.fullName=createFullName(memberIdentifier.name,memberIdentifier.vName);
			sDate=getSubstringFromLine(stringList[i], DATE_OFFSET, DATE_LEN);

			if (isAuswertung) {
				if (n.size()>0) {
					if (!memberIdentifier.name.contains(n,Qt::CaseInsensitive)) {
						continue;
					}
				}
				if (v.size()>0) {
					if (!memberIdentifier.vName.contains(v,Qt::CaseInsensitive)) {
						continue;
					}
				}
			}

			sDate=sDate.mid(0,6)+"20"+sDate.mid(6,2);
			QDate date=QDate::fromString(sDate,"dd-MM-yyyy");



			memberIdentifier.hashKey=createHashKey(date,memberIdentifier.name,memberIdentifier.vName, isAuswertung?true:false);
			//fprintf(stderr,"Parser: Full name: %s in Line %d\n",memberIdentifier.fullName.toAscii().data(),i);
			if (memberIdentifier.fullName.size()>1) {
				memberIdentifierExist=searchMember(memberIdentifier.hashKey);
				if(!memberIdentifierExist) {
					//ok, name not found ...
					
					if (addMember(memberIdentifier)) {
						memberIdentifierExist=searchMember(memberIdentifier.hashKey);
					}
				}
			}
			if (memberIdentifierExist) {
				QString sTime=getSubstringFromLine(stringList[i], TIME_OFFSET, TIME_LEN);
				QString sIn=getSubstringFromLine(stringList[i], STATE_OFFSET, STATE_LEN);
                QString sAnm=getSubstringFromLine(stringList[i], ANM_OFFSET, ANM_LEN);
                sAnm=sAnm.simplified();
                QString sOptrest=getReststringFromLine(stringList[i], OPTREST_OFFSET);
				QString zusatz="";
				QString shortname="";
				

				if (sAnm.size()>=5 && sAnm[0]=='(') {
					int xx;
					for (xx=1;xx<sAnm.size();xx++) {
						if (sAnm[xx]<'a' || sAnm[xx]>'z') {
							break;
						}
					}
					if (xx>1) {
						zusatz=sAnm.mid(1,xx-1);
					}
					shortname=zusatz;
					if (sAnm.size()>xx) {
						sAnm=sAnm.remove(0,xx+1);
					} else {
						sAnm=sAnm.remove(0,xx);
					}
				}
				

				if (sDate.size()>0 && sTime.size()>0 && sIn.size()>0) {
					QTime time=QTime::fromString(sTime,"hh:mm:ss");
					//date.addYears(100);
					//fprintf(stderr,"TIME: %s\n",sTime.toAscii().data());
					bool in=false;
					if (sIn=="IN") {
						in=true;
					}
					if (tel.size()>0 && zusatz.size()>0) {
						zusatz=zusatz+" / ";
					}
					if (tel.size()>0) {
						zusatz=zusatz+"DW: "+tel;
					}
					memberIdentifierExist->addLogToMember(date, time, in, sAnm, zusatz,shortname,sOptrest);
				}
			}
		}
		if (!isAuswertung) {
			removeOlderLogsFromAllMembers(now, true);
		}
		calcAllDiffs();
	} else {
		if (!isAuswertung) {
			removeOlderLogsFromAllMembers(now, true);
		}
	}
	return rv;
}

