// CPose4InfoDialog.cpp: 구현 파일
//

#include "pch.h"
#include "SizeKorea-2020.h"
#include "CPose4InfoDialog.h"
#include "MainFrm.h"

extern EgSizer theSizer;

// CPose4InfoDialog

IMPLEMENT_DYNAMIC(CPose4InfoDialog, CPaneDialog)

CPose4InfoDialog::CPose4InfoDialog()
{
	m_SltButtonId = -1;
	m_Pose4MarkCoords = "(0.0, 0.0, 0.0)";
}

CPose4InfoDialog::~CPose4InfoDialog()
{
}


BEGIN_MESSAGE_MAP(CPose4InfoDialog, CPaneDialog)
	ON_BN_CLICKED(IDC_P4_HT, &CPose4InfoDialog::OnBnClickedP4HT)
	ON_BN_CLICKED(IDC_P4_ECTOCANTHUS, &CPose4InfoDialog::OnBnClickedP4Ectocanthus)
	ON_BN_CLICKED(IDC_P4_CERVICALE, &CPose4InfoDialog::OnBnClickedP4Cervicale)
	ON_BN_CLICKED(IDC_P4_LSR, &CPose4InfoDialog::OnBnClickedP4LSR)
	ON_BN_CLICKED(IDC_P4_LSL, &CPose4InfoDialog::OnBnClickedP4LSL)
	ON_BN_CLICKED(IDC_P4_AR, &CPose4InfoDialog::OnBnClickedP4AR)
	ON_BN_CLICKED(IDC_P4_AL, &CPose4InfoDialog::OnBnClickedP4AL)
	ON_BN_CLICKED(IDC_P4_BP, &CPose4InfoDialog::OnBnClickedP4BP)
	ON_BN_CLICKED(IDC_P4_CO, &CPose4InfoDialog::OnBnClickedP4CO)
	ON_BN_CLICKED(IDC_P4_US, &CPose4InfoDialog::OnBnClickedP4US)
	ON_BN_CLICKED(IDC_P4_DACTYLION, &CPose4InfoDialog::OnBnClickedP4Dactylion)
	ON_BN_CLICKED(IDC_P4_BS, &CPose4InfoDialog::OnBnClickedP4BS)
	ON_BN_CLICKED(IDC_P4_STS, &CPose4InfoDialog::OnBnClickedP4STS)
	ON_BN_CLICKED(IDC_P4_SPS, &CPose4InfoDialog::OnBnClickedP4SPS)
	ON_BN_CLICKED(IDC_P4_PJCT, &CPose4InfoDialog::OnBnClickedP4PJCT)
	ON_BN_CLICKED(IDC_P4_DR, &CPose4InfoDialog::OnBnClickedP4DR)
	ON_BN_CLICKED(IDC_P4_DL, &CPose4InfoDialog::OnBnClickedP4DL)
	ON_BN_CLICKED(IDC_P4_LHER, &CPose4InfoDialog::OnBnClickedP4LHER)
	ON_BN_CLICKED(IDC_P4_LHEL, &CPose4InfoDialog::OnBnClickedP4LHEL)
	ON_BN_CLICKED(IDC_P4_MHWR, &CPose4InfoDialog::OnBnClickedP4MHWR)
	ON_BN_CLICKED(IDC_P4_MHWL, &CPose4InfoDialog::OnBnClickedP4MHWL)
	ON_BN_CLICKED(IDC_P4_AKS, &CPose4InfoDialog::OnBnClickedP4AKS)
	ON_BN_CLICKED(IDC_P4_ROS, &CPose4InfoDialog::OnBnClickedP4ROS)
	ON_BN_CLICKED(IDC_P4_BOS, &CPose4InfoDialog::OnBnClickedP4BOS)
	ON_BN_CLICKED(IDC_P4_APS, &CPose4InfoDialog::OnBnClickedP4APS)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


CSizeKorea2020View *CPose4InfoDialog::GetView() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020View *)pFrame->GetActiveView();
}

CSizeKorea2020Doc *CPose4InfoDialog::GetDocument() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020Doc *)pFrame->GetActiveDocument();
}

void CPose4InfoDialog::DoDataExchange(CDataExchange * pDX)
{
	CPaneDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_P4_MARK_POS, m_Pose4MarkCoords);
}

// CPose4InfoDialog 메시지 처리기
void CPose4InfoDialog::OnBnClickedP4HT()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 0;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.707107, 0.0, 0.0, 0.707107);
	pView->m_OffSet[0] = -221.892944;
	pView->m_OffSet[1] = -1120.0;
	pView->m_OffSet[2] = -148.845856;
	pView->m_TrackBallCenter[2] = -1100.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_HT;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4Ectocanthus()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 1;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);
	pView->m_OffSet[0] = -212.524750;
	pView->m_OffSet[1] = -1035.680908;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -1100.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_ECTOCANTHUS;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4Cervicale()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 2;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);
	pView->m_OffSet[0] = -226.850006;
	pView->m_OffSet[1] = -1021.813843;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -1400.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_CERVICALE;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4LSR()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 3;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.349421, 0.0, 0.0, 0.936966);
	pView->m_OffSet[0] = -220.481140;
	pView->m_OffSet[1] = -853.810730;
	pView->m_OffSet[2] = 22.642029;
	pView->m_TrackBallCenter[2] = -1000.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_LSR;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4LSL()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 4;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.349421, 0.0, 0.0, 0.936966);
	pView->m_OffSet[0] = -220.481140;
	pView->m_OffSet[1] = -853.810730;
	pView->m_OffSet[2] = 22.642029;
	pView->m_TrackBallCenter[2] = -1000.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_LSL;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4AR()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 5;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.349421, 0.0, 0.0, 0.936966);
	pView->m_OffSet[0] = -220.481140;
	pView->m_OffSet[1] = -853.810730;
	pView->m_OffSet[2] = 22.642029;
	pView->m_TrackBallCenter[2] = -1000.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_AR;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4AL()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 6;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.349421, 0.0, 0.0, 0.936966);
	pView->m_OffSet[0] = -220.481140;
	pView->m_OffSet[1] = -853.810730;
	pView->m_OffSet[2] = 22.642029;
	pView->m_TrackBallCenter[2] = -1000.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_AL;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4BP()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 7;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);
	pView->m_OffSet[0] = -320.0;
	pView->m_OffSet[1] = -500.0;
	pView->m_OffSet[2] = 130.0;
	pView->m_TrackBallCenter[2] = -1130.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_BP;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4CO()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 8;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.889633, 0.0, 0.456676);
	pView->m_OffSet[0] = -80.0;
	pView->m_OffSet[1] = -800.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -1120.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_CO;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4US()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 9;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.707107, 0.0, 0.707107);
	pView->m_OffSet[0] = -80.0;
	pView->m_OffSet[1] = -750.0;
	pView->m_OffSet[2] = -100.0;
	pView->m_TrackBallCenter[2] = -800.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_US;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4Dactylion()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 10;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.707107, 0.0, 0.707107);
	pView->m_OffSet[0] = -80.0;
	pView->m_OffSet[1] = -750.0;
	pView->m_OffSet[2] = -100.0;
	pView->m_TrackBallCenter[2] = -800.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_DACTYLION;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4BS()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 11;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.707107, 0.0, 0.707107);
	pView->m_OffSet[0] = -410.0;
	pView->m_OffSet[1] = -550.0;
	pView->m_OffSet[2] = -80.0;
	pView->m_TrackBallCenter[2] = -1900.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_BS;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4STS()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 12;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.189123, 0.0, 0.0, 0.9811953);
	pView->m_OffSet[0] = -220.0;
	pView->m_OffSet[1] = -550.0;
	pView->m_OffSet[2] = -20.0;
	pView->m_TrackBallCenter[2] = -1500.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_STS;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4SPS()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 13;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.189123, 0.0, 0.0, 0.9811953);
	pView->m_OffSet[0] = -220.0;
	pView->m_OffSet[1] = -550.0;
	pView->m_OffSet[2] = -20.0;
	pView->m_TrackBallCenter[2] = -1500.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_SPS;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4PJCT()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 14;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.707107, 0.0, 0.707107);
	pView->m_OffSet[0] = -200.0;
	pView->m_OffSet[1] = -430.0;
	pView->m_OffSet[2] = -180.0;
	pView->m_TrackBallCenter[2] = -1800.0;
	pView->Invalidate(false);
	
	m_SltButtonId = IDC_P4_PJCT;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4DR()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 15;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.707107, 0.0, 0.707107);
	pView->m_OffSet[0] = -203.0;
	pView->m_OffSet[1] = -960.0;
	pView->m_OffSet[2] = 20.0;
	pView->m_TrackBallCenter[2] = -1430.0;
	pView->Invalidate(false);
	
	m_SltButtonId = IDC_P4_DR;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4DL()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 16;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, -0.707107, 0.0, 0.707107);
	pView->m_OffSet[0] = -200.0;
	pView->m_OffSet[1] = -940.0;
	pView->m_OffSet[2] = 20.0;
	pView->m_TrackBallCenter[2] = -1900.0;
	pView->Invalidate(false);
	
	m_SltButtonId = IDC_P4_DL;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4LHER()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 17;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.889633, 0.0, 0.456676);
	pView->m_OffSet[0] = -80.0;
	pView->m_OffSet[1] = -800.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -1120.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_LHER;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4LHEL()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 18;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, -0.707107, 0.0, 0.707107);
	pView->m_OffSet[0] = -203.0;
	pView->m_OffSet[1] = -808.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -1800.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_LHEL;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4MHWR()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 19;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.707107, 0.0, 0.707107);
	pView->m_OffSet[0] = -203.0;
	pView->m_OffSet[1] = -545.0;
	pView->m_OffSet[2] = -30.0;
	pView->m_TrackBallCenter[2] = -2500.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_MHWR;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4MHWL()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 20;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, -0.707107, 0.0, 0.707107);
	pView->m_OffSet[0] = -203.0;
	pView->m_OffSet[1] = -600.0;
	pView->m_OffSet[2] = 63.0;
	pView->m_TrackBallCenter[2] = -2300.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_MHWL;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4AKS()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 21;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.189123, 0.0, 0.0, 0.9811953);
	pView->m_OffSet[0] = -220.0;
	pView->m_OffSet[1] = -550.0;
	pView->m_OffSet[2] = -20.0;
	pView->m_TrackBallCenter[2] = -1500.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_AKS;
	Invalidate(false);
	pView->SetFocus();
}


void CPose4InfoDialog::OnBnClickedP4ROS()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 22;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.889633, 0.0, 0.456676);
	pView->m_OffSet[0] = -80.0;
	pView->m_OffSet[1] = -800.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -1120.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_ROS;
	Invalidate(false);
	pView->SetFocus();
}

void CPose4InfoDialog::OnBnClickedP4BOS()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 23;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.889633, 0.0, 0.456676);
	pView->m_OffSet[0] = -80.0;
	pView->m_OffSet[1] = -800.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -1120.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_BOS;
	Invalidate(false);
	pView->SetFocus();
}

void CPose4InfoDialog::OnBnClickedP4APS()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 24;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose4MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);
	pView->m_OffSet[0] = -203.0;
	pView->m_OffSet[1] = -700.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -2000.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P4_APS;
	Invalidate(false);
	pView->SetFocus();
}

void CPose4InfoDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if ((nIDCtl == m_SltButtonId))
	{
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct->hDC);                //버튼의 dc구하기
		rect = lpDrawItemStruct->rcItem;                   //버튼영역 구하기
		dc.Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0));  //버튼의 외각선 그리기
		dc.FillSolidRect(&rect, RGB(51, 51, 51));             //버튼색상
		UINT state = lpDrawItemStruct->itemState;         //버튼상태구하기
		if ((state &ODS_SELECTED))
		{
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		}
		else
		{
			dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
		}
		dc.SetBkColor(RGB(51, 51, 51));                       //text의 백그라운드 색상
		dc.SetTextColor(RGB(255, 255, 255));                     //texttort
		TCHAR buffer[MAX_PATH];                            //버튼의 text를 얻기위한 임시버퍼
		ZeroMemory(buffer, MAX_PATH);                       //버퍼초기화
		::GetWindowTextA(lpDrawItemStruct->hwndItem, buffer, MAX_PATH); //버튼의 text얻기
		dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE); //버튼의 text넣기
		dc.Detach();                                                  //버튼의 dc 풀어주기
	}
	else
	{
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct->hDC);                //버튼의 dc구하기
		rect = lpDrawItemStruct->rcItem;                   //버튼영역 구하기
		dc.Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0));  //버튼의 외각선 그리기
		dc.FillSolidRect(&rect, RGB(240, 240, 240));             //버튼색상
		UINT state = lpDrawItemStruct->itemState;         //버튼상태구하기
		if ((state &ODS_SELECTED))
		{
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		}
		else
		{
			dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
		}
		dc.SetBkColor(RGB(240, 240, 240));                       //text의 백그라운드 색상
		dc.SetTextColor(RGB(0, 0, 0));                     //texttort
		TCHAR buffer[MAX_PATH];                            //버튼의 text를 얻기위한 임시버퍼
		ZeroMemory(buffer, MAX_PATH);                       //버퍼초기화
		::GetWindowTextA(lpDrawItemStruct->hwndItem, buffer, MAX_PATH); //버튼의 text얻기
		dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE); //버튼의 text넣기
		dc.Detach();
	}
	CPaneDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
