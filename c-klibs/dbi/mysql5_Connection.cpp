#include "mysql5_pDriver.h"

#if (DBD_MYSQL5)

namespace klib {
namespace dbi {

// ******************************
// DbdDyn_C_Connection_mysql5
// ******************************

void DbdDyn_C_Connection_mysql5::i_connect(void)
{
	void *pDummy = NULL;
	MYSQL *iMySql = (MYSQL *)getDriverInternals();

	pDummy = mysql_real_connect(iMySql, m_Host.c_str(), m_User.c_str(), m_Passwd.c_str(), m_Database.c_str(), 0/*port*/, NULL/*unix_socket*/, 0/*client_flag*/);
	if (pDummy == NULL) {
		DBD_ThrowException(NULL, iMySql, NULL);
	}
}

void DbdDyn_C_Connection_mysql5::i_disconnect(void)
{
	mysql_close((MYSQL *)getDriverInternals());
}

void DbdDyn_C_Connection_mysql5::i_set_autocommit(bool iSet)
{
	MYSQL *iMySql = (MYSQL *)getDriverInternals();
	my_bool r_bool = iSet;
	r_bool = mysql_autocommit(iMySql, r_bool);
	if (r_bool) {
		DBD_ThrowException(NULL, iMySql, NULL);
	}
}

void DbdDyn_C_Connection_mysql5::i_commit(void)
{	
	my_bool r = 0;
	r = mysql_commit((MYSQL *)getDriverInternals());
}

void DbdDyn_C_Connection_mysql5::i_rollback(void)
{
	my_bool r = 0;
	r = mysql_rollback((MYSQL *)getDriverInternals());
}

DbdDyn_C_Statement *DbdDyn_C_Connection_mysql5::i_create_stmt(void)
{
	return (DbdDyn_C_Statement *)new DbdDyn_C_Statement_mysql5(this);
}

void DbdDyn_C_Connection_mysql5::i_destroy_stmt(DbdDyn_C_Statement *iStmt)
{
	delete (DbdDyn_C_Statement_mysql5 *)iStmt;
}

} // namespace dbi
} // namespace

#endif

//eof
