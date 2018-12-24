// XObjectControlForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "XObjectControlForm.h"
#include "afxdialogex.h"

// 생성
XObjectControlForm* XObjectControlForm::CreateOne(CWnd* pParent)
{
	XObjectControlForm* pForm = new XObjectControlForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}
// XObjectControlForm 대화 상자

IMPLEMENT_DYNAMIC(XObjectControlForm, CFormView)

XObjectControlForm::XObjectControlForm()
	: CFormView(IDD_OBJECT_FORM)
{

}

XObjectControlForm::~XObjectControlForm()
{
}

void XObjectControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(XObjectControlForm, CFormView)
END_MESSAGE_MAP()


// XObjectControlForm 메시지 처리기
