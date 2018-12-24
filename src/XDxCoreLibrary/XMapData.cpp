#include "XMapData.h"

bool XMapData::FileOpen(TCHAR* filename, TCHAR* mode)
{
	_tfopen_s(&m_fp, filename, mode);
	if (m_fp == NULL)
	{
		MessageBox(0, L"File Open FAIL", 0, 0);
		return false;
	}
	return true;
}

bool XMapData::FileClose()
{
	if (m_fp != NULL)
	{
		fclose(m_fp);
		return true;
	}
	return false;
}

XMapData::XMapData()
{
	for (int iLoop = 0; iLoop < 4; iLoop++)
	{
		m_szAlphaTexture[iLoop] = _T("none");
	}
}

XMapData::~XMapData()
{

}