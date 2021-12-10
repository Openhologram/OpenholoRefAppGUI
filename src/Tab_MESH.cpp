// Tab_MESH.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "OpenholoRefAppGUIDlg.h"
#include "Tab_MESH.h"
#include "afxdialogex.h"
#include "Console.h"

// CTab_MESH dialog

#include <ophTriMesh.h>
#include "Dialog_BMP_Viewer.h"
#include "Dialog_Progress.h"
#include "Dialog_Prompt.h"
#define KEY_NAME L"TriMesh"

IMPLEMENT_DYNAMIC(CTab_MESH, CDialogEx)

CTab_MESH::CTab_MESH(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_MESH, pParent)
	, m_scaleX(0)
	, m_scaleY(0)
	, m_scaleZ(0)
	, m_lampdirX(0)
	, m_lampdirY(0)
	, m_lampdirZ(0)
	, m_argParam()
	, m_resultPath()
	, m_bConfig(FALSE)
	, m_bMeshData(FALSE)
	, m_bEncode(FALSE)
#ifdef TEST_MODE
	, m_bTest(FALSE)
#endif
{

}

CTab_MESH::~CTab_MESH()
{
}

void CTab_MESH::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SCALE_X_MESH, m_scaleX);
	DDX_Text(pDX, IDC_SCALE_Y_MESH, m_scaleY);
	DDX_Text(pDX, IDC_SCALE_Z_MESH, m_scaleZ);
	DDX_Text(pDX, IDC_LAMP_DIRECTION_X, m_lampdirX);
	DDX_Text(pDX, IDC_LAMP_DIRECTION_Y, m_lampdirY);
	DDX_Text(pDX, IDC_LAMP_DIRECTION_Z, m_lampdirZ);
}


BEGIN_MESSAGE_MAP(CTab_MESH, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_READ_CONFIG_MESH, &CTab_MESH::OnBnClickedReadConfigMesh)
	ON_BN_CLICKED(IDC_LOAD_MESH, &CTab_MESH::OnBnClickedLoadMesh)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_VIEW_MESH, &CTab_MESH::OnBnClickedViewMesh)
	ON_MESSAGE(GENERATE, &CTab_MESH::OnGenerate)
	ON_MESSAGE(ENCODE, &CTab_MESH::OnEncode)
	ON_MESSAGE(SAVE_IMG, &CTab_MESH::OnSaveIMG)
	ON_MESSAGE(SAVE_OHC, &CTab_MESH::OnSaveOHC)
END_MESSAGE_MAP()


// CTab_MESH message handlers


BOOL CTab_MESH::PreTranslateMessage(MSG* pMsg)
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
BOOL CTab_MESH::AutoTest()
{
	if (!m_bMeshData || !m_bConfig)
		return FALSE;
	m_bTest = TRUE;
	Dialog_Prompt *prompt = new Dialog_Prompt;
	if (IDOK == prompt->DoModal()) {
		int nRepeat = prompt->GetInputInteger();
		for (int i = 0; i < nRepeat; i++)
			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_GENERATE_MESH, BN_CLICKED), 0L);
	}
	delete prompt;
	m_bTest = FALSE;
	return TRUE;
}
#endif

int CTab_MESH::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pMesh = new ophTri();

	return 0;
}


void CTab_MESH::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//SetWindowPos(NULL, 0, 0, 353, 305, SWP_NOMOVE);
}


void CTab_MESH::OnBnClickedReadConfigMesh()
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


	if (!m_pMesh->readConfig(CW2A(path))) {
		AfxMessageBox(L"it is not xml config file for Triangle Mesh.");
		return;
	}

	auto context = m_pMesh->getContext();
	auto imgCfg = m_pMesh->getImageConfig();
	m_scaleX = m_pMesh->getObjSize()[_X];
	m_scaleY = m_pMesh->getObjSize()[_Y];
	m_scaleZ = m_pMesh->getObjSize()[_Z];

	m_lampdirX = m_pMesh->getIllumination()[_X];
	m_lampdirY = m_pMesh->getIllumination()[_Y];
	m_lampdirZ = m_pMesh->getIllumination()[_Z];

	//m_pPointCloud->getScale(scale);
	//auto context = m_pPointCloud->getContext();
	COpenholoRefAppDlg *pParent = (COpenholoRefAppDlg *)AfxGetMainWnd();
	pParent->SetWaveNum(context.waveNum);
	pParent->SetWaveLength(context.wave_length);
	pParent->SetPixelNum(context.pixel_number[_X], context.pixel_number[_Y]);
	pParent->SetPixelPitch(context.pixel_pitch[_X], context.pixel_pitch[_Y]);
	pParent->SetShift(context.shift[_X], context.shift[_Y], context.shift[_Z]);
	pParent->SetImageRotate(imgCfg.bRotation);
	pParent->SetImageMerge(imgCfg.bMergeImage);
	pParent->SetImageFlip(imgCfg.nFlip);
	AfxGetMainWnd()->SendMessage(LOAD_CFG, LOAD_CFG, 0);

	m_bConfig = true;

	UpdateData(FALSE);
}


void CTab_MESH::OnBnClickedLoadMesh()
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

	_tcscpy_s(m_argParam, path.GetBuffer());

	if (!m_pMesh->loadMeshData(CW2A(path), "ply"))
	{
		AfxMessageBox(L"it is not ply file for Mesh Data.");
		return;
	}

	m_bMeshData = true;
	if (m_bConfig)
		AfxGetMainWnd()->SendMessage(LOAD_DATA, LOAD_DATA, 0);
	GetDlgItem(IDC_VIEW_MESH)->EnableWindow(TRUE);

	UpdateData(FALSE);
}

void CTab_MESH::OnBnClickedViewMesh()
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

UINT CallFuncMESH(void* param)
{
	parammeter *pParam = (parammeter *)param;



	((ophTri*)pParam->pInst)->generateHologram(pParam->flag);
	pParam->pDialog->m_bFinished = TRUE;

	ophTri *pMesh = ((ophTri *)pParam->pInst);
	Complex<Real> **pp = pMesh->getComplexField();

	Console::getInstance()->SetColor(Console::Color::YELLOW, Console::Color::BLACK);
	for (uint i = 0; i < pMesh->getContext().waveNum; i++)
		printf("=> Complex Field[%d][0] = %.15e / %.15e \n", i, pp[i][0][_RE], pp[i][0][_IM]);
	Console::getInstance()->ResetColor();
	delete pParam;

	return 1;
}

void CTab_MESH::OnBnClickedViewMeshBmp()
{
	// TODO: Add your control notification handler code here
	Dialog_BMP_Viewer viewer;

	viewer.Init(m_resultPath, INIT_SINGLE);
	viewer.DoModal();

	viewer.DestroyWindow();
}


BOOL CTab_MESH::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	((CButton*)GetDlgItem(IDC_RADIO_FLAT))->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CTab_MESH::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	m_pMesh->release();
}

void CTab_MESH::MakeFileName(CString szAppend)
{
	if (szAppend.IsEmpty()) {
		m_szFileName.Empty();
	}

	m_szFileName = ((COpenholoRefAppDlg *)AfxGetMainWnd())->GetFileName();
	m_szFileName.AppendFormat(L"%dch_", m_pMesh->getContext().waveNum);
	m_szFileName.AppendFormat(L"%dx%d_", m_pMesh->getContext().pixel_number[_X], m_pMesh->getContext().pixel_number[_Y]);
	m_szFileName.AppendFormat(L"f%d_", m_pMesh->getNumMesh());
}

LRESULT CTab_MESH::OnGenerate(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	auto context = m_pMesh->getContext();

	int mode = (int)wParam;
	COpenholoRefAppDlg *dlg = (COpenholoRefAppDlg *)AfxGetMainWnd();
	m_pMesh->SetMode(mode);
	m_pMesh->SetRandomPhase(dlg->m_buttonRandomPhase.GetCheck());

	m_pMesh->SetMaxThreadNum(dlg->m_nCurThread);

	dlg->ForegroundConsole();

	Dialog_Progress progress;

	BOOL bIsFinish = FALSE;

	parammeter *pParam = new parammeter;
	pParam->pInst = m_pMesh;
	pParam->flag = ((CButton*)GetDlgItem(IDC_RADIO_FLAT))->GetCheck() ? m_pMesh->SHADING_FLAT : m_pMesh->SHADING_CONTINUOUS;
	pParam->pDialog = &progress;
	progress.m_bPercent = true;
	progress.m_iPercent = m_pMesh->getProgress();

	CWinThread* pThread = AfxBeginThread(CallFuncMESH, pParam);
	progress.m_bGen = true;
	progress.DoModal();
	progress.DestroyWindow();

	return TRUE;
}

LRESULT CTab_MESH::OnEncode(WPARAM wParam, LPARAM lParam)
{
	COpenholoRefAppDlg *pParent = (COpenholoRefAppDlg *)AfxGetMainWnd();
	Real shiftX = pParent->GetShiftX();
	Real shiftY = pParent->GetShiftY();
	m_pMesh->Shift(shiftX, shiftY);
	int idx = wParam;
	switch (ophGen::ENCODE_FLAG(idx)) {
	case ophGen::ENCODE_PHASE:
	case ophGen::ENCODE_AMPLITUDE:
	case ophGen::ENCODE_REAL:
	case ophGen::ENCODE_IMAGINEARY:
	case ophGen::ENCODE_SIMPLENI:
	case ophGen::ENCODE_BURCKHARDT:
	case ophGen::ENCODE_TWOPHASE:
		((ophGen*)m_pMesh)->encoding(ophGen::ENCODE_FLAG(idx));
		break;
	case ophGen::ENCODE_SSB:
	case ophGen::ENCODE_OFFSSB:
		m_pMesh->encoding(ophGen::ENCODE_FLAG(idx), (int)lParam);
		break;
	}
	m_pMesh->normalize();
	return TRUE;
}

LRESULT CTab_MESH::OnSaveIMG(WPARAM wParam, LPARAM lParam)
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

	m_pMesh->setImageMerge(wParam & 0x1 ? true : false);
	m_pMesh->setImageRotate(wParam & 0x2 ? true : false);
	m_pMesh->setImageFlip((int)lParam);
	ivec2 encode_size = m_pMesh->getEncodeSize();
	int ch = m_pMesh->getContext().waveNum;
	m_pMesh->save(CW2A(path), 8 * ch, nullptr, encode_size[_X], encode_size[_Y]);

	pParent->OpenExplorer(path);
	return TRUE;
}

LRESULT CTab_MESH::OnSaveOHC(WPARAM wParam, LPARAM lParam)
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
	m_pMesh->saveAsOhc(CW2A(path));
	pParent->OpenExplorer(path);

	return TRUE;
}