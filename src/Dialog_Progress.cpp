// Dialog_Progress.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppUI.h"
#include "Dialog_Progress.h"
#include "afxdialogex.h"


// Dialog_Progress dialog

IMPLEMENT_DYNAMIC(Dialog_Progress, CDialogEx)

Dialog_Progress::Dialog_Progress(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PROGRESS, pParent)
	, m_bFinished(FALSE)
	, m_iProgress(0)
{

}

Dialog_Progress::~Dialog_Progress()
{
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

	while (!dlg->m_bFinished) {
		dlg->m_iProgress++;
		if (dlg->m_iProgress == 1) { dlg->SetDlgItemText(IDC_TEXT_GEN, CString(L"\t\tHOLOGRAM GENERATING")); Sleep(500); }
		else if (dlg->m_iProgress == 2) { dlg->SetDlgItemText(IDC_TEXT_GEN, CString(L"\t\tHOLOGRAM GENERATING.")); Sleep(500); }
		else if (dlg->m_iProgress == 3) { dlg->SetDlgItemText(IDC_TEXT_GEN, CString(L"\t\tHOLOGRAM GENERATING..")); Sleep(500); }
		else if (dlg->m_iProgress == 4) { dlg->SetDlgItemText(IDC_TEXT_GEN, CString(L"\t\tHOLOGRAM GENERATING...")); Sleep(500); }
		else if (dlg->m_iProgress >= 5) { dlg->SetDlgItemText(IDC_TEXT_GEN, CString(L"\t\tHOLOGRAM GENERATING....")); dlg->m_iProgress = 0; Sleep(500); }
	}
	dlg->UpdateData(FALSE);
	::SendMessage(dlg->GetSafeHwnd(), WM_CLOSE, NULL, NULL);

	return 1;
}

BOOL Dialog_Progress::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
