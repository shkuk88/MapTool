#include "XCore.h"


LRESULT XCore::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

bool XCore::Init() { return true; }
bool XCore::Frame() { return true; }
bool XCore::Render() { return true; }
bool XCore::Release() { return true; }

bool XCore::XInit()
{
	if (!I_Device.Init())	return false;
	I_Input.Init();
	I_Timer.Init();

	// DxWrite를 쓰기 위해 백버퍼는 넘겨준다. 사용 후에 바로 릴리즈 해준다.
	IDXGISurface1* pBackBuffer;
	V_RETURN(hr = I_Device.m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (LPVOID*)&pBackBuffer));
	if (!I_Write.Set(pBackBuffer))	return false;
	SAFE_RELEASE(pBackBuffer);

	XDxState::SetState(I_Device.m_pD3dDevice.Get());
	I_Device.m_pD3dContext->OMSetBlendState(XDxState::g_pAlphaBlend.Get(), 0, -1);
	I_Device.m_pD3dContext->RSSetState(XDxState::g_pRSFSolid.Get());
	I_Device.m_pD3dContext->PSSetSamplers(0, 1, XDxState::g_SSLinear.GetAddressOf());
	if (!Init())			return false;
	return true;
}
 
bool XCore::XFrame()
{
	I_Timer.Frame();
	I_Input.Frame();
	if (!Frame())
		return false;
	return true;
}

bool XCore::PreRender()
{
	I_Device.PreRender();
	I_Device.m_pD3dContext->OMSetDepthStencilState(XDxState::g_DSSEnable.Get(), 0x00);
	I_Device.m_pD3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	if (I_Input.KeyCheck(DIK_F1) == KEY_PUSH)
	{
		I_Device.m_pD3dContext->RSSetState(XDxState::g_pRSFWire.Get());
	}
	if (I_Input.KeyCheck(DIK_F2) == KEY_PUSH)
	{
		I_Device.m_pD3dContext->RSSetState(XDxState::g_pRSFSolid.Get());
	}	
	if (I_Input.KeyCheck(DIK_F3) == KEY_PUSH)
	{
		I_Device.m_pD3dContext->PSSetSamplers(0, 1, XDxState::g_SSPoint.GetAddressOf());
	}
	if (I_Input.KeyCheck(DIK_F4) == KEY_PUSH)
	{
		I_Device.m_pD3dContext->PSSetSamplers(0, 1, XDxState::g_SSLinear.GetAddressOf());
	}
	return true;
}

bool XCore::PostRender()
{
	I_Device.PostRender();
	return true;
}

bool XCore::XRender()
{
	
	PreRender();
		I_Timer.Render();
		Render();
		DrawDebug();
	PostRender();
	return true;
}

bool XCore::XRelease()
{
	Release();
	I_Timer.Release();	I_Input.Release();
	I_Write.Release();
	I_Device.Release();

	return true;
}

bool XCore::DrawDebug()
{
	D2D1_RECT_F rt = { 0.0f,0.0f,800.0f,600.0f };
	if (I_Input.KeyCheck(DIK_V) == KEY_HOLD)
	{
		I_Write.m_pRT->BeginDraw();
		I_Write.m_pRT->DrawText(I_Timer.m_szBuffer, wcslen(I_Timer.m_szBuffer), I_Write.m_pTextFormat, rt, I_Write.m_pSolidBrush);
		I_Write.m_pRT->EndDraw();
	}
	return true;
}

bool XCore::Run() 
{
	XInit();
	while (1) 
	{
		if (!MsgRun())
			break;
		if (!XFrame())
			break;
		XRender();
	}
	XRelease();
	return true;
}

XCore::XCore()
{
}


XCore::~XCore()
{
}
