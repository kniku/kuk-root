/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h20
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : ListWidget.cpp
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "ListWidget.h"


MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent):QSortFilterProxyModel(parent) {

}
bool MySortFilterProxyModel::lessThan ( const QModelIndex & left, const QModelIndex & right ) const {
    if (left.parent()!=QModelIndex() || right.parent()!=QModelIndex()) {
        // kein Haupteintrag im TreeModel
        return false;
    }
    return QSortFilterProxyModel::lessThan(left,right);
}


ListWidget::ListWidget(QWidget *parent):QTreeView(parent) {
	//lastTimerId=-1;
	myTabWidget=NULL;
	lineEditPointer=NULL;
	memset(lastRequestString,0,256);
	setupModel();
	lastRequest="";
	requestObject = new RequestObject();
	memberData = new MemberData();
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setAlternatingRowColors(true);
  connect(requestObject, SIGNAL(requestFinish()), this, SLOT(onRequestFinish()));

	timer = new QTimer(this);
    	connect(timer, SIGNAL(timeout()), this, SLOT(on_timerFinish()));
	connect(this, SIGNAL(pressed(QModelIndex)), this, SLOT(onLineClicked(QModelIndex)));
}

void ListWidget::onLineClicked( const QModelIndex & index) {
	MemberIdentifier memberIdentifier;
	MemberIdentifier *memberIdentifierExist;
    int row=0;
    if (index.parent().isValid()) {
        row=getModelMainRowFromVisualRow(index.parent().row());
    } else {
        row=getModelMainRowFromVisualRow(index.row());
    }


	QVariant name=model->data(model->index(row,NAME_INDEX));
	QVariant vname=model->data(model->index(row,VNAME_INDEX));
	QVariant date=model->data(model->index(row,DATENATIVE_INDEX));

	memberIdentifier.hashKey=memberData->createHashKey(date.toDate()/*QDate::currentDate()*/,name.toString(),vname.toString(),false);
	memberIdentifierExist=memberData->searchMember(memberIdentifier.hashKey);
	//fprintf(stderr,"Hash: %s\n",memberIdentifier.hashKey.toAscii().data());
	if (memberIdentifierExist && memberIdentifierExist->fullName.size()>1) {
		fprintf(stderr,"Name: %s\n",memberIdentifierExist->fullName.toAscii().data());
		emit onMemberSelected(memberIdentifierExist);
	}
}

TabPanel::TabPanel(QWidget *parent):QWidget(parent) {
}



void ListWidget::setTabLabel(QString label) {
	if (myTabWidget) {
		for (int i=0;i<myTabWidget->count();i++) {
			if (myTabWidget->widget(i)==this->parentWidget()) {
				myTabWidget->setTabText(i,label);
				break;
			}
		}
	}
}

ListWidget::~ListWidget() {
	if (requestObject) delete requestObject;
	if (memberData) delete memberData;
	if (model) delete model;
}

QString ListWidget::buildTabLabel(QTime _time, QString postfix) {
	if (isAuswertung) {
		return "Auswertung";
	}
	QString tmpTabLabel="Heute ("+_time.toString()+")";
	if (postfix.size()>0) {
		tmpTabLabel+=" - "+postfix;
	}
	return tmpTabLabel;
}

void ListWidget::onRequestFinish() {
	fprintf(stderr,"ListWidget::onRequestFinish called\n");
	lastRequest_ca=QTime::currentTime();
	QByteArray array;
	bool	secondRound=false;
	Options *options;
	options=getGlobalOptionsObj();

	if (options && requestObject->resultBuf.size()>0) {
		QHeaderView *xheader=header();

		if (!isAuswertung) {
			if (xheader && model->columnCount()>0) {
				secondRound=true;
				array=xheader->saveState();
                //fprintf(stderr,"ListWidget::Layout: %s\n",array.toBase64().constData());
				options->setListState(array);
			}
		}

		memberData->updateAllMembers(&requestObject->resultBuf);
        this->setSortingEnabled(false);

        fillList();

		if (!isAuswertung) {
			if (xheader) {
				if (secondRound) {
					xheader->restoreState(array);
				} else {
					array=options->listState();
					if (array.size()>0) {
						xheader->restoreState(array);
					} else {
						resizeColumnToContents(VIS_INDEX);
						resizeColumnToContents(NAME_INDEX);
						resizeColumnToContents(VNAME_INDEX);
						resizeColumnToContents(ANM_INDEX);
						resizeColumnToContents(INOUT_INDEX);
                    }
				}
			}
		} else {
			resizeColumnToContents(VIS_INDEX);
			resizeColumnToContents(NAME_INDEX);
			resizeColumnToContents(VNAME_INDEX);
			resizeColumnToContents(ANM_INDEX);
			resizeColumnToContents(INOUT_INDEX);
            resizeColumnToContents(WEEKDAY_INDEX);
        }
        //if (!isAuswertung) {
        this->setSortingEnabled(true);
        QHeaderView* headerv=this->header();
        int index=headerv->sortIndicatorSection();
        if (index>=0) {
            //this->sortByColumn(index,headerv->sortIndicatorOrder());
            model->sort(index,headerv->sortIndicatorOrder());
        }
        //}
        if (isAuswertung) {
			hideColumn(OPTREST_INDEX);
        } else {
            hideColumn(WEEKDAY_INDEX);
        }
		hideColumn(DATENATIVE_INDEX);

		//if (!isAuswertung) {
		//	setTabLabel(buildTabLabel(lastRequest_ca, lastSearchString);
		//}
		//resizeColumnToContents(INOUTTOTAL_INDEX);

		if (!isAuswertung) {
			MemberLog *log=memberData->getLastLogForCurrentUser();
			if (log) {
				emit onCurrentMemberStateUpdated(log);
			}
			emit onCurrentListRefreshed();
		}
	}
	lastRequest=getLastRequest();
}
QString ListWidget::getLastRequest() {
	return memberData->lastRequest;
}
bool ListWidget::fillList() {
    memberData->fillModel(model,isAuswertung);

	lastSearchString="";
	on_filterLineEdit_Leave();
#if 0
	model->setData(model->index(0,0),"Line 1");
	model->setData(model->index(1,0),"Line 2");
	model->insertRows ( 0, 3, model->index(0,0));
	//model->insertColumns ( 0, 3, model->index(0,0));
	model->setData(model->index(0,0,model->index(0,0)),"Subline 1.1");
	//model->setData(model->index(1,0).child(0,0),"Subline 2.1");
#endif
	return true;
}

void ListWidget::on_timerFinish() {
	updateData(0);
}


/*void ListWidget::timerEventX(QTimerEvent *event) {
	//QTreeView.timerEvent(event);
	if (event->timerId()==lastTimerId) {
		killTimer(event->timerId());
		updateData(0);
	}
}*/



void ListWidget::startRefreshTimer() {
	Options *options;
	options=getGlobalOptionsObj();
	if (options) {
		long sec=options->timerSequence();
		if (sec<=10) sec=60;
		
	    timer->start(sec*1000);
		//lastTimerId=startTimer(sec*1000);
	} else {
	    timer->start(60*1000);
		//lastTimerId=startTimer(60*1000);
	}	
}

bool ListWidget::updateData(char *extRequestString) {
	//fprintf(stderr,"updateData called\n");
	fprintf(stderr,"ListWidget::updateData called...\n");
	char requestString[1000+1];
	Options *options;
	QString user_name;
	options=getGlobalOptionsObj();

	memset(requestString,0,1000);

	user_name=options->getUsername();

	if (extRequestString) {
		/* Auswertung */
		isAuswertung=true;
		strncpy(requestString,extRequestString,1000);
	} else {
		isAuswertung=false;
		//sprintf(requestString,"es3.5:PZE:000000000000000000000000000000:00%s",lastRequest.toString("hhmmss").toAscii().data());
		sprintf(requestString,"es3.5:PZE:%-30s:%s",user_name.toAscii().data(),lastRequest.size()>0?lastRequest.toAscii().data():"0000000000");
		//fprintf(stderr,requestString);	
	}	

	requestObject->requestCompanylogServer(requestString);

	if (strstr(requestString,":AUS:")) {
	} else {
		startRefreshTimer();
	}
	return true;
}

void ListWidget::setupModel()
{
	model = new QStandardItemModel(this/*0, 12, this*/);
	model->clear();

    proxyModel = new MySortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    //setModel(model);
    setModel(proxyModel);
    this->setSortingEnabled(false);

    //selectionModel = new QItemSelectionModel(model);
	//setSelectionModel(selectionModel);
}

int ListWidget::getVisualMainRow(int modelRow) {
    QModelIndex visualIndex=proxyModel->mapFromSource(model->index(modelRow,0,QModelIndex()));
    return visualIndex.row();
}
int ListWidget::getModelMainRowFromVisualRow(int visualRow) {
    QModelIndex modelxIndex=proxyModel->mapToSource(proxyModel->index(visualRow,0,QModelIndex()));
    return modelxIndex.row();
}

void ListWidget::on_filterLineEdit_Leave() {
	QString				searchtext;
	QStandardItemModel *myModel;

	if (lineEditPointer!=0) {
		int	searchlength=0;
		searchtext=lineEditPointer->text();
		searchtext.remove(QRegExp("[*+ -]"));
		searchlength=searchtext.size();

		myModel=(QStandardItemModel*)model;
		if(myModel && lastSearchString!=searchtext) {
			int rowCount=myModel->rowCount();	
			for (int i=0;i<rowCount;i++) {
				int isIn=true;
                int visualRow=getVisualMainRow(i);
                if (searchlength>0) {
					QString rowString=myModel->data(myModel->index(i,NAME_INDEX)).toString();
					if (!rowString.contains(searchtext,Qt::CaseInsensitive)) {
						QString rowString=myModel->data(myModel->index(i,VNAME_INDEX)).toString();
						if (!rowString.contains(searchtext,Qt::CaseInsensitive)) {
							QString rowString=myModel->data(myModel->index(i,ZUSATZ_INDEX)).toString();
							if (!rowString.contains(searchtext,Qt::CaseInsensitive)) {
								QString rowString=myModel->data(myModel->index(i,ANM_INDEX)).toString();
								if (!rowString.contains(searchtext,Qt::CaseInsensitive)) {
                                    QString rowString=myModel->data(myModel->index(i,OPTREST_INDEX)).toString();
                                    if (!rowString.contains(searchtext,Qt::CaseInsensitive)) {
                                        isIn=false;
                                    }
								}
							}
						}
					}
                    if (isIn) {
                        setRowHidden(visualRow,QModelIndex(),false);
                    } else {
                        setRowHidden(visualRow,QModelIndex(),true);
                    }
				} else {
					/* kein Filtertext -> alles sichtbar */
                    setRowHidden(visualRow,QModelIndex(),false);
				}
			}
		}
		lastSearchString=searchtext;
	} else lastSearchString="";
	setTabLabel(buildTabLabel(lastRequest_ca, lastSearchString));
}

/* Editbox for Filter */
LabelLineEdit::LabelLineEdit(QString _label,QWidget *parent) : QWidget(parent) {
	label=new QLabel(_label,this);
	lineEdit=new QLineEdit(this);
	lineEdit->setMaxLength(100);
	lineEdit->setMaximumWidth(200);//Size(lineEdit->maximumSize().height(),100);
	hbox=new QHBoxLayout(this);
    hbox->setMargin(1);
	hbox->addWidget(label);
	hbox->addWidget(lineEdit);
    hbox->addStretch();
}

LabelLineEdit::~LabelLineEdit() {
}

QLineEdit* LabelLineEdit::lineEditPointer() {
	return lineEdit;
}
