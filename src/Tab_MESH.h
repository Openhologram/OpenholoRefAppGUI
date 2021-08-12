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
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnGenerate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEncode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveIMG(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveOHC(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedViewMeshBmp();
#ifdef TEST_MODE
	BOOL AutoTest();
#endif
	void GetEncodeName(CString &szEncode);
	void MakeFileName(CString szAppend = L"");
	ophTri* GetInstance() { return m_pMesh; }
	CString m_szFileName;
	CString m_szEncodeName;
	bool m_bConfig;
	bool m_bMeshData;
	bool m_bEncode;

	ophTri		*m_pMesh;

	TCHAR		m_argParam[MAX_PATH];
	TCHAR		m_resultPath[MAX_PATH];
	
	double m_scaleX;
	double m_scaleY;
	double m_scaleZ;
	double m_lampdirX;
	double m_lampdirY;
	double m_lampdirZ;
#ifdef TEST_MODE
	BOOL m_bTest;
#endif
};
