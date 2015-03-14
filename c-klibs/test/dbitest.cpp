#include "dbitest.h"

void dummy_exception_handler(const class CEnvironment *iEnv, const klib::base::Exception &ex)
{
	// just catch it
}

int main(int argc, char* argv[])
{
	cout << "using ["KLIBS_NAME" v" 
		<< KLIBS_VERSION_MAJOR 
		<< "." 
		<< KLIBS_VERSION_MINOR 
		<< "."  
		<< KLIBS_VERSION_REVISION 
		<< "[" 
		<< klib::base::LibDependencies() 
		<< "]]\n";

	DbiGen_M1::RegisterTablesOf_M1();
//	LookupTable("T2");

	test_mysql();
	test_oracle();
	test_sqlite();

	return 0;
}

