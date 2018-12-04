#include "XMap.h"

bool XMap::Init()
{
	CreateVertexList();
	CreateIndexList();
	CalcPerVertexNormalsFastLookUp();
	m_pVertexBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_VertexList.at(0), m_VertexList.size(), sizeof(PNCT_Vertex), D3D11_BIND_VERTEX_BUFFER));
	m_pIndexBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_dwIndexList.at(0), m_dwIndexList.size(), sizeof(DWORD), D3D11_BIND_INDEX_BUFFER));
	//InitConstant();
	m_pConstantBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_ConstantDataMatrix, 1, sizeof(ConstantBuffer_Matrix), D3D11_BIND_CONSTANT_BUFFER));
	InitLight();
	m_pLightConstantBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_LightData, 1, sizeof(ConstantBuffer_Light), D3D11_BIND_CONSTANT_BUFFER));
	return true;
}

void XMap::SetScaleHeight(float fScaleHeight)
{
	m_fScaleHeight = fScaleHeight;
}

void XMap::InitLight()
{
	D3DXMatrixIdentity(&m_LightData.matInvWorld);
	m_LightData.vDiffuseMaterial = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightData.vAmbientMaterial = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	m_LightData.vSpecularMaterial = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightData.vLightDirection = D3DXVECTOR4(-100.0f, -100.0f, 0.0f, 1.0f);
	m_LightData.vDiffuseLightColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightData.vAmbientLightColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightData.vSpecularLightColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
}

bool XMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TCHAR* szTexture, TCHAR* szVSfile, char* szVSFunctionName, TCHAR* szPSfile, char* szPSFunctionName, TCHAR* szHeightTexture)
{
	if (FAILED(CreateHeightMap(pDevice, pContext, szHeightTexture)))
	{
		MessageBox(0, L"CreateHeightMap FAIL", 0, 0);
		return false;
	}
	Init();

	m_pVS.Attach(m_Object.CreateVertexShader(szVSfile, szVSFunctionName, &m_pVSBuf));
	m_pPS.Attach(m_Object.CreatePixelShader(szPSfile, szPSFunctionName, &m_pPSBuf));

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_pVertexLayout.Attach(m_Object.CreateInputlayout(I_Device.m_pD3dDevice.Get(), m_pVSBuf->GetBufferSize(), m_pVSBuf->GetBufferPointer(), layout, sizeof(layout) / sizeof(layout[0])));
	
	m_pTextureSRV = I_SRV.Find(I_SRV.Add(pDevice, szTexture));
	
	return true;
}

HRESULT	XMap::CreateHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TCHAR* HeightMap)
{
	D3DX11_IMAGE_INFO ImageInfo;

	V_RETURN(D3DX11GetImageInfoFromFile(HeightMap, NULL, &ImageInfo, &hr));

	D3DX11_IMAGE_LOAD_INFO LoadInfo;
	ZeroMemory(&LoadInfo, sizeof(LoadInfo));
	LoadInfo.Format = DXGI_FORMAT_FROM_FILE;
	LoadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	LoadInfo.pSrcInfo = &ImageInfo;
	LoadInfo.Usage = D3D11_USAGE_STAGING;

	ID3D11Resource *pTexture = NULL;
	V_RETURN(D3DX11CreateTextureFromFile(pDevice, HeightMap, &LoadInfo, NULL, &pTexture, NULL));

	ID3D11Texture2D *pTexture2D = NULL;
	V_RETURN(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D));
	SAFE_RELEASE(pTexture);

	D3D11_TEXTURE2D_DESC td;
	pTexture2D->GetDesc(&td);

	m_iRow = td.Height;
	m_iCol = td.Width;
	m_fHeightList.resize(m_iRow*m_iCol);

	if (pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		if (SUCCEEDED(pContext->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &msr)))
		{
			UCHAR* pTexel = (UCHAR*)msr.pData;
			for (int Row = 0; Row < td.Height; Row++)
			{

				int rowStart = Row * msr.RowPitch;
				for (int Col = 0; Col < td.Width; Col++)
				{
					int colStart = Col * 4;
					int red = pTexel[rowStart + colStart + 0];			//+0= red,+1= green, +2= brue, +3= alpha
					m_fHeightList[Row*m_iRow + Col] = red * m_fScaleHeight;
				}
			}
		}
		pContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
	}
	return hr;
}

bool XMap::CreateVertexList()
{
	m_VertexList.resize(m_iRow*m_iCol);

	float txU = 1.0f / (m_iCol - 1);
	float txV = 1.0f / (m_iRow - 1);
	int iHalfCol = (m_iCol - 1) * 0.5f;
	int iHalfRow = (m_iRow - 1) * 0.5f;
	int Iindex = 0;
	for (int Row = 0; Row < m_iRow; Row++)
	{
		for (int Col = 0; Col < m_iCol; Col++)
		{

			int iVerIndex = Row * m_iCol + Col;
			m_VertexList[iVerIndex].p.x = (Col - iHalfCol)*m_fDistance;
			m_VertexList[iVerIndex].p.y = m_fHeightList[(m_iCol * Row) + Col] * m_fDistance / 20;
			m_VertexList[iVerIndex].p.z = -((Row - iHalfRow)*m_fDistance);
			m_VertexList[iVerIndex].n = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_VertexList[iVerIndex].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_VertexList[iVerIndex].t = D3DXVECTOR2(txU*Col, txV*Row);
		}
	}
	return true;
}

bool XMap::CreateIndexList()
{
	m_dwIndexList.resize((m_iRow - 1)*(m_iCol - 1) * 3 * 2);

	int iCur = 0;
	for (int Row = 0; Row < m_iRow - 1; Row++)
	{
		int ColStart = m_iCol * Row;
		int NextCol = m_iCol * (Row + 1);
		for (int Col = 0; Col < m_iCol - 1; Col++)
		{

			m_dwIndexList[iCur + 0] = ColStart + Col;
			m_dwIndexList[iCur + 1] = ColStart + Col + 1;
			m_dwIndexList[iCur + 2] = NextCol + Col;

			m_dwIndexList[iCur + 3] = NextCol + Col;
			m_dwIndexList[iCur + 4] = ColStart + Col + 1;
			m_dwIndexList[iCur + 5] = NextCol + Col + 1;

			iCur += 6;
		}
	}

	return true;
}

float XMap::GetHeight(float x, float z)
{
	float fHeight = 0.0f;
	float fCellX = (float)((m_iCol - 1) / 2.0f + x);
	float fCellZ = (float)((m_iRow - 1) / 2.0f - z);

	//�Ҽ��κ� �߶�   v0,(x,z)�κ�?
	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);
	//���̸� ����� ���� �ʱ�ȭ
	if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
	if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
	if ((float)(m_iCol - 2) < fVertexCol)	fVertexCol = (float)(m_iCol - 2);
	if ((float)(m_iRow - 2) < fVertexRow)	fVertexRow = (float)(m_iRow - 2);
	//���ϴ� y�� 4���� ����
	float v0 = GetHeightmap((int)fVertexRow, (int)fVertexCol);
	float v1 = GetHeightmap((int)fVertexRow, (int)fVertexCol + 1);
	float v2 = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol);
	float v3 = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol + 1);
	// v0 ������ ��ġ���� ������ ���� ���
	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;
	// �����̽��� �������� ����
	if (fDeltaZ < (1.0f - fDeltaX))  //���ﰢ��
	{
		float uy = v1 - v0; // v0->v1
		float vy = v2 - v0; // v0->v2	
							// �� ������ ���̰��� ���̸� ���Ͽ� ��ŸX�� ���� ���� �������� ã�´�.		
		fHeight = v0 + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	// �Ʒ� ���̽� �������� ����
	else // �Ʒ� �ﰢ��
	{
		float uy = v2 - v3; // v3->v2
		float vy = v1 - v3; // v3->v1
							// �� ������ ���̰��� ���̸� ���Ͽ� ��ŸZ�� ���� ���� �������� ã�´�.		
		fHeight = v3 + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
	}
	return fHeight;
}

float XMap::Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - (fStart*fTangent) + (fEnd*fTangent);
}

float XMap::GetHeightmap(int row, int col)
{
	return m_VertexList[row * m_iRow + col].p.y;
}

//void XMap::InitConstant()
//{
//	m_ConstantDataMatrix.vLight = D3DXVECTOR4(-50.0f, -50.0f, 0.0f, 1.0f);
//}

bool XMap::CalcPerVertexNormalsFastLookUp()
{
	InitFaceNormals();		// Face�� �븻�� ����Ʈ�� ����
	GetNormalLookUpTable(); // ���� �븻�� �����ϱ� ���ؼ� ������ �����ϴ� ���̽��� ã�´�.
	// �����ϴ� Face�� ������ �ջ��ؼ� ����ȭ�Ͽ� �븻���� ����Ʈ�� ����
	for (int iVertex = 0; iVertex < (m_iRow*m_iCol); iVertex++)
	{
		D3DXVECTOR3 avgNormal;
		avgNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int iNegigbor = 0; iNegigbor < 6; iNegigbor++)
		{
			int iIndex;
			iIndex = m_iNormalLookUpTable[iVertex * 6 + iNegigbor];

			if (iIndex != -1)
			{
				avgNormal += m_vFaceNormals[iIndex];
			}
			else
				break;
		}
		D3DXVec3Normalize(&avgNormal, &avgNormal);
		m_VertexList[iVertex].n = avgNormal;
	}
	return 0;
}


bool XMap::InitFaceNormals()
{
	m_iFace = (m_iRow - 1)*(m_iCol - 1) * 2;
	m_vFaceNormals.resize(m_iFace);

	for (int iNum = 0; iNum < m_iFace; iNum++)
	{
		int n0 = m_dwIndexList[iNum * 3 + 0];
		int n1 = m_dwIndexList[iNum * 3 + 1];
		int n2 = m_dwIndexList[iNum * 3 + 2];

		m_vFaceNormals[iNum] = ComputeFaceNormal(n0, n1, n2);
	}

	return true;
}

D3DXVECTOR3 XMap::ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2)
{
	D3DXVECTOR3 vNormal;
	D3DXVECTOR3 v0 = m_VertexList[dwIndex1].p - m_VertexList[dwIndex0].p;
	D3DXVECTOR3 v1 = m_VertexList[dwIndex2].p - m_VertexList[dwIndex0].p;

	D3DXVec3Cross(&vNormal, &v0, &v1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	return vNormal;
}

//void TMap::CalcPerVertexNormalsFastLookup() sample
//{
//	CreateFaceNormals();
//
//	for (int iVertex = 0; iVertex < (m_iNumCols * m_iNumRows); iVertex++)
//	{
//		float divisor = 1.0f;
//		D3DXVECTOR3 avgNormal;
//		avgNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//		for (int iNegigbor = 0; iNegigbor<6; iNegigbor++)
//		{
//			int triIndex;
//			triIndex = m_pNormalLookupTable[iVertex * 6 + iNegigbor];
//
//			if (triIndex != -1)
//			{
//				avgNormal += m_pFaceNormals[triIndex];
//				divisor += 1.0f;
//			}
//			else
//				break;
//		}

//		avgNormal.x /= divisor;
//		avgNormal.y /= divisor;
//		avgNormal.z /= divisor;
//		D3DXVec3Normalize(&avgNormal, &avgNormal);
//
//		m_pvHeightMap[iVertex].n = avgNormal;
//	}
//}

bool XMap::GetNormalLookUpTable()
{
	int bfsize = m_iRow * m_iCol * 6;
	m_iNormalLookUpTable.resize(bfsize);

	for (int i = 0; i < bfsize; i++)
	{
		m_iNormalLookUpTable[i] = -1;
	}

	for (int i = 0; i < m_iFace; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 6; k++) // DWORD�� XVertexLookUp�� �ٲ۴ٸ� �ణ ���� �ʿ�

			{
				int vertex = m_dwIndexList[i * 3 + j];
				if (m_iNormalLookUpTable[vertex * 6 + k] == -1)
				{
					m_iNormalLookUpTable[vertex * 6 + k] = i;
					break;
				}
			}
		}
	}

	return true;
}

void XMap::SetLookVector(D3DXVECTOR3 vLook)
{
	m_vLook = vLook;
}

void XMap::UpdateLight()
{
	D3DXMATRIX matInvWorld;
	D3DXMatrixInverse(&matInvWorld, NULL, &m_matWorld);
	D3DXMatrixTranspose(&matInvWorld, &matInvWorld);
	D3DXMatrixTranspose(&m_LightData.matInvWorld, &matInvWorld);

	m_LightData.vSightDirection.x = m_vLook.x;
	m_LightData.vSightDirection.y = m_vLook.y;
	m_LightData.vSightDirection.z = m_vLook.z;
	m_LightData.vSightDirection.w = 10.0f; // ����

	m_LightData.vSpecularMaterial = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
}

bool XMap::Frame()
{
	return true;
}

bool XMap::Render(ID3D11DeviceContext* pContext)
{
	UpdateLight();	// SetMatrix�� ���� �Ǿ��Ѵ�.
	pContext->UpdateSubresource(m_pLightConstantBuffer.Get(), 0, NULL, &m_LightData, NULL, NULL);
	pContext->VSSetShader(m_pVS.Get(), NULL, 0);
	pContext->PSSetShader(m_pPS.Get(), NULL, 0);
	pContext->IASetInputLayout(m_pVertexLayout.Get());

	UINT stride = sizeof(PNCT_Vertex);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
	pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, NULL, &m_ConstantDataMatrix, NULL, NULL);
	pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pContext->VSSetConstantBuffers(1, 1, m_pLightConstantBuffer.GetAddressOf());
	pContext->PSSetConstantBuffers(1, 1, m_pLightConstantBuffer.GetAddressOf());
	pContext->PSSetShaderResources(0, 1, m_pTextureSRV.GetAddressOf());
	pContext->DrawIndexed(m_dwIndexList.size(), 0, 0);

	return true;
}

bool XMap::Release()
{
	return true;
}

XMap::XMap()
{
	m_fScaleHeight = 1.0f;
	m_fDistance = 1.0f;
}

XMap::~XMap()
{
}