// Dialog_BMP_Viewer.cpp : implementation file
//

#include "stdafx.h"
#include "OpenholoRefAppGUI.h"
#include "Dialog_BMP_Viewer.h"
#include "afxdialogex.h"


// Dialog_BMP_Viewer dialog

IMPLEMENT_DYNAMIC(Dialog_BMP_Viewer, CDialogEx)

Dialog_BMP_Viewer::Dialog_BMP_Viewer(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_BMP_VIEWER, pParent)
	, m_curImageIdx(0)
	, m_prevInterval(0)
	, m_nextInterval(0)
{

}

Dialog_BMP_Viewer::~Dialog_BMP_Viewer()
{
}

void Dialog_BMP_Viewer::Init(CString pDirectoryPath, INIT_FLAG flag)
{
	switch (flag)
	{
	case INIT_DIR:
	{
		CString filePath = pDirectoryPath.GetBuffer();

		filePath.Append(L"\\*.bmp");

		CFileFind finder;

		BOOL bWorking = finder.FindFile(filePath);
		CString fileName;
		CString DirName;

		while (bWorking) {
			bWorking = finder.FindNextFile();

			if (finder.IsArchived()) {
				CString _fileName = finder.GetFileName();

				if (_fileName == _T(".") || _fileName == _T("..") || _fileName == _T("Thumbs.db")) continue;

				CImage* pImage = new CImage;
				pImage->Destroy();

				pImage->Load(finder.GetFilePath());
				m_image.push_back(pImage);
				m_imageName.push_back(finder.GetFileTitle());
			}

		}
	}
	break;
	case INIT_SINGLE:
	{
		if (flag != INIT_SINGLE) return;
		CImage* pImage = new CImage;
		pImage->Destroy();

		pImage->Load(pDirectoryPath.GetBuffer());
		m_image.push_back(pImage);
		CString token = pDirectoryPath.Right(pDirectoryPath.GetLength() - pDirectoryPath.ReverseFind('\\') - 1);
		m_imageName.push_back(token);
	}
		break;
	}

	SetWindowPos(NULL, 0, 0, m_image[0]->GetWidth() + 100, m_image[0]->GetHeight() + 80, SWP_SHOWWINDOW);

	this->ModifyStyle(0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
}

void Dialog_BMP_Viewer::Init(std::vector<CString>& rPathArr, INIT_FLAG flag)
{
	if (flag != INIT_ARR) return;
	for (auto path : rPathArr) {
		CImage* pImage = new CImage;
		pImage->Destroy();

		pImage->Load(path);
		m_image.push_back(pImage);
		CString token = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
		m_imageName.push_back(token);
	}

	SetWindowPos(NULL, 0, 0, m_image[0]->GetWidth() + 100, m_image[0]->GetHeight() + 80, SWP_SHOWWINDOW);
}

BOOL Dialog_BMP_Viewer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	RECT wndRect;
	GetClientRect(&wndRect);

	GetDlgItem(IDC_PREV)->SetWindowPos(NULL, wndRect.left + 7, wndRect.bottom - 25, 0, 0, SWP_NOSIZE);
	GetDlgItem(IDC_NEXT)->SetWindowPos(NULL, wndRect.right - 30, wndRect.bottom - 25, 0, 0, SWP_NOSIZE);

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_IMAGE);

	for (auto item : m_imageName) {
		pComboBox->AddString(item.GetBuffer());
	}
	pComboBox->SetCurSel(0);

	CRect comborect;
	pComboBox->GetClientRect(&comborect);
	auto comboWidth = comborect.right - comborect.left;

	pComboBox->SetWindowPos(NULL, (wndRect.right / 2) - (comboWidth / 2), wndRect.bottom - 25, 0, 0, SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void Dialog_BMP_Viewer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dialog_BMP_Viewer, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PREV, &Dialog_BMP_Viewer::OnBnClickedPrev)
	ON_BN_CLICKED(IDC_NEXT, &Dialog_BMP_Viewer::OnBnClickedNext)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_IMAGE, &Dialog_BMP_Viewer::OnCbnSelchangeComboImage)
END_MESSAGE_MAP()


// Dialog_BMP_Viewer message handlers


void Dialog_BMP_Viewer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages

	if (!m_image[m_curImageIdx]->IsNull()) {

		// 이미지 배치
		RECT wndRect;
		GetClientRect(&wndRect);
		auto wndRectWidth = wndRect.right - wndRect.left;
		auto wndRectHeight = wndRect.bottom - wndRect.top;
		auto imageWidth = m_image[m_curImageIdx]->GetWidth();
		auto imageHeight = m_image[m_curImageIdx]->GetHeight();

		while (imageWidth > 960) {imageWidth /= 2;
		while (imageHeight > 540) imageHeight /= 2;}

		RECT rect;
		rect.top = (wndRectHeight - imageHeight) / 2 - 5;
		rect.bottom = rect.top + imageHeight;
		rect.left = (wndRectWidth - imageWidth) / 2;
		rect.right = rect.left + imageWidth;

		//SetWindowPos(NULL, 0, 0, m_image[m_curImageIdx]->GetWidth() + 100, m_image[m_curImageIdx]->GetHeight() + 80, SWP_NOREPOSITION);

		m_image[m_curImageIdx]->Draw(dc.m_hDC, rect);

		//GetDlgItem(IDC_PREV)->SetWindowPos(NULL, wndRect.left + 7, wndRect.bottom - 25, 0, 0, SWP_NOSIZE);
		//GetDlgItem(IDC_NEXT)->SetWindowPos(NULL, wndRect.right - 30, wndRect.bottom - 25, 0, 0, SWP_NOSIZE);
	}
}


void Dialog_BMP_Viewer::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here

	for (auto item : m_image)
	{
		delete item;
		item = nullptr;
	}
	m_image.clear();
}


void Dialog_BMP_Viewer::OnBnClickedPrev()
{
	// TODO: Add your control notification handler code here
	if (m_image.size() == 1) return;
	if (m_curImageIdx == 0) m_curImageIdx = m_image.size() - 1;
	else m_curImageIdx--;
	auto pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_IMAGE);
	pCombo->SetCurSel(m_curImageIdx);
	UpdateData(FALSE);

	//Invalidate(FALSE);
	RECT wndRect;
	GetClientRect(&wndRect);
	auto wndRectWidth = wndRect.right - wndRect.left;
	auto wndRectHeight = wndRect.bottom - wndRect.top;
	auto imageWidth = m_image[m_curImageIdx]->GetWidth();
	auto imageHeight = m_image[m_curImageIdx]->GetHeight();

	while (imageWidth > 960) {
		imageWidth /= 2;
		while (imageHeight > 540) imageHeight /= 2;
	}

	RECT rect;
	rect.top = (wndRectHeight - imageHeight) / 2 - 5;
	rect.bottom = rect.top + imageHeight;
	rect.left = (wndRectWidth - imageWidth) / 2;
	rect.right = rect.left + imageWidth;

	InvalidateRect(&rect, TRUE);
}


void Dialog_BMP_Viewer::OnBnClickedNext()
{
	// TODO: Add your control notification handler code here
	if (m_image.size() == 1) return;
	if (m_curImageIdx == m_image.size() - 1) m_curImageIdx = 0;
	else m_curImageIdx++;
	auto pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_IMAGE);
	pCombo->SetCurSel(m_curImageIdx);
	UpdateData(FALSE);

	//Invalidate(FALSE);
	RECT wndRect;
	GetClientRect(&wndRect);
	auto wndRectWidth = wndRect.right - wndRect.left;
	auto wndRectHeight = wndRect.bottom - wndRect.top;
	auto imageWidth = m_image[m_curImageIdx]->GetWidth();
	auto imageHeight = m_image[m_curImageIdx]->GetHeight();

	while (imageWidth > 960) {
		imageWidth /= 2;
		while (imageHeight > 540) imageHeight /= 2;
	}

	RECT rect;
	rect.top = (wndRectHeight - imageHeight) / 2 - 5;
	rect.bottom = rect.top + imageHeight;
	rect.left = (wndRectWidth - imageWidth) / 2;
	rect.right = rect.left + imageWidth;

	InvalidateRect(&rect, TRUE);
}

void Dialog_BMP_Viewer::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	auto imageWidth = m_image[m_curImageIdx]->GetWidth();
	auto imageHeight = m_image[m_curImageIdx]->GetHeight();

	while (imageWidth > 960) {
		imageWidth /= 2;
		while (imageHeight > 540) imageHeight /= 2;
	}
	SetWindowPos(NULL, 0, 0, imageWidth + 100, imageHeight + 80, SWP_NOMOVE);
}

void Dialog_BMP_Viewer::OnCbnSelchangeComboImage()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_image.size() == 1) return;
	auto pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_IMAGE);

	auto idx = pCombo->GetCurSel();

	if (idx > m_image.size() - 1 || idx < 0) return;

	m_curImageIdx = idx;

	RECT wndRect;
	GetClientRect(&wndRect);
	auto wndRectWidth = wndRect.right - wndRect.left;
	auto wndRectHeight = wndRect.bottom - wndRect.top;
	auto imageWidth = m_image[m_curImageIdx]->GetWidth();
	auto imageHeight = m_image[m_curImageIdx]->GetHeight();

	while (imageWidth > 960) {
		imageWidth /= 2;
		while (imageHeight > 540) imageHeight /= 2;
	}

	RECT rect;
	rect.top = (wndRectHeight - imageHeight) / 2 - 5;
	rect.bottom = rect.top + imageHeight;
	rect.left = (wndRectWidth - imageWidth) / 2;
	rect.right = rect.left + imageWidth;

	InvalidateRect(&rect, TRUE);
}
