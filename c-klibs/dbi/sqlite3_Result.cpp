#include "sqlite3_pDriver.h"

#if (DBD_SQLITE3)

namespace klib {
namespace dbi {

// ******************************
// DbdDyn_C_Result_sqlite3
// ******************************

bool DbdDyn_C_Result_sqlite3::i_Next(void)
{
	return getStatement()->i_next_result();
}

DataType DbdDyn_C_Result_sqlite3::i_getType(unsigned int idx) const
{
	DataType r = DataType_None;
// TODO
	return r;
}

bool DbdDyn_C_Result_sqlite3::i_isNull(unsigned int idx) const
{
	bool r = false;
// TODO
	return r;
}

} // namespace dbi
} // namespace klib

#endif	// DBD_SQLITE3

// eof
