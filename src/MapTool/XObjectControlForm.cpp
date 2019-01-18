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
	, m_bSetCollider(false)
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
	DDX_Control(pDX, IDC_ROTATION_X2, m_SliderRotateX);
	DDX_Control(pDX, IDC_ROTATION_Y2, m_SliderRotateY);
	DDX_Control(pDX, IDC_ROTATION_Z2, m_SliderRotateZ);
	DDX_Check(pDX, IDC_SET_COLLIDER, m_bSetCollider);
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
	ON_BN_CLICKED(IDC_COMPLETE, &XObjectControlForm::OnBnClickedComplete)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ROTATION_X2, &XObjectControlForm::OnNMCustomdrawRotationX2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ROTATION_Y2, &XObjectControlForm::OnNMCustomdrawRotationY2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ROTATION_Z2, &XObjectControlForm::OnNMCustomdrawRotationZ2)
	ON_BN_CLICKED(IDC_Cancel, &XObjectControlForm::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SET_COLLIDER, &XObjectControlForm::OnBnClickedSetCollider)
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
	if (I_ObjectCtrl.m_bSelect) return;
	I_HeightCtrl.SetHeightCtrlState(None);
	I_SpreatCtrl.SetEnable(FALSE);
	UpdateData(TRUE);
	m_iSelectObject = m_ObjectList.GetCurSel();
	if (m_iSelectObject == -1)	
		return;
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	// 경로 추출한 문자열이 리스트에 보이도록 한다. 경로가 포함 문자열은 따로 가지고 있는다. 
	CString szDelPath = m_szObjectPath[m_iSelectObject].Right(m_szObjectPath[m_iSelectObject].GetLength() - m_szObjectPath[m_iSelectObject].ReverseFind('\\') - 1);

	I_ObjectCtrl.AddObjectWorldMat(I_ObjectCtrl.AddObject(I_Device.m_pD3dDevice.Get(), I_Device.m_pD3dContext.Get(), (TCHAR*)(LPCTSTR)szDelPath), matWorld);
	I_ObjectCtrl.SelectModeOn();
	I_ObjectCtrl.SetSelectObject((TCHAR*)(LPCTSTR)szDelPath, I_ObjectCtrl.GetLastMatIndex((TCHAR*)(LPCTSTR)szDelPath));
	D3DXVECTOR3 vScale, vLocation;
	D3DXQUATERNION qRotation;
	D3DXMatrixDecompose(&vScale, &qRotation, &vLocation, &matWorld);
	m_fScaleX = vScale.x;
	m_fScaleY = vScale.y;
	m_fScaleZ = vScale.z;
	m_fRotationX = qRotation.x;
	m_fRotationY = qRotation.y;
	m_fRotationZ = qRotation.z;
	m_fLocationX = vLocation.x;
	m_fLocationY = vLocation.y;
	m_fLocationZ = vLocation.z;
	m_SliderRotateX.SetPos(0);
	m_SliderRotateY.SetPos(0);
	m_SliderRotateZ.SetPos(0);
	UpdateData(FALSE);
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

	if(I_ObjectCtrl.m_bSelect)
	{
		I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum]._41 = m_fLocationX;
	}
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

	if (I_ObjectCtrl.m_bSelect)
	{
		I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum]._42 = m_fLocationY;
	}
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

	if (I_ObjectCtrl.m_bSelect)
	{
		I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum]._43 = m_fLocationZ;
	}
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
	float fDegreeX = D3DXToRadian(m_fRotationX);

	GetDlgItemText(IDC_ROTATION_Y, m_szGetItem);
	m_fRotationY = _ttof(m_szGetItem);
	float fDegreeY = D3DXToRadian(m_fRotationY);

	GetDlgItemText(IDC_ROTATION_Z, m_szGetItem);
	m_fRotationZ = _ttof(m_szGetItem);
	float fDegreeZ = D3DXToRadian(m_fRotationZ);

	if (I_ObjectCtrl.m_bSelect)
	{
		D3DXVECTOR3 vScale, vLocation;
		D3DXQUATERNION qRotation;
		D3DXMATRIX matWorld, matRotation, matScale;
		D3DXMatrixDecompose(&vScale, &qRotation, &vLocation, &I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum]);
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationYawPitchRoll(&matRotation, fDegreeY, fDegreeX, fDegreeZ);
		matWorld = matScale * matRotation;
		matWorld._41 = vLocation.x;
		matWorld._42 = vLocation.y;
		matWorld._43 = vLocation.z;
		I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum] = matWorld;
	}
	m_SliderRotateX.SetPos((float)m_fRotationX);
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
	GetDlgItemText(IDC_ROTATION_X, m_szGetItem);
	m_fRotationX = _ttof(m_szGetItem);
	float fDegreeX = D3DXToRadian(m_fRotationX);

	GetDlgItemText(IDC_ROTATION_Y, m_szGetItem);
	m_fRotationY = _ttof(m_szGetItem);
	float fDegreeY = D3DXToRadian(m_fRotationY);

	GetDlgItemText(IDC_ROTATION_Z, m_szGetItem);
	m_fRotationZ = _ttof(m_szGetItem);
	float fDegreeZ = D3DXToRadian(m_fRotationZ);

	if (I_ObjectCtrl.m_bSelect)
	{
		D3DXVECTOR3 vScale, vLocation;
		D3DXQUATERNION qRotation;
		D3DXMATRIX matWorld, matRotation, matScale;
		D3DXMatrixDecompose(&vScale, &qRotation, &vLocation, &I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum]);
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationYawPitchRoll(&matRotation, fDegreeY, fDegreeX, fDegreeZ);
		matWorld = matScale * matRotation;
		matWorld._41 = vLocation.x;
		matWorld._42 = vLocation.y;
		matWorld._43 = vLocation.z;
		I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum] = matWorld;
	}
	m_SliderRotateY.SetPos((float)m_fRotationY);
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
	GetDlgItemText(IDC_ROTATION_X, m_szGetItem);
	m_fRotationX = _ttof(m_szGetItem);
	float fDegreeX = D3DXToRadian(m_fRotationX);

	GetDlgItemText(IDC_ROTATION_Y, m_szGetItem);
	m_fRotationY = _ttof(m_szGetItem);
	float fDegreeY = D3DXToRadian(m_fRotationY);

	GetDlgItemText(IDC_ROTATION_Z, m_szGetItem);
	m_fRotationZ = _ttof(m_szGetItem);
	float fDegreeZ = D3DXToRadian(m_fRotationZ);

	if (I_ObjectCtrl.m_bSelect)
	{
		D3DXVECTOR3 vScale, vLocation;
		D3DXQUATERNION qRotation;
		D3DXMATRIX matWorld, matRotation, matScale;
		D3DXMatrixDecompose(&vScale, &qRotation, &vLocation, &I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum]);
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationYawPitchRoll(&matRotation, fDegreeY, fDegreeX, fDegreeZ);
		matWorld = matScale * matRotation;
		matWorld._41 = vLocation.x;
		matWorld._42 = vLocation.y;
		matWorld._43 = vLocation.z;
		I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum] = matWorld;
	}
	m_SliderRotateZ.SetPos((float)m_fRotationZ);
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

	if (I_ObjectCtrl.m_bSelect)
	{
		D3DXVECTOR3 vScale, vLocation;
		D3DXQUATERNION qRotation;
		D3DXMATRIX matWorld, matRotation, matScale;
		D3DXMatrixDecompose(&vScale, &qRotation, &vLocation, &I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum]);
		D3DXMatrixScaling(&matScale, m_fScaleX, vScale.y, vScale.z);
		D3DXMatrixRotationQuaternion(&matRotation, &qRotation);
		matWorld = matScale * matRotation;
		matWorld._41 = vLocation.x;
		matWorld._42 = vLocation.y;
		matWorld._43 = vLocation.z;
		I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum] = matWorld;
	}
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

	if (I_ObjectCtrl.m_bSelect)
	{
		D3DXVECTOR3 vScale, vLocation;
		D3DXQUATERNION qRotation;
		D3DXMATRIX matWorld, matRotation, matScale;
		D3DXMatrixDecompose(&vScale, &qRotation, &vLocation, &I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum]);
		D3DXMatrixScaling(&matScale, vScale.x, m_fScaleY, vScale.z);
		D3DXMatrixRotationQuaternion(&matRotation, &qRotation);
		matWorld = matScale * matRotation;
		matWorld._41 = vLocation.x;
		matWorld._42 = vLocation.y;
		matWorld._43 = vLocation.z;
		I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum] = matWorld;
	}
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

	if (I_ObjectCtrl.m_bSelect)
	{
		D3DXVECTOR3 vScale, vLocation;
		D3DXQUATERNION qRotation;
		D3DXMATRIX matWorld, matRotation, matScale;
		D3DXMatrixDecompose(&vScale, &qRotation, &vLocation, &I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum]);
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, m_fScaleZ);
		D3DXMatrixRotationQuaternion(&matRotation, &qRotation);
		matWorld = matScale * matRotation;
		matWorld._41 = vLocation.x;
		matWorld._42 = vLocation.y;
		matWorld._43 = vLocation.z;
		I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum] = matWorld;
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void XObjectControlForm::OnBnClickedComplete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	I_ObjectCtrl.m_bSelect = false;
}


void XObjectControlForm::OnNMCustomdrawRotationX2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	m_fRotationX = m_SliderRotateX.GetPos();
	float fDegreeX = D3DXToRadian(m_fRotationX);

	GetDlgItemText(IDC_ROTATION_Y, m_szGetItem);
	m_fRotationY = _ttof(m_szGetItem);
	float fDegreeY = D3DXToRadian(m_fRotationY);

	GetDlgItemText(IDC_ROTATION_Z, m_szGetItem);
	m_fRotationZ = _ttof(m_szGetItem);
	float fDegreeZ = D3DXToRadian(m_fRotationZ);
	if (I_ObjectCtrl.m_bSelect)
	{
		D3DXVECTOR3 vScale, vLocation;
		D3DXQUATERNION qRotation;
		D3DXMATRIX matWorld, matRotation, matScale;
		D3DXMatrixDecompose(&vScale, &qRotation, &vLocation, &I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum]);
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationYawPitchRoll(&matRotation, fDegreeY, fDegreeX, fDegreeZ);
		matWorld = matScale * matRotation;
		matWorld._41 = vLocation.x;
		matWorld._42 = vLocation.y;
		matWorld._43 = vLocation.z;
		I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum] = matWorld;
	}
	UpdateData(FALSE);
}


void XObjectControlForm::OnNMCustomdrawRotationY2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	
	GetDlgItemText(IDC_ROTATION_X, m_szGetItem);
	m_fRotationX = _ttof(m_szGetItem);
	float fDegreeX = D3DXToRadian(m_fRotationX);

	m_fRotationY = m_SliderRotateY.GetPos();
	float fDegreeY = D3DXToRadian(m_fRotationY);

	GetDlgItemText(IDC_ROTATION_Z, m_szGetItem);
	m_fRotationZ = _ttof(m_szGetItem);
	float fDegreeZ = D3DXToRadian(m_fRotationZ);
	if (I_ObjectCtrl.m_bSelect)
	{
		D3DXVECTOR3 vScale, vLocation;
		D3DXQUATERNION qRotation;
		D3DXMATRIX matWorld, matRotation, matScale;
		D3DXMatrixDecompose(&vScale, &qRotation, &vLocation, &I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum]);
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationYawPitchRoll(&matRotation, fDegreeY, fDegreeX, fDegreeZ);
		matWorld = matScale * matRotation;
		matWorld._41 = vLocation.x;
		matWorld._42 = vLocation.y;
		matWorld._43 = vLocation.z;
		I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum] = matWorld;
	}
	UpdateData(FALSE);
}


void XObjectControlForm::OnNMCustomdrawRotationZ2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);
	GetDlgItemText(IDC_ROTATION_X, m_szGetItem);
	m_fRotationX = _ttof(m_szGetItem);
	float fDegreeX = D3DXToRadian(m_fRotationX);

	GetDlgItemText(IDC_ROTATION_Y, m_szGetItem);
	m_fRotationY = _ttof(m_szGetItem);
	float fDegreeY = D3DXToRadian(m_fRotationY);

	m_fRotationZ = m_SliderRotateZ.GetPos();
	float fDegreeZ = D3DXToRadian(m_fRotationZ);
	if (I_ObjectCtrl.m_bSelect)
	{
		D3DXVECTOR3 vScale, vLocation;
		D3DXQUATERNION qRotation;
		D3DXMATRIX matWorld, matRotation, matScale;
		D3DXMatrixDecompose(&vScale, &qRotation, &vLocation, &I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum]);
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationYawPitchRoll(&matRotation, fDegreeY, fDegreeX, fDegreeZ);
		matWorld = matScale * matRotation;
		matWorld._41 = vLocation.x;
		matWorld._42 = vLocation.y;
		matWorld._43 = vLocation.z;
		I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject].Matrix[I_ObjectCtrl.m_iSelectMatNum] = matWorld;
	}
	UpdateData(FALSE);
}


void XObjectControlForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_SliderRotateX.SetRange(-180.0f, 180.0f);
	m_SliderRotateY.SetRange(-180.0f, 180.0f);
	m_SliderRotateZ.SetRange(-180.0f, 180.0f);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void XObjectControlForm::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	I_Object.DelObjWorldMat(I_ObjectCtrl.m_szSelectObject, I_ObjectCtrl.m_iSelectMatNum);
	I_ObjectCtrl.m_bSelect = false;
}


void XObjectControlForm::OnBnClickedSetCollider()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	I_ObjectCtrl.SetColliderSwitch(m_bSetCollider);
}
