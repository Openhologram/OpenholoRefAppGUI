// Dialog_Prompt.cpp: 구현 파일
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "Dialog_Prompt.h"
#include "afxdialogex.h"


// Dialog_Prompt 대화 상자

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


// Dialog_Prompt 메시지 처리기


BOOL Dialog_Prompt::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_szPrompt = L"반복 할 테스트 수를 입력하시오.";
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

CString Dialog_Prompt::GetInputString()
{
	return m_szAnswer;
}

int Dialog_Prompt::GetInputInteger()
{
	return _ttoi(m_szAnswer);
}
