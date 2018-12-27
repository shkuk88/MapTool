#include "XViewer.h"

bool	XViewer::LoadShk(TCHAR* filename)
{
	if (filename == NULL)	return false;

	m_Parser.FileOpen(filename, L"r");

	_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
	_stscanf_s(m_Parser.m_strBuffer, _T("%s%d%d%d%d"), m_Parser.m_strName, _countof(m_Parser.m_strName), &m_Scene.m_iFirstFrame, &m_Scene.m_iLastFrame, &m_Scene.m_iFrameSpeed, &m_Scene.m_iTickPerFrame);	//Scene정보

	ReadMtrl();		//Mtrl정보
	ReadMesh();

	m_Parser.FileClose();
	return true;
}

void	XViewer::ReadMtrl()
{
	int iMtrlCnt = 0;
	_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
	_stscanf_s(m_Parser.m_strBuffer, _T("%s%d"), m_Parser.m_strName, _countof(m_Parser.m_strName), &iMtrlCnt);
	m_Mtrl.resize(iMtrlCnt);
	for (int iMtrl = 0; iMtrl < iMtrlCnt; iMtrl++)
	{
		int iTexture = 0;
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%s%d"), m_Parser.m_strName, _countof(m_Parser.m_strName), &iTexture);

		for (int iTexCnt = 0; iTexCnt < iTexture; iTexCnt++)
		{
			XTexture tx;
			_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
			_stscanf_s(m_Parser.m_strBuffer, _T("%d%s"), &tx.m_dwSubNum, tx.m_TextureName, _countof(tx.m_TextureName));
			m_Parser.SetDir(tx.m_TextureName);
			m_Mtrl[iMtrl].m_Texture.push_back(tx);
		}

		int iSubMtrlCnt = 0;
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%s%d"), m_Parser.m_strName, _countof(m_Parser.m_strName), &iSubMtrlCnt);
		for (int iSubMtrl = 0; iSubMtrl < iSubMtrlCnt; iSubMtrl++)
		{
			XMtrl sub;
			_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
			_stscanf_s(m_Parser.m_strBuffer, _T("%s%d"), m_Parser.m_strName, _countof(m_Parser.m_strName), &iTexture);
			for (int iTexCnt = 0; iTexCnt < iTexture; iTexCnt++)
			{
				XTexture tx;
				_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
				_stscanf_s(m_Parser.m_strBuffer, _T("%d%s"), &tx.m_dwSubNum, tx.m_TextureName, _countof(tx.m_TextureName));
				m_Parser.SetDir(tx.m_TextureName);
				sub.m_Texture.push_back(tx);
			}
			m_Mtrl[iMtrl].m_SubMtrl.push_back(sub);
		}
	}
}

void	XViewer::ReadMesh()
{
	int iObjCnt = 0;
	_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
	_stscanf_s(m_Parser.m_strBuffer, _T("%s%d"), m_Parser.m_strName, _countof(m_Parser.m_strName), &iObjCnt);
	m_Mesh.resize(iObjCnt);//서브메터리얼 별로나눌것
	for (int iObj = 0; iObj < iObjCnt; iObj++)
	{
		XMesh mesh;
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%s%d"), m_Parser.m_strName, _countof(m_Parser.m_strName), &m_Mesh[iObj].m_ObjType);
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%s"), m_Mesh[iObj].m_NodeName, _countof(m_Mesh[iObj].m_NodeName));
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%s"), m_Mesh[iObj].m_Parents, _countof(m_Mesh[iObj].m_Parents));
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%f%f%f%f"), &m_Mesh[iObj].m_matTM._11, &m_Mesh[iObj].m_matTM._12, &m_Mesh[iObj].m_matTM._13, &m_Mesh[iObj].m_matTM._14);
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%f%f%f%f"), &m_Mesh[iObj].m_matTM._21, &m_Mesh[iObj].m_matTM._22, &m_Mesh[iObj].m_matTM._23, &m_Mesh[iObj].m_matTM._24);
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%f%f%f%f"), &m_Mesh[iObj].m_matTM._31, &m_Mesh[iObj].m_matTM._32, &m_Mesh[iObj].m_matTM._33, &m_Mesh[iObj].m_matTM._34);
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%f%f%f%f"), &m_Mesh[iObj].m_matTM._41, &m_Mesh[iObj].m_matTM._42, &m_Mesh[iObj].m_matTM._43, &m_Mesh[iObj].m_matTM._44);
		//_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		//_stscanf_s(m_Parser.m_strBuffer, _T("%f%f%f%f"), &m_Mesh[iObj].m_matTM._11, &m_Mesh[iObj].m_matTM._13, &m_Mesh[iObj].m_matTM._12, &m_Mesh[iObj].m_matTM._14);
		//_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		//_stscanf_s(m_Parser.m_strBuffer, _T("%f%f%f%f"), &m_Mesh[iObj].m_matTM._31, &m_Mesh[iObj].m_matTM._33, &m_Mesh[iObj].m_matTM._32, &m_Mesh[iObj].m_matTM._34);
		//_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		//_stscanf_s(m_Parser.m_strBuffer, _T("%f%f%f%f"), &m_Mesh[iObj].m_matTM._21, &m_Mesh[iObj].m_matTM._23, &m_Mesh[iObj].m_matTM._22, &m_Mesh[iObj].m_matTM._24);
		//_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		//_stscanf_s(m_Parser.m_strBuffer, _T("%f%f%f%f"), &m_Mesh[iObj].m_matTM._41, &m_Mesh[iObj].m_matTM._43, &m_Mesh[iObj].m_matTM._42, &m_Mesh[iObj].m_matTM._44);

		int iVertexCnt = 0;
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%s%d"), m_Parser.m_strName, _countof(m_Parser.m_strName), &iVertexCnt);
		if (m_Mesh[iObj].m_iRef >= 0)
		{
			if (m_Mtrl[m_Mesh[iObj].m_iRef].m_SubMtrl.size() > 0)
			{
				m_Mesh[iObj].m_SubMesh.resize(m_Mtrl[m_Mesh[iObj].m_iRef].m_SubMtrl.size());
			}
		}
		for (int iVertex = 0; iVertex < iVertexCnt; iVertex++)
		{
			PNCTGroup pnct;

			_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
			_stscanf_s(m_Parser.m_strBuffer, _T("%f%f%f%f%f%f%f%f%f%f%f%f%d"),
				&pnct.m_Vertex.p.x, &pnct.m_Vertex.p.y, &pnct.m_Vertex.p.z,
				&pnct.m_Vertex.n.x, &pnct.m_Vertex.n.y, &pnct.m_Vertex.n.z,
				&pnct.m_Vertex.c.x, &pnct.m_Vertex.c.y, &pnct.m_Vertex.c.z, &pnct.m_Vertex.c.w,
				&pnct.m_Vertex.t.x, &pnct.m_Vertex.t.y, &pnct.m_dwMtrlId);
			if (m_Mesh[iObj].m_SubMesh.size() == 0)
			{
				m_Mesh[iObj].m_Vertex.push_back(pnct);
				m_Mesh[iObj].m_VertexList.push_back(pnct.m_Vertex);
			}
			else
			{
				if (pnct.m_dwMtrlId > m_Mesh[iObj].m_SubMesh.size()) continue;// mtrl id 가 255인경우 스킵
				m_Mesh[iObj].m_SubMesh[pnct.m_dwMtrlId].m_Vertex.push_back(pnct);
				m_Mesh[iObj].m_SubMesh[pnct.m_dwMtrlId].m_VertexList.push_back(pnct.m_Vertex);
			}
		}
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%d"),&m_Mesh[iObj].m_iRef);
		ReadObjAnim(&m_Mesh[iObj]);
	}
}

bool	XViewer::ReadObjAnim(XMesh* mesh)
{
	if (!mesh) return false;
	int AniCnt = 0;
	_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
	_stscanf_s(m_Parser.m_strBuffer, _T("%s%d"), m_Parser.m_strName, _countof(m_Parser.m_strName), &AniCnt);
	for (int iCnt = 0; iCnt < AniCnt; iCnt++)
	{
		TAnim tanim;
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%d%f%f%f"), &tanim.m_dwTick, &tanim.m_Pos.x, &tanim.m_Pos.y, &tanim.m_Pos.z);
		mesh->m_tAnim.push_back(tanim);
	}
	_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
	_stscanf_s(m_Parser.m_strBuffer, _T("%s%d"), m_Parser.m_strName, _countof(m_Parser.m_strName), &AniCnt);
	for (int iCnt = 0; iCnt < AniCnt; iCnt++)
	{
		RAnim ranim;
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%d%f%f%f%f"), &ranim.m_dwTick, &ranim.m_Rot.x, &ranim.m_Rot.y, &ranim.m_Rot.z, &ranim.m_Rot.w);
		mesh->m_rAnim.push_back(ranim);
	}
	_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
	_stscanf_s(m_Parser.m_strBuffer, _T("%s%d"), m_Parser.m_strName, _countof(m_Parser.m_strName), &AniCnt);
	for (int iCnt = 0; iCnt < AniCnt; iCnt++)
	{
		SAnim sanim;
		_fgetts(m_Parser.m_strBuffer, sizeof(TCHAR) * 256, m_Parser.m_fp);
		_stscanf_s(m_Parser.m_strBuffer, _T("%d%f%f%f%f%f%f%f"), &sanim.m_dwTick, &sanim.m_vPos.x, &sanim.m_vPos.y, &sanim.m_vPos.z, &sanim.m_qRot.x, &sanim.m_qRot.y, &sanim.m_qRot.z, &sanim.m_qRot.w);
		mesh->m_sAnim.push_back(sanim);
	}
	return true;
}

bool	XViewer::CreateMeshSRV(XMesh* mesh, int idx)
{
	if (mesh->m_iRef < 0)	return false;

	int texCnt = 0;
	if (m_Mtrl[mesh->m_iRef].m_SubMtrl.size() == 0)
	{
		texCnt = m_Mtrl[mesh->m_iRef].m_Texture.size();
		for (int iTexture = 0; iTexture < texCnt; iTexture++)
		{
			if (FAILED(hr = CreateShaderResourceView(mesh, m_Mtrl[mesh->m_iRef].m_Texture[iTexture].m_TextureName)))
			{
				//MessageBox(0, L"LoadTexture FAIL", 0, 0);
				return false;
			}
		}
	}
	else
	{
		texCnt = m_Mtrl[mesh->m_iRef].m_SubMtrl[idx].m_Texture.size();
		for (int iTexture = 0; iTexture < texCnt; iTexture++)
		{
			if (FAILED(hr = CreateShaderResourceView(mesh, m_Mtrl[mesh->m_iRef].m_SubMtrl[idx].m_Texture[iTexture].m_TextureName)))
			{
				//MessageBox(0, L"LoadTexture FAIL", 0, 0);
				return false;
			}
		}
	}

}

HRESULT		XViewer::CreateVertexBuffer(XMesh* mesh)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(PNCT) * mesh->m_VertexList.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA VertexData;
	VertexData.pSysMem = &mesh->m_VertexList.at(0);

	return m_pd3dDevice->CreateBuffer(&bd, &VertexData, &mesh->m_pVertexBuffer);
}

HRESULT		XViewer::CreateIndexBuffer(XMesh* mesh)
{

	D3D11_BUFFER_DESC IndexBufferDesc;
	ZeroMemory(&IndexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	IndexBufferDesc.ByteWidth = sizeof(DWORD) * mesh->m_dwIndexData.size();
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA IndexBufferData;
	ZeroMemory(&IndexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	IndexBufferData.pSysMem = &mesh->m_dwIndexData.at(0);
	V_RETURN(m_pd3dDevice->CreateBuffer(&IndexBufferDesc, &IndexBufferData, &mesh->m_pIndexBuffer));
}

HRESULT		XViewer::CreateConstantBuffer(XMesh* mesh)
{
	D3D11_BUFFER_DESC ConstsntBufferDesc;
	ZeroMemory(&ConstsntBufferDesc, sizeof(D3D11_BUFFER_DESC));
	ConstsntBufferDesc.ByteWidth = sizeof(CB);
	ConstsntBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	V_RETURN(m_pd3dDevice->CreateBuffer(&ConstsntBufferDesc, NULL, &mesh->m_pConstantBuffer));
	return hr;
}

HRESULT		XViewer::CreateShaderResourceView(XMesh* mesh, TCHAR* TextureFile)
{
	ID3D11ShaderResourceView*   m_pTextureRSV;
	V_RETURN(D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, TextureFile, NULL, NULL, &m_pTextureRSV, NULL));
	mesh->m_pTextureSRV.push_back(m_pTextureRSV);
	return hr;
}

HRESULT		XViewer::LoadShader(TCHAR* VSfile, TCHAR* PSfile)
{
	V_RETURN(D3DX11CompileFromFile(VSfile, NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, NULL, &m_pVSBuf, NULL, NULL));
	V_RETURN(m_pd3dDevice->CreateVertexShader((DWORD*)m_pVSBuf->GetBufferPointer(), m_pVSBuf->GetBufferSize(), NULL, &m_pVS));

	V_RETURN(D3DX11CompileFromFile(PSfile, NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, NULL, &m_pPSBuf, NULL, NULL));
	V_RETURN(m_pd3dDevice->CreatePixelShader((DWORD*)m_pPSBuf->GetBufferPointer(), m_pPSBuf->GetBufferSize(), NULL, &m_pPS));

	return hr;
}

HRESULT		XViewer::InputLayout()
{
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	int iEle = sizeof(layout) / sizeof(layout[0]);
	V_RETURN(m_pd3dDevice->CreateInputLayout(layout, iEle, m_pVSBuf->GetBufferPointer(), m_pVSBuf->GetBufferSize(), &m_pVertexLayout));
	return hr;
}

void	XViewer::SetMatrix(D3DXMATRIX *matWorld, D3DXMATRIX *matView, D3DXMATRIX *matProj)
{
	if (matWorld)
	{
		m_matWorld = *matWorld;
	}
	if (matView)
	{
		m_matView = *matView;
	}
	if (matProj)
	{
		m_matProj = *matProj;
	}

	D3DXMatrixTranspose(&cb.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&cb.matView, &m_matView);
	D3DXMatrixTranspose(&cb.matProj, &m_matProj);
}

void XViewer::SelectObjTransparency(ID3D11DeviceContext* pContext)
{
	for (int iMeshCnt = 0; iMeshCnt < m_Mesh.size(); iMeshCnt++)
	{
		if (m_Mesh[iMeshCnt].m_SubMesh.size() == 0)
		{
			if (m_Mesh[iMeshCnt].m_VertexList.size() != 0)
			{
				for (int iLoop = 0; iLoop < m_Mesh[iMeshCnt].m_VertexList.size(); iLoop++)
				{
					m_Mesh[iMeshCnt].m_VertexList[iLoop].c.w = 0.7f;
				}
				pContext->UpdateSubresource(m_Mesh[iMeshCnt].m_pVertexBuffer, 0, NULL, &m_Mesh[iMeshCnt].m_VertexList.at(0), NULL, NULL);
			}
		}
		else
		{
			for (int iSubCnt = 0; iSubCnt < m_Mesh[iMeshCnt].m_SubMesh.size(); iSubCnt++)
			{
				if (m_Mesh[iMeshCnt].m_SubMesh[iSubCnt].m_VertexList.size() != 0)
				{
					if (m_Mesh[iMeshCnt].m_VertexList.size() != 0)
					{
						for (int iLoop = 0; iLoop < m_Mesh[iMeshCnt].m_SubMesh[iSubCnt].m_VertexList.size(); iLoop++)
						{
							m_Mesh[iMeshCnt].m_SubMesh[iSubCnt].m_VertexList[iLoop].c.w = 0.7f;
						}
						pContext->UpdateSubresource(m_Mesh[iMeshCnt].m_SubMesh[iSubCnt].m_pVertexBuffer, 0, NULL, &m_Mesh[iMeshCnt].m_SubMesh[iSubCnt].m_VertexList.at(0), NULL, NULL);
					}
				}
			}
		}
	}

}

void XViewer::SelectObjOpaque(ID3D11DeviceContext* pContext)
{
	for (int iMeshCnt = 0; iMeshCnt < m_Mesh.size(); iMeshCnt++)
	{
		if (m_Mesh[iMeshCnt].m_SubMesh.size() == 0)
		{
			if (m_Mesh[iMeshCnt].m_VertexList.size() != 0)
			{
				for (int iLoop = 0; iLoop < m_Mesh[iMeshCnt].m_VertexList.size(); iLoop++)
				{
					m_Mesh[iMeshCnt].m_VertexList[iLoop].c.w = 1.0f;
				}
				pContext->UpdateSubresource(m_Mesh[iMeshCnt].m_pVertexBuffer, 0, NULL, &m_Mesh[iMeshCnt].m_VertexList.at(0), NULL, NULL);
			}
		}
		else
		{
			for (int iSubCnt = 0; iSubCnt < m_Mesh[iMeshCnt].m_SubMesh.size(); iSubCnt++)
			{
				if (m_Mesh[iMeshCnt].m_SubMesh[iSubCnt].m_VertexList.size() != 0)
				{
					if (m_Mesh[iMeshCnt].m_VertexList.size() != 0)
					{
						for (int iLoop = 0; iLoop < m_Mesh[iMeshCnt].m_SubMesh[iSubCnt].m_VertexList.size(); iLoop++)
						{
							m_Mesh[iMeshCnt].m_SubMesh[iSubCnt].m_VertexList[iLoop].c.w = 1.0f;
						}
						pContext->UpdateSubresource(m_Mesh[iMeshCnt].m_SubMesh[iSubCnt].m_pVertexBuffer, 0, NULL, &m_Mesh[iMeshCnt].m_SubMesh[iSubCnt].m_VertexList.at(0), NULL, NULL);
					}
				}
			}
		}
	}
}

bool	XViewer::SetBuffer(XMesh* mesh)
{
	if (FAILED(CreateVertexBuffer(mesh)))
	{
		MessageBox(0, L"CreateVertexBuffer FAIL", 0, 0);
		return false;
	}
	//if (FAILED(CreateIndexBuffer(mesh)))
	//{
	//	MessageBox(0, L"CreateIndexBuffer FAIL", 0, 0);
	//	return false;
	//}
}

bool	XViewer::FindParent()
{
	for (int iMesh = 0; iMesh < m_Mesh.size(); iMesh++)
	{
		if (!_tcscmp(m_Mesh[iMesh].m_Parents, _T("none")))
		{
			continue;
		}
		for (int iObj = 0; iObj < iMesh; iObj++)
		{
			if (!_tcscmp(m_Mesh[iMesh].m_Parents, m_Mesh[iObj].m_NodeName))
			{
				m_Mesh[iMesh].m_ParentObj.push_back(iObj);
				m_Mesh[iObj].m_ChildObj.push_back(iMesh);
				break;
			}
		}
	}
	return true;
}

bool	XViewer::Init(TCHAR* filename, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, TCHAR* VSfile, TCHAR* PSfile)
{
	m_pd3dDevice = pd3dDevice;
	m_pContext = pContext;


	if (!_tcscmp(m_Parser.ExtType(filename), L".Shk"))
	{
		if (!LoadShk(filename))
		{
			MessageBox(0, L"Load FAIL", 0, 0);
			return false;
		}
	}
	else
	{
		MessageBox(0, L"Load FAIL", 0, 0);
		return false;
	}
	FindParent();

	for (int iMeshCnt = 0; iMeshCnt < m_Mesh.size(); iMeshCnt++)
	{
		if (m_Mesh[iMeshCnt].m_SubMesh.size() == 0)
		{
			if (m_Mesh[iMeshCnt].m_VertexList.size() != 0)
			{
				SetBuffer(&m_Mesh[iMeshCnt]);
			}
		}
		else
		{
			for (int iSubCnt = 0; iSubCnt < m_Mesh[iMeshCnt].m_SubMesh.size(); iSubCnt++)
			{
				if (m_Mesh[iMeshCnt].m_SubMesh[iSubCnt].m_VertexList.size() != 0)
				{
					SetBuffer(&m_Mesh[iMeshCnt].m_SubMesh[iSubCnt]);
				}
			}
		}
	}
	if (FAILED(hr = LoadShader(VSfile, PSfile)))
	{
		MessageBox(0, L"LoadShader FAIL", 0, 0);
		return false;
	}
	if (FAILED(hr = InputLayout()))
	{
		MessageBox(0, L"InputLayout FAIL", 0, 0);
		return false;
	}
	for (int iMesh = 0; iMesh < m_Mesh.size(); iMesh++)
	{
		if (FAILED(hr = CreateConstantBuffer(&m_Mesh[iMesh])))
		{
			MessageBox(0, L"CreateConstantBuffer FAIL", 0, 0);
			return false;
		}
	}

	for (int iMesh = 0; iMesh < m_Mesh.size(); iMesh++)
	{
		if (m_Mesh[iMesh].m_SubMesh.size() == 0)
		{
			CreateMeshSRV(&m_Mesh[iMesh]);
		}
		else
		{
			for (int iSubMesh = 0; iSubMesh < m_Mesh[iMesh].m_SubMesh.size(); iSubMesh++)
			{
				CreateMeshSRV(&m_Mesh[iMesh].m_SubMesh[iSubMesh], iSubMesh);
			}
		}
	}

	return true;
}

void	XViewer::SetTickSpeed(float speed)
{
	m_fTickSpeed = speed;
}

D3DXMATRIX	XViewer::GetSAnim(XMesh* mesh)
{
	D3DXMATRIX sMat;
	if (mesh->m_sAnim.size() <= 0)
	{
		if (mesh->m_ParentObj.size() != 0)
		{
			D3DXVECTOR3 s, t;
			D3DXQUATERNION r;
			D3DXMATRIX invPMW, m_matInverse;
			D3DXMatrixInverse(&invPMW, NULL, &m_Mesh[mesh->m_ParentObj[0]].m_matTM);
			m_matInverse = mesh->m_matTM * invPMW;
			D3DXMatrixDecompose(&s, &r, &t, &m_matInverse);
			D3DXMatrixScaling(&sMat, s.x, s.y, s.z);
			return sMat;
		}
		else/////////////부모없을때 0번
		{
			D3DXVECTOR3 s, t;
			D3DXQUATERNION r;
			D3DXMATRIX	m_matInverse;
			D3DXMatrixInverse(&m_matInverse, NULL, &mesh->m_matTM);
			D3DXMatrixDecompose(&s, &r, &t, &m_matInverse);
			D3DXMatrixScaling(&sMat, s.x, s.y, s.z);
			return sMat;
			
		}
	}
	for (int iCnt = 0; iCnt<mesh->m_sAnim.size() - 1; iCnt++)
	{
		if (mesh->m_sAnim[iCnt].m_dwTick<=m_fTick&&mesh->m_sAnim[iCnt + 1].m_dwTick>m_fTick)
		{
			float		t = 1 - (mesh->m_sAnim[iCnt + 1].m_dwTick - m_fTick) / (mesh->m_sAnim[iCnt + 1].m_dwTick - mesh->m_sAnim[iCnt].m_dwTick);
			D3DXMATRIX rMat, rInvMat, tMat;
			D3DXQUATERNION q, qInverse;
			D3DXQuaternionSlerp(&q, &mesh->m_sAnim[iCnt].m_qRot, &mesh->m_sAnim[iCnt + 1].m_qRot, t);/////////구면 선형보간
			D3DXMatrixRotationQuaternion(&rMat, &q);
			D3DXMatrixInverse(&rInvMat, NULL, &rMat);
			D3DXVECTOR3 p;
			D3DXVec3Lerp(&p, &mesh->m_sAnim[iCnt].m_vPos, &mesh->m_sAnim[iCnt + 1].m_vPos, t);
			D3DXMatrixScaling(&tMat, p.x, p.y, p.z);
			sMat = rInvMat * tMat * rMat;
			return sMat;
		}
	}
}

D3DXMATRIX	XViewer::GetRAnim(XMesh* mesh)
{
	D3DXMATRIX rMat;

	if (mesh->m_rAnim.size() <= 0)
	{
		if (mesh->m_ParentObj.size() != 0)
		{
			D3DXVECTOR3 s, t;
			D3DXQUATERNION r;
			D3DXMATRIX invPMW, m_matInverse;
			D3DXMatrixInverse(&invPMW, NULL, &m_Mesh[mesh->m_ParentObj[0]].m_matTM);
			m_matInverse = mesh->m_matTM * invPMW;
			D3DXMatrixDecompose(&s, &r, &t, &m_matInverse);
			D3DXMatrixRotationQuaternion(&rMat, &r);
			return rMat;
		}
		else/////////////부모없을때 0번
		{
			D3DXVECTOR3 s, t;
			D3DXQUATERNION r;
			D3DXMatrixDecompose(&s, &r, &t, &mesh->m_matTM);
			D3DXMatrixRotationQuaternion(&rMat, &r);
			return rMat;
		}
	}
	for (int iCnt = 0; iCnt<mesh->m_rAnim.size() - 1; iCnt++)
	{
		if (mesh->m_rAnim[iCnt].m_dwTick<=m_fTick&&mesh->m_rAnim[iCnt + 1].m_dwTick>m_fTick)
		{
			float		t = 1 - (mesh->m_rAnim[iCnt + 1].m_dwTick - m_fTick) / (mesh->m_rAnim[iCnt + 1].m_dwTick - mesh->m_rAnim[iCnt].m_dwTick);
			D3DXQUATERNION q;
			D3DXQuaternionSlerp(&q, &mesh->m_rAnim[iCnt].m_Rot, &mesh->m_rAnim[iCnt + 1].m_Rot, t);/////////구면 선형보간
			D3DXMatrixRotationQuaternion(&rMat, &q);
			return rMat;
		}
	}
}

D3DXMATRIX	XViewer::GetTAnim(XMesh* mesh)
{
	D3DXMATRIX	tMat;
	if (mesh->m_tAnim.size() <= 0)
	{
		if (mesh->m_ParentObj.size() != 0)
		{
			D3DXVECTOR3 s, t;
			D3DXQUATERNION r;
			D3DXMATRIX invPMW, m_matInverse;
			D3DXMatrixInverse(&invPMW, NULL, &m_Mesh[mesh->m_ParentObj[0]].m_matTM);
			m_matInverse = mesh->m_matTM * invPMW;
			D3DXMatrixDecompose(&s, &r, &t, &m_matInverse);
			D3DXMatrixTranslation(&tMat, t.x, t.y, t.z);
			return tMat;

		}
		else/////////////부모없을때 0번
		{
			D3DXVECTOR3 s, t;
			D3DXQUATERNION r;
			D3DXMATRIX	m_matInverse;
			D3DXMatrixInverse(&m_matInverse, NULL, &mesh->m_matTM);
			D3DXMatrixDecompose(&s, &r, &t, &m_matInverse);
			D3DXMatrixTranslation(&tMat, t.x, t.y, t.z);
			return tMat;
		}
	}
	for (int iCnt = 0; iCnt<mesh->m_tAnim.size() - 1; iCnt++)
	{
		if (mesh->m_tAnim[iCnt].m_dwTick <= m_fTick&&mesh->m_tAnim[iCnt + 1].m_dwTick>m_fTick)
		{
			float		t = 1 - (mesh->m_tAnim[iCnt + 1].m_dwTick - m_fTick) / (mesh->m_tAnim[iCnt + 1].m_dwTick - mesh->m_tAnim[iCnt].m_dwTick);
			D3DXVECTOR3 p;
			D3DXVec3Lerp(&p, &mesh->m_tAnim[iCnt].m_Pos, &mesh->m_tAnim[iCnt + 1].m_Pos, t);
			D3DXMatrixTranslation(&tMat, p.x, p.y, p.z);
			return tMat;
		}
	}
}

void	XViewer::SetObjAnim(XMesh* mesh)
{
	D3DXMATRIX s = GetSAnim(mesh);
	D3DXMATRIX r = GetRAnim(mesh);
	D3DXMATRIX t = GetTAnim(mesh);
	if (mesh->m_ParentObj.size() != 0)
	{
		mesh->m_matCalculation = s*r*t;
		mesh->m_matCalculation *= m_Mesh[mesh->m_ParentObj[0]].m_matCalculation;
	}
	else
	{
		mesh->m_matCalculation = s*r*t;
	}
	mesh->m_matCalculation *= m_matWorld;
	D3DXMatrixTranspose(&mesh->cb.matWorld, &mesh->m_matCalculation);
	D3DXMatrixTranspose(&mesh->cb.matView, &m_matView);
	D3DXMatrixTranspose(&mesh->cb.matProj, &m_matProj);
}

bool	XViewer::Frame()
{
	m_fTick += g_fSecPerFrame * m_Scene.m_iFrameSpeed * m_Scene.m_iTickPerFrame * m_fTickSpeed;
	if (m_fTick > m_Scene.m_iLastFrame * m_Scene.m_iTickPerFrame)
	{
		m_fTick = 0.0f;
	}
	for (int iObj = 0; iObj < m_Mesh.size(); iObj++)
	{
		SetObjAnim(&m_Mesh[iObj]);
	}

	return true;
}

bool	XViewer::PreRender(ID3D11DeviceContext*	pContext)
{
	pContext->PSSetSamplers(0, 1, XDxState::g_SSLinear.GetAddressOf());
	//pContext->RSSetState(XDxState::g_pRSFSolidNone.Get());
	return true;
}

bool	XViewer::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->PSSetSamplers(0, 1, XDxState::g_SSPoint.GetAddressOf());
	//pContext->RSSetState(XDxState::g_pRSFSolid.Get());
	return true;
}

bool	XViewer::DrawObject(ID3D11DeviceContext* pContext, XMesh* mesh)
{
	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->PSSetShader(m_pPS, NULL, 0);
	pContext->IASetInputLayout(m_pVertexLayout);

	UINT stride = sizeof(PNCT);
	UINT offset = 0;

	pContext->IASetVertexBuffers(0, 1, &mesh->m_pVertexBuffer, &stride, &offset);
	pContext->IASetIndexBuffer(mesh->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (int iCnt = 0; iCnt < mesh->m_pTextureSRV.size(); iCnt++)
	{
		pContext->PSSetShaderResources(0, 1, &mesh->m_pTextureSRV[iCnt]);
	}
	//pContext->DrawIndexed(mesh->m_dwIndexData.size(), 0, 0);
	pContext->Draw(mesh->m_VertexList.size(),0);
	return true;
}

bool	XViewer::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);

	for (int iMeshCnt = 0; iMeshCnt < m_Mesh.size(); iMeshCnt++)
	{
		if (m_Mesh[iMeshCnt].m_ObjType == HELPEROBJ)
		{
			continue;
		}
		if (m_Mesh[iMeshCnt].m_SubMesh.size() == 0)
		{
			// m_Mesh[iMeshCnt].cb.matWorld <-- 오브젝트의 원래 월드행렬, 회전,위치,스케일은 직접 줄 예정
			pContext->UpdateSubresource(m_Mesh[iMeshCnt].m_pConstantBuffer, 0, NULL, &cb.matWorld, NULL, NULL);
			pContext->VSSetConstantBuffers(0, 1, &m_Mesh[iMeshCnt].m_pConstantBuffer);
			DrawObject(pContext, &m_Mesh[iMeshCnt]);
		}
		else
		{
			for (int iSubCnt = 0; iSubCnt < m_Mesh[iMeshCnt].m_SubMesh.size(); iSubCnt++)
			{
				pContext->UpdateSubresource(m_Mesh[iMeshCnt].m_pConstantBuffer, 0, NULL, &cb.matWorld, NULL, NULL);
				pContext->VSSetConstantBuffers(0, 1, &m_Mesh[iMeshCnt].m_pConstantBuffer);
				DrawObject(pContext, &m_Mesh[iMeshCnt].m_SubMesh[iSubCnt]);
			}
		}
	}

	PostRender(pContext);
	return true;
}

bool	XViewer::Release()
{
	return true;
}

XViewer::XViewer()
{
	m_fTick			= 0.0f;
	m_fTickSpeed	= 1.0f;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

XViewer::~XViewer()
{
}
