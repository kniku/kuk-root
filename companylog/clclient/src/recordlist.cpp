#include "recordlist.h"

RecordList::RecordList()
{
    ierrorMessage="";
    ihaveError=false;
}

RecordList::~RecordList() {
	int i;
	for (i=0;i<list.size();i++) {
		RecordRow r=list.at(i);
		r.clear();
	}
	list.clear();
	rdList.clear();
}

bool        RecordList::haveError() {
    return ihaveError;
}
QString     RecordList::errorMessage() {
    return ierrorMessage;
}
void     RecordList::setErrorMessage(QString newErrorMessage) {
    ierrorMessage=newErrorMessage;
    if (newErrorMessage.size()>0) {
        ihaveError=true;
    }
}

void RecordList::appendRow(RecordRow &v) {
	list.append(v);
}

void RecordList::appendRecordDescriptor(RecordColumnDescriptor &rd) {
	rdList.append(rd);
}

const RecordColumnDescriptor& RecordList::recordColumnDescriptor(int index){
	return rdList.at(index);
}

const RecordRow& RecordList::row(int index){
	return list.at(index);
}

void RecordList::clear(){
	list.clear();
	rdList.clear();
    ihaveError=false;
    ierrorMessage="";
}

int RecordList::size(){
	return list.size();
}

int RecordList::sizeColumnDescriptor(){
	return rdList.size();
}

