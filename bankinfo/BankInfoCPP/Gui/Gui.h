// Gui.h : Hauptheaderdatei für die Gui-Anwendung
//
#pragma once

#include "resource.h"       // Hauptsymbole
#include <XTToolkitPro.h> // Xtreme Toolkit Pro component library
#include <klibdbi.h>
#include <datenmodell.h>


// CGuiApp:
// Siehe Gui.cpp für die Implementierung dieser Klasse
//

class CGuiApp : public CWinApp
{
private:
	klib::dbi::CEnvironment *m_pDbEnv;
	klib::dbi::CConnection *m_pDbConn;
	klib::base::CLogger m_Logger;
	klib::base::BArray *m_Kontenliste;
	klib::base::BArray *m_Positionsliste;
public:
	CGuiApp();
	~CGuiApp();
	klib::base::CLogger &getLogger(void);
	klib::dbi::CEnvironment *getDbEnv(void) const;
	klib::base::BArray *getKontenliste(void) const { return m_Kontenliste; }
	klib::base::BArray *getPositionsliste(void) const { return m_Positionsliste; }
	bool connectDatabase(void);
	bool disconnectDatabase(void);
	bool ladeKonten(void);
	bool ladePositionen(const long KontId, long &oCnt);

// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGuiApp theApp;
