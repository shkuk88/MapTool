#include "XMap.h"

bool XMap::Init()
{
	CreateVertexList();
	CreateIndexList();
	CalcPerVertexNormalsFastLookUp();
	InitLight();
	return true;
}

void XMap::SetScaleHeight(float fScaleHeight)
{
	m_fScaleHeight = fScaleHeight;
}

void XMap::SetAlphaSRV(ID3D11ShaderResourceView * pSRV, int iColor)
{
	m_AlphaSRV[iColor].Detach();
	m_AlphaSRV[iColor].Attach(pSRV);
}

void XMap::CreateBuffer()
{
	m_pVertexBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_VertexList.at(0), m_VertexList.size(), sizeof(PNCT_Vertex), D3D11_BIND_VERTEX_BUFFER));
	m_pIndexBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_dwIndexList.at(0), m_dwIndexList.size(), sizeof(DWORD), D3D11_BIND_INDEX_BUFFER));
	m_pConstantBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_ConstantDataMatrix, 1, sizeof(ConstantBuffer_Matrix), D3D11_BIND_CONSTANT_BUFFER));
	m_pLightConstantBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_LightData, 1, sizeof(ConstantBuffer_Light), D3D11_BIND_CONSTANT_BUFFER));
}

void XMap::CreateShader(TCHAR* szMapShader, TCHAR* szOnlyColorShader, char* szVSFunctionName, char* szPSFunctionName)
{
	m_pVS[Tex_Have].Attach(m_Object.CreateVertexShader(szMapShader, szVSFunctionName, &m_pVSBuf));
	m_pPS[Tex_Have].Attach(m_Object.CreatePixelShader(szMapShader, szPSFunctionName, &m_pPSBuf));
	m_pVS[Tex_None].Attach(m_Object.CreateVertexShader(szOnlyColorShader, szVSFunctionName, &m_pVSBuf));
	m_pPS[Tex_None].Attach(m_Object.CreatePixelShader(szOnlyColorShader, szPSFunctionName, &m_pPSBuf));
}

void XMap::InitLight()
{
	D3DXMatrixIdentity(&m_LightData.matInvWorld);
	m_LightData.vDiffuseMaterial = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightData.vAmbientMaterial = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	m_LightData.vSpecularMaterial = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightData.vLightDirection = D3DXVECTOR4(100.0f, 100.0f, 0.0f, 1.0f);
	m_LightData.vDiffuseLightColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightData.vAmbientLightColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightData.vSpecularLightColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
}

bool XMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, float fDistance,TCHAR* szTexture, TCHAR* szHeightTexture, TCHAR* szMapShader, TCHAR* szOnlyColorShader, char* szVSFunctionName, char* szPSFunctionName)
{
	if (szHeightTexture)
	{
		if (FAILED(CreateHeightMap(pDevice, pContext, szHeightTexture)))
		{
			MessageBox(0, L"CreateHeightMap FAIL", 0, 0);
			return false;
		}
		m_fDistance = fDistance;
	}

	Init();
	CreateBuffer();
	CreateShader(szMapShader, szOnlyColorShader, szVSFunctionName, szPSFunctionName);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_pVertexLayout.Attach(m_Object.CreateInputlayout(I_Device.m_pD3dDevice.Get(), m_pVSBuf->GetBufferSize(), m_pVSBuf->GetBufferPointer(), layout, sizeof(layout) / sizeof(layout[0])));
	if (szTexture)
	{
		m_pTextureSRV = I_SRV.Find(I_SRV.Add(pDevice, szTexture));
	}

	return true;
}

bool XMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, float fCellCount, float fDistance, TCHAR* szTexture, TCHAR* szMapShader, TCHAR* szOnlyColorShader, char* szVSFunctionName, char* szPSFunctionName)
{
	m_iCol = fCellCount+1;
	m_iRow = fCellCount+1;
	m_fDistance = fDistance;

	// 정점 갯수 제한
	if (m_iCol > 1025 || m_iRow > 1025)
	{
		m_iCol = 1025; 
		m_iRow = 1025; 
	}
	Init();
	CreateBuffer();
	CreateShader(szMapShader, szOnlyColorShader, szVSFunctionName, szPSFunctionName);
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_pVertexLayout.Attach(m_Object.CreateInputlayout(I_Device.m_pD3dDevice.Get(), m_pVSBuf->GetBufferSize(), m_pVSBuf->GetBufferPointer(), layout, sizeof(layout) / sizeof(layout[0])));
	if (szTexture)
	{
		m_pTextureSRV = I_SRV.Find(I_SRV.Add(pDevice, szTexture));
	}
	else
	{
		m_pTextureSRV.Attach(nullptr);
	}
	return true;
}

bool XMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, XMapImporter* pImporter, TCHAR* szMapShader, TCHAR* szOnlyColorShader, char* szVSFunctionName, char* szPSFunctionName)
{
	ImportData(pDevice, pImporter);
	CalcPerVertexNormalsFastLookUp();
	InitLight();
	CreateBuffer();
	CreateShader(szMapShader, szOnlyColorShader, szVSFunctionName, szPSFunctionName);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_pVertexLayout.Attach(m_Object.CreateInputlayout(I_Device.m_pD3dDevice.Get(), m_pVSBuf->GetBufferSize(), m_pVSBuf->GetBufferPointer(), layout, sizeof(layout) / sizeof(layout[0])));

	return true;
}

bool XMap::ImportData(ID3D11Device* pDevice,XMapImporter* pImporter)
{
	if (!pDevice || !pImporter) return false;

	// Standard
	m_iCol = pImporter->GetCellCnt();
	m_iRow = pImporter->GetCellCnt();
	m_fDistance = pImporter->GetDistance();
	// SpreatTexture
	m_pTextureSRV = I_SRV.Find(I_SRV.Add(pDevice, pImporter->GetSpreatTex()->c_str()));
	// AlphaTexture
	map<int, TString>* pAlphaTex = pImporter->GetAlphaTex();
	for (int iAlphaColor = 0; iAlphaColor < 4; iAlphaColor++)
	{
		ID3D11ShaderResourceView* pSRV = I_SRV.Find(I_SRV.Add(pDevice, (*pAlphaTex)[iAlphaColor].c_str())).Get();
		SetAlphaSRV(pSRV, iAlphaColor);
	}
	// VertexList
	int iSize = pImporter->GetVertexList()->size();
	vector<PNCT_Vertex>* VertexList = pImporter->GetVertexList();
	m_VertexList.resize(iSize);
	for (int iLoop = 0; iLoop < iSize; iLoop++)
	{
		m_VertexList[iLoop].p = (*VertexList)[iLoop].p;
		m_VertexList[iLoop].n = (*VertexList)[iLoop].n;
		m_VertexList[iLoop].c = (*VertexList)[iLoop].c;
		m_VertexList[iLoop].t = (*VertexList)[iLoop].t;
	}
	// IndexList
	iSize = pImporter->GetIndexList()->size();
	vector<DWORD>* IndexList = pImporter->GetIndexList();
	m_dwIndexList.resize(iSize);
	for (int iLoop = 0; iLoop < iSize; iLoop++)
	{
		m_dwIndexList[iLoop] = (*IndexList)[iLoop];

	}

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

	m_iRow = td.Height + 1;
	m_iCol = td.Width + 1;
	m_fHeightList.resize(m_iRow*m_iCol);

	if (pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		if (SUCCEEDED(pContext->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &msr)))
		{
			UCHAR* pTexel = (UCHAR*)msr.pData;
			for (int Row = 0; Row < m_iRow; Row++)
			{

				int rowStart = Row * msr.RowPitch;
				for (int Col = 0; Col < m_iCol; Col++)
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
			if(m_fHeightList.size())
				m_VertexList[iVerIndex].p.y = m_fHeightList[(m_iCol * Row) + Col] * m_fDistance / 20;
			else
				m_VertexList[iVerIndex].p.y = 0.0f;
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
	float fCellX = (float)((m_iCol - 1) / 2.0f + x/m_fDistance);
	float fCellZ = (float)((m_iRow - 1) / 2.0f - z/m_fDistance);

	//소수부분 잘라냄   v0,(x,z)부분?
	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);
	//높이맵 벗어나면 강제 초기화
	if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
	if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
	if ((float)(m_iCol - 2) < fVertexCol)	fVertexCol = (float)(m_iCol - 2);
	if ((float)(m_iRow - 2) < fVertexRow)	fVertexRow = (float)(m_iRow - 2);
	//원하는 y의 4개의 정점
	float v0 = GetHeightmap((int)fVertexRow, (int)fVertexCol);
	float v1 = GetHeightmap((int)fVertexRow, (int)fVertexCol + 1);
	float v2 = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol);
	float v3 = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol + 1);
	// v0 정점의 위치에서 떨어진 값을 계산
	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;
	// 윗페이스를 기준으로 보간
	if (fDeltaZ < (1.0f - fDeltaX))  //윗삼각형
	{
		float uy = v1 - v0; // v0->v1
		float vy = v2 - v0; // v0->v2	
							// 두 정점의 높이값의 차이를 비교하여 델타X의 값에 따라 보간값을 찾는다.		
		fHeight = v0 + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	// 아래 페이스 기준으로 보간
	else // 아랫 삼각형
	{
		float uy = v2 - v3; // v3->v2
		float vy = v1 - v3; // v3->v1
							// 두 정점의 높이값의 차이를 비교하여 델타Z의 값에 따라 보간값을 찾는다.		
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

bool XMap::CalcPerVertexNormalsFastLookUp()
{
	InitFaceNormals();		// Face당 노말을 리스트로 저장
	GetNormalLookUpTable(); // 정점 노말을 생성하기 위해서 정점당 공유하는 페이스를 찾는다.
	// 공유하는 Face의 방향을 합산해서 정규화하여 노말값을 리스트로 저장
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
			for (int k = 0; k < 6; k++) // DWORD를 XVertexLookUp로 바꾼다면 약간 수정 필요

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
	m_LightData.vSightDirection.w = 10.0f; // 강도
	m_LightData.vSpecularMaterial = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); // light 색상

	//m_LightData.vLightDirection.x = sinf(g_fTime)*0.01f;
	//m_LightData.vLightDirection.y = cosf(g_fTime)*0.01f;

}

bool XMap::Frame()
{
	return true;
}

bool XMap::PreRender(ID3D11DeviceContext* pContext)
{
	UpdateLight();	// SetMatrix가 먼저 되야한다.

	pContext->UpdateSubresource(m_pVertexBuffer.Get(), 0, NULL, &m_VertexList.at(0), NULL, NULL);
	pContext->UpdateSubresource(m_pLightConstantBuffer.Get(), 0, NULL, &m_LightData, NULL, NULL);
	if (m_pTextureSRV)
	{
		pContext->VSSetShader(m_pVS[Tex_Have].Get(), NULL, 0);
		pContext->PSSetShader(m_pPS[Tex_Have].Get(), NULL, 0);
	}
	else
	{
		pContext->VSSetShader(m_pVS[Tex_None].Get(), NULL, 0);
		pContext->PSSetShader(m_pPS[Tex_None].Get(), NULL, 0);
	}
	pContext->IASetInputLayout(m_pVertexLayout.Get());

	UINT stride = sizeof(PNCT_Vertex);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
	pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, NULL, &m_ConstantDataMatrix, NULL, NULL);
	pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pContext->VSSetConstantBuffers(1, 1, m_pLightConstantBuffer.GetAddressOf());
	pContext->PSSetConstantBuffers(1, 1, m_pLightConstantBuffer.GetAddressOf());
	if (m_pTextureSRV)
	{
		pContext->PSSetShaderResources(0, 1, m_pTextureSRV.GetAddressOf());
	}
	return true;
}

bool XMap::PostRender(ID3D11DeviceContext* pContext)
{
	
	pContext->DrawIndexed(m_dwIndexList.size(), 0, 0);
	return true;
}

bool XMap::Render(ID3D11DeviceContext* pContext)
{
	UpdateLight();	// SetMatrix가 먼저 되야한다.

	pContext->UpdateSubresource(m_pVertexBuffer.Get(), 0, NULL, &m_VertexList.at(0), NULL, NULL);
	pContext->UpdateSubresource(m_pLightConstantBuffer.Get(), 0, NULL, &m_LightData, NULL, NULL);
	if (m_pTextureSRV)
	{
		pContext->VSSetShader(m_pVS[Tex_Have].Get(), NULL, 0);
		pContext->PSSetShader(m_pPS[Tex_Have].Get(), NULL, 0);
	}
	else
	{
		pContext->VSSetShader(m_pVS[Tex_None].Get(), NULL, 0);
		pContext->PSSetShader(m_pPS[Tex_None].Get(), NULL, 0);
	}
	pContext->IASetInputLayout(m_pVertexLayout.Get());

	UINT stride = sizeof(PNCT_Vertex);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
	pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, NULL, &m_ConstantDataMatrix, NULL, NULL);
	pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pContext->VSSetConstantBuffers(1, 1, m_pLightConstantBuffer.GetAddressOf());
	pContext->PSSetConstantBuffers(1, 1, m_pLightConstantBuffer.GetAddressOf());
	if (m_pTextureSRV)
	{
		pContext->PSSetShaderResources(0, 1, m_pTextureSRV.GetAddressOf());
	}
	if (m_pTextureSRV && m_AlphaSRV.size())
	{
		pContext->PSSetShader(m_pPS[Tex_Have].Get(), NULL, 0); 
		for (int iAlphaTex = 0; iAlphaTex < m_AlphaSRV.size(); iAlphaTex++)
		{
			pContext->PSSetShaderResources(2 + iAlphaTex, 1, m_AlphaSRV[iAlphaTex].GetAddressOf());
		}
	}
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
