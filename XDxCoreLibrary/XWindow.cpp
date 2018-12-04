#include "XWindow.h"

XWindow* g_pWindow = NULL;
HWND g_hWnd;
HINSTANCE g_hinstance;
RECT g_rtClient;

bool XWindow::Init() { return true; }
bool XWindow::Frame() { return true; }
bool XWindow::Render() { return true; }
bool XWindow::Release() { return true; }

LRESULT CALLBACK XWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (g_pWindow->MsgProc(hWnd, message, wParam, lParam) > 0)	return 0;

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(g_hWnd, &ps);
		EndPaint(g_hWnd,&ps);
	}
	case WM_SIZE:
	{
		if (wParam != SIZE_MINIMIZED && g_pWindow != NULL)
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			g_pWindow->ResizeClient(width, height);

			return 0;
		}
	}break;
	case WM_DESTROY: 
	{
		PostQuitMessage(0);
		return 0;
	}
	default: 
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}

	return 0;
}

LRESULT XWindow::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void XWindow::ResizeClient(UINT iWidth, UINT iHeight)
{
	GetClientRect(m_hWnd, &m_rtClient);
	g_rtClient = m_rtClient;
}

ATOM XWindow::MyRegisterClass() 
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = (sizeof(WNDCLASSEX));
	wc.style = (CS_HREDRAW | CS_VREDRAW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hInstance = m_hinstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"shkuk";
	return RegisterClassEx(&wc);
}

bool XWindow::SetWindows(HINSTANCE hinstance, TCHAR* pClientName, int iWidth, int iHeight, DWORD dwStyle)
{
	m_hinstance = hinstance;
	g_hinstance = m_hinstance;
	// 운영체제에 객체 등록
	MyRegisterClass();

	RECT rt = { 0,0,iWidth, iHeight };
	//--------------- 원래 윈도우 사이즈를 가져오는 함수 --------------------------//
	AdjustWindowRect(&rt, dwStyle, FALSE);
	// 윈도우 생성  
	m_hWnd = CreateWindow(L"shkuk", pClientName, dwStyle, 0, 0, rt.right - rt.left, rt.bottom - rt.top, NULL, NULL, m_hinstance, NULL);
	if (m_hWnd == NULL)	return false;
	g_hWnd = m_hWnd;
	GetClientRect(m_hWnd, &m_rtClient);
	g_rtClient = m_rtClient;
	CenterWindow();
	ShowWindow(m_hWnd, SW_SHOWNORMAL);

	return true;
}

void XWindow::CenterWindow()
{
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	m_iWindowPosX = (iScreenWidth - (m_rtClient.right - m_rtClient.left)) / 2;
	m_iWindowPosY = (iScreenHeight - (m_rtClient.bottom - m_rtClient.top)) / 2;

	RECT rt = { 0,0,m_rtClient.right, m_rtClient.bottom };
	//--------------- 원래 윈도우 사이즈를 가져오는 함수 --------------------------//
	AdjustWindowRect(&rt, GetWindowStyle(m_hWnd), FALSE);

	MoveWindow(m_hWnd, m_iWindowPosX, m_iWindowPosY, m_rtClient.right - m_rtClient.left, m_rtClient.bottom - m_rtClient.top, true);
}

bool XWindow::MsgRun() 
{
	MSG message;
	ZeroMemory(&message, sizeof(MSG));
	if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
		if (message.message == WM_QUIT) 
		{
			return false;
		}
	}
	return true;
}

XWindow::XWindow()
{
	m_iWindowPosX = m_iWindowPosY = 0;
	g_pWindow = this;
}

XWindow::~XWindow()
{
}
