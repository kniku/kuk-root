// sTimerDlg.h : Headerdatei
//

#pragma once


// CsTimerDlg Dialogfeld
class CsTimerDlg : public CDialog
{
// Konstruktion
public:
	CsTimerDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_STIMER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterst�tzung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen f�r die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
