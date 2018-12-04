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
	// 운영체제로 부터 메세지를 받는 함수
	// 메세지는 메세지 큐에 저장되고 
	// 필요시 해당 윈도가 큐에서 꺼내온다.
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void ResizeClient(UINT iWidth, UINT iHeight);
	//1. 윈도우 클래스 객체 등록(운영체제)
	ATOM MyRegisterClass();
	//2, 등록된 객체를 사용하여 윈도우 생성
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

