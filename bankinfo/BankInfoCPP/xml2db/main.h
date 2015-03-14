#ifndef MAIN_H
#define MAIN_H

#define VERSION_MAJOR		"0"
#define VERSION_MINOR		"1"
#define VERSION_SUBMINOR	"0"
#define VERSION	VERSION_MAJOR"."VERSION_MINOR"."VERSION_SUBMINOR

#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <klibdbi.h>

using namespace std;
using namespace klib::dbi;

// program options
#define OPT_IMPORT			"import"
#define OPT_COMMIT			"commit"
#define OPT_HELP			"help"
#define OPT_VERBOSE			"verbose"
#define OPT_INPUTFILE		"input-file"
#define OPT_USER			"user"
#define OPT_USER_DEFAULT	"bankinfo"
#define OPT_PASSWD			"passwd"
#define OPT_PASSWD_DEFAULT	"bankinfo"
#define OPT_HOST			"host"
#define OPT_HOST_DEFAULT	"localhost"

struct Globals {
	// program options
	boost::program_options::variables_map params;
	bool mode_verbose;
	bool mode_import;
	// database connection
	CEnvironment *pEnv;
	CConnection *pConn;
	// statistics
	double AmtIn, AmtOut;
	unsigned long cntPos;
	unsigned long cntPosDuplicate;
};

extern Globals appGlobals;

#endif	// MAIN_H

