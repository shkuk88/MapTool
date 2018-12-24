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
	, m_fLocationX(0)
	, m_fLocationY(0)
	, m_fLocationZ(0)
	, m_fRotationX(0)
	, m_fRotationY(0)
	, m_fRotationZ(0)
	, m_fScaleX(0)
	, m_fScaleY(0)
	, m_fScaleZ(0)
{

}

XObjectControlForm::~XObjectControlForm()
{
}

void XObjectControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJ_LIST, m_ObjectList);
	DDX_Text(pDX, IDC_LOCATION_X, m_fLocationX);
	DDX_Text(pDX, IDC_LOCATION_Y, m_fLocationY);
	DDX_Text(pDX, IDC_LOCATION_Z, m_fLocationZ);
	DDX_Text(pDX, IDC_ROTATION_X, m_fRotationX);
	DDX_Text(pDX, IDC_ROTATION_Y, m_fRotationY);
	DDX_Text(pDX, IDC_ROTATION_Z, m_fRotationZ);
	DDX_Text(pDX, IDC_SCALE_X, m_fScaleX);
	DDX_Text(pDX, IDC_SCALE_Y, m_fScaleY);
	DDX_Text(pDX, IDC_SCALE_Z, m_fScaleZ);
}


BEGIN_MESSAGE_MAP(XObjectControlForm, CFormView)
	ON_BN_CLICKED(IDC_ADD_OBJECT, &XObjectControlForm::OnBnClickedAddObject)
	ON_BN_CLICKED(IDC_DEL_OBJECT, &XObjectControlForm::OnBnClickedDelObject)
	ON_BN_CLICKED(IDC_CREATE_OBJECT, &XObjectControlForm::OnBnClickedCreateObject)
	ON_EN_CHANGE(IDC_LOCATION_X, &XObjectControlForm::OnEnChangeLocationX)
	ON_EN_CHANGE(IDC_LOCATION_Y, &XObjectControlForm::OnEnChangeLocationY)
	ON_EN_CHANGE(IDC_LOCATION_Z, &XObjectControlForm::OnEnChangeLocationZ)
	ON_EN_CHANGE(IDC_ROTATION_X, &XObjectControlForm::OnEnChangeRotationX)
	ON_EN_CHANGE(IDC_ROTATION_Y, &XObjectControlForm::OnEnChangeRotationY)
	ON_EN_CHANGE(IDC_ROTATION_Z, &XObjectControlForm::OnEnChangeRotationZ)
	ON_EN_CHANGE(IDC_SCALE_X, &XObjectControlForm::OnEnChangeScaleX)
	ON_EN_CHANGE(IDC_SCALE_Y, &XObjectControlForm::OnEnChangeScaleY)
	ON_EN_CHANGE(IDC_SCALE_Z, &XObjectControlForm::OnEnChangeScaleZ)
END_MESSAGE_MAP()


// XObjectControlForm 메시지 처리기


void XObjectControlForm::OnBnClickedAddObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR fileFilter[] = _T("Object (*.shk) | *.SHK; | All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, fileFilter);

	if (dlg.DoModal() == IDOK)
	{
		for (int iObj = 0; iObj < m_szObjectPath.size(); iObj++)
		{
			if (!m_szObjectPath[iObj].Compare(dlg.GetPathName()))	// 같은 Texture면 그냥 return
			{
				return;
			}
		}
		m_szObjectPath.push_back(dlg.GetPathName());
	}
	else // 확인 외에 다른 버튼을 누를시 리턴
	{
		return;
	}
	m_iSelectObject = m_szObjectPath.size() - 1;
	// 경로 추출한 문자열이 리스트에 보이도록 한다. 경로가 포함 문자열은 따로 가지고 있는다. 
	CString szDelPath = m_szObjectPath[m_iSelectObject].Right(m_szObjectPath[m_iSelectObject].GetLength() - m_szObjectPath[m_iSelectObject].ReverseFind('\\') - 1);
	m_ObjectList.InsertString(m_iSelectObject, szDelPath);
}


void XObjectControlForm::OnBnClickedDelObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iSelectObject = m_ObjectList.GetCurSel();
	if (m_iSelectObject == -1)	return;
	m_ObjectList.DeleteString(m_iSelectObject);

	m_szObjectPath.erase(m_szObjectPath.begin() + m_iSelectObject);

}


void XObjectControlForm::OnBnClickedCreateObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_iSelectObject = m_ObjectList.GetCurSel();
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
 	pApp->m_Sample.GetObjCtrl()->AddObjectWorldMat(pApp->m_Sample.GetObjCtrl()->AddObject(I_Device.m_pD3dDevice.Get(), I_Device.m_pD3dContext.Get(), (TCHAR*)(LPCTSTR)m_szObjectPath[m_iSelectObject]), matWorld);
	pApp->m_Sample.GetObjCtrl()->SelectModeOn();
	pApp->m_Sample.GetObjCtrl()->SetSelectObject((TCHAR*)(LPCTSTR)m_szObjectPath[m_iSelectObject], pApp->m_Sample.GetObjCtrl()->GetLastMatIndex((TCHAR*)(LPCTSTR)m_szObjectPath[m_iSelectObject]));

}


void XObjectControlForm::OnEnChangeLocationX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	GetDlgItemText(IDC_LOCATION_X, m_szGetItem);
	m_fLocationX = _ttof(m_szGetItem);
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void XObjectControlForm::OnEnChangeLocationY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	GetDlgItemText(IDC_LOCATION_Y, m_szGetItem);
	m_fLocationY = _ttof(m_szGetItem);
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void XObjectControlForm::OnEnChangeLocationZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	GetDlgItemText(IDC_LOCATION_Z, m_szGetItem);
	m_fLocationZ = _ttof(m_szGetItem);
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void XObjectControlForm::OnEnChangeRotationX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	GetDlgItemText(IDC_ROTATION_X, m_szGetItem);
	m_fRotationX = _ttof(m_szGetItem);
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void XObjectControlForm::OnEnChangeRotationY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	GetDlgItemText(IDC_ROTATION_Y, m_szGetItem);
	m_fRotationY = _ttof(m_szGetItem);
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void XObjectControlForm::OnEnChangeRotationZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	GetDlgItemText(IDC_ROTATION_Z, m_szGetItem);
	m_fRotationZ = _ttof(m_szGetItem);
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void XObjectControlForm::OnEnChangeScaleX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	GetDlgItemText(IDC_SCALE_X, m_szGetItem);
	m_fScaleX = _ttof(m_szGetItem);
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void XObjectControlForm::OnEnChangeScaleY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	GetDlgItemText(IDC_SCALE_Y, m_szGetItem);
	m_fScaleY = _ttof(m_szGetItem);
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void XObjectControlForm::OnEnChangeScaleZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	GetDlgItemText(IDC_SCALE_Z, m_szGetItem);
	m_fScaleZ = _ttof(m_szGetItem);
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
