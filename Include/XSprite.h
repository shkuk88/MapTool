#pragma once
#include "XStd.h"

class XSprite :public XBase
{
public:
	float m_fStartTime;
	float m_fSpriteSpeed;
	int m_iCurrentSprite;
	int m_iStep;
	map<int, map<int, XRectangle>> m_Movement;
public:
	bool LoadSprite(TString szFileName);
	void SetSprite(int iCurrent, float fSpriteSpeed = 0.0f);
	XRectangle SpriteRun();
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	XSprite();
	virtual ~XSprite();
};

