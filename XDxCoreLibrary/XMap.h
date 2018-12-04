#pragma once
#include "XShape.h"


struct ConstantBuffer_Light
{
	D3DXMATRIX	matInvWorld;		// ����� �� ����ϴ��� �ǹ�
	D3DXVECTOR4	vDiffuseMaterial;
	D3DXVECTOR4	vAmbientMaterial;	// ��ο� �κ��� �⺻������ ��� �ϵ��� �ִ� ��
	D3DXVECTOR4 vSpecularMaterial;	// �⺻ ����ŧ���� ��
	D3DXVECTOR4	vDiffuseLightColor;
	D3DXVECTOR4	vAmbientLightColor;	
	D3DXVECTOR4 vSpecularLightColor;// ����ŧ�� ����Ʈ�� ��
	D3DXVECTOR4 vLightDirection;	// �𷺼� ����Ʈ, w�� ����(�ϴ��� ������� ����)
	D3DXVECTOR4 vSightDirection;	// �ü� ����, w�� ����

};

struct XVertexLookUp
{
	set<DWORD> vListNormal;	// �����Ǵ� ������ ��ġ�� �ȵǱ� ������ set����
};

class XMap :public XShape
{
public:
	DX::XDxHelper			m_Object;
	vector<float>			m_fHeightList;
	vector<D3DXVECTOR3>		m_vFaceNormals;
	vector<long>			m_iNormalLookUpTable; //���� ������ �����ϴ� ���̽�, DWORD�� XVertexLookUp�� �ֵ��� ������ ����
	int						m_iRow;
	int						m_iCol;
	int						m_iSellRows;
	int						m_iSellCols;
	int						m_iVertexs;
	int						m_iFace;
	float					m_fDistance;
	float					m_fScaleHeight;
public:
	D3DXVECTOR3				m_vLook;
	ComPtr<ID3D11Buffer>	m_pLightConstantBuffer;
	ConstantBuffer_Light	m_LightData;
public:
	bool		CreateVertexList();	// Vertex,Index,Normal,Color ���� �� �󿡼� ������ �ϰ� ���Ϸ� �ҷ����°� �´�.
	bool		CreateIndexList();
	HRESULT		CreateHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TCHAR* HeightMap);
	float		GetHeight(float x, float z);
	float		GetHeightmap(int row, int col);
	float		Lerp(float fStart, float fEnd, float fTangent);
	bool		InitFaceNormals();				//���̽����� �븻���� ����ؼ� ����
	D3DXVECTOR3 ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2);
	bool		GetNormalLookUpTable();			//������ ���� FACE���� ���
	bool		CalcPerVertexNormalsFastLookUp();
	void		InitConstant();		// shader�� �ѱ� light vector�� ���Ƿ� ������ۿ� ����
	void		SetScaleHeight(float fScaleHeight = 1.0f);
public:
	void		SetLookVector(D3DXVECTOR3 vLook);
	void		InitLight();
	void		UpdateLight();
public:
	virtual bool Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TCHAR* szTexture, TCHAR* szVSfile, char* szVSFunctionName, TCHAR* szPSfile, char* szPSFunctionName, TCHAR* szHeightTexture);
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool Release();
public:
	XMap();
	virtual~XMap();
};
