#pragma once
enum ColliderType { OBB = 0, Sphere, Complete };

// XColliderControlForm 대화 상자

class XColliderControlForm : public CFormView
{
	DECLARE_DYNAMIC(XColliderControlForm)
// 사용자 정의 변수
public:
	bool m_bComplete = true;
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
public:
	BOOL m_bColliderType;
	afx_msg void OnBnClickedColliderCreate();
	afx_msg void OnEnChangeObbLocationX();
	afx_msg void OnEnChangeObbLocationY();
	afx_msg void OnEnChangeObbLocationZ();
	afx_msg void OnEnChangeObbRotationX();
	afx_msg void OnEnChangeObbRotationY();
	afx_msg void OnEnChangeObbRotationZ();
	afx_msg void OnEnChangeObbScaleX();
	afx_msg void OnEnChangeObbScaleY();
	afx_msg void OnEnChangeObbScaleZ();
	afx_msg void OnNMCustomdrawObbLocationX2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawObbLocationY2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawObbLocationZ2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawObbRotationX2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawObbRotationY2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawObbRotationZ2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawObbScaleX2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawObbScaleY2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawObbScaleZ2(NMHDR *pNMHDR, LRESULT *pResult);
	float m_fOBBLocationX;
	float m_fOBBLocationY;
	float m_fOBBLocationZ;
	float m_fOBBRotationX;
	float m_fOBBRotationY;
	float m_fOBBRotationZ;
	float m_fOBBScaleX;
	float m_fOBBScaleY;
	float m_fOBBScaleZ;
	afx_msg void OnBnClickedColliderComplete();
	CSliderCtrl m_OBBSliderLocationX;
	CSliderCtrl m_OBBSliderLocationY;
	CSliderCtrl m_OBBSliderLocationZ;
	virtual void OnInitialUpdate();
	CSliderCtrl m_OBBRotationX;
	CSliderCtrl m_OBBRotationY;
	CSliderCtrl m_OBBRotationZ;
	CSliderCtrl m_OBBScaleX;
	CSliderCtrl m_OBBScaleY;
	CSliderCtrl m_OBBScaleZ;
};
