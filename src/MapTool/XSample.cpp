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
	I_HeightCtrl.SetLeafNode(m_pMapTree->GetRootNode());
	I_HeightCtrl.SetMap(m_pMap);
	I_HeightCtrl.Start();
	// 맵 생성시 스플리팅 컨트롤러 시동
	I_SpreatCtrl.SetLeafNode(m_pMapTree->GetRootNode());
	I_SpreatCtrl.SetMap(m_pMap);
	I_SpreatCtrl.Start();
	I_SpreatCtrl.SetMapTexture();
	// 맵 생성시 오브젝트 컨트롤러 시동
	I_ObjectCtrl.SetLeafNode(m_pMapTree->GetRootNode());
	I_ObjectCtrl.SetMap(m_pMap);
	I_ObjectCtrl.Start();
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
	I_HeightCtrl.SetLeafNode(m_pMapTree->GetRootNode());
	I_HeightCtrl.SetMap(m_pMap);
	I_HeightCtrl.Start();
	// 맵 생성시 스플리팅 컨트롤러 시동
	I_SpreatCtrl.SetLeafNode(m_pMapTree->GetRootNode());
	I_SpreatCtrl.SetMap(m_pMap);
	I_SpreatCtrl.Start();
	// 맵 생성시 오브젝트 컨트롤러 시동
	I_ObjectCtrl.SetLeafNode(m_pMapTree->GetRootNode());
	I_ObjectCtrl.SetMap(m_pMap);
	I_ObjectCtrl.Start();

	return false;
}

D3DXMATRIX XSample::GetSelectObjMat()
{
	return I_Object.m_ObjectMatrix[I_ObjectCtrl.m_szSelectObject][I_ObjectCtrl.m_iSelectMatNum];
}

bool XSample::Init()
{
	m_Camera.Init();
	m_Sky.LoadTextures(I_Device.m_pD3dDevice.Get());
	m_Sky.Create(_T("../../Data/Map/Shader/SkyShader_Sample_0.hlsl"), _T("../../Data/Map/Shader/SkyShader_Sample_0.hlsl"), "VS", "PS");
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
	I_HeightCtrl.Frame();
	I_SpreatCtrl.Frame();
	I_ObjectCtrl.Frame(I_Device.m_pD3dContext.Get());
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
	I_SpreatCtrl.Render(I_Device.m_pD3dContext.Get());
	I_ObjectCtrl.SetMatrix(&m_Camera.GetViewMatrix(), &m_Camera.GetProjMatrix());
	I_ObjectCtrl.Render(I_Device.m_pD3dContext.Get());
	return true;
}

bool XSample::Release()
{
	if (m_pMap)		m_pMap->Release();
	if (m_pMapTree) m_pMapTree->Release();
	I_ObjectCtrl.Release();
	return true;
}

XSample::XSample()
{
	
}


XSample::~XSample()
{
}
