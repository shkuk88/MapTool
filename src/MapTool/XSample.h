#pragma once
#include "XCore.h"
#include "XDevice.h"
#include "XCamera.h"
#include "XHeightMapController.h"
#include "XSpreatController.h"
#include "XSkyBox.h"
#include "XMapExporter.h"
#include "XMapImporter.h"
#include "XObjectController.h"

class XSample :public XCore
{
private:
	XCamera			m_Camera;
	XMap*			m_pMap;
	XQuadTreeIndex*	m_pMapTree;
	XHeightMapController m_HeightMapCtrl;
	XSpreatController	m_SpreatCtrl;
	XObjectController m_ObjectCtrl;
	XSkyBox			m_Sky;
	XMapExporter	m_Exporter;
	XMapImporter	m_Importer;
public:
	bool CreateMap(TCHAR* szTexture, TCHAR* szHeightTexture, float fCellCount, float fDistance);
	bool CreateMap();
	XHeightMapController* GetHeightCtrl(){ return &m_HeightMapCtrl; }
	XSpreatController* GetSpreatCtrl() { return &m_SpreatCtrl; }
	XMapExporter* GetExporter() { return &m_Exporter; }
	XMapImporter* GetImporter() { return &m_Importer; }
	XMap* GetMap() { return m_pMap; }
	XObjectController* GetObjCtrl() { return &m_ObjectCtrl; }
	D3DXMATRIX GetSelectObjMat();
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	XSample();
	virtual ~XSample();
};

