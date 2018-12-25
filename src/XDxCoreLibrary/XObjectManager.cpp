#include "XObjectManager.h"

TString XObjectManager::AddObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TString szFileName)
{
	for (auto iter : m_ObjectList)
	{
		if (iter.first == szFileName)
			return iter.first;
	}
	TString szPath = _T("../../Data/Map/Save/")+ szFileName;
	shared_ptr<XViewer> pObject = make_shared<XViewer>();
	TCHAR* szAddPath = (TCHAR*)szPath.c_str();
	pObject->Init(szAddPath, pDevice, pContext, _T("../../Data/Map/Shader/Ase.hlsl"), _T("../../Data/Map/Shader/Ase.hlsl"));
	m_ObjectList[szFileName] = pObject;
	return szFileName;
}

bool XObjectManager::AddObjectWorldMat(TString szObj, D3DXMATRIX matWorld)
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

void XObjectManager::SetMatrix(D3DXMATRIX * matView, D3DXMATRIX * matProj)
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

bool XObjectManager::Init()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	return true;
}

bool XObjectManager::Frame()
{	
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

bool XObjectManager::Render(ID3D11DeviceContext* pContext)
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

bool XObjectManager::Release()
{
	m_ObjectList.clear();
	m_ObjectMatrix.clear();
	return true;
}

XObjectManager::XObjectManager()
{

}


XObjectManager::~XObjectManager()
{
}
