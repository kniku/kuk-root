#include "dbitest.h"

void test_mysql()
{
#if TRY_MYSQL
	printf("===== starting test: InterfaceType: mysql5 =====\n");
	printf("creating:\n");
	CEnvironment Env(InterfaceType_mysql5, new klib::base::CLogger(new klib::base::CLogWriter_stdout("stdout")));
	Env.getLogHandler()->setLogLevel(klib::base::LogLevel_Trace);
	Env.getLogHandler()->setLogFormat("[$(Level)] $(Message)");
	//Env.setExceptionHandler(DbiExceptionHandler_Stdout);
#ifdef SYSTEM_WIN32
	CConnection *pConn = Env.createConnection(/*host*/"localhost", /*user*/"dbitest", /*passwd*/"dbitest", /*database*/"dbitest");
	CConnection *pConn2 = Env.createConnection(/*host*/"localhost", /*user*/"dbitest", /*passwd*/"dbitest", /*database*/"dbitest");
#else
	CConnection *pConn = Env.createConnection(/*host*/"note33", /*user*/"dbitest", /*passwd*/"dbitest", /*database*/"dbitest");
	CConnection *pConn2 = Env.createConnection(/*host*/"note33", /*user*/"dbitest", /*passwd*/"dbitest", /*database*/"dbitest");
#endif
	printf("done.\n");

	test_common(pConn, pConn2);

//	Env.destroyConnection(pConn);
#endif
}
