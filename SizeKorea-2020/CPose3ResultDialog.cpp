// CPose3ResultDialog.cpp: 구현 파일
//

#include "pch.h"
#include "SizeKorea-2020.h"
#include "CPose3ResultDialog.h"
#include "afxdialogex.h"
#include "EgSizer.h"
#include "MainFrm.h"

extern EgSizer theSizer;
extern std::vector<std::string> ResultPose3;

// CPose3ResultDialog 대화 상자
IMPLEMENT_DYNAMIC(CPose3ResultDialog, CDialogEx)

CPose3ResultDialog::CPose3ResultDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POSE3_RESULT_DIALOG, pParent)
	, m_bPose3ShowPath1(FALSE)
{
	m_bPose3ShowPath1 = theSizer.m_bShowPaths[0];
	m_bPose3ShowPath2 = theSizer.m_bShowPaths[1];
	m_bPose3ShowPath3 = theSizer.m_bShowPaths[2];
	m_bPose3ShowPath4 = theSizer.m_bShowPaths[3];
	m_bPose3ShowPath5 = theSizer.m_bShowPaths[4];
	m_bPose3ShowPath6 = theSizer.m_bShowPaths[5];
	m_bPose3ShowPath7 = theSizer.m_bShowPaths[6];
	m_bPose3ShowPath8 = theSizer.m_bShowPaths[7];
}

CPose3ResultDialog::~CPose3ResultDialog()
{

}

void CPose3ResultDialog::InitDialog()
{
	m_bPose3ShowPath1 = theSizer.m_bShowPaths[0];
	m_bPose3ShowPath2 = theSizer.m_bShowPaths[1];
	m_bPose3ShowPath3 = theSizer.m_bShowPaths[2];
	m_bPose3ShowPath4 = theSizer.m_bShowPaths[3];
	m_bPose3ShowPath5 = theSizer.m_bShowPaths[4];
	m_bPose3ShowPath6 = theSizer.m_bShowPaths[5];
	m_bPose3ShowPath7 = theSizer.m_bShowPaths[6];
	m_bPose3ShowPath8 = theSizer.m_bShowPaths[7];
	UpdateData(FALSE);
	GetView()->Invalidate(false);
}

void CPose3ResultDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POSE3_RESULT1, m_Pose3Result1);
	DDX_Text(pDX, IDC_POSE3_RESULT2, m_Pose3Result2);
	DDX_Text(pDX, IDC_POSE3_RESULT3, m_Pose3Result3);
	DDX_Text(pDX, IDC_POSE3_RESULT4, m_Pose3Result4);
	DDX_Text(pDX, IDC_POSE3_RESULT5, m_Pose3Result5);
	DDX_Text(pDX, IDC_POSE3_RESULT6, m_Pose3Result6);
	DDX_Text(pDX, IDC_POSE3_RESULT7, m_Pose3Result7);
	DDX_Text(pDX, IDC_POSE3_RESULT8, m_Pose3Result8);
	DDX_Check(pDX, IDC_POSE3_SHOW_PATH1, m_bPose3ShowPath1);
	DDX_Check(pDX, IDC_POSE3_SHOW_PATH2, m_bPose3ShowPath2);
	DDX_Check(pDX, IDC_POSE3_SHOW_PATH3, m_bPose3ShowPath3);
	DDX_Check(pDX, IDC_POSE3_SHOW_PATH4, m_bPose3ShowPath4);
	DDX_Check(pDX, IDC_POSE3_SHOW_PATH5, m_bPose3ShowPath5);
	DDX_Check(pDX, IDC_POSE3_SHOW_PATH6, m_bPose3ShowPath6);
	DDX_Check(pDX, IDC_POSE3_SHOW_PATH7, m_bPose3ShowPath7);
	DDX_Check(pDX, IDC_POSE3_SHOW_PATH8, m_bPose3ShowPath8);
}

BEGIN_MESSAGE_MAP(CPose3ResultDialog, CDialogEx)
	ON_BN_CLICKED(IDC_POSE3_SHOW_PATH1, &CPose3ResultDialog::OnBnClickedPose3ShowPath1)
	ON_BN_CLICKED(IDC_POSE3_SHOW_PATH2, &CPose3ResultDialog::OnBnClickedPose3ShowPath2)
	ON_BN_CLICKED(IDC_POSE3_SHOW_PATH3, &CPose3ResultDialog::OnBnClickedPose3ShowPath3)
	ON_BN_CLICKED(IDC_POSE3_SHOW_PATH4, &CPose3ResultDialog::OnBnClickedPose3ShowPath4)
	ON_BN_CLICKED(IDC_POSE3_SHOW_PATH5, &CPose3ResultDialog::OnBnClickedPose3ShowPath5)
	ON_BN_CLICKED(IDC_POSE3_SHOW_PATH6, &CPose3ResultDialog::OnBnClickedPose3ShowPath6)
	ON_BN_CLICKED(IDC_POSE3_SHOW_PATH7, &CPose3ResultDialog::OnBnClickedPose3ShowPath7)
	ON_BN_CLICKED(IDC_POSE3_SHOW_PATH8, &CPose3ResultDialog::OnBnClickedPose3ShowPath8)
	ON_BN_CLICKED(IDC_POSE3_SHOW_PATH_ALL, &CPose3ResultDialog::OnBnClickedPose3ShowPathAll)
	ON_BN_CLICKED(IDC_POSE3_SHOW_PATH_NONE, &CPose3ResultDialog::OnBnClickedPose3ShowPathNone)
	ON_BN_CLICKED(IDC_POSE3_EXPORT_RESULT, &CPose3ResultDialog::OnBnClickedPose3ExportResult)
END_MESSAGE_MAP()


// CPose3ResultDialog 메시지 처리기

CSizeKorea2020View *CPose3ResultDialog::GetView()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020View *)pFrame->GetActiveView();
}

CSizeKorea2020Doc *CPose3ResultDialog::GetDocument()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020Doc *)pFrame->GetActiveDocument();
}

void CPose3ResultDialog::OnBnClickedPose3ShowPath1()
{
	theSizer.m_bShowPaths[0] = !theSizer.m_bShowPaths[0];
	GetView()->Invalidate(FALSE);
}


void CPose3ResultDialog::OnBnClickedPose3ShowPath2()
{
	theSizer.m_bShowPaths[1] = !theSizer.m_bShowPaths[1];
	GetView()->Invalidate(FALSE);
}


void CPose3ResultDialog::OnBnClickedPose3ShowPath3()
{
	theSizer.m_bShowPaths[2] = !theSizer.m_bShowPaths[2];
	GetView()->Invalidate(FALSE);
}


void CPose3ResultDialog::OnBnClickedPose3ShowPath4()
{
	theSizer.m_bShowPaths[3] = !theSizer.m_bShowPaths[3];
	GetView()->Invalidate(FALSE);
}


void CPose3ResultDialog::OnBnClickedPose3ShowPath5()
{
	theSizer.m_bShowPaths[4] = !theSizer.m_bShowPaths[4];
	GetView()->Invalidate(FALSE);
}
	

void CPose3ResultDialog::OnBnClickedPose3ShowPath6()
{
	theSizer.m_bShowPaths[5] = !theSizer.m_bShowPaths[5];
	GetView()->Invalidate(FALSE);
}

void CPose3ResultDialog::OnBnClickedPose3ShowPath7()
{
	theSizer.m_bShowPaths[6] = !theSizer.m_bShowPaths[6];
	GetView()->Invalidate(FALSE);
}

void CPose3ResultDialog::OnBnClickedPose3ShowPath8()
{
	theSizer.m_bShowPaths[7] = !theSizer.m_bShowPaths[7];
	GetView()->Invalidate(FALSE);
}

void CPose3ResultDialog::OnBnClickedPose3ShowPathAll()
{
	m_bPose3ShowPath1 = true;
	m_bPose3ShowPath2 = true;
	m_bPose3ShowPath3 = true;
	m_bPose3ShowPath4 = true;
	m_bPose3ShowPath5 = true;
	m_bPose3ShowPath6 = true;
	m_bPose3ShowPath7 = true;
	m_bPose3ShowPath8 = true;
	for (int i = 0; i < NUM(theSizer.m_bShowPaths); ++i)
		theSizer.m_bShowPaths[i] = true;
	UpdateData(FALSE);
	GetView()->Invalidate(FALSE);
}

void CPose3ResultDialog::OnBnClickedPose3ShowPathNone()
{
	m_bPose3ShowPath1 = false;
	m_bPose3ShowPath2 = false;
	m_bPose3ShowPath3 = false;
	m_bPose3ShowPath4 = false;
	m_bPose3ShowPath5 = false;
	m_bPose3ShowPath6 = false;
	m_bPose3ShowPath7 = false;
	m_bPose3ShowPath8 = false;
	for (int i = 0; i < NUM(theSizer.m_bShowPaths); ++i)
		theSizer.m_bShowPaths[i] = false;
	UpdateData(FALSE);
	GetView()->Invalidate(FALSE);
}

void CPose3ResultDialog::OnBnClickedPose3ExportResult()
{
	CString strFileName;
	LPCTSTR filter = "Measurement file(ascii)(*.csv)|*.csv||";
	CFileDialog FileDlg(false, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (FileDlg.DoModal() != IDOK)
		return;

	strFileName = FileDlg.GetPathName(); // get file path.
	if (strFileName.Find(".csv") == -1)
		strFileName += ".csv";

	FILE *fp;
	fopen_s(&fp, strFileName, "w");
	int NumResults = (int)theSizer.m_Results.size();
	for (int i = 0; i < NumResults; i++)
		fprintf(fp, "%s,%.1lf\n", ResultPose3[i].c_str(), theSizer.m_Results[i]);
	fclose(fp);
}
