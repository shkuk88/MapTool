#pragma once
#include"XStd.h"

enum KeyState { KEY_FREE, KEY_PUSH, KEY_HOLD, KEY_UP };
enum MouseKey { LBUTTON, RBUTTON };
class XInput :public XBase, public XSingleton<XInput>
{
public:
	friend class XSingleton<XInput>;
public:
	DWORD		m_dwKeyState[256];

public:
	DWORD		m_dwMouseState[2];
	POINT		m_mousePosition;
public:
	DWORD		KeyCheck(DWORD key);
	DWORD		MouseCheck(MouseKey key, DWORD state);
public:
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
private:
	XInput();
public:
	virtual ~XInput();
};

#define I_Input XInput::Get()
