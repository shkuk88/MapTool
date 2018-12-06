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
	m_bHeightCtrlState = None;
}

XMapControlForm::~XMapControlForm()
{
}

void XMapControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(XMapControlForm, CFormView)
	ON_BN_CLICKED(IDC_UP_VECTOR, &XMapControlForm::OnBnClickedUpVector)
	ON_BN_CLICKED(IDC_DOWN_VECTOR, &XMapControlForm::OnBnClickedDownVector)
	ON_BN_CLICKED(IDC_FLAT_VECTOR, &XMapControlForm::OnBnClickedFlatVector)
	ON_BN_CLICKED(IDC_CONTROL_APPLY, &XMapControlForm::OnBnClickedControlApply)
	ON_BN_CLICKED(IDC_CONTROL_OFF, &XMapControlForm::OnBnClickedControlOff)
END_MESSAGE_MAP()


// XMapControlForm 메시지 처리기


void XMapControlForm::OnBnClickedUpVector()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bHeightCtrlState = Up;
}


void XMapControlForm::OnBnClickedDownVector()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bHeightCtrlState = Down;
}


void XMapControlForm::OnBnClickedFlatVector()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bHeightCtrlState = Flat;
}


void XMapControlForm::OnBnClickedControlOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Sample.GetHeightCtrl()->SetHeightCtrlState(None);
}


void XMapControlForm::OnBnClickedControlApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Core로 넘겨주는 역할
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Sample.GetHeightCtrl()->SetHeightCtrlState((HeightControl)m_bHeightCtrlState);
}



