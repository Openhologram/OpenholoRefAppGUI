#pragma once


// CTab_MESH dialog
class ophTri;

class CTab_MESH : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_MESH)

public:
	CTab_MESH(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTab_MESH();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MESH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedReadConfigMesh();
	afx_msg void OnBnClickedLoadMesh();
	afx_msg void OnBnClickedViewMesh();
	afx_msg void OnBnClickedGenerateMesh();
	afx_msg void OnBnClickedEncodingMesh();
	afx_msg void OnBnClickedSaveBmpMesh();
	afx_msg void OnBnClickedViewMeshBmp();
	afx_msg void OnBnClickedSaveOhcMesh();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeEncodeMethodMesh();
#ifdef TEST_MODE
	BOOL AutoTest();
#endif
	CString GetEncodeName();

	bool m_bConfig;
	bool m_bMeshData;
	bool m_bEncode;

	int m_idxEncode;

	ophTri		*m_pMesh;

	TCHAR		m_argParam[MAX_PATH];
	TCHAR		m_resultPath[MAX_PATH];
	
	double m_fieldLens;
	double m_objectsize;
	double m_objectshiftX;
	double m_objectshiftY;
	double m_objectshiftZ;
	double m_lampdirX;
	double m_lampdirY;
	double m_lampdirZ;
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
