#include "DBD_Interface.h"

namespace klib {
namespace dbi {

DbdDyn_C_Connection::DbdDyn_C_Connection(DbdDyn_C_Environment *iEnv, const char *iHost, const char *iUser, const char *iPasswd, const char *iDatabase)
{
	m_DriverInternals = 0;
	m_hEnv = iEnv;
	m_Database = iDatabase ? iDatabase : "";
	m_Host = iHost ? iHost : "";
	m_User = iUser ? iUser : "";
	m_Passwd = iPasswd ? iPasswd : "";
}

} // namespace dbi
} // namespace

// eof
