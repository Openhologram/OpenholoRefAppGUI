// Tab_IFTA.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "OpenholoRefAppGUIDlg.h"
#include "Tab_IFTA.h"
#include "Console.h"
#include "afxdialogex.h"

#include <ophIFTA.h>
#include "Dialog_Progress.h"
#include "Dialog_Prompt.h"

#define KEY_NAME L"IFTA"
// CTab_IFTA dialog

IMPLEMENT_DYNAMIC(CTab_IFTA, CDialogEx)

CTab_IFTA::CTab_IFTA(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_IFTA, pParent)
	, m_bConfig(false)
	, m_bDimg(false)
	, m_bRGBimg(false)
	, m_bEncode(false)
	, m_argParamDimg()
	, m_argParamRGBimg()
	, m_resultPath()
	, m_nDepth(0)
	, m_nIteration(0)
	, m_nearDepth(0)
	, m_farDepth(0)
#ifdef TEST_MODE
	, m_bTest(FALSE)
#endif
{

}

CTab_IFTA::~CTab_IFTA()
{
}

void CTab_IFTA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NEAR_DEPTH_IFTA, m_nearDepth);
	DDX_Text(pDX, IDC_FAR_DEPTH_IFTA, m_farDepth);
	DDX_Text(pDX, IDC_ITERATION, m_nIteration);
	DDX_Text(pDX, IDC_DEPTH_LEVEL_IFTA, m_nDepth);
}


BEGIN_MESSAGE_MAP(CTab_IFTA, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_READ_CONFIG_IFTA, &CTab_IFTA::OnBnClickedReadConfig_Ifta)
	ON_BN_CLICKED(IDC_LOAD_RGB_IMG_IFTA, &CTab_IFTA::OnBnClickedLoadRGBImg)
	ON_BN_CLICKED(IDC_LOAD_D_IMG_IFTA, &CTab_IFTA::OnBnClickedLoadDepthImg)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(GENERATE, &CTab_IFTA::OnGenerate)
	ON_MESSAGE(ENCODE, &CTab_IFTA::OnEncode)
	ON_MESSAGE(SAVE_IMG, &CTab_IFTA::OnSaveIMG)
	ON_MESSAGE(SAVE_OHC, &CTab_IFTA::OnSaveOHC)
END_MESSAGE_MAP()


// CTab_IFTA message handlers
BOOL CTab_IFTA::PreTranslateMessage(MSG* pMsg)
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
BOOL CTab_IFTA::AutoTest()
{
	if (!m_bDimg || !m_bRGBimg || !m_bConfig)
		return FALSE;
	m_bTest = TRUE;
	Dialog_Prompt *prompt = new Dialog_Prompt;
	if (IDOK == prompt->DoModal()) {
		int nRepeat = prompt->GetInputInteger();
		for (int i = 0; i < nRepeat; i++) {
			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_GENERATE_IFTA, BN_CLICKED), 0L);
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

int CTab_IFTA::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pIFTA = new ophIFTA();

	return 0;
}


void CTab_IFTA::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//SetWindowPos(NULL, 0, 0, 353, 305, SWP_NOMOVE);
}


void CTab_IFTA::OnBnClickedReadConfig_Ifta()
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


	if (!m_pIFTA->readConfig(CW2A(path))) {
		AfxMessageBox(L"it is not xml config file for IFTA.");
		return;
	}

	auto context = m_pIFTA->getContext();
	OphIFTAConfig config = m_pIFTA->getConfig();

	m_nearDepth = config.near_depthmap;
	m_farDepth = config.far_depthmap;
	m_nDepth = config.num_of_depth;
	m_nIteration = config.num_of_iteration;

	COpenholoRefAppDlg *pParent = (COpenholoRefAppDlg *)AfxGetMainWnd();
	pParent->SetWaveNum(context.waveNum);
	pParent->SetWaveLength(context.wave_length);
	pParent->SetPixelNum(context.pixel_number[_X], context.pixel_number[_Y]);
	pParent->SetPixelPitch(context.pixel_pitch[_X], context.pixel_pitch[_Y]);
	pParent->SetShift(context.shift[_X], context.shift[_Y], context.shift[_Z]);
	AfxGetMainWnd()->SendMessage(LOAD_CFG, LOAD_CFG, 0);

	m_bConfig = true;
	//if (m_bDimg && m_bRGBimg) GetDlgItem(IDC_GENERATE_IFTA)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOAD_RGB_IMG_IFTA)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOAD_D_IMG_IFTA)->EnableWindow(TRUE);

	UpdateData(FALSE);
}

void CTab_IFTA::OnBnClickedLoadDepthImg()
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

	if (!m_pIFTA->readImage(CW2A(path), false)) {
		AfxMessageBox(L"BMP load failed : Please show LOG.");
		return;
	}
	m_bDimg = TRUE;
	if (m_bRGBimg && m_bDimg)
		AfxGetMainWnd()->SendMessage(LOAD_DATA, LOAD_DATA, 0);
}

void CTab_IFTA::OnBnClickedLoadRGBImg()
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
			m_szDname = FileDialog.GetFileTitle();
		}
		else return;
	}

	SetCurrentDirectory(szCurPath);
	if (!path.GetLength()) return;
	AfxGetApp()->WriteProfileString(KEY_NAME, L"RGB-Img Path", path.Left(path.ReverseFind('\\') + 1));

	if (!m_pIFTA->readImage(CW2A(path.GetBuffer()), true)) {
		AfxMessageBox(L"BMP load failed : Please show LOG.");
		return;
	}
	m_bRGBimg = TRUE;

	if (m_bRGBimg && m_bDimg)
		AfxGetMainWnd()->SendMessage(LOAD_DATA, LOAD_DATA, 0);

}

UINT CallFuncIFTA(void* param)
{
	parammeter *pParam = (parammeter *)param;

#if 0
	Console::getInstance()->SetColor(Console::Color::YELLOW, Console::Color::BLACK);

	ophIFTA *pIFTA = ((ophIFTA *)pParam->pInst);
	Complex<Real> **pp = pIFTA->getComplexField();

	for (int i = 0; i < 10; i++) {
		((ophIFTA*)pParam->pInst)->generateHologram();
		for (uint i = 0; i < pIFTA->getContext().waveNum; i++) {
			printf("=> Complex Field[%d][0] = %.15e / %.15e \n", i, pp[i][0][_RE], pp[i][0][_IM]);
		}
	}

	pParam->pDialog->m_bFinished = TRUE;
#else
	((ophIFTA*)pParam->pInst)->generateHologram();
	pParam->pDialog->m_bFinished = TRUE;

	ophIFTA *pIFTA = ((ophIFTA *)pParam->pInst);
	Complex<Real> **pp = pIFTA->getComplexField();

	Console::getInstance()->SetColor(Console::Color::YELLOW, Console::Color::BLACK);
	for (uint i = 0; i < pIFTA->getContext().waveNum; i++) {
		printf("=> Complex Field[%d][0] = %.15e / %.15e \n", i, pp[i][0][_RE], pp[i][0][_IM]);
	}
#endif
	Console::getInstance()->ResetColor();
	delete pParam;

	return 1;
}


void CTab_IFTA::MakeFileName(CString szAppend)
{
	if (szAppend.IsEmpty()) {
		m_szFileName.Empty();
	}

	m_szFileName = ((COpenholoRefAppDlg *)AfxGetMainWnd())->GetFileName();
	m_szFileName.AppendFormat(L"%dch_", m_pIFTA->getContext().waveNum);
	m_szFileName.AppendFormat(L"%dx%d_", m_pIFTA->getContext().pixel_number[_X], m_pIFTA->getContext().pixel_number[_Y]);
}

void fft(vector<Complex<Real>> &src, vector<Complex<Real>> &dst)
{
	int n = (int)src.size();
	if (n == 1) {
		dst[0] = src[0];
		return;
	}
	vector<Complex<Real>> even(n / 2), odd(n / 2), Even(n / 2), Odd(n / 2);
	for (int i = 0; i < n / 2; i++) {
		even[i] = src[2 * i];
		odd[i] = src[2 * i + 1];
	}
	fft(even, Even);
	fft(odd, Odd);
	double th = -2.0 * M_PI / n;
	Complex<Real> w(cos(th), sin(th));
	Complex<Real> z(1, 0);
	for (int i = 0; i < n / 2; i++) {
		dst[i] = Even[i] + z * Odd[i];
		dst[i + n / 2] = Even[i] - z * Odd[i];
		z *= w;
	}
}

void ifft(vector<Complex<Real>> &src, vector<Complex<Real>> &dst)
{
	fft(src, dst);
	/*
	int n = dst.size();
	for (int i = 0; i < n; i++) {
		dst[i][_RE] /= n;
		dst[i][_IM] /= -n;
	}
	*/
}


BOOL CTab_IFTA::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	// GeForce GPU 일 때만, 활성화
	COpenholoRefAppDlg *pDlg = (COpenholoRefAppDlg *)AfxGetApp()->GetMainWnd();

	vector<Complex<Real>> vc;
	vector<Complex<Real>> vc2(3);
	vc.push_back(Complex<Real>(1, 0));
	vc.push_back(Complex<Real>(2, 0));
	vc.push_back(Complex<Real>(3, 0));
	//vc.push_back(Complex<Real>(0, 0));
	//vc.push_back(Complex<Real>(5, 0));
	ifft(vc, vc2);




	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}



void CTab_IFTA::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	m_pIFTA->release();
}


bool CTab_IFTA::CheckConfig()
{
	bool bChanged = false;
	double nearDepth = m_nearDepth;
	double farDepth = m_farDepth;
	int nIteration = m_nIteration;
	int nDepth = m_nDepth;
	UpdateData(TRUE);

	if (nDepth != m_nDepth) {
		printf("\n*Changed Depth Level*\nDepth Level: %d -> %d\n",
			nDepth, m_nDepth);
		bChanged = true;
	}
	if (nearDepth != m_nearDepth) {
		printf("\n*Changed Near Depth*\nNear Depth: %lf -> %lf\n",
			nearDepth, m_nearDepth);
		bChanged = true;
	}
	if (farDepth != m_farDepth) {
		printf("\n*Changed Far Depth*\nFar Depth: %lf -> %lf\n",
			farDepth, m_farDepth);
		bChanged = true;
	}
	if (nIteration != m_nIteration) {
		printf("\n*Changed Iteration*\nIteration: %d -> %d\n",
			nIteration, m_nIteration);
		bChanged = true;
	}

	return bChanged;
}

void CTab_IFTA::InitUI()
{
	GetDlgItem(IDC_READ_CONFIG_IFTA)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOAD_D_IMG_IFTA)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOAD_RGB_IMG_IFTA)->EnableWindow(TRUE);
	m_bRGBimg = false;
	m_bDimg = false;
}


LRESULT CTab_IFTA::OnGenerate(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);

	((COpenholoRefAppDlg *)AfxGetMainWnd())->ForegroundConsole();

	Dialog_Progress progress;

	BOOL bIsFinish = FALSE;

	parammeter *pParam = new parammeter;
	pParam->pInst = m_pIFTA;
	pParam->pDialog = &progress;

	CWinThread* pThread = AfxBeginThread(CallFuncIFTA, pParam);
	progress.m_bGen = true;
	progress.DoModal();
	progress.DestroyWindow();
	return TRUE;
}

LRESULT CTab_IFTA::OnEncode(WPARAM wParam, LPARAM lParam)
{
	COpenholoRefAppDlg *pParent = (COpenholoRefAppDlg *)AfxGetMainWnd();
	Real shiftX = pParent->GetShiftX();
	Real shiftY = pParent->GetShiftY();
	m_pIFTA->Shift(shiftX, shiftY);

	int idx = wParam;
	switch (ophGen::ENCODE_FLAG(idx)) {
	case ophGen::ENCODE_PHASE:
	case ophGen::ENCODE_AMPLITUDE:
	case ophGen::ENCODE_REAL:
	case ophGen::ENCODE_IMAGINEARY:
	case ophGen::ENCODE_SIMPLENI:
	case ophGen::ENCODE_BURCKHARDT:
	case ophGen::ENCODE_TWOPHASE:
		((ophGen*)m_pIFTA)->encoding(ophGen::ENCODE_FLAG(idx));
		break;
	case ophGen::ENCODE_SSB:
	case ophGen::ENCODE_OFFSSB:
		m_pIFTA->encoding(ophGen::ENCODE_FLAG(idx), (int)lParam);
		break;
	}
	m_pIFTA->normalize();
	return TRUE;
}

LRESULT CTab_IFTA::OnSaveIMG(WPARAM wParam, LPARAM lParam)
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
	int ch = m_pIFTA->getContext().waveNum;
	auto size = m_pIFTA->getEncodeSize();
	m_pIFTA->save(CW2A(path), 8 * ch, nullptr, size[_X], size[_Y]);

	pParent->OpenExplorer(path);
	return TRUE;
}

LRESULT CTab_IFTA::OnSaveOHC(WPARAM wParam, LPARAM lParam)
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
	m_pIFTA->saveAsOhc(CW2A(path));
	pParent->OpenExplorer(path);
	return TRUE;
}