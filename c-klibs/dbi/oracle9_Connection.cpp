#include "oracle9_pDriver.h"
#include <boost/format.hpp>

#if (DBD_ORACLE9)

namespace klib {
namespace dbi {

// ***********************************
// DbdDyn_C_Connection_oracle9
// ***********************************

void DbdDyn_C_Connection_oracle9::i_connect(void)
{
	sword r_oci;
	OCIEnv *pEnv = (OCIEnv *)m_hEnv->getDriverInternals();
	struct Connection_Internals *pInternals = (Connection_Internals *)getDriverInternals();
	if (pInternals) {
		r_oci = OCILogon(pEnv,
			pInternals->pError, &pInternals->pService,
			(OraText *)m_User.c_str(), (ub4 )m_User.length(),
			(OraText *)m_Passwd.c_str(),	(ub4 )m_Passwd.length(),
			(OraText *)m_Host.c_str(), (ub4 )m_Host.length());
		if (r_oci == OCI_SUCCESS) {
//			m_hEnv->getLogHandler()->Log(klib::base::LogLevel_Trace, "oracle9::i_connect = SUCCESS\n");
		} else {
			DBD_ThrowException("oracle9::i_connect failed!", pInternals->pError, r_oci);
		}
	}
}

void DbdDyn_C_Connection_oracle9::i_disconnect(void)
{
	sword r_oci;
	struct Connection_Internals *pInternals = (Connection_Internals *)getDriverInternals();
	if (pInternals) {
		i_rollback();
		r_oci = OCILogoff(pInternals->pService, pInternals->pError);
		if (r_oci == OCI_SUCCESS) {
//			m_hEnv->getLogHandler()->Log(klib::base::LogLevel_Trace, "oracle9::i_disconnect = SUCCESS\n");
		} else {
			DBD_ThrowException("oracle9::i_disconnect failed!", pInternals->pError, r_oci);
		}
	}
}

void DbdDyn_C_Connection_oracle9::i_commit(void)
{
	struct Connection_Internals *pInternals = (Connection_Internals *)getDriverInternals();
	sword r_oci;
	r_oci = OCITransCommit(pInternals->pService, pInternals->pError, OCI_DEFAULT);
	if (r_oci == OCI_SUCCESS) {
//		m_hEnv->getLogHandler()->Log(klib::base::LogLevel_Trace, "oracle9::i_commit = SUCCESS\n");
	} else {
		DBD_ThrowException("oracle9::i_commit failed!", pInternals->pError, r_oci);
	}
}

void DbdDyn_C_Connection_oracle9::i_set_autocommit(bool iSet)
{
	struct Connection_Internals *pInternals = (Connection_Internals *)getDriverInternals();
	pInternals->m_isAutocommit = iSet;
//	m_hEnv->getLogHandler()->Log(klib::base::LogLevel_Trace, (boost::format("oracle9::i_set_autocommit = SUCCESS (autocommit is now %s)\n") % (iSet ? "ON" : "OFF")).str());
	// using OCI_COMMIT_ON_SUCCESS flag on OCIExecute
	// -> nothing to do here
}

void DbdDyn_C_Connection_oracle9::i_rollback(void)
{
	struct Connection_Internals *pInternals = (Connection_Internals *)getDriverInternals();
	sword r_oci;
	r_oci = OCITransRollback(pInternals->pService, pInternals->pError, OCI_DEFAULT);
	if (r_oci == OCI_SUCCESS) {
//		m_hEnv->getLogHandler()->Log(klib::base::LogLevel_Trace, "oracle9::i_rollback = SUCCESS\n");
	} else {
		DBD_ThrowException("oracle9::i_rollback failed!", pInternals->pError, r_oci);
	}
}

DbdDyn_C_Statement *DbdDyn_C_Connection_oracle9::i_create_stmt(void)
{
	return (DbdDyn_C_Statement *)new DbdDyn_C_Statement_oracle9(this);
}

void DbdDyn_C_Connection_oracle9::i_destroy_stmt(DbdDyn_C_Statement *iStmt)
{
	delete (DbdDyn_C_Statement_oracle9 *)iStmt;
}

} // namespace dbi
} // namespace

#endif

// eof
