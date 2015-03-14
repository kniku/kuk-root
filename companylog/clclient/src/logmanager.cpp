#include "logmanager.h"

LogManager::LogManager(QObject *parent) : QObject(parent)
{
}

void LogManager::setupIconPaths(QString logIconPath, QString logAlternativePath) {
    pixmapCache.setPixmapPath(logIconPath);
    pixmapCache.setPixmapAlternativePath(logAlternativePath);
}

void LogManager::clearActions() {
    foreach(LogAction* item, logActions) {
        if (item) {
            delete item;
        }
    }
    logActions.clear();
}

void LogManager::clearIdentifiers() {
    foreach(LogIdentifier* item, logIdentifiers) {
        if (item) {
            delete item;
        }
    }
    logIdentifiers.clear();
}

LogManager::~LogManager()
{
    clearActions();
    clearIdentifiers();
}

int LogManager::logActionCount() {return logActions.count();}
int LogManager::logIdentifiersCount() {return logIdentifiers.count();}
LogAction* LogManager::logAction(int index) {return logActions.value(index);}

LogAction* LogManager::logActionFromText(QString text) {
    int count=logActionCount();
    for (int i=0;i<count;i++) {
        if (logAction(i)->text()==text) {
            return logAction(i);
        }
    }
    return 0;
}

void LogManager::logActionAppend(LogAction *logAction) {
    if (logAction) {
        logActions.append(logAction);
    }
}
void LogManager::logIdentifierAppend(LogIdentifier *logIdentifier) {
    if (logIdentifier) {
        logIdentifiers.append(logIdentifier);
    }
}

LogIdentifier* LogManager::logIdentifier(int index) {return logIdentifiers.value(index);}

/*

  <logoptions>
        <logactions>
                <entry name="Mittagessen" text="Mittagessen" icon="mittag.png" in="0" />
                <entry name="Pause" text="Pause" icon="rauch.png" in="0" />
                <entry name="Internet" text="www" icon="www.png" in="0" />
                <entry name="gr. Besprechungsraum" text="gr. Besprechungsraum" icon="besprechung.png" in="1" />
                <entry name="kl. Besprechungsraum" text="kl. Besprechungsraum" icon="klbesprechung.png" in="1" />
                <entry name="Wartungsraum" text="Wartungsraum" icon="wartung.png" in="1" />
                <entry name="Jourfix" text="Jourfix" icon="jourfix.png" in="1" locked="1/0"/>
        </logactions>
        <logidetifiers>
                <entry icon="www.png">
                        <token pattern="*www*"/>
                        <token pattern="*Internet*"/>
                        <token pattern="*surfen*"/>
                </entry>
                <entry icon="mittag.png">
                        <token pattern="*mittag*"/>
                        <token pattern="*essen*"/>
                </entry>
                <entry icon="rauch.png">
                        <token pattern="*pause*"/>
                        <token pattern="*rauch*"/>
                </entry>
                <entry icon="binbei.png">
                        <token pattern="*bei *"/>
                </entry>
                <entry icon="wartung.png">
                        <token pattern="*wartung*"/>
                </entry>
                <entry icon="klbesprechung.png">
                        <token pattern="*kl.*"/>
                        <token pattern="*kleiner*"/>
                        <token pattern="*k.*"/>
                        <token pattern="*kl *"/>
                </entry>
                <entry icon="besprechung.png">
                        <token pattern="*bespr*"/>
                        <token pattern="*bspr.*"/>
                        <token pattern="*bspr*"/>
                        <token pattern="*raum*"/>
                        <token pattern="*b.r.*"/>
                        <token pattern="*BR *"/>
                        <token pattern="*BR.*"/>
                </entry>
                <entry icon="jourfix.png">
                        <token pattern="*jour*"/>
                        <token pattern="*fix*"/>
                        <token pattern="*JF*"/>
                        <token pattern="*J.F.*"/>
                </entry>
        </logidetifiers>
</logoptions>

*/
QString LogManager::createXmlString() {
    QString rv="";

    QDomDocument doc;
    QDomElement logoptions=doc.createElement("logoptions");
    QDomElement logactions=doc.createElement("logactions");
    QDomElement logidetifiers=doc.createElement("logidetifiers");
    doc.appendChild(logoptions);
    logoptions.appendChild(logactions);
    logoptions.appendChild(logidetifiers);

    int count=logActions.count();
    for (int i=0;i<count;i++) {
        LogAction *logAction=logActions.value(i);
        if (logAction) {
            QDomElement entry=doc.createElement("entry");
            entry.setAttribute("name",logAction->name());
            entry.setAttribute("text",logAction->text());
            entry.setAttribute("icon",logAction->iconName());
            entry.setAttribute("in",logAction->in());
            entry.setAttribute("locked",logAction->locked());
            logactions.appendChild(entry);
        }
    }
    count=logIdentifiers.count();
    for (int i=0;i<count;i++) {
        LogIdentifier *logIdentifier=logIdentifiers.value(i);
        if (logIdentifier) {
            QDomElement entry=doc.createElement("entry");
            entry.setAttribute("icon",logIdentifier->iconName());
            int xcount=logIdentifier->patternCount();
            for (int x=0;x<xcount;x++) {
                QString pattern=logIdentifier->pattern(x);
                if (pattern.size()>0) {
                    QDomElement token=doc.createElement("token");
                    token.setAttribute("pattern",pattern);
                    entry.appendChild(token);
                }
            }
            logidetifiers.appendChild(entry);
        }
    }
    rv=doc.toString();
    fprintf(stderr,"LogManager-String:\n%s\n",rv.toAscii().data());
    return rv;
}



void LogManager::setLogActionsFromXml(QString message) {
    clearActions();
    //pixmapCache.clear();

    QDomDocument doc( "AdBookML" );
    doc.setContent(message);
    QDomElement root = doc.documentElement();
    QDomNodeList logactions=root.elementsByTagName("logactions");
    if (logactions.count()>0) {
        QDomNode logaction0_node=logactions.item(0);
        if (logaction0_node.isElement()) {
            QDomElement logaction0 = logaction0_node.toElement();
            QDomNodeList entrys=logaction0.elementsByTagName("entry");
            for (int i=0;i<entrys.count();i++) {
                QDomNode entry_node=entrys.item(i);
                if (entry_node.isElement()) {
                    QDomElement entry=entry_node.toElement();
                    QString name=entry.attribute("name","");
                    QString text=entry.attribute("text","");
                    QString icon=entry.attribute("icon","");
                    QString in=entry.attribute("in","");
                    QString locked=entry.attribute("locked","");
                    if (name.size()>0) {
                        LogAction *logAction=new LogAction();
                        if (logAction) {
                            logAction->setName(name);
                            logAction->setText(text);
                            logAction->setIconName(icon);
                            logAction->setIn(in);
                            logAction->setLocked(locked);
                            logActionAppend(logAction);
                        }
                    }
                }
            }
        }
    }
}

void LogManager::setLogIdentifiersFromXml(QString message) {
    clearIdentifiers();
    //pixmapCache.clear();

    QDomDocument doc( "AdBookML" );
    doc.setContent(message);
    QDomElement root = doc.documentElement();
    QDomNodeList logidentifiers=root.elementsByTagName("logidetifiers");
    if (logidentifiers.count()>0) {
        QDomNode logidentifier0_node=logidentifiers.item(0);
        if (logidentifier0_node.isElement()) {
            QDomElement logidentifier0 = logidentifier0_node.toElement();
            QDomNodeList entrys=logidentifier0.elementsByTagName("entry");
            for (int i=0;i<entrys.count();i++) {
                QDomNode entry_node=entrys.item(i);
                if (entry_node.isElement()) {
                    QDomElement entry=entry_node.toElement();
                    QString icon=entry.attribute("icon","");
                    QDomNodeList tokens=entry.elementsByTagName("token");

                    if (tokens.count()>0) {
                        LogIdentifier *logIdentifier=new LogIdentifier();
                        if (logIdentifier) {
                            logIdentifier->setIconName(icon);
                            logIdentifierAppend(logIdentifier);

                            for (int j=0;j<tokens.count();j++) {
                                QDomNode token_node=tokens.item(j);
                                if (token_node.isElement()) {
                                    QDomElement token=token_node.toElement();
                                    QString pattern=token.attribute("pattern","");
                                    if (pattern.size()>0) {
                                        logIdentifier->addPattern(pattern);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

QPixmap* LogManager::getPixmapFromLog(QString text, bool in) {
    LogIdentifier* logIdentifier=searchLogIdentifierFromUserText(text);

    if (logIdentifier && logIdentifier->iconName().size()>0) {
        return pixmapCache.getPixmap(logIdentifier->iconName());
    }
    LogAction *logActionIn=logActionFromText(LOGIN);
    LogAction *logActionOut=logActionFromText(LOGOUT);
    if (logActionIn && logActionOut) {
        if (in) {
            return pixmapCache.getPixmap(logActionIn->iconName());
        } else {
            return pixmapCache.getPixmap(logActionOut->iconName());
        }
    }
    return pixmapCache.getPixmap("unknown.jpg");
}

QIcon* LogManager::getIconFromLog(QString text, bool in) {
    LogIdentifier* logIdentifier=searchLogIdentifierFromUserText(text);
    if (logIdentifier && logIdentifier->iconName().size()>0) {
        return pixmapCache.getIcon(logIdentifier->iconName());
    }
    LogAction *logActionIn=logActionFromText(LOGIN);
    LogAction *logActionOut=logActionFromText(LOGOUT);
    if (logActionIn && logActionOut) {
        if (in) {
            return pixmapCache.getIcon(logActionIn->iconName());
        } else {
            return pixmapCache.getIcon(logActionOut->iconName());
        }
    }
    return pixmapCache.getIcon("unknown.jpg");
}

QPixmap* LogManager::getPixmap(QString pixmapName) {
    return pixmapCache.getPixmap(pixmapName);
}

QIcon* LogManager::getIcon(QString pixmapName) {
    return pixmapCache.getIcon(pixmapName);
}

QPixmap* LogManager::getMixedPixmap(QPixmap* pixmapBackground, QString backgroundName, QString pixmapName) {
    QPixmap *pixmapLog=0;
    if (pixmapCache.contains(backgroundName+pixmapName)) {
        return pixmapCache.getPixmap(backgroundName+pixmapName);
    }
    pixmapLog=pixmapCache.getPixmap(pixmapName);
    if (pixmapLog && pixmapBackground) {
        pixmapCache.insertMergePixmap(pixmapBackground, pixmapLog, backgroundName+pixmapName, 0, 1.4);
        return pixmapCache.getPixmap(backgroundName+pixmapName);
    }
    return pixmapCache.getPixmap("unknown.jpg");
}

QIcon* LogManager::getMixedIconFromLog(QPixmap* pixmapBackground, QString backgroundName, QString text, bool in) {
    QString out_PixmapName;
    if (getMixedPixmapFromLog(pixmapBackground, backgroundName, text, in, &out_PixmapName)) {
        if (out_PixmapName.size()>0) {
            return pixmapCache.getIcon(out_PixmapName);
        }
    }
    return pixmapCache.getIcon("unknown.jpg");
}

QPixmap* LogManager::getMixedPixmapFromLog(QPixmap* pixmapBackground, QString backgroundName, QString text, bool in, QString *out_PixmapName) {
    QPixmap* pixmapLog=0;
    QString  pixmapLog_IconName;
    LogAction *logActionIn=logActionFromText(LOGIN);
    LogAction *logActionOut=logActionFromText(LOGOUT);

    if (logActionIn && logActionOut) {
        LogIdentifier* logIdentifier=searchLogIdentifierFromUserText(text);
        if (logIdentifier && logIdentifier->iconName().size()>0) {
            if (pixmapCache.contains(backgroundName+logIdentifier->iconName())) {
                if (out_PixmapName) {*out_PixmapName=backgroundName+logIdentifier->iconName();}
                return pixmapCache.getPixmap(backgroundName+logIdentifier->iconName());
            }
            pixmapLog=pixmapCache.getPixmap(logIdentifier->iconName());
            pixmapLog_IconName=logIdentifier->iconName();
        }
        if (!pixmapLog) {
            if (in) {
                if (pixmapCache.contains(backgroundName+logActionIn->iconName())) {
                    if (out_PixmapName) {*out_PixmapName=backgroundName+logActionIn->iconName();}
                    return pixmapCache.getPixmap(backgroundName+logActionIn->iconName());
                }
                pixmapLog=pixmapCache.getPixmap(logActionIn->iconName());
                pixmapLog_IconName=logActionIn->iconName();
            } else {
                if (pixmapCache.contains(backgroundName+logActionOut->iconName())) {
                    if (out_PixmapName) {*out_PixmapName=backgroundName+logActionOut->iconName();}
                    return pixmapCache.getPixmap(backgroundName+logActionOut->iconName());
                }
                pixmapLog=pixmapCache.getPixmap(logActionOut->iconName());
                pixmapLog_IconName=logActionOut->iconName();
            }
        }
        if (pixmapLog && pixmapBackground) {
            pixmapCache.insertMergePixmap(pixmapBackground, pixmapLog, backgroundName+pixmapLog_IconName, 0, 1.4);
            if (out_PixmapName) {*out_PixmapName=backgroundName+pixmapLog_IconName;}
            return pixmapCache.getPixmap(backgroundName+pixmapLog_IconName);
        }
        if (out_PixmapName) {*out_PixmapName="unknown.jpg";}
    }
    return pixmapCache.getPixmap("unknown.jpg");
}

LogIdentifier* LogManager::searchLogIdentifierFromUserText(QString text) {
    int count=logIdentifiers.count();

    /* zuerst exakten Match suchen */
    for(int i=0;i<count;i++) {
        LogIdentifier* logIdentifier=logIdentifiers.value(i);
        if (logIdentifier) {
            int count_token=logIdentifier->patternCount();
            for(int t=0;t<count_token;t++) {
                if (logIdentifier->pattern(t)==text) {
                    return logIdentifier;
                }
            }
        }
    }
    /* danach per Wildcard suchen */
    for(int i=0;i<count;i++) {
        LogIdentifier* logIdentifier=logIdentifiers.value(i);
        if (logIdentifier) {
            int count_token=logIdentifier->patternCount();
            for(int t=0;t<count_token;t++) {
                QRegExp rx(logIdentifier->pattern(t));
                rx.setPatternSyntax(QRegExp::Wildcard);
                rx.setCaseSensitivity(Qt::CaseInsensitive);
                if (rx.exactMatch(text)) {
                    return logIdentifier;
                }
            }
        }
    }
    return 0;
}

LogIdentifier* LogManager::searchLogIdentifierFromIconName(QString iconName) {
    int count=logIdentifiers.count();
    for(int i=0;i<count;i++) {
        LogIdentifier* logIdentifier=logIdentifiers.value(i);
        if (logIdentifier && logIdentifier->iconName()==iconName) {
            return logIdentifier;
        }
    }
    return 0;
}
