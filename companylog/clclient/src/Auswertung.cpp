/********************************************************************************************************
 * PROGRAM      : companylog
 * DATE - TIME  : Sonntag 14 Mai 2006 - 10h21
 * AUTHOR       : Martin Paar ( martin.paar@gmx.at )
 * FILENAME     : Auswertung.cpp
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "Auswertung.h"

Auswertung::Auswertung(QWidget *parent) : QDialog(parent) {
	setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);

    Options *options;
    options=getGlobalOptionsObj();

    if (options) {
        setName(options->name());
        setVName(options->vName());
        setMonth(options->month());
        setYear(options->year());
        setDay(options->day());
    }
}

Auswertung::~Auswertung() {
	fprintf(stderr,"Auswertung destructor called\n");
}


void Auswertung::accept() {
    Options *options;
    options=getGlobalOptionsObj();
    if (options) {
        options->setName(name());
        options->setVName(vName());
        options->setMonth(month());
        options->setYear(year());
        options->setDay(day());
        options->saveOptionsToFile(options->getFilenameFromUser());
    }

    emit(auswertungOk());

    QDialog::accept();
}
void Auswertung::reject() {
    QDialog::reject();
}


QString Auswertung::vName() {return editVname->text();}
QString Auswertung::name() {return editName->text();}
QString Auswertung::year() {return editYear->text();}
QString Auswertung::month() {return editMonth->text();}
QString Auswertung::day() {return editDay->text();}

void Auswertung::setVName(QString s) {editVname->setText(s);}
void Auswertung::setName(QString s) {editName->setText(s);}
void Auswertung::setYear(QString s) {editYear->setText(s);}
void Auswertung::setMonth(QString s) {editMonth->setText(s);}
void Auswertung::setDay(QString s) {editDay->setText(s);}


/* EOF */
