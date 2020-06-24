// Tab_PC.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "OpenholoRefAppGUIDlg.h"
#include "Tab_PC.h"
#include "Console.h"
#include "afxdialogex.h"
#include "ophIFTA.h"
#include "ophSimulator.h"
// CTab_PC dialog

#include <ophPointCloud.h>
#include "Dialog_BMP_Viewer.h"
#include "Dialog_Progress.h"
#include "Dialog_Prompt.h"
#include "GLUTviewer.h"


#define KEY_NAME L"PointCloud"

IMPLEMENT_DYNAMIC(CTab_PC, CDialogEx)

CTab_PC::CTab_PC(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_PC, pParent)
	, m_scaleX(0)
	, m_scaleY(0)
	, m_scaleZ(0)
	, m_distance(0)
	, m_bConfig(FALSE)
	, m_bPC(FALSE)
	, m_bEncode(FALSE)
	, m_argParam()
	, m_argParamW()
	, m_resultPath()
	, m_idxDiff(0)
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
	DDX_Text(pDX, IDC_SCALE_X_PC, m_scaleX);
	DDX_Text(pDX, IDC_SCALE_Y_PC, m_scaleY);
	DDX_Text(pDX, IDC_SCALE_Z_PC, m_scaleZ);
	DDX_Text(pDX, IDC_DISTANCE_PC, m_distance);
}


BEGIN_MESSAGE_MAP(CTab_PC, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//	ON_WM_INITMENU()
	ON_BN_CLICKED(IDC_LOAD_PC, &CTab_PC::OnBnClickedLoadPc)
	ON_BN_CLICKED(IDC_READ_CONFIG_PC, &CTab_PC::OnBnClickedReadConfig_PC)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_VIEW_PC, &CTab_PC::OnBnClickedViewPc)
	//ON_WM_KEYDOWN()
	ON_CBN_SELCHANGE(IDC_DIFF_METHOD_PC, &CTab_PC::OnCbnSelchangeDiffMethodPc)
	ON_MESSAGE(GENERATE, &CTab_PC::OnMsg)
	ON_MESSAGE(ENCODE, &CTab_PC::OnMsg)
	ON_MESSAGE(SAVE_IMG, &CTab_PC::OnMsg)
	ON_MESSAGE(SAVE_OHC, &CTab_PC::OnMsg)
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
		if (!ext.CompareNoCase(L"xml")) 
			path = FileDialog.GetPathName();
		else return;
	}
	SetCurrentDirectory(szCurPath);
	if (!path.GetLength()) return;
	AfxGetApp()->WriteProfileString(KEY_NAME, L"Config Path", path.Left(path.ReverseFind('\\') + 1));
	if (!m_pPointCloud->readConfig(CW2A(path))) {
		AfxMessageBox(L"it is not xml config file for PointCloud.");
		return;
	}

	oph::vec3 scale;
	m_pPointCloud->getScale(scale);
	m_scaleX = scale[_X];
	m_scaleY = scale[_Y];
	m_scaleZ = scale[_Z];
	m_distance = m_pPointCloud->getDistance();

	auto context = m_pPointCloud->getContext();
	COpenholoRefAppDlg *pParent = (COpenholoRefAppDlg *)AfxGetMainWnd();
	pParent->SetWaveNum(context.waveNum);
	pParent->SetWaveLength(context.wave_length);
	pParent->SetPixelNum(context.pixel_number[_X], context.pixel_number[_Y]);
	pParent->SetPixelPitch(context.pixel_pitch[_X], context.pixel_pitch[_Y]);
	pParent->SetShift(context.shift[_X], context.shift[_Y], context.shift[_Z]);
	pParent->SendMessage(LOAD_CFG, LOAD_CFG, 0);

	m_bConfig = true;
	//if (m_bPC) m_buttonGenerate.EnableWindow(TRUE);

	UpdateData(FALSE);
}


void CTab_PC::OnBnClickedLoadPc()
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
		if (!ext.CompareNoCase(L"ply"))
			path = FileDialog.GetPathName(); 
		else return;
	}

	SetCurrentDirectory(szCurPath);
	if (!path.GetLength()) return;
	AfxGetApp()->WriteProfileString(KEY_NAME, L"PLY Path", path.Left(path.ReverseFind('\\') + 1));

	if (m_pPointCloud->loadPointCloud(CW2A(path)) == -1) 
	{
		AfxMessageBox(L"it is not ply file for PointCloud.");
		return;
	}

	strcpy_s(m_argParam, path.GetLength() + 1, CW2A(path));

	m_bPC = true;
	GetDlgItem(IDC_VIEW_PC)->EnableWindow(TRUE);


	COpenholoRefAppDlg *pParent = (COpenholoRefAppDlg *)AfxGetMainWnd();
	pParent->SendMessage(LOAD_DATA, LOAD_DATA, 0);
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

UINT CallFunc(void* param)
{
	parammeter *pParam = (parammeter *)param;
	((ophPointCloud *)pParam->pGEN)->generateHologram(pParam->flag);
	pParam->pDialog->m_bFinished = TRUE;
	
	Console::getInstance()->SetColor(Console::Color::YELLOW, Console::Color::BLACK);
	ophPointCloud *pPC = ((ophPointCloud *)pParam->pGEN);
	Complex<Real> **pp = pPC->getComplexField();
	for (uint i = 0; i < pPC->getContext().waveNum; i++)
		printf("=> Complex Field[%d][0] = %.15e / %.15e\n", i, pp[i][0][_RE], pp[i][0][_IM]);
	Console::getInstance()->ResetColor();
	delete pParam;

	return 1;
}

void CTab_PC::MakeFileName(CString szAppend)
{
	if (szAppend.IsEmpty()) {
		m_szFileName.Empty();
	}

	m_szFileName = ((COpenholoRefAppDlg *)AfxGetMainWnd())->GetFileName();
	m_szFileName.AppendFormat(L"%dch_", m_pPointCloud->getContext().waveNum);
	m_szFileName.AppendFormat(L"%dx%d_", m_pPointCloud->getContext().pixel_number[_X], m_pPointCloud->getContext().pixel_number[_Y]);
	m_szFileName.AppendFormat(L"v%d_", m_pPointCloud->getNumberOfPoints());
	m_szFileName.AppendFormat(L"%s_", m_idxDiff == 0 ? L"RS" : L"Fresnel");
}

BOOL CTab_PC::SaveIMG()
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
	auto size = m_pPointCloud->getEncodeSize();
	int ch = m_pPointCloud->getContext().waveNum;
	auto context = m_pPointCloud->getContext();	

	m_pPointCloud->save(CW2A(path), 8 * ch, nullptr, size[_X], size[_Y]);
	
	pParent->OpenExplorer(path);
	
	return TRUE;
}


void CTab_PC::OnBnClickedViewPcBmp()
{
	// TODO: Add your control notification handler code here
	Dialog_BMP_Viewer viewer;

	viewer.Init(m_resultPath, INIT_SINGLE);
	viewer.DoModal();

	viewer.DestroyWindow();
}


void CTab_PC::SaveOHC()
{
	// TODO: Add your control notification handler code here
	COpenholoRefAppDlg *pParent = ((COpenholoRefAppDlg *)AfxGetMainWnd());

	TCHAR szCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurPath);

	CString path;
	pParent->SaveOHC(path);

	SetCurrentDirectory(szCurPath);

	_tcscpy_s(m_resultPath, path.GetBuffer());

	if (!path.GetLength()) return;
	m_pPointCloud->saveAsOhc(CW2A(path));
	pParent->OpenExplorer(path);
}

BOOL CTab_PC::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	((CComboBox*)GetDlgItem(IDC_DIFF_METHOD_PC))->AddString(L"R-S diffaction");
	((CComboBox*)GetDlgItem(IDC_DIFF_METHOD_PC))->AddString(L"Fresnel diffaction");
	((CComboBox*)GetDlgItem(IDC_DIFF_METHOD_PC))->SetCurSel(m_idxDiff);
	
	
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

bool CTab_PC::CheckConfig()
{
	bool bChanged		= false;
	double scaleX		= m_scaleX;
	double scaleY		= m_scaleY;
	double scaleZ		= m_scaleZ;
	double distance		= m_distance;
	UpdateData(TRUE);

	if (scaleX != m_scaleX || scaleY != m_scaleY || scaleZ != m_scaleZ) {
		printf("\n*Changed Scale*\nScaleX: %lf -> %lf\nScaleY: %lf -> %lf\nScaleZ: %lf -> %lf\n",
			scaleX, m_scaleX, scaleY, m_scaleY, scaleZ, m_scaleZ);
		bChanged = true;
	}
	if (distance != m_distance) {
		printf("\n*Changed Distance*\nOffset Depth: %lf -> %lf\n",
			distance, m_distance);
		bChanged = true;
	}

	return bChanged;
}

LRESULT CTab_PC::OnMsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam == GENERATE) {
		bool bChangedConfig = CheckConfig();

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
		auto context = m_pPointCloud->getContext();
		m_pPointCloud->setDistance(m_distance);
		m_pPointCloud->setScale(m_scaleX, m_scaleY, m_scaleZ);
		m_pPointCloud->setMode(!dlg->UseGPGPU());
		m_pPointCloud->setViewingWindow(dlg->UseVW());

		dlg->ForegroundConsole();

		Dialog_Progress progress;

		BOOL bIsFinish = FALSE;

		parammeter *pParam = new parammeter;
		pParam->flag = m_idxDiff;
		pParam->pGEN = m_pPointCloud;
		pParam->pDialog = &progress;
		progress.m_bPercent = true;
		progress.m_iPercent = m_pPointCloud->getPercent();

		CWinThread* pThread = AfxBeginThread(CallFunc, pParam);
		progress.DoModal();
		progress.DestroyWindow();	
	}
	else if (wParam == ENCODE) {
		int idx = lParam;
		switch (ophGen::ENCODE_FLAG(idx)) {
		case ophGen::ENCODE_PHASE:
		case ophGen::ENCODE_AMPLITUDE:
		case ophGen::ENCODE_REAL:
		case ophGen::ENCODE_SIMPLENI:
		case ophGen::ENCODE_BURCKHARDT:
		case ophGen::ENCODE_TWOPHASE:
			((ophGen*)m_pPointCloud)->encoding(ophGen::ENCODE_FLAG(idx));
			break;
		case ophGen::ENCODE_SSB:
		case ophGen::ENCODE_OFFSSB:
			m_pPointCloud->encoding(ophGen::ENCODE_FLAG(idx), ophGen::SSB_TOP);
			break;
		case ophGen::ENCODE_SYMMETRIZATION:
			m_pPointCloud->ophGen::encoding(ophGen::ENCODE_FLAG(idx));
		}
		m_pPointCloud->normalize();
	}
	else if (wParam == SAVE_IMG) {
		return SaveIMG();
	}
	else if (wParam == SAVE_OHC) {
		SaveOHC();
	}
	return TRUE;
}