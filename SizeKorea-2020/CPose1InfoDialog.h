#pragma once

//#include "MainFrm.h"
#include "SizeKorea-2020View.h"
#include "SizeKorea-2020Doc.h"

// CPose1InfoDialog
class CPose1InfoDialog : public CPaneDialog
{
	DECLARE_DYNAMIC(CPose1InfoDialog)

public:
	CPose1InfoDialog();
	virtual ~CPose1InfoDialog();
	
	CSizeKorea2020View *GetView();
	CSizeKorea2020Doc *GetDocument();

	int m_SltButtonId;
	CString m_Pose1MarkCoords;

protected:
	virtual void OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisablelfNoHndler)
	{
		CPaneDialog::OnUpdateCmdUI(pTarget, FALSE);
	}
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedPose1Mark1();
	afx_msg void OnBnClickedPose1Mark2();
	afx_msg void OnBnClickedPose1Mark3();
	afx_msg void OnBnClickedPose1Mark4();
	afx_msg void OnBnClickedPose1Mark5();
	afx_msg void OnBnClickedPose1Mark6();
	afx_msg void OnBnClickedPose1Mark7();
	afx_msg void OnBnClickedPose1Mark8();
	afx_msg void OnBnClickedPose1Mark9();
	afx_msg void OnBnClickedPose1Mark10();
	afx_msg void OnBnClickedPose1Mark11();
	afx_msg void OnBnClickedPose1Mark12();
	afx_msg void OnBnClickedPose1Mark13();
	afx_msg void OnBnClickedPose1Mark14();
	afx_msg void OnBnClickedPose1Mark15();
	afx_msg void OnBnClickedPose1Mark16();
	afx_msg void OnBnClickedPose1Mark17();
	afx_msg void OnBnClickedPose1Mark18();
	afx_msg void OnBnClickedPose1Mark19();
	afx_msg void OnBnClickedPose1Mark20();
	afx_msg void OnBnClickedPose1Mark21();
	afx_msg void OnBnClickedPose1Mark22();
	afx_msg void OnBnClickedPose1Mark23();
	afx_msg void OnBnClickedPose1Mark24();
	afx_msg void OnBnClickedPose1Mark25();
	afx_msg void OnBnClickedPose1Mark26();
	afx_msg void OnBnClickedPose1Mark27();
	afx_msg void OnBnClickedPose1Mark28();
	afx_msg void OnBnClickedPose1Mark29();
	afx_msg void OnBnClickedPose1Mark30();
	afx_msg void OnBnClickedPose1Mark31();
	afx_msg void OnBnClickedPose1Mark32();
	afx_msg void OnBnClickedPose1Mark33();
	afx_msg void OnBnClickedPose1Mark34();
	afx_msg void OnBnClickedPose1Mark35();
	afx_msg void OnBnClickedPose1Mark36();
	afx_msg void OnBnClickedPose1Mark37();
	afx_msg void OnBnClickedPose1Mark38();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnBnClickedPose1Mark39();
	afx_msg void OnBnClickedPose1Mark40();
	afx_msg void OnBnClickedPose1Mark41();
	afx_msg void OnBnClickedPose1Mark42();
};


