#include "XQuadTree.h"

bool XQuadTree::Build(float width, float height) 
{
	//RootNode 생성
	m_pRootNode = CreateNode(NULL, -width * 0.5f, width*0.5f, -height * 0.5f, height*0.5f);
	//RootNode 생성후 분할
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
	assert(pNode);// debug에서만 동작하고 조건식이 false이면 프로그램 중단

	pNode->m_Box.vMin = D3DXVECTOR3(fL, 0.0f, fB);
	pNode->m_Box.vMax = D3DXVECTOR3(fR, 0.0f, fT);
	pNode->m_Box.vCenter = (pNode->m_Box.vMin + pNode->m_Box.vMax)*0.5f;
	pNode->m_Box.fExtent[0] = pNode->m_Box.vMax.x - pNode->m_Box.vCenter.x;
	pNode->m_Box.fExtent[1] = pNode->m_Box.vMax.y - pNode->m_Box.vCenter.y;
	pNode->m_Box.fExtent[2] = pNode->m_Box.vMax.z - pNode->m_Box.vCenter.z;
	pNode->m_CornerList.reserve(4);

	//RootNode이면 Depth 추가되는 것 방어
	if (pParent)
	{
		pNode->m_dwDepth = pParent->m_dwDepth + 1;
	}
	pNode->m_CornerList[0] = D3DXVECTOR3(fL, 0.0f, fT);
	pNode->m_CornerList[1] = D3DXVECTOR3(fR, 0.0f, fT);    //XBox의 max값
	pNode->m_CornerList[2] = D3DXVECTOR3(fR, 0.0f, fB);
	pNode->m_CornerList[3] = D3DXVECTOR3(fL, 0.0f, fB);    //XBox의 min값

	return pNode;
}

bool XQuadTree::BuildTree(XNode* pNode)
{
	if (pNode != NULL)
	//분할 가능 여부 확인 후 분할
	if (SubDivide(pNode))
	{
		for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
		{
			BuildTree(pNode->m_ChildList[iNode]);//분할 후에도 자식노드를 재귀적으로 조건에 맞추어서 분할
		}
	}

	return true;
}

bool XQuadTree::SubDivide(XNode* pNode)
{
	//분할 가능 여부
	if (pNode == NULL)
	{
		return false;
	}

	//조건
	if (m_iMaxDepth <= pNode->m_dwDepth)
	{
		pNode->m_bLeaf = TRUE;		//최대 Depth만큼 분할후에 마지막 Node에 LeafNode표시
		return false;
	}

	//현재 노드 크기 계산
	float fWidth = (pNode->m_CornerList[1].x - pNode->m_CornerList[0].x) / 2;
	float fHeight = (pNode->m_CornerList[0].z - pNode->m_CornerList[3].z) / 2;

	// 지정한 분할 크기보다 자식노드가 작아지면 분할X
	if (fWidth < 1 || fHeight < 1)
	{
		pNode->m_bLeaf = TRUE;
		return false;
	}
	// 자식Node도 RootNode처럼 노드를 생성하기 위에 좌표를 넘겨줌
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
	if (!PreFrame()) return false; // 클리어 작업을 해주고 다시 그릴 노드와 오브젝트를 찾는다.
	DrawFindNode(m_pRootNode);
	return true;
}

void XQuadTree::DrawFindNode(XNode* pNode)
{
	// 전체적인 수정이 필요하다. 체크가 조금 다르게 적용됨.
	X_POSITION x_Pos = m_pCamera->CheckPoitionOBBInPlane(&pNode->m_Box); // 현재 OBB로 충돌 처리를 하지만 구로 충돌 처리를 하면 계산량이 조금 더 적기 때문에 최적화시 구로 먼저 충돌처리를 한다.

	//if (pNode->m_dwDepth > RenderDepth) return; // 랜더 뎁스를 설정하고 싶을 때
	// 샘플 코드의 Visual Node,Visual Obj 함수를 구현하지 않았는데, 탐색은 레벨오더탐색으로 한다. 상위만 먼저 검색을 해보면 되기 때문.
	// 리프노드 일 경우는 완전히 제외되지 않았다면(걸쳐 있거나 완전 포함)추가한다.
	if (pNode->m_bLeaf == TRUE && x_Pos != P_BACK)
	{
		m_DrawNodeList.push_back(pNode);
		return;
	}
	// 완전히 포함되어 있으면 자식을 탐색하지 않고 노드를 추가한다.
	if (x_Pos == P_FRONT)
	{
		m_DrawNodeList.push_back(pNode);
		return;
	}
	// 걸쳐져 있는 노드에 포함된 오브젝트 체크, 다른 구조로 짜게 되서 사실상 쓰지 않음
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
