
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "SizeKorea-2020Doc.h"
#include "SizeKorea-2020View.h"

#include "CPose1InfoDialog.h"
#include "CPose2InfoDialog.h"
#include "CPose3InfoDialog.h"
#include "CPose4InfoDialog.h"
#include "CPose5InfoDialog.h"

#include "CPose1ResultDialog.h"

#include "CPose2ResultDialog.h"
// #include "CPose2ResultPathDialog.h"

#include "CPose3ResultDialog.h"
#include "CPose4ResultDialog.h"

#include "CSelectPoseDialog.h"
#include "CEditModelDialog.h"
#include "CProgressDialog.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	int scanPose;
	
	/*! \brief 자세(1, 2, 3, 4, 5) 랜드마크 정보를 갖는 대화상자 */
	CPose1InfoDialog m_Pose1InfoDlg;
	CPose2InfoDialog m_Pose2InfoDlg;
	CPose3InfoDialog m_Pose3InfoDlg;
	CPose4InfoDialog m_Pose4InfoDlg;
	CPose5InfoDialog m_Pose5InfoDlg;
	CEditModelDialog m_EditModelDlg;
	
	/*! \brief 자세(1, 2, 3, 4, 5) 측정 정보를 갖는 대화상자 */
	CPose1ResultDialog	*m_pPose1ResultDlg;
	CPose2ResultDialog	*m_pPose2ResultDlg;
	CPose3ResultDialog	*m_pPose3ResultDlg;
	CPose4ResultDialog	*m_pPose4ResultDlg;

	CProgressDialog		*m_pLandmarkProgressDlg;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// Implementation
public:
	virtual ~CMainFrame();
	CSizeKorea2020Doc* GetDocument();
	CSizeKorea2020View* GetView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar			m_wndMenuBar;
	CMFCToolBar			m_wndToolBar;
	CMFCToolBarImages	m_UserImages;
	CMFCStatusBar		m_wndStatusBar;

public:

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnViewResult();
	afx_msg void OnScanPose1();
	afx_msg void OnScanPose2();
	afx_msg void OnScanPose3();
	afx_msg void OnScanPose4();
	afx_msg void OnScanPose3Old();
	afx_msg void OnEditModel();
};


