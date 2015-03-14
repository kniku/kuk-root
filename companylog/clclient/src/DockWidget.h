/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h15
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : DockWidget.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef DOCK_WIDGET_H
#define DOCK_WIDGET_H

#include <QDockWidget>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QIODevice>
#include <QFile>
#include <QPainter>
#include "Options.h"
#include "MemberIdentifier.h"
#include "PixmapRequester.h"


class SidebarInfo: public QWidget {
	Q_OBJECT
	public:
	QPixmap *qPixmapPersonScaled;
	QString name;
	QString vName;
	QString zusatz;
	QString shortName;

	SidebarInfo(QWidget *parent=0);
	~SidebarInfo();
	void requestPersonPicture(MemberIdentifier *memberIdentifier);
	QSize sizeHint () const;
	public slots:
		void pixmapFound(QString name, QPixmap *pixmap);
	protected:
	PixmapRequester	*pixmapRequester;
	void paintEvent (QPaintEvent * event );
}
;

class DockWidget: public QDockWidget {
	Q_OBJECT
	public:
	DockWidget(QWidget *parent=0);
	~DockWidget();
	SidebarInfo *panel;
	void requestPersonPicture(MemberIdentifier *memberIdentifier);
	protected:
};


#endif
