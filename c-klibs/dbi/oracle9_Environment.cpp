#include "oracle9_pDriver.h"

#if (DBD_ORACLE9)

namespace klib {
namespace dbi {

void _throw_err(char *file, int line, sword iErrCode, OCIError *iError, const char *iMsg)
{
	sb4 errcode;
	char errbuf[1024+1] = "";
	char *errstr = errbuf;
	if (iError) {
		switch (iErrCode) {
		case OCI_SUCCESS:
			break;
		case OCI_SUCCESS_WITH_INFO:
			break;
		case OCI_NEED_DATA:
			strcpy(errbuf, "ErrorOCI_NEED_DATA");
			break;
		case OCI_NO_DATA:
			strcpy(errbuf, "ErrorOCI_NO_DATA");
			break;
		case OCI_ERROR:
			OCIErrorGet(iError, 1, 0, &errcode, (OraText *)errbuf, (ub4)sizeof(errbuf),(ub4)OCI_HTYPE_ERROR);
			break;
		case OCI_INVALID_HANDLE:
			strcpy(errbuf, "ErrorOCI_INVALID_HANDLE\n");
			break;
		case OCI_STILL_EXECUTING:
			strcpy(errbuf, "ErrorOCI_STILL_EXECUTE\n");
			break;
		case OCI_CONTINUE:
			strcpy(errbuf, "ErrorOCI_CONTINUE\n");
			break;
		default:
			break;
		}
		if (errbuf[0] == 0 && iMsg) errstr = (char *)iMsg;
		throw klib::base::Exception(errbuf, file, line, 0, 0);
	} else {
		throw klib::base::Exception(iMsg, file, line);
	}
}


// ***********************************
// DbdDyn_C_Environment_oracle9
// ***********************************

void DbdDyn_C_Environment_oracle9::i_init(void)
{
	OCIEnv *pEnv = 0;
	if (OCIEnvCreate(&pEnv, OCI_DEFAULT, (void *)0, 0, 0, 0, (size_t)0, (dvoid **)0) == OCI_SUCCESS && pEnv) {
		setDriverInternals(pEnv);
//		m_Logger->Log(klib::base::LogLevel_Trace, "oracle9::i_init = SUCCESS\n");
	} else {
		DBD_ThrowException("environment initialization failed!", 0, 0);
	}
}

void DbdDyn_C_Environment_oracle9::i_destroy(void)
{
	OCIEnv *pEnv = (OCIEnv *)getDriverInternals();
	if (pEnv) {
		if (OCIHandleFree(pEnv, OCI_HTYPE_ENV) == OCI_SUCCESS) {
			setDriverInternals(0);
//			m_Logger->Log(klib::base::LogLevel_Trace, "oracle9::i_destroy = SUCCESS\n");
		} else {
			DBD_ThrowException("environment destruction failed!", 0, 0);
		}
	}
}

DbdDyn_C_Connection *DbdDyn_C_Environment_oracle9::i_create_connection(const char *iHost, const char *iUser, const char *iPasswd, const char *iDatabase)
{
	DbdDyn_C_Connection *r = new DbdDyn_C_Connection_oracle9(this, iHost, iUser, iPasswd, iDatabase);
	OCIEnv *pEnv = (OCIEnv *)getDriverInternals();
	struct Connection_Internals *pInternals = new Connection_Internals;
	if (pEnv && pInternals) {
		OCIHandleAlloc(pEnv, (void **)&pInternals->pError, OCI_HTYPE_ERROR, 0, (dvoid **) 0);
		r->setDriverInternals(pInternals);
//		m_Logger->Log(klib::base::LogLevel_Trace, "oracle9::i_create_connection = SUCCESS\n");
	}
	return r;
}

void DbdDyn_C_Environment_oracle9::i_destroy_connection(DbdDyn_C_Connection *iConnection)
{
	struct Connection_Internals *pInternals = (Connection_Internals *)iConnection->getDriverInternals();
	if (pInternals) {
		OCIHandleFree(pInternals->pError, OCI_HTYPE_ERROR);
		delete pInternals;
		delete iConnection;
//		m_Logger->Log(klib::base::LogLevel_Trace, "oracle9::i_destroy_connection = SUCCESS\n");
	}
}

void DbdDyn_C_Environment_oracle9::i_dbiDateTime2DBMSDateTime(void *ipDst, DateTime *pSrc)
{
	char *pDst = (char *)ipDst;
	if (pDst && pSrc) {
		pDst[0]			= pSrc->Year / 100 + 100;
		pDst[1]			= pSrc->Year % 100 + 100;
		pDst[2]			= pSrc->Month;
		pDst[3]			= pSrc->Day;
		pDst[4]			= pSrc->Hour + 1;
		pDst[5]			= pSrc->Minute + 1;
		pDst[6]			= pSrc->Second + 1;
	}
}

void DbdDyn_C_Environment_oracle9::i_DBMSDateTime2dbiDateTime(DateTime *pDst, void *ipSrc)
{
	char *pSrc = (char *)ipSrc;
	if (pDst && pSrc) {
		pDst->Year = (pSrc[0] - 100) * 100 + (pSrc[1] - 100);
		pDst->Month = pSrc[2];
		pDst->Day = pSrc[3];
		pDst->Hour = pSrc[4] - 1;
		pDst->Minute = pSrc[5] - 1;
		pDst->Second = pSrc[6] - 1;
	}
}

} // namespace dbi
} // namespace klib

#endif

// eof
