// Dialog_Progress.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "Dialog_Progress.h"
#include "afxdialogex.h"


// Dialog_Progress dialog

IMPLEMENT_DYNAMIC(Dialog_Progress, CDialogEx)

Dialog_Progress::Dialog_Progress(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PROGRESS, pParent)
	, m_bFinished(FALSE)
	, m_iProgress(0)
	, m_iPercent(nullptr)
	, m_bPercent(false)
{
}

Dialog_Progress::~Dialog_Progress()
{
	m_iPercent = nullptr;
	m_font.DeleteObject();
}

void Dialog_Progress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}


BEGIN_MESSAGE_MAP(Dialog_Progress, CDialogEx)
END_MESSAGE_MAP()


// Dialog_Progress message handlers

int progressPos(int a, int b) {
	return (int)((double)(a * 100) / (double)b);
}

UINT CallFunction(LPVOID lParam) {
	Dialog_Progress *dlg = (Dialog_Progress*)lParam;

	if (!dlg) return 0;
	CString szMsg = L"GENERATING";
	while (!dlg->m_bFinished) {
		if (dlg->m_bPercent) {
			szMsg.Format(L"%d (%%)", *dlg->m_iPercent);
			dlg->SetDlgItemTextW(IDC_TEXT_GEN, szMsg);
			Sleep(100);
		}
		else {
			dlg->m_iProgress++;
			if (dlg->m_iProgress <= 5) {
				szMsg.AppendChar('.');
			}
			else {
				szMsg = L"GENERATING";
				dlg->m_iProgress = 0;
			}
			dlg->SetDlgItemTextW(IDC_TEXT_GEN, szMsg);
			Sleep(500);
		}
	}

	::SendMessage(dlg->GetSafeHwnd(), WM_CLOSE, NULL, NULL);

	return 1;
}

BOOL Dialog_Progress::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_font.CreatePointFont(180, L"±¼¸²");
	GetDlgItem(IDC_TEXT_GEN)->SetFont(&m_font, TRUE);


	CWinThread *pThread = nullptr;
	pThread = AfxBeginThread(CallFunction, this);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL Dialog_Progress::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::DestroyWindow();
}
