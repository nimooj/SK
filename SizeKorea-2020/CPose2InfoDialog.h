#pragma once

#include "SizeKorea-2020View.h"
#include "SizeKorea-2020Doc.h"
// CPose2InfoDialog

#include "Pose2Landmarks.h"

class CPose2InfoDialog : public CPaneDialog
{
	DECLARE_DYNAMIC(CPose2InfoDialog)

public:
	CPose2InfoDialog();
	virtual ~CPose2InfoDialog();

	CSizeKorea2020View *GetView();
	CSizeKorea2020Doc *GetDocument();

	int m_SltButtonId;
	CString m_Pose2MarkCoords;

protected:
	virtual void OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisablelfNoHndler)
	{
		CPaneDialog::OnUpdateCmdUI(pTarget, FALSE);
	}
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPose2Mark1();
	afx_msg void OnBnClickedPose2Mark2();
	afx_msg void OnBnClickedPose2Mark3();
	afx_msg void OnBnClickedPose2Mark4();
	afx_msg void OnBnClickedPose2Mark5();
	afx_msg void OnBnClickedPose2Mark6();
	afx_msg void OnBnClickedPose2Mark7();
	afx_msg void OnBnClickedPose2Mark8();
	afx_msg void OnBnClickedPose2Mark9();
	afx_msg void OnBnClickedPose2Mark10();
	afx_msg void OnBnClickedPose2Mark11();
	afx_msg void OnBnClickedPose2Mark12();
	afx_msg void OnBnClickedPose2Mark14();
	afx_msg void OnBnClickedPose2Mark17();
	afx_msg void OnBnClickedPose2Mark18();
	afx_msg void OnBnClickedPose2Mark19();
	afx_msg void OnBnClickedPose2Mark20();
	afx_msg void OnBnClickedPose2Mark21();
	afx_msg void OnBnClickedPose2Mark22();
	afx_msg void OnBnClickedPose2Mark23();
	afx_msg void OnBnClickedPose2Mark24();
	afx_msg void OnBnClickedPose2Mark25();
	afx_msg void OnBnClickedPose2Mark26();
	afx_msg void OnBnClickedPose2Mark27();
	afx_msg void OnBnClickedPose2Mark28();
	afx_msg void OnBnClickedPose2Mark29();
	afx_msg void OnBnClickedPose2Mark30();
	afx_msg void OnBnClickedPose2Mark32();
	afx_msg void OnBnClickedPose2Mark33();
	afx_msg void OnBnClickedPose2Mark34();
	afx_msg void OnBnClickedPose2Mark35();
	afx_msg void OnBnClickedPose2Mark36();
	afx_msg void OnBnClickedPose2Mark42();
	afx_msg void OnBnClickedPose2Mark43();
	afx_msg void OnBnClickedPose2Mark16();
	afx_msg void OnBnClickedPose2Mark13();
	afx_msg void OnBnClickedPose2Mark37();
	afx_msg void OnBnClickedPose2Mark15();
	afx_msg void OnBnClickedPose2Mark38();
	afx_msg void OnBnClickedPose2Mark39();
	afx_msg void OnBnClickedPose2Mark40();
	afx_msg void OnBnClickedPose2Mark41();
	afx_msg void OnBnClickedPose2Mark44();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DoDataExchange(CDataExchange* pDX);
};


