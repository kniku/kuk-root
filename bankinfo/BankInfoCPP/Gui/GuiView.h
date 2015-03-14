// GuiView.h : Schnittstelle der Klasse CGuiView
//


#pragma once


class CGuiView : public CView
{
protected: // Nur aus Serialisierung erstellen
	CGuiView();
	DECLARE_DYNCREATE(CGuiView)

// Attribute
public:
	CGuiDoc* GetDocument() const;

// Operationen
public:

// Überschreibungen
	public:
	virtual void OnDraw(CDC* pDC);  // Überladen, um diese Ansicht darzustellen
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementierung
public:
	virtual ~CGuiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Funktionen für die Meldungstabellen
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Debugversion in GuiView.cpp
inline CGuiDoc* CGuiView::GetDocument() const
   { return reinterpret_cast<CGuiDoc*>(m_pDocument); }
#endif

