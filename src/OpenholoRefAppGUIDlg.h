
// OpenholoRefAppGUIDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Time.h"
#include "Option.h"
#include <vector>
class CTab_PC;
class CTab_DM;
class CTab_LF;
class CTab_MESH;
class CTab_WRP;
class CTab_IFTA;
class CTab_RECON;

#define GENERATE	WM_USER + 1
#define ENCODE		WM_USER + 2
#define SAVE_IMG	WM_USER + 3
#define SAVE_OHC	WM_USER + 4
#define LOAD_CFG	WM_USER + 5
#define LOAD_DATA	WM_USER + 6
#define RECONSTRUCT	WM_USER + 7

// COpenholoRefAppDlg dialog
class COpenholoRefAppDlg : public CDialogEx
{
// Construction
public:
	COpenholoRefAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENHOLOREFAPPGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTcnSelchangeGenTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedLogCheck();
	afx_msg void OnBnClickedAlwaysCheck();
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnBnClickedGenerate();
	afx_msg void OnBnClickedReconstruct();
	afx_msg void OnBnClickedSaveImg();
	afx_msg void OnBnClickedEncoding();
	afx_msg void OnBnClickedSaveOhc();
	afx_msg LRESULT OnFlip(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRotate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonExpand();
	afx_msg void OnCbnSelchangeEncodeMethod();
	DECLARE_MESSAGE_MAP()


public:
	void initTabs(void);
	void OpenExplorer(CString szPath);
	void ForegroundConsole();
	char* GetDirectoryPath(LPCTSTR szfilter, CWnd *pParentWnd);
	BOOL IsGeforceGPU();
	void report(char *szMsg);
	CString GetFileName();
	CString GetFileNameExt();
	void initUI();
	bool SaveImage(CString &path);
	bool SaveOHC(CString &path);
	double GetShiftX() { return m_shiftX; }
	double GetShiftY() { return m_shiftY; }
	void MoveOptionDlg();
	double GetRotate() { return m_rotate; }
	int GetFlip() { return m_flip; }
	bool UseGPGPU() { return m_buttonGPU.GetCheck(); }
	bool UseVW() { return m_buttonViewingWindow.GetCheck(); }
	void ReloadContents();


	CImage		m_imgOPH_LOGO, m_imgKETI_LOGO;
	CRect		m_rcOPH, m_rcKETI;
	BOOL		m_bClickOPH, m_bClickKETI;
	CStatic		m_szImgPath;

	CStatic m_picOphLogo;
	CStatic m_picKetiLogo;

	CTabCtrl	m_Tab;
	CComboBox	m_Algo;
	CComboBox	m_AlgoRecon;
	IStream		*pStreamOph;
	IStream		*pStreamKeti;

	int			m_iEncode;
	int			m_iPassband;

	double		m_rotate;
	int			m_flip;

	CTab_PC		*pTabPC;
	CTab_DM		*pTabDM;
	CTab_LF		*pTabLF;
	CTab_MESH	*pTabMESH;
	CTab_WRP	*pTabWRP;
	CTab_IFTA	*pTabIFTA;
	CTab_RECON	*pTabRECON;
	Option		*m_option;

	CButton		m_buttonLog;
	CButton		m_buttonExplorer;
	CButton		m_buttonGenerate;
	CButton		m_buttonReconstruct;
	CButton		m_buttonEncode;
	CButton		m_buttonSaveBmp;
	CButton		m_buttonSaveOhc;
	CButton		m_buttonAlways;
	CButton		m_buttonViewImg;
	CComboBox	m_encodeMethod;
	CComboBox	m_encodePassband;
	CButton		m_buttonGPU;
	CButton		m_buttonViewingWindow;
	std::vector<CDialogEx *> m_vector;

	// 항상 위에 표시
	double			m_shiftX;
	double			m_shiftY;
	double			m_shiftZ;
	double			m_pixelpitchX;
	double			m_pixelpitchY;
	unsigned int	m_pixelnumX;
	unsigned int	m_pixelnumY;
	double			m_wavelength[3];
	int				m_nWave;
	void SetShift(double shiftX, double shiftY, double shiftZ) {
		m_shiftX = shiftX;
		m_shiftY = shiftY;
		m_shiftZ = shiftZ;
	}

	void SetPixelPitch(double pitchX, double pitchY) {
		m_pixelpitchX = pitchX;
		m_pixelpitchY = pitchY;
	}

	void SetPixelNum(unsigned int width, unsigned int height) {
		m_pixelnumX = width;
		m_pixelnumY = height;
	}

	void SetWaveNum(int nWave) {
		m_nWave = nWave > 3 ? 3 : nWave;
	}

	void SetWaveLength(double *wave) {
		for (int i = 0; i < m_nWave && i < 3; i++) {
			m_wavelength[i] = wave[i];
		}
	}
	afx_msg void OnCbnSelchangeComboAlgorithm();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeComboAlgorithmRecon();
	afx_msg void OnCbnSelchangeEncodePassband();
};
