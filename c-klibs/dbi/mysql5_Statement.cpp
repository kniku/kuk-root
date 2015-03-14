#include "mysql5_pDriver.h"

#if (DBD_MYSQL5)

namespace klib {
namespace dbi {

// ******************************
// DbdDyn_C_Statement_mysql5
// ******************************

DbdDyn_C_Statement_mysql5::~DbdDyn_C_Statement_mysql5()
{
	MYSQL_STMT *MyStmt = (MYSQL_STMT *)getDriverInternals();
	if (MyStmt) {
		my_bool r = 0;
		r = mysql_stmt_close(MyStmt);
		if (r) {
			DBD_ThrowException(NULL, NULL, MyStmt);
		}
	}
}

static char _stmtBuffer[65000];

static char *checkNfix_bind_syntax(const char *iStmt)
{
	char *r = 0;
	char *p;
	if (iStmt) {
		strcpy(_stmtBuffer, iStmt);
		p = r = _stmtBuffer;
		while ((p = strchr(p, ':'))) {
			*p = '?';
			for (++p; *p && *p != ' ' && *p != ',' && *p != '(' && *p != ')'; p++) *p = ' ';
		}
	}
	return r;
}

static MYSQL_BIND _bind_buffer[DbdDyn_C_Statement::DBI_MAX_RESULT_COLUMNS];

void DbdDyn_C_Statement_mysql5::i_Prepare(const char *iStmt)
{
	MYSQL *MySql = (MYSQL *) m_hConnection->getDriverInternals();
	MYSQL_STMT *MyStmt = NULL;
	char *Stmt;

	// convert bind param syntax
	Stmt = checkNfix_bind_syntax(iStmt);

	/* init sql stmt */
	MyStmt = mysql_stmt_init(MySql);
	if (MyStmt == NULL) {
		DBD_ThrowException(NULL, MySql, NULL);
	}

	/* prepare stmt */
	int r_int;
	r_int = mysql_stmt_prepare(MyStmt, Stmt, (unsigned long)strlen(Stmt));
	if (r_int) {
		DBD_ThrowException(NULL, MySql, MyStmt);
	}
	setDriverInternals(MyStmt);
}

static enum_field_types dbitype2mysqltype(DataType iType)
{
	enum_field_types r = MYSQL_TYPE_NULL;
	switch (iType) {
		case DataType_Long:
			r = MYSQL_TYPE_LONG;
			break;
		case DataType_Double:
			r = MYSQL_TYPE_DOUBLE;
			break;
		case DataType_String:
			r = MYSQL_TYPE_VAR_STRING;
			break;
		case DataType_DateTime:
			r = MYSQL_TYPE_DATETIME;
			break;
		default:
			break;
	}
	return r;
}

void DbdDyn_C_Statement_mysql5::i_BindResults(void)
{
	MYSQL *MySql = (MYSQL *) m_hConnection->getDriverInternals();
	MYSQL_STMT *MyStmt = (MYSQL_STMT *)getDriverInternals();
	MYSQL_RES *MyRes = NULL;
	MYSQL_FIELD *pField;
	MYSQL_BIND *MyBind = _bind_buffer, *piBind;
	DbdDyn_T_Binding *pBindInfo;

	if (MyStmt == NULL) {
		return;
	}

	// get result metadata; maybe NULL: no results -> skip
	MyRes = mysql_stmt_result_metadata(MyStmt);
	if (MyRes == NULL) {
		return;
	}

	// check result count
	if (MyRes->field_count > (unsigned int )DbdDyn_C_Statement::DBI_MAX_RESULT_COLUMNS) {
		DBD_ThrowException("max. of result coulmns exceeded!", MySql, MyStmt);
	}

	// reset bind buffer
	memset(MyBind, 0, sizeof(MYSQL_BIND) * MyRes->field_count);
	
	// walk through result fields
	unsigned long i = 0;
	while (i < MyRes->field_count && (pBindInfo = (DbdDyn_T_Binding *)m_bindResultArray.GetAt(i))) {
//		iParam1 = iType1 ? va_arg(args, void *) : NULL;
		pField = &MyRes->fields[i];
		piBind = &MyBind[i];

		// type check
		if (mysqltype2dbitype(pField->type) != pBindInfo->dataType) {
			_LogStream.str(""); _LogStream << "result type mismatch (result idx=" << i << ")";
			DBD_ThrowException(_LogStream.str().c_str(), MySql, MyStmt);
		}

		piBind->is_null = (my_bool *)calloc(1, sizeof(my_bool));
		piBind->buffer_type = pField->type;

		switch (pBindInfo->dataType) {
		case DataType_String:
			piBind->buffer_length = pBindInfo->length;//pField->length+1;
			// fall through
		case DataType_Long:
		case DataType_Double:
			piBind->buffer = pBindInfo->appBuffer;//iParam1;
			break;
		case DataType_DateTime:
			pBindInfo->bindBuffer = new MYSQL_TIME;
			pBindInfo->isAllocBind = true;
			piBind->buffer = pBindInfo->bindBuffer;
			break;
		default:
			break;
		}

		// get next parameter
		i++;
	}
	// check if all results given
	if (i < MyRes->field_count) {
		DBD_ThrowException("too few bind variables!", MySql, MyStmt);
	}

	// Bind ...
	my_bool r_bool;
	r_bool = mysql_stmt_bind_result(MyStmt, &MyBind[0]);
	if (r_bool) {
		DBD_ThrowException(NULL, MySql, MyStmt);
	}
}

void DbdDyn_C_Statement_mysql5::i_BindResultTransfer(void)
{
	DbdDyn_T_Binding *pBind;
	MYSQL_STMT *MyStmt = (MYSQL_STMT *)getDriverInternals();
	if (MyStmt == NULL) {
		return;
	}
	for (unsigned int i = 0; (pBind = (DbdDyn_T_Binding *)m_bindResultArray.GetAt(i)); i++) {
		DataType iType = pBind->dataType;// TODO: remove it? mysqltype2dbitype(MyStmt->bind[i].buffer_type);
		switch (iType) {
			case DataType_Long:
				if (MyStmt->bind[i].is_null && *MyStmt->bind[i].is_null) {
					if (pBind->appBuffer) *(long *)pBind->appBuffer = 0;
				}
				break;
			case DataType_Double:
				if (MyStmt->bind[i].is_null && *MyStmt->bind[i].is_null) {
					if (pBind->appBuffer) *(double *)pBind->appBuffer = 0.0;
				}
				break;
			case DataType_String:
				if (MyStmt->bind[i].is_null && *MyStmt->bind[i].is_null) {
					if (pBind->appBuffer) *(char *)pBind->appBuffer = 0;
				}
				break;
			case DataType_DateTime:
				if (MyStmt->bind[i].is_null && *MyStmt->bind[i].is_null) {
					memset(pBind->appBuffer, 0, sizeof(DateTime));
				} else {
					m_hConnection->getEnvironment()->i_DBMSDateTime2dbiDateTime((DateTime *)pBind->appBuffer, pBind->bindBuffer/*MyStmt->bind[i].buffer*/);
				}
				break;
			default:
				break;
		}
	}
}

void DbdDyn_C_Statement_mysql5::i_BindParams(void)
{
	MYSQL *MySql = (MYSQL *) m_hConnection->getDriverInternals();
	MYSQL_STMT *MyStmt = (MYSQL_STMT *)getDriverInternals();
	MYSQL_BIND *MyBind = _bind_buffer, *piBind;
	DbdDyn_T_Binding *pBindInfo;

	if (MyStmt == NULL) {
		return;
	}
	/* get number of params */
	unsigned long param_count;
	param_count = mysql_stmt_param_count(MyStmt);
	if (param_count == 0) {
		return;
	}
	
	memset(MyBind, 0, sizeof(MYSQL_BIND) * param_count);
	unsigned long i = 0;
	while (i < param_count && (pBindInfo = (DbdDyn_T_Binding *)m_bindParamArray.GetAt(i))) {
		piBind = &MyBind[i];
		// TODO: check and usage of types with mysql_stmt_param_metadata() */
		piBind->buffer_type = dbitype2mysqltype(pBindInfo->dataType);
		switch (pBindInfo->dataType) {
			case DataType_Long:
			case DataType_Double:
				piBind->buffer = pBindInfo->appBuffer;//iParam1;
				break;
			case DataType_String:
				piBind->buffer = pBindInfo->appBuffer;//iParam1;
				//va_arg(args, int /*length*/);
				piBind->buffer_length = pBindInfo->length;//(unsigned long )strlen((char *)iParam1);
				break;
			case DataType_DateTime:
				pBindInfo->bindBuffer = new MYSQL_TIME;
				pBindInfo->isAllocBind = true;
				piBind->buffer = pBindInfo->bindBuffer;
//				m_hConnection->getEnvironment()->i_dbiDateTime2DBMSDateTime(piBind->buffer, (DateTime *)pBindInfo->appBuffer/*iParam1*/);
				break;
			default:
				break;
		}
		// get next parameter
		i++;
	}
	if (i < param_count) {
		DBD_ThrowException("too few bind parameters!", MySql, MyStmt);
	}
	// call bind function...
	my_bool r_bool = 0;
	r_bool = mysql_stmt_bind_param(MyStmt, MyBind);
	if (r_bool) {
		DBD_ThrowException(NULL, MySql, MyStmt);
	}
}

void DbdDyn_C_Statement_mysql5::i_BindParamTransfer(void)
{
	DbdDyn_T_Binding *pBind;
	MYSQL_STMT *MyStmt = (MYSQL_STMT *)getDriverInternals();
	if (MyStmt == NULL) {
		return;
	}

	for (unsigned long i = 0; (pBind = (DbdDyn_T_Binding *)m_bindParamArray.GetAt(i)); i++) {
		if (pBind == 0) break;
		DataType iType = pBind->dataType;// TODO: remove? mysqltype2dbitype(MyStmt->params[i].buffer_type);
		switch (iType) {
			case DataType_Long:
			case DataType_Double:
				// nothing to do
				break;
			case DataType_String:
				MyStmt->params[i].buffer_length = (unsigned long)strlen((char *)pBind->appBuffer);
				break;
			case DataType_DateTime:
				m_hConnection->getEnvironment()->i_dbiDateTime2DBMSDateTime(pBind->bindBuffer,/*MyStmt->params[i].buffer,*/ (DateTime *)pBind->appBuffer);
				break;
			default:
				break;
		}
	}
}

void DbdDyn_C_Statement_mysql5::i_Execute()
{
	MYSQL *MySql = (MYSQL *) m_hConnection->getDriverInternals();
	MYSQL_STMT *MyStmt = (MYSQL_STMT *)getDriverInternals();
	if (MySql && MyStmt) {
		/* execute stmt */
		my_bool r = 0;
		r = mysql_stmt_execute(MyStmt);
		if (r) {
			DBD_ThrowException(NULL, MySql, MyStmt);
		}
	}
}

void DbdDyn_C_Statement_mysql5::i_Reset()
{
//	MYSQL *MySql = (MYSQL *) m_hConnection->getDriverInternals();
	MYSQL_STMT *MyStmt = (MYSQL_STMT *)getDriverInternals();
	if (MyStmt) {
		/* reset stmt */
		my_bool r = 0;
		r = mysql_stmt_reset(MyStmt);
		if (r) {
			DBD_ThrowException(NULL, MyStmt->mysql, MyStmt);
		}
	}
}

class DbdDyn_C_Result *DbdDyn_C_Statement_mysql5::i_create_result(void) const
{
	return (DbdDyn_C_Result *) new DbdDyn_C_Result_mysql5((DbdDyn_C_Statement *)this);
}

void DbdDyn_C_Statement_mysql5::i_destroy_result(DbdDyn_C_Result *iResult) const
{
	delete (DbdDyn_C_Result_mysql5 *)iResult;
}

bool DbdDyn_C_Statement_mysql5::i_next_result(void) {
	bool r = false;
	MYSQL_STMT *MyStmt = (MYSQL_STMT *)getDriverInternals();
	int r_int;
	if (MyStmt) {
		r_int = mysql_stmt_fetch(MyStmt);
		if (r_int == 0) {
			r = true;
		} else if (r_int != MYSQL_NO_DATA) {
			DBD_ThrowException(NULL, MyStmt->mysql, MyStmt);
		}
	}
	return r;
}

} // namespace dbi
} // namespace

#endif

//eof
