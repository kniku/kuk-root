#include "recorddescriptor.h"

RecordColumnDescriptor::RecordColumnDescriptor()
{
}
SQL_BASETYPE RecordColumnDescriptor::type() {
	return itype;
}
QString	RecordColumnDescriptor::name() {
	return iname;
}
void RecordColumnDescriptor::setType(SQL_BASETYPE ty) {
	itype=ty;
}
void RecordColumnDescriptor::setName(QString	  na) {
	iname=na;
}

