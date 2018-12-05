#pragma once
#include "XStd.h"

class XDevice :public XSingleton<XDevice>
{
public:
	friend class XSingleton<XDevice>;
public:
	// d3d11.h
	ComPtr<ID3D11Device>			m_pD3dDevice;
	ComPtr<ID3D11DeviceContext>		m_pD3dContext;
	ComPtr<ID3D11RenderTargetView>	m_pRenderTV;
	D3D11_VIEWPORT					m_ViewPort;
	ComPtr<ID3D11DepthStencilView>	m_pDepthSV;
	D3D11_DEPTH_STENCIL_VIEW_DESC	m_DepthStencilDesc;
	//dxgi.h
	ComPtr<IDXGIFactory>			m_pGIFactory;		//그래픽스 인프라
	ComPtr<IDXGISwapChain>			m_pSwapChain;
	DXGI_SWAP_CHAIN_DESC			m_SwapChainDesc;	//백버퍼구조

	BOOL							m_IsFullScreenMode;
public:
	HRESULT CreateDevice();
	HRESULT CreateGIFactory();
	HRESULT CreateSwapChain(BOOL IsFullScreen = FALSE);
	HRESULT SetRenderTargetView();
	HRESULT SetDepthStencilView();
	void	SetViewPort();
	HRESULT	ResizeDevice(UINT iWidth, UINT iHeight);
	BOOL	GetFullScreenFlag();
	void	SetFullScreenFlag(BOOL bFlag);
	HRESULT CreateDxResource() { return S_OK; }
	HRESULT DeleteDxResource() { return S_OK; }
public:
	bool Init();
	void PreRender();
	void PostRender();
	void Release();
private:
	XDevice();
public:
	virtual ~XDevice();
};

#define I_Device XSingleton<XDevice>::Get()
