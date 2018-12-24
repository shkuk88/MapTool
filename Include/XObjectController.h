#pragma once
#include "XMapController.h"
#include "XViewer.h"

class XObjectController :public XMapController
{
public:
	map<TString, shared_ptr<XViewer>>	m_ObjectList;		// 같은 오브젝트는 하나만 저장
	map<TString, vector<D3DXMATRIX>>	m_ObjectMatrix;		// 오브젝트 별 matWorld 배열 
	bool m_bSelect = false;
	TString m_szSelectObject;
	int m_iSelectMatNum = -1;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
public:
	TString AddObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TString szFileName);
	bool AddObjectWorldMat(TString szObj, D3DXMATRIX matWorld);
	D3DXMATRIX GetObjectWorldMat(TString szObj, int iSelectMatNum);
	bool SetSelectObject(TString szObj, int iSelectMatNum);
	bool MoveObject();
	void SelectModeOn();
	void SelectModeOff();
	void SetMatrix(D3DXMATRIX* matView, D3DXMATRIX* matProj);
	int GetLastMatIndex(TString szObj);
public:
	virtual void Start() override;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render(ID3D11DeviceContext* pContext) override;
	virtual bool Release() override;
public:
	XObjectController();
	virtual ~XObjectController();
};

