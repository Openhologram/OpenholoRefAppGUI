#pragma once
#define FLIP_APPLY WM_USER+10
#define ROTATE_APPLY WM_USER+11


// Option 대화 상자

class Option : public CDialogEx
{
	DECLARE_DYNAMIC(Option)

public:
	Option(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Option();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_OPTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	// rotate value
	double m_rotate;
	int m_flip;
	CButton m_radioFlipNone;
	CButton m_radioFlipVert;
	CButton m_radioFlipHori;
	virtual BOOL OnInitDialog();
	double GetRotate() { return m_rotate; }
	int GetFlip() { return m_flip; }
};
