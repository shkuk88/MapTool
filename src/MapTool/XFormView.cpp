// XFormView.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "XFormView.h"


// XFormView

IMPLEMENT_DYNCREATE(XFormView, CView)

XFormView::XFormView()
{

}

XFormView::~XFormView()
{
}

BEGIN_MESSAGE_MAP(XFormView, CView)
END_MESSAGE_MAP()


// XFormView 그리기

void XFormView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// XFormView 진단

#ifdef _DEBUG
void XFormView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void XFormView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// XFormView 메시지 처리기
