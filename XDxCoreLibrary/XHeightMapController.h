#pragma once
#include "XMapController.h"

enum HeightControl { None = 0, Up, Down, Flat };
class XHeightMapController :public XMapController
{
private:
	// Mode
	BOOL m_bHeightCtrlState = None;
	// 높이값을 조절할 강도
	float m_fStrength = 0.0f;
public:
	void SetHeightCtrlState(HeightControl HeightCtrlState);
	void SetControlValue(float fRadius, float fStrength);
	void HeightUpDown();
	void HeightFlat();
public:
	virtual bool Init() { return true; } 
	virtual bool Frame()	override;
	virtual bool Render() { return true; }
	virtual bool Release()	override;
public:
	XHeightMapController();
	virtual ~XHeightMapController();
};

