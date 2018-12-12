#pragma once
#include "XMapController.h"

enum AlphaColor { Spreat_Red = 0, Spreat_Green, Spreat_Blue, Spreat_Alpha, Spreat_None };

class XSpreatController: public XMapController
{
private:
	D3D11_TEXTURE2D_DESC			m_TextureDesc;
	ID3D11Texture2D*				m_AlphaTexture;
	ID3D11Texture2D*				m_StagingTexture;
	AlphaColor						m_SpreatColor;
	ComPtr<ID3D11ShaderResourceView> m_SpreatingTextureSRV;
	vector<ComPtr<ID3D11ShaderResourceView>> m_RGBA_TextureSRV; // 랜더에서 필요시 XTileRender class로 이전
public:
	HRESULT CreateSpreatTexture();
	// 생성한 Texture에 직접 접근해서 컬러값을 바꿈.
	void	Spreating(ID3D11DeviceContext* pContext, X_Box CollisionBox, D3DXVECTOR3 vCrash, float fRadius, int iColor);
	// RGBA마다 컬러값에 Texture를 부여, 랜더에서 필요시 XTileRender class로 이전
	HRESULT RGBA_TextureLoad(ID3D11Device* pDevice, const TCHAR* szFile, AlphaColor Color);
public:
	virtual bool Init() { return true; }
	virtual bool Frame()	override;
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool Release()	override;
public:
	XSpreatController();
	virtual ~XSpreatController();
};

