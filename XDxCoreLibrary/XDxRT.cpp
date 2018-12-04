#include "XDxRT.h"

HRESULT XDxRT::SetViewPort(float fWidth, float fHeight)
{
	m_vp.TopLeftX = 0.0f;
	m_vp.TopLeftY = 0.0f;
	m_vp.Width = fWidth;
	m_vp.Height = fHeight;
	m_vp.MinDepth = 0.0f;
	m_vp.MaxDepth = 1.0f;

	return hr;
}

HRESULT XDxRT::Create(ID3D11Device * pd3dDevice, ID3D11DeviceContext* pContext, float fWidth, float fHeight)
{
	m_pd3dDevice = pd3dDevice;
	m_pContext = pContext;
	m_pPlaneShape = make_shared<XPlaneShape>();
	m_pPlaneShape->Create(NULL, _T("../Data/Shader/PlaneShader.hlsl"), _T("../Data/Shader/PlaneShader.hlsl"), "VS", "PS");
	SetViewPort(fWidth, fHeight);
	//텍스쳐2d->렌더타겟뷰->셰이더리소스뷰->텍스쳐2d->뎁스스텐실뷰

	//백버퍼용 텍스처 생성
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = (UINT)fWidth;
	td.Height = (UINT)fHeight;
	td.MipLevels = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	td.ArraySize = 1;
	V_RETURN(m_pd3dDevice->CreateTexture2D(&td, NULL, m_pTexture.GetAddressOf()));

	//랜더 타겟 뷰 생성
	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	ZeroMemory(&rtvd, sizeof(rtvd));
	rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	V_RETURN(m_pd3dDevice->CreateRenderTargetView(m_pTexture.Get(), &rtvd, m_pRTV.GetAddressOf()));

	//쉐이더 리소스 뷰 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = td.Format;
	srvd.Texture2D.MipLevels = 1;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	V_RETURN(m_pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), &srvd, m_pSRV.GetAddressOf()));

	//뎁스 스텐실용 텍스처 생성
	ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC td1;
	ZeroMemory(&td, sizeof(td));
	td.Width = fWidth;
	td.Height = fHeight;
	td.MipLevels = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;// | D3D11_BIND_SHADER_RESOURCE; 샘플에는 들어가 있는데 클리어시 문제가 생김.
	td.Usage = D3D11_USAGE_DEFAULT;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	td.ArraySize = 1;
	V_RETURN(m_pd3dDevice->CreateTexture2D(&td, NULL, pDSTexture.GetAddressOf()));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	V_RETURN(m_pd3dDevice->CreateDepthStencilView(pDSTexture.Get(), &dsvd, m_pDSV.GetAddressOf()));

	return hr;
}

bool XDxRT::begin()
{
	m_iViewPorts = 1;
	//전의 랜더타겟 저장 해놓음
	m_pContext->RSGetViewports(&m_iViewPorts, &m_pOldVp);
	m_pContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pOldDSV);
	//새로운 랜더타겟 셋팅
	ID3D11RenderTargetView* pNull = NULL;
	m_pContext->OMSetRenderTargets(1, &pNull, NULL);
	m_pContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), m_pDSV.Get());

	FLOAT Color[4] = { 0.5f,0.5f,0.5f,1.0f };
	m_pContext->ClearRenderTargetView(m_pRTV.Get(), Color);
	m_pContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_pContext->RSSetViewports(1, &m_vp);

	return true;
}

bool XDxRT::end()
{
	m_pContext->OMSetRenderTargets(1, &m_pOldRTV, m_pOldDSV);
	SAFE_RELEASE(m_pOldRTV);
	SAFE_RELEASE(m_pOldDSV);
	return true;
}

bool XDxRT::Render(ID3D11DeviceContext* pContext)
{
	D3D11_VIEWPORT  vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = 200;
	vp.Height = 200;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_pPlaneShape->m_pTextureSRV = m_pSRV;
	m_pContext->RSSetViewports(1, &vp);
	m_pPlaneShape->SetMatrix(NULL, NULL, NULL);
	m_pContext->UpdateSubresource(m_pPlaneShape->m_pConstantBuffer.Get(), 0, NULL, &m_pPlaneShape->m_ConstantDataMatrix, 0, 0);
	m_pPlaneShape->Render(pContext);

	m_pContext->RSSetViewports(m_iViewPorts, &m_pOldVp);
	return true;
}

XDxRT::XDxRT()
{
}


XDxRT::~XDxRT()
{
}
