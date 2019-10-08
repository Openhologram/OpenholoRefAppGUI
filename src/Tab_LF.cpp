// Tab_LF.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "OpenholoRefAppGUIDlg.h"
#include "Tab_LF.h"
#include "afxdialogex.h"

// CTab_LF dialog

#include <ophLightField.h>
#include "Dialog_BMP_Viewer.h"
#include "Dialog_Progress.h"
#include "Dialog_Prompt.h"

IMPLEMENT_DYNAMIC(CTab_LF, CDialogEx)

CTab_LF::CTab_LF(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LF, pParent)
	, m_fieldLens(0)
	, m_distance(0)
	, m_numimgX(0)
	, m_numimgY(0)
	, m_pixelpitchX(0)
	, m_pixelpitchY(0)
	, m_pixelnumX(0)
	, m_pixelnumY(0)
	, m_wavelength(0)
	, m_argParam()
	, m_resultPath()
	, m_bConfig(FALSE)
	, m_bDir(FALSE)
	, m_bEncode(FALSE)
	, m_idxEncode(3)
#ifdef TEST_MODE
	, m_bTest(FALSE)
#endif
{

}

CTab_LF::~CTab_LF()
{
}

void CTab_LF::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FIELD_LENS, m_fieldLens);
	DDX_Text(pDX, IDC_SCALE_X, m_distance);
	DDX_Text(pDX, IDC_SCALE_Y, m_numimgX);
	DDX_Text(pDX, IDC_SCALE_Z, m_numimgY);
	DDX_Text(pDX, IDC_PIXEL_PITCH_X, m_pixelpitchX);
	DDX_Text(pDX, IDC_PIXEL_PITCH_Y, m_pixelpitchY);
	DDX_Text(pDX, IDC_PIXEL_NUM_X, m_pixelnumX);
	DDX_Text(pDX, IDC_PIXEL_NUM_Y, m_pixelnumY);
	DDX_Text(pDX, IDC_WAVE_LENGTH, m_wavelength);
	DDX_Control(pDX, IDC_GPU_CHECK_LF, m_buttonGPU);
	DDX_Control(pDX, IDC_GENERATE_LF, m_buttonGenerate);
	DDX_Control(pDX, IDC_SAVE_BMP_LF, m_buttonSaveBmp);
	DDX_Control(pDX, IDC_SAVE_OHC_LF, m_buttonSaveOhc);
	DDX_Control(pDX, IDC_TRANSFORM_VW, m_buttonViewingWindow);
}


BEGIN_MESSAGE_MAP(CTab_LF, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_READ_CONFIG_LF, &CTab_LF::OnBnClickedReadConfig_LF)
	ON_BN_CLICKED(IDC_FIND_DIR, &CTab_LF::OnBnClickedFindDir)
	ON_BN_CLICKED(IDC_GENERATE_LF, &CTab_LF::OnBnClickedGenerate_LF)
	ON_BN_CLICKED(IDC_SAVE_BMP_LF, &CTab_LF::OnBnClickedSaveBmp_LF)
	ON_BN_CLICKED(IDC_SAVE_OHC_LF, &CTab_LF::OnBnClickedSaveOhc_LF)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_VIEW_LF, &CTab_LF::OnBnClickedViewLf)
	ON_BN_CLICKED(IDC_VIEW_LF_BMP, &CTab_LF::OnBnClickedViewLfBmp)
	ON_CBN_SELCHANGE(IDC_ENCODE_METHOD_LF, &CTab_LF::OnCbnSelchangeEncodeMethodLf)
	ON_BN_CLICKED(IDC_ENCODING_LF, &CTab_LF::OnBnClickedEncodingLf)
END_MESSAGE_MAP()


// CTab_LF message handlers
BOOL CTab_LF::PreTranslateMessage(MSG* pMsg)
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
BOOL CTab_LF::AutoTest()
{
	if (!m_bDir || !m_bConfig)
		return FALSE;
	m_bTest = TRUE;
	Dialog_Prompt *prompt = new Dialog_Prompt;
	if (IDOK == prompt->DoModal()) {
		int nRepeat = prompt->GetInputInteger();
		for (int i = 0; i < nRepeat; i++)
			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_GENERATE_LF, BN_CLICKED), 0L);
	}
	delete prompt;
	m_bTest = FALSE;
	return TRUE;
}
#endif

int CTab_LF::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pLightField = new ophLF();

	return 0;
}


void CTab_LF::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//SetWindowPos(NULL, 0, 0, 353, 305, SWP_NOMOVE);
}


void CTab_LF::OnBnClickedReadConfig_LF()
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

	if (!m_pLightField->readLFConfig(mulpath)) {
		AfxMessageBox(L"it is not xml config file for LightField.");
		return;
	}

	m_fieldLens = m_pLightField->getFieldLens();
	m_distance = m_pLightField->getDistRS2Holo();
	m_numimgX = m_pLightField->getNumImage()[_X];
	m_numimgY = m_pLightField->getNumImage()[_Y];
	m_pixelpitchX = m_pLightField->getContext().pixel_pitch[_X];
	m_pixelpitchY = m_pLightField->getContext().pixel_pitch[_Y];
	m_pixelnumX = m_pLightField->getContext().pixel_number[_X];
	m_pixelnumY = m_pLightField->getContext().pixel_number[_Y];
	m_wavelength = m_pLightField->getContext().wave_length[0];

	m_bConfig = true;
	GetDlgItem(IDC_FIND_DIR)->EnableWindow(TRUE);

	UpdateData(FALSE);
}


void CTab_LF::OnBnClickedFindDir()
{
	// TODO: Add your control notification handler code here
	TCHAR szSelPath[MAX_PATH] = { 0, };
	TCHAR szCurPath[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, szCurPath);

	CFolderPickerDialog dlg(NULL, OFN_FILEMUSTEXIST, NULL, 0);
	if (dlg.DoModal() == IDOK) {
		wsprintf(szSelPath, L"%s", dlg.GetPathName());
	}
	else
		return;

	SetCurrentDirectory(szCurPath);
	CStringA szPath = CW2A(szSelPath);
	if (szPath.IsEmpty()) return;
	if (m_pLightField->loadLF(szPath, "bmp") == -1) {
		AfxMessageBox(L"Load LF image failed");
		return;
	}

	m_bDir = TRUE;
	if (m_bConfig) m_buttonGenerate.EnableWindow(TRUE);
	GetDlgItem(IDC_VIEW_LF)->EnableWindow(TRUE);
}


void CTab_LF::OnBnClickedViewLf()
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

		int mesh_flag = 2;

		CString szArgParam = CString("\"") + (m_argParam)+CString("\"");

		wsprintf(argParam, L"%d %s", mesh_flag, szArgParam.GetBuffer());

		auto a = (int)::ShellExecute(NULL, _T("open"),
			path,																								//실행 파일 경로
			argParam,																							//argument value 파라미터
			NULL, SW_SHOW);
	}
	else {
		AfxMessageBox(localPath + L"을(를) 찾을 수 없습니다.");
	}

}

UINT CallFuncLF(void* param)
{
	parammeter *pParam = (parammeter *)param;
	((ophLF*)pParam->pGEN)->generateHologram();
	pParam->pDialog->m_bFinished = TRUE;
	delete pParam;

	return 1;
}

void CTab_LF::OnBnClickedGenerate_LF()
{
	// TODO: Add your control notification handler code here	
	UpdateData(TRUE);
	if (m_buttonViewingWindow.GetCheck() && m_fieldLens == 0.0) {
		AfxMessageBox(L"Config value error - field lens");
		return;
	}
	if (m_distance == 0.0) {
		AfxMessageBox(L"Config value error - distance RS to Hologram");
		return;
	}
	if (m_numimgX == 0 || m_numimgY == 0) {
		AfxMessageBox(L"Config value error - number of images of LF");
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
	m_pLightField->setDistRS2Holo(m_distance);
	m_pLightField->setNumImage(m_numimgX, m_numimgY);
	m_pLightField->setPixelPitch(vec2(m_pixelpitchX, m_pixelpitchY));
	m_pLightField->setPixelNumber(ivec2(m_pixelnumX, m_pixelnumY));
	m_pLightField->setWaveLength(m_wavelength, 0);
	m_pLightField->setMode(!m_buttonGPU.GetCheck());
	m_pLightField->setViewingWindow(m_buttonViewingWindow.GetCheck());


	GetDlgItem(IDC_SAVE_OHC_LF)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVE_BMP_LF)->EnableWindow(FALSE);
	GetDlgItem(IDC_ENCODING_LF)->EnableWindow(TRUE);

	Dialog_Progress progress;

	BOOL bIsFinish = FALSE;

	parammeter *pParam = new parammeter;
	pParam->pGEN = m_pLightField;
	pParam->pDialog = &progress;

	CWinThread* pThread = AfxBeginThread(CallFuncLF, pParam);
	progress.DoModal();
	progress.DestroyWindow();

	UpdateData(FALSE);
}

void CTab_LF::OnBnClickedEncodingLf()
{
	// TODO: Add your control notification handler code here
	auto dist = m_pLightField->getDistRS2Holo();
	m_pLightField->waveCarry(0, 0.1, dist);
	switch (ophGen::ENCODE_FLAG(m_idxEncode)) {
	case ophGen::ENCODE_PHASE:
	case ophGen::ENCODE_AMPLITUDE:
	case ophGen::ENCODE_REAL:
	case ophGen::ENCODE_SIMPLENI:
	case ophGen::ENCODE_BURCKHARDT:
	case ophGen::ENCODE_TWOPHASE:
		m_pLightField->encoding(ophGen::ENCODE_FLAG(m_idxEncode));
		break;
	case ophGen::ENCODE_SSB:
	case ophGen::ENCODE_OFFSSB:
		m_pLightField->encoding(ophGen::ENCODE_FLAG(m_idxEncode), ophGen::SSB_TOP);
		break;
	}
	m_pLightField->normalizeEncoded();

	m_buttonSaveBmp.EnableWindow(TRUE);
}


void CTab_LF::OnBnClickedSaveBmp_LF()
{
	// TODO: Add your control notification handler code here
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);

	LPTSTR szFilter = L"BMP File (*.bmp) |*.bmp|";

	
	CFileDialog FileDialog(FALSE, NULL, Time::GetTime(L"LightField"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (!ext.CompareNoCase(L"bmp")) path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName();
		else path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName() + L".bmp";
	}

	SetCurrentDirectory(current_path);

	TCHAR widepath[MAX_PATH] = { 0 };
	char mulpath[MAX_PATH] = { 0 };

	_tcscpy_s(widepath, path.GetBuffer());
	_tcscpy_s(m_resultPath, path.GetBuffer());
	WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);

	if (strcmp(mulpath, "") == 0) return;

	ivec2 encodeSize = m_pLightField->getEncodeSize();
	m_pLightField->save(mulpath, 8, nullptr, encodeSize[_X], encodeSize[_Y]);

	GetDlgItem(IDC_VIEW_LF_BMP)->EnableWindow(TRUE);
}

void CTab_LF::OnBnClickedViewLfBmp()
{
	// TODO: Add your control notification handler code here
	Dialog_BMP_Viewer viewer;

	viewer.Init(m_resultPath, INIT_SINGLE);
	viewer.DoModal();

	viewer.DestroyWindow();
}

 
void CTab_LF::OnBnClickedSaveOhc_LF()
{
	// TODO: Add your control notification handler code here
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);

	LPTSTR szFilter = L"OHC File (*.ohc) |*.ohc|";
	
	CFileDialog FileDialog(FALSE, NULL, Time::GetTime(L"LightField"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
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
	if (m_pLightField->saveAsOhc(mulpath)) {
	}
}

BOOL CTab_LF::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_LF))->AddString(L"Phase");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_LF))->AddString(L"Amplitude");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_LF))->AddString(L"Real");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_LF))->AddString(L"Simple NI");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_LF))->AddString(L"Burckhardt");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_LF))->AddString(L"Two-Phase");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_LF))->AddString(L"Single-Side Band");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_LF))->AddString(L"Off-SSB");

	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_LF))->SetCurSel(m_idxEncode);

	// GeForce GPU 일 때만, 활성화
	COpenholoRefAppDlg *pDlg = (COpenholoRefAppDlg *)AfxGetApp()->GetMainWnd();
	((CButton*)GetDlgItem(IDC_GPU_CHECK_LF))->EnableWindow(pDlg->IsGeforceGPU());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CTab_LF::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	m_pLightField->release();
}

void CTab_LF::OnCbnSelchangeEncodeMethodLf()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_idxEncode = ((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_LF))->GetCurSel();
}
