// Tab_WRP.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "OpenholoRefAppGUIDlg.h"
#include "Tab_WRP.h"
#include "afxdialogex.h"
#include "Console.h"

// CTab_WRP dialog
#include <ophWRP.h>
#include "Dialog_BMP_Viewer.h"
#include "Dialog_Progress.h"
#include "Dialog_Prompt.h"

IMPLEMENT_DYNAMIC(CTab_WRP, CDialogEx)

CTab_WRP::CTab_WRP(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_WRP, pParent)
	, m_fieldLens(0)
	, m_scaleX(0)
	, m_scaleY(0)
	, m_scaleZ(0)
	, m_numofWRP(0)
	, m_locationWRP(0)
	, m_distance(0)
	, m_pixelpitchX(0)
	, m_pixelpitchY(0)
	, m_pixelnumX(0)
	, m_pixelnumY(0)
	, m_wavelength(0)
	, m_argParam()
	, m_resultPath()
	, m_bConfig(FALSE)
	, m_bPC(FALSE)
	, m_bEncode(FALSE)
	, m_idxEncode(0)
#ifdef TEST_MODE
	, m_bTest(FALSE)
#endif
{

}

CTab_WRP::~CTab_WRP()
{
}

void CTab_WRP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FIELD_LENS, m_fieldLens);
	DDX_Text(pDX, IDC_SCALE_X, m_scaleX);
	DDX_Text(pDX, IDC_SCALE_Y, m_scaleY);
	DDX_Text(pDX, IDC_SCALE_Z, m_scaleZ);
	DDX_Text(pDX, IDC_OFFSET_DEPTH, m_numofWRP);
	DDX_Text(pDX, IDC_SCALE_Y2, m_locationWRP);
	DDX_Text(pDX, IDC_SCALE_Z2, m_distance);
	DDX_Text(pDX, IDC_PIXEL_PITCH_X, m_pixelpitchX);
	DDX_Text(pDX, IDC_PIXEL_PITCH_Y, m_pixelpitchY);
	DDX_Text(pDX, IDC_PIXEL_NUM_X, m_pixelnumX);
	DDX_Text(pDX, IDC_PIXEL_NUM_Y, m_pixelnumY);
	DDX_Text(pDX, IDC_WAVE_LENGTH, m_wavelength);
	DDX_Control(pDX, IDC_TRANSFORM_VW, m_buttonViewingWindow);
	DDX_Control(pDX, IDC_GPU_CHECK_WRP, m_buttonGPU);
}


BEGIN_MESSAGE_MAP(CTab_WRP, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_READ_CONFIG_WRP, &CTab_WRP::OnBnClickedReadConfigWrp)
	ON_BN_CLICKED(IDC_LOAD_PC_WRP, &CTab_WRP::OnBnClickedLoadPcWrp)
	ON_BN_CLICKED(IDC_GENERATE_WRP, &CTab_WRP::OnBnClickedGenerateWrp)
	ON_BN_CLICKED(IDC_SAVE_BMP_WRP, &CTab_WRP::OnBnClickedSaveBmpWrp)
	ON_BN_CLICKED(IDC_SAVE_OHC_WRP, &CTab_WRP::OnBnClickedSaveOhcWrp)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_VIEW_WRP, &CTab_WRP::OnBnClickedViewWrp)
	ON_BN_CLICKED(IDC_VIEW_WRP_BMP, &CTab_WRP::OnBnClickedViewWrpBmp)
	ON_CBN_SELCHANGE(IDC_ENCODE_METHOD_WRP, &CTab_WRP::OnCbnSelchangeEncodeMethodWrp)
	ON_BN_CLICKED(IDC_ENCODING_WRP, &CTab_WRP::OnBnClickedEncodingWrp)
END_MESSAGE_MAP()


// CTab_WRP message handlers

BOOL CTab_WRP::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class	
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
#ifdef TEST_MODE
	if (!m_bTest && pMsg->wParam == VK_SPACE) {
		AutoTest();
		return TRUE;
	}
#endif
	return CDialogEx::PreTranslateMessage(pMsg);
}

#ifdef TEST_MODE
BOOL CTab_WRP::AutoTest()
{
	if (!m_bPC || !m_bConfig)
		return FALSE;
	m_bTest = TRUE;
	Dialog_Prompt *prompt = new Dialog_Prompt;
	if (IDOK == prompt->DoModal()) {
		int nRepeat = prompt->GetInputInteger();
		for (int i = 0; i < nRepeat; i++)
			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_GENERATE_WRP, BN_CLICKED), 0L);
	}
	delete prompt;
	m_bTest = FALSE;
	return TRUE;
}
#endif

int CTab_WRP::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pWRP = new ophWRP();

	//auto pszArgv = GetCommandLine();
	//int argc = 0;
	//CString strArgv = pszArgv;
	//vector<std::string> vArgv;
	//while (1) {
	//	int nNextSpace = 0;
	//	nNextSpace = strArgv.Find(_T(" "));
	//	if (nNextSpace == 0) break;
	//	argc++;

	//	CString strNextArgv = strArgv.Mid(nNextSpace + 2);
	//	AfxMessageBox(strNextArgv);


	//}
	return 0;
}


void CTab_WRP::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//SetWindowPos(NULL, 0, 0, 353, 305, SWP_NOMOVE);
}


void CTab_WRP::OnBnClickedReadConfigWrp()
{
	// TODO: Add your control notification handler code here
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);

	LPTSTR szFilter = L"XML File (*.xml) |*.xml|";

	CFileDialog FileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (!ext.CompareNoCase(L"xml")) path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName();
		else return;
	}

	SetCurrentDirectory(current_path);

	TCHAR widepath[MAX_PATH] = { 0 };
	char mulpath[MAX_PATH] = { 0 };

	_tcscpy_s(widepath, path.GetBuffer());
	WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);
	if (strcmp(mulpath, "") == 0) return;

	if (!m_pWRP->readConfig(mulpath)) {
		AfxMessageBox(L"it is not xml config file for WRP.");
		return;
	}

	oph::vec3 scale;

	scale = m_pWRP->getScale();
	auto context = m_pWRP->getContext();
	m_fieldLens = m_pWRP->getFieldLens();
	m_scaleX = scale[_X];
	m_scaleY = scale[_Y];
	m_scaleZ = scale[_Z];
	m_distance = m_pWRP->getDistance();
	m_pixelpitchX = context.pixel_pitch[_X];
	m_pixelpitchY = context.pixel_pitch[_Y];
	m_pixelnumX = context.pixel_number[_X];
	m_pixelnumY = context.pixel_number[_Y];
	m_wavelength = context.wave_length[0];
	m_numofWRP = m_pWRP->getNumOfWRP();
	m_locationWRP = m_pWRP->getLocation();

	m_bConfig = true;
	if (m_bPC) GetDlgItem(IDC_GENERATE_WRP)->EnableWindow(TRUE);

	UpdateData(FALSE);
}


void CTab_WRP::OnBnClickedLoadPcWrp()
{
	// TODO: Add your control notification handler code here
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);

	LPTSTR szFilter = L"PLY File (*.ply) |*.ply|";

	CFileDialog FileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (!ext.CompareNoCase(L"ply")) path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName();
		else return;
	}

	SetCurrentDirectory(current_path);

	TCHAR widepath[MAX_PATH] = { 0 };
	char mulpath[MAX_PATH] = { 0 };

	_tcscpy_s(widepath, path.GetBuffer());
	_tcscpy_s(m_argParam, path.GetBuffer());
	WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);
	if (strcmp(mulpath, "") == 0) return;

	if (m_pWRP->loadPointCloud(mulpath) == -1)
	{
		AfxMessageBox(L"it is not a ply file for WRP.");
		return;
	}

	m_bPC = true;
	if (m_bConfig) GetDlgItem(IDC_GENERATE_WRP)->EnableWindow(TRUE);

	GetDlgItem(IDC_VIEW_WRP)->EnableWindow(TRUE);

	UpdateData(FALSE);
}


void CTab_WRP::OnBnClickedViewWrp()
{
	// TODO: Add your control notification handler code here
	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, sizeof(path));

	CString localPath = path;
	int i = localPath.ReverseFind('\\');
	localPath = localPath.Left(i);
	localPath.Append(L"\\3D_Object_Viewer.exe");
	_tcscpy_s(path, localPath.GetBuffer());

	CFileFind ff;
	if (ff.FindFile(localPath)) {
		TCHAR argParam[MAX_PATH * 3] = { 0 };

		int pc_flag = 0;

		CString szArgParam = CString("\"") + (m_argParam)+CString("\"");

		wsprintf(argParam, L"%d %s", pc_flag, szArgParam.GetBuffer());


		::ShellExecute(NULL, _T("open"), path, argParam, NULL, SW_SHOW);
	}
	else {
		AfxMessageBox(localPath + L"을(를) 찾을 수 없습니다.");
	}
}

UINT CallFuncWRP(void* param)
{
	parammeter *pParam = (parammeter *)param;
	((ophWRP*)pParam->pGEN)->generateHologram();
	pParam->pDialog->m_bFinished = TRUE;

	ophWRP *pWRP = ((ophWRP *)pParam->pGEN);
	Complex<Real> **pp = pWRP->getComplexField();

	Console::getInstance()->SetColor(Console::Color::YELLOW, Console::Color::BLACK);
	for (uint i = 0; i < pWRP->getContext().waveNum; i++)
		printf("=> Complex Field[%d][0] = %.15e / %.15e\n", i, pp[i][0][_RE], pp[i][0][_IM]);
	Console::getInstance()->ResetColor();
	delete pParam;

	return 1;
}

void CTab_WRP::OnBnClickedGenerateWrp()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_buttonViewingWindow.GetCheck() && m_fieldLens == 0.0) {
		AfxMessageBox(L"Config value error - field lens");
		return;
	}
	if (m_scaleX == 0.0 || m_scaleY == 0.0 || m_scaleZ == 0.0) {
		AfxMessageBox(L"Config value error - scale");
		return;
	}
	if (m_distance == 0.0) {
		AfxMessageBox(L"Config value error - offset depth");
		return;
	}
	if (m_pixelpitchX == 0.0 || m_pixelpitchY == 0.0) {
		AfxMessageBox(L"Config value error - pixel pitch");
		return;
	}
	if (m_pixelnumX == 0 || m_pixelnumY == 0) {
		AfxMessageBox(L"Config value error - pixel number");
		return;
	}
	if (m_wavelength == 0.0) {
		AfxMessageBox(L"Config value error - wave length");
		return;
	}

	auto context = m_pWRP->getContext();
	m_pWRP->setDistance(m_distance);
	context.pixel_pitch[_X] = m_pixelpitchX;
	context.pixel_pitch[_Y] = m_pixelpitchY;
	context.pixel_number[_X] = m_pixelnumX;
	context.pixel_number[_Y] = m_pixelnumY;
	context.wave_length[0] = m_wavelength;
	
	m_pWRP->setMode(!m_buttonGPU.GetCheck());
	m_pWRP->setScale(vec3(m_scaleX, m_scaleY, m_scaleZ));
	m_pWRP->setLocation(m_locationWRP);
	m_pWRP->setViewingWindow(m_buttonViewingWindow.GetCheck());

	GetDlgItem(IDC_SAVE_BMP_WRP)->EnableWindow(FALSE);
	GetDlgItem(IDC_SAVE_OHC_WRP)->EnableWindow(TRUE);
	GetDlgItem(IDC_ENCODING_WRP)->EnableWindow(TRUE);

	((COpenholoRefAppDlg *)AfxGetMainWnd())->ForegroundConsole();

	Dialog_Progress progress;

	BOOL bIsFinish = FALSE;

	parammeter *pParam = new parammeter;
	pParam->pGEN = m_pWRP;
	pParam->pDialog = &progress;

	CWinThread* pThread = AfxBeginThread(CallFuncWRP, pParam);
	progress.DoModal();
	progress.DestroyWindow();

	char szMsg[256] = { 0, };
	sprintf_s(szMsg, "Total Elapsed Time: %lf (s)\n", m_pWRP->getElapsedTime());
	((COpenholoRefAppDlg *)AfxGetMainWnd())->report(szMsg);

	MakeFileName();
}

void CTab_WRP::MakeFileName(CString szAppend)
{
	if (szAppend.IsEmpty()) {
		m_szFileName.Empty();
	}

	m_szFileName = ((COpenholoRefAppDlg *)AfxGetMainWnd())->GetFileName();
	m_szFileName.AppendFormat(L"%dch_", m_pWRP->getContext().waveNum);
	m_szFileName.AppendFormat(L"%dx%d_", m_pWRP->getContext().pixel_number[_X], m_pWRP->getContext().pixel_number[_Y]);
	m_szFileName.AppendFormat(L"v%d_", m_pWRP->getNumOfPoints());
	m_szFileName.AppendFormat(L"%s_", m_buttonGPU.GetCheck() ? L"GPU" : L"CPU");
	m_szFileName.AppendFormat(L"%s", m_buttonViewingWindow.GetCheck() ? L"VW_" : L"");
}

void CTab_WRP::OnBnClickedEncodingWrp()
{
	// TODO: Add your control notification handler code here
	auto dist = m_pWRP->getDistance();
//	m_pWRP->waveCarry(0, 0.1, dist);
	switch (ophGen::ENCODE_FLAG(m_idxEncode)) {
	case ophGen::ENCODE_PHASE:
	case ophGen::ENCODE_AMPLITUDE:
	case ophGen::ENCODE_REAL:
	case ophGen::ENCODE_SIMPLENI:
	case ophGen::ENCODE_BURCKHARDT:
	case ophGen::ENCODE_TWOPHASE:
		m_pWRP->encoding(ophGen::ENCODE_FLAG(m_idxEncode));
		break;
	case ophGen::ENCODE_SSB:
	case ophGen::ENCODE_OFFSSB:
		m_pWRP->encoding(ophGen::ENCODE_FLAG(m_idxEncode), ophGen::SSB_TOP);
		break;
	}

	m_pWRP->normalize();

	GetDlgItem(IDC_SAVE_BMP_WRP)->EnableWindow(TRUE);
	GetEncodeName(m_szEncodeName);
}


void CTab_WRP::OnBnClickedSaveBmpWrp()
{
	// TODO: Add your control notification handler code here
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);
#if 0
	LPTSTR szFilter = 
		L"BMP File (*.bmp)|*.bmp|\
		JPG File (*.jpg)|*.jpg|\
		GIF File (*.gif)|*.gif|\
		PNG File (*.png)|*.png|\
		All Files (*.*)|*.*||";
	
	CFileDialog FileDialog(FALSE, NULL, Time::getInstance()->GetTime(L"WRP"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{		
		CString name = FileDialog.GetFileName();
		path.Format(L"%s\\%s", FileDialog.GetFolderPath(), FileDialog.GetFileName());
		int iFilter = FileDialog.GetOFN().nFilterIndex;
		switch (iFilter) {
		case 1: {
			if (name.Find(L".bmp") < 0)//.CompareNoCase(L"bmp")) 
				path.Append(L".bmp");
			break;
		}
		case 2: {
			if (name.Find(L".jpg") < 0)
				path.Append(L".jpg");
			break;
		}
		case 3: {
			if (name.Find(L".gif") < 0)
				path.Append(L".gif");
			break;
		}
		case 4: {
			if (name.Find(L".png") < 0)
				path.Append(L".png");
			break;
		}
		default: {
			if (name.Find(L".bmp") < 0)
				path.Append(L".bmp");
			break;
		}
		}
	}
#else
	LPTSTR szFilter = L"BMP File (*.bmp) |*.bmp|";

	CString szFileName = m_szFileName;
	szFileName.AppendFormat(L"%s", m_szEncodeName);
	
	CFileDialog FileDialog(FALSE, NULL, szFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (!ext.CompareNoCase(L"bmp")) path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName();
		else path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName() + L".bmp";
	}

#endif
	SetCurrentDirectory(current_path);

	TCHAR widepath[MAX_PATH] = { 0 };
	char mulpath[MAX_PATH] = { 0 };

	_tcscpy_s(widepath, path.GetBuffer());
	_tcscpy_s(m_resultPath, path.GetBuffer());
	WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);

	if (strcmp(mulpath, "") == 0) return;
	m_pWRP->save(mulpath);

	GetDlgItem(IDC_VIEW_WRP_BMP)->EnableWindow(TRUE);

	((COpenholoRefAppDlg *)AfxGetMainWnd())->OpenExplorer(path);
}

void CTab_WRP::OnBnClickedViewWrpBmp()
{
	// TODO: Add your control notification handler code here
	Dialog_BMP_Viewer viewer;

	viewer.Init(m_resultPath, INIT_SINGLE);
	viewer.DoModal();

	viewer.DestroyWindow();
}


void CTab_WRP::OnBnClickedSaveOhcWrp()
{
	// TODO: Add your control notification handler code here
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);

	LPTSTR szFilter = L"OHC File (*.ohc) |*.ohc|";

	CFileDialog FileDialog(FALSE, NULL, m_szFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (!ext.CompareNoCase(L"ohc")) path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName();
		else path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName() + L".ohc";
	}

	SetCurrentDirectory(current_path);

	TCHAR widepath[MAX_PATH] = { 0 };
	char mulpath[MAX_PATH] = { 0 };

	_tcscpy_s(widepath, path.GetBuffer());
	WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);

	if (strcmp(mulpath, "") == 0) return;
	if (m_pWRP->saveAsOhc(mulpath)) {

		((COpenholoRefAppDlg *)AfxGetMainWnd())->OpenExplorer(path);
	}
}

void CTab_WRP::GetEncodeName(CString &szEncode)
{
	switch (m_idxEncode)
	{
	case 0: szEncode = L"Phase"; break;
	case 1: szEncode = L"Amplitude"; break;
	case 2: szEncode = L"Real"; break;
	case 3: szEncode = L"SimpleNI"; break;
	case 4: szEncode = L"Burckhardt"; break;
	case 5: szEncode = L"TwoPhase"; break;
	case 6: szEncode = L"SSB"; break;
	case 7: szEncode = L"OffSSB"; break;
	default: szEncode = L"Unknown"; break;
	}
}

BOOL CTab_WRP::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_WRP))->AddString(L"Phase");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_WRP))->AddString(L"Amplitude");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_WRP))->AddString(L"Real");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_WRP))->AddString(L"Simple NI");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_WRP))->AddString(L"Burckhardt");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_WRP))->AddString(L"Two-Phase");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_WRP))->AddString(L"Single-Side Band");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_WRP))->AddString(L"Off-SSB");

	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_WRP))->SetCurSel(m_idxEncode);

	// GeForce GPU 일 때만, 활성화
	COpenholoRefAppDlg *pDlg = (COpenholoRefAppDlg *)AfxGetApp()->GetMainWnd();
	((CButton*)GetDlgItem(IDC_GPU_CHECK_WRP))->EnableWindow(pDlg->IsGeforceGPU());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CTab_WRP::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	m_pWRP->release();
}

void CTab_WRP::OnCbnSelchangeEncodeMethodWrp()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_idxEncode = ((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_WRP))->GetCurSel();
}
