#include "logidentifier.h"

LogIdentifier::LogIdentifier(QObject *parent) :QObject(parent)
{
}

void LogIdentifier::setIconName(QString iconname) {iiconname=iconname;}
void LogIdentifier::addPattern(QString pattern) { ipatternlist.append(pattern);}
void LogIdentifier::removePattern(QString xpattern) {
    int count=patternCount();
    for (int i=count-1;i>=0;i--) {
        if (pattern(i)==xpattern) {
            ipatternlist.removeAt(i);
        }
    }
}
QString LogIdentifier::fullPatternString() {
    int count=patternCount();
    QString rv="";
    for (int i=0;i<count;i++) {
        if (rv.size()>0) {
            rv+=",";
        }
        rv+=pattern(i);
    }
    return rv;
}

void LogIdentifier::setPatternFromFullPatternString(QString patternString) {
    clearPatternList();
    QStringList xx=patternString.split(",");
    int count=xx.size();
    for (int i=0;i<count;i++) {
        if (xx.value(i).size()>0) {
            QString pat=xx.value(i).simplified();
            if (pat.size()>0) {
                addPattern(pat);
            }
        }
    }
}

void LogIdentifier::clearPatternList() {ipatternlist.clear();}
int LogIdentifier::patternCount() {return ipatternlist.size();}
QString LogIdentifier::iconName() {return iiconname;}
QString LogIdentifier::pattern(int index) {return ipatternlist.value(index);}
