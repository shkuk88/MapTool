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
	// DrawNodeList는 찾는 범위 내 노드를 검색 시 컬링에 의해 짤리는 Node가 발생할 소지가 있어서 모든 노드를 검색. 
	// 최적화 필요시 DrawNodeList로 대체
	for (auto LeafNode : m_LeafNodeList)
	{

	}
}

bool XMapController::Release()
{
	if (!bStart) return false;
	// 포인터기 때문에 clear 작업만 해준다.
	m_LeafNodeList.clear();
	return false;
}


XMapController::XMapController()
{
}


XMapController::~XMapController()
{
}
