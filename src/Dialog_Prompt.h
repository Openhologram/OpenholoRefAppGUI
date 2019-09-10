#pragma once
#include "afxcmn.h"

// Dialog_Prompt dialog

class Dialog_Prompt : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog_Progress)

public:

	Dialog_Prompt(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dialog_Prompt();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROMPT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
};