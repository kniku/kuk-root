#include "DBD_Interface.h"

#if (DBD_MYSQL5)

#ifndef _PDBD_MYSQL_H
#define _PDBD_MYSQL_H

namespace klib {
namespace dbi {

class DbdDyn_C_Environment_mysql5 : public DbdDyn_C_Environment {
public:
	DbdDyn_C_Environment_mysql5(const klib::base::CLogger *iLogger) : DbdDyn_C_Environment(iLogger) {}
	// interface implementation
	void					i_init(void);
	void					i_destroy(void);
	DbdDyn_C_Connection *	i_create_connection(const char *iHost, const char *iUser, const char *iPasswd, const char *iDatabase);
	void					i_destroy_connection(DbdDyn_C_Connection *iConnection);
	// date conversion
	void					i_dbiDateTime2DBMSDateTime(void *ipDst, DateTime *pSrc);
	void					i_DBMSDateTime2dbiDateTime(DateTime *pDst, void *ipSrc);
};


class DbdDyn_C_Connection_mysql5 : public DbdDyn_C_Connection {
public:
	DbdDyn_C_Connection_mysql5(DbdDyn_C_Environment *iEnv, const char *iHost, const char *iUser, const char *iPasswd, const char *iDatabase) : DbdDyn_C_Connection(iEnv, iHost, iUser, iPasswd, iDatabase) { }
	// interface implementation
	void					i_connect(void);
	void					i_disconnect(void);
	void					i_commit(void);
	void					i_set_autocommit(bool iSet);
	void					i_rollback(void);
	DbdDyn_C_Statement *	i_create_stmt(void);
	void					i_destroy_stmt(DbdDyn_C_Statement *iStmt);
};

class DbdDyn_C_Statement_mysql5 : public DbdDyn_C_Statement {
public:
	DbdDyn_C_Statement_mysql5(DbdDyn_C_Connection *iInterface) : DbdDyn_C_Statement(iInterface) {}
	~DbdDyn_C_Statement_mysql5();
	// interface implementation
	void				i_Prepare(const char *iStmt);
	void				i_BindResults(void);
	void				i_BindResultTransfer(void);
	void				i_BindParams(void);
	void				i_BindParamTransfer(void);
	void				i_Execute();
	void				i_Reset();
	class DbdDyn_C_Result *	i_create_result(void) const;
	void					i_destroy_result(DbdDyn_C_Result *iResult) const;
	bool					i_next_result(void);
};

class DbdDyn_C_Result_mysql5 : public DbdDyn_C_Result {
public:
	DbdDyn_C_Result_mysql5(DbdDyn_C_Statement *iStmt) : DbdDyn_C_Result (iStmt) {}
	~DbdDyn_C_Result_mysql5();
	bool DbdDyn_C_Result_mysql5::get_result(DataType iType, unsigned int idx, void *pData) const;
	// interface implementation
	bool							i_Next(void);
	DataType						i_getType(unsigned int idx) const;
	bool							i_isNull(unsigned int idx) const;
};

} // namespace
} // namespace

#endif
#endif
