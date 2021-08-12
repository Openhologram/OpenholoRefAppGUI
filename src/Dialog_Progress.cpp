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
	, m_bGen(true)
{
}

Dialog_Progress::~Dialog_Progress()
{
	m_iPercent = nullptr;
	m_font.DeleteObject();
	m_font2.DeleteObject();
}

void Dialog_Progress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
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
	CString szMsg = dlg->m_bGen ? L"GENERATING" : L"REBUILDING";
	CString szForm;
	DWORD dwBegin = GetTickCount64();
	DWORD dwElapsedTime;
	dlg->m_progress.SetRange(0, 100);

	CString szHour = L"", szMin = L"", szSec = L"";

	while (!dlg->m_bFinished) {
		dwElapsedTime = GetTickCount64() - dwBegin;
		long dSec = dwElapsedTime / 1000;
		if (dlg->m_bPercent) {
			szMsg.Format(L"%d (%%)", *dlg->m_iPercent);

			if ((dSec / 3600) > 0)
				szHour.Format(L"%02d:", dSec / 3600);
			if ((dSec / 60) > 0)
				szMin.Format(L"%02d:", (dSec % 3600) / 60);
			szSec.Format(L"%02d", dSec % 60);
			
			szForm.Format(L"Elapsed time: %s%s%s", szHour, szMin, szSec);

			dlg->SetDlgItemTextW(IDC_TEXT_GEN, szMsg);
			dlg->SetDlgItemTextW(IDC_TEXT_TIME, szForm);
			dlg->m_progress.SetPos(*dlg->m_iPercent);
			Sleep(100);
		}
		else {
			dlg->m_iProgress++;
			if (dlg->m_iProgress <= 5) {
				szMsg.AppendChar('.');
			}
			else {
				szMsg = dlg->m_bGen ? L"GENERATING" : L"REBUILDING";
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
	m_font.CreatePointFont(180, L"Gulim");
	m_font2.CreatePointFont(100, L"Gulim");
	GetDlgItem(IDC_TEXT_GEN)->SetFont(&m_font, TRUE);
	GetDlgItem(IDC_TEXT_TIME)->SetFont(&m_font2, TRUE);


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
