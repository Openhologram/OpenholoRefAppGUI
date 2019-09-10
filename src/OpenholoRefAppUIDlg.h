
// OpenholoRefAppUIDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

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
	enum { IDD = IDD_OPENHOLOREFAPPUI_DIALOG };
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
	DECLARE_MESSAGE_MAP()

	void initTabs(void);

public:
	afx_msg void OnTcnSelchangeGenTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	char* GetDirectoryPath(LPCTSTR szfilter, CWnd *pParentWnd);
	afx_msg void OnBnClickedLogCheck();


	CImage		m_imgOPH_LOGO, m_imgKETI_LOGO;

	CStatic m_picOphLogo;

	CTabCtrl	m_Tab;


	CTab_PC		*pTabPC;
	CTab_DM		*pTabDM;
	CTab_LF		*pTabLF;
	CTab_MESH	*pTabMESH;
	CTab_WRP	*pTabWRP;
	CButton		m_buttonLog;
};
