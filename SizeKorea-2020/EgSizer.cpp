#pragma once

#include "pch.h"
#include "EgSizer.h"
#include "MainFrm.h"
#include "resource.h"

bool m_bWoman = true;
float m_MinX = INFINITY, m_MaxX = -INFINITY;
float m_MinY = INFINITY, m_MaxY = -INFINITY;
float m_MinZ = INFINITY, m_MaxZ = -INFINITY;

//////////////////// mj::Pose 1
extern EgVertex* m_LandmarkPose1[LANDMARK_NUM_POSE1];
extern EgVertex* m_LandmarkHelpPose1[LANDMARK_HELP_NUM_POSE1];

////////////////////////////////

//////////////////// mj::Pose 2
#include "Pose2Landmarks.h"
#include "Pose2Measures.h"

extern EgVertex* m_nLandmarkPose2[NEW_LANDMARK_NUM_POSE2];

extern REAL m_nResultPose2[NEW_RESULT_NUM_POSE2];
extern BOOL m_nResultPathViewPose2[NEW_RESULT_NUM_POSE2];

extern std::vector<EgPos> m_nResultPathPose2[NEW_RESULT_NUM_POSE2];
////////////////////////////////


// mj :: 자세1, 자세2 수동 측정점 참조 여부 변수 (20. 12. 8)
std::vector<bool> m_bFindLandmark;


extern std::vector<std::string> LandmarkPose1;
extern std::vector<std::string> LandmarkPose2;
extern std::vector<std::string> LandmarkPose3;
extern std::vector<std::string> LandmarkPose4;
extern std::vector<std::string> LandmarkPose5;
extern std::vector<std::string> ResultPose1;
extern std::vector<std::string> ResultPose2;
extern std::vector<std::string> ResultPose3;
extern std::vector<std::string> ResultPose4;
extern std::vector<std::string> ResultPose5;
extern EgSlicer theSlicer;

bool sortPointByX(EgPos a, EgPos b);
bool sortPointByY(EgPos a, EgPos b);
bool sortPointByZ(EgPos a, EgPos b);



bool sortPointByX(EgPos a, EgPos b) {
	return a.m_Coords[0] > b.m_Coords[0];
}

// mj::Sort EgPos.Y by descending order
bool sortPointByY(EgPos a, EgPos b) {
	return a.m_Coords[1] > b.m_Coords[1];
}

// mj::Sort EgPos.Z by ascending order
bool sortPointByZ(EgPos a, EgPos b) {
	return a.m_Coords[2] < b.m_Coords[2];
}


/*********************/
/* EgSizer 클래스 구현 */
/*********************/
/*!
*	\brief	생성자
*	\note	마지막 수정일: 2020-09-27
*/
EgSizer::EgSizer()
{
	m_SltLandmarkIdx = -1;
	m_pMesh = NULL;
	m_pArrowMesh = new EgMesh("Arrow Mesh");
	m_bWoman = true;
	m_bShowLandMark = true;

	// 정점을 생성하여 메쉬에 추가한다.
	int snum = 20;
	double rad = 5.0;
	double height = 15.0;
	for (int i = 0; i < snum - 1; ++i)
	{
		double theta = 2 * M_PI * (double)i / (double)(snum - 1);
		double x = rad * sin(theta);
		double z = rad * cos(theta);
		m_pArrowMesh->AddVertex(new EgVertex(x, -height, z));
	}
	EgVertex *pApex = new EgVertex(0.0, 0.0, 0.0);
	EgVertex *pBase = new EgVertex(0.0, -height, 0.0);
	m_pArrowMesh->AddVertex(pApex);
	m_pArrowMesh->AddVertex(pBase);

	// 삼각형을 생성하여 메쉬에 추가한다.
	for (int i = 0; i < snum - 1; ++i)
	{
		EgVertex *v1 = m_pArrowMesh->m_pVerts[i];
		EgVertex *v2 = m_pArrowMesh->m_pVerts[(i + 1) % (snum - 1)];
		EgFace *f0 = new EgFace(pApex, v1, v2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "");
		EgFace *f1 = new EgFace(pBase, v2, v1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "");
		m_pArrowMesh->AddFace(f0);
		m_pArrowMesh->AddFace(f1);
	}

	// 메쉬의 위상정보, 경계상자, 법선을 계산한다.
	m_pArrowMesh->InitEdgeMate();
	m_pArrowMesh->UpdateBndBox();
	m_pArrowMesh->UpdateNormal(FLAT);

	// 메쉬에 임의의 재질을 할당한다.
	EgMaterial *pMtl = ::create_material("arrow", 0.1f, 0.1f, 0.1f, 0.8f, 0.0f, 0.0f, 0.7f, 0.7f, 0.7f, 16.0f);
	m_pArrowMesh->AddMaterial(pMtl);
	m_pArrowMesh->SetMaterial(pMtl);
}


/*!
*	\brief	소멸자
*	\note	마지막 수정일: 2020-07-15
*/
EgSizer::~EgSizer()
{
	if (m_pArrowMesh != NULL)
		delete m_pArrowMesh;
}

/*!
*	\brief	스캔 자세와 메쉬를 설정한다.
*	\note	마지막 수정일: 2020-09-05
*
*	\param	PoseId[in]	스캔 메쉬의 자세
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*/
void EgSizer::Init(int PoseId, EgMesh *pMesh)
{
	m_PoseId = PoseId;
	m_pMesh = pMesh;
	m_SltLandmarkIdx = -1;
	m_bWoman = true;

	if (PoseId == 1)
	{
		int NumLandmarks = LANDMARK_NUM_POSE1;
		m_Landmarks.assign(NumLandmarks, NULL);
		m_bFindLandmark.assign(NumLandmarks, true);

		int NumResults = RESULT_NUM_POSE1;
		m_Results.assign(NumResults, 0.0);
		m_Paths.assign(NumResults, std::vector<EgPos>());
		m_bShowPaths.assign(NumResults, true);
	}

	if (PoseId == 2) {
		int NumLandmarks = NEW_LANDMARK_NUM_POSE2;
		m_Landmarks.assign(NumLandmarks, NULL);
		m_bFindLandmark.assign(NumLandmarks, true);

		int NumResults = NEW_RESULT_NUM_POSE2;
		m_Results.assign(NumResults, 0.0);
		m_Paths.assign(NumResults, std::vector<EgPos>());
		m_bShowPaths.assign(NumResults, true);
	}


	if (PoseId == 3 || PoseId == 5)
	{
		int NumLandmarks = (int)LandmarkPose3.size();
		m_Landmarks.assign(NumLandmarks, NULL);
		m_bUserPick.assign(NumLandmarks, false);	// 수동랜드마크

		int NumResults = (int)ResultPose3.size();
		m_Results.assign(NumResults, 0.0);
		m_Paths.assign(NumResults, std::vector<EgPos>());
		m_bShowPaths.assign(NumResults, true);
	}

	if (PoseId == 4)
	{
		int NumLandmarks = (int)LandmarkPose4.size();
		m_Landmarks.assign(NumLandmarks, NULL);
		m_bUserPick.assign(NumLandmarks, false);	// 수동랜드마크


		int NumResults = (int)ResultPose4.size();
		m_Results.assign(NumResults, 0.0);
		m_Paths.assign(NumResults, std::vector<EgPos>());
		m_bShowPaths.assign(NumResults, true);
	}
}

/*!
*	\brief	측정점을 찾는다.
*	\note	마지막 수정일: 2020-09-05
*/
void EgSizer::FindLandmarks()
{
	// 자세에 따라 측정점을 찾는다.
	switch (m_PoseId)
	{
	case 1:
		FindLandmarkPose1();
		break;

	case 2:
		// m_nLandmarkPose2를 EgSizer에 할당한다
		FindLandmarkPose2();
		break;

	case 3:
		FindLandmarkPose3();
		break;

	case 4:
		FindLandmarkPose4();
		break;

	case 5:
		FindLandmarkPose5();
		break;
	}

	// 슬라이서를 초기화 한다.
	//theSlicer.Reset();
}

/*!
*	\brief	현재 선택된 특징점의 위치를 갱신한다.
*	\note	마지막 수정일: 2020-08-28
*
*	\param	pos[in]		특징점의 새로운 위치
*/
void EgSizer::UpdateLandmark(EgPos pos)
{
	if (m_SltLandmarkIdx == -1)
		return;

	EgVertex *v = FindClosestVert(m_pMesh, pos);
	m_Landmarks[m_SltLandmarkIdx] = v;
	m_bUserPick[m_SltLandmarkIdx] = true;	// 수동랜드마크


	// mj :: 자동 측정 제외
	m_bFindLandmark[m_SltLandmarkIdx] = false;

	// mj :: 자세1도 수동 측정점은 제외하고 Find Landmark할 경우가 생겨서 Global varibale 업데이트 필요 (20. 12. 8)
	if (m_PoseId == 1) {
		m_LandmarkPose1[m_SltLandmarkIdx] = v;
	}
	// mj :: 자세2는 EgSizer의 m_Landmarks 말고도 Doc에서 처리하기 때문에 필요 (20. 12. 8)
	else if (m_PoseId == 2) {
		m_nLandmarkPose2[m_SltLandmarkIdx] = v;
	}
}

/*!
*	\brief	해당 랜드마크 정점의 포인터를 반환한다.
*	\note	마지막 수정일: 2020-07-20
*
*	\param	Name[in]	랜드마크이 이름
*
*	\return	해당 랜드마크 정점의 포인터를 반환한다.
*/
EgVertex *EgSizer::GetLandmark(std::string Name)
{
	int idx = GetLandmarkIdx(Name);
	if (idx == -1)
		return NULL;
	return m_Landmarks[idx];
}

/*!
*	\brief	해당 자세의 측정 항목을 측정한다.
*	\note	마지막 수정일: 2020-08-28
*/
void EgSizer::Measure()
{
	// 결과를 초기화한다.
	int NumResults = (int)m_Results.size();
	m_Results.assign(NumResults, 0.0);
	m_Paths.assign(NumResults, std::vector<EgPos>());
	m_bShowPaths.assign(NumResults, true);

	// 자세에 따라 측정점을 찾는다.
	switch (m_PoseId)
	{
	case 1:
		MeasurePose1();
		break;

	case 2:
		// m_nResultPose2를 EgSizer에 할당한다
		MeasurePose2();
		break;

	case 3:
		MeasurePose3();
		break;

	case 4:
		MeasurePose4();
		break;

	case 5:
		MeasurePose5();
		break;
	}
}

void EgSizer::FindLandmarkPose1() 
{
	// Set Min-Max values
	m_MinX = m_pMesh->m_BndBox.m_MinPt.m_Coords[0];
	m_MinY = m_pMesh->m_BndBox.m_MinPt.m_Coords[1];
	m_MinZ = m_pMesh->m_BndBox.m_MinPt.m_Coords[2];

	m_MaxX = m_pMesh->m_BndBox.m_MaxPt.m_Coords[0];
	m_MaxY = m_pMesh->m_BndBox.m_MaxPt.m_Coords[1];
	m_MaxZ = m_pMesh->m_BndBox.m_MaxPt.m_Coords[2];

	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame->m_pLandmarkProgressDlg == NULL)
	{
		pFrame->m_pLandmarkProgressDlg = new CProgressDialog();
		pFrame->m_pLandmarkProgressDlg->Create(IDD_PROGRESS_DIALOG);
		pFrame->m_pLandmarkProgressDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pFrame->m_pLandmarkProgressDlg->ShowWindow(SW_SHOW);
		pFrame->m_pLandmarkProgressDlg->SetFocus();
	}
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetRange(0, LANDMARK_NUM_POSE1);


	_cprintf("====================== LANDMARKS ======================\n");
	///////////////////////////////////////  측정점::머리마루점 
	Pose1_FindTopHeadPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(1);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("머리마루점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	///////////////////////////////////////  Make sagittal/coronal plane
	Pose1_MakeSilhouette(m_pMesh);


	/////////////////////////////////////// 측정점::겨드랑점
	/////////////////////////////////////// 측정점::겨드랑앞접힘점(R/L)
	/////////////////////////////////////// 측정점::겨드랑뒤접힘점(R/L)
	/////////////////////////////////////// 보조점::겨드랑앞점
	/////////////////////////////////////// 보조점::겨드랑뒤점
	Pose1_FindArmpitPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(2);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("겨드랑점, 겨드랑앞/뒤점, 겨드랑앞/뒤접힘점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	/////////////////////////////////////// 측정점::어깨점(R/L)
	/////////////////////////////////////// 측정점::어깨가쪽점(R/L)
	Pose1_FindShoulderPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(3);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("어깨점, 어깨가쪽점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	

	/////////////////////////////////////// 측정점::목옆점 
	Pose1_FindSideNeckPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(4);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("목옆점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	///////////////////////////////////////  측정점::목앞점
	Pose1_FindFneckPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(5);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("목앞점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 보조점::코끝점
	Pose1_FindNosePt(m_pMesh);


	/////////////////////////////////////// 측정점::턱끝점
	Pose1_FindJawPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(6);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("턱끝점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::엉덩이돌출점
	/////////////////////////////////////// 측정점::장딴지돌출점
	Pose1_FindHipnCalfExtrudePt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(7);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("엉덩이돌출점, 장딴지돌출점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	/////////////////////////////////////// 측정점::볼기고랑점
	Pose1_FindUnderHipPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(8);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("볼기고랑점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	/////////////////////////////////////// 측정점::배꼽점(앞/뒤)
	Pose1_FindNavelPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(9);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("배꼽점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	/////////////////////////////////////// 측정점::배돌출점
	Pose1_FindStomachExtrudePt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(10);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("배돌출점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::무릎뼈가운데점
	/////////////////////////////////////// 측정점::정강뼈위점
	/////////////////////////////////////// 측정점::오금점
	Pose1_FindKneePt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(11);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("무릎뼈가운데점, 정강뼈위점, 오금점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::샅점
	Pose1_FindCrotchPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(12);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("샅점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::목뒤점
	Pose1_FindBackNeckPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(13);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("목뒤점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::허리(옆)점
	/////////////////////////////////////// 측정점::허리앞점
	/////////////////////////////////////// 측정점::허리뒤점
	Pose1_FindWaistPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(15);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("허리옆점, 허리앞/뒤점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::가쪽복사점
	Pose1_FindAnklePt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(16);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("가쪽복사점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::젖꼭지점(R/L)
	Pose1_FindBustPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(18);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("젖꼭지점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::겨드랑점들 수정
	Pose1_ModifyArmpitWithShoudler(m_pMesh);


	/////////////////////////////////////// 측정점::겨드랑앞벽점(R/L)
	/////////////////////////////////////// 측정점::겨드랑뒤벽점(R/L)
	Pose1_FindArmpitWall(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(22);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("겨드랑앞/뒤벽점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::젖가슴아래점
	Pose1_FindUnderBustPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(24);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("젖가슴아래점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::손끝점
	Pose1_FindHandPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(25);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("손끝점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	/////////////////////////////////////// 측정점::손목안쪽점
	Pose1_FindWristPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(26);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("손목안쪽점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::노뼈위점
	Pose1_FindElbowPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(28);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("노뼈위점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::눈초리점
	Pose1_FindEyePt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(30);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("눈초리점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::미드리프점
	Pose1_FindMidRiffPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(31);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("미드리프점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::Top hip점
	Pose1_FindTopHipPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(32);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("Top-hip점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// 측정점::Upper hip점
	Pose1_FindUpperHipPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(33);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("Upper-hip점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	/////////////////////////////////////// 보조점::등돌출점
	Pose1_FindBackExtrudePt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(LANDMARK_NUM_POSE1);
	pFrame->m_pLandmarkProgressDlg->ShowWindow(FALSE);

	for (int i = 0; i < LANDMARK_NUM_POSE1; i++) {
		m_Landmarks[i] = m_LandmarkPose1[i];
	}
}

void EgSizer::MeasurePose1() {
	_cprintf("\n\n\n====================== MEASURES ======================\n");


	///////////////////////////////////////  키 1
	EgPos TopHeadPt = m_Landmarks[POSE1_LANDMARK_TOPHEAD]->m_Pos,
		FloorPt = TopHeadPt;

	TopHeadPt.m_Coords[0] += 500.0f;
	FloorPt.m_Coords[0] += 500.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_HEIGHT].push_back(TopHeadPt);
	m_Paths[POSE1_MEASURE_HEIGHT].push_back(FloorPt);

	m_Results[POSE1_MEASURE_HEIGHT] = TopHeadPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Height : %f\n", m_Results[POSE1_MEASURE_HEIGHT]);


	///////////////////////////////////////  눈높이 2
	EgPos EyePt = m_Landmarks[POSE1_LANDMARK_EYE]->m_Pos;
	FloorPt = EyePt;

	EyePt.m_Coords[0] += 400.0f;
	FloorPt.m_Coords[0] += 400.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_EYE_LEVEL].push_back(EyePt);
	m_Paths[POSE1_MEASURE_EYE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_EYE_LEVEL] = EyePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Eye level : %f\n", m_Results[POSE1_MEASURE_EYE_LEVEL]);


	///////////////////////////////////////  턱끝높이 3
	EgPos JawPt = m_Landmarks[POSE1_LANDMARK_JAW]->m_Pos;
	FloorPt = JawPt;

	JawPt.m_Coords[0] += 300.0f;
	FloorPt.m_Coords[0] += 300.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_JAW_LEVEL].push_back(JawPt);
	m_Paths[POSE1_MEASURE_JAW_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_JAW_LEVEL] = JawPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Jaw level : %f\n", m_Results[POSE1_MEASURE_JAW_LEVEL]);


	///////////////////////////////////////  목뒤높이 4
	EgPos BneckPt = m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos;
	FloorPt = BneckPt;

	BneckPt.m_Coords[2] -= 300.0f;
	FloorPt.m_Coords[2] -= 300.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_NECK_BACK_LEVEL].push_back(BneckPt);
	m_Paths[POSE1_MEASURE_NECK_BACK_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_NECK_BACK_LEVEL] = BneckPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Neck back level : %f\n", m_Results[POSE1_MEASURE_NECK_BACK_LEVEL]);


	///////////////////////////////////////  목옆높이 5
	EgPos NeckSidePt = m_Landmarks[POSE1_LANDMARK_NECK_SIDE]->m_Pos;
	FloorPt = NeckSidePt;

	NeckSidePt.m_Coords[0] += 200.0f;
	FloorPt.m_Coords[0] += 200.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_NECK_SIDE_LEVEL].push_back(NeckSidePt);
	m_Paths[POSE1_MEASURE_NECK_SIDE_LEVEL].push_back(FloorPt);
	m_Results[POSE1_MEASURE_NECK_SIDE_LEVEL] = NeckSidePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Neck side level : %f\n", m_Results[POSE1_MEASURE_NECK_SIDE_LEVEL]);


	///////////////////////////////////////  목앞높이 6
	EgPos NeckFrontPt = m_Landmarks[POSE1_LANDMARK_NECK_FRONT]->m_Pos;
	FloorPt = NeckFrontPt;

	NeckFrontPt.m_Coords[2] += 300.0f;
	FloorPt.m_Coords[2] += 300.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_NECK_FRONT_LEVEL].push_back(NeckFrontPt);
	m_Paths[POSE1_MEASURE_NECK_FRONT_LEVEL].push_back(FloorPt);
	m_Results[POSE1_MEASURE_NECK_FRONT_LEVEL] = NeckFrontPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Neck front level : %f\n", m_Results[POSE1_MEASURE_NECK_FRONT_LEVEL]);


	///////////////////////////////////////  어깨높이 7
	EgPos ShoulderPt = m_Landmarks[POSE1_LANDMARK_SHOULDER_R]->m_Pos;
	FloorPt = ShoulderPt;

	ShoulderPt.m_Coords[0] += 100.0f;
	FloorPt.m_Coords[0] += 100.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_SHOULDER_LEVEL].push_back(ShoulderPt);
	m_Paths[POSE1_MEASURE_SHOULDER_LEVEL].push_back(FloorPt);
	m_Results[POSE1_MEASURE_SHOULDER_LEVEL] = NeckFrontPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Shoulder level : %f\n", m_Results[POSE1_MEASURE_SHOULDER_LEVEL]);



	///////////////////////////////////////  어깨가쪽높이 8
	EgPos ShoulderSidePt = m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_R]->m_Pos;

	ShoulderSidePt.m_Coords[0] += 200.0f;
	FloorPt.m_Coords[0] += 200.0f;
	FloorPt = ShoulderSidePt;

	FloorPt.m_Coords[1] = 0.0f;
	m_Paths[POSE1_MEASURE_SHOULDERSIDE_LEVEL].push_back(ShoulderSidePt);
	m_Paths[POSE1_MEASURE_SHOULDERSIDE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_SHOULDERSIDE_LEVEL] = ShoulderSidePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Shoulder side level : %f\n", m_Results[POSE1_MEASURE_SHOULDERSIDE_LEVEL]);


	///////////////////////////////////////  겨드랑높이 9
	EgPos ArmpitPt = m_Landmarks[POSE1_LANDMARK_ARMPIT]->m_Pos;
	FloorPt = ArmpitPt;

	ArmpitPt.m_Coords[0] += 300.0f;
	FloorPt.m_Coords[0] += 300.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_ARMPIT_LEVEL].push_back(ArmpitPt);
	m_Paths[POSE1_MEASURE_ARMPIT_LEVEL].push_back(FloorPt);
	m_Results[POSE1_MEASURE_ARMPIT_LEVEL] = ArmpitPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Armpit level : %f\n", m_Results[POSE1_MEASURE_ARMPIT_LEVEL]);


	///////////////////////////////////////  젖가슴높이 10
	EgPos BustPt = m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos;
	FloorPt = BustPt;
	BustPt.m_Coords[2] += 200.0f;
	FloorPt.m_Coords[2] += 200.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_BP_LEVEL].push_back(BustPt);
	m_Paths[POSE1_MEASURE_BP_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_BP_LEVEL] = BustPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Bust level : %f\n", m_Results[POSE1_MEASURE_BP_LEVEL]);


	///////////////////////////////////////  젖가슴아래높이 11
	EgPos UnderBustPt = m_Landmarks[POSE1_LANDMARK_UNDERBUST]->m_Pos;
	FloorPt = UnderBustPt;

	UnderBustPt.m_Coords[2] += 400.0f;
	FloorPt.m_Coords[2] += 400.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_UNDERBUST_LEVEL].push_back(UnderBustPt);
	m_Paths[POSE1_MEASURE_UNDERBUST_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_UNDERBUST_LEVEL] = UnderBustPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Under Bust level : %f\n", m_Results[POSE1_MEASURE_UNDERBUST_LEVEL]);


	///////////////////////////////////////  미드리프높이 12
	EgPos MidRiffPt = m_Landmarks[POSE1_LANDMARK_MIDRIFF]->m_Pos;
	FloorPt = MidRiffPt;

	MidRiffPt.m_Coords[0] += 450.0f;
	FloorPt.m_Coords[0] += 450.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_MIDRIFF_LEVEL].push_back(MidRiffPt);
	m_Paths[POSE1_MEASURE_MIDRIFF_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_MIDRIFF_LEVEL] = MidRiffPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Mid Riff level : %f\n", m_Results[POSE1_MEASURE_MIDRIFF_LEVEL]);


	///////////////////////////////////////  허리높이 13
	EgPos WaistPt = m_Landmarks[POSE1_LANDMARK_WAIST]->m_Pos;
	FloorPt = WaistPt;

	WaistPt.m_Coords[0] += 350.0f;
	FloorPt.m_Coords[0] += +350.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_WAIST_LEVEL].push_back(WaistPt);
	m_Paths[POSE1_MEASURE_WAIST_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_WAIST_LEVEL] = WaistPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Waist level : %f\n", m_Results[POSE1_MEASURE_WAIST_LEVEL]);


	///////////////////////////////////////  배꼽수준허리높이 14
	EgPos NavelPt = m_Landmarks[POSE1_LANDMARK_NAVEL]->m_Pos;
	FloorPt = NavelPt;

	NavelPt.m_Coords[0] += 250.0f;
	FloorPt.m_Coords[0] += 250.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_NAVEL_LEVEL].push_back(NavelPt);
	m_Paths[POSE1_MEASURE_NAVEL_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_NAVEL_LEVEL] = NavelPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Navel level : %f\n", m_Results[POSE1_MEASURE_NAVEL_LEVEL]);


	///////////////////////////////////////  배높이 15
	EgPos StomachExtrudePt = m_Landmarks[POSE1_LANDMARK_STOMACH_EXTRUDE]->m_Pos;
	FloorPt = StomachExtrudePt;

	StomachExtrudePt.m_Coords[2] += 250.0f;
	FloorPt.m_Coords[2] += 250.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_STOMACH_EXTRUDE_LEVEL].push_back(StomachExtrudePt);
	m_Paths[POSE1_MEASURE_STOMACH_EXTRUDE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_STOMACH_EXTRUDE_LEVEL] = StomachExtrudePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("StomachE Extrude level : %f\n", m_Results[POSE1_MEASURE_STOMACH_EXTRUDE_LEVEL]);


	///////////////////////////////////////  Upper-hip높이 16
	EgPos UpperHipPt = m_Landmarks[POSE1_LANDMARK_UPPERHIP]->m_Pos;
	FloorPt = UpperHipPt;

	UpperHipPt.m_Coords[0] -= 150.0f;
	FloorPt.m_Coords[0] -= 150.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_UPPERHIP_LEVEL].push_back(UpperHipPt);
	m_Paths[POSE1_MEASURE_UPPERHIP_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_UPPERHIP_LEVEL] = UpperHipPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Upper Hip level : %f\n", m_Results[POSE1_MEASURE_UPPERHIP_LEVEL]);


	///////////////////////////////////////  Top-hip높이 17
	EgPos TopHipPt = m_Landmarks[POSE1_LANDMARK_TOPHIP]->m_Pos;
	FloorPt = TopHipPt;

	TopHipPt.m_Coords[2] = m_MinZ - 100.0f;
	FloorPt.m_Coords[2] = m_MinZ - 100.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_TOPHIP_LEVEL].push_back(TopHipPt);
	m_Paths[POSE1_MEASURE_TOPHIP_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_TOPHIP_LEVEL] = TopHipPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Top Hip level : %f\n", m_Results[POSE1_MEASURE_TOPHIP_LEVEL]);


	///////////////////////////////////////  엉덩이높이 18
	EgPos HipExtrudePt = m_Landmarks[POSE1_LANDMARK_HIP_EXTRUDE]->m_Pos;
	FloorPt = HipExtrudePt;

	HipExtrudePt.m_Coords[2] = m_MinZ - 150.0f;
	FloorPt.m_Coords[2] = m_MinZ - 150.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_HIP_EXTRUDE_LEVEL].push_back(HipExtrudePt);
	m_Paths[POSE1_MEASURE_HIP_EXTRUDE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_HIP_EXTRUDE_LEVEL] = HipExtrudePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Hip Extrude level : %f\n", m_Results[POSE1_MEASURE_HIP_EXTRUDE_LEVEL]);


	///////////////////////////////////////  샅높이 19
	EgPos CrotchPt = m_Landmarks[POSE1_LANDMARK_CROTCH]->m_Pos;
	FloorPt = CrotchPt;

	CrotchPt.m_Coords[2] = m_MaxZ + 50.0f;
	FloorPt.m_Coords[2] = m_MaxZ + 50.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_CROTCH_LEVEL].push_back(CrotchPt);
	m_Paths[POSE1_MEASURE_CROTCH_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_CROTCH_LEVEL] = CrotchPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Crotch level : %f\n", m_Results[POSE1_MEASURE_CROTCH_LEVEL]);


	///////////////////////////////////////  볼기고랑높이 20
	EgPos UnderHipPt = m_Landmarks[POSE1_LANDMARK_UNDERHIP]->m_Pos;
	FloorPt = UnderHipPt;

	UnderHipPt.m_Coords[2] = m_MinZ - 200.0f;
	FloorPt.m_Coords[2] = m_MinZ - 200.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_UNDERHIP_LEVEL].push_back(UnderHipPt);
	m_Paths[POSE1_MEASURE_UNDERHIP_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_UNDERHIP_LEVEL] = UnderHipPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Under Hip level : %f\n", m_Results[POSE1_MEASURE_UNDERHIP_LEVEL]);


	///////////////////////////////////////  무릎뼈가운데높이 21
	EgPos KneeMidPt = m_Landmarks[POSE1_LANDMARK_KNEE_MID]->m_Pos;
	FloorPt = KneeMidPt;

	KneeMidPt.m_Coords[2] = m_MaxZ + 100.0f;
	FloorPt.m_Coords[2] = m_MaxZ + 100.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_KNEE_MID_LEVEL].push_back(KneeMidPt);
	m_Paths[POSE1_MEASURE_KNEE_MID_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_KNEE_MID_LEVEL] = KneeMidPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Knee Mid level : %f\n", m_Results[POSE1_MEASURE_KNEE_MID_LEVEL]);


	///////////////////////////////////////  무릎높이 22
	EgPos KneeSidePt = m_Landmarks[POSE1_LANDMARK_KNEE_SIDE]->m_Pos;
	FloorPt = KneeSidePt;

	KneeSidePt.m_Coords[2] = m_MaxZ + 150.0f;
	FloorPt.m_Coords[2] = m_MaxZ + 150.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_KNEE_LEVEL].push_back(KneeSidePt);
	m_Paths[POSE1_MEASURE_KNEE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_KNEE_LEVEL] = KneeSidePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Knee level : %f\n", m_Results[POSE1_MEASURE_KNEE_LEVEL]);


	///////////////////////////////////////  장딴지높이 23
	EgPos CalfExtrudePt = m_Landmarks[POSE1_LANDMARK_CALF_EXTRUDE]->m_Pos;
	FloorPt = CalfExtrudePt;

	CalfExtrudePt.m_Coords[2] = m_MaxZ + 200.0f;
	FloorPt.m_Coords[2] = m_MaxZ + 200.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_CALF_EXTRUDE_LEVEL].push_back(CalfExtrudePt);
	m_Paths[POSE1_MEASURE_CALF_EXTRUDE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_CALF_EXTRUDE_LEVEL] = CalfExtrudePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Calf Extrude level : %f\n", m_Results[POSE1_MEASURE_CALF_EXTRUDE_LEVEL]);


	///////////////////////////////////////  가쪽복사높이 24
	EgPos AnklePt = m_Landmarks[POSE1_LANDMARK_ANKLE]->m_Pos;
	FloorPt = AnklePt;

	AnklePt.m_Coords[0] = m_MinX - 50.0f;
	FloorPt.m_Coords[0] = m_MinX - 50.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_ANKLE_LEVEL].push_back(AnklePt);
	m_Paths[POSE1_MEASURE_ANKLE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_ANKLE_LEVEL] = AnklePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Ankle level : %f\n", m_Results[POSE1_MEASURE_ANKLE_LEVEL]);


	///////////////////////////////////////  손끝높이 25
	EgPos HandPt = m_Landmarks[POSE1_LANDMARK_HAND]->m_Pos;
	FloorPt = HandPt;

	HandPt.m_Coords[0] = m_MinX - 100.0f;
	FloorPt.m_Coords[0] = m_MinX - 100.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_HAND_LEVEL].push_back(HandPt);
	m_Paths[POSE1_MEASURE_HAND_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_HAND_LEVEL] = HandPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Hand level : %f\n", m_Results[POSE1_MEASURE_HAND_LEVEL]);


	//////////////////////////////////////////////////////////////////////////// 길이 수준
	///////////////////////////////////////  어깨길이 26
	EgPos p;
	EgPos q;
	EgPos r;
	EgPlane pln;

	m_Results[POSE1_MEASURE_SHOULDER_LENGTH] = SK_Shortest_Distance(
		m_Landmarks[POSE1_LANDMARK_NECK_SIDE],
		m_Landmarks[POSE1_LANDMARK_SHOULDER_R],
		m_Paths[POSE1_MEASURE_SHOULDER_LENGTH]
	);

	_cprintf("Shoulder Length : %f\n", m_Results[POSE1_MEASURE_SHOULDER_LENGTH]);


	///////////////////////////////////////  어깨가쪽길이 27

	m_Results[POSE1_MEASURE_SHOULDERSIDE_LENGTH] = SK_Shortest_Distance(
		m_Landmarks[POSE1_LANDMARK_NECK_SIDE],
		m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_R],
		m_Paths[POSE1_MEASURE_SHOULDERSIDE_LENGTH]
	);

	_cprintf("Shoulder Side Length : %f\n", m_Results[POSE1_MEASURE_SHOULDERSIDE_LENGTH]);


	///////////////////////////////////////  어깨사이길이 28
	EgSlicer theSlicer;
	p = m_Landmarks[POSE1_LANDMARK_SHOULDER_R]->m_Pos;
	q = m_Landmarks[POSE1_LANDMARK_SHOULDER_L]->m_Pos;
	r = m_Landmarks[POSE1_LANDMARK_SHOULDER_L]->m_Pos;
	r.m_Coords[2] += 10.0f;

	pln = EgPlane(p, q, r);

	m_Results[POSE1_MEASURE_BTWN_SHOULDER_LENGTH] = SK_Planar_Distance(
		m_Landmarks[POSE1_LANDMARK_SHOULDER_R],
		m_Landmarks[POSE1_LANDMARK_SHOULDER_L],
		&pln,
		m_Paths[POSE1_MEASURE_BTWN_SHOULDER_LENGTH],
		false
	);

	_cprintf("Btwn Shoulder Length : %f\n", m_Results[POSE1_MEASURE_BTWN_SHOULDER_LENGTH]);


	///////////////////////////////////////  어깨가쪽사이길이 29
	p = m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_R]->m_Pos;
	q = m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_L]->m_Pos;
	r = m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_L]->m_Pos;
	r.m_Coords[2] += 10.0f;

	pln = EgPlane(p, q, r);
	m_Results[POSE1_MEASURE_BTWN_SHOULDERSIDE_LENGTH] = SK_Planar_Distance(
		m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_R],
		m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_L],
		&pln,
		m_Paths[POSE1_MEASURE_BTWN_SHOULDERSIDE_LENGTH],
		false
	);

	_cprintf("Btwn Shoulder Side Length : %f\n", m_Results[POSE1_MEASURE_BTWN_SHOULDERSIDE_LENGTH]);


	/////////////////////////////////////// 목뒤어깨길이 30
	m_Results[POSE1_MEASURE_BNECK_SHOULDER_LENGTH] =
		SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_BACK],
			m_Landmarks[POSE1_LANDMARK_SHOULDER_R],
			m_Paths[POSE1_MEASURE_BNECK_SHOULDER_LENGTH]
		);
	_cprintf("Bneck to Shoulder Length : %f\n", m_Results[POSE1_MEASURE_BNECK_SHOULDER_LENGTH]);


	/////////////////////////////////////// 목뒤어깨가쪽길이 31
	m_Results[POSE1_MEASURE_BNECK_SHOULDERSIDE_LENGTH] =
		SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_BACK],
			m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_R],
			m_Paths[POSE1_MEASURE_BNECK_SHOULDERSIDE_LENGTH]
		);
	_cprintf("Bneck to Shoulder Side Length : %f\n", m_Results[POSE1_MEASURE_BNECK_SHOULDERSIDE_LENGTH]);


	/////////////////////////////////////// 목뒤어깨사이길이 32
	std::vector<EgPos> r2bn, l2bn;
	m_Results[POSE1_MEASURE_BTWN_BNECK_SHOULDER_LENGTH] = SK_Shortest_Distance(
		m_Landmarks[POSE1_LANDMARK_SHOULDER_R],
		m_Landmarks[POSE1_LANDMARK_NECK_BACK],
		// m_Paths[POSE1_MEASURE_BTWN_BNECK_SHOULDER_LENGTH]
		r2bn
	);
	m_Results[POSE1_MEASURE_BTWN_BNECK_SHOULDER_LENGTH] += SK_Shortest_Distance(
		m_Landmarks[POSE1_LANDMARK_SHOULDER_L],
		m_Landmarks[POSE1_LANDMARK_NECK_BACK],
		// m_Paths[POSE1_MEASURE_BTWN_BNECK_SHOULDER_LENGTH]
		l2bn
	);
	m_Paths[POSE1_MEASURE_BTWN_BNECK_SHOULDER_LENGTH].insert(
		m_Paths[POSE1_MEASURE_BTWN_BNECK_SHOULDER_LENGTH].end(),
		r2bn.begin(),
		r2bn.end()
	);
	m_Paths[POSE1_MEASURE_BTWN_BNECK_SHOULDER_LENGTH].insert(
		m_Paths[POSE1_MEASURE_BTWN_BNECK_SHOULDER_LENGTH].end(),
		l2bn.begin(),
		l2bn.end()
	);

	std::sort(m_Paths[POSE1_MEASURE_BTWN_BNECK_SHOULDER_LENGTH].begin(), m_Paths[POSE1_MEASURE_BTWN_BNECK_SHOULDER_LENGTH].end(), sortPointByX);
	_cprintf("Between Bneck to Shoulder Length : %f\n", m_Results[POSE1_MEASURE_BTWN_BNECK_SHOULDER_LENGTH]);


	/////////////////////////////////////// 겨드랑뒤벽사이길이 33
	m_Results[POSE1_MEASURE_ARMPITWALL_BACK_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_ARMPITWALL_BACK_R],
			m_Landmarks[POSE1_LANDMARK_ARMPITWALL_BACK_L],
			m_Paths[POSE1_MEASURE_ARMPITWALL_BACK_LENGTH]);
	_cprintf("Armpit Wall Back Length : %f\n", m_Results[POSE1_MEASURE_ARMPITWALL_BACK_LENGTH]);


	/////////////////////////////////////// 겨드랑뒤접힘사이길이 34
	m_Results[POSE1_MEASURE_ARMPITFOLD_BACK_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_ARMPITFOLD_BACK_R],
			m_Landmarks[POSE1_LANDMARK_ARMPITFOLD_BACK_L],
			m_Paths[POSE1_MEASURE_ARMPITFOLD_BACK_LENGTH]);
	_cprintf("Armpit Fold Back Length : %f\n", m_Results[POSE1_MEASURE_ARMPITFOLD_BACK_LENGTH]);


	/////////////////////////////////////// 겨드랑앞벽사이길이 35
	m_Results[POSE1_MEASURE_ARMPITWALL_FRONT_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_ARMPITWALL_FRONT_R],
			m_Landmarks[POSE1_LANDMARK_ARMPITWALL_FRONT_L],
			m_Paths[POSE1_MEASURE_ARMPITWALL_FRONT_LENGTH]);
	_cprintf("Armpit Wall Front Length : %f\n", m_Results[POSE1_MEASURE_ARMPITWALL_FRONT_LENGTH]);


	/////////////////////////////////////// 겨드랑앞접힘사이길이 36
	m_Results[POSE1_MEASURE_ARMPITFOLD_FRONT_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_ARMPITFOLD_FRONT_R],
			m_Landmarks[POSE1_LANDMARK_ARMPITFOLD_FRONT_L],
			m_Paths[POSE1_MEASURE_ARMPITFOLD_FRONT_LENGTH]);
	_cprintf("Armpit Fold Front Length : %f\n", m_Results[POSE1_MEASURE_ARMPITFOLD_FRONT_LENGTH]);


	/////////////////////////////////////// 젖꼭지수평길이 37
	EgPos BP_R = m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos,
		BP_L = m_Landmarks[POSE1_LANDMARK_BP_L]->m_Pos;

	// 높이 수준을 맞춘다
	BP_L.m_Coords[1] = BP_R.m_Coords[1];

	BP_R.m_Coords[2] += 30.0f;
	BP_L.m_Coords[2] += 30.0f;

	m_Paths[POSE1_MEASURE_BP_LENGTH].push_back(BP_R);
	m_Paths[POSE1_MEASURE_BP_LENGTH].push_back(BP_L);
	m_Results[POSE1_MEASURE_BP_LENGTH] = ABS(BP_R.m_Coords[0] - BP_L.m_Coords[0]);
	_cprintf("BP Length : %f\n", m_Results[POSE1_MEASURE_BP_LENGTH]);



	/////////////////////////////////////// 겨드랑앞뒤접힘점사이길이 38
	EgPos ArmpitFoldPt_Front_R = m_Landmarks[POSE1_LANDMARK_ARMPITFOLD_FRONT_R]->m_Pos,
		ArmpitFoldPt_Back_R = m_Landmarks[POSE1_LANDMARK_ARMPITFOLD_BACK_R]->m_Pos;

	ArmpitFoldPt_Front_R.m_Coords[0] = m_MinX - 100.0f;
	ArmpitFoldPt_Back_R.m_Coords[0] = m_MinX - 100.0f;

	m_Paths[POSE1_MEASURE_ARMPITWALL_FRONT_BACK_LENGTH].push_back(ArmpitFoldPt_Front_R);
	m_Paths[POSE1_MEASURE_ARMPITWALL_FRONT_BACK_LENGTH].push_back(ArmpitFoldPt_Back_R);
	m_Results[POSE1_MEASURE_ARMPITWALL_FRONT_BACK_LENGTH] = dist(ArmpitFoldPt_Front_R, ArmpitFoldPt_Back_R);
	_cprintf("Armpit Wall Front-Back Length : %f\n", m_Results[POSE1_MEASURE_ARMPITWALL_FRONT_BACK_LENGTH]);



	/////////////////////////////////////// 앞중심길이 39
	EgPos FneckPt = m_Landmarks[POSE1_LANDMARK_NECK_FRONT]->m_Pos,
		FwaistPt = m_Landmarks[POSE1_LANDMARK_WAIST_FRONT]->m_Pos;

	FwaistPt.m_Coords[0] = FneckPt.m_Coords[0];
	EgVertex *FwaistVert = FindClosestVert(m_pMesh, FwaistPt);

	m_Results[POSE1_MEASURE_CENTER_FRONT_TO_WAIST_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_FRONT],
			FwaistVert,
			m_Paths[POSE1_MEASURE_CENTER_FRONT_TO_WAIST_LENGTH]);
	_cprintf("Center front to waist Length : %f\n", m_Results[POSE1_MEASURE_CENTER_FRONT_TO_WAIST_LENGTH]);



	/////////////////////////////////////// 배꼽수준앞중심길이 40
	NavelPt = m_Landmarks[POSE1_LANDMARK_NAVEL]->m_Pos;
	NavelPt.m_Coords[0] = FneckPt.m_Coords[0];
	EgVertex *NavelVertex = FindClosestVert(m_pMesh, NavelPt);

	m_Results[POSE1_MEASURE_CENTER_FRONT_TO_NAVEL_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_FRONT],
			NavelVertex,
			m_Paths[POSE1_MEASURE_CENTER_FRONT_TO_NAVEL_LENGTH]);
	_cprintf("Center front to navel Length : %f\n", m_Results[POSE1_MEASURE_CENTER_FRONT_TO_NAVEL_LENGTH]);


	/////////////////////////////////////// 목옆젖꼭지길이 41
	m_Results[POSE1_MEASURE_SNECK_BP_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_SIDE],
			m_Landmarks[POSE1_LANDMARK_BP_R],
			m_Paths[POSE1_MEASURE_SNECK_BP_LENGTH]);
	_cprintf("Side Neck to BP Length : %f\n", m_Results[POSE1_MEASURE_SNECK_BP_LENGTH]);


	/////////////////////////////////////// 목옆(젖꼭지)허리둘레선길이 42
	double Sneck2BP
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_SIDE],
			m_Landmarks[POSE1_LANDMARK_BP_R],
			m_Paths[POSE1_MEASURE_SNECK_BP_WAIST_LENGTH]);

	double BP2waistLevel = ABS(m_Landmarks[POSE1_LANDMARK_NECK_SIDE]->m_Pos[1] - m_Landmarks[POSE1_LANDMARK_WAIST]->m_Pos[1]);

	m_Results[POSE1_MEASURE_SNECK_BP_WAIST_LENGTH] = Sneck2BP + BP2waistLevel;

	m_Paths[POSE1_MEASURE_SNECK_BP_WAIST_LENGTH].push_back(EgPos(m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos[0],
		m_Landmarks[POSE1_LANDMARK_WAIST]->m_Pos[1],
		m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos[2]));
	std::sort(m_Paths[POSE1_MEASURE_SNECK_BP_WAIST_LENGTH].begin(), m_Paths[POSE1_MEASURE_SNECK_BP_WAIST_LENGTH].end(), sortPointByY);

	_cprintf("Side Neck to BP to Waist Level Length : %f\n", m_Results[POSE1_MEASURE_SNECK_BP_WAIST_LENGTH]);


	/////////////////////////////////////// 목뒤젖꼭지길이 43
	// 목옆점에서 젖꼭지점까지 수직거리이기 때문에 BustPt의 x 수준을 SneckPt x 수준으로 이동한다
	std::vector<EgPos> bn2sn, sn2bp;

	double Bneck2Sneck = SK_Shortest_Distance(
		m_Landmarks[POSE1_LANDMARK_NECK_BACK],
		m_Landmarks[POSE1_LANDMARK_NECK_SIDE],
		// m_Paths[POSE1_MEASURE_BNECK_BP_LENGTH]
		bn2sn
	);

	Sneck2BP = SK_Shortest_Distance(
		m_Landmarks[POSE1_LANDMARK_NECK_SIDE],
		m_Landmarks[POSE1_LANDMARK_BP_R],
		sn2bp
	);

	m_Results[POSE1_MEASURE_BNECK_BP_LENGTH] = Bneck2Sneck + Sneck2BP;

	m_Paths[POSE1_MEASURE_BNECK_BP_LENGTH].insert(
		m_Paths[POSE1_MEASURE_BNECK_BP_LENGTH].end(),
		bn2sn.begin(),
		bn2sn.end()
	);
	m_Paths[POSE1_MEASURE_BNECK_BP_LENGTH].insert(
		m_Paths[POSE1_MEASURE_BNECK_BP_LENGTH].end(),
		sn2bp.begin(),
		sn2bp.end()
	);
	std::sort(m_Paths[POSE1_MEASURE_BNECK_BP_LENGTH].begin(), m_Paths[POSE1_MEASURE_BNECK_BP_LENGTH].end(), sortPointByZ);


	_cprintf("Bneck to Sneck to BP Length : %f\n", m_Results[POSE1_MEASURE_BNECK_BP_LENGTH]);


	/////////////////////////////////////// 목뒤젖꼭지허리둘레선길이 44

	m_Paths[POSE1_MEASURE_BNECK_BP_WAIST_LENGTH].insert(
		m_Paths[POSE1_MEASURE_BNECK_BP_WAIST_LENGTH].end(),
		m_Paths[POSE1_MEASURE_BNECK_BP_LENGTH].begin(),
		m_Paths[POSE1_MEASURE_BNECK_BP_LENGTH].end()
	);
	m_Paths[POSE1_MEASURE_BNECK_BP_WAIST_LENGTH].push_back(EgPos(m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos[0], m_Landmarks[POSE1_LANDMARK_WAIST]->m_Pos[1], m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos[2]));

	double BP2WaistLevel = m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos[1] - m_Landmarks[POSE1_LANDMARK_WAIST]->m_Pos[1];

	m_Results[POSE1_MEASURE_BNECK_BP_WAIST_LENGTH] = Bneck2Sneck + Sneck2BP + BP2WaistLevel;

	_cprintf("Bneck to Sneck to BP to Waist Level : %f\n", m_Results[POSE1_MEASURE_BNECK_BP_WAIST_LENGTH]);


	/////////////////////////////////////// 목뒤등뼈위겨드랑수준길이 45
	// Armpit을 Bneck x 수준으로 일치시킨 후, 뒤로 ray를 쏴서 만나는 점과의 체표길이를 구한다
	EgLine ray(EgPos(m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos[0], m_Landmarks[POSE1_LANDMARK_ARMPIT]->m_Pos[1], m_Landmarks[POSE1_LANDMARK_ARMPIT]->m_Pos[2]), EgVec3(0, 0, 1));
	EgPos tmpArmpitPt;

	for (EgFace *face : m_pMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		EgPos tmpIntersection;
		if (intersect_line_triangle(p0, p1, p2, ray, tmpIntersection, false)) {
			if (tmpIntersection.m_Coords[2] < m_Landmarks[POSE1_LANDMARK_ARMPIT]->m_Pos[2])
				tmpArmpitPt = tmpIntersection;
		}
	}

	m_Results[POSE1_MEASURE_BNECK_ARMPIT_LEVEL_LENGTH] = SK_Shortest_Distance(
		m_Landmarks[POSE1_LANDMARK_NECK_BACK],
		FindClosestVert(m_pMesh, tmpArmpitPt),
		m_Paths[POSE1_MEASURE_BNECK_ARMPIT_LEVEL_LENGTH]
	);
	_cprintf("Bneck to Armpit Level : %f\n", m_Results[POSE1_MEASURE_BNECK_ARMPIT_LEVEL_LENGTH]);


	/////////////////////////////////////// 목옆뒤허리둘레선길이 46

	m_Results[POSE1_MEASURE_SNECK_BWAIST_LEVEL_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_SIDE],
			FindClosestVert(m_pMesh, EgPos(m_Landmarks[POSE1_LANDMARK_NECK_SIDE]->m_Pos[0], m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos[1], m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos[2])),
			m_Paths[POSE1_MEASURE_SNECK_BWAIST_LEVEL_LENGTH]);

	_cprintf("Side Neck to BP Length : %f\n", m_Results[POSE1_MEASURE_SNECK_BWAIST_LEVEL_LENGTH]);


	/////////////////////////////////////// 등길이 47
	m_Results[POSE1_MEASURE_BNECK_BWAIST_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_BACK],
			FindClosestVert(m_pMesh, EgPos(m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos[0], m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos[1], m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos[2])),
			m_Paths[POSE1_MEASURE_BNECK_BWAIST_LENGTH]);

	_cprintf("Back Length : %f\n", m_Results[POSE1_MEASURE_BNECK_BWAIST_LENGTH]);


	/////////////////////////////////////// 배꼽수준등길이 48

	m_Results[POSE1_MEASURE_BNECK_BNAVEL_LENGTH] =
		SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_BACK],
			FindClosestVert(m_pMesh, EgPos(m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos[0], m_Landmarks[POSE1_LANDMARK_NAVEL_BACK]->m_Pos[1], m_Landmarks[POSE1_LANDMARK_NAVEL_BACK]->m_Pos[2])),
			m_Paths[POSE1_MEASURE_BNECK_BNAVEL_LENGTH]
		);
	_cprintf("Navel level Back Length : %f\n", m_Results[POSE1_MEASURE_BNECK_BNAVEL_LENGTH]);


	/////////////////////////////////////// 목뒤오금길이 49

	std::vector<EgPos> Bneck2BwaistPath;
	double Bneck2Bwaist = SK_Shortest_Distance(
		m_Landmarks[POSE1_LANDMARK_NECK_BACK],
		FindClosestVert(m_pMesh, EgPos(m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos[0], m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos[1], m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos[2])),
		Bneck2BwaistPath
	);
	std::sort(Bneck2BwaistPath.begin(), Bneck2BwaistPath.end(), sortPointByY);

	std::vector<EgPos> Bwaist2HipExtrudePath;
	double Bwaist2HipExtrude = SK_Shortest_Distance(
		FindClosestVert(m_pMesh, EgPos(m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos[0], m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos[1], m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos[2])),
		FindClosestVert(m_pMesh, EgPos(m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos[0], m_Landmarks[POSE1_LANDMARK_HIP_EXTRUDE]->m_Pos[1], m_Landmarks[POSE1_LANDMARK_HIP_EXTRUDE]->m_Pos[2])),
		Bwaist2HipExtrudePath
	);
	std::sort(Bwaist2HipExtrudePath.begin(), Bwaist2HipExtrudePath.end(), sortPointByY);

	double HipExtrude2Popliteus = m_Landmarks[POSE1_LANDMARK_HIP_EXTRUDE]->m_Pos[1] - m_Landmarks[POSE1_LANDMARK_POPLITEUS]->m_Pos[1];

	m_Results[POSE1_MEASURE_BNECK_BWAIST_HIPEXTRUDE_POPLITEUS_LENGTH] = Bneck2Bwaist + Bwaist2HipExtrude + HipExtrude2Popliteus;

	m_Paths[POSE1_MEASURE_BNECK_BWAIST_HIPEXTRUDE_POPLITEUS_LENGTH].insert(
		m_Paths[POSE1_MEASURE_BNECK_BWAIST_HIPEXTRUDE_POPLITEUS_LENGTH].end(),
		Bneck2BwaistPath.begin(),
		Bneck2BwaistPath.end()
	);
	m_Paths[POSE1_MEASURE_BNECK_BWAIST_HIPEXTRUDE_POPLITEUS_LENGTH].insert(
		m_Paths[POSE1_MEASURE_BNECK_BWAIST_HIPEXTRUDE_POPLITEUS_LENGTH].end(),
		Bwaist2HipExtrudePath.begin(),
		Bwaist2HipExtrudePath.end()
	);
	m_Paths[POSE1_MEASURE_BNECK_BWAIST_HIPEXTRUDE_POPLITEUS_LENGTH].push_back(EgPos(m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos[0], m_Landmarks[POSE1_LANDMARK_POPLITEUS]->m_Pos[1], m_Landmarks[POSE1_LANDMARK_HIP_EXTRUDE]->m_Pos[2]));
	_cprintf("Bneck to Bwaist to Hip Extrude to Popliteus : %f\n", m_Results[POSE1_MEASURE_BNECK_BWAIST_HIPEXTRUDE_POPLITEUS_LENGTH]);


	/////////////////////////////////////// 총길이 50
	m_Paths[POSE1_MEASURE_BACK_LENGTH].insert(
		m_Paths[POSE1_MEASURE_BACK_LENGTH].end(),
		Bneck2BwaistPath.begin(),
		Bneck2BwaistPath.end()
	);
	m_Paths[POSE1_MEASURE_BACK_LENGTH].insert(
		m_Paths[POSE1_MEASURE_BACK_LENGTH].end(),
		Bwaist2HipExtrudePath.begin(),
		Bwaist2HipExtrudePath.end()
	);
	m_Paths[POSE1_MEASURE_BACK_LENGTH].push_back(EgPos(m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos[0], 0, m_Landmarks[POSE1_LANDMARK_HIP_EXTRUDE]->m_Pos[2]));

	double HipExtrude2Floor = HipExtrudePt.m_Coords[1];

	m_Results[POSE1_MEASURE_BACK_LENGTH] = Bneck2Bwaist + Bwaist2HipExtrude + HipExtrude2Floor;

	// _cprintf("Back Length : %f\n", m_Results[POSE1_MEASURE_BACK_LENGTH]);




	/////////////////////////////////////// 팔길이(어깨점) 53
	//pln = EgPlane(m_Landmarks[POSE1_LANDMARK_SHOULDER_R]->m_Pos, m_Landmarks[POSE1_LANDMARK_ELBOW]->m_Pos, m_Landmarks[POSE1_LANDMARK_WRIST]->m_Pos);
	//m_Results[POSE1_MEASURE_SHOULDER_ELBOW_WRIST_LENGTH] = SK_Planar_Distance(
	//	m_Landmarks[POSE1_LANDMARK_SHOULDER_R],
	//	m_Landmarks[POSE1_LANDMARK_WRIST],
	//	&pln,
	//	m_Paths[POSE1_MEASURE_SHOULDER_ELBOW_WRIST_LENGTH],
	//	true
	//);
	std::vector<EgPos> UpperPath, LowerPath;
	double UpperLen = SK_Planar_Distance(
		m_Landmarks[POSE1_LANDMARK_SHOULDER_R],
		m_Landmarks[POSE1_LANDMARK_ELBOW],
		UpperPath,
		true
	);
	double LowerLen = SK_Planar_Distance(
		m_Landmarks[POSE1_LANDMARK_ELBOW],
		m_Landmarks[POSE1_LANDMARK_WRIST],
		LowerPath,
		true
	);
	m_Results[POSE1_MEASURE_SHOULDER_ELBOW_WRIST_LENGTH] = UpperLen + LowerLen;
	m_Paths[POSE1_MEASURE_SHOULDER_ELBOW_WRIST_LENGTH] = UpperPath;
	for (EgPos p : LowerPath)
		m_Paths[POSE1_MEASURE_SHOULDER_ELBOW_WRIST_LENGTH].push_back(p);
	_cprintf("Shouler to Elbow to Wrist Length : %f\n", m_Results[POSE1_MEASURE_SHOULDER_ELBOW_WRIST_LENGTH]);


	/////////////////////////////////////// 위팔길이(어깨점) 51
	pln = EgPlane(m_Landmarks[POSE1_LANDMARK_SHOULDER_R]->m_Pos, m_Landmarks[POSE1_LANDMARK_ELBOW]->m_Pos, m_Landmarks[POSE1_LANDMARK_WRIST]->m_Pos);

	m_Results[POSE1_MEASURE_SHOULDER_ELBOW_LENGTH] = UpperLen;
	for (EgPos pos : UpperPath)
		m_Paths[POSE1_MEASURE_SHOULDER_ELBOW_LENGTH].push_back(pos);



	/////////////////////////////////////// 팔길이(어깨가쪽점) 54
	//pln = EgPlane(m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_R]->m_Pos, m_Landmarks[POSE1_LANDMARK_ELBOW]->m_Pos, m_Landmarks[POSE1_LANDMARK_WRIST]->m_Pos);
	//m_Results[POSE1_MEASURE_SHOULDERSIDE_ELBOW_WRIST_LENGTH] = SK_Planar_Distance(
	//	m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_R],
	//	m_Landmarks[POSE1_LANDMARK_WRIST],
	//	&pln,
	//	m_Paths[POSE1_MEASURE_SHOULDERSIDE_ELBOW_WRIST_LENGTH],
	//	true
	//);
	// _cprintf("Shouler Side to Elbow to Wrist Length : %f\n", m_Results[POSE1_MEASURE_SHOULDERSIDE_ELBOW_WRIST_LENGTH]);
	UpperPath.clear();
	LowerPath.clear();
	UpperLen = SK_Planar_Distance(
		m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_R],
		m_Landmarks[POSE1_LANDMARK_ELBOW],
		UpperPath,
		true
	);
	LowerLen = SK_Planar_Distance(
		m_Landmarks[POSE1_LANDMARK_ELBOW],
		m_Landmarks[POSE1_LANDMARK_WRIST],
		LowerPath,
		true
	);
	m_Results[POSE1_MEASURE_SHOULDERSIDE_ELBOW_WRIST_LENGTH] = UpperLen + LowerLen;
	m_Paths[POSE1_MEASURE_SHOULDERSIDE_ELBOW_WRIST_LENGTH] = UpperPath;
	for (EgPos p : LowerPath)
		m_Paths[POSE1_MEASURE_SHOULDERSIDE_ELBOW_WRIST_LENGTH].push_back(p);
	_cprintf("Shouler Side to Elbow to Wrist Length : %f\n", m_Results[POSE1_MEASURE_SHOULDERSIDE_ELBOW_WRIST_LENGTH]);


	/////////////////////////////////////// 위팔길이(어깨가쪽점) 52
	m_Results[POSE1_MEASURE_SHOULDERSIDE_ELBOW_LENGTH] = UpperLen;
	for (EgPos pos : UpperPath)
		m_Paths[POSE1_MEASURE_SHOULDERSIDE_ELBOW_LENGTH].push_back(pos);



	/////////////////////////////////////// 몸통수직길이 55
	BneckPt = m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos;
	CrotchPt = m_Landmarks[POSE1_LANDMARK_CROTCH]->m_Pos;

	BneckPt.m_Coords[2] = m_MinZ - 300.0f;
	CrotchPt.m_Coords[2] = m_MinZ - 300.0f;
	CrotchPt.m_Coords[0] = BneckPt.m_Coords[0];

	m_Paths[POSE1_MEASURE_TORSO_LENGTH].push_back(BneckPt);
	m_Paths[POSE1_MEASURE_TORSO_LENGTH].push_back(CrotchPt);

	m_Results[POSE1_MEASURE_TORSO_LENGTH] = BneckPt.m_Coords[1] - CrotchPt.m_Coords[1];

	// _cprintf("Torso Length : %f\n", m_Results[POSE1_MEASURE_TORSO_LENGTH]);



	/////////////////////////////////////// 엉덩이수직길이 56
	EgPos BwaistPt = m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos;
	CrotchPt = m_Landmarks[POSE1_LANDMARK_CROTCH]->m_Pos;

	BwaistPt.m_Coords[2] = m_MinZ - 400.0f;
	CrotchPt.m_Coords[2] = m_MinZ - 400.0f;
	CrotchPt.m_Coords[0] = BwaistPt.m_Coords[0];

	m_Paths[POSE1_MEASURE_HIP_LENGTH].push_back(BwaistPt);
	m_Paths[POSE1_MEASURE_HIP_LENGTH].push_back(CrotchPt);

	m_Results[POSE1_MEASURE_HIP_LENGTH] = BwaistPt.m_Coords[1] - CrotchPt.m_Coords[1];
	// _cprintf("Hip Length : %f\n", m_Results[POSE1_MEASURE_HIP_LENGTH]);


	/////////////////////////////////////// 목뒤손목안쪽길이(팔내린) 57
	std::vector<EgPos> Bneck2ShoulderPath, Shoulder2WristPath;

	double Bneck2Shoulder =
		SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_BACK],
			m_Landmarks[POSE1_LANDMARK_SHOULDER_R],
			Bneck2ShoulderPath
		);

	std::sort(Bneck2ShoulderPath.begin(), Bneck2ShoulderPath.end(), sortPointByY);

	m_Paths[POSE1_MEASURE_BNECK_SHOULDER_WRIST_LENGTH].insert(
		m_Paths[POSE1_MEASURE_BNECK_SHOULDER_WRIST_LENGTH].end(),
		Bneck2ShoulderPath.begin(),
		Bneck2ShoulderPath.end()
	);
	m_Paths[POSE1_MEASURE_BNECK_SHOULDER_WRIST_LENGTH].insert(
		m_Paths[POSE1_MEASURE_BNECK_SHOULDER_WRIST_LENGTH].end(),
		m_Paths[POSE1_MEASURE_SHOULDER_ELBOW_WRIST_LENGTH].begin(),
		m_Paths[POSE1_MEASURE_SHOULDER_ELBOW_WRIST_LENGTH].end()
	);
	m_Results[POSE1_MEASURE_BNECK_SHOULDER_WRIST_LENGTH] = Bneck2Shoulder + m_Results[POSE1_MEASURE_SHOULDER_ELBOW_WRIST_LENGTH];

	// _cprintf("Bneck to Shoulder to Wrist Length : %f\n", m_Results[POSE1_MEASURE_BNECK_SHOULDER_WRIST_LENGTH]);


	/////////////////////////////////////// 넙다리직선길이 58
	m_Paths[POSE1_MEASURE_UNDERHIP_POPLITEUS_LENGTH].push_back(m_Landmarks[POSE1_LANDMARK_UNDERHIP]->m_Pos);
	m_Paths[POSE1_MEASURE_UNDERHIP_POPLITEUS_LENGTH].push_back(m_Landmarks[POSE1_LANDMARK_POPLITEUS]->m_Pos);

	m_Results[POSE1_MEASURE_UNDERHIP_POPLITEUS_LENGTH] = dist(m_Landmarks[POSE1_LANDMARK_UNDERHIP]->m_Pos, m_Landmarks[POSE1_LANDMARK_POPLITEUS]->m_Pos);


	/////////////////////////////////////////////////////////// 두께 수준
	/////////////////////////////////////// 벽면배돌출두께 59
	StomachExtrudePt = m_Landmarks[POSE1_LANDMARK_STOMACH_EXTRUDE]->m_Pos;
	HipExtrudePt = m_Landmarks[POSE1_LANDMARK_HIP_EXTRUDE]->m_Pos;

	StomachExtrudePt.m_Coords[0] = m_MinX - 350.0f;
	HipExtrudePt.m_Coords[0] = m_MinX - 350.0f;
	HipExtrudePt.m_Coords[1] = StomachExtrudePt.m_Coords[1];

	m_Paths[POSE1_MEASURE_STOMACH_HIP_DEPTH].push_back(StomachExtrudePt);
	m_Paths[POSE1_MEASURE_STOMACH_HIP_DEPTH].push_back(HipExtrudePt);

	m_Results[POSE1_MEASURE_STOMACH_HIP_DEPTH] = StomachExtrudePt.m_Coords[2] - HipExtrudePt.m_Coords[2];
	// _cprintf("Stomach-Hip Depth : %f\n", m_Results[POSE1_MEASURE_STOMACH_HIP_DEPTH]);


	/////////////////////////////////////// 벽면가슴두께 60
	BP_R = m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos;
	EgPos BackExtrudePt = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_BACK_EXTRUDE]->m_Pos;

	BP_R.m_Coords[0] = m_MinX - 450.0f;
	BackExtrudePt.m_Coords[0] = m_MinX - 450.0f;
	BackExtrudePt.m_Coords[1] = BP_R.m_Coords[1];

	m_Paths[POSE1_MEASURE_BUST_DEPTH].push_back(BP_R);
	m_Paths[POSE1_MEASURE_BUST_DEPTH].push_back(BackExtrudePt);

	m_Results[POSE1_MEASURE_BUST_DEPTH] = BP_R.m_Coords[2] - BackExtrudePt.m_Coords[2];
	// _cprintf("Bust Depth : %f\n", m_Results[POSE1_MEASURE_BUST_DEPTH]);



	/////////////////////////////////////// 벽면몸통두께 61
	BP_R = m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos;
	StomachExtrudePt = m_Landmarks[POSE1_LANDMARK_STOMACH_EXTRUDE]->m_Pos;
	HipExtrudePt = m_Landmarks[POSE1_LANDMARK_HIP_EXTRUDE]->m_Pos;

	EgPos higherExtrusionPt;
	if (BP_R.m_Coords[2] > StomachExtrudePt.m_Coords[2])
		higherExtrusionPt = BP_R;
	else
		higherExtrusionPt = StomachExtrudePt;

	higherExtrusionPt.m_Coords[0] = m_MinX - 600.0f;
	HipExtrudePt.m_Coords[0] = m_MinX - 600.0f;
	HipExtrudePt.m_Coords[1] = higherExtrusionPt.m_Coords[1];

	m_Paths[POSE1_MEASURE_TORSO_DEPTH].push_back(higherExtrusionPt);
	m_Paths[POSE1_MEASURE_TORSO_DEPTH].push_back(HipExtrudePt);

	m_Results[POSE1_MEASURE_TORSO_DEPTH] = higherExtrusionPt.m_Coords[2] - HipExtrudePt.m_Coords[2];
	// _cprintf("Torso Depth : %f\n", m_Results[POSE1_MEASURE_TORSO_DEPTH]);



	/////////////////////////////////////////////////////////// 각도 수준
	/////////////////////////////////////// 오른쪽어깨가쪽경사각 62
	EgLine StandardLine(m_Landmarks[POSE1_LANDMARK_NECK_SIDE]->m_Pos, EgVec3(-1, 0, 0));
	EgLine ShoulderSideLine(m_Landmarks[POSE1_LANDMARK_NECK_SIDE]->m_Pos, m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_R]->m_Pos);

	m_Results[POSE1_MEASURE_SHOULDERSIDE_RIGHT_ANGLE] = angle(StandardLine.m_Dir, ShoulderSideLine.m_Dir, false);
	// _cprintf("Shoulder Side Right Angle : %f\n", m_Results[POSE1_MEASURE_SHOULDERSIDE_RIGHT_ANGLE]);


	/////////////////////////////////////// 왼쪽어깨가쪽경사각 63
	EgPos SneckPt_L = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_NECK_SIDE_L]->m_Pos;
	EgPos ShoulderSidePt_L = m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_L]->m_Pos;

	StandardLine = EgLine(SneckPt_L, EgVec3(1, 0, 0));
	ShoulderSideLine = EgLine(SneckPt_L, ShoulderSidePt_L);

	m_Results[POSE1_MEASURE_SHOULDERSIDE_LEFT_ANGLE] = angle(StandardLine.m_Dir, ShoulderSideLine.m_Dir, false);
	// _cprintf("Shoulder Side Left Angle : %f\n", m_Results[POSE1_MEASURE_SHOULDERSIDE_LEFT_ANGLE]);


	/////////////////////////////////////// 오른쪽어깨경사각 64
	StandardLine = EgLine(m_Landmarks[POSE1_LANDMARK_NECK_SIDE]->m_Pos, EgVec3(-1, 0, 0));
	EgLine ShoulderLine(m_Landmarks[POSE1_LANDMARK_NECK_SIDE]->m_Pos, m_Landmarks[POSE1_LANDMARK_SHOULDER_R]->m_Pos);

	m_Results[POSE1_MEASURE_SHOULDER_RIGHT_ANGLE] = angle(StandardLine.m_Dir, ShoulderLine.m_Dir, false);
	// _cprintf("Shoulder Side Angle : %f\n", m_Results[POSE1_MEASURE_SHOULDER_RIGHT_ANGLE]);


	/////////////////////////////////////// 왼쪽어깨경사각 65
	SneckPt_L = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_NECK_SIDE_L]->m_Pos;
	EgPos ShoulderPt_L = m_Landmarks[POSE1_LANDMARK_SHOULDER_L]->m_Pos;

	StandardLine = EgLine(SneckPt_L, EgVec3(1, 0, 0));
	ShoulderLine = EgLine(SneckPt_L, ShoulderPt_L);

	m_Results[POSE1_MEASURE_SHOULDER_LEFT_ANGLE] = angle(StandardLine.m_Dir, ShoulderLine.m_Dir, false);
	// _cprintf("Shoulder Left Angle : %f\n", m_Results[POSE1_MEASURE_SHOULDER_LEFT_ANGLE]);
}

void EgSizer::FindLandmarkPose2() {
	for (int i = 0; i < NEW_LANDMARK_NUM_POSE2; i++) {
		m_Landmarks[i] = m_nLandmarkPose2[i];
		_cprintf("%d : %f %f %f\n", i, m_Landmarks[i]->m_Pos[0], m_Landmarks[i]->m_Pos[1], m_Landmarks[i]->m_Pos[2]);
	}
}

// mj::m_nLandmarkPose2를 m_Landmarks에 할당한다
void EgSizer::MeasurePose2() {
	for (int i = 0; i < NEW_RESULT_NUM_POSE2; i++) {
		m_Results[i] = m_nResultPose2[i];
	}

	for (int i = 0; i < NEW_RESULT_NUM_POSE2; i++) {
		m_Paths[i] = m_nResultPathPose2[i];
	}
}

/*!
*	\brief	자세 3의 측정점을 찾는다.
*	\note	마지막 수정일: 2020-07-15
*/
void EgSizer::FindLandmarkPose3()	// 수동랜드마크 
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame->m_pLandmarkProgressDlg == NULL)
	{
		pFrame->m_pLandmarkProgressDlg = new CProgressDialog();
		pFrame->m_pLandmarkProgressDlg->Create(IDD_PROGRESS_DIALOG);
		pFrame->m_pLandmarkProgressDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pFrame->m_pLandmarkProgressDlg->ShowWindow(SW_SHOW);
		pFrame->m_pLandmarkProgressDlg->SetFocus();
	}
	int NumMarks = (int)m_Landmarks.size();
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetRange(0, NumMarks);

	// 1. 엉덩이돌출점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(1);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("엉덩이돌출점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	int idx = GetLandmarkIdx("엉덩이돌출점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Buttock_Protrusion(m_pMesh);

	// 2. 배돌출점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(2);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("배돌출점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("배돌출점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Abdominal_Protrusion(m_pMesh);

	// 3. 엉덩이최대둘레수준점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(3);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("엉덩이최대둘레수준점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("엉덩이최대둘레수준점");
	std::vector<EgPos> Path;
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Buttock_Max_Perimeter(m_pMesh, Path);

	// 4. 주먹가운데점(왼쪽)
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(4);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("왼쪽주먹가운데점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("왼쪽주먹가운데점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Left_Fist_Center(m_pMesh);

	// 4. 주먹가운데점(오른쪽)
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(5);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("오른쪽주먹가운데점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("오른쪽주먹가운데점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Right_Fist_Center(m_pMesh);

	// 5. 오른쪽 등부위돌출점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(6);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("오른쪽등부위돌출점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("오른쪽등부위돌출점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Right_Back_Protrusion(m_pMesh);

	// 6. 팔꿈치뒤점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(7);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("왼쪽팔꿈치뒤점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("왼쪽팔꿈치뒤점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Rear_Olecranon(m_pMesh);

	// 7. 팔꿈치아래점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(8);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("왼쪽팔꿈치아래점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("왼쪽팔꿈치아래점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Bottom_Olecranon(m_pMesh);

	// 8. 오른쪽 어깨점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(9);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("오른쪽어깨점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("오른쪽어깨점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Right_Acromion(m_pMesh);

	// 상태 진행 창을 초기화 한다.
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(0);
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	pFrame->m_pLandmarkProgressDlg->ShowWindow(FALSE);
}

/*!
*	\brief	자세 3의 측정 항목을 계산한다.
*	\note	마지막 수정일: 2020-10-22
*/
void EgSizer::MeasurePose3()
{
	int mark_idx, mark_idx1;
	double minY = m_pMesh->m_BndBox.m_MinPt[1];

	// 1. 엉덩이 최대둘레 높이
	mark_idx = GetLandmarkIdx("엉덩이최대둘레수준점");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("엉덩이최대둘레높이");
		m_Results[result_idx] = p[1] - minY;
		m_Paths[result_idx].push_back(p);
		m_Paths[result_idx].push_back(EgPos(p[0], minY, p[2]));
	}

	// 2. 굽힌 팔꿈치 높이
	mark_idx = GetLandmarkIdx("왼쪽팔꿈치아래점");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("굽힌팔꿈치높이");
		m_Results[result_idx] = p[1] - minY;
		m_Paths[result_idx].push_back(p);
		m_Paths[result_idx].push_back(EgPos(p[0], minY, p[2]));
	}

	// 3. 엉덩이 둘레
	mark_idx = GetLandmarkIdx("엉덩이돌출점");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("엉덩이둘레");
		EgPlane Pln(EgVec3(0.0, 1.0, 0.0), p);
		double len = SK_Planar_Perimeter(m_Landmarks[mark_idx], &Pln, m_Paths[result_idx], true);
		m_Results[result_idx] = len;
	}

	// 4. 엉덩이최대둘레
	mark_idx = GetLandmarkIdx("엉덩이최대둘레수준점");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("엉덩이최대둘레");
		EgPlane Pln(EgVec3(0.0, 1.0, 0.0), p);
		double len = SK_Planar_Perimeter(m_Landmarks[mark_idx], &Pln, m_Paths[result_idx], true);
		m_Results[result_idx] = len;
	}

	// 5. 배돌출점기준엉덩이둘레
	mark_idx = GetLandmarkIdx("엉덩이돌출점");
	mark_idx1 = GetLandmarkIdx("배돌출점");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		// 엉덩이 둘레 정점을 복사한다.
		std::vector<EgPos> Pts1 = m_Paths[GetResultIdx("엉덩이둘레")];

		// 배돌출점 둘레를 구한다.
		std::vector<EgPos> Pts2;
		EgPlane Pln(EgVec3(0.0, 1.0, 0.0), m_Landmarks[mark_idx1]->m_Pos);
		double len = SK_Planar_Perimeter(m_Landmarks[mark_idx1], &Pln, Pts2, true);

		// 엉덩이 둘레 정점과 배돌출점 둘레 정점을 합친다.
		for (EgPos pt : Pts2)
		{
			pt[1] = Pts1[0][1];
			Pts1.push_back(pt);
		}

		// 배돌출점기준엉덩이둘레를 구한다.
		int result_idx = GetResultIdx("배돌출점기준엉덩이둘레");
		Pln = EgPlane(EgVec3(0.0, 1.0, 0.0), m_Landmarks[mark_idx]->m_Pos);
		m_Results[result_idx] = SK_ConvexHull(&Pln, Pts1, m_Paths[result_idx]);
	}

	// 6. 팔꿈치주먹수평길이
	mark_idx = GetLandmarkIdx("왼쪽주먹가운데점");
	mark_idx1 = GetLandmarkIdx("왼쪽팔꿈치뒤점");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		EgPos q = m_Landmarks[mark_idx1]->m_Pos;
		int result_idx = GetResultIdx("팔꿈치주먹수평길이");
		m_Results[result_idx] = p[2] - q[2];
		m_Paths[result_idx].push_back(EgPos(p[0] + 50, p[1], p[2]));
		m_Paths[result_idx].push_back(EgPos(p[0] + 50, p[1], q[2]));
	}

	// 7. 벽면앞으로뻗은주먹수평길이
	mark_idx = GetLandmarkIdx("오른쪽주먹가운데점");
	mark_idx1 = GetLandmarkIdx("오른쪽등부위돌출점");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		EgPos q = m_Landmarks[mark_idx1]->m_Pos;
		int result_idx = GetResultIdx("벽면앞으로뻗은주먹수평길이");
		m_Results[result_idx] = p[2] - q[2];
		m_Paths[result_idx].push_back(EgPos(p[0] - 50, p[1], p[2]));
		m_Paths[result_idx].push_back(EgPos(p[0] - 50, p[1], q[2]));
	}

	// 8. 벽면어깨수평길이
	mark_idx = GetLandmarkIdx("오른쪽어깨점");
	mark_idx1 = GetLandmarkIdx("오른쪽등부위돌출점");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		EgPos q = m_Landmarks[mark_idx1]->m_Pos;
		int result_idx = GetResultIdx("벽면어깨수평길이");
		m_Results[result_idx] = p[2] - q[2];
		m_Paths[result_idx].push_back(EgPos(p[0], p[1], p[2]));
		m_Paths[result_idx].push_back(EgPos(p[0], p[1], q[2]));
	}

	printf("/*******************/\n");
	printf("  자세 3 측정 결과 \n");
	printf("/*******************/\n");
	for (int i = 0; i < NUM(ResultPose3); ++i)
		_cprintf("%d. %s: %.2lf (mm)\n", i + 1, ResultPose3[i].c_str(), m_Results[i]);
}

/*!
*	\brief	자세 4의 측정점을 찾는다.
*	\note	마지막 수정일: 2020-07-15
*/
void EgSizer::FindLandmarkPose4()	// 수동랜드마크
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame->m_pLandmarkProgressDlg == NULL)
	{
		pFrame->m_pLandmarkProgressDlg = new CProgressDialog();
		pFrame->m_pLandmarkProgressDlg->Create(IDD_PROGRESS_DIALOG);
		pFrame->m_pLandmarkProgressDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pFrame->m_pLandmarkProgressDlg->ShowWindow(SW_SHOW);
		pFrame->m_pLandmarkProgressDlg->SetFocus();
	}
	int NumMarks = (int)m_Landmarks.size();
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetRange(0, NumMarks);

	// 머리마루점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(1);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("머리마루점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	int idx = GetLandmarkIdx("머리마루점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Head_Top(m_pMesh);

	// 앉은면수준점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(2);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("앉은면수준 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("앉은면수준점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Base_Sit(m_pMesh);

	// 앉은무릎앞점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(3);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("앉은무릎앞점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("앉은무릎앞점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Anterior_Knee_Sit(m_pMesh);

	// 앉은오금점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(4);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("앉은오금점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("앉은오금점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Posterior_Juncture_Calf_Thigh(m_pMesh);

	// (오른쪽)앉은엉덩이사이최대돌출수준점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(5);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("오른쪽앉은엉덩이사이최대돌출수준점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("오른쪽앉은엉덩이사이최대돌출수준점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Max_Hip_Width_Right(m_pMesh);

	// (왼쪽)앉은엉덩이사이최대돌출수준점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(6);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("왼쪽앉은엉덩이사이최대돌출수준점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("왼쪽앉은엉덩이사이최대돌출수준점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Max_Hip_Width_Left(m_pMesh);

	// 손끝점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(7);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("손끝점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("손끝점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Dactylion(m_pMesh);

	// 앉은엉덩이뒤돌출점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(8);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("앉은엉덩이뒤돌출점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("앉은엉덩이뒤돌출점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Buttock_Protrusion(m_pMesh);

	// 손목안쪽점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(9);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("손목안쪽점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("손목안쪽점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Ulnar_Styloid(m_pMesh);

	// 앉은넙다리위점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(10);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("앉은넙다리위점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("앉은넙다리위점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Superior_Thigh_Sit(m_pMesh);

	// 무릎뼈위점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(12);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("무릎뼈위점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("무릎뼈위점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Superior_Patella_Sit(m_pMesh);

	// 팔꿈치가운데점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(13);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("팔꿈치가운데점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("팔꿈치가운데점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Center_Olecranon(m_pMesh);

	// 팔꿈치뒤점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(14);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("팔꿈치뒤점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("팔꿈치뒤점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Rear_Olecranon_Sit(m_pMesh);

	// 팔꿈치아래점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(15);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("팔꿈치아래점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("팔꿈치아래점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Bottom_Olecranon_Sit(m_pMesh);

	// 앉은배돌출점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(11);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("앉은배돌출점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("앉은배돌출점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Abdominal_Protrusion_Sit(m_pMesh);

	// 오른쪽팔꿈치가쪽점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(16);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("오른쪽팔꿈치가쪽점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("오른쪽팔꿈치가쪽점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Lateral_Humeral_Epicondyle_Right(m_pMesh);

	// 왼쪽팔꿈치가쪽점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(17);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("왼쪽팔꿈치가쪽점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("왼쪽팔꿈치가쪽점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Lateral_Humeral_Epicondyle_Left(m_pMesh);

	// 오른쪽어깨가쪽점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(18);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("오른쪽어깨가쪽점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("오른쪽어깨가쪽점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Lateral_Shoulder_Right(m_pMesh);

	// 왼쪽어깨가쪽점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(19);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("왼쪽어깨가쪽점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("왼쪽어깨가쪽점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Lateral_Shoulder_Left(m_pMesh);

	// 오른쪽어깨점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(20);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("오른쪽어깨점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("오른쪽어깨점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Acromion_Right(m_pMesh);

	// 왼쪽어깨점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(21);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("왼쪽어깨점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("왼쪽어깨점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Acromion_Left(m_pMesh);

	// 오른쪽어깨세모근점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(22);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("오른쪽어깨세모근점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("오른쪽어깨세모근점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Deltoid_Right(m_pMesh);

	// 왼쪽어깨세모근점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(23);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("왼쪽어깨세모근점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("왼쪽어깨세모근점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Deltoid_Left(m_pMesh);

	// 목뒤점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(24);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("목뒤점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("목뒤점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Cervicale(m_pMesh);

	// 눈초리점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(25);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("눈초리점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("눈초리점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Ectocanthus(m_pMesh);

	// 상태 진행 창을 초기화 한다.
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(0);
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	pFrame->m_pLandmarkProgressDlg->ShowWindow(FALSE);
}

/*!
*	\brief	자세 4의 측정 항목을 계산한다.
*	\note	마지막 수정일: 2020-09-27
*/
void EgSizer::MeasurePose4()
{
	int mark_idx1, mark_idx2;
	double minX = m_pMesh->m_BndBox.m_MinPt[0];
	double minY = m_pMesh->m_BndBox.m_MinPt[1];
	double minZ = m_pMesh->m_BndBox.m_MinPt[2];
	double maxZ = m_pMesh->m_BndBox.m_MaxPt[2];

	// 1. 앉은키
	mark_idx1 = GetLandmarkIdx("머리마루점");
	mark_idx2 = GetLandmarkIdx("앉은면수준점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("앉은키");
		m_Results[ret_idx] = p[1] - q[1];
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 150.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], q[1], minZ - 150.0));
	}

	// 2. 앉은눈높이
	mark_idx1 = GetLandmarkIdx("눈초리점");
	mark_idx2 = GetLandmarkIdx("앉은면수준점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("앉은눈높이");
		m_Results[ret_idx] = p[1] - q[1];
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 120.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], q[1], minZ - 120.0));
	}

	// 3. 앉은목뒤높이
	mark_idx1 = GetLandmarkIdx("목뒤점");
	mark_idx2 = GetLandmarkIdx("앉은면수준점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("앉은목뒤높이");
		m_Results[ret_idx] = p[1] - q[1];
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 90.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], q[1], minZ - 90.0));
	}

	// 4. 앉은어깨높이
	mark_idx1 = GetLandmarkIdx("오른쪽어깨점");
	mark_idx2 = GetLandmarkIdx("앉은면수준점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("앉은어깨높이");
		m_Results[ret_idx] = p[1] - q[1];
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 60.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], q[1], minZ - 60.0));
	}

	// 5. 앉은팔꿈치높이
	mark_idx1 = GetLandmarkIdx("팔꿈치아래점");
	mark_idx2 = GetLandmarkIdx("앉은면수준점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("앉은팔꿈치높이");
		m_Results[ret_idx] = p[1] - q[1];
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], q[1], minZ - 30.0));
	}

	// 6. 앉은넙다리높이
	mark_idx1 = GetLandmarkIdx("앉은넙다리위점");
	if (m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		int ret_idx = GetResultIdx("앉은넙다리높이");
		m_Results[ret_idx] = p[1] - minY;
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], maxZ + 90.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], minY, maxZ + 90.0));
	}

	// 7. 앉은무릎높이
	mark_idx1 = GetLandmarkIdx("무릎뼈위점");
	if (m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		int ret_idx = GetResultIdx("앉은무릎높이");
		m_Results[ret_idx] = p[1] - minY;
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], maxZ + 60.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], minY, maxZ + 60.0));
	}

	// 8. 앉은오금높이
	mark_idx1 = GetLandmarkIdx("앉은오금점");
	if (m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		int ret_idx = GetResultIdx("앉은오금높이");
		m_Results[ret_idx] = p[1] - minY;
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], maxZ + 30.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], minY, maxZ + 30.0));
	}

	// 9. 어깨사이너비
	mark_idx1 = GetLandmarkIdx("오른쪽어깨점");
	mark_idx2 = GetLandmarkIdx("왼쪽어깨점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("어깨사이너비");
		m_Results[ret_idx] = ABS(q[0] - p[0]);
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1] - 15, minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(q[0], p[1] - 15, minZ - 30.0));
	}

	// 10. 어깨가쪽사이너비
	mark_idx1 = GetLandmarkIdx("오른쪽어깨가쪽점");
	mark_idx2 = GetLandmarkIdx("왼쪽어깨가쪽점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("어깨가쪽사이너비");
		m_Results[ret_idx] = ABS(q[0] - p[0]);
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1] + 15, minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(q[0], p[1] + 15, minZ - 30.0));
	}

	// 11. 위팔사이너비
	mark_idx1 = GetLandmarkIdx("오른쪽어깨세모근점");
	mark_idx2 = GetLandmarkIdx("왼쪽어깨세모근점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("위팔사이너비");
		m_Results[ret_idx] = ABS(q[0] - p[0]);
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(q[0], p[1], minZ - 30.0));
	}

	// 12. 팔꿈치사이너비
	mark_idx1 = GetLandmarkIdx("오른쪽팔꿈치가쪽점");
	mark_idx2 = GetLandmarkIdx("왼쪽팔꿈치가쪽점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("팔꿈치사이너비");
		m_Results[ret_idx] = ABS(q[0] - p[0]);
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(q[0], p[1], minZ - 30.0));
	}

	// 13. 앉은엉덩이너비
	mark_idx1 = GetLandmarkIdx("오른쪽앉은엉덩이사이최대돌출수준점");
	mark_idx2 = GetLandmarkIdx("왼쪽앉은엉덩이사이최대돌출수준점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("앉은엉덩이너비");
		m_Results[ret_idx] = ABS(q[0] - p[0]);
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(q[0], p[1], minZ - 30.0));
	}

	// 14. 앉은넙다리두께
	mark_idx1 = GetLandmarkIdx("앉은넙다리위점");
	mark_idx2 = GetLandmarkIdx("앉은면수준점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("앉은넙다리두께");
		m_Results[ret_idx] = ABS(p[1] - q[1]);
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, q[1], p[2]));
	}

	// 15. 앉은배두께
	mark_idx1 = GetLandmarkIdx("앉은배돌출점");
	if (m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgLine ray(p, EgVec3(0.0, 0.0, -1.0));
		EgPos q(0.0, 0.0, INFINITY);
		for (EgFace *f : m_pMesh->m_pFaces)
		{
			EgPos tmp;
			EgPos p0 = f->GetVertexPos(0);
			EgPos p1 = f->GetVertexPos(1);
			EgPos p2 = f->GetVertexPos(2);
			if (::intersect_line_triangle(p0, p1, p2, ray, tmp, false))
			{
				double d = dist(p, tmp);
				if (d > 0.0001 && tmp[2] < q[2])
					q = tmp;
			}
		}
		int ret_idx = GetResultIdx("앉은배두께");
		m_Results[ret_idx] = ABS(p[2] - q[2]);
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, q[1], q[2]));
	}

	// 16. 앉은엉덩이배두께
	mark_idx1 = GetLandmarkIdx("앉은배돌출점");
	mark_idx2 = GetLandmarkIdx("앉은엉덩이뒤돌출점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		p[1] = q[1];
		int ret_idx = GetResultIdx("앉은엉덩이배두께");
		m_Results[ret_idx] = ABS(p[2] - q[2]);
		m_Paths[ret_idx].push_back(EgPos(minX, q[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, q[1], q[2]));
	}

	// 17. 굽힌팔길이
	mark_idx1 = GetLandmarkIdx("오른쪽어깨점");
	mark_idx2 = GetLandmarkIdx("팔꿈치가운데점");
	int mark_idx3 = GetLandmarkIdx("손목안쪽점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL &&
		m_Landmarks[mark_idx3] != NULL)
	{
		int ret_idx = GetResultIdx("굽힌팔길이");
		EgVertex *p = m_Landmarks[mark_idx1];
		EgVertex *q = m_Landmarks[mark_idx2];
		EgVertex *r = m_Landmarks[mark_idx3];

		std::vector<EgPos> PQ, QR;
		double d1 = SK_Planar_Distance(p, q, PQ, true);
		double d2 = SK_Planar_Distance(q, r, QR, true);
		m_Results[ret_idx] = d1 + d2;
		m_Paths[ret_idx] = PQ;
		for (EgPos pt : QR)
			m_Paths[ret_idx].push_back(pt);
		//EgPlane pln(p->m_Pos, q->m_Pos, r->m_Pos);
		//std::vector<EgPos> Path;
		//m_Results[ret_idx] = SK_Planar_Distance(p, r, &pln, Path, true);
		//m_Paths[ret_idx] = Path;
	}

	// 18. 앉은엉덩이오금수평길이
	mark_idx1 = GetLandmarkIdx("앉은오금점");
	mark_idx2 = GetLandmarkIdx("앉은엉덩이뒤돌출점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		q[1] = p[1];
		int ret_idx = GetResultIdx("앉은엉덩이오금수평길이");
		m_Results[ret_idx] = ABS(p[2] - q[2]);
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], q[2]));
	}

	// 19. 앉은엉덩이무릎(앞)수평길이
	mark_idx1 = GetLandmarkIdx("앉은무릎앞점");
	mark_idx2 = GetLandmarkIdx("앉은엉덩이뒤돌출점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		q[1] = p[1];
		int ret_idx = GetResultIdx("앉은엉덩이무릎(앞)수평길이");
		m_Results[ret_idx] = ABS(p[2] - q[2]);
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], q[2]));
	}

	// 20. (팔굽힌)위팔수직길이
	mark_idx1 = GetLandmarkIdx("오른쪽어깨점");
	mark_idx2 = GetLandmarkIdx("팔꿈치아래점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		q[0] = p[0];
		int ret_idx = GetResultIdx("(팔굽힌)위팔수직길이");
		m_Results[ret_idx] = ABS(p[1] - q[1]);
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(q[0], q[1], minZ - 30.0));
	}

	// 21. (팔굽힌)아래팔수평길이
	mark_idx1 = GetLandmarkIdx("팔꿈치뒤점");
	mark_idx2 = GetLandmarkIdx("손목안쪽점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		q[1] = p[1];
		int ret_idx = GetResultIdx("(팔굽힌)아래팔수평길이");
		m_Results[ret_idx] = ABS(p[2] - q[2]);
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, q[1], q[2]));
	}

	// 22. (팔굽힌)팔꿈치손끝수평길이
	mark_idx1 = GetLandmarkIdx("손끝점");
	mark_idx2 = GetLandmarkIdx("팔꿈치뒤점");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		q[1] = p[1];
		int ret_idx = GetResultIdx("(팔굽힌)팔꿈치손끝수평길이");
		m_Results[ret_idx] = ABS(p[2] - q[2]);
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, q[1], q[2]));
	}

	printf("/*******************/\n");
	printf("  자세 4 측정 결과 \n");
	printf("/*******************/\n");
	for (int i = 0; i < NUM(ResultPose4); ++i)
		printf("%d. %s: %.2lf (mm)\n", i + 1, ResultPose4[i].c_str(), m_Results[i]);
}

/*!
*	\brief	자세5(자세3의 이전 자세)의 측정점을 찾는다.
*	\note	마지막 수정일: 2020-09-05
*/
void EgSizer::FindLandmarkPose5()	// 수동랜드마크
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame->m_pLandmarkProgressDlg == NULL)
	{
		pFrame->m_pLandmarkProgressDlg = new CProgressDialog();
		pFrame->m_pLandmarkProgressDlg->Create(IDD_PROGRESS_DIALOG);
		pFrame->m_pLandmarkProgressDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pFrame->m_pLandmarkProgressDlg->ShowWindow(SW_SHOW);
		pFrame->m_pLandmarkProgressDlg->SetFocus();
	}
	int NumMarks = (int)m_Landmarks.size();
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetRange(0, NumMarks);

	// 1. 엉덩이돌출점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(1);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("엉덩이돌출점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	int idx = GetLandmarkIdx("엉덩이돌출점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Buttock_Protrusion(m_pMesh);

	// 2. 배돌출점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(2);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("배돌출점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("배돌출점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Abdominal_Protrusion(m_pMesh);

	// 3. 엉덩이최대둘레수준점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(3);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("엉덩이최대둘레수준점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("엉덩이최대둘레수준점");
	std::vector<EgPos> Path;
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Buttock_Max_Perimeter(m_pMesh, Path);

	// 4. 주먹가운데점(왼쪽)
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(4);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("왼쪽주먹가운데점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("왼쪽주먹가운데점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Left_Fist_Center(m_pMesh);

	// 4. 주먹가운데점(오른쪽)
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(5);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("오른쪽주먹가운데점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("오른쪽주먹가운데점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Right_Fist_Center(m_pMesh);

	// 5. 왼쪽 등부위돌출점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(6);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("왼쪽등부위돌출점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("왼쪽등부위돌출점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Left_Back_Protrusion(m_pMesh);

	// 6. 팔꿈치아래점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(7);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("오른쪽팔꿈치아래점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("오른쪽팔꿈치아래점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Bottom_Olecranon(m_pMesh);

	// 7. 팔꿈치뒤점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(8);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("오른쪽팔꿈치뒤점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("오른쪽팔꿈치뒤점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Rear_Olecranon(m_pMesh);

	// 8. 왼쪽어깨점
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(9);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("왼쪽어깨점 찾는 중..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("왼쪽어깨점");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Right_Acromion(m_pMesh);

	// 상태 진행 창을 초기화 한다.
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(0);
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	pFrame->m_pLandmarkProgressDlg->ShowWindow(FALSE);
}

/*!
*	\brief	자세 3(구)의 측정 항목을 계산한다.
*	\note	마지막 수정일: 2020-10-22
*/
void EgSizer::MeasurePose5()
{
	int mark_idx, mark_idx1;
	double minY = m_pMesh->m_BndBox.m_MinPt[1];

	// 1. 엉덩이 최대둘레 높이
	mark_idx = GetLandmarkIdx("엉덩이최대둘레수준점");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("엉덩이최대둘레높이");
		m_Results[result_idx] = p[1] - minY;
		m_Paths[result_idx].push_back(p);
		m_Paths[result_idx].push_back(EgPos(p[0], minY, p[2]));
	}

	// 2. 굽힌 팔꿈치 높이
	mark_idx = GetLandmarkIdx("오른쪽팔꿈치아래점");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("굽힌팔꿈치높이");
		m_Results[result_idx] = p[1] - minY;
		m_Paths[result_idx].push_back(p);
		m_Paths[result_idx].push_back(EgPos(p[0], minY, p[2]));
	}

	// 3. 엉덩이 둘레
	mark_idx = GetLandmarkIdx("엉덩이돌출점");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("엉덩이둘레");
		EgPlane Pln(EgVec3(0.0, 1.0, 0.0), p);
		double len = SK_Planar_Perimeter(m_Landmarks[mark_idx], &Pln, m_Paths[result_idx], true);
		m_Results[result_idx] = len;
	}

	// 4. 엉덩이최대둘레
	mark_idx = GetLandmarkIdx("엉덩이최대둘레수준점");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("엉덩이최대둘레");
		EgPlane Pln(EgVec3(0.0, 1.0, 0.0), p);
		double len = SK_Planar_Perimeter(m_Landmarks[mark_idx], &Pln, m_Paths[result_idx], true);
		m_Results[result_idx] = len;
	}

	// 5. 배돌출점기준엉덩이둘레
	mark_idx = GetLandmarkIdx("엉덩이돌출점");
	mark_idx1 = GetLandmarkIdx("배돌출점");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		// 엉덩이 둘레 정점을 복사한다.
		std::vector<EgPos> Pts1 = m_Paths[GetResultIdx("엉덩이둘레")];

		// 배돌출점 둘레를 구한다.
		std::vector<EgPos> Pts2;
		EgPlane Pln(EgVec3(0.0, 1.0, 0.0), m_Landmarks[mark_idx1]->m_Pos);
		double len = SK_Planar_Perimeter(m_Landmarks[mark_idx1], &Pln, Pts2, true);

		// 엉덩이 둘레 정점과 배돌출점 둘레 정점을 합친다.
		for (EgPos pt : Pts2)
		{
			pt[1] = Pts1[0][1];
			Pts1.push_back(pt);
		}

		// 배돌출점기준엉덩이둘레를 구한다.
		int result_idx = GetResultIdx("배돌출점기준엉덩이둘레");
		Pln = EgPlane(EgVec3(0.0, 1.0, 0.0), m_Landmarks[mark_idx]->m_Pos);
		m_Results[result_idx] = SK_ConvexHull(&Pln, Pts1, m_Paths[result_idx]);
	}


	// 6. 팔꿈치주먹수평길이
	mark_idx = GetLandmarkIdx("오른쪽주먹가운데점");
	mark_idx1 = GetLandmarkIdx("오른쪽팔꿈치뒤점");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		EgPos q = m_Landmarks[mark_idx1]->m_Pos;
		int result_idx = GetResultIdx("팔꿈치주먹수평길이");
		m_Results[result_idx] = p[2] - q[2];
		m_Paths[result_idx].push_back(EgPos(p[0] - 50, p[1], p[2]));
		m_Paths[result_idx].push_back(EgPos(p[0] - 50, p[1], q[2]));
	}

	// 7. 벽면앞으로뻗은주먹수평길이
	mark_idx = GetLandmarkIdx("왼쪽주먹가운데점");
	mark_idx1 = GetLandmarkIdx("왼쪽등부위돌출점");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		EgPos q = m_Landmarks[mark_idx1]->m_Pos;
		int result_idx = GetResultIdx("벽면앞으로뻗은주먹수평길이");
		m_Results[result_idx] = p[2] - q[2];
		m_Paths[result_idx].push_back(EgPos(p[0] + 50, p[1], p[2]));
		m_Paths[result_idx].push_back(EgPos(p[0] + 50, p[1], q[2]));
	}

	// 8. 벽면어깨수평길이
	mark_idx = GetLandmarkIdx("왼쪽어깨점");
	mark_idx1 = GetLandmarkIdx("왼쪽등부위돌출점");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		EgPos q = m_Landmarks[mark_idx1]->m_Pos;
		int result_idx = GetResultIdx("벽면어깨수평길이");
		m_Results[result_idx] = p[2] - q[2];
		m_Paths[result_idx].push_back(EgPos(p[0], p[1], p[2]));
		m_Paths[result_idx].push_back(EgPos(p[0], p[1], q[2]));
	}

	printf("/*******************/\n");
	printf("  자세 3(구) 측정 결과 \n");
	printf("/*******************/\n");
	for (int i = 0; i < NUM(ResultPose5); ++i)
		_cprintf("%d. %s: %.1lf (mm)\n", i + 1, ResultPose5[i].c_str(), m_Results[i]);
}

// mj::
void EgSizer::SetGender() {
	m_bWoman = !m_bWoman;
}

/*!
*	\brief	해당 자세의 측정 항목의 정보를 렌더링한다.
*	\note	마지막 수정일: 2020-07-15
*/
void EgSizer::Render()
{
	// 바닥면을 렌더링 한다.
	//if (m_pMesh != NULL)
	//{
	//	EgPos p = m_pMesh->m_BndBox.m_MinPt;
	//	EgPos q = m_pMesh->m_BndBox.m_MaxPt;
	//	double w = m_pMesh->m_BndBox.GetWidth();
	//	double h = m_pMesh->m_BndBox.GetHeight();
	//	double d = m_pMesh->m_BndBox.GetDepth();
	//	{
	//		//glColor3d(0.0, 1.0, 1.0);
	//		static EgMaterial *pMtl = ::create_material("", 0.1f, 0.1f, 0.1f, 0.2f, 0.5f, 0.7f, 0.7f, 0.7f, 0.7f, 32.0f);
	//		pMtl->Enable();
	//		glBegin(GL_POLYGON);
	//		{
	//			glNormal3d(0.0, 1.0, 0.0);
	//			glVertex3d(p[0] - w, p[1] - 10, p[2] - d);
	//			glVertex3d(p[0] - w, p[1] - 10, q[2] + d);
	//			glVertex3d(q[0] + w, p[1] - 10, q[2] + d);
	//			glVertex3d(q[0] + w, p[1] - 10, p[2] - d);
	//		}			
	//		glEnd();
	//	}
	//}

	// 렌드 마크를 렌더링 한다.
	EgMaterial Mtl0("Blue"), Mtl1("Red");
	Mtl0.SetDiffuse(0.0, 0.0, 1.0);
	Mtl1.SetDiffuse(1.0, 0.0, 0.0);
	if (m_bShowLandMark)
	{
		for (int i = 0; i < NUM(m_Landmarks); ++i)
		{
			if (m_Landmarks[i] == NULL)
				continue;
			glPushMatrix();
			{
				EgPos p = m_Landmarks[i]->m_Pos;
				glTranslated(p[0], p[1], p[2]);

				if (i != m_SltLandmarkIdx)
					Mtl0.Enable();
				else
					Mtl1.Enable();
				gluSphere(gluNewQuadric(), 4.0f, 10.0f, 10.f);
			}
			glPopMatrix();
		}
	}	

	// 경로를 렌더링 한다.
	for (int i = 0; i < NUM(m_Paths); ++i)
	{
		if (m_bShowPaths[i] == false)
			continue;

		std::vector<EgPos> Path = m_Paths[i];

		if ((int)Path.size() == 2)
		{
			RenderArrow(Path[0], Path[1]);
		}
		else
		{
			glDisable(GL_LIGHTING);
			glColor3d(1.0, 1.0, 0.0);
			glLineWidth(2.0);
			glBegin(GL_LINE_STRIP);
			for (EgPos &p : Path)
				glVertex3dv(p.m_Coords);
			glEnd();
			glLineWidth(1.0);
			glEnable(GL_LIGHTING);
		}
	}
}

/*!
*	\brief	양 끝점에 화살표를 갖는 두 점을 잇는 직선을 렌더링 한다.
*	\note	마지막 수정일: 2020-09-27
*/
void EgSizer::RenderArrow(EgPos p, EgPos q)
{
	EgVec3 y_axis = (q - p).Normalize();
	EgQuat qt;
	qt.SetFromArcBall(EgVec3(0.0, 1.0, 0.0), y_axis);
	glPushMatrix();
	{
		glTranslated(q[0], q[1], q[2]);
		glMultMatrixd(cast_arr(cast_mat(qt, true)));
		m_pArrowMesh->Render();
	}
	glPopMatrix();

	qt.SetFromArcBall(EgVec3(0.0, 1.0, 0.0), -y_axis);
	glPushMatrix();
	{
		glTranslated(p[0], p[1], p[2]);
		glMultMatrixd(cast_arr(cast_mat(qt, true)));
		m_pArrowMesh->Render();
	}
	glPopMatrix();

	p = p + y_axis * 15.0;
	q = q - y_axis * 15.0;
	glDisable(GL_LIGHTING);
	glLineWidth(2.0f);
	//glColor3f(1.0f, 1.0f, 0.0f);

	if (p[0] != q[0])
		glColor3d(1.0, 0.0, 0.0);
	if (p[1] != q[1])
		glColor3d(0.0, 1.0, 0.0);
	if (p[2] != q[2])
		glColor3d(0.0, 0.0, 1.0);

	glBegin(GL_LINES);
	{
		glVertex3dv(p.m_Coords);
		glVertex3dv(q.m_Coords);
	}
	glEnd();
	glLineWidth(1.0f);
	glEnable(GL_LIGHTING);
}

/*!
*	\brief	랜드마크의 인덱스를 반환한다.
*	\note	마지막 수정일: 2020-07-15
*
*	\param	Name[in]	랜드마크 이름
*
*	\return	해당 랜드마크의 인덱스를 반환한다.
*/
int EgSizer::GetLandmarkIdx(std::string Name)
{
	switch (m_PoseId)
	{
	case 1:
		for (int i = 0; i < NUM(LandmarkPose1); ++i)
			if (Name == LandmarkPose1[i])
				return i;
		break;

	case 2:
		for (int i = 0; i < NUM(LandmarkPose2); ++i)
			if (Name == LandmarkPose2[i])
				return i;
		break;

	case 3:
		for (int i = 0; i < NUM(LandmarkPose3); ++i)
			if (Name == LandmarkPose3[i])
				return i;
		break;

	case 4:
		for (int i = 0; i < NUM(LandmarkPose4); ++i)
			if (Name == LandmarkPose4[i])
				return i;
		break;

	case 5:
		for (int i = 0; i < NUM(LandmarkPose5); ++i)
			if (Name == LandmarkPose5[i])
				return i;
		break;
	}
	return -1;
}

/*!
*	\brief	측정 항목의 인덱스를 반환한다.
*	\note	마지막 수정일: 2020-07-15
*
*	\param	Name[in]	측정 항목의 이름
*
*	\return	해당 항목의 인덱스를 반환한다.
*/
int EgSizer::GetResultIdx(std::string Name)
{
	switch (m_PoseId)
	{
	case 1:
		for (int i = 0; i < NUM(ResultPose1); ++i)
			if (Name == ResultPose1[i])
				return i;
		return -1;

	case 2:
		for (int i = 0; i < NUM(ResultPose2); ++i)
			if (Name == ResultPose2[i])
				return i;
		return -1;

	case 3:
		for (int i = 0; i < NUM(ResultPose3); ++i)
			if (Name == ResultPose3[i])
				return i;
		return -1;

	case 4:
		for (int i = 0; i < NUM(ResultPose4); ++i)
			if (Name == ResultPose4[i])
				return i;
		return -1;

	case 5:
		for (int i = 0; i < NUM(ResultPose5); ++i)
			if (Name == ResultPose5[i])
				return i;
		return -1;
	}
	return -1;
}

/*!
*	\brief	현재 선택된 랜드마크의 좌표를 반환한다.
*	\note	마지막 수정일: 2020-09-24
*
*	\return	선택된 랜드마크의 좌표를 반환한다.
*/
EgPos EgSizer::GetSltLandmarkPos()
{
	if (m_Landmarks[m_SltLandmarkIdx] == NULL)
		return EgPos();
	else
		return m_Landmarks[m_SltLandmarkIdx]->m_Pos;
}

/*!
*	\brief	초기화 한다.
*	\note	마지막 수정일: 2020-07-20
*/
void EgSizer::Reset()
{
	m_PoseId = 0;
	m_pMesh = NULL;
	m_Landmarks.clear();
	m_Results.clear();
	m_Paths.clear();
}

