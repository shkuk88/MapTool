﻿#pragma once


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
};
