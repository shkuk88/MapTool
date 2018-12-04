#pragma once
#include "XStd.h"

class XWindow :public XBase
{
public:
	HWND m_hWnd;
	RECT m_rtClient;
	HINSTANCE m_hinstance;
	int m_iWindowPosX;
	int m_iWindowPosY;
public:
	// �ü���� ���� �޼����� �޴� �Լ�
	// �޼����� �޼��� ť�� ����ǰ� 
	// �ʿ�� �ش� ������ ť���� �����´�.
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void ResizeClient(UINT iWidth, UINT iHeight);
	//1. ������ Ŭ���� ��ü ���(�ü��)
	ATOM MyRegisterClass();
	//2, ��ϵ� ��ü�� ����Ͽ� ������ ����
	bool SetWindows(HINSTANCE hinstance, TCHAR* pClientName, int iWidth, int iHeight, DWORD dwStyle);
	bool MsgRun();
	void CenterWindow();
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
public:
	XWindow();
	virtual ~XWindow();
};

