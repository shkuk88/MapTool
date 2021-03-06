#pragma once
#include "XMapCreateForm.h"

// XMapCreatePane

class XMapCreatePane : public CDockablePane
{
	DECLARE_DYNAMIC(XMapCreatePane)
//사용자 정의 변수
private:
	XMapCreateForm* m_MapForm;

public:
	XMapCreatePane();
	virtual ~XMapCreatePane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


