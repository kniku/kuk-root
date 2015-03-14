#include "popupwindow.h"

extern EmoticonsRec emoticons[];

void PopupWindow::pixmapFound(QString name, QPixmap *pixmapx) {
    if (name.size()) {}

    if (qTextEdit && pixmapx) {
#if 0
        QTextDocument *document = qTextEdit->document();
        document->addResource(QTextDocument::ImageResource, QUrl(name), *pixmapx);
        QTextCursor cursor = qTextEdit->textCursor();
        cursor.setPosition(0);
        cursor.insertText(" ");// damit das zuvor noch nicht vorhanden Bild neu gezeichnet wird
#endif
        setUserBitmap(pixmapx);
        //this-> setBackgroundRole();
    }
}

void PopupWindow::startHideTimer() {
	Options *options;
	options=getGlobalOptionsObj();

	if (lastHideTimer!=-1) {
		killTimer(lastHideTimer);
        lastHideTimer=-1;
	}
	if (options->notifierTimer()>0) {
		lastHideTimer=startTimer(1000*options->notifierTimer());
	} else {
		lastHideTimer=startTimer(1000*8);
	}
}

void PopupWindow::timerEvent(QTimerEvent *event) {
	if (event->timerId()==lastHideTimer) {
		killTimer(event->timerId());
		lastHideTimer=-1;
        on_qPushButtonClose_ClickedX();
	}
}

void PopupWindow::mousePressEvent( QMouseEvent *e )
{
    if(isTopLevel())
        clickPos = e->pos() + QPoint(geometry().topLeft() - frameGeometry().topLeft());
}

void PopupWindow::mouseMoveEvent( QMouseEvent *e )
{
    if(isTopLevel()) {
        Options *options;
        options=getGlobalOptionsObj();

        move( e->globalPos() - clickPos );
        options->setNotifierX(geometry().x());
        options->setNotifierY(geometry().y());
    }
}


#define USER_BITMAP_WIDTH 30
#define NOTIFIER_WIDHT 320
#define NOTIFIER_HEIGHT 80

void PopupWindow::setUserBitmap(QPixmap *pixmapx) {
	if (qPushButtonIcon && pixmapx) {
		qPushButtonIcon->setIcon(*pixmapx);
		qPushButtonIcon->setMaximumSize(USER_BITMAP_WIDTH,pixmapx->height());
        qPushButtonIcon->setMinimumSize(USER_BITMAP_WIDTH,pixmapx->height());
        qPushButtonIcon->setGeometry(8,0,USER_BITMAP_WIDTH,pixmapx->height());
		qPushButtonIcon->setIconSize(QSize(USER_BITMAP_WIDTH,pixmapx->height()));
	}
}

void PopupWindow::on_qPushButtonClose_ClickedX() {
    qLineEditMessage->setText("");
    this->hide();
}

void PopupWindow::setXYFromOption() {
	Options *options=getGlobalOptionsObj();
	int xx;
	int yy;
    int first_set=0;
	if (options) {
		QRect xScreenRect=QApplication::desktop()->screenGeometry();
        if (options->notifierX()!=-1) {
			xx=options->notifierX();
			if (xx>xScreenRect.width()-NOTIFIER_WIDHT) {
				xx=xScreenRect.width()-NOTIFIER_WIDHT;
			}
        } else {
            xx=xScreenRect.width()-(NOTIFIER_WIDHT+60);
            first_set=1;
        }
        if (options->notifierY()!=-1) {
			yy=options->notifierY();
			if (yy>xScreenRect.height()-NOTIFIER_HEIGHT) {
				yy=xScreenRect.height()-NOTIFIER_HEIGHT;
			}
		} else {
			yy=xScreenRect.height()-(NOTIFIER_HEIGHT+75);
            first_set=1;
        }
		this->setGeometry(xx,yy,NOTIFIER_WIDHT,NOTIFIER_HEIGHT);
        if (first_set) {
            // notifier Position wurde noch nie verschoben, u. wurde per default in die rechte untere ecke verschoben -> fuer spaeter in den Optionen speichern
            options->setNotifierX(geometry().x());
            options->setNotifierY(geometry().y());
        }
    }
}


PopupWindow::PopupWindow(QWidget * parent, Qt::WindowFlags f) : QDialog(parent,f)
{
    Options *options=getGlobalOptionsObj();
	lastHideTimer=-1;
	pixmapRequester=new PixmapRequester();
	pixmapRequester->setXScaling(USER_BITMAP_WIDTH);
	connect(pixmapRequester,SIGNAL(pixmapFound (QString,QPixmap*)),this,SLOT(pixmapFound (QString,QPixmap*)));

    qTextEdit=new QTextEdit("",this);
	qTextEdit->setReadOnly(true);

	qPushButtonIcon=new QPushButton(this);
	qPushButtonIcon->setFlat(true);
	qPushButtonIcon->setContentsMargins(0,0,0,0);
	qPushButtonIcon->setMaximumSize(USER_BITMAP_WIDTH,60);
	qPushButtonIcon->setGeometry(0,0,USER_BITMAP_WIDTH,60);
	qPushButtonIcon->setIconSize(QSize(30,60));

	qPushButtonClose=new QPushButton(this);
	qPushButtonClose->setFlat(true);
	qPushButtonClose->setMaximumSize(12,12);
	qPushButtonClose->setGeometry(0,0,12,12);
	qPushButtonClose->setIconSize(QSize(12,12));
	qPushButtonClose->setContentsMargins(0,0,0,0);
    qPushButtonClose->setIcon(*options->pixmapCacheIcons()->getIcon("messagebox_critical.png"));
	connect(qPushButtonClose,SIGNAL(clicked()),this,SLOT(on_qPushButtonClose_ClickedX()));

	qLineEditMessage=new QLineEdit(this);
	qLineEditMessage->setMaxLength(1000);
	connect(qLineEditMessage,SIGNAL(returnPressed()),this,SLOT(onLineEditMessage_ReturnPressed()));

	qTextLabel=new QLabel(this);
	QFont xfont("Arial", 9, QFont::Bold);
	qTextLabel->setFont(xfont);
	
	setXYFromOption();
	QVBoxLayout *layoutV = new QVBoxLayout;
	QVBoxLayout *layoutV2 = new QVBoxLayout;
	QHBoxLayout *layoutH = new QHBoxLayout;
	QHBoxLayout *layoutH2 = new QHBoxLayout;
	layoutV->addLayout(layoutH);
	layoutV->addWidget(qLineEditMessage);
	layoutV->setContentsMargins(8,3,8,5);
	
	layoutH->addWidget(qPushButtonIcon);
	layoutH->addLayout(layoutV2);

	layoutV2->addLayout(layoutH2);
	layoutH2->addWidget(qTextLabel);
	layoutH2->addWidget(qPushButtonClose);
	layoutV2->addWidget(qTextEdit);
	layoutV2->setContentsMargins(0,0,0,2);
	this->setLayout(layoutV);
	this->setWindowOpacity(0.95);
	//this->setStyleSheet("QTextEdit { background-color: rgb(213, 228, 255); border: 2px solid rgb(190, 200, 255); border-radius: 5px;}");
	//this->setStyleSheet("QTextEdit { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #D7DFFF, stop: 1 #6CA0FF); border: 2px solid rgb(190, 200, 255); border-radius: 5px;}");
	this->setStyleSheet("QLabel { color: rgb(220,230,235)} "
		                "PopupWindow { background-color: "
							"qlineargradient(x1: 1, y1: 0, x2: 0, y2: 0, stop: 0 #133145, stop: 1 #327DAF); "
							"border: 2px solid rgb(15, 30, 35); border-radius: 3px;} "
						"QTextEdit { background-color: "
							"qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #C0C0C0, stop: 1 #FFFFFF); "
							"border: 2px solid rgb(190, 200, 255); border-radius: 5px;"
						"}"
						"QLineEdit { border: 2px solid rgb(190, 200, 255); border-radius: 3px; }"
						);
	
	
}

void PopupWindow::onLineEditMessage_ReturnPressed() {
	 if (qLineEditMessage->text().size()>0 && qSender.size()>0) {
		emit sendMessage(qSender,qLineEditMessage->text());
	 }
	 this->on_qPushButtonClose_ClickedX();
}

PopupWindow::~PopupWindow() {
    if (qTextEdit) {
        delete qTextEdit;
        qTextEdit=0;
    }
	if (pixmapRequester) delete pixmapRequester;
	
}

void PopupWindow::setMessage(QString shortName, QString longName, QString message) {
	//int	count=model->rowCount();
	Options *options=getGlobalOptionsObj();
	int height=16;
	int width=16;

	 if (options && qTextEdit) {
		qSender = shortName;
		//qTextEdit->setTextColor(QColor(100,100,130));
		qTextEdit->clear();
		 QTextDocument *document = qTextEdit->document();
        document->addResource(QTextDocument::ImageResource, QUrl("image_info"), options->pixmapCacheIcons()->getPixmap("messagebox_info.png")->scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        document->addResource(QTextDocument::ImageResource, QUrl("image_in"), options->pixmapCacheIcons()->getPixmap("inmessage.png")->scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        document->addResource(QTextDocument::ImageResource, QUrl("image_out"), options->pixmapCacheIcons()->getPixmap("outmessage.png")->scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        document->addResource(QTextDocument::ImageResource, QUrl("emoticon_smile"), options->pixmapCacheEmoticons()->getPixmap("smile.png")->scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        document->addResource(QTextDocument::ImageResource, QUrl("emoticon_wink"), options->pixmapCacheEmoticons()->getPixmap("wink.png")->scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        document->addResource(QTextDocument::ImageResource, QUrl("emoticon_sad"), options->pixmapCacheEmoticons()->getPixmap("sad.png")->scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        document->addResource(QTextDocument::ImageResource, QUrl("emoticon_cry"), options->pixmapCacheEmoticons()->getPixmap("cry.png")->scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        document->addResource(QTextDocument::ImageResource, QUrl("emoticon_omg"), options->pixmapCacheEmoticons()->getPixmap("omg.png")->scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        document->addResource(QTextDocument::ImageResource, QUrl("emoticon_tongue"), options->pixmapCacheEmoticons()->getPixmap("tongue.png")->scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        document->addResource(QTextDocument::ImageResource, QUrl("emoticon_bat"), options->pixmapCacheEmoticons()->getPixmap("bat.png")->scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
		
		//setWindowTitle("Companylog Mitteilung von ["+shortName+"]");
		qTextLabel->setText("Companylog Mitteilung von "+longName);
		 
		QTextCursor cursor = qTextEdit->textCursor();

		cursor.setPosition(0);
		//cursor.insertText("Companylog:\n\n");

        if (shortName=="[Server]") {
            QPixmap* pixmapx=options->pixmapCacheIcons()->getPixmap("companylog.png");
            if (pixmapx) {
                setUserBitmap(pixmapx);
            }
            //cursor.insertImage("image_info");
        } else {
            QPixmap* pixmapx=pixmapRequester->getLoadedPixmap("unknown");
            if (pixmapx) {
                setUserBitmap(pixmapx);
            }
            if (!pixmapRequester->havePixmap(shortName)) {
				pixmapRequester->requestPixmap(shortName);
			} else {
				QPixmap* pixmapx=pixmapRequester->getLoadedPixmap(shortName);
				if (pixmapx) {
					setUserBitmap(pixmapx);
					//document->addResource(QTextDocument::ImageResource, QUrl(shortName), *pixmapx);
				}
			}
			//cursor.insertImage(shortName);
		}
		//QDateTime ts = QDateTime::currentDateTime();
		//cursor.insertText("   ");
		//cursor.insertHtml("<p><strong>      Companylog Mitteilung von "+shortName+" "+":     </strong></p>");
		//cursor.insertHtml("      Nachricht von "+shortName+" "+":     ");
		//cursor.insertHtml("<p><strong> <HR></p></strong>");
		//cursor.insertText("\n");

		int act_start=0;
		int found_index=0;
		int emoticons_found=0;

		do {
			EmoticonsRec *p_emoticons=&emoticons[0];
			emoticons_found=0;
			while(p_emoticons->used) {
				found_index=message.indexOf(p_emoticons->substring,act_start);
				if (found_index>=0) {
					if (act_start<found_index) {
						cursor.insertText(message.mid(act_start,found_index-act_start));
					}
					act_start=found_index;
					act_start+=p_emoticons->substring.size();
					emoticons_found=1;
					cursor.insertImage("emoticon_"+p_emoticons->picture);
					break;
				}
				p_emoticons++;
			}
			if (!emoticons_found) {
				cursor.insertText(message.mid(act_start,message.size()));
			}
		} while (act_start+1<message.size() && emoticons_found==1);
		cursor.insertText("\n");
		if (options->notifierTimer()>0) {
			startHideTimer();
		}
	}
}
