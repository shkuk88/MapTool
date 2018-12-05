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
}


BEGIN_MESSAGE_MAP(XMapForm, CFormView)
END_MESSAGE_MAP()


// XMapForm 메시지 처리기