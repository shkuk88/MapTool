#pragma once


// XMapSpreatForm 대화 상자

class XMapSpreatForm : public CFormView
{
	DECLARE_DYNAMIC(XMapSpreatForm)

public:
	static XMapSpreatForm* CreateOne(CWnd* pParent);
public:
	XMapSpreatForm();   // 표준 생성자입니다.
	virtual ~XMapSpreatForm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAP_SPREAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bSpreatSwitch;
	afx_msg void OnBnClickedAlpha();
	afx_msg void OnBnClickedTexture();
	afx_msg void OnBnClickedSpreatSwitch();
	float m_fRadius;
	afx_msg void OnBnClickedRadiusApply();
	afx_msg void OnBnClickedSpreatColor0();
	afx_msg void OnBnClickedSpreatColor1();
	afx_msg void OnBnClickedSpreatColor2();
	afx_msg void OnBnClickedSpreatColor3();
};
