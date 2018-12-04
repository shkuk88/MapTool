#include "XShape.h"


void XShape::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_iTopology = topology;
}

void XShape::SetMatrix(D3DXMATRIX *matWorld, D3DXMATRIX *matView, D3DXMATRIX *matProj)
{
	if (matWorld != NULL)
	{
		m_matWorld = *matWorld;
	}
	if (matView != NULL)
	{
		m_matView = *matView;
	}
	if (matProj != NULL)
	{
		m_matProj = *matProj;
	}

	D3DXMatrixTranspose(&m_ConstantDataMatrix.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_ConstantDataMatrix.matView, &m_matView);
	D3DXMatrixTranspose(&m_ConstantDataMatrix.matProj, &m_matProj);
}

XShape::XShape()
{
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; 

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

XShape::~XShape()
{
}

void XLineShape::SetVertex()
{
	m_VertexList_Line.resize(2);
	//Á¤¸é
	m_VertexList_Line[0].p = { 0.0f, 0.0f, 0.0f };	m_VertexList_Line[0].c = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_VertexList_Line[1].p = { 1.0f, 0.0f, 0.0f };	m_VertexList_Line[1].c = { 1.0f, 1.0f, 1.0f, 1.0f };
}

bool XLineShape::Init()
{
	SetVertex();
	m_pVertexBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_VertexList_Line.at(0), m_VertexList_Line.size(), sizeof(PC_Vertex), D3D11_BIND_VERTEX_BUFFER));
	m_pConstantBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_ConstantDataMatrix, 1, sizeof(ConstantBuffer_Matrix), D3D11_BIND_CONSTANT_BUFFER));

	return true;
}

bool XLineShape::Create(TCHAR* szVertexShaderFile, TCHAR* szPixelShaderFile, char* szVSFunctionName, char* szPSFunctionName)
{
	Init();
	m_pVS.Attach(m_Object.CreateVertexShader(szVertexShaderFile, szVSFunctionName, &m_pVSBuf));
	m_pPS.Attach(m_Object.CreatePixelShader(szPixelShaderFile, szPSFunctionName, &m_pPSBuf));

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_pVertexLayout.Attach(m_Object.CreateInputlayout(I_Device.m_pD3dDevice.Get(), m_pVSBuf->GetBufferSize(), m_pVSBuf->GetBufferPointer(), layout, sizeof(layout) / sizeof(layout[0])));
	return true;
}

bool XLineShape::Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor)
{
	m_VertexList_Line[0].p = vStart;
	m_VertexList_Line[0].c = vColor;
	m_VertexList_Line[1].p = vEnd;
	m_VertexList_Line[1].c = vColor;

	pContext->UpdateSubresource(m_pVertexBuffer.Get(), 0, NULL, &m_VertexList_Line.at(0), 0, 0);
	return Render(pContext);
}

bool XLineShape::Frame()
{
	return true;
}

bool XLineShape::Render(ID3D11DeviceContext* pContext)
{
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	pContext->VSSetShader(m_pVS.Get(), NULL, 0);
	pContext->PSSetShader(m_pPS.Get(), NULL, 0);
	pContext->IASetInputLayout(m_pVertexLayout.Get());
	UINT stride = sizeof(PC_Vertex);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, NULL, &m_ConstantDataMatrix, NULL, NULL);
	pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pContext->Draw(2, 0);
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

bool XLineShape::Release()
{
	return true;
}

XLineShape::XLineShape()
{
}

XLineShape::~XLineShape()
{
}

void XBoxShape::SetVertex()
{
	m_VertexList.resize(24);
	//Á¤¸é
	m_VertexList[0].p = { -1.0f, 1.0f, -1.0f };			m_VertexList[0].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[0].c = { 1.0f, 0.0f, 0.0f, 1.0f };			m_VertexList[0].t = { 0.0f, 0.0f };
	m_VertexList[1].p = { 1.0f, 1.0f, -1.0f };			m_VertexList[1].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[1].c = { 1.0f, 0.0f, 0.0f, 1.0f };			m_VertexList[1].t = { 1.0f, 0.0f };
	m_VertexList[2].p = { -1.0f, -1.0f, -1.0f };		m_VertexList[2].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[2].c = { 1.0f, 0.0f, 0.0f, 1.0f };			m_VertexList[2].t = { 0.0f, 1.0f };
	m_VertexList[3].p = { 1.0f, -1.0f, -1.0f };			m_VertexList[3].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[3].c = { 1.0f, 0.0f, 0.0f, 1.0f };			m_VertexList[3].t = { 1.0f, 1.0f };
	//ÁÂÃø
	m_VertexList[4].p = { -1.0f, 1.0f, 1.0f };			m_VertexList[4].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[4].c = { 1.0f, 1.0f, 0.0f, 1.0f };			m_VertexList[4].t = { 0.0f, 0.0f };
	m_VertexList[5].p = { -1.0f, 1.0f, -1.0f };			m_VertexList[5].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[5].c = { 1.0f, 1.0f, 0.0f, 1.0f };			m_VertexList[5].t = { 1.0f, 0.0f };
	m_VertexList[6].p = { -1.0f, -1.0f, 1.0f };			m_VertexList[6].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[6].c = { 1.0f, 1.0f, 0.0f, 1.0f };			m_VertexList[6].t = { 0.0f, 1.0f };
	m_VertexList[7].p = { -1.0f, -1.0f, -1.0f };		m_VertexList[7].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[7].c = { 1.0f, 1.0f, 0.0f, 1.0f };			m_VertexList[7].t = { 1.0f, 1.0f };
	//¿ìÃø
	m_VertexList[8].p = { 1.0f, 1.0f, -1.0f };			m_VertexList[8].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[8].c = { 0.0f, 1.0f, 1.0f, 1.0f };			m_VertexList[8].t = { 0.0f, 0.0f };
	m_VertexList[9].p = { 1.0f, 1.0f, 1.0f };			m_VertexList[9].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[9].c = { 0.0f, 1.0f, 1.0f, 1.0f };			m_VertexList[9].t = { 1.0f, 0.0f };
	m_VertexList[10].p = { 1.0f, -1.0f, -1.0f };		m_VertexList[10].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[10].c = { 0.0f, 1.0f, 1.0f, 1.0f };		m_VertexList[10].t = { 0.0f, 1.0f };
	m_VertexList[11].p = { 1.0f, -1.0f, 1.0f }; 		m_VertexList[11].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[11].c = { 0.0f, 1.0f, 1.0f, 1.0f };		m_VertexList[11].t = { 1.0f, 1.0f };
	//µÞ¸é
	m_VertexList[12].p = { 1.0f, 1.0f,  1.0f };			m_VertexList[12].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[12].c = { 0.0f, 0.0f, 1.0f, 1.0f };		m_VertexList[12].t = { 0.0f, 0.0f };
	m_VertexList[13].p = { -1.0f, 1.0f,  1.0f };		m_VertexList[13].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[13].c = { 0.0f, 0.0f, 1.0f, 1.0f };		m_VertexList[13].t = { 1.0f, 0.0f };
	m_VertexList[14].p = { 1.0f, -1.0f,  1.0f };		m_VertexList[14].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[14].c = { 0.0f, 0.0f, 1.0f, 1.0f };		m_VertexList[14].t = { 0.0f, 1.0f };
	m_VertexList[15].p = { -1.0f, -1.0f, 1.0f };		m_VertexList[15].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[15].c = { 0.0f, 0.0f, 1.0f, 1.0f };		m_VertexList[15].t = { 1.0f, 1.0f };
	//À­¸é
	m_VertexList[16].p = { -1.0f, 1.0f,  1.0f }; 		m_VertexList[16].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[16].c = { 0.0f, 1.0f, 0.0f, 1.0f };		m_VertexList[16].t = { 0.0f, 0.0f };
	m_VertexList[17].p = { 1.0f, 1.0f,	 1.0f };		m_VertexList[17].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[17].c = { 0.0f, 1.0f, 0.0f, 1.0f };		m_VertexList[17].t = { 1.0f, 0.0f };
	m_VertexList[18].p = { -1.0f, 1.0f, -1.0f };		m_VertexList[18].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[18].c = { 0.0f, 1.0f, 0.0f, 1.0f };		m_VertexList[18].t = { 0.0f, 1.0f };
	m_VertexList[19].p = { 1.0f, 1.0f,  -1.0f };		m_VertexList[19].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[19].c = { 0.0f, 1.0f, 0.0f, 1.0f };		m_VertexList[19].t = { 1.0f, 1.0f };
	//¾Æ·§¸é
	m_VertexList[20].p = { -1.0f, -1.0f, -1.0f }; 		m_VertexList[20].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[20].c = { 1.0f, 1.0f, 1.0f, 1.0f };		m_VertexList[20].t = { 0.0f, 0.0f };
	m_VertexList[21].p = { 1.0f, -1.0f,  -1.0f };		m_VertexList[21].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[21].c = { 1.0f, 1.0f, 1.0f, 1.0f };		m_VertexList[21].t = { 1.0f, 0.0f };
	m_VertexList[22].p = { -1.0f, -1.0f,  1.0f };		m_VertexList[22].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[22].c = { 1.0f, 1.0f, 1.0f, 1.0f };		m_VertexList[22].t = { 0.0f, 1.0f };
	m_VertexList[23].p = { 1.0f, -1.0f,  1.0f };		m_VertexList[23].n = { 0.0f, 0.0f, 0.0f };			m_VertexList[23].c = { 1.0f, 1.0f, 1.0f, 1.0f };		m_VertexList[23].t = { 1.0f, 1.0f };
}

void XBoxShape::SetIndex()
{
	m_dwIndexList.resize(36);
	//Á¤¸é
	m_dwIndexList[0] = 0;	m_dwIndexList[1] = 1;	m_dwIndexList[2] = 2;
	m_dwIndexList[3] = 3;	m_dwIndexList[4] = 2;	m_dwIndexList[5] = 1;
	//ÁÂÃø
	m_dwIndexList[6] = 4;	m_dwIndexList[7] = 5;	m_dwIndexList[8] = 6;
	m_dwIndexList[9] = 7;	m_dwIndexList[10] = 6;	m_dwIndexList[11] = 5;
	//¿ìÃø
	m_dwIndexList[12] = 8;	m_dwIndexList[13] = 9;	m_dwIndexList[14] = 10;
	m_dwIndexList[15] = 11;	m_dwIndexList[16] = 10;	m_dwIndexList[17] = 9;
	//µÞ¸é
	m_dwIndexList[18] = 12;	m_dwIndexList[19] = 13;	m_dwIndexList[20] = 14;
	m_dwIndexList[21] = 15;	m_dwIndexList[22] = 14;	m_dwIndexList[23] = 13;
	//À­¸é
	m_dwIndexList[24] = 16;	m_dwIndexList[25] = 17;	m_dwIndexList[26] = 18;
	m_dwIndexList[27] = 19;	m_dwIndexList[28] = 18;	m_dwIndexList[29] = 17;
	//¾Æ·§¸é
	m_dwIndexList[30] = 20;	m_dwIndexList[31] = 21;	m_dwIndexList[32] = 22;
	m_dwIndexList[33] = 23;	m_dwIndexList[34] = 22;	m_dwIndexList[35] = 21;
}

bool XBoxShape::Init()
{
	SetVertex();
	m_pVertexBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_VertexList.at(0), m_VertexList.size(), sizeof(PNCT_Vertex), D3D11_BIND_VERTEX_BUFFER));
	SetIndex();
	m_pIndexBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_dwIndexList.at(0), m_dwIndexList.size(), sizeof(DWORD), D3D11_BIND_INDEX_BUFFER));
	m_pConstantBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_ConstantDataMatrix, 1, sizeof(ConstantBuffer_Matrix), D3D11_BIND_CONSTANT_BUFFER));

	return true; 
}

bool XBoxShape::Create(TCHAR* szVertexShaderFile, TCHAR* szPixelShaderFile, char* szVSFunctionName, char* szPSFunctionName)
{
	Init();
	m_pVS.Attach(m_Object.CreateVertexShader(szVertexShaderFile, szVSFunctionName, &m_pVSBuf));
	m_pPS.Attach(m_Object.CreatePixelShader(szPixelShaderFile, szPSFunctionName, &m_pPSBuf));

	//SRV  ÀÏ´Ü ÅØ½ºÃÄ´Â ³ÖÁö ¾ÊÀ½
	//TString Add(ID3D11Device* pd3dDevice, TString szTexture);
	//ComPtr<ID3D11ShaderResourceView> Find(TString szTexture);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_pVertexLayout.Attach(m_Object.CreateInputlayout(I_Device.m_pD3dDevice.Get(), m_pVSBuf->GetBufferSize(), m_pVSBuf->GetBufferPointer(), layout, sizeof(layout) / sizeof(layout[0])));
	return true;
}

bool XBoxShape::Frame() 
{
	return true; 
}

bool XBoxShape::Render(ID3D11DeviceContext* pContext)
{
	pContext->VSSetShader(m_pVS.Get(), NULL, 0);
	pContext->PSSetShader(m_pPS.Get(), NULL, 0);

	pContext->IASetInputLayout(m_pVertexLayout.Get());

	UINT stride = sizeof(PNCT_Vertex);
	UINT offset = 0;

	pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, NULL, &m_ConstantDataMatrix, NULL, NULL);
	pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

	//pContext->PSSetShaderResources(0, 1, &m_pTextureSRV);

	pContext->DrawIndexed(m_dwIndexList.size(), 0, 0);
	return true; 
}

bool XBoxShape::Release()
{
	return true; 
}

XBoxShape::XBoxShape()
{
}

XBoxShape::~XBoxShape()
{
}

void XPlaneShape::SetVertex()
{
	m_VertexList.resize(4);

	m_VertexList[0].p = { -1.0f, 1.0f, 0.0f };	 m_VertexList[0].n = { 0.0f, 0.0f, 0.0f }; m_VertexList[0].c = { 1.0f, 1.0f, 1.0f, 1.0f }; m_VertexList[0].t = { 0.0f, 0.0f };
	m_VertexList[1].p = { 1.0f, 1.0f, 0.0f };	 m_VertexList[1].n = { 0.0f, 0.0f, 0.0f }; m_VertexList[1].c = { 1.0f, 1.0f, 1.0f, 1.0f }; m_VertexList[1].t = { 1.0f, 0.0f };
	m_VertexList[2].p = { -1.0f, -1.0f, 0.0f };	 m_VertexList[2].n = { 0.0f, 0.0f, 0.0f }; m_VertexList[2].c = { 1.0f, 1.0f, 1.0f, 1.0f }; m_VertexList[2].t = { 0.0f, 1.0f };
	m_VertexList[3].p = { 1.0f, -1.0f, 0.0f };	 m_VertexList[3].n = { 0.0f, 0.0f, 0.0f }; m_VertexList[3].c = { 1.0f, 1.0f, 1.0f, 1.0f }; m_VertexList[3].t = { 1.0f, 1.0f };
}

void XPlaneShape::SetIndex()
{
	m_dwIndexList.resize(6);

	m_dwIndexList[0] = 0;	m_dwIndexList[1] = 1;	m_dwIndexList[2] = 2;
	m_dwIndexList[3] = 3;	m_dwIndexList[4] = 2;	m_dwIndexList[5] = 1;
}

bool XPlaneShape::Init()
{
	SetVertex();
	m_pVertexBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_VertexList.at(0), m_VertexList.size(), sizeof(PNCT_Vertex), D3D11_BIND_VERTEX_BUFFER));
	SetIndex();
	m_pIndexBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_dwIndexList.at(0), m_dwIndexList.size(), sizeof(DWORD), D3D11_BIND_INDEX_BUFFER));
	m_pConstantBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_ConstantDataMatrix, 1, sizeof(ConstantBuffer_Matrix), D3D11_BIND_CONSTANT_BUFFER));

	return true;
}

bool XPlaneShape::Create(TCHAR* szTexture, TCHAR* szVertexShaderFile, TCHAR* szPixelShaderFile, char* szVSFunctionName, char* szPSFunctionName)
{
	Init();
	m_pVS.Attach(m_Object.CreateVertexShader(szVertexShaderFile, szVSFunctionName, &m_pVSBuf));
	m_pPS.Attach(m_Object.CreatePixelShader(szPixelShaderFile, szPSFunctionName, &m_pPSBuf));

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_pVertexLayout.Attach(m_Object.CreateInputlayout(I_Device.m_pD3dDevice.Get(), m_pVSBuf->GetBufferSize(), m_pVSBuf->GetBufferPointer(), layout, sizeof(layout) / sizeof(layout[0])));

	if(szTexture != NULL)	m_pTextureSRV = I_SRV.Find(I_SRV.Add(I_Device.m_pD3dDevice.Get(), szTexture));
	return true;
}

bool XPlaneShape::Frame()
{
	return true;
}

bool XPlaneShape::Render(ID3D11DeviceContext* pContext)
{
	pContext->VSSetShader(m_pVS.Get(), NULL, 0);
	pContext->PSSetShader(m_pPS.Get(), NULL, 0);

	pContext->IASetInputLayout(m_pVertexLayout.Get());

	UINT stride = sizeof(PNCT_Vertex);
	UINT offset = 0;

	pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, NULL, &m_ConstantDataMatrix, NULL, NULL);
	pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

	pContext->PSSetShaderResources(0, 1, m_pTextureSRV.GetAddressOf());

	pContext->DrawIndexed(m_dwIndexList.size(), 0, 0);
	return true;
}

bool XPlaneShape::Release()
{
	return true;
}

XPlaneShape::XPlaneShape()
{
}

XPlaneShape::~XPlaneShape()
{
}