#include "sqltunnel.h"

SqlTunnel::SqlTunnel(QObject *parent) :QObject(parent)
{

}

bool SqlTunnel::sendPlainSqlStatement(QString statement, ...) {
	bool rv=false;
	va_list vl;
	XmlFormatter xmlFormatter;

	va_start(vl,statement);

	if (vl) {
		if (xmlFormatter.createSqlRequest(statement,vl)) {
			QString requestXMLString=xmlFormatter.getLastFormattedXmlString();
			if (requestXMLString.size()>0) {
				if (requestObjectWait.sendRequestStringToServer(requestXMLString,false)) {
					rv=true;
				}
			}
		}
	}
	return rv;
}

QString *SqlTunnel::lastPlainResultString() {
	return requestObjectWait.getResultBuffer();
}


RecordList* SqlTunnel::lastResultAsRecordList() {
	XmlToRecordList xmlToRecordList;
	QString *resultB=requestObjectWait.getResultBuffer();
	if (resultB && resultB->size()>0) {
		RecordList *recordList=xmlToRecordList.translate(resultB);
		return recordList;
	}
	return 0;
}
