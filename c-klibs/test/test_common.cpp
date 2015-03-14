#include "dbitest.h"

using namespace DbiGen_M1;

static void dump_record(const S1 &iRec) {
	printf("[%s],[%ld],[%.2f],[%02d.%02d.%04d,%02d:%02d:%02d][%s]\n", iRec.getData()->Id, iRec.getData()->inum, iRec.getData()->dnum,
		iRec.getData()->adate.Day,iRec.getData()->adate.Month,iRec.getData()->adate.Year,iRec.getData()->adate.Hour,iRec.getData()->adate.Minute,iRec.getData()->adate.Second,
		iRec.getData()->astring);
}

static void _connect(CConnection *pConn, const char *iName)
{
	cout << "connecting [" << iName << "]: ";
	if (pConn) {
		pConn->Connect();
		if (pConn->isConnected()) {
			cout << "established";
		} else {
			cout << "FAILED!";
		}
	} else {
		cout << "nothing to do";
	}
	cout << endl;
}

static void _create_tables(CConnection *pConn, const char *iName)
{
	cout << "create tables [" << iName << "]: ";
	if (pConn) {
		cout << endl;
		if (pConn->getEnvironment()->getInterfaceType() == InterfaceType_oracle9) {
			pConn->Execute(
					"create table XTEST1"
					" (Id varchar(10) NOT NULL,"
					" inum numeric(9,0),"
					" dnum numeric(15,2),"
					" adate date,"
					" astring varchar(200),"
					" primary key (Id))"
				);
		} else {
			pConn->Execute(
					"create table XTEST1"
					" (Id varchar(10) NOT NULL,"
					" inum int,"
					" dnum double,"
					" adate datetime,"
	//				" adate varchar(14),"//datetime,"
					" astring varchar(200),"
					" primary key (Id))"
				);
		}
	} else {
		cout << "nothing to do" << endl;
	}
}

static void _drop_tables(CConnection *pConn, const char *iName)
{
	cout << "drop tables [" << iName << "]: ";
	if (pConn) {
		cout << endl;
		pConn->Execute("drop table XTEST1");
	} else {
		cout << "nothing to do" << endl;
	}
}

static void _insert_data(CConnection *pConn, const char *iName)
{
	S1 gStruct;
	cout << "insert data [" << iName << "]: ";
	if (pConn) {
		cout << endl;
		CStatement *stmt = pConn->createStatement();
		if (stmt) {
//			stmt->Prepare("insert into XTEST1 (id,inum,dnum,adate,astring) values('x1',1,1.1,'20060310','x1 insert ok')");
			stmt->Prepare("insert into XTEST1 (id) values('x1')");
			stmt->Execute();
			
			stmt->Prepare("insert into XTEST1 (id,inum,dnum,adate,astring) values('x2',2,2.2,'20060311','x2 insert ok')");
			stmt->Execute();

			gStruct.reset();
			strcpy(gStruct.getData()->Id, "x3");
			gStruct.getData()->inum = 3;
			gStruct.getData()->dnum = 3.3;
			strcpy(gStruct.getData()->astring, "x...............3");
			gStruct.getData()->adate.Year = 1;
			stmt->Prepare("insert into XTEST1 (id,inum,dnum,adate,astring) values (:a,:b,:c,:d,:e)");
			stmt->BindVa(
					BINDPARAM_String(gStruct.getData()->Id, MAX_LEN_STR10),
					BINDPARAM_Long(gStruct.getData()->inum),
					BINDPARAM_Double(gStruct.getData()->dnum),
					BINDPARAM_DateTime(gStruct.getData()->adate),
					BINDPARAM_String(gStruct.getData()->astring, MAX_LEN_STR200),
					BIND_None);
			stmt->Execute();
			strcpy(gStruct.getData()->Id, "x4");
			gStruct.getData()->inum = 4;
			gStruct.getData()->dnum = 4.4;
			strcpy(gStruct.getData()->astring, "x...............4");
			gStruct.getData()->adate.Year = 2006;
			gStruct.getData()->adate.Month = 5;
			gStruct.getData()->adate.Day = 22;
			gStruct.getData()->adate.Hour = 11;
			gStruct.getData()->adate.Minute = 22;
			gStruct.getData()->adate.Second = 33;
			stmt->Execute();
		} else {
			cout << "drop table stmt failed!" << endl;
		}
	} else {
		cout << "nothing to do" << endl;
	}
}

static void _select_data(CConnection *pConn, const char *iName)
{
	S1 gStruct;
	cout << "select data [" << iName << "]: ";
	if (pConn) {
		cout << endl;
		CStatement *stmt = pConn->createStatement();
		stmt->Prepare("select id,inum,dnum,adate,astring from xtest1 where id=:a or astring like :b or 1=1");
		stmt->BindVa(
				BINDPARAM_String("x1", MAX_LEN_STR10),
				BINDPARAM_String("%..4", MAX_LEN_STR200),
				BINDRESULT_String(gStruct.getData()->Id, MAX_LEN_STR10),
				BINDRESULT_Long(gStruct.getData()->inum),
				BINDRESULT_Double(gStruct.getData()->dnum),
				BINDRESULT_DateTime(gStruct.getData()->adate),
				BINDRESULT_String(gStruct.getData()->astring, MAX_LEN_STR200),
				BIND_None);
		stmt->Execute();
		while (stmt->Next()) {
			dump_record(gStruct);
		}
	} else {
		cout << "nothing to do" << endl;
	}
}

#define CONN1 "C1"
#define CONN2 "C2"

void test_common(CConnection *pConn1, CConnection *pConn2)
{
	pConn1->getEnvironment()->setExceptionHandler(dummy_exception_handler);
	if (pConn2) {
		pConn2->getEnvironment()->setExceptionHandler(dummy_exception_handler);
	}
	_connect(pConn1, CONN1);
	_connect(pConn2, CONN2);
	_create_tables(pConn1, CONN1);
//	_create_tables(pConn2, CONN2);
	_insert_data(pConn1, CONN1);
//	_insert_data(pConn2, CONN2);
	_select_data(pConn1, CONN1);
//	_select_data(pConn2, CONN2);
	_drop_tables(pConn1, CONN1);
//	_drop_tables(pConn2, CONN2);
}

#if 0
void test_common(CConnection *pConn, CConnection *pConn2)
{
	S1 gStruct;
	char *currentStmt;

	printf("connecting:\n");
	pConn->Connect();
	if (pConn2) pConn2->Connect();
	//pConn->setAutocommit(true);
	printf("done.\n");

	printf("std record select:\nINPUT =");
	gStruct.reset();
	strcpy(gStruct.getData()->Id, "3");
	dump_record(gStruct);
	pConn->StdRecSelect(gStruct, "T1");
	cout << "OUTPUT=";
	dump_record(gStruct);

	printf("std record update:\nINPUT = LAST OUTPUT\n");
	strcpy(gStruct.getData()->astring, "UPDATED");
	pConn->StdRecUpdate(gStruct, "T1");
	gStruct.reset();
	strcpy(gStruct.getData()->Id, "3");
	pConn->StdRecSelect(gStruct, "T1");
	cout << "OUTPUT=";
	dump_record(gStruct);

	printf("std record insert:\nINPUT =");
	strcpy(gStruct.getData()->Id, "3new");
	pConn->StdRecInsert(gStruct, "T1");
	gStruct.reset();
	strcpy(gStruct.getData()->Id, "3new");
	pConn->StdRecSelect(gStruct, "T1");
	cout << "OUTPUT=";
	dump_record(gStruct);

	printf("std record delete:\nINPUT = LAST OUTPUT\n");
	pConn->StdRecDelete(gStruct, "T1");
	gStruct.reset();
	strcpy(gStruct.getData()->Id, "3new");
	pConn->StdRecSelect(gStruct, "T1");
	cout << "OUTPUT=";
	dump_record(gStruct);
	printf("done.\n");

	gStruct.reset();
	CStatement *stmt1 = pConn->createStatement();
	currentStmt = "insert into t1 (id,inum,dnum,adate,astring) values (:a,:b,:c,:d,:e)";
	printf("executing stmt: [%s]\n", currentStmt);
	stmt1->Prepare(currentStmt);
	strcpy(gStruct.getData()->Id, "NEW1");
	strcpy(gStruct.getData()->astring, "TEST1");
	stmt1->BindVa(
			BINDPARAM_String(gStruct.getData()->Id, MAX_LEN_STR10),
			BINDPARAM_Long(gStruct.getData()->inum),
			BINDPARAM_Double(gStruct.getData()->dnum),
			BINDPARAM_DateTime(gStruct.getData()->adate),
			BINDPARAM_String(gStruct.getData()->astring, MAX_LEN_STR200),
			BIND_None);
	stmt1->Execute();
	strcpy(gStruct.getData()->Id, "NEW2");
	strcpy(gStruct.getData()->astring, "TEST200");
	stmt1->Execute();
	stmt1->Reset();
	//pConn->Commit();

	currentStmt = "delete from t1 where id like :a";
	printf("executing stmt: [%s]\n", currentStmt);
	stmt1->Prepare(currentStmt);
	stmt1->BindVa(BINDPARAM_String(gStruct.getData()->Id, MAX_LEN_STR10), BIND_None);
	strcpy(gStruct.getData()->Id, "1");
	stmt1->Execute();
	strcpy(gStruct.getData()->Id, "2");
	stmt1->Execute();
	strcpy(gStruct.getData()->Id, "XNEW%");
	stmt1->Execute();
	//pConn->Commit();

	CStatement *stmt2 = pConn->createStatement();
	currentStmt = "select *{T1} from t1 order by id";
	printf("executing stmt: [%s]\n", currentStmt);
	stmt2->Prepare(currentStmt);
	stmt2->BindVa(
			BINDRESULT_Struct(gStruct),
			BIND_None);
	stmt2->Execute();
	CResult *Result = stmt2->createResult();
	printf("RESULTS: ===============\n");
	while (Result->Next()) {
		dump_record(gStruct);
	}

	if (pConn2) {
		CStatement *stmt3 = pConn2->createStatement();
		currentStmt = "select id,inum,astring,dnum,adate from t1 order by id";
		printf("executing stmt: [%s]\n", currentStmt);
		stmt3->Prepare(currentStmt);
		stmt3->BindVa(
				BINDRESULT_String(gStruct.getData()->Id, MAX_LEN_STR10),
				BINDRESULT_Long(gStruct.getData()->inum),
				BINDRESULT_String(gStruct.getData()->astring, MAX_LEN_STR200),
				BINDRESULT_Double(gStruct.getData()->dnum),
				BINDRESULT_DateTime(gStruct.getData()->adate),
				BIND_None);
		stmt3->Execute();
		CResult *Result3 = stmt3->createResult();
		printf("RESULTS: ===============\n");
		while (Result3->Next()) {
			dump_record(gStruct);
		}
	}

	//pConn->Rollback();

	printf("===============\n");
	printf("disconnecting:\n");
	pConn->Disconnect();
	if (pConn2) pConn2->Disconnect();
	printf("done.\n");

}
#endif