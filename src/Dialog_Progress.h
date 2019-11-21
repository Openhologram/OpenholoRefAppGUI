#pragma once
#include "afxcmn.h"


#include <functional>
// Dialog_Progress dialog

class Dialog_Progress : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog_Progress)

public:

	Dialog_Progress(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dialog_Progress();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROGRESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	//CProgressCtrl m_progress;
	HANDLE m_thread;
	BOOL	m_bFinished;
	UINT	*m_iPercent;
	UINT	m_iProgress;
	bool	m_bPercent;
	CFont m_font;

	virtual BOOL DestroyWindow();
};

struct parammeter {
	void* pGEN;
	UINT flag;
	Dialog_Progress* pDialog;
};