#pragma once
#include "XInput.h"
#include "XFrustum.h"

class XCamera :public XFrustum
{
public:
	//D3DXMATRIX			m_matWorld;
	//D3DXMATRIX			m_matView;
	//D3DXMATRIX			m_matProj;

	D3DXVECTOR3			m_vCameraPos;			// ī�޶� ��ġ
	D3DXVECTOR3			m_vTargetPos;			// ī�޶� Ÿ��		
	D3DXVECTOR3			m_vAt;					// ī�޶� �Ӹ�

	D3DXVECTOR3			m_vLook;				// z�� ����
	D3DXVECTOR3			m_vSide;				// y�� ����
	D3DXVECTOR3			m_vUp;					// x�� ����

	float				m_fFOV;					// ī�޶� �þ߰�
	float				m_fAspect;				// ī�޶� ����
	float				m_fNearPlane;			// ���� �ּ� �Ÿ�
	float				m_fFarPlane;			// ���� �ִ� �Ÿ�

	// ���Ϸ� ��
	float				m_fYaw;
	float				m_fPitch;
	float				m_fRoll;

	float				m_fSpeed;
public:
	D3DXMATRIX			GetWorldMatrix() { return m_matWorld; }
	D3DXMATRIX			GetViewMatrix() { return m_matView; }
	D3DXMATRIX			GetProjMatrix() { return m_matProj; }

	bool				SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget, D3DXVECTOR3 vUp);			//����� ����
	bool				SetProjMatrix(float fFOV, float fAspect, float fNearPlane, float fFarPlane);	//������� ����
	bool				SetYawPitchRoll();	//����ķ� ���� ���Ϸ����� �޾ƿ�
	bool				Update();
	bool				UpdateVector();
	bool				UpdateQuaternion();
	//////////////////////////////////
	bool				 Init(D3DXVECTOR3 vCameraPos, D3DXVECTOR3 vTargetPos, D3DXVECTOR3 vUp, float fFOV, float fAspect, float fNearPlane, float fFarPlane);
	bool				 Init();
	virtual bool		 Frame();
	bool				 Render();
	bool				 Release();
public:
	XCamera();
	virtual ~XCamera();
};

