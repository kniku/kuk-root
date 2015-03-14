#include "sqlite3_pDriver.h"
#include <boost/format.hpp>

#if (DBD_SQLITE3)

namespace klib {
namespace dbi {

// ******************************
// DbdDyn_C_Statement_sqlite3
// ******************************

DbdDyn_C_Statement_sqlite3::DbdDyn_C_Statement_sqlite3(DbdDyn_C_Connection *iInterface)
: DbdDyn_C_Statement(iInterface)
{
// TODO
}

DbdDyn_C_Statement_sqlite3::~DbdDyn_C_Statement_sqlite3()
{
// TODO
}

void DbdDyn_C_Statement_sqlite3::i_Prepare(const char *iStmt)
{
	int r_int;
	sqlite3_stmt *pStmt = 0;
	sqlite3 *pConn = (sqlite3 *)this->getConnection()->getDriverInternals();
	r_int = sqlite3_prepare(pConn, iStmt, (int )strlen(iStmt), &pStmt, 0 /* OUT: Pointer to unused portion of zSql */);
	if (r_int != SQLITE_OK) {
		DBD_ThrowException("stmt prepare failed!", pConn, r_int);
	}
	setDriverInternals((Dbi_T_Handle )pStmt);
}

void DbdDyn_C_Statement_sqlite3::i_BindResults(void)
{
	int r_int;
	unsigned long i = 0;
	DbdDyn_T_Binding *pBindInfo;
	sqlite3_stmt *pStmt = (sqlite3_stmt *)getDriverInternals();

	while ((pBindInfo = (DbdDyn_T_Binding *)m_bindResultArray.GetAt(i))) {
		switch (pBindInfo->dataType) {
			case DataType_Long:
				r_int = SQLITE_OK;
				break;
			case DataType_Double:
				r_int = SQLITE_OK;
				break;
			case DataType_String:
				r_int = SQLITE_OK;
				break;
			case DataType_DateTime:
				pBindInfo->bindBuffer = new char[14+1];
				pBindInfo->isAllocBind = true;
				r_int = SQLITE_OK;
				break;
			default:
				break;
		}

		if (r_int == SQLITE_OK) {
//			m_hConnection->getEnvironment()->getLogHandler()->Log(klib::base::LogLevel_Trace, (boost::format("sqlite3::i_BindParamVa: binding param %d as %s\n") % (i + 1) % DBD_NAME_OF_DATATYPE(pBindInfo->dataType)).str());
		} else {
			DBD_ThrowException("sqlite3::i_BindResultVa failed!", (sqlite3 *)this->getConnection()->getDriverInternals(), r_int);
		}
		// get next parameter
		i++;
	}
}

void DbdDyn_C_Statement_sqlite3::i_BindResultTransfer(void)
{
	DbdDyn_T_Binding *pBind;
	sqlite3_stmt *pStmt = (sqlite3_stmt *)getDriverInternals();
	int r_int = SQLITE_OK;
//	int column_type;
	char *p;
	for (unsigned long i = 0; (pBind = (DbdDyn_T_Binding *)m_bindResultArray.GetAt(i)); i++) {
/*		column_type = sqlite3_column_type(pStmt, i);
		if (column_type == SQLITE_NULL) {
			((char *)pBind->appBuffer)[0] = 0;
		}
*/		switch (pBind->dataType) {
		case DataType_Long:
			*(int *)pBind->appBuffer = sqlite3_column_int(pStmt, i);
			break;
		case DataType_Double:
			*(double *)pBind->appBuffer = sqlite3_column_double(pStmt, i);
			break;
		case DataType_String:
			p = (char *)sqlite3_column_text(pStmt, i);
			if (p) {
				strncpy((char *)pBind->appBuffer, p, pBind->length);
			} else { // NULL
				((char *)pBind->appBuffer)[0] = 0;
			}
			break;
		case DataType_DateTime:
			p = (char *)sqlite3_column_text(pStmt, i);
			if (p) {
				m_hConnection->getEnvironment()->i_DBMSDateTime2dbiDateTime((DateTime *)pBind->appBuffer, p);
			} else { // NULL
				memset(pBind->appBuffer, 0, sizeof(DateTime));
			}
			break;
		default:
			break;
		}
		if (r_int != SQLITE_OK) {
			DBD_ThrowException("sqlite3::i_BindResultTransfer failed!", (sqlite3 *)this->getConnection()->getDriverInternals(), r_int);
		}
	}
}

void DbdDyn_C_Statement_sqlite3::i_BindParams(void)
{
	int r_int;
	unsigned long i = 0;
	DbdDyn_T_Binding *pBindInfo;
	sqlite3_stmt *pStmt = (sqlite3_stmt *)getDriverInternals();

	while ((pBindInfo = (DbdDyn_T_Binding *)m_bindParamArray.GetAt(i))) {
		switch (pBindInfo->dataType) {
			case DataType_Long:
				//r_int = sqlite3_bind_int(pStmt, i+1, *(int *)pBindInfo->appBuffer);
				r_int = SQLITE_OK;
				break;
			case DataType_Double:
				//r_int = sqlite3_bind_double(pStmt, i+1, *(double *)pBindInfo->appBuffer);
				r_int = SQLITE_OK;
				break;
			case DataType_String:
				r_int = sqlite3_bind_text(pStmt, i+1, (char *)pBindInfo->appBuffer, -1/*pBindInfo->length*/, SQLITE_STATIC);
				break;
			case DataType_DateTime:
				pBindInfo->bindBuffer = new char[14+1];
				pBindInfo->isAllocBind = true;
//				m_hConnection->getEnvironment()->i_dbiDateTime2DBMSDateTime(pBindInfo->bindBuffer, (DateTime *)pBindInfo->appBuffer);
				r_int = sqlite3_bind_text(pStmt, i+1, (char *)pBindInfo->bindBuffer, -1/*pBindInfo->length*/, SQLITE_STATIC);
				break;
			default:
				break;
		}

		if (r_int == SQLITE_OK) {
//			m_hConnection->getEnvironment()->getLogHandler()->Log(klib::base::LogLevel_Trace, (boost::format("sqlite3::i_BindParamVa: binding param %d as %s\n") % (i + 1) % DBD_NAME_OF_DATATYPE(pBindInfo->dataType)).str());
		} else {
			DBD_ThrowException("sqlite3::i_BindParamVa failed!", (sqlite3 *)this->getConnection()->getDriverInternals(), r_int);
		}
		// get next parameter
		i++;
	}
}

void DbdDyn_C_Statement_sqlite3::i_BindParamTransfer(void)
{
	DbdDyn_T_Binding *pBind;
	sqlite3_stmt *pStmt = (sqlite3_stmt *)getDriverInternals();
	int r_int = SQLITE_OK;
	for (unsigned long i = 0; (pBind = (DbdDyn_T_Binding *)m_bindParamArray.GetAt(i)); i++) {
		switch (pBind->dataType) {
		case DataType_Long:
			r_int = sqlite3_bind_int(pStmt, i+1, *(int *)pBind->appBuffer);
			break;
		case DataType_Double:
			r_int = sqlite3_bind_double(pStmt, i+1, *(double *)pBind->appBuffer);
			break;
		case DataType_String:
			// nothing to do
			break;
		case DataType_DateTime:
			m_hConnection->getEnvironment()->i_dbiDateTime2DBMSDateTime(pBind->bindBuffer, (DateTime *)pBind->appBuffer);
			break;
		default:
			break;
		}
		if (r_int != SQLITE_OK) {
			DBD_ThrowException("sqlite3::i_BindParamTransfer failed!", (sqlite3 *)this->getConnection()->getDriverInternals(), r_int);
		}
	}
}

void DbdDyn_C_Statement_sqlite3::i_Execute()
{
	sqlite3_stmt *pStmt = (sqlite3_stmt *)getDriverInternals();
	int r_int;
	m_fetch_cnt = 0;
	if (pStmt) {
		r_int = sqlite3_step(pStmt);
		switch (r_int) {
		case SQLITE_DONE:
			m_fetch_cnt = -1;
			r_int = sqlite3_reset(pStmt);
			if (r_int != SQLITE_OK) {
				DBD_ThrowException("stmt exec_reset failed!", (sqlite3 *)this->getConnection()->getDriverInternals(), r_int);
			}
			break;
		case SQLITE_ROW:
			break;
		case SQLITE_BUSY:
			// TODO: database is locked: retry ...
			// intentionally fall through
		default:
			DBD_ThrowException("stmt execution failed!", (sqlite3 *)this->getConnection()->getDriverInternals(), r_int);
			break;
		}
	}
}

void DbdDyn_C_Statement_sqlite3::i_Reset()
{
	int r_int;
	sqlite3_stmt *pStmt = (sqlite3_stmt *)getDriverInternals();
	if (pStmt) {
		r_int = sqlite3_finalize(pStmt);
		if (r_int != SQLITE_OK) {
			DBD_ThrowException("stmt reset failed!", (sqlite3 *)this->getConnection()->getDriverInternals(), r_int);
		}
		setDriverInternals((Dbi_T_Handle )0);
	}
}

class DbdDyn_C_Result *	DbdDyn_C_Statement_sqlite3::i_create_result(void) const
{
	return (DbdDyn_C_Result *) new DbdDyn_C_Result_sqlite3((DbdDyn_C_Statement *)this);
}

void DbdDyn_C_Statement_sqlite3::i_destroy_result(DbdDyn_C_Result *iResult) const
{
	delete (DbdDyn_C_Result_sqlite3 *)iResult;
}

bool DbdDyn_C_Statement_sqlite3::i_next_result(void) {
	int r_int;
	bool r = false;
	if (getFetchCnt() == 0) {
		setFetchCnt(1);
		r = true;
	} else if (getFetchCnt() < 0) {
		r = false;
	} else {
		sqlite3_stmt *pStmt = (sqlite3_stmt *)getDriverInternals();
		if (pStmt) {
			r_int = sqlite3_step(pStmt);
			switch (r_int) {
			case SQLITE_ROW:
				r = true;
				break;
			case SQLITE_DONE:
				r = false;
				break;
			default:
				DBD_ThrowException("sqlite3::i_next_result failed!", (sqlite3 *)this->getConnection()->getDriverInternals(), r_int);
				break;
			}
		}
	}
	return r;
}

} // namespace dbi
} // namespace klib

#endif	// DBD_SQLITE3

// eof
