#pragma once


// Dialog_Prompt 대화 상자

class Dialog_Prompt : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog_Prompt)

public:
	Dialog_Prompt(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Dialog_Prompt();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PROMPT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	void SetPromptText(CString szText) { m_szPrompt = szText; };
	CString GetInputString();
	int GetInputInteger();

private:
	CString m_szPrompt;
	CString m_szAnswer;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
