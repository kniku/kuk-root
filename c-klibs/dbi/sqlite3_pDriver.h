#include "sqlite3_Driver.h"

#if (DBD_SQLITE3)

#if DBI_SYSTEM_WIN32
//#	include <winsock2.h>
#elif DBI_SYSTEM_LINUX
#endif

#include <klibbase.h>
#include <sqlite3.h>

namespace klib {
namespace dbi {

#undef DBD_ThrowException
#define DBD_ThrowException(iSimpleMsg, iConn, iErrCode) _throw_err_sqlite3(__FILE__, __LINE__, (iConn), (iErrCode), (iSimpleMsg))
extern void _throw_err_sqlite3(char *file, int line, sqlite3 *iConn, int iErrCode, const char *iMsg);

} // namespace dbi
} // namespace klib

#endif	// DBD_SQLITE3
