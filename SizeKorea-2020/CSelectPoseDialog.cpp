// CSelectPoseDialog.cpp : implementation file
//

#include "pch.h"
#include "SizeKorea-2020.h"
#include "CSelectPoseDialog.h"
#include "afxdialogex.h"


// CSelectPoseDialog dialog

IMPLEMENT_DYNAMIC(CSelectPoseDialog, CDialogEx)

CSelectPoseDialog::CSelectPoseDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SELECT_POSE_DIALOG, pParent)
	, m_ScanPose(0)
{
}

CSelectPoseDialog::~CSelectPoseDialog()
{
}

void CSelectPoseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_SELECT_POSE1, m_ScanPose);
	DDV_MinMaxInt(pDX, m_ScanPose, 0, 4);
}


BEGIN_MESSAGE_MAP(CSelectPoseDialog, CDialogEx)
	ON_BN_CLICKED(IDC_SELECT_POSE1, &CSelectPoseDialog::OnBnClickedSelectPose1)
	ON_BN_CLICKED(IDC_SELECT_POSE2, &CSelectPoseDialog::OnBnClickedSelectPose2)
	ON_BN_CLICKED(IDC_SELECT_POSE3, &CSelectPoseDialog::OnBnClickedSelectPose3)
	ON_BN_CLICKED(IDC_SELECT_POSE4, &CSelectPoseDialog::OnBnClickedSelectPose4)
END_MESSAGE_MAP()


// CSelectPoseDialog message handlers


void CSelectPoseDialog::OnBnClickedSelectPose1()
{
}


void CSelectPoseDialog::OnBnClickedSelectPose2()
{
}


void CSelectPoseDialog::OnBnClickedSelectPose3()
{
}

void CSelectPoseDialog::OnBnClickedSelectPose4()
{
}
