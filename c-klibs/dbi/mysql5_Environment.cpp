#include "mysql5_pDriver.h"

#if (DBD_MYSQL5)

namespace klib {
namespace dbi {

ostringstream _LogStream;

void _throw_err_mysql5(char *file, int line, MYSQL *iMySql, MYSQL_STMT *iStmt, const char *iMsg)
{
	const char *sqlstate;
	const char *error;
	unsigned int myerrno;
	if (iStmt) {
		error = mysql_stmt_error(iStmt);
		myerrno = mysql_stmt_errno(iStmt);
		sqlstate = mysql_stmt_sqlstate(iStmt);
		if ((error == NULL || *error == 0) && iMsg) error = iMsg;
		throw klib::base::Exception(error, file, line, myerrno, sqlstate);
	} else if (iMySql) {
		error = mysql_error(iMySql);
		myerrno = mysql_errno(iMySql);
		if ((error == NULL || *error == 0) && iMsg) error = iMsg;
		throw klib::base::Exception(error, file, line, myerrno);
	} else {
		throw klib::base::Exception(iMsg, file, line);
	}
}

DataType mysqltype2dbitype(enum_field_types iType)
{
	DataType r = DataType_None;
	switch (iType) {
		case MYSQL_TYPE_LONG:
			r = DataType_Long;
			break;
		case MYSQL_TYPE_DOUBLE:
			r = DataType_Double;
			break;
		case MYSQL_TYPE_STRING:
		case MYSQL_TYPE_VAR_STRING:
			r = DataType_String;
			break;
		case MYSQL_TYPE_DATE:
		case MYSQL_TYPE_TIME:
		case MYSQL_TYPE_DATETIME:
		case MYSQL_TYPE_TIMESTAMP:
			r = DataType_DateTime;
			break;
		default:
			break;
	}
	return r;
}

// ******************************
// DbdDyn_C_Environment_mysql5
// ******************************

void DbdDyn_C_Environment_mysql5::i_init(void)
{
	// TODO
}

void DbdDyn_C_Environment_mysql5::i_destroy(void)
{
	// TODO cleanup?
}

DbdDyn_C_Connection * DbdDyn_C_Environment_mysql5::i_create_connection(const char *iHost, const char *iUser, const char *iPasswd, const char *iDatabase)
{
	DbdDyn_C_Connection_mysql5 *r = new DbdDyn_C_Connection_mysql5(this, iHost, iUser, iPasswd, iDatabase);
	MYSQL *iMySql = NULL;
	iMySql = mysql_init(NULL);
	if (iMySql == NULL) {
		DBD_ThrowException("initialization failed!", NULL, NULL);
	}
	r->setDriverInternals((Dbi_T_Handle )iMySql);
	return r;
}
void DbdDyn_C_Environment_mysql5::i_destroy_connection(DbdDyn_C_Connection *iConnection)
{
	delete iConnection;
}

void DbdDyn_C_Environment_mysql5::i_dbiDateTime2DBMSDateTime(void *ipDst, DateTime *pSrc)
{
	MYSQL_TIME *pDst = (MYSQL_TIME *)ipDst;
	if (pDst && pSrc) {
		memset(pDst, 0, sizeof(MYSQL_TIME));
		pDst->year		= pSrc->Year;
		pDst->month		= pSrc->Month;
		pDst->day		= pSrc->Day;
		pDst->hour		= pSrc->Hour;
		pDst->minute	= pSrc->Minute;
		pDst->second	= pSrc->Second;
	}
}
void DbdDyn_C_Environment_mysql5::i_DBMSDateTime2dbiDateTime(DateTime *pDst, void *ipSrc)
{
	MYSQL_TIME *pSrc = (MYSQL_TIME *)ipSrc;
	if (pDst && pSrc) {
		pDst->Year = pSrc->year;
		pDst->Month = pSrc->month; 
		pDst->Day = pSrc->day;
		pDst->Hour = pSrc->hour;
		pDst->Minute = pSrc->minute;
		pDst->Second = pSrc->second;
	}
}

} // namespace dbi
} // namespace

#endif // (DBD_MYSQL5)

// eof
