#ifndef RECORDLIST_H
#define RECORDLIST_H

#include <QObject>
#include "recordrow.h"
#include "recorddescriptor.h"
#include <QList>

class RecordList
{
public:
	RecordList();
	~RecordList();
    bool        haveError();
    QString     errorMessage();
    void        setErrorMessage(QString newErrorMessage);
    void		appendRow(RecordRow &v);
	const RecordRow&	row(int index);
	int			size();
	void		clear();

	void		appendRecordDescriptor(RecordColumnDescriptor &rd);
	int			sizeColumnDescriptor();
	const RecordColumnDescriptor& recordColumnDescriptor(int index);

private:
	QList<RecordRow>list;
	QList<RecordColumnDescriptor>rdList;
    QString ierrorMessage;
    bool     ihaveError;
};

#endif // RECORDLIST_H
