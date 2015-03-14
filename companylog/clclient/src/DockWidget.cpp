/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h15
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : DockWidget.cpp
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "DockWidget.h"
#if 0
PictureManager::PictuerManager(QObject *parent) : QObject(parent) {
	Options *options=getGlobalOptionsObj();

	qhttp=new QHttp(options->serverName_2(),options->serverPort_2());
	connect(qhttp,SIGNAL(requestFinished (int,bool)),this,SLOT(requestFinished(int,bool)));

} 
#endif

SidebarInfo::SidebarInfo(QWidget *parent) : QWidget(parent) {
	qPixmapPersonScaled=0;
	pixmapRequester=new PixmapRequester();
	pixmapRequester->setXScaling(140);
	connect(pixmapRequester,SIGNAL(pixmapFound (QString,QPixmap*)),this,SLOT(pixmapFound (QString,QPixmap*)));
}

void SidebarInfo::pixmapFound(QString name, QPixmap *pixmapx) {
    if (name.size()>0) {}
    qPixmapPersonScaled=pixmapx;
	repaint(0,0,width(),height());
}


SidebarInfo::~SidebarInfo() {
	qPixmapPersonScaled=0;
	if (pixmapRequester) delete pixmapRequester;
}


void SidebarInfo::requestPersonPicture(MemberIdentifier *memberIdentifier) {
	Options *options=getGlobalOptionsObj();

	if (memberIdentifier && options) {
		name=memberIdentifier->name;
		vName=memberIdentifier->vName;
		zusatz=memberIdentifier->zusatz;

		pixmapRequester->requestPixmap(memberIdentifier->shortname);
	}
}

void SidebarInfo::paintEvent (QPaintEvent * event ) {
	QPainter painter(this);
	QPixmap *pixmap=0;
	int x=0,y=0,dx=0,dy=0;

    if (event) {}
	pixmap=qPixmapPersonScaled;
	if (pixmap) {
		x=width()/2-pixmap->width()/2;
		y=10;
		dx=pixmap->width();
		dy=pixmap->height();

		QRectF rectangle(x, y, dx, dy);
		painter.drawPixmap(QPoint(x,y),*pixmap);

		QPen pen;  // creates a default pen

 		//pen.setStyle(Qt::DashDotLine);
 		pen.setWidth(3);
 		pen.setBrush(QColor(20,20,20,20));
 		pen.setCapStyle(Qt::RoundCap);
 		pen.setJoinStyle(Qt::RoundJoin);		
		painter.setPen(pen);
		painter.drawRoundRect(rectangle,5,5);


 		//pen.setWidth(3);
		QFontMetrics fm(painter.font());
		QPen pen3;
 		pen3.setWidth(2);
		//pen3.setBrush(QBrush(Qt::RadialGradientPattern));
 		pen3.setColor(QColor(30,30,20,20));
		painter.setPen(pen3);
		//QRectF rectangle2(x-5, y+dy+1, dx+10, fm.height()*2);
		painter.drawLine(x-5,y+dy+3+fm.height()*2,x-5+dx+10,y+dy+3+fm.height()*2);


		QPen pen2;  // creates a default pen
 		//pen.setCapStyle(Qt::RoundCap);
		painter.setPen(pen2);

		/* Text */
		painter.drawText(x+2,dy+y+15,vName+" "+name.trimmed());
		painter.drawText(x+2,dy+y+15+fm.height(),zusatz.trimmed());


	}
}

QSize SidebarInfo::sizeHint () const
{
	return QSize(150,200);
}
void DockWidget::requestPersonPicture(MemberIdentifier *memberIdentifier) {
	panel->requestPersonPicture(memberIdentifier);
}


DockWidget::DockWidget(QWidget *parent) : QDockWidget(parent) {
	panel=new SidebarInfo(this);
	//panel->setMinimumSize(QSize(150,100));
	setWidget(panel);
	
}


DockWidget::~DockWidget() {
}
