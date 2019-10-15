
// OpenholoRefAppGUIDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Time.h"
class CTab_PC;
class CTab_DM;
class CTab_LF;
class CTab_MESH;
class CTab_WRP;


// COpenholoRefAppDlg dialog
class COpenholoRefAppDlg : public CDialogEx
{
// Construction
public:
	COpenholoRefAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENHOLOREFAPPGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTcnSelchangeGenTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedLogCheck();
	DECLARE_MESSAGE_MAP()

	void initTabs(void);

public:

	char* GetDirectoryPath(LPCTSTR szfilter, CWnd *pParentWnd);
	BOOL IsGeforceGPU();
	void report(char *szMsg);

	CImage		m_imgOPH_LOGO, m_imgKETI_LOGO;
	CRect		m_rcOPH;
	BOOL		m_bClickOPH;

	CStatic m_picOphLogo;
	CStatic m_picKetiLogo;

	CTabCtrl	m_Tab;
	IStream		*pStreamOph;
	IStream		*pStreamKeti;

	CTab_PC		*pTabPC;
	CTab_DM		*pTabDM;
	CTab_LF		*pTabLF;
	CTab_MESH	*pTabMESH;
	CTab_WRP	*pTabWRP;
	CButton		m_buttonLog;
};
