#pragma once
#include "XShape.h"
#include "XInput.h"
#include "XSpriteManager.h"
enum UI_STATE {UI_PLAY = 0};
enum CurrentSRV { NORMAL, IN_CURSUR, PRESS_CURSUR };

class XUI : public XShape
{
public:
	vector<ID3D11ShaderResourceView*> m_pTexturesSRV;
	int m_iCurrentSRV;
	int m_iUI;
	XRectangle m_rtPos;
	XSprite* m_pSprite;
	int m_iSprite;
	D3D11_TEXTURE2D_DESC	m_objDesc;
	XRectangle m_rtBasicPose;
public:
	ID3D11ShaderResourceView* ReturnShaderResourceView(ID3D11Device* pd3dDevice, TCHAR* szTextureFile);
	bool RectInPoint(POINT* pt);
	bool AddSprite(TCHAR* szFileName);
	void CoordinateSystemTransformation();
public:
	virtual void Update(ID3D11DeviceContext* pd3dContext);
	virtual bool Create(const XRectangle& rt, ID3D11Device* pd3dDevice, TCHAR* szVertexShader, TCHAR* szPixelShader,  TCHAR* szTexture1, TCHAR* szFileName=NULL, TCHAR* szTexture2 = NULL, TCHAR* szTexture3 = NULL);
	virtual bool Frame(ID3D11DeviceContext* pd3dContext);
	virtual bool Render(ID3D11DeviceContext* pd3dContext);
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
public:
	XUI();
	virtual ~XUI();
};

