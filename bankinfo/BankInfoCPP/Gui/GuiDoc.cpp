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


// CGuiDoc Erstellung/Zerst�rung

CGuiDoc::CGuiDoc()
{
	// TODO: Hier Code f�r One-Time-Konstruktion einf�gen

}

CGuiDoc::~CGuiDoc()
{
}

BOOL CGuiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: Hier Code zur Reinitialisierung einf�gen
	// (SDI-Dokumente verwenden dieses Dokument)

	return TRUE;
}




// CGuiDoc Serialisierung

void CGuiDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: Hier Code zum Speichern einf�gen
	}
	else
	{
		// TODO: Hier Code zum Laden einf�gen
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
