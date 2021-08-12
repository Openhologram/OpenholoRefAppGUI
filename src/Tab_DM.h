#pragma once
#include "afxwin.h"


// CTab_DM dialog
class ophDepthMap;

class CTab_DM : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_DM)

public:
	CTab_DM(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTab_DM();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedReadConfig_DM();
	afx_msg void OnBnClickedLoadDImg();
	afx_msg void OnBnClickedLoadRgbImg();
	afx_msg void OnBnClickedViewDm();
	afx_msg void OnBnClickedViewDmBmp();
	afx_msg void OnBnClickedViewDmImg();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangePropagationMethodDm();
	afx_msg LRESULT OnGenerate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEncode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveIMG(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveOHC(WPARAM wParam, LPARAM lParam);
#ifdef TEST_MODE
	BOOL AutoTest();
#endif
	void InitUI();
	void GetEncodeName(CString &szEncode);
	void MakeFileName(CString szAppend = L"");
	bool CheckConfig();
	ophDepthMap* GetInstance() { return m_pDepthMap; }
	
	void Generate();

	CString m_szFileName;
	CString m_szEncodeName;
	ophDepthMap*	m_pDepthMap;
	CString			m_szPath;
	CString			m_szDname;
	CString			m_szRGBname;

	TCHAR			m_argParamDimg[MAX_PATH];
	TCHAR			m_argParamRGBimg[MAX_PATH];
	TCHAR			m_resultPath[MAX_PATH];

	bool	m_bConfig;
	bool	m_bDimg;
	bool	m_bRGBimg;
	bool	m_bEncode;

	double	m_nearDepth;
	double	m_farDepth;
	int		m_numDepth;

	int		m_idxPropagation;
#ifdef TEST_MODE
	BOOL m_bTest;
#endif
};
