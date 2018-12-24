#include "stdafx.h"
#include "XSample.h"

bool XSample::CreateMap(TCHAR* szTexture, TCHAR* szHeightTexture, float fCellCount, float fDistance)
{
	if (!m_pMap)
		m_pMap = new XMap;
	if(!fCellCount)
		m_pMap->Create(I_Device.m_pD3dDevice.Get(), I_Device.m_pD3dContext.Get(), fDistance, szTexture, szHeightTexture, _T("../../Data/Map/Shader/MapShader_Specular.hlsl"), _T("../../Data/Map/Shader/MapShader_Color_Specular.hlsl"), "VS", "PS");
	else
		m_pMap->Create(I_Device.m_pD3dDevice.Get(), I_Device.m_pD3dContext.Get(), fCellCount, fDistance, szTexture, _T("../../Data/Map/Shader/MapShader_Specular.hlsl"), _T("../../Data/Map/Shader/MapShader_Color_Specular.hlsl"), "VS", "PS");
	if (!m_pMapTree)
		m_pMapTree = new XQuadTreeIndex;
	m_pMapTree->Build(m_pMap);
	m_pMapTree->SetCamera(&m_Camera);
	// 맵 생성시 높이맵 컨트롤러 시동
	m_HeightMapCtrl.SetLeafNode(m_pMapTree->GetRootNode());
	m_HeightMapCtrl.SetMap(m_pMap);
	m_HeightMapCtrl.Start();
	// 맵 생성시 스플리팅 컨트롤러 시동
	m_SpreatCtrl.SetLeafNode(m_pMapTree->GetRootNode());
	m_SpreatCtrl.SetMap(m_pMap);
	m_SpreatCtrl.Start();
	m_SpreatCtrl.SetMapTexture();
	return true;
}

bool XSample::CreateMap()
{
	if (!m_pMap)
		m_pMap = new XMap;
	m_pMap->Create(I_Device.m_pD3dDevice.Get(), I_Device.m_pD3dContext.Get(), &m_Importer, _T("../../Data/Map/Shader/MapShader_Specular.hlsl"), _T("../../Data/Map/Shader/MapShader_Color_Specular.hlsl"), "VS", "PS");
	if (!m_pMapTree)
		m_pMapTree = new XQuadTreeIndex;
	m_pMapTree->Build(m_pMap);
	m_pMapTree->SetCamera(&m_Camera);
	// 맵 생성시 높이맵 컨트롤러 시동
	m_HeightMapCtrl.SetLeafNode(m_pMapTree->GetRootNode());
	m_HeightMapCtrl.SetMap(m_pMap);
	m_HeightMapCtrl.Start();
	// 맵 생성시 스플리팅 컨트롤러 시동
	m_SpreatCtrl.SetLeafNode(m_pMapTree->GetRootNode());
	m_SpreatCtrl.SetMap(m_pMap);
	m_SpreatCtrl.Start();
	
	return false;
}

bool XSample::Init()
{
	m_Camera.Init();
	m_Sky.LoadTextures(I_Device.m_pD3dDevice.Get());
	m_Sky.Create(_T("../../Data/Map/Shader/SkyShader_Sample_0.hlsl"), _T("../../Data/Map/Shader/SkyShader_Sample_0.hlsl"), "VS", "PS");
	//Object test
	m_Object.Init(_T("../../Data/Map/Save/dwarf_sword.Shk"), I_Device.m_pD3dDevice.Get(), I_Device.m_pD3dContext.Get(), _T("../../DATA/Map/Shader/Ase.hlsl"), _T("../../DATA/Map/Shader/Ase.hlsl"));
	return true;
}

bool XSample::Frame()
{
	m_Camera.Frame();
	if (m_pMap)
	{
		I_Select.SetMatrix(NULL, &m_Camera.GetViewMatrix(), &m_Camera.GetProjMatrix());
		m_pMap->Frame();
	}
	if (m_pMapTree)	m_pMapTree->Frame();
	m_HeightMapCtrl.Frame();
	m_SpreatCtrl.Frame();
	m_Object.Frame();
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
	if (m_pMap)		m_pMap->Render(I_Device.m_pD3dContext.Get());
	//if (m_pMapTree)	m_pMapTree->Render(I_Device.m_pD3dContext.Get());
	m_SpreatCtrl.Render(I_Device.m_pD3dContext.Get());

	m_Object.SetMatrix(NULL, &m_Camera.GetViewMatrix(), &m_Camera.GetProjMatrix());
	m_Object.Render();
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
