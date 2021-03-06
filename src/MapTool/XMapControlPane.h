#pragma once
#include "XMapControlForm.h"

// XMapControlPane

class XMapControlPane : public CDockablePane
{
	DECLARE_DYNAMIC(XMapControlPane)
//사용자 정의 변수
private:
	XMapControlForm * m_MapControlForm;
public:
	XMapControlPane();
	virtual ~XMapControlPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


