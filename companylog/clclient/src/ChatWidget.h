/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Freitag 01 Feb 2008 - 10h15
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : ChatWidget.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef CHAT_WIDGET_H
#define CHAT_WIDGET_H

#include <QDockWidget>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QStandardItemModel>
#include "Options.h"
#include "MemberLog.h"
#include <QTextDocument>
#include <QTextCursor>
#include <QUrl>
#include "PixmapRequester.h"





class ChatInfo: public QWidget {
	Q_OBJECT
	public:
	ChatInfo(QWidget *parent=0);
	~ChatInfo();
	void addPersonToEditfield(QString shortName);
	void setMessage(bool incoming, QString shortName, QString longName, QString message, bool doPopup);
	int getMessageCount();
	void insertImageResources();

	QSize sizeHint () const;
	public slots:
	void qPushButtonNameOk_Clicked(bool checked=false);
	void qPushButtonNameDel_Clicked(bool checked=false);
	void pixmapFound(QString name, QPixmap *pixmap);
#if 0
	void onLineClicked( const QModelIndex & index);
#endif
	void onLineEditName_ReturnPressed();
	void onLineEditMessage_ReturnPressed();

	protected:
	//void paintEvent (QPaintEvent * event );
	QLabel 		*qLabelName;
	QLabel 		*qLabelMessage;
	QLineEdit	*qLineEditName;
	QLineEdit	*qLineEditMessage;
	QPushButton *qPushButtonNameOk;
	QPushButton *qPushButtonNameDel;
	//QStandardItemModel *model;
	QTextEdit	*qTextEdit;
	PixmapRequester	*pixmapRequester;
	QPixmap *qPixmapPersonScaled;

  signals:
	void sendMessage(QString to, QString message);
};



class ChatWidget: public QDockWidget {
	Q_OBJECT
	public:
	void addPersonToEditfield(QString shortName);
	int getMessageCount();
	void setMessage(bool incoming, QString shortName, QString longName, QString message, bool doPopup);

	ChatWidget(QWidget *parent=0);
	~ChatWidget();
	ChatInfo *panel;
	//void requestPersonPicture(QString shortName);
  public slots:
	void slot_onSendMessage(QString to, QString message);
  signals:
	void sendMessage(QString to, QString message);
//signals:
	//void onPersonAdded(QString shortName);
};


#endif
