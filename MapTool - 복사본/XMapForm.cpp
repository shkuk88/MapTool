// XMapForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "XMapForm.h"
#include "afxdialogex.h"


// 생성
XMapForm* XMapForm::CreateOne(CWnd* pParent)
{
	XMapForm* pForm = new XMapForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}


// XMapForm 대화 상자

IMPLEMENT_DYNAMIC(XMapForm, CFormView)

XMapForm::XMapForm()
	: CFormView(IDD_XMapForm)
{

}

XMapForm::~XMapForm()
{
}

void XMapForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTURE_LIST, m_MapTexture);
}


BEGIN_MESSAGE_MAP(XMapForm, CFormView)
	ON_BN_CLICKED(IDC_FIND1, &XMapForm::OnBnClickedFind1)
END_MESSAGE_MAP()


// XMapForm 메시지 처리기

void XMapForm::OnBnClickedFind1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR fileFilter[] = _T("Image (*.BMP, *.GIF, *.JPG, *.PNG, *.DDS) | *.BMP;*.GIF;*.JPG;*.PNG;*.DDS; | All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, fileFilter);

	if (dlg.DoModal() == IDOK)
	{
		m_szMapTexture.push_back(dlg.GetPathName());
	}
	GetDlgItem(IDC_TEXTURE_LIST)->SetWindowText(m_szMapTexture);

	//m_MapTexture.ResetContent();
}
