#include "sqlite3_pDriver.h"

#if (DBD_SQLITE3)

namespace klib {
namespace dbi {

extern void _throw_err_sqlite3(char *file, int line, sqlite3 *iConn, int iErrCode, const char *iMsg) {
	const char *error;
	if (iConn) {
		error = sqlite3_errmsg(iConn);
		if (error == 0) error = iMsg;
		throw klib::base::Exception(error, file, line, iErrCode);
	} else {
		throw base::Exception(iMsg, file, line);
	}
}

// ******************************
// DbdDyn_C_Environment_sqlite3
// ******************************

void DbdDyn_C_Environment_sqlite3::i_init(void)
{
	// nothing to do
}

void DbdDyn_C_Environment_sqlite3::i_destroy(void)
{
	// nothing to do
}

DbdDyn_C_Connection * DbdDyn_C_Environment_sqlite3::i_create_connection(const char *iHost, const char *iUser, const char *iPasswd, const char *iDatabase)
{
	DbdDyn_C_Connection_sqlite3 *r = new DbdDyn_C_Connection_sqlite3(this, iHost, iUser, iPasswd, iDatabase);
	return r;
}

void DbdDyn_C_Environment_sqlite3::i_destroy_connection(DbdDyn_C_Connection *iConnection)
{
	delete iConnection;
}

void DbdDyn_C_Environment_sqlite3::i_dbiDateTime2DBMSDateTime(void *ipDst, DateTime *pSrc)
{
	char *pDst = (char *)ipDst;
	if (pDst && pSrc) {
		sprintf(pDst, "%04d%02d%02d%02d%02d%02d",
			pSrc->Year,
			pSrc->Month,
			pSrc->Day,
			pSrc->Hour,
			pSrc->Minute,
			pSrc->Second
		);
	}
}
void DbdDyn_C_Environment_sqlite3::i_DBMSDateTime2dbiDateTime(DateTime *pDst, void *ipSrc)
{
	char *pSrc = (char *)ipSrc;
	char _s[14+1] = "00000000000000";
	if (pDst && pSrc) {
		unsigned int l = (unsigned int )strlen(pSrc);
		if (l < 14) {
			strncpy(_s, pSrc, l);
			pSrc = _s;
		}
		sscanf(pSrc, "%04d%02d%02d%02d%02d%02d",
			&pDst->Year,
			&pDst->Month,
			&pDst->Day,
			&pDst->Hour,
			&pDst->Minute,
			&pDst->Second
		);
	}
}

} // namespace dbi
} // namespace

#endif // (DBD_SQLITE3)

// eof
