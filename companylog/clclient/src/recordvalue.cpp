#include "recordvalue.h"

RecordValue::RecordValue()
{
	itype=SQL_BASETYPE_STRING;
}

void RecordValue::setValue(QVariant v) {
	ivalue=v;
	itype=SQL_BASETYPE_STRING;
}
void		RecordValue::setValueString(QString v) {
	ivalue=QVariant(v);
	itype=SQL_BASETYPE_STRING;
}
void		RecordValue::setValueInt(int v) {
	ivalue=QVariant(v);
	itype=SQL_BASETYPE_INT;
}
void		RecordValue::setValueDouble(double v) {
	ivalue=QVariant(v);
	itype=SQL_BASETYPE_DOUBLE;
}
void		RecordValue::setValueDateTime(QDateTime v) {
	ivalue=QVariant(v);
	itype=SQL_BASETYPE_DATETIME;
}
void		RecordValue::setValueBlob(QByteArray v) {
	ivalue=QVariant(v);
	itype=SQL_BASETYPE_BLOB;
}

QVariant	RecordValue::value() {
	return ivalue;
}
QString		RecordValue::valueString() {
	return ivalue.toString();
}
int			RecordValue::valueInt() {
	return ivalue.toInt();
}
double		RecordValue::valueDouble() {
	return ivalue.toDouble();
}
QDateTime	RecordValue::valueDateTime() {
	return ivalue.toDateTime();
}
QByteArray	RecordValue::valueBlob() {
	return ivalue.toByteArray();
}

SQL_BASETYPE RecordValue::type() {
	return itype;
}
