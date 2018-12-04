#pragma once
#include "XStd.h"


class XCollision :public XSingleton<XCollision>, public XBase
{
public:
	friend class XSingleton<XCollision>;
public:
	vector<XRectangle>* m_Collider;
	vector<XRectangle>* m_Cliff;
	typedef vector<XRectangle>::iterator m_iter;
public:
	//------- 충돌이면 true 반환 -------//
	bool RectInRect(XRectangle rtDesk,XRectangle rtSrc);
	XRectangle GetIntersectRectangle(XRectangle rtDesk, XRectangle rtSrc);
	bool SetMapInfo(vector<XRectangle>* collider, vector<XRectangle>* cliff);
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
private:
	XCollision();
public:
	virtual ~XCollision();
};

#define I_Collision XCollision::Get()
