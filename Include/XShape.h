#pragma once
#include "XDevice.h"

struct PNCTVertex			//position,normal,color,texture
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
};

struct ConstantBuffer
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
};

class XShape:public XBase
{
public:
	ID3D11Device*				m_pd3dDevice;
	ID3D11DeviceContext*		m_pContext;
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_pConstantBuffer;
	ID3D11VertexShader*			m_pVS;
	ID3D11PixelShader*			m_pPS;
	ID3D11InputLayout*			m_pVertexLayout;
	ID3D11ShaderResourceView*   m_pTextureSRV;
	ID3DBlob*					m_pVSBlob;
	ID3DBlob*					m_pPSBlob;
	
	vector<PNCTVertex>			m_VertexList;
	vector<DWORD>				m_IndexList;
	ConstantBuffer				m_ConstantBuffer;
public:
	virtual HRESULT		CreateVertexBuffer();
	virtual HRESULT		CreateIndexBuffer();
	HRESULT				CreateConstantBuffer();
	HRESULT				LoadShader(TCHAR* VSfile, TCHAR* PSfile);
	virtual HRESULT		InputLayout();
	HRESULT				CreateShaderResourceView(TCHAR* TextureFile);
	virtual void		Update();
	virtual bool		SetPosition(D3DXVECTOR3 StartPos, float MinX, float MinY, float MaxX, float MaxY) {return true;}
public:
	virtual bool		Init(ID3D11Device* pd3dDevice,ID3D11DeviceContext* pContext, TCHAR* szTexture, TCHAR* szVSfile, TCHAR* szPSfile);
	virtual bool		PreRender();
	virtual bool		PostRender();
	virtual bool		Init()		override;
	virtual bool		Frame()		override;
	virtual bool		Render()	override;
	virtual bool		Release()	override;
public:
	XShape();
	virtual~XShape();
};

