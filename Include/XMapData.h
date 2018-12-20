#pragma once
#include "XShape.h"

class XMapData
{
protected:
	FILE*				m_fp;
	vector<PNCT_Vertex> m_VertexList;
	vector<DWORD>		m_IndexList;
	TString				m_szSpreatTexture = {_T("none"),};
	map<int, TString>	m_szAlphaTexture;
public:
	bool		FileOpen(TCHAR* filename, TCHAR* mode);
	bool		FileClose();
public:
	XMapData();
	virtual ~XMapData();
};

