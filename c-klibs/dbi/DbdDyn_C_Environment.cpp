#include "DBD_Interface.h"

#if (DBD_ORACLE9)
#	include "oracle9_Driver.h"
#endif
#if (DBD_MYSQL5)
#	include "mysql5_Driver.h"
#endif
#if (DBD_SQLITE3)
#	include "sqlite3_Driver.h"
#endif

namespace klib {
namespace dbi {

DbdDyn_C_Environment *DbdDyn_CreateEnvironment(InterfaceType iType, klib::base::CLogger *iLogger)
{
	DbdDyn_C_Environment *r = 0;
	switch (iType) {
#if (DBD_MYSQL5)
	case InterfaceType_mysql5:
		r = new DbdDyn_C_Environment_mysql5(iLogger);
		break;
#endif
#if (DBD_ORACLE9)
	case InterfaceType_oracle9:
		r = new DbdDyn_C_Environment_oracle9(iLogger);
		break;
#endif
#if (DBD_SQLITE3)
	case InterfaceType_sqlite3:
		r = new DbdDyn_C_Environment_sqlite3(iLogger);
		break;
#endif
	default:
		throw klib::base::Exception("invalid/unsupported dbms connection type!");
		break;
	}
	return r;
}

DbdDyn_C_Environment::DbdDyn_C_Environment(const klib::base::CLogger *iLogger)
{
	m_Logger = iLogger;
	m_DriverInternals = 0;
}

} // namespace dbi
} // namespace

// eof
