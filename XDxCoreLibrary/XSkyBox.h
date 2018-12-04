#pragma once
#include "XShape.h"

class XSkyBox :public XBoxShape
{
public:
	D3DXVECTOR3						 m_vCameraPos;
	D3DXVECTOR3						 m_vTargetPos;
	D3DXVECTOR3						 m_vUp;
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV[6];
public:
	HRESULT	LoadTextures(ID3D11Device* pDevice);
	bool	Render(ID3D11DeviceContext* pContext);
	void	SetSkyMatrix();
public:
	XSkyBox();
	virtual ~XSkyBox();
};

