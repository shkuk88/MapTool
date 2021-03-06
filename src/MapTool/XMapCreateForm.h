#pragma once


// XMapForm 대화 상자
// CFormView가 클래스 생성에 나오지 않기 때문에 CDialogEX로 만든 후 상속을 CFormView로 바꾼다
// CDialogEX 만들기전에 Dialog 하나를 만들고 속성에서 title을 false로 하고 style을 child로 한다.

class XMapCreateForm : public CFormView
{
	DECLARE_DYNAMIC(XMapCreateForm)

//사용자 정의 변수
private:
	vector<CString> m_szMapTexture;
	vector<CString> m_szHeightMapTexture;
	int m_iMapTexSelect;
	int m_iHeightMapTexSelect;
	CImage	m_img;
	bool m_bOnMap;
// 사용자 정의 함수
public:
	static XMapCreateForm* CreateOne(CWnd* pParent); // 하나만 만들도록 한다.

public:
	XMapCreateForm();   // 표준 생성자입니다.
	virtual ~XMapCreateForm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XMapForm };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_MapTexture;
	afx_msg void OnBnClickedFind1();
	afx_msg void OnLbnSelchangeTextureList();
	CListBox m_HeightMapTexture;
	afx_msg void OnBnClickedFind2();
	afx_msg void OnLbnSelchangeTextureList2();
	afx_msg void OnBnClickedDel1();
	afx_msg void OnBnClickedDel2();
	afx_msg void OnBnClickedCreateMap();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	float m_fCellCount;
	float m_fDistance;
};
