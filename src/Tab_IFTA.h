#pragma once
#include "afxwin.h"


// CTab_DM dialog
class ophIFTA;

class CTab_IFTA : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_IFTA)

public:
	CTab_IFTA(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTab_IFTA();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_IFTA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedReadConfig_Ifta();
	afx_msg void OnBnClickedLoadRGBImg();
	afx_msg void OnBnClickedLoadDepthImg();
	afx_msg void OnBnClickedViewIfta();
	afx_msg LRESULT OnMsg(WPARAM wParam, LPARAM lParam);
	void SaveIMG();
	void SaveOHC();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	bool CheckConfig();
#ifdef TEST_MODE
	BOOL AutoTest();
#endif
	void InitUI();
	void GetEncodeName(CString &szEncode);
	void MakeFileName(CString szAppend = L"");
	ophIFTA* GetInstance() { return m_pIFTA; }
	CString m_szFileName;
	CString m_szEncodeName;
	ophIFTA*		m_pIFTA;
	CString			m_szPath;
	CString			m_szDname;
	CString			m_szRGBname;

	TCHAR			m_argParamDimg[MAX_PATH];
	TCHAR			m_argParamRGBimg[MAX_PATH];
	TCHAR			m_resultPath[MAX_PATH];

	bool	m_bConfig;
	bool	m_bRGBimg;
	bool	m_bDimg;
	bool	m_bEncode;

	int		m_nDepth;
	int		m_nIteration;
	double	m_nearDepth;
	double	m_farDepth;

#ifdef TEST_MODE
	BOOL m_bTest;
#endif
};
