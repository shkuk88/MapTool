#include "XObjectController.h"



TString XObjectController::AddObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TString szFileName)
{
	return I_Object.AddObject(pDevice, pContext, szFileName);
}

bool XObjectController::AddObjectWorldMat(TString szObj, D3DXMATRIX matWorld)
{
	return I_Object.AddObjectWorldMat(szObj, matWorld);
}

D3DXMATRIX XObjectController::GetObjectWorldMat(TString szObj, int iSelectMatNum)
{
	return I_Object.m_ObjectMatrix[szObj][iSelectMatNum];
}

bool XObjectController::SetSelectObject(TString szObj, int iSelectMatNum)
{
	if (!m_bSelect)
		return false;
	m_szSelectObject = szObj;
	m_iSelectMatNum = iSelectMatNum;
	return true;
}

void XObjectController::SelectModeOn()
{
	m_bSelect = true;
}

void XObjectController::SelectModeOff()
{
	m_bSelect = false;
}

void XObjectController::SetMatrix(D3DXMATRIX * matView, D3DXMATRIX * matProj)
{
	if (matView)
	{
		m_matView = *matView;
	}
	if (matProj)
	{
		m_matProj = *matProj;
	}
}

int XObjectController::GetLastMatIndex(TString szObj)
{
	return I_Object.m_ObjectMatrix[szObj].size() - 1;
}

bool XObjectController::MoveObject(ID3D11DeviceContext * pContext)
{
	if (!m_bSelect)	return false;
	if (I_Input.m_MouseState[0]== KEY_HOLD)
	{
		I_Object.m_ObjectMatrix[m_szSelectObject.c_str()][m_iSelectMatNum]._41 = m_vIntersection.x;
		I_Object.m_ObjectMatrix[m_szSelectObject.c_str()][m_iSelectMatNum]._42 = m_pMap->GetHeight(m_vIntersection.x, m_vIntersection.z);
		I_Object.m_ObjectMatrix[m_szSelectObject.c_str()][m_iSelectMatNum]._43 = m_vIntersection.z;
	}
}

void XObjectController::Start()
{
	XMapController::Start();
	Init();
}

bool XObjectController::Init()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	return true;
}

bool XObjectController::Frame() { return true; }
bool XObjectController::Frame(ID3D11DeviceContext* pContext)
{
	if (!m_bStart)	return true;
	if(!FindIntersection())   return true;
	MoveObject(pContext);
	I_Object.Frame();
	// 모든 오브젝트를 순회하는 Frame
	//for (auto pObject : I_Object.m_ObjectList)
	//{
	//	for (int iMatrix = 0; iMatrix < I_Object.m_ObjectMatrix[pObject.first].size(); iMatrix++)
	//	{
	//		pObject.second->Frame();
	//	}
	//}
	return true;
}

bool XObjectController::Render(ID3D11DeviceContext * pContext)
{
	if (!m_bSelect)
	{
		I_Object.SetMatrix(&m_matView, &m_matProj);
		I_Object.Render(pContext);
		return true;
	}

	// 선택모드시
	for (auto pObject : I_Object.m_ObjectList)
	{
		for (int iMatrix = 0; iMatrix < I_Object.m_ObjectMatrix[pObject.first].size(); iMatrix++)
		{
			pObject.second->SetMatrix(&I_Object.m_ObjectMatrix[pObject.first][iMatrix], &m_matView, &m_matProj);
			if (!_tcscmp(pObject.first.c_str(), m_szSelectObject.c_str()) && iMatrix == m_iSelectMatNum)
			{
				SelectObjTransparency(pContext);
				m_bTransparency = true;
			}
			pObject.second->Render(pContext);
			if (m_bTransparency)
			{
				I_Object.m_ObjectList[m_szSelectObject]->SelectObjOpaque(pContext);
				m_bTransparency = false;
			}
		}
	}
	return true;
}

void XObjectController::SelectObjTransparency(ID3D11DeviceContext* pContext)
{
	if (m_bSelect)
		I_Object.m_ObjectList[m_szSelectObject]->SelectObjTransparency(pContext);
}
bool XObjectController::Release()
{
	I_Object.Release();
	return true;
}


XObjectController::XObjectController()
{
}


XObjectController::~XObjectController()
{
}
