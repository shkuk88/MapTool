#pragma once
#include "XMapData.h"

class XMapExporter :public XMapData
{
public:
	bool SaveVertexData(vector<PNCT_Vertex>* pVertexList, vector<DWORD>* pIndexList);
	bool ExportSpreatTexture(ID3D11Texture2D* pSpreatTexture);
	bool SaveAlphaTexture(TCHAR* szAlphaTexture, int iColor);
	void Export();
public:
	XMapExporter();
	virtual ~XMapExporter();
};

