#pragma once
#include "XCore.h"

class XSample :public XCore
{
public:
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
public:
	XSample();
	virtual ~XSample();
};

