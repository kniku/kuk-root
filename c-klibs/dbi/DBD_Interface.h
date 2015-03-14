#ifndef _DBDINTERFACE_H
#define _DBDINTERFACE_H

#ifdef USE_MYSQL5
#	define DBD_MYSQL5	1
#else
#	define DBD_MYSQL5	0
#endif

#ifdef USE_ORACLE9
#	define DBD_ORACLE9	1
#else
#	define DBD_ORACLE9	0
#endif

#ifdef USE_SQLITE3
#	define DBD_SQLITE3	1
#else
#	define DBD_SQLITE3	0
#endif

#if (WIN32)
#	define SYSTEM_WIN32 1
#endif

// check system type
#if (SYSTEM_WIN32==0) && (SYSTEM_LINUX==0)
#	error ##### can't determine system type! #####
#endif

// check interface defines
#if (DBD_MYSQL5==0) && (DBD_ORACLE9==0) && (DBD_SQLITE3==0)
#	error ##### at least ONE interface type must be defined! #####
#endif

#include "klibdbi.h"

namespace klib {
namespace dbi {

class DbdDyn_C_Connection;
class DbdDyn_C_Statement;

/* generic types */
typedef void * Dbi_T_Handle;

#define DBD_NAME_OF_DATATYPE(itype) ((itype)==DataType_Long?"Long":((itype)==DataType_Double?"Double":((itype)==DataType_String?"String":((itype)==DataType_DateTime?"DateTime":"UNKNOWN"))))

class DbdDyn_C_Environment *DbdDyn_CreateEnvironment(InterfaceType iType, klib::base::CLogger *iLogger);

class DbdDyn_C_Environment {
protected:
	Dbi_T_Handle m_DriverInternals;
	const klib::base::CLogger *m_Logger;
public:
	DbdDyn_C_Environment(const klib::base::CLogger *iLogger);
	// setter
	void								setDriverInternals(Dbi_T_Handle iInternals) { m_DriverInternals = iInternals; }
	// simple getter
	Dbi_T_Handle						getDriverInternals(void) const { return m_DriverInternals; }
	const klib::base::CLogger *				getLogHandler(void) { return m_Logger; }
	// public interface
	virtual void						i_init(void) = 0;
	virtual void						i_destroy(void) = 0;
	virtual DbdDyn_C_Connection *		i_create_connection(const char *iHost, const char *iUser, const char *iPasswd, const char *iDatabase) = 0;
	virtual void						i_destroy_connection(DbdDyn_C_Connection *iConnection) = 0;
	// date conversions
	virtual void						i_dbiDateTime2DBMSDateTime(void *pDst, DateTime *pSrc) = 0;
	virtual void						i_DBMSDateTime2dbiDateTime(DateTime *pDst, void *pSrc) = 0;

};

class DbdDyn_C_Connection {
protected:
	Dbi_T_Handle m_DriverInternals;
	DbdDyn_C_Environment *m_hEnv;
	std::string m_Database, m_Host, m_User, m_Passwd;
public:
	DbdDyn_C_Connection(DbdDyn_C_Environment *iEnv, const char *iHost, const char *iUser, const char *iPasswd, const char *iDatabase);
	virtual ~DbdDyn_C_Connection() {}
	// setter
	void							setDriverInternals(Dbi_T_Handle iInternals) { m_DriverInternals = iInternals; }
	// simple getter
	Dbi_T_Handle					getDriverInternals(void) const { return m_DriverInternals; };
	DbdDyn_C_Environment *			getEnvironment(void) { return m_hEnv; }
	// public interface
	virtual void					i_connect(void) = 0;
	virtual void					i_disconnect(void) = 0;
	virtual void					i_commit(void) = 0;
	virtual void					i_set_autocommit(bool iSet) = 0;
	virtual void					i_rollback(void) = 0;
	virtual DbdDyn_C_Statement *	i_create_stmt(void) = 0;
	virtual void					i_destroy_stmt(DbdDyn_C_Statement *iStmt) = 0;
};

struct DbdDyn_T_Binding {
	void *appBuffer;	// deleted if != 0 && isAllocApp == true
	bool isAllocApp;
	void *bindBuffer;	// deleted if != 0 && isAllocBind == true
	bool isAllocBind;
	void *nullIndicator;	// deleted if != 0
	int length;
	DataType dataType;
};

class DbdDyn_C_Statement {
protected:
	DbdDyn_C_Connection *m_hConnection;
	Dbi_T_Handle m_DriverInternals;
public:
	static const int DBI_MAX_RESULT_COLUMNS = 1000;
	klib::base::BArray m_bindResultArray;	// holds DbdDyn_T_Binding structures
	klib::base::BArray m_bindParamArray;	// holds DbdDyn_T_Binding structures
	bool m_varsBound;
	bool m_stmtPrepared;					// set on i_Prepare, reset on i_Reset
	DbdDyn_C_Statement(DbdDyn_C_Connection *iConnection);
	virtual ~DbdDyn_C_Statement() {}
	// setter
	void								setDriverInternals(Dbi_T_Handle iInternals) { m_DriverInternals = iInternals; }
	// simple getter
	Dbi_T_Handle						getDriverInternals(void) const { return m_DriverInternals; };
	DbdDyn_C_Connection *				getConnection(void) { return m_hConnection; }
	// public interface
	virtual void						i_Prepare(const char *iStmt) = 0;
	virtual void						i_BindResults(void) = 0;
	virtual void						i_BindResultTransfer(void) = 0;
	virtual void						i_BindParams(void) = 0;
	virtual void						i_BindParamTransfer(void) = 0;
	virtual void						i_Execute(void) = 0;
	virtual void						i_Reset(void) = 0;
	virtual class DbdDyn_C_Result *		i_create_result(void) const = 0;
	virtual void						i_destroy_result(DbdDyn_C_Result *iResult) const = 0;
	virtual bool						i_next_result(void) = 0;
};

class DbdDyn_C_Result {
protected:
	DbdDyn_C_Statement *m_hStmt;
public:
	DbdDyn_C_Result(DbdDyn_C_Statement *iStmt) { m_hStmt = iStmt; }
	virtual ~DbdDyn_C_Result() {}
	// simple getter
	DbdDyn_C_Statement *	getStatement(void) { return m_hStmt; }
	// public interface
	virtual bool			i_Next(void) = 0;
	virtual bool			i_isNull(unsigned int idx) const = 0;
	virtual DataType		i_getType(unsigned int idx) const = 0;
};

#if 0
/*
 * jump table entry (dynamic lib function call)
 */
typedef struct _DbiDyn_T_JumpTableEntryRec {
	const char *function_name;
	void *function_addr;
} DbiDyn_T_JumpTableEntryRec, *DbiDyn_T_JumpTableEntry;

/*
 * dynamic lib function call
 */
#ifdef SYSTEM_WIN32
#define __DBIDYN_LIBCALL_WITH_RV(iLib, iRv, iFunc, iParams) \
{ \
	DbdDyn_FT##iLib##iFunc func = (DbdDyn_FT##iLib##iFunc )DbdDyn##iLib##JumpTable[DbdDyn_FI##iLib##iFunc].function_addr; \
	if (func == NULL) { \
		if (DbdDyn_H##iLib##Library == NULL) { \
			DbdDyn_H##iLib##Library = (Dbi_T_Handle )LoadLibrary(DBDDYN##iLib##LibName);  \
		} \
		if (DbdDyn_H##iLib##Library) { \
			func = (DbdDyn_FT##iLib##iFunc )GetProcAddress((HMODULE )DbdDyn_H##iLib##Library, #iFunc); \
			if (func) { \
				DbdDyn##iLib##JumpTable[DbdDyn_FI##iLib##iFunc].function_addr = (void *)func; \
			} \
		} else { \
			throw Dbi_C_Exception("(DynLoad) can't call \""#iFunc"()\": library \""DBDDYN##iLib##LibName"\" not loaded!", __FILE__, __LINE__); \
		} \
	} \
	if (func) { \
		iRv func iParams; \
	} \
}

#elif SYSTEM_LINUX

#define __DBIDYN_LIBCALL_WITH_RV(iLib, iRv, iFunc, iParams) \
{ \
	DbdDyn_FT##iLib##iFunc func = (DbdDyn_FT##iLib##iFunc )DbdDyn##iLib##JumpTable[DbdDyn_FI##iLib##iFunc].function_addr; \
	if (func == NULL) { \
		if (DbdDyn_H##iLib##Library == NULL) { \
			DbdDyn_H##iLib##Library = dlopen(DBDDYN##iLib##LibName, RTLD_LAZY); \
		} \
		if (DbdDyn_H##iLib##Library) { \
			func = (DbdDyn_FT##iLib##iFunc )dlsym(DbdDyn_H##iLib##Library, #iFunc); \
			if (func) { \
				DbdDyn##iLib##JumpTable[DbdDyn_FI##iLib##iFunc].function_addr = (void *)func; \
			} else { \
				throw Dbi_C_Exception("(DynLoad) can't call \""#iFunc"()\": function not found in library \""DBDDYN##iLib##LibName"\"!", __FILE__, __LINE__); \
			} \
		} else { \
			throw Dbi_C_Exception("(DynLoad) can't call \""#iFunc"()\": library \""DBDDYN##iLib##LibName"\" not loaded!", __FILE__, __LINE__); \
		} \
	} \
	if (func) { \
		iRv func iParams; \
	} \
}
#endif

#define DBIDYN_LIBCALL_WITH_RV(iLib, iRv, iFunc, iParams) __DBIDYN_LIBCALL_WITH_RV(iLib, iRv=, iFunc, iParams)
#define DBIDYN_LIBCALL(iLib, iFunc, iParams) __DBIDYN_LIBCALL_WITH_RV(iLib, , iFunc, iParams)
#endif	// if 0

#define DBD_ThrowException(iError, iErrno, iSqlState)	throw Dbi_C_Exception(iError, __FILE__, __LINE__, iErrno, iSqlState)

} // namespace dbi
} // namespace klib

#endif	// _DBDINTERFACE_H
