// Dialog_Prompt.cpp: ���� ����
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "Dialog_Prompt.h"
#include "afxdialogex.h"


// Dialog_Prompt ��ȭ ����

IMPLEMENT_DYNAMIC(Dialog_Prompt, CDialogEx)

Dialog_Prompt::Dialog_Prompt(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_PROMPT, pParent)
{

}

Dialog_Prompt::~Dialog_Prompt()
{
}

void Dialog_Prompt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC, m_szPrompt);
	DDX_Text(pDX, IDC_EDIT_ANSWER, m_szAnswer);
}


BEGIN_MESSAGE_MAP(Dialog_Prompt, CDialogEx)
END_MESSAGE_MAP()


// Dialog_Prompt �޽��� ó����


BOOL Dialog_Prompt::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_szPrompt = L"�ݺ� �� �׽�Ʈ ���� �Է��Ͻÿ�.";
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

CString Dialog_Prompt::GetInputString()
{
	return m_szAnswer;
}

int Dialog_Prompt::GetInputInteger()
{
	return _ttoi(m_szAnswer);
}
