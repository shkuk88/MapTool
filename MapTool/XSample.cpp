#include "stdafx.h"
#include "XSample.h"

bool XSample::Init()
{
	m_Camera.Init();
	m_Map.Create(I_Device.m_pD3dDevice.Get(), I_Device.m_pD3dContext.Get(), _T("../Data/Texture/castle.jpg"), _T("../Data/Shader/MapShader_Specular.hlsl"), _T("../Data/Shader/MapShader_Color_Specular.hlsl"), "VS", "PS", _T("../Data/Texture/HEIGHT_CASTLE.bmp"));
	m_MapTree.Build(&m_Map);
	m_MapTree.SetCamera(&m_Camera);
	return true;
}

bool XSample::Frame()
{
	m_Camera.Frame();
	m_Select.SetMatrix(&m_Map.GetWorldMatrix(), &m_Camera.GetViewMatrix(), &m_Camera.GetProjMatrix());
	m_Map.Frame();
	m_MapTree.Frame();
	return true;
}

bool XSample::Render()
{
	m_Map.SetMatrix(NULL, &m_Camera.GetViewMatrix(), &m_Camera.GetProjMatrix());
	m_MapTree.Render(I_Device.m_pD3dContext.Get());
	return true;
}

bool XSample::Release()
{
	m_Map.Release();
	m_MapTree.Release();
	return true;
}

XSample::XSample()
{
}


XSample::~XSample()
{
}
