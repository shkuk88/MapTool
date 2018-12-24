#include "XDxState.h"

ComPtr<ID3D11RasterizerState>    XDxState::g_pRSFSolid = 0;
ComPtr<ID3D11RasterizerState>    XDxState::g_pRSFSolidNone = 0;
ComPtr<ID3D11RasterizerState>    XDxState::g_pRSFWire = 0;

ComPtr<ID3D11DepthStencilState>	 XDxState::g_DSSEnable = 0;
ComPtr<ID3D11DepthStencilState>	 XDxState::g_DSSDisable = 0;
ComPtr<ID3D11DepthStencilState>	 XDxState::g_DSSEnableNoWrite = 0;

ComPtr<ID3D11SamplerState>		 XDxState::g_SSLinear = 0;
ComPtr<ID3D11SamplerState>		 XDxState::g_SSPoint = 0;

ComPtr<ID3D11BlendState>		 XDxState::g_pAlphaBlend = 0;
ComPtr<ID3D11BlendState>		 XDxState::g_pAlphaBlendOneZero = 0;
ComPtr<ID3D11BlendState>		 XDxState::g_pAlphaBlendDisable = 0;


bool	XDxState::SetState(ID3D11Device* d3dDevice)
{
	D3D11_RASTERIZER_DESC		rsd;
	ZeroMemory(&rsd, sizeof(D3D11_RASTERIZER_DESC));
	rsd.FillMode = D3D11_FILL_SOLID;
	rsd.CullMode = D3D11_CULL_BACK;
	V_RETURN(d3dDevice->CreateRasterizerState(&rsd, &g_pRSFSolid));

	rsd.CullMode = D3D11_CULL_NONE;
	V_RETURN(d3dDevice->CreateRasterizerState(&rsd, &g_pRSFSolidNone));

	rsd.FillMode = D3D11_FILL_WIREFRAME;
	rsd.CullMode = D3D11_CULL_BACK;
	V_RETURN(d3dDevice->CreateRasterizerState(&rsd, g_pRSFWire.GetAddressOf()));

	D3D11_DEPTH_STENCIL_DESC	dsd;
	ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	V_RETURN(d3dDevice->CreateDepthStencilState(&dsd, g_DSSEnable.GetAddressOf()));

	ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsd.DepthEnable = FALSE;
	V_RETURN(d3dDevice->CreateDepthStencilState(&dsd, g_DSSDisable.GetAddressOf()));

	ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsd.DepthFunc = D3D11_COMPARISON_ALWAYS;
	V_RETURN(d3dDevice->CreateDepthStencilState(&dsd, g_DSSEnableNoWrite.GetAddressOf()));

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dDevice->CreateSamplerState(&sd, &g_SSLinear);

	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	d3dDevice->CreateSamplerState(&sd, &g_SSPoint);

	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.AlphaToCoverageEnable = TRUE;
	//bd.IndependentBlendEnable = TRUE;

	bd.RenderTarget[0].BlendEnable = TRUE;

	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	V_RETURN(d3dDevice->CreateBlendState(&bd, g_pAlphaBlend.GetAddressOf()));

	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	V_RETURN(d3dDevice->CreateBlendState(&bd, g_pAlphaBlendOneZero.GetAddressOf()));

	bd.RenderTarget[0].BlendEnable = FALSE;
	V_RETURN(d3dDevice->CreateBlendState(&bd, g_pAlphaBlendDisable.GetAddressOf()));
	return true;
}