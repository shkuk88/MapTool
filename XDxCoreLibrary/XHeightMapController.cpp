#include "XHeightMapController.h"



void XHeightMapController::SetHeightCtrlState(HeightControl HeightCtrlState)
{
	Start();
	m_bHeightCtrlState = HeightCtrlState;
}

void XHeightMapController::SetControlValue(float fRadius, float fStrength)
{
	m_fRadius = fRadius;
	m_fStrength = fStrength;
}

void XHeightMapController::HeightUpDown()
{
	float fHeightControl = 0.0f;
	if (m_bHeightCtrlState == Up)
	{
		fHeightControl = g_fSecPerFrame * m_fStrength;
	}
	else
	{
		fHeightControl = -g_fSecPerFrame * m_fStrength;
	}

	for (auto pNode : m_CrashNode)
	{
		for (int iFace = 0; iFace < pNode->m_IndexList.size();)
		{
			for (int iTri = 0; iTri < 3; iTri++, iFace++)
			{
				PNCT_Vertex* pControl = &m_pMap->m_VertexList[pNode->m_IndexList[iFace]];
				D3DXVECTOR3 vControlPos = pControl->p;
				D3DXVECTOR3 vLength = vControlPos - m_vIntersection;
				float fLength = D3DXVec3Length(&vLength);
				fLength = fLength / m_fRadius;
				if (fLength < 1.0f)
				{
					fLength *= 90;
					fLength = D3DX_PI * fLength / 180.0;
					fLength = cosf(fLength);
					fLength *= fHeightControl;
					pControl->p.y += fLength;
				}
			}
		}
		pNode->m_Box.vMin = m_pMap->m_VertexList[pNode->m_CornerIndex[XQuadTreeIndex::BL]].p;
		pNode->m_Box.vMax = m_pMap->m_VertexList[pNode->m_CornerIndex[XQuadTreeIndex::TR]].p;
		pNode->m_Box.vCenter = (pNode->m_Box.vMin + pNode->m_Box.vMax)*0.5f;
		pNode->m_Box.fExtent[0] = pNode->m_Box.vMax.x - pNode->m_Box.vCenter.x;
		pNode->m_Box.fExtent[1] = pNode->m_Box.vMax.y - pNode->m_Box.vCenter.y;
		pNode->m_Box.fExtent[2] = pNode->m_Box.vMax.z - pNode->m_Box.vCenter.z;
	}
}

void XHeightMapController::HeightFlat()
{
	for (auto pNode : m_CrashNode)
	{
		for (int iFace = 0; iFace < pNode->m_IndexList.size();)
		{
			for (int iTri = 0; iTri < 3; iTri++, iFace++)
			{
				PNCT_Vertex* pControl = &m_pMap->m_VertexList[pNode->m_IndexList[iFace]];
				D3DXVECTOR3 vControlPos = pControl->p;
				pControl->p.y = m_vIntersection.y;
			}
		}
		pNode->m_Box.vMin = m_pMap->m_VertexList[pNode->m_CornerIndex[XQuadTreeIndex::BL]].p;
		pNode->m_Box.vMax = m_pMap->m_VertexList[pNode->m_CornerIndex[XQuadTreeIndex::TR]].p;
		pNode->m_Box.vCenter = (pNode->m_Box.vMin + pNode->m_Box.vMax)*0.5f;
		pNode->m_Box.fExtent[0] = pNode->m_Box.vMax.x - pNode->m_Box.vCenter.x;
		pNode->m_Box.fExtent[1] = pNode->m_Box.vMax.y - pNode->m_Box.vCenter.y;
		pNode->m_Box.fExtent[2] = pNode->m_Box.vMax.z - pNode->m_Box.vCenter.z;
	}
}

bool XHeightMapController::Frame()
{
	if (!bStart || m_bHeightCtrlState == None) return false;
	if (I_Input.m_MouseState[0])
	{
		if (!CheakInRange())	return false;
		if (m_bHeightCtrlState == Up || m_bHeightCtrlState == Down)
		{
			HeightUpDown();
		}
		else  // Flat 상황
		{
			HeightFlat();
		}
		m_pMap->CalcPerVertexNormalsFastLookUp();
	}
	//if (I_Input.m_MouseState[0] == KEY_UP)
	//{
	//	if (m_bHeightCtrlState != None) m_pMap->CalcPerVertexNormalsFastLookUp();
	//}
	return true;
}

bool XHeightMapController::Release()
{
	if (!bStart) return false;
	// 포인터기 때문에 clear 작업만 해준다.
	m_LeafNodeList.clear();
	m_CrashNode.clear();
	m_CraseVertex.clear();
	return true;
}

XHeightMapController::XHeightMapController()
{
}


XHeightMapController::~XHeightMapController()
{
}
