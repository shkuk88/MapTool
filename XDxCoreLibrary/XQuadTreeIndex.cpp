#include "XQuadTreeIndex.h"


bool XQuadTreeIndex::Build(XMap* pMap)
{
	m_Map = pMap;
	m_dwWidth = m_Map->m_iCol;
	m_dwHeight = m_Map->m_iRow;

	m_iNumFace = (m_dwWidth - 1)*(m_dwHeight - 1) * 2;
	//RootNode ����
	m_pRootNode = CreateNode(NULL, 0, m_dwWidth - 1, m_dwWidth*(m_dwHeight - 1), m_dwWidth*m_dwHeight - 1);
	//RootNode ������ ����
	if (BuildTree(m_pRootNode))
	{
		if (FindLeafNode(m_pRootNode))  // LOD������ �ε��� ���۸� �������� ���� ���� �����ϱ� ������ ������忡 ���� �ε����� �ο��� �ʿ䰡 ��������.
		{
			return true;
		}
		return true;
	}
	return false;
}

XNode* XQuadTreeIndex::CreateNode(XNode* pParent, DWORD TL, DWORD TR, DWORD BL, DWORD BR)
{
	XNode* pNode = NULL;
	SAFE_NEW(pNode, XNode);

	pNode->m_bLeaf = FALSE;

	pNode->m_ChildList.reserve(4);
	pNode->m_CornerIndex.reserve(4);
	//pNode->m_CornerList.reserve(4);

	
	pNode->m_Box.vMin = m_Map->m_VertexList[BL].p;
	pNode->m_Box.vMax = m_Map->m_VertexList[TR].p;
	pNode->m_Box.vCenter = (pNode->m_Box.vMin + pNode->m_Box.vMax)*0.5f;
	pNode->m_Box.fExtent[0] = pNode->m_Box.vMax.x - pNode->m_Box.vCenter.x;;
	pNode->m_Box.fExtent[1] = pNode->m_Box.vMax.y - pNode->m_Box.vCenter.y;;
	pNode->m_Box.fExtent[2] = pNode->m_Box.vMax.z - pNode->m_Box.vCenter.z;;

	//RootNode�̸� Depth �߰��Ǵ� �� ���
	if (pParent)
	{
		pNode->m_dwDepth = pParent->m_dwDepth + 1;
	}
	pNode->m_CornerIndex.push_back(TL);
	pNode->m_CornerIndex.push_back(TR);   //XBox�� max��
	pNode->m_CornerIndex.push_back(BL);
	pNode->m_CornerIndex.push_back(BR);   //XBox�� min��
	return pNode;
}


bool XQuadTreeIndex::SubDivide(XNode* pNode)
{
	//���� ���� ����
	if (pNode == NULL)	return false;

	DWORD dwOffsetX = (pNode->m_CornerIndex[TR] - pNode->m_CornerIndex[TL]);
	DWORD dwOffsetZ = ((pNode->m_CornerIndex[BL] - pNode->m_CornerIndex[TL]) / m_dwWidth);

	DWORD dwHalfWidth = dwOffsetX / 2;
	DWORD dwHalfHeight = dwOffsetZ / 2;

	//����
	if (m_iMaxDepth <= pNode->m_dwDepth)	// �ִ� ���� �ѵ� ����
	{
		pNode->m_bLeaf = TRUE;		// �ִ� Depth��ŭ �����Ŀ� ������ Node�� LeafNodeǥ��
		return false;
	}

	if (dwHalfWidth < 1 || dwHalfHeight < 1)	// ���� �ּ� ������, ���� �޾ƿ��� �ʰ� �׳� 1�� ����
	{
		pNode->m_bLeaf = TRUE;
		return false;
	}

	// ���� = �����ڳ� + ���ø�ũ��
	// ���ϱ� = ���� + ( ���ø� ũ�� * ��ü�� ����ũ�� )
	DWORD dwCenter = (pNode->m_CornerIndex[TL] + dwHalfWidth) + (dwHalfHeight*m_dwWidth);
	DWORD dwEdgeCenter[4];
	dwEdgeCenter[0] = pNode->m_CornerIndex[TL] + dwHalfWidth;				//��
	dwEdgeCenter[1] = pNode->m_CornerIndex[TR] + dwHalfHeight * m_dwWidth;	//��
	dwEdgeCenter[2] = pNode->m_CornerIndex[BL] + dwHalfWidth;				//��
	dwEdgeCenter[3] = pNode->m_CornerIndex[TL] + dwHalfHeight * m_dwWidth;	//��
	// �ڽĳ�� ����
	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerIndex[TL], dwEdgeCenter[0], dwEdgeCenter[3], dwCenter));
	pNode->m_ChildList.push_back(CreateNode(pNode, dwEdgeCenter[0], pNode->m_CornerIndex[TR], dwCenter, dwEdgeCenter[1]));
	pNode->m_ChildList.push_back(CreateNode(pNode, dwEdgeCenter[3], dwCenter, pNode->m_CornerIndex[BL], dwEdgeCenter[2]));
	pNode->m_ChildList.push_back(CreateNode(pNode, dwCenter, dwEdgeCenter[1], dwEdgeCenter[2], pNode->m_CornerIndex[BR]));

	return true;
}

bool XQuadTreeIndex::FindLeafNode(XNode* pNode) 
{
	if (pNode == NULL)	return false;
	if (pNode->m_bLeaf == TRUE)
	{
		CreateLeafNodeIndex(pNode);
	}
	else
	{
		FindLeafNode(pNode->m_ChildList[TL]);
		FindLeafNode(pNode->m_ChildList[TR]);
		FindLeafNode(pNode->m_ChildList[BL]);
		FindLeafNode(pNode->m_ChildList[BR]);
	}
	return true;
}

HRESULT XQuadTreeIndex::CreateLeafNodeIndex(XNode* pNode)
{
	int LT = pNode->m_CornerIndex[TL];
	int RT = pNode->m_CornerIndex[TR];
	int LB = pNode->m_CornerIndex[BL];
	int RB = pNode->m_CornerIndex[BR];

	int iMapWidth = m_Map->m_iCol;

	int iWidthCnt = RT - LT;
	int iHeightCnt = (LB - LT) / iMapWidth;
	int iTotalSize = iWidthCnt * iHeightCnt;

	int iStartRow = LT / iMapWidth;
	int iEndRow = LB / iMapWidth;
	int iStartCol = LT % iMapWidth;
	int iEndCol = RT % iMapWidth;

	pNode->m_IndexList.resize(iTotalSize * 2 * 3);

	int iCnt = 0;
	for (int iRow = iStartRow; iRow < iEndRow; iRow++)
	{
		for (int iCol = iStartCol; iCol < iEndCol; iCol++)
		{
			int iRowNext = iRow + 1;
			int iColNext = iCol + 1;

			pNode->m_IndexList[iCnt++] = (iRow* iMapWidth) + iCol;
			pNode->m_IndexList[iCnt++] = (iRow* iMapWidth) + iColNext;
			pNode->m_IndexList[iCnt++] = (iRowNext* iMapWidth) + iCol;

			pNode->m_IndexList[iCnt++] = (iRowNext* iMapWidth) + iCol;
			pNode->m_IndexList[iCnt++] = (iRow* iMapWidth) + iColNext;
			pNode->m_IndexList[iCnt++] = (iRowNext* iMapWidth) + iColNext;

		}
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(DWORD) * pNode->m_IndexList.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA bData;
	ZeroMemory(&bData, sizeof(bData));
	bData.pSysMem = &pNode->m_IndexList.at(0);
	V_RETURN(I_Device.m_pD3dDevice->CreateBuffer(&bd, &bData, pNode->m_IndexBuffer.GetAddressOf()));
};


bool XQuadTreeIndex::Frame()
{
	XQuadTree::Frame();
	return true;
}

bool XQuadTreeIndex::LeafRender(ID3D11DeviceContext* pContext, XNode* pNode)
{
	if (pNode == NULL)
	{
		return true;
	}

	if (pNode->m_bLeaf == TRUE)
	{
		pContext->IASetIndexBuffer(pNode->m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		pContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);
	}
	else
	{
		LeafRender(pContext, pNode->m_ChildList[TL]);
		LeafRender(pContext, pNode->m_ChildList[TR]);
		LeafRender(pContext, pNode->m_ChildList[BL]);
		LeafRender(pContext, pNode->m_ChildList[BR]);
	}

	return false;
}

bool XQuadTreeIndex::Render(ID3D11DeviceContext* pContext)
{
	m_Map->UpdateLight();
	if (m_Map->m_pTextureSRV)
	{
		pContext->VSSetShader(m_Map->m_pVS[Tex_Have].Get(), NULL, 0);
		pContext->PSSetShader(m_Map->m_pPS[Tex_Have].Get(), NULL, 0);
	}
	else
	{
		pContext->VSSetShader(m_Map->m_pVS[Tex_None].Get(), NULL, 0);
		pContext->PSSetShader(m_Map->m_pPS[Tex_None].Get(), NULL, 0);
	}
	pContext->IASetInputLayout(m_Map->m_pVertexLayout.Get());

	UINT stride = sizeof(PNCT_Vertex);
	UINT offset = 0;
	
	// ���������ÿ��� ������Ʈ, LOD�� ����
	I_Device.m_pD3dContext->UpdateSubresource(m_Map->m_pVertexBuffer.Get(), 0, NULL, &m_Map->m_VertexList.at(0), NULL, NULL);
	
	pContext->IASetVertexBuffers(0, 1, m_Map->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->UpdateSubresource(m_Map->m_pLightConstantBuffer.Get(), 0, NULL, &m_Map->m_LightData, NULL, NULL);
	pContext->UpdateSubresource(m_Map->m_pConstantBuffer.Get(), 0, NULL, &m_Map->m_ConstantDataMatrix, NULL, NULL);
	pContext->VSSetConstantBuffers(0, 1, m_Map->m_pConstantBuffer.GetAddressOf());
	pContext->VSSetConstantBuffers(1, 1, m_Map->m_pLightConstantBuffer.GetAddressOf());
	pContext->PSSetConstantBuffers(1, 1, m_Map->m_pLightConstantBuffer.GetAddressOf());
	if(m_Map->m_pTextureSRV)
	{
		pContext->PSSetShaderResources(0, 1, m_Map->m_pTextureSRV.GetAddressOf());
	}
	for (int i = 0; i < m_DrawNodeList.size(); i++)
	{
		if (LeafRender(pContext, m_DrawNodeList[i]))
		{
			return true;
		}
	}

	return false;
}

bool XQuadTreeIndex::Release()
{
	return XQuadTree::Release();
}

XQuadTreeIndex::XQuadTreeIndex()
{
	m_pRootNode = NULL;
}

XQuadTreeIndex::~XQuadTreeIndex()
{
}