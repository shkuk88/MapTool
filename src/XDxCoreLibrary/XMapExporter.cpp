#include "XMapExporter.h"



void XMapExporter::ExportStandard(int iCellCnt, float fDistance)
{
	m_iCellConut = iCellCnt;
	m_fDistance = fDistance;
}

bool XMapExporter::ExportVertexData(vector<PNCT_Vertex>* pVertexList, vector<DWORD>* pIndexList)
{
	m_VertexList.resize(pVertexList->size());
	for (int iLoop = 0; iLoop < pVertexList->size(); iLoop++)
	{
		m_VertexList[iLoop].p = (*pVertexList)[iLoop].p;
		m_VertexList[iLoop].n = (*pVertexList)[iLoop].n;
		m_VertexList[iLoop].c = (*pVertexList)[iLoop].c;
		m_VertexList[iLoop].t = (*pVertexList)[iLoop].t;
	}

	m_IndexList.resize(pIndexList->size());
	for (int iLoop = 0; iLoop < pIndexList->size(); iLoop++)
	{
		m_IndexList[iLoop] = (*pIndexList)[iLoop];
	}
	return true;
}

bool XMapExporter::ExportSpreatTexture(ID3D11Texture2D * pSpreatTexture)
{
	if (!pSpreatTexture)	return false;
	m_szSpreatTexture = { _T("../../Data/Map/Texture/SpreatTexture.png") };
	D3DX11SaveTextureToFile(I_Device.m_pD3dContext.Get(), pSpreatTexture, D3DX11_IFF_PNG, m_szSpreatTexture.c_str());
	return true;
}

bool XMapExporter::ExportAlphaTexture(TCHAR * szAlphaTexture, int iColor)
{
	m_szAlphaTexture[iColor] = szAlphaTexture;
	return true;
}

bool XMapExporter::ExportObject()
{
	// 오브젝트의 종류의 수
	int iObjTypeCnt = I_Object.m_ObjectList.size();
	if (!iObjTypeCnt)	return true;
	_ftprintf(m_fp, _T("%d\n"), iObjTypeCnt);

	// 오브젝트 종류별 순회
	for (auto pObject : I_Object.m_ObjectList)
	{
		// 오브젝트의 이름(파일명으로 사용)
		_ftprintf(m_fp, _T("%s "), pObject.first.c_str());

		// 오브젝트 타입당 충돌체(OBB) 갯수, 현재는 Sphere타입은 사용하지 않음
		int iObjOBBCnt = I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB.size();
		_ftprintf(m_fp, _T("%d\n"), iObjOBBCnt);
		for (int iLoop = 0; iLoop < iObjOBBCnt; iLoop++)
		{
			// AABB, Quaternion, OBB 순으로 저장,
			_ftprintf(m_fp, _T("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n"), I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vCenter.x, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vCenter.y, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vCenter.z,
				I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].fExtent[0], I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].fExtent[1], I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].fExtent[2],
				I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vAxis[0].x, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vAxis[0].y, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vAxis[0].z,
				I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vAxis[1].x, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vAxis[1].y, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vAxis[1].z,
				I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vAxis[2].x, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vAxis[2].y, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vAxis[2].z,
				I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vMin.x, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vMin.y, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vMin.z,
				I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vMax.x, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vMax.y, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vMax.z);
			_ftprintf(m_fp, _T("%f %f %f %f\n"), I_Object.m_ObjectCollider[pObject.first.c_str()].qRotation[iLoop].x, I_Object.m_ObjectCollider[pObject.first.c_str()].qRotation[iLoop].y, I_Object.m_ObjectCollider[pObject.first.c_str()].qRotation[iLoop].z, I_Object.m_ObjectCollider[pObject.first.c_str()].qRotation[iLoop].w);
			_ftprintf(m_fp, _T("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n"), I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vCenter.x, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderAABB[iLoop].vCenter.y, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vCenter.z,
				I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].fExtent[0], I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].fExtent[1], I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].fExtent[2],
				I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vAxis[0].x, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vAxis[0].y, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vAxis[0].z,
				I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vAxis[1].x, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vAxis[1].y, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vAxis[1].z,
				I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vAxis[2].x, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vAxis[2].y, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vAxis[2].z,
				I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vMin.x, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vMin.y, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vMin.z,
				I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vMax.x, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vMax.y, I_Object.m_ObjectCollider[pObject.first.c_str()].ColliderOBB[iLoop].vMax.z);
		}
		
		// 오브젝트의 타입당 갯수
		int iObjCnt = I_Object.m_ObjectMatrix[pObject.first.c_str()].Matrix.size();
		_ftprintf(m_fp, _T("%d\n"), iObjCnt);
		for (int iLoop = 0; iLoop < iObjCnt; iLoop++)
		{
			// 오브젝트의 타입당 갯수만큼의 월드행렬
			D3DXMATRIX matWorld = I_Object.m_ObjectMatrix[pObject.first.c_str()].Matrix[iLoop];
			_ftprintf(m_fp, _T("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n"),
				matWorld._11, matWorld._12, matWorld._13, matWorld._14,
				matWorld._21, matWorld._22, matWorld._23, matWorld._24,
				matWorld._31, matWorld._32, matWorld._33, matWorld._34,
				matWorld._41, matWorld._42, matWorld._43, matWorld._44 );

			D3DXVECTOR3 vLocation, vScale;
			D3DXQUATERNION qRotation;
			D3DXMatrixDecompose(&vScale, &qRotation, &vLocation, &matWorld);
			_ftprintf(m_fp, _T("%f %f %f\n"), vLocation.x, vLocation.y, vLocation.z);
			_ftprintf(m_fp, _T("%f %f %f %f\n"), qRotation.x, qRotation.y, qRotation.z, qRotation.w);
			_ftprintf(m_fp, _T("%f %f %f\n"), vScale.x, vScale.y, vScale.z);
		}
	}

	return true;
}

bool XMapExporter::Export()
{
	if (!FileOpen(_T("../../Data/Map/Save/Map.txt"), _T("wt")))	return false;
	// standard
	_ftprintf(m_fp, _T("%d\n"), m_iCellConut);
	_ftprintf(m_fp, _T("%f\n"), m_fDistance);
	// spreat texture
	_ftprintf(m_fp, _T("%s\n"), m_szSpreatTexture.c_str());
	// alpha texture
	for (int iLoop = 0; iLoop < m_szAlphaTexture.size(); iLoop++)
	{
		_ftprintf(m_fp, _T("%s\n"), m_szAlphaTexture[iLoop].c_str());
	}

	// vector list
	_ftprintf(m_fp, _T("%d\n"), m_VertexList.size());
	for (int iLoop = 0; iLoop < m_VertexList.size(); iLoop++)
	{
		_ftprintf(m_fp, _T("%f %f %f %f %f %f %f %f %f %f %f %f\n"), m_VertexList[iLoop].p.x, m_VertexList[iLoop].p.y, m_VertexList[iLoop].p.z,
			m_VertexList[iLoop].n.x, m_VertexList[iLoop].n.y, m_VertexList[iLoop].n.z,
			m_VertexList[iLoop].c.x, m_VertexList[iLoop].c.y, m_VertexList[iLoop].c.z, m_VertexList[iLoop].c.w,
			m_VertexList[iLoop].t.x, m_VertexList[iLoop].t.y);
	}
	// index list
	_ftprintf(m_fp, _T("%d\n"), m_IndexList.size());
	for (int iLoop = 0; iLoop < m_IndexList.size(); iLoop++)
	{
		_ftprintf(m_fp, _T("%d\n"), m_IndexList[iLoop]);
	}
	// Object Export
	ExportObject();

	if (!FileClose())	return false;
	return true;
}

XMapExporter::XMapExporter()
{

}


XMapExporter::~XMapExporter()
{
}
