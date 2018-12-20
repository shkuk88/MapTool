#include "XMapImporter.h"



bool XMapImporter::Import()
{
	if (!FileOpen(_T("../Data/Map/Save/Map.txt"), _T("rt")))	return false;
	
	// standard
	_ftscanf(m_fp, _T("%d"), &m_iCellConut);
	_ftscanf(m_fp, _T("%f"), &m_fDistance);

	ImportTexturePath();
	ImportVertexData();
	ImportIndexData();
	
	if (!FileClose())	return false;
	return true;
}

void XMapImporter::ImportVertexData()
{
	// vector list
	int iSize = 0;
	_ftscanf(m_fp, _T("%d"), &iSize);
	m_VertexList.resize(iSize);
	for (int iLoop = 0; iLoop < iSize; iLoop++)
	{
		_ftscanf(m_fp, _T("%f%f%f%f%f%f%f%f%f%f%f%f"), &m_VertexList[iLoop].p.x, &m_VertexList[iLoop].p.y, &m_VertexList[iLoop].p.z,
			&m_VertexList[iLoop].n.x, &m_VertexList[iLoop].n.y, &m_VertexList[iLoop].n.z,
			&m_VertexList[iLoop].c.x, &m_VertexList[iLoop].c.y, &m_VertexList[iLoop].c.z, &m_VertexList[iLoop].c.w,
			&m_VertexList[iLoop].t.x, &m_VertexList[iLoop].t.y);
	}
}

void XMapImporter::ImportIndexData()
{
	// index list
	int iSize = 0;
	_ftscanf(m_fp, _T("%d"), &iSize);
	m_IndexList.resize(iSize);
	for (int iLoop = 0; iLoop < iSize; iLoop++)
	{
		_ftscanf(m_fp, _T("%d"), &m_IndexList[iLoop]);
	}
}

void XMapImporter::ImportTexturePath()
{
	TCHAR temp[256];
	ZeroMemory(temp, sizeof(temp));

	// spreat texture
	_ftscanf(m_fp, _T("%s"), temp);
	m_szSpreatTexture = temp;

	// alpha texture
	for (int iLoop = 0; iLoop < m_szAlphaTexture.size(); iLoop++)
	{
		ZeroMemory(temp, sizeof(temp));
		_ftscanf(m_fp, _T("%s"), temp);
		m_szAlphaTexture[iLoop] = temp;
	}
}

XMapImporter::XMapImporter()
{
}


XMapImporter::~XMapImporter()
{
}
