#include "XMapImporter.h"
#include "XObjectManager.h"


bool XMapImporter::Import()
{
	if (!FileOpen(_T("../../Data/Map/Save/Map.txt"), _T("rt")))	return false;
	
	// standard
	_ftscanf(m_fp, _T("%d"), &m_iCellConut);
	_ftscanf(m_fp, _T("%f"), &m_fDistance);

	ImportTexturePath();
	ImportVertexData();
	ImportIndexData();
	ImportObject();
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

bool XMapImporter::ImportObject()
{	
	// 오브젝트의 종류의 수
	int iObjTypeCnt = 0;
	_ftscanf(m_fp, _T("%d"), &iObjTypeCnt);
	if (!iObjTypeCnt)	return true;
	TCHAR temp[256];

	// 오브젝트 종류별 순회
	for (int iObjType = 0; iObjType < iObjTypeCnt; iObjType++)
	{
		// 오브젝트의 이름(파일명으로 사용)
		ZeroMemory(temp, sizeof(temp));
		_ftscanf(m_fp, _T("%s"), temp);
		TString szObjName = temp;
		I_Object.AddObject(I_Device.m_pD3dDevice.Get(), I_Device.m_pD3dContext.Get(), szObjName.c_str());

		// 오브젝트 타입당 충돌체(OBB) 갯수, 현재는 Sphere타입은 사용하지 않음
		int iObjOBBCnt = 0;
		_ftscanf(m_fp, _T("%d"), &iObjOBBCnt);
		for (int iLoop = 0; iLoop < iObjOBBCnt; iLoop++)
		{
			// AABB, Quaternion, OBB 순
			X_Box AABB;
			_ftscanf(m_fp, _T("%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f"), &AABB.vCenter.x, &AABB.vCenter.y, &AABB.vCenter.z,
				&AABB.fExtent[0], &AABB.fExtent[1], &AABB.fExtent[2],
				&AABB.vAxis[0].x, &AABB.vAxis[0].y, &AABB.vAxis[0].z,
				&AABB.vAxis[1].x, &AABB.vAxis[1].y, &AABB.vAxis[1].z,
				&AABB.vAxis[2].x, &AABB.vAxis[2].y, &AABB.vAxis[2].z,
				&AABB.vMin.x, &AABB.vMin.y, &AABB.vMin.z,
				&AABB.vMax.x, &AABB.vMax.y, &AABB.vMax.z);
			I_Object.m_ObjectCollider[temp].ColliderAABB.push_back(AABB);

			D3DXQUATERNION Quaternion;
			_ftscanf(m_fp, _T("%f%f%f%f"), &Quaternion.x, &Quaternion.y, &Quaternion.z, &Quaternion.w);
			I_Object.m_ObjectCollider[temp].qRotation.push_back(Quaternion);

			X_Box OBB;
			_ftscanf(m_fp, _T("%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f"), &OBB.vCenter.x, &OBB.vCenter.y, &OBB.vCenter.z,
				&OBB.fExtent[0], &OBB.fExtent[1], &OBB.fExtent[2],
				&OBB.vAxis[0].x, &OBB.vAxis[0].y, &OBB.vAxis[0].z,
				&OBB.vAxis[1].x, &OBB.vAxis[1].y, &OBB.vAxis[1].z,
				&OBB.vAxis[2].x, &OBB.vAxis[2].y, &OBB.vAxis[2].z,
				&OBB.vMin.x, &OBB.vMin.y, &OBB.vMin.z,
				&OBB.vMax.x, &OBB.vMax.y, &OBB.vMax.z);
			I_Object.m_ObjectCollider[temp].ColliderOBB.push_back(OBB);

			D3DXMATRIX matRotation;
			D3DXMatrixRotationQuaternion(&matRotation, &Quaternion);
			I_Object.AddCollider(temp, AABB);
			I_Object.m_OBBViewerList[temp][iLoop].SetRotateCollider(I_Device.m_pD3dContext.Get(), I_Object.m_ObjectCollider[temp].ColliderAABB[iLoop], matRotation);
		}

		// 오브젝트의 타입당 갯수
		int iObjCnt = 0;
		_ftscanf(m_fp, _T("%d"), &iObjCnt);
		for (int iObj = 0; iObj < iObjCnt; iObj++)
		{
			// 오브젝트의 타입당 갯수만큼의 월드행렬
			D3DXMATRIX matWorld;
			_ftscanf(m_fp, _T("%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f"),
				&matWorld._11, &matWorld._12, &matWorld._13, &matWorld._14,
				&matWorld._21, &matWorld._22, &matWorld._23, &matWorld._24,
				&matWorld._31, &matWorld._32, &matWorld._33, &matWorld._34,
				&matWorld._41, &matWorld._42, &matWorld._43, &matWorld._44);
			I_Object.AddObjectWorldMat(szObjName.c_str(), matWorld);

			D3DXVECTOR3 vLocation, vScale;
			D3DXQUATERNION qRotation;
			_ftscanf(m_fp, _T("%f%f%f"), &vLocation.x, &vLocation.y, &vLocation.z);
			_ftscanf(m_fp, _T("%f%f%f%f"), &qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w);
			_ftscanf(m_fp, _T("%f%f%f"), &vScale.x, &vScale.y, &vScale.z);
			I_Object.m_ObjectMatrix[szObjName.c_str()].vLocation.push_back(vLocation);
			I_Object.m_ObjectMatrix[szObjName.c_str()].qRotation.push_back(qRotation);
			I_Object.m_ObjectMatrix[szObjName.c_str()].vScale.push_back(vScale);
		}
	}
	return false;
}



XMapImporter::XMapImporter()
{
}


XMapImporter::~XMapImporter()
{
}
