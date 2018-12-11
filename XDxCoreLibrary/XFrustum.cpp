#include "XFrustum.h"

bool XFrustum::CreateFrustum()
{
	//Á¤¸é
	m_vFrustum[0] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_vFrustum[1] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_vFrustum[2] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	m_vFrustum[3] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	//µÞ¸é
	m_vFrustum[4] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_vFrustum[5] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vFrustum[6] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_vFrustum[7] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);

	D3DXMATRIX matVP;
	D3DXMatrixMultiply(&matVP, &m_matView, &m_matProj);
	D3DXMatrixInverse(&matVP, NULL, &matVP);

	for (int index = 0; index < 8; index++)
	{
		D3DXVec3TransformCoord(&m_vFrustum[index], &m_vFrustum[index], &matVP);
	}

	m_Plane[0].CreatePlane(m_vFrustum[0], m_vFrustum[3], m_vFrustum[2]);		//Á¤¸é
	m_Plane[1].CreatePlane(m_vFrustum[4], m_vFrustum[5], m_vFrustum[6]);		//ÈÄ¸é
	m_Plane[2].CreatePlane(m_vFrustum[0], m_vFrustum[4], m_vFrustum[7]);		//ÁÂ
	m_Plane[3].CreatePlane(m_vFrustum[1], m_vFrustum[2], m_vFrustum[6]);		//¿ì
	m_Plane[4].CreatePlane(m_vFrustum[0], m_vFrustum[1], m_vFrustum[5]);		//»ó
	m_Plane[5].CreatePlane(m_vFrustum[2], m_vFrustum[3], m_vFrustum[7]);		//ÇÏ

	return true;
}

void XFrustum::SetMatrix(D3DXMATRIX *matWorld, D3DXMATRIX *matView, D3DXMATRIX *matProj)
{
	if (matWorld != NULL)
	{
		m_matWorld = *matWorld;
	}
	if (matWorld != NULL)
	{
		m_matView = *matView;
	}
	if (matWorld != NULL)
	{
		m_matProj = *matProj;
	}
}

bool XFrustum::CheckPoint(D3DXVECTOR3* pPos)
{
	float PlaneToPoint = 0.0f;
	for (int index = 0; index < 6; index++)
	{
		PlaneToPoint = m_Plane[index].a * pPos->x + m_Plane[index].b * pPos->y + m_Plane[index].c * pPos->z + m_Plane[index].d;
		if (PlaneToPoint < 0.0f)
		{
			return false;
		}
	}
	return true;
}
bool XFrustum::CheckSphere(X_Sphere* pSphere)
{
	float PlaneToCenter = 0.0f;
	for (int index = 0; index < 6; index++)
	{
		PlaneToCenter = m_Plane[index].a * pSphere->vCenter.x + m_Plane[index].b * pSphere->vCenter.y + m_Plane[index].c * pSphere->vCenter.z + m_Plane[index].d;
		if (PlaneToCenter <= -pSphere->fRadius)
		{
			return false;
		}
	}
	return true;
}
bool XFrustum::CheckOBBInPlane(X_Box* pOBB)
{
	float PlaneToCenter = 0.0f;
	float distance = 0.0f;
	D3DXVECTOR3	vDir;

	for (int index = 0; index < 6; index++)
	{
		vDir = pOBB->vAxis[0] * pOBB->fExtent[0];		//Ãà º¤ÅÍ¿¡ ½ºÄ®¶ó °ªÀ» °öÇØ¼­ º¤ÅÍ¸¦ ¸¸µë
		distance = fabs(m_Plane[index].a * vDir.x + m_Plane[index].b * vDir.y + m_Plane[index].c * vDir.z + m_Plane[index].d);
		vDir = pOBB->vAxis[1] * pOBB->fExtent[1];
		distance += fabs(m_Plane[index].a * vDir.x + m_Plane[index].b * vDir.y + m_Plane[index].c * vDir.z + m_Plane[index].d);
		vDir = pOBB->vAxis[2] * pOBB->fExtent[2];
		distance += fabs(m_Plane[index].a * vDir.x + m_Plane[index].b *vDir.y + m_Plane[index].c * vDir.z + m_Plane[index].d);

		PlaneToCenter = m_Plane[index].a * pOBB->vCenter.x + m_Plane[index].b * pOBB->vCenter.y + m_Plane[index].c * pOBB->vCenter.z + m_Plane[index].d;
		if (PlaneToCenter <= -distance)
		{
			return false;
		}
	}
	return true;
}

X_POSITION XFrustum::CheckPoitionOBBInPlane(X_Box*  pOBB)	
{
	float		fPlaneToCenter = 0.0;
	float		fDistance = 0.0f;
	D3DXVECTOR3 vDir;
	X_POSITION  x_Position;

	x_Position = P_FRONT;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pOBB->vAxis[0] * pOBB->fExtent[0];
		fDistance = fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b * vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = pOBB->vAxis[1] * pOBB->fExtent[1];
		fDistance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b * vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = pOBB->vAxis[2] * pOBB->fExtent[2];
		fDistance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b * vDir.y + m_Plane[iPlane].c * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].a * pOBB->vCenter.x + m_Plane[iPlane].b * pOBB->vCenter.y + m_Plane[iPlane].c * pOBB->vCenter.z + m_Plane[iPlane].d;

		if (fPlaneToCenter <= fDistance)		
			x_Position = P_SPANNING;
		if (fPlaneToCenter + 2.0f < -fDistance)	
			return P_BACK;
	}
	return x_Position;
}

XFrustum::XFrustum()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}


XFrustum::~XFrustum()
{

}
