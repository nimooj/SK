#pragma once

#include "SizeKorea-2020View.h"
#include "SizeKorea-2020Doc.h"
// CPose3InfoDialog

class CPose3InfoDialog : public CPaneDialog
{
	DECLARE_DYNAMIC(CPose3InfoDialog)

public:
	CPose3InfoDialog();
	virtual ~CPose3InfoDialog();

	CSizeKorea2020View *GetView();
	CSizeKorea2020Doc *GetDocument();

	int m_SltButtonId;
	CString m_Pose3MarkCoords;
		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	virtual void OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisablelfNoHndler)
	{
		CPaneDialog::OnUpdateCmdUI(pTarget, FALSE);
	}
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedP3BP();
	afx_msg void OnBnClickedP3AP();
	afx_msg void OnBnClickedP3BMP();
	afx_msg void OnBnClickedP3LFC();
	afx_msg void OnBnClickedP3RFC();
	afx_msg void OnBnClickedP3RBP();
	afx_msg void OnBnClickedP3BO();
	afx_msg void OnBnClickedP3RO();
	afx_msg void OnBnClickedP3RA();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};


