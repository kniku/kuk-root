// sTimer.h : Hauptheaderdatei f�r die sTimer-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error 'stdafx.h' muss vor dieser Datei in PCH eingeschlossen werden.
#endif

#include "resource.h"		// Hauptsymbole


// CsTimerApp:
// Siehe sTimer.cpp f�r die Implementierung dieser Klasse
//

class CsTimerApp : public CWinApp
{
public:
	CsTimerApp();

// �berschreibungen
	public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CsTimerApp theApp;
