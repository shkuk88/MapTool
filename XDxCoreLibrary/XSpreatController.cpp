#include "XSpreatController.h"



void XSpreatController::SetSpreatViewState(SpreatView SpreatViewState)
{
	m_SpreatView = SpreatViewState;
}

HRESULT XSpreatController::CreateSpreatTexture()
{
	m_TextureDesc.Width = m_pMap->m_iRow * m_pMap->m_fDistance* 10.0f;
	m_TextureDesc.Height = m_pMap->m_iCol * m_pMap->m_fDistance* 10.0f;
	m_TextureDesc.MipLevels = 1;
	m_TextureDesc.ArraySize = 1;
	m_TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_TextureDesc.SampleDesc.Count = 1;
	m_TextureDesc.SampleDesc.Quality = 0;
	m_TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	m_TextureDesc.CPUAccessFlags = 0;
	m_TextureDesc.MiscFlags = 0;
	m_TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	hr = I_Device.m_pD3dDevice->CreateTexture2D(&m_TextureDesc, NULL, &m_SpreatTexture);
	hr = I_Device.m_pD3dDevice->CreateTexture2D(&m_TextureDesc, NULL, &m_AlphaTexture[0]);
	hr = I_Device.m_pD3dDevice->CreateTexture2D(&m_TextureDesc, NULL, &m_AlphaTexture[1]);
	hr = I_Device.m_pD3dDevice->CreateTexture2D(&m_TextureDesc, NULL, &m_AlphaTexture[2]);
	hr = I_Device.m_pD3dDevice->CreateTexture2D(&m_TextureDesc, NULL, &m_AlphaTexture[3]);


	m_TextureDesc.Usage = D3D11_USAGE_STAGING;
	m_TextureDesc.BindFlags = 0;
	m_TextureDesc.MiscFlags = 0;
	m_TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	hr = I_Device.m_pD3dDevice->CreateTexture2D(&m_TextureDesc, NULL, &m_StagingTexture);

	return hr;
}

void XSpreatController::Spreating(ID3D11DeviceContext * pContext, X_Box CollisionBox, D3DXVECTOR3 vCrash, float fRadius, int iColor)
{
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	D3DXVECTOR3 CrashDot = vCrash;
	CrashDot.x = CrashDot.x * m_TextureDesc.Width;
	CrashDot.y = 0.0f;
	CrashDot.z = CrashDot.z * m_TextureDesc.Height;
	X_Box FixedArea;
	FixedArea.vMax.x = (int)(CollisionBox.vMax.x * m_TextureDesc.Width);
	FixedArea.vMax.z = (int)(CollisionBox.vMax.z * m_TextureDesc.Height);
	FixedArea.vMin.x = (int)(CollisionBox.vMin.x * m_TextureDesc.Width);
	FixedArea.vMin.z = (int)(CollisionBox.vMin.z * m_TextureDesc.Height);

	if (FixedArea.vMin.x < 0.0f)
	{
		FixedArea.vMin.x = 0.0f;
	}
	if (FixedArea.vMin.z < 0.0f)
	{
		FixedArea.vMin.z = 0.0f;
	}
	if (FixedArea.vMax.x > m_TextureDesc.Width)
	{
		FixedArea.vMax.x = m_TextureDesc.Width;
	}
	if (FixedArea.vMax.z > m_TextureDesc.Height)
	{
		FixedArea.vMax.z = m_TextureDesc.Height;
	}

	float CheckRadius = fRadius * 10.0f;
	if (SUCCEEDED(pContext->Map((ID3D11Resource*)m_StagingTexture, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		pDestBytes = pDestBytes + MappedFaceDest.RowPitch * (UINT)FixedArea.vMin.z;
		for (int y = FixedArea.vMin.z; y < FixedArea.vMax.z; y++)
		{
			BYTE* pDest = pDestBytes;
			pDest = pDest + (UINT)(4 * FixedArea.vMin.x);
			for (int x = FixedArea.vMin.x; x < FixedArea.vMax.x; x++)
			{
				D3DXVECTOR3 Dot = D3DXVECTOR3(x, 0.0f, y);
				float fColorPower = D3DXVec3Length(&(Dot - CrashDot));
				if (CheckRadius >= fColorPower)
				{
					switch (iColor)
					{
					case 0:
						*pDest++ = 255;
						*pDest++ = 0;
						*pDest++ = 0;
						*pDest++ = 0;
						break;
					case 1:
						*pDest++ = 0;
						*pDest++ = 255;
						*pDest++ = 0;
						*pDest++ = 0;
						break;
					case 2:
						*pDest++ = 0;
						*pDest++ = 0;
						*pDest++ = 255;
						*pDest++ = 0;
						break;
					case 3:
						*pDest++ = 0;
						*pDest++ = 0;
						*pDest++ = 0;
						*pDest++ = 255;
						break;
					default:
						break;
					}
				}
				else
				{
					pDest += 4;
				}
			}
			pDestBytes += MappedFaceDest.RowPitch;
		}
		pContext->Unmap(m_StagingTexture, 0);
		pContext->CopyResource(m_SpreatTexture, m_StagingTexture);
		//D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		//ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		//SRVDesc.Format = m_TextureDesc.Format;
		//SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//SRVDesc.Texture2D.MipLevels = m_TextureDesc.MipLevels;
		//FAILED(hr = I_Device.m_pD3dDevice->CreateShaderResourceView(m_SpreatTexture, &SRVDesc, m_SpreatingTextureSRV.GetAddressOf()));
	}
}


HRESULT XSpreatController::RGBA_TextureLoad(ID3D11Device * pDevice, const TCHAR * szFile, AlphaColor Color)
{
	if (!m_bStart)
	{
		hr = E_FAIL;
		return hr;
	}
	ID3D11ShaderResourceView* pSRV = nullptr;
	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(pDevice, szFile, NULL, NULL, &pSRV, NULL)))
	{
		m_RGBA_TextureSRV[Color].Attach(nullptr);
	}
	m_RGBA_TextureSRV[Color].Attach(pSRV);
	if(!Color != Spreat_None)
		m_pMap->SetAlphaSRV(pSRV, (int)Color);
	return hr;
}

void XSpreatController::MakeSRV(ID3D11Texture2D* pTexture, ComPtr<ID3D11ShaderResourceView>* pSRV)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	SRVDesc.Format = m_TextureDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = m_TextureDesc.MipLevels;
	hr = I_Device.m_pD3dDevice->CreateShaderResourceView(pTexture, &SRVDesc, pSRV->GetAddressOf());
}

void XSpreatController::Start()
{
	XMapController::Start();
	Init();
}

bool XSpreatController::Init()
{
	CreateSpreatTexture();
	MakeSRV(m_SpreatTexture, &m_SpreatingTextureSRV);
	MakeSRV(m_AlphaTexture[0], &m_RGBA_TextureSRV[Spreat_Red]);
	m_pMap->SetAlphaSRV(m_RGBA_TextureSRV[Spreat_Red].Get(), (int)Spreat_Red);
	MakeSRV(m_AlphaTexture[1], &m_RGBA_TextureSRV[Spreat_Green]);
	m_pMap->SetAlphaSRV(m_RGBA_TextureSRV[Spreat_Green].Get(), (int)Spreat_Green);
	MakeSRV(m_AlphaTexture[2], &m_RGBA_TextureSRV[Spreat_Blue]);
	m_pMap->SetAlphaSRV(m_RGBA_TextureSRV[Spreat_Blue].Get(), (int)Spreat_Blue);
	MakeSRV(m_AlphaTexture[3], &m_RGBA_TextureSRV[Spreat_Alpha]);
	m_pMap->SetAlphaSRV(m_RGBA_TextureSRV[Spreat_Alpha].Get(), (int)Spreat_Alpha);

	m_AlphaPS.Attach(m_pMap->m_Object.CreatePixelShader(_T("../Data/Shader/MapShader_Specular.hlsl"), "AlphaMap_PS", &m_pMap->m_pPSBuf));
	return true;
}

bool XSpreatController::Frame()
{
	if (!m_bStart || !m_bEnable) return false;
	if (I_Input.m_MouseState[0])
	{
		if (!CheakInRange())	return false;
		for (auto pNode : m_CrashNode)
		{
			for (int iFace = 0; iFace < pNode->m_IndexList.size();)
			{
				for (int iTri = 0; iTri < 3; iTri++, iFace++)
				{
					float fRootNodeSize = m_pRootNode->m_Box.vMax.x - m_pRootNode->m_Box.vMin.x;
					X_Box SpreatBox;
					D3DXVECTOR3 vCrash = m_vIntersection;
					vCrash.z *= -1.0f;
					vCrash.x = vCrash.x + (fRootNodeSize / 2);
					vCrash.z = vCrash.z + (fRootNodeSize / 2);
					SpreatBox.vMin.x = vCrash.x - m_fRadius;
					SpreatBox.vMin.z = vCrash.z - m_fRadius;
					SpreatBox.vMax.x = vCrash.x + m_fRadius;
					SpreatBox.vMax.z = vCrash.z + m_fRadius;

					vCrash.x = vCrash.x / fRootNodeSize;
					vCrash.z = vCrash.z / fRootNodeSize;

					SpreatBox.vMin.x = SpreatBox.vMin.x / fRootNodeSize;
					SpreatBox.vMin.z = SpreatBox.vMin.z / fRootNodeSize;
					SpreatBox.vMax.x = SpreatBox.vMax.x / fRootNodeSize;
					SpreatBox.vMax.z = SpreatBox.vMax.z / fRootNodeSize;
					Spreating(I_Device.m_pD3dContext.Get(), SpreatBox, vCrash, m_fRadius, m_SpreatColor);
				}
			}
		}
	}
	return true;
}

bool XSpreatController::Render(ID3D11DeviceContext* pContext)
{
	if (m_bEnable)
	{
		m_pMap->PreRender(pContext);
		// 알파텍스쳐에 입힐 이미지 등록
		if (m_SpreatView == SpreatView_Texture)
		{
			pContext->PSSetShader(m_pMap->m_pPS->Get(), NULL, 0);
			for (int iAlphaColor = 0; iAlphaColor < 4; iAlphaColor++)
			{
				pContext->PSSetShaderResources((iAlphaColor + 2), 1, m_RGBA_TextureSRV[iAlphaColor].GetAddressOf()); //사실상 쓰지않음. 일단 RGBA로만 뿌려볼 예정
			}
		}
		else if (m_SpreatView == SpreatView_Alpha)
		{
			pContext->PSSetShader(m_AlphaPS.Get(), NULL, 0);
		}
		else
		{
			//pContext->PSSetShader(m_AlphaPS.Get(), NULL, 0);
			MessageBox(NULL, _T("등록된 텍스처가 없습니다"), NULL, NULL);
			exit(-1);
		}
		// 알파텍스쳐 등록
		if (m_SpreatingTextureSRV.Get())
		{
			pContext->PSSetShaderResources(0, 1, m_SpreatingTextureSRV.GetAddressOf()); // 텍스
		}
		m_pMap->PostRender(pContext);
	}
	return true;
}

bool XSpreatController::Release()
{
	if (!m_bStart) return false;
	// 포인터기 때문에 clear 작업만 해준다.
	m_LeafNodeList.clear();
	m_CrashNode.clear();
	m_CraseVertex.clear();
	return true;
}

XSpreatController::XSpreatController()
{
	m_SpreatColor = Spreat_None;
}


XSpreatController::~XSpreatController()
{
}
