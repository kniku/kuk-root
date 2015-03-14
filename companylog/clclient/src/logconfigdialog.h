#ifndef LOGCONFIGDIALOG_H
#define LOGCONFIGDIALOG_H

#include <QtGui/QDialog>
#include "ui_logconfigdialog.h"
#include <QStandardItemModel>
#include "Options.h"
#include "logmanager.h"
#include <QFileDialog>
#include <QFileInfo>

class LogConfigDialog : public QDialog, private Ui::LogConfigDialog {
    Q_OBJECT
    Q_DISABLE_COPY(LogConfigDialog)
public:
    explicit LogConfigDialog(QWidget *parent = 0);
    virtual ~LogConfigDialog();

protected:
    virtual void changeEvent(QEvent *e);
private:
    QStandardItemModel *model;
    void setupModel();
    void fillModel();
    void copyIcon(QString fromAbsolut, QString toAbsolut);
public slots:
    void accept();
    void reject();
    void pushButtonAdd_clicked();
    void pushButtonRemove_clicked();
    void pushButtonChangeIcon_clicked();
signals:
    void LogConfigChanged();
};

#endif // LOGCONFIGDIALOG_H
