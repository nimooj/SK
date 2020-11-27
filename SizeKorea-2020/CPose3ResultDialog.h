#pragma once

#include "SizeKorea-2020Doc.h"
#include "SizeKorea-2020View.h"

// CPose3ResultDialog 대화 상자

class CPose3ResultDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPose3ResultDialog)

public:
	CPose3ResultDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CPose3ResultDialog();

	CSizeKorea2020View *GetView();
	CSizeKorea2020Doc *GetDocument();
	void InitDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POSE3_RESULT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_Pose3Result1;
	CString m_Pose3Result2;
	CString m_Pose3Result3;
	CString m_Pose3Result4;
	CString m_Pose3Result5;
	CString m_Pose3Result6;
	CString m_Pose3Result7;
	CString m_Pose3Result8;
	BOOL m_bPose3ShowPath1;
	BOOL m_bPose3ShowPath2;
	BOOL m_bPose3ShowPath3;
	BOOL m_bPose3ShowPath4;
	BOOL m_bPose3ShowPath5;
	BOOL m_bPose3ShowPath6;
	BOOL m_bPose3ShowPath7;
	BOOL m_bPose3ShowPath8;
	afx_msg void OnBnClickedPose3ShowPath1();
	afx_msg void OnBnClickedPose3ShowPath2();
	afx_msg void OnBnClickedPose3ShowPath3();
	afx_msg void OnBnClickedPose3ShowPath4();
	afx_msg void OnBnClickedPose3ShowPath5();
	afx_msg void OnBnClickedPose3ShowPath6();
	afx_msg void OnBnClickedPose3ShowPath7();
	afx_msg void OnBnClickedPose3ShowPath8();
	afx_msg void OnBnClickedPose3ShowPathAll();
	afx_msg void OnBnClickedPose3ShowPathNone();
	
	afx_msg void OnBnClickedPose3ExportResult();
};
