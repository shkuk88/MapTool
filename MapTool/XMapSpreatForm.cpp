// XMapSpreatForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "XMapSpreatForm.h"
#include "afxdialogex.h"



// 생성
XMapSpreatForm* XMapSpreatForm::CreateOne(CWnd* pParent)
{
	XMapSpreatForm* pForm = new XMapSpreatForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

// XMapSpreatForm 대화 상자

IMPLEMENT_DYNAMIC(XMapSpreatForm, CFormView)

XMapSpreatForm::XMapSpreatForm()
	: CFormView(IDD_MAP_SPREAT)
	, m_bSpreatSwitch(FALSE)
	, m_fRadius(1)
{

}

XMapSpreatForm::~XMapSpreatForm()
{
}

void XMapSpreatForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_SPREAT_SWITCH, m_bSpreatSwitch);
	DDX_Text(pDX, IDC_SPREAT_RADIUS, m_fRadius);
}


BEGIN_MESSAGE_MAP(XMapSpreatForm, CFormView)
	ON_BN_CLICKED(IDC_ALPHA, &XMapSpreatForm::OnBnClickedAlpha)
	ON_BN_CLICKED(IDC_TEXTURE, &XMapSpreatForm::OnBnClickedTexture)
	ON_BN_CLICKED(IDC_SPREAT_SWITCH, &XMapSpreatForm::OnBnClickedSpreatSwitch)
	ON_BN_CLICKED(IDC_RADIUS_APPLY, &XMapSpreatForm::OnBnClickedRadiusApply)
	ON_BN_CLICKED(IDC_SPREAT_COLOR_0, &XMapSpreatForm::OnBnClickedSpreatColor0)
	ON_BN_CLICKED(IDC_SPREAT_COLOR_1, &XMapSpreatForm::OnBnClickedSpreatColor1)
	ON_BN_CLICKED(IDC_SPREAT_COLOR_2, &XMapSpreatForm::OnBnClickedSpreatColor2)
	ON_BN_CLICKED(IDC_SPREAT_COLOR_3, &XMapSpreatForm::OnBnClickedSpreatColor3)
END_MESSAGE_MAP()


// XMapSpreatForm 메시지 처리기


void XMapSpreatForm::OnBnClickedAlpha()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Sample.GetSpreatCtrl()->SetSpreatViewState(SpreatView_Alpha);
}


void XMapSpreatForm::OnBnClickedTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Sample.GetSpreatCtrl()->SetSpreatViewState(SpreatView_Texture);
}


void XMapSpreatForm::OnBnClickedSpreatSwitch()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Sample.GetSpreatCtrl()->SetEnable((BOOL)m_bSpreatSwitch);
	if (m_bSpreatSwitch == TRUE)
	{
		pApp->m_Sample.GetHeightCtrl()->SetHeightCtrlState(None);
	}
}


void XMapSpreatForm::OnBnClickedRadiusApply()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Sample.GetSpreatCtrl()->SetRadius(m_fRadius);
}


void XMapSpreatForm::OnBnClickedSpreatColor0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Sample.GetSpreatCtrl()->SetSpreatColor(Spreat_Red);
}


void XMapSpreatForm::OnBnClickedSpreatColor1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Sample.GetSpreatCtrl()->SetSpreatColor(Spreat_Green);
}


void XMapSpreatForm::OnBnClickedSpreatColor2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Sample.GetSpreatCtrl()->SetSpreatColor(Spreat_Blue);
}


void XMapSpreatForm::OnBnClickedSpreatColor3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Sample.GetSpreatCtrl()->SetSpreatColor(Spreat_Alpha);
}
