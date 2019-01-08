#pragma once
#include "XColliderControlForm.h"

// XColliderControlPane

class XColliderControlPane : public CDockablePane
{
	DECLARE_DYNAMIC(XColliderControlPane)
// 사용자 정의 변수
private:
	XColliderControlForm* m_ColliderCtrlForm;
public:
	XColliderControlPane();
	virtual ~XColliderControlPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


