/********************************************************************************************************
 * PROGRAM      : src
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h19
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : TabWidget.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef TAB_WIDGET_H
#define TAB_WIDGET_H

#include <QTabWidget>
#include <QPushButton>
#include <QListWidget>
#include "EcoSocket.h"
#include "ListWidget.h"
#include <QDate>
#include <QHBoxLayout>
#include "pokerview.h"
#include "sqlworksheet.h"

class TabWidget: public QTabWidget {
	Q_OBJECT
	public:
	TabWidget(QWidget *parent=0);
	~TabWidget();
	QListWidget	*listWidget1;
	
        PokerView* createNewPokerTab();
        SqlWorksheet* createNewSqlWorksheetTab();
        TabPanel* createNewStandardTab();
	void createNewAuswertungTab(QString name, QString vName, QString year, QString month, QString day);

	private:
	QPushButton	*cornerPushButton;
	ListWidget *actRequestForListWidget;
	public slots:
	void on_cornerPushButton_ClickedX();

};




#endif
