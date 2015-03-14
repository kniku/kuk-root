#ifndef _DBI_CPPDBI_H
#define _DBI_CPPDBI_H

#include <klibbase.h>

#ifdef SYSTEM_WIN32
#	ifdef DBI_EXPORTS
#		define DBIDLLCALL	__declspec( dllexport )
#	else
#		define DBIDLLCALL	__declspec( dllimport )
#	endif
#else
#	define DBIDLLCALL
#endif

namespace klib {
namespace dbi {

// generic return value
#ifdef SYSTEM_WIN32
	typedef unsigned long long RetValue;
#else
	typedef unsigned long RetValue;
#endif

// simple datetime type
struct DateTime {
	unsigned int Year;
	unsigned int Month;
	unsigned int Day;
	unsigned int Hour;
	unsigned int Minute;
	unsigned int Second;
};

// supported DBMS
enum InterfaceType {
	InterfaceType_None,
	InterfaceType_mysql5 = 1,
	InterfaceType_oracle9,
	InterfaceType_sqlite3,
	InterfaceType_Last
};

// supported datatypes
enum DataType {
	DataType_None,
	DataType_Long,		// long
	DataType_Double,	// double
	DataType_String,	// char *
	DataType_DateTime,	// DateTime *
	DataType_Struct,	// structure (void *)
	DataType_Last
};

// bind types
enum BindType {
	BindType_None,
	BindType_Param,
	BindType_Result,
	BindType_Last
};

#define BINDPARAM_Long(value)				klib::dbi::BindType_Param,klib::dbi::DataType_Long,&(value)
#define BINDPARAM_Double(value)				klib::dbi::BindType_Param,klib::dbi::DataType_Double,&(value)
// use sizeof(str) for maxlen; means: buffersize in bytes
#define BINDPARAM_String(value,maxlen)		klib::dbi::BindType_Param,klib::dbi::DataType_String,((char *)value),maxlen
#define BINDPARAM_DateTime(value)			klib::dbi::BindType_Param,klib::dbi::DataType_DateTime,&(value)
#define BINDPARAM_Struct(value)				klib::dbi::BindType_Param,klib::dbi::DataType_Struct,(void *)&(value)

#define BINDRESULT_Long(value)				klib::dbi::BindType_Result,klib::dbi::DataType_Long,&(value)
#define BINDRESULT_Double(value)			klib::dbi::BindType_Result,klib::dbi::DataType_Double,&(value)
// use sizeof(str) for maxlen; means: buffersize in bytes
#define BINDRESULT_String(value,len)		klib::dbi::BindType_Result,klib::dbi::DataType_String,((char *)value),len
#define BINDRESULT_DateTime(value)			klib::dbi::BindType_Result,klib::dbi::DataType_DateTime,&(value)
#define BINDRESULT_Struct(value)			klib::dbi::BindType_Result,klib::dbi::DataType_Struct,(void *)&(value)

#define BIND_None							klib::dbi::BindType_None

// std record binding and class

typedef const struct BindDesc *BindDescPtr;

enum BindDescResource {
	BindDescNdataType = 1,	// datatype (DataType)
	BindDescNcolumnName,	// column name (const char *)
	BindDescNcolumnSize,	// size of member in bytes (int)
	BindDescNcolumnOffset,	// offset of member within Data aera in bytes (int)
	BindDescNlast
};
DBIDLLCALL RetValue BindDescGet(BindDescPtr iBindDesc, BindDescResource iWhat);

// base class of the generated record classes
class CStdRecord {
public:
	virtual void		reset(void) = 0;
	virtual BindDescPtr	getBindDesc(const unsigned int iIndex) const = 0;
	virtual void *		getStdData(void) const = 0;
};

// table registering

typedef const struct TableDesc * TableDescPtr;

DBIDLLCALL void RegisterTables(TableDescPtr iTableDesc);
DBIDLLCALL TableDescPtr LookupTable(const char *iTableName);

enum TableDescResource {
	TableDescNtableName = 1,		// tablename; iIndex is ignored
	TableDescNbindPKColumnCount,	// length binddesc of primary key
	TableDescNbindPKColumn,			// binddesc of iIndex column of primary key
	TableDescNbindColumnCount,		// length binddesc
	TableDescNbindColumn,			// binddesc of iIndex column
	TableDescNlast
};
DBIDLLCALL RetValue TableDescGet(TableDescPtr iTableDesc, TableDescResource iWhat, unsigned int iIndex);

// type of exception handler (use CConnection::setExceptionHandler()
typedef void (*DbiExceptionHandler)(const class CEnvironment *iEnv, const klib::base::Exception &ex);

// class forward declarations
class CConnection;
class CStatement;
class CResult;

// a environment
class DBIDLLCALL CEnvironment {
private:
	struct CEnvironment_Internals *m_Internals;
public:
	// constructor(s)
	CEnvironment(InterfaceType iInterface, klib::base::CLogger *iLogHandler = 0);
	// destructor
	~CEnvironment();
	// setter
	void						setExceptionHandler(DbiExceptionHandler iHandler);
	void						setLogHandler(klib::base::CLogger *iHandler);
	// getter
	const InterfaceType			getInterfaceType(void) const;
	class DbdDyn_C_Environment *getInterface() const;
	DbiExceptionHandler			getExceptionHandler() const;
	klib::base::CLogger *			getLogHandler() const;
	// methods
	CConnection *				createConnection(const char *iHost, const char *iUser, const char *iPasswd, const char *iDatabase);
	CConnection *				destroyConnection(CConnection *iConn);
};

// a connection
class DBIDLLCALL CConnection {
private:
	struct CConnection_Internals *m_Internals;
public:
	// constructor(s)
	CConnection(CEnvironment *iEnv, const char *iHost, const char *iUser, const char *iPasswd, const char *iDatabase);
	// destructor
	~CConnection();
	// setter
	void						setAutocommit(bool iSet);
	// getter
	CEnvironment *				getEnvironment() const;
	class DbdDyn_C_Connection *	getInterface() const;
	bool						getAutocommit() const;
	bool						isConnected() const;
	// methods
	void						Connect(void);
	void						Disconnect(void);
	void						Execute(const char *iStmt);
	void						Commit(void);
	void						Rollback(void);
	CStatement *				createStatement(void);
	CStatement *				destroyStatement(CStatement *iStmt);
	// std record interface
	void						StdRecInsert(const CStdRecord &iRec, const char *iTableName);
	void						StdRecSelect(const CStdRecord &iRec, const char *iTableName);
	void						StdRecUpdate(const CStdRecord &iRec, const char *iTableName);
	void						StdRecDelete(const CStdRecord &iRec, const char *iTableName);
};

// statement class
class DBIDLLCALL CStatement {
private:
	struct CStatement_Internals *m_Internals;
	void						Reset();
public:
	// construction
	CStatement(CConnection *iConnection);
	// destructor
	~CStatement();
	// getter
	CConnection *				getConnection() const;
	class DbdDyn_C_Statement *	getInterface() const;
	// methods
	void						Prepare(const char *iStmt);
	void						BindVa(BindType iBindType1, DataType iType1, void *iParam1, ...);
	void						Execute();
	CResult *					createResult(void);
	CResult *					destroyResult(CResult *iResult);
	bool						Next(void) const;
};

// statement result
class DBIDLLCALL CResult
{
private:
	struct CResult_Internals *m_Internals;
public:
	CResult(CStatement *iStmt);
	~CResult(void);
	bool						Next(void) const;
	bool						isNull(unsigned int idx) const;
	DataType					getType(unsigned int idx) const;
};

// **********************************************************************
// private decl part: for generated code
// **********************************************************************
#ifdef DBI_GENERATED_CODE

struct BindDesc {
	DataType Type;
	const char *Name;
	unsigned int Length;	// in bytes
	unsigned int Offset;
};

struct TableDesc {
	const char *tableName;
	const BindDesc **primKeyCols;
	const BindDesc *allCols;
	unsigned int cntPrimKeyCols;
	unsigned int cntCols;
};

#endif // DBI_GENERATED_CODE

} // namespace
} // namespace

#endif

