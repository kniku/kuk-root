#ifndef LOGIDENTIFIERVIEW_H
#define LOGIDENTIFIERVIEW_H

#include "ui_logidentifierview.h"
#include <QStandardItemModel>
#include "Options.h"
#include "logmanager.h"
#include <QFileDialog>
#include <QFileInfo>

class LogIdentifierView : public QDialog, private Ui::LogIdentifierView {
    Q_OBJECT
    Q_DISABLE_COPY(LogIdentifierView)
public:
    explicit LogIdentifierView(QWidget *parent = 0);
private:
    QStandardItemModel *model;
    void setupModel();
    void fillModel();
    void copyIcon(QString fromAbsolut, QString toAbsolut);

protected:
    virtual void changeEvent(QEvent *e);
public slots:
    void accept();
    void reject();
    void pushButtonAdd_clicked();
    void pushButtonRemove_clicked();
    void pushButtonChangeIcon_clicked();
};

#endif // LOGIDENTIFIERVIEW_H
