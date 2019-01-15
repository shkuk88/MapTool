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

bool XObjectManager::DelObjWorldMat(TString szObjName, int iSelecObj)
{
	m_ObjectMatrix[szObjName].erase(m_ObjectMatrix[szObjName].begin()+iSelecObj);
	return true;
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

void XObjectManager::AddCollider(TString szObject, X_Box OBB)
{
	XBoxShape box;
	box.Create(_T("../../Data/Map/Shader/BoxShader.hlsl"), _T("../../Data/Map/Shader/BoxShader.hlsl"), "VS", "PS");
	box.SetCollider(I_Device.m_pD3dContext.Get(), OBB);
	m_OBBViewerList[szObject].push_back(box);
}

void XObjectManager::ViewColliderSwitch()
{
	if (m_bViewCollider)
		m_bViewCollider = false;
	else
		m_bViewCollider = true;
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

void XObjectManager::DrawCollider(TString szObject, ID3D11DeviceContext * pContext, D3DXMATRIX matObjWorld)
{
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	for (int iOBBCount = 0; iOBBCount < m_OBBViewerList[szObject].size(); iOBBCount++)
	{
		m_OBBViewerList[szObject][iOBBCount].SetMatrix(&matObjWorld, &m_matView, &m_matProj);
		m_OBBViewerList[szObject][iOBBCount].Render(pContext);
	}
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool XObjectManager::Render(ID3D11DeviceContext* pContext)
{
	// 모든 오브젝트를 순회하는  Render
	for (auto pObject : m_ObjectList)
	{
		for (int iMatrix = 0; iMatrix < m_ObjectMatrix[pObject.first].size(); iMatrix++)
		{
			D3DXMATRIX matObjectWorld = m_ObjectMatrix[pObject.first][iMatrix];
			pObject.second->SetMatrix(&matObjectWorld, &m_matView, &m_matProj);
			pObject.second->Render(pContext);

			if (m_bViewCollider)
			{
				DrawCollider(pObject.first, pContext, matObjectWorld);
			}
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
