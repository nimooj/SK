// CPose4ResultDialog.cpp: 구현 파일
//

#include "pch.h"
#include "SizeKorea-2020.h"
#include "CPose4ResultDialog.h"
#include "afxdialogex.h"
#include "EgSizer.h"
#include "MainFrm.h"


extern EgSizer theSizer;
extern std::vector<std::string> ResultPose4;

// CPose4ResultDialog 대화 상자

IMPLEMENT_DYNAMIC(CPose4ResultDialog, CDialogEx)

CPose4ResultDialog::CPose4ResultDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POSE4_RESULT_DIALOG, pParent)
{

}

CPose4ResultDialog::~CPose4ResultDialog()
{
}

void CPose4ResultDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POSE4_RESULT1, m_Pose4Result1);
	DDX_Text(pDX, IDC_POSE4_RESULT2, m_Pose4Result2);
	DDX_Text(pDX, IDC_POSE4_RESULT3, m_Pose4Result3);
	DDX_Text(pDX, IDC_POSE4_RESULT4, m_Pose4Result4);
	DDX_Text(pDX, IDC_POSE4_RESULT5, m_Pose4Result5);
	DDX_Text(pDX, IDC_POSE4_RESULT6, m_Pose4Result6);
	DDX_Text(pDX, IDC_POSE4_RESULT7, m_Pose4Result7);
	DDX_Text(pDX, IDC_POSE4_RESULT8, m_Pose4Result8);
	DDX_Text(pDX, IDC_POSE4_RESULT9, m_Pose4Result9);
	DDX_Text(pDX, IDC_POSE4_RESULT10, m_Pose4Result10);
	DDX_Text(pDX, IDC_POSE4_RESULT11, m_Pose4Result11);
	DDX_Text(pDX, IDC_POSE4_RESULT12, m_Pose4Result12);
	DDX_Text(pDX, IDC_POSE4_RESULT13, m_Pose4Result13);
	DDX_Text(pDX, IDC_POSE4_RESULT14, m_Pose4Result14);
	DDX_Text(pDX, IDC_POSE4_RESULT15, m_Pose4Result15);
	DDX_Text(pDX, IDC_POSE4_RESULT16, m_Pose4Result16);
	DDX_Text(pDX, IDC_POSE4_RESULT17, m_Pose4Result17);
	DDX_Text(pDX, IDC_POSE4_RESULT18, m_Pose4Result18);
	DDX_Text(pDX, IDC_POSE4_RESULT19, m_Pose4Result19);
	DDX_Text(pDX, IDC_POSE4_RESULT20, m_Pose4Result20);
	DDX_Text(pDX, IDC_POSE4_RESULT21, m_Pose4Result21);
	DDX_Text(pDX, IDC_POSE4_RESULT22, m_Pose4Result22);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH1, m_bPose4ShowPath1);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH2, m_bPose4ShowPath2);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH3, m_bPose4ShowPath3);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH4, m_bPose4ShowPath4);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH5, m_bPose4ShowPath5);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH6, m_bPose4ShowPath6);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH7, m_bPose4ShowPath7);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH8, m_bPose4ShowPath8);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH9, m_bPose4ShowPath9);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH10, m_bPose4ShowPath10);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH11, m_bPose4ShowPath11);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH12, m_bPose4ShowPath12);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH13, m_bPose4ShowPath13);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH14, m_bPose4ShowPath14);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH15, m_bPose4ShowPath15);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH16, m_bPose4ShowPath16);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH17, m_bPose4ShowPath17);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH18, m_bPose4ShowPath18);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH19, m_bPose4ShowPath19);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH20, m_bPose4ShowPath20);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH21, m_bPose4ShowPath21);
	DDX_Check(pDX, IDC_POSE4_SHOW_PATH22, m_bPose4ShowPath22);
}


BEGIN_MESSAGE_MAP(CPose4ResultDialog, CDialogEx)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH1, &CPose4ResultDialog::OnBnClickedPose4ShowPath1)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH2, &CPose4ResultDialog::OnBnClickedPose4ShowPath2)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH3, &CPose4ResultDialog::OnBnClickedPose4ShowPath3)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH4, &CPose4ResultDialog::OnBnClickedPose4ShowPath4)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH5, &CPose4ResultDialog::OnBnClickedPose4ShowPath5)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH6, &CPose4ResultDialog::OnBnClickedPose4ShowPath6)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH7, &CPose4ResultDialog::OnBnClickedPose4ShowPath7)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH8, &CPose4ResultDialog::OnBnClickedPose4ShowPath8)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH9, &CPose4ResultDialog::OnBnClickedPose4ShowPath9)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH10, &CPose4ResultDialog::OnBnClickedPose4ShowPath10)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH11, &CPose4ResultDialog::OnBnClickedPose4ShowPath11)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH12, &CPose4ResultDialog::OnBnClickedPose4ShowPath12)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH13, &CPose4ResultDialog::OnBnClickedPose4ShowPath13)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH14, &CPose4ResultDialog::OnBnClickedPose4ShowPath14)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH15, &CPose4ResultDialog::OnBnClickedPose4ShowPath15)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH16, &CPose4ResultDialog::OnBnClickedPose4ShowPath16)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH17, &CPose4ResultDialog::OnBnClickedPose4ShowPath17)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH18, &CPose4ResultDialog::OnBnClickedPose4ShowPath18)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH19, &CPose4ResultDialog::OnBnClickedPose4ShowPath19)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH20, &CPose4ResultDialog::OnBnClickedPose4ShowPath20)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH21, &CPose4ResultDialog::OnBnClickedPose4ShowPath21)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH22, &CPose4ResultDialog::OnBnClickedPose4ShowPath22)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH_ALL, &CPose4ResultDialog::OnBnClickedPose4ShowPathAll)
	ON_BN_CLICKED(IDC_POSE4_SHOW_PATH_NONE, &CPose4ResultDialog::OnBnClickedPose4ShowPathNone)
	ON_BN_CLICKED(IDC_POSE4_EXPORT_RESULT, &CPose4ResultDialog::OnBnClickedPose4ExportResult)
END_MESSAGE_MAP()


// CPose4ResultDialog 메시지 처리기

CSizeKorea2020View *CPose4ResultDialog::GetView()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020View *)pFrame->GetActiveView();
}

CSizeKorea2020Doc *CPose4ResultDialog::GetDocument()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020Doc *)pFrame->GetActiveDocument();
}


void CPose4ResultDialog::InitDialog()
{
	m_bPose4ShowPath1 = theSizer.m_bShowPaths[0];
	m_bPose4ShowPath2 = theSizer.m_bShowPaths[1];
	m_bPose4ShowPath3 = theSizer.m_bShowPaths[2];
	m_bPose4ShowPath4 = theSizer.m_bShowPaths[3];
	m_bPose4ShowPath5 = theSizer.m_bShowPaths[4];
	m_bPose4ShowPath6 = theSizer.m_bShowPaths[5];
	m_bPose4ShowPath7 = theSizer.m_bShowPaths[6];
	m_bPose4ShowPath8 = theSizer.m_bShowPaths[7];
	m_bPose4ShowPath9 = theSizer.m_bShowPaths[8];
	m_bPose4ShowPath10 = theSizer.m_bShowPaths[9];
	m_bPose4ShowPath11 = theSizer.m_bShowPaths[10];
	m_bPose4ShowPath12 = theSizer.m_bShowPaths[11];
	m_bPose4ShowPath13 = theSizer.m_bShowPaths[12];
	m_bPose4ShowPath14 = theSizer.m_bShowPaths[13];
	m_bPose4ShowPath15 = theSizer.m_bShowPaths[14];
	m_bPose4ShowPath16 = theSizer.m_bShowPaths[15];
	m_bPose4ShowPath17 = theSizer.m_bShowPaths[16];
	m_bPose4ShowPath18 = theSizer.m_bShowPaths[17];
	m_bPose4ShowPath19 = theSizer.m_bShowPaths[18];
	m_bPose4ShowPath20 = theSizer.m_bShowPaths[19];
	m_bPose4ShowPath21 = theSizer.m_bShowPaths[20];
	m_bPose4ShowPath22 = theSizer.m_bShowPaths[21];
	UpdateData(FALSE);
	GetView()->Invalidate(false);
}

void CPose4ResultDialog::OnBnClickedPose4ShowPath1()
{
	theSizer.m_bShowPaths[0] = !theSizer.m_bShowPaths[0];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath2()
{
	theSizer.m_bShowPaths[1] = !theSizer.m_bShowPaths[1];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath3()
{
	theSizer.m_bShowPaths[2] = !theSizer.m_bShowPaths[2];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath4()
{
	theSizer.m_bShowPaths[3] = !theSizer.m_bShowPaths[3];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath5()
{
	theSizer.m_bShowPaths[4] = !theSizer.m_bShowPaths[4];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath6()
{
	theSizer.m_bShowPaths[5] = !theSizer.m_bShowPaths[5];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath7()
{
	theSizer.m_bShowPaths[6] = !theSizer.m_bShowPaths[6];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath8()
{
	theSizer.m_bShowPaths[7] = !theSizer.m_bShowPaths[7];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath9()
{
	theSizer.m_bShowPaths[8] = !theSizer.m_bShowPaths[8];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath10()
{
	theSizer.m_bShowPaths[9] = !theSizer.m_bShowPaths[9];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath11()
{
	theSizer.m_bShowPaths[10] = !theSizer.m_bShowPaths[10];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath12()
{
	theSizer.m_bShowPaths[11] = !theSizer.m_bShowPaths[11];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath13()
{
	theSizer.m_bShowPaths[12] = !theSizer.m_bShowPaths[12];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath14()
{
	theSizer.m_bShowPaths[13] = !theSizer.m_bShowPaths[13];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath15()
{
	theSizer.m_bShowPaths[14] = !theSizer.m_bShowPaths[14];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath16()
{
	theSizer.m_bShowPaths[15] = !theSizer.m_bShowPaths[15];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath17()
{
	theSizer.m_bShowPaths[16] = !theSizer.m_bShowPaths[16];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath18()
{
	theSizer.m_bShowPaths[17] = !theSizer.m_bShowPaths[17];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath19()
{
	theSizer.m_bShowPaths[18] = !theSizer.m_bShowPaths[18];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath20()
{
	theSizer.m_bShowPaths[19] = !theSizer.m_bShowPaths[19];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath21()
{
	theSizer.m_bShowPaths[20] = !theSizer.m_bShowPaths[20];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPath22()
{
	theSizer.m_bShowPaths[21] = !theSizer.m_bShowPaths[21];
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPathAll()
{
	m_bPose4ShowPath1 = true;
	m_bPose4ShowPath2 = true;
	m_bPose4ShowPath3 = true;
	m_bPose4ShowPath4 = true;
	m_bPose4ShowPath5 = true;
	m_bPose4ShowPath6 = true;
	m_bPose4ShowPath7 = true;
	m_bPose4ShowPath8 = true;
	m_bPose4ShowPath9 = true;
	m_bPose4ShowPath10 = true;
	m_bPose4ShowPath11 = true;
	m_bPose4ShowPath12 = true;
	m_bPose4ShowPath13 = true;
	m_bPose4ShowPath14 = true;
	m_bPose4ShowPath15 = true;
	m_bPose4ShowPath16 = true;
	m_bPose4ShowPath17 = true;
	m_bPose4ShowPath18 = true;
	m_bPose4ShowPath19 = true;
	m_bPose4ShowPath20 = true;
	m_bPose4ShowPath21 = true;
	m_bPose4ShowPath22 = true;
	for (int i = 0; i < NUM(theSizer.m_bShowPaths); ++i)
		theSizer.m_bShowPaths[i] = true;
	UpdateData(FALSE);
	GetView()->Invalidate(FALSE);
}


void CPose4ResultDialog::OnBnClickedPose4ShowPathNone()
{
	m_bPose4ShowPath1 = false;
	m_bPose4ShowPath2 = false;
	m_bPose4ShowPath3 = false;
	m_bPose4ShowPath4 = false;
	m_bPose4ShowPath5 = false;
	m_bPose4ShowPath6 = false;
	m_bPose4ShowPath7 = false;
	m_bPose4ShowPath8 = false;
	m_bPose4ShowPath9 = false;
	m_bPose4ShowPath10 = false;
	m_bPose4ShowPath11 = false;
	m_bPose4ShowPath12 = false;
	m_bPose4ShowPath13 = false;
	m_bPose4ShowPath14 = false;
	m_bPose4ShowPath15 = false;
	m_bPose4ShowPath16 = false;
	m_bPose4ShowPath17 = false;
	m_bPose4ShowPath18 = false;
	m_bPose4ShowPath19 = false;
	m_bPose4ShowPath20 = false;
	m_bPose4ShowPath21 = false;
	m_bPose4ShowPath22 = false;
	for (int i = 0; i < NUM(theSizer.m_bShowPaths); ++i)
		theSizer.m_bShowPaths[i] = false;
	UpdateData(FALSE);
	GetView()->Invalidate(FALSE);
}

void CPose4ResultDialog::OnBnClickedPose4ExportResult()
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
		fprintf(fp, "%s,%.1lf\n", ResultPose4[i].c_str(), theSizer.m_Results[i]);
	fclose(fp);
}
