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
	afx_msg void OnBnClickedGenerateWrp();
	afx_msg void OnBnClickedViewWrpBmp();
	afx_msg void OnBnClickedSaveBmpWrp();
	afx_msg void OnBnClickedSaveOhcWrp();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeEncodeMethodWrp();
	afx_msg void OnBnClickedEncodingWrp();
#ifdef TEST_MODE
	BOOL AutoTest();
#endif
	void GetEncodeName(CString &szEncode);
	void MakeFileName(CString szAppend = L"");
	CString m_szFileName;
	CString m_szEncodeName;
	ophWRP	*m_pWRP;
	bool	m_bConfig;
	bool	m_bPC;
	bool	m_bEncode;

	int		m_idxEncode;

	TCHAR	m_argParam[MAX_PATH];
	TCHAR	m_resultPath[MAX_PATH];

	double m_fieldLens;
	double m_scaleX;
	double m_scaleY;
	double m_scaleZ;
	double m_numofWRP;
	double m_locationWRP;
	double m_distance;
	double m_pixelpitchX;
	double m_pixelpitchY;
	double m_pixelnumX;
	double m_pixelnumY;
	double m_wavelength;

	CButton m_buttonGPU;
	CButton m_buttonViewingWindow;
#ifdef TEST_MODE
	BOOL m_bTest;
#endif
};
