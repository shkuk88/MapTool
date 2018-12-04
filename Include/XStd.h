#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <windowsx.h>
#include <time.h>
#include <map>
#include <list>
#include <vector>
#include <memory>
#include <process.h> // _beginthreadex()의 헤더
#include <crtdbg.h> // 메모리 누수를 잡기위한 헤더
// Dx
#include <d3d11.h>
#include <dxgi.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dinput.h>
#include <dwrite.h>
/////////다이렉트 컴파일러헤더///////////
#include <D3DX11.h>
#include <D3DX10math.h>
#include <d3dcompiler.h>
#include <wrl.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "XDxCore.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

using namespace Microsoft::WRL;
using namespace std;

//#define CRTDBG_MAP_ALLOC
//#define new new(_CLIENT_BLOCK, __FILE__,__LINE__)

class XBase
{
public:
	virtual bool Init() = 0;
	virtual bool Frame() = 0;
	virtual bool Render() = 0;
	virtual bool Release() = 0;
public:
	XBase() {}
	virtual ~XBase() {}
};

template<class X>
class XSingleton
{
public:
	static X& Get()
	{
		static X instance;
		return instance;
	}

};



typedef basic_string<TCHAR> TString;
extern HWND			g_hWnd;
extern HINSTANCE	g_hinstance;
extern RECT			g_rtClient;
extern float		g_fSecPerFrame;
extern float		g_fTime;
extern HRESULT		hr;

#include "XTimer.h"

#ifndef V
#define V(x)	{hr = (x);}
#endif

#ifndef V_RETURN
#define V_RETURN(x)	{hr = (x); if(FAILED(hr)) return hr; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(x, y)				{ if (!x) x = new y; }
#endif

#ifndef SAFE_DEL
#define SAFE_DEL(x)					{ if (x) delete x; (x)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)				{ if(x) { (x)->Release(); (x)=NULL; } }
#endif 

class XPosition
{
public:
	float x,y;
public:
	XPosition& operator =(const XPosition& pos)
	{
		x = pos.x;
		y = pos.y;
		return *this;
	}
public:
	XPosition() { x = y = 0.0f; }
	XPosition(float fX, float fY = 0.0f)
	{
		x = fX;
		y = fY;
	}
	~XPosition() {}
};

class XRectangle
{
public:
	float left, top, right, bottom;
public:
	XRectangle& operator =(const XRectangle& rt)
	{
		left = rt.left;
		top = rt.top;
		right = rt.right;
		bottom = rt.bottom;
		return *this;
	}
public:
	XRectangle() { left = top = right = bottom = 0.0f; }
	XRectangle(float fLeft, float fTop = 0.0f, float fRight = 0.0f, float fBottom = 0.0f)
	{
		left = fLeft;
		top = fTop;
		right = fRight;
		bottom = fBottom;
	}
	~XRectangle() {}
};

#define WINSTART int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){
#define WINEND return 1;}