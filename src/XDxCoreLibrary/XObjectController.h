#pragma once
#include "XMapController.h"

class XObjectController :public XMapController
{
public:
	virtual void Start() override;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render(ID3D11DeviceContext* pContext) override;
	virtual bool Release() override;
public:
	XObjectController();
	virtual ~XObjectController();
};

