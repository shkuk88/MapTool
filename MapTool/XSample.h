#pragma once
#include "XCore.h"
#include "XDevice.h"
#include "XCamera.h"
#include "XHeightMapController.h"
#include "XSkyBox.h"


//class XObject : public XBoxShape
//{
//public:
//	D3DXVECTOR3 m_vBefore;
//
//public:
//	void MoveObject()
//	{
//		if (I_Input.m_DIMouseState.rgbButtons[0])
//		{
//			if (I_Select->CheakOBBToRay(&m_Box))
//			{
//				I_Select->CheakOBBToRay(&m_Box);
//				D3DXVECTOR3 vCurrent = I_Select->m_vIntersection;
//				D3DXVECTOR3 vLength = vCurrent - m_vBefore;
//				D3DXVec3Normalize(&vLength, &vLength);
//				m_matWorld._41 += vLength.x*g_fSecPerFrame*10;
//				m_matWorld._43 += vLength.z*g_fSecPerFrame*10;
//			}
//		}
//		m_vBefore = I_Select->m_vIntersection;
//	}
//	virtual bool Init() {
//		SetVertex();
//		m_pVertexBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_VertexList.at(0), m_VertexList.size(), sizeof(PNCT_Vertex), D3D11_BIND_VERTEX_BUFFER));
//		SetIndex();
//		m_pIndexBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_dwIndexList.at(0), m_dwIndexList.size(), sizeof(DWORD), D3D11_BIND_INDEX_BUFFER));
//		m_pConstantBuffer.Attach(m_Object.CreateBuffer(I_Device.m_pD3dDevice.Get(), &m_ConstantDataMatrix, 1, sizeof(ConstantBuffer_Matrix), D3D11_BIND_CONSTANT_BUFFER));
//
//		return true;
//	}
//	virtual bool Frame() {
//		m_Box.vCenter.x = m_matWorld._41;
//		m_Box.vCenter.y = m_matWorld._42;
//		m_Box.vCenter.z = m_matWorld._43; 
//		return true; }
//	virtual bool Render(ID3D11DeviceContext* pContext) {
//		pContext->VSSetShader(m_pVS.Get(), NULL, 0);
//		pContext->PSSetShader(m_pPS.Get(), NULL, 0);
//
//		pContext->IASetInputLayout(m_pVertexLayout.Get());
//
//		UINT stride = sizeof(PNCT_Vertex);
//		UINT offset = 0;
//
//		pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
//		pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
//
//		pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, NULL, &m_ConstantDataMatrix, NULL, NULL);
//		pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
//
//		//pContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
//
//		pContext->DrawIndexed(m_dwIndexList.size(), 0, 0);
//		return true;
//	}
//	virtual bool Release() { return true; }
//public:
//	XObject() {
//		m_vBefore = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		m_Box.vAxis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
//		m_Box.vAxis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//		m_Box.vAxis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//	}
//	~XObject() {}
//};

class XSample :public XCore
{
private:
	XCamera			m_Camera;
	XMap*			m_pMap;
	XQuadTreeIndex*	m_pMapTree;
	XHeightMapController m_HeightMapCtrl;
	XSkyBox			m_Sky;
public:
	bool CreateMap(TCHAR* szTexture, TCHAR* szHeightTexture, float fCellCount, float fDistance);
	XHeightMapController* GetHeightCtrl(){ return &m_HeightMapCtrl; }
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	XSample();
	virtual ~XSample();
};

