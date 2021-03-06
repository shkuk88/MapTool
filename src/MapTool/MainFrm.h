
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once

#include "XMapCreatePane.h"
#include "XMapControlPane.h"
#include "XMapSpreatPane.h"
#include "XObjectControlPane.h"
#include "XColliderControlPane.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 사용자 정의 변수
private:
	CTabbedPane		pTab;			// 탭에 다른 페인을 붙이기 위해 생성
	XMapCreatePane	m_MapCreate;	// Create Map
	XMapControlPane m_MapControl;	// Map Height Control
	XMapSpreatPane	m_MapSpreat;	// Map Texture Control
	XObjectControlPane m_MapObject; // Map Object Control
	XColliderControlPane m_ColliderCtrl; // Map Object Collider Control
// 사용자 정의 함수
public:
	void CreatePaneWindow(CDockablePane* pDockablePane, TCHAR* szTitle, DWORD id);
	XObjectControlPane* GetObjCtrlPane() { return &m_MapObject; }
// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

};


