#include "UserStatusWidget.h"


UserStatusWidget::UserStatusWidget(QWidget *parent) : QWidget(parent) {
	act_icon=0;
	//setGeometry(0,0,22,22);
	//show();
}
UserStatusWidget::~UserStatusWidget() {
}

void UserStatusWidget::setStatusFromMemberLog(MemberLog *memberLog) {
/*	QIcon *new_icon;
	if (memberLog) {
		new_icon=memberLog->iconFromMemberStatus(true);
		if (new_icon!=act_icon) {
			act_icon=new_icon;
			update();
		}
        }*/
}

void UserStatusWidget::paintEvent (QPaintEvent * event ) {
	QPainter painter(this);
	if(act_icon) {
		QPixmap _pixmap = act_icon->pixmap(QSize(22, 22));
		painter.drawPixmap(QPoint(0,0),_pixmap);
	}
}
QSize UserStatusWidget::minimumSizeHint() const
{
	return QSize(10, 10);
}

QSize UserStatusWidget::sizeHint() const
{
	return QSize(22, 22);
}

