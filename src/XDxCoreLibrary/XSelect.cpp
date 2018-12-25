#include "XSelect.h"

//카메라가 포인터 형식
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
	GetPhysicalCursorPos(&ptCursor);		//마우스 커서를 얻음
	ScreenToClient(g_hWnd, &ptCursor);		//스크린좌표의 마우스 좌표를 얻음

	D3DXVECTOR3 k;

	//화면좌표 -> 투영좌표로 변환 [책103장]
	k.x = (((2.0f*ptCursor.x) / g_rtClient.right) - 1) / m_matProj._11;	//11,22 성분이 정점에 미치기에 역행렬 곱 작업 대체함
	k.y = -(((2.0f*ptCursor.y) / g_rtClient.bottom) - 1) / m_matProj._22;
	k.z = 1.0f;

	////광선 (방향 벡터 )좌표계일치
	D3DXMATRIX mWorldView = m_matWorld * m_matView;
	D3DXMATRIX m;
	D3DXMatrixInverse(&m, NULL, &mWorldView);		//월드 뷰의 역행렬


	m_Ray.vOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//광선의 원점
	m_Ray.vDirection = D3DXVECTOR3(k.x, k.y, k.z);  //광선의 방향

	D3DXVec3TransformCoord(&m_Ray.vOrigin, &m_Ray.vOrigin, &m);			//원점 // coord  w를 1 맞춤
	D3DXVec3TransformNormal(&m_Ray.vDirection, &m_Ray.vDirection, &m);	//방향 // Normal w를 0으로 맞추고 사용
	D3DXVec3Normalize(&m_Ray.vDirection, &m_Ray.vDirection);  //정규화가 역행렬을 곱해주는 작업임

															  //	}
}

D3DXVECTOR3 XSelect::GetIntersection()
{
	return m_vIntersection;
}

//교점을 찾는다
bool XSelect::GetIntersection(D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR3 vNormal, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
	D3DXVECTOR3 vDirection = vEnd - vStart;				//방향벡터
	float D = D3DXVec3Dot(&vNormal, &vDirection);		//방향벡터 삼각형? 노말벡터을 외적=>평면의 노말벡터
	float a0 = D3DXVec3Dot(&vNormal, &(v0 - vStart));	//노말벡터 , v0 - 출발점 => 시작점 부터 삼격형까지의 벡터
	float fT = a0 / D;									//교점까지의 거리
	if (fT < 0.0f || fT > 1.0f) { return false; }
	m_vIntersection = vStart + vDirection * fT;		//출발점 + (방향벡터 * 교점까지거리) => 교점
	return true;
}

//삼각형의 교점  교점을 찾고 u와 v로
bool XSelect::IntersectTriangle(const D3DXVECTOR3& origin, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT* u, FLOAT* v)
{
	D3DXVECTOR3 edge1 = v1 - v0;
	D3DXVECTOR3 edge2 = v2 - v0;

	D3DXVec3Cross(&pvec, &dir, &edge2);		//v벡터 = 방향 x 엣지2	= 외적을 하면 평면노말 같은 P벡터가 만들어 진다 위쪽?
	FLOAT det = D3DXVec3Dot(&edge1, &pvec);	//삼각형 높이까지 벡터   


	D3DXVECTOR3 tvec; // 내적이 양수가 될 수 있도록 det 방향을 뒤집는다.

	if (det > 0)			//tvec삼각형이 앞을 보던 뒤를 보던 피킹
	{
		tvec = origin - v0;		//삼각형 까지의 벡터
	}
	else
	{
		tvec = v0 - origin;
		det = -det;				//밑쪽이라 방향 전환?
	}

	if (det < 0.0001f)
	{
		return false;
	}

	// Calculate U parameter and test bounds
	*u = D3DXVec3Dot(&tvec, &pvec);				//삼격형으로 가는 t벡터 삼각형 높이길이만큼의 p벡터  내적
	if (*u < 0.0f || *u > det)					//0보다 작고, 삼각형높이 보다 크니까 아웃
	{
		return false;
	}

	//D3DXVECTOR3 qvec;
	D3DXVec3Cross(&qvec, &tvec, &edge1);		//삼격형으로 가는 t벡터, edg1 1 외적 => Q벡터	

	*v = D3DXVec3Dot(&dir, &qvec);				//0보다 작고, 삼각형높이 보다 크니까 아웃 (u+v =1)

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

// AABB 충돌
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

	// 직선 방향을 파악
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

// OBB 충돌
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
	// D X pBox->vAxis[0]  분리축
	fCross[0] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[0]));
	float fAxis2 = pBox->fExtent[1] * fa[2];
	float fAxis1 = pBox->fExtent[2] * fa[1];
	fRhs = fAxis2 + fAxis1;
	if (fCross[0] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}
	// D x pBox->vAxis[1]  분리축
	fCross[1] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[1]));
	fRhs = pBox->fExtent[0] * fa[2] + pBox->fExtent[2] * fa[0];
	if (fCross[1] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}
	// D x pBox->vAxis[2]  분리축
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
