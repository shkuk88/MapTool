#include "XDevice.h"
HRESULT hr;

bool XDevice::Init()
{
	if (FAILED(CreateDevice()))			return false;
	if (FAILED(CreateGIFactory()))		return false;
	if (FAILED(CreateSwapChain()))		return false;
	if (FAILED(SetRenderTargetView()))	return false;
	SetViewPort();
	if (FAILED(SetDepthStencilView()))	return false;
	m_pD3dContext->OMSetRenderTargets(1, m_pRenderTV.GetAddressOf(), m_pDepthSV.Get());
	return true;
}

HRESULT XDevice::CreateDevice()
{
	//IDXGIAdapter* pAdapter = NULL;								// �׷���ī��
	//D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE;		// �ϵ����� �Ҳ��� ����Ʈ����� �Ҳ���(�ϵ��� �ȵɶ� �ڵ��� �Ϸ��� ����Ʈ����� ����)
	UINT Flags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	//D3D_FEATURE_LEVEL pRetFeatureLevel;
	D3D_FEATURE_LEVEL pInFeatureLevel = D3D_FEATURE_LEVEL_11_0;		// dx����
	//UINT FeatureLevels = 1;										// dx 11_0���� �ϳ� ��� ���̴� 1�Է��ϸ��
	//UINT SDKVersion = D3D11_SDK_VERSION;
	//D3D11CreateDevice(pAdapter, DriverType, NULL, Flags, &pInFeatureLevel, FeatureLevels, SDKVersion,&m_pD3dDevice,&pRetFeatureLevel,&m_pD3dContext);

	V_RETURN(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, Flags, &pInFeatureLevel, 1, D3D11_SDK_VERSION, m_pD3dDevice.GetAddressOf(), NULL, m_pD3dContext.GetAddressOf()));

	return hr;
}

HRESULT XDevice::CreateGIFactory()
{
	V_RETURN(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)m_pGIFactory.GetAddressOf()));

	return hr;
}

HRESULT XDevice::CreateSwapChain()
{
	//typedef struct DXGI_SWAP_CHAIN_DESC
	//{
	//	DXGI_MODE_DESC BufferDesc;		// Ŭ���̾�Ʈ ������
	//	DXGI_SAMPLE_DESC SampleDesc;	// 
	//	DXGI_USAGE BufferUsage;			// ������ �뵵
	//	UINT BufferCount;				// ������� ����
	//	HWND OutputWindow;				// 
	//	BOOL Windowed;					// 
	//	DXGI_SWAP_EFFECT SwapEffect;	// 
	//	UINT Flags;						// 
	//} 	DXGI_SWAP_CHAIN_DESC;
	DXGI_SWAP_CHAIN_DESC pDesc;
	ZeroMemory(&pDesc, sizeof(DXGI_SWAP_CHAIN_DESC));		// ���� �ʴ� �͵� 0���� ä���� �־���ϱ� ������ ZeroMemory�� �Ѵ�.
	pDesc.BufferDesc.Width = g_rtClient.right;
	pDesc.BufferDesc.Height = g_rtClient.bottom;
	pDesc.BufferDesc.RefreshRate.Numerator = 60;			// ����� �ֻ���,������
	pDesc.BufferDesc.RefreshRate.Denominator = 1;			// �ʴ� �踸ŭ �Ѹ���
	pDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// RGBA�� ����ȭ���� �ʰڴ�.
	pDesc.SampleDesc.Count = 1;								// �ּ��� �ѹ��� �Ѹ���.
	pDesc.BufferCount = 1;									// ����� ����(OffScrean)
	pDesc.OutputWindow = g_hWnd;							// ������ �ڵ�, ���� �Ѹ�����
	pDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// ������ �뵵
	pDesc.Windowed = TRUE;
	pDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	V_RETURN(m_pGIFactory->CreateSwapChain(m_pD3dDevice.Get(), &pDesc, m_pSwapChain.GetAddressOf()));

	return hr;
}

HRESULT XDevice::SetRenderTargetView()
{
	ID3D11Texture2D* pResource;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pResource); //__uuidof ������ �ڵ带 ��½����ִ� ��ũ��
	if (FAILED(hr))	return hr;
	hr = m_pD3dDevice->CreateRenderTargetView(pResource, NULL, m_pRenderTV.GetAddressOf());
	if (FAILED(hr))	return hr;
	m_pD3dContext->OMSetRenderTargets(1, m_pRenderTV.GetAddressOf(), NULL);

	return hr;
}

void XDevice::SetViewPort()
{
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = g_rtClient.right;
	m_ViewPort.Height = g_rtClient.bottom;
	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;

	m_pD3dContext->RSSetViewports(1, &m_ViewPort);
}

HRESULT XDevice::SetDepthStencilView()
{
	if (m_pDepthSV != nullptr)
	{
		m_pDepthSV.ReleaseAndGetAddressOf();
	}
	ID3D11Texture2D *pDSTexture = NULL;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = m_ViewPort.Width;
	td.Height = m_ViewPort.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	V_RETURN(m_pD3dDevice->CreateTexture2D(&td, NULL, &pDSTexture));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&dsvd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;					//���е� ���϶� R32_TYPELESS , ���̴����ҽ��� �Ҷ� D32�� �Ѵ�(����������)
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	V_RETURN(m_pD3dDevice->CreateDepthStencilView(pDSTexture, &dsvd, m_pDepthSV.GetAddressOf()));
	
	
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	//ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	//if (dsvd.Format == DXGI_FORMAT_D24_UNORM_S8_UINT)
	//{
	//	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//	srvd.Texture2D.MostDetailedMip = 0;
	//	srvd.Texture2D.MipLevels = 1;
	//	m_pd3dDevice->CreateShaderResourceView(pDSTexture, &srvd, m_pSRV.GetAddressOf());
	//}

	m_pDepthSV->GetDesc(&m_DepthStencilDesc);

	return hr;
}

void XDevice::PreRender()
{
	//float color[4] = { 0.0f,0.0f,0.0f,1.0f };
	float color[4] = { 0.7f,0.7f,0.7f,1.0f };
	m_pD3dContext->ClearRenderTargetView(m_pRenderTV.Get(), color);						// ���� Ŭ����
	m_pD3dContext->ClearDepthStencilView(m_pDepthSV.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);	// ���� Ŭ����
}

void XDevice::PostRender()
{
	m_pSwapChain->Present(0, 0); // ����Ű�� �ִ� ���۸� �ٲ۴�.
}

void XDevice::Release()
{
	m_pRenderTV	  = nullptr;
	m_pSwapChain  = nullptr;
	m_pGIFactory  = nullptr;
	m_pD3dContext = nullptr;
	m_pD3dDevice  = nullptr;
}

XDevice::XDevice()
{
}


XDevice::~XDevice()
{
}
