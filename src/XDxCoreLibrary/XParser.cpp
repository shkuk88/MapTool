#include "XParser.h"

bool	XParser::FileOpen(TCHAR* filename, TCHAR* mode)
{
	_tfopen_s(&m_fp, filename, mode);
	if (m_fp == NULL)
	{
		MessageBox(0, L"File Open FAIL", 0, 0);
		return false;
	}
	return true;
}

bool	XParser::FileClose()
{
	if (m_fp != NULL)
	{
		fclose(m_fp);
		return true;
	}
	return false;
}

TCHAR*	XParser::GetNextLine()
{
	_fgetts(m_strBuffer, sizeof(TCHAR) * 256, m_fp);
	_stscanf_s(m_strBuffer, _T("%s"), m_strName, _countof(m_strName));
	return m_strBuffer;
}

bool	XParser::GetData(TCHAR* string, void* pData, int DataType)
{
	fgetpos(m_fp, &m_FilePos);

	while (!feof(m_fp))
	{
		GetNextLine();

		if (!_tcscmp(string, m_strName))
		{
			if (pData == NULL)		return true;
			switch (DataType)
			{
			case INT_TYPE:
				_stscanf_s(m_strBuffer, _T("%s%d"), m_strName, _countof(m_strName), pData);
				return true;
			case FLOAT_TYPE:
				_stscanf_s(m_strBuffer, _T("%s%f"), m_strName, _countof(m_strName), pData);
				return true;
			case STR_TYPE:
				TCHAR strTemp[256];
				_stscanf_s(m_strBuffer, _T("%s%s"), m_strName, _countof(m_strName), strTemp, _countof(strTemp));
				if (strTemp[0] == '"')
				{
					// 문자열의 시작과 끝에 있는 (")문자를 삭제한다.
					size_t filelen = _tcslen(strTemp);
					strTemp[filelen - 1] = 0;
					_tcscpy((TCHAR*)pData, &strTemp[1]);
				}
				else
				{
					_tcscpy((TCHAR*)pData, &strTemp[0]);
				}
				return true;
			case VECTOR_TYPE:
				_stscanf_s(m_strBuffer, _T("%s%f%f%f"), m_strName, _countof(m_strName), &((D3DXVECTOR3*)pData)->x, &((D3DXVECTOR3*)pData)->z, &((D3DXVECTOR3*)pData)->y);
				return true;
			case QUATERNION_TYPE:
				_stscanf_s(m_strBuffer, _T("%s%d%f%f%f%f"), m_strName, _countof(m_strName), &m_iTemp, &((D3DXQUATERNION*)pData)->x, &((D3DXQUATERNION*)pData)->z, &((D3DXQUATERNION*)pData)->y, &((D3DXQUATERNION*)pData)->w );
				return true;
			case VERTEX_TYPE:
				_stscanf_s(m_strBuffer, _T("%s%d%f%f%f"), m_strName, _countof(m_strName), &m_iTemp, &((D3DXVECTOR3*)pData)->x, &((D3DXVECTOR3*)pData)->z, &((D3DXVECTOR3*)pData)->y);
				return true;
			case CVERTEX_TYPE:
				_stscanf_s(m_strBuffer, _T("%s%d%f%f%f"), m_strName, _countof(m_strName), &m_iTemp, &((D3DXVECTOR3*)pData)->x, &((D3DXVECTOR3*)pData)->y, &((D3DXVECTOR3*)pData)->z);
				return true;
			case UVW_TYPE:
				_stscanf_s(m_strBuffer, _T("%s%d%f%f%f"), m_strName, _countof(m_strName), &m_iTemp, &((D3DXVECTOR3*)pData)->x, &((D3DXVECTOR3*)pData)->y, &((D3DXVECTOR3*)pData)->z);
				return true;
			case TFACE_LIST_TYPE:
				_stscanf_s(m_strBuffer, _T("%s%d%d%d%d"), m_strName, _countof(m_strName), &m_iTemp, &((XFaceList*)pData)->_0, &((XFaceList*)pData)->_2, &((XFaceList*)pData)->_1);
				return true;
			case FACE_LIST_TYPE:
				_stscanf_s(m_strBuffer, _T("%s%s %s%d %s%d %s%d %s%d%s%d%s%d %s%d %s%d"), m_strName, _countof(m_strName), m_strName, _countof(m_strName), m_strName, _countof(m_strName),
				&((XFaceList*)pData)->_0, m_strName, _countof(m_strName), &((XFaceList*)pData)->_2, m_strName, _countof(m_strName), &((XFaceList*)pData)->_1, m_strName, _countof(m_strName), //A,C,B
				&m_iTemp, m_strName, _countof(m_strName), &m_iTemp, m_strName, _countof(m_strName), &m_iTemp, //AB,BC,CA
				m_strName, _countof(m_strName), &m_iTemp,
				m_strName, _countof(m_strName), &((XFaceList*)pData)->MtrlId);
				return true;
			}
		}
	}
	fsetpos(m_fp, &m_FilePos);
	return false;
}

bool	XParser::FindLine(TCHAR* token)
{
	if (token == NULL)
	{
		return false;
	}
	fgetpos(m_fp, &m_FilePos);
	while (!feof(m_fp))
	{
		GetNextLine();
		if (!_tcscmp(m_strName, token))
		{
			fsetpos(m_fp, &m_FilePos);
			return true;
		}
		if (!_tcscmp(m_strName, L"}"))
		{
			fsetpos(m_fp, &m_FilePos);
			return false;
		}
	}
}

BOOL	XParser::FindToken(TCHAR** token,int TokenSize)
{
	if (token == NULL)
	{
		return false;
	}
	while (!feof(m_fp))
	{
		GetNextLine();
		for (int iCnt = 0; iCnt < TokenSize; iCnt++)
		{
			if (!_tcscmp(m_strName, token[iCnt]))
			{
				return iCnt;
			}
			else if (!_tcscmp(m_strName, L"}"))
			{
				return -1;
			}
		}
	}
}

TCHAR*	XParser::DeleteDir(TCHAR* path)
{
	TCHAR path_buffer[256];			// 전체 경로
	TCHAR drive[256];				// 드라이브 명
	TCHAR dir[256];					// 디렉토리 경로
	TCHAR name[256];				// 파일명
	TCHAR ext[256];                 // 확장자 명

	_tcscpy_s(path_buffer, path);
	_tsplitpath(path_buffer, drive, dir, name, ext);
	ZeroMemory(path, sizeof(path));
	_tcscat(path, L"../../Data/Map/Texture/");
	_tcscat(path, name);
	_tcscat(path, ext);
	return path;
}

TCHAR*	XParser::SetDir(TCHAR* filename)
{
	TCHAR path[256];
	_tcscpy_s(path, L"../../Data/Map/Texture/");
	_tcscat(path, filename);
	ZeroMemory(filename, sizeof(filename));
	_tcscat(filename, path);
	return path;
}

TCHAR*	XParser::ExtType(TCHAR* path)
{
	TCHAR path_buffer[256];			// 전체 경로
	TCHAR drive[256];				// 드라이브 명
	TCHAR dir[256];					// 디렉토리 경로
	TCHAR name[256];				// 파일명
	TCHAR ext[256];                 // 확장자 명

	_tcscpy_s(path_buffer, path);
	_tsplitpath(path_buffer, drive, dir, name, ext);
	return ext;
}

XParser::XParser()
{
	m_fp		= NULL;
	m_FilePos	= NULL;

	ZeroMemory(m_strBuffer, sizeof(TCHAR) * 256);
	ZeroMemory(m_strName, sizeof(TCHAR) * 256);
}


XParser::~XParser()
{
}
