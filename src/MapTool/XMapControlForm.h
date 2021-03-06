#pragma once


// XMapControlForm 대화 상자

class XMapControlForm : public CFormView
{
	DECLARE_DYNAMIC(XMapControlForm)

public:
	XMapControlForm();   // 표준 생성자입니다.
	virtual ~XMapControlForm();

// 사용자 정의 변수
private:
	BOOL m_bHeightCtrlState;
// 사용자 정의 함수
public:
	static XMapControlForm* CreateOne(CWnd* pParent); // 하나만 만들도록 한다.
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAP_CONTROL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUpVector();
	afx_msg void OnBnClickedDownVector();
	afx_msg void OnBnClickedFlatVector();
	afx_msg void OnBnClickedControlApply();
	afx_msg void OnBnClickedControlOff();
	float m_fRadius;
	float m_fStrength;
};
