#include "XHeightMapController.h"



void XHeightMapController::SetHeightCtrlState(HeightControl HeightCtrlState)
{
	Start();
	m_bHeightCtrlState = HeightCtrlState;
}

void XHeightMapController::SetControlValue(float fRadius, float fStrength)
{
	m_fRadius = fRadius;
	m_fStrength = fStrength;
}


bool XHeightMapController::Frame()
{
	if (!bStart) return false;
	if (I_Input.m_MouseState[0])
	{
		CheckCrashNode();
	}
	return true;
}

bool XHeightMapController::Release()
{
	if (!bStart) return false;
	// �����ͱ� ������ clear �۾��� ���ش�.
	m_LeafNodeList.clear();
	return true;
}

XHeightMapController::XHeightMapController()
{
}


XHeightMapController::~XHeightMapController()
{
}
