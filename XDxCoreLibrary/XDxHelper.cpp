#include "XDxHelper.h"

namespace DX
{
	ID3D11Buffer* XDxHelper::CreateBuffer(ID3D11Device*  pd3dDevice, void *pData, UINT iNumCount, UINT iSize, D3D11_BIND_FLAG bindFlag, bool bDynamic)
	{
		HRESULT hr = S_OK;
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		if (bDynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}
		bd.ByteWidth = iSize * iNumCount;
		bd.BindFlags = bindFlag;
		InitData.pSysMem = pData;
		if (pData != NULL)
		{
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer)))
			{
				return nullptr;
			}
		}
		else
		{
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
			{
				return nullptr;
			}
		}
		return pBuffer;
	}

	ID3D11InputLayout* XDxHelper::CreateInputlayout(ID3D11Device*  pd3dDevice, DWORD dwSize, LPCVOID lpData, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements)
	{
		HRESULT hr = S_OK;
		ID3D11InputLayout* pInutLayout = nullptr;
		if (FAILED(hr = pd3dDevice->CreateInputLayout(layout, numElements, lpData, dwSize, &pInutLayout)))
		{
			return nullptr;
		}
		return pInutLayout;
	}

	ID3D11VertexShader* XDxHelper::CreateVertexShader(TCHAR* szFileName, char* szFunctionName, ComPtr<ID3DBlob>* pVertexBuffer)
	{
		ID3D11VertexShader* pVertexShader;
		ID3DBlob* pErrorBlob = 0;
		hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szFunctionName, "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, NULL, pVertexBuffer->GetAddressOf(), &pErrorBlob, NULL);
		if (FAILED(hr))
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			return NULL;
		}
		hr = I_Device.m_pD3dDevice->CreateVertexShader((DWORD*)pVertexBuffer->Get()->GetBufferPointer(), pVertexBuffer->Get()->GetBufferSize(), NULL, &pVertexShader);
		if (FAILED(hr))
		{
			return NULL;
		}
		return pVertexShader;
	}

	ID3D11PixelShader* XDxHelper::CreatePixelShader(TCHAR* szFileName, char* szFunctionName, ComPtr<ID3DBlob>* pPixelBuffer)
	{
		ID3D11PixelShader* pPixelShader;
		ID3DBlob* pErrorBlob = 0;
		hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szFunctionName, "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, NULL, pPixelBuffer->GetAddressOf(), &pErrorBlob, NULL);
		if(FAILED(hr))
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			return NULL;
		}
		hr = I_Device.m_pD3dDevice->CreatePixelShader((DWORD*)pPixelBuffer->Get()->GetBufferPointer(), pPixelBuffer->Get()->GetBufferSize(), NULL, &pPixelShader);
		if (FAILED(hr))
		{

			return NULL;
		}
		return pPixelShader;
	}

	XDxHelper::XDxHelper()
	{
	}


	XDxHelper::~XDxHelper()
	{
	}
}