#pragma once

#include "SizeKorea-2020Doc.h"
#include "SizeKorea-2020View.h"
// CPose4ResultDialog 대화 상자

class CPose4ResultDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPose4ResultDialog)

public:
	CPose4ResultDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CPose4ResultDialog();

	CSizeKorea2020View *GetView();
	CSizeKorea2020Doc *GetDocument();
	void InitDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POSE4_RESULT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_Pose4Result1;
	CString m_Pose4Result2;
	CString m_Pose4Result3;
	CString m_Pose4Result4;
	CString m_Pose4Result5;
	CString m_Pose4Result6;
	CString m_Pose4Result7;
	CString m_Pose4Result8;
	CString m_Pose4Result9;
	CString m_Pose4Result10;
	CString m_Pose4Result11;
	CString m_Pose4Result12;
	CString m_Pose4Result13;
	CString m_Pose4Result14;
	CString m_Pose4Result15;
	CString m_Pose4Result16;
	CString m_Pose4Result17;
	CString m_Pose4Result18;
	CString m_Pose4Result19;
	CString m_Pose4Result20;
	CString m_Pose4Result21;
	CString m_Pose4Result22;
	afx_msg void OnBnClickedPose4ShowPath1();
	afx_msg void OnBnClickedPose4ShowPath2();
	afx_msg void OnBnClickedPose4ShowPath3();
	afx_msg void OnBnClickedPose4ShowPath4();
	afx_msg void OnBnClickedPose4ShowPath5();
	afx_msg void OnBnClickedPose4ShowPath6();
	afx_msg void OnBnClickedPose4ShowPath7();
	afx_msg void OnBnClickedPose4ShowPath8();
	afx_msg void OnBnClickedPose4ShowPath9();
	afx_msg void OnBnClickedPose4ShowPath10();
	afx_msg void OnBnClickedPose4ShowPath11();
	afx_msg void OnBnClickedPose4ShowPath12();
	afx_msg void OnBnClickedPose4ShowPath13();
	afx_msg void OnBnClickedPose4ShowPath14();
	afx_msg void OnBnClickedPose4ShowPath15();
	afx_msg void OnBnClickedPose4ShowPath16();
	afx_msg void OnBnClickedPose4ShowPath17();
	afx_msg void OnBnClickedPose4ShowPath18();
	afx_msg void OnBnClickedPose4ShowPath19();
	afx_msg void OnBnClickedPose4ShowPath20();
	afx_msg void OnBnClickedPose4ShowPath21();
	afx_msg void OnBnClickedPose4ShowPath22();
	afx_msg void OnBnClickedPose4ShowPathAll();
	afx_msg void OnBnClickedPose4ShowPathNone();
	BOOL m_bPose4ShowPath1;
	BOOL m_bPose4ShowPath2;
	BOOL m_bPose4ShowPath3;
	BOOL m_bPose4ShowPath4;
	BOOL m_bPose4ShowPath5;
	BOOL m_bPose4ShowPath6;
	BOOL m_bPose4ShowPath7;
	BOOL m_bPose4ShowPath8;
	BOOL m_bPose4ShowPath9;
	BOOL m_bPose4ShowPath10;
	BOOL m_bPose4ShowPath11;
	BOOL m_bPose4ShowPath12;
	BOOL m_bPose4ShowPath13;
	BOOL m_bPose4ShowPath14;
	BOOL m_bPose4ShowPath15;
	BOOL m_bPose4ShowPath16;
	BOOL m_bPose4ShowPath17;
	BOOL m_bPose4ShowPath18;
	BOOL m_bPose4ShowPath19;
	BOOL m_bPose4ShowPath20;
	BOOL m_bPose4ShowPath21;
	BOOL m_bPose4ShowPath22;
	afx_msg void OnBnClickedPose4ExportResult();
};
