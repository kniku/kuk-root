#include "oracle9_Driver.h"

#if (DBD_ORACLE9)

#if DBI_SYSTEM_WIN32
//#	include <winsock2.h>
#elif DBI_SYSTEM_LINUX
#endif

#include <klibbase.h>
#include <oci.h>

namespace klib {
namespace dbi {

#define NOT_IMPL_EXCEPTION(fct)		throw Dbi_C_Exception("oracle interface: function \""#fct"\" not impl. yet!");

#undef DBD_ThrowException
#define DBD_ThrowException(iSimpleMsg, iError, iErrCode) _throw_err(__FILE__, __LINE__, (iErrCode), (iError), (iSimpleMsg))

extern void _throw_err(char *file, int line, sword iErrCode, OCIError *iError, const char *iMsg);

struct Connection_Internals {
	OCIError *pError;
	OCISvcCtx *pService;
	bool m_isAutocommit;
};

} // namespace dbi
} // namespace

#endif
