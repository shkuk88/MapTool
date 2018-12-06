#include "XMapController.h"



bool XMapController::SetLeafNode(XNode * pNode)
{
	
	if (!pNode)	return false;

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

void XMapController::CheckCrashNode()
{
	for (auto LeafNode : m_LeafNodeList)
	{

	}
}

bool XMapController::Release()
{
	if (!bStart) return false;
	// �����ͱ� ������ clear �۾��� ���ش�.
	m_LeafNodeList.clear();
	return false;
}


XMapController::XMapController()
{
}


XMapController::~XMapController()
{
}
