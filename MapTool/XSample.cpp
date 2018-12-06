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
