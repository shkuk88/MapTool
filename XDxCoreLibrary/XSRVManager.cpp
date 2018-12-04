#include "XSRVManager.h"

TString XSRVManager::Add(ID3D11Device* pd3dDevice, TString szTexture)
{
	TString sz = szTexture;
	for (auto iter : m_SRVList)
	{
		if (iter.first == szTexture)
			return iter.first;
	}

	ComPtr<ID3D11ShaderResourceView> pSRV = NULL;

	hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, szTexture.c_str(), NULL, NULL, pSRV.GetAddressOf(), NULL);
	if (FAILED(hr))
	{
		return NULL;
	}
	m_SRVList.insert(make_pair(szTexture, pSRV));

	return szTexture;
}

ComPtr<ID3D11ShaderResourceView> XSRVManager::Find(TString szTexture)
{
	auto iter = m_SRVList.find(szTexture);
	return iter->second;
}

void XSRVManager::Clear()
{
	m_SRVList.clear();
}

XSRVManager::XSRVManager()
{
}

XSRVManager::~XSRVManager()
{
}
