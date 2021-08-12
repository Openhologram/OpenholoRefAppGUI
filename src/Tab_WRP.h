#pragma once


// CTab_WRP dialog
class ophWRP;

class CTab_WRP : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_WRP)

public:
	CTab_WRP(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTab_WRP();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_WRP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedReadConfigWrp();
	afx_msg void OnBnClickedLoadPcWrp();
	afx_msg void OnBnClickedViewWrp();
	afx_msg void OnBnClickedViewWrpBmp();
	afx_msg LRESULT OnGenerate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEncode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveIMG(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveOHC(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
#ifdef TEST_MODE
	BOOL AutoTest();
#endif
	void GetEncodeName(CString &szEncode);
	void MakeFileName(CString szAppend = L"");
	ophWRP* GetInstance() { return m_pWRP; }
	CString m_szFileName;
	CString m_szEncodeName;
	ophWRP	*m_pWRP;
	bool	m_bConfig;
	bool	m_bPC;
	bool	m_bEncode;


	TCHAR	m_argParam[MAX_PATH];
	TCHAR	m_resultPath[MAX_PATH];

	double m_scaleX;
	double m_scaleY;
	double m_scaleZ;
	double m_numofWRP;
	double m_locationWRP;
	double m_distance;

#ifdef TEST_MODE
	BOOL m_bTest;
#endif
};
