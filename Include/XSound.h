#pragma once
#include "XStd.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

#if defined(_WIN64)
#pragma comment(lib, "fmod64_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif

#define MAX_SOUND 32

using namespace FMOD;

class XSound :public XSingleton<XSound>, public XBase
{
public:
	friend class XSingleton<XSound>;
public:
	FMOD::System*	m_pSystem;
	FMOD::Sound*	m_pSound[MAX_SOUND];
	FMOD::Channel*	m_pChannel[MAX_SOUND];
public:
	int	m_iSound;
public:
	int Load(const char* FileName, bool bLoop = false);
	void Play(int iIndex, bool bEffect = false);
	void Stop(int iIndex);
	void Paused(int iIndex);
	void Volume(int iIndex, float fVolume);
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
private:
	XSound();
public:
	virtual ~XSound();
};

#define I_Sound XSound::Get()
