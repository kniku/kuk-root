#ifndef LOGACTION_H
#define LOGACTION_H

#include <QObject>

class LogAction : public QObject
{
public:
    LogAction(QObject *parent=0);
    void setName(QString name);
    void setText(QString text);
    void setIconName(QString iconname);
    void setIn(QString in);
    void setLocked(QString locked);

    QString name();
    QString text();
    QString iconName();
    QString in();
    QString locked();
private:
    QString iname;
    QString itext;
    QString iiconname;
    QString iin;
    QString ilocked;
};

#endif // LOGACTION_H
