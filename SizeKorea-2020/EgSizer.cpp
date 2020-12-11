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


// mj :: �ڼ�1, �ڼ�2 ���� ������ ���� ���� ���� (20. 12. 8)
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
/* EgSizer Ŭ���� ���� */
/*********************/
/*!
*	\brief	������
*	\note	������ ������: 2020-09-27
*/
EgSizer::EgSizer()
{
	m_SltLandmarkIdx = -1;
	m_pMesh = NULL;
	m_pArrowMesh = new EgMesh("Arrow Mesh");
	m_bWoman = true;
	m_bShowLandMark = true;

	// ������ �����Ͽ� �޽��� �߰��Ѵ�.
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

	// �ﰢ���� �����Ͽ� �޽��� �߰��Ѵ�.
	for (int i = 0; i < snum - 1; ++i)
	{
		EgVertex *v1 = m_pArrowMesh->m_pVerts[i];
		EgVertex *v2 = m_pArrowMesh->m_pVerts[(i + 1) % (snum - 1)];
		EgFace *f0 = new EgFace(pApex, v1, v2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "");
		EgFace *f1 = new EgFace(pBase, v2, v1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "");
		m_pArrowMesh->AddFace(f0);
		m_pArrowMesh->AddFace(f1);
	}

	// �޽��� ��������, ������, ������ ����Ѵ�.
	m_pArrowMesh->InitEdgeMate();
	m_pArrowMesh->UpdateBndBox();
	m_pArrowMesh->UpdateNormal(FLAT);

	// �޽��� ������ ������ �Ҵ��Ѵ�.
	EgMaterial *pMtl = ::create_material("arrow", 0.1f, 0.1f, 0.1f, 0.8f, 0.0f, 0.0f, 0.7f, 0.7f, 0.7f, 16.0f);
	m_pArrowMesh->AddMaterial(pMtl);
	m_pArrowMesh->SetMaterial(pMtl);
}


/*!
*	\brief	�Ҹ���
*	\note	������ ������: 2020-07-15
*/
EgSizer::~EgSizer()
{
	if (m_pArrowMesh != NULL)
		delete m_pArrowMesh;
}

/*!
*	\brief	��ĵ �ڼ��� �޽��� �����Ѵ�.
*	\note	������ ������: 2020-09-05
*
*	\param	PoseId[in]	��ĵ �޽��� �ڼ�
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
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
		m_bUserPick.assign(NumLandmarks, false);	// �������帶ũ

		int NumResults = (int)ResultPose3.size();
		m_Results.assign(NumResults, 0.0);
		m_Paths.assign(NumResults, std::vector<EgPos>());
		m_bShowPaths.assign(NumResults, true);
	}

	if (PoseId == 4)
	{
		int NumLandmarks = (int)LandmarkPose4.size();
		m_Landmarks.assign(NumLandmarks, NULL);
		m_bUserPick.assign(NumLandmarks, false);	// �������帶ũ


		int NumResults = (int)ResultPose4.size();
		m_Results.assign(NumResults, 0.0);
		m_Paths.assign(NumResults, std::vector<EgPos>());
		m_bShowPaths.assign(NumResults, true);
	}
}

/*!
*	\brief	�������� ã�´�.
*	\note	������ ������: 2020-09-05
*/
void EgSizer::FindLandmarks()
{
	// �ڼ��� ���� �������� ã�´�.
	switch (m_PoseId)
	{
	case 1:
		FindLandmarkPose1();
		break;

	case 2:
		// m_nLandmarkPose2�� EgSizer�� �Ҵ��Ѵ�
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

	// �����̼��� �ʱ�ȭ �Ѵ�.
	//theSlicer.Reset();
}

/*!
*	\brief	���� ���õ� Ư¡���� ��ġ�� �����Ѵ�.
*	\note	������ ������: 2020-08-28
*
*	\param	pos[in]		Ư¡���� ���ο� ��ġ
*/
void EgSizer::UpdateLandmark(EgPos pos)
{
	if (m_SltLandmarkIdx == -1)
		return;

	EgVertex *v = FindClosestVert(m_pMesh, pos);
	m_Landmarks[m_SltLandmarkIdx] = v;
	m_bUserPick[m_SltLandmarkIdx] = true;	// �������帶ũ


	// mj :: �ڵ� ���� ����
	m_bFindLandmark[m_SltLandmarkIdx] = false;

	// mj :: �ڼ�1�� ���� �������� �����ϰ� Find Landmark�� ��찡 ���ܼ� Global varibale ������Ʈ �ʿ� (20. 12. 8)
	if (m_PoseId == 1) {
		m_LandmarkPose1[m_SltLandmarkIdx] = v;
	}
	// mj :: �ڼ�2�� EgSizer�� m_Landmarks ���� Doc���� ó���ϱ� ������ �ʿ� (20. 12. 8)
	else if (m_PoseId == 2) {
		m_nLandmarkPose2[m_SltLandmarkIdx] = v;
	}
}

/*!
*	\brief	�ش� ���帶ũ ������ �����͸� ��ȯ�Ѵ�.
*	\note	������ ������: 2020-07-20
*
*	\param	Name[in]	���帶ũ�� �̸�
*
*	\return	�ش� ���帶ũ ������ �����͸� ��ȯ�Ѵ�.
*/
EgVertex *EgSizer::GetLandmark(std::string Name)
{
	int idx = GetLandmarkIdx(Name);
	if (idx == -1)
		return NULL;
	return m_Landmarks[idx];
}

/*!
*	\brief	�ش� �ڼ��� ���� �׸��� �����Ѵ�.
*	\note	������ ������: 2020-08-28
*/
void EgSizer::Measure()
{
	// ����� �ʱ�ȭ�Ѵ�.
	int NumResults = (int)m_Results.size();
	m_Results.assign(NumResults, 0.0);
	m_Paths.assign(NumResults, std::vector<EgPos>());
	m_bShowPaths.assign(NumResults, true);

	// �ڼ��� ���� �������� ã�´�.
	switch (m_PoseId)
	{
	case 1:
		MeasurePose1();
		break;

	case 2:
		// m_nResultPose2�� EgSizer�� �Ҵ��Ѵ�
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
	///////////////////////////////////////  ������::�Ӹ������� 
	Pose1_FindTopHeadPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(1);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�Ӹ������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	///////////////////////////////////////  Make sagittal/coronal plane
	Pose1_MakeSilhouette(m_pMesh);


	/////////////////////////////////////// ������::�ܵ����
	/////////////////////////////////////// ������::�ܵ����������(R/L)
	/////////////////////////////////////// ������::�ܵ����������(R/L)
	/////////////////////////////////////// ������::�ܵ������
	/////////////////////////////////////// ������::�ܵ������
	Pose1_FindArmpitPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(2);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�ܵ����, �ܵ����/����, �ܵ����/�������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	/////////////////////////////////////// ������::�����(R/L)
	/////////////////////////////////////// ������::���������(R/L)
	Pose1_FindShoulderPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(3);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����, ��������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	

	/////////////////////////////////////// ������::���� 
	Pose1_FindSideNeckPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(4);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	///////////////////////////////////////  ������::�����
	Pose1_FindFneckPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(5);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("����� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::�ڳ���
	Pose1_FindNosePt(m_pMesh);


	/////////////////////////////////////// ������::�γ���
	Pose1_FindJawPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(6);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�γ��� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::�����̵�����
	/////////////////////////////////////// ������::�����������
	Pose1_FindHipnCalfExtrudePt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(7);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����̵�����, ����������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	/////////////////////////////////////// ������::��������
	Pose1_FindUnderHipPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(8);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	/////////////////////////////////////// ������::�����(��/��)
	Pose1_FindNavelPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(9);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("����� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	/////////////////////////////////////// ������::�赹����
	Pose1_FindStomachExtrudePt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(10);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�赹���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::�����������
	/////////////////////////////////////// ������::����������
	/////////////////////////////////////// ������::������
	Pose1_FindKneePt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(11);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����������, ����������, ������ ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::����
	Pose1_FindCrotchPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(12);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::�����
	Pose1_FindBackNeckPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(13);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("����� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::�㸮(��)��
	/////////////////////////////////////// ������::�㸮����
	/////////////////////////////////////// ������::�㸮����
	Pose1_FindWaistPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(15);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�㸮����, �㸮��/���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::���ʺ�����
	Pose1_FindAnklePt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(16);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���ʺ����� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::��������(R/L)
	Pose1_FindBustPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(18);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::�ܵ������ ����
	Pose1_ModifyArmpitWithShoudler(m_pMesh);


	/////////////////////////////////////// ������::�ܵ���պ���(R/L)
	/////////////////////////////////////// ������::�ܵ���ں���(R/L)
	Pose1_FindArmpitWall(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(22);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�ܵ����/�ں��� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::�������Ʒ���
	Pose1_FindUnderBustPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(24);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�������Ʒ��� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::�ճ���
	Pose1_FindHandPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(25);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�ճ��� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	/////////////////////////////////////// ������::�ո������
	Pose1_FindWristPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(26);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�ո������ ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::�������
	Pose1_FindElbowPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(28);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::���ʸ���
	Pose1_FindEyePt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(30);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���ʸ��� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::�̵帮����
	Pose1_FindMidRiffPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(31);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�̵帮���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::Top hip��
	Pose1_FindTopHipPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(32);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("Top-hip�� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);


	/////////////////////////////////////// ������::Upper hip��
	Pose1_FindUpperHipPt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(33);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("Upper-hip�� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);

	/////////////////////////////////////// ������::�����
	Pose1_FindBackExtrudePt(m_pMesh);
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(LANDMARK_NUM_POSE1);
	pFrame->m_pLandmarkProgressDlg->ShowWindow(FALSE);

	for (int i = 0; i < LANDMARK_NUM_POSE1; i++) {
		m_Landmarks[i] = m_LandmarkPose1[i];
	}
}

void EgSizer::MeasurePose1() {
	_cprintf("\n\n\n====================== MEASURES ======================\n");


	///////////////////////////////////////  Ű 1
	EgPos TopHeadPt = m_Landmarks[POSE1_LANDMARK_TOPHEAD]->m_Pos,
		FloorPt = TopHeadPt;

	TopHeadPt.m_Coords[0] += 500.0f;
	FloorPt.m_Coords[0] += 500.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_HEIGHT].push_back(TopHeadPt);
	m_Paths[POSE1_MEASURE_HEIGHT].push_back(FloorPt);

	m_Results[POSE1_MEASURE_HEIGHT] = TopHeadPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Height : %f\n", m_Results[POSE1_MEASURE_HEIGHT]);


	///////////////////////////////////////  ������ 2
	EgPos EyePt = m_Landmarks[POSE1_LANDMARK_EYE]->m_Pos;
	FloorPt = EyePt;

	EyePt.m_Coords[0] += 400.0f;
	FloorPt.m_Coords[0] += 400.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_EYE_LEVEL].push_back(EyePt);
	m_Paths[POSE1_MEASURE_EYE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_EYE_LEVEL] = EyePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Eye level : %f\n", m_Results[POSE1_MEASURE_EYE_LEVEL]);


	///////////////////////////////////////  �γ����� 3
	EgPos JawPt = m_Landmarks[POSE1_LANDMARK_JAW]->m_Pos;
	FloorPt = JawPt;

	JawPt.m_Coords[0] += 300.0f;
	FloorPt.m_Coords[0] += 300.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_JAW_LEVEL].push_back(JawPt);
	m_Paths[POSE1_MEASURE_JAW_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_JAW_LEVEL] = JawPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Jaw level : %f\n", m_Results[POSE1_MEASURE_JAW_LEVEL]);


	///////////////////////////////////////  ��ڳ��� 4
	EgPos BneckPt = m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos;
	FloorPt = BneckPt;

	BneckPt.m_Coords[2] -= 300.0f;
	FloorPt.m_Coords[2] -= 300.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_NECK_BACK_LEVEL].push_back(BneckPt);
	m_Paths[POSE1_MEASURE_NECK_BACK_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_NECK_BACK_LEVEL] = BneckPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Neck back level : %f\n", m_Results[POSE1_MEASURE_NECK_BACK_LEVEL]);


	///////////////////////////////////////  �񿷳��� 5
	EgPos NeckSidePt = m_Landmarks[POSE1_LANDMARK_NECK_SIDE]->m_Pos;
	FloorPt = NeckSidePt;

	NeckSidePt.m_Coords[0] += 200.0f;
	FloorPt.m_Coords[0] += 200.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_NECK_SIDE_LEVEL].push_back(NeckSidePt);
	m_Paths[POSE1_MEASURE_NECK_SIDE_LEVEL].push_back(FloorPt);
	m_Results[POSE1_MEASURE_NECK_SIDE_LEVEL] = NeckSidePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Neck side level : %f\n", m_Results[POSE1_MEASURE_NECK_SIDE_LEVEL]);


	///////////////////////////////////////  ��ճ��� 6
	EgPos NeckFrontPt = m_Landmarks[POSE1_LANDMARK_NECK_FRONT]->m_Pos;
	FloorPt = NeckFrontPt;

	NeckFrontPt.m_Coords[2] += 300.0f;
	FloorPt.m_Coords[2] += 300.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_NECK_FRONT_LEVEL].push_back(NeckFrontPt);
	m_Paths[POSE1_MEASURE_NECK_FRONT_LEVEL].push_back(FloorPt);
	m_Results[POSE1_MEASURE_NECK_FRONT_LEVEL] = NeckFrontPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Neck front level : %f\n", m_Results[POSE1_MEASURE_NECK_FRONT_LEVEL]);


	///////////////////////////////////////  ������� 7
	EgPos ShoulderPt = m_Landmarks[POSE1_LANDMARK_SHOULDER_R]->m_Pos;
	FloorPt = ShoulderPt;

	ShoulderPt.m_Coords[0] += 100.0f;
	FloorPt.m_Coords[0] += 100.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_SHOULDER_LEVEL].push_back(ShoulderPt);
	m_Paths[POSE1_MEASURE_SHOULDER_LEVEL].push_back(FloorPt);
	m_Results[POSE1_MEASURE_SHOULDER_LEVEL] = NeckFrontPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Shoulder level : %f\n", m_Results[POSE1_MEASURE_SHOULDER_LEVEL]);



	///////////////////////////////////////  ������ʳ��� 8
	EgPos ShoulderSidePt = m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_R]->m_Pos;

	ShoulderSidePt.m_Coords[0] += 200.0f;
	FloorPt.m_Coords[0] += 200.0f;
	FloorPt = ShoulderSidePt;

	FloorPt.m_Coords[1] = 0.0f;
	m_Paths[POSE1_MEASURE_SHOULDERSIDE_LEVEL].push_back(ShoulderSidePt);
	m_Paths[POSE1_MEASURE_SHOULDERSIDE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_SHOULDERSIDE_LEVEL] = ShoulderSidePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Shoulder side level : %f\n", m_Results[POSE1_MEASURE_SHOULDERSIDE_LEVEL]);


	///////////////////////////////////////  �ܵ������ 9
	EgPos ArmpitPt = m_Landmarks[POSE1_LANDMARK_ARMPIT]->m_Pos;
	FloorPt = ArmpitPt;

	ArmpitPt.m_Coords[0] += 300.0f;
	FloorPt.m_Coords[0] += 300.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_ARMPIT_LEVEL].push_back(ArmpitPt);
	m_Paths[POSE1_MEASURE_ARMPIT_LEVEL].push_back(FloorPt);
	m_Results[POSE1_MEASURE_ARMPIT_LEVEL] = ArmpitPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Armpit level : %f\n", m_Results[POSE1_MEASURE_ARMPIT_LEVEL]);


	///////////////////////////////////////  ���������� 10
	EgPos BustPt = m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos;
	FloorPt = BustPt;
	BustPt.m_Coords[2] += 200.0f;
	FloorPt.m_Coords[2] += 200.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_BP_LEVEL].push_back(BustPt);
	m_Paths[POSE1_MEASURE_BP_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_BP_LEVEL] = BustPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Bust level : %f\n", m_Results[POSE1_MEASURE_BP_LEVEL]);


	///////////////////////////////////////  �������Ʒ����� 11
	EgPos UnderBustPt = m_Landmarks[POSE1_LANDMARK_UNDERBUST]->m_Pos;
	FloorPt = UnderBustPt;

	UnderBustPt.m_Coords[2] += 400.0f;
	FloorPt.m_Coords[2] += 400.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_UNDERBUST_LEVEL].push_back(UnderBustPt);
	m_Paths[POSE1_MEASURE_UNDERBUST_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_UNDERBUST_LEVEL] = UnderBustPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Under Bust level : %f\n", m_Results[POSE1_MEASURE_UNDERBUST_LEVEL]);


	///////////////////////////////////////  �̵帮������ 12
	EgPos MidRiffPt = m_Landmarks[POSE1_LANDMARK_MIDRIFF]->m_Pos;
	FloorPt = MidRiffPt;

	MidRiffPt.m_Coords[0] += 450.0f;
	FloorPt.m_Coords[0] += 450.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_MIDRIFF_LEVEL].push_back(MidRiffPt);
	m_Paths[POSE1_MEASURE_MIDRIFF_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_MIDRIFF_LEVEL] = MidRiffPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Mid Riff level : %f\n", m_Results[POSE1_MEASURE_MIDRIFF_LEVEL]);


	///////////////////////////////////////  �㸮���� 13
	EgPos WaistPt = m_Landmarks[POSE1_LANDMARK_WAIST]->m_Pos;
	FloorPt = WaistPt;

	WaistPt.m_Coords[0] += 350.0f;
	FloorPt.m_Coords[0] += +350.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_WAIST_LEVEL].push_back(WaistPt);
	m_Paths[POSE1_MEASURE_WAIST_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_WAIST_LEVEL] = WaistPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Waist level : %f\n", m_Results[POSE1_MEASURE_WAIST_LEVEL]);


	///////////////////////////////////////  ��ż����㸮���� 14
	EgPos NavelPt = m_Landmarks[POSE1_LANDMARK_NAVEL]->m_Pos;
	FloorPt = NavelPt;

	NavelPt.m_Coords[0] += 250.0f;
	FloorPt.m_Coords[0] += 250.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_NAVEL_LEVEL].push_back(NavelPt);
	m_Paths[POSE1_MEASURE_NAVEL_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_NAVEL_LEVEL] = NavelPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Navel level : %f\n", m_Results[POSE1_MEASURE_NAVEL_LEVEL]);


	///////////////////////////////////////  ����� 15
	EgPos StomachExtrudePt = m_Landmarks[POSE1_LANDMARK_STOMACH_EXTRUDE]->m_Pos;
	FloorPt = StomachExtrudePt;

	StomachExtrudePt.m_Coords[2] += 250.0f;
	FloorPt.m_Coords[2] += 250.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_STOMACH_EXTRUDE_LEVEL].push_back(StomachExtrudePt);
	m_Paths[POSE1_MEASURE_STOMACH_EXTRUDE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_STOMACH_EXTRUDE_LEVEL] = StomachExtrudePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("StomachE Extrude level : %f\n", m_Results[POSE1_MEASURE_STOMACH_EXTRUDE_LEVEL]);


	///////////////////////////////////////  Upper-hip���� 16
	EgPos UpperHipPt = m_Landmarks[POSE1_LANDMARK_UPPERHIP]->m_Pos;
	FloorPt = UpperHipPt;

	UpperHipPt.m_Coords[0] -= 150.0f;
	FloorPt.m_Coords[0] -= 150.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_UPPERHIP_LEVEL].push_back(UpperHipPt);
	m_Paths[POSE1_MEASURE_UPPERHIP_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_UPPERHIP_LEVEL] = UpperHipPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Upper Hip level : %f\n", m_Results[POSE1_MEASURE_UPPERHIP_LEVEL]);


	///////////////////////////////////////  Top-hip���� 17
	EgPos TopHipPt = m_Landmarks[POSE1_LANDMARK_TOPHIP]->m_Pos;
	FloorPt = TopHipPt;

	TopHipPt.m_Coords[2] = m_MinZ - 100.0f;
	FloorPt.m_Coords[2] = m_MinZ - 100.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_TOPHIP_LEVEL].push_back(TopHipPt);
	m_Paths[POSE1_MEASURE_TOPHIP_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_TOPHIP_LEVEL] = TopHipPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Top Hip level : %f\n", m_Results[POSE1_MEASURE_TOPHIP_LEVEL]);


	///////////////////////////////////////  �����̳��� 18
	EgPos HipExtrudePt = m_Landmarks[POSE1_LANDMARK_HIP_EXTRUDE]->m_Pos;
	FloorPt = HipExtrudePt;

	HipExtrudePt.m_Coords[2] = m_MinZ - 150.0f;
	FloorPt.m_Coords[2] = m_MinZ - 150.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_HIP_EXTRUDE_LEVEL].push_back(HipExtrudePt);
	m_Paths[POSE1_MEASURE_HIP_EXTRUDE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_HIP_EXTRUDE_LEVEL] = HipExtrudePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Hip Extrude level : %f\n", m_Results[POSE1_MEASURE_HIP_EXTRUDE_LEVEL]);


	///////////////////////////////////////  ������ 19
	EgPos CrotchPt = m_Landmarks[POSE1_LANDMARK_CROTCH]->m_Pos;
	FloorPt = CrotchPt;

	CrotchPt.m_Coords[2] = m_MaxZ + 50.0f;
	FloorPt.m_Coords[2] = m_MaxZ + 50.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_CROTCH_LEVEL].push_back(CrotchPt);
	m_Paths[POSE1_MEASURE_CROTCH_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_CROTCH_LEVEL] = CrotchPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Crotch level : %f\n", m_Results[POSE1_MEASURE_CROTCH_LEVEL]);


	///////////////////////////////////////  ���������� 20
	EgPos UnderHipPt = m_Landmarks[POSE1_LANDMARK_UNDERHIP]->m_Pos;
	FloorPt = UnderHipPt;

	UnderHipPt.m_Coords[2] = m_MinZ - 200.0f;
	FloorPt.m_Coords[2] = m_MinZ - 200.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_UNDERHIP_LEVEL].push_back(UnderHipPt);
	m_Paths[POSE1_MEASURE_UNDERHIP_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_UNDERHIP_LEVEL] = UnderHipPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Under Hip level : %f\n", m_Results[POSE1_MEASURE_UNDERHIP_LEVEL]);


	///////////////////////////////////////  ������������� 21
	EgPos KneeMidPt = m_Landmarks[POSE1_LANDMARK_KNEE_MID]->m_Pos;
	FloorPt = KneeMidPt;

	KneeMidPt.m_Coords[2] = m_MaxZ + 100.0f;
	FloorPt.m_Coords[2] = m_MaxZ + 100.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_KNEE_MID_LEVEL].push_back(KneeMidPt);
	m_Paths[POSE1_MEASURE_KNEE_MID_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_KNEE_MID_LEVEL] = KneeMidPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Knee Mid level : %f\n", m_Results[POSE1_MEASURE_KNEE_MID_LEVEL]);


	///////////////////////////////////////  �������� 22
	EgPos KneeSidePt = m_Landmarks[POSE1_LANDMARK_KNEE_SIDE]->m_Pos;
	FloorPt = KneeSidePt;

	KneeSidePt.m_Coords[2] = m_MaxZ + 150.0f;
	FloorPt.m_Coords[2] = m_MaxZ + 150.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_KNEE_LEVEL].push_back(KneeSidePt);
	m_Paths[POSE1_MEASURE_KNEE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_KNEE_LEVEL] = KneeSidePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Knee level : %f\n", m_Results[POSE1_MEASURE_KNEE_LEVEL]);


	///////////////////////////////////////  ��������� 23
	EgPos CalfExtrudePt = m_Landmarks[POSE1_LANDMARK_CALF_EXTRUDE]->m_Pos;
	FloorPt = CalfExtrudePt;

	CalfExtrudePt.m_Coords[2] = m_MaxZ + 200.0f;
	FloorPt.m_Coords[2] = m_MaxZ + 200.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_CALF_EXTRUDE_LEVEL].push_back(CalfExtrudePt);
	m_Paths[POSE1_MEASURE_CALF_EXTRUDE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_CALF_EXTRUDE_LEVEL] = CalfExtrudePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Calf Extrude level : %f\n", m_Results[POSE1_MEASURE_CALF_EXTRUDE_LEVEL]);


	///////////////////////////////////////  ���ʺ������ 24
	EgPos AnklePt = m_Landmarks[POSE1_LANDMARK_ANKLE]->m_Pos;
	FloorPt = AnklePt;

	AnklePt.m_Coords[0] = m_MinX - 50.0f;
	FloorPt.m_Coords[0] = m_MinX - 50.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_ANKLE_LEVEL].push_back(AnklePt);
	m_Paths[POSE1_MEASURE_ANKLE_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_ANKLE_LEVEL] = AnklePt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Ankle level : %f\n", m_Results[POSE1_MEASURE_ANKLE_LEVEL]);


	///////////////////////////////////////  �ճ����� 25
	EgPos HandPt = m_Landmarks[POSE1_LANDMARK_HAND]->m_Pos;
	FloorPt = HandPt;

	HandPt.m_Coords[0] = m_MinX - 100.0f;
	FloorPt.m_Coords[0] = m_MinX - 100.0f;
	FloorPt.m_Coords[1] = 0.0f;

	m_Paths[POSE1_MEASURE_HAND_LEVEL].push_back(HandPt);
	m_Paths[POSE1_MEASURE_HAND_LEVEL].push_back(FloorPt);

	m_Results[POSE1_MEASURE_HAND_LEVEL] = HandPt.m_Coords[1] - FloorPt.m_Coords[1];
	_cprintf("Hand level : %f\n", m_Results[POSE1_MEASURE_HAND_LEVEL]);


	//////////////////////////////////////////////////////////////////////////// ���� ����
	///////////////////////////////////////  ������� 26
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


	///////////////////////////////////////  ������ʱ��� 27

	m_Results[POSE1_MEASURE_SHOULDERSIDE_LENGTH] = SK_Shortest_Distance(
		m_Landmarks[POSE1_LANDMARK_NECK_SIDE],
		m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_R],
		m_Paths[POSE1_MEASURE_SHOULDERSIDE_LENGTH]
	);

	_cprintf("Shoulder Side Length : %f\n", m_Results[POSE1_MEASURE_SHOULDERSIDE_LENGTH]);


	///////////////////////////////////////  ������̱��� 28
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


	///////////////////////////////////////  ������ʻ��̱��� 29
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


	/////////////////////////////////////// ��ھ������ 30
	m_Results[POSE1_MEASURE_BNECK_SHOULDER_LENGTH] =
		SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_BACK],
			m_Landmarks[POSE1_LANDMARK_SHOULDER_R],
			m_Paths[POSE1_MEASURE_BNECK_SHOULDER_LENGTH]
		);
	_cprintf("Bneck to Shoulder Length : %f\n", m_Results[POSE1_MEASURE_BNECK_SHOULDER_LENGTH]);


	/////////////////////////////////////// ��ھ�����ʱ��� 31
	m_Results[POSE1_MEASURE_BNECK_SHOULDERSIDE_LENGTH] =
		SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_BACK],
			m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_R],
			m_Paths[POSE1_MEASURE_BNECK_SHOULDERSIDE_LENGTH]
		);
	_cprintf("Bneck to Shoulder Side Length : %f\n", m_Results[POSE1_MEASURE_BNECK_SHOULDERSIDE_LENGTH]);


	/////////////////////////////////////// ��ھ�����̱��� 32
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


	/////////////////////////////////////// �ܵ���ں����̱��� 33
	m_Results[POSE1_MEASURE_ARMPITWALL_BACK_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_ARMPITWALL_BACK_R],
			m_Landmarks[POSE1_LANDMARK_ARMPITWALL_BACK_L],
			m_Paths[POSE1_MEASURE_ARMPITWALL_BACK_LENGTH]);
	_cprintf("Armpit Wall Back Length : %f\n", m_Results[POSE1_MEASURE_ARMPITWALL_BACK_LENGTH]);


	/////////////////////////////////////// �ܵ�����������̱��� 34
	m_Results[POSE1_MEASURE_ARMPITFOLD_BACK_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_ARMPITFOLD_BACK_R],
			m_Landmarks[POSE1_LANDMARK_ARMPITFOLD_BACK_L],
			m_Paths[POSE1_MEASURE_ARMPITFOLD_BACK_LENGTH]);
	_cprintf("Armpit Fold Back Length : %f\n", m_Results[POSE1_MEASURE_ARMPITFOLD_BACK_LENGTH]);


	/////////////////////////////////////// �ܵ���պ����̱��� 35
	m_Results[POSE1_MEASURE_ARMPITWALL_FRONT_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_ARMPITWALL_FRONT_R],
			m_Landmarks[POSE1_LANDMARK_ARMPITWALL_FRONT_L],
			m_Paths[POSE1_MEASURE_ARMPITWALL_FRONT_LENGTH]);
	_cprintf("Armpit Wall Front Length : %f\n", m_Results[POSE1_MEASURE_ARMPITWALL_FRONT_LENGTH]);


	/////////////////////////////////////// �ܵ�����������̱��� 36
	m_Results[POSE1_MEASURE_ARMPITFOLD_FRONT_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_ARMPITFOLD_FRONT_R],
			m_Landmarks[POSE1_LANDMARK_ARMPITFOLD_FRONT_L],
			m_Paths[POSE1_MEASURE_ARMPITFOLD_FRONT_LENGTH]);
	_cprintf("Armpit Fold Front Length : %f\n", m_Results[POSE1_MEASURE_ARMPITFOLD_FRONT_LENGTH]);


	/////////////////////////////////////// ������������� 37
	EgPos BP_R = m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos,
		BP_L = m_Landmarks[POSE1_LANDMARK_BP_L]->m_Pos;

	// ���� ������ �����
	BP_L.m_Coords[1] = BP_R.m_Coords[1];

	BP_R.m_Coords[2] += 30.0f;
	BP_L.m_Coords[2] += 30.0f;

	m_Paths[POSE1_MEASURE_BP_LENGTH].push_back(BP_R);
	m_Paths[POSE1_MEASURE_BP_LENGTH].push_back(BP_L);
	m_Results[POSE1_MEASURE_BP_LENGTH] = ABS(BP_R.m_Coords[0] - BP_L.m_Coords[0]);
	_cprintf("BP Length : %f\n", m_Results[POSE1_MEASURE_BP_LENGTH]);



	/////////////////////////////////////// �ܵ���յ����������̱��� 38
	EgPos ArmpitFoldPt_Front_R = m_Landmarks[POSE1_LANDMARK_ARMPITFOLD_FRONT_R]->m_Pos,
		ArmpitFoldPt_Back_R = m_Landmarks[POSE1_LANDMARK_ARMPITFOLD_BACK_R]->m_Pos;

	ArmpitFoldPt_Front_R.m_Coords[0] = m_MinX - 100.0f;
	ArmpitFoldPt_Back_R.m_Coords[0] = m_MinX - 100.0f;

	m_Paths[POSE1_MEASURE_ARMPITWALL_FRONT_BACK_LENGTH].push_back(ArmpitFoldPt_Front_R);
	m_Paths[POSE1_MEASURE_ARMPITWALL_FRONT_BACK_LENGTH].push_back(ArmpitFoldPt_Back_R);
	m_Results[POSE1_MEASURE_ARMPITWALL_FRONT_BACK_LENGTH] = dist(ArmpitFoldPt_Front_R, ArmpitFoldPt_Back_R);
	_cprintf("Armpit Wall Front-Back Length : %f\n", m_Results[POSE1_MEASURE_ARMPITWALL_FRONT_BACK_LENGTH]);



	/////////////////////////////////////// ���߽ɱ��� 39
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



	/////////////////////////////////////// ��ż��ؾ��߽ɱ��� 40
	NavelPt = m_Landmarks[POSE1_LANDMARK_NAVEL]->m_Pos;
	NavelPt.m_Coords[0] = FneckPt.m_Coords[0];
	EgVertex *NavelVertex = FindClosestVert(m_pMesh, NavelPt);

	m_Results[POSE1_MEASURE_CENTER_FRONT_TO_NAVEL_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_FRONT],
			NavelVertex,
			m_Paths[POSE1_MEASURE_CENTER_FRONT_TO_NAVEL_LENGTH]);
	_cprintf("Center front to navel Length : %f\n", m_Results[POSE1_MEASURE_CENTER_FRONT_TO_NAVEL_LENGTH]);


	/////////////////////////////////////// ������������ 41
	m_Results[POSE1_MEASURE_SNECK_BP_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_SIDE],
			m_Landmarks[POSE1_LANDMARK_BP_R],
			m_Paths[POSE1_MEASURE_SNECK_BP_LENGTH]);
	_cprintf("Side Neck to BP Length : %f\n", m_Results[POSE1_MEASURE_SNECK_BP_LENGTH]);


	/////////////////////////////////////// ��(������)�㸮�ѷ������� 42
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


	/////////////////////////////////////// ������������� 43
	// �������� ������������ �����Ÿ��̱� ������ BustPt�� x ������ SneckPt x �������� �̵��Ѵ�
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


	/////////////////////////////////////// ����������㸮�ѷ������� 44

	m_Paths[POSE1_MEASURE_BNECK_BP_WAIST_LENGTH].insert(
		m_Paths[POSE1_MEASURE_BNECK_BP_WAIST_LENGTH].end(),
		m_Paths[POSE1_MEASURE_BNECK_BP_LENGTH].begin(),
		m_Paths[POSE1_MEASURE_BNECK_BP_LENGTH].end()
	);
	m_Paths[POSE1_MEASURE_BNECK_BP_WAIST_LENGTH].push_back(EgPos(m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos[0], m_Landmarks[POSE1_LANDMARK_WAIST]->m_Pos[1], m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos[2]));

	double BP2WaistLevel = m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos[1] - m_Landmarks[POSE1_LANDMARK_WAIST]->m_Pos[1];

	m_Results[POSE1_MEASURE_BNECK_BP_WAIST_LENGTH] = Bneck2Sneck + Sneck2BP + BP2WaistLevel;

	_cprintf("Bneck to Sneck to BP to Waist Level : %f\n", m_Results[POSE1_MEASURE_BNECK_BP_WAIST_LENGTH]);


	/////////////////////////////////////// ��ڵ�����ܵ�����ر��� 45
	// Armpit�� Bneck x �������� ��ġ��Ų ��, �ڷ� ray�� ���� ������ ������ üǥ���̸� ���Ѵ�
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


	/////////////////////////////////////// �񿷵��㸮�ѷ������� 46

	m_Results[POSE1_MEASURE_SNECK_BWAIST_LEVEL_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_SIDE],
			FindClosestVert(m_pMesh, EgPos(m_Landmarks[POSE1_LANDMARK_NECK_SIDE]->m_Pos[0], m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos[1], m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos[2])),
			m_Paths[POSE1_MEASURE_SNECK_BWAIST_LEVEL_LENGTH]);

	_cprintf("Side Neck to BP Length : %f\n", m_Results[POSE1_MEASURE_SNECK_BWAIST_LEVEL_LENGTH]);


	/////////////////////////////////////// ����� 47
	m_Results[POSE1_MEASURE_BNECK_BWAIST_LENGTH]
		= SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_BACK],
			FindClosestVert(m_pMesh, EgPos(m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos[0], m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos[1], m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos[2])),
			m_Paths[POSE1_MEASURE_BNECK_BWAIST_LENGTH]);

	_cprintf("Back Length : %f\n", m_Results[POSE1_MEASURE_BNECK_BWAIST_LENGTH]);


	/////////////////////////////////////// ��ż��ص���� 48

	m_Results[POSE1_MEASURE_BNECK_BNAVEL_LENGTH] =
		SK_Shortest_Distance(
			m_Landmarks[POSE1_LANDMARK_NECK_BACK],
			FindClosestVert(m_pMesh, EgPos(m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos[0], m_Landmarks[POSE1_LANDMARK_NAVEL_BACK]->m_Pos[1], m_Landmarks[POSE1_LANDMARK_NAVEL_BACK]->m_Pos[2])),
			m_Paths[POSE1_MEASURE_BNECK_BNAVEL_LENGTH]
		);
	_cprintf("Navel level Back Length : %f\n", m_Results[POSE1_MEASURE_BNECK_BNAVEL_LENGTH]);


	/////////////////////////////////////// ��ڿ��ݱ��� 49

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


	/////////////////////////////////////// �ѱ��� 50
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




	/////////////////////////////////////// �ȱ���(�����) 53
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


	/////////////////////////////////////// ���ȱ���(�����) 51
	pln = EgPlane(m_Landmarks[POSE1_LANDMARK_SHOULDER_R]->m_Pos, m_Landmarks[POSE1_LANDMARK_ELBOW]->m_Pos, m_Landmarks[POSE1_LANDMARK_WRIST]->m_Pos);

	m_Results[POSE1_MEASURE_SHOULDER_ELBOW_LENGTH] = UpperLen;
	for (EgPos pos : UpperPath)
		m_Paths[POSE1_MEASURE_SHOULDER_ELBOW_LENGTH].push_back(pos);



	/////////////////////////////////////// �ȱ���(���������) 54
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


	/////////////////////////////////////// ���ȱ���(���������) 52
	m_Results[POSE1_MEASURE_SHOULDERSIDE_ELBOW_LENGTH] = UpperLen;
	for (EgPos pos : UpperPath)
		m_Paths[POSE1_MEASURE_SHOULDERSIDE_ELBOW_LENGTH].push_back(pos);



	/////////////////////////////////////// ����������� 55
	BneckPt = m_Landmarks[POSE1_LANDMARK_NECK_BACK]->m_Pos;
	CrotchPt = m_Landmarks[POSE1_LANDMARK_CROTCH]->m_Pos;

	BneckPt.m_Coords[2] = m_MinZ - 300.0f;
	CrotchPt.m_Coords[2] = m_MinZ - 300.0f;
	CrotchPt.m_Coords[0] = BneckPt.m_Coords[0];

	m_Paths[POSE1_MEASURE_TORSO_LENGTH].push_back(BneckPt);
	m_Paths[POSE1_MEASURE_TORSO_LENGTH].push_back(CrotchPt);

	m_Results[POSE1_MEASURE_TORSO_LENGTH] = BneckPt.m_Coords[1] - CrotchPt.m_Coords[1];

	// _cprintf("Torso Length : %f\n", m_Results[POSE1_MEASURE_TORSO_LENGTH]);



	/////////////////////////////////////// �����̼������� 56
	EgPos BwaistPt = m_Landmarks[POSE1_LANDMARK_WAIST_BACK]->m_Pos;
	CrotchPt = m_Landmarks[POSE1_LANDMARK_CROTCH]->m_Pos;

	BwaistPt.m_Coords[2] = m_MinZ - 400.0f;
	CrotchPt.m_Coords[2] = m_MinZ - 400.0f;
	CrotchPt.m_Coords[0] = BwaistPt.m_Coords[0];

	m_Paths[POSE1_MEASURE_HIP_LENGTH].push_back(BwaistPt);
	m_Paths[POSE1_MEASURE_HIP_LENGTH].push_back(CrotchPt);

	m_Results[POSE1_MEASURE_HIP_LENGTH] = BwaistPt.m_Coords[1] - CrotchPt.m_Coords[1];
	// _cprintf("Hip Length : %f\n", m_Results[POSE1_MEASURE_HIP_LENGTH]);


	/////////////////////////////////////// ��ڼո���ʱ���(�ȳ���) 57
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


	/////////////////////////////////////// �Ҵٸ��������� 58
	m_Paths[POSE1_MEASURE_UNDERHIP_POPLITEUS_LENGTH].push_back(m_Landmarks[POSE1_LANDMARK_UNDERHIP]->m_Pos);
	m_Paths[POSE1_MEASURE_UNDERHIP_POPLITEUS_LENGTH].push_back(m_Landmarks[POSE1_LANDMARK_POPLITEUS]->m_Pos);

	m_Results[POSE1_MEASURE_UNDERHIP_POPLITEUS_LENGTH] = dist(m_Landmarks[POSE1_LANDMARK_UNDERHIP]->m_Pos, m_Landmarks[POSE1_LANDMARK_POPLITEUS]->m_Pos);


	/////////////////////////////////////////////////////////// �β� ����
	/////////////////////////////////////// ����赹��β� 59
	StomachExtrudePt = m_Landmarks[POSE1_LANDMARK_STOMACH_EXTRUDE]->m_Pos;
	HipExtrudePt = m_Landmarks[POSE1_LANDMARK_HIP_EXTRUDE]->m_Pos;

	StomachExtrudePt.m_Coords[0] = m_MinX - 350.0f;
	HipExtrudePt.m_Coords[0] = m_MinX - 350.0f;
	HipExtrudePt.m_Coords[1] = StomachExtrudePt.m_Coords[1];

	m_Paths[POSE1_MEASURE_STOMACH_HIP_DEPTH].push_back(StomachExtrudePt);
	m_Paths[POSE1_MEASURE_STOMACH_HIP_DEPTH].push_back(HipExtrudePt);

	m_Results[POSE1_MEASURE_STOMACH_HIP_DEPTH] = StomachExtrudePt.m_Coords[2] - HipExtrudePt.m_Coords[2];
	// _cprintf("Stomach-Hip Depth : %f\n", m_Results[POSE1_MEASURE_STOMACH_HIP_DEPTH]);


	/////////////////////////////////////// ���鰡���β� 60
	BP_R = m_Landmarks[POSE1_LANDMARK_BP_R]->m_Pos;
	EgPos BackExtrudePt = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_BACK_EXTRUDE]->m_Pos;

	BP_R.m_Coords[0] = m_MinX - 450.0f;
	BackExtrudePt.m_Coords[0] = m_MinX - 450.0f;
	BackExtrudePt.m_Coords[1] = BP_R.m_Coords[1];

	m_Paths[POSE1_MEASURE_BUST_DEPTH].push_back(BP_R);
	m_Paths[POSE1_MEASURE_BUST_DEPTH].push_back(BackExtrudePt);

	m_Results[POSE1_MEASURE_BUST_DEPTH] = BP_R.m_Coords[2] - BackExtrudePt.m_Coords[2];
	// _cprintf("Bust Depth : %f\n", m_Results[POSE1_MEASURE_BUST_DEPTH]);



	/////////////////////////////////////// �������β� 61
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



	/////////////////////////////////////////////////////////// ���� ����
	/////////////////////////////////////// �����ʾ�����ʰ�簢 62
	EgLine StandardLine(m_Landmarks[POSE1_LANDMARK_NECK_SIDE]->m_Pos, EgVec3(-1, 0, 0));
	EgLine ShoulderSideLine(m_Landmarks[POSE1_LANDMARK_NECK_SIDE]->m_Pos, m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_R]->m_Pos);

	m_Results[POSE1_MEASURE_SHOULDERSIDE_RIGHT_ANGLE] = angle(StandardLine.m_Dir, ShoulderSideLine.m_Dir, false);
	// _cprintf("Shoulder Side Right Angle : %f\n", m_Results[POSE1_MEASURE_SHOULDERSIDE_RIGHT_ANGLE]);


	/////////////////////////////////////// ���ʾ�����ʰ�簢 63
	EgPos SneckPt_L = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_NECK_SIDE_L]->m_Pos;
	EgPos ShoulderSidePt_L = m_Landmarks[POSE1_LANDMARK_SHOULDERSIDE_L]->m_Pos;

	StandardLine = EgLine(SneckPt_L, EgVec3(1, 0, 0));
	ShoulderSideLine = EgLine(SneckPt_L, ShoulderSidePt_L);

	m_Results[POSE1_MEASURE_SHOULDERSIDE_LEFT_ANGLE] = angle(StandardLine.m_Dir, ShoulderSideLine.m_Dir, false);
	// _cprintf("Shoulder Side Left Angle : %f\n", m_Results[POSE1_MEASURE_SHOULDERSIDE_LEFT_ANGLE]);


	/////////////////////////////////////// �����ʾ����簢 64
	StandardLine = EgLine(m_Landmarks[POSE1_LANDMARK_NECK_SIDE]->m_Pos, EgVec3(-1, 0, 0));
	EgLine ShoulderLine(m_Landmarks[POSE1_LANDMARK_NECK_SIDE]->m_Pos, m_Landmarks[POSE1_LANDMARK_SHOULDER_R]->m_Pos);

	m_Results[POSE1_MEASURE_SHOULDER_RIGHT_ANGLE] = angle(StandardLine.m_Dir, ShoulderLine.m_Dir, false);
	// _cprintf("Shoulder Side Angle : %f\n", m_Results[POSE1_MEASURE_SHOULDER_RIGHT_ANGLE]);


	/////////////////////////////////////// ���ʾ����簢 65
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

// mj::m_nLandmarkPose2�� m_Landmarks�� �Ҵ��Ѵ�
void EgSizer::MeasurePose2() {
	for (int i = 0; i < NEW_RESULT_NUM_POSE2; i++) {
		m_Results[i] = m_nResultPose2[i];
	}

	for (int i = 0; i < NEW_RESULT_NUM_POSE2; i++) {
		m_Paths[i] = m_nResultPathPose2[i];
	}
}

/*!
*	\brief	�ڼ� 3�� �������� ã�´�.
*	\note	������ ������: 2020-07-15
*/
void EgSizer::FindLandmarkPose3()	// �������帶ũ 
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

	// 1. �����̵�����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(1);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����̵����� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	int idx = GetLandmarkIdx("�����̵�����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Buttock_Protrusion(m_pMesh);

	// 2. �赹����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(2);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�赹���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�赹����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Abdominal_Protrusion(m_pMesh);

	// 3. �������ִ�ѷ�������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(3);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�������ִ�ѷ������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�������ִ�ѷ�������");
	std::vector<EgPos> Path;
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Buttock_Max_Perimeter(m_pMesh, Path);

	// 4. �ָ԰����(����)
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(4);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����ָ԰���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����ָ԰����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Left_Fist_Center(m_pMesh);

	// 4. �ָ԰����(������)
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(5);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�������ָ԰���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�������ָ԰����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Right_Fist_Center(m_pMesh);

	// 5. ������ �����������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(6);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����ʵ���������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����ʵ����������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Right_Back_Protrusion(m_pMesh);

	// 6. �Ȳ�ġ����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(7);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����Ȳ�ġ���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����Ȳ�ġ����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Rear_Olecranon(m_pMesh);

	// 7. �Ȳ�ġ�Ʒ���
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(8);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����Ȳ�ġ�Ʒ��� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����Ȳ�ġ�Ʒ���");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Bottom_Olecranon(m_pMesh);

	// 8. ������ �����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(9);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����ʾ���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����ʾ����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose3_Right_Acromion(m_pMesh);

	// ���� ���� â�� �ʱ�ȭ �Ѵ�.
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(0);
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	pFrame->m_pLandmarkProgressDlg->ShowWindow(FALSE);
}

/*!
*	\brief	�ڼ� 3�� ���� �׸��� ����Ѵ�.
*	\note	������ ������: 2020-10-22
*/
void EgSizer::MeasurePose3()
{
	int mark_idx, mark_idx1;
	double minY = m_pMesh->m_BndBox.m_MinPt[1];

	// 1. ������ �ִ�ѷ� ����
	mark_idx = GetLandmarkIdx("�������ִ�ѷ�������");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("�������ִ�ѷ�����");
		m_Results[result_idx] = p[1] - minY;
		m_Paths[result_idx].push_back(p);
		m_Paths[result_idx].push_back(EgPos(p[0], minY, p[2]));
	}

	// 2. ���� �Ȳ�ġ ����
	mark_idx = GetLandmarkIdx("�����Ȳ�ġ�Ʒ���");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("�����Ȳ�ġ����");
		m_Results[result_idx] = p[1] - minY;
		m_Paths[result_idx].push_back(p);
		m_Paths[result_idx].push_back(EgPos(p[0], minY, p[2]));
	}

	// 3. ������ �ѷ�
	mark_idx = GetLandmarkIdx("�����̵�����");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("�����̵ѷ�");
		EgPlane Pln(EgVec3(0.0, 1.0, 0.0), p);
		double len = SK_Planar_Perimeter(m_Landmarks[mark_idx], &Pln, m_Paths[result_idx], true);
		m_Results[result_idx] = len;
	}

	// 4. �������ִ�ѷ�
	mark_idx = GetLandmarkIdx("�������ִ�ѷ�������");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("�������ִ�ѷ�");
		EgPlane Pln(EgVec3(0.0, 1.0, 0.0), p);
		double len = SK_Planar_Perimeter(m_Landmarks[mark_idx], &Pln, m_Paths[result_idx], true);
		m_Results[result_idx] = len;
	}

	// 5. �赹�������ؾ����̵ѷ�
	mark_idx = GetLandmarkIdx("�����̵�����");
	mark_idx1 = GetLandmarkIdx("�赹����");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		// ������ �ѷ� ������ �����Ѵ�.
		std::vector<EgPos> Pts1 = m_Paths[GetResultIdx("�����̵ѷ�")];

		// �赹���� �ѷ��� ���Ѵ�.
		std::vector<EgPos> Pts2;
		EgPlane Pln(EgVec3(0.0, 1.0, 0.0), m_Landmarks[mark_idx1]->m_Pos);
		double len = SK_Planar_Perimeter(m_Landmarks[mark_idx1], &Pln, Pts2, true);

		// ������ �ѷ� ������ �赹���� �ѷ� ������ ��ģ��.
		for (EgPos pt : Pts2)
		{
			pt[1] = Pts1[0][1];
			Pts1.push_back(pt);
		}

		// �赹�������ؾ����̵ѷ��� ���Ѵ�.
		int result_idx = GetResultIdx("�赹�������ؾ����̵ѷ�");
		Pln = EgPlane(EgVec3(0.0, 1.0, 0.0), m_Landmarks[mark_idx]->m_Pos);
		m_Results[result_idx] = SK_ConvexHull(&Pln, Pts1, m_Paths[result_idx]);
	}

	// 6. �Ȳ�ġ�ָԼ������
	mark_idx = GetLandmarkIdx("�����ָ԰����");
	mark_idx1 = GetLandmarkIdx("�����Ȳ�ġ����");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		EgPos q = m_Landmarks[mark_idx1]->m_Pos;
		int result_idx = GetResultIdx("�Ȳ�ġ�ָԼ������");
		m_Results[result_idx] = p[2] - q[2];
		m_Paths[result_idx].push_back(EgPos(p[0] + 50, p[1], p[2]));
		m_Paths[result_idx].push_back(EgPos(p[0] + 50, p[1], q[2]));
	}

	// 7. ��������λ����ָԼ������
	mark_idx = GetLandmarkIdx("�������ָ԰����");
	mark_idx1 = GetLandmarkIdx("�����ʵ����������");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		EgPos q = m_Landmarks[mark_idx1]->m_Pos;
		int result_idx = GetResultIdx("��������λ����ָԼ������");
		m_Results[result_idx] = p[2] - q[2];
		m_Paths[result_idx].push_back(EgPos(p[0] - 50, p[1], p[2]));
		m_Paths[result_idx].push_back(EgPos(p[0] - 50, p[1], q[2]));
	}

	// 8. �������������
	mark_idx = GetLandmarkIdx("�����ʾ����");
	mark_idx1 = GetLandmarkIdx("�����ʵ����������");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		EgPos q = m_Landmarks[mark_idx1]->m_Pos;
		int result_idx = GetResultIdx("�������������");
		m_Results[result_idx] = p[2] - q[2];
		m_Paths[result_idx].push_back(EgPos(p[0], p[1], p[2]));
		m_Paths[result_idx].push_back(EgPos(p[0], p[1], q[2]));
	}

	printf("/*******************/\n");
	printf("  �ڼ� 3 ���� ��� \n");
	printf("/*******************/\n");
	for (int i = 0; i < NUM(ResultPose3); ++i)
		_cprintf("%d. %s: %.2lf (mm)\n", i + 1, ResultPose3[i].c_str(), m_Results[i]);
}

/*!
*	\brief	�ڼ� 4�� �������� ã�´�.
*	\note	������ ������: 2020-07-15
*/
void EgSizer::FindLandmarkPose4()	// �������帶ũ
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

	// �Ӹ�������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(1);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�Ӹ������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	int idx = GetLandmarkIdx("�Ӹ�������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Head_Top(m_pMesh);

	// �����������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(2);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("��������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Base_Sit(m_pMesh);

	// ������������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(3);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("������������ ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("������������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Anterior_Knee_Sit(m_pMesh);

	// ����������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(4);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("����������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Posterior_Juncture_Calf_Thigh(m_pMesh);

	// (������)���������̻����ִ뵹�������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(5);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����ʾ��������̻����ִ뵹������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����ʾ��������̻����ִ뵹�������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Max_Hip_Width_Right(m_pMesh);

	// (����)���������̻����ִ뵹�������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(6);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���ʾ��������̻����ִ뵹������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("���ʾ��������̻����ִ뵹�������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Max_Hip_Width_Left(m_pMesh);

	// �ճ���
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(7);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�ճ��� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�ճ���");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Dactylion(m_pMesh);

	// ���������̵ڵ�����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(8);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���������̵ڵ����� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("���������̵ڵ�����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Buttock_Protrusion(m_pMesh);

	// �ո������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(9);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�ո������ ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�ո������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Ulnar_Styloid(m_pMesh);

	// �����Ҵٸ�����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(10);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����Ҵٸ����� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����Ҵٸ�����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Superior_Thigh_Sit(m_pMesh);

	// ����������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(12);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("����������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Superior_Patella_Sit(m_pMesh);

	// �Ȳ�ġ�����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(13);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�Ȳ�ġ����� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�Ȳ�ġ�����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Center_Olecranon(m_pMesh);

	// �Ȳ�ġ����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(14);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�Ȳ�ġ���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�Ȳ�ġ����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Rear_Olecranon_Sit(m_pMesh);

	// �Ȳ�ġ�Ʒ���
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(15);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�Ȳ�ġ�Ʒ��� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�Ȳ�ġ�Ʒ���");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Bottom_Olecranon_Sit(m_pMesh);

	// �����赹����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(11);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����赹���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����赹����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Abdominal_Protrusion_Sit(m_pMesh);

	// �������Ȳ�ġ������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(16);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�������Ȳ�ġ������ ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�������Ȳ�ġ������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Lateral_Humeral_Epicondyle_Right(m_pMesh);

	// �����Ȳ�ġ������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(17);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����Ȳ�ġ������ ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����Ȳ�ġ������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Lateral_Humeral_Epicondyle_Left(m_pMesh);

	// �����ʾ��������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(18);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����ʾ�������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����ʾ��������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Lateral_Shoulder_Right(m_pMesh);

	// ���ʾ��������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(19);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���ʾ�������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("���ʾ��������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Lateral_Shoulder_Left(m_pMesh);

	// �����ʾ����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(20);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����ʾ���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����ʾ����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Acromion_Right(m_pMesh);

	// ���ʾ����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(21);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���ʾ���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("���ʾ����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Acromion_Left(m_pMesh);

	// �����ʾ���������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(22);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����ʾ��������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����ʾ���������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Deltoid_Right(m_pMesh);

	// ���ʾ���������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(23);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���ʾ��������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("���ʾ���������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Deltoid_Left(m_pMesh);

	// �����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(24);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("����� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Cervicale(m_pMesh);

	// ���ʸ���
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(25);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���ʸ��� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("���ʸ���");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose4_Ectocanthus(m_pMesh);

	// ���� ���� â�� �ʱ�ȭ �Ѵ�.
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(0);
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	pFrame->m_pLandmarkProgressDlg->ShowWindow(FALSE);
}

/*!
*	\brief	�ڼ� 4�� ���� �׸��� ����Ѵ�.
*	\note	������ ������: 2020-09-27
*/
void EgSizer::MeasurePose4()
{
	int mark_idx1, mark_idx2;
	double minX = m_pMesh->m_BndBox.m_MinPt[0];
	double minY = m_pMesh->m_BndBox.m_MinPt[1];
	double minZ = m_pMesh->m_BndBox.m_MinPt[2];
	double maxZ = m_pMesh->m_BndBox.m_MaxPt[2];

	// 1. ����Ű
	mark_idx1 = GetLandmarkIdx("�Ӹ�������");
	mark_idx2 = GetLandmarkIdx("�����������");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("����Ű");
		m_Results[ret_idx] = p[1] - q[1];
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 150.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], q[1], minZ - 150.0));
	}

	// 2. ����������
	mark_idx1 = GetLandmarkIdx("���ʸ���");
	mark_idx2 = GetLandmarkIdx("�����������");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("����������");
		m_Results[ret_idx] = p[1] - q[1];
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 120.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], q[1], minZ - 120.0));
	}

	// 3. ������ڳ���
	mark_idx1 = GetLandmarkIdx("�����");
	mark_idx2 = GetLandmarkIdx("�����������");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("������ڳ���");
		m_Results[ret_idx] = p[1] - q[1];
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 90.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], q[1], minZ - 90.0));
	}

	// 4. �����������
	mark_idx1 = GetLandmarkIdx("�����ʾ����");
	mark_idx2 = GetLandmarkIdx("�����������");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("�����������");
		m_Results[ret_idx] = p[1] - q[1];
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 60.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], q[1], minZ - 60.0));
	}

	// 5. �����Ȳ�ġ����
	mark_idx1 = GetLandmarkIdx("�Ȳ�ġ�Ʒ���");
	mark_idx2 = GetLandmarkIdx("�����������");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("�����Ȳ�ġ����");
		m_Results[ret_idx] = p[1] - q[1];
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], q[1], minZ - 30.0));
	}

	// 6. �����Ҵٸ�����
	mark_idx1 = GetLandmarkIdx("�����Ҵٸ�����");
	if (m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		int ret_idx = GetResultIdx("�����Ҵٸ�����");
		m_Results[ret_idx] = p[1] - minY;
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], maxZ + 90.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], minY, maxZ + 90.0));
	}

	// 7. ������������
	mark_idx1 = GetLandmarkIdx("����������");
	if (m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		int ret_idx = GetResultIdx("������������");
		m_Results[ret_idx] = p[1] - minY;
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], maxZ + 60.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], minY, maxZ + 60.0));
	}

	// 8. �������ݳ���
	mark_idx1 = GetLandmarkIdx("����������");
	if (m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		int ret_idx = GetResultIdx("�������ݳ���");
		m_Results[ret_idx] = p[1] - minY;
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], maxZ + 30.0));
		m_Paths[ret_idx].push_back(EgPos(p[0], minY, maxZ + 30.0));
	}

	// 9. ������̳ʺ�
	mark_idx1 = GetLandmarkIdx("�����ʾ����");
	mark_idx2 = GetLandmarkIdx("���ʾ����");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("������̳ʺ�");
		m_Results[ret_idx] = ABS(q[0] - p[0]);
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1] - 15, minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(q[0], p[1] - 15, minZ - 30.0));
	}

	// 10. ������ʻ��̳ʺ�
	mark_idx1 = GetLandmarkIdx("�����ʾ��������");
	mark_idx2 = GetLandmarkIdx("���ʾ��������");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("������ʻ��̳ʺ�");
		m_Results[ret_idx] = ABS(q[0] - p[0]);
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1] + 15, minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(q[0], p[1] + 15, minZ - 30.0));
	}

	// 11. ���Ȼ��̳ʺ�
	mark_idx1 = GetLandmarkIdx("�����ʾ���������");
	mark_idx2 = GetLandmarkIdx("���ʾ���������");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("���Ȼ��̳ʺ�");
		m_Results[ret_idx] = ABS(q[0] - p[0]);
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(q[0], p[1], minZ - 30.0));
	}

	// 12. �Ȳ�ġ���̳ʺ�
	mark_idx1 = GetLandmarkIdx("�������Ȳ�ġ������");
	mark_idx2 = GetLandmarkIdx("�����Ȳ�ġ������");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("�Ȳ�ġ���̳ʺ�");
		m_Results[ret_idx] = ABS(q[0] - p[0]);
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(q[0], p[1], minZ - 30.0));
	}

	// 13. ���������̳ʺ�
	mark_idx1 = GetLandmarkIdx("�����ʾ��������̻����ִ뵹�������");
	mark_idx2 = GetLandmarkIdx("���ʾ��������̻����ִ뵹�������");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("���������̳ʺ�");
		m_Results[ret_idx] = ABS(q[0] - p[0]);
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(q[0], p[1], minZ - 30.0));
	}

	// 14. �����Ҵٸ��β�
	mark_idx1 = GetLandmarkIdx("�����Ҵٸ�����");
	mark_idx2 = GetLandmarkIdx("�����������");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		int ret_idx = GetResultIdx("�����Ҵٸ��β�");
		m_Results[ret_idx] = ABS(p[1] - q[1]);
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, q[1], p[2]));
	}

	// 15. ������β�
	mark_idx1 = GetLandmarkIdx("�����赹����");
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
		int ret_idx = GetResultIdx("������β�");
		m_Results[ret_idx] = ABS(p[2] - q[2]);
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, q[1], q[2]));
	}

	// 16. ���������̹�β�
	mark_idx1 = GetLandmarkIdx("�����赹����");
	mark_idx2 = GetLandmarkIdx("���������̵ڵ�����");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		p[1] = q[1];
		int ret_idx = GetResultIdx("���������̹�β�");
		m_Results[ret_idx] = ABS(p[2] - q[2]);
		m_Paths[ret_idx].push_back(EgPos(minX, q[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, q[1], q[2]));
	}

	// 17. �����ȱ���
	mark_idx1 = GetLandmarkIdx("�����ʾ����");
	mark_idx2 = GetLandmarkIdx("�Ȳ�ġ�����");
	int mark_idx3 = GetLandmarkIdx("�ո������");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL &&
		m_Landmarks[mark_idx3] != NULL)
	{
		int ret_idx = GetResultIdx("�����ȱ���");
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

	// 18. ���������̿��ݼ������
	mark_idx1 = GetLandmarkIdx("����������");
	mark_idx2 = GetLandmarkIdx("���������̵ڵ�����");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		q[1] = p[1];
		int ret_idx = GetResultIdx("���������̿��ݼ������");
		m_Results[ret_idx] = ABS(p[2] - q[2]);
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], q[2]));
	}

	// 19. ���������̹���(��)�������
	mark_idx1 = GetLandmarkIdx("������������");
	mark_idx2 = GetLandmarkIdx("���������̵ڵ�����");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		q[1] = p[1];
		int ret_idx = GetResultIdx("���������̹���(��)�������");
		m_Results[ret_idx] = ABS(p[2] - q[2]);
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], q[2]));
	}

	// 20. (�ȱ���)���ȼ�������
	mark_idx1 = GetLandmarkIdx("�����ʾ����");
	mark_idx2 = GetLandmarkIdx("�Ȳ�ġ�Ʒ���");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		q[0] = p[0];
		int ret_idx = GetResultIdx("(�ȱ���)���ȼ�������");
		m_Results[ret_idx] = ABS(p[1] - q[1]);
		m_Paths[ret_idx].push_back(EgPos(p[0], p[1], minZ - 30.0));
		m_Paths[ret_idx].push_back(EgPos(q[0], q[1], minZ - 30.0));
	}

	// 21. (�ȱ���)�Ʒ��ȼ������
	mark_idx1 = GetLandmarkIdx("�Ȳ�ġ����");
	mark_idx2 = GetLandmarkIdx("�ո������");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		q[1] = p[1];
		int ret_idx = GetResultIdx("(�ȱ���)�Ʒ��ȼ������");
		m_Results[ret_idx] = ABS(p[2] - q[2]);
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, q[1], q[2]));
	}

	// 22. (�ȱ���)�Ȳ�ġ�ճ��������
	mark_idx1 = GetLandmarkIdx("�ճ���");
	mark_idx2 = GetLandmarkIdx("�Ȳ�ġ����");
	if (m_Landmarks[mark_idx1] != NULL && m_Landmarks[mark_idx2] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx1]->m_Pos;
		EgPos q = m_Landmarks[mark_idx2]->m_Pos;
		q[1] = p[1];
		int ret_idx = GetResultIdx("(�ȱ���)�Ȳ�ġ�ճ��������");
		m_Results[ret_idx] = ABS(p[2] - q[2]);
		m_Paths[ret_idx].push_back(EgPos(minX, p[1], p[2]));
		m_Paths[ret_idx].push_back(EgPos(minX, q[1], q[2]));
	}

	printf("/*******************/\n");
	printf("  �ڼ� 4 ���� ��� \n");
	printf("/*******************/\n");
	for (int i = 0; i < NUM(ResultPose4); ++i)
		printf("%d. %s: %.2lf (mm)\n", i + 1, ResultPose4[i].c_str(), m_Results[i]);
}

/*!
*	\brief	�ڼ�5(�ڼ�3�� ���� �ڼ�)�� �������� ã�´�.
*	\note	������ ������: 2020-09-05
*/
void EgSizer::FindLandmarkPose5()	// �������帶ũ
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

	// 1. �����̵�����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(1);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����̵����� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	int idx = GetLandmarkIdx("�����̵�����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Buttock_Protrusion(m_pMesh);

	// 2. �赹����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(2);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�赹���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�赹����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Abdominal_Protrusion(m_pMesh);

	// 3. �������ִ�ѷ�������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(3);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�������ִ�ѷ������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�������ִ�ѷ�������");
	std::vector<EgPos> Path;
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Buttock_Max_Perimeter(m_pMesh, Path);

	// 4. �ָ԰����(����)
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(4);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�����ָ԰���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�����ָ԰����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Left_Fist_Center(m_pMesh);

	// 4. �ָ԰����(������)
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(5);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�������ָ԰���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�������ָ԰����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Right_Fist_Center(m_pMesh);

	// 5. ���� �����������
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(6);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���ʵ���������� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("���ʵ����������");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Left_Back_Protrusion(m_pMesh);

	// 6. �Ȳ�ġ�Ʒ���
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(7);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�������Ȳ�ġ�Ʒ��� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�������Ȳ�ġ�Ʒ���");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Bottom_Olecranon(m_pMesh);

	// 7. �Ȳ�ġ����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(8);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("�������Ȳ�ġ���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("�������Ȳ�ġ����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Rear_Olecranon(m_pMesh);

	// 8. ���ʾ����
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(9);
	pFrame->m_pLandmarkProgressDlg->m_Status.Format("%s", _T("���ʾ���� ã�� ��..."));
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	idx = GetLandmarkIdx("���ʾ����");
	if (m_bUserPick[idx] == false)
		m_Landmarks[idx] = SK_Pose5_Right_Acromion(m_pMesh);

	// ���� ���� â�� �ʱ�ȭ �Ѵ�.
	pFrame->m_pLandmarkProgressDlg->m_LandmarkProgress.SetPos(0);
	pFrame->m_pLandmarkProgressDlg->UpdateData(FALSE);
	pFrame->m_pLandmarkProgressDlg->ShowWindow(FALSE);
}

/*!
*	\brief	�ڼ� 3(��)�� ���� �׸��� ����Ѵ�.
*	\note	������ ������: 2020-10-22
*/
void EgSizer::MeasurePose5()
{
	int mark_idx, mark_idx1;
	double minY = m_pMesh->m_BndBox.m_MinPt[1];

	// 1. ������ �ִ�ѷ� ����
	mark_idx = GetLandmarkIdx("�������ִ�ѷ�������");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("�������ִ�ѷ�����");
		m_Results[result_idx] = p[1] - minY;
		m_Paths[result_idx].push_back(p);
		m_Paths[result_idx].push_back(EgPos(p[0], minY, p[2]));
	}

	// 2. ���� �Ȳ�ġ ����
	mark_idx = GetLandmarkIdx("�������Ȳ�ġ�Ʒ���");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("�����Ȳ�ġ����");
		m_Results[result_idx] = p[1] - minY;
		m_Paths[result_idx].push_back(p);
		m_Paths[result_idx].push_back(EgPos(p[0], minY, p[2]));
	}

	// 3. ������ �ѷ�
	mark_idx = GetLandmarkIdx("�����̵�����");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("�����̵ѷ�");
		EgPlane Pln(EgVec3(0.0, 1.0, 0.0), p);
		double len = SK_Planar_Perimeter(m_Landmarks[mark_idx], &Pln, m_Paths[result_idx], true);
		m_Results[result_idx] = len;
	}

	// 4. �������ִ�ѷ�
	mark_idx = GetLandmarkIdx("�������ִ�ѷ�������");
	if (m_Landmarks[mark_idx] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		int result_idx = GetResultIdx("�������ִ�ѷ�");
		EgPlane Pln(EgVec3(0.0, 1.0, 0.0), p);
		double len = SK_Planar_Perimeter(m_Landmarks[mark_idx], &Pln, m_Paths[result_idx], true);
		m_Results[result_idx] = len;
	}

	// 5. �赹�������ؾ����̵ѷ�
	mark_idx = GetLandmarkIdx("�����̵�����");
	mark_idx1 = GetLandmarkIdx("�赹����");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		// ������ �ѷ� ������ �����Ѵ�.
		std::vector<EgPos> Pts1 = m_Paths[GetResultIdx("�����̵ѷ�")];

		// �赹���� �ѷ��� ���Ѵ�.
		std::vector<EgPos> Pts2;
		EgPlane Pln(EgVec3(0.0, 1.0, 0.0), m_Landmarks[mark_idx1]->m_Pos);
		double len = SK_Planar_Perimeter(m_Landmarks[mark_idx1], &Pln, Pts2, true);

		// ������ �ѷ� ������ �赹���� �ѷ� ������ ��ģ��.
		for (EgPos pt : Pts2)
		{
			pt[1] = Pts1[0][1];
			Pts1.push_back(pt);
		}

		// �赹�������ؾ����̵ѷ��� ���Ѵ�.
		int result_idx = GetResultIdx("�赹�������ؾ����̵ѷ�");
		Pln = EgPlane(EgVec3(0.0, 1.0, 0.0), m_Landmarks[mark_idx]->m_Pos);
		m_Results[result_idx] = SK_ConvexHull(&Pln, Pts1, m_Paths[result_idx]);
	}


	// 6. �Ȳ�ġ�ָԼ������
	mark_idx = GetLandmarkIdx("�������ָ԰����");
	mark_idx1 = GetLandmarkIdx("�������Ȳ�ġ����");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		EgPos q = m_Landmarks[mark_idx1]->m_Pos;
		int result_idx = GetResultIdx("�Ȳ�ġ�ָԼ������");
		m_Results[result_idx] = p[2] - q[2];
		m_Paths[result_idx].push_back(EgPos(p[0] - 50, p[1], p[2]));
		m_Paths[result_idx].push_back(EgPos(p[0] - 50, p[1], q[2]));
	}

	// 7. ��������λ����ָԼ������
	mark_idx = GetLandmarkIdx("�����ָ԰����");
	mark_idx1 = GetLandmarkIdx("���ʵ����������");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		EgPos q = m_Landmarks[mark_idx1]->m_Pos;
		int result_idx = GetResultIdx("��������λ����ָԼ������");
		m_Results[result_idx] = p[2] - q[2];
		m_Paths[result_idx].push_back(EgPos(p[0] + 50, p[1], p[2]));
		m_Paths[result_idx].push_back(EgPos(p[0] + 50, p[1], q[2]));
	}

	// 8. �������������
	mark_idx = GetLandmarkIdx("���ʾ����");
	mark_idx1 = GetLandmarkIdx("���ʵ����������");
	if (m_Landmarks[mark_idx] != NULL && m_Landmarks[mark_idx1] != NULL)
	{
		EgPos p = m_Landmarks[mark_idx]->m_Pos;
		EgPos q = m_Landmarks[mark_idx1]->m_Pos;
		int result_idx = GetResultIdx("�������������");
		m_Results[result_idx] = p[2] - q[2];
		m_Paths[result_idx].push_back(EgPos(p[0], p[1], p[2]));
		m_Paths[result_idx].push_back(EgPos(p[0], p[1], q[2]));
	}

	printf("/*******************/\n");
	printf("  �ڼ� 3(��) ���� ��� \n");
	printf("/*******************/\n");
	for (int i = 0; i < NUM(ResultPose5); ++i)
		_cprintf("%d. %s: %.1lf (mm)\n", i + 1, ResultPose5[i].c_str(), m_Results[i]);
}

// mj::
void EgSizer::SetGender() {
	m_bWoman = !m_bWoman;
}

/*!
*	\brief	�ش� �ڼ��� ���� �׸��� ������ �������Ѵ�.
*	\note	������ ������: 2020-07-15
*/
void EgSizer::Render()
{
	// �ٴڸ��� ������ �Ѵ�.
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

	// ���� ��ũ�� ������ �Ѵ�.
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

	// ��θ� ������ �Ѵ�.
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
*	\brief	�� ������ ȭ��ǥ�� ���� �� ���� �մ� ������ ������ �Ѵ�.
*	\note	������ ������: 2020-09-27
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
*	\brief	���帶ũ�� �ε����� ��ȯ�Ѵ�.
*	\note	������ ������: 2020-07-15
*
*	\param	Name[in]	���帶ũ �̸�
*
*	\return	�ش� ���帶ũ�� �ε����� ��ȯ�Ѵ�.
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
*	\brief	���� �׸��� �ε����� ��ȯ�Ѵ�.
*	\note	������ ������: 2020-07-15
*
*	\param	Name[in]	���� �׸��� �̸�
*
*	\return	�ش� �׸��� �ε����� ��ȯ�Ѵ�.
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
*	\brief	���� ���õ� ���帶ũ�� ��ǥ�� ��ȯ�Ѵ�.
*	\note	������ ������: 2020-09-24
*
*	\return	���õ� ���帶ũ�� ��ǥ�� ��ȯ�Ѵ�.
*/
EgPos EgSizer::GetSltLandmarkPos()
{
	if (m_Landmarks[m_SltLandmarkIdx] == NULL)
		return EgPos();
	else
		return m_Landmarks[m_SltLandmarkIdx]->m_Pos;
}

/*!
*	\brief	�ʱ�ȭ �Ѵ�.
*	\note	������ ������: 2020-07-20
*/
void EgSizer::Reset()
{
	m_PoseId = 0;
	m_pMesh = NULL;
	m_Landmarks.clear();
	m_Results.clear();
	m_Paths.clear();
}

