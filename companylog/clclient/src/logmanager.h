#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QList>
#include "logidentifier.h"
#include "logaction.h"
#include <QDomElement>
#include <QDomDocument>
#include "pixmapcache.h"

#define LOGIN_SONST "LOGIN mit sonstigen Eintrag"
#define LOGOUT_SONST "LOGOUT mit sonstigen Eintrag"
#define LOGIN "LOGIN"
#define LOGOUT "LOGOUT"

class LogManager : public QObject
{
public:
    LogManager(QObject *parent=0);
    ~LogManager();
    void setupIconPaths(QString logIconPath, QString logAlternativePath);
    int logActionCount();
    int logIdentifiersCount();
    LogAction* logAction(int index);
    LogAction* logActionFromText(QString text);
    void logActionAppend(LogAction *logAction);
    void logIdentifierAppend(LogIdentifier *logIdentifier);
    LogIdentifier* logIdentifier(int index);
    void clearActions();
    void clearIdentifiers();
    void setLogActionsFromXml(QString message);
    void setLogIdentifiersFromXml(QString message);
    QString createXmlString();

    LogIdentifier* searchLogIdentifierFromUserText(QString text);
    LogIdentifier* searchLogIdentifierFromIconName(QString iconName);
    QPixmap* getPixmapFromLog(QString text, bool in);
    QIcon* getIconFromLog(QString text, bool in);
    QIcon* getMixedIconFromLog(QPixmap* pixmapBackground, QString backgroundName, QString text, bool in);
    QPixmap* getMixedPixmapFromLog(QPixmap* pixmapBackground, QString backgroundName, QString text, bool in, QString *out_PixmapName);

    QPixmap* getPixmap(QString pixmapName);
    QIcon* getIcon(QString pixmapName);
    QPixmap* getMixedPixmap(QPixmap* pixmapBackground, QString backgroundName, QString pixmapName);

private:
    QList<LogIdentifier*> logIdentifiers;
    QList<LogAction*>     logActions;
    PixmapCache pixmapCache;

};

#endif // LOGMANAGER_H
