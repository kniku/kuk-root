#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QDialog>
#include <QTextEdit>
#include <QTextDocument>
#include <QTextCursor>
#include "Options.h"
#include <QTextDocument>
#include <QTextCursor>
#include <QUrl>
#include "PixmapRequester.h"
#include <QDateTime>
#include <QDesktopWidget>
#include <QRect>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>

class PopupWindow : public QDialog
{
	Q_OBJECT
public:
    PopupWindow(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~PopupWindow();
	void setMessage(QString shortName, QString longName, QString message);
	public slots:
	void pixmapFound(QString name, QPixmap *pixmap);
	void setXYFromOption();
private:
	PixmapRequester	*pixmapRequester;
    QTextEdit *qTextEdit;
	int  lastHideTimer;
	void setUserBitmap(QPixmap *pixmapx);
	void startHideTimer();

	QPushButton	*qPushButtonIcon;
	QPushButton	*qPushButtonClose;
	QLabel		*qTextLabel;
	QLineEdit	*qLineEditMessage;
	QString		qSender;
        QPoint          clickPos;
protected:
	void timerEvent(QTimerEvent *event);
        void mousePressEvent( QMouseEvent * e );
        void mouseMoveEvent ( QMouseEvent * e );
public slots:
	void on_qPushButtonClose_ClickedX();
	void onLineEditMessage_ReturnPressed();

signals:
	void sendMessage(QString to, QString message);

};


#endif // POPUPWINDOW_H
