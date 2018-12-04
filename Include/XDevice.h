#pragma once
#include "XStd.h"

class XDevice :public XSingleton<XDevice>
{
public:
	friend class XSingleton<XDevice>;
public:
	// d3d11.h
	ComPtr<ID3D11Device>			m_pd3dDevice;
	ComPtr<ID3D11DeviceContext>		m_pImmediateContext;
	ComPtr<ID3D11RenderTargetView>	m_pRTV;
	//dxgi.h
	ComPtr<IDXGIFactory>			m_pGIFactory;		//그래픽스 인프라
	ComPtr<IDXGISwapChain>			m_pSwapChain;
	DXGI_SWAP_CHAIN_DESC			m_SwapChainDesc;	//백버퍼구조
public:
	HRESULT CreateDevice();
	HRESULT CreateGIFactory();
	HRESULT CreateSwapChain();
	HRESULT SetRenderTargetView();
	void	SetViewPort();
	HRESULT	ResizeDevice(UINT width, UINT height);
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
