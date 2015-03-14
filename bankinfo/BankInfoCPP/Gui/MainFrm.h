// MainFrm.h : Schnittstelle der Klasse CMainFrame
//


#pragma once
class kCXTPTaskPanel : public CXTPTaskPanel
{
public:
	void refreshPositionen(std::string *iFilter);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFilter();
protected:
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

class CMainFrame : public CXTPFrameWnd
{
	
protected: // Nur aus Serialisierung erstellen
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attribute
public:

// Operationen
public:
	CXTSplitterWndEx m_wndSplitter;
	kCXTPTaskPanel m_wndTaskPanel;
	CXTPReportControl m_wndLogger;
	CXTPReportControl m_wndPositionen;
	CEdit *m_wndFilter;
// Überschreibungen
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementierung
public:
	virtual ~CMainFrame();

protected:  // Eingebundene Elemente der Steuerleiste
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generierte Funktionen für die Meldungstabellen
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


