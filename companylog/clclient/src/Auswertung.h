/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h21
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : Auswertung.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef AUSWERTUNG_H
#define AUSWERTUNG_H

#include <QDialog>
#include <QWidget>
#include "ui_Auswertung.h"
#include "Options.h"

class Auswertung:public QDialog, private Ui::Auswertung {
	Q_OBJECT
public:
	Auswertung(QWidget *parent=0);
    ~Auswertung();
private:
    QString name();
    QString vName();
    QString year();
    QString month();
    QString day();

    void setName(QString s);
    void setVName(QString s);
    void setYear(QString s);
    void setMonth(QString s);
    void setDay(QString s);
public slots:
    void accept();
    void reject();
signals:
	void auswertungOk();
};



#endif
