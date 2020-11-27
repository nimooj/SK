#pragma once

#include "SizeKorea-2020View.h"
#include "SizeKorea-2020Doc.h"
// CPose4InfoDialog

class CPose4InfoDialog : public CPaneDialog
{
	DECLARE_DYNAMIC(CPose4InfoDialog)

public:
	CPose4InfoDialog();
	virtual ~CPose4InfoDialog();

	CSizeKorea2020View *GetView();
	CSizeKorea2020Doc *GetDocument();

	int m_SltButtonId;
	CString m_Pose4MarkCoords;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisablelfNoHndler)
	{
		CPaneDialog::OnUpdateCmdUI(pTarget, FALSE);
	}
	DECLARE_MESSAGE_MAP()	
public:
	afx_msg void OnBnClickedP4HT();
	afx_msg void OnBnClickedP4Ectocanthus();
	afx_msg void OnBnClickedP4Cervicale();
	afx_msg void OnBnClickedP4LSR();
	afx_msg void OnBnClickedP4LSL();
	afx_msg void OnBnClickedP4AR();
	afx_msg void OnBnClickedP4AL();
	afx_msg void OnBnClickedP4BP();
	afx_msg void OnBnClickedP4CO();
	afx_msg void OnBnClickedP4US();
	afx_msg void OnBnClickedP4Dactylion();
	afx_msg void OnBnClickedP4BS();
	afx_msg void OnBnClickedP4STS();
	afx_msg void OnBnClickedP4SPS();
	afx_msg void OnBnClickedP4PJCT();
	afx_msg void OnBnClickedP4DR();
	afx_msg void OnBnClickedP4DL();
	afx_msg void OnBnClickedP4LHER();
	afx_msg void OnBnClickedP4LHEL();
	afx_msg void OnBnClickedP4MHWR();
	afx_msg void OnBnClickedP4MHWL();
	afx_msg void OnBnClickedP4AKS();
	afx_msg void OnBnClickedP4ROS();
	afx_msg void OnBnClickedP4BOS();
	afx_msg void OnBnClickedP4APS();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};


