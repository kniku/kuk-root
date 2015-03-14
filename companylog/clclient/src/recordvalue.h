#ifndef RECORDVALUE_H
#define RECORDVALUE_H

#include <QObject>
#include "Options.h"
#include <QDateTime>

class RecordValue
{
public:
	RecordValue();
	void		setValue(QVariant v);
	void		setValueString(QString v);
	void		setValueInt(int v);
	void		setValueDouble(double v);
	void		setValueDateTime(QDateTime v);
	void		setValueBlob(QByteArray v);

	QVariant	value();
	QString		valueString();
	int			valueInt();
	double		valueDouble();
	QDateTime	valueDateTime();
	QByteArray	valueBlob();
	SQL_BASETYPE type();
private:
	QVariant	ivalue;
	SQL_BASETYPE itype;
};

#endif // RECORDVALUE_H
