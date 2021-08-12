#pragma once
#include "afxwin.h"


// CTab_LF dialog
class ophLF;

class CTab_LF : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_LF)

public:
	CTab_LF(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTab_LF();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LF };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedReadConfig_LF();
	afx_msg void OnBnClickedFindDir();
	afx_msg void OnBnClickedViewLf();
	afx_msg void OnBnClickedViewLfBmp();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnGenerate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEncode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveIMG(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveOHC(WPARAM wParam, LPARAM lParam);
#ifdef TEST_MODE
	BOOL AutoTest();
#endif
	void GetEncodeName(CString &szEncode);
	void MakeFileName(CString szAppend = L"");
	ophLF* GetInstance() { return m_pLightField; }
	CString m_szFileName;
	CString m_szEncodeName;
	ophLF			*m_pLightField;
	bool			m_bConfig;
	bool			m_bDir;
	bool			m_bEncode;
	TCHAR			m_argParam[MAX_PATH];
	TCHAR			m_resultPath[MAX_PATH];

	double	m_distance;
	int		m_numimgX;
	int		m_numimgY;

#ifdef TEST_MODE
		BOOL m_bTest;
#endif
};
