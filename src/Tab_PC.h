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
	afx_msg void OnBnClickedGenerate_PC();
	afx_msg void OnBnClickedEncodingPc();
	afx_msg void OnBnClickedSaveBmp_PC();
	afx_msg void OnBnClickedSaveOhc_PC();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeDiffMethodPc();
	afx_msg void OnCbnSelchangeEncodeMethodPc();
#ifdef TEST_MODE
	BOOL AutoTest();
#endif
	ophPointCloud *m_pPointCloud;

	bool	m_bConfig;
	bool	m_bPC;
	bool	m_bEncode;
	char	m_argParam[MAX_PATH];
	TCHAR	m_argParamW[MAX_PATH];
	TCHAR	m_resultPath[MAX_PATH];
	BOOL	m_bFinish;

	double			m_fieldLens;
	double			m_scaleX;
	double			m_scaleY;
	double			m_scaleZ;
	double			m_offsetdepth;
	double			m_pixelpitchX;
	double			m_pixelpitchY;
	unsigned int	m_pixelnumX;
	unsigned int	m_pixelnumY;
	double			m_wavelength;

	int		m_idxDiff;
	int		m_idxEncode;

	CButton m_buttonGenerate;
	CButton m_buttonSaveBmp;
	CButton m_buttonSaveOhc;
	CButton m_buttonGPU;
	CButton m_buttonViewingWindow;
	BOOL m_bTest;
};
