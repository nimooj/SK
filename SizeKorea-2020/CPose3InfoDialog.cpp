// CPose3InfoDialog.cpp: 구현 파일
//

#include "pch.h"
#include "SizeKorea-2020.h"
#include "CPose3InfoDialog.h"
#include "MainFrm.h"

extern EgSizer theSizer;

// CPose3InfoDialog

IMPLEMENT_DYNAMIC(CPose3InfoDialog, CPaneDialog)

CPose3InfoDialog::CPose3InfoDialog()
{
	m_SltButtonId = -1;
	m_Pose3MarkCoords = "(0.0, 0.0, 0.0)";
}

CPose3InfoDialog::~CPose3InfoDialog()
{
}

BEGIN_MESSAGE_MAP(CPose3InfoDialog, CPaneDialog)
	ON_BN_CLICKED(IDC_P3_BP, &CPose3InfoDialog::OnBnClickedP3BP)
	ON_BN_CLICKED(IDC_P3_AP, &CPose3InfoDialog::OnBnClickedP3AP)
	ON_BN_CLICKED(IDC_P3_BMP, &CPose3InfoDialog::OnBnClickedP3BMP)
	ON_BN_CLICKED(IDC_P3_LFC, &CPose3InfoDialog::OnBnClickedP3LFC)
	ON_BN_CLICKED(IDC_P3_RFC, &CPose3InfoDialog::OnBnClickedP3RFC)
	ON_BN_CLICKED(IDC_P3_RBP, &CPose3InfoDialog::OnBnClickedP3RBP)
	ON_BN_CLICKED(IDC_P3_BO, &CPose3InfoDialog::OnBnClickedP3BO)
	ON_BN_CLICKED(IDC_P3_RO, &CPose3InfoDialog::OnBnClickedP3RO)
	ON_BN_CLICKED(IDC_P3_RA, &CPose3InfoDialog::OnBnClickedP3RA)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

CSizeKorea2020View *CPose3InfoDialog::GetView() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020View *)pFrame->GetActiveView();
}

CSizeKorea2020Doc *CPose3InfoDialog::GetDocument() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020Doc *)pFrame->GetActiveDocument();
}

void CPose3InfoDialog::DoDataExchange(CDataExchange * pDX)
{
	CPaneDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_P3_MARK_POS, m_Pose3MarkCoords);
}

// CPose3InfoDialog 메시지 처리기
void CPose3InfoDialog::OnBnClickedP3BP()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 0;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose3MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);
	pView->m_OffSet[0] = -203.0;
	pView->m_OffSet[1] = -808.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -882.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P3_BP;
	Invalidate(false);
	pView->SetFocus();
}

void CPose3InfoDialog::OnBnClickedP3AP()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 1;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose3MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.0, 0.0, 1.0);
	pView->m_OffSet[0] = -218.0;
	pView->m_OffSet[1] = -887.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -668.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P3_AP;
	Invalidate(false);
	pView->SetFocus();
}


void CPose3InfoDialog::OnBnClickedP3BMP()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 2;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose3MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);
	pView->m_OffSet[0] = -218.0;
	pView->m_OffSet[1] = -810.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -640.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P3_BMP;
	Invalidate(false);
	pView->SetFocus();
}

void CPose3InfoDialog::OnBnClickedP3LFC()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 3;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose3MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, -0.707107, 0.0, 0.707107);
	pView->m_OffSet[0] = 0.0;
	pView->m_OffSet[1] = -1131.0;
	pView->m_OffSet[2] = -410.0;
	pView->m_TrackBallCenter[2] = -886.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P3_LFC;
	Invalidate(false);
	pView->SetFocus();
}

void CPose3InfoDialog::OnBnClickedP3RFC()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 4;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose3MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 0.707107, 0.0, 0.707107);
	pView->m_OffSet[0] = 0.0;
	pView->m_OffSet[1] = -1273.0;
	pView->m_OffSet[2] = -700.0;
	pView->m_TrackBallCenter[2] = -850.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P3_RFC;
	Invalidate(false);
	pView->SetFocus();
}

void CPose3InfoDialog::OnBnClickedP3RBP()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 5;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose3MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.0, 1.0, 0.0, 0.0);
	pView->m_OffSet[0] = -198.0;
	pView->m_OffSet[1] = -1266.0;
	pView->m_OffSet[2] = 0.0;
	pView->m_TrackBallCenter[2] = -730.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P3_RBP;
	Invalidate(false);
	pView->SetFocus();
}

void CPose3InfoDialog::OnBnClickedP3BO()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 6;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose3MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(-0.001264, 0.976432, 0.003759, -0.215790);
	pView->m_OffSet[0] = -500.0;
	pView->m_OffSet[1] = -1017.0;
	pView->m_OffSet[2] = -20.0;
	pView->m_TrackBallCenter[2] = -750.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P3_BO;
	Invalidate(false);
	pView->SetFocus();
}


void CPose3InfoDialog::OnBnClickedP3RO()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 7;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose3MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(-0.001264, 0.976432, 0.003759, -0.215790);
	pView->m_OffSet[0] = -500.0;
	pView->m_OffSet[1] = -1077.0;
	pView->m_OffSet[2] = -20.0;
	pView->m_TrackBallCenter[2] = -750.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P3_RO;
	Invalidate(false);
	pView->SetFocus();
}

void CPose3InfoDialog::OnBnClickedP3RA()
{
	CSizeKorea2020View *pView = GetView();
	theSizer.m_SltLandmarkIdx = 8;
	EgPos p = theSizer.GetSltLandmarkPos();
	m_Pose3MarkCoords.Format("(%.1lf, %.1lf, %.1lf)", p[0], p[1], p[2]);
	UpdateData(false);

	pView->m_TrackBall.quat.SetQuat(0.099755, 0.700538, 0.101474, 0.699294);
	pView->m_OffSet[0] = -279.0;
	pView->m_OffSet[1] = -1400.0;
	pView->m_OffSet[2] = -120.0;
	pView->m_TrackBallCenter[2] = -900.0;
	pView->Invalidate(false);

	m_SltButtonId = IDC_P3_RA;
	Invalidate(false);
	pView->SetFocus();
}

void CPose3InfoDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
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

