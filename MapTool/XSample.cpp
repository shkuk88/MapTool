#include "stdafx.h"
#include "XSample.h"

bool XSample::Init()
{
	m_Camera.Init();
	m_Map.Create(I_Device.m_pD3dDevice.Get(), I_Device.m_pD3dContext.Get(), _T("../Data/Texture/castle.jpg"), _T("../Data/Shader/MapShader_Sample_Specular.hlsl"), "VS", _T("../Data/Shader/MapShader_Sample_Specular.hlsl"), "PS", _T("../Data/Texture/HEIGHT_CASTLE.bmp"));
	m_Box.Create(_T("../Data/Shader/BoxShader.hlsl"), _T("../Data/Shader/BoxShader.hlsl"), "VS", "PS");
	m_Box.SetSelect(&m_Select);
	
	return true;
}

bool XSample::Frame()
{
	m_Camera.Frame();
	m_Select.SetMatrix(&m_Map.GetWorldMatrix(), &m_Camera.GetViewMatrix(), &m_Camera.GetProjMatrix());
	m_Map.Frame();
	m_Box.MoveObject();
	m_Box.m_matWorld._42 = m_Map.GetHeight(m_Box.m_matWorld._41, m_Box.m_matWorld._43)+1.0f;
	return true;
}

bool XSample::Render()
{
	m_Map.SetMatrix(NULL, &m_Camera.GetViewMatrix(), &m_Camera.GetProjMatrix());
	m_Map.Render(I_Device.m_pD3dContext.Get());
	m_Box.SetMatrix(NULL, &m_Camera.GetViewMatrix(), &m_Camera.GetProjMatrix());
	m_Box.Render(I_Device.m_pD3dContext.Get());
	return true;
}

bool XSample::Release()
{
	m_Map.Release();
	m_Box.Release();
	return true;
}

XSample::XSample()
{
}


XSample::~XSample()
{
}
