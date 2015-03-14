#include "mysql5_pDriver.h"

#if (DBD_MYSQL5)

namespace klib {
namespace dbi {

// ******************************
// DbdDyn_C_Result_mysql5
// ******************************

DbdDyn_C_Result_mysql5::~DbdDyn_C_Result_mysql5()
{
}

bool DbdDyn_C_Result_mysql5::i_Next(void)
{
	return getStatement()->i_next_result();
}

bool DbdDyn_C_Result_mysql5::get_result(DataType iType, unsigned int idx, void *pData) const
{
	bool r = false;
	MYSQL_STMT *MyStmt = (MYSQL_STMT *)((DbdDyn_C_Statement_mysql5 *)m_hStmt)->getDriverInternals();
	if (MyStmt && MyStmt->bind && MyStmt->field_count > idx) {
		switch (iType) {
		case DataType_None:
			/* type request */
			*(DataType *)pData = mysqltype2dbitype(MyStmt->bind[idx].buffer_type);
			r = true;
			break;
		default:
			break;
		}
		if (r == false) {
			_LogStream.str(""); _LogStream << "get_result: invalid type or unbound result (column idx=" << idx << ")!";
			DBD_ThrowException(_LogStream.str().c_str(), NULL, NULL);
		}
	}
	return r;
}
bool DbdDyn_C_Result_mysql5::i_isNull(unsigned int idx) const
{
	bool r = false;
	MYSQL_STMT *MyStmt = (MYSQL_STMT *)((DbdDyn_C_Statement_mysql5 *)m_hStmt)->getDriverInternals();
	if (MyStmt && MyStmt->bind && MyStmt->field_count > idx) {
		if (MyStmt->bind[idx].is_null && *MyStmt->bind[idx].is_null) {
			r = true;
		}
	} else {
		_LogStream.str(""); _LogStream << "isNull: invalid or unbound result (column idx=" << idx << ")!";
		DBD_ThrowException(_LogStream.str().c_str(), NULL, NULL);
	}
	return r;
}
DataType DbdDyn_C_Result_mysql5::i_getType(unsigned int idx) const {
	DataType r = DataType_None;
	get_result(DataType_None, idx, &r);
	return r;
}

} // namespace dbi
} // namespace klib

#endif	// DBD_MYSQL5

//eof
