// CPose1InfoDialog.cpp: 구현 파일
#include "pch.h"
#include "SizeKorea-2020.h"
#include "MainFrm.h"
#include "CPose1InfoDialog.h"

extern EgSizer theSizer;
extern EgVertex* m_LandmarkPose1[LANDMARK_NUM_POSE1];

// CPose1InfoDialog
IMPLEMENT_DYNAMIC(CPose1InfoDialog, CPaneDialog)

CPose1InfoDialog::CPose1InfoDialog()
{
	m_SltButtonId = -1;
	m_Pose1MarkCoords = "(0.0, 0.0, 0.0)";
}

CPose1InfoDialog::~CPose1InfoDialog()
{
}

BEGIN_MESSAGE_MAP(CPose1InfoDialog, CPaneDialog)
	ON_BN_CLICKED(IDC_POSE1_MARK1, &CPose1InfoDialog::OnBnClickedPose1Mark1)
	ON_BN_CLICKED(IDC_POSE1_MARK2, &CPose1InfoDialog::OnBnClickedPose1Mark2)
	ON_BN_CLICKED(IDC_POSE1_MARK3, &CPose1InfoDialog::OnBnClickedPose1Mark3)
	ON_BN_CLICKED(IDC_POSE1_MARK4, &CPose1InfoDialog::OnBnClickedPose1Mark4)
	ON_BN_CLICKED(IDC_POSE1_MARK5, &CPose1InfoDialog::OnBnClickedPose1Mark5)
	ON_BN_CLICKED(IDC_POSE1_MARK6, &CPose1InfoDialog::OnBnClickedPose1Mark6)
	ON_BN_CLICKED(IDC_POSE1_MARK7, &CPose1InfoDialog::OnBnClickedPose1Mark7)
	ON_BN_CLICKED(IDC_POSE1_MARK8, &CPose1InfoDialog::OnBnClickedPose1Mark8)
	ON_BN_CLICKED(IDC_POSE1_MARK9, &CPose1InfoDialog::OnBnClickedPose1Mark9)
	ON_BN_CLICKED(IDC_POSE1_MARK10, &CPose1InfoDialog::OnBnClickedPose1Mark10)
	ON_BN_CLICKED(IDC_POSE1_MARK11, &CPose1InfoDialog::OnBnClickedPose1Mark11)
	ON_BN_CLICKED(IDC_POSE1_MARK12, &CPose1InfoDialog::OnBnClickedPose1Mark12)
	ON_BN_CLICKED(IDC_POSE1_MARK13, &CPose1InfoDialog::OnBnClickedPose1Mark13)
	ON_BN_CLICKED(IDC_POSE1_MARK14, &CPose1InfoDialog::OnBnClickedPose1Mark14)
	ON_BN_CLICKED(IDC_POSE1_MARK15, &CPose1InfoDialog::OnBnClickedPose1Mark15)
	ON_BN_CLICKED(IDC_POSE1_MARK16, &CPose1InfoDialog::OnBnClickedPose1Mark16)
	ON_BN_CLICKED(IDC_POSE1_MARK17, &CPose1InfoDialog::OnBnClickedPose1Mark17)
	ON_BN_CLICKED(IDC_POSE1_MARK18, &CPose1InfoDialog::OnBnClickedPose1Mark18)
	ON_BN_CLICKED(IDC_POSE1_MARK19, &CPose1InfoDialog::OnBnClickedPose1Mark19)
	ON_BN_CLICKED(IDC_POSE1_MARK20, &CPose1InfoDialog::OnBnClickedPose1Mark20)
	ON_BN_CLICKED(IDC_POSE1_MARK21, &CPose1InfoDialog::OnBnClickedPose1Mark21)
	ON_BN_CLICKED(IDC_POSE1_MARK22, &CPose1InfoDialog::OnBnClickedPose1Mark22)
	ON_BN_CLICKED(IDC_POSE1_MARK23, &CPose1InfoDialog::OnBnClickedPose1Mark23)
	ON_BN_CLICKED(IDC_POSE1_MARK24, &CPose1InfoDialog::OnBnClickedPose1Mark24)
	ON_BN_CLICKED(IDC_POSE1_MARK25, &CPose1InfoDialog::OnBnClickedPose1Mark25)
	ON_BN_CLICKED(IDC_POSE1_MARK26, &CPose1InfoDialog::OnBnClickedPose1Mark26)
	ON_BN_CLICKED(IDC_POSE1_MARK27, &CPose1InfoDialog::OnBnClickedPose1Mark27)
	ON_BN_CLICKED(IDC_POSE1_MARK28, &CPose1InfoDialog::OnBnClickedPose1Mark28)
	ON_BN_CLICKED(IDC_POSE1_MARK29, &CPose1InfoDialog::OnBnClickedPose1Mark29)
	ON_BN_CLICKED(IDC_POSE1_MARK30, &CPose1InfoDialog::OnBnClickedPose1Mark30)
	ON_BN_CLICKED(IDC_POSE1_MARK31, &CPose1InfoDialog::OnBnClickedPose1Mark31)
	ON_BN_CLICKED(IDC_POSE1_MARK32, &CPose1InfoDialog::OnBnClickedPose1Mark32)
	ON_BN_CLICKED(IDC_POSE1_MARK33, &CPose1InfoDialog::OnBnClickedPose1Mark33)
	ON_BN_CLICKED(IDC_POSE1_MARK34, &CPose1InfoDialog::OnBnClickedPose1Mark34)
	ON_BN_CLICKED(IDC_POSE1_MARK35, &CPose1InfoDialog::OnBnClickedPose1Mark35)
	ON_BN_CLICKED(IDC_POSE1_MARK36, &CPose1InfoDialog::OnBnClickedPose1Mark36)
	ON_BN_CLICKED(IDC_POSE1_MARK37, &CPose1InfoDialog::OnBnClickedPose1Mark37)
	ON_BN_CLICKED(IDC_POSE1_MARK38, &CPose1InfoDialog::OnBnClickedPose1Mark38)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_POSE1_MARK39, &CPose1InfoDialog::OnBnClickedPose1Mark39)
	ON_BN_CLICKED(IDC_POSE1_MARK40, &CPose1InfoDialog::OnBnClickedPose1Mark40)
	ON_BN_CLICKED(IDC_POSE1_MARK41, &CPose1InfoDialog::OnBnClickedPose1Mark41)
	ON_BN_CLICKED(IDC_POSE1_MARK42, &CPose1InfoDialog::OnBnClickedPose1Mark42)
END_MESSAGE_MAP()

CSizeKorea2020View *CPose1InfoDialog::GetView() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020View *)pFrame->GetActiveView();
}

CSizeKorea2020Doc* CPose1InfoDialog::GetDocument()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020Doc *)pFrame->GetActiveDocument();
}

void CPose1InfoDialog::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class

	CPaneDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POSE1_MARK_POS, m_Pose1MarkCoords);
}

void CPose1InfoDialog::OnBnClickedPose1Mark1()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_TOPHEAD;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);
	pView->m_OffSet[0] = -308.895569;
	pView->m_OffSet[1] = -1654.735474;
	pView->m_OffSet[2] = 21.000000;
	pView->m_TrackBallCenter[2] = -815.390320;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK1;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark2()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_EYE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -266.291321;
	pView->m_OffSet[1] = -1604.484741;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -815.390320;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK2;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark3()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_JAW;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -290.324402;
	pView->m_OffSet[1] = -1496.336426;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -815.390320;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK3;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark4()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_NECK_BACK;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 1.000000, 0.000000, 0.000000);
	pView->m_OffSet[0] = -282.677521;
	pView->m_OffSet[1] = -1471.211060;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -815.390320;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK4;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark5()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_NECK_SIDE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.223501, 0.662542, 0.217432, 0.681035);
	pView->m_OffSet[0] = -152.224762;
	pView->m_OffSet[1] = -1372.032959;
	pView->m_OffSet[2] = -119.502441;
	pView->m_TrackBallCenter[2] = -815.390320;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK5;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark6()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_NECK_FRONT;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -283.769989;
	pView->m_OffSet[1] = -1412.906006;
	pView->m_OffSet[2] = -104.420868;
	pView->m_TrackBallCenter[2] = -815.390320;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK6;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark7()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_SHOULDER_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.229500, 0.668827, 0.229500, 0.668827);
	pView->m_OffSet[0] = -355.980316;
	pView->m_OffSet[1] = -1125.872437;
	pView->m_OffSet[2] = -109.812477;
	pView->m_TrackBallCenter[2] = -1909.782959;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK7;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark8()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_SHOULDER_L;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.324070, -0.628473, -0.324070, 0.628473);
	pView->m_OffSet[0] = -355.980316;
	pView->m_OffSet[1] = -1125.872437;
	pView->m_OffSet[2] = -109.812477;
	pView->m_TrackBallCenter[2] = -1909.782959;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK8;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark9()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_SHOULDERSIDE_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.229500, 0.668827, 0.229500, 0.668827);
	pView->m_OffSet[0] = -355.980316;
	pView->m_OffSet[1] = -1125.872437;
	pView->m_OffSet[2] = -109.812477;
	pView->m_TrackBallCenter[2] = -1909.782959;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK9;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark10()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_SHOULDERSIDE_L;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.324070, -0.628473, -0.324070, 0.628473);
	pView->m_OffSet[0] = -355.980316;
	pView->m_OffSet[1] = -1125.872437;
	pView->m_OffSet[2] = -109.812477;
	pView->m_TrackBallCenter[2] = -1909.782959;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK10;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark11()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_ARMPIT;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.707107, 0.000000, 0.707107);
	pView->m_OffSet[0] = -121.0;
	pView->m_OffSet[1] = -1280.382568;
	pView->m_OffSet[2] = -120.930634;
	pView->m_TrackBallCenter[2] = -643.078064;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK11;
	Invalidate(false);
	pView->SetFocus();
}


void CPose1InfoDialog::OnBnClickedPose1Mark12()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_BP_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -177.863037;
	pView->m_OffSet[1] = -1223.519043;
	pView->m_OffSet[2] = -120.930634;
	pView->m_TrackBallCenter[2] = -643.078064;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK12;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark13()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_BP_L;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -388.945679;
	pView->m_OffSet[1] = -1228.688477;
	pView->m_OffSet[2] = -120.930634;
	pView->m_TrackBallCenter[2] = -643.078064;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK13;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark14()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_UNDERBUST;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -190.600906;
	pView->m_OffSet[1] = -1178.491577;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -643.078064;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK14;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark15()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_WAIST;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -118.229767;
	pView->m_OffSet[1] = -1059.596069;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -643.078064;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK15;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark16()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_NAVEL;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -275.895599;
	pView->m_OffSet[1] = -1038.918945;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -643.078064;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK16;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark17()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_NAVEL_BACK;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 1.000000, 0.000000, 0.000000);
	pView->m_OffSet[0] = -275.895599;
	pView->m_OffSet[1] = -1038.918945;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -643.078064;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK17;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark18()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_STOMACH_EXTRUDE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -271.773132;
	pView->m_OffSet[1] = -1030.876221;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -643.078064;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK18;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark19()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_HIP_EXTRUDE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 1.000000, 0.000000, 0.000000);
	pView->m_OffSet[0] = -272.634552;
	pView->m_OffSet[1] = -842.194153;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -643.078064;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK19;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark20()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_CROTCH;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -266.438416;
	pView->m_OffSet[1] = -802.906555;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -1033.873535;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK20;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark21()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_UNDERHIP;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 1.000000, 0.000000, 0.000000);
	pView->m_OffSet[0] = -290.518585;
	pView->m_OffSet[1] = -792.766846;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -826.792786;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK21;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark22()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_KNEE_MID;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -252.089905;
	pView->m_OffSet[1] = -466.187653;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -815.390320;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK22;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark23()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_KNEE_SIDE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -252.089905;
	pView->m_OffSet[1] = -466.187653;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -815.390320;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK23;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark24()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_CALF_EXTRUDE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 1.000000, 0.000000, 0.000000);
	pView->m_OffSet[0] = -229.149323;
	pView->m_OffSet[1] = -307.787506;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -815.390320;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK24;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark25()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_ANKLE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.707107, 0.000000, 0.707107);
	pView->m_OffSet[0] = -160.327072;
	pView->m_OffSet[1] = -119.892059;
	pView->m_OffSet[2] = -95.888481;
	pView->m_TrackBallCenter[2] = -815.390320;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK25;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark26()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_HAND;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.707107, 0.000000, 0.707107);
	pView->m_OffSet[0] = -80.371109;
	pView->m_OffSet[1] = -788.915894;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -890.745361;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK26;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark27()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_ARMPITWALL_BACK_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 1.000000, 0.000000, 0.000000);
	pView->m_OffSet[0] = -121.378044;
	pView->m_OffSet[1] = -1335.903687;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -1033.873535;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK27;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark28()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_ARMPITWALL_BACK_L;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 1.000000, 0.000000, 0.000000);
	pView->m_OffSet[0] = -395.255371;
	pView->m_OffSet[1] = -1296.012939;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -1033.873535;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK28;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark29()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_ARMPITWALL_FRONT_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -121.378044;
	pView->m_OffSet[1] = -1335.903687;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -1033.873535;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK29;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark30()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_ARMPITWALL_FRONT_L;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -395.255371;
	pView->m_OffSet[1] = -1296.012939;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -1033.873535;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK30;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark31()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_ARMPITFOLD_FRONT_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.118495, 0.383833, 0.049739, 0.914416);
	pView->m_OffSet[0] = -200.336777;
	pView->m_OffSet[1] = -1286.346924;
	pView->m_OffSet[2] = -78.115669;
	pView->m_TrackBallCenter[2] = -1033.873535;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK31;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark32()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_ARMPITFOLD_FRONT_L;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.165518, -0.292796, -0.051538, 0.940329);
	pView->m_OffSet[0] = -285.993622;
	pView->m_OffSet[1] = -1274.630005;
	pView->m_OffSet[2] = -24.216316;
	pView->m_TrackBallCenter[2] = -1033.873535;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK32;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark33()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_ARMPITFOLD_BACK_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.054076, 0.920962, 0.138233, 0.360272);
	pView->m_OffSet[0] = -50.155945;
	pView->m_OffSet[1] = -1335.903687;
	pView->m_OffSet[2] = -44.790970;
	pView->m_TrackBallCenter[2] = -1033.873535;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK33;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark34()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_WAIST_FRONT;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.000000, 0.000000, 1.000000);
	pView->m_OffSet[0] = -278.904541;
	pView->m_OffSet[1] = -1060.540527;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -756.847961;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK34;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark35()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_WAIST_BACK;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 1.000000, 0.000000, 0.000000);
	pView->m_OffSet[0] = -278.904541;
	pView->m_OffSet[1] = -1060.540527;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -756.847961;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK35;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark36()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_POPLITEUS;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 1.000000, 0.000000, 0.000000);
	pView->m_OffSet[0] = -263.429932;
	pView->m_OffSet[1] = -457.147430;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -959.644775;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK36;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark37()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_ELBOW;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.707107, 0.000000, 0.707107);
	pView->m_OffSet[0] = -98.837914;
	pView->m_OffSet[1] = -1093.783691;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -1033.873535;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK37;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnBnClickedPose1Mark38()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_WRIST;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 0.707107, 0.000000, 0.707107);
	pView->m_OffSet[0] = -75.290764;
	pView->m_OffSet[1] = -948.344849;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -1033.873535;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK38;
	Invalidate(false);
	pView->SetFocus();
}


void CPose1InfoDialog::OnBnClickedPose1Mark39()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_MIDRIFF;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 1.000000, 0.000000, 0.000000);
	pView->m_OffSet[0] = -275.895599;
	pView->m_OffSet[1] = -1038.918945;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -643.078064;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK39;
	Invalidate(false);
	pView->SetFocus();
}


void CPose1InfoDialog::OnBnClickedPose1Mark40()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_TOPHIP;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 1.000000, 0.000000, 0.000000);
	pView->m_OffSet[0] = -275.895599;
	pView->m_OffSet[1] = -1038.918945;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -643.078064;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK40;
	Invalidate(false);
	pView->SetFocus();
}


void CPose1InfoDialog::OnBnClickedPose1Mark41()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_UPPERHIP;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.000000, 1.000000, 0.000000, 0.000000);
	pView->m_OffSet[0] = -275.895599;
	pView->m_OffSet[1] = -1038.918945;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -643.078064;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK41;
	Invalidate(false);
	pView->SetFocus();
}



void CPose1InfoDialog::OnBnClickedPose1Mark42()
{
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE1_LANDMARK_ARMPITFOLD_BACK_L;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose1MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(-0.074815, 0.912221, 0.193043, -0.353539);
	pView->m_OffSet[0] = -230.430618;
	pView->m_OffSet[1] = -1257.949585;
	pView->m_OffSet[2] = -210.664551;
	pView->m_TrackBallCenter[2] = -1033.873535;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE1_MARK42;
	Invalidate(false);
	pView->SetFocus();
}

void CPose1InfoDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
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


