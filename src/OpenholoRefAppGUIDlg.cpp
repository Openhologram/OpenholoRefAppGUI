
// OpenholoRefAppGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "OpenholoRefAppGUIDlg.h"
#include "afxdialogex.h"
#include <d3d9.h>
#include <omp.h>
#include "ophGen.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "Tab_PC.h"
#include "Tab_DM.h"
#include "Tab_LF.h"
#include "Tab_MESH.h"
#include "Tab_WRP.h"
#include "Tab_RECON.h"
#include "Dialog_BMP_Viewer.h"
#include "Dialog_Progress.h"
#include "Dialog_Prompt.h"

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
	, m_shiftX(0)
	, m_shiftY(0)
	, m_shiftZ(0)
	, m_rotate(0)
	, m_flip(0)
	, m_pixelpitchX(0)
	, m_pixelpitchY(0)
	, m_pixelnumX(0)
	, m_pixelnumY(0)
	, m_iEncode(0)
	, m_iPassband(2)
	, m_nWave(0)
	, m_iRecon(0)
	, m_iFrom(0)
	, m_bImgRotate(FALSE)
	, m_bImgMerge(FALSE)
	, m_nMaxThread(0)
	, m_nCurThread(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bClickOPH = FALSE;
	m_wavelength[0] = m_wavelength[1] = m_wavelength[2] = 0.0;
}

void COpenholoRefAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GEN_TAB, m_Tab);
	DDX_Control(pDX, IDC_COMBO_ALGORITHM, m_Algo);
	DDX_Control(pDX, IDC_COMBO_ALGORITHM_RECON, m_AlgoRecon);
	DDX_Control(pDX, IDC_COMBO_IMAGE_FLIP, m_ImgFlip);
	DDX_Control(pDX, IDC_OPH_LOGO, m_picOphLogo);
	DDX_Control(pDX, IDC_LOG_CHECK, m_buttonLog);
	DDX_Control(pDX, IDC_EXPLORER_CHECK, m_buttonExplorer);
	DDX_Control(pDX, IDC_KETI_LOGO, m_picKetiLogo);
	DDX_Control(pDX, IDC_ALWAYS_CHECK, m_buttonAlways);
	DDX_Control(pDX, IDC_GENERATE, m_buttonGenerate);
	DDX_Control(pDX, IDC_RECONSTRUCT, m_buttonReconstruct);
	DDX_Control(pDX, IDC_SAVE_IMG, m_buttonSaveBmp);
	DDX_Control(pDX, IDC_SAVE_OHC, m_buttonSaveOhc);
	DDX_Control(pDX, IDC_LOAD_OHC, m_buttonLoadOhc);
	DDX_Control(pDX, IDC_ENCODING, m_buttonEncode);
	DDX_Control(pDX, IDC_GPU_CHECK, m_buttonGPU);
	DDX_Control(pDX, IDC_TRANSFORM_VW, m_buttonViewingWindow);
	DDX_Control(pDX, IDC_RANDOM_PHASE, m_buttonRandomPhase);
	DDX_Text(pDX, IDC_SHIFT_X, m_shiftX);
	DDX_Text(pDX, IDC_SHIFT_Y, m_shiftY);
	DDX_Text(pDX, IDC_SHIFT_Z, m_shiftZ);
	DDX_Text(pDX, IDC_PIXEL_PITCH_X, m_pixelpitchX);
	DDX_Text(pDX, IDC_PIXEL_PITCH_Y, m_pixelpitchY);
	DDX_Text(pDX, IDC_PIXEL_NUM_X, m_pixelnumX);
	DDX_Text(pDX, IDC_PIXEL_NUM_Y, m_pixelnumY);
	DDX_Text(pDX, IDC_WAVE_LENGTH, m_wavelength[0]);
	DDX_Text(pDX, IDC_WAVE_LENGTH2, m_wavelength[1]);
	DDX_Text(pDX, IDC_WAVE_LENGTH3, m_wavelength[2]);
	DDX_Check(pDX, IDC_IMG_ROTATE, m_bImgRotate);
	DDX_Check(pDX, IDC_IMG_MERGE, m_bImgMerge);
	DDX_Control(pDX, IDC_ENCODE_METHOD, m_encodeMethod);
	DDX_Control(pDX, IDC_ENCODE_PASSBAND, m_encodePassband);
	DDX_Control(pDX, IDC_VIEW_IMG, m_buttonViewImg);
	DDX_Text(pDX, IDC_OMP_MAX, m_nMaxThread);
	DDX_Text(pDX, IDC_OMP_CUR, m_nCurThread);
	DDX_Control(pDX, IDC_STATIC_ALGORITHM, m_staticAlgo);
	DDX_Control(pDX, IDC_USE_FASTMATH, m_buttonFastMath);
	DDX_Control(pDX, IDC_PRECISION_SINGLE, m_buttonSingle);
	DDX_Control(pDX, IDC_PRECISION_DOUBLE, m_buttonDouble);
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
	ON_BN_CLICKED(IDC_BUTTON_EXPAND, &COpenholoRefAppDlg::OnBnClickedButtonExpand)
	ON_WM_MOVING()
	ON_MESSAGE(FLIP_APPLY, &COpenholoRefAppDlg::OnFlip)
	ON_MESSAGE(ROTATE_APPLY, &COpenholoRefAppDlg::OnRotate)
	ON_MESSAGE(LOAD_CFG, &COpenholoRefAppDlg::OnReceive)
	ON_MESSAGE(LOAD_DATA, &COpenholoRefAppDlg::OnReceive)
	ON_BN_CLICKED(IDC_GENERATE, &COpenholoRefAppDlg::OnBnClickedGenerate)
	ON_BN_CLICKED(IDC_RECONSTRUCT, &COpenholoRefAppDlg::OnBnClickedReconstruct)
	ON_BN_CLICKED(IDC_SAVE_IMG, &COpenholoRefAppDlg::OnBnClickedSaveImg)
	ON_BN_CLICKED(IDC_ENCODING, &COpenholoRefAppDlg::OnBnClickedEncoding)
	ON_BN_CLICKED(IDC_SAVE_OHC, &COpenholoRefAppDlg::OnBnClickedSaveOhc)
	ON_CBN_SELCHANGE(IDC_ENCODE_METHOD, &COpenholoRefAppDlg::OnCbnSelchangeEncodeMethod)
	ON_CBN_SELCHANGE(IDC_COMBO_ALGORITHM, &COpenholoRefAppDlg::OnCbnSelchangeComboAlgorithm)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_ALGORITHM_RECON, &COpenholoRefAppDlg::OnCbnSelchangeComboAlgorithmRecon)
	ON_CBN_SELCHANGE(IDC_ENCODE_PASSBAND, &COpenholoRefAppDlg::OnCbnSelchangeEncodePassband)
	ON_BN_CLICKED(IDC_LOAD_OHC, &COpenholoRefAppDlg::OnBnClickedLoadOhc)
	ON_BN_CLICKED(IDC_GPU_CHECK, &COpenholoRefAppDlg::OnBnClickedGpuCheck)
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
	CheckDlgButton(IDC_EXPLORER_CHECK, FALSE);
	m_AlgoRecon.ShowWindow(SW_HIDE);
	
	m_imgOPH_LOGO.Load(_T("res/OpenHolo_logo.png"));
	m_imgKETI_LOGO.Load(_T("res/KETI_logo.png"));
	GetDlgItem(IDC_OPH_LOGO)->SetWindowPos(NULL, 0, 0, m_imgOPH_LOGO.GetWidth(), m_imgOPH_LOGO.GetHeight(), SWP_NOMOVE);
	GetDlgItem(IDC_KETI_LOGO)->SetWindowPos(NULL, 0, 0, m_imgKETI_LOGO.GetWidth(), m_imgKETI_LOGO.GetHeight(), SWP_NOMOVE);
	m_option = new Option();
	m_option->Create(IDD_DLG_OPTION);
	initTabs();
	
	initUI();
	GetDlgItem(IDC_GPU_CHECK)->EnableWindow(IsGeforceGPU());
	((CButton *)GetDlgItem(IDC_PRECISION_DOUBLE))->SetCheck(TRUE);
	CWnd *pConsole = CWnd::FromHandle(GetConsoleWindow());
	if(pConsole) {
		EnableMenuItem(::GetSystemMenu(pConsole->m_hWnd, FALSE), SC_CLOSE, MF_DISABLED);
		pConsole->DrawMenuBar();
	}
	m_nMaxThread = omp_get_max_threads();
	m_nCurThread = m_nMaxThread;
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COpenholoRefAppDlg::initUI()
{
	m_encodeMethod.Clear();
	m_encodeMethod.AddString(L"Phase");
	m_encodeMethod.AddString(L"Amplitude");
	m_encodeMethod.AddString(L"Real");
	m_encodeMethod.AddString(L"Imaginary");
	m_encodeMethod.AddString(L"Simple NI");
	m_encodeMethod.AddString(L"Burckhardt");
	m_encodeMethod.AddString(L"Two-Phase");
	m_encodeMethod.AddString(L"SSB");
	m_encodeMethod.AddString(L"Off-SSB");
	m_encodeMethod.SetCurSel(m_iEncode);

	m_encodePassband.Clear();
	m_encodePassband.AddString(L"LEFT");
	m_encodePassband.AddString(L"RIGHT");
	m_encodePassband.AddString(L"TOP");
	m_encodePassband.AddString(L"BOTTOM");
	m_encodePassband.SetCurSel(m_iPassband);
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
		ReleaseDC(dc);

		pWnd = (CWnd*)GetDlgItem(IDC_KETI_LOGO);
		dc = pWnd->GetDC();
		staticSize = (CStatic *)GetDlgItem(IDC_KETI_LOGO);
		staticSize->GetClientRect(rect);
		m_imgKETI_LOGO.Destroy();
		m_imgKETI_LOGO.Load(_T("res/KETI_logo.png"));
		m_imgKETI_LOGO.Draw(dc->m_hDC, -2, -2, m_imgKETI_LOGO.GetWidth()+4, m_imgKETI_LOGO.GetHeight()+4);
		ReleaseDC(dc);

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
	m_Tab.InsertItem(0, L"Generation");
	m_Tab.InsertItem(1, L"Reconstruction");
	m_Tab.SetCurSel(0);

	m_AlgoRecon.InsertString(0, L"Fringe");
	m_AlgoRecon.InsertString(1, L"Phase & Amplitude");
	m_AlgoRecon.InsertString(2, L"Real & Imaginary");

	m_ImgFlip.InsertString(0, L"NONE");
	m_ImgFlip.InsertString(1, L"VERTICAL");
	m_ImgFlip.InsertString(2, L"HORIZONTAL");
	m_ImgFlip.InsertString(3, L"BOTH");

	CRect rc;
	m_Tab.GetWindowRect(&rc);

	m_Algo.InsertString(0, L"Point Cloud");
	m_Algo.InsertString(1, L"Depth Map");
	m_Algo.InsertString(2, L"Light Field");
	m_Algo.InsertString(3, L"Triangle Mesh");
	m_Algo.InsertString(4, L"WRP");

	m_Algo.SetCurSel(0);	
	m_AlgoRecon.SetCurSel(0);
	m_ImgFlip.SetCurSel(0);

	pTabPC = new CTab_PC;
	pTabPC->Create(IDD_DLG_PC, &m_Tab);
	pTabPC->MoveWindow(0, 55, rc.Width(), rc.Height() - 30);
	pTabPC->ShowWindow(SW_SHOW);

	pTabDM = new CTab_DM;
	pTabDM->Create(IDD_DLG_DM, &m_Tab);
	pTabDM->MoveWindow(0, 55, rc.Width(), rc.Height() - 30);
	pTabDM->ShowWindow(SW_HIDE);

	pTabLF = new CTab_LF;
	pTabLF->Create(IDD_DLG_LF, &m_Tab);
	pTabLF->MoveWindow(0, 55, rc.Width(), rc.Height() - 30);
	pTabLF->ShowWindow(SW_HIDE);

	pTabMESH = new CTab_MESH;
	pTabMESH->Create(IDD_DLG_MESH, &m_Tab);
	pTabMESH->MoveWindow(0, 55, rc.Width(), rc.Height() - 30);
	pTabMESH->ShowWindow(SW_HIDE);

	pTabWRP = new CTab_WRP;
	pTabWRP->Create(IDD_DLG_WRP, &m_Tab);
	pTabWRP->MoveWindow(0, 55, rc.Width(), rc.Height() - 30);
	pTabWRP->ShowWindow(SW_HIDE);
	
	pTabRECON = new CTab_RECON;
	pTabRECON->Create(IDD_DLG_RECON, &m_Tab);
	pTabRECON->MoveWindow(0, 55, rc.Width(), rc.Height() - 30);
	pTabRECON->ShowWindow(SW_HIDE);

	m_vector.clear();
	m_vector.push_back(pTabPC);
	m_vector.push_back(pTabDM);
	m_vector.push_back(pTabLF);
	m_vector.push_back(pTabMESH);
	m_vector.push_back(pTabWRP);
	m_vector.push_back(pTabRECON);

}



void COpenholoRefAppDlg::OnTcnSelchangeGenTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	ReloadContents();
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
		if (!ext.CompareNoCase(L"xml")) path = FileDialog.GetPathName();
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
	pTabRECON->DestroyWindow();
	delete pTabPC;
	delete pTabDM;
	delete pTabMESH;
	delete pTabWRP;
	delete pTabLF;
	delete pTabRECON;
	if (m_option->GetSafeHwnd())
		m_option->DestroyWindow();
	if (m_option)
		delete m_option;

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

CString COpenholoRefAppDlg::GetFileName()
{
	CString szFileName;
#if true
	szFileName = L"";
#else
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
#endif
	szFileName.AppendFormat(L"%s", m_buttonGPU.GetCheck() ? L"GPU" : L"CPU");

	return szFileName;
}

CString COpenholoRefAppDlg::GetFileNameExt()
{
	CString szFileName = GetFileName();
	CString szEncode;
	m_encodeMethod.GetLBText(m_encodeMethod.GetCurSel(), szEncode);
	szFileName.AppendFormat(L"_%s", szEncode);
	szFileName.AppendFormat(L" (RandomPhase %s)", m_buttonRandomPhase.GetCheck() ? L"On" : L"Off");
	if (m_bImgRotate)
		szFileName.AppendFormat(L" (Rotate)");
	int nFlip = m_ImgFlip.GetCurSel();
	if (nFlip != 0)
		szFileName.AppendFormat(L" (Flip_%s)", nFlip == 1 ? L"V" : nFlip == 2 ? L"H" : L"B");

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

bool COpenholoRefAppDlg::SaveOHC(CString &path)
{
	LPTSTR szFilter = L"OHC File (*.ohc) |*.ohc|";

	TCHAR szCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurPath);

	CFileDialog FileDialog(FALSE, NULL, GetFileName(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString szCfgPath = AfxGetApp()->GetProfileString(L"Result", L"OHC Path", szCurPath);
	FileDialog.m_ofn.lpstrInitialDir = szCfgPath;
	if (FileDialog.DoModal() == IDOK)
	{
		path.Format(L"%s.ohc", FileDialog.GetPathName());
		AfxGetApp()->WriteProfileString(L"Result", L"OHC Path", path.Left(path.ReverseFind('\\') + 1));
		return true;
	}
	return false;
}

bool COpenholoRefAppDlg::SaveImage(CString &path)
{
	TCHAR szExtension[][5] = { L"bmp", L"jpg", L"gif", L"png", L"tif", L"bmp" };
	LPTSTR szFilter =
		L"BMP File (*.bmp)|*.bmp|\
		JPEG File (*.jpg;*.jpeg)|*.jpg;*.jpeg|\
		GIF File (*.gif)|*.gif|\
		PNG File (*.png)|*.png|\
		TIFF File (*.tif;*.tiff)|*.tif;*.tiff|\
		All Files (*.*)|*.*||";

	TCHAR szCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurPath);
	CFileDialog FileDialog(FALSE, NULL, GetFileNameExt(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString szCfgPath = AfxGetApp()->GetProfileString(L"Result", L"Image Path", szCurPath);
	FileDialog.m_ofn.lpstrInitialDir = szCfgPath;
	if (FileDialog.DoModal() == IDOK)
	{
		int idx = FileDialog.m_ofn.nFilterIndex;
		if (!FileDialog.GetFileExt().Compare(szExtension[idx - 1]))
			path.Format(L"%s\\%s",
				FileDialog.GetFolderPath(),
				FileDialog.GetFileName());
		else
			path.Format(L"%s.%s",
			FileDialog.GetPathName(),
			szExtension[idx - 1]);
		AfxGetApp()->WriteProfileString(L"Result", L"Image Path", path.Left(path.ReverseFind('\\') + 1));
		return true;
	}
	return false;
}

void COpenholoRefAppDlg::OnBnClickedButtonExpand()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static bool bClicked = false;
	if (!bClicked) {
		MoveOptionDlg();
		bClicked = true;
	}
	m_option->ShowWindow(m_option->IsWindowVisible() ? SW_HIDE : SW_SHOW);
	GetDlgItem(IDC_BUTTON_EXPAND)->SetWindowTextW(m_option->IsWindowVisible() ? L"◀" : L"▶");
}

void COpenholoRefAppDlg::MoveOptionDlg()
{
	CRect rc, rcOption;
	GetWindowRect(&rc);
	//ScreenToClient(&rc);
	m_option->GetWindowRect(&rcOption);
	m_option->MoveWindow(rc.right - 15, rc.top, rcOption.Width(), rcOption.Height(), 0);
}

void COpenholoRefAppDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	MoveOptionDlg();
}

LRESULT COpenholoRefAppDlg::OnFlip(WPARAM wParam, LPARAM lParam)
{
	m_flip = m_option->GetFlip();
	return TRUE;
}


LRESULT COpenholoRefAppDlg::OnRotate(WPARAM wParam, LPARAM lParam)
{
	m_rotate = m_option->GetRotate();
	return TRUE;
}

void COpenholoRefAppDlg::OnBnClickedGenerate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_pixelpitchX == 0.0 || m_pixelpitchY == 0.0) {
		AfxMessageBox(L"Config value error - pixel pitch");
		return;
	}
	if (m_pixelnumX == 0 || m_pixelnumY == 0) {
		AfxMessageBox(L"Config value error - pixel number");
		return;
	}
	if (m_wavelength[0] == 0.0) {
		AfxMessageBox(L"Config value error - wave length");
		return;
	}
	int sel = m_Algo.GetCurSel();
	int mode = MODE_CPU;
	if (m_buttonGPU.GetCheck())
		mode |= MODE_GPU;
	if (m_buttonSingle.GetCheck())
		mode |= MODE_FLOAT;
	if (m_buttonFastMath.GetCheck())
		mode |= MODE_FASTMATH;

	m_vector[sel]->SendMessage(GENERATE, mode, 0);
	m_encodeMethod.EnableWindow(TRUE);
	m_encodePassband.EnableWindow(m_iEncode > 6 ? TRUE : FALSE);
	m_buttonEncode.EnableWindow(TRUE);
	m_buttonSaveBmp.EnableWindow(FALSE);
	m_buttonSaveOhc.EnableWindow(TRUE);
	m_buttonViewImg.EnableWindow(FALSE);
}

void COpenholoRefAppDlg::OnBnClickedReconstruct()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_pixelpitchX == 0.0 || m_pixelpitchY == 0.0) {
		AfxMessageBox(L"Config value error - pixel pitch");
		return;
	}
	if (m_pixelnumX == 0 || m_pixelnumY == 0) {
		AfxMessageBox(L"Config value error - pixel number");
		return;
	}
	if (m_wavelength[0] == 0.0) {
		AfxMessageBox(L"Config value error - wave length");
		return;
	}
	int sel = m_vector.size() - 1;

	int mode = MODE_CPU;
	if (m_buttonGPU.GetCheck())
		mode |= MODE_GPU;

	m_vector[sel]->SendMessage(RECONSTRUCT, mode, 0);

	//m_buttonEncode.EnableWindow(TRUE);
	m_buttonSaveBmp.EnableWindow(TRUE);
	m_buttonViewImg.EnableWindow(TRUE);
	//m_buttonSaveOhc.EnableWindow(TRUE);
	//m_buttonViewImg.EnableWindow(FALSE);
}


void COpenholoRefAppDlg::OnBnClickedSaveImg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int tab = m_Tab.GetCurSel();
	int mode = 0;
	mode |= m_bImgMerge ? 0x1 : 0x0;
	mode |= m_bImgRotate ? 0x2 : 0x0;

	if (tab == 0)
	{
		int sel = m_Algo.GetCurSel();
		BOOL res = m_vector[sel]->SendMessage(SAVE_IMG, mode, m_ImgFlip.GetCurSel());
	}
	else
	{
		BOOL res = m_vector[m_vector.size()-1]->SendMessage(SAVE_IMG, mode, m_ImgFlip.GetCurSel());
	}
}


void COpenholoRefAppDlg::OnBnClickedEncoding()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UpdateData(FALSE);
	int sel = m_Algo.GetCurSel();

	BOOL res = m_vector[sel]->SendMessage(ENCODE, m_iEncode, m_iPassband);
	
	m_buttonViewImg.EnableWindow(TRUE);
	m_buttonSaveBmp.EnableWindow(TRUE);
}


void COpenholoRefAppDlg::OnBnClickedSaveOhc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int sel = m_Algo.GetCurSel();
	m_vector[sel]->SendMessage(SAVE_OHC, 0, 0);
}


void COpenholoRefAppDlg::OnCbnSelchangeEncodeMethod()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iEncode = m_encodeMethod.GetCurSel();
	m_encodePassband.EnableWindow(m_iEncode > 6 ? TRUE : FALSE);
}


void COpenholoRefAppDlg::OnCbnSelchangeEncodePassband()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iPassband = m_encodePassband.GetCurSel();
}


LRESULT COpenholoRefAppDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{	
	int sel = m_Tab.GetCurSel();
	if (wParam == LOAD_CFG) {
		if (sel == 0)
		{
			m_encodeMethod.EnableWindow(FALSE);
			m_encodePassband.EnableWindow(FALSE);
			m_buttonGenerate.EnableWindow(FALSE);
			m_buttonEncode.EnableWindow(FALSE);
			m_buttonSaveOhc.EnableWindow(FALSE);
			m_buttonViewImg.EnableWindow(FALSE);
		}
		else
			m_buttonReconstruct.EnableWindow(FALSE);		
		m_buttonSaveBmp.EnableWindow(FALSE);

		UpdateData(FALSE);
	}
	else if (wParam == LOAD_DATA) {
		if (sel == 0)
		{
			m_encodeMethod.EnableWindow(FALSE);
			m_encodePassband.EnableWindow(FALSE);
			m_buttonGenerate.EnableWindow(TRUE);
			m_buttonEncode.EnableWindow(FALSE);
			m_buttonSaveOhc.EnableWindow(FALSE);
			m_buttonViewImg.EnableWindow(FALSE);
		}
		else
			m_buttonReconstruct.EnableWindow(TRUE);
		m_buttonSaveBmp.EnableWindow(FALSE);
	}
	return TRUE;
}

void COpenholoRefAppDlg::OnCbnSelchangeComboAlgorithm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ReloadContents();
}


HBRUSH COpenholoRefAppDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void COpenholoRefAppDlg::ReloadContents()
{
	int comboSel = m_Algo.GetCurSel();
	int tabSel = m_Tab.GetCurSel();

	m_Algo.ShowWindow(tabSel == 0 ? SW_SHOW : SW_HIDE);
	m_AlgoRecon.ShowWindow(tabSel == 1 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_STATIC_SHIFT)->ShowWindow(tabSel == 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_STATIC_ENCODE)->ShowWindow(tabSel == 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_SHIFT_X)->ShowWindow(tabSel == 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_SHIFT_Y)->ShowWindow(tabSel == 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_SHIFT_Z)->ShowWindow(tabSel == 0 ? SW_SHOW : SW_HIDE);
	m_encodeMethod.ShowWindow(tabSel == 0 ? SW_SHOW : SW_HIDE);
	m_encodePassband.ShowWindow(tabSel == 0 ? SW_SHOW : SW_HIDE);
	m_buttonEncode.ShowWindow(tabSel == 0 ? SW_SHOW : SW_HIDE);
	m_buttonRandomPhase.ShowWindow(tabSel == 0 ? SW_SHOW : SW_HIDE);
	m_buttonViewingWindow.ShowWindow(tabSel == 0 ? SW_SHOW : SW_HIDE);
	m_buttonFastMath.ShowWindow(tabSel == 0 ? SW_SHOW : SW_HIDE);

	if (tabSel == 0)
	{
		pTabPC->ShowWindow(comboSel == 0 ? SW_SHOW : SW_HIDE);
		pTabDM->ShowWindow(comboSel == 1 ? SW_SHOW : SW_HIDE);
		pTabLF->ShowWindow(comboSel == 2 ? SW_SHOW : SW_HIDE);
		pTabMESH->ShowWindow(comboSel == 3 ? SW_SHOW : SW_HIDE);
		pTabWRP->ShowWindow(comboSel == 4 ? SW_SHOW : SW_HIDE);
		pTabRECON->ShowWindow(SW_HIDE);
		m_buttonGenerate.ShowWindow(SW_SHOW);
		m_buttonReconstruct.ShowWindow(SW_HIDE);
		m_buttonLoadOhc.ShowWindow(SW_SHOW);
		m_buttonSaveOhc.ShowWindow(SW_SHOW);
		m_staticAlgo.SetWindowTextW(L"Algorithm: ");			
	}
	else
	{
		pTabRECON->ShowWindow(SW_SHOW);
		pTabPC->ShowWindow(SW_HIDE);
		pTabDM->ShowWindow(SW_HIDE);
		pTabLF->ShowWindow(SW_HIDE);
		pTabMESH->ShowWindow(SW_HIDE);
		pTabWRP->ShowWindow(SW_HIDE);
		m_buttonGenerate.ShowWindow(SW_HIDE);
		m_buttonReconstruct.ShowWindow(SW_SHOW);
		m_buttonLoadOhc.ShowWindow(SW_HIDE);
		m_buttonSaveOhc.ShowWindow(SW_HIDE);
		pTabRECON->ReloadContents(m_iRecon);
		m_staticAlgo.SetWindowTextW(L"From: ");
	}
}


void COpenholoRefAppDlg::OnCbnSelchangeComboAlgorithmRecon()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int sel = m_AlgoRecon.GetCurSel();

	pTabRECON->ReloadContents(sel);
	m_iRecon = sel;
}


void COpenholoRefAppDlg::OnBnClickedLoadOhc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int sel = m_Algo.GetCurSel();
	m_vector[sel]->SendMessage(LOAD_OHC, 0, 0);
	m_encodeMethod.EnableWindow(TRUE);
	m_encodePassband.EnableWindow(m_iEncode > 6 ? TRUE : FALSE);
	m_buttonEncode.EnableWindow(TRUE);
	m_buttonSaveBmp.EnableWindow(FALSE);
	//m_buttonSaveOhc.EnableWindow(TRUE);
	m_buttonViewImg.EnableWindow(FALSE);
}


void COpenholoRefAppDlg::OnBnClickedGpuCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CButton *btn = ((CButton *)GetDlgItem(IDC_GPU_CHECK));
	if (btn != nullptr)
	{
		GetDlgItem(IDC_OMP_CUR)->EnableWindow(btn->GetCheck() ? FALSE : TRUE);
		GetDlgItem(IDC_PRECISION_DOUBLE)->EnableWindow(btn->GetCheck() ? TRUE : FALSE);
		GetDlgItem(IDC_PRECISION_SINGLE)->EnableWindow(btn->GetCheck() ? TRUE : FALSE);
		GetDlgItem(IDC_USE_FASTMATH)->EnableWindow(btn->GetCheck() ? TRUE : FALSE);
	}
}
