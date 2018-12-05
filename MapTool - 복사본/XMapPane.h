#pragma once
#include "XMapForm.h"

// XMapPane

class XMapPane : public CDockablePane
{
	DECLARE_DYNAMIC(XMapPane)
//사용자 정의 변수
private:
	XMapForm* m_MapForm;

public:
	XMapPane();
	virtual ~XMapPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


