#pragma once
#include "XObjectManager.h"

class XColliderViewer
{
public:
	
public:
	bool Init() { return true; }
	bool Frame() { return true; }
	bool Render(ID3D11DeviceContext* pContext) { return true; }
	bool Release() { return true; }
public:
	XColliderViewer();
	virtual ~XColliderViewer();
};

