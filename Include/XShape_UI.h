#pragma once
#include "XShape.h"

class XShape_UI:public XShape
{
public:
	bool  m_bSelectedState;
	POINT m_startPos;
	POINT m_endPos;
	float m_fMoveInSpace;
public:
	void CoordinateSystemTransformation();
	XRectangle ChangeToRectOfVertex(const XRectangle& rt);
	void SetPosition(const POINT& InPos, POINT& outPos);
	POINT GetDistance(POINT startPos, POINT endPos);
	//void RectangleToMove();
	//void ChangeRectangleSize();
public:
	virtual bool Init(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, TCHAR* szTexture, TCHAR* szVSfile, TCHAR* szPSfile, const XRectangle& rtValue,TCHAR* szSaveFile = NULL);
	virtual void Update();
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
public:
	XShape_UI();
	virtual ~XShape_UI();
};

