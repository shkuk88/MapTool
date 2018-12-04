#include "XTimer.h"
float g_fSecPerFrame = 0.0f;
float g_fTime = 0.0f;

bool XTimer::Init() 
{
	m_dwBeforeTime = timeGetTime();
	ZeroMemory(m_szBuffer, sizeof(TCHAR) * 256);

	//------------------ Font 셋팅 ----------------------------------// 손봐야함
	//SetTextColor(g_hOffScreenDC, RGB(256, 0, 0));
	//m_font = CreateFont(15, 0, 0, 0, FW_HEAVY, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, L"샘물");
	//SelectObject(g_hOffScreenDC, m_font);

	// 배경색상을 사용하되, 투명(TRANSPARENT)한 색상으로 한다. 폰트 외의 색상을 지워줌.
	//SetBkMode(g_hOffScreenDC, TRANSPARENT);
		
	return true;
}

bool XTimer::Frame() 
{
	DWORD dwCurrentTick = timeGetTime();
	DWORD dwTick = dwCurrentTick - m_dwBeforeTime;
	m_fTime += dwTick / 1000.0f;
	g_fTime = m_fTime;
	m_fSecPerFrame = dwTick / 1000.0f;
	m_fCurrentTime += m_fSecPerFrame;
	g_fSecPerFrame = m_fSecPerFrame;

	if (m_fCurrentTime >= 1.0f)
	{
		m_dwFramePerSecond = m_dwElapseFPS;
		m_fCurrentTime = 0.0f;
		m_dwElapseFPS = 0;
	}
	m_dwElapseFPS++;
	m_dwBeforeTime = dwCurrentTick;
	return true;
}
bool XTimer::Render() 
{
	_stprintf_s(m_szBuffer, L"Timer:%10.4f FPS: %d SPF: %10.4f", m_fTime, m_dwFramePerSecond, m_fSecPerFrame);
	//TextOut(g_hOffScreenDC, 0, 0, m_szBuffer, wcslen(m_szBuffer));
	//OutputDebugString(m_szBuffer);
	return true;
}

bool XTimer::Release() 
{
	DeleteObject(m_font);
	return true;
}

XTimer::XTimer() 
{
	m_dwFramePerSecond = m_dwElapseFPS = 0;
	m_fTime = m_fSecPerFrame = m_fCurrentTime = 0.0f;
	m_dwBeforeTime = timeGetTime();
}

XTimer::~XTimer() 
{
}
