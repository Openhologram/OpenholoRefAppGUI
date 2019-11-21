
// OpenholoRefAppGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "OpenholoRefAppGUIDlg.h"
#include "afxdialogex.h"
#include <d3d9.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "Tab_PC.h"
#include "Tab_DM.h"
#include "Tab_LF.h"
#include "Tab_MESH.h"
#include "Tab_WRP.h"

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COpenholoRefAppDlg dialog



BOOL COpenholoRefAppDlg::IsGeforceGPU()
{
	D3DADAPTER_IDENTIFIER9 id;
	IDirect3D9 *iDirect3D = NULL;
	iDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (iDirect3D) {
		iDirect3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &id);
		CStringA szTmp(id.Description);
		iDirect3D->Release();

		if (szTmp.Find("NVIDIA") >= 0) {
			if(szTmp.Find("GeForce") >= 0)
				return true;
			
		}
		else
			return false;
	}
	return false;
}

COpenholoRefAppDlg::COpenholoRefAppDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPENHOLOREFAPPGUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bClickOPH = FALSE;
}

void COpenholoRefAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GEN_TAB, m_Tab);
	DDX_Control(pDX, IDC_OPH_LOGO, m_picOphLogo);
	DDX_Control(pDX, IDC_LOG_CHECK, m_buttonLog);
	DDX_Control(pDX, IDC_EXPLORER_CHECK, m_buttonExplorer);
	DDX_Control(pDX, IDC_KETI_LOGO, m_picKetiLogo);
	DDX_Control(pDX, IDC_ALWAYS_CHECK, m_buttonAlways);
}

BEGIN_MESSAGE_MAP(COpenholoRefAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_GEN_TAB, &COpenholoRefAppDlg::OnTcnSelchangeGenTab)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_LOG_CHECK, &COpenholoRefAppDlg::OnBnClickedLogCheck)
	ON_WM_CLOSE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_ALWAYS_CHECK, &COpenholoRefAppDlg::OnBnClickedAlwaysCheck)
END_MESSAGE_MAP()


// COpenholoRefAppDlg message handlers


BOOL COpenholoRefAppDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) return TRUE;
	//if (pMsg->message == WM_MOUSEHOVER && pMsg->hwnd == m_picOphLogo.m_hWnd) {
	//	AfxGetApp()->LoadCursorW(IDC_HAND);		
	//}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL COpenholoRefAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	SetWindowText(L"Openholo Reference Software-Generation");

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CheckDlgButton(IDC_LOG_CHECK, TRUE);
	CheckDlgButton(IDC_EXPLORER_CHECK, TRUE);

	
	m_imgOPH_LOGO.Load(_T("res/OpenHolo_logo.png"));
	m_imgKETI_LOGO.Load(_T("res/KETI_logo.png"));
	GetDlgItem(IDC_OPH_LOGO)->SetWindowPos(NULL, 0, 0, m_imgOPH_LOGO.GetWidth(), m_imgOPH_LOGO.GetHeight(), SWP_NOMOVE);
	GetDlgItem(IDC_KETI_LOGO)->SetWindowPos(NULL, 0, 0, m_imgKETI_LOGO.GetWidth(), m_imgKETI_LOGO.GetHeight(), SWP_NOMOVE);

	initTabs();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COpenholoRefAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COpenholoRefAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		CWnd *pWnd = (CWnd*)GetDlgItem(IDC_OPH_LOGO);
		CDC* dc = pWnd->GetDC();
		CStatic *staticSize = (CStatic *)GetDlgItem(IDC_OPH_LOGO);
		CRect rect;
		staticSize->GetClientRect(rect);
		m_imgOPH_LOGO.Destroy();

		//m_imgOPH_LOGO.LoadFromResource(AfxGetApp()->m_hInstance, IDB_OPENHOLO_LOGO);
		m_imgOPH_LOGO.Load(_T("res/OpenHolo_logo.png"));
		m_imgOPH_LOGO.Draw(dc->m_hDC, 0, 0, m_imgOPH_LOGO.GetWidth(), m_imgOPH_LOGO.GetHeight());


		pWnd = (CWnd*)GetDlgItem(IDC_KETI_LOGO);
		dc = pWnd->GetDC();
		staticSize = (CStatic *)GetDlgItem(IDC_KETI_LOGO);
		staticSize->GetClientRect(rect);
		m_imgKETI_LOGO.Destroy();
		m_imgKETI_LOGO.Load(_T("res/KETI_logo.png"));
		m_imgKETI_LOGO.Draw(dc->m_hDC, 0, 0, m_imgKETI_LOGO.GetWidth(), m_imgKETI_LOGO.GetHeight());
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COpenholoRefAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COpenholoRefAppDlg::initTabs(void)
{
	m_Tab.InsertItem(0, _T("Point Cloud"));
	m_Tab.InsertItem(1, _T("Depth Map"));
	m_Tab.InsertItem(2, _T("Light Field"));
	m_Tab.InsertItem(3, _T("Triangle Mesh"));
	m_Tab.InsertItem(4, _T("WRP"));

	m_Tab.SetCurSel(0);

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	pTabPC = new CTab_PC;
	pTabPC->Create(IDD_DLG_PC, &m_Tab);
	pTabPC->MoveWindow(0, 25, rect.Width(), rect.Height());
	pTabPC->ShowWindow(SW_SHOW);

	pTabDM = new CTab_DM;
	pTabDM->Create(IDD_DLG_DM, &m_Tab);
	pTabDM->MoveWindow(0, 25, rect.Width(), rect.Height());
	pTabDM->ShowWindow(SW_HIDE);

	pTabLF = new CTab_LF;
	pTabLF->Create(IDD_DLG_LF, &m_Tab);
	pTabLF->MoveWindow(0, 25, rect.Width(), rect.Height());
	pTabLF->ShowWindow(SW_HIDE);

	pTabMESH = new CTab_MESH;
	pTabMESH->Create(IDD_DLG_MESH, &m_Tab);
	pTabMESH->MoveWindow(0, 25, rect.Width(), rect.Height());
	pTabMESH->ShowWindow(SW_HIDE);

	pTabWRP = new CTab_WRP;
	pTabWRP->Create(IDD_DLG_WRP, &m_Tab);
	pTabWRP->MoveWindow(0, 25, rect.Width(), rect.Height());
	pTabWRP->ShowWindow(SW_HIDE);
}



void COpenholoRefAppDlg::OnTcnSelchangeGenTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		pTabPC->ShowWindow(SW_SHOW);
		pTabDM->ShowWindow(SW_HIDE);
		pTabLF->ShowWindow(SW_HIDE);
		pTabMESH->ShowWindow(SW_HIDE);
		pTabWRP->ShowWindow(SW_HIDE);
		break;
	case 1:
		pTabPC->ShowWindow(SW_HIDE);
		pTabDM->ShowWindow(SW_SHOW);
		pTabLF->ShowWindow(SW_HIDE);
		pTabMESH->ShowWindow(SW_HIDE);
		pTabWRP->ShowWindow(SW_HIDE);
		break;
	case 2:
		pTabPC->ShowWindow(SW_HIDE);
		pTabDM->ShowWindow(SW_HIDE);
		pTabLF->ShowWindow(SW_SHOW);
		pTabMESH->ShowWindow(SW_HIDE);
		pTabWRP->ShowWindow(SW_HIDE);
		break;
	case 3:
		pTabPC->ShowWindow(SW_HIDE);
		pTabDM->ShowWindow(SW_HIDE);
		pTabLF->ShowWindow(SW_HIDE);
		pTabMESH->ShowWindow(SW_SHOW);
		pTabWRP->ShowWindow(SW_HIDE);
		break;
	case 4:
		pTabPC->ShowWindow(SW_HIDE);
		pTabDM->ShowWindow(SW_HIDE);
		pTabLF->ShowWindow(SW_HIDE);
		pTabMESH->ShowWindow(SW_HIDE);
		pTabWRP->ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}


void COpenholoRefAppDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//SetWindowPos(NULL, 0, 0, 420, 480, SWP_NOMOVE);

	// TODO: Add your message handler code here
}


char * COpenholoRefAppDlg::GetDirectoryPath(LPCTSTR szfilter, CWnd *pParentWnd)
{
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);	

	CFileDialog FileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szfilter, pParentWnd);
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (!ext.CompareNoCase(L"xml")) path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName();
		else path = "XML file only.";
	}

	SetCurrentDirectory(current_path);

	TCHAR widepath[MAX_PATH] = { 0 };
	char* mulpath = new char[path.GetLength()];

	_tcscpy_s(widepath, path.GetBuffer());
	WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);

	return mulpath;
}


void COpenholoRefAppDlg::OnBnClickedLogCheck()
{
	// TODO: Add your control notification handler code here
	ShowWindowAsync(GetConsoleWindow(), m_buttonLog.GetCheck() ? SW_SHOW : SW_HIDE);
}

void COpenholoRefAppDlg::ForegroundConsole()
{
	HWND hConsole = GetConsoleWindow();
	if (hConsole) {
		if (::IsIconic(hConsole)) {
			::ShowWindow(hConsole, SW_RESTORE);
		}
		::SetForegroundWindow(hConsole);
	}
}

void COpenholoRefAppDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	pTabPC->DestroyWindow();
	pTabDM->DestroyWindow();
	pTabMESH->DestroyWindow();
	pTabWRP->DestroyWindow();
	pTabLF->DestroyWindow();
	delete pTabPC;
	delete pTabDM;
	delete pTabMESH;
	delete pTabWRP;
	delete pTabLF;

	CDialogEx::OnClose();
}

BOOL COpenholoRefAppDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint pt;
	GetCursorPos(&pt);
	m_picOphLogo.GetWindowRect(m_rcOPH);
	m_picKetiLogo.GetWindowRect(m_rcKETI);

	if (m_rcOPH.PtInRect(pt)) {
		SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_HAND)));
		return TRUE;
	}
	else if (m_rcKETI.PtInRect(pt)) {
		SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_HAND)));
		return TRUE;
	}
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void COpenholoRefAppDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ScreenToClient(&m_rcOPH);
	ScreenToClient(&m_rcKETI);
	if (!m_bClickOPH && m_rcOPH.PtInRect(point)) {
		m_bClickOPH = TRUE;
		m_picOphLogo.GetWindowRect(m_rcOPH);
		return;
	}
	else if (!m_bClickKETI && m_rcKETI.PtInRect(point)) {
		m_bClickKETI = TRUE;
		m_picKetiLogo.GetWindowRect(m_rcKETI);
		return;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void COpenholoRefAppDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ScreenToClient(&m_rcOPH);
	ScreenToClient(&m_rcKETI);
	if (m_bClickOPH && m_rcOPH.PtInRect(point)) {
		ShellExecute(NULL, L"open", L"http://openholo.org/", NULL, NULL, SW_SHOWNORMAL);
		m_bClickOPH = FALSE;
		m_picOphLogo.GetWindowRect(m_rcOPH);
		return;
	}
	else if (m_bClickKETI&& m_rcKETI.PtInRect(point)) {
		ShellExecute(NULL, L"open", L"https://www.keti.re.kr/", NULL, NULL, SW_SHOWNORMAL);
		m_bClickKETI = FALSE;
		m_picKetiLogo.GetWindowRect(m_rcKETI);
		return;
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void COpenholoRefAppDlg::report(char *szMsg)
{
#ifdef TEST_MODE
	unsigned long long nLen = strlen(szMsg);
	HWND hwndNotepad = NULL;
	hwndNotepad = ::FindWindow(NULL, L"test.txt - 메모장");
	if (hwndNotepad == NULL) {
		ShellExecute(NULL, L"open", L"D:\\test.txt", NULL, NULL, SW_SHOW);
		Sleep(100);
		hwndNotepad = ::FindWindow(NULL, L"test.txt - 메모장");
	}
	if (hwndNotepad) {
		hwndNotepad = ::FindWindowEx(hwndNotepad, NULL, L"edit", NULL);

		char *pBuf = NULL;
		int nCur = (int)::SendMessageA(hwndNotepad, WM_GETTEXTLENGTH, 0, 0);
		pBuf = new char[nCur + nLen + 1];

		::SendMessageA(hwndNotepad, WM_GETTEXT, nCur + 1, (LPARAM)pBuf);

		wsprintfA(pBuf, "%s%s", pBuf, szMsg);

		::SendMessageA(hwndNotepad, WM_SETTEXT, 0, (LPARAM)pBuf);
		delete[] pBuf;
	}
#endif
}


CString COpenholoRefAppDlg::GetFileName()
{
	CString szFileName;

	int sel = m_Tab.GetCurSel();
	switch (sel)
	{
	case 0:
		szFileName.Format(L"PointCloud_");
		break;
	case 1:
		szFileName.Format(L"DepthMap_");
		break;
	case 2:
		szFileName.Format(L"LightField_");
		break;
	case 3:
		szFileName.Format(L"TriMesh_");
		break;
	case 4:
		szFileName.Format(L"WRP_");
		break;
	}
	return szFileName;
}

void COpenholoRefAppDlg::OpenExplorer(CString szPath)
{
	if (m_buttonExplorer.GetCheck() && !szPath.IsEmpty()) {
		ShellExecute(NULL, L"open", L"explorer", L"/select, " + szPath, NULL, SW_SHOW);
	}
}

void COpenholoRefAppDlg::OnBnClickedAlwaysCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SetWindowPos(m_hWnd, m_buttonAlways.GetCheck() ? 
		HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, 
		SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
}
