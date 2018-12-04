#include "stdafx.h"
#include "XSample.h"

bool XSample::Init()
{
	m_Camera.Init();
	m_Map.Create(I_Device.m_pD3dDevice.Get(), I_Device.m_pD3dContext.Get(), _T("../Data/Texture/castle.jpg"), _T("../Data/Shader/MapShader_Sample_Specular.hlsl"), "VS", _T("../Data/Shader/MapShader_Sample_Specular.hlsl"), "PS", _T("../Data/Texture/HEIGHT_CASTLE.bmp"));

	return true;
}

bool XSample::Frame()
{
	m_Camera.Frame();
	m_Map.Frame();
	return true;
}

bool XSample::Render()
{
	m_Map.SetMatrix(NULL, &m_Camera.GetViewMatrix(), &m_Camera.GetProjMatrix());
	m_Map.Render(I_Device.m_pD3dContext.Get());
	return true;
}

bool XSample::Release()
{
	m_Map.Release();
	return true;
}

XSample::XSample()
{
}


XSample::~XSample()
{
}
