// XMapControlForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "XMapControlForm.h"
#include "afxdialogex.h"

// 생성
XMapControlForm* XMapControlForm::CreateOne(CWnd* pParent)
{
	XMapControlForm* pForm = new XMapControlForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

// XMapControlForm 대화 상자

IMPLEMENT_DYNAMIC(XMapControlForm, CFormView)

XMapControlForm::XMapControlForm()
	: CFormView(IDD_MAP_CONTROL)
{

}

XMapControlForm::~XMapControlForm()
{
}

void XMapControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(XMapControlForm, CFormView)
END_MESSAGE_MAP()


// XMapControlForm 메시지 처리기
