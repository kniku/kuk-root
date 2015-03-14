#include "recordrow.h"

RecordRow::RecordRow()
{
}

RecordRow::~RecordRow()
{
	clear();
}

void RecordRow::appendColumn(RecordValue &v) {
	rowx.append(v);
}

int RecordRow::size() {
	return rowx.size();
}

void RecordRow::clear() {
	rowx.clear();
}

const RecordValue& RecordRow::column(int index) {
	return rowx.at(index);
}
