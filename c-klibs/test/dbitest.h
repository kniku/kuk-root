#ifndef _DBITEST_H
#define _DBITEST_H

#include <klibdbi.h>
#include <iostream>
#include "generated/database.h"

#define TRY_ORACLE			1
#define TRY_MYSQL			1
#define TRY_SQLITE			1

using namespace std;
using namespace klib::base;
using namespace klib::dbi;

//#define _UNICODE

void dummy_exception_handler(const class CEnvironment *iEnv, const klib::base::Exception &ex);

void test_common(CConnection *pConn, CConnection *pConn2);

void test_mysql(void);
void test_oracle(void);
void test_sqlite(void);

#endif	// _DBITEST_H
