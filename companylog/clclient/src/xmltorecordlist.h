#ifndef XMLTORECORDLIST_H
#define XMLTORECORDLIST_H

#include <QObject>
#include "recordlist.h"
#include "Options.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDateTime>

class XmlToRecordList : public QObject
{
public:
	XmlToRecordList(QObject *parent=0);
	RecordList *translate(QString *xmlFile);
private:
	SQL_BASETYPE getTypeFromString(QString tString);

};

#endif // XMLTORECORDLIST_H
