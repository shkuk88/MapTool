#include "XMapExporter.h"



bool XMapExporter::SaveVertexData(vector<PNCT_Vertex>* pVertexList, vector<DWORD>* pIndexList)
{
	m_VertexList.resize(pVertexList->size());
	for (int iLoop = 0; iLoop < pVertexList->size(); iLoop++)
	{
		m_VertexList[iLoop].p = (*pVertexList)[iLoop].p;
		m_VertexList[iLoop].n = (*pVertexList)[iLoop].n;
		m_VertexList[iLoop].c = (*pVertexList)[iLoop].c;
		m_VertexList[iLoop].t = (*pVertexList)[iLoop].t;
	}

	m_IndexList.resize(pVertexList->size());
	for (int iLoop = 0; iLoop < pIndexList->size(); iLoop++)
	{
		m_IndexList[iLoop] = (*pIndexList)[iLoop];
	}
	return false;
}

bool XMapExporter::ExportSpreatTexture(ID3D11Texture2D * pSpreatTexture)
{
	if (!pSpreatTexture)	return false;
	m_szSpreatTexture = { _T("../Data/Texture/SpreatTexture.bmp") };
	D3DX11SaveTextureToFile(I_Device.m_pD3dContext.Get(), pSpreatTexture, D3DX11_IFF_BMP, m_szSpreatTexture.c_str());
	return true;
}

bool XMapExporter::SaveAlphaTexture(TCHAR * szAlphaTexture, int iColor)
{
	m_szAlphaTexture[iColor] = szAlphaTexture;
	return true;
}

void XMapExporter::Export()
{
}

XMapExporter::XMapExporter()
{
}


XMapExporter::~XMapExporter()
{
}
