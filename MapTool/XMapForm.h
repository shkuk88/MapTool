#pragma once


// XMapForm 대화 상자
// CFormView가 클래스 생성에 나오지 않기 때문에 CDialogEX로 만든 후 상속을 CFormView로 바꾼다
// CDialogEX 만들기전에 Dialog 하나를 만들고 속성에서 title을 false로 하고 style을 child로 한다.

class XMapForm : public CFormView
{
	DECLARE_DYNAMIC(XMapForm)

//사용자 정의 변수
private:


// 사용자 정의 함수
public:
	static XMapForm* CreateOne(CWnd* pParent); // 하나만 만들도록 한다.

public:
	XMapForm();   // 표준 생성자입니다.
	virtual ~XMapForm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XMapForm };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
