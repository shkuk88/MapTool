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
	, m_bColliderType(Complete)
	, m_fOBBLocationX(0)
	, m_fOBBLocationY(0)
	, m_fOBBLocationZ(0)
	, m_fOBBRotationX(0)
	, m_fOBBRotationY(0)
	, m_fOBBRotationZ(0)
	, m_fOBBScaleX(1)
	, m_fOBBScaleY(1)
	, m_fOBBScaleZ(1)
{

}

XColliderControlForm::~XColliderControlForm()
{
}

void XColliderControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_OBB, m_bColliderType);
	DDX_Text(pDX, IDC_OBB_LOCATION_X, m_fOBBLocationX);
	DDX_Text(pDX, IDC_OBB_LOCATION_Y, m_fOBBLocationY);
	DDX_Text(pDX, IDC_OBB_LOCATION_Z, m_fOBBLocationZ);
	DDX_Text(pDX, IDC_OBB_ROTATION_X, m_fOBBRotationX);
	DDX_Text(pDX, IDC_OBB_ROTATION_Y, m_fOBBRotationY);
	DDX_Text(pDX, IDC_OBB_ROTATION_Z, m_fOBBRotationZ);
	DDX_Text(pDX, IDC_OBB_SCALE_X, m_fOBBScaleX);
	DDX_Text(pDX, IDC_OBB_SCALE_Y, m_fOBBScaleY);
	DDX_Text(pDX, IDC_OBB_SCALE_Z, m_fOBBScaleZ);
	DDX_Control(pDX, IDC_OBB_LOCATION_X2, m_OBBSliderLocationX);
	DDX_Control(pDX, IDC_OBB_LOCATION_Y2, m_OBBSliderLocationY);
	DDX_Control(pDX, IDC_OBB_LOCATION_Z2, m_OBBSliderLocationZ);
	DDX_Control(pDX, IDC_OBB_ROTATION_X2, m_OBBRotationX);
	DDX_Control(pDX, IDC_OBB_ROTATION_Y2, m_OBBRotationY);
	DDX_Control(pDX, IDC_OBB_ROTATION_Z2, m_OBBRotationZ);
	DDX_Control(pDX, IDC_OBB_SCALE_X2, m_OBBScaleX);
	DDX_Control(pDX, IDC_OBB_SCALE_Y2, m_OBBScaleY);
	DDX_Control(pDX, IDC_OBB_SCALE_Z2, m_OBBScaleZ);
}


BEGIN_MESSAGE_MAP(XColliderControlForm, CFormView)
	ON_BN_CLICKED(IDC_COLLIDER_CREATE, &XColliderControlForm::OnBnClickedColliderCreate)
	ON_EN_CHANGE(IDC_OBB_LOCATION_X, &XColliderControlForm::OnEnChangeObbLocationX)
	ON_EN_CHANGE(IDC_OBB_LOCATION_Y, &XColliderControlForm::OnEnChangeObbLocationY)
	ON_EN_CHANGE(IDC_OBB_LOCATION_Z, &XColliderControlForm::OnEnChangeObbLocationZ)
	ON_EN_CHANGE(IDC_OBB_ROTATION_X, &XColliderControlForm::OnEnChangeObbRotationX)
	ON_EN_CHANGE(IDC_OBB_ROTATION_Y, &XColliderControlForm::OnEnChangeObbRotationY)
	ON_EN_CHANGE(IDC_OBB_ROTATION_Z, &XColliderControlForm::OnEnChangeObbRotationZ)
	ON_EN_CHANGE(IDC_OBB_SCALE_X, &XColliderControlForm::OnEnChangeObbScaleX)
	ON_EN_CHANGE(IDC_OBB_SCALE_Y, &XColliderControlForm::OnEnChangeObbScaleY)
	ON_EN_CHANGE(IDC_OBB_SCALE_Z, &XColliderControlForm::OnEnChangeObbScaleZ)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_OBB_LOCATION_X2, &XColliderControlForm::OnNMCustomdrawObbLocationX2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_OBB_LOCATION_Y2, &XColliderControlForm::OnNMCustomdrawObbLocationY2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_OBB_LOCATION_Z2, &XColliderControlForm::OnNMCustomdrawObbLocationZ2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_OBB_ROTATION_X2, &XColliderControlForm::OnNMCustomdrawObbRotationX2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_OBB_ROTATION_Y2, &XColliderControlForm::OnNMCustomdrawObbRotationY2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_OBB_ROTATION_Z2, &XColliderControlForm::OnNMCustomdrawObbRotationZ2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_OBB_SCALE_X2, &XColliderControlForm::OnNMCustomdrawObbScaleX2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_OBB_SCALE_Y2, &XColliderControlForm::OnNMCustomdrawObbScaleY2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_OBB_SCALE_Z2, &XColliderControlForm::OnNMCustomdrawObbScaleZ2)
	ON_BN_CLICKED(IDC_COLLIDER_COMPLETE, &XColliderControlForm::OnBnClickedColliderComplete)
END_MESSAGE_MAP()


// XColliderControlForm 메시지 처리기


void XColliderControlForm::OnBnClickedColliderCreate()
{
	UpdateData(TRUE);
	if (!I_ObjectCtrl.m_bSelect)	return;
	if (!m_bComplete)	return;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch (m_bColliderType)
	{
		case OBB:
		{
			m_bComplete = false;
			X_Box AABB;
			AABB.vCenter = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
			AABB.vAxis[0] = D3DXVECTOR3{ 1.0f,0.0f,0.0f };
			AABB.vAxis[1] = D3DXVECTOR3{ 0.0f,1.0f,0.0f };
			AABB.vAxis[2] = D3DXVECTOR3{ 0.0f,0.0f,1.0f };
			AABB.fExtent[0] = 1.0f;	// x축 반지름
			AABB.fExtent[1] = 1.0f;	// y축 반지름
			AABB.fExtent[2] = 1.0f;	// z축 반지름
			AABB.vMin.x = AABB.vCenter.x - AABB.fExtent[0];
			AABB.vMin.y = AABB.vCenter.y - AABB.fExtent[1];
			AABB.vMin.z = AABB.vCenter.z - AABB.fExtent[2];
			AABB.vMax.x = AABB.vCenter.x + AABB.fExtent[0];
			AABB.vMax.y = AABB.vCenter.y + AABB.fExtent[1];
			AABB.vMax.z = AABB.vCenter.z + AABB.fExtent[2];
			I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.push_back(AABB);
			I_Object.AddCollider(I_ObjectCtrl.m_szSelectObject, AABB);
			D3DXQUATERNION qRotation = { 0.0f, 0.0f, 0.0f, 0.0f };
			I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation.push_back(qRotation);
			I_Object.ViewColliderSwitch();
			
			break;
		}
		case Sphere:
		{
			m_bComplete = false;
			X_Sphere sphere;
			D3DXMATRIX matWolrd = I_ObjectCtrl.GetObjectWorldMat(I_ObjectCtrl.m_szSelectObject, I_ObjectCtrl.m_iSelectMatNum);
			sphere.fRadius = 1;
			sphere.vCenter = D3DXVECTOR3{ matWolrd._41,matWolrd._42,matWolrd._43 };
			I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderSphere.push_back(sphere);
			I_Object.ViewColliderSwitch();
			break;
		}
		default:
			break;
	}

}


void XColliderControlForm::OnEnChangeObbLocationX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	if (m_bColliderType != OBB) return;
	m_OBBSliderLocationX.SetPos(m_fOBBLocationX);
	// 생성시에 만들기 때문에 마지막 index 사용
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB].vCenter.x = m_fOBBLocationX;
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB]);
	UpdateData(FALSE);
}


void XColliderControlForm::OnEnChangeObbLocationY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_OBBSliderLocationY.SetPos(m_fOBBLocationY);
	if (m_bColliderType != OBB) return;
	// 생성시에 만들기 때문에 마지막 index 사용
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB].vCenter.y = m_fOBBLocationY;
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB]);
	UpdateData(FALSE);
}


void XColliderControlForm::OnEnChangeObbLocationZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_OBBSliderLocationZ.SetPos(m_fOBBLocationZ);
	if (m_bColliderType != OBB) return;
	// 생성시에 만들기 때문에 마지막 index 사용
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB].vCenter.z = m_fOBBLocationZ;
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB]);
	UpdateData(FALSE);
}


void XColliderControlForm::OnEnChangeObbRotationX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_OBBRotationX.SetPos(m_fOBBRotationX);
	if (m_bColliderType != OBB) return;
	float fDegreeX = D3DXToRadian(m_fOBBRotationX);
	float fDegreeY = D3DXToRadian(m_fOBBRotationY);
	float fDegreeZ = D3DXToRadian(m_fOBBRotationZ);
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	D3DXQuaternionRotationYawPitchRoll(&I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB], fDegreeY, fDegreeX, fDegreeZ);
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB]);
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetRotateCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB], matRotation);
	UpdateData(FALSE);
}


void XColliderControlForm::OnEnChangeObbRotationY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_OBBRotationY.SetPos(m_fOBBRotationY);
	if (m_bColliderType != OBB) return;
	float fDegreeX = D3DXToRadian(m_fOBBRotationX);
	float fDegreeY = D3DXToRadian(m_fOBBRotationY);
	float fDegreeZ = D3DXToRadian(m_fOBBRotationZ);
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	D3DXQuaternionRotationYawPitchRoll(&I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB], fDegreeY, fDegreeX, fDegreeZ);
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB]);
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetRotateCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB], matRotation);
	UpdateData(FALSE);
}


void XColliderControlForm::OnEnChangeObbRotationZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_OBBRotationZ.SetPos(m_fOBBRotationZ);
	if (m_bColliderType != OBB) return;
	float fDegreeX = D3DXToRadian(m_fOBBRotationX);
	float fDegreeY = D3DXToRadian(m_fOBBRotationY);
	float fDegreeZ = D3DXToRadian(m_fOBBRotationZ);
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	D3DXQuaternionRotationYawPitchRoll(&I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB], fDegreeY, fDegreeX, fDegreeZ);
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB]);
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetRotateCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB], matRotation);
	UpdateData(FALSE);
}


void XColliderControlForm::OnEnChangeObbScaleX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_OBBScaleX.SetPos(m_fOBBScaleX);
	if (m_bColliderType != OBB) return;
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB].fExtent[0] = m_fOBBScaleX;
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB]);
	UpdateData(FALSE);
}


void XColliderControlForm::OnEnChangeObbScaleY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_OBBScaleY.SetPos(m_fOBBScaleY);
	if (m_bColliderType != OBB) return;
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB].fExtent[1] = m_fOBBScaleY;
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB]);
	UpdateData(FALSE);
}


void XColliderControlForm::OnEnChangeObbScaleZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_OBBScaleZ.SetPos(m_fOBBScaleZ);
	if (m_bColliderType != OBB) return;
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB].fExtent[2] = m_fOBBScaleZ;
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB]);
	UpdateData(FALSE);
}


void XColliderControlForm::OnNMCustomdrawObbLocationX2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	if (m_bColliderType != OBB) return;
	m_fOBBLocationX = m_OBBSliderLocationX.GetPos();
	// 생성시에 만들기 때문에 마지막 index 사용
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB].vCenter.x = m_fOBBLocationX;
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB]);
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetRotateCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB], matRotation);
	UpdateData(FALSE);
}


void XColliderControlForm::OnNMCustomdrawObbLocationY2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	if (m_bColliderType != OBB) return;
	m_fOBBLocationY = m_OBBSliderLocationY.GetPos();
	// 생성시에 만들기 때문에 마지막 index 사용
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB].vCenter.y = m_fOBBLocationY;
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB]);
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetRotateCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB], matRotation);
	UpdateData(FALSE);
}


void XColliderControlForm::OnNMCustomdrawObbLocationZ2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	if (m_bColliderType != OBB) return;
	m_fOBBLocationZ = m_OBBSliderLocationZ.GetPos();
	// 생성시에 만들기 때문에 마지막 index 사용
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB].vCenter.z = m_fOBBLocationZ;
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB]);
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetRotateCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB], matRotation);
	UpdateData(FALSE);
}


void XColliderControlForm::OnNMCustomdrawObbRotationX2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	if (m_bColliderType != OBB) return;
	m_fOBBRotationX = m_OBBRotationX.GetPos();
	float fDegreeX = D3DXToRadian(m_fOBBRotationX);
	float fDegreeY = D3DXToRadian(m_fOBBRotationY);
	float fDegreeZ = D3DXToRadian(m_fOBBRotationZ);
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	D3DXQuaternionRotationYawPitchRoll(&I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB], fDegreeY, fDegreeX, fDegreeZ);
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB]);
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetRotateCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB], matRotation);

	UpdateData(FALSE);
}


void XColliderControlForm::OnNMCustomdrawObbRotationY2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	if (m_bColliderType != OBB) return;
	m_fOBBRotationY = m_OBBRotationY.GetPos();
	float fDegreeX = D3DXToRadian(m_fOBBRotationX);
	float fDegreeY = D3DXToRadian(m_fOBBRotationY);
	float fDegreeZ = D3DXToRadian(m_fOBBRotationZ);
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	D3DXQuaternionRotationYawPitchRoll(&I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB], fDegreeY, fDegreeX, fDegreeZ);
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB]);
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetRotateCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB], matRotation);

	UpdateData(FALSE);
}


void XColliderControlForm::OnNMCustomdrawObbRotationZ2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	if (m_bColliderType != OBB) return;
	m_fOBBRotationZ = m_OBBRotationZ.GetPos();
	float fDegreeX = D3DXToRadian(m_fOBBRotationX);
	float fDegreeY = D3DXToRadian(m_fOBBRotationY);
	float fDegreeZ = D3DXToRadian(m_fOBBRotationZ);
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	D3DXQuaternionRotationYawPitchRoll(&I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB], fDegreeY, fDegreeX, fDegreeZ);
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB]);
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetRotateCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB], matRotation);

	UpdateData(FALSE);
}


void XColliderControlForm::OnNMCustomdrawObbScaleX2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	m_fOBBScaleX = m_OBBScaleX.GetPos();
	if (m_bColliderType != OBB) return;
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB].fExtent[0] = m_fOBBScaleX;
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB]);
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetRotateCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB], matRotation);

	UpdateData(FALSE);
}


void XColliderControlForm::OnNMCustomdrawObbScaleY2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	m_fOBBScaleY = m_OBBScaleY.GetPos();
	if (m_bColliderType != OBB) return;
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB].fExtent[1] = m_fOBBScaleY;
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB]);
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetRotateCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB], matRotation);

	UpdateData(FALSE);
}


void XColliderControlForm::OnNMCustomdrawObbScaleZ2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	m_fOBBScaleZ = m_OBBScaleZ.GetPos();
	if (m_bColliderType != OBB) return;
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	if (iLastOBB == -1)	return;
	I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB].fExtent[2] = m_fOBBScaleZ;
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB]);
	I_Object.m_OBBViewerList[I_ObjectCtrl.m_szSelectObject][iLastOBB].SetRotateCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB], matRotation);

	UpdateData(FALSE);
}


void XColliderControlForm::OnBnClickedColliderComplete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bComplete)	return;

	m_bComplete = true;
	int iLastOBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB.size() - 1;
	switch (m_bColliderType)
	{
		case OBB:
		{
			m_bColliderType = Complete;
			X_Box OBB;
			D3DXMATRIX matRotation;
			D3DXMatrixRotationQuaternion(&matRotation, &I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].qRotation[iLastOBB]);
			OBB = I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderAABB[iLastOBB];
			OBB.Rotation(matRotation);
			I_Object.m_ObjectCollider[I_ObjectCtrl.m_szSelectObject].ColliderOBB.push_back(OBB);

			m_fOBBLocationX = 0.0f;
			m_fOBBLocationY = 0.0f;
			m_fOBBLocationZ = 0.0f;
			m_fOBBRotationX = 0.0f;
			m_fOBBRotationY = 0.0f;
			m_fOBBRotationZ = 0.0f;
			m_fOBBScaleX = 1.0f;
			m_fOBBScaleY = 1.0f;
			m_fOBBScaleZ = 1.0f;

			break;
		}
		case Sphere:
		{
			m_bColliderType = Complete;
			break;
		}
		default:
			break;
	}
	UpdateData(FALSE);
}


void XColliderControlForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_OBBSliderLocationX.SetRange(-1000.0f, 1000.0f);
	m_OBBSliderLocationY.SetRange(-1000.0f, 1000.0f);
	m_OBBSliderLocationZ.SetRange(-1000.0f, 1000.0f);
	m_OBBRotationX.SetRange(-180.0f, 180.0f);
	m_OBBRotationY.SetRange(-180.0f, 180.0f);
	m_OBBRotationZ.SetRange(-180.0f, 180.0f);
	m_OBBScaleX.SetRange(0.0f, 1000.0f);
	m_OBBScaleY.SetRange(0.0f, 1000.0f);
	m_OBBScaleZ.SetRange(0.0f, 1000.0f);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
