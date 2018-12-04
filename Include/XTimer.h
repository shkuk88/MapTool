#pragma once
#include "XStd.h"
//#include <time.h> //winmm.lib ÇÊ¿ä

class XTimer :public XBase, public XSingleton<XTimer>
{
public:
	friend class XSingleton<XTimer>;
public:
	HDC	  m_hdc;
	TCHAR m_szBuffer[256];
	DWORD m_dwFramePerSecond;
	DWORD m_dwElapseFPS;
	DWORD m_dwBeforeTime;
	HFONT m_font;
	float m_fTime;
	float m_fSecPerFrame;
	float m_fCurrentTime;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
private:
	XTimer();
public:
	virtual ~XTimer();
};

#define I_Timer XTimer::Get()