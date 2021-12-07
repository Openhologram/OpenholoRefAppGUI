#pragma once
#include "afxwin.h"

// CTab_PC dialog
class ophPointCloud;
class Dialog_Progress;

class CTab_PC : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_PC)

public:
	CTab_PC(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTab_PC();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedReadConfig_PC();
	afx_msg void OnBnClickedLoadPc();
	afx_msg void OnBnClickedViewPc();
	afx_msg void OnBnClickedViewPcBmp();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeDiffMethodPc();
	afx_msg LRESULT OnGenerate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEncode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveIMG(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveOHC(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoadOHC(WPARAM wParam, LPARAM lParam);
	/*
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	*/
	bool CheckConfig();

#ifdef TEST_MODE
	BOOL AutoTest();
#endif
	void GetEncodeName(CString &szEncode);
	void MakeFileName(CString szAppend = L"");
	ophPointCloud* GetInstance() { return m_pPointCloud; }
	CString m_szFileName;
	CString m_szEncodeName;
	ophPointCloud *m_pPointCloud;

	bool	m_bConfig;
	bool	m_bPC;
	bool	m_bEncode;
	char	m_argParam[MAX_PATH];
	TCHAR	m_argParamW[MAX_PATH];
	TCHAR	m_resultPath[MAX_PATH];
	BOOL	m_bFinish;

	double			m_scaleX;
	double			m_scaleY;
	double			m_scaleZ;
	double			m_distance;

	int		m_idxDiff;

	BOOL m_bTest;
	/*
	CFont m_font;
	CMFCButton m_btnLoadPC;
	CMFCButton m_btnLoadCfg;
	CMFCButton m_btnViewer;
	*/
};
