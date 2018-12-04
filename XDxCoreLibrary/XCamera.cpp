#include "XCamera.h"

bool XCamera::SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget, D3DXVECTOR3 vUp)
{
	m_vCameraPos = vPos;
	m_vTargetPos = vTarget;
	m_vUp = vUp;

	D3DXMatrixLookAtLH(&m_matView, &vPos, &vTarget, &vUp);//  카메라위치,카메라타겟,카메라머리
	UpdateVector();

	return true;
}

bool XCamera::SetProjMatrix(float fFOV, float fAspect, float fNearPlane, float fFarPlane)
{
	m_fFOV = fFOV;
	m_fAspect = fAspect;
	m_fNearPlane = fNearPlane;
	m_fFarPlane = fFarPlane;
	D3DXMatrixPerspectiveFovLH(&m_matProj, fFOV, fAspect, fNearPlane, fFarPlane);//  카메라데이터,카메라 시야각, 카메라 비율,최소 ,최대
	return true;
}

bool XCamera::UpdateVector()
{
	m_vSide.x = m_matView._11;
	m_vSide.y = m_matView._21;
	m_vSide.z = m_matView._31;

	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;

	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;

	D3DXVec3Normalize(&m_vSide, &m_vSide);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	return true;
}

bool XCamera::Update()
{
	return true;
}

bool XCamera::UpdateQuaternion()
{
	D3DXQUATERNION qRotation;
	// x축행렬과 y축행렬이 결합되어서 새로운 행렬이 만들어진다.
	D3DXQuaternionRotationYawPitchRoll(&qRotation, m_fYaw, m_fPitch, m_fRoll); // 단발성이 아니라면 회전이 들어간다 그러면 이 함수를 사용해야한다. 이유는 나중에
	D3DXMATRIX matRotation;
	ZeroMemory(&matRotation, sizeof(matRotation));

	D3DXMatrixRotationQuaternion(&matRotation, &qRotation); // 사원수(회전 행렬에만 적용됨)를 행렬로 변환해주는 함수, 서로 상호변환 가능

	// 여기까지 만들어진 것은 오브젝트의 행렬이므로 카메라 행렬로 변환(역행렬)해주어야 한다.
	D3DXMatrixInverse(&m_matView, NULL, &matRotation);
	// 이걸 뷰행렬로 사용하겠다!
	// 그러므로 SetViewMatrix에 LookAtLH 함수가 필요가 없어진다.

	// 회전행렬을 만들었으므로 이동행렬을 만들면
	UpdateVector();

	m_matView._41 = -D3DXVec3Dot(&m_vCameraPos, &m_vSide);
	m_matView._42 = -D3DXVec3Dot(&m_vCameraPos, &m_vUp);
	m_matView._43 = -D3DXVec3Dot(&m_vCameraPos, &m_vLook);

	return true;
}

bool XCamera::SetYawPitchRoll()
{
	D3DXMATRIX matInvView;
	D3DXMatrixInverse(&matInvView, NULL, &m_matView);
	D3DXVECTOR3* pZ = (D3DXVECTOR3*)&matInvView._31;

	m_fYaw = atan2f(pZ->x, pZ->z);
	float fLen = sqrtf(pZ->x*pZ->x + pZ->z*pZ->z);
	m_fPitch = -atan2f(pZ->y, fLen);

	return true;
}

bool XCamera::Init(D3DXVECTOR3 vCameraPos, D3DXVECTOR3 vTargetPos, D3DXVECTOR3 vUp, float fFOV, float fAspect, float fNearPlane, float fFarPlane)
{
	SetViewMatrix(vCameraPos, vTargetPos, vUp);
	SetProjMatrix(fFOV, fAspect, fNearPlane, fFarPlane);
	SetYawPitchRoll();
	CreateFrustum();
	return true;
}

bool XCamera::Init()
{
	m_fFOV = D3DX_PI * 0.25f;
	m_fAspect = 800.0f / 600.0f;
	m_fNearPlane = 0.1f;
	m_fFarPlane = 1000.0f;

	SetViewMatrix(m_vCameraPos, m_vTargetPos, m_vUp);
	SetProjMatrix(m_fFOV, m_fAspect, m_fNearPlane, m_fFarPlane);
	SetYawPitchRoll();
	CreateFrustum();
	return true;
}

bool XCamera::Frame()
{
	// 가속도
	if (I_Input.KeyCheck(DIK_SPACE) == KEY_HOLD)
	{
		m_fSpeed += 10.0f * g_fSecPerFrame;
	}
	else
	{
		m_fSpeed -= 10.0f * g_fSecPerFrame;
		if (m_fSpeed < 1.0f)
			m_fSpeed = 1.0f;
	}

	// 위
	if (I_Input.KeyCheck(DIK_W) == KEY_HOLD)
	{
		m_vCameraPos += m_vLook * m_fSpeed * 10.0f * g_fSecPerFrame;
	}

	// 아래
	if (I_Input.KeyCheck(DIK_S) == KEY_HOLD)
	{
		m_vCameraPos -= m_vLook * m_fSpeed * 10.0f * g_fSecPerFrame;
	}

	// 왼쪽
	if (I_Input.KeyCheck(DIK_A) == KEY_HOLD)
	{
		m_vCameraPos -= m_vSide * m_fSpeed * 10.0f * g_fSecPerFrame;
	}

	// 오른쪽
	if (I_Input.KeyCheck(DIK_D) == KEY_HOLD)
	{
		m_vCameraPos += m_vSide * m_fSpeed * 10.0f * g_fSecPerFrame;
	}

	if (I_Input.m_MouseState[0])
	{
		m_fPitch += D3DXToRadian(I_Input.m_DIMouseState.lY) * 0.5;
		m_fYaw += D3DXToRadian(I_Input.m_DIMouseState.lX) * 0.5;
	}

	CreateFrustum();
	UpdateQuaternion();
	return true;
}

XCamera::XCamera()
{
	m_vCameraPos = D3DXVECTOR3(0.0f, 50.0f, -50.0f);
	m_vTargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.01f);

	m_fYaw = 0.0f;
	m_fPitch = 0.0f;
	m_fRoll = 0.0f;

	m_fSpeed = 1.0f;
}


XCamera::~XCamera()
{
}
