#include "oracle9_pDriver.h"
#include <boost/format.hpp>

#if (DBD_ORACLE9)

namespace klib {
namespace dbi {

// ******************************
// DbdDyn_C_Statement_oracle9
// ******************************

DbdDyn_C_Statement_oracle9::DbdDyn_C_Statement_oracle9(DbdDyn_C_Connection *iInterface)
: DbdDyn_C_Statement(iInterface)
{
	OCIStmt *stmthp;
	OCIEnv *pEnv = (OCIEnv *)m_hConnection->getEnvironment()->getDriverInternals();
	OCIHandleAlloc(pEnv, (dvoid **)&stmthp, OCI_HTYPE_STMT, 0, 0);
	setDriverInternals(stmthp);
}

DbdDyn_C_Statement_oracle9::~DbdDyn_C_Statement_oracle9()
{
	OCIStmt *stmthp = (OCIStmt *)getDriverInternals();
	OCIHandleFree(stmthp, OCI_HTYPE_STMT);
}

void DbdDyn_C_Statement_oracle9::i_Prepare(const char *iStmt)
{
	struct Connection_Internals *pConnInt = (Connection_Internals *)m_hConnection->getDriverInternals();
	sword r_oci = OCIStmtPrepare((OCIStmt *)getDriverInternals(), pConnInt->pError,
						(const OraText *)iStmt, (ub4 )strlen(iStmt),
						OCI_NTV_SYNTAX, OCI_DEFAULT);
	if (r_oci == OCI_SUCCESS) {
//		m_hConnection->getEnvironment()->getLogHandler()->LogVa(klib::base::LogLevel_Trace, "oracle9::i_Prepare: [%s]\n", iStmt);
//		m_hConnection->getEnvironment()->getLogHandler()->Log(klib::base::LogLevel_Trace, (boost::format("oracle9::i_Prepare: [%s]\n") % iStmt).str());
//		m_hConnection->getEnvironment()->getLogHandler()->Log(klib::base::LogLevel_Trace, "oracle9::i_Prepare = SUCCESS\n");
	} else {
		DBD_ThrowException("oracle9::i_Prepare failed!", pConnInt->pError, r_oci);
	}
}

void DbdDyn_C_Statement_oracle9::i_BindResults(void)
{
	struct Connection_Internals *pConnInt = (Connection_Internals *)m_hConnection->getDriverInternals();
	OCIStmt *pStmt = (OCIStmt *)getDriverInternals();
	OCIDefine *pDef = 0;
	unsigned long i = 0;
	sword r_oci;
	sb4 buffer_len = 0;
	ub2 buffer_type = 0;
	DbdDyn_T_Binding *pBindInfo;

	while ((pBindInfo = (DbdDyn_T_Binding *)m_bindResultArray.GetAt(i))) {
		switch (pBindInfo->dataType) {
			case DataType_Long:
				pBindInfo->bindBuffer = pBindInfo->appBuffer;//iParam1;
				buffer_len = sizeof(long);
				buffer_type = SQLT_INT;
				break;
			case DataType_Double:
				pBindInfo->bindBuffer = pBindInfo->appBuffer;//iParam1;
				buffer_len = sizeof(double);
				buffer_type = SQLT_FLT;
				break;
			case DataType_String:
				pBindInfo->bindBuffer = pBindInfo->appBuffer;//iParam1;
				buffer_len = pBindInfo->length;//(int )va_arg(args, int);
				buffer_type = SQLT_STR;
				break;
			case DataType_DateTime:
				pBindInfo->bindBuffer = new char[7];
				pBindInfo->isAllocBind = true;
				buffer_len = 7;
				buffer_type = SQLT_DAT;
				break;
			default:
				DBD_ThrowException("oracle9::i_BindResultVa failed: unsupported column type!", pConnInt->pError, r_oci);
				break;
		}
		pBindInfo->nullIndicator = new sb2;
		r_oci = OCIDefineByPos(pStmt, &pDef, pConnInt->pError, (ub4 )i + 1,
                       pBindInfo->bindBuffer,
                       buffer_len,
                       buffer_type,
					   pBindInfo->nullIndicator,
                       0,
                       0,
                       OCI_DEFAULT);
		if (r_oci == OCI_SUCCESS) {
//			m_hConnection->getEnvironment()->getLogHandler()->Log(klib::base::LogLevel_Trace, (boost::format("oracle9::i_BindResultVa: binding result %d as %s\n") % (i + 1) % DBD_NAME_OF_DATATYPE(pBindInfo->dataType)).str());
		} else {
			DBD_ThrowException("oracle9::i_BindResultVa failed!", pConnInt->pError, r_oci);
		}
		// get next parameter
		i++;
	}
}

void DbdDyn_C_Statement_oracle9::i_BindResultTransfer(void)
{
	DbdDyn_T_Binding *pBind;
	for (unsigned long i = 0; (pBind = (DbdDyn_T_Binding *)m_bindResultArray.GetAt(i)); i++) {
		switch (pBind->dataType) {
		case DataType_Long:
			if (*(sb2 *)pBind->nullIndicator == -1) {
				*(long *)pBind->appBuffer = 0;
			}
			break;
		case DataType_Double:
			if (*(sb2 *)pBind->nullIndicator == -1) {
				*(double *)pBind->appBuffer = 0;
			}
			break;
		case DataType_String:
			if (*(sb2 *)pBind->nullIndicator == -1) {
				*(char *)pBind->appBuffer = 0;
			}
			break;
		case DataType_DateTime:
			if (*(sb2 *)pBind->nullIndicator == -1) {
				memset(pBind->appBuffer, 0, sizeof(DateTime));
			} else {
				m_hConnection->getEnvironment()->i_DBMSDateTime2dbiDateTime((DateTime *)pBind->appBuffer, pBind->bindBuffer);
			}
			break;
		default:
			break;
		}
	}
}

void DbdDyn_C_Statement_oracle9::i_BindParams(void)
{
	struct Connection_Internals *pConnInt = (Connection_Internals *)m_hConnection->getDriverInternals();
	OCIStmt *pStmt = (OCIStmt *)getDriverInternals();
	unsigned long i = 0;
	sword r_oci;
	sb4 buffer_len = 0;
	ub2 buffer_type = 0;
	OCIBind      *pBind;
	DbdDyn_T_Binding *pBindInfo;

	while ((pBindInfo = (DbdDyn_T_Binding *)m_bindParamArray.GetAt(i))) {
		switch (pBindInfo->dataType) {
			case DataType_Long:
				pBindInfo->bindBuffer = pBindInfo->appBuffer;//iParam1;
				buffer_len = sizeof(long);
				buffer_type = SQLT_INT;
				break;
			case DataType_Double:
				pBindInfo->bindBuffer = pBindInfo->appBuffer;//iParam1;
				buffer_len = sizeof(double);
				buffer_type = SQLT_FLT;
				break;
			case DataType_String:
				pBindInfo->bindBuffer = pBindInfo->appBuffer;//iParam1;
				buffer_len = (sb4 )pBindInfo->length;//va_arg(args, int /*length*/);
				buffer_type = SQLT_STR;
				break;
			case DataType_DateTime:
				pBindInfo->bindBuffer = new char[7];
				pBindInfo->isAllocBind = true;
				buffer_len = 7;
//				m_hConnection->getEnvironment()->i_dbiDateTime2DBMSDateTime(pBindInfo->bindBuffer, (DateTime *)pBindInfo->appBuffer/*(klib::DateTime *)iParam1*/);
				buffer_type = SQLT_DAT;
				break;
			default:
				break;
		}
		r_oci = OCIBindByPos(pStmt, &pBind, pConnInt->pError, (ub4 )i + 1,
							pBindInfo->bindBuffer, buffer_len,
							buffer_type,
							0,//x1,
							0,//x2,
							0,//x3,
							0,
							0, 
							OCI_DEFAULT);

		if (r_oci == OCI_SUCCESS) {
//			m_hConnection->getEnvironment()->getLogHandler()->Log(klib::base::LogLevel_Trace, (boost::format("oracle9::i_BindParamVa: binding param %d as %s\n") % (i + 1) % DBD_NAME_OF_DATATYPE(pBindInfo->dataType)).str());
		} else {
			DBD_ThrowException("oracle9::i_BindParamVa failed!", pConnInt->pError, r_oci);
		}
		// get next parameter
		i++;
	}
}

void DbdDyn_C_Statement_oracle9::i_BindParamTransfer(void)
{
	DbdDyn_T_Binding *pBind;
	for (unsigned long i = 0; (pBind = (DbdDyn_T_Binding *)m_bindParamArray.GetAt(i)); i++) {
		switch (pBind->dataType) {
		case DataType_Long:
		case DataType_Double:
			// nothing to do
			break;
		case DataType_String:
			// TODO correct string length for the new string?
			break;
		case DataType_DateTime:
			m_hConnection->getEnvironment()->i_dbiDateTime2DBMSDateTime(pBind->bindBuffer, (DateTime *)pBind->appBuffer);
			break;
		default:
			break;
		}
	}
}

void DbdDyn_C_Statement_oracle9::i_Execute()
{
	struct Connection_Internals *pConnInt = (Connection_Internals *)m_hConnection->getDriverInternals();
	OCIStmt *pStmt = (OCIStmt *)getDriverInternals();
	sword r_oci;
	m_fetch_cnt = 0;
	r_oci = OCIStmtExecute(pConnInt->pService, pStmt, pConnInt->pError,
                       1,
                       0,
                       0,
                       0,
					   pConnInt->m_isAutocommit ? OCI_COMMIT_ON_SUCCESS|OCI_DEFAULT : OCI_DEFAULT);
	if (r_oci == OCI_SUCCESS) {
//		m_hConnection->getEnvironment()->getLogHandler()->Log(klib::base::LogLevel_Trace, "oracle9::i_Execute = SUCCESS\n");
	} else if (r_oci == OCI_NO_DATA) {
		m_fetch_cnt = -1;
	} else {
		DBD_ThrowException("oracle9::i_Execute failed!", pConnInt->pError, r_oci);
	}
}

void DbdDyn_C_Statement_oracle9::i_Reset()
{
//	NOT_IMPL_EXCEPTION(DbdDyn_C_Statement_oracle9)
	// TODO?
}

class DbdDyn_C_Result *	DbdDyn_C_Statement_oracle9::i_create_result(void) const
{
	return (DbdDyn_C_Result *) new DbdDyn_C_Result_oracle9((DbdDyn_C_Statement *)this);
}

void DbdDyn_C_Statement_oracle9::i_destroy_result(DbdDyn_C_Result *iResult) const
{
	delete (DbdDyn_C_Result_oracle9 *)iResult;
}

bool DbdDyn_C_Statement_oracle9::i_next_result(void) {
	bool r = false;
	if (getFetchCnt() == 0) {
		setFetchCnt(1);
		return true;
	} 
	if (getFetchCnt() < 0) {
		return false;
	} 
	OCIStmt *pStmt = (OCIStmt *)getDriverInternals();
	struct Connection_Internals *pConnInt = (Connection_Internals *)getConnection()->getDriverInternals();
	sword r_oci;
	r_oci = OCIStmtFetch(pStmt, pConnInt->pError, (ub4 )1, OCI_FETCH_NEXT, OCI_DEFAULT);
	if (r_oci == OCI_SUCCESS) {
		r = true;
		//m_hConnection->getEnvironment()->getLogHandler()->Log(Logger::LogLevel_Trace, "oracle9::i_Execute = SUCCESS\n");
	} else if (r_oci == OCI_NO_DATA) {
		// eof
	} else {
		DBD_ThrowException("oracle9::i_Next failed!", pConnInt->pError, r_oci);
	}
	return r;
}

} // namespace
} // namespace

#endif

// eof
