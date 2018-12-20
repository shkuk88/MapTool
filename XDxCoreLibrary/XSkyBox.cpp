#include "XSkyBox.h"


HRESULT XSkyBox::LoadTextures(ID3D11Device* pDevice)
{
	TCHAR* SkyTextures[] =
	{
		L"../Data/Map/Texture/Sunny_01A_front.jpg",
		L"../Data/Map/Texture/Sunny_01A_right.jpg",
		L"../Data/Map/Texture/Sunny_01A_left.jpg",
		L"../Data/Map/Texture/Sunny_01A_back.jpg",
		L"../Data/Map/Texture/Sunny_01A_up.jpg",
		L"../Data/Map/Texture/Sunny_01A_down.jpg"
		//L"../Data/Map/Texture/sky_front.bmp",
		//L"../Data/Map/Texture/sky_left.bmp",
		//L"../Data/Map/Texture/sky_right.bmp",
		//L"../Data/Map/Texture/sky_back.bmp",
		//L"../Data/Map/Texture/sky_up.bmp",
		//L"../Data/Map/Texture/sky_down.bmp"
	};
	for (int iCnt = 0; iCnt < 6; iCnt++)
	{
		m_pTextureSRV[iCnt] = I_SRV.Find(I_SRV.Add(pDevice, SkyTextures[iCnt]));
	}
	return hr;
}


bool XSkyBox::Render(ID3D11DeviceContext* pContext)
{
	m_matView._41 = 0.0f; m_matView._42 = 0.0f; m_matView._43 = 0.0f;
	SetMatrix(&m_matWorld, &m_matView, &m_matProj);
	
	pContext->VSSetShader(m_pVS.Get(), NULL, 0);
	pContext->PSSetShader(m_pPS.Get(), NULL, 0);

	pContext->IASetInputLayout(m_pVertexLayout.Get());

	UINT stride = sizeof(PNCT_Vertex);
	UINT offset = 0;

	pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, NULL, &m_ConstantDataMatrix, NULL, NULL);
	pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());


	pContext->PSSetShaderResources(0, 6, m_pTextureSRV->GetAddressOf());
	pContext->DrawIndexed(m_dwIndexList.size(), 0, 0);
	return true;
}



XSkyBox::XSkyBox()
{
}


XSkyBox::~XSkyBox()
{
}
