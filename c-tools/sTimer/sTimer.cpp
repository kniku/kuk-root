// sTimer.cpp : Definiert das Klassenverhalten für die Anwendung.
//

#include "stdafx.h"
#include "sTimer.h"
#include "sTimerDlg.h"
#include <string>
#include <boost/program_options.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace boost::program_options;

// CsTimerApp

BEGIN_MESSAGE_MAP(CsTimerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CsTimerApp-Erstellung

CsTimerApp::CsTimerApp()
{
	// TODO: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance positionieren
}


// Das einzige CsTimerApp-Objekt

CsTimerApp theApp;


// CsTimerApp Initialisierung

static void print_usage() {
	string s = "sTimer, v1.0, (c) kuk 2007";
	s += "\n\nSimple Timer.\nWaits a given timeout, displays the message and terminates...";
	s += "\n\nusage: sTimer [-h hours] [-m minutes] [-s seconds] [message]";
	AfxMessageBox(s.c_str(), MB_SYSTEMMODAL+MB_OK+MB_ICONINFORMATION, 0);
	exit(0);
}

BOOL CsTimerApp::InitInstance()
{
	// InitCommonControls() ist für Windows XP erforderlich, wenn ein Anwendungsmanifest
	// die Verwendung von ComCtl32.dll Version 6 oder höher zum Aktivieren
	// von visuellen Stilen angibt. Ansonsten treten beim Erstellen von Fenstern Fehler auf.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// Ändern Sie den Registrierungsschlüssel unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
//	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));

	variables_map params;

	options_description allowed("Allowed options");
	allowed.add_options()
		("help",										"produce help message")
		(",h",				value<int>(),				"hours (default=0)")
		(",m",				value<int>(),				"minutes (default=0)")
		(",s",				value<int>(),				"seconds (default=0)")
	;

	options_description hidden("Hidden options");
	hidden.add_options()
		("message",			value< vector<string> >(),	"message")
	;

	options_description all("All options");
	all.add(allowed).add(hidden);

	positional_options_description pd;
	pd.add("message", -1);

	try {
		vector<string> args = split_winmain(theApp.m_lpCmdLine);
		store(command_line_parser(args).options(all).positional(pd).run(), params);
		notify(params);
	} catch (error e) {
		AfxMessageBox(e.what(), MB_SYSTEMMODAL+MB_OK, 0);
		exit(-1);
	}

	int seconds = 0;
	int minutes = 0;
	int hours = 0;

	if (params.count("-s")) {
		seconds = params["-s"].as<int>();
	}
	if (params.count("-m")) {
		minutes = params["-m"].as<int>();
	}
	if (params.count("-h")) {
		hours = params["-h"].as<int>();
	}
	
	int timeout = hours * 60 * 60 + minutes * 60 + seconds;

	if (timeout <= 0) print_usage();
	Sleep(timeout * 1000);
	string msg = "*** TIMEOUT ***";
	if (params.count("message")) {
		msg += "\n\n[";
		vector<string> x = params["message"].as< vector<string> >();
		for (unsigned int i = 0; i < x.size(); i++) {
			if (i != 0) msg += " ";
			msg += x[i];
		}
		msg += "]";
	}
	AfxMessageBox(msg.c_str(), MB_SYSTEMMODAL+MB_OK+MB_ICONINFORMATION, 0);

	return FALSE;
}
