#pragma once
#include "XQuadTree.h"

class XStaticData	// PatchLevel���� �������� ������ ���� ��� ��ġ, �� 16���� ��ġ�� ����ȴ�. 
{
public:
	DWORD					dwLevel;
	vector<vector<DWORD>>	IndexList;
	vector<ComPtr<ID3D11Buffer>>	IndexBufferList;
public:
	XStaticData() {}
	~XStaticData()
	{
		for (int iList = 0; iList < IndexBufferList.size(); iList++)
		{
			IndexList[iList].clear();
		}
		IndexBufferList.clear();
		IndexList.clear();
	}
};

class XQuadTreeIndex :public XQuadTree
{
public:
	enum TREECORNER { TL = 0, TR = 1, BL = 2, BR = 3, };
	DWORD				m_dwWidth;
	DWORD				m_dwHeight;
	int					m_iNumFace;
	XMap*				m_Map;
	vector<XStaticData*>	m_pdwLodIndexArray;
public:
	bool				Build(XMap* pMap);
	bool				FindLeafNode(XNode* pNode);			// ��������� ������带 ã�Ƽ� CreateLeafNodeIndex�Լ��� Ȱ���Ͽ� ������忡 �ε����� �ο��Ѵ�.
	HRESULT				CreateLeafNodeIndex(XNode* pNode);
	XNode*				CreateNode(XNode* pParent, DWORD TL, DWORD TR, DWORD BL, DWORD BR);
	bool				SubDivide(XNode* pNode);
	bool				LeafRender(ID3D11DeviceContext* pContext, XNode* pNode);
public:
	virtual	bool		Init() { return true; }
	virtual bool		Frame();
	virtual bool		Render(ID3D11DeviceContext* pContext);
	virtual bool		Release();
public:
	XQuadTreeIndex();
	virtual ~XQuadTreeIndex();
};