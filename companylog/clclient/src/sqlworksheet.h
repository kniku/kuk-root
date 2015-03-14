#ifndef SQLWORKSHEET_H
#define SQLWORKSHEET_H

#include <QtGui/QWidget>
#include "sqltunnel.h"


namespace Ui {
    class SqlWorksheet;
}

class SqlWorksheet : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(SqlWorksheet)
public:
    explicit SqlWorksheet(QWidget *parent = 0);
    virtual ~SqlWorksheet();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SqlWorksheet *m_ui;

private slots:
	void on_pushButtonAusfuehren_clicked();
};

#endif // SQLWORKSHEET_H
