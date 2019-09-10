// Dialog_Prompt.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppUI.h"
#include "Dialog_Prompt.h"
#include "afxdialogex.h"


// Dialog_Prompt dialog

IMPLEMENT_DYNAMIC(Dialog_Prompt, CDialogEx)

Dialog_Prompt::Dialog_Prompt(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PROMPT, pParent)
{

}

Dialog_Prompt::~Dialog_Prompt()
{
}

void Dialog_Prompt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}


BEGIN_MESSAGE_MAP(Dialog_Prompt, CDialogEx)
END_MESSAGE_MAP()


// Dialog_Prompt message handlers


BOOL Dialog_Prompt::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL Dialog_Prompt::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::DestroyWindow();
}
