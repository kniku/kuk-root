#ifndef USER_STATUS_WIDGET_H
#define USER_STATUS_WIDGET_H

#include <QWidget>
#include "MemberLog.h"
#include <QPixmap>
#include <QIcon>
#include <QPainter>

class UserStatusWidget:public QWidget {
	Q_OBJECT
	public:
	UserStatusWidget(QWidget *parent=0);
	~UserStatusWidget();
	void setStatusFromMemberLog(MemberLog *memberLog);
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	private:
	QIcon *act_icon;
	protected:
	void paintEvent (QPaintEvent * event );
};





#endif
