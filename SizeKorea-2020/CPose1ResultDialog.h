#pragma once

#include "SizeKorea-2020Doc.h"
#include "SizeKorea-2020View.h"


// CPose1ResultDialog dialog

class CPose1ResultDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPose1ResultDialog)

public:
	CPose1ResultDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPose1ResultDialog();

	CSizeKorea2020View *GetView();
	CSizeKorea2020Doc *GetDocument();
	void InitDialog();


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POSE1_RESULT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPose1ShowPath1();
	afx_msg void OnBnClickedPose1ShowPath2();
	afx_msg void OnBnClickedPose1ShowPath3();
	afx_msg void OnBnClickedPose1ShowPath4();
	afx_msg void OnBnClickedPose1ShowPath5();
	afx_msg void OnBnClickedPose1ShowPath6();
	afx_msg void OnBnClickedPose1ShowPath7();
	afx_msg void OnBnClickedPose1ShowPath8();
	afx_msg void OnBnClickedPose1ShowPath9();
	afx_msg void OnBnClickedPose1ShowPath10();
	afx_msg void OnBnClickedPose1ShowPath11();
	afx_msg void OnBnClickedPose1ShowPath12();
	afx_msg void OnBnClickedPose1ShowPath13();
	afx_msg void OnBnClickedPose1ShowPath14();
	afx_msg void OnBnClickedPose1ShowPath15();
	afx_msg void OnBnClickedPose1ShowPath16();
	afx_msg void OnBnClickedPose1ShowPath17();
	afx_msg void OnBnClickedPose1ShowPath18();
	afx_msg void OnBnClickedPose1ShowPath19();
	afx_msg void OnBnClickedPose1ShowPath20();
	afx_msg void OnBnClickedPose1ShowPath21();
	afx_msg void OnBnClickedPose1ShowPath22();
	afx_msg void OnBnClickedPose1ShowPath23();
	afx_msg void OnBnClickedPose1ShowPath24();
	afx_msg void OnBnClickedPose1ShowPath25();
	afx_msg void OnBnClickedPose1ShowPath26();
	afx_msg void OnBnClickedPose1ShowPath27();
	afx_msg void OnBnClickedPose1ShowPath28();
	afx_msg void OnBnClickedPose1ShowPath29();
	afx_msg void OnBnClickedPose1ShowPath30();
	afx_msg void OnBnClickedPose1ShowPath31();
	afx_msg void OnBnClickedPose1ShowPath32();
	afx_msg void OnBnClickedPose1ShowPath33();
	afx_msg void OnBnClickedPose1ShowPath34();
	afx_msg void OnBnClickedPose1ShowPath35();
	afx_msg void OnBnClickedPose1ShowPath36();
	afx_msg void OnBnClickedPose1ShowPath37();
	afx_msg void OnBnClickedPose1ShowPath38();
	afx_msg void OnBnClickedPose1ShowPath39();
	afx_msg void OnBnClickedPose1ShowPath40();
//	afx_msg void OnBnClickedPose1ShowPath41();
	afx_msg void OnBnClickedPose1ShowPath42();
	afx_msg void OnBnClickedPose1ShowPath43();
	afx_msg void OnBnClickedPose1ShowPath44();
	afx_msg void OnBnClickedPose1ShowPath45();
	afx_msg void OnBnClickedPose1ShowPath46();
	afx_msg void OnBnClickedPose1ShowPath47();
	afx_msg void OnBnClickedPose1ShowPath48();
	afx_msg void OnBnClickedPose1ShowPath49();
	afx_msg void OnBnClickedPose1ShowPath50();
	afx_msg void OnBnClickedPose1ShowPath51();
	afx_msg void OnBnClickedPose1ShowPath52();
	afx_msg void OnBnClickedPose1ShowPath53();
	afx_msg void OnBnClickedPose1ShowPath54();
	afx_msg void OnBnClickedPose1ShowPath55();
	afx_msg void OnBnClickedPose1ShowPath56();
	afx_msg void OnBnClickedPose1ShowPath57();
	afx_msg void OnBnClickedPose1ShowPath58();
	afx_msg void OnBnClickedPose1ShowPath59();
	afx_msg void OnBnClickedPose1ShowPath60();
	afx_msg void OnBnClickedPose1ShowPath61();
	afx_msg void OnBnClickedPose1ShowPath62();
	afx_msg void OnBnClickedPose1ShowPath63();
	afx_msg void OnBnClickedPose1ShowPath64();
	afx_msg void OnBnClickedPose1ShowPath65();
	CString m_Pose1Result1;
	CString m_Pose1Result2;
	CString m_Pose1Result3;
	CString m_Pose1Result4;
	CString m_Pose1Result5;
	CString m_Pose1Result6;
	CString m_Pose1Result7;
	CString m_Pose1Result8;
	CString m_Pose1Result9;
	CString m_Pose1Result10;
	CString m_Pose1Result11;
	CString m_Pose1Result12;
	CString m_Pose1Result13;
	CString m_Pose1Result14;
	CString m_Pose1Result15;
	CString m_Pose1Result16;
	CString m_Pose1Result17;
	CString m_Pose1Result18;
	CString m_Pose1Result19;
	CString m_Pose1Result20;
	CString m_Pose1Result21;
	CString m_Pose1Result22;
	CString m_Pose1Result23;
	CString m_Pose1Result24;
	CString m_Pose1Result25;
	CString m_Pose1Result26;
	CString m_Pose1Result27;
	CString m_Pose1Result28;
	CString m_Pose1Result29;
	CString m_Pose1Result30;
	CString m_Pose1Result31;
	CString m_Pose1Result32;
	CString m_Pose1Result33;
	CString m_Pose1Result34;
	CString m_Pose1Result35;
	CString m_Pose1Result36;
	CString m_Pose1Result37;
	CString m_Pose1Result38;
	CString m_Pose1Result39;
	CString m_Pose1Result40;
	CString m_Pose1Result41;
	CString m_Pose1Result42;
	CString m_Pose1Result43;
	CString m_Pose1Result44;
	CString m_Pose1Result45;
	CString m_Pose1Result46;
	CString m_Pose1Result47;
	CString m_Pose1Result48;
	CString m_Pose1Result49;
	CString m_Pose1Result50;
	CString m_Pose1Result51;
	CString m_Pose1Result52;
	CString m_Pose1Result53;
	CString m_Pose1Result54;
	CString m_Pose1Result55;
	CString m_Pose1Result56;
	CString m_Pose1Result57;
	CString m_Pose1Result58;
	CString m_Pose1Result59;
	CString m_Pose1Result60;
	CString m_Pose1Result61;
	CString m_Pose1Result62;
	CString m_Pose1Result63;
	CString m_Pose1Result64;
	CString m_Pose1Result65;
	BOOL m_bPose1ShowPath1;
	BOOL m_bPose1ShowPath2;
	BOOL m_bPose1ShowPath3;
	BOOL m_bPose1ShowPath4;
	BOOL m_bPose1ShowPath5;
	BOOL m_bPose1ShowPath6;
	BOOL m_bPose1ShowPath7;
	BOOL m_bPose1ShowPath8;
	BOOL m_bPose1ShowPath9;
	BOOL m_bPose1ShowPath10;
	BOOL m_bPose1ShowPath11;
	BOOL m_bPose1ShowPath12;
	BOOL m_bPose1ShowPath13;
	BOOL m_bPose1ShowPath14;
	BOOL m_bPose1ShowPath15;
	BOOL m_bPose1ShowPath16;
	BOOL m_bPose1ShowPath17;
	BOOL m_bPose1ShowPath18;
	BOOL m_bPose1ShowPath19;
	BOOL m_bPose1ShowPath20;
	BOOL m_bPose1ShowPath21;
	BOOL m_bPose1ShowPath22;
	BOOL m_bPose1ShowPath23;
	BOOL m_bPose1ShowPath24;
	BOOL m_bPose1ShowPath25;
	BOOL m_bPose1ShowPath26;
	BOOL m_bPose1ShowPath27;
	BOOL m_bPose1ShowPath28;
	BOOL m_bPose1ShowPath29;
	BOOL m_bPose1ShowPath30;
	BOOL m_bPose1ShowPath31;
	BOOL m_bPose1ShowPath32;
	BOOL m_bPose1ShowPath33;
	BOOL m_bPose1ShowPath34;
	BOOL m_bPose1ShowPath35;
	BOOL m_bPose1ShowPath36;
	BOOL m_bPose1ShowPath37;
	BOOL m_bPose1ShowPath38;
	BOOL m_bPose1ShowPath39;
	BOOL m_bPose1ShowPath40;
	BOOL m_bPose1ShowPath41;
	BOOL m_bPose1ShowPath42;
	BOOL m_bPose1ShowPath43;
	BOOL m_bPose1ShowPath44;
	BOOL m_bPose1ShowPath45;
	BOOL m_bPose1ShowPath46;
	BOOL m_bPose1ShowPath47;
	BOOL m_bPose1ShowPath48;
	BOOL m_bPose1ShowPath49;
	BOOL m_bPose1ShowPath50;
	BOOL m_bPose1ShowPath51;
	BOOL m_bPose1ShowPath52;
	BOOL m_bPose1ShowPath53;
	BOOL m_bPose1ShowPath54;
	BOOL m_bPose1ShowPath55;
	BOOL m_bPose1ShowPath56;
	BOOL m_bPose1ShowPath57;
	BOOL m_bPose1ShowPath58;
	BOOL m_bPose1ShowPath59;
	BOOL m_bPose1ShowPath60;
	BOOL m_bPose1ShowPath61;
	BOOL m_bPose1ShowPath62;
	BOOL m_bPose1ShowPath63;
	BOOL m_bPose1ShowPath64;
	BOOL m_bPose1ShowPath65;
	afx_msg void OnBnClickedPose1ShowPathAll();
	afx_msg void OnBnClickedPose1ShowPathNone();
	afx_msg void OnBnClickedPose1ExportResult();
	afx_msg void OnBnClickedPose1ShowPath41();
};
