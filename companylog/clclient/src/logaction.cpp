#include "logaction.h"

LogAction::LogAction(QObject *parent) :QObject(parent)
{
}
void LogAction::setName(QString name) {iname=name;}
void LogAction::setText(QString text) {itext=text;}
void LogAction::setIconName(QString iconname) {iiconname=iconname;}
void LogAction::setIn(QString in) {
    if (!in.compare("TRUE", Qt::CaseInsensitive) ||
        !in.compare("1", Qt::CaseInsensitive) ||
        !in.compare("YES", Qt::CaseInsensitive) ||
        !in.compare("JA", Qt::CaseInsensitive) ||
        !in.compare("IN", Qt::CaseInsensitive)) {
        iin="IN";
    } else {
        iin="OUT";
    }
}
void LogAction::setLocked(QString locked) {
    if (locked.size()==0 || locked=="0") {
        ilocked="0";
    } else {
        ilocked="1";
    }
}

QString LogAction::name() {return iname;}
QString LogAction::text() {return itext;}
QString LogAction::iconName() {return iiconname;}
QString LogAction::in() {return iin;}
QString LogAction::locked() {return ilocked;}
