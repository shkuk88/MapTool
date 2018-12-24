#include "XQuadTree.h"

bool XQuadTree::Build(float width, float height) 
{
	//RootNode ����
	m_pRootNode = CreateNode(NULL, -width * 0.5f, width*0.5f, -height * 0.5f, height*0.5f);
	//RootNode ������ ����
	if (BuildTree(m_pRootNode))
	{
		return true;
	}
	return false;
}

XNode* XQuadTree::CreateNode(XNode* pParent, float fL, float fR, float fB, float fT)
{
	XNode* pNode = NULL;
	SAFE_NEW(pNode, XNode);
	assert(pNode);// debug������ �����ϰ� ���ǽ��� false�̸� ���α׷� �ߴ�

	pNode->m_Box.vMin = D3DXVECTOR3(fL, 0.0f, fB);
	pNode->m_Box.vMax = D3DXVECTOR3(fR, 0.0f, fT);
	pNode->m_Box.vCenter = (pNode->m_Box.vMin + pNode->m_Box.vMax)*0.5f;
	pNode->m_Box.fExtent[0] = pNode->m_Box.vMax.x - pNode->m_Box.vCenter.x;
	pNode->m_Box.fExtent[1] = pNode->m_Box.vMax.y - pNode->m_Box.vCenter.y;
	pNode->m_Box.fExtent[2] = pNode->m_Box.vMax.z - pNode->m_Box.vCenter.z;
	pNode->m_CornerList.reserve(4);

	//RootNode�̸� Depth �߰��Ǵ� �� ���
	if (pParent)
	{
		pNode->m_dwDepth = pParent->m_dwDepth + 1;
	}
	pNode->m_CornerList[0] = D3DXVECTOR3(fL, 0.0f, fT);
	pNode->m_CornerList[1] = D3DXVECTOR3(fR, 0.0f, fT);    //XBox�� max��
	pNode->m_CornerList[2] = D3DXVECTOR3(fR, 0.0f, fB);
	pNode->m_CornerList[3] = D3DXVECTOR3(fL, 0.0f, fB);    //XBox�� min��

	return pNode;
}

bool XQuadTree::BuildTree(XNode* pNode)
{
	if (pNode != NULL)
	//���� ���� ���� Ȯ�� �� ����
	if (SubDivide(pNode))
	{
		for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
		{
			BuildTree(pNode->m_ChildList[iNode]);//���� �Ŀ��� �ڽĳ�带 ��������� ���ǿ� ���߾ ����
		}
	}

	return true;
}

bool XQuadTree::SubDivide(XNode* pNode)
{
	//���� ���� ����
	if (pNode == NULL)
	{
		return false;
	}

	//����
	if (m_iMaxDepth <= pNode->m_dwDepth)
	{
		pNode->m_bLeaf = TRUE;		//�ִ� Depth��ŭ �����Ŀ� ������ Node�� LeafNodeǥ��
		return false;
	}

	//���� ��� ũ�� ���
	float fWidth = (pNode->m_CornerList[1].x - pNode->m_CornerList[0].x) / 2;
	float fHeight = (pNode->m_CornerList[0].z - pNode->m_CornerList[3].z) / 2;

	// ������ ���� ũ�⺸�� �ڽĳ�尡 �۾����� ����X
	if (fWidth < 1 || fHeight < 1)
	{
		pNode->m_bLeaf = TRUE;
		return false;
	}
	// �ڽ�Node�� RootNodeó�� ��带 �����ϱ� ���� ��ǥ�� �Ѱ���
	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[0].x, pNode->m_CornerList[0].x + fWidth, pNode->m_CornerList[0].z - fHeight, pNode->m_CornerList[0].z));
	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[0].x + fWidth, pNode->m_CornerList[1].x, pNode->m_CornerList[0].z - fHeight, pNode->m_CornerList[0].z));
	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[0].x + fWidth, pNode->m_CornerList[1].x, pNode->m_CornerList[2].z, pNode->m_CornerList[0].z - fHeight));
	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[0].x, pNode->m_CornerList[0].x + fWidth, pNode->m_CornerList[2].z, pNode->m_CornerList[0].z - fHeight));

	return true;
}

XNode* XQuadTree::FindNode(XNode* pNode, XShape* pObj)
{
	do {
		for (DWORD dwChild = 0; dwChild < pNode->m_ChildList.size(); dwChild++)
		{
			if (pNode->m_ChildList[dwChild] && CheckRect(pNode->m_ChildList[dwChild], pObj))
			{
				m_QuadTreeQueue.push(pNode->m_ChildList[dwChild]);
			}
		}
		if (m_QuadTreeQueue.empty()) break;
		pNode = m_QuadTreeQueue.front();
		m_QuadTreeQueue.pop();
	} while (pNode);
	return pNode;
}

bool XQuadTree::CheckRect(XNode* pNode, XShape* pObj)
{
	if (pNode->m_Box.vMin.x <= pObj->m_Box.vMin.x && pNode->m_Box.vMax.x >= pObj->m_Box.vMax.x)
	{
		if (pNode->m_Box.vMin.z <= pObj->m_Box.vMin.z && pNode->m_Box.vMax.z >= pObj->m_Box.vMax.z)
		{
			return true;
		}
	}
	return false;
}

void XQuadTree::SetCamera(XCamera* pCamera)
{
	m_pCamera = pCamera;
}

bool XQuadTree::PreFrame()
{
	m_DrawNodeList.clear();
	if (!m_pCamera) return false;

	return true;
}

bool XQuadTree::Frame()
{
	if (!PreFrame()) return false; // Ŭ���� �۾��� ���ְ� �ٽ� �׸� ���� ������Ʈ�� ã�´�.
	DrawFindNode(m_pRootNode);
	return true;
}

void XQuadTree::DrawFindNode(XNode* pNode)
{
	// ��ü���� ������ �ʿ��ϴ�. üũ�� ���� �ٸ��� �����.
	X_POSITION x_Pos = m_pCamera->CheckPoitionOBBInPlane(&pNode->m_Box); // ���� OBB�� �浹 ó���� ������ ���� �浹 ó���� �ϸ� ��귮�� ���� �� ���� ������ ����ȭ�� ���� ���� �浹ó���� �Ѵ�.

	//if (pNode->m_dwDepth > RenderDepth) return; // ���� ������ �����ϰ� ���� ��
	// ���� �ڵ��� Visual Node,Visual Obj �Լ��� �������� �ʾҴµ�, Ž���� ��������Ž������ �Ѵ�. ������ ���� �˻��� �غ��� �Ǳ� ����.
	// ������� �� ���� ������ ���ܵ��� �ʾҴٸ�(���� �ְų� ���� ����)�߰��Ѵ�.
	if (pNode->m_bLeaf == TRUE && x_Pos != P_BACK)
	{
		m_DrawNodeList.push_back(pNode);
		return;
	}
	// ������ ���ԵǾ� ������ �ڽ��� Ž������ �ʰ� ��带 �߰��Ѵ�.
	if (x_Pos == P_FRONT)
	{
		m_DrawNodeList.push_back(pNode);
		return;
	}
	// ������ �ִ� ��忡 ���Ե� ������Ʈ üũ, �ٸ� ������ ¥�� �Ǽ� ��ǻ� ���� ����
	if (x_Pos == P_SPANNING)
	{
	
	}

	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	{
		DrawFindNode(pNode->m_ChildList[iNode]);
	}
}

XQuadTree::XQuadTree()
{
	m_pRootNode = NULL;
	m_iMaxDepth = MAX_DEPTH;
}

XQuadTree::~XQuadTree()
{
	SAFE_DEL(m_pRootNode);
}
