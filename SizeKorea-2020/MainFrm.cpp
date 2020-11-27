
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "SizeKorea-2020.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_VIEW_RESULT, &CMainFrame::OnViewResult)
	ON_COMMAND(ID_SCAN_POSE1, &CMainFrame::OnScanPose1)
	ON_COMMAND(ID_SCAN_POSE2, &CMainFrame::OnScanPose2)
	ON_COMMAND(ID_SCAN_POSE3, &CMainFrame::OnScanPose3)
	ON_COMMAND(ID_SCAN_POSE4, &CMainFrame::OnScanPose4)
	ON_COMMAND(ID_SCAN_POSE3_OLD, &CMainFrame::OnScanPose3Old)
	ON_COMMAND(ID_EDIT_MODEL, &CMainFrame::OnEditModel)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLACK);

	m_pPose1ResultDlg = NULL;
	m_pPose2ResultDlg = NULL;
	m_pPose3ResultDlg = NULL;
	m_pPose4ResultDlg = NULL;

	m_pLandmarkProgressDlg = NULL;
}

CMainFrame::~CMainFrame()
{
	if (m_pPose1ResultDlg != NULL)
		delete m_pPose1ResultDlg;
	if (m_pPose2ResultDlg != NULL)
		delete m_pPose2ResultDlg;
	if (m_pPose3ResultDlg != NULL)
		delete m_pPose3ResultDlg;
	if (m_pPose4ResultDlg != NULL)
		delete m_pPose4ResultDlg;
	if (m_pLandmarkProgressDlg != NULL)
		delete m_pLandmarkProgressDlg;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 상태바 만들기
	//if (!m_wndStatusBar.Create(this) ||
	//	!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT)))
	//{
	//	TRACE0("Failed to create status bar\n");
	//	return -1;     // fail to create
	//}

	// Allow user-defined toolbars operations:
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 자세(1, 2, 3, 4, 5) 랜드마크 대화상자 생성
	if (!m_Pose1InfoDlg.Create(_T("자세1"), this, TRUE,
		MAKEINTRESOURCE(IDD_POSE1_INFO_DIALOG),
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
		ID_SCAN_POSE1))
	{
		TRACE0("Failed to create Dialog Bar\n");
		return FALSE;      // fail to create
	}
	if (!m_Pose2InfoDlg.Create(_T("자세2"), this, TRUE,
		MAKEINTRESOURCE(IDD_POSE2_INFO_DIALOG),
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
		ID_SCAN_POSE2))
	{
		TRACE0("Failed to create Dialog Bar\n");
		return FALSE;      // fail to create
	}
	if (!m_Pose3InfoDlg.Create(_T("자세3"), this, TRUE,
		MAKEINTRESOURCE(IDD_POSE3_INFO_DIALOG),
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
		ID_SCAN_POSE3))
	{
		TRACE0("Failed to create Dialog Bar\n");
		return FALSE;      // fail to create
	}
	if (!m_Pose4InfoDlg.Create(_T("자세4"), this, TRUE,
		MAKEINTRESOURCE(IDD_POSE4_INFO_DIALOG),
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
		ID_SCAN_POSE4))
	{
		TRACE0("Failed to create Dialog Bar\n");
		return FALSE;      // fail to create
	}
	if (!m_Pose5InfoDlg.Create(_T("자세3(반전)"), this, TRUE,
		MAKEINTRESOURCE(IDD_POSE5_INFO_DIALOG),
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
		ID_SCAN_POSE3_OLD))
	{
		TRACE0("Failed to create Dialog Bar\n");
		return FALSE;      // fail to create
	}
	if (!m_EditModelDlg.Create(_T("모델 편집"), this, TRUE,
		MAKEINTRESOURCE(IDD_EDIT_MODEL_DIALOG),
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
		ID_EDIT_MODEL))
	{
		TRACE0("Failed to create Dialog Bar\n");
		return FALSE;      // fail to create
	}
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);
	m_Pose1InfoDlg.EnableDocking(CBRS_ALIGN_ANY);
	m_Pose2InfoDlg.EnableDocking(CBRS_ALIGN_ANY);
	m_Pose3InfoDlg.EnableDocking(CBRS_ALIGN_ANY);
	m_Pose4InfoDlg.EnableDocking(CBRS_ALIGN_ANY);
	m_Pose5InfoDlg.EnableDocking(CBRS_ALIGN_ANY);
	m_EditModelDlg.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_Pose1InfoDlg);
	DockPane(&m_Pose2InfoDlg);
	DockPane(&m_Pose3InfoDlg);
	DockPane(&m_Pose4InfoDlg);
	DockPane(&m_Pose5InfoDlg);
	DockPane(&m_EditModelDlg);

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

CSizeKorea2020Doc* CMainFrame::GetDocument() {
	return (CSizeKorea2020Doc*)GetActiveDocument();
}

CSizeKorea2020View* CMainFrame::GetView() {
	return (CSizeKorea2020View*)GetActiveView();
}


void CMainFrame::OnViewResult()
{
	// TODO: Add your command handler code here
	CSizeKorea2020Doc *pDoc = GetDocument();

	switch (pDoc->m_ScanPose)
	{
	case 1:
		if (m_pPose1ResultDlg == NULL)
		{
			m_pPose1ResultDlg = new CPose1ResultDialog();
			m_pPose1ResultDlg->Create(IDD_POSE1_RESULT_DIALOG);
			m_pPose1ResultDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pPose1ResultDlg->ShowWindow(SW_SHOW);
			m_pPose1ResultDlg->SetFocus();
		}
		break;

	case 2:
		if (m_pPose2ResultDlg == NULL) {
			m_pPose2ResultDlg = new CPose2ResultDialog();
			m_pPose2ResultDlg->Create(IDD_POSE2_RESULT_DIALOG);
			m_pPose2ResultDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pPose2ResultDlg->ShowWindow(SW_SHOW);
			m_pPose2ResultDlg->SetFocus();
		}
		break;

	case 3:
		if (m_pPose3ResultDlg == NULL)
		{
			m_pPose3ResultDlg = new CPose3ResultDialog();
			m_pPose3ResultDlg->Create(IDD_POSE3_RESULT_DIALOG);
			m_pPose3ResultDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pPose3ResultDlg->ShowWindow(SW_SHOW);
			m_pPose3ResultDlg->SetFocus();
		}
		break;

	case 4:
		if (m_pPose4ResultDlg == NULL)
		{
			m_pPose4ResultDlg = new CPose4ResultDialog();
			m_pPose4ResultDlg->Create(IDD_POSE4_RESULT_DIALOG);
			m_pPose4ResultDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pPose4ResultDlg->ShowWindow(SW_SHOW);
			m_pPose4ResultDlg->SetFocus();
		}
		break;

	case 5:
		if (m_pPose3ResultDlg == NULL)
		{
			m_pPose3ResultDlg = new CPose3ResultDialog();
			m_pPose3ResultDlg->Create(IDD_POSE3_RESULT_DIALOG);
			m_pPose3ResultDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pPose3ResultDlg->ShowWindow(SW_SHOW);
			m_pPose3ResultDlg->SetFocus();
		}
		break;

	default:
		break;
	}
}

void CMainFrame::OnScanPose1()
{
	ShowPane(&m_Pose1InfoDlg, !(m_Pose1InfoDlg.IsVisible()), FALSE, TRUE);
	RecalcLayout();
}


void CMainFrame::OnScanPose2()
{
	ShowPane(&m_Pose2InfoDlg, !(m_Pose2InfoDlg.IsVisible()), FALSE, TRUE);
	RecalcLayout();
}

void CMainFrame::OnScanPose3()
{
	ShowPane(&m_Pose3InfoDlg, !(m_Pose3InfoDlg.IsVisible()), FALSE, TRUE);
	RecalcLayout();
}

void CMainFrame::OnScanPose3Old()
{
	ShowPane(&m_Pose5InfoDlg, !(m_Pose5InfoDlg.IsVisible()), FALSE, TRUE);
	RecalcLayout();
}

void CMainFrame::OnScanPose4()
{
	ShowPane(&m_Pose4InfoDlg, !(m_Pose4InfoDlg.IsVisible()), FALSE, TRUE);
	RecalcLayout();
}

void CMainFrame::OnEditModel()
{
	ShowPane(&m_EditModelDlg, !(m_EditModelDlg.IsVisible()), FALSE, TRUE);
	RecalcLayout();
}
