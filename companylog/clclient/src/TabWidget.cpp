/********************************************************************************************************
		* PROGRAM      : companylog
		* DATE - TIME  : Sonntag 14 Mai 2006 - 10h19
		* AUTHOR       : Martin Paar ( martin.paar@gmx.at )
			* FILENAME     : TabWidget.cpp
			* LICENSE      :
* COMMENTARY   :
********************************************************************************************************/
#include "TabWidget.h"

// Button Tab-Corner PushButton
void TabWidget::on_cornerPushButton_ClickedX() {
	if (currentIndex()>=0) {
		int current=currentIndex();
		QWidget *w=currentWidget();
		if (w) {
			delete w;
		}
		if (count()<=current) {
			current=count()-1;
		}
		setCurrentIndex(current);
		//removeTab(currentIndex());
	}
}

TabWidget::TabWidget(QWidget *parent):QTabWidget(parent) {
    Options *options=getGlobalOptionsObj();

    actRequestForListWidget=0;
    cornerPushButton = new QPushButton(*options->pixmapCacheIcons()->getIcon("remove.png"),"",this);
	if (cornerPushButton) {
        cornerPushButton->setToolTip("schließen");
		setCornerWidget(cornerPushButton);
		connect(cornerPushButton,SIGNAL(clicked()),this,SLOT(on_cornerPushButton_ClickedX()));
	}
	//setContentsMargins(0,0,0,0);
	//setMargin(0);
	
	//listWidget1=new QListWidget(this);
	//setWidget(listWidget1);
}
TabWidget::~TabWidget() {
}


void TabWidget::createNewAuswertungTab(QString name, QString vName, QString year, QString month, QString day) {
	TabPanel *tabPanel = new TabPanel();
	ListWidget	*listWidget = new ListWidget(tabPanel);
	int		tab_index;
	QVBoxLayout	*hbox = new QVBoxLayout(tabPanel);
	QHBoxLayout *hhbox = new QHBoxLayout(0);

	hbox->addLayout(hhbox);
	hbox->addWidget(listWidget);

	
	char requestString[1000];
	memset(requestString,0,1000);
	QString fulln;
	QString fullz;
	char *user_name=getenv("USER");
	
	/*	fulln=name;
	if (fulln.size()>0 && vName.size()>0) {
		fulln+=" ";
		fulln+=vName;
	}*/
	fullz=year;
	if (month.size()>0) {
		fullz+=month.rightJustified(2, '0');
	}
	if (day.size()>0) {
		fullz+=day.rightJustified(2, '0');;
	}
	fullz+="*.pze";
	
	
	sprintf(requestString,"es3.5:AUS:%-30s:%s",user_name?user_name:"",fullz.toAscii().data());
	
	if (listWidget) {
		//tab->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
		//tab->updateGeometry();
		tabPanel->listWidget=listWidget;
		tab_index=addTab(tabPanel,tr("Auswertung"));
		if (tab_index>=0) {
			listWidget->myTabWidget=this;
			listWidget->updateData(requestString);
			//setTabText(tab_index,"Heute um: "+lastRequest.toString("hh:mm:ss"));
			//requestCompanylogServerFor(listWidget);
			setCurrentIndex(tab_index);
		}
	}
}

PokerView* TabWidget::createNewPokerTab() {
    int		tab_index;

    PokerView *pokerView=new PokerView();
    if (pokerView) {
        //pokerView->setGeometry(0,0,300,300);
        //QVBoxLayout *layoutV = new QVBoxLayout(this);
        //layoutV->addWidget (pokerView);
        //this->setStyleSheet("PokerView { background-color: rgb(220,230,235)} ");
        tab_index=addTab(pokerView,tr("Poker"));
        if(tab_index>=0) {
            setCurrentIndex(tab_index);
        }
    }
    return pokerView;
}

SqlWorksheet* TabWidget::createNewSqlWorksheetTab() {
    int		tab_index;

    SqlWorksheet *sqlWorksheet=new SqlWorksheet();
    if (sqlWorksheet) {
        //pokerView->setGeometry(0,0,300,300);
        //QVBoxLayout *layoutV = new QVBoxLayout(this);
        //layoutV->addWidget (pokerView);
        //this->setStyleSheet("PokerView { background-color: rgb(220,230,235)} ");
        tab_index=addTab(sqlWorksheet,tr("Sql-Worksheet"));
        if(tab_index>=0) {
            setCurrentIndex(tab_index);
        }
    }
    return sqlWorksheet;
}


TabPanel* TabWidget::createNewStandardTab() {
	int		tab_index;
	TabPanel *tabPanel = new TabPanel();
	ListWidget	*listWidget = new ListWidget(tabPanel);

	QVBoxLayout	*hbox = new QVBoxLayout(tabPanel);
	QHBoxLayout *hhbox = new QHBoxLayout(0);
	LabelLineEdit	*labelLineEdit=new LabelLineEdit("Filter",tabPanel);
	
	if (tabPanel && hbox && hhbox && listWidget) {
		connect(labelLineEdit->lineEditPointer(),SIGNAL(editingFinished()),listWidget,SLOT(on_filterLineEdit_Leave()));
		listWidget->lineEditPointer=labelLineEdit->lineEditPointer();
		tabPanel->listWidget=listWidget;
		//hhbox->setSizeConstraint(QLayout::SetNoConstraint);
		//refreshPushButton->setFlat(true);
		//refreshPushButton->setMinimumSize(5,5);
		//connect(refreshPushButton,SIGNAL(clicked()),listWidget,SLOT(on_refreshPushButton_ClickedX()));
		//hhbox->addWidget(refreshPushButton);
		hhbox->addWidget(labelLineEdit);
        hhbox->setMargin(1);
        hbox->setMargin(1);
		//tabPanel->setForegroundRole(QPalette::WindowText);
		//tabPanel->setBackgroundRole(QPalette::WindowText);
		//tabPanel->setMinimumSize(100,100);
		//tabPanel->setMaximumSize(100,100);
        hbox->addLayout(hhbox);
        hbox->addWidget(listWidget);
        //tabPanel->setLayout(hbox);
		//widg->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
			//listWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
		tab_index=addTab(tabPanel,tr("Heute"));
		if(tab_index>=0) {
			listWidget->myTabWidget=this;
			listWidget->updateData(0);
			setCurrentIndex(tab_index);
		}
	}
	return tabPanel;
}



