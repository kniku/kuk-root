#include "pklibdbi.h"
#include <string>

namespace klib {
namespace dbi {

struct CConnection_Internals {
	CEnvironment *		m_hEnv;
	DbdDyn_C_Connection *	m_Interface;
	bool					m_flagAutocommit;
	bool					m_isConnected;
	base::BPtrArray *		m_Statements;
};

// statement destroyer
static void cbDestroy_Statement(void *iElem)
{
	CStatement *pStmt = (CStatement *)base::BPtrArray::derefPtr(iElem);
	if (pStmt) {
		delete pStmt;
	}
}

#define DB_INTERFACE	(m_Internals->m_Interface)

// ***************************************************************
// construction
// ***************************************************************

CConnection::CConnection(CEnvironment *iEnv, const char *iHost, const char *iUser, const char *iPasswd, const char *iDatabase)
{
	m_Internals = new CConnection_Internals;
	if (m_Internals) {
		memset(m_Internals, 0, sizeof(CConnection_Internals));
		m_Internals->m_hEnv = iEnv;
		try {
			m_Internals->m_Interface = m_Internals->m_hEnv->getInterface()->i_create_connection(iHost, iUser, iPasswd, iDatabase);
			m_Internals->m_Statements = new base::BPtrArray(10, 0, cbDestroy_Statement, base::BPtrArray::comparePtr);
		} catch (base::Exception e) {
			HandleException(getEnvironment(), e);
		}
	}
}

// ***************************************************************
// destruction
// ***************************************************************

CConnection::~CConnection()
{
	if (m_Internals && m_Internals->m_Interface) {
		try {
			if (m_Internals) {
				if (m_Internals->m_Statements) {
					delete m_Internals->m_Statements;
					m_Internals->m_Statements = 0;
				}
				if (isConnected()) {
					DB_INTERFACE->i_disconnect();
				}
				m_Internals->m_hEnv->getInterface()->i_destroy_connection(m_Internals->m_Interface);
				delete m_Internals;
			}
		} catch (base::Exception e) {
			HandleException(getEnvironment(), e);
		}
	}
}

// ***************************************************************
// getter
// ***************************************************************

CEnvironment *CConnection::getEnvironment() const
{
	if (m_Internals) return m_Internals->m_hEnv;
	else return 0;
};

DbdDyn_C_Connection *CConnection::getInterface() const
{
	if (m_Internals) return m_Internals->m_Interface;
	else return 0;
};

bool CConnection::getAutocommit() const
{
	if (m_Internals) return m_Internals->m_flagAutocommit;
	else return false;
};

bool CConnection::isConnected() const
{
	if (m_Internals) return m_Internals->m_isConnected;
	else return false;
};

// ***************************************************************
// setter 
// ***************************************************************

void CConnection::setAutocommit(bool iSet)
{
	if (m_Internals && m_Internals->m_Interface) {
		try {
			DB_INTERFACE->i_set_autocommit(iSet);
			m_Internals->m_flagAutocommit = iSet;
		} catch (base::Exception e) {
			HandleException(getEnvironment(), e);
		}
	}
}

// ***************************************************************
// methods
// ***************************************************************

void CConnection::Connect(void) {
	if (m_Internals && m_Internals->m_Interface) {
		try {
			DB_INTERFACE->i_connect();
			setAutocommit(false);
			m_Internals->m_isConnected = true;
		} catch (base::Exception e) {
			HandleException(getEnvironment(), e);
		}
	}
}

void CConnection::Disconnect(void) {
	if (m_Internals && m_Internals->m_Interface) {
		try {
			DB_INTERFACE->i_disconnect();
			m_Internals->m_isConnected = false;
		} catch (base::Exception e) {
			HandleException(getEnvironment(), e);
		}
	}
}

void CConnection::Execute(const char *iStmt) {
	try {
		CStatement *pStmt = createStatement();
		if (pStmt) {
			pStmt->Prepare(iStmt);
			pStmt->Execute();
			destroyStatement(pStmt);
		}
	} catch (base::Exception e) {
		HandleException(getEnvironment(), e);
	}
}

void CConnection::Commit(void) {
	if (m_Internals && m_Internals->m_Interface) {
		try {
			DB_INTERFACE->i_commit();
		} catch (base::Exception e) {
			HandleException(getEnvironment(), e);
		}
	}
}

void CConnection::Rollback(void) {
	if (m_Internals && m_Internals->m_Interface) {
		try {
			DB_INTERFACE->i_rollback();
		} catch (base::Exception e) {
			HandleException(getEnvironment(), e);
		}
	}
}

CStatement *CConnection::createStatement(void)
{
	CStatement *r = new CStatement(this);
	if (r) {
		m_Internals->m_Statements->BInsert(r);
	}
	return r;
}

CStatement *CConnection::destroyStatement(CStatement *iStmt)
{
//	CStatement *pStmt = 0;
	if (iStmt) {
		m_Internals->m_Statements->BRemove(iStmt);
	}
	return 0;
}

// ***************************************************************
// std record interface 
// ***************************************************************

static const string &_getColumnStr(const CStdRecord &iRec, const string &iTableName) {
	BindDescPtr pBD;
	static string r;
	r.clear();
	for (unsigned int i = 0; (pBD = iRec.getBindDesc(i)); i++) {
		if (i > 0) {
			r += ",";
		}
		r += iTableName + "." + (char *)BindDescGet(pBD, BindDescNcolumnName);
	}
	return r;
}

static const string &_getBindStr_update(const TableDesc *iTable, const string &iTableName) {
	BindDescPtr pBD;
	static string r;
	char varname[5];
	r.clear();
	for (int i = 0; (pBD = (const BindDesc *)TableDescGet(iTable, TableDescNbindColumn, i)); i++) {
		if (i > 0) r += ",";
		sprintf(varname, "%d", i);
		r += iTableName + "." + (char *)BindDescGet(pBD, BindDescNcolumnName) + "=:U" + varname;
	}
	return r;
}

static const string &_getBindStr_insert(const TableDesc *iTable, const string &iTableName) {
	static string r;
	char varname[5];
	r.clear();
	for (int i = 0; i < (int )TableDescGet(iTable, TableDescNbindColumnCount, 0); i++) {
		if (i > 0) r += ",";
		sprintf(varname, "%d", i);
		r = r + ":I" + varname;
	}
	return r;
}

static const string &_getPKBindStr(const TableDesc *iTable, const string &iTableName) {
	BindDescPtr pBD;
	static string r;
	char varname[5];
	r.clear();
	for (int i = 0; (pBD = (const BindDesc *)TableDescGet(iTable, TableDescNbindPKColumn, i)); i++) {
		if (i > 0) r += " and ";
		sprintf(varname, "%d", i);
		r += iTableName + "." + (char *)BindDescGet(pBD, BindDescNcolumnName) + "=:P" + varname;
	}
	return r;
}

void CConnection::StdRecInsert(const CStdRecord &iRec, const char *iTableName) {
	TableDescPtr iTable;
	if (iTableName && (iTable = LookupTable(iTableName)) && (BindDescPtr )TableDescGet(iTable, TableDescNbindColumn, 0) == iRec.getBindDesc(0)) {
		string TableName = iTableName;
		string strSelect = "insert into " + TableName + " (" + _getColumnStr(iRec, TableName) + ") values (" + _getBindStr_insert(iTable, TableName) + ")";
		CStatement *stmt = createStatement();
		stmt->Prepare(strSelect.c_str());
//		char *pData = (char *)iRec.getStdData();
		stmt->BindVa(BINDPARAM_Struct(iRec), BIND_None);
		stmt->Execute();
		destroyStatement(stmt);
	}
}

void CConnection::StdRecSelect(const CStdRecord &iRec, const char *iTableName) {
	TableDescPtr iTable;
	if (iTableName && (iTable = LookupTable(iTableName)) && (BindDescPtr )TableDescGet(iTable, TableDescNbindColumn, 0) == iRec.getBindDesc(0)) {
		string TableName = iTableName;
		string strSelect = "select " + _getColumnStr(iRec, TableName) + " from " + TableName + " where " + _getPKBindStr(iTable, TableName);
		CStatement *stmt = createStatement();
		stmt->Prepare(strSelect.c_str());
		char *pData = (char *)iRec.getStdData();
		BindDescPtr pBD;
		stmt->BindVa(BINDRESULT_Struct(iRec), BIND_None);
		for (int i = 0; (pBD = (BindDescPtr )TableDescGet(iTable, TableDescNbindPKColumn, i)); i++) {
			if ((DataType )BindDescGet(pBD, BindDescNdataType) == DataType_String) {
				stmt->BindVa(BINDPARAM_String(pData + (int )BindDescGet(pBD, BindDescNcolumnOffset), (int )BindDescGet(pBD, BindDescNcolumnSize)), BIND_None);
			} else {
				stmt->BindVa(BindType_Param, (DataType )BindDescGet(pBD, BindDescNdataType), pData + (int )BindDescGet(pBD, BindDescNcolumnOffset), BIND_None);
			}
		}
		stmt->Execute();
		CResult *Result = stmt->createResult();
		Result->Next();
		destroyStatement(stmt);
	}
}

void CConnection::StdRecUpdate(const CStdRecord &iRec, const char *iTableName) {
	TableDescPtr iTable;
	if (iTableName && (iTable = LookupTable(iTableName)) && (BindDescPtr )TableDescGet(iTable, TableDescNbindColumn, 0) == iRec.getBindDesc(0)) {
		string TableName = iTableName;
		string strSelect = "update " + TableName + " set " + _getBindStr_update(iTable, TableName) + " where " + _getPKBindStr(iTable, TableName);
		CStatement *stmt = createStatement();
		stmt->Prepare(strSelect.c_str());
		char *pData = (char *)iRec.getStdData();
		BindDescPtr pBD;
		stmt->BindVa(BINDPARAM_Struct(iRec), BIND_None);
		for (int i = 0; (pBD = (const BindDesc *)TableDescGet(iTable, TableDescNbindPKColumn, i)); i++) {
			if ((DataType )BindDescGet(pBD, BindDescNdataType) == DataType_String) {
				stmt->BindVa(BINDPARAM_String(pData + (int )BindDescGet(pBD, BindDescNcolumnOffset), (int )BindDescGet(pBD, BindDescNcolumnSize)), BIND_None);
			} else {
				stmt->BindVa(BindType_Param, (DataType )BindDescGet(pBD, BindDescNdataType), pData + (int )BindDescGet(pBD, BindDescNcolumnOffset), BIND_None);
			}
		}
		stmt->Execute();
		destroyStatement(stmt);
	}
}

void CConnection::StdRecDelete(const CStdRecord &iRec, const char *iTableName) {
	TableDescPtr iTable;
	if (iTableName && (iTable = LookupTable(iTableName)) && (BindDescPtr )TableDescGet(iTable, TableDescNbindColumn, 0) == iRec.getBindDesc(0)) {
		string TableName = iTableName;
		string strSelect = "delete from " + TableName + " where " + _getPKBindStr(iTable, TableName);
		CStatement *stmt = createStatement();
		stmt->Prepare(strSelect.c_str());
		char *pData = (char *)iRec.getStdData();
		BindDescPtr pBD;
		for (int i = 0; (pBD = (const BindDesc *)TableDescGet(iTable, TableDescNbindPKColumn, i)); i++) {
			if ((DataType )BindDescGet(pBD, BindDescNdataType) == DataType_String) {
				stmt->BindVa(BINDPARAM_String(pData + (int )BindDescGet(pBD, BindDescNcolumnOffset), (int )BindDescGet(pBD, BindDescNcolumnSize)), BIND_None);
			} else {
				stmt->BindVa(BindType_Param, (DataType )BindDescGet(pBD, BindDescNdataType), pData + (int )BindDescGet(pBD, BindDescNcolumnOffset), BIND_None);
			}
		}
		stmt->Execute();
		destroyStatement(stmt);
	}
}


} // namespace dbi
} // namespace

// eof
