#pragma once


// CTab_MESH_R 대화 상자
class ophRec;

class CTab_RECON : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_RECON)

public:
	CTab_RECON(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTab_RECON();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_RECON };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void ReloadContents(int opt);
	afx_msg void OnBnClickedBtnLoadPhase();
	afx_msg void OnBnClickedBtnLoadAmplitude();
	afx_msg void OnBnClickedBtnLoadFringe();
	afx_msg void OnBnClickedBtnLoadReal();
	afx_msg void OnBnClickedBtnLoadImaginary();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedReadConfigRec();
	afx_msg LRESULT OnReconstruct(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveIMG(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioPosition();
	afx_msg void OnBnClickedRadioFocus();

private:
	bool m_bReal;
	bool m_bImag;
	bool m_bPhase;
	bool m_bAmpli;
	CString m_szReal;
	CString m_szImag;
	CString m_szPhase;
	CString m_szAmpli;

public:
	ophRec *m_rec;
	TCHAR	m_argParam[MAX_PATH];
	TCHAR	m_resultPath[MAX_PATH];
	double m_from;
	double m_to;
	int m_step;
};
