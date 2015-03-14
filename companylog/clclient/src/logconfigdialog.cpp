#include "logconfigdialog.h"

enum {
    model_icon_index=0,
    model_message_index,
    model_in_index,
    model_icon_name_index,
    model_locked_index,
    model_column_count
};

LogConfigDialog::LogConfigDialog(QWidget *parent) :
    QDialog(parent)
{
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

LogConfigDialog::~LogConfigDialog()
{
}

void LogConfigDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
void LogConfigDialog::accept() {
    Options *options=getGlobalOptionsObj();
    int count=options->logManager()->logActionCount();
    for (int x=0;x<count;x++) {
        LogAction* logAction=options->logManager()->logAction(x);
        if (logAction && logAction->locked()=="0") {
            LogIdentifier* logIdentifier=options->logManager()->searchLogIdentifierFromIconName(logAction->iconName());
            if (logIdentifier) {
                logIdentifier->removePattern(logAction->text());
            }
        }
    }

    options->logManager()->clearActions();
    for (int i=0;i<model->rowCount();i++) {
        QVariant data;
        data=model->data(model->index(i,model_icon_name_index),Qt::DisplayRole);
        if (data.toString().size()>0) {
            LogAction *logAction=new LogAction();
            logAction->setIconName(data.toString());
            data=model->data(model->index(i,model_message_index),Qt::DisplayRole);
            logAction->setText(data.toString());
            logAction->setName(data.toString());
            data=model->data(model->index(i,model_in_index),Qt::DisplayRole);
            logAction->setIn(data.toString());
            data=model->data(model->index(i,model_locked_index),Qt::DisplayRole);
            logAction->setLocked(data.toString());
            options->logManager()->logActionAppend(logAction);

            if (logAction->locked()=="0") {
                LogIdentifier* logIdentifier=options->logManager()->searchLogIdentifierFromIconName(logAction->iconName());
                if (logIdentifier) {
                    logIdentifier->addPattern(logAction->text());
                } else {
                    LogIdentifier *logIdentifier=new LogIdentifier();
                    if (logIdentifier) {
                        logIdentifier->setIconName(logAction->iconName());
                        logIdentifier->addPattern(logAction->text());
                        options->logManager()->logIdentifierAppend(logIdentifier);
                    }
                }
            }
        }
    }
    emit LogConfigChanged();
    QDialog::accept();
}
void LogConfigDialog::reject() {
    QDialog::reject();
}


void LogConfigDialog::setupModel() {
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
        model->setHeaderData(model_message_index, Qt::Horizontal, tr("Logeintrag"));
        model->setHeaderData(model_in_index, Qt::Horizontal, tr("IN/OUT"));
        model->setHeaderData(model_locked_index, Qt::Horizontal, tr("Gesperrt"));
    }

}

void LogConfigDialog::fillModel() {
    Options *options=getGlobalOptionsObj();
    int count=options->logManager()->logActionCount();
    QColor cLightRed(255,240,248);
    for (int i=0;i<count;i++) {
        LogAction *logAction=options->logManager()->logAction(i);
        if (logAction->iconName().size()>0) {
            QIcon *icon=options->logManager()->getIcon(logAction->iconName());
            if (icon) {
                model->insertRow(model->rowCount());
                model->setData(model->index(model->rowCount()-1,model_icon_index),*icon,Qt::DecorationRole);
                model->setData(model->index(model->rowCount()-1,model_icon_name_index),logAction->iconName()/*,Qt::DisplayRole*/);
                model->setData(model->index(model->rowCount()-1,model_message_index),logAction->text()/*,Qt::DisplayRole*/);
                model->setData(model->index(model->rowCount()-1,model_in_index),logAction->in()/*,Qt::DisplayRole*/);
                model->setData(model->index(model->rowCount()-1,model_locked_index),logAction->locked()/*,Qt::DisplayRole*/);
                QStandardItem * it=model->itemFromIndex(model->index(model->rowCount()-1,model_icon_index));
                if (it) {
                    it->setEditable(false);
                }
                it=model->itemFromIndex(model->index(model->rowCount()-1,model_icon_name_index));
                if (it) {
                    it->setEditable(false);
                    if (logAction->locked()=="1") {
                        it->setBackground(cLightRed);
                    }
                }
                it=model->itemFromIndex(model->index(model->rowCount()-1,model_locked_index));
                if (it) {
                    it->setEditable(false);
                    if (logAction->locked()=="1") {
                        it->setBackground(cLightRed);
                    }
                }
                if (logAction->locked()=="1") {
                    it=model->itemFromIndex(model->index(model->rowCount()-1,model_in_index));
                    if (it) {
                        it->setEditable(false);
                        it->setBackground(cLightRed);
                    }
                    it=model->itemFromIndex(model->index(model->rowCount()-1,model_message_index));
                    if (it) {
                        it->setEditable(false);
                        it->setBackground(cLightRed);
                    }
                }
            }
        }
    }
    this->treeView->resizeColumnToContents(model_icon_index);
    this->treeView->resizeColumnToContents(model_icon_name_index);
    this->treeView->resizeColumnToContents(model_message_index);
    this->treeView->resizeColumnToContents(model_in_index);
    this->treeView->resizeColumnToContents(model_locked_index);

}

void LogConfigDialog::copyIcon(QString fromAbsolut, QString toAbsolut) {
    QFile f(fromAbsolut);
    f.copy(toAbsolut);
}

void LogConfigDialog::pushButtonAdd_clicked() {
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
                model->setData(model->index(model->rowCount()-1,model_in_index),"IN"/*,Qt::DisplayRole*/);
                model->setData(model->index(model->rowCount()-1,model_locked_index),"0"/*,Qt::DisplayRole*/);
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

void LogConfigDialog::pushButtonRemove_clicked() {
     QItemSelectionModel *itemSelectionModel=this->treeView->selectionModel();

    for (int i=model->rowCount()-1;i>=0;i--) {
        if (itemSelectionModel->isRowSelected(i,QModelIndex())) {
            QVariant data=model->data(model->index(i,model_locked_index),Qt::DisplayRole);
            if (data.toString()=="1") {
                /* gesperrter Eintrag */
                return;
            }
            model->removeRow(i);
            break;//ansonsten kommte der Index durcheinander!
        }
    }
}

void LogConfigDialog::pushButtonChangeIcon_clicked() {
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
