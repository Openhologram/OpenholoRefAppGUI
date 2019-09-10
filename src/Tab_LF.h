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
	afx_msg void OnBnClickedGenerate_LF();
	afx_msg void OnBnClickedEncodingLf();
	afx_msg void OnBnClickedSaveBmp_LF();
	afx_msg void OnBnClickedViewLfBmp();
	afx_msg void OnBnClickedSaveOhc_LF();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeEncodeMethodLf();

	ophLF			*m_pLightField;
	bool			m_bConfig;
	bool			m_bDir;
	bool			m_bEncode;
	TCHAR			m_argParam[MAX_PATH];
	TCHAR			m_resultPath[MAX_PATH];

	int				m_idxEncode;

	double	m_distance;
	int		m_numimgX;
	int		m_numimgY;
	double	m_pixelpitchX;
	double	m_pixelpitchY;
	int		m_pixelnumX;
	int		m_pixelnumY;
	double	m_wavelength;

	CButton m_buttonGenerate;
	CButton m_buttonSaveBmp;
	CButton m_buttonSaveOhc;
};
