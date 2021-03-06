
// MapToolView.cpp: CMapToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MapTool.h"
#endif

#include "MapToolDoc.h"
#include "MapToolView.h"

// 폼 업데이트를 위해 넣음
#include  "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapToolView

IMPLEMENT_DYNCREATE(CMapToolView, CFormView)

BEGIN_MESSAGE_MAP(CMapToolView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMapToolView 생성/소멸

CMapToolView::CMapToolView()
	: CFormView(IDD_MAPTOOL_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMapToolView::~CMapToolView()
{
}

void CMapToolView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CMapToolView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

void CMapToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CMapToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMapToolView 진단

#ifdef _DEBUG
void CMapToolView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMapToolView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMapToolDoc* CMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
	return (CMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolView 메시지 처리기


void CMapToolView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	if (cx != 0 && cy != 0)
	{
		// 리사이즈 오류
		// 스왑체인 ResizeBuffers 함수에서 오류가 나오는데 모든 자원을 해제 하지 않았을 가능성, MFC를 사용하면서 핸들을 두개를 가르킬 가능성 확인 해야 함.
		//I_Device.ResizeDevice(cx, cy);
	}
}


void CMapToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!I_ObjectCtrl.m_bSelect)
		return;		// Select Mode가 아니면 반환
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	// Transform 갱신
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->GetObjCtrlPane()->m_ObjectForm->UpdateData(TRUE);
	D3DXVECTOR3 vScale, vLocation;
	D3DXQUATERNION qRotation;
	D3DXMatrixDecompose(&vScale, &qRotation, &vLocation, &pApp->m_Sample.GetSelectObjMat());
	pFrame->GetObjCtrlPane()->m_ObjectForm->m_fScaleX = vScale.x;
	pFrame->GetObjCtrlPane()->m_ObjectForm->m_fScaleY = vScale.y;
	pFrame->GetObjCtrlPane()->m_ObjectForm->m_fScaleZ = vScale.z;
	pFrame->GetObjCtrlPane()->m_ObjectForm->m_fRotationX = D3DXToDegree(qRotation.x);
	pFrame->GetObjCtrlPane()->m_ObjectForm->m_fRotationY = D3DXToDegree(qRotation.y);
	pFrame->GetObjCtrlPane()->m_ObjectForm->m_fRotationZ = D3DXToDegree(qRotation.z);
	pFrame->GetObjCtrlPane()->m_ObjectForm->m_fLocationX = vLocation.x;
	pFrame->GetObjCtrlPane()->m_ObjectForm->m_fLocationY = vLocation.y;
	pFrame->GetObjCtrlPane()->m_ObjectForm->m_fLocationZ = vLocation.z;
	pFrame->GetObjCtrlPane()->m_ObjectForm->UpdateData(FALSE);



	CFormView::OnLButtonUp(nFlags, point);
}
