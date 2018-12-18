#pragma once
#include "XShape.h"

class XMapData
{
protected:
	vector<PNCT_Vertex> m_VertexList;
	vector<DWORD>		m_IndexList;
	TString				m_szSpreatTexture;
	map<int, TString>	m_szAlphaTexture;
public:
	XMapData() {}
	virtual ~XMapData() {}
};

