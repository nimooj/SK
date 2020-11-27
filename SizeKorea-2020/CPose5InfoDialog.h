#pragma once

#include "SizeKorea-2020View.h"
#include "SizeKorea-2020Doc.h"

// CPose5InfoDialog
class CPose5InfoDialog : public CPaneDialog
{
	DECLARE_DYNAMIC(CPose5InfoDialog)

public:
	CPose5InfoDialog();
	virtual ~CPose5InfoDialog();
	CSizeKorea2020View *GetView();
	CSizeKorea2020Doc *GetDocument();

	int m_SltButtonId;
	CString m_Pose5MarkCoords;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisablelfNoHndler)
	{
		CPaneDialog::OnUpdateCmdUI(pTarget, FALSE);
	}

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedP5Bp();
	afx_msg void OnBnClickedP5Ap();
	afx_msg void OnBnClickedP5Bmp();
	afx_msg void OnBnClickedP5Lfc();
	afx_msg void OnBnClickedP5Rfc();
	afx_msg void OnBnClickedP5Lbp();
	afx_msg void OnBnClickedP5Bo();
	afx_msg void OnBnClickedP5Ro();
	afx_msg void OnBnClickedP5La();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};


