// Gui.cpp : Definiert das Klassenverhalten für die Anwendung.
//

#include "stdafx.h"
#include "Gui.h"
#include "MainFrm.h"

#include "GuiDoc.h"
#include "GuiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGuiApp

BEGIN_MESSAGE_MAP(CGuiApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Dateibasierte Standarddokumentbefehle
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standarddruckbefehl "Seite einrichten"
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CGuiApp-Erstellung

CGuiApp::CGuiApp()
{
	m_pDbConn = 0;
	m_pDbEnv = 0;
	m_Kontenliste = 0;
	m_Positionsliste = 0;
	DbiGen_M::RegisterTablesOf_M();
}

CGuiApp::~CGuiApp() {
	disconnectDatabase();
}

// Das einzige CGuiApp-Objekt

CGuiApp theApp;

// CGuiApp Initialisierung

BOOL CGuiApp::InitInstance()
{
	// InitCommonControls() ist für Windows XP erforderlich, wenn ein Anwendungsmanifest
	// die Verwendung von ComCtl32.dll Version 6 oder höher zum Aktivieren
	// von visuellen Stilen angibt. Ansonsten treten beim Erstellen von Fenstern Fehler auf.
	InitCommonControls();

	CWinApp::InitInstance();

	// OLE-Bibliotheken initialisieren
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// Ändern Sie den Registrierungsschlüssel unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));
	LoadStdProfileSettings(4);  // Standard INI-Dateioptionen laden (einschließlich MRU)
	// Dokumentvorlagen der Anwendung registrieren. Dokumentvorlagen
	//  dienen als Verbindung zwischen Dokumenten, Rahmenfenstern und Ansichten.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGuiDoc),
		RUNTIME_CLASS(CMainFrame),       // Haupt-SDI-Rahmenfenster
		RUNTIME_CLASS(CGuiView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// Befehlszeile parsen, um zu prüfen auf Standardumgebungsbefehle DDE, Datei offen
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Verteilung der in der Befehlszeile angegebenen Befehle. Es wird FALSE zurückgegeben, wenn
	// die Anwendung mit /RegServer, /Register, /Unregserver oder /Unregister gestartet wurde.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// Das einzige Fenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// Rufen Sie DragAcceptFiles nur auf, wenn eine Suffix vorhanden ist.
	//  In einer SDI-Anwendung ist dies nach ProcessShellCommand erforderlich
	return TRUE;
}

klib::base::CLogger &CGuiApp::getLogger(void) {
	return m_Logger;
}

klib::dbi::CEnvironment *CGuiApp::getDbEnv(void) const {
	return m_pDbEnv;
}

bool CGuiApp::connectDatabase(void) {
	bool r = false;
	if (m_pDbEnv == 0 && m_pDbConn == 0) {
		m_pDbEnv = new klib::dbi::CEnvironment(klib::dbi::InterfaceType_mysql5, new klib::base::CLogger(new klib::base::CLogWriter_file("logfilewriter", "GuiLog.log")));
		if (m_pDbEnv) {
			m_pDbConn = m_pDbEnv->createConnection("note33","bankinfo_echt","bankinfo_echt","bankinfo_echt");
//			m_pDbConn = m_pDbEnv->createConnection("note33","bankinfo","bankinfo","bankinfo");
			try {
				m_pDbConn->Connect();
				r = true;
			} catch (...) {
				// just catch it...
			}
		}
	}
	return r;
}

bool CGuiApp::disconnectDatabase(void) {
	if (m_pDbEnv) {
		delete m_pDbEnv;
		m_pDbEnv = 0;
		m_pDbConn = 0;
	}
	return true;
}

bool CGuiApp::ladeKonten(void) {
	bool r = false;
	if (m_Kontenliste != 0) {
		delete m_Kontenliste;
	}
	m_Kontenliste = new klib::base::BArray(sizeof(DbiGen_M::D_Konten));
	if (m_pDbConn && m_pDbConn->isConnected()) {
		DbiGen_M::Konten konto;
		klib::dbi::CStatement *pStmt = m_pDbConn->createStatement();
		try {
			pStmt->Prepare("select *{Konten} from konten order by ktonr");
			pStmt->BindVa(BINDRESULT_Struct(konto), BIND_None);
			pStmt->Execute();
//			klib::dbi::CResult *pResult = pStmt->createResult();
//			while (pResult->Next()) {
			while (pStmt->Next()) {
//				getLogger().Log(klib::base::LogLevel_Trace, konto.getData()->KtoNr);
				m_Kontenliste->Append(konto.getData());
			}
			m_pDbConn->destroyStatement(pStmt);
			r = true;
		} catch (...) {
			// just catch the exception...
		}
	}
	return r;
}

bool CGuiApp::ladePositionen(const long KontoId, long &cnt) {
	bool r = false;
	if (m_Positionsliste != 0) {
		delete m_Positionsliste;
	}
	m_Positionsliste = new klib::base::BArray(sizeof(DbiGen_M::D_Positionen));
	if (m_pDbConn && m_pDbConn->isConnected()) {
		DbiGen_M::Positionen pos;
		klib::dbi::CStatement *pStmt = m_pDbConn->createStatement();
		cnt = 0;
		try {
//			pStmt->Prepare("select *{Positionen} from positionen where ktoid=:a order by PostedDt desc");
			pStmt->Prepare("select *{Positionen} from positionen where ktoid=1 order by PostedDt desc");
			pStmt->BindVa(
				BINDRESULT_Struct(pos),
//				BINDPARAM_Long(KontoId),
				BIND_None);
			pStmt->Execute();
			klib::dbi::CResult *pResult = pStmt->createResult();
			while (pResult->Next()) {
//				getLogger().Log(klib::base::LogLevel_Trace, pos.getData()->Id);
				m_Positionsliste->Append(pos.getData());
				cnt++;
			}
			m_pDbConn->destroyStatement(pStmt);
			r = true;
		} catch (...) {
			// just catch the exception...
		}
	}
	return r;
}

// CAboutDlg-Dialogfeld für Anwendungsbefehl 'Info'

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// Anwendungsbefehl zum Ausführen des Dialogfelds
void CGuiApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CGuiApp Meldungshandler

