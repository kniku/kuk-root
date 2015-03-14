#include "sqlite3_pDriver.h"
#include <boost/format.hpp>

#if (DBD_SQLITE3)

namespace klib {
namespace dbi {

// ***********************************
// DbdDyn_C_Connection_sqlite3
// ***********************************

void DbdDyn_C_Connection_sqlite3::i_connect(void)
{
	sqlite3 *pConn = 0;
	int r_int = sqlite3_open(m_Database.c_str(), &pConn);
	if (r_int != SQLITE_OK) {
		DBD_ThrowException("connection failed!", pConn, r_int);
	}
	setDriverInternals((Dbi_T_Handle )pConn);
}

void DbdDyn_C_Connection_sqlite3::i_disconnect(void)
{
	sqlite3 *pConn = (sqlite3 *)getDriverInternals();
	int r_int;
	if (pConn) {
		r_int = sqlite3_close(pConn);
		if (r_int != SQLITE_OK) {
			DBD_ThrowException("closing connection failed!", pConn, r_int);
		}
		setDriverInternals((Dbi_T_Handle )0);
	}
}

void DbdDyn_C_Connection_sqlite3::i_commit(void)
{
	// TODO
}

void DbdDyn_C_Connection_sqlite3::i_set_autocommit(bool iSet)
{
	// TODO
}

void DbdDyn_C_Connection_sqlite3::i_rollback(void)
{
	// TODO
}

DbdDyn_C_Statement *DbdDyn_C_Connection_sqlite3::i_create_stmt(void)
{
	return (DbdDyn_C_Statement *)new DbdDyn_C_Statement_sqlite3(this);
}

void DbdDyn_C_Connection_sqlite3::i_destroy_stmt(DbdDyn_C_Statement *iStmt)
{
	delete (DbdDyn_C_Statement_sqlite3 *)iStmt;
}

} // namespace dbi
} // namespace klib

#endif	// DBD_SQLITE3

// eof
