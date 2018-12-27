#pragma once
#include "XViewer.h"

class XObjectManager:public XSingleton<XObjectManager>
{
private:
	friend class XSingleton<XObjectManager>;
public:
	map<TString, shared_ptr<XViewer>>	m_ObjectList;		// ���� ������Ʈ�� �ϳ��� ����
	map<TString, vector<D3DXMATRIX>>	m_ObjectMatrix;		// ������Ʈ �� matWorld �迭
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
public:
	TString AddObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TString szFileName);
	bool DelObjWorldMat(TString szObjName, int iSelecObj);
	bool AddObjectWorldMat(TString szObj, D3DXMATRIX matWorld);
	void SetMatrix(D3DXMATRIX* matView, D3DXMATRIX* matProj);
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool Release();
public:
	XObjectManager();
	virtual ~XObjectManager();
};

#define I_Object XSingleton<XObjectManager>::Get()