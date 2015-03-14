// MainFrm.cpp : Implementierung der Klasse CMainFrame
//

#include "stdafx.h"
#include "Gui.h"

#include "MainFrm.h"
#include ".\mainfrm.h"
#include <string>
#include <boost/format.hpp>
//#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // Statusleistenanzeige
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame Erstellung/Zerstörung

CMainFrame::CMainFrame()
{
	// TODO: Hier Code für die Memberinitialisierung einfügen
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Create Status bar.
    // Important: All control bars including the Status Bar
    // must be created before CommandBars....
    if (!m_wndStatusBar.Create(this) ||
        !m_wndStatusBar.SetIndicators(indicators,
        sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }

    // Initialize the command bars
    if (!InitCommandBars())
        return -1;

    // Get a pointer to the command bars object.
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if(pCommandBars == NULL)
    {
        TRACE0("Failed to create command bars object.\n");
        return -1;      // fail to create
    }

    // Add the menu bar
    CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(
        _T("Menu Bar"), IDR_MAINFRAME);
    if(pMenuBar == NULL)
    {
        TRACE0("Failed to create menu bar.\n");
        return -1;      // fail to create
    }
#if 0
    // Create ToolBar
    CXTPToolBar* pToolBar = (CXTPToolBar*)
        pCommandBars->Add(_T("Standard"), xtpBarTop);
    if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;
    }
#endif
    // Set Office 2003 Theme
    CXTPPaintManager::SetTheme(xtpThemeOffice2003);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Ändern Sie hier die Fensterklasse oder die Darstellung, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return TRUE;
}


// CMainFrame Meldungshandler

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nID < 50000) {
		int x = 10;
	}
	return CXTPFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	return CXTPFrameWnd::PreTranslateMessage(pMsg);
}

class CLW : klib::base::CLogWriter {
private:
	CXTPReportControl *m_pRepCtrl;
public:
	CLW(const std::string &iName, CXTPReportControl *iRepCtrl) : klib::base::CLogWriter(iName) { m_pRepCtrl = 0; m_pRepCtrl = iRepCtrl; }
	void write(klib::base::LogLevel iLogLevel, const std::string &iMsg) const {
		if (m_pRepCtrl) {
			CXTPReportRecord *pRecord;
			CXTPReportRecordItem *pItem;
			pRecord = new CXTPReportRecord;
			pItem = new CXTPReportRecordItem;
			pItem->SetCaption(iMsg.c_str());
			//pItem->SetEditable(false);
			if (iLogLevel == klib::base::LogLevel_Warning) {
				pItem->SetBackgroundColor(RGB(100,100,255));
			} else if (iLogLevel == klib::base::LogLevel_Error) {
				pItem->SetBackgroundColor(RGB(255,100,100));
			} else {
				pItem->SetTextColor(RGB(100,100,100));
			}
			pRecord->AddItem(pItem);
			m_pRepCtrl->AddRecord(pRecord);
			m_pRepCtrl->Populate();
			m_pRepCtrl->SetTopRow(m_pRepCtrl->GetRows()->GetCount());
		}
	};	
};

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CFont *pFont;
	CXTPTaskPanelGroupItem * pGroupItem;
	CXTPReportColumn *pColumn;
	pColumn = new CXTPReportColumn(0,"internal log",20,true,0,false,true);
	pColumn = m_wndLogger.AddColumn(pColumn);

	pColumn = new CXTPReportColumn(0,"Buchungsdatum",13,true,0,true,true);
	pColumn = m_wndPositionen.AddColumn(pColumn);
	pColumn = new CXTPReportColumn(1,"Valuta",13,true,0,true,true);
	pColumn = m_wndPositionen.AddColumn(pColumn);
	pColumn = new CXTPReportColumn(2,"Betrag",15,true,0,true,true);
	pColumn->SetAlignment(DT_RIGHT);
	pColumn = m_wndPositionen.AddColumn(pColumn);
	pColumn = new CXTPReportColumn(3,"CCY",7,true,0,true,true);
	pColumn = m_wndPositionen.AddColumn(pColumn);
	pColumn = new CXTPReportColumn(4,"Text",100,true,0,true,true);
//	pColumn->SetAlignment(DT_WORDBREAK);
	pColumn = m_wndPositionen.AddColumn(pColumn);
	m_wndPositionen.SetGridStyle(true, xtpReportGridSmallDots);

	theApp.getLogger().addLogWriter((klib::base::CLogWriter *)new CLW("replog", &m_wndLogger));
	theApp.getLogger().setLogFormat("$(Time): $(Message)");

	m_wndSplitter.CreateStatic(this, 1, 2);

	CXTSplitterWndEx *psw = new CXTSplitterWndEx;
	psw->CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD|WS_VISIBLE,m_wndSplitter.IdFromRowCol(0, 0));

	m_wndTaskPanel.Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP, CRect(0, 0, 0, 0), psw, psw->IdFromRowCol(0, 0));
	m_wndTaskPanel.SetOwner(this);
	CXTPTaskPanelGroup* pGroupKonten = m_wndTaskPanel.AddGroup(IDS_TASKGROUP_KONTEN);
	pGroupKonten->SetSpecialGroup();
	CXTPTaskPanelGroup* pGroup = m_wndTaskPanel.AddGroup(IDS_TASKGROUP_AKTIONEN);
	

	pGroupItem = pGroup->AddTextItem("Positionen filtern:");
	pFont = pGroupItem->GetItemFont();
	pGroupItem->SetBold();

	m_wndFilter = new CEdit;
	m_wndFilter->Create(ES_LEFT+ES_AUTOHSCROLL, CRect(0,0,0,15),&m_wndTaskPanel, 0);
	m_wndFilter->SetLimitText(100);
	m_wndFilter->SetFont(pFont);
	pGroup->AddControlItem(m_wndFilter->m_hWnd);

	CButton *pB = new CButton;
	pB->Create("Filter >>",BS_FLAT,CRect(0,0,0,20),&m_wndTaskPanel, 100);
	pB->SetFont(pFont);
	pGroupItem = pGroup->AddControlItem(pB->m_hWnd);

	pGroupItem = pGroup->AddTextItem(" ");
	pGroupItem = pGroup->AddTextItem("Sonstiges:");
	pGroupItem->SetBold();

	pGroup->AddLinkItem(IDS_TASKITEM_LOGFILE);

	m_wndPositionen.Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP, CRect(0, 0, 0, 0), &m_wndSplitter, m_wndSplitter.IdFromRowCol(0, 1));
	m_wndLogger.Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP, CRect(0, 0, 0, 0), psw, psw->IdFromRowCol(1, 0));

	m_wndSplitter.ShowTopBorder(false, 0);
	m_wndSplitter.SetColumnInfo(0, 210, 0);
	m_wndSplitter.SetRowInfo(0, 300, 0);
	m_wndSplitter.RecalcLayout();
	psw->ShowTopBorder(false, 0);
	psw->ShowTopBorder(false, 0);
	psw->SetColumnInfo(0, 210, 0);
	psw->SetRowInfo(0, 500, 0);
	psw->RecalcLayout();

	if (theApp.connectDatabase()) {
		theApp.getLogger().Log(klib::base::LogLevel_Info, "Datenbankverbindung hergestellt...");
	} else {
		MessageBox("Verbindung zur Datenbank fehlgeschlagen!","FEHLER",MB_ICONERROR+MB_OK);
		theApp.getLogger().Log(klib::base::LogLevel_Error, "Keine Datenbankverbindung!");
	}

	if (theApp.ladeKonten()) {
		theApp.getLogger().Log(klib::base::LogLevel_Info, "Kontenliste geladen...");
		DbiGen_M::D_Konten *p = 0;
		while ((p = (DbiGen_M::D_Konten *)theApp.getKontenliste()->IterateNext(p))) {
			CXTPTaskPanelGroupItem *pItem;
			pItem = pGroupKonten->AddLinkItem(IDS_TASKITEM_KONTONUMMER);
			pItem->SetCaption((boost::format("%2%: %1%") % p->KtoNr % p->CurCode ).str().c_str());
		}
	} else {
		theApp.getLogger().Log(klib::base::LogLevel_Error, "Laden der Konten fehlgeschlagen!");
	}

	theApp.getLogger().Log(klib::base::LogLevel_Info, "Applikation gestartet...");
	return TRUE;
}

BEGIN_MESSAGE_MAP(kCXTPTaskPanel, CXTPTaskPanel)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED( 100, OnFilter )
END_MESSAGE_MAP()

afx_msg void kCXTPTaskPanel::OnFilter() {
	//MessageBox("XXXX");
	CMainFrame *pw = (CMainFrame *)theApp.m_pMainWnd;
	CString ftext;
	pw->m_wndFilter->GetWindowText(ftext);
	std::string s = ftext;
	refreshPositionen(s.empty() ? 0 : &s);
}

const std::string &stdDateToStr(klib::dbi::DateTime &iDate) {
	static std::string s;
	s = (boost::format("%|3$02d|.%|2$02d|.%|1$04d|") % iDate.Year % iDate.Month % iDate.Day).str();
	return s;
}

/*
virtual int CompareGroupCaption(
    CXTPReportColumn* pColumn, 
    CXTPReportRecordItem* pItem
);
*/
static void _normalize_memo(char *iMemo, std::string &oMemo) {
	oMemo = iMemo;
#if 0
	boost::algorithm::replace_all(oMemo, "\t", " ");
	while (strstr(oMemo.c_str(),"  ")) {
		boost::algorithm::replace_all(oMemo, "  ", " ");
	}
#else
	char *p2,*p = (char *)oMemo.c_str();
	while (*p) {
		for (; *p && *p!=' '; p++);
		if (*p) {
			for (p2 = ++p; *p2 && *p2==' '; p2++);
			if (*p2 && p2 != p) {
				memmove(p, p2, strlen(p2) + 1);
				p++;
			} else p = p2;
		}
	}
#endif
}

void kCXTPTaskPanel::refreshPositionen(std::string *iFilter) {
	long poscnt = 0;
	std::string Memo;
	DbiGen_M::D_Positionen *p = 0;
	CMainFrame *pw = (CMainFrame *)theApp.m_pMainWnd;
//	theApp.getLogger().Log(klib::base::LogLevel_Info, (boost::format("%1% Auszugspositionen geladen...")%poscnt).str());
	pw->m_wndPositionen.BeginUpdate();
	pw->m_wndPositionen.GetRecords()->RemoveAll();
	while ((p = (DbiGen_M::D_Positionen *)theApp.getPositionsliste()->IterateNext(p))) {
		if (iFilter) {
			if (!boost::algorithm::ifind_first(p->Memo, iFilter->c_str())) {
				continue;
			}
		}
		COLORREF bgcol;
		if (false && poscnt%2) {
			bgcol = RGB(220,220,250);
		} else {
			bgcol = RGB(240,240,240);
		}
		poscnt++;
		CXTPReportRecord *pRecord;
		CXTPReportRecordItem *pItem;
		pRecord = new CXTPReportRecord;

		pItem = new CXTPReportRecordItem;
		pItem->SetCaption(stdDateToStr(p->PostedDt).c_str());
		pItem->SetBackgroundColor(bgcol);
		pRecord->AddItem(pItem);

		pItem = new CXTPReportRecordItem;
		pItem->SetCaption(stdDateToStr(p->EffDt).c_str());
		pItem->SetBackgroundColor(bgcol);
		pRecord->AddItem(pItem);

		pItem = new CXTPReportRecordItemNumber(p->Amt);
//		pItem->SetCaption((boost::format("%.2f")%p->Amt).str().c_str());
		pItem->SetBackgroundColor(bgcol);
		if (p->Amt < 0.0) {
			pItem->SetTextColor(RGB(255,0,0));
		}
		pRecord->AddItem(pItem);

		pItem = new CXTPReportRecordItem;
		pItem->SetCaption(p->CurCode);
		pItem->SetBackgroundColor(bgcol);
		pRecord->AddItem(pItem);

		pItem = new CXTPReportRecordItem;
		_normalize_memo(p->Memo, Memo);
		pItem->SetCaption(Memo.c_str());
		pItem->SetBackgroundColor(bgcol);
		pRecord->AddItem(pItem);

		pw->m_wndPositionen.AddRecord(pRecord);
	}
	pw->m_wndPositionen.Populate();
	pw->m_wndPositionen.EndUpdate();
}

void kCXTPTaskPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	static CXTPTaskPanelGroupItem *pLastItem = 0;
	//CXTPTaskPanelItem *pItem = this->HitTestItem(point);
	CXTPTaskPanelGroupItem *pItem = (CXTPTaskPanelGroupItem *)this->HitTestItem(point);
	if (pItem && pItem->GetID() == IDS_TASKITEM_LOGFILE) {
		((klib::base::CLogWriter_file *)(theApp.getDbEnv()->getLogHandler()->getLogWriter("logfilewriter")))->flush();
		//MessageBox("not implemented!");
		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		memset(&pi, 0, sizeof(pi));
		memset(&si, 0, sizeof(si));
		si.cb = sizeof(si);
		CreateProcess(NULL,"notepad.exe guilog.log", NULL, NULL, false,0 , NULL, NULL,&si,&pi);
	} else if (pItem && pItem->GetID() == IDS_TASKITEM_KONTONUMMER) {
		long KontoIndex = pItem->GetIndex();
		KontoIndex = ((DbiGen_M::D_Konten *)theApp.getKontenliste()->GetAt(KontoIndex))->Id;
		//CMainFrame *pw = (CMainFrame *)theApp.m_pMainWnd;
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		//MessageBox(x->GetCaption());
		long poscnt = 0;
		if (theApp.ladePositionen(KontoIndex, poscnt)) {
			theApp.getLogger().Log(klib::base::LogLevel_Info, (boost::format("%1% Auszugspositionen geladen...")%poscnt).str());
			refreshPositionen(NULL);
			pItem->SetBold();
			if (pLastItem) pLastItem->SetBold(false);
			pLastItem = pItem;
		} else {
			theApp.getLogger().Log(klib::base::LogLevel_Error, "Laden der Auszugspositionen fehlgeschlagen!");
		}
	}
	CXTPTaskPanel::OnLButtonUp(nFlags, point);
}
