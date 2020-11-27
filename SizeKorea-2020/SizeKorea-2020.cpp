
// SizeKorea-2020.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "SizeKorea-2020.h"
#include "MainFrm.h"

#include "SizeKorea-2020Doc.h"
#include "SizeKorea-2020View.h"

// mj::Added for pdf open (20. 11. 13)
#include "shellapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSizeKorea2020App

BEGIN_MESSAGE_MAP(CSizeKorea2020App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CSizeKorea2020App::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CSizeKorea2020App construction

CSizeKorea2020App::CSizeKorea2020App() noexcept
{
	m_bHiColorIcons = TRUE;

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("SizeKorea2020.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CSizeKorea2020App object
CSizeKorea2020App theApp;


// CSizeKorea2020App initialization

BOOL CSizeKorea2020App::InitInstance()
{
	CWinAppEx::InitInstance();

	// mj::Enable console
	if (!AllocConsole())
		AfxMessageBox("Failed to create the console!", MB_ICONEXCLAMATION);

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSizeKorea2020Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSizeKorea2020View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->SetWindowTextA("BodySizer 1.0.0"); // mj::
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// CSizeKorea2020App message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CSizeKorea2020App::OnAppAbout()
{
	// CAboutDlg aboutDlg;
	// aboutDlg.DoModal();
	ShellExecute(NULL, "open", "..\\usr\\bin\\BodySizer 2020 Manual.pdf", NULL, NULL, SW_SHOW);
}

// CSizeKorea2020App customization load/save methods

void CSizeKorea2020App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CSizeKorea2020App::LoadCustomState()
{
}

void CSizeKorea2020App::SaveCustomState()
{
}

// CSizeKorea2020App message handlers
int CSizeKorea2020App::ExitInstance()
{
	// mj::Free console
	if (!FreeConsole())
		AfxMessageBox("Could not free the console!");

	return CWinAppEx::ExitInstance();
}
