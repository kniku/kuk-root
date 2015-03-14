#include "dbitest.h"

void test_xxx(CConnection *pConn, CConnection *pConn2)
{
}

void test_sqlite()
{
#if TRY_SQLITE
	cout << "===== starting test: InterfaceType: sqlite3 =====" << endl;
	CEnvironment Env(InterfaceType_sqlite3, new klib::base::CLogger(new klib::base::CLogWriter_stdout("stdout")));
	Env.getLogHandler()->setLogLevel(klib::base::LogLevel_Trace);
	Env.getLogHandler()->setLogFormat("[$(Level)] $(Message)");
	CConnection *pConn = Env.createConnection(/*host*/0, /*user*/0, /*passwd*/0, /*database*/"sqlite_test.db");
	CConnection *pConn2 = Env.createConnection(/*host*/0, /*user*/0, /*passwd*/0, /*database*/"sqlite_test.db");

	test_common(pConn, pConn2);

#endif
}
