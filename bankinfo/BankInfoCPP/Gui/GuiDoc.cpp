// GuiDoc.cpp : Implementierung der Klasse CGuiDoc
//

#include "stdafx.h"
#include "Gui.h"

#include "GuiDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGuiDoc

IMPLEMENT_DYNCREATE(CGuiDoc, CDocument)

BEGIN_MESSAGE_MAP(CGuiDoc, CDocument)
END_MESSAGE_MAP()


// CGuiDoc Erstellung/Zerstörung

CGuiDoc::CGuiDoc()
{
	// TODO: Hier Code für One-Time-Konstruktion einfügen

}

CGuiDoc::~CGuiDoc()
{
}

BOOL CGuiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: Hier Code zur Reinitialisierung einfügen
	// (SDI-Dokumente verwenden dieses Dokument)

	return TRUE;
}




// CGuiDoc Serialisierung

void CGuiDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: Hier Code zum Speichern einfügen
	}
	else
	{
		// TODO: Hier Code zum Laden einfügen
	}
}


// CGuiDoc Diagnose

#ifdef _DEBUG
void CGuiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGuiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGuiDoc-Befehle
