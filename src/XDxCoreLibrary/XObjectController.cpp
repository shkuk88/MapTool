#include "XObjectController.h"



void XObjectController::Start()
{
	XMapController::Start();
	Init();
}

bool XObjectController::Init()
{
	return true;
}

bool XObjectController::Frame()
{
	return true;
}

bool XObjectController::Render(ID3D11DeviceContext * pContext)
{
	return true;
}

bool XObjectController::Release()
{
	return true;
}


XObjectController::XObjectController()
{
}


XObjectController::~XObjectController()
{
}
