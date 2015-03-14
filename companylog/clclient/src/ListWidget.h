/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h20
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : ListWidget.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef LIST_WIDGET_H
#define LIST_WIDGET_H

#include <QTabWidget>
#include <QPushButton>
#include <QTableView>
#include "EcoSocket.h"
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QTableView>
#include <QTreeView>
#include <QStandardItemModel>

#include "RequestObject.h"
#include "MemberData.h"
#include <QTimer>
#include "Options.h"
#include <QTabWidget>
#include <QLabel>
#include <QHeaderView>
#include <QSortFilterProxyModel>

class LabelLineEdit: public QWidget {
	Q_OBJECT
	
	QLabel *label;
	QHBoxLayout *hbox;
	QLineEdit *lineEdit;
	public:
	QLineEdit* lineEditPointer();
	LabelLineEdit(QString _label,QWidget *parent=0);
	~LabelLineEdit();
};

class MySortFilterProxyModel: public QSortFilterProxyModel {
    Q_OBJECT
public:
    MySortFilterProxyModel(QObject *parent=0);
protected:
    bool lessThan ( const QModelIndex & left, const QModelIndex & right ) const;
};

class ListWidget: public QTreeView {
	Q_OBJECT
	public:
	ListWidget(QWidget *parent);
	~ListWidget();
	RequestObject 	*requestObject; /* get the server data */
	MemberData		*memberData;	/* list of members with logs */
	QTabWidget	*myTabWidget;
	QLineEdit *lineEditPointer;
	QString		lastSearchString;

    int getVisualMainRow(int modelRow);
    int getModelMainRowFromVisualRow(int visualRow);
    bool updateData(char *requestString);
	bool fillList();
	bool isAuswertung;
	void setupModel();	
	QString getLastRequest();
	//int	lastTimerId;
	void setTabLabel(QString label);
	QString buildTabLabel(QTime _time, QString postfix);

    QStandardItemModel *model;
    MySortFilterProxyModel *proxyModel;
    //QAbstractItemView *pieChart;
    QItemSelectionModel *selectionModel;
	void startRefreshTimer();
	QTimer *timer;

	public slots:
	void onRequestFinish();

	void onLineClicked( const QModelIndex & index);
	void on_filterLineEdit_Leave();
	void on_timerFinish();

	protected:
	//void timerEvent(QTimerEvent *event);
	QString	lastRequest;
	QTime	lastRequest_ca;
	char	lastRequestString[256+1];	/* last Request to server */
signals:
	void onMemberSelected(MemberIdentifier* memberIdentifierExist);
	void onCurrentMemberStateUpdated(MemberLog *);
	void onCurrentListRefreshed();
};

class TabPanel: public QWidget {
	Q_OBJECT
	public:
	TabPanel(QWidget *parent=0);
	ListWidget	*listWidget;
};



#endif
