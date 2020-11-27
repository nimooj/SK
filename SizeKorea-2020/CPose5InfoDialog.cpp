// CPose5InfoDialog.cpp: 구현 파일
//

#include "pch.h"
#include "SizeKorea-2020.h"
#include "CPose5InfoDialog.h"
#include "MainFrm.h"

extern EgSizer theSizer;

// CPose5InfoDialog

IMPLEMENT_DYNAMIC(CPose5InfoDialog, CPaneDialog)

CPose5InfoDialog::CPose5InfoDialog()
{
	m_SltButtonId = -1;
	m_Pose5MarkCoords = "(0.0, 0.0, 0.0)";
}

CPose5InfoDialog::~CPose5InfoDialog()
{
}


BEGIN_MESSAGE_MAP(CPose5InfoDialog, CPaneDialog)
	ON_BN_CLICKED(IDC_P5_BP, &CPose5InfoDialog::OnBnClickedP5Bp)
	ON_BN_CLICKED(IDC_P5_AP, &CPose5InfoDialog::OnBnClickedP5Ap)
	ON_BN_CLICKED(IDC_P5_BMP, &CPose5InfoDialog::OnBnClickedP5Bmp)
	ON_BN_CLICKED(IDC_P5_LFC, &CPose5InfoDialog::OnBnClickedP5Lfc)
	ON_BN_CLICKED(IDC_P5_RFC, &CPose5InfoDialog::OnBnClickedP5Rfc)
	ON_BN_CLICKED(IDC_P5_LBP, &CPose5InfoDialog::OnBnClickedP5Lbp)
	ON_BN_CLICKED(IDC_P5_BO, &CPose5InfoDialog::OnBnClickedP5Bo)
	ON_BN_CLICKED(IDC_P5_RO, &CPose5InfoDialog::OnBnClickedP5Ro)
	ON_BN_CLICKED(IDC_P5_LA, &CPose5InfoDialog::OnBnClickedP5La)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


CSizeKorea2020View *CPose5InfoDialog::GetView()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020View *)pFrame->GetActiveView();
}

CSizeKorea2020Doc *CPose5InfoDialog::GetDocument()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020Doc *)pFrame->GetActiveDocument();
}

void CPose5InfoDialog::DoDataExchange(CDataExchange * pDX)
{
	CPaneDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_P5_MARK_POS, m_Pose5MarkCoords);
}


// CPose5InfoDialog 메시지 처리기
void CPose5InfoDialog::OnBnClickedP5Bp()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 0;
	double x = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[0];
	double y = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[1];
	double z = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[2];
	m_Pose5MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", x, y, z);
	UpdateData(false);
	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);
	pView->m_OffSet[0] = -203.0;
	pView->m_OffSet[1] = -808.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -882.0;
	pView->Invalidate(false);
	m_SltButtonId = IDC_P5_BP;
	Invalidate(false);
	pView->SetFocus();
}


void CPose5InfoDialog::OnBnClickedP5Ap()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 1;
	double x = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[0];
	double y = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[1];
	double z = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[2];
	m_Pose5MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", x, y, z);
	UpdateData(false);
	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);
	pView->m_OffSet[0] = -218.0;
	pView->m_OffSet[1] = -887.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -668.0;
	pView->Invalidate(false);
	m_SltButtonId = IDC_P5_AP;
	Invalidate(false);
	pView->SetFocus();
}


void CPose5InfoDialog::OnBnClickedP5Bmp()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 2;
	double x = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[0];
	double y = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[1];
	double z = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[2];
	m_Pose5MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", x, y, z);
	UpdateData(false);
	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);
	pView->m_OffSet[0] = -218.0;
	pView->m_OffSet[1] = -810.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -640.0;
	//pDoc->m_MarkIdx = POSE1_LANDMARK_TOPHEAD;
	//pView->SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	pView->Invalidate(false);
	m_SltButtonId = IDC_P5_BMP;
	this->Invalidate(true);
	pView->SetFocus();
}


void CPose5InfoDialog::OnBnClickedP5Lfc()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 3;
	double x = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[0];
	double y = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[1];
	double z = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[2];
	m_Pose5MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", x, y, z);
	UpdateData(false);
	pView->m_TrackBall.quat.SetQuat(0.0, -0.707107, 0.0, 0.707107);
	pView->m_OffSet[0] = 0.0;
	pView->m_OffSet[1] = -1131.0;
	pView->m_OffSet[2] = -410.0;
	pView->m_TrackBallCenter[2] = -886.0;

	//pDoc->m_MarkIdx = POSE1_LANDMARK_TOPHEAD;
	//pView->SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	pView->Invalidate(false);
	m_SltButtonId = IDC_P5_LFC;
	Invalidate(false);
	pView->SetFocus();
}

void CPose5InfoDialog::OnBnClickedP5Rfc()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 4;
	double x = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[0];
	double y = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[1];
	double z = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[2];
	m_Pose5MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", x, y, z);
	UpdateData(false);
	pView->m_TrackBall.quat.SetQuat(0.0, 0.707107, 0.0, 0.707107);
	pView->m_OffSet[0] = 0.0;
	pView->m_OffSet[1] = -988.0;
	pView->m_OffSet[2] = -168.0;
	pView->m_TrackBallCenter[2] = -850.0;

	//pDoc->m_MarkIdx = POSE1_LANDMARK_TOPHEAD;
	//pView->SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	pView->Invalidate(false);
	m_SltButtonId = IDC_P5_RFC;
	Invalidate(false);
	pView->SetFocus();
}


void CPose5InfoDialog::OnBnClickedP5Lbp()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 5;
	double x = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[0];
	double y = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[1];
	double z = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[2];
	m_Pose5MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", x, y, z);
	UpdateData(false);
	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);
	pView->m_OffSet[0] = -198.0;
	pView->m_OffSet[1] = -1266.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -730.0;

	//pDoc->m_MarkIdx = POSE1_LANDMARK_TOPHEAD;
	//pView->SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	pView->Invalidate(false);
	m_SltButtonId = IDC_P5_LBP;
	Invalidate(false);
	pView->SetFocus();
}


void CPose5InfoDialog::OnBnClickedP5Bo()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 6;
	double x = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[0];
	double y = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[1];
	double z = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[2];
	m_Pose5MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", x, y, z);
	UpdateData(false);
	pView->m_TrackBall.quat.SetQuat(-0.163461, 0.694275, -0.133879, 0.687996);
	pView->m_OffSet[0] = -121.0;
	pView->m_OffSet[1] = -1055.0;
	pView->m_OffSet[2] = 21.0;
	pView->m_TrackBallCenter[2] = -400.0;
	pView->Invalidate(false);
	m_SltButtonId = IDC_P5_BO;
	Invalidate(false);
	pView->SetFocus();
}


void CPose5InfoDialog::OnBnClickedP5Ro()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 7;
	double x = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[0];
	double y = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[1];
	double z = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[2];
	m_Pose5MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", x, y, z);
	UpdateData(false);
	pView->m_TrackBall.quat.SetQuat(0.0, 0.976072, 0.0, 0.217448);
	pView->m_OffSet[0] = -61.0;
	pView->m_OffSet[1] = -1055.0;
	pView->m_OffSet[2] = -6.8;
	pView->m_TrackBallCenter[2] = -500.0;
	pView->Invalidate(false);
	m_SltButtonId = IDC_P5_RO;
	Invalidate(false);
	pView->SetFocus();
}


void CPose5InfoDialog::OnBnClickedP5La()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 8;
	double x = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[0];
	double y = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[1];
	double z = theSizer.m_Landmarks[theSizer.m_SltLandmarkIdx]->m_Pos[2];
	m_Pose5MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", x, y, z);
	UpdateData(false);
	pView->m_TrackBall.quat.SetQuat(0.194569, -0.679811, -0.194569, 0.679811);
	pView->m_OffSet[0] = -279.0;
	pView->m_OffSet[1] = -1222.0;
	pView->m_OffSet[2] = -6.8;
	pView->m_TrackBallCenter[2] = -555.0;
	pView->Invalidate(false);
	m_SltButtonId = IDC_P5_LA;
	Invalidate(false);
	pView->SetFocus();
}

void CPose5InfoDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// nIDCtl = IDC값
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