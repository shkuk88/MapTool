#pragma once
#include "XCore.h"
#include "XDevice.h"
#include "XCamera.h"
#include "XHeightMapController.h"
#include "XSkyBox.h"


class XSample :public XCore
{
private:
	XCamera			m_Camera;
	XMap*			m_pMap;
	XQuadTreeIndex*	m_pMapTree;
	XHeightMapController m_HeightMapCtrl;
	XSkyBox			m_Sky;
public:
	bool CreateMap(TCHAR* szTexture, TCHAR* szHeightTexture, float fCellCount, float fDistance);
	XHeightMapController* GetHeightCtrl(){ return &m_HeightMapCtrl; }
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	XSample();
	virtual ~XSample();
};

