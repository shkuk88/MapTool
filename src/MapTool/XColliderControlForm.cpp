// XColliderControlForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "XColliderControlForm.h"
#include "afxdialogex.h"

// 생성
XColliderControlForm* XColliderControlForm::CreateOne(CWnd* pParent)
{
	XColliderControlForm* pForm = new XColliderControlForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

// XColliderControlForm 대화 상자

IMPLEMENT_DYNAMIC(XColliderControlForm, CFormView)

XColliderControlForm::XColliderControlForm()
	: CFormView(IDD_COLLIDER_CONTROL)
{

}

XColliderControlForm::~XColliderControlForm()
{
}

void XColliderControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(XColliderControlForm, CFormView)
END_MESSAGE_MAP()


// XColliderControlForm 메시지 처리기
