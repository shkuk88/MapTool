#pragma once


// XMapSpreatForm 대화 상자

class XMapSpreatForm : public CDialogEx
{
	DECLARE_DYNAMIC(XMapSpreatForm)

public:
	XMapSpreatForm(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~XMapSpreatForm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAP_SPREAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
