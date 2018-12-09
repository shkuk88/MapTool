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
		for (int iCorner = 0; iCorner < 4; iCorner++)
		{
				PNCT_Vertex* pControl = &m_pMap->m_VertexList[pNode->m_CornerIndex[iCorner]];
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
}

void XHeightMapController::HeightFlat()
{
}

bool XHeightMapController::Frame()
{
	if (!bStart || m_bHeightCtrlState == None) return false;
	if (I_Input.m_MouseState[0])
	{
		CheakInRange();
		if (m_bHeightCtrlState == Up || m_bHeightCtrlState == Down)
		{
			HeightUpDown();
		}
		//else  // Flat 상황
		//{
		//	HeightFlat();
		//}
		if (m_bHeightCtrlState != None) m_pMap->CalcPerVertexNormalsFastLookUp();
	}
	return true;
}

bool XHeightMapController::Release()
{
	if (!bStart) return false;
	// 포인터기 때문에 clear 작업만 해준다.
	m_LeafNodeList.clear();
	return true;
}

XHeightMapController::XHeightMapController()
{
}


XHeightMapController::~XHeightMapController()
{
}
