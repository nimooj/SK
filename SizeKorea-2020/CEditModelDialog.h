#pragma once

#include "SizeKorea-2020View.h"
#include "SizeKorea-2020Doc.h"
// CEditModelDialog

class CEditModelDialog : public CPaneDialog
{
	DECLARE_DYNAMIC(CEditModelDialog)

public:
	CEditModelDialog();
	virtual ~CEditModelDialog();

	CSizeKorea2020View *GetView();
	CSizeKorea2020Doc *GetDocument();
	void UpdateDlgData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisablelfNoHndler)
	{
		CPaneDialog::OnUpdateCmdUI(pTarget, FALSE);
	}

	DECLARE_MESSAGE_MAP()

public:
	CString m_FileName;
	CString m_ScanPose;
	int		m_NumVert;
	int		m_NumFace;
	CString	m_Width;
	CString	m_Height;
	CString	m_Depth;
	double	m_TransX;
	double	m_TransY;
	double	m_TransZ;
	double	m_RotX;
	double	m_RotY;
	double	m_RotZ;
	double	m_Scale;
	afx_msg void OnBnClickedApplyEdit();
	afx_msg void OnBnClickedRenderPolygon();
	afx_msg void OnBnClickedRenderWire();
	afx_msg void OnBnClickedRenderPoint();
	afx_msg void OnBnClickedRenderTexture();
	afx_msg void OnBnClickedInitOrigin();
	afx_msg void OnBnClickedEditAuto();
};


