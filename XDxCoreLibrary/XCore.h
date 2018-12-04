#pragma once
#include "XWindow.h"
#include "XDevice.h"
#include "XInput.h"
#include "XDxWrite.h"
#include "XDxState.h"

class XCore :public XWindow
{
public:
	bool DrawDebug();
public:
	virtual LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
public:
	bool Run();
	bool XInit();
	bool XFrame();
	bool XRender();
	bool XRelease();
public:
	bool PreRender();
	bool PostRender();
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	XCore();
	virtual ~XCore();
};