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
	ON_BN_CLICKED(IDC_FIND_RED_TEXTURE, &XMapSpreatForm::OnBnClickedFindRedTexture)
	ON_BN_CLICKED(IDC_FIND_GREEN_TEXTURE, &XMapSpreatForm::OnBnClickedFindGreenTexture)
	ON_BN_CLICKED(IDC_FIND_BLUE_TEXTURE, &XMapSpreatForm::OnBnClickedFindBlueTexture)
	ON_BN_CLICKED(IDC_FIND_ALPHA_TEXTURE, &XMapSpreatForm::OnBnClickedFindAlphaTexture)
	ON_BN_CLICKED(IDC_MODE1, &XMapSpreatForm::OnBnClickedMode1)
	ON_BN_CLICKED(IDC_MODE2, &XMapSpreatForm::OnBnClickedMode2)
END_MESSAGE_MAP()


// XMapSpreatForm 메시지 처리기


void XMapSpreatForm::OnBnClickedAlpha()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	I_SpreatCtrl.SetSpreatViewState(SpreatView_Alpha);
}


void XMapSpreatForm::OnBnClickedTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	I_SpreatCtrl.SetSpreatViewState(SpreatView_Texture);
}


void XMapSpreatForm::OnBnClickedSpreatSwitch()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	I_SpreatCtrl.SetEnable((BOOL)m_bSpreatSwitch);
	if (m_bSpreatSwitch == TRUE)
	{
		I_HeightCtrl.SetHeightCtrlState(None);
	}
}


void XMapSpreatForm::OnBnClickedRadiusApply()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	I_SpreatCtrl.SetRadius(m_fRadius);
}


void XMapSpreatForm::OnBnClickedSpreatColor0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	I_SpreatCtrl.SetSpreatColor(Spreat_Red);
}


void XMapSpreatForm::OnBnClickedSpreatColor1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	I_SpreatCtrl.SetSpreatColor(Spreat_Green);
}


void XMapSpreatForm::OnBnClickedSpreatColor2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	I_SpreatCtrl.SetSpreatColor(Spreat_Blue);
}


void XMapSpreatForm::OnBnClickedSpreatColor3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	I_SpreatCtrl.SetSpreatColor(Spreat_Alpha);
}


void XMapSpreatForm::OnBnClickedFindRedTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR fileFilter[] = _T("Image (*.BMP, *.GIF, *.JPG, *.PNG) | *.BMP;*.GIF;*.JPG;*.PNG;| All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, fileFilter);

	if (dlg.DoModal() == IDOK)
	{
		m_szAlphaTexture[Spreat_Red] = dlg.GetPathName();;
	}
	else
	{
		return;
	}
	CClientDC dc(GetDlgItem(IDC_RED_IMAGE)); // device context for painting
												// TODO: 여기에 메시지 처리기 코드를 추가합니다.
												// 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.
	m_img.Destroy();
	m_img.Load(m_szAlphaTexture[Spreat_Red]);// 이미지 로드
	if (m_img == NULL)
	{
		CString strTemp;
		strTemp.Format(L"잘못된 파일명 입니다.");
		AfxMessageBox(strTemp);
		return;
	}
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_RED_IMAGE);
	CRect rtBoxSize;
	staticSize->GetClientRect(rtBoxSize);	// Picture Box 사이즈를 가져온다.
	CRect rtTextureSize;
	rtTextureSize = { 0, 0, m_img.GetWidth(), m_img.GetHeight() };
	SetStretchBltMode(dc.m_hDC, COLORONCOLOR);
	m_img.StretchBlt(dc.m_hDC, rtBoxSize, rtTextureSize, SRCCOPY);
	
	CString szDelPath = m_szAlphaTexture[Spreat_Red].Right(m_szAlphaTexture[Spreat_Red].GetLength() - m_szAlphaTexture[Spreat_Red].ReverseFind('\\') - 1);
	CString szAddPath = _T("../../Data/Map/Texture/") + szDelPath;
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	I_SpreatCtrl.RGBA_TextureLoad(I_Device.m_pD3dDevice.Get(), m_szAlphaTexture[Spreat_Red], Spreat_Red);
	pApp->m_Sample.GetExporter()->ExportAlphaTexture((TCHAR*)(LPCTSTR)szAddPath, Spreat_Red);
}


void XMapSpreatForm::OnBnClickedFindGreenTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR fileFilter[] = _T("Image (*.BMP, *.GIF, *.JPG, *.PNG) | *.BMP;*.GIF;*.JPG;*.PNG;| All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, fileFilter);

	if (dlg.DoModal() == IDOK)
	{
		m_szAlphaTexture[Spreat_Green] = dlg.GetPathName();;
	}
	else
	{
		return;
	}
	CClientDC dc(GetDlgItem(IDC_GREEN_IMAGE)); // device context for painting
											 // TODO: 여기에 메시지 처리기 코드를 추가합니다.
											 // 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.
	m_img.Destroy();
	m_img.Load(m_szAlphaTexture[Spreat_Green]);// 이미지 로드
	if (m_img == NULL)
	{
		CString strTemp;
		strTemp.Format(L"잘못된 파일명 입니다.");
		AfxMessageBox(strTemp);
		return;
	}
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_GREEN_IMAGE);
	CRect rtBoxSize;
	staticSize->GetClientRect(rtBoxSize);	// Picture Box 사이즈를 가져온다.
	CRect rtTextureSize;
	rtTextureSize = { 0, 0, m_img.GetWidth(), m_img.GetHeight() };
	SetStretchBltMode(dc.m_hDC, COLORONCOLOR);
	m_img.StretchBlt(dc.m_hDC, rtBoxSize, rtTextureSize, SRCCOPY);

	CString szDelPath = m_szAlphaTexture[Spreat_Green].Right(m_szAlphaTexture[Spreat_Green].GetLength() - m_szAlphaTexture[Spreat_Green].ReverseFind('\\') - 1);
	CString szAddPath = _T("../../Data/Map/Texture/") + szDelPath;
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	I_SpreatCtrl.RGBA_TextureLoad(I_Device.m_pD3dDevice.Get(), m_szAlphaTexture[Spreat_Green], Spreat_Green);
	pApp->m_Sample.GetExporter()->ExportAlphaTexture((TCHAR*)(LPCTSTR)szAddPath, Spreat_Green);
}


void XMapSpreatForm::OnBnClickedFindBlueTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR fileFilter[] = _T("Image (*.BMP, *.GIF, *.JPG, *.PNG) | *.BMP;*.GIF;*.JPG;*.PNG;| All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, fileFilter);

	if (dlg.DoModal() == IDOK)
	{
		m_szAlphaTexture[Spreat_Blue] = dlg.GetPathName();;
	}
	else
	{
		return;
	}
	CClientDC dc(GetDlgItem(IDC_BLUE_IMAGE)); // device context for painting
											   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
											   // 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.
	m_img.Destroy();
	m_img.Load(m_szAlphaTexture[Spreat_Blue]);// 이미지 로드
	if (m_img == NULL)
	{
		CString strTemp;
		strTemp.Format(L"잘못된 파일명 입니다.");
		AfxMessageBox(strTemp);
		return;
	}
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_BLUE_IMAGE);
	CRect rtBoxSize;
	staticSize->GetClientRect(rtBoxSize);	// Picture Box 사이즈를 가져온다.
	CRect rtTextureSize;
	rtTextureSize = { 0, 0, m_img.GetWidth(), m_img.GetHeight() };
	SetStretchBltMode(dc.m_hDC, COLORONCOLOR);
	m_img.StretchBlt(dc.m_hDC, rtBoxSize, rtTextureSize, SRCCOPY);

	CString szDelPath = m_szAlphaTexture[Spreat_Blue].Right(m_szAlphaTexture[Spreat_Blue].GetLength() - m_szAlphaTexture[Spreat_Blue].ReverseFind('\\') - 1);
	CString szAddPath = _T("../../Data/Map/Texture/") + szDelPath;
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	I_SpreatCtrl.RGBA_TextureLoad(I_Device.m_pD3dDevice.Get(), m_szAlphaTexture[Spreat_Blue], Spreat_Blue);
	pApp->m_Sample.GetExporter()->ExportAlphaTexture((TCHAR*)(LPCTSTR)szAddPath, Spreat_Blue);
}


void XMapSpreatForm::OnBnClickedFindAlphaTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR fileFilter[] = _T("Image (*.BMP, *.GIF, *.JPG, *.PNG) | *.BMP;*.GIF;*.JPG;*.PNG;| All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, fileFilter);

	if (dlg.DoModal() == IDOK)
	{
		m_szAlphaTexture[Spreat_Alpha] = dlg.GetPathName();;
	}
	else
	{
		return;
	}
	CClientDC dc(GetDlgItem(IDC_ALPHA_IMAGE)); // device context for painting
											  // TODO: 여기에 메시지 처리기 코드를 추가합니다.
											  // 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.
	m_img.Destroy();
	m_img.Load(m_szAlphaTexture[Spreat_Alpha]);// 이미지 로드
	if (m_img == NULL)
	{
		CString strTemp;
		strTemp.Format(L"잘못된 파일명 입니다.");
		AfxMessageBox(strTemp);
		return;
	}
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_ALPHA_IMAGE);
	CRect rtBoxSize;
	staticSize->GetClientRect(rtBoxSize);	// Picture Box 사이즈를 가져온다.
	CRect rtTextureSize;
	rtTextureSize = { 0, 0, m_img.GetWidth(), m_img.GetHeight() };
	SetStretchBltMode(dc.m_hDC, COLORONCOLOR);
	m_img.StretchBlt(dc.m_hDC, rtBoxSize, rtTextureSize, SRCCOPY);

	CString szDelPath = m_szAlphaTexture[Spreat_Alpha].Right(m_szAlphaTexture[Spreat_Alpha].GetLength() - m_szAlphaTexture[Spreat_Alpha].ReverseFind('\\') - 1);
	CString szAddPath = _T("../../Data/Map/Texture/") + szDelPath;
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	I_SpreatCtrl.RGBA_TextureLoad(I_Device.m_pD3dDevice.Get(), m_szAlphaTexture[Spreat_Alpha], Spreat_Alpha);
	pApp->m_Sample.GetExporter()->ExportAlphaTexture((TCHAR*)(LPCTSTR)szAddPath, Spreat_Alpha);
}


void XMapSpreatForm::OnBnClickedMode1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	I_SpreatCtrl.SetSpreatMode(SpreatMode_Basic);
}


void XMapSpreatForm::OnBnClickedMode2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	I_SpreatCtrl.SetSpreatMode(SpreatMode_Gradation);
}
