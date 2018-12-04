#pragma once
#include "XSprite.h"

class XSpriteManager :public XSingleton <XSpriteManager>
{
public:
	friend class XSingleton<XSpriteManager>;
public:
	int			m_Iindex;
	map<int, XSprite*> m_spriteList;
	typedef	map<int, XSprite*>::iterator m_iter;
public:
	int			Add(TString szFileName);
	XSprite*	Find(int iSpriteKey);
	void		Clear();
private:
	XSpriteManager();
public:
	virtual~XSpriteManager();
};

#define I_Sprite XSpriteManager::Get()