#include "XObjectController.h"



TString XObjectController::AddObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TString szFileName)
{
	for (auto iter : m_ObjectList)
	{
		if (iter.first == szFileName)
			return iter.first;
	}
	shared_ptr<XViewer> pObject = make_shared<XViewer>();
	TCHAR* szTemp = (TCHAR*)szFileName.c_str();
	pObject->Init(szTemp, pDevice, pContext, _T("../../Data/Map/Shader/Ase.hlsl"), _T("../../Data/Map/Shader/Ase.hlsl"));
	m_ObjectList[szFileName] = pObject;
	return szFileName;
}

bool XObjectController::AddObjectWorldMat(TString szObj, D3DXMATRIX matWorld)
{
	for (auto iter : m_ObjectList)
	{
		if (iter.first == szObj)
		{
			m_ObjectMatrix[szObj].push_back(matWorld);
			return true;
		}
	}
	return false;
}

D3DXMATRIX XObjectController::GetObjectWorldMat(TString szObj, int iSelectMatNum)
{
	return m_ObjectMatrix[szObj][iSelectMatNum];
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
	return m_ObjectMatrix[szObj].size() - 1;
}

bool XObjectController::MoveObject()
{
	if (!m_bSelect)	return false;
	if (I_Input.m_MouseState[0]== KEY_HOLD)
	{
		m_ObjectMatrix[m_szSelectObject.c_str()][m_iSelectMatNum]._41 = m_vIntersection.x;
		m_ObjectMatrix[m_szSelectObject.c_str()][m_iSelectMatNum]._42 = m_pMap->GetHeight(m_vIntersection.x, m_vIntersection.z);
		m_ObjectMatrix[m_szSelectObject.c_str()][m_iSelectMatNum]._43 = m_vIntersection.z;
	}
	else if (I_Input.KeyCheck(DIK_RETURN) == KEY_PUSH)
	{
		m_bSelect = false;
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

bool XObjectController::Frame()
{
	if (!m_bStart)	return true;
	FindIntersection();
	MoveObject();
	// 모든 오브젝트를 순회하는 Frame
	for (auto pObject : m_ObjectList)
	{
		for (int iMatrix = 0; iMatrix < m_ObjectMatrix[pObject.first].size(); iMatrix++)
		{
			pObject.second->Frame();
		}
	}
	return true;
}

bool XObjectController::Render(ID3D11DeviceContext * pContext)
{
	// 모든 오브젝트를 순회하는  Render
	for (auto pObject : m_ObjectList)
	{
		for (int iMatrix = 0; iMatrix < m_ObjectMatrix[pObject.first].size(); iMatrix++)
		{
			pObject.second->SetMatrix(&m_ObjectMatrix[pObject.first][iMatrix], &m_matView, &m_matProj);
			pObject.second->Render(pContext);
		}
	}
	return true;
}

bool XObjectController::Release()
{
	return true;
}


XObjectController::XObjectController()
{
}


XObjectController::~XObjectController()
{
}
