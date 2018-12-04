#pragma once
#include "XStd.h"

class XHDC_Manager :public XSingleton<XHDC_Manager>
{
public:
	friend class XSingleton<XHDC_Manager>;
public:
	HDC		m_hMainDC;
	HDC		m_hOffScreenDC;
	HBITMAP	m_hOffScreenBitmap;
	HBITMAP	m_hOldBitmap;
	HBRUSH	m_hBrBack;
	HBRUSH	m_hOldBrush;
public:
	void CreateHDC(HWND hWnd);
	void PreRenderHDC();
	void PostRenderHDC();
	void ReleaseHDC();
private:
	XHDC_Manager();
public:
	virtual~XHDC_Manager();
};

#define I_HDC XHDC_Manager::Get()
