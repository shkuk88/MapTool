#pragma once
#include "XQuadTreeIndex.h"
#include "XSelect.h"

// Sample���� ��� ó���ϸ� ������ ���� ������ ���� ��Ǵ� Contoller�� ���� texture��� Height���� ���Ŭ������
// ����� ���Ѽ� ���� ó���� ����.		ex) XHeightMapController  <-  XMapController  ->  XTextureMapController
// ���Ŀ� LOD�� ���� PatchNode����Ʈ�� �߰�

class XMapController
{
protected:
	// Map�� �����͸� ������. LOD�� �ƴϱ� ������ ������ VertexList�� �ʿ�.
	XMap* m_pMap;
	// �õ� ����
	bool bStart = false;
	// Map Control�� �ʿ��ϹǷ�, ������带 ���� ����
	vector<XNode*>	m_LeafNodeList;
	vector<XNode*>	m_CrashNode;
	vector<PNCT_Vertex*> m_CraseVertex;
	// ���콺 �����Ϳ� Node�� �浹����
	D3DXVECTOR3	m_vIntersection = { 0.0f,0.0f,0.0f };
	// ���������� ������
	float m_fRadius = 0.0f;
public:
	// ��Ʈ��带 ����Ʈ������ �޾ƿ��� �������� �����ؼ� �����ϴ� �Լ�.
	bool SetLeafNode(XNode* pRootNode);
	// Vertex Control�� ���� Map�� �����ͷ� �����´�.
	bool SetMap(XMap* pMap);
	// ��Ʈ�ѷ� �õ�
	void Start() { bStart = true; }
	bool CheakInRange();
	bool FindIntersection();
public:
	virtual bool Init() { return true; }
	virtual bool Frame() { return true; }
	virtual bool Render() { return true; }
	virtual bool Release();
public:
	XMapController();
	virtual ~XMapController();
};

