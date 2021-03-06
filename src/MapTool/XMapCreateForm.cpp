// XMapForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "XMapCreateForm.h"
#include "afxdialogex.h"


// 생성
XMapCreateForm* XMapCreateForm::CreateOne(CWnd* pParent)
{
	XMapCreateForm* pForm = new XMapCreateForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}


// XMapCreateForm 대화 상자

IMPLEMENT_DYNAMIC(XMapCreateForm, CFormView)

XMapCreateForm::XMapCreateForm()
	: CFormView(IDD_XMapForm)
	, m_fCellCount(0)
	, m_fDistance(1)
{
	m_bOnMap = false;;
	m_iMapTexSelect = -1;
}

XMapCreateForm::~XMapCreateForm()
{
}

void XMapCreateForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTURE_LIST, m_MapTexture);
	DDX_Control(pDX, IDC_TEXTURE_LIST2, m_HeightMapTexture);
	DDX_Text(pDX, IDC_CELL_COUNT, m_fCellCount);
	DDX_Text(pDX, IDC_DISTANCE, m_fDistance);
}


BEGIN_MESSAGE_MAP(XMapCreateForm, CFormView)
	ON_BN_CLICKED(IDC_FIND1, &XMapCreateForm::OnBnClickedFind1)
	ON_LBN_SELCHANGE(IDC_TEXTURE_LIST, &XMapCreateForm::OnLbnSelchangeTextureList)
	ON_BN_CLICKED(IDC_FIND2, &XMapCreateForm::OnBnClickedFind2)
	ON_LBN_SELCHANGE(IDC_TEXTURE_LIST2, &XMapCreateForm::OnLbnSelchangeTextureList2)
	ON_BN_CLICKED(IDC_DEL1, &XMapCreateForm::OnBnClickedDel1)
	ON_BN_CLICKED(IDC_DEL2, &XMapCreateForm::OnBnClickedDel2)
	ON_BN_CLICKED(IDC_CREATE_MAP, &XMapCreateForm::OnBnClickedCreateMap)
	ON_BN_CLICKED(IDC_SAVE, &XMapCreateForm::OnBnClickedSave)
	ON_BN_CLICKED(IDC_Load, &XMapCreateForm::OnBnClickedLoad)
END_MESSAGE_MAP()


// XMapCreateForm 메시지 처리기

void XMapCreateForm::OnBnClickedFind1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR fileFilter[] = _T("Image (*.BMP, *.GIF, *.JPG, *.PNG) | *.BMP;*.GIF;*.JPG;*.PNG; | All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, fileFilter);

	if (dlg.DoModal() == IDOK)
	{
		for (int iTex = 0; iTex < m_szMapTexture.size(); iTex++)
		{
			if (!m_szMapTexture[iTex].Compare(dlg.GetPathName()))	// 같은 Texture면 그냥 return
			{
				return;
			}
		}
		m_szMapTexture.push_back(dlg.GetPathName());
	}
	else // 확인 외에 다른 버튼을 누를시 리턴
	{
		return;
	}
	m_iMapTexSelect = m_szMapTexture.size() - 1;
	// 경로 추출한 문자열이 리스트에 보이도록 한다. 경로가 포함 문자열은 따로 가지고 있는다. 
	CString szDelPath = m_szMapTexture[m_iMapTexSelect].Right(m_szMapTexture[m_iMapTexSelect].GetLength() - m_szMapTexture[m_iMapTexSelect].ReverseFind('\\') - 1);
	m_MapTexture.InsertString(m_iMapTexSelect, szDelPath);
}


void XMapCreateForm::OnLbnSelchangeTextureList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iMapTexSelect = m_MapTexture.GetCurSel();
	if (m_iMapTexSelect == -1)	return; // 리스트 외 부분을 클릭시 오류 방지

	CClientDC dc(GetDlgItem(IDC_MAP_TEXTURE)); // device context for painting
											   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
											   // 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.
	m_img.Destroy();
	m_img.Load(m_szMapTexture[m_iMapTexSelect]);// 이미지 로드
	if (m_img == NULL)
	{
		CString strTemp;
		strTemp.Format(L"잘못된 파일명 입니다.");
		AfxMessageBox(strTemp);
		return;
	}
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_MAP_TEXTURE);
	CRect rtBoxSize;
	staticSize->GetClientRect(rtBoxSize);	// Picture Box 사이즈를 가져온다.
	CRect rtTextureSize;
	rtTextureSize = { 0, 0, m_img.GetWidth(), m_img.GetHeight() };
	SetStretchBltMode(dc.m_hDC, COLORONCOLOR);
	m_img.StretchBlt(dc.m_hDC, rtBoxSize, rtTextureSize, SRCCOPY);
}


void XMapCreateForm::OnBnClickedFind2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR fileFilter[] = _T("Image (*.BMP, *.GIF, *.JPG, *.PNG) | *.BMP;*.GIF;*.JPG;*.PNG;| All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, fileFilter);

	if (dlg.DoModal() == IDOK)
	{
		for (int iTex = 0; iTex < m_szHeightMapTexture.size(); iTex++)
		{
			if (!m_szHeightMapTexture[iTex].Compare(dlg.GetPathName()))	// 같은 Texture면 그냥 return
			{
				return;
			}
		}
		m_szHeightMapTexture.push_back(dlg.GetPathName());
	}
	else
	{
		return;
	}
	m_iHeightMapTexSelect = m_szHeightMapTexture.size() - 1;
	// 경로 추출한 문자열이 리스트에 보이도록 한다. 경로가 포함 문자열은 따로 가지고 있는다. 
	CString szDelPath = m_szHeightMapTexture[m_iHeightMapTexSelect].Right(m_szHeightMapTexture[m_iHeightMapTexSelect].GetLength() - m_szHeightMapTexture[m_iHeightMapTexSelect].ReverseFind('\\') - 1);
	m_HeightMapTexture.InsertString(m_iHeightMapTexSelect, szDelPath);
}


void XMapCreateForm::OnLbnSelchangeTextureList2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iHeightMapTexSelect = m_HeightMapTexture.GetCurSel();
	if (m_iHeightMapTexSelect == -1)	return; // 리스트 외 부분을 클릭시 오류 방지

	CClientDC dc(GetDlgItem(IDC_MAP_TEXTURE2)); // device context for painting
											   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
											   // 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.
	m_img.Destroy();
	m_img.Load(m_szHeightMapTexture[m_iHeightMapTexSelect]);// 이미지 로드
	if (m_img == NULL)
	{
		CString strTemp;
		strTemp.Format(L"잘못된 파일명 입니다.");
		AfxMessageBox(strTemp);
		return;
	}
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_MAP_TEXTURE2);
	CRect rtBoxSize;
	staticSize->GetClientRect(rtBoxSize);	// Picture Box 사이즈를 가져온다.
	CRect rtTextureSize;
	rtTextureSize = { 0, 0, m_img.GetWidth(), m_img.GetHeight() };
	SetStretchBltMode(dc.m_hDC, COLORONCOLOR);
	m_img.StretchBlt(dc.m_hDC, rtBoxSize, rtTextureSize, SRCCOPY);
}


void XMapCreateForm::OnBnClickedDel1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iMapTexSelect = m_MapTexture.GetCurSel();
	if (m_iMapTexSelect == -1)	return;
	m_MapTexture.DeleteString(m_iMapTexSelect);

	m_szMapTexture.erase(m_szMapTexture.begin()+ m_iMapTexSelect);
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_MAP_TEXTURE);
	CRect rtBoxSize;
	staticSize->GetClientRect(rtBoxSize);	// Picture Box 사이즈를 가져온다.
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 255, 255));
	CClientDC dc(GetDlgItem(IDC_MAP_TEXTURE));
	dc.FillRect(rtBoxSize, &brush);
}


void XMapCreateForm::OnBnClickedDel2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iHeightMapTexSelect = m_HeightMapTexture.GetCurSel();
	if (m_iHeightMapTexSelect == -1)	return;
	m_HeightMapTexture.DeleteString(m_iHeightMapTexSelect);

	m_szHeightMapTexture.erase(m_szHeightMapTexture.begin() + m_iHeightMapTexSelect);
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_MAP_TEXTURE2);
	CRect rtBoxSize;
	staticSize->GetClientRect(rtBoxSize);	// Picture Box 사이즈를 가져온다.
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 255, 255));
	CClientDC dc(GetDlgItem(IDC_MAP_TEXTURE2));
	dc.FillRect(rtBoxSize, &brush);
}


void XMapCreateForm::OnBnClickedCreateMap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_bOnMap)	return; // 하나의 맵만 생성할 수 있도록 제한
	if (!m_fCellCount && m_iHeightMapTexSelect == -1) return;
	m_bOnMap = true;
	TCHAR* szMapTexture = NULL;
	TCHAR* szHeightMapTexture = NULL;
	TCHAR szCopyTexture[MAX_PATH] = { 0 };
	if (m_iMapTexSelect != -1)
	{
		_tcscpy_s(szCopyTexture, MAX_PATH, m_szMapTexture[m_iMapTexSelect].GetBuffer(0));
		m_szMapTexture[m_iMapTexSelect].ReleaseBuffer();
		szMapTexture = new TCHAR[MAX_PATH];
		_tcscpy_s(szMapTexture, MAX_PATH, szCopyTexture);
	}
	if (!m_fCellCount)
	{
		if (m_iHeightMapTexSelect != -1)
		{
			_tcscpy_s(szCopyTexture, MAX_PATH, m_szHeightMapTexture[m_iHeightMapTexSelect].GetBuffer(0));
			m_szHeightMapTexture[m_iHeightMapTexSelect].ReleaseBuffer();
			szHeightMapTexture = new TCHAR[MAX_PATH];
			_tcscpy_s(szHeightMapTexture, MAX_PATH, szCopyTexture);
		}
	}
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Sample.CreateMap(szMapTexture, szHeightMapTexture, m_fCellCount, m_fDistance);
}

void XMapCreateForm::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Sample.GetExporter()->ExportStandard(pApp->m_Sample.GetMap()->m_iRow, pApp->m_Sample.GetMap()->m_fDistance);
	pApp->m_Sample.GetExporter()->ExportSpreatTexture(I_SpreatCtrl.GetSpreatTex());
	pApp->m_Sample.GetExporter()->ExportVertexData(&pApp->m_Sample.GetMap()->m_VertexList, &pApp->m_Sample.GetMap()->m_dwIndexList);
	if(!pApp->m_Sample.GetExporter()->Export())
		AfxMessageBox(_T("Export Error"));
}

void XMapCreateForm::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	if (!pApp->m_Sample.GetImporter()->Import())
		AfxMessageBox(_T("Import Error"));
	m_bOnMap = true;
	pApp->m_Sample.CreateMap();
}