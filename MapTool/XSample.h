#pragma once
#include "XCore.h"
#include "XDevice.h"
#include "XCamera.h"
#include "XHeightMapController.h"
#include "XSpreatController.h"
#include "XSkyBox.h"


class XSample :public XCore
{
private:
	XCamera			m_Camera;
	XMap*			m_pMap;
	XQuadTreeIndex*	m_pMapTree;
	XHeightMapController m_HeightMapCtrl;
	XSpreatController	m_SpreatCtrl;
	XSkyBox			m_Sky;
public:
	bool CreateMap(TCHAR* szTexture, TCHAR* szHeightTexture, float fCellCount, float fDistance);
	XHeightMapController* GetHeightCtrl(){ return &m_HeightMapCtrl; }
	XSpreatController* GetSpreatCtrl() { return &m_SpreatCtrl; }
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	XSample();
	virtual ~XSample();
};

