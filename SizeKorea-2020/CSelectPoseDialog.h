#pragma once


// CSelectPoseDialog dialog

class CSelectPoseDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectPoseDialog)

public:
	CSelectPoseDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSelectPoseDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECT_POSE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelectPose1();
	afx_msg void OnBnClickedSelectPose2();
	afx_msg void OnBnClickedSelectPose3();
	afx_msg void OnBnClickedSelectPose4();
	int m_ScanPose;
};
