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

// CGuiView Erstellung/Zerst�rung

CGuiView::CGuiView()
{
	// TODO: Hier Code zum Erstellen einf�gen

}

CGuiView::~CGuiView()
{
}

BOOL CGuiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ndern Sie hier die Fensterklasse oder die Darstellung, indem Sie
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

	// TODO: Code zum Zeichnen der systemeigenen Daten hinzuf�gen
}


// CGuiView drucken

BOOL CGuiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Standardvorbereitung
	return DoPreparePrinting(pInfo);
}

void CGuiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: Zus�tzliche Initialisierung vor dem Drucken hier einf�gen
}

void CGuiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: Bereinigung nach dem Drucken einf�gen
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
