#pragma once
#include "XViewer.h"
#include "XShape.h"

struct XCollider
{
	vector<X_Box>	ColliderOBB;
	vector<D3DXQUATERNION> qRotation; // OBB�� �׸��� ȸ������ �˱� ���� ����
	vector<X_Box>	ColliderAABB;	  // ȸ���� ��Ű�� ���� ����
	vector<X_Sphere> ColliderSphere;
};

class XObjectManager:public XSingleton<XObjectManager>
{
private:
	friend class XSingleton<XObjectManager>;
public:
	map<TString, shared_ptr<XViewer>>	m_ObjectList;		// ���� ������Ʈ �ϳ��� ����
	map<TString, vector<D3DXMATRIX>>	m_ObjectMatrix;		// ������Ʈ �� matWorld �迭
	map<TString, XCollider>				m_ObjectCollider;	// ���� ������Ʈ �� �ϳ��� �浹ü�� ����
	map<TString, vector<XBoxShape>>		m_OBBViewerList;	// ������Ʈ ���� �� �ϳ��� �߰�
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	bool m_bViewCollider = false;
public:
	TString AddObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TString szFileName);
	bool DelObjWorldMat(TString szObjName, int iSelecObj);
	bool AddObjectWorldMat(TString szObj, D3DXMATRIX matWorld);
	void SetMatrix(D3DXMATRIX* matView, D3DXMATRIX* matProj);
	void AddCollider(TString szObject, X_Box OBB);
	void ViewColliderSwitch();
	void DrawCollider(TString szObject, ID3D11DeviceContext* pContext, D3DXMATRIX matObjWorld);
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