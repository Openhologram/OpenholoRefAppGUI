#pragma once


// Dialog_BMP_Viewer dialog

#include <vector>
#include "afxwin.h"

enum INIT_FLAG {
	INIT_DIR,
	INIT_ARR,
	INIT_SINGLE
};

class Dialog_BMP_Viewer : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog_BMP_Viewer)

public:
	Dialog_BMP_Viewer(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dialog_BMP_Viewer();

	void Init(CString pDirectoryPath, INIT_FLAG flag);
	void Init(std::vector<CString>& rPathArr, INIT_FLAG flag);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_BMP_VIEWER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()



	std::vector<CImage*>	m_image;
	std::vector<CString>	m_imageName;
	UINT					m_curImageIdx;
	int						m_prevInterval;
	int						m_nextInterval;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedPrev();
	afx_msg void OnBnClickedNext();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeComboImage();
};
