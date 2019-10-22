// Tab_DM.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "OpenholoRefAppGUIDlg.h"
#include "Tab_DM.h"
#include "Console.h"
#include "afxdialogex.h"

#include <ophDepthMap.h>
#include "Dialog_BMP_Viewer.h"
#include "Dialog_Progress.h"
#include "Dialog_Prompt.h"

// CTab_DM dialog

IMPLEMENT_DYNAMIC(CTab_DM, CDialogEx)

CTab_DM::CTab_DM(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_DM, pParent)
	, m_fieldLens(0)
	, m_nearDepth(0)
	, m_farDepth(0)
	, m_numDepth(0)
	, m_pixelpitchX(0)
	, m_pixelpitchY(0)
	, m_pixelnumX(0)
	, m_pixelnumY(0)
	, m_wavelength(0)
	, m_bConfig(false)
	, m_bDimg(false)
	, m_bRGBimg(false)
	, m_bEncode(false)
	, m_argParamDimg()
	, m_argParamRGBimg()
	, m_resultPath()
	, m_idxEncode(6)
	, m_idxPropagation(1)
#ifdef TEST_MODE
	, m_bTest(FALSE)
#endif
{

}

CTab_DM::~CTab_DM()
{
}

void CTab_DM::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FIELD_LENS, m_fieldLens);
	DDX_Text(pDX, IDC_NEAR_DEPTH, m_nearDepth);
	DDX_Text(pDX, IDC_FAR_DEPTH, m_farDepth);
	DDX_Text(pDX, IDC_NUM_OF_DEPTH, m_numDepth);
	DDX_Text(pDX, IDC_PIXEL_PITCH_X, m_pixelpitchX);
	DDX_Text(pDX, IDC_PIXEL_PITCH_Y, m_pixelpitchY);
	DDX_Text(pDX, IDC_PIXEL_NUM_X, m_pixelnumX);
	DDX_Text(pDX, IDC_PIXEL_NUM_Y, m_pixelnumY);
	DDX_Text(pDX, IDC_WAVE_LENGTH, m_wavelength);
	DDX_Control(pDX, IDC_GPU_CHECK_DM, m_buttonGPU);
	DDX_Control(pDX, IDC_SAVE_BMP_DM, m_buttonSaveBmp);
	DDX_Control(pDX, IDC_SAVE_OHC_DM, m_buttonSaveOhc);
	DDX_Control(pDX, IDC_TRANSFORM_VW, m_buttonViewingWindow);
}


BEGIN_MESSAGE_MAP(CTab_DM, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_READ_CONFIG_DM, &CTab_DM::OnBnClickedReadConfig_DM)
	ON_BN_CLICKED(IDC_LOAD_D_IMG, &CTab_DM::OnBnClickedLoadDImg)
	ON_BN_CLICKED(IDC_LOAD_RGB_IMG, &CTab_DM::OnBnClickedLoadRgbImg)
	ON_BN_CLICKED(IDC_GENERATE_DM, &CTab_DM::OnBnClickedGenerate_DM)
	ON_BN_CLICKED(IDC_SAVE_BMP_DM, &CTab_DM::OnBnClickedSaveBmp_DM)
	ON_BN_CLICKED(IDC_SAVE_OHC_DM, &CTab_DM::OnBnClickedSaveOhc_DM)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_VIEW_DM, &CTab_DM::OnBnClickedViewDm)
	ON_BN_CLICKED(IDC_VIEW_DM_BMP, &CTab_DM::OnBnClickedViewDmBmp)
	ON_BN_CLICKED(IDC_VIEW_DM_IMG, &CTab_DM::OnBnClickedViewDmImg)
	ON_CBN_SELCHANGE(IDC_ENCODE_METHOD_DM, &CTab_DM::OnCbnSelchangeEncodeMethodDm)
	ON_CBN_SELCHANGE(IDC_PROPAGATION_METHOD_DM, &CTab_DM::OnCbnSelchangePropagationMethodDm)
	ON_BN_CLICKED(IDC_ENCODING_DM, &CTab_DM::OnBnClickedEncodingDm)
END_MESSAGE_MAP()


// CTab_DM message handlers
BOOL CTab_DM::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class	
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
#ifdef TEST_MODE
	if (!m_bTest && pMsg->wParam == VK_SPACE) {
		AutoTest();
		return TRUE;
	}
#endif
	return CDialogEx::PreTranslateMessage(pMsg);
}


#ifdef TEST_MODE
BOOL CTab_DM::AutoTest()
{
	if (!m_bDimg || !m_bRGBimg || !m_bConfig)
		return FALSE;
	m_bTest = TRUE;
	Dialog_Prompt *prompt = new Dialog_Prompt;
	if (IDOK == prompt->DoModal()) {
		int nRepeat = prompt->GetInputInteger();
		for (int i = 0; i < nRepeat; i++) {
			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_GENERATE_DM, BN_CLICKED), 0L);
			Console::getInstance()->SetColor(Console::Color::YELLOW, Console::Color::BLACK);
			printf("%d / %d\n", i + 1, nRepeat);
			Console::getInstance()->ResetColor();
		}
	}
	delete prompt;
	m_bTest = FALSE;
	return TRUE;
}
#endif

int CTab_DM::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pDepthMap = new ophDepthMap();

	return 0;
}


void CTab_DM::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//SetWindowPos(NULL, 0, 0, 353, 305, SWP_NOMOVE);
}


void CTab_DM::OnBnClickedReadConfig_DM()
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

	if (!m_pDepthMap->readConfig(mulpath)) {
		AfxMessageBox(L"it is not xml config file for DepthMap.");
		return;
	}

	auto context = m_pDepthMap->getContext();
	//auto config = m_pDepthMap->getConfig();

	m_fieldLens = m_pDepthMap->getFieldLens();
	m_nearDepth = m_pDepthMap->getNearDepth();
	m_farDepth = m_pDepthMap->getFarDepth();
	m_numDepth = m_pDepthMap->getNumOfDepth();
	m_pixelpitchX = context.pixel_pitch[_X];
	m_pixelpitchY = context.pixel_pitch[_Y];
	int pixX = context.pixel_number[_X];
	m_pixelnumX = context.pixel_number[_X];//pixX / 3;
	m_pixelnumY = context.pixel_number[_Y];
	m_wavelength = *context.wave_length;

	m_bConfig = true;
	//if (m_bDimg && m_bRGBimg) GetDlgItem(IDC_GENERATE_DM)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOAD_D_IMG)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOAD_RGB_IMG)->EnableWindow(TRUE);

	// 이미지를 로드하면, pixel num 변경을 금지.
	::SendMessage(GetDlgItem(IDC_PIXEL_NUM_X)->GetSafeHwnd(), EM_SETREADONLY, false, 0);
	::SendMessage(GetDlgItem(IDC_PIXEL_NUM_Y)->GetSafeHwnd(), EM_SETREADONLY, false, 0);
	UpdateData(FALSE);
}


void CTab_DM::OnBnClickedLoadDImg()
{
	// TODO: Add your control notification handler code here	
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);

	LPTSTR szFilter = L"BMP File (*.bmp) |*.bmp|";

	CFileDialog FileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (!ext.CompareNoCase(L"bmp")) {
			path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName();
			m_szPath = FileDialog.GetFolderPath();
			m_szDname = FileDialog.GetFileTitle();
		}
		else return;
	}

	SetCurrentDirectory(current_path);

	TCHAR widepath[MAX_PATH] = { 0 };
	char mulpath[MAX_PATH] = { 0 };

	_tcscpy_s(widepath, path.GetBuffer());
	_tcscpy_s(m_argParamDimg, path.GetBuffer());
	WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);
	if (strcmp(mulpath, "") == 0) return;

	if (m_bRGBimg) {
		_tcscpy_s(widepath, m_szPath.GetBuffer());
		WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);

		TCHAR wideDname[MAX_PATH] = { 0 };
		char mulDname[MAX_PATH] = { 0 };
		_tcscpy_s(wideDname, m_szDname.GetBuffer());
		WideCharToMultiByte(CP_ACP, 0, wideDname, MAX_PATH, mulDname, MAX_PATH, NULL, NULL);

		TCHAR wideRGBname[MAX_PATH] = { 0 };
		char mulRGBname[MAX_PATH] = { 0 };
		_tcscpy_s(wideRGBname, m_szRGBname.GetBuffer());
		WideCharToMultiByte(CP_ACP, 0, wideRGBname, MAX_PATH, mulRGBname, MAX_PATH, NULL, NULL);

		if (!m_pDepthMap->readImageDepth(mulpath, mulRGBname, mulDname)) {
			AfxMessageBox(L"BMP load failed : Please show LOG.");
			return;
		}
		GetDlgItem(IDC_VIEW_DM_IMG)->EnableWindow(TRUE);
		GetDlgItem(IDC_VIEW_DM_MODEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_GENERATE_DM)->EnableWindow(TRUE);
	}
	m_bDimg = TRUE;

	// 이미지를 로드하면, pixel num 변경을 금지.
	::SendMessage(GetDlgItem(IDC_PIXEL_NUM_X)->GetSafeHwnd(), EM_SETREADONLY, true, 0);
	::SendMessage(GetDlgItem(IDC_PIXEL_NUM_Y)->GetSafeHwnd(), EM_SETREADONLY, true, 0);
}


void CTab_DM::OnBnClickedLoadRgbImg()
{
	// TODO: Add your control notification handler code here
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);

	LPTSTR szFilter = L"BMP File (*.bmp) |*.bmp|";

	CFileDialog FileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (!ext.CompareNoCase(L"bmp")) {
			path = FileDialog.GetFolderPath() + L"\\" + FileDialog.GetFileName();
			m_szPath = FileDialog.GetFolderPath();
			m_szRGBname = FileDialog.GetFileTitle();
		}
		else return;
	}

	SetCurrentDirectory(current_path);

	TCHAR widepath[MAX_PATH] = { 0 };
	char mulpath[MAX_PATH] = { 0 };

	_tcscpy_s(widepath, path.GetBuffer());
	_tcscpy_s(m_argParamRGBimg, path.GetBuffer());
	WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);
	if (strcmp(mulpath, "") == 0) return;

	if (m_bDimg) {
		_tcscpy_s(widepath, m_szPath.GetBuffer());
		WideCharToMultiByte(CP_ACP, 0, widepath, MAX_PATH, mulpath, MAX_PATH, NULL, NULL);

		TCHAR wideDname[MAX_PATH] = { 0 };
		char mulDname[MAX_PATH] = { 0 };
		_tcscpy_s(wideDname, m_szDname.GetBuffer());
		WideCharToMultiByte(CP_ACP, 0, wideDname, MAX_PATH, mulDname, MAX_PATH, NULL, NULL);

		TCHAR wideRGBname[MAX_PATH] = { 0 };
		char mulRGBname[MAX_PATH] = { 0 };
		_tcscpy_s(wideRGBname, m_szRGBname.GetBuffer());
		WideCharToMultiByte(CP_ACP, 0, wideRGBname, MAX_PATH, mulRGBname, MAX_PATH, NULL, NULL);


		if (!m_pDepthMap->readImageDepth(mulpath, mulRGBname, mulDname)) {
			AfxMessageBox(L"BMP load failed : Please show LOG.");
			return;
		}
		GetDlgItem(IDC_VIEW_DM_IMG)->EnableWindow(TRUE);
		GetDlgItem(IDC_VIEW_DM_MODEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_GENERATE_DM)->EnableWindow(TRUE);
	}
	m_bRGBimg = TRUE;
	// 이미지를 로드하면, pixel num 변경을 금지.
	::SendMessage(GetDlgItem(IDC_PIXEL_NUM_X)->GetSafeHwnd(), EM_SETREADONLY, true, 0);
	::SendMessage(GetDlgItem(IDC_PIXEL_NUM_Y)->GetSafeHwnd(), EM_SETREADONLY, true, 0);
}

void CTab_DM::OnBnClickedViewDmImg()
{
	// TODO: Add your control notification handler code here
	Dialog_BMP_Viewer viewer;

	std::vector<CString> imagePathArr;
	imagePathArr.push_back(m_argParamDimg);
	imagePathArr.push_back(m_argParamRGBimg);

	viewer.Init(imagePathArr, INIT_ARR);
	viewer.DoModal();

	viewer.DestroyWindow();
}

void CTab_DM::OnBnClickedViewDm()
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

		CString szArgParamD = CString("\"") + (m_argParamDimg)+CString("\"");
		CString szArgParamRGB = CString("\"") + (m_argParamRGBimg)+CString("\"");

		wsprintf(argParam, L"%d %s %s", 1, szArgParamD.GetBuffer(), szArgParamRGB.GetBuffer());

		auto a = (int)::ShellExecute(NULL, _T("open"),
			path,																								//실행 파일 경로
			argParam,																							//argument value 파라미터
			NULL, SW_SHOW);
	}
	else {
		AfxMessageBox(localPath + L"을(를) 찾을 수 없습니다.");
	}
}

UINT CallFuncDM(void* param)
{
	parammeter *pParam = (parammeter *)param;
	((ophDepthMap*)pParam->pGEN)->generateHologram();
	pParam->pDialog->m_bFinished = TRUE;

	Complex<Real> **pp = ((ophDepthMap *)pParam->pGEN)->getComplexField();
	Console::getInstance()->SetColor(Console::Color::YELLOW, Console::Color::BLACK);
	printf("=> Complex Field[0] = %lf / %lf\n", (*pp)[0][_RE], (*pp)[0][_IM]);
	Console::getInstance()->ResetColor();
	delete pParam;

	return 1;
}

void CTab_DM::OnBnClickedGenerate_DM()
{
	// TODO: Add your control notification handler code here
	bool bChangedConfig = CheckConfig();

	if (m_buttonViewingWindow.GetCheck() && m_fieldLens == 0.0) {
		AfxMessageBox(L"Config value error - field lens");
		return;
	}
	if (m_numDepth == 0) {
		AfxMessageBox(L"Config value error - number of depth");
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
	
	OphDepthMapConfig config = m_pDepthMap->getConfig();
	config.DEFAULT_DEPTH_QUANTIZATION = m_numDepth;
	config.far_depthmap = m_farDepth;
	config.near_depthmap = m_nearDepth;
	config.fieldLength = m_fieldLens;
	config.FLAG_CHANGE_DEPTH_QUANTIZATION = 1;
	config.NUMBER_OF_DEPTH_QUANTIZATION = m_numDepth;
	config.num_of_depth = m_numDepth;
	config.RANDOM_PHASE = 0;
	m_pDepthMap->setConfig(config);
#ifndef USE_3CHANNEL
	m_pDepthMap->setWaveLength(m_wavelength, 0);
#else
#endif
	//m_pDepthMap->setPixelPitch(vec2(m_pixelpitchX, m_pixelpitchY));
	m_pDepthMap->setResolution(ivec2(m_pixelnumX, m_pixelnumY));
	m_pDepthMap->setMode(!m_buttonGPU.GetCheck());
	m_pDepthMap->setViewingWindow(m_buttonViewingWindow.GetCheck());
	m_pDepthMap->setPropagationMethod(m_idxPropagation);

	ivec2 rgbImg = m_pDepthMap->getRGBImgSize();
	ivec2 depthImg = m_pDepthMap->getDepthImgSize();

	// 불러온 이미지 해상도와 해상도 설정값을 비교
	if ((rgbImg[_X] != m_pixelnumX || rgbImg[_Y] != m_pixelnumY) ||
		(depthImg[_X] != m_pixelnumX || depthImg[_Y] != m_pixelnumY)) {
		AfxMessageBox(L"Value differs between Image Size and Config value.");
		InitUI();
		return;
	}

	GetDlgItem(IDC_SAVE_OHC_DM)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVE_BMP_DM)->EnableWindow(FALSE);
	GetDlgItem(IDC_ENCODING_DM)->EnableWindow(TRUE);

	((COpenholoRefAppDlg *)AfxGetMainWnd())->ForegroundConsole();

	Dialog_Progress progress;

	BOOL bIsFinish = FALSE;

	parammeter *pParam = new parammeter;
	pParam->pGEN = m_pDepthMap;
	pParam->pDialog = &progress;

	CWinThread* pThread = AfxBeginThread(CallFuncDM, pParam);
	progress.DoModal();
	progress.DestroyWindow();

	char szMsg[256] = { 0, };
	sprintf_s(szMsg, "Total Elapsed Time: %lf (s)\n", m_pDepthMap->getElapsedTime());
	((COpenholoRefAppDlg *)AfxGetMainWnd())->report(szMsg);

	//UpdateData(FALSE);
}

void CTab_DM::OnBnClickedEncodingDm()
{
	// TODO: Add your control notification handler code here
	if (m_idxEncode == ophGen::ENCODE_SSB) {
		m_pDepthMap->encodeHologram();
		m_pDepthMap->normalize();
	}
	else {
		//m_pDepthMap->waveCarry(0, 0.5);
		switch (ophGen::ENCODE_FLAG(m_idxEncode)) {
		case ophGen::ENCODE_PHASE:
		case ophGen::ENCODE_AMPLITUDE:
		case ophGen::ENCODE_REAL:
		case ophGen::ENCODE_SIMPLENI:
		case ophGen::ENCODE_BURCKHARDT:
		case ophGen::ENCODE_TWOPHASE:
		case ophGen::ENCODE_SYMMETRIZATION:
			m_pDepthMap->encoding(ophGen::ENCODE_FLAG(m_idxEncode));
			break;
		case ophGen::ENCODE_SSB:
			m_pDepthMap->encoding(ophGen::ENCODE_FLAG(m_idxEncode), ophGen::SSB_TOP);
			break;
		case ophGen::ENCODE_OFFSSB:
			m_pDepthMap->encoding(ophGen::ENCODE_FLAG(m_idxEncode), ophGen::SSB_TOP);
			break;
		}
		m_pDepthMap->normalizeEncoded();
	}

	m_buttonSaveBmp.EnableWindow(TRUE);
}

void CTab_DM::OnBnClickedSaveBmp_DM()
{
	// TODO: Add your control notification handler code here
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);

	LPTSTR szFilter = L"BMP File (*.bmp) |*.bmp|";
	CFileDialog FileDialog(FALSE, NULL, Time::getInstance()->GetTime(L"DepthMap"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
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
	m_pDepthMap->save(mulpath, 8);

	GetDlgItem(IDC_VIEW_DM_BMP)->EnableWindow(TRUE);
}

void CTab_DM::OnBnClickedViewDmBmp()
{
	// TODO: Add your control notification handler code here
	Dialog_BMP_Viewer viewer;

	viewer.Init(m_resultPath, INIT_SINGLE);
	viewer.DoModal();

	viewer.DestroyWindow();
}


void CTab_DM::OnBnClickedSaveOhc_DM()
{
	// TODO: Add your control notification handler code here
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);

	LPTSTR szFilter = L"OHC File (*.ohc) |*.ohc|";
	
	CFileDialog FileDialog(FALSE, NULL, Time::getInstance()->GetTime(L"DepthMap"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
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
	if (m_pDepthMap->saveAsOhc(mulpath)) {
	}
}


BOOL CTab_DM::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_DM))->AddString(L"Phase");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_DM))->AddString(L"Amplitude");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_DM))->AddString(L"Real");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_DM))->AddString(L"Simple NI");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_DM))->AddString(L"Burckhardt");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_DM))->AddString(L"Two-Phase");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_DM))->AddString(L"Single-Side Band");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_DM))->AddString(L"Off-SSB");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_DM))->AddString(L"Symmetrization");

	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_DM))->SetCurSel(m_idxEncode);

	((CComboBox*)GetDlgItem(IDC_PROPAGATION_METHOD_DM))->AddString(L"None");
	((CComboBox*)GetDlgItem(IDC_PROPAGATION_METHOD_DM))->AddString(L"Angular Spectrum");

	((CComboBox*)GetDlgItem(IDC_PROPAGATION_METHOD_DM))->SetCurSel(m_idxPropagation);

	// GeForce GPU 일 때만, 활성화
	COpenholoRefAppDlg *pDlg = (COpenholoRefAppDlg *)AfxGetApp()->GetMainWnd();
	((CButton*)GetDlgItem(IDC_GPU_CHECK_DM))->EnableWindow(pDlg->IsGeforceGPU());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CTab_DM::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	m_pDepthMap->release();
}

void CTab_DM::OnCbnSelchangeEncodeMethodDm()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_idxEncode = ((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_DM))->GetCurSel();
}

void CTab_DM::OnCbnSelchangePropagationMethodDm()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_idxPropagation = ((CComboBox*)GetDlgItem(IDC_PROPAGATION_METHOD_DM))->GetCurSel();
}



bool CTab_DM::CheckConfig()
{
	bool bChanged		= false;
	double fieldLens	= m_fieldLens;
	double nearDepth	= m_nearDepth;
	double farDepth		= m_farDepth;
	int numDepth		= m_numDepth;
	double ppX			= m_pixelpitchX;
	double ppY			= m_pixelpitchY;
	unsigned int pnX	= m_pixelnumX;
	unsigned int pnY	= m_pixelnumY;
	double wavelength	= m_wavelength;
	UpdateData(TRUE);

	if (nearDepth != m_nearDepth || farDepth != m_farDepth) {
		printf("\n*Changed Depth Range*\nNear Depth: %lf -> %lf\nFar Depth: %lf -> %lf\n",
			nearDepth, m_nearDepth, farDepth, m_farDepth);
		bChanged = true;
	}
	if (numDepth != m_numDepth) {
		printf("\n*Changed Num of Depth*\nNum Depth: %d -> %d\n",
			numDepth, m_numDepth);
		bChanged = true;
	}
	if (ppX != m_pixelpitchX || ppY != m_pixelpitchY) {
		printf("\n*Changed Pixel Pitch*\nPixelPitchX: %lf -> %lf\nPixelPitchY: %lf -> %lf\n",
			ppX, m_pixelpitchX, ppY, m_pixelpitchY);
		bChanged = true;
	}
	if (pnX != m_pixelnumX || pnY != m_pixelnumY) {
		printf("\n*Changed Pixel Num*\nPixelNumX: %u -> %u\nPixelNumY: %u -> %u\n",
			pnX, m_pixelnumX, pnY, m_pixelnumY);
		bChanged = true;
	}
	if (fieldLens != m_fieldLens) {
		printf("\n*Changed Field Length*\nField Length: %lf -> %lf\n",
			fieldLens, m_fieldLens);
		bChanged = true;
	}
	if (wavelength != m_wavelength) {
		printf("\n*Changed Wave Length*\nWave Length: %lf -> %lf\n",
			wavelength, m_wavelength);
		bChanged = true;
	}

	return bChanged;
}

void CTab_DM::InitUI()
{
	GetDlgItem(IDC_READ_CONFIG_DM)->EnableWindow(TRUE);

	GetDlgItem(IDC_LOAD_D_IMG)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOAD_RGB_IMG)->EnableWindow(TRUE);
	m_bRGBimg = false;
	m_bDimg = false;

	GetDlgItem(IDC_GENERATE_DM)->EnableWindow(FALSE);
	GetDlgItem(IDC_ENCODING_DM)->EnableWindow(FALSE);
	GetDlgItem(IDC_SAVE_BMP_DM)->EnableWindow(FALSE);
	GetDlgItem(IDC_SAVE_OHC_DM)->EnableWindow(FALSE);
}