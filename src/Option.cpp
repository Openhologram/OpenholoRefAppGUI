// Option.cpp: 구현 파일
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "Option.h"
#include "afxdialogex.h"


// Option 대화 상자

IMPLEMENT_DYNAMIC(Option, CDialogEx)

Option::Option(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_OPTION, pParent)
	, m_rotate(0)
	, m_flip(0)
{

}

Option::~Option()
{
}

void Option::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROTATE, m_rotate);
	DDX_Control(pDX, IDC_RADIO_FLIP_NONE, m_radioFlipNone);
	DDX_Control(pDX, IDC_RADIO_FLIP_VERT, m_radioFlipVert);
	DDX_Control(pDX, IDC_RADIO_FLIP_HORI, m_radioFlipHori);
}


BEGIN_MESSAGE_MAP(Option, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &Option::OnBnClickedOk)
END_MESSAGE_MAP()


// Option 메시지 처리기


void Option::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ShowWindow(SW_HIDE);
}


void Option::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();	
	UpdateData(TRUE);

	if (m_radioFlipNone) m_flip = 0;
	else if (m_radioFlipVert) m_flip = 1;
	else if (m_radioFlipHori) m_flip = 2;

	::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), FLIP_APPLY, 0, 0);
	::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), ROTATE_APPLY, 0, 0);
}


BOOL Option::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_radioFlipNone.SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
