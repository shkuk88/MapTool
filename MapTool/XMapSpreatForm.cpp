// XMapSpreatForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "XMapSpreatForm.h"
#include "afxdialogex.h"


// XMapSpreatForm 대화 상자

IMPLEMENT_DYNAMIC(XMapSpreatForm, CDialogEx)

XMapSpreatForm::XMapSpreatForm(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAP_SPREAT, pParent)
{

}

XMapSpreatForm::~XMapSpreatForm()
{
}

void XMapSpreatForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(XMapSpreatForm, CDialogEx)
END_MESSAGE_MAP()


// XMapSpreatForm 메시지 처리기
