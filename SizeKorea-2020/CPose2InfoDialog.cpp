// CPose2InfoDialog.cpp: 구현 파일
#include "pch.h"
#include "SizeKorea-2020.h"
#include "CPose2InfoDialog.h"
#include "MainFrm.h"

extern EgSizer theSizer;
extern EgVertex* m_nLandmarkPose2[NEW_LANDMARK_NUM_POSE2];

IMPLEMENT_DYNAMIC(CPose2InfoDialog, CPaneDialog)

CPose2InfoDialog::CPose2InfoDialog()
{
	m_SltButtonId = -1;
	m_Pose2MarkCoords = "(0.0, 0.0, 0.0)";
}

CPose2InfoDialog::~CPose2InfoDialog()
{
}

BEGIN_MESSAGE_MAP(CPose2InfoDialog, CPaneDialog)
	ON_BN_CLICKED(IDC_POSE2_MARK1, &CPose2InfoDialog::OnBnClickedPose2Mark1)
	ON_BN_CLICKED(IDC_POSE2_MARK2, &CPose2InfoDialog::OnBnClickedPose2Mark2)
	ON_BN_CLICKED(IDC_POSE2_MARK3, &CPose2InfoDialog::OnBnClickedPose2Mark3)
	ON_BN_CLICKED(IDC_POSE2_MARK4, &CPose2InfoDialog::OnBnClickedPose2Mark4)
	ON_BN_CLICKED(IDC_POSE2_MARK5, &CPose2InfoDialog::OnBnClickedPose2Mark5)
	ON_BN_CLICKED(IDC_POSE2_MARK6, &CPose2InfoDialog::OnBnClickedPose2Mark6)
	ON_BN_CLICKED(IDC_POSE2_MARK7, &CPose2InfoDialog::OnBnClickedPose2Mark7)
	ON_BN_CLICKED(IDC_POSE2_MARK8, &CPose2InfoDialog::OnBnClickedPose2Mark8)
	ON_BN_CLICKED(IDC_POSE2_MARK9, &CPose2InfoDialog::OnBnClickedPose2Mark9)
	ON_BN_CLICKED(IDC_POSE2_MARK10, &CPose2InfoDialog::OnBnClickedPose2Mark10)
	ON_BN_CLICKED(IDC_POSE2_MARK11, &CPose2InfoDialog::OnBnClickedPose2Mark11)
	ON_BN_CLICKED(IDC_POSE2_MARK12, &CPose2InfoDialog::OnBnClickedPose2Mark12)
	ON_BN_CLICKED(IDC_POSE2_MARK14, &CPose2InfoDialog::OnBnClickedPose2Mark14)
	ON_BN_CLICKED(IDC_POSE2_MARK17, &CPose2InfoDialog::OnBnClickedPose2Mark17)
	ON_BN_CLICKED(IDC_POSE2_MARK18, &CPose2InfoDialog::OnBnClickedPose2Mark18)
	ON_BN_CLICKED(IDC_POSE2_MARK19, &CPose2InfoDialog::OnBnClickedPose2Mark19)
	ON_BN_CLICKED(IDC_POSE2_MARK20, &CPose2InfoDialog::OnBnClickedPose2Mark20)
	ON_BN_CLICKED(IDC_POSE2_MARK21, &CPose2InfoDialog::OnBnClickedPose2Mark21)
	ON_BN_CLICKED(IDC_POSE2_MARK22, &CPose2InfoDialog::OnBnClickedPose2Mark22)
	ON_BN_CLICKED(IDC_POSE2_MARK23, &CPose2InfoDialog::OnBnClickedPose2Mark23)
	ON_BN_CLICKED(IDC_POSE2_MARK24, &CPose2InfoDialog::OnBnClickedPose2Mark24)
	ON_BN_CLICKED(IDC_POSE2_MARK25, &CPose2InfoDialog::OnBnClickedPose2Mark25)
	ON_BN_CLICKED(IDC_POSE2_MARK26, &CPose2InfoDialog::OnBnClickedPose2Mark26)
	ON_BN_CLICKED(IDC_POSE2_MARK27, &CPose2InfoDialog::OnBnClickedPose2Mark27)
	ON_BN_CLICKED(IDC_POSE2_MARK28, &CPose2InfoDialog::OnBnClickedPose2Mark28)
	ON_BN_CLICKED(IDC_POSE2_MARK29, &CPose2InfoDialog::OnBnClickedPose2Mark29)
	ON_BN_CLICKED(IDC_POSE2_MARK30, &CPose2InfoDialog::OnBnClickedPose2Mark30)
	ON_BN_CLICKED(IDC_POSE2_MARK32, &CPose2InfoDialog::OnBnClickedPose2Mark32)
	ON_BN_CLICKED(IDC_POSE2_MARK33, &CPose2InfoDialog::OnBnClickedPose2Mark33)
	ON_BN_CLICKED(IDC_POSE2_MARK34, &CPose2InfoDialog::OnBnClickedPose2Mark34)
	ON_BN_CLICKED(IDC_POSE2_MARK35, &CPose2InfoDialog::OnBnClickedPose2Mark35)
	ON_BN_CLICKED(IDC_POSE2_MARK36, &CPose2InfoDialog::OnBnClickedPose2Mark36)
	ON_BN_CLICKED(IDC_POSE2_MARK42, &CPose2InfoDialog::OnBnClickedPose2Mark42)
	ON_BN_CLICKED(IDC_POSE2_MARK43, &CPose2InfoDialog::OnBnClickedPose2Mark43)
	ON_BN_CLICKED(IDC_POSE2_MARK16, &CPose2InfoDialog::OnBnClickedPose2Mark16)
	ON_BN_CLICKED(IDC_POSE2_MARK13, &CPose2InfoDialog::OnBnClickedPose2Mark13)
	ON_BN_CLICKED(IDC_POSE2_MARK37, &CPose2InfoDialog::OnBnClickedPose2Mark37)
	ON_BN_CLICKED(IDC_POSE2_MARK15, &CPose2InfoDialog::OnBnClickedPose2Mark15)
	ON_BN_CLICKED(IDC_POSE2_MARK38, &CPose2InfoDialog::OnBnClickedPose2Mark38)
	ON_BN_CLICKED(IDC_POSE2_MARK39, &CPose2InfoDialog::OnBnClickedPose2Mark39)
	ON_BN_CLICKED(IDC_POSE2_MARK40, &CPose2InfoDialog::OnBnClickedPose2Mark40)
	ON_BN_CLICKED(IDC_POSE2_MARK41, &CPose2InfoDialog::OnBnClickedPose2Mark41)
	ON_BN_CLICKED(IDC_POSE2_MARK44, &CPose2InfoDialog::OnBnClickedPose2Mark44)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

CSizeKorea2020View* CPose2InfoDialog::GetView() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020View *)pFrame->GetActiveView();
}

CSizeKorea2020Doc* CPose2InfoDialog::GetDocument() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020Doc *)pFrame->GetActiveDocument();
}

void CPose2InfoDialog::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class

	CPaneDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POSE2_MARK_POS, m_Pose2MarkCoords);
}

// CPose2InfoDialog 메시지 처리기
void CPose2InfoDialog::OnBnClickedPose2Mark1()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_TOPHEAD;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -476.322906;
	pView->m_OffSet[1] = -1547.214478;
	pView->m_OffSet[2] = 0.000;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK1;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark2()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_NECK_THYROID;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -505.459656;
	pView->m_OffSet[1] = -1385.062012;
	pView->m_OffSet[2] = 0.000;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK2;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark3()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_NECK_FRONT;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -500.822815;
	pView->m_OffSet[1] = -1334.056763;
	pView->m_OffSet[2] = 0.000;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK3;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark4()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_NECK_SIDE_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -440.543488;
	pView->m_OffSet[1] = -1373.933594;
	pView->m_OffSet[2] = 0.000;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK4;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark5()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_NECK_SIDE_L;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -545.336670;
	pView->m_OffSet[1] = -1373.006226;
	pView->m_OffSet[2] = 0.000;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK5;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark6()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_NECK_BACK;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);

	pView->m_OffSet[0] = -488.766937;
	pView->m_OffSet[1] = -1379.497803;
	pView->m_OffSet[2] = 0.000;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK6;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark7()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_SHOULDERSIDE_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.707107, 0.0, 0.707107);

	pView->m_OffSet[0] = -315.348114;
	pView->m_OffSet[1] = -1314.582031;
	pView->m_OffSet[2] = -118.703911;

	pView->m_TrackBallCenter[2] = -692.201416;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK7;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark8()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_SHOULDERSIDE_L;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, -0.707107, 0.0, 0.707107);

	pView->m_OffSet[0] = -315.348114;
	pView->m_OffSet[1] = -1314.582031;
	pView->m_OffSet[2] = -110.703911;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK8;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark9()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_ARMPIT_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -337.605042;
	pView->m_OffSet[1] = -1233.900513;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK9;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark10()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_ARMPIT_L;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -639.929077;
	pView->m_OffSet[1] = -1220.917358;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK10;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark12()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_ARMPITFOLD_FRONT_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -255.996262;
	pView->m_OffSet[1] = -1239.464722;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK12;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark14()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_ARMPITFOLD_BACK_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);

	pView->m_OffSet[0] = -330.186218;
	pView->m_OffSet[1] = -1219.989990;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK14;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark17()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_BP_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -423.850952;
	pView->m_OffSet[1] = -1158.783691;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK17;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark18()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_BP_L;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -560.174805;
	pView->m_OffSet[1] = -1158.783691;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;
	
	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK18;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark19()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_UNDERBUST;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -423.850891;
	pView->m_OffSet[1] = -1095.722656;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK19;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark20()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_WAIST;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);


	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -375.627716;
	pView->m_OffSet[1] = -1004.840576;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK20;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark21()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_WAIST_L;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -605.616089;
	pView->m_OffSet[1] = -1004.840576;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK21;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark22()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_WAIST_FRONT;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -496.18618;
	pView->m_OffSet[1] = -1007.622681;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK22;
	Invalidate(false);
	pView->SetFocus();

}


void CPose2InfoDialog::OnBnClickedPose2Mark23()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_WAIST_BACK;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);

	pView->m_OffSet[0] = -496.18618;
	pView->m_OffSet[1] = -1007.622681;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK23;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark24()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_STOMACH_EXTRUDE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -498.040894;
	pView->m_OffSet[1] = -926.941650;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK24;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark25()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_NAVEL;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -502.677795;
	pView->m_OffSet[1] = -946.416382;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK25;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark13()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_NAVEL_BACK;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);

	pView->m_OffSet[0] = -502.677795;
	pView->m_OffSet[1] = -946.416382;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK13;
	Invalidate(false); 
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark26()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_HIP_EXTRUDE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);

	pView->m_OffSet[0] = -493.403931;
	pView->m_OffSet[1] = -795.255249;
	pView->m_OffSet[2] = -158.580902;

	pView->m_TrackBallCenter[2] = -692.201416;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK26;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark27()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_CROTCH;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(-0.707107, 0.0, 0.0, 0.707107);

	pView->m_OffSet[0] = -502.677612;
	pView->m_OffSet[1] = -736.831055;
	pView->m_OffSet[2] = -107.575371;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK27;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark28()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_UNDERHIP;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);

	pView->m_OffSet[0] = -503.604980;
	pView->m_OffSet[1] = -696.026855;
	pView->m_OffSet[2] = -107.575371;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK28;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark29()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_KNEE_MID;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -419.214111;
	pView->m_OffSet[1] = -397.412659;
	pView->m_OffSet[2] = -107.575371;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK29;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark30()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_CALF_EXTRUDE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);

	pView->m_OffSet[0] = -388.610870;
	pView->m_OffSet[1] = -265.725616;
	pView->m_OffSet[2] = -107.575371;

	pView->m_TrackBallCenter[2] = -692.201416;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK30;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark32()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_ANKLE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.707107, 0.0, 0.707107);

	pView->m_OffSet[0] = -388.610870;
	pView->m_OffSet[1] = -64.485413;
	pView->m_OffSet[2] = -69.553040;

	pView->m_TrackBallCenter[2] = -692.201416;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK32;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark33()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_ANKLE_INSIDE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -496.090485;
	pView->m_OffSet[1] = -110.795418;
	pView->m_OffSet[2] = -69.553040;

	pView->m_TrackBallCenter[2] = -642.503540;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK33;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark34()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_WRIST;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);

	pView->m_OffSet[0] = -56.013355;
	pView->m_OffSet[1] = -790.285217;
	pView->m_OffSet[2] = -69.553040;

	pView->m_TrackBallCenter[2] = -1032.949829;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK34;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark35()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_WRIST_OUTSIDE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -62.932743;
	pView->m_OffSet[1] = -798.588562;
	pView->m_OffSet[2] = -69.553040;

	pView->m_TrackBallCenter[2] = -1032.949829;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK35;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark36()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_HAND;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -125.207787;
	pView->m_OffSet[1] = -773.678467;
	pView->m_OffSet[2] = -69.553040;

	pView->m_TrackBallCenter[2] = -1032.949829;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK36;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark42()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_ELBOW;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.707107, 0.0, 0.707107);

	pView->m_OffSet[0] = -223.464157;
	pView->m_OffSet[1] = -1006.172180;
	pView->m_OffSet[2] = -113.837532;

	pView->m_TrackBallCenter[2] = -1032.949829;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK42;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark43()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_SHOULDER_MID;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -385.617371;
	pView->m_OffSet[1] = -1337.630859;
	pView->m_OffSet[2] = -113.837532;

	pView->m_TrackBallCenter[2] = -889.949585;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK43;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark16()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_MIDRIFF;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);

	pView->m_OffSet[0] = -482.768250;
	pView->m_OffSet[1] = -1041.553345;
	pView->m_OffSet[2] = -113.837532;

	pView->m_TrackBallCenter[2] = -972.195068;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK16;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark37()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_UPPERHIP;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);

	pView->m_OffSet[0] = -482.768250;
	pView->m_OffSet[1] = -997.268677;
	pView->m_OffSet[2] = -113.837532;

	pView->m_TrackBallCenter[2] = -972.195068;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK37;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark38()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_THIGH_MID;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.707107, 0.0, 0.707107);

	pView->m_OffSet[0] = -482.768250;
	pView->m_OffSet[1] = -604.313782;
	pView->m_OffSet[2] = -105.593155;

	pView->m_TrackBallCenter[2] = -1062.041260;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK38;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark15()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_TOPHIP;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	
	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);

	pView->m_OffSet[0] = -490.703583;
	pView->m_OffSet[1] = -967.142944;
	pView->m_OffSet[2] = -105.593155;

	pView->m_TrackBallCenter[2] = -915.013550;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK15;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark11()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_CALF_EXTRUDE_FRONT;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -396.309814;
	pView->m_OffSet[1] = -269.613953;
	pView->m_OffSet[2] = -105.593155;

	pView->m_TrackBallCenter[2] = -915.013550;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK11;
	Invalidate(false);
	pView->SetFocus();
}

void CPose2InfoDialog::OnBnClickedPose2Mark39() {
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_UNDERKNEE;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -406.116913;
	pView->m_OffSet[1] = -350.522675;
	pView->m_OffSet[2] = -105.593155;

	pView->m_TrackBallCenter[2] = -915.013550;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK39;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark40()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_UNDERCALF;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);

	pView->m_OffSet[0] = -456.378326;
	pView->m_OffSet[1] = -183.802200;
	pView->m_OffSet[2] = -105.593155;

	pView->m_TrackBallCenter[2] = -915.013550;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK40;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark41()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_SHOULDER_R;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.707107, 0.0, 0.707107);

	pView->m_OffSet[0] = -456.378326;
	pView->m_OffSet[1] = -1316.518921;
	pView->m_OffSet[2] = -117.851936;

	pView->m_TrackBallCenter[2] = -915.013550;


	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK41;
	Invalidate(false);
	pView->SetFocus();
}


void CPose2InfoDialog::OnBnClickedPose2Mark44()
{
	// TODO: Add your control notification handler code here
	CSizeKorea2020View *pView = GetView();

	theSizer.m_SltLandmarkIdx = POSE2_LANDMARK_SHOULDER_L;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose2MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, -0.707107, 0.0, 0.707107);

	pView->m_OffSet[0] = -456.378326;
	pView->m_OffSet[1] = -1316.518921;
	pView->m_OffSet[2] = -117.851936;

	pView->m_TrackBallCenter[2] = -915.013550;

	pView->Invalidate(false);
	m_SltButtonId = IDC_POSE2_MARK44;
	Invalidate(false);
	pView->SetFocus();
}

void CPose2InfoDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
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


