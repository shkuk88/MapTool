#include "XSelect.h"

//ī�޶� ������ ����
void XSelect::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;

		m_vCenter.x = pWorld->_41;
		m_vCenter.y = pWorld->_42;
		m_vCenter.z = pWorld->_43;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}
	Update();
}

void XSelect::Update()
{
	POINT ptCursor;
	GetPhysicalCursorPos(&ptCursor);		//���콺 Ŀ���� ����
	ScreenToClient(g_hWnd, &ptCursor);		//��ũ����ǥ�� ���콺 ��ǥ�� ����

	D3DXVECTOR3 k;

	//ȭ����ǥ -> ������ǥ�� ��ȯ [å103��]
	k.x = (((2.0f*ptCursor.x) / g_rtClient.right) - 1) / m_matProj._11;	//11,22 ������ ������ ��ġ�⿡ ����� �� �۾� ��ü��
	k.y = -(((2.0f*ptCursor.y) / g_rtClient.bottom) - 1) / m_matProj._22;
	k.z = 1.0f;

	////���� (���� ���� )��ǥ����ġ
	D3DXMATRIX mWorldView = m_matWorld * m_matView;
	D3DXMATRIX m;
	D3DXMatrixInverse(&m, NULL, &mWorldView);		//���� ���� �����


	m_Ray.vOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//������ ����
	m_Ray.vDirection = D3DXVECTOR3(k.x, k.y, k.z);  //������ ����

	D3DXVec3TransformCoord(&m_Ray.vOrigin, &m_Ray.vOrigin, &m);			//���� // coord  w�� 1 ����
	D3DXVec3TransformNormal(&m_Ray.vDirection, &m_Ray.vDirection, &m);	//���� // Normal w�� 0���� ���߰� ���
	D3DXVec3Normalize(&m_Ray.vDirection, &m_Ray.vDirection);  //����ȭ�� ������� �����ִ� �۾���

															  //	}
}

D3DXVECTOR3 XSelect::GetIntersection()
{
	return m_vIntersection;
}

//������ ã�´�
bool XSelect::GetIntersection(D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR3 vNormal, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
	D3DXVECTOR3 vDirection = vEnd - vStart;				//���⺤��
	float D = D3DXVec3Dot(&vNormal, &vDirection);		//���⺤�� �ﰢ��? �븻������ ����=>����� �븻����
	float a0 = D3DXVec3Dot(&vNormal, &(v0 - vStart));	//�븻���� , v0 - ����� => ������ ���� ����������� ����
	float fT = a0 / D;									//���������� �Ÿ�
	if (fT < 0.0f || fT > 1.0f) { return false; }
	m_vIntersection = vStart + vDirection * fT;		//����� + (���⺤�� * ���������Ÿ�) => ����
	return true;
}

//�ﰢ���� ����  ������ ã�� u�� v��
bool XSelect::IntersectTriangle(const D3DXVECTOR3& origin, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT* u, FLOAT* v)
{
	D3DXVECTOR3 edge1 = v1 - v0;
	D3DXVECTOR3 edge2 = v2 - v0;

	D3DXVec3Cross(&pvec, &dir, &edge2);		//v���� = ���� x ����2	= ������ �ϸ� ���븻 ���� P���Ͱ� ����� ���� ����?
	FLOAT det = D3DXVec3Dot(&edge1, &pvec);	//�ﰢ�� ���̱��� ����   


	D3DXVECTOR3 tvec; // ������ ����� �� �� �ֵ��� det ������ �����´�.

	if (det > 0)			//tvec�ﰢ���� ���� ���� �ڸ� ���� ��ŷ
	{
		tvec = origin - v0;		//�ﰢ�� ������ ����
	}
	else
	{
		tvec = v0 - origin;
		det = -det;				//�����̶� ���� ��ȯ?
	}

	if (det < 0.0001f)
	{
		return false;
	}

	// Calculate U parameter and test bounds
	*u = D3DXVec3Dot(&tvec, &pvec);				//��������� ���� t���� �ﰢ�� ���̱��̸�ŭ�� p����  ����
	if (*u < 0.0f || *u > det)					//0���� �۰�, �ﰢ������ ���� ũ�ϱ� �ƿ�
	{
		return false;
	}

	//D3DXVECTOR3 qvec;
	D3DXVec3Cross(&qvec, &tvec, &edge1);		//��������� ���� t����, edg1 1 ���� => Q����	

	*v = D3DXVec3Dot(&dir, &qvec);				//0���� �۰�, �ﰢ������ ���� ũ�ϱ� �ƿ� (u+v =1)

	if (*v < 0.0f || *u + *v > det)
	{
		return false;
	}

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot(&edge2, &qvec);
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	return true;
}

// AABB �浹
bool XSelect::IntersectBox(X_Box* pBox)
{
	D3DXVECTOR3 tmin;
	tmin.x = (pBox->vMin.x - m_Ray.vOrigin.x) / m_Ray.vDirection.x + 0.001f;
	tmin.y = (pBox->vMin.y - m_Ray.vOrigin.y) / m_Ray.vDirection.y + 0.001f;
	tmin.z = (pBox->vMin.z - m_Ray.vOrigin.z) / m_Ray.vDirection.z + 0.001f;

	D3DXVECTOR3 tmax;
	tmax.x = (pBox->vMax.x - m_Ray.vOrigin.x) / m_Ray.vDirection.x + 0.001f;
	tmax.y = (pBox->vMax.y - m_Ray.vOrigin.y) / m_Ray.vDirection.y + 0.001f;
	tmax.z = (pBox->vMax.z - m_Ray.vOrigin.z) / m_Ray.vDirection.z + 0.001f;

	// ���� ������ �ľ�
	D3DXVECTOR3 real_min;
	real_min.x = min(tmin.x, tmax.x);
	real_min.y = min(tmin.y, tmax.y);
	real_min.z = min(tmin.z, tmax.z);
	D3DXVECTOR3 real_max;
	real_max.x = max(tmin.x, tmax.x);
	real_max.y = max(tmin.y, tmax.y);
	real_max.z = max(tmin.z, tmax.z);

	float minmax = min(min(real_max.x, real_max.y), real_max.z);
	float maxmin = max(max(real_min.x, real_min.y), real_min.z);

	if (minmax >= maxmin)
	{
		m_vIntersection = m_Ray.vOrigin + m_Ray.vDirection* maxmin;
		return true;
	}
	return false;
}

// OBB �浹
bool XSelect::CheakOBBToRay(X_Box* pBox)
{
	if (pBox == NULL)
	{
		return false;
	}
	float t_min = -999999.0f;
	float t_max = 999999.0f;
	float  f[3], fa[3], s[3], sa[3];

	D3DXVECTOR3 vR = m_Ray.vOrigin - pBox->vCenter;

	for (int v = 0; v < 3; v++)
	{
		f[v] = D3DXVec3Dot(&pBox->vAxis[v], &m_Ray.vDirection);
		s[v] = D3DXVec3Dot(&pBox->vAxis[v], &vR);
		fa[v] = fabs(f[v]);
		sa[v] = fabs(s[v]);

		if (sa[v] > pBox->fExtent[v] && s[v] * f[v] >= 0.0f)
		{
			return false;
		}
		float t1 = (-s[v] - pBox->fExtent[v]) / f[v];
		float t2 = (-s[v] + pBox->fExtent[v]) / f[v];
		if (t1 > t2)
		{
			swap(t1, t2);
		}
		t_min = max(t_min, t1);
		t_max = min(t_max, t2);
		if (t_min > t_max)
		{
			return false;
		}
	}

	float  fCross[3], fRhs;
	D3DXVECTOR3 vDxR;
	D3DXVec3Cross(&vDxR, &m_Ray.vDirection, &vR);
	// D X pBox->vAxis[0]  �и���
	fCross[0] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[0]));
	float fAxis2 = pBox->fExtent[1] * fa[2];
	float fAxis1 = pBox->fExtent[2] * fa[1];
	fRhs = fAxis2 + fAxis1;
	if (fCross[0] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}
	// D x pBox->vAxis[1]  �и���
	fCross[1] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[1]));
	fRhs = pBox->fExtent[0] * fa[2] + pBox->fExtent[2] * fa[0];
	if (fCross[1] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}
	// D x pBox->vAxis[2]  �и���
	fCross[2] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[2]));
	fRhs = pBox->fExtent[0] * fa[1] + pBox->fExtent[1] * fa[0];
	if (fCross[2] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}

	m_vIntersection = m_Ray.vOrigin + m_Ray.vDirection* t_min;
	return true;
}

bool XSelect::CheckPick(D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2)
{
	FLOAT fBary1, fBary2;
	FLOAT fDist;

	if (IntersectTriangle(m_Ray.vOrigin, m_Ray.vDirection, v0, v1, v2, &fDist, &fBary1, &fBary2))
	{
		m_vSrcVex[0] = v0;
		m_vSrcVex[1] = v1;
		m_vSrcVex[2] = v2;

		return true;
	}

	return false;
}

bool XSelect::PointInRect(POINT pt, RECT rc)
{
	if (rc.left <= pt.x && pt.x <= rc.right
		&& rc.top <= pt.y && pt.y >= rc.bottom)
	{
		return true;
	}
	return false;
}


XSelect::XSelect()
{
	m_vSrcVex[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vSrcVex[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vSrcVex[2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_fPickDistance = 10000.0f;

	m_vIntersection = D3DXVECTOR3(0, 0, 0);
	m_vCenter = D3DXVECTOR3(0, 0, 0);

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}


XSelect::~XSelect()
{
}
