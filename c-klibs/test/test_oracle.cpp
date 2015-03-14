#include "dbitest.h"

void test_oracle()
{
#if TRY_ORACLE
	printf("===== starting test: InterfaceType: oracle9 =====\n");
	printf("creating:\n");
	CEnvironment Env(InterfaceType_oracle9, new klib::base::CLogger(new klib::base::CLogWriter_stdout("stdout")));
	Env.getLogHandler()->setLogLevel(klib::base::LogLevel_Trace);
	Env.getLogHandler()->setLogFormat("[$(Level)] $(Message)");
//	Env.setExceptionHandler(DbiExceptionHandler_Stdout);
	CConnection *pConn = Env.createConnection(/*host*/"note33", /*user*/"dbitest", /*passwd*/"dbitest", /*database*/NULL);
	CConnection *pConn2 = Env.createConnection(/*host*/"note33", /*user*/"dbitest", /*passwd*/"dbitest", /*database*/NULL);
	printf("done.\n");

	test_common(pConn, pConn2);

#endif
}
