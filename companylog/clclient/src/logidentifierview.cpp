#include "logidentifierview.h"

enum {
    model_icon_index=0,
    model_message_index,
    model_icon_name_index,
    model_column_count
};

LogIdentifierView::LogIdentifierView(QWidget *parent) :
    QDialog(parent){
    Options *options=getGlobalOptionsObj();
    model=0;
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);

    this->pushButtonAdd->setIcon(*options->pixmapCacheIcons()->getIcon("addplus.png"));
    this->pushButtonRemove->setIcon(*options->pixmapCacheIcons()->getIcon("remove.png"));
    connect(this->pushButtonAdd,SIGNAL(clicked()),this,SLOT(pushButtonAdd_clicked()));
    connect(this->pushButtonRemove,SIGNAL(clicked()),this,SLOT(pushButtonRemove_clicked()));
    connect(this->pushButtonChangeIcon,SIGNAL(clicked()),this,SLOT(pushButtonChangeIcon_clicked()));

    setupModel();
    fillModel();
}

void LogIdentifierView::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void LogIdentifierView::accept() {
    Options *options=getGlobalOptionsObj();
    options->logManager()->clearIdentifiers();
    for (int i=0;i<model->rowCount();i++) {
        QVariant data;
        data=model->data(model->index(i,model_icon_name_index),Qt::DisplayRole);
        if (data.toString().size()>0) {
            QVariant data2;
            data2=model->data(model->index(i,model_message_index),Qt::DisplayRole);
            if (data2.toString().size()>0) {
                LogIdentifier *logIdentifier=new LogIdentifier();
                logIdentifier->setIconName(data.toString());
                logIdentifier->setPatternFromFullPatternString(data2.toString());

                options->logManager()->logIdentifierAppend(logIdentifier);
            }
        }
    }
    QDialog::accept();
}

void LogIdentifierView::reject() {
    QDialog::reject();
}

void LogIdentifierView::setupModel() {
    if (!model) {
        model=new QStandardItemModel(0,model_column_count);
        this->treeView->setModel(model);
    } else {
        model->clear();
    }
    if (model) {
        model->setColumnCount(model_column_count);
        model->setHeaderData(model_icon_index, Qt::Horizontal, "Icon");
        model->setHeaderData(model_icon_name_index, Qt::Horizontal, "Icon Dateiname");
        model->setHeaderData(model_message_index, Qt::Horizontal, tr("Suchmuster"));
    }
}

void LogIdentifierView::fillModel() {
    Options *options=getGlobalOptionsObj();
    int count=options->logManager()->logIdentifiersCount();
    for (int i=0;i<count;i++) {
        LogIdentifier *logIdentifier=options->logManager()->logIdentifier(i);
        if (logIdentifier->iconName().size()>0) {
            QIcon *icon=options->logManager()->getIcon(logIdentifier->iconName());
            if (icon) {
                int pattern_count=logIdentifier->patternCount();
                if (pattern_count>0) {
                    model->insertRow(model->rowCount());
                    model->setData(model->index(model->rowCount()-1,model_icon_index),*icon,Qt::DecorationRole);
                    model->setData(model->index(model->rowCount()-1,model_icon_name_index),logIdentifier->iconName()/*,Qt::DisplayRole*/);
                    model->setData(model->index(model->rowCount()-1,model_message_index),logIdentifier->fullPatternString()/*,Qt::DisplayRole*/);
                    QStandardItem * it=model->itemFromIndex(model->index(model->rowCount()-1,model_icon_index));
                    if (it) {
                        it->setEditable(false);
                    }
                    it=model->itemFromIndex(model->index(model->rowCount()-1,model_icon_name_index));
                    if (it) {
                        it->setEditable(false);
                    }
                }
            }
        }
    }
    this->treeView->resizeColumnToContents(model_icon_index);
    this->treeView->resizeColumnToContents(model_icon_name_index);
    this->treeView->resizeColumnToContents(model_message_index);
}


void LogIdentifierView::copyIcon(QString fromAbsolut, QString toAbsolut) {
    QFile f(fromAbsolut);
    f.copy(toAbsolut);
}

void LogIdentifierView::pushButtonAdd_clicked() {
    Options *options=getGlobalOptionsObj();
    QString fileNameFull = QFileDialog::getOpenFileName(this,
         tr("Icon selektieren"), options->getLogIconPath(), tr("Icon Dateien (*.png *.jpg *.bmp)"));
    if (fileNameFull.size()>0) {
        QFileInfo pathInfo(fileNameFull);
        QString fileName(pathInfo.fileName());
        QString filePath(pathInfo.absolutePath());
        if (fileName.size()>4) {
            if (filePath!=options->getLogIconPath() && filePath!=options->getLogIconPrivatePath()) {
                copyIcon(fileNameFull, options->getLogIconPrivatePath()+"/"+fileName);
            }

            QIcon *icon=options->logManager()->getIcon(fileName);
            if (icon) {
                model->insertRow(model->rowCount());
                model->setData(model->index(model->rowCount()-1,model_icon_index),*icon,Qt::DecorationRole);
                model->setData(model->index(model->rowCount()-1,model_icon_name_index),fileName/*,Qt::DisplayRole*/);
                QStandardItem * it=model->itemFromIndex(model->index(model->rowCount()-1,model_icon_index));
                if (it) {
                    it->setEditable(false);
                }
                it=model->itemFromIndex(model->index(model->rowCount()-1,model_icon_name_index));
                if (it) {
                    it->setEditable(false);
                }
            }
        }
    }
}

void LogIdentifierView::pushButtonChangeIcon_clicked() {
    QItemSelectionModel *itemSelectionModel=this->treeView->selectionModel();

    for (int i=model->rowCount()-1;i>=0;i--) {
        if (itemSelectionModel->isRowSelected(i,QModelIndex())) {
            Options *options=getGlobalOptionsObj();
            QString fileNameFull = QFileDialog::getOpenFileName(this,
                 tr("Icon selektieren"), options->getLogIconPath(), tr("Icon Dateien (*.png *.jpg *.bmp)"));
            if (fileNameFull.size()>0) {
                QFileInfo pathInfo(fileNameFull);
                QString fileName(pathInfo.fileName());
                QString filePath(pathInfo.absolutePath());
                if (fileName.size()>4) {
                    if (filePath!=options->getLogIconPath() && filePath!=options->getLogIconPrivatePath()) {
                        copyIcon(fileNameFull, options->getLogIconPrivatePath()+"/"+fileName);
                    }
                    QIcon *icon=options->logManager()->getIcon(fileName);
                    if (icon) {
                        model->setData(model->index(i,model_icon_index),*icon,Qt::DecorationRole);
                        model->setData(model->index(i,model_icon_name_index),fileName/*,Qt::DisplayRole*/);
                    }
                }
            }
            break;//ansonsten kommte der Index durcheinander!
        }
    }
}


void LogIdentifierView::pushButtonRemove_clicked() {
    QItemSelectionModel *itemSelectionModel=this->treeView->selectionModel();

    for (int i=model->rowCount()-1;i>=0;i--) {
        if (itemSelectionModel->isRowSelected(i,QModelIndex())) {
            model->removeRow(i);
            break;//ansonsten kommte der Index durcheinander!
        }
    }
}
