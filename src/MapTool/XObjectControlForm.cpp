// XObjectControlForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "XObjectControlForm.h"
#include "afxdialogex.h"

// 생성
XObjectControlForm* XObjectControlForm::CreateOne(CWnd* pParent)
{
	XObjectControlForm* pForm = new XObjectControlForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}
// XObjectControlForm 대화 상자

IMPLEMENT_DYNAMIC(XObjectControlForm, CFormView)

XObjectControlForm::XObjectControlForm()
	: CFormView(IDD_OBJECT_FORM)
{

}

XObjectControlForm::~XObjectControlForm()
{
}

void XObjectControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJ_LIST, m_ObjectList);
}


BEGIN_MESSAGE_MAP(XObjectControlForm, CFormView)
	ON_BN_CLICKED(IDC_ADD_OBJECT, &XObjectControlForm::OnBnClickedAddObject)
	ON_BN_CLICKED(IDC_DEL_OBJECT, &XObjectControlForm::OnBnClickedDelObject)
END_MESSAGE_MAP()


// XObjectControlForm 메시지 처리기


void XObjectControlForm::OnBnClickedAddObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR fileFilter[] = _T("Object (*.shk) | *.SHK; | All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, fileFilter);

	if (dlg.DoModal() == IDOK)
	{
		for (int iObj = 0; iObj < m_szObjectPath.size(); iObj++)
		{
			if (!m_szObjectPath[iObj].Compare(dlg.GetPathName()))	// 같은 Texture면 그냥 return
			{
				return;
			}
		}
		m_szObjectPath.push_back(dlg.GetPathName());
	}
	else // 확인 외에 다른 버튼을 누를시 리턴
	{
		return;
	}
	m_iSelectObject = m_szObjectPath.size() - 1;
	// 경로 추출한 문자열이 리스트에 보이도록 한다. 경로가 포함 문자열은 따로 가지고 있는다. 
	CString szDelPath = m_szObjectPath[m_iSelectObject].Right(m_szObjectPath[m_iSelectObject].GetLength() - m_szObjectPath[m_iSelectObject].ReverseFind('\\') - 1);
	m_ObjectList.InsertString(m_iSelectObject, szDelPath);
}


void XObjectControlForm::OnBnClickedDelObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iSelectObject = m_ObjectList.GetCurSel();
	if (m_iSelectObject == -1)	return;
	m_ObjectList.DeleteString(m_iSelectObject);

	m_szObjectPath.erase(m_szObjectPath.begin() + m_iSelectObject);
}
