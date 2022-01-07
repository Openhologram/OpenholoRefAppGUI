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
#define KEY_NAME L"DepthMap"
// CTab_DM dialog

IMPLEMENT_DYNAMIC(CTab_DM, CDialogEx)

CTab_DM::CTab_DM(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_DM, pParent)
	, m_nearDepth(0)
	, m_farDepth(0)
	, m_numDepth(0)
	, m_bConfig(false)
	, m_bDimg(false)
	, m_bRGBimg(false)
	, m_bEncode(false)
	, m_argParamDimg()
	, m_argParamRGBimg()
	, m_resultPath()
	, m_idxPropagation(0)
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
	DDX_Text(pDX, IDC_NEAR_DEPTH_DM, m_nearDepth);
	DDX_Text(pDX, IDC_FAR_DEPTH_DM, m_farDepth);
	DDX_Text(pDX, IDC_DEPTH_LEVEL_DM, m_numDepth);
}


BEGIN_MESSAGE_MAP(CTab_DM, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_READ_CONFIG_DM, &CTab_DM::OnBnClickedReadConfig_DM)
	ON_BN_CLICKED(IDC_LOAD_D_IMG_DM, &CTab_DM::OnBnClickedLoadDImg)
	ON_BN_CLICKED(IDC_LOAD_RGB_IMG_DM, &CTab_DM::OnBnClickedLoadRgbImg)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_VIEW_MODEL_DM, &CTab_DM::OnBnClickedViewDm)
	//ON_BN_CLICKED(IDC_VIEW_DM_BMP, &CTab_DM::OnBnClickedViewDmBmp)
	ON_BN_CLICKED(IDC_VIEW_IMG_DM, &CTab_DM::OnBnClickedViewDmImg)
	ON_CBN_SELCHANGE(IDC_PROPAGATION_METHOD_DM, &CTab_DM::OnCbnSelchangePropagationMethodDm)
	ON_MESSAGE(GENERATE, &CTab_DM::OnGenerate)
	ON_MESSAGE(ENCODE, &CTab_DM::OnEncode)
	ON_MESSAGE(SAVE_IMG, &CTab_DM::OnSaveIMG)
	ON_MESSAGE(SAVE_OHC, &CTab_DM::OnSaveOHC)
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
	TCHAR szCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurPath);

	LPTSTR szFilter = L"XML File (*.xml) |*.xml|";

	CFileDialog FileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString szCfgPath = AfxGetApp()->GetProfileString(KEY_NAME, L"Config Path", szCurPath);
	FileDialog.m_ofn.lpstrInitialDir = szCfgPath;
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (!ext.CompareNoCase(L"xml")) path = FileDialog.GetPathName();
		else return;
	}

	SetCurrentDirectory(szCurPath);
	if (!path.GetLength()) return;
	AfxGetApp()->WriteProfileString(KEY_NAME, L"Config Path", path.Left(path.ReverseFind('\\') + 1));

	if (!m_pDepthMap->readConfig(CW2A(path))) {
		AfxMessageBox(L"it is not xml config file for DepthMap.");
		return;
	}

	auto context = m_pDepthMap->getContext();
	auto imgCfg = m_pDepthMap->getImageConfig();
	//auto config = m_pDepthMap->getConfig();
	m_nearDepth = m_pDepthMap->getNearDepth();
	m_farDepth = m_pDepthMap->getFarDepth();
	m_numDepth = m_pDepthMap->getNumOfDepth();

	m_bConfig = true;
	//if (m_bDimg && m_bRGBimg) GetDlgItem(IDC_GENERATE_DM)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOAD_D_IMG_DM)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOAD_RGB_IMG_DM)->EnableWindow(TRUE);


	COpenholoRefAppDlg *pParent = (COpenholoRefAppDlg *)AfxGetMainWnd();
	pParent->m_buttonRandomPhase.SetCheck(m_pDepthMap->getConfig().random_phase);
	pParent->SetWaveNum(context.waveNum);
	pParent->SetWaveLength(context.wave_length);
	pParent->SetPixelNum(context.pixel_number[_X], context.pixel_number[_Y]);
	pParent->SetPixelPitch(context.pixel_pitch[_X], context.pixel_pitch[_Y]);
	pParent->SetShift(context.shift[_X], context.shift[_Y], context.shift[_Z]);
	pParent->SetImageRotate(imgCfg.bRotation);
	pParent->SetImageMerge(imgCfg.bMergeImage);
	pParent->SetImageFlip(imgCfg.nFlip);
	pParent->SendMessage(LOAD_CFG, LOAD_CFG, 0);

	UpdateData(FALSE);
}


void CTab_DM::OnBnClickedLoadDImg()
{
	// TODO: Add your control notification handler code here	
	TCHAR szCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurPath);

	LPTSTR szFilter = L"BMP File (*.bmp) |*.bmp|";

	CFileDialog FileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString szImgPath = AfxGetApp()->GetProfileString(KEY_NAME, L"Depth-Img Path", szCurPath);
	FileDialog.m_ofn.lpstrInitialDir = szImgPath;
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (!ext.CompareNoCase(L"bmp")) {
			path = FileDialog.GetPathName();
			m_szPath = FileDialog.GetFolderPath();
			m_szDname = FileDialog.GetFileTitle();
		}
		else return;
	}

	SetCurrentDirectory(szCurPath);
	if (!path.GetLength()) return;
	AfxGetApp()->WriteProfileString(KEY_NAME, L"Depth-Img Path", path.Left(path.ReverseFind('\\') + 1));
	_tcscpy_s(m_argParamDimg, path.GetBuffer());
	
	if (m_bRGBimg) {
		if (!m_pDepthMap->readImageDepth(CW2A(m_szPath), CW2A(m_szRGBname), CW2A(m_szDname))) {
			AfxMessageBox(L"BMP load failed : Please show LOG.");
			return;
		}
		GetDlgItem(IDC_VIEW_IMG_DM)->EnableWindow(TRUE);
		GetDlgItem(IDC_VIEW_MODEL_DM)->EnableWindow(TRUE);
	}
	m_bDimg = TRUE;

	if (m_bDimg && m_bRGBimg)
		AfxGetMainWnd()->SendMessage(LOAD_DATA, LOAD_DATA, 0);
}


void CTab_DM::OnBnClickedLoadRgbImg()
{
	// TODO: Add your control notification handler code here
	TCHAR szCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurPath);

	LPTSTR szFilter = L"BMP File (*.bmp) |*.bmp|";

	CFileDialog FileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString szImgPath = AfxGetApp()->GetProfileString(KEY_NAME, L"RGB-Img Path", szCurPath);
	FileDialog.m_ofn.lpstrInitialDir = szImgPath;
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (!ext.CompareNoCase(L"bmp")) {
			path = FileDialog.GetPathName();
			m_szPath = FileDialog.GetFolderPath();
			m_szRGBname = FileDialog.GetFileTitle();
		}
		else return;
	}

	SetCurrentDirectory(szCurPath);
	if (!path.GetLength()) return;
	AfxGetApp()->WriteProfileString(KEY_NAME, L"RGB-Img Path", path.Left(path.ReverseFind('\\') + 1));
	_tcscpy_s(m_argParamRGBimg, path.GetBuffer());

	if (m_bDimg) {
		if (!m_pDepthMap->readImageDepth(CW2A(m_szPath), CW2A(m_szRGBname), CW2A(m_szDname))) {
			AfxMessageBox(L"BMP load failed : Please show LOG.");
			return;
		}
		GetDlgItem(IDC_VIEW_IMG_DM)->EnableWindow(TRUE);
		GetDlgItem(IDC_VIEW_MODEL_DM)->EnableWindow(TRUE);
	}
	m_bRGBimg = TRUE;

	if (m_bDimg && m_bRGBimg)
		AfxGetMainWnd()->SendMessage(LOAD_DATA, LOAD_DATA, 0);
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

		::ShellExecute(NULL, _T("open"), path, argParam, NULL, SW_SHOW);
	}
	else {
		AfxMessageBox(localPath + L"을(를) 찾을 수 없습니다.");
	}
}

UINT CallFuncDM(void* param)
{
	parammeter *pParam = (parammeter *)param;
	((ophDepthMap*)pParam->pInst)->generateHologram();
	pParam->pDialog->m_bFinished = TRUE;

	ophDepthMap *pDepth = ((ophDepthMap *)pParam->pInst);
	Complex<Real> **pp = pDepth->getComplexField();
	
	Console::getInstance()->SetColor(Console::Color::YELLOW, Console::Color::BLACK);
	for (uint i = 0; i < pDepth->getContext().waveNum; i++) {
		printf("=> Complex Field[%d][0] = %.15e / %.15e \n", i, pp[i][0][_RE], pp[i][0][_IM]);
	}
	Console::getInstance()->ResetColor();
	delete pParam;

	return 1;
}

void CTab_DM::Generate()
{
	((COpenholoRefAppDlg *)AfxGetMainWnd())->ForegroundConsole();

	Dialog_Progress progress;

	BOOL bIsFinish = FALSE;

	parammeter *pParam = new parammeter;
	pParam->pInst = m_pDepthMap;
	pParam->pDialog = &progress;
	progress.m_bPercent = true;
	progress.m_iPercent = m_pDepthMap->getProgress();

	CWinThread* pThread = AfxBeginThread(CallFuncDM, pParam);
	progress.m_bGen = true;
	progress.DoModal();
	progress.DestroyWindow();
}
/*
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
	m_pDepthMap->setWaveLength(m_wavelength, 0);

	if (bChangedConfig) {
		m_pDepthMap->setPixelPitch(vec2(m_pixelpitchX, m_pixelpitchY));
		m_pDepthMap->setResolution(ivec2(m_pixelnumX, m_pixelnumY));
	}
	m_pDepthMap->setMode(!m_buttonGPU.GetCheck());
	m_pDepthMap->setViewingWindow(m_buttonViewingWindow.GetCheck());

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
	pParam->pInst = m_pDepthMap;
	pParam->pDialog = &progress;
	progress.m_bPercent = true;
	progress.m_iPercent = m_pDepthMap->getPercent();

	CWinThread* pThread = AfxBeginThread(CallFuncDM, pParam);
	progress.DoModal();
	progress.DestroyWindow();

	char szMsg[256] = { 0, };
	sprintf_s(szMsg, "Total Elapsed Time: %lf (s)\n", m_pDepthMap->getElapsedTime());
	((COpenholoRefAppDlg *)AfxGetMainWnd())->report(szMsg);

	//UpdateData(FALSE);
	MakeFileName();
}

void CTab_DM::OnBnClickedEncodingDm()
{
	// TODO: Add your control notification handler code here
	//if (m_idxEncode == ophGen::ENCODE_SSB) {
	//	m_pDepthMap->encodeHologram();
	//	m_pDepthMap->normalize();
	//}
	//else {
		//m_pDepthMap->waveCarry(0, 0.5);
		switch (ophGen::ENCODE_FLAG(m_idxEncode)) {
		case ophGen::ENCODE_PHASE:
		case ophGen::ENCODE_AMPLITUDE:
		case ophGen::ENCODE_REAL:
		case ophGen::ENCODE_SIMPLENI:
		case ophGen::ENCODE_BURCKHARDT:
		case ophGen::ENCODE_TWOPHASE:
			((ophGen*)m_pDepthMap)->encoding(ophGen::ENCODE_FLAG(m_idxEncode));
			break;
		case ophGen::ENCODE_SSB:
			m_pDepthMap->encoding(ophGen::ENCODE_FLAG(m_idxEncode), ophGen::SSB_TOP);
			break;
		case ophGen::ENCODE_OFFSSB:
			m_pDepthMap->encoding(ophGen::ENCODE_FLAG(m_idxEncode), ophGen::SSB_TOP);
			break;
		}
		m_pDepthMap->normalize();
	//}

	m_buttonSaveBmp.EnableWindow(TRUE);
	GetEncodeName(m_szEncodeName);
}
*/
void CTab_DM::MakeFileName(CString szAppend)
{
	if (szAppend.IsEmpty()) {
		m_szFileName.Empty();
	}

	m_szFileName = ((COpenholoRefAppDlg *)AfxGetMainWnd())->GetFileName();
	m_szFileName.AppendFormat(L"%dch_", m_pDepthMap->getContext().waveNum);
	m_szFileName.AppendFormat(L"%dx%d_", m_pDepthMap->getContext().pixel_number[_X], m_pDepthMap->getContext().pixel_number[_Y]);
	m_szFileName.AppendFormat(L"d%d_", m_pDepthMap->getNumOfDepth());
	m_szFileName.AppendFormat(L"%s_", m_idxPropagation == 0 ? L"AS" : L"Unknown");
}


void CTab_DM::OnBnClickedViewDmBmp()
{
	// TODO: Add your control notification handler code here
	Dialog_BMP_Viewer viewer;

	viewer.Init(m_resultPath, INIT_SINGLE);
	viewer.DoModal();

	viewer.DestroyWindow();
}

BOOL CTab_DM::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	((CComboBox*)GetDlgItem(IDC_PROPAGATION_METHOD_DM))->AddString(L"Angular Spectrum");
	((CComboBox*)GetDlgItem(IDC_PROPAGATION_METHOD_DM))->SetCurSel(m_idxPropagation);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CTab_DM::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: Add your message handler code here
	m_pDepthMap->release();
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
	double nearDepth	= m_nearDepth;
	double farDepth		= m_farDepth;
	int numDepth		= m_numDepth;
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

	return bChanged;
}

void CTab_DM::InitUI()
{
	GetDlgItem(IDC_READ_CONFIG_DM)->EnableWindow(TRUE);

	GetDlgItem(IDC_LOAD_D_IMG_DM)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOAD_RGB_IMG_DM)->EnableWindow(TRUE);
	m_bRGBimg = false;
	m_bDimg = false;
}


LRESULT CTab_DM::OnGenerate(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	bool bChangedConfig = CheckConfig();

	if (m_numDepth == 0) {
		AfxMessageBox(L"Config value error - number of depth");
		return FALSE;
	}

	OphDepthMapConfig config = m_pDepthMap->getConfig();
	config.default_depth_quantization = m_numDepth;
	config.far_depthmap = m_farDepth;
	config.near_depthmap = m_nearDepth;
	//config.fieldLength = ;
	config.change_depth_quantization = 1;
	config.num_of_depth_quantization = m_numDepth;
	config.num_of_depth = m_numDepth;
	config.random_phase = 0;
	m_pDepthMap->setConfig(config);

	int mode = (int)wParam;
	COpenholoRefAppDlg *dlg = (COpenholoRefAppDlg *)AfxGetMainWnd();
	m_pDepthMap->SetMode(mode);
	m_pDepthMap->setViewingWindow(dlg->UseVW());
	m_pDepthMap->SetRandomPhase(dlg->m_buttonRandomPhase.GetCheck());
	m_pDepthMap->SetMaxThreadNum(dlg->m_nCurThread);
	dlg->ForegroundConsole();

	Dialog_Progress progress;

	BOOL bIsFinish = FALSE;

	parammeter *pParam = new parammeter;
	pParam->pInst = m_pDepthMap;
	pParam->pDialog = &progress;
	progress.m_bPercent = true;
	progress.m_iPercent = m_pDepthMap->getProgress();

	CWinThread* pThread = AfxBeginThread(CallFuncDM, pParam);
	progress.m_bGen = true;
	progress.DoModal();
	progress.DestroyWindow();
	MakeFileName();
	m_bDimg = FALSE;
	m_bRGBimg = FALSE;
	return TRUE;
}

LRESULT CTab_DM::OnEncode(WPARAM wParam, LPARAM lParam)
{
	COpenholoRefAppDlg *pParent = (COpenholoRefAppDlg *)AfxGetMainWnd();
	Real shiftX = pParent->GetShiftX();
	Real shiftY = pParent->GetShiftY();
	m_pDepthMap->Shift(shiftX, shiftY);
	int idx = wParam;
	switch (ophGen::ENCODE_FLAG(idx)) {
	case ophGen::ENCODE_PHASE:
	case ophGen::ENCODE_AMPLITUDE:
	case ophGen::ENCODE_REAL:
	case ophGen::ENCODE_IMAGINEARY:
	case ophGen::ENCODE_SIMPLENI:
	case ophGen::ENCODE_BURCKHARDT:
	case ophGen::ENCODE_TWOPHASE:
		m_pDepthMap->encoding(ophGen::ENCODE_FLAG(idx));
		break;
	case ophGen::ENCODE_SSB:
	case ophGen::ENCODE_OFFSSB:
		m_pDepthMap->encoding(ophGen::ENCODE_FLAG(idx), (int)lParam);
		break;
	}

	m_pDepthMap->normalize();
	return TRUE;
}

LRESULT CTab_DM::OnSaveIMG(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your control notification handler code here
	COpenholoRefAppDlg *pParent = ((COpenholoRefAppDlg *)AfxGetMainWnd());

	TCHAR szCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurPath);

	CString path;
	pParent->SaveImage(path);

	SetCurrentDirectory(szCurPath);

	_tcscpy_s(m_resultPath, path.GetBuffer());

	if (!path.GetLength()) return FALSE;
	
	m_pDepthMap->setImageRotate(wParam & 0x2 ? true : false);
	m_pDepthMap->setImageMerge(wParam & 0x1 ? true : false);
	m_pDepthMap->setImageFlip((int)lParam);
	auto size = m_pDepthMap->getEncodeSize();
	int ch = m_pDepthMap->getContext().waveNum;
	m_pDepthMap->save(CW2A(path), 8 * ch, nullptr, size[_X], size[_Y]);

	pParent->OpenExplorer(path);
	return TRUE;
}


LRESULT CTab_DM::OnSaveOHC(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your control notification handler code here
	COpenholoRefAppDlg *pParent = ((COpenholoRefAppDlg *)AfxGetMainWnd());

	TCHAR szCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurPath);

	CString path;
	pParent->SaveOHC(path);

	SetCurrentDirectory(szCurPath);

	_tcscpy_s(m_resultPath, path.GetBuffer());

	if (!path.GetLength()) return FALSE;
	m_pDepthMap->saveAsOhc(CW2A(path));
	pParent->OpenExplorer(path);
	return TRUE;
}