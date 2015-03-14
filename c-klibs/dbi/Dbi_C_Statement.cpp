#include "pklibdbi.h"
#include <stdarg.h>
#include <boost/format.hpp>

namespace klib {
namespace dbi {

struct CStatement_Internals {
	CConnection *		m_hConnection;
	DbdDyn_C_Statement *	m_Interface;
	base::BPtrArray *		m_Results;
};

// result destroyer
static void cbDestroy_Result(void *iElem)
{
	CResult *pResult = (CResult *)base::BPtrArray::derefPtr(iElem);
	if (pResult) {
		delete pResult;
	}
}

// ***************************************************************
// construction
// ***************************************************************

CStatement::CStatement(CConnection *iConnection)
{
	m_Internals = new CStatement_Internals;
	if (m_Internals) {
		memset(m_Internals, 0, sizeof(CStatement_Internals));
		m_Internals->m_hConnection = iConnection;
		try {
			m_Internals->m_Interface = m_Internals->m_hConnection->getInterface()->i_create_stmt();
			if (m_Internals->m_Interface == 0) {
				throw base::Exception("create statement interface failed!");
			}
			m_Internals->m_Results = new base::BPtrArray(10, 0, cbDestroy_Result, base::BPtrArray::comparePtr);
			m_Internals->m_Interface->m_varsBound = false;
			m_Internals->m_Interface->m_stmtPrepared = false;
		} catch (base::Exception e) {
			HandleException(m_Internals->m_hConnection->getEnvironment(), e);
		}
	}
}

// ***************************************************************
// destructor
// ***************************************************************

CStatement::~CStatement()
{
	try {
		if (m_Internals) {
			Reset();
			if (m_Internals->m_Results) {
				delete m_Internals->m_Results;
				m_Internals->m_Results = 0;
			}
			m_Internals->m_hConnection->getInterface()->i_destroy_stmt(m_Internals->m_Interface);
			delete m_Internals;
		}
	} catch (base::Exception e) {
			HandleException(m_Internals->m_hConnection->getEnvironment(), e);
	}
}

// ***************************************************************
// getter
// ***************************************************************

CConnection *CStatement::getConnection() const
{
	if (m_Internals) return m_Internals->m_hConnection;
	else return 0;
};

DbdDyn_C_Statement * CStatement::getInterface() const
{
	if (m_Internals) return m_Internals->m_Interface;
	else return 0;
};

// ***************************************************************
// methods
// ***************************************************************

static const char *_expand(const char *iStmt) {
	static string r;
	TableDescPtr iTable;
	int p1 = 0, p2;
	if (strstr(iStmt, "*{")) {
		r = iStmt;
		while ((p1 = (int )r.find("*{", p1)) >= 0) {
			if ((p2 = (int )r.find("}", p1 + 2)) >= 0) {
				string s = r.substr(p1+2, p2 - p1 - 2);
				if ((iTable = LookupTable(s.c_str()))) {
					string s_left = r.substr(0, p1);
					BindDescPtr pBD;
					for (unsigned int i = 0; (pBD = (BindDescPtr )TableDescGet(iTable, TableDescNbindColumn, i)); i++) {
						if (i > 0) s_left = s_left + "," + s + "." + (char *)BindDescGet(pBD, BindDescNcolumnName);
						else s_left +=  s + "." + (char *)BindDescGet(pBD, BindDescNcolumnName);
					}
					p1 = (int )s_left.length();
					r = s_left + r.substr(p2 + 1 , r.length() - p2);
				} else {
					throw base::Exception((boost::format("stmt expand: table not registered: '%1%'!")%s).str().c_str());
				};
			} else break;
		}
		iStmt = r.c_str();
	}
	return iStmt;
}

void CStatement::Prepare(const char *iStmt)
{	
	try {
		if (iStmt == 0) {
			throw base::Exception("stmt prepare: no statement!");
		}
		Reset();
		if (m_Internals->m_Interface) {
			iStmt = _expand(iStmt);
//			m_Internals->m_hConnection->getEnvironment()->getLogHandler()->Log(base::LogLevel_Trace, (boost::format("CStatement::Prepare: Stmt=[%s]\n") % iStmt).str());
			m_Internals->m_Interface->i_Prepare(iStmt);
			m_Internals->m_Interface->m_stmtPrepared = true;
		}
	} catch (base::Exception e) {
			HandleException(m_Internals->m_hConnection->getEnvironment(), e);
	}
}

static void _bind_struct(base::BArray *ipBindArray, const CStdRecord *iRec) {
	DbdDyn_T_Binding bindInfo;
	BindDescPtr pBD;
	for (unsigned int i = 0; (pBD = iRec->getBindDesc(i)); i++) {
		memset(&bindInfo, 0, sizeof(bindInfo));
		bindInfo.dataType = (DataType )BindDescGet(pBD, BindDescNdataType);
		bindInfo.appBuffer = ((char *)iRec->getStdData()) + (int )BindDescGet(pBD, BindDescNcolumnOffset);
		bindInfo.length = (int )BindDescGet(pBD, BindDescNcolumnSize);
		ipBindArray->Append(&bindInfo);
	}
}

void CStatement::BindVa(BindType iBindType1, DataType iType1, void *iParam1, ...)
{
	if (m_Internals->m_Interface == 0) return;
	va_list arglist;
	// remember pointer for params
	DbdDyn_T_Binding bindInfo;
	va_start(arglist, iParam1);
	BindType iBindType = iBindType1;
	DataType iType = iType1;
	void *iParam = iParam1;
	base::BArray *pBindArray;
	while (iBindType != BindType_None) {
		if (iBindType == BindType_Param) {
			pBindArray = &m_Internals->m_Interface->m_bindParamArray;
		} else {
			pBindArray = &m_Internals->m_Interface->m_bindResultArray;
		}
		if (iType == DataType_Struct) {
			_bind_struct(pBindArray, (CStdRecord *)iParam);
		} else {
			memset(&bindInfo, 0, sizeof(bindInfo));
			bindInfo.dataType = iType;
			bindInfo.appBuffer = iParam;
			if (iType == DataType_String) {
				bindInfo.length = va_arg(arglist, int /*length*/);
			}
			pBindArray->Append(&bindInfo);
		}
		iBindType = (BindType )va_arg(arglist, int /* BindType */);
		if (iBindType != BindType_None) {
			iType = (DataType )va_arg(arglist, int /* DataType */);
			iParam = iType ? va_arg(arglist, void *) : NULL;
		}
	}
	va_end(arglist);
}

void CStatement::Execute()
{
	if (m_Internals->m_Interface == 0) return;
	try {
		if (m_Internals->m_Interface->m_varsBound == false) {
			// bind param pointers
			if (m_Internals->m_Interface->m_bindParamArray.GetCount() > 0) {
				m_Internals->m_Interface->i_BindParams();
				m_Internals->m_Interface->i_BindParamTransfer();
			}
			// bind result pointers
			if (m_Internals->m_Interface->m_bindResultArray.GetCount() > 0) {
				m_Internals->m_Interface->i_BindResults();
			}
			m_Internals->m_Interface->m_varsBound = true;
		} else {
			if (m_Internals->m_Interface->m_bindParamArray.GetCount() > 0) {
				m_Internals->m_Interface->i_BindParamTransfer();
			}
		}
		m_Internals->m_Interface->i_Execute();
	} catch (base::Exception e) {
		HandleException(m_Internals->m_hConnection->getEnvironment(), e);
	}
}

void CStatement::Reset()
{
	if (m_Internals->m_Interface == 0) return;
	if (m_Internals->m_Interface->m_stmtPrepared) {
		m_Internals->m_Interface->m_bindResultArray.RemoveAll();
		m_Internals->m_Interface->m_bindParamArray.RemoveAll();
		try {
			m_Internals->m_Interface->i_Reset();
			m_Internals->m_Interface->m_varsBound = false;
			m_Internals->m_Interface->m_stmtPrepared = false;
		} catch (base::Exception e) {
			HandleException(m_Internals->m_hConnection->getEnvironment(), e);
		}
	}
}

CResult *CStatement::createResult(void)
{
	CResult *r = new CResult(this);
	if (r) {
		m_Internals->m_Results->BInsert(r);
	}
	return r;
}

CResult *CStatement::destroyResult(CResult *iResult)
{
	if (iResult) {
		m_Internals->m_Results->BRemove(iResult);
	}
	return 0;
}

bool CStatement::Next(void) const {
	if (m_Internals->m_Interface == 0) return false;
	bool r = m_Internals->m_Interface->i_next_result();
	if (r == true) {
		m_Internals->m_Interface->i_BindResultTransfer();
	}
	return r;
}

} // namespace dbi
} // namespace klib

// eof
