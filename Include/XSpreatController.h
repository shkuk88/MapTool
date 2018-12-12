#pragma once
#include "XMapController.h"

class XSpreatController: public XMapController
{
private:
	D3D11_TEXTURE2D_DESC			m_TextureDesc;
	ID3D11Texture2D*				m_AlphaTexture;
	ID3D11Texture2D*				m_StagingTexture;
	ComPtr<ID3D11ShaderResourceView> m_SpreatingTextureSRV;
public:
	HRESULT CreateSpreatTexture();
	void	Spreating(ID3D11DeviceContext* pContext, X_Box CollisionBox, D3DXVECTOR3 vCrash, float fRadius, int iColor);
public:
	XSpreatController();
	virtual ~XSpreatController();
};

