#pragma once
#include "XMapSpreatForm.h"

// XMapSpreatPane

class XMapSpreatPane : public CDockablePane
{
	DECLARE_DYNAMIC(XMapSpreatPane)
	//사용자 정의 변수
private:
	XMapSpreatForm* m_SpreatForm;
public:
	XMapSpreatPane();
	virtual ~XMapSpreatPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


