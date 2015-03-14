/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Freitag 01 Feb 2008 - 10h15
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : WaitForWidget.cpp
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "WaitForWidget.h"

#define VIS_INDEX 0
#define SNAME_INDEX 1
#define ANM_INDEX 2
 
#define COLUMN_COUNT 3


WaitInfo::WaitInfo(QWidget *parent) : QWidget(parent) {
	Options *options=getGlobalOptionsObj();

	if (options) {
		//fprintf(stderr,"Picture Server: %s\n",options->serverName_2().toAscii().data());
		QHBoxLayout *layout = new QHBoxLayout;
		QVBoxLayout *layoutV = new QVBoxLayout;
		qLabelName=new QLabel("Kürzel");
		qLabelName->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
		qLineEditName=new QLineEdit();
		qLineEditName->setMaxLength(3);
		qLineEditName->setMaximumWidth(40);
		qLineEditName->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
		connect(qLineEditName,SIGNAL(returnPressed()),this,SLOT(onLineEditName_ReturnPressed()));

        qPushButtonNameOk = new QPushButton(*options->pixmapCacheIcons()->getIcon("add.png"),"");
		qPushButtonNameOk->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
		qPushButtonNameOk->setAutoDefault(true);
		qPushButtonNameOk->setToolTip(tr("zur Warteliste hinzufügen"));
		connect(qPushButtonNameOk,SIGNAL(clicked (bool)),this,SLOT(qPushButtonNameOk_Clicked(bool)));

        qPushButtonNameDel = new QPushButton(*options->pixmapCacheIcons()->getIcon("remove.png"),"");
		qPushButtonNameDel->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
		qPushButtonNameDel->setToolTip(tr("aus Warteliste entfernen"));
		//qPushButtonNameDel->setAutoDefault(true);
		connect(qPushButtonNameDel,SIGNAL(clicked (bool)),this,SLOT(qPushButtonNameDel_Clicked(bool)));

		model=new QStandardItemModel(0,COLUMN_COUNT);
		model->setColumnCount(COLUMN_COUNT);
		model->setHeaderData(VIS_INDEX, Qt::Horizontal, "");
		model->setHeaderData(SNAME_INDEX, Qt::Horizontal, tr("Kürzel"));
		model->setHeaderData(ANM_INDEX, Qt::Horizontal, tr("Anmerkung"));

		qTreeView=new QTreeView();		
		qTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
		qTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
		qTreeView->setModel(model);

		connect(qTreeView, SIGNAL(pressed(QModelIndex)), this, SLOT(onLineClicked(QModelIndex)));

		
		layout->addWidget(qLabelName);
		layout->addWidget(qLineEditName);
		layout->addWidget(qPushButtonNameOk);
		layout->addWidget(qPushButtonNameDel);
		layout->insertStretch(-1);

		layoutV->addLayout(layout);
		layoutV->addWidget(qTreeView);
		this->setLayout(layoutV);

	}
}

void WaitInfo::onLineClicked( const QModelIndex & index) {
	int row=index.row();	
	if (row>=0) {
		QVariant var=model->data(model->index(row,SNAME_INDEX)/*,Qt::DisplayRole*/);
		addPersonToEditfield(var.toString());
	}
}

void WaitInfo::onLineEditName_ReturnPressed() {
	qPushButtonNameOk_Clicked(false);
}


WaitInfo::~WaitInfo() {
	if (model) { delete model; model=0;}
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

void WaitInfo::qPushButtonNameDel_Clicked(bool checked) {
	removePerson(qLineEditName->text());
	addPersonToEditfield("");
}

void WaitInfo::qPushButtonNameOk_Clicked(bool checked) {
	if (qLineEditName->text().size()==3) {
		if (!isPersonExists(qLineEditName->text())) {
			addPerson(qLineEditName->text());
			addPersonToEditfield("");
		}
	}
}

int WaitInfo::getPersonIndex(QString shortName) {
	int	count=model->rowCount();
	for (int i=0;i<count;i++) {
		QVariant var=model->data(model->index(i,SNAME_INDEX)/*,Qt::DisplayRole*/);
		if (shortName.compare(var.toString(),Qt::CaseInsensitive)==0) {
			return i;
		}
	}
	return -1;
}

int WaitInfo::getPersonCount() {
	return model->rowCount();
}

QString WaitInfo::getPersonAtIndex(int index) {
	
	if (index>=0 && index<model->rowCount()) {
		QVariant var=model->data(model->index(index,SNAME_INDEX)/*,Qt::DisplayRole*/);
		return var.toString();
	}
	return "";
}

bool WaitInfo::isPersonExists(QString shortName) {
	if (getPersonIndex(shortName)>=0) {
		return true;
	}
	return false;
}

void WaitInfo::addPersonToEditfield(QString shortName) {
	qLineEditName->setText(shortName);
}


void WaitInfo::setMemberLog(QString shortName, MemberLog *memberLog) {
    Options *options=getGlobalOptionsObj();
    int index=getPersonIndex(shortName);
    if (index>=0 && memberLog) {
        QIcon* icon=options->logManager()->getIconFromLog(memberLog->message(), memberLog->isIn());
        if (icon) {
            model->setData(model->index(index,VIS_INDEX),*icon,Qt::DecorationRole);
        }
        model->setData(model->index(index,ANM_INDEX),memberLog->anm.trimmed()/*,Qt::DisplayRole*/);
        //model->setData(model->index(index,NAME_INDEX),memberLog(/*,Qt::DisplayRole*/);
    }
}

void WaitInfo::addPerson(QString shortName) {
	MemberLog memberLog;
	int	count=model->rowCount();
	
	model->insertRows(count,1);
	QModelIndex act_modelIndex=model->index(count,0 /*VIS_INDEX*/);
	model->insertColumns (0, COLUMN_COUNT, act_modelIndex);

	model->setData(model->index(count,SNAME_INDEX),shortName/*,Qt::DisplayRole*/);
	//model->setData(model->index(count,1),""/*,Qt::DisplayRole*/);

	setMemberLog(shortName, &memberLog);
	qTreeView->resizeColumnToContents(VIS_INDEX);
	qTreeView->resizeColumnToContents(SNAME_INDEX);

	emit onPersonAdded(shortName);

/*
	QStandardItem *parentItem = model->invisibleRootItem();
    QStandardItem *item = new QStandardItem(shortName);
	QList<QStandardItem*> cols;
	cols << new QStandardItem("test");
	item->appendColumn(cols);
    parentItem->appendRow(item);*/
}



void WaitInfo::removePerson(QString shortName){
	int index=getPersonIndex(shortName);
	if (index>=0) {
		model->removeRow(index);
	}
}


QSize WaitInfo::sizeHint () const
{
	return QSize(150,200);
}

/*void WaitForWidget::requestPersonPicture(QString shortName) {
	panel->requestPersonPicture(shortName);
}*/


WaitForWidget::WaitForWidget(QWidget *parent) : QDockWidget(parent) {
	panel=new WaitInfo(this);
	//panel->setMinimumSize(QSize(150,100));
	setWidget(panel);
	connect(panel,SIGNAL(onPersonAdded (QString)),this,SLOT(slot_onPersonAdded(QString)));
}

void WaitForWidget::slot_onPersonAdded(QString shortName) {
	emit onPersonAdded(shortName);
}

void WaitForWidget::addPersonToEditfield(QString shortName) {
	if (panel) {
		panel->addPersonToEditfield(shortName);
	}
}

void WaitForWidget::removePerson(QString shortName){
	if (panel) {
		panel->removePerson(shortName);
	}
}

int WaitForWidget::getPersonCount() {
	if (panel) {
		return panel->getPersonCount();
	}
	return 0;
}

QString WaitForWidget::getPersonAtIndex(int index) {
	if (panel) {
		return panel->getPersonAtIndex(index);
	}
	return "";
}

void WaitForWidget::setMemberLog(QString shortName, MemberLog *memberLog) {
	if (panel) {
		panel->setMemberLog(shortName,memberLog);
	}
}

WaitForWidget::~WaitForWidget() {
}
