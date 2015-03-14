  /********************************************************************************************************
  * PROGRAM      : companylog
  * DATE - TIME  : Freitag 01 Feb 2008 - 10h15
  * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
  * FILENAME     : ChatWidget.cpp
  * LICENSE      : 
  * COMMENTARY   : 
  ********************************************************************************************************/
  #include "ChatWidget.h"

  #define VIS_INDEX 0
  #define SNAME_INDEX 1
  #define ANM_INDEX 2
  #define TS_INDEX 3
  
  #define COLUMN_COUNT 4

EMOTICONS_REC

void ChatInfo::pixmapFound(QString name, QPixmap *pixmapx) {
	qPixmapPersonScaled=pixmapx;

	if (qTextEdit) {
		QTextDocument *document = qTextEdit->document();
		document->addResource(QTextDocument::ImageResource, QUrl(name), *qPixmapPersonScaled);
		QTextCursor cursor = qTextEdit->textCursor();
		cursor.setPosition(0);		
		cursor.insertText(" ");// damit das zuvor noch nicht vorhanden Bild neu gezeichnet wird
		//repaint();
		//ChatWidget	*xparent;
		//repaint(0,0,width(),height());
		/*xparent=qobject_cast<ChatWidget*>(parent());
		if (xparent) {
			xparent->repaint();
		}
		repaint();*/
	}
	//repaint(0,0,width(),height());
}
//QVariant QTextDocument::loadResource ( int type, const QUrl & name )

  ChatInfo::ChatInfo(QWidget *parent) : QWidget(parent) {
	 Options *options=getGlobalOptionsObj();
	 pixmapRequester=new PixmapRequester();
	 pixmapRequester->setXScaling(25);
	 connect(pixmapRequester,SIGNAL(pixmapFound (QString,QPixmap*)),this,SLOT(pixmapFound (QString,QPixmap*)));

	 if (options) {
		  //fprintf(stderr,"Picture Server: %s\n",options->serverName_2().toAscii().data());
		  QHBoxLayout *layout = new QHBoxLayout;
		  QHBoxLayout *layout2 = new QHBoxLayout;
		  QVBoxLayout *layoutV = new QVBoxLayout;
		  qLabelName=new QLabel("An");
		  qLabelName->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
		  qLabelMessage=new QLabel("Text");
		  qLabelMessage->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
		  qLineEditName=new QLineEdit();
		  qLineEditName->setMaxLength(20);
		  qLineEditName->setMaximumWidth(60);
		  qLineEditName->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
		  connect(qLineEditName,SIGNAL(returnPressed()),this,SLOT(onLineEditName_ReturnPressed()));

		  qLineEditMessage=new QLineEdit();
		  qLineEditMessage->setMaxLength(1000);
		  //qLineEditMessage->setMaximumWidth(280);
		  //qLineEditMessage->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
		  connect(qLineEditMessage,SIGNAL(returnPressed()),this,SLOT(onLineEditMessage_ReturnPressed()));

          qPushButtonNameOk = new QPushButton(*options->pixmapCacheIcons()->getIcon("add.png"),"");
		  qPushButtonNameOk->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
		  qPushButtonNameOk->setAutoDefault(true);
		  qPushButtonNameOk->setToolTip(tr("Nachricht senden"));
		  connect(qPushButtonNameOk,SIGNAL(clicked (bool)),this,SLOT(qPushButtonNameOk_Clicked(bool)));

          qPushButtonNameDel = new QPushButton(*options->pixmapCacheIcons()->getIcon("remove.png"),"");
		  qPushButtonNameDel->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
		  qPushButtonNameDel->setToolTip(tr("Nachrichtenliste löschen"));
		  //qPushButtonNameDel->setAutoDefault(true);
		  connect(qPushButtonNameDel,SIGNAL(clicked (bool)),this,SLOT(qPushButtonNameDel_Clicked(bool)));

#if 0
		  model=new QStandardItemModel(0,COLUMN_COUNT);
		  model->setColumnCount(COLUMN_COUNT);
		  model->setHeaderData(VIS_INDEX, Qt::Horizontal, "");
		  model->setHeaderData(SNAME_INDEX, Qt::Horizontal, tr("An/Von"));
		  model->setHeaderData(ANM_INDEX, Qt::Horizontal, tr("Nachricht"));
		  model->setHeaderData(TS_INDEX, Qt::Horizontal, tr("Zeitpunkt"));
#endif
		  qTextEdit=new QTextEdit();		
		  qTextEdit->setReadOnly(true);
		  insertImageResources();

		  //qTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
		  //qTreeView->setEditTriggers(QAbstractItemView::DoubleClicked);
		  //qTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
		  //qTreeView->setModel(model);

		  //connect(qTreeView, SIGNAL(pressed(QModelIndex)), this, SLOT(onLineClicked(QModelIndex)));

		  
		  layout->addWidget(qLabelName);
		  layout->addWidget(qLineEditName);
          //layout->setMargin(1);
		  layout->insertStretch(-1);
          //layout->setContentsMargins(1,1,1,1);
          //layout->addWidget(qPushButtonNameOk);

		  //layout2->addWidget(qLabelMessage);
		  layout2->addWidget(qLineEditMessage);
          //layout2->setMargin(1);
         // layout2->insertStretch(-1);
		  layout2->addWidget(qPushButtonNameOk);
		  layout2->addWidget(qPushButtonNameDel);
          //layout2->setContentsMargins(1,1,1,1);

		  layoutV->addLayout(layout);
		  layoutV->addLayout(layout2);
		  layoutV->addWidget(qTextEdit);
          //layoutV->setContentsMargins(1,1,1,1);
          this->setLayout(layoutV);
	 }
  }

#if 0
  void ChatInfo::onLineClicked( const QModelIndex & index) {
	 int row=index.row();	
	 if (row>=0) {
		  QVariant var=model->data(model->index(row,SNAME_INDEX)/*,Qt::DisplayRole*/);
		  if (var.toString()!="[Server]") {
			 addPersonToEditfield(var.toString());
		  }
	 }
  }
#endif

  void ChatInfo::insertImageResources() {
	 Options *options=getGlobalOptionsObj();
		int height=16;
		int width=16;

		if (options && qTextEdit) {
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
		}
  }

  void ChatInfo::onLineEditName_ReturnPressed() {
	 qPushButtonNameOk_Clicked(false);
  }
  void ChatInfo::onLineEditMessage_ReturnPressed() {
	 qPushButtonNameOk_Clicked(false);
  }

  ChatInfo::~ChatInfo() {
		qPixmapPersonScaled=0;
		if (pixmapRequester) delete pixmapRequester;

	 //if (model) { delete model; model=0;}
  }
  #if 0
  QStandardItemModel model(4, 4);
  for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			 QStandardItem *item = new QStandardItem(QString("row %0, column %1").arg(row).arg(column));
			 model.setItem(row, column, item);
		}
  }
  #endif

  void ChatInfo::qPushButtonNameOk_Clicked(bool checked) {
	 if (qLineEditMessage->text().size()>0 && qLineEditName->text().size()>0) {
		emit sendMessage(qLineEditName->text().replace(" ",""),qLineEditMessage->text());
	 }
	 qLineEditMessage->setText("");
  }

  void ChatInfo::qPushButtonNameDel_Clicked(bool checked) {
	if (qTextEdit) {
		qTextEdit->clear();
		pixmapRequester->clearPixmaps();
		insertImageResources();
	}
#if 0
	 if (model) {
		model->clear();
		  model->setColumnCount(COLUMN_COUNT);
		  model->setHeaderData(VIS_INDEX, Qt::Horizontal, "");
		  model->setHeaderData(SNAME_INDEX, Qt::Horizontal, tr("Von"));
		  model->setHeaderData(ANM_INDEX, Qt::Horizontal, tr("Nachricht"));
		  model->setHeaderData(TS_INDEX, Qt::Horizontal, tr("Zeitpunkt"));
	 }
#endif
  }

int ChatInfo::getMessageCount() {
	 //return model->rowCount();
	return 0;
}

  void ChatInfo::addPersonToEditfield(QString shortName) {
	 // nicht mehr gewollt!! qLineEditName->setText(shortName);
  }

  void ChatInfo::setMessage(bool incoming, QString shortName, QString longName, QString message, bool doPopup) {
	 //int	count=model->rowCount();
	 Options *options=getGlobalOptionsObj();

	 if (options && qTextEdit) {
		//qTextEdit->setTextColor(QColor(100,100,130));
		QTextCursor cursor = qTextEdit->textCursor();

		cursor.setPosition(0);
		if (shortName=="[Server]") {
            if (doPopup) {
                cursor.insertImage("image_info");
            }
		} else {
			if (incoming) {
				//cursor.insertImage("image_in");
				//if (incoming) {
				//	cursor.insertHtml("<HR>");
				//}
				if (options->showPersonPicture()) {
					if (!pixmapRequester->havePixmap(shortName)) {
						pixmapRequester->requestPixmap(shortName);
					}
					cursor.insertImage(shortName);
				} else {
					cursor.insertImage("image_in");
				}
			} else {
				cursor.insertImage("image_out");
			}
		}
		cursor.insertText("   ");

		QDateTime ts = QDateTime::currentDateTime();
		if (incoming) {
			cursor.insertHtml("<p><strong>  ["+shortName+" "+ts.toString("hh:mm:ss")+"]:   </strong></p>");
			//cursor.insertText("\n");
		} else {
			cursor.insertText("  ["+shortName+"]:   ");
		}
		//cursor.insertText("  ["+shortName+"]:   ");

		//qTextEdit->setTextColor(QColor(0,0,0));

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
		//cursor.insertText("("+ts.toString("hh:mm:ss")+")\n");
#if 0
		int count=0;
		//fprintf(stderr,"COUNT:::: %d...\n",count);

		model->insertRows(count,1);
		QModelIndex act_modelIndex=model->index(count,0 /*VIS_INDEX*/);
		model->insertColumns (0, COLUMN_COUNT, act_modelIndex);
		if (doPopup) model->setData(model->index(count,SNAME_INDEX),shortName/*,Qt::DisplayRole*/);
		model->setData(model->index(count,ANM_INDEX),message/*,Qt::DisplayRole*/);

		QDateTime ts = QDateTime::currentDateTime();
		model->setData(model->index(count, TS_INDEX), ts.toString());

		QStandardItem *item1 = model->itemFromIndex(model->index(count,VIS_INDEX));
		item1->setSelectable(false);
		item1->setEditable(false);
		QStandardItem *item2 = model->itemFromIndex(model->index(count,SNAME_INDEX));
		item2->setSelectable(false);
		item2->setEditable(false);
		QStandardItem *item = model->itemFromIndex(model->index(count,ANM_INDEX));
		item->setSelectable(true);
		item->setEditable(true);
		item->setToolTip(message);

		if (incoming && doPopup) {
			QFont f = item2->font();
			f.setBold(true);
			item2->setFont(f);
		}

		QIcon *x_icon = 0;
		if (shortName=="[Server]") {
			x_icon=options->loadedInfoIcon();
		} else {
		 if (incoming) {
			x_icon=options->loadedInMessageIcon();
		 } else {
			x_icon=options->loadedOutMessageIcon();
		 }
		}
		if (doPopup && x_icon) {
		 model->setData(model->index(count,VIS_INDEX),*x_icon,Qt::DecorationRole);
		}
		qTreeView->resizeColumnToContents(VIS_INDEX);
		qTreeView->resizeColumnToContents(SNAME_INDEX);
#endif
	}
  }
  /*
	 QStandardItem *parentItem = model->invisibleRootItem();
		QStandardItem *item = new QStandardItem(shortName);
	 QList<QStandardItem*> cols;
	 cols << new QStandardItem("test");
	 item->appendColumn(cols);
		parentItem->appendRow(item);*/



#if 0
  void ChatInfo::removeMessage(int count) {
	 int index=getPersonIndex(shortName);
	 if (index>=0) {
		  model->removeRow(index);
	 }
  }
#endif

  QSize ChatInfo::sizeHint () const
  {
	 return QSize(150,200);
  }

  /*void ChatWidget::requestPersonPicture(QString shortName) {
	 panel->requestPersonPicture(shortName);
  }*/


  ChatWidget::ChatWidget(QWidget *parent) : QDockWidget(parent) {
	 panel=new ChatInfo(this);
	 //panel->setMinimumSize(QSize(150,100));
	 setWidget(panel);
	 connect(panel,SIGNAL(sendMessage (QString, QString)),this,SLOT(slot_onSendMessage(QString, QString)));
  }
#if 0
  void ChatWidget::slot_onPersonAdded(QString shortName) {
	 emit onPersonAdded(shortName);
  }
#endif
  void ChatWidget::slot_onSendMessage(QString to, QString message) {
		emit sendMessage(to,message);
  }


  void ChatWidget::addPersonToEditfield(QString shortName) {
	 if (panel) {
		  panel->addPersonToEditfield(shortName);
	 }
  }

  int ChatWidget::getMessageCount() {
#if 0
	 if (panel) {
		  return panel->getMessageCount();
	 }
#endif
	 return 0;
  }

	void ChatWidget::setMessage(bool incoming, QString shortName, QString longName, QString message, bool doPopup) {
	 if (panel) {
		  panel->setMessage(incoming, shortName,longName, message, doPopup);
	 }
  }

  ChatWidget::~ChatWidget() {
  }
