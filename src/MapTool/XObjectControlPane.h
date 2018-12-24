#pragma once
#include "XObjectControlForm.h"

// XObjectControlPane

class XObjectControlPane : public CDockablePane
{
	DECLARE_DYNAMIC(XObjectControlPane)
// 사용자 정의 변수
public:
	XObjectControlForm* m_ObjectForm;
public:
	XObjectControlPane();
	virtual ~XObjectControlPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


