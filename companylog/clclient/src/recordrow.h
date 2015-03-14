#ifndef RECORDROW_H
#define RECORDROW_H

#include <QObject>
#include <QList>
#include "recordvalue.h"

class RecordRow
{
public:
	RecordRow();
	~RecordRow();
	void appendColumn(RecordValue &v);
	const RecordValue& column(int index);
	int size();
	void clear();
private:
	QList<RecordValue>rowx;
};

#endif // RECORDROW_H
