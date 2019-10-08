// Tab_MESH.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "OpenholoRefAppGUIDlg.h"
#include "Tab_MESH.h"
#include "afxdialogex.h"


// CTab_MESH dialog

#include <ophTriMesh.h>
#include "Dialog_BMP_Viewer.h"
#include "Dialog_Progress.h"
#include "Dialog_Prompt.h"

IMPLEMENT_DYNAMIC(CTab_MESH, CDialogEx)

CTab_MESH::CTab_MESH(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_MESH, pParent)
	, m_fieldLens(0)
	, m_objectsize(0)
	, m_objectshiftX(0)
	, m_objectshiftY(0)
	, m_objectshiftZ(0)
	, m_lampdirX(0)
	, m_lampdirY(0)
	, m_lampdirZ(0)
	, m_pixelpitchX(0)
	, m_pixelpitchY(0)
	, m_pixelnumX(0)
	, m_pixelnumY(0)
	, m_wavelength(0)
	, m_argParam()
	, m_resultPath()
	, m_bConfig(FALSE)
	, m_bMeshData(FALSE)
	, m_bEncode(FALSE)
	, m_idxEncode(3)
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
	DDX_Text(pDX, IDC_FIELD_LENS, m_fieldLens);
	DDX_Text(pDX, IDC_SCALE_X, m_objectsize);
	DDX_Text(pDX, IDC_SCALE_Y, m_objectshiftX);
	DDX_Text(pDX, IDC_SCALE_Z, m_objectshiftY);
	DDX_Text(pDX, IDC_OFFSET_DEPTH, m_objectshiftZ);
	DDX_Text(pDX, IDC_SCALE_Y2, m_lampdirX);
	DDX_Text(pDX, IDC_SCALE_Z2, m_lampdirY);
	DDX_Text(pDX, IDC_OFFSET_DEPTH2, m_lampdirZ);
	DDX_Text(pDX, IDC_PIXEL_PITCH_X, m_pixelpitchX);
	DDX_Text(pDX, IDC_PIXEL_PITCH_Y, m_pixelpitchY);
	DDX_Text(pDX, IDC_PIXEL_NUM_X, m_pixelnumX);
	DDX_Text(pDX, IDC_PIXEL_NUM_Y, m_pixelnumY);
	DDX_Text(pDX, IDC_WAVE_LENGTH, m_wavelength);
	DDX_Control(pDX, IDC_GPU_CHECK_MESH, m_buttonGPU);
	DDX_Control(pDX, IDC_TRANSFORM_VW, m_buttonViewingWindow);
}


BEGIN_MESSAGE_MAP(CTab_MESH, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_READ_CONFIG_MESH, &CTab_MESH::OnBnClickedReadConfigMesh)
	ON_BN_CLICKED(IDC_LOAD_MESH, &CTab_MESH::OnBnClickedLoadMesh)
	ON_BN_CLICKED(IDC_GENERATE_MESH, &CTab_MESH::OnBnClickedGenerateMesh)
	ON_BN_CLICKED(IDC_SAVE_BMP_MESH, &CTab_MESH::OnBnClickedSaveBmpMesh)
	ON_BN_CLICKED(IDC_SAVE_OHC_MESH, &CTab_MESH::OnBnClickedSaveOhcMesh)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_VIEW_MESH, &CTab_MESH::OnBnClickedViewMesh)
	ON_BN_CLICKED(IDC_VIEW_MESH_BMP, &CTab_MESH::OnBnClickedViewMeshBmp)
	ON_CBN_SELCHANGE(IDC_ENCODE_METHOD_MESH, &CTab_MESH::OnCbnSelchangeEncodeMethodMesh)
	ON_BN_CLICKED(IDC_ENCODING_MESH, &CTab_MESH::OnBnClickedEncodingMesh)
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

	if (!m_pMesh->readMeshConfig(mulpath)) {
		AfxMessageBox(L"it is not xml config file for Triangle Mesh.");
		return;
	}

	auto context = m_pMesh->getContext();
	m_fieldLens = m_pMesh->getFieldLens();
	m_objectsize = m_pMesh->getObjSize();
	m_objectshiftX = m_pMesh->getObjShift()[_X];
	m_objectshiftY = m_pMesh->getObjShift()[_Y];
	m_objectshiftZ = m_pMesh->getObjShift()[_Z];

	m_lampdirX = m_pMesh->getIllumination()[_X];
	m_lampdirY = m_pMesh->getIllumination()[_Y];
	m_lampdirZ = m_pMesh->getIllumination()[_Z];

	m_pixelpitchX = context.pixel_pitch[_X];
	m_pixelpitchY = context.pixel_pitch[_Y];

	m_pixelnumX = context.pixel_number[_X];
	m_pixelnumY = context.pixel_number[_Y];

	m_wavelength = *context.wave_length;

	//m_pPointCloud->getScale(scale);
	//auto context = m_pPointCloud->getContext();

	//m_scaleX = scale[_X];
	//m_scaleY = scale[_Y];
	//m_scaleZ = scale[_Z];
	//m_offsetdepth = m_pPointCloud->getOffsetDepth();
	//m_pixelpitchX = context.pixel_pitch[_X];
	//m_pixelpitchY = context.pixel_pitch[_Y];
	//m_pixelnumX = context.pixel_number[_X];
	//m_pixelnumY = context.pixel_number[_Y];
	//m_wavelength = *context.wave_length;

	m_bConfig = true;
	if (m_bMeshData) GetDlgItem(IDC_GENERATE_MESH)->EnableWindow(TRUE);

	UpdateData(FALSE);
}


void CTab_MESH::OnBnClickedLoadMesh()
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

	if (!m_pMesh->loadMeshData(mulpath, "ply"))
	{
		AfxMessageBox(L"it is not ply file for Mesh Data.");
		return;
	}

	m_bMeshData = true;
	if (m_bConfig) GetDlgItem(IDC_GENERATE_MESH)->EnableWindow(TRUE);
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

		auto a = (int)::ShellExecute(NULL, _T("open"),
			path,																								//실행 파일 경로
			argParam,																							//argument value 파라미터
			NULL, SW_SHOW);
	}
	else {
		AfxMessageBox(localPath + L"을(를) 찾을 수 없습니다.");
	}
}

UINT CallFuncMESH(void* param)
{
	parammeter *pParam = (parammeter *)param;	
	((ophTri*)pParam->pGEN)->objScaleShift();
	((ophTri*)pParam->pGEN)->generateMeshHologram(((ophTri*)pParam->pGEN)->SHADING_FLAT);
	pParam->pDialog->m_bFinished = TRUE;
	delete pParam;

	return 1;
}

void CTab_MESH::OnBnClickedGenerateMesh()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_buttonViewingWindow.GetCheck() && m_fieldLens == 0.0) {
		AfxMessageBox(L"Config value error - field lens");
		return;
	}
	if (m_objectsize == 0.0) {
		AfxMessageBox(L"Config value error - object size");
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

	auto context = m_pMesh->getContext();
	m_pMesh->setObjSize(m_objectsize);
	m_pMesh->setObjShift(vec3(m_objectshiftX, m_objectshiftY, m_objectshiftZ));
	m_pMesh->setIllumination(vec3(m_lampdirX, m_lampdirY, m_lampdirZ));

	context.pixel_pitch[_X] = m_pixelpitchX;
	context.pixel_pitch[_Y] = m_pixelpitchY;

	context.pixel_number[_X] = m_pixelnumX;
	context.pixel_number[_Y] = m_pixelnumY;

	*context.wave_length = m_wavelength;

	//m_pMesh->setMode(!m_buttonGPU.GetCheck());
	m_pMesh->setViewingWindow(m_buttonViewingWindow.GetCheck());


	GetDlgItem(IDC_SAVE_OHC_MESH)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVE_BMP_MESH)->EnableWindow(FALSE);
	GetDlgItem(IDC_ENCODING_MESH)->EnableWindow(TRUE);

	Dialog_Progress progress;

	BOOL bIsFinish = FALSE;

	parammeter *pParam = new parammeter;
	pParam->pGEN = m_pMesh;
	pParam->pDialog = &progress;

	CWinThread* pThread = AfxBeginThread(CallFuncMESH, pParam);
	progress.DoModal();
	progress.DestroyWindow();

	UpdateData(FALSE);
}


void CTab_MESH::OnBnClickedEncodingMesh()
{
	// TODO: Add your control notification handler code here
	auto dist = m_pMesh->getObjShift()[_Z];
	m_pMesh->waveCarry(0, 0.1, dist);
	switch (ophGen::ENCODE_FLAG(m_idxEncode)) {
	case ophGen::ENCODE_PHASE:
	case ophGen::ENCODE_AMPLITUDE:
	case ophGen::ENCODE_REAL:
	case ophGen::ENCODE_SIMPLENI:
	case ophGen::ENCODE_BURCKHARDT:
	case ophGen::ENCODE_TWOPHASE:
		m_pMesh->encoding(ophGen::ENCODE_FLAG(m_idxEncode));
		break;
	case ophGen::ENCODE_SSB:
	case ophGen::ENCODE_OFFSSB:
		m_pMesh->encoding(ophGen::ENCODE_FLAG(m_idxEncode), ophGen::SSB_TOP);
		break;
	}
	m_pMesh->normalizeEncoded();

	GetDlgItem(IDC_SAVE_BMP_MESH)->EnableWindow(TRUE);
}


void CTab_MESH::OnBnClickedSaveBmpMesh()
{
	// TODO: Add your control notification handler code here
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);

	LPTSTR szFilter = L"BMP File (*.bmp) |*.bmp|";

	
	CFileDialog FileDialog(FALSE, NULL, Time::GetTime(L"TriMesh"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
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
	ivec2 encode_size = m_pMesh->getEncodeSize();
	m_pMesh->save(mulpath, 8, nullptr, encode_size[_X], encode_size[_Y]);

	GetDlgItem(IDC_VIEW_MESH_BMP)->EnableWindow(TRUE);
}


void CTab_MESH::OnBnClickedViewMeshBmp()
{
	// TODO: Add your control notification handler code here
	Dialog_BMP_Viewer viewer;

	viewer.Init(m_resultPath, INIT_SINGLE);
	viewer.DoModal();

	viewer.DestroyWindow();
}


void CTab_MESH::OnBnClickedSaveOhcMesh()
{
	// TODO: Add your control notification handler code here
	TCHAR current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);

	LPTSTR szFilter = L"OHC File (*.ohc) |*.ohc|";

	CFileDialog FileDialog(FALSE, NULL, Time::GetTime(L"TriMesh"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
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
	if (!m_pMesh->saveAsOhc(mulpath)) {
		MessageBox(L"Save failed", L"Error", MB_ICONWARNING);
	}
}


BOOL CTab_MESH::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_MESH))->AddString(L"Phase");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_MESH))->AddString(L"Amplitude");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_MESH))->AddString(L"Real");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_MESH))->AddString(L"Simple NI");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_MESH))->AddString(L"Burckhardt");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_MESH))->AddString(L"Two-Phase");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_MESH))->AddString(L"Single-Side Band");
	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_MESH))->AddString(L"Off-SSB");

	((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_MESH))->SetCurSel(m_idxEncode);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CTab_MESH::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	m_pMesh->release();
}


void CTab_MESH::OnCbnSelchangeEncodeMethodMesh()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_idxEncode = ((CComboBox*)GetDlgItem(IDC_ENCODE_METHOD_MESH))->GetCurSel();
}
