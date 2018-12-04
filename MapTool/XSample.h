#pragma once
#include "XCore.h"
#include "XDevice.h"
#include "XCamera.h"
#include "XMap.h"

class XSample :public XCore
{
private:
	XCamera	m_Camera;
	XMap	m_Map;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	XSample();
	virtual ~XSample();
};

