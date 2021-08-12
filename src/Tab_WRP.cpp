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

#define KEY_NAME L"WRP"
IMPLEMENT_DYNAMIC(CTab_WRP, CDialogEx)

CTab_WRP::CTab_WRP(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_WRP, pParent)
	, m_scaleX(0)
	, m_scaleY(0)
	, m_scaleZ(0)
	, m_numofWRP(0)
	, m_locationWRP(0)
	, m_distance(0)
	, m_argParam()
	, m_resultPath()
	, m_bConfig(FALSE)
	, m_bPC(FALSE)
	, m_bEncode(FALSE)
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
	DDX_Text(pDX, IDC_SCALE_X_WRP, m_scaleX);
	DDX_Text(pDX, IDC_SCALE_Y_WRP, m_scaleY);
	DDX_Text(pDX, IDC_SCALE_Z_WRP, m_scaleZ);
	DDX_Text(pDX, IDC_NUM_OF_WRP, m_numofWRP);
	DDX_Text(pDX, IDC_LOCATION_OF_WRP, m_locationWRP);
	DDX_Text(pDX, IDC_DISTANCE_WRP, m_distance);
}


BEGIN_MESSAGE_MAP(CTab_WRP, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_READ_CONFIG_WRP, &CTab_WRP::OnBnClickedReadConfigWrp)
	ON_BN_CLICKED(IDC_LOAD_WRP, &CTab_WRP::OnBnClickedLoadPcWrp)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_VIEW_WRP, &CTab_WRP::OnBnClickedViewWrp)
	ON_MESSAGE(GENERATE, &CTab_WRP::OnGenerate)
	ON_MESSAGE(ENCODE, &CTab_WRP::OnEncode)
	ON_MESSAGE(SAVE_IMG, &CTab_WRP::OnSaveIMG)
	ON_MESSAGE(SAVE_OHC, &CTab_WRP::OnSaveOHC)
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

	if (!m_pWRP->readConfig(CW2A(path))) {
		AfxMessageBox(L"it is not xml config file for WRP.");
		return;
	}

	oph::vec3 scale;

	scale = m_pWRP->getScale();
	auto context = m_pWRP->getContext();
	m_scaleX = scale[_X];
	m_scaleY = scale[_Y];
	m_scaleZ = scale[_Z];
	m_distance = m_pWRP->getDistance();
	m_numofWRP = m_pWRP->getNumOfWRP();
	m_locationWRP = m_pWRP->getLocation();

	COpenholoRefAppDlg *pParent = (COpenholoRefAppDlg *)AfxGetMainWnd();
	pParent->SetWaveNum(context.waveNum);
	pParent->SetWaveLength(context.wave_length);
	pParent->SetPixelNum(context.pixel_number[_X], context.pixel_number[_Y]);
	pParent->SetPixelPitch(context.pixel_pitch[_X], context.pixel_pitch[_Y]);
	pParent->SetShift(context.shift[_X], context.shift[_Y], context.shift[_Z]);
	AfxGetMainWnd()->SendMessage(LOAD_CFG, LOAD_CFG, context.waveNum);

	m_bConfig = true;
	UpdateData(FALSE);
}


void CTab_WRP::OnBnClickedLoadPcWrp()
{
	// TODO: Add your control notification handler code here
	TCHAR szCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurPath);

	LPTSTR szFilter = L"PLY File (*.ply) |*.ply|";

	CFileDialog FileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString szPlyPath = AfxGetApp()->GetProfileString(KEY_NAME, L"PLY Path", szCurPath);
	FileDialog.m_ofn.lpstrInitialDir = szPlyPath;
	CString path;
	if (FileDialog.DoModal() == IDOK)
	{
		CString ext = FileDialog.GetFileExt();
		if (!ext.CompareNoCase(L"ply")) path = FileDialog.GetPathName();
		else return;
	}

	SetCurrentDirectory(szCurPath);
	if (!path.GetLength()) return;
	AfxGetApp()->WriteProfileString(KEY_NAME, L"PLY Path", path.Left(path.ReverseFind('\\') + 1));

	if (m_pWRP->loadPointCloud(CW2A(path)) == -1)
	{
		AfxMessageBox(L"it is not a ply file for WRP.");
		return;
	}

	m_bPC = true;
	if (m_bConfig)
		AfxGetMainWnd()->SendMessage(LOAD_DATA, LOAD_DATA, 0);

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
	((ophWRP*)pParam->pInst)->generateHologram();
	pParam->pDialog->m_bFinished = TRUE;

	ophWRP *pWRP = ((ophWRP *)pParam->pInst);
	Complex<Real> **pp = pWRP->getComplexField();

	Console::getInstance()->SetColor(Console::Color::YELLOW, Console::Color::BLACK);
	for (uint i = 0; i < pWRP->getContext().waveNum; i++)
		printf("=> Complex Field[%d][0] = %.15e / %.15e\n", i, pp[i][0][_RE], pp[i][0][_IM]);
	Console::getInstance()->ResetColor();
	delete pParam;

	return 1;
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
}

void CTab_WRP::OnBnClickedViewWrpBmp()
{
	// TODO: Add your control notification handler code here
	Dialog_BMP_Viewer viewer;

	viewer.Init(m_resultPath, INIT_SINGLE);
	viewer.DoModal();

	viewer.DestroyWindow();
}

BOOL CTab_WRP::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	// GeForce GPU 일 때만, 활성화
	COpenholoRefAppDlg *pDlg = (COpenholoRefAppDlg *)AfxGetApp()->GetMainWnd();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CTab_WRP::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	m_pWRP->release();
}


LRESULT CTab_WRP::OnGenerate(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);

	if (m_scaleX == 0.0 || m_scaleY == 0.0 || m_scaleZ == 0.0) {
		AfxMessageBox(L"Config value error - scale");
		return FALSE;
	}
	if (m_distance == 0.0) {
		AfxMessageBox(L"Config value error - offset depth");
		return FALSE;
	}

	COpenholoRefAppDlg *dlg = (COpenholoRefAppDlg *)AfxGetMainWnd();

	auto context = m_pWRP->getContext();
	m_pWRP->setDistance(m_distance);
	m_pWRP->setMode(!dlg->UseGPGPU());
	m_pWRP->setScale(vec3(m_scaleX, m_scaleY, m_scaleZ));
	m_pWRP->setLocation(m_locationWRP);
	m_pWRP->setViewingWindow(dlg->UseVW());

	dlg->ForegroundConsole();

	Dialog_Progress progress;

	BOOL bIsFinish = FALSE;

	parammeter *pParam = new parammeter;
	pParam->pInst = m_pWRP;
	pParam->pDialog = &progress;
	//progress.m_bPercent = true;
	//progress.m_iPercent = m_pWRP->getProgress();

	CWinThread* pThread = AfxBeginThread(CallFuncWRP, pParam);
	progress.m_bGen = true;
	progress.DoModal();
	progress.DestroyWindow();
	return TRUE;
}

LRESULT CTab_WRP::OnEncode(WPARAM wParam, LPARAM lParam)
{
	COpenholoRefAppDlg *pParent = (COpenholoRefAppDlg *)AfxGetMainWnd();
	Real shiftX = pParent->GetShiftX();
	Real shiftY = pParent->GetShiftY();
	m_pWRP->Shift(shiftX, shiftY);
	int idx = wParam;
	switch (ophGen::ENCODE_FLAG(idx)) {
	case ophGen::ENCODE_PHASE:
	case ophGen::ENCODE_AMPLITUDE:
	case ophGen::ENCODE_REAL:
	case ophGen::ENCODE_IMAGINEARY:
	case ophGen::ENCODE_SIMPLENI:
	case ophGen::ENCODE_BURCKHARDT:
	case ophGen::ENCODE_TWOPHASE:
		((ophGen*)m_pWRP)->encoding(ophGen::ENCODE_FLAG(idx));
		break;
	case ophGen::ENCODE_SSB:
	case ophGen::ENCODE_OFFSSB:
		m_pWRP->encoding(ophGen::ENCODE_FLAG(idx), (int)lParam);
		break;
	}
	m_pWRP->normalize();
	return TRUE;
}

LRESULT CTab_WRP::OnSaveIMG(WPARAM wParam, LPARAM lParam)
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
	int ch = m_pWRP->getContext().waveNum;
	auto size = m_pWRP->getEncodeSize();
	m_pWRP->save(CW2A(path), 8 * ch, nullptr, size[_X], size[_Y]);

	pParent->OpenExplorer(path);
	return TRUE;
}

LRESULT CTab_WRP::OnSaveOHC(WPARAM wParam, LPARAM lParam)
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
	m_pWRP->saveAsOhc(CW2A(path));
	pParent->OpenExplorer(path);
	return TRUE;
}