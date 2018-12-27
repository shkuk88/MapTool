#pragma once


// XObjectControlForm 대화 상자

class XObjectControlForm : public CFormView
{
	DECLARE_DYNAMIC(XObjectControlForm)

//사용자 정의 변수
private:
	vector<CString> m_szObjectPath;
	int m_iSelectObject;
	
// 사용자 정의 함수
public:
	static XObjectControlForm* CreateOne(CWnd* pParent);
public:
	XObjectControlForm();   // 표준 생성자입니다.
	virtual ~XObjectControlForm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECT_FORM};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddObject();
	afx_msg void OnBnClickedDelObject();
	CListBox m_ObjectList;
	afx_msg void OnBnClickedCreateObject();
	afx_msg void OnEnChangeLocationX();
	afx_msg void OnEnChangeLocationY();
	afx_msg void OnEnChangeLocationZ();
	afx_msg void OnEnChangeRotationX();
	afx_msg void OnEnChangeRotationY();
	afx_msg void OnEnChangeRotationZ();
	afx_msg void OnEnChangeScaleX();
	afx_msg void OnEnChangeScaleY();
	afx_msg void OnEnChangeScaleZ();
	float m_fLocationX;
	float m_fLocationY;
	float m_fLocationZ;
	float m_fRotationX;
	float m_fRotationY;
	float m_fRotationZ;
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	CString m_szGetItem;
	afx_msg void OnBnClickedComplete();
	afx_msg void OnNMCustomdrawRotationX2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawRotationY2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawRotationZ2(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SliderRotateX;
	CSliderCtrl m_SliderRotateY;
	CSliderCtrl m_SliderRotateZ;
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedCancel();
};
