#pragma once
#include "XMapController.h"

enum SpreatView { SpreatView_Alpha = 0, SpreatView_Texture};
enum AlphaColor { Spreat_Red = 0, Spreat_Green, Spreat_Blue, Spreat_Alpha, Spreat_None };

class XSpreatController: public XMapController,public XSingleton<XSpreatController>
{
private:
	friend class XSingleton<XSpreatController>;
	SpreatView						m_SpreatView = SpreatView_Alpha;
	ComPtr<ID3D11PixelShader>		m_AlphaPS;
	D3D11_TEXTURE2D_DESC			m_TextureDesc;
	ID3D11Texture2D*				m_SpreatTexture;
	ID3D11Texture2D*				m_StagingTexture;
	ID3D11Texture2D*				m_AlphaTexture[4];
	AlphaColor						m_SpreatColor = Spreat_Red;
	ComPtr<ID3D11ShaderResourceView> m_SpreatingTextureSRV;
	map<int, ComPtr<ID3D11ShaderResourceView>> m_RGBA_TextureSRV; // �������� �ʿ�� XTileRender class�� ����
public:
	void MakeSRV(ID3D11Texture2D* pTexture, ComPtr<ID3D11ShaderResourceView>* pSRV);
	// ���ĸʸ� ���� ��Ƽ�ؽ�ó�� ���� ���°� ����
	void SetSpreatViewState(SpreatView SpreatViewState);
	void SetSpreatColor(AlphaColor color) { m_SpreatColor = color; }
	// ���ĸ� �ؽ�ó�� ����.
	HRESULT CreateSpreatTexture();
	// ������ Texture�� ���� �����ؼ� �÷����� �ٲ�.
	void	Spreating(ID3D11DeviceContext* pContext, X_Box CollisionBox, D3DXVECTOR3 vCrash, float fRadius, int iColor);
	// RGBA���� �÷����� Texture�� �ο�, �������� �ʿ�� XTileRender class�� ����
	HRESULT RGBA_TextureLoad(ID3D11Device* pDevice, const TCHAR* szFile, AlphaColor Color);
	
	void SetMapTexture();
	ID3D11Texture2D* GetSpreatTex() { return m_SpreatTexture; }
public:
	virtual void Start()	override;
public:
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render(ID3D11DeviceContext* pContext) override;
	virtual bool Release()	override;
public:
	XSpreatController();
	virtual ~XSpreatController();
};

#define I_SpreatCtrl XSingleton<XSpreatController>::Get()