
#include "main.h"
#include <boost/version.hpp>
#include <openssl/opensslv.h>
#include "MySAX2Handler.h"

using namespace boost::program_options;

Globals appGlobals;

static void handle_options(int argc, char **argv) {
	
	appGlobals.AmtIn = appGlobals.AmtOut = 0.0;
	appGlobals.cntPos = appGlobals.cntPosDuplicate = 0;
	appGlobals.pConn = 0;
	appGlobals.pEnv = 0;

	options_description allowed("Allowed options");
	allowed.add_options()
		(OPT_HELP",h",									"produce help message")
		(OPT_VERBOSE",v",								"verbose mode")
		(OPT_COMMIT,									"commit inserted data (with --"OPT_IMPORT")")
		(OPT_IMPORT,									"insert data into database")
		(OPT_USER,			value<string>(),			"database user (default="OPT_USER_DEFAULT")")
		(OPT_PASSWD,		value<string>(),			"database password (default="OPT_PASSWD_DEFAULT")")
		(OPT_HOST,			value<string>(),			"database host (default="OPT_HOST_DEFAULT")")
	;

	options_description hidden("Hidden options");
	hidden.add_options()
		(OPT_INPUTFILE",f",	value< vector<string> >(),	"input file")
	;
	options_description all("All options");
	all.add(allowed).add(hidden);

	positional_options_description pd;
	pd.add(OPT_INPUTFILE, -1);

	try {
		store(command_line_parser(argc, argv).options(all).positional(pd).run(), appGlobals.params);
		notify(appGlobals.params);
	} catch (error e) {
		cout << endl << "*** " << e.what() << endl << endl;
		cout << allowed << endl;
		exit(-1);
	}
	
	if (appGlobals.params.count(OPT_HELP) || !appGlobals.params.count(OPT_INPUTFILE)) {
		cout << argv[0] << ", v"VERSION", (c) kuk, 2006" << endl <<
			"using: " <<
			"boost v" << BOOST_VERSION/100000 << "." << BOOST_VERSION/100%1000 << "." << BOOST_VERSION%100 << ", " <<
			"xerces v"XERCES_FULLVERSIONDOT", " <<
			OPENSSL_VERSION_TEXT", " <<
			KLIBS_NAME" v" << KLIBS_VERSION_MAJOR<<"."<<KLIBS_VERSION_MINOR<<"."<<KLIBS_VERSION_REVISION << "[" << klib::base::LibDependencies() << "]" <<
			endl;
		cout << allowed << endl;
		exit(0);
	}
	if (appGlobals.params.count(OPT_VERBOSE)) {
		appGlobals.mode_verbose = true;
		if (appGlobals.params.count(OPT_INPUTFILE)) {
			vector<string> x = appGlobals.params[OPT_INPUTFILE].as< vector<string> >();
			cout << "Input files are: [";
			for (unsigned int i = 0; i < x.size(); i++) {
				cout << " " << x[i];
			}
			cout << "]" << endl;
		}
	} else {
		appGlobals.mode_verbose = false;
	}
	if (appGlobals.params.count(OPT_IMPORT)) {
		appGlobals.mode_import = true;
	} else {
		appGlobals.mode_import = false;
	}
}

static void init_libs(void) {
	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) {
		cout << endl << "*** " << toCatch.getMessage() << endl << endl;
		exit(-1);
	}
}

static void done_libs(void) {
	XMLPlatformUtils::Terminate();
}

static void parse_file(const char *iFile) {
	SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();	
    parser->setFeature(XMLUni::  fgXercesLoadExternalDTD, false);	// Ignore the external DTD completely.
	MySAX2Handler* defaultHandler = new MySAX2Handler();
	parser->setContentHandler(defaultHandler);
	parser->setErrorHandler(defaultHandler);
	try {
		parser->parse(iFile);
	}
	catch (const XMLException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		cout << "*** Exception message is:" << endl	<< message << endl;
		XMLString::release(&message);
		//return;
	}
	catch (const SAXParseException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		cout << "*** Exception message is:" << endl	<< message << endl;
		XMLString::release(&message);
		//return;
	}
	catch (const klib::base::Exception &e) {
		// nothing to do: already logged out
	}
	catch (const exception &e) {
		cout << "*** Unexpected Exception: " << e.what() << endl;
		//return;
	}
	catch (...) {
		cout << "*** Unexpected Exception! ***" << endl;
		//return;
	}

	delete parser;
	delete defaultHandler;
}

static void log_statistics(void) {
	cout << "Auszugspositionen: " << appGlobals.cntPos << endl
		<< " -> davon bereits in der Datenbank: " << appGlobals.cntPosDuplicate << endl
		<< "Summe Eingaenge: " << appGlobals.AmtIn << endl
		<< "Summe Ausgaenge: " << appGlobals.AmtOut * -1.0 << endl;
}

static void init_database(void) {
//	static Dbi_C_Logger Logger;
//	Logger.setLogLevel(Dbi_C_Logger::LogLevel_Trace);
	appGlobals.pEnv = new CEnvironment(InterfaceType_mysql5, new klib::base::CLogger(new klib::base::CLogWriter_stdout("stdout")));
//	appGlobals.pEnv->setExceptionHandler(DbiExceptionHandler_Stdout);
	appGlobals.pEnv->getLogHandler()->setLogLevel(klib::base::LogLevel_Warning);
	string user = OPT_USER_DEFAULT, passwd = OPT_PASSWD_DEFAULT, host = OPT_HOST_DEFAULT;

	if (appGlobals.params.count(OPT_USER)) {
		user = appGlobals.params[OPT_USER].as<string>();
	}
	if (appGlobals.params.count(OPT_PASSWD)) {
		passwd = appGlobals.params[OPT_PASSWD].as<string>();
	}
	if (appGlobals.params.count(OPT_HOST)) {
		host = appGlobals.params[OPT_HOST].as<string>();
	}
	appGlobals.pConn = appGlobals.pEnv->createConnection(host.c_str(), user.c_str(), passwd.c_str(), /*database*/user.c_str());
	appGlobals.pConn->Connect();
	if (appGlobals.pConn->isConnected() == false) {
		exit(-1);
	}
//	if (appGlobals.params.count(OPT_COMMIT)) {
//		appGlobals.pConn->setAutocommit(true);
//	}
}

static void done_database(void) {
	if (appGlobals.pEnv) {
		if (appGlobals.params.count(OPT_COMMIT)) {
			appGlobals.pConn->Commit();
		}
		delete appGlobals.pEnv;
		appGlobals.pEnv = 0;
		appGlobals.pConn = 0;
	}
}

int main(int argc, char **argv) {
	init_libs();
	handle_options(argc, argv);
	if (appGlobals.params.count(OPT_INPUTFILE)) {
		vector<string> x = appGlobals.params[OPT_INPUTFILE].as< vector<string> >();
		if (appGlobals.mode_import) init_database();
		for (unsigned int i = 0; i < x.size(); i++) {
			cout << '{' << x[i] << '}' << endl;
			parse_file(x[i].c_str());
		}
		if (appGlobals.mode_import) done_database();
	}
	log_statistics();
	done_libs();
}
