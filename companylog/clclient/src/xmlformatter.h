#ifndef XMLFORMATTER_H
#define XMLFORMATTER_H

#include "Options.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDateTime>

class XmlFormatter : QObject
{
	Q_OBJECT
public:
	XmlFormatter(QObject *parent=0);
	bool createSqlRequest(QString statement, va_list vl);
	QString getLastFormattedXmlString();
private:
	QString lastXmlResult;
	QDomDocument doc;
	QDomElement esm;
	bool createESMHeader(QString t);
	bool appendSqlBindings(QDomElement *parentElement, va_list vl);
	bool appendSqlBindingValue(QDomElement *parentElement, QString type, QString value);
};

#endif // XMLFORMATTER_H
