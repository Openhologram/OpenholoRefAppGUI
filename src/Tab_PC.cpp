// Tab_PC.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "OpenholoRefAppGUIDlg.h"
#include "Tab_PC.h"
#include "afxdialogex.h"



// CTab_PC dialog

#include <ophPointCloud.h>
#include "Dialog_BMP_Viewer.h"
#include "Dialog_Progress.h"
#include "Dialog_Prompt.h"

#include "GLUTviewer.h"

#include <shellapi.h>

IMPLEMENT_DYNAMIC(CTab_PC, CDialogEx)

CTab_PC::CTab_PC(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_PC, pParent)
	, m_fieldLens(0)
	, m_scaleX(0)
	, m_scaleY(0)
	, m_scaleZ(0)
	, m_offsetdepth(0)
	, m_pixelpitchX(0)
	, m_pixelpitchY(0)
	, m_pixelnumX(0)
	, m_pixelnumY(0)
	, m_wavelength(0)
	, m_bConfig(FALSE)
	, m_bPC(FALSE)
	, m_bEncode(FALSE)
	, m_argParam()
	, m_argParamW()
	, m_resultPath()
	, m_idxDiff(0)
	, m_idxEncode(6)
	, m_bFinish(FALSE)
#ifdef TEST_MODE
	, m_bTest(FALSE)
#endif
{

}

CTab_PC::~CTab_PC()
{
}

void CTab_PC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FIELD_LENS, m_fieldLens);
	DDX_Text(pDX, IDC_SCALE_X, m_scaleX);
	DDX_Text(pDX, IDC_SCALE_Y, m_scaleY);
	DDX_Text(pDX, IDC_SCALE_Z, m_scaleZ);
	DDX_Text(pDX, IDC_OFFSET_DEPTH, m_offsetdepth);
	DDX_Text(pDX, IDC_PIXEL_PITCH_X, m_pixelpitchX);
	DDX_Text(pDX, IDC_PIXEL_PITCH_Y, m_pixelpitchY);
	DDX_Text(pDX, IDC_PIXEL_NUM_X, m_pixelnumX);
	DDX_Text(pDX, IDC_PIXEL_NUM_Y, m_pixelnumY);
	DDX_Text(pDX, IDC_WAVE_LENGTH, m_wavelength);
	DDX_Control(pDX, IDC_GENERATE_PC, m_buttonGenerate);
	DDX_Control(pDX, IDC_SAVE_BMP_PC, m_buttonSaveBmp);
	DDX_Control(pDX, IDC_SAVE_OHC_PC, m_buttonSaveOhc);
	DDX_Control(pDX, IDC_GPU_CHECK_PC, m_buttonGPU);
	DDX_Control(pDX, IDC_TRANSFORM_VW, m_buttonViewingWindow);
}


BEGIN_MESSAGE_MAP(CTab_PC, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
//	ON_WM_INITMENU()
ON_BN_CLICKED(IDC_LOAD_PC, &CTab_PC::OnBnClickedLoadPc)
ON_BN_CLICKED(IDC_GENERATE_PC, &CTab_PC::OnBnClickedGenerate_PC)
ON_BN_CLICKED(IDC_SAVE_BMP_PC, &CTab_PC::OnBnClickedSaveBmp_PC)
ON_BN_CLICKED(IDC_SAVE_OHC_PC, &CTab_PC::OnBnClickedSaveOhc_PC)
ON_BN_CLICKED(IDC_READ_CONFIG_PC, &CTab_PC::OnBnClickedReadConfig_PC)
ON_WM_CREATE()
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_VIEW_PC, &CTab_PC::OnBnClickedViewPc)
//ON_WM_KEYDOWN()
ON_BN_CLICKED(IDC_VIEW_PC_BMP, &CTab_PC::OnBnClickedViewPcBmp)
ON_CBN_SELCHANGE(IDC_DIFF_METHOD_PC, &CTab_PC::OnCbnSelchangeDiffMethodPc)
ON_CBN_SELCHANGE(IDC_ENCODE_METHOD_PC, &CTab_PC::OnCbnSelchangeEncodeMethodPc)
ON_BN_CLICKED(IDC_ENCODING_PC, &CTab_PC::OnBnClickedEncodingPc)
END_MESSAGE_MAP()

// CTab_PC message handlers
BOOL CTab_PC::PreTranslateMessage(MSG* pMsg)
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
BOOL CTab_PC::AutoTest()
{
	if (!m_bPC || !m_bConfig)
		return FALSE;
	m_bTest = TRUE;
	Dialog_Prompt *prompt = new Dialog_Prompt;
	if (IDOK == prompt->DoModal()) {
		int nRepeat = prompt->GetInputInteger();
		for (int i = 0; i < nRepeat; i++)
			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_GENERATE_PC, BN_CLICKED), 0L);
	}
	delete prompt;
	m_bTest = FALSE;
	return TRUE;
}
#endif

int CTab_PC::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pPointCloud = new ophPointCloud();

	return 0;
}


void CTab_PC::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//SetWindowPos(NULL, 0, 0, 353, 305, SWP_NOMOVE | SWP_NOREPOSITION);
}


void CTab_PC::OnBnClickedReadConfig_PC()
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
		if (ext == "xml") path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName();
		else return;
	}

	SetCurrentDirectory(current_path);

	TCHAR widepath[MAX_PATH] = { 0 };	
	char mulpath[MAX_PATH] = { 0 };

	_tcscpy_s(widepath, path.GetBuffer());
	WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);
	if (strcmp(mulpath, "") == 0) return;

	if (!m_pPointCloud->readConfig(mulpath)) {
		AfxMessageBox(TEXT("it is not xml config file for PointCloud."));
		return;
	}

	oph::vec3 scale;

	m_fieldLens = m_pPointCloud->getFieldLens();
	m_pPointCloud->getScale(scale);
	auto context = m_pPointCloud->getContext();
	m_scaleX = scale[_X];
	m_scaleY = scale[_Y];
	m_scaleZ = scale[_Z];
	m_offsetdepth = m_pPointCloud->getOffsetDepth();
	m_pixelpitchX = context.pixel_pitch[_X];
	m_pixelpitchY = context.pixel_pitch[_Y];
	m_pixelnumX = context.pixel_number[_X];
	m_pixelnumY = context.pixel_number[_Y];
	m_wavelength = *context.wave_length;

	m_bConfig = true;
	if (m_bPC) m_buttonGenerate.EnableWindow(TRUE);

	UpdateData(FALSE);
}


void CTab_PC::OnBnClickedLoadPc()
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
		if (ext == "ply") path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName();
		else return;
	}

	SetCurrentDirectory(current_path);

	TCHAR widepath[MAX_PATH] = { 0 };
	char mulpath[MAX_PATH] = { 0 };

	_tcscpy_s(widepath, path.GetBuffer());
	_tcscpy_s(m_argParamW, path.GetBuffer());
	WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);
	if (strcmp(mulpath, "") == 0) return;

	if (m_pPointCloud->loadPointCloud(mulpath) == -1) 
	{
		AfxMessageBox(TEXT("it is not ply file for PointCloud."));
		return;
	}

	strcpy_s(m_argParam, strlen(mulpath) + 1, mulpath);

	m_bPC = true;
	if (m_bConfig) m_buttonGenerate.EnableWindow(TRUE);

	GetDlgItem(IDC_VIEW_PC)->EnableWindow(TRUE);

	UpdateData(FALSE);
}


void CTab_PC::OnBnClickedViewPc()
{
	// TODO: Add your control notification handler code here
	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, sizeof(path));

	CString localPath = path;
	int i = localPath.ReverseFind('\\');
	localPath = localPath.Left(i);
	localPath.Append(L"\\3D_Object_Viewer.exe");
	_tcscpy_s(path, localPath.GetBuffer());

	TCHAR argParam[MAX_PATH * 3] = { 0 };

	int pc_flag = 0;

	CString szArgParam = CString("\"") + (m_argParam) + CString("\"");

	wsprintf(argParam, L"%d %s", pc_flag, szArgParam.GetBuffer());

	auto a = (int)::ShellExecute(NULL, _T("open"), 
		path,																								//실행 파일 경로
		argParam,																							//argument value 파라미터
		NULL, SW_SHOW);
}

UINT CallFunc(void* param)
{
	((ophPointCloud*)((parammeter*)param)->pGEN)->generateHologram(((parammeter*)param)->flag);

	*((parammeter*)param)->pFinish = TRUE;
	((parammeter*)param)->pDialog->m_bFinished = TRUE;
	//::SendMessage(((parammeter*)param)->pDialog->GetSafeHwnd(), WM_CLOSE, NULL, NULL);
	delete param;

	return 1;
}

void CTab_PC::OnBnClickedGenerate_PC()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_scaleX == 0.0 || m_scaleY == 0.0 || m_scaleZ == 0.0) {
		AfxMessageBox(TEXT("Config value error - scale"));
		return;
	}
	if (m_offsetdepth == 0.0) {
		AfxMessageBox(TEXT("Config value error - offset depth"));
		return;
	}	
	if (m_pixelpitchX == 0.0 || m_pixelpitchY == 0.0) {
		AfxMessageBox(TEXT("Config value error - pixel pitch"));
		return;
	}
	if (m_pixelnumX == 0 || m_pixelnumY == 0) {
		AfxMessageBox(TEXT("Config value error - pixel number"));
		return;
	}
	if (m_wavelength == 0.0) {
		AfxMessageBox(TEXT("Config value error - wave length"));
		return;
	}

	auto context = m_pPointCloud->getContext();
	m_pPointCloud->setOffsetDepth(m_offsetdepth);
	context.pixel_pitch[_X] = m_pixelpitchX;
	context.pixel_pitch[_Y] = m_pixelpitchY;
	context.pixel_number[_X] = m_pixelnumX;
	context.pixel_number[_Y] = m_pixelnumY;
	*context.wave_length = m_wavelength;
	m_pPointCloud->setScale(m_scaleX, m_scaleY, m_scaleZ);
	m_pPointCloud->setMode(!m_buttonGPU.GetCheck());
	m_pPointCloud->setViewingWindow(m_buttonViewingWindow.GetCheck());

	Dialog_Progress progress;

	parammeter *pParam = new parammeter;
	pParam->pGEN = m_pPointCloud;
	pParam->flag = m_idxDiff;
	pParam->pFinish = &m_bFinish;
	pParam->pDialog = &progress;

	CWinThread* pThread = AfxBeginThread(CallFunc, pParam);
	progress.DoModal();
	progress.DestroyWindow();

	//progress.GetDlgItem(IDC_TEXT_GEN);

	//while (!m_bFinish)
	//	progress.Progressing();

	GetDlgItem(IDC_ENCODING_PC)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVE_OHC_PC)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVE_BMP_PC)->EnableWindow(FALSE);

	UpdateData(FALSE);
}


void CTab_PC::OnBnClickedEncodingPc()
{
	// TODO: Add your control notification handler code here
/*	if (m_idxEncode == ophGen::ENCODE_SSB) {
		m_pPointCloud->encodeHologram();
		m_pPointCloud->normalize();
	}
	else */{
		auto depth = m_pPointCloud->getOffsetDepth();
		m_pPointCloud->waveCarry(0, 2, depth);
		switch (ophGen::ENCODE_FLAG(m_idxEncode)) {
		case ophGen::ENCODE_PHASE:
		case ophGen::ENCODE_AMPLITUDE:
		case ophGen::ENCODE_REAL:
		case ophGen::ENCODE_SIMPLENI:
		case ophGen::ENCODE_BURCKHARDT:
		case ophGen::ENCODE_TWOPHASE:
			((ophGen*)m_pPointCloud)->encoding(ophGen::ENCODE_FLAG(m_idxEncode));
			break;
		case ophGen::ENCODE_SSB:
		case ophGen::ENCODE_OFFSSB:
			m_pPointCloud->encoding(ophGen::ENCODE_FLAG(m_idxEncode), ophGen::SSB_TOP);
			break;
		}
		m_pPointCloud->normalizeEncoded();
	}

	m_buttonSaveBmp.EnableWindow(TRUE);
}


void CTab_PC::OnBnClickedSaveBmp_PC()
{
	// TODO: Add your control notification handler code here
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);

	LPTSTR szFilter = L"BMP File (*.bmp) |*.bmp|";

	CFileDialog FileDialog(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (ext == "bmp") path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName();
		else path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName() + L".bmp";
	}

	SetCurrentDirectory(current_path);

	TCHAR widepath[MAX_PATH] = { 0 };
	char mulpath[MAX_PATH] = { 0 };

	_tcscpy_s(widepath, path.GetBuffer());
	_tcscpy_s(m_resultPath, path.GetBuffer());
	WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);

	if (strcmp(mulpath, "") == 0) return;
	auto size = m_pPointCloud->getEncodeSize();
	m_pPointCloud->save(mulpath, 8, nullptr, size[_X], size[_Y]);

	GetDlgItem(IDC_VIEW_PC_BMP)->EnableWindow(TRUE);
}


void CTab_PC::OnBnClickedViewPcBmp()
{
	// TODO: Add your control notification handler code here
	Dialog_BMP_Viewer viewer;

	viewer.Init(m_resultPath, INIT_SINGLE);
	viewer.DoModal();

	viewer.DestroyWindow();
}


void CTab_PC::OnBnClickedSaveOhc_PC()
{
	// TODO: Add your control notification handler code here
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);

	LPTSTR szFilter = L"OHC File (*.ohc) |*.ohc|";

	CFileDialog FileDialog(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (ext == "ohc") path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName();
		else path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName() + L".ohc";
	}

	SetCurrentDirectory(current_path);

	TCHAR widepath[MAX_PATH] = { 0 };
	char mulpath[MAX_PATH] = { 0 };

	_tcscpy_s(widepath, path.GetBuffer());
	WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);

	if (strcmp(mulpath, "") == 0) return;
	if (m_pPointCloud->saveAsOhc(mulpath)) {

		//TCHAR strExecutable[FILENAME_MAX];
		//int result = (int)FindExecutable(widepath, NULL, (LPTSTR)&strExecutable);

		//if (result == 31) {
		//	SHELLEXECUTEINFO sei = { sizeof(sei), 0, m_hWnd, L"Openas",	widepath, NULL, NULL, SW_SHOWNORMAL, AfxGetApp()->m_hInstance };
		//	ShellExecuteEx(&sei);
		//}
		//else if (result == 32) {
		//	SHELLEXECUTEINFO sei = { sizeof(sei), 0, m_hWnd, L"Open", widepath, NULL, NULL,	SW_SHOWNORMAL, AfxGetApp()->m_hInstance };
		//	ShellExecuteEx(&sei);
		//}

		(int)::ShellExecute(NULL, _T("open"),
			widepath,																								//실행 파일 경로
			NULL,																							//argument value 파라미터
			NULL, SW_SHOW);
	}
}

BOOL CTab_PC::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	((CComboBox*)GetDlgItem(IDC_DIFF_METHOD_PC))->AddString(L"R-S diffaction");
	((CComboBox*)GetDlgItem(IDC_DIFF_METHOD_PC))->AddString(L"Fresnel diffaction");
	((CComboBox*)GetDlgItem(IDC_DIFF_METHOD_PC))->SetCurSel(m_idxDiff);

	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_PC))->AddString(L"Phase");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_PC))->AddString(L"Amplitude");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_PC))->AddString(L"Real");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_PC))->AddString(L"Simple NI");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_PC))->AddString(L"Burckhardt");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_PC))->AddString(L"Two-Phase");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_PC))->AddString(L"Single-Side Band");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_PC))->AddString(L"Off-SSB");

	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_PC))->SetCurSel(m_idxEncode);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CTab_PC::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	m_pPointCloud->release();
}

void CTab_PC::OnCbnSelchangeDiffMethodPc()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_idxDiff = ((CComboBox*)GetDlgItem(IDC_DIFF_METHOD_PC))->GetCurSel();
}


void CTab_PC::OnCbnSelchangeEncodeMethodPc()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_idxEncode = ((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_PC))->GetCurSel();
}
