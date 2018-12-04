#include "XDxWrite.h"


bool XDxWrite::Set(IDXGISurface1* pSurface)
{
	if (FAILED(CreateDeviceIndependentResources()))	return false;
	if (FAILED(CreateDeviceResources(pSurface)))	return false;

	return true;
}

HRESULT XDxWrite::CreateDeviceIndependentResources()
{
	D2D1_FACTORY_TYPE type = D2D1_FACTORY_TYPE_SINGLE_THREADED;
	V_RETURN(D2D1CreateFactory(type, &m_pD2DFactory))

	m_pD2DFactory->GetDesktopDpi(&m_fDpiX, &m_fDpiY);
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pDWriteFactory);
	if (SUCCEEDED(hr))
	{
		V_RETURN(m_pDWriteFactory->CreateTextFormat(L"고딕", NULL, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_ITALIC, DWRITE_FONT_STRETCH_NORMAL, 20, L"en-us", &m_pTextFormat));		//영어면 L"en-us" 한글은 L"ko-kr
	}
	return hr;
}

HRESULT XDxWrite::CreateDeviceResources(IDXGISurface1* pSurface)
{
	D2D1_RENDER_TARGET_PROPERTIES rtp;
	ZeroMemory(&rtp, sizeof(D2D1_RENDER_TARGET_PROPERTIES));
	rtp.type					= D2D1_RENDER_TARGET_TYPE_DEFAULT;
	rtp.pixelFormat.format		= DXGI_FORMAT_UNKNOWN;
	rtp.pixelFormat.alphaMode	= D2D1_ALPHA_MODE_PREMULTIPLIED;
	rtp.dpiX					= m_fDpiX;
	rtp.dpiY					= m_fDpiY;

	V_RETURN(m_pD2DFactory->CreateDxgiSurfaceRenderTarget(pSurface, (D2D1_RENDER_TARGET_PROPERTIES*)&rtp, &m_pRT))

	D2D1_COLOR_F color = { 1.0f, 1.0f, 1.0f, 1.0f };
	V_RETURN(m_pRT->CreateSolidColorBrush(color, &m_pSolidBrush))

	return hr;
}

bool XDxWrite::Release() 
{
	SAFE_RELEASE(m_pSolidBrush);
	SAFE_RELEASE(m_pTextFormat);
	SAFE_RELEASE(m_pDWriteFactory);
	SAFE_RELEASE(m_pD2DFactory);
	SAFE_RELEASE(m_pRT);
	return true;
}

XDxWrite::XDxWrite()
{
}


XDxWrite::~XDxWrite()
{
}
