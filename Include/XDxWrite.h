#pragma once
#include "XStd.h"

class XDxWrite:public XSingleton<XDxWrite>
{
public:
	friend class XSingleton<XDxWrite>;
public:
	//d2d
	ID2D1RenderTarget * m_pRT;   // <<�񵶸���,���������� �����ؾ���
								 ////////////����̽� �������̴�//////////�ٸ��ſ� ������ �ȹ���
	ID2D1Factory*			m_pD2DFactory;		//2d ���ҽ��� ���� �� ����ϱ� ���� �ʿ��� ��ü
												//dwrite
	IDWriteFactory*			m_pDWriteFactory;	// �ؽ�Ʈ�� ������ϱ� ���� �ݵ�� �ʿ��� �������̽�
	IDWriteTextFormat*		m_pTextFormat;		// direct�� write�� ��Ʈ�� �÷�����, �÷��� ��� ��ȯ ����
	ID2D1SolidColorBrush*	m_pSolidBrush;		    //�������ε� m_pRT�� �ʿ��ϹǷ� CreateDeviceResourcesdp���� ����  m_pRT->CreateSolidColorBrush
	float					m_fDpiX;
	float					m_fDpiY;
public:
	bool Release();
	bool Set(IDXGISurface1* pSurface);
	HRESULT CreateDeviceIndependentResources();						//������
	HRESULT CreateDeviceResources(IDXGISurface1* pSurface);			//�񵶸���
private:
	XDxWrite();
public:
	virtual ~XDxWrite();
};

#define I_Write XSingleton<XDxWrite>::Get()

