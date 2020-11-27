// CProgressDialog.cpp: 구현 파일
//

#include "pch.h"
#include "SizeKorea-2020.h"
#include "CProgressDialog.h"
#include "afxdialogex.h"


// CProgressDialog 대화 상자

IMPLEMENT_DYNAMIC(CProgressDialog, CDialogEx)

CProgressDialog::CProgressDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROGRESS_DIALOG, pParent)
	, m_Status(_T(""))
{

}

CProgressDialog::~CProgressDialog()
{
}

void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LANDMARK_PROGRESS, m_LandmarkProgress);
	DDX_Text(pDX, IDC_STATUS, m_Status);
}


BEGIN_MESSAGE_MAP(CProgressDialog, CDialogEx)
END_MESSAGE_MAP()


// CProgressDialog 메시지 처리기
