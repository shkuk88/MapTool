#pragma once
#include "XStd.h"

class XDxWrite:public XSingleton<XDxWrite>
{
public:
	friend class XSingleton<XDxWrite>;
public:
	//d2d
	ComPtr<ID2D1RenderTarget>		m_pRT;   // <<�񵶸���,���������� �����ؾ���
	////////////����̽� �������̴�//////////�ٸ��ſ� ������ �ȹ���
	ComPtr<ID2D1Factory>			m_pD2DFactory;		//2d ���ҽ��� ���� �� ����ϱ� ���� �ʿ��� ��ü
	//dwrite
	ComPtr<IDWriteFactory>			m_pDWriteFactory;	// �ؽ�Ʈ�� ������ϱ� ���� �ݵ�� �ʿ��� �������̽�
	ComPtr<IDWriteTextFormat>		m_pTextFormat;		// direct�� write�� ��Ʈ�� �÷�����, �÷��� ��� ��ȯ ����
	ComPtr<ID2D1SolidColorBrush>	m_pSolidBrush;		    //�������ε� m_pRT�� �ʿ��ϹǷ� CreateDeviceResourcesdp���� ����  m_pRT->CreateSolidColorBrush
	float							m_fDpiX;
	float							m_fDpiY;
public:
	bool	Begin();
	bool	Draw(const D2D1_RECT_F rtPlace, TCHAR* szData);
	bool	End();
	bool	Resize() { return true; }
	bool	Release();
	bool	Set(IDXGISurface1* pSurface);
	HRESULT CreateDeviceIndependentResources();						//������
	HRESULT CreateDeviceResources(IDXGISurface1* pSurface);			//�񵶸���
private:
	XDxWrite();
public:
	virtual ~XDxWrite();
};

#define I_Write XSingleton<XDxWrite>::Get()

