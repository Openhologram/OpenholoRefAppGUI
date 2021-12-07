// Tab_LF.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "OpenholoRefAppGUIDlg.h"
#include "Tab_LF.h"
#include "afxdialogex.h"
#include "Console.h"
// CTab_LF dialog

#include <ophLightField.h>
#include "Dialog_BMP_Viewer.h"
#include "Dialog_Progress.h"
#include "Dialog_Prompt.h"

#define KEY_NAME L"LightField"
IMPLEMENT_DYNAMIC(CTab_LF, CDialogEx)

CTab_LF::CTab_LF(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LF, pParent)
	, m_distance(0)
	, m_numimgX(0)
	, m_numimgY(0)
	, m_argParam()
	, m_resultPath()
	, m_bConfig(FALSE)
	, m_bDir(FALSE)
	, m_bEncode(FALSE)
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
	DDX_Text(pDX, IDC_DISTANCE_LF, m_distance);
	DDX_Text(pDX, IDC_NUM_OF_IMAGE_X, m_numimgX);
	DDX_Text(pDX, IDC_NUM_OF_IMAGE_Y, m_numimgY);
}


BEGIN_MESSAGE_MAP(CTab_LF, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_READ_CONFIG_LF, &CTab_LF::OnBnClickedReadConfig_LF)
	ON_BN_CLICKED(IDC_FIND_DIR, &CTab_LF::OnBnClickedFindDir)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_VIEW_LF, &CTab_LF::OnBnClickedViewLf)
	ON_MESSAGE(GENERATE, &CTab_LF::OnGenerate)
	ON_MESSAGE(ENCODE, &CTab_LF::OnEncode)
	ON_MESSAGE(SAVE_IMG, &CTab_LF::OnSaveIMG)
	ON_MESSAGE(SAVE_OHC, &CTab_LF::OnSaveOHC)
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

	if (!m_pLightField->readConfig(CW2A(path))) {
		AfxMessageBox(L"it is not xml config file for LightField.");
		return;
	}

	m_distance = m_pLightField->getDistRS2Holo();
	m_numimgX = m_pLightField->getNumImage()[_X];
	m_numimgY = m_pLightField->getNumImage()[_Y];

	m_bConfig = true;
	GetDlgItem(IDC_FIND_DIR)->EnableWindow(TRUE);

	auto context = m_pLightField->getContext();
	auto imgCfg = m_pLightField->getImageConfig();
	COpenholoRefAppDlg *pParent = (COpenholoRefAppDlg *)AfxGetMainWnd();
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


void CTab_LF::OnBnClickedFindDir()
{
	// TODO: Add your control notification handler code here
	TCHAR szSelPath[MAX_PATH] = { 0, };
	TCHAR szCurPath[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, szCurPath);
	CString path;
	CString szImgsPath = AfxGetApp()->GetProfileString(KEY_NAME, L"Images Path", szCurPath);
	CFolderPickerDialog dlg(szImgsPath, OFN_FILEMUSTEXIST, NULL, 0);
	if (dlg.DoModal() == IDOK) {
		path = dlg.GetPathName();
	}
	else
		return;

	SetCurrentDirectory(szCurPath);
	if (!path.GetLength()) return;
	AfxGetApp()->WriteProfileString(KEY_NAME, L"Images Path", path.Left(path.ReverseFind('\\') + 1));
	if (m_pLightField->loadLF(CW2A(path), "bmp") == -1) {
		AfxMessageBox(L"Load LF image failed");
		return;
	}

	m_bDir = TRUE;
	GetDlgItem(IDC_VIEW_LF)->EnableWindow(TRUE);

	COpenholoRefAppDlg *pParent = (COpenholoRefAppDlg *)AfxGetMainWnd();
	pParent->SendMessage(LOAD_DATA, LOAD_DATA, 0);
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

		::ShellExecute(NULL, _T("open"), path, argParam, NULL, SW_SHOW);
	}
	else {
		AfxMessageBox(localPath + L"을(를) 찾을 수 없습니다.");
	}

}

UINT CallFuncLF(void* param)
{
	parammeter *pParam = (parammeter *)param;
	((ophLF*)pParam->pInst)->generateHologram();
	pParam->pDialog->m_bFinished = TRUE;

	ophLF *pLF = ((ophLF *)pParam->pInst);
	Complex<Real> **pp = pLF->getComplexField();

	Console::getInstance()->SetColor(Console::Color::YELLOW, Console::Color::BLACK);
	for (uint i = 0; i < pLF->getContext().waveNum; i++)
		printf("=> Complex Field[%d][0] = %.15e / %.15e \n", i, pp[i][0][_RE], pp[i][0][_IM]);
	Console::getInstance()->ResetColor();
	delete pParam;

	return 1;
}

void CTab_LF::OnBnClickedViewLfBmp()
{
	// TODO: Add your control notification handler code here
	Dialog_BMP_Viewer viewer;

	viewer.Init(m_resultPath, INIT_SINGLE);
	viewer.DoModal();

	viewer.DestroyWindow();
}


BOOL CTab_LF::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	// GeForce GPU 일 때만, 활성화
	COpenholoRefAppDlg *pDlg = (COpenholoRefAppDlg *)AfxGetApp()->GetMainWnd();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CTab_LF::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	m_pLightField->release();
}

void CTab_LF::MakeFileName(CString szAppend)
{
	if (szAppend.IsEmpty()) {
		m_szFileName.Empty();
	}

	m_szFileName = ((COpenholoRefAppDlg *)AfxGetMainWnd())->GetFileName();
	m_szFileName.AppendFormat(L"%dch_", m_pLightField->getContext().waveNum);
	m_szFileName.AppendFormat(L"%dx%d_", m_pLightField->getContext().pixel_number[_X], m_pLightField->getContext().pixel_number[_Y]);
	m_szFileName.AppendFormat(L"%dx%d_", m_pLightField->getNumImage()[_X], m_pLightField->getNumImage()[_Y]);
}


LRESULT CTab_LF::OnGenerate(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	if (m_distance == 0.0) {
		AfxMessageBox(L"Config value error - distance RS to Hologram");
		return FALSE;
	}
	if (m_numimgX == 0 || m_numimgY == 0) {
		AfxMessageBox(L"Config value error - number of images of LF");
		return FALSE;
	}
	m_pLightField->setDistRS2Holo(m_distance);
	m_pLightField->setNumImage(m_numimgX, m_numimgY);

	int mode = (int)wParam;

	COpenholoRefAppDlg *dlg = (COpenholoRefAppDlg *)AfxGetMainWnd();
	m_pLightField->SetMode(mode);
	m_pLightField->setMode(!dlg->UseGPGPU());
	m_pLightField->SetRandomPhase(dlg->m_buttonRandomPhase.GetCheck());
	m_pLightField->setViewingWindow(dlg->UseVW());
	m_pLightField->SetMaxThreadNum(dlg->m_nCurThread);
	dlg->ForegroundConsole();

	Dialog_Progress progress;

	BOOL bIsFinish = FALSE;

	parammeter *pParam = new parammeter;
	pParam->pInst = m_pLightField;
	pParam->pDialog = &progress;

	CWinThread* pThread = AfxBeginThread(CallFuncLF, pParam);
	progress.m_bGen = true;
	progress.DoModal();
	progress.DestroyWindow();
	return TRUE;
}


LRESULT CTab_LF::OnEncode(WPARAM wParam, LPARAM lParam)
{
	COpenholoRefAppDlg *pParent = (COpenholoRefAppDlg *)AfxGetMainWnd();
	Real shiftX = pParent->GetShiftX();
	Real shiftY = pParent->GetShiftY();
	m_pLightField->Shift(shiftX, shiftY);
	int idx = wParam;
	switch (ophGen::ENCODE_FLAG(idx)) {
	case ophGen::ENCODE_PHASE:
	case ophGen::ENCODE_AMPLITUDE:
	case ophGen::ENCODE_REAL:
	case ophGen::ENCODE_IMAGINEARY:
	case ophGen::ENCODE_SIMPLENI:
	case ophGen::ENCODE_BURCKHARDT:
	case ophGen::ENCODE_TWOPHASE:
		((ophGen*)m_pLightField)->encoding(ophGen::ENCODE_FLAG(idx));
		break;
	case ophGen::ENCODE_SSB:
	case ophGen::ENCODE_OFFSSB:
		m_pLightField->encoding(ophGen::ENCODE_FLAG(idx), (int)lParam);
		break;
	}
	m_pLightField->normalize();
	return TRUE;
}
LRESULT CTab_LF::OnSaveIMG(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your control notification handler code here
	COpenholoRefAppDlg *pParent = ((COpenholoRefAppDlg *)AfxGetMainWnd());

	TCHAR szCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurPath);

	CString path;
	pParent->SaveImage(path);

	SetCurrentDirectory(szCurPath);
	if (!path.GetLength()) return FALSE;
	_tcscpy_s(m_resultPath, path.GetBuffer());

	m_pLightField->setImageMerge(wParam & 0x1 ? true : false);
	m_pLightField->setImageRotate(wParam & 0x2 ? true : false);
	m_pLightField->setImageFlip((int)lParam);
	ivec2 encodeSize = m_pLightField->getEncodeSize();
	int ch = m_pLightField->getContext().waveNum;
	m_pLightField->save(CW2A(path), 8 * ch, nullptr, encodeSize[_X], encodeSize[_Y]);
	//m_pLightField->save(CW2A(path), 8 * ch, m_pLightField->m_tmp2, encodeSize[_X], encodeSize[_Y]);

	pParent->OpenExplorer(path);
	return TRUE;
}

LRESULT CTab_LF::OnSaveOHC(WPARAM wParam, LPARAM lParam)
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
	m_pLightField->saveAsOhc(CW2A(path));
	pParent->OpenExplorer(path);
	return TRUE;
}