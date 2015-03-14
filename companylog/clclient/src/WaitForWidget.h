/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Freitag 01 Feb 2008 - 10h15
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : WaitForWidget.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef WAITFOR_WIDGET_H
#define WAITFOR_WIDGET_H

#include <QDockWidget>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTreeView>
#include <QStandardItemModel>
#include "Options.h"
#include "MemberLog.h"

class WaitInfo: public QWidget {
	Q_OBJECT
	public:
	WaitInfo(QWidget *parent=0);
	~WaitInfo();
	void addPersonToEditfield(QString shortName);
	void addPerson(QString shortName);
	void removePerson(QString shortName);
	bool isPersonExists(QString shortName);
	void setMemberLog(QString shortName, MemberLog *memberLog);
	int getPersonCount();
	QString getPersonAtIndex(int index);
	QSize sizeHint () const;
	public slots:
	void qPushButtonNameOk_Clicked(bool checked=false);
	void qPushButtonNameDel_Clicked(bool checked=false);
	void onLineClicked( const QModelIndex & index);
	void onLineEditName_ReturnPressed();

	protected:
	//void paintEvent (QPaintEvent * event );
	QLabel 		*qLabelName;
	QLineEdit	*qLineEditName;
	QPushButton *qPushButtonNameOk;
	QPushButton *qPushButtonNameDel;
	QStandardItemModel *model;
	QTreeView	*qTreeView;

	int getPersonIndex(QString shortName);
signals:
	void onPersonAdded(QString shortName);
};



class WaitForWidget: public QDockWidget {
	Q_OBJECT
	public:
	void addPersonToEditfield(QString shortName);
	int getPersonCount();
	void removePerson(QString shortName);
	QString getPersonAtIndex(int index);
	void setMemberLog(QString shortName, MemberLog *memberLog);

	WaitForWidget(QWidget *parent=0);
	~WaitForWidget();
	WaitInfo *panel;
	//void requestPersonPicture(QString shortName);
public slots:
	void slot_onPersonAdded(QString);
signals:
	void onPersonAdded(QString shortName);
};


#endif
