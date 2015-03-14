#include "mysql5_Driver.h"

#if (DBD_MYSQL5)

#include <klibbase.h>

#if SYSTEM_WIN32
#	include <winsock2.h>
#elif SYSTEM_LINUX
#	define __stdcall
#	include <dlfcn.h>
#endif

#include <sstream>
#include <mysql.h>

using namespace std;

namespace klib {
namespace dbi {

extern ostringstream _LogStream;

#undef DBD_ThrowException
#define DBD_ThrowException(iSimpleMsg, iMySql, iStmt) _throw_err_mysql5(__FILE__, __LINE__, (iMySql), (iStmt), (iSimpleMsg))

extern void _throw_err_mysql5(char *file, int line, MYSQL *iMySql, MYSQL_STMT *iStmt, const char *iMsg);
extern DataType mysqltype2dbitype(enum_field_types iType);

} // namespace
} // namespace

#endif

// eof
