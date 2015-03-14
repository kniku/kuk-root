// GuiView.cpp : Implementierung der Klasse CGuiView
//

#include "stdafx.h"
#include "Gui.h"

#include "GuiDoc.h"
#include "GuiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGuiView

IMPLEMENT_DYNCREATE(CGuiView, CView)

BEGIN_MESSAGE_MAP(CGuiView, CView)
	// Standarddruckbefehle
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CGuiView Erstellung/Zerstörung

CGuiView::CGuiView()
{
	// TODO: Hier Code zum Erstellen einfügen

}

CGuiView::~CGuiView()
{
}

BOOL CGuiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Ändern Sie hier die Fensterklasse oder die Darstellung, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CView::PreCreateWindow(cs);
}

// CGuiView-Zeichnung

void CGuiView::OnDraw(CDC* /*pDC*/)
{
	CGuiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: Code zum Zeichnen der systemeigenen Daten hinzufügen
}


// CGuiView drucken

BOOL CGuiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Standardvorbereitung
	return DoPreparePrinting(pInfo);
}

void CGuiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: Zusätzliche Initialisierung vor dem Drucken hier einfügen
}

void CGuiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: Bereinigung nach dem Drucken einfügen
}


// CGuiView Diagnose

#ifdef _DEBUG
void CGuiView::AssertValid() const
{
	CView::AssertValid();
}

void CGuiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGuiDoc* CGuiView::GetDocument() const // Nicht-Debugversion ist inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGuiDoc)));
	return (CGuiDoc*)m_pDocument;
}
#endif //_DEBUG


// CGuiView Meldungshandler
