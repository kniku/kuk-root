#ifndef LOGIDENTIFIER_H
#define LOGIDENTIFIER_H

#include <QObject>
#include <QStringList>
//#include "Options.h"

class LogIdentifier : public QObject
{
public:
    LogIdentifier(QObject *parent=0);
    void setIconName(QString iconname);
    void addPattern(QString pattern);
    void removePattern(QString xpattern);
    void clearPatternList();
    int patternCount();
    QString iconName();
    QString pattern(int index);
    QString fullPatternString();
    void setPatternFromFullPatternString(QString patternString);
private:
    QString iiconname;
    QStringList ipatternlist;
};

#endif // LOGIDENTIFIER_H
