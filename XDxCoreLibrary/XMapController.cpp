#include "XMapController.h"

bool XMapController::SetMap(XMap* pMap)
{
	if (!pMap)	return false;
	m_pMap = pMap;
	return true;
}

bool XMapController::SetLeafNode(XNode * pNode)
{
	if (!pNode)	return false;
	if (pNode->m_dwDepth == 0) m_pRootNode = pNode;
	if (pNode->m_bLeaf == TRUE)
	{
		m_LeafNodeList.push_back(pNode);
	}
	else
	{
		SetLeafNode(pNode->m_ChildList[XQuadTreeIndex::TL]);
		SetLeafNode(pNode->m_ChildList[XQuadTreeIndex::TR]);
		SetLeafNode(pNode->m_ChildList[XQuadTreeIndex::BL]);
		SetLeafNode(pNode->m_ChildList[XQuadTreeIndex::BR]);
	}
	return true;
}

bool XMapController::CheakInRange()
{
	// ���� ������ ã��,
	// ���� ��ġ���� ������ ��ŭ ���� ��带 ��� �����Ѵ�.
	// ����� ������ 4���ε� �ٸ� ����� ������ ��ġ�� ���� ����� ���� ������ �ϳ��� ������ ���� ���� �����Ѵ�.
	if (!FindIntersection())	return false;
	m_CrashNode.clear();
	for (auto pNode : m_LeafNodeList)
	{
		D3DXVECTOR3 vLength = pNode->m_Box.vCenter - m_vIntersection;
		float fLength = D3DXVec3Length(&vLength);
		if (fLength < m_fRadius)
			m_CrashNode.push_back(pNode);
	}

	return true;
}

bool XMapController::FindIntersection()
{
	for (auto pNode : m_LeafNodeList)
	{
		if (I_Select.IntersectBox(&pNode->m_Box))
		{
			m_vIntersection = I_Select.GetIntersection();
			return true;
		}
	}
	return false;
}

bool XMapController::Release()
{
	if (!bStart) return false;
	// �����ͱ� ������ clear �۾��� ���ش�.
	m_LeafNodeList.clear();
	m_CrashNode.clear();
	return false;
}


XMapController::XMapController()
{
}


XMapController::~XMapController()
{
}
