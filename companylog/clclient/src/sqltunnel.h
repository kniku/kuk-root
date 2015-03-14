#ifndef SQLTUNNEL_H
#define SQLTUNNEL_H

#include <QObject>
#include "requestobjectwait.h"
#include "xmlformatter.h"
#include "xmltorecordlist.h"

class SqlTunnel : public QObject
{
	Q_OBJECT
public:
	SqlTunnel(QObject *parent=0);
	bool sendPlainSqlStatement(QString statement,...);
	QString *lastPlainResultString();
	RecordList* lastResultAsRecordList();
private:
	RequestObjectWait requestObjectWait;
	QString plainXmlRequestString;
};

#endif // SQLTUNNEL_H
