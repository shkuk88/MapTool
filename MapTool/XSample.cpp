#include "stdafx.h"
#include "XSample.h"

bool XSample::CreateMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TCHAR* szTexture, TCHAR* szHeightTexture, TCHAR* szMapShader, TCHAR* szOnlyColorShader, char* szVSFunctionName, char* szPSFunctionName)
{
	if (!m_pMap)
		m_pMap = new XMap;
	m_pMap->Create(pDevice, pContext, szTexture, szHeightTexture, szMapShader, szOnlyColorShader, szVSFunctionName, szPSFunctionName);
	if (!m_pMapTree)
		m_pMapTree = new XQuadTreeIndex;
	m_pMapTree->Build(m_pMap);
	m_pMapTree->SetCamera(&m_Camera);
	
	return true;
}

bool XSample::Init()
{
	m_Camera.Init();
	m_Sky.LoadTextures(I_Device.m_pD3dDevice.Get());
	m_Sky.Create(_T("../Data/Shader/SkyShader_Sample_0.hlsl"), _T("../Data/Shader/SkyShader_Sample_0.hlsl"), "VS", "PS");
	return true;
}

bool XSample::Frame()
{
	m_Camera.Frame();
	if (m_pMap)
	{
		m_Select.SetMatrix(&m_pMap->GetWorldMatrix(), &m_Camera.GetViewMatrix(), &m_Camera.GetProjMatrix());
		m_pMap->Frame();
	}
	if (m_pMapTree)	m_pMapTree->Frame();

	return true;
}

bool XSample::Render()
{
#pragma region Sky
	if (!m_bWireMode)
	{
		I_Device.m_pD3dContext->RSSetState(XDxState::g_pRSFSolidNone.Get());
		I_Device.m_pD3dContext->PSSetSamplers(0, 1, XDxState::g_SSPoint.GetAddressOf());
		I_Device.m_pD3dContext->OMSetDepthStencilState(XDxState::g_DSSDisable.Get(), 0x00);
		m_Sky.SetMatrix(NULL, &m_Camera.m_matView, &m_Camera.m_matProj);
		m_Sky.Render(I_Device.m_pD3dContext.Get());
		I_Device.m_pD3dContext->RSSetState(XDxState::g_pRSFSolid.Get());
		I_Device.m_pD3dContext->PSSetSamplers(0, 1, XDxState::g_SSLinear.GetAddressOf());
		I_Device.m_pD3dContext->OMSetDepthStencilState(XDxState::g_DSSEnable.Get(), 0x00);
	}
#pragma endregion

	if (m_pMap)		m_pMap->SetMatrix(NULL, &m_Camera.GetViewMatrix(), &m_Camera.GetProjMatrix());
	if (m_pMapTree)	m_pMapTree->Render(I_Device.m_pD3dContext.Get());
	return true;
}

bool XSample::Release()
{
	if (m_pMap)		m_pMap->Release();
	if (m_pMapTree) m_pMapTree->Release();
	return true;
}

XSample::XSample()
{
}


XSample::~XSample()
{
}
