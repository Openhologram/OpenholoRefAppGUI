#pragma once


// Dialog_Prompt ��ȭ ����

class Dialog_Prompt : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog_Prompt)

public:
	Dialog_Prompt(CWnd* pParent = nullptr);   // ǥ�� �������Դϴ�.
	virtual ~Dialog_Prompt();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PROMPT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
