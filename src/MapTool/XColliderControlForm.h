#pragma once


// XColliderControlForm 대화 상자

class XColliderControlForm : public CFormView
{
	DECLARE_DYNAMIC(XColliderControlForm)
	// 사용자 정의 함수
public:
	static XColliderControlForm* CreateOne(CWnd* pParent); // 하나만 만들도록 한다.
public:
	XColliderControlForm();   // 표준 생성자입니다.
	virtual ~XColliderControlForm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDER_CONTROL};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
