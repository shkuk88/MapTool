// XMapPane.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "XMapCreatePane.h"


// XMapCreatePane

IMPLEMENT_DYNAMIC(XMapCreatePane, CDockablePane)

XMapCreatePane::XMapCreatePane()
{

}

XMapCreatePane::~XMapCreatePane()
{
}


BEGIN_MESSAGE_MAP(XMapCreatePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// XMapCreatePane 메시지 처리기




int XMapCreatePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_MapForm = XMapCreateForm::CreateOne(this); // 폼 생성

	return 0;
}


void XMapCreatePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_MapForm)
	{
		// cx,cy 윈도우 사이즈를 따라 페인 윈도우도 리사이즈
		m_MapForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER); // SWP_NOZORDER Z값을 따라가지 않게 해줌
	}
}


int XMapCreatePane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFrameWnd* pParentFrame = GetParentFrame(); // 부모 윈도우를 따라서 선택하게 됨
	if (pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame)) // 이상태면 정상적으로 처리
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE; // 아니라면 MA_NOACTIVATE
}
