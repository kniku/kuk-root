#ifndef RECORDDESCRIPTOR_H
#define RECORDDESCRIPTOR_H

#include <QObject>
#include "Options.h"

class RecordColumnDescriptor
{
public:
	RecordColumnDescriptor();
	SQL_BASETYPE type();
	QString	name();
	void setType(SQL_BASETYPE ty);
	void setName(QString	  na);
private:
	SQL_BASETYPE		itype;
	QString				iname;
};

#endif // RECORDDESCRIPTOR_H
