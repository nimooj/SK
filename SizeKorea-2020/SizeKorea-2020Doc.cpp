
// SizeKorea-2020Doc.cpp : implementation of the CSizeKorea2020Doc class
//

#include "pch.h"
#include "framework.h"

#include "MainFrm.h"

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SizeKorea-2020.h"
#endif

#include "SizeKorea-2020Doc.h"

#include <propkey.h>
 #ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										mj :: Global variables for EgSizer									//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SizeKorea_Pose2.h" // mj::LandmarkPose2, ResultPose2 위해 include
#define NEW_RESULT_NUM_POSE2 79

EgVertex* m_nLandmarkPose2[NEW_LANDMARK_NUM_POSE2];

REAL m_nResultPose2[NEW_RESULT_NUM_POSE2];
BOOL m_nResultPathViewPose2[NEW_RESULT_NUM_POSE2];

std::vector<EgPos> m_nResultPathPose2[NEW_RESULT_NUM_POSE2];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													mj :: END												//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


extern float RefAngle[100];
extern bool UpdateAngle;
extern EgSizer theSizer;
extern EgSlicer theSlicer;
extern std::vector<std::string> LandmarkPose1; // mj::SizeKorea_Pose1.cpp
extern std::vector<std::string> LandmarkPose2; // mj::SizeKorea_Pose2.h
extern std::vector<std::string> LandmarkPose3;
extern std::vector<std::string> LandmarkPose4;
extern std::vector<std::string> LandmarkPose5;


extern std::vector<std::string> LandmarkHelpPose1;
extern EgVertex* m_LandmarkHelpPose1[LANDMARK_HELP_NUM_POSE1];

// CSizeKorea2020Doc

IMPLEMENT_DYNCREATE(CSizeKorea2020Doc, CDocument)

BEGIN_MESSAGE_MAP(CSizeKorea2020Doc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CSizeKorea2020Doc::OnFileOpen)
	ON_COMMAND(ID_FIND_LANDMARK, &CSizeKorea2020Doc::OnFindLandmark)
	ON_COMMAND(ID_MEASURE, &CSizeKorea2020Doc::OnMeasure)
//	ON_COMMAND(ID_FILE_SELECTPOSE, &CSizeKorea2020Doc::OnFileSelectpose)
ON_COMMAND(ID_IMPORT_LANDMARK, &CSizeKorea2020Doc::OnImportLandmark)
ON_COMMAND(ID_EXPORT_LANDMARK, &CSizeKorea2020Doc::OnExportLandmark)
ON_COMMAND(ID_SET_GENDER, &CSizeKorea2020Doc::OnSetGender)
ON_COMMAND(ID_FILE_SAVE, &CSizeKorea2020Doc::OnFileSave)
ON_COMMAND(ID_MODEL_LANDMARK_SAVE, &CSizeKorea2020Doc::OnModelLandmarkSave)
ON_COMMAND(ID_SELECT_POSE, &CSizeKorea2020Doc::OnSelectPose)
ON_UPDATE_COMMAND_UI(ID_SET_GENDER, &CSizeKorea2020Doc::OnUpdateSetGender)
END_MESSAGE_MAP()


// CSizeKorea2020Doc construction/destruction

CSizeKorea2020Doc::CSizeKorea2020Doc() noexcept
{
	m_ScanPose = 1;
	m_ScanMesh = NULL;

	///////////////////////////////////////////////////////////////////////////////	
	m_Vertex = NULL;
	m_Color = NULL;
	m_Normal = NULL;
	m_FaceIdx = NULL;
	///////////////////////////////////////////////////////////////////////////////	
	m_vNum = 0;
	m_iNum = 0;
	///////////////////////////////////////////////////////////////////////////////
	m_LandmarkNumExport = 35;
	m_LandmarkHelpNumExport = 60;
	m_ResultSizeNumExport = 70;

	m_MinX = 10000.0f;
	m_MaxX = -10000.0f;
	m_MinY = 10000.0f;
	m_MaxY = -10000.0f;
	m_MinZ = 10000.0f;
	m_MaxZ = -10000.0f;
	m_MinXOrigin = 0.0f;
	m_MaxXOrigin = 0.0f;
	m_MinYOrigin = 0.0f;
	m_MaxYOrigin = 0.0f;
	m_MinZOrigin = 0.0f;
	m_MaxZOrigin = 0.0f;
	m_Height = 0.0f;
	m_Obesity = 0.0f;
	////////////////////////////////////////////////////////////////////////////
	m_bOpened = false;
	m_bSizeResult = false;
	m_bArrangedXY = false;
	m_bArrangedY = false;
	m_bArrangedX = false;
	m_bLandmarkFind = false;
	m_bWoman = true;
	m_bParam = false;
	////////////////////////////////////////////////////////////////////////////
	m_MarkIdx = -1;
	////////////////////////////////////////////////////////////////////////////
	int i = 0;
	// mj::Init pose1 results
	// m_ResultPathViewPose1[0] = FALSE;
	// for (i = 1; i < RESULT_NUM_POSE1; m_ResultPose1[i] = 0.0, m_ResultPathViewPose1[i] = TRUE, i++);

	// mj::Init pose2 results
	for (i = 0; i < RESULT_NUM_POSE2; m_ResultPose2[i] = 0.0f, m_ResultPose2PathView[i] = TRUE, i++);
	for (i = 0; i < 15; m_ResultPose2[i] = 0.0f, m_ResultPose2PathView[i] = FALSE, i++);

	// for (i = 0; i < RESULT_NUM_POSE2; m_ResultPose2[i] = 0.0f, m_ResultPose2PathView[i] = TRUE, i++);

	/*
	m_pPose2ResultDlg = NULL;
	m_pPose2ResultDlg = NULL;

	m_pPose2ResultPathDlg = NULL;
	m_pPose2ResultPathDlg = NULL;
	*/

	m_bReplaceResult_Underbust = false;

	m_pAlignedPointsV_X = NULL;
	m_pAlignedPointsV_Y = NULL;

	m_pSlicesPtList = NULL;
	m_pSliceAnglePtList = NULL;


	m_nCount_X = 0;
	m_nCount_Y = 0;
	m_nCount_Z = 0;
	m_HInterval = 2;

	m_Error = ASERR_OK;

	InitVariable();
}

CSizeKorea2020Doc::~CSizeKorea2020Doc()
{
	if (m_ScanMesh != NULL)
		delete m_ScanMesh;

	if (m_Vertex != NULL)
		delete[] m_Vertex;

	if (m_Color != NULL)
		delete[] m_Color;

	if (m_Normal != NULL)
		delete[] m_Normal;

	if (m_FaceIdx != NULL)
		delete[] m_FaceIdx;
}

BOOL CSizeKorea2020Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

CSizeKorea2020View* CSizeKorea2020Doc::GetView() 
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	return (CSizeKorea2020View*) pFrame->GetActiveView();
}

void CSizeKorea2020Doc::OnMeasurePose1() {
	theSizer.Measure();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame->m_pPose1ResultDlg == NULL)
	{
		pFrame->m_pPose1ResultDlg = new CPose1ResultDialog();
		pFrame->m_pPose1ResultDlg->Create(IDD_POSE1_RESULT_DIALOG);
	}
	CString str[RESULT_NUM_POSE1];
	for (int i = 0; i < RESULT_NUM_POSE1; ++i)
		str[i].Format("%.2lf ", theSizer.m_Results[i]);

	CPose1ResultDialog *pDlg = pFrame->m_pPose1ResultDlg;
	pDlg->InitDialog();

	pDlg->m_Pose1Result1 = str[0];
	pDlg->m_Pose1Result2 = str[1];
	pDlg->m_Pose1Result3 = str[2];
	pDlg->m_Pose1Result4 = str[3];
	pDlg->m_Pose1Result5 = str[4];
	pDlg->m_Pose1Result6 = str[5];
	pDlg->m_Pose1Result7 = str[6];
	pDlg->m_Pose1Result8 = str[7];
	pDlg->m_Pose1Result9 = str[8];
	pDlg->m_Pose1Result10 = str[9];
	pDlg->m_Pose1Result11= str[10];
	pDlg->m_Pose1Result12 = str[11];
	pDlg->m_Pose1Result13 = str[12];
	pDlg->m_Pose1Result14 = str[13];
	pDlg->m_Pose1Result15 = str[14];
	pDlg->m_Pose1Result16 = str[15];
	pDlg->m_Pose1Result17 = str[16];
	pDlg->m_Pose1Result18 = str[17];
	pDlg->m_Pose1Result19 = str[18];
	pDlg->m_Pose1Result20 = str[19];
	pDlg->m_Pose1Result21 = str[20];
	pDlg->m_Pose1Result22 = str[21];
	pDlg->m_Pose1Result23 = str[22];
	pDlg->m_Pose1Result24 = str[23];
	pDlg->m_Pose1Result25 = str[24];
	pDlg->m_Pose1Result26 = str[25];
	pDlg->m_Pose1Result27 = str[26];
	pDlg->m_Pose1Result28 = str[27];
	pDlg->m_Pose1Result29 = str[28];
	pDlg->m_Pose1Result30 = str[29];
	pDlg->m_Pose1Result31 = str[30];
	pDlg->m_Pose1Result32 = str[31];
	pDlg->m_Pose1Result33 = str[32];
	pDlg->m_Pose1Result34 = str[33];
	pDlg->m_Pose1Result35 = str[34];
	pDlg->m_Pose1Result36 = str[35];
	pDlg->m_Pose1Result37 = str[36];
	pDlg->m_Pose1Result38 = str[37];
	pDlg->m_Pose1Result39 = str[38];
	pDlg->m_Pose1Result40 = str[39];
	pDlg->m_Pose1Result41 = str[40];
	pDlg->m_Pose1Result42 = str[41];
	pDlg->m_Pose1Result43 = str[42];
	pDlg->m_Pose1Result44 = str[43];
	pDlg->m_Pose1Result45 = str[44];
	pDlg->m_Pose1Result46 = str[45];
	pDlg->m_Pose1Result47 = str[46];
	pDlg->m_Pose1Result48 = str[47];
	pDlg->m_Pose1Result49 = str[48];
	pDlg->m_Pose1Result50 = str[49];
	pDlg->m_Pose1Result51 = str[50];
	pDlg->m_Pose1Result52 = str[51];
	pDlg->m_Pose1Result53 = str[52];
	pDlg->m_Pose1Result54 = str[53];
	pDlg->m_Pose1Result55 = str[54];
	pDlg->m_Pose1Result56 = str[55];
	pDlg->m_Pose1Result57 = str[56];
	pDlg->m_Pose1Result58 = str[57];
	pDlg->m_Pose1Result59 = str[58];
	pDlg->m_Pose1Result60 = str[59];
	pDlg->m_Pose1Result61 = str[60];
	pDlg->m_Pose1Result62 = str[61];
	pDlg->m_Pose1Result63 = str[62];
	pDlg->m_Pose1Result64 = str[63];
	pDlg->m_Pose1Result65 = str[64];

	pDlg->UpdateData(FALSE);

}


float GetCirc(std::vector<EgPos> vecs) {
	float distance = 0;
	for (int i = 0; i < vecs.size() - 1; i++) {
		distance += dist(vecs[i], vecs[i + 1]);
	}

	distance += dist(vecs[vecs.size() - 1], vecs[0]);

	return distance;
}

// mj::Descending order
bool sortPointsByX(EgPos &a, EgPos &b) {
	return a.m_Coords[0] > b.m_Coords[0];
}

bool sortPointsByY(EgPos &a, EgPos &b) {
	return a.m_Coords[1] > b.m_Coords[1];
}

void CSizeKorea2020Doc::OnMeasurePose2() {
	// mj::Clear global variables
	for (int i = 0; i < NEW_RESULT_NUM_POSE2; i++) {
		m_nResultPose2[i] = -1;
		m_nResultPathPose2[i].clear();
	}

	// 측정항목 1~20 : 높이 항목들  73 배꼽높이
	::GetResultPose2_1_20(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2);

	GVector3f a = m_LandMarkPose2[19];
	a.Z = m_LandMarkPose2[24].Z;
	a.X = m_LandMarkPose2[24].X;
	m_ResultPathPose2[18].Insert(m_LandMarkPose2[24]);
	m_ResultPathPose2[18].Insert(a);


	a = m_LandMarkPose2[6];
	a.Z = m_LandMarkPose2[24].Z;

	GVector3f b = m_LandMarkPose2[24];
	b.X = a.X;
	m_ResultPathPose2[19].Insert(b);
	m_ResultPathPose2[19].Insert(a);


	// 측정항목 21 : 목둘레
	::GetResultPose2_21(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[21]);

	// 22 : 목밑둘레,  23:목뒤밑둘레,  24:목앞밑둘레 
	// 68 : 목(밑)너비   89 : 목밑 curve   /보조점: 목앞점좌우점(46, 47)
	::GetResultPose2_22_68(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, &m_ResultPathPose2[22], &m_ResultPathPose2[68], &m_ResultPathPose2[89]);
	//::GetResultPose2_22_68_a( m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, &m_ResultPathPose2[22], &m_ResultPathPose2[68], &m_ResultPathPose2[89]);


	// 목밀둘레의 오른쪽 최단점 구하기 (=보조점 51 -목옆젖꼭지길이 측정시 사용)
	// 참고둘레: 목밑둘레
	FindNeckCircRight();

	// 측정항목 25 : 겨드랑둘레 - Right      /보조점: 오른쪽 겨드랑앞접힘점(42),   오른쪽 겨드랑뒤접힘점(43).
	::GetResultPose2_25_a(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[25]);


	//측정항목 88 : 겨드랑둘레  -  Left
	::GetResultPose2_88(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[88]);


	// 측정항목 26_70_79 : 가슴 둘레, 가슴 너비, 가슴 두께.
	::GetResultPose2_26_70_79(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[26], &m_ResultPathPose2[70], &m_ResultPathPose2[79]);


	if (m_bArrangedXY) { //자동 랜드마크를 했을때

		//27~36 수평둘레들,  68~76 너비들
		Measure_Circum_Arranged();

		//54 진동깊이, 56 등길이
		GetResultPose2_54_56();
	}
	else { 				//랜드마크를 불러들여서 높이별 Arrange가 안되어 있을때.

		//27~36 수평둘레들
		Measure_Circum_UnArranged();

		// 56 : 등길이.
		::GetResultPose2_56(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[56]);

		// 54 : 진동 깊이.
		::GetResultPose2_54(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[54]);
	}

	// 57 배꼽수준등길이
	m_ResultPose2[57] = m_ResultPose2[56] + (m_LandMarkPose2[21].Y - m_LandMarkPose2[35].Y);


	// 측정항목 37 : 발목최대둘레
	::GetResultPose2_37(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[37]);

	// 측정항목 38 : 편위팔둘레
	::GetResultPose2_38(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[38]);


	// 39 : 편팔꿈치둘레,  
	// 64 : 팔안쪽길이
	::GetResultPose2_39_64(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[39], &m_ResultPathPose2[64]);


	// 측정항목 40 : 손목둘레
	::GetResultPose2_40(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[40]);


	// 측정항목 41 : 몸통 세로 둘레.
	::GetResultPose2_41(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[41]);

	// 측정항목 42 : 앞중심 길이.
	::GetResultPose2_42(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[42]);

	// 측정항목 43 : 배꼽수준앞중심 길이.
	m_ResultPose2[43] = m_ResultPose2[42] + (m_LandMarkPose2[21].Y - m_LandMarkPose2[35].Y);

	// 44 : 어깨 길이.
	::GetResultPose2_44(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[44]);

	// 45 : 목뒤어깨가쪽길이
	// 46 : 어깨가쪽 사이 길이-목뒤.
	// 69 : 어깨 너비.
	::GetResultPose2_45_46_69(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[45], &m_ResultPathPose2[46], &m_ResultPathPose2[69]);

	// 47 : 겨드랑앞벽사이길이 앞품.
	::GetResultPose2_47(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[47]);

	// 48 : 겨드랑뒤벽사이길이 뒤품.
	::GetResultPose2_48(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[48]);

	// 49 : 목옆젖꼭지 길이.  /보조점: 목둘레 최우점(51)
	::GetResultPose2_49(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, &m_ResultPathPose2[49]);


	// 50 : 목옆-젖꼭지 허리둘레선길이.
	// 51 :      젖꼭지 허리둘레선길이.
	::GetResultPose2_50(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[50]);

	//52 목뒤젖꼭지길이 =   목뒤밑둘레(23)/2 +목옆젖꼭지 길이(49)
	m_ResultPose2[52] = m_ResultPose2[23] * 0.5f + m_ResultPose2[49];

	//53 목뒤젖꼭지허리둘레선길이
	m_ResultPose2[53] = m_ResultPose2[52] + m_ResultPose2[51];

	//54,56,57 위에서 측정했음

	//55 목옆뒤허리둘레선길이(뒤길이)
	::GetResultPose2_55(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, &m_ResultPathPose2[55]);

	// 측정항목 58 : 엉덩이옆길이,  
	::GetResultPose2_58(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, &m_ResultPathPose2[58]);

	// 59 : 샅위앞뒤길이.  /보조점: 샅앞점(16)
	::GetResultPose2_59(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, &m_ResultPathPose2[59]);

	// 60 :  배꼽수준 샅위앞뒤길이
	m_ResultPose2[60] = m_ResultPose2[59] - 2.0*(m_LandMarkPose2[21].Y - m_LandMarkPose2[35].Y);


	// 61 : 허리옆가쪽복사길이(=기능적다리길이=바지길이)
	// 62 : 다리길이
	::GetResultPose2_61_62(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[61], &m_ResultPathPose2[62]);


	// 64는 위에서 했음
	// 63: 팔길이, 
	// 66: 위팔길이, 
	// 65: 목뒤손목안쪽길이
	::GetResultPose2_63_66_65(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[63], &m_ResultPathPose2[66], &m_ResultPathPose2[65]);

	//팔길이   - 머리가 포함되는 에러가 발생 ㅠㅠㅠ
	//Measure_LengthSurface(m_LandMarkPose2[7], m_LandMarkPose2[32], m_LandMarkPose2[31], 3, 0, 0, m_ResultPose2, 63, &m_ResultPathPose2[63]);

	//67 : 아래팔길이
	m_ResultPose2[67] = m_ResultPose2[63] - m_ResultPose2[66];

	//68~76 너비들

	// 측정항목 78 : 겨드랑 두께.  /보조점 : 오른쪽겨드랑앞돌출(Help 54), 오른쪽겨드랑이뒤돌출(Help 55)
	::GetResultPose2_78(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[78]);

	// 79~85  두께들

	// 측정항목 86 : 오른쪽 어깨기울기
	::GetResultPose2_86(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[86], &m_ResultPathPose2[87]);


	// 측정항목 90 : 겨드랑둘레(구버젼)
	//::GetResultPose2_90( m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[90] );

	m_bSizeResult = true;
	UpdateOldLandMark(1);
	UpdateAngle = false;


	// mj::m_nResultPose2에 m_ResultPose2를 이관
	GVector3f v;
	// 목둘레
	m_nResultPose2[POSE2_MEASURE_NECK_THYROID_CIRC] = m_ResultPose2[21];
	for (int i = 0; i < m_ResultPathPose2[21].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[21][i];
		m_nResultPathPose2[POSE2_MEASURE_NECK_THYROID_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}
	v = m_ResultPathPose2[21][0];
	m_nResultPathPose2[POSE2_MEASURE_NECK_THYROID_CIRC].push_back(EgPos(v.X, v.Y, v.Z));


	// 목밑둘레
	m_nResultPose2[POSE2_MEASURE_NECK_BACK_CIRC] = m_ResultPose2[22];
	for (int i = 0; i < m_ResultPathPose2[22].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[22][i];
		m_nResultPathPose2[POSE2_MEASURE_NECK_BACK_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}
	v = m_ResultPathPose2[22][0];
	m_nResultPathPose2[POSE2_MEASURE_NECK_BACK_CIRC].push_back(EgPos(v.X, v.Y, v.Z));


	m_nResultPose2[POSE2_MEASURE_ARMPIT_SHOULDERSIDE_CIRC] = m_ResultPose2[25];
	for (int i = 0; i < m_ResultPathPose2[25].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[25][i];
		m_nResultPathPose2[POSE2_MEASURE_ARMPIT_SHOULDERSIDE_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}
	v = m_ResultPathPose2[25][0];
	m_nResultPathPose2[POSE2_MEASURE_ARMPIT_SHOULDERSIDE_CIRC].push_back(EgPos(v.X, v.Y, v.Z));

	// 가슴둘레(convex)
	m_nResultPose2[POSE2_MEASURE_BUST_CIRC] = m_ResultPose2[26];
	for (int i = 0; i < m_ResultPathPose2[26].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[26][i];
		m_nResultPathPose2[POSE2_MEASURE_BUST_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}
	v = m_ResultPathPose2[26][0];
	m_nResultPathPose2[POSE2_MEASURE_BUST_CIRC].push_back(EgPos(v.X, v.Y, v.Z));

	// 젖가슴둘레(convex)
	m_nResultPose2[POSE2_MEASURE_CONVEX_BP_CIRC] = m_ResultPose2[27];
	for (int i = 0; i < m_ResultPathPose2[27].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[27][i];
		m_nResultPathPose2[POSE2_MEASURE_CONVEX_BP_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}
	v = m_ResultPathPose2[27][0];
	m_nResultPathPose2[POSE2_MEASURE_CONVEX_BP_CIRC].push_back(EgPos(v.X, v.Y, v.Z));

	//젖가슴아래둘레
	m_nResultPose2[POSE2_MEASURE_UNDERBUST_CIRC] = m_ResultPose2[28];
	for (int i = 0; i < m_ResultPathPose2[28].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[28][i];
		m_nResultPathPose2[POSE2_MEASURE_UNDERBUST_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_WAIST_CIRC] = m_ResultPose2[29];
	for (int i = 0; i < m_ResultPathPose2[29].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[29][i];
		m_nResultPathPose2[POSE2_MEASURE_WAIST_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_STOMACH_CIRC] = m_ResultPose2[30];
	for (int i = 0; i < m_ResultPathPose2[30].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[30][i];
		m_nResultPathPose2[POSE2_MEASURE_STOMACH_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_NAVEL_CIRC] = m_ResultPose2[31];
	for (int i = 0; i < m_ResultPathPose2[31].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[31][i];
		m_nResultPathPose2[POSE2_MEASURE_NAVEL_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_HIP_CIRC] = m_ResultPose2[32];
	for (int i = 0; i < m_ResultPathPose2[32].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[32][i];
		m_nResultPathPose2[POSE2_MEASURE_HIP_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_UNDERHIP_CIRC] = m_ResultPose2[33];
	for (int i = 0; i < m_ResultPathPose2[33].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[33][i];
		m_nResultPathPose2[POSE2_MEASURE_UNDERHIP_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}
	v = m_ResultPathPose2[33][0];
	m_nResultPathPose2[POSE2_MEASURE_UNDERHIP_CIRC].push_back(EgPos(v.X, v.Y, v.Z));

	m_nResultPose2[POSE2_MEASURE_KNEE_CIRC] = m_ResultPose2[34];
	for (int i = 0; i < m_ResultPathPose2[34].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[34][i];
		m_nResultPathPose2[POSE2_MEASURE_KNEE_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_CALF_EXTRUDE_CIRC] = m_ResultPose2[35];
	for (int i = 0; i < m_ResultPathPose2[35].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[35][i];
		m_nResultPathPose2[POSE2_MEASURE_CALF_EXTRUDE_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_UNDERCALF_CIRC] = m_ResultPose2[36];
	for (int i = 0; i < m_ResultPathPose2[36].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[36][i];
		m_nResultPathPose2[POSE2_MEASURE_UNDERCALF_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}

	EgPlane anklePln(m_nLandmarkPose2[POSE2_LANDMARK_ANKLE]->m_Pos, m_nLandmarkPose2[POSE2_LANDMARK_ANKLE_INSIDE]->m_Pos, EgPos(m_nLandmarkPose2[POSE2_LANDMARK_ANKLE]->m_Pos[0], m_nLandmarkPose2[POSE2_LANDMARK_ANKLE]->m_Pos[1], m_nLandmarkPose2[POSE2_LANDMARK_ANKLE]->m_Pos[2] + 10.0f));
	m_nResultPose2[POSE2_MEASURE_MAX_ANKLE_CIRC] = SK_Planar_Perimeter(
		m_nLandmarkPose2[POSE2_LANDMARK_ANKLE],
		&anklePln,
		m_nResultPathPose2[POSE2_MEASURE_MAX_ANKLE_CIRC],
		false
	);

	// 편위팔둘레
	m_nResultPose2[POSE2_MEASURE_UPPERARM_CIRC] = m_ResultPose2[38];
	for (int i = 0; i < m_ResultPathPose2[38].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[38][i];
		m_nResultPathPose2[POSE2_MEASURE_UPPERARM_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}
	v = m_ResultPathPose2[38][0];
	m_nResultPathPose2[POSE2_MEASURE_UPPERARM_CIRC].push_back(EgPos(v.X, v.Y, v.Z));


	// 편팔꿈치둘레
	EgVec3 se = m_nLandmarkPose2[POSE2_LANDMARK_SHOULDER_R]->m_Pos - m_nLandmarkPose2[POSE2_LANDMARK_ELBOW]->m_Pos;
	EgPlane elbowPln(se.Normalize(), m_nLandmarkPose2[POSE2_LANDMARK_ELBOW]->m_Pos);
	m_nResultPose2[POSE2_MEASURE_ELBOW_CIRC] = SK_Planar_Perimeter(
		m_nLandmarkPose2[POSE2_LANDMARK_ELBOW],
		&elbowPln,
		m_nResultPathPose2[POSE2_MEASURE_ELBOW_CIRC],
		false
	);


	m_nResultPose2[POSE2_MEASURE_WRIST_CIRC] = m_ResultPose2[40];
	for (int i = 0; i < m_ResultPathPose2[40].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[40][i];
		m_nResultPathPose2[POSE2_MEASURE_WRIST_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}


	m_nResultPose2[POSE2_MEASURE_TORSO_CIRC] = m_ResultPose2[41];
	for (int i = 0; i < m_ResultPathPose2[41].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[41][i];
		m_nResultPathPose2[POSE2_MEASURE_TORSO_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_HIP_SIDE_LENGTH] = m_ResultPose2[58];
	for (int i = 0; i < m_ResultPathPose2[58].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[58][i];
		m_nResultPathPose2[POSE2_MEASURE_HIP_SIDE_LENGTH].push_back(EgPos(v.X, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_CROTCH_LENGTH] = m_ResultPose2[59];
	for (int i = 0; i < m_ResultPathPose2[59].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[59][i];
		m_nResultPathPose2[POSE2_MEASURE_CROTCH_LENGTH].push_back(EgPos(v.X, v.Y, v.Z));
	}

	// 샅앞뒤길이에서 배꼽점 이상은 넣지 않는다
	m_nResultPose2[POSE2_MEASURE_NAVEL_CROTCH_LENGTH] = m_ResultPose2[59] - 2 * (m_nLandmarkPose2[POSE2_LANDMARK_WAIST]->m_Pos[1] - m_nLandmarkPose2[POSE2_LANDMARK_NAVEL]->m_Pos[1]);
	for (int i = 0; i < m_ResultPathPose2[59].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[59][i];
		if (v.Y <= m_nLandmarkPose2[POSE2_LANDMARK_NAVEL]->m_Pos[1])
			m_nResultPathPose2[POSE2_MEASURE_NAVEL_CROTCH_LENGTH].push_back(EgPos(v.X, v.Y, v.Z));
	}

	// 허리옆가쪽복사길이
	// 허리옆점에서 가쪽복사점까지의 체표길이
	m_nResultPose2[POSE2_MEASURE_WAIST_ANKLE_LENGTH] = SK_Shortest_Distance(
		m_nLandmarkPose2[POSE2_LANDMARK_WAIST],
		m_nLandmarkPose2[POSE2_LANDMARK_ANKLE],
		m_nResultPathPose2[POSE2_MEASURE_WAIST_ANKLE_LENGTH]
	);

	// 다리가쪽길이
	// 허리둘레수준에서 엉덩이돌출수준까지 신체의 측면에서 체표를 따라 측정하고 이어서 바닥면까지의 수직길이
	// 허리둘레의 minX, 엉덩이돌출둘레의 minX
	EgPos waistMinX = EgPos(INFINITY, 0, 0);
	EgPos hipMinX = EgPos(INFINITY, 0, 0);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_WAIST_CIRC]) {
		if (pos.m_Coords[0] < waistMinX.m_Coords[0]) {
			waistMinX = pos;
		}
	}
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_HIP_CIRC]) {
		if (pos.m_Coords[0] < hipMinX.m_Coords[0]) {
			hipMinX = pos;
		}
	}
	m_nResultPose2[POSE2_MEASURE_LEG_LENGTH] = SK_Shortest_Distance(
		FindClosestVert(m_ScanMesh, waistMinX),
		FindClosestVert(m_ScanMesh, hipMinX),
		m_nResultPathPose2[POSE2_MEASURE_LEG_LENGTH]
	);
	std::sort(m_nResultPathPose2[POSE2_MEASURE_LEG_LENGTH].begin(), m_nResultPathPose2[POSE2_MEASURE_LEG_LENGTH].end(), sortPointsByY);
	m_nResultPose2[POSE2_MEASURE_LEG_LENGTH] += hipMinX.m_Coords[1];

	m_nResultPathPose2[POSE2_MEASURE_LEG_LENGTH].push_back(EgPos(hipMinX.m_Coords[0], 0, hipMinX.m_Coords[2]));


	m_nResultPose2[POSE2_MEASURE_ARM_INSIDE_LENGTH] = m_ResultPose2[64];
	for (int i = 0; i < m_ResultPathPose2[64].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[64][i];
		m_nResultPathPose2[POSE2_MEASURE_ARM_INSIDE_LENGTH].push_back(EgPos(v.X, v.Y, v.Z));
	}

	// 목너비 -> 목밑너비(20. 9. 23)
	m_nResultPose2[POSE2_MEASURE_NECK_BACK_WIDTH] = m_ResultPose2[68];
	for (int i = 0; i < m_ResultPathPose2[68].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[68][i];
		m_nResultPathPose2[POSE2_MEASURE_NECK_BACK_WIDTH].push_back(EgPos(v.X, v.Y, v.Z + 300.0f));
	}

	m_nResultPose2[POSE2_MEASURE_BUST_WIDTH] = m_ResultPose2[70];
	for (int i = 0; i < m_ResultPathPose2[70].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[70][i];
		m_nResultPathPose2[POSE2_MEASURE_BUST_WIDTH].push_back(EgPos(v.X, v.Y, v.Z + 500.0f));
	}

	m_nResultPose2[POSE2_MEASURE_BP_WIDTH] = m_ResultPose2[71];
	for (int i = 0; i < m_ResultPathPose2[71].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[71][i];
		m_nResultPathPose2[POSE2_MEASURE_BP_WIDTH].push_back(EgPos(v.X, v.Y, v.Z + 500.0f));
	}

	m_nResultPose2[POSE2_MEASURE_UNDERBUST_WIDTH] = m_ResultPose2[72];
	for (int i = 0; i < m_ResultPathPose2[72].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[72][i];
		m_nResultPathPose2[POSE2_MEASURE_UNDERBUST_WIDTH].push_back(EgPos(v.X, v.Y, v.Z + 500.0f));
	}

	m_nResultPose2[POSE2_MEASURE_WAIST_WIDTH] = ABS(m_nLandmarkPose2[POSE2_LANDMARK_WAIST]->m_Pos[0] - m_nLandmarkPose2[POSE2_LANDMARK_WAIST_L]->m_Pos[0]);
	m_nResultPathPose2[POSE2_MEASURE_WAIST_WIDTH].push_back(EgPos(m_nLandmarkPose2[POSE2_LANDMARK_WAIST]->m_Pos[0], m_nLandmarkPose2[POSE2_LANDMARK_WAIST]->m_Pos[1], m_nLandmarkPose2[POSE2_LANDMARK_WAIST]->m_Pos[2] + 500.0f));
	// z수준 통일
	m_nResultPathPose2[POSE2_MEASURE_WAIST_WIDTH].push_back(EgPos(m_nLandmarkPose2[POSE2_LANDMARK_WAIST_L]->m_Pos[0], m_nLandmarkPose2[POSE2_LANDMARK_WAIST_L]->m_Pos[1], m_nLandmarkPose2[POSE2_LANDMARK_WAIST]->m_Pos[2] + 500.0f));

	m_nResultPose2[POSE2_MEASURE_STOMACH_WIDTH] = m_ResultPose2[74];
	for (int i = 0; i < m_ResultPathPose2[74].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[74][i];
		m_nResultPathPose2[POSE2_MEASURE_STOMACH_WIDTH].push_back(EgPos(v.X, v.Y, v.Z + 500.0f));
	}

	m_nResultPose2[POSE2_MEASURE_NAVEL_WAIST_WIDTH] = m_ResultPose2[75];
	for (int i = 0; i < m_ResultPathPose2[75].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[75][i];
		m_nResultPathPose2[POSE2_MEASURE_NAVEL_WAIST_WIDTH].push_back(EgPos(v.X, v.Y, v.Z + 500.0f));
	}

	m_nResultPose2[POSE2_MEASURE_HIP_WIDTH] = m_ResultPose2[76];
	for (int i = 0; i < m_ResultPathPose2[76].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[76][i];
		m_nResultPathPose2[POSE2_MEASURE_HIP_WIDTH].push_back(EgPos(v.X, v.Y, v.Z - 500.0f));
	}

	m_nResultPose2[POSE2_MEASURE_ARMPIT_DEPTH] = m_ResultPose2[78];
	for (int i = 0; i < m_ResultPathPose2[78].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[78][i];
		m_nResultPathPose2[POSE2_MEASURE_ARMPIT_DEPTH].push_back(EgPos(v.X - 500.0f, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_BUST_DEPTH] = m_ResultPose2[79];
	for (int i = 0; i < m_ResultPathPose2[79].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[79][i];
		m_nResultPathPose2[POSE2_MEASURE_BUST_DEPTH].push_back(EgPos(m_MinX - 50.0f, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_BP_DEPTH] = m_ResultPose2[80];
	for (int i = 0; i < m_ResultPathPose2[80].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[80][i];
		m_nResultPathPose2[POSE2_MEASURE_BP_DEPTH].push_back(EgPos(m_MinX - 50.0f, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_UNDERBUST_DEPTH] = m_ResultPose2[81];
	for (int i = 0; i < m_ResultPathPose2[81].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[81][i];
		m_nResultPathPose2[POSE2_MEASURE_UNDERBUST_DEPTH].push_back(EgPos(m_MinX - 50.0f, v.Y, v.Z));
	}

	theSlicer.SetCutPlane(EgPlane(EgVec3(0, 1, 0), m_nLandmarkPose2[POSE2_LANDMARK_WAIST]->m_Pos));
	theSlicer.Slice(m_ScanMesh);
	theSlicer.SortByLength(true);
	EgPos maxZ = EgPos(0, 0, -INFINITY);
	EgPos minZ = EgPos(0, 0, INFINITY);

	for (EgPos pos : theSlicer.m_CutLines[0]) {
		if (pos.m_Coords[2] < minZ.m_Coords[2])
			minZ = pos;
		if (pos.m_Coords[2] > maxZ.m_Coords[2])
			maxZ = pos;
	}

	m_nResultPose2[POSE2_MEASURE_WAIST_DEPTH] = ABS(maxZ.m_Coords[2] - minZ.m_Coords[2]);

	m_nResultPathPose2[POSE2_MEASURE_WAIST_DEPTH].push_back(EgPos(m_MinX - 50.0f, minZ.m_Coords[1], minZ.m_Coords[2]));
	m_nResultPathPose2[POSE2_MEASURE_WAIST_DEPTH].push_back(EgPos(m_MinX - 50.0f, maxZ.m_Coords[1], maxZ.m_Coords[2]));



	m_nResultPose2[POSE2_MEASURE_STOMACH_DEPTH] = m_ResultPose2[83];
	for (int i = 0; i < m_ResultPathPose2[83].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[83][i];
		m_nResultPathPose2[POSE2_MEASURE_STOMACH_DEPTH].push_back(EgPos(m_MinX - 50.0f, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_NAVEL_WAIST_DEPTH] = m_ResultPose2[84];
	for (int i = 0; i < m_ResultPathPose2[84].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[84][i];
		m_nResultPathPose2[POSE2_MEASURE_NAVEL_WAIST_DEPTH].push_back(EgPos(m_MinX - 50.0f, v.Y, v.Z));
	}

	m_nResultPose2[POSE2_MEASURE_HIP_DEPTH] = m_ResultPose2[85];
	for (int i = 0; i < m_ResultPathPose2[85].m_nCount; i++) {
		GVector3f v = m_ResultPathPose2[85][i];
		m_nResultPathPose2[POSE2_MEASURE_HIP_DEPTH].push_back(EgPos(m_MinX - 50.0f, v.Y, v.Z));
	}


	///////////////////////////////////////////////////////////////////////////////////////
	//									 2020. 9. 3										//
	//										ERROR										//
	//////////////////////////////////////////////////////////////////////////////////////


	// ToDo::자세2는 기존방법으로(20. 9. 3)


	EgSlicer theSlicer;


	//////// 미드리프둘레
	// 미드리프 수준의 평면과 만나는 교차점들의 집합

	// 수직 절단 평면 (팔 부위 제외) : 겨드랑점 ~ (샅점 + 손목가쪽점)/2을 지나는 평면
	EgPos Armpit_R = m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_R]->m_Pos;
	EgPos Armpit_L = m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_L]->m_Pos;
	EgPos Crotch = m_nLandmarkPose2[POSE2_LANDMARK_CROTCH]->m_Pos;
	EgPos Wrist_R = m_nLandmarkPose2[POSE2_LANDMARK_WRIST]->m_Pos;

	EgPos mid_R = Wrist_R;
	mid_R.m_Coords[0] = (Crotch.m_Coords[0] + Wrist_R.m_Coords[0]) / 2;

	EgPos tmp_R = mid_R;
	tmp_R.m_Coords[2] += 10.0f;

	// 왼쪽 손목점은 오른쪽 손목점을 mirror
	EgPos Wrist_L = m_nLandmarkPose2[POSE2_LANDMARK_WRIST]->m_Pos;
	float centerX = (m_MinX + m_MaxX) / 2;
	Wrist_L.m_Coords[0] = centerX + abs(centerX - Wrist_R.m_Coords[0]);

	EgPos mid_L = Wrist_L;
	mid_L.m_Coords[0] = (Crotch.m_Coords[0] + Wrist_L.m_Coords[0]) / 2;

	EgPos tmp_L = mid_L;
	tmp_L.m_Coords[2] += 10.0f;


	// 반시계방향으로 normal이 몸통 쪽으로 향하도록
	EgPlane pln_R(Armpit_R, tmp_R, mid_R);
	EgPlane pln_L(Armpit_L, mid_L, tmp_L);

	EgPlane pln = EgPlane(EgVec3(0, 1, 0), m_nLandmarkPose2[POSE2_LANDMARK_MIDRIFF]->m_Pos);
	std::vector<EgPos> crossList;
	for (EgFace *face : m_ScanMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		if (pln.IsAbovePlane(p0) && pln.IsAbovePlane(p1) && pln.IsAbovePlane(p2))
			continue;
		if (pln.IsBelowPlane(p0) && pln.IsBelowPlane(p1) && pln.IsBelowPlane(p2))
			continue;

		if (pln_R.IsAbovePlane(p0) && pln_R.IsAbovePlane(p1) && pln_R.IsAbovePlane(p2)
			&& pln_L.IsAbovePlane(p0) && pln_L.IsAbovePlane(p1) && pln_L.IsAbovePlane(p2)) {
			if (dist(pln, p0) * dist(pln, p1) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p1, pln);

				crossList.push_back(crossPt0);
				crossList.push_back(crossPt1);
			}
			if (dist(pln, p0) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p2, pln);

				crossList.push_back(crossPt0);
				crossList.push_back(crossPt1);
			}
			if (dist(pln, p1) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p1, pln),
					crossPt1 = proj(p2, pln);

				crossList.push_back(crossPt0);
				crossList.push_back(crossPt1);
			}
		}
	}

	EgConvexhull cvxMidriff(crossList);
	cvxMidriff.MakeConvexHullXZ();
	cvxMidriff.ArrangeHullPtXZ();
	std::vector<EgPos> convexList = cvxMidriff.m_HullPtList;
	convexList.push_back(convexList[0]);

	m_nResultPose2[POSE2_MEASURE_MIDRIFF_CIRC] = GetCirc(convexList);

	m_nResultPathPose2[POSE2_MEASURE_MIDRIFF_CIRC].insert(
		m_nResultPathPose2[POSE2_MEASURE_MIDRIFF_CIRC].end(),
		convexList.begin(),
		convexList.end()
	);


	//////// Upper-hip 둘레
	pln = EgPlane(EgVec3(0, 1, 0), m_nLandmarkPose2[POSE2_LANDMARK_UPPERHIP]->m_Pos);
	crossList.clear();
	convexList.clear();
	for (EgFace *face : m_ScanMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		if (pln.IsAbovePlane(p0) && pln.IsAbovePlane(p1) && pln.IsAbovePlane(p2))
			continue;
		if (pln.IsBelowPlane(p0) && pln.IsBelowPlane(p1) && pln.IsBelowPlane(p2))
			continue;

		if (pln_R.IsAbovePlane(p0) && pln_R.IsAbovePlane(p1) && pln_R.IsAbovePlane(p2)
			&& pln_L.IsAbovePlane(p0) && pln_L.IsAbovePlane(p1) && pln_L.IsAbovePlane(p2)) {
			if (dist(pln, p0) * dist(pln, p1) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p1, pln);
				crossList.push_back(crossPt0);
				crossList.push_back(crossPt1);
			}
			if (dist(pln, p0) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p2, pln);
				crossList.push_back(crossPt0);
				crossList.push_back(crossPt1);
			}
			if (dist(pln, p1) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p1, pln),
					crossPt1 = proj(p2, pln);
				crossList.push_back(crossPt0);
				crossList.push_back(crossPt1);
			}
		}
	}
	crossList.push_back(crossList[0]);

	EgConvexhull cvxUpperhip(crossList);
	cvxUpperhip.MakeConvexHullXZ();
	cvxUpperhip.ArrangeHullPtXZ();
	convexList = cvxUpperhip.m_HullPtList;
	convexList.push_back(convexList[0]);

	m_nResultPose2[POSE2_MEASURE_UPPERHIP_CIRC] = GetCirc(convexList);

	m_nResultPathPose2[POSE2_MEASURE_UPPERHIP_CIRC].insert(
		m_nResultPathPose2[POSE2_MEASURE_UPPERHIP_CIRC].end(),
		convexList.begin(),
		convexList.end()
	);


	//////// Top-hip 둘레
	pln = EgPlane(EgVec3(0, 1, 0), m_nLandmarkPose2[POSE2_LANDMARK_TOPHIP]->m_Pos);
	crossList.clear();
	for (EgFace *face : m_ScanMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		if (pln.IsAbovePlane(p0) && pln.IsAbovePlane(p1) && pln.IsAbovePlane(p2))
			continue;
		if (pln.IsBelowPlane(p0) && pln.IsBelowPlane(p1) && pln.IsBelowPlane(p2))
			continue;

		if (pln_R.IsAbovePlane(p0) && pln_R.IsAbovePlane(p1) && pln_R.IsAbovePlane(p2)
			&& pln_L.IsAbovePlane(p0) && pln_L.IsAbovePlane(p1) && pln_L.IsAbovePlane(p2)) {
			if (dist(pln, p0) * dist(pln, p1) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p1, pln);
				crossList.push_back(crossPt0);
				// crossList.push_back(crossPt1);
			}
			if (dist(pln, p0) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p2, pln);
				crossList.push_back(crossPt0);
				// crossList.push_back(crossPt1);
			}
			if (dist(pln, p1) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p1, pln),
					crossPt1 = proj(p2, pln);
				crossList.push_back(crossPt0);
				// crossList.push_back(crossPt1);
			}
		}
	}

	convexList.clear();
	EgConvexhull cvxTophip(crossList);
	cvxTophip.MakeConvexHullXZ();
	cvxTophip.ArrangeHullPtXZ();
	convexList = cvxTophip.m_HullPtList;
	convexList.push_back(convexList[0]);

	m_nResultPose2[POSE2_MEASURE_TOPHIP_CIRC] = GetCirc(convexList);

	m_nResultPathPose2[POSE2_MEASURE_TOPHIP_CIRC].insert(
		m_nResultPathPose2[POSE2_MEASURE_TOPHIP_CIRC].end(),
		convexList.begin(),
		convexList.end()
	);



	// 젖가슴둘레(체표를 따르는)
	pln = EgPlane(EgVec3(0, 1, 0), m_nLandmarkPose2[POSE2_LANDMARK_BP_R]->m_Pos);
	std::vector<EgPos> tmpBpCirc;
	SK_Planar_Perimeter(
		m_nLandmarkPose2[POSE2_LANDMARK_BP_R],
		&pln,
		tmpBpCirc,
		false
	);

	// 몸통이랑 팔이 붙어 스캔됐을 경우
	// 겨드랑점 바깥은 제외
	for (EgPos pos : tmpBpCirc) {
		if (pos.m_Coords[0] > m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_R]->m_Pos[0]
			&& pos.m_Coords[0] < m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_L]->m_Pos[0]) {
			m_nResultPathPose2[POSE2_MEASURE_BP_CIRC].push_back(pos);
		}
	}
	if (!m_nResultPathPose2[POSE2_MEASURE_BP_CIRC].empty()) {
		for (int i = 0; i < m_nResultPathPose2[POSE2_MEASURE_BP_CIRC].size() - 1; i++) {
			m_nResultPose2[POSE2_MEASURE_BP_CIRC] += dist(m_nResultPathPose2[POSE2_MEASURE_BP_CIRC][i], m_nResultPathPose2[POSE2_MEASURE_BP_CIRC][i + 1]);
		}
	}



	//////// 목밑뒤길이
	EgPlane BnecksidePlane(m_nLandmarkPose2[POSE2_LANDMARK_NECK_BACK]->m_Pos, m_nLandmarkPose2[POSE2_LANDMARK_NECK_SIDE_R]->m_Pos, m_nLandmarkPose2[POSE2_LANDMARK_NECK_SIDE_L]->m_Pos);

	// mj::return -1 (20. 10. 5)
	m_nResultPose2[POSE2_MEASURE_NECK_BACK_LENGTH] = SK_Planar_Distance(
		m_nLandmarkPose2[POSE2_LANDMARK_NECK_SIDE_L],
		m_nLandmarkPose2[POSE2_LANDMARK_NECK_SIDE_R],
		&BnecksidePlane,
		m_nResultPathPose2[POSE2_MEASURE_NECK_BACK_LENGTH],
		false
	);

	//////// 넙다리중간둘레
	// 왼쪽 다리 제외를 위한 샅점 수직 평면
	EgPlane pln_mid(EgVec3(-1, 0, 0), m_nLandmarkPose2[POSE2_LANDMARK_CROTCH]->m_Pos);
	pln = EgPlane(EgVec3(0, 1, 0), m_nLandmarkPose2[POSE2_LANDMARK_THIGH_MID]->m_Pos);
	crossList.clear();
	for (EgFace *face : m_ScanMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		if (pln.IsAbovePlane(p0) && pln.IsAbovePlane(p1) && pln.IsAbovePlane(p2))
			continue;
		if (pln.IsBelowPlane(p0) && pln.IsBelowPlane(p1) && pln.IsBelowPlane(p2))
			continue;

		if (pln_mid.IsAbovePlane(p0) && pln_mid.IsAbovePlane(p1) && pln_mid.IsAbovePlane(p2)) {
			if (dist(pln, p0) * dist(pln, p1) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p1, pln);
				crossList.push_back(crossPt0);
				// crossList.push_back(crossPt1);
			}
			if (dist(pln, p0) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p2, pln);
				crossList.push_back(crossPt0);
				// crossList.push_back(crossPt1);
			}
			if (dist(pln, p1) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p1, pln),
					crossPt1 = proj(p2, pln);
				crossList.push_back(crossPt0);
				// crossList.push_back(crossPt1);
			}
		}
	}

	convexList.clear();
	EgConvexhull cvxThigh(crossList);
	cvxThigh.MakeConvexHullXZ();
	cvxThigh.ArrangeHullPtXZ();
	convexList = cvxThigh.m_HullPtList;
	convexList.push_back(convexList[0]);

	m_nResultPose2[POSE2_MEASURE_THIGH_CIRC] = GetCirc(convexList);

	m_nResultPathPose2[POSE2_MEASURE_THIGH_CIRC].insert(
		m_nResultPathPose2[POSE2_MEASURE_THIGH_CIRC].end(),
		convexList.begin(),
		convexList.end()
	);

	//////// 무릎아래둘레
	pln = EgPlane(EgVec3(0, 1, 0), m_nLandmarkPose2[POSE2_LANDMARK_UNDERKNEE]->m_Pos);
	crossList.clear();
	for (EgFace *face : m_ScanMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		if (pln.IsAbovePlane(p0) && pln.IsAbovePlane(p1) && pln.IsAbovePlane(p2))
			continue;
		if (pln.IsBelowPlane(p0) && pln.IsBelowPlane(p1) && pln.IsBelowPlane(p2))
			continue;

		if (pln_mid.IsAbovePlane(p0) && pln_mid.IsAbovePlane(p1) && pln_mid.IsAbovePlane(p2)) {
			if (dist(pln, p0) * dist(pln, p1) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p1, pln);
				crossList.push_back(crossPt0);
				// crossList.push_back(crossPt1);
			}
			if (dist(pln, p0) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p2, pln);
				crossList.push_back(crossPt0);
				// crossList.push_back(crossPt1);
			}
			if (dist(pln, p1) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p1, pln),
					crossPt1 = proj(p2, pln);
				crossList.push_back(crossPt0);
				// crossList.push_back(crossPt1);
			}
		}
	}

	convexList.clear();
	EgConvexhull cvxUnderknee(crossList);
	cvxUnderknee.MakeConvexHullXZ();
	cvxUnderknee.ArrangeHullPtXZ();
	convexList = cvxUnderknee.m_HullPtList;
	convexList.push_back(convexList[0]);

	m_nResultPose2[POSE2_MEASURE_UNDERKNEE_CIRC] = GetCirc(convexList);

	m_nResultPathPose2[POSE2_MEASURE_UNDERKNEE_CIRC].insert(
		m_nResultPathPose2[POSE2_MEASURE_UNDERKNEE_CIRC].end(),
		convexList.begin(),
		convexList.end()
	);


	//////// 겨드랑둘레(어깨점)
	int i;
	GObList<GVector3f> CrossPtList;
	//오른 어깨
	GLandMark shp = GLandMark(m_nLandmarkPose2[POSE2_LANDMARK_SHOULDER_R]->m_Pos[0], m_nLandmarkPose2[POSE2_LANDMARK_SHOULDER_R]->m_Pos[1], m_nLandmarkPose2[POSE2_LANDMARK_SHOULDER_R]->m_Pos[2]);
	//오른 겨드랑
	GLandMark armpit = GLandMark(m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_R]->m_Pos[0], m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_R]->m_Pos[1], m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_R]->m_Pos[2]);
	GLandMark f = m_LandMarkHelp[54]; //
	GLandMark bh = m_LandMarkHelp[55]; //
	GLandMark foldf = m_LandMarkPose2[44]; //오른쪽겨드랑앞접힘점8
	GLandMark foldb = m_LandMarkPose2[45]; //오른쪽겨드랑뒤점힘점8

	CHECK(shp);
	CHECK(armpit);
	CHECK(f);
	CHECK(bh);
	CHECK(foldf);
	CHECK(foldb);

	//겨드랑점 z좌표 조정  F:B = 0.531: 0.469
	armpit.Z = f.Z*0.469f + bh.Z*0.531f;

	//겨드랑아래 보조점 계산하기
	GLandMark ff, bb;
	GVector3f fcornerRdn = GVector3f(armpit.X, armpit.Y, f.Z);
	GVector3f bcornerLdn = GVector3f(armpit.X, armpit.Y, bh.Z);
	GVector3f fcornerLup = GVector3f(armpit.X, f.Y, armpit.Z);
	GVector3f bcornerRup = GVector3f(armpit.X, bh.Y, armpit.Z);

	GVector3f intersectPtF, intersectPtB;

	::GetIntersectPointZY(f, armpit, fcornerLup, fcornerRdn, &intersectPtF);
	::GetIntersectPointZY(bh, armpit, bcornerRup, bcornerLdn, &intersectPtB);

	ff = GVector3f((foldf.X*0.3f + armpit.X*0.7f),
		(intersectPtF.Y*0.5f + fcornerRdn.Y*0.5f), //0.4, 0.6
		(intersectPtF.Z*0.5f + fcornerRdn.Z*0.5f));

	bb = GVector3f((foldb.X*0.3f + armpit.X*0.7f),
		(intersectPtB.Y*0.4f + bcornerLdn.Y*0.6f), //0.33 0.67
		(intersectPtB.Z*0.4f + bcornerLdn.Z*0.6f));


	//새로운 겨드랑점 = 앞점힘점과 뒤접힘점의 중간, 그냥 겨드랑점으로 하면 겨드랑점을 앞에 새로찍었을때 z가 중간이 되지 못한다

	//armpit.Z=( f.Z + b.Z ) * 0.5f;

	CrossPtList.Insert(shp);
	CrossPtList.Insert(f);
	CrossPtList.Insert(bh);

	// make plane
	GVector3f tmp;
	tmp = bh; tmp.X -= 100.0f;
	GPlane refPln(foldb, tmp, foldf);   //겨드랑 앞,뒤 접힘점을 지나는 수평에 가까운 평면
	GPlane cutPln(shp, foldf, foldb);   //어깨점과 겨드랑 앞,뒤 접힘점을 지나는 수직에 가까운 평면


	//어깨점과 겨드랑 앞,뒤 접힘점을 지나는 평면위에 있고, 겨드랑 앞,뒤 접힘점  위에 있는 점만 모으기
	for (i = 0; i < m_iNum; i += 3)
	{
		GVector3f v0, v1, v2;
		v0 = m_Vertex[m_FaceIdx[i]];
		v1 = m_Vertex[m_FaceIdx[i + 1]];
		v2 = m_Vertex[m_FaceIdx[i + 2]];

		// culling.
		if (v0.Y > shp.Y + 2.0f && v1.Y > shp.Y + 2.0f && v2.Y > shp.Y + 2.0f)  //어깨점 아래만
			continue;
		if (v0.Y < (armpit.Y - 2.0f) && v1.Y < (armpit.Y - 2.0f) && v2.Y < (armpit.Y - 2.0f))  // 겨드랑점 위만
			continue;
		if (v0.Z > f.Z + 10.0f && v1.Z > f.Z + 10.0f && v2.Z > f.Z + 10.0f)  //겨드랑앞점힘점 뒤만
			continue;
		if (v0.Z < bh.Z - 20.0f && v1.Z < bh.Z - 20.0f && v2.Z < bh.Z - 20.0f)  //겨드랑뒤점힘점 앞만
			continue;

		if (refPln.IsBelowPlane(v0) && refPln.IsBelowPlane(v1) && refPln.IsBelowPlane(v2))
			continue;

		if (cutPln.IsBelowPlane(v0) && cutPln.IsBelowPlane(v1) && cutPln.IsBelowPlane(v2))
			continue;

		if (cutPln.IsAbovePlane(v0) && cutPln.IsAbovePlane(v1) && cutPln.IsAbovePlane(v2))
			continue;

		if (cutPln.GetSignedDistance(v0) * cutPln.GetSignedDistance(v1) <= 0)
		{
			GVector3f CrossPt = cutPln.GetProjectionPt(v0, v1 - v0);
			CrossPtList.Insert(CrossPt);
		}
		if (cutPln.GetSignedDistance(v1) * cutPln.GetSignedDistance(v2) <= 0)
		{
			GVector3f CrossPt = cutPln.GetProjectionPt(v1, v2 - v1);
			CrossPtList.Insert(CrossPt);
		}
		if (cutPln.GetSignedDistance(v2) * cutPln.GetSignedDistance(v0) <= 0)
		{
			GVector3f CrossPt = cutPln.GetProjectionPt(v0, v2 - v0);
			CrossPtList.Insert(CrossPt);
		}
	}

	// 겨드랑 앞,뒤 접힘점 아래는 곡선으로 --> 앞에서 뒤의 순서로 
	REAL4 P[5];
	P[0][0] = foldf[0];		P[0][1] = foldf[1];		P[0][2] = foldf[2];		P[0][3] = 1.0f;
	P[1][0] = ff[0];		P[1][1] = ff[1];		P[1][2] = ff[2];		P[1][3] = 1.0f;
	P[2][0] = armpit[0];	P[2][1] = armpit[1];	P[2][2] = armpit[2];    P[2][3] = 1.0f;
	P[3][0] = bb[0];		P[3][1] = bb[1];		P[3][2] = bb[2];		P[3][3] = 1.0f;
	P[4][0] = foldb[0];		P[4][1] = foldb[1];		P[4][2] = foldb[2];		P[4][3] = 1.0f;
	GNurbsCrv PathCrv;
	PathCrv.InterCrv(3, P, 4);

	int snum = 20;
	REAL4 v0;
	REAL min = PathCrv.GetMinKnotVal();
	REAL max = PathCrv.GetMaxKnotVal();
	for (i = 0; i < snum; i++)
	{
		REAL u0 = min + ((max - min)*i) / (REAL)(snum - 1);
		PathCrv.Eval(u0, v0);
		GVector3f pt(v0);
		CrossPtList.Insert(pt);
	}
	CConvexHull cvx(CrossPtList);
	cvx.MakeConvexHullYZ();

	cvx.ArrangeHullPtYZ();
	float val = cvx.GetHullLength();

	m_nResultPose2[POSE2_MEASURE_ARMPIT_SHOULDER_CIRC] = val;
	for (int i = 0; i < cvx.m_HullPtList.m_nCount; i++) {
		GVector3f v = cvx.m_HullPtList[i];
		m_nResultPathPose2[POSE2_MEASURE_ARMPIT_SHOULDER_CIRC].push_back(EgPos(v.X, v.Y, v.Z));
	}
	v = cvx.m_HullPtList[0];
	m_nResultPathPose2[POSE2_MEASURE_ARMPIT_SHOULDER_CIRC].push_back(EgPos(v.X, v.Y, v.Z));




	//////// 가쪽복사수평둘레
	pln = EgPlane(EgVec3(0, 1, 0), m_nLandmarkPose2[POSE2_LANDMARK_ANKLE]->m_Pos);
	crossList.clear();
	for (EgFace *face : m_ScanMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		if (pln.IsAbovePlane(p0) && pln.IsAbovePlane(p1) && pln.IsAbovePlane(p2))
			continue;
		if (pln.IsBelowPlane(p0) && pln.IsBelowPlane(p1) && pln.IsBelowPlane(p2))
			continue;

		if (pln_mid.IsAbovePlane(p0) && pln_mid.IsAbovePlane(p1) && pln_mid.IsAbovePlane(p2)) {
			if (dist(pln, p0) * dist(pln, p1) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p1, pln);
				crossList.push_back(crossPt0);
				crossList.push_back(crossPt1);
			}
			if (dist(pln, p0) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p2, pln);
				crossList.push_back(crossPt0);
				crossList.push_back(crossPt1);
			}
			if (dist(pln, p1) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p1, pln),
					crossPt1 = proj(p2, pln);
				crossList.push_back(crossPt0);
				crossList.push_back(crossPt1);
			}
		}
	}

	convexList.clear();
	EgConvexhull cvxAnkle(crossList);
	cvxAnkle.MakeConvexHullXZ();
	cvxAnkle.ArrangeHullPtXZ();
	convexList = cvxAnkle.m_HullPtList;
	convexList.push_back(convexList[0]);

	m_nResultPose2[POSE2_MEASURE_ANKLE_CIRC] = GetCirc(convexList);

	m_nResultPathPose2[POSE2_MEASURE_ANKLE_CIRC].insert(
		m_nResultPathPose2[POSE2_MEASURE_ANKLE_CIRC].end(),
		convexList.begin(),
		convexList.end()
	);

	//////// 장딴지최대둘레
	// 장딴지 부위에서 최대 둘레를 갖는 부분
	pln = EgPlane(EgVec3(0, 1, 0), m_nLandmarkPose2[POSE2_LANDMARK_CALF_EXTRUDE]->m_Pos);
	crossList.clear();
	for (EgFace *face : m_ScanMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		if (pln.IsAbovePlane(p0) && pln.IsAbovePlane(p1) && pln.IsAbovePlane(p2))
			continue;
		if (pln.IsBelowPlane(p0) && pln.IsBelowPlane(p1) && pln.IsBelowPlane(p2))
			continue;

		if (pln_mid.IsAbovePlane(p0) && pln_mid.IsAbovePlane(p1) && pln_mid.IsAbovePlane(p2)) {
			if (dist(pln, p0) * dist(pln, p1) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p1, pln);
				crossList.push_back(crossPt0);
				crossList.push_back(crossPt1);
			}
			if (dist(pln, p0) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p0, pln),
					crossPt1 = proj(p2, pln);
				crossList.push_back(crossPt0);
				crossList.push_back(crossPt1);
			}
			if (dist(pln, p1) * dist(pln, p2) < 0) {
				EgPos crossPt0 = proj(p1, pln),
					crossPt1 = proj(p2, pln);
				crossList.push_back(crossPt0);
				crossList.push_back(crossPt1);
			}
		}
	}

	convexList.clear();
	EgConvexhull cvxMaxCalf(crossList);
	cvxMaxCalf.MakeConvexHullXZ();
	cvxMaxCalf.ArrangeHullPtXZ();
	convexList = cvxMaxCalf.m_HullPtList;
	convexList.push_back(convexList[0]);

	m_nResultPose2[POSE2_MEASURE_MAX_CALF_EXTRUDE_CIRC] = GetCirc(convexList);

	m_nResultPathPose2[POSE2_MEASURE_MAX_CALF_EXTRUDE_CIRC].insert(
		m_nResultPathPose2[POSE2_MEASURE_MAX_CALF_EXTRUDE_CIRC].end(),
		convexList.begin(),
		convexList.end()
	);


	//////// 샅앞길이
	pln = EgPlane(m_nLandmarkPose2[POSE2_LANDMARK_WAIST_FRONT]->m_Pos, m_nLandmarkPose2[POSE2_LANDMARK_CROTCH]->m_Pos, m_nLandmarkPose2[POSE2_LANDMARK_WAIST_BACK]->m_Pos);

	m_nResultPose2[POSE2_MEASURE_CROTCH_FRONT_LENGTH]
		= SK_Planar_Distance(
			m_nLandmarkPose2[POSE2_LANDMARK_WAIST_FRONT],
			m_nLandmarkPose2[POSE2_LANDMARK_CROTCH],
			&pln,
			m_nResultPathPose2[POSE2_MEASURE_CROTCH_FRONT_LENGTH],
			false
		);


	//////// 샅뒤길이
	// 다리가 붙어나오는 부분 제외
	std::vector<EgPos> tmpCrotchBack;
	// m_nResultPose2[POSE2_MEASURE_CROTCH_BACK_LENGTH] = 
	SK_Planar_Distance(
		m_nLandmarkPose2[POSE2_LANDMARK_CROTCH],
		m_nLandmarkPose2[POSE2_LANDMARK_WAIST_BACK],
		&pln,
		// m_nResultPathPose2[POSE2_MEASURE_CROTCH_BACK_LENGTH],
		tmpCrotchBack,
		false
	);

	for (EgPos pos : tmpCrotchBack) {
		if (pos.m_Coords[1] >= m_nLandmarkPose2[POSE2_LANDMARK_CROTCH]->m_Pos[1]) {
			m_nResultPathPose2[POSE2_MEASURE_CROTCH_BACK_LENGTH].push_back(pos);
		}
	}

	if (!m_nResultPathPose2[POSE2_MEASURE_CROTCH_BACK_LENGTH].empty()) {
		for (int i = 0; i < m_nResultPathPose2[POSE2_MEASURE_CROTCH_BACK_LENGTH].size() - 1; i++) {
			m_nResultPose2[POSE2_MEASURE_CROTCH_BACK_LENGTH] += dist(m_nResultPathPose2[POSE2_MEASURE_CROTCH_BACK_LENGTH][i], m_nResultPathPose2[POSE2_MEASURE_CROTCH_BACK_LENGTH][i + 1]);
		}
	}



	//////// 몸통옆길이
	m_nResultPose2[POSE2_MEASURE_TORSO_SIDE_LENGTH] = m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_R]->m_Pos[1] - m_nLandmarkPose2[POSE2_LANDMARK_WAIST]->m_Pos[1];

	EgPos p = m_nLandmarkPose2[POSE2_LANDMARK_WAIST]->m_Pos;
	p.m_Coords[0] = m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_R]->m_Pos[0];
	p.m_Coords[2] = m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_R]->m_Pos[2];

	m_nResultPathPose2[POSE2_MEASURE_TORSO_SIDE_LENGTH].push_back(m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_R]->m_Pos);
	m_nResultPathPose2[POSE2_MEASURE_TORSO_SIDE_LENGTH].push_back(p);


	// 목두께
	p = m_nLandmarkPose2[POSE2_LANDMARK_NECK_THYROID]->m_Pos;
	EgPos q = m_nLandmarkPose2[POSE2_LANDMARK_NECK_BACK]->m_Pos;

	m_nResultPose2[POSE2_MEASURE_NECK_DEPTH] = dist(
		m_nLandmarkPose2[POSE2_LANDMARK_NECK_THYROID]->m_Pos,
		m_nLandmarkPose2[POSE2_LANDMARK_NECK_BACK]->m_Pos
	);

	m_nResultPathPose2[POSE2_MEASURE_NECK_DEPTH].push_back(EgPos(p.m_Coords[0] + 100.0f, p.m_Coords[1], p.m_Coords[2]));
	m_nResultPathPose2[POSE2_MEASURE_NECK_DEPTH].push_back(EgPos(q.m_Coords[0] + 100.0f, q.m_Coords[1], q.m_Coords[2]));


	// 가슴두께(정중면)
	// 겨드랑수준의 정중면에서 ray를 쏴서 생기는 두 교차점의 두께를 구한다
	EgPos centerMaxZ = EgPos(), centerMinZ = EgPos();
	EgLine ray(EgPos((m_MinX + m_MaxX) / 2, m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_R]->m_Pos[1], 0), EgVec3(0, 0, 1));
	for (EgFace *face : m_ScanMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		EgPos tmpIntersection;
		if (intersect_line_triangle(p0, p1, p2, ray, tmpIntersection, false)) {
			if (tmpIntersection.m_Coords[2] > (m_MinZ + m_MaxZ) / 2) {
				centerMaxZ = tmpIntersection;
			}
			else if (tmpIntersection.m_Coords[2] < (m_MinZ + m_MaxZ) / 2) {
				centerMinZ = tmpIntersection;
			}
		}
	}

	centerMaxZ.m_Coords[0] = m_MinX - 50.0f;
	centerMinZ.m_Coords[0] = m_MinX - 50.0f;

	m_nResultPose2[POSE2_MEASURE_BUST_CENTER_DEPTH] = ABS(centerMaxZ.m_Coords[2] - centerMinZ.m_Coords[2]);

	m_nResultPathPose2[POSE2_MEASURE_BUST_CENTER_DEPTH].push_back(centerMaxZ);
	m_nResultPathPose2[POSE2_MEASURE_BUST_CENTER_DEPTH].push_back(centerMinZ);


	// 허리두께(정중면)
	ray = EgLine(EgPos((m_MinX + m_MaxX) / 2, m_nLandmarkPose2[POSE2_LANDMARK_WAIST]->m_Pos[1], 0), EgVec3(0, 0, 1));

	for (EgFace *face : m_ScanMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		EgPos tmpIntersection;
		if (intersect_line_triangle(p0, p1, p2, ray, tmpIntersection, false)) {
			if (tmpIntersection.m_Coords[2] > (m_MinZ + m_MaxZ) / 2) {
				centerMaxZ = tmpIntersection;
			}
			else if (tmpIntersection.m_Coords[2] < (m_MinZ + m_MaxZ) / 2) {
				centerMinZ = tmpIntersection;
			}
		}
	}

	centerMaxZ.m_Coords[0] = m_MinX - 50.0f;
	centerMinZ.m_Coords[0] = m_MinX - 50.0f;

	m_nResultPose2[POSE2_MEASURE_WAIST_CENTER_DEPTH] = ABS(centerMaxZ.m_Coords[2] - centerMinZ.m_Coords[2]);

	m_nResultPathPose2[POSE2_MEASURE_WAIST_CENTER_DEPTH].push_back(centerMaxZ);
	m_nResultPathPose2[POSE2_MEASURE_WAIST_CENTER_DEPTH].push_back(centerMinZ);


	// 넙다리두께
	// 넙다리 둘레에서 min/max Z로 두께 사용
	centerMaxZ = EgPos(0, 0, -INFINITY);
	centerMinZ = EgPos(0, 0, INFINITY);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_UNDERHIP_CIRC]) {
		if (pos.m_Coords[2] > centerMaxZ.m_Coords[2]) {
			centerMaxZ = pos;
		}
		if (pos.m_Coords[2] < centerMinZ.m_Coords[2]) {
			centerMinZ = pos;
		}
	}

	centerMaxZ.m_Coords[0] = m_MinX - 50.0f;
	centerMinZ.m_Coords[0] = m_MinX - 50.0f;

	m_nResultPose2[POSE2_MEASURE_UNDERHIP_DEPTH] = ABS(centerMaxZ.m_Coords[2] - centerMinZ.m_Coords[2]);

	m_nResultPathPose2[POSE2_MEASURE_UNDERHIP_DEPTH].push_back(centerMaxZ);
	m_nResultPathPose2[POSE2_MEASURE_UNDERHIP_DEPTH].push_back(centerMinZ);


	// 넙다리중간두께
	centerMaxZ = EgPos(0, 0, -INFINITY);
	centerMinZ = EgPos(0, 0, INFINITY);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_THIGH_CIRC]) {
		if (pos.m_Coords[2] > centerMaxZ.m_Coords[2]) {
			centerMaxZ = pos;
		}
		if (pos.m_Coords[2] < centerMinZ.m_Coords[2]) {
			centerMinZ = pos;
		}
	}

	centerMaxZ.m_Coords[0] = m_MinX - 50.0f;
	centerMinZ.m_Coords[0] = m_MinX - 50.0f;

	m_nResultPose2[POSE2_MEASURE_THIGH_DEPTH] = ABS(centerMaxZ.m_Coords[2] - centerMinZ.m_Coords[2]);
	m_nResultPathPose2[POSE2_MEASURE_THIGH_DEPTH].push_back(centerMaxZ);
	m_nResultPathPose2[POSE2_MEASURE_THIGH_DEPTH].push_back(centerMinZ);


	// 무릎두께
	centerMaxZ = EgPos(0, 0, -INFINITY);
	centerMinZ = EgPos(0, 0, INFINITY);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_KNEE_CIRC]) {
		if (pos.m_Coords[2] > centerMaxZ.m_Coords[2]) {
			centerMaxZ = pos;
		}
		if (pos.m_Coords[2] < centerMinZ.m_Coords[2]) {
			centerMinZ = pos;
		}
	}

	centerMaxZ.m_Coords[0] = m_MinX - 50.0f;
	centerMinZ.m_Coords[0] = m_MinX - 50.0f;

	m_nResultPose2[POSE2_MEASURE_KNEE_DEPTH] = ABS(centerMaxZ.m_Coords[2] - centerMinZ.m_Coords[2]);

	m_nResultPathPose2[POSE2_MEASURE_KNEE_DEPTH].push_back(centerMaxZ);
	m_nResultPathPose2[POSE2_MEASURE_KNEE_DEPTH].push_back(centerMinZ);


	// 무릎아래두께
	centerMaxZ = EgPos(0, 0, -INFINITY);
	centerMinZ = EgPos(0, 0, INFINITY);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_UNDERKNEE_CIRC]) {
		if (pos.m_Coords[2] > centerMaxZ.m_Coords[2]) {
			centerMaxZ = pos;
		}
		if (pos.m_Coords[2] < centerMinZ.m_Coords[2]) {
			centerMinZ = pos;
		}
	}

	centerMaxZ.m_Coords[0] = m_MinX - 50.0f;
	centerMinZ.m_Coords[0] = m_MinX - 50.0f;

	m_nResultPose2[POSE2_MEASURE_UNDERKNEE_DEPTH] = ABS(centerMaxZ.m_Coords[2] - centerMinZ.m_Coords[2]);

	m_nResultPathPose2[POSE2_MEASURE_UNDERKNEE_DEPTH].push_back(centerMaxZ);
	m_nResultPathPose2[POSE2_MEASURE_UNDERKNEE_DEPTH].push_back(centerMinZ);


	// 장딴지두께
	centerMaxZ = EgPos(0, 0, -INFINITY);
	centerMinZ = EgPos(0, 0, INFINITY);

	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_CALF_EXTRUDE_CIRC]) {
		if (pos.m_Coords[2] < centerMinZ.m_Coords[2]) {
			centerMinZ = pos;
		}
		if (pos.m_Coords[2] > centerMaxZ.m_Coords[2]) {
			centerMaxZ = pos;
		}
	}

	centerMaxZ.m_Coords[0] = m_MinX - 50.0f;
	centerMinZ.m_Coords[0] = m_MinX - 50.0f;

	m_nResultPose2[POSE2_MEASURE_CALF_DEPTH] = ABS(centerMaxZ.m_Coords[2] - centerMinZ.m_Coords[2]);

	m_nResultPathPose2[POSE2_MEASURE_CALF_DEPTH].push_back(centerMaxZ);
	m_nResultPathPose2[POSE2_MEASURE_CALF_DEPTH].push_back(centerMinZ);


	// 종아리아래두께
	centerMaxZ = EgPos(0, 0, -INFINITY);
	centerMinZ = EgPos(0, 0, INFINITY);

	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_UNDERCALF_CIRC]) {
		if (pos.m_Coords[2] < centerMinZ.m_Coords[2]) {
			centerMinZ = pos;
		}
		if (pos.m_Coords[2] > centerMaxZ.m_Coords[2]) {
			centerMaxZ = pos;
		}
	}

	centerMaxZ.m_Coords[0] = m_MinX - 50.0f;
	centerMinZ.m_Coords[0] = m_MinX - 50.0f;

	m_nResultPose2[POSE2_MEASURE_UNDERCALF_DEPTH] = ABS(centerMaxZ.m_Coords[2] - centerMinZ.m_Coords[2]);

	m_nResultPathPose2[POSE2_MEASURE_UNDERCALF_DEPTH].push_back(centerMaxZ);
	m_nResultPathPose2[POSE2_MEASURE_UNDERCALF_DEPTH].push_back(centerMinZ);


	// 위팔두께
	// mj::편위팔둘레선 경로에서 찾는다
	minZ = EgPos(0, 0, INFINITY);
	maxZ = EgPos(0, 0, -INFINITY);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_UPPERARM_CIRC]) {
		if (pos.m_Coords[2] < minZ.m_Coords[2])
			minZ = pos;
		if (pos.m_Coords[2] > maxZ.m_Coords[2])
			maxZ = pos;
	}

	m_nResultPose2[POSE2_MEASURE_UPPERARM_DEPTH] = ABS(maxZ.m_Coords[2] - minZ.m_Coords[2]);
	maxZ.m_Coords[0] -= 100.0f;
	minZ.m_Coords[0] -= 100.0f;
	m_nResultPathPose2[POSE2_MEASURE_UPPERARM_DEPTH].push_back(maxZ);
	m_nResultPathPose2[POSE2_MEASURE_UPPERARM_DEPTH].push_back(minZ);


	// 팔꿈치두께
	minZ = EgPos(0, 0, INFINITY);
	maxZ = EgPos(0, 0, -INFINITY);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_ELBOW_CIRC]) {
		if (pos.m_Coords[2] < minZ.m_Coords[2])
			minZ = pos;
		if (pos.m_Coords[2] > maxZ.m_Coords[2])
			maxZ = pos;
	}

	m_nResultPose2[POSE2_MEASURE_ELBOW_DEPTH] = ABS(maxZ.m_Coords[2] - minZ.m_Coords[2]);
	maxZ.m_Coords[0] -= 100.0f;
	minZ.m_Coords[0] -= 100.0f;
	m_nResultPathPose2[POSE2_MEASURE_ELBOW_DEPTH].push_back(maxZ);
	m_nResultPathPose2[POSE2_MEASURE_ELBOW_DEPTH].push_back(minZ);


	// 손목두께
	minZ = EgPos(0, 0, INFINITY);
	maxZ = EgPos(0, 0, -INFINITY);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_WRIST_CIRC]) {
		if (pos.m_Coords[2] < minZ.m_Coords[2])
			minZ = pos;
		if (pos.m_Coords[2] > maxZ.m_Coords[2])
			maxZ = pos;
	}

	m_nResultPose2[POSE2_MEASURE_WRIST_DEPTH] = ABS(maxZ.m_Coords[2] - minZ.m_Coords[2]);
	maxZ.m_Coords[0] -= 100.0f;
	minZ.m_Coords[0] -= 100.0f;
	m_nResultPathPose2[POSE2_MEASURE_WRIST_DEPTH].push_back(maxZ);
	m_nResultPathPose2[POSE2_MEASURE_WRIST_DEPTH].push_back(minZ);


	// 목밑너비 -> 목너비(방패연골아래점) (20. 9. 23)
	EgPos minX = EgPos(INFINITY, 0, 0);
	EgPos maxX = EgPos(-INFINITY, 0, 0);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_NECK_THYROID_CIRC]) {
		if (pos.m_Coords[0] < minX.m_Coords[0]) {
			minX = pos;
		}
		if (pos.m_Coords[0] > maxX.m_Coords[0]) {
			maxX = pos;
		}
	}

	// z 수준 통일
	minX.m_Coords[2] = maxX.m_Coords[2];
	minX.m_Coords[2] += 200.0f;
	maxX.m_Coords[2] += 200.0f;

	m_nResultPose2[POSE2_MEASURE_NECK_WIDTH] = ABS(maxX.m_Coords[0] - minX.m_Coords[0]);
	m_nResultPathPose2[POSE2_MEASURE_NECK_WIDTH].push_back(minX);
	m_nResultPathPose2[POSE2_MEASURE_NECK_WIDTH].push_back(maxX);



	// 넙다리너비
	// mj::넙다리 둘레에서 최대최소 x값을 가진 점들로 구한다
	minX = EgPos(INFINITY, 0, 0), maxX = EgPos(-INFINITY, 0, 0);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_UNDERHIP_CIRC]) {
		if (pos.m_Coords[0] < minX.m_Coords[0])
			minX = pos;
		if (pos.m_Coords[0] > maxX.m_Coords[0])
			maxX = pos;
	}

	minX.m_Coords[2] = maxX.m_Coords[2];
	minX.m_Coords[2] += 500.0f;
	maxX.m_Coords[2] += 500.0f;

	m_nResultPose2[POSE2_MEASURE_UNDERHIP_WIDTH] = ABS(maxX.m_Coords[0] - minX.m_Coords[0]);
	m_nResultPathPose2[POSE2_MEASURE_UNDERHIP_WIDTH].push_back(minX);
	m_nResultPathPose2[POSE2_MEASURE_UNDERHIP_WIDTH].push_back(maxX);


	// 넙다리중간너비
	minX = EgPos(INFINITY, 0, 0);
	maxX = EgPos(-INFINITY, 0, 0);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_THIGH_CIRC]) {
		if (pos.m_Coords[0] < minX.m_Coords[0])
			minX = pos;
		if (pos.m_Coords[0] > maxX.m_Coords[0])
			maxX = pos;
	}

	minX.m_Coords[2] = maxX.m_Coords[2];
	minX.m_Coords[2] += 500.0f;
	maxX.m_Coords[2] += 500.0f;

	m_nResultPose2[POSE2_MEASURE_THIGH_WIDTH] = ABS(maxX.m_Coords[0] - minX.m_Coords[0]);
	m_nResultPathPose2[POSE2_MEASURE_THIGH_WIDTH].push_back(minX);
	m_nResultPathPose2[POSE2_MEASURE_THIGH_WIDTH].push_back(maxX);


	// 무릎너비
	minX = EgPos(INFINITY, 0, 0);
	maxX = EgPos(-INFINITY, 0, 0);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_KNEE_CIRC]) {
		if (pos.m_Coords[0] < minX.m_Coords[0])
			minX = pos;
		if (pos.m_Coords[0] > maxX.m_Coords[0])
			maxX = pos;
	}

	minX.m_Coords[2] = maxX.m_Coords[2];
	minX.m_Coords[2] += 500.0f;
	maxX.m_Coords[2] += 500.0f;

	m_nResultPose2[POSE2_MEASURE_KNEE_WIDTH] = ABS(maxX.m_Coords[0] - minX.m_Coords[0]);
	m_nResultPathPose2[POSE2_MEASURE_KNEE_WIDTH].push_back(minX);
	m_nResultPathPose2[POSE2_MEASURE_KNEE_WIDTH].push_back(maxX);


	// 무릎아래너비
	minX = EgPos(INFINITY, 0, 0);
	maxX = EgPos(-INFINITY, 0, 0);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_UNDERKNEE_CIRC]) {
		if (pos.m_Coords[0] < minX.m_Coords[0])
			minX = pos;
		if (pos.m_Coords[0] > maxX.m_Coords[0])
			maxX = pos;
	}

	minX.m_Coords[2] = maxX.m_Coords[2];
	minX.m_Coords[2] += 500.0f;
	maxX.m_Coords[2] += 500.0f;

	m_nResultPose2[POSE2_MEASURE_UNDERKNEE_WIDTH] = ABS(maxX.m_Coords[0] - minX.m_Coords[0]);
	m_nResultPathPose2[POSE2_MEASURE_UNDERKNEE_WIDTH].push_back(minX);
	m_nResultPathPose2[POSE2_MEASURE_UNDERKNEE_WIDTH].push_back(maxX);


	// 장딴지너비
	minX = EgPos(INFINITY, 0, 0);
	maxX = EgPos(-INFINITY, 0, 0);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_CALF_EXTRUDE_CIRC]) {
		if (pos.m_Coords[0] < minX.m_Coords[0])
			minX = pos;
		if (pos.m_Coords[0] > maxX.m_Coords[0])
			maxX = pos;
	}

	minX.m_Coords[2] = maxX.m_Coords[2];
	minX.m_Coords[2] += 500.0f;
	maxX.m_Coords[2] += 500.0f;

	m_nResultPose2[POSE2_MEASURE_CALF_WIDTH] = ABS(maxX.m_Coords[0] - minX.m_Coords[0]);
	m_nResultPathPose2[POSE2_MEASURE_CALF_WIDTH].push_back(minX);
	m_nResultPathPose2[POSE2_MEASURE_CALF_WIDTH].push_back(maxX);


	// 종아리아래너비
	minX = EgPos(INFINITY, 0, 0);
	maxX = EgPos(-INFINITY, 0, 0);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_UNDERCALF_CIRC]) {
		if (pos.m_Coords[0] < minX.m_Coords[0])
			minX = pos;
		if (pos.m_Coords[0] > maxX.m_Coords[0])
			maxX = pos;
	}

	minX.m_Coords[2] = maxX.m_Coords[2];
	minX.m_Coords[2] += 500.0f;
	maxX.m_Coords[2] += 500.0f;

	m_nResultPose2[POSE2_MEASURE_UNDERCALF_WIDTH] = ABS(maxX.m_Coords[0] - minX.m_Coords[0]);
	m_nResultPathPose2[POSE2_MEASURE_UNDERCALF_WIDTH].push_back(minX);
	m_nResultPathPose2[POSE2_MEASURE_UNDERCALF_WIDTH].push_back(maxX);


	// 위팔너비
	minX = EgPos(INFINITY, 0, 0);
	maxX = EgPos(-INFINITY, 0, 0);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_UPPERARM_CIRC]) {
		if (pos.m_Coords[0] < minX.m_Coords[0])
			minX = pos;
		if (pos.m_Coords[0] > maxX.m_Coords[0])
			maxX = pos;
	}

	minX.m_Coords[2] = maxX.m_Coords[2];
	minX.m_Coords[2] += 300.0f;
	maxX.m_Coords[2] += 300.0f;

	m_nResultPose2[POSE2_MEASURE_UPPERARM_WIDTH] = ABS(maxX.m_Coords[0] - minX.m_Coords[0]);
	m_nResultPathPose2[POSE2_MEASURE_UPPERARM_WIDTH].push_back(minX);
	m_nResultPathPose2[POSE2_MEASURE_UPPERARM_WIDTH].push_back(maxX);


	// 팔꿈치너비
	minX = EgPos(INFINITY, 0, 0);
	maxX = EgPos(-INFINITY, 0, 0);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_ELBOW_CIRC]) {
		if (pos.m_Coords[0] < minX.m_Coords[0])
			minX = pos;
		if (pos.m_Coords[0] > maxX.m_Coords[0])
			maxX = pos;
	}

	minX.m_Coords[2] = maxX.m_Coords[2];
	minX.m_Coords[2] += 300.0f;
	maxX.m_Coords[2] += 300.0f;

	m_nResultPose2[POSE2_MEASURE_ELBOW_WIDTH] = ABS(maxX.m_Coords[0] - minX.m_Coords[0]);
	m_nResultPathPose2[POSE2_MEASURE_ELBOW_WIDTH].push_back(minX);
	m_nResultPathPose2[POSE2_MEASURE_ELBOW_WIDTH].push_back(maxX);


	// 손목너비
	minX = EgPos(INFINITY, 0, 0);
	maxX = EgPos(-INFINITY, 0, 0);
	for (EgPos pos : m_nResultPathPose2[POSE2_MEASURE_WRIST_CIRC]) {
		if (pos.m_Coords[0] < minX.m_Coords[0])
			minX = pos;
		if (pos.m_Coords[0] > maxX.m_Coords[0])
			maxX = pos;
	}

	minX.m_Coords[2] = maxX.m_Coords[2];
	minX.m_Coords[2] += 300.0f;
	maxX.m_Coords[2] += 300.0f;

	m_nResultPose2[POSE2_MEASURE_WRIST_WIDTH] = ABS(maxX.m_Coords[0] - minX.m_Coords[0]);
	m_nResultPathPose2[POSE2_MEASURE_WRIST_WIDTH].push_back(minX);
	m_nResultPathPose2[POSE2_MEASURE_WRIST_WIDTH].push_back(maxX);



	// 몸통세로길이(체표를 따르는)
	pln = EgPlane(m_nLandmarkPose2[POSE2_LANDMARK_NECK_FRONT]->m_Pos,
		m_nLandmarkPose2[POSE2_LANDMARK_CROTCH]->m_Pos,
		m_nLandmarkPose2[POSE2_LANDMARK_NECK_BACK]->m_Pos);

	std::vector<EgPos> tmpTorso;
	int result = SK_Planar_Distance(
		m_nLandmarkPose2[POSE2_LANDMARK_NECK_FRONT],
		m_nLandmarkPose2[POSE2_LANDMARK_NECK_BACK],
		&pln,
		tmpTorso,
		false
	);

	// 목앞점-샅점-목뒤점 평면 경로가 생성된 경우
	if (result != -1) {
		// 샅점 밑의 점은 모두 제외
		for (EgPos pos : tmpTorso) {
			if (pos.m_Coords[1] >= m_nLandmarkPose2[POSE2_LANDMARK_CROTCH]->m_Pos[1]) {
				m_nResultPathPose2[POSE2_MEASURE_TORSO_LENGTH].push_back(pos);
			}
		}
		for (int i = 0; i < m_nResultPathPose2[POSE2_MEASURE_TORSO_LENGTH].size() - 1; i++) {
			m_nResultPose2[POSE2_MEASURE_TORSO_LENGTH] += dist(m_nResultPathPose2[POSE2_MEASURE_TORSO_LENGTH][i], m_nResultPathPose2[POSE2_MEASURE_TORSO_LENGTH][i + 1]);
		}

		// 몸통세로길이(줄자)
		EgConvexhull convexTorsoList(m_nResultPathPose2[POSE2_MEASURE_TORSO_LENGTH]);
		convexTorsoList.MakeConvexHullYZ();
		convexTorsoList.ArrangeHullPtYZ();

		m_nResultPose2[POSE2_MEASURE_CONVEX_TORSO_LENGTH] = GetCirc(convexTorsoList.m_HullPtList);
		m_nResultPathPose2[POSE2_MEASURE_CONVEX_TORSO_LENGTH].insert(
			m_nResultPathPose2[POSE2_MEASURE_CONVEX_TORSO_LENGTH].end(),
			convexTorsoList.m_HullPtList.begin(),
			convexTorsoList.m_HullPtList.end()
		);
	}


	// 젖가슴컵사이즈
	float diff = m_nResultPose2[POSE2_MEASURE_BUST_CIRC] - m_nResultPose2[POSE2_MEASURE_UNDERBUST_CIRC];
	std::string cup = "";

	if (diff < 625) {
		cup = "AAA";
		m_nResultPose2[POSE2_MEASURE_CUP_SIZE] = 5;
	}
	else if (diff < 875) {
		cup = "AA";
		m_nResultPose2[POSE2_MEASURE_CUP_SIZE] = 7.5;
	}
	else if (diff < 1125) {
		cup = "A";
		m_nResultPose2[POSE2_MEASURE_CUP_SIZE] = 10;
	}
	else if (diff < 1375) {
		cup = "B";
		m_nResultPose2[POSE2_MEASURE_CUP_SIZE] = 12.5;
	}
	else if (diff < 1625) {
		cup = "C";
		m_nResultPose2[POSE2_MEASURE_CUP_SIZE] = 15;
	}
	else if (diff < 1875) {
		cup = "D";
		m_nResultPose2[POSE2_MEASURE_CUP_SIZE] = 17.5;
	}
	else if (diff < 2125) {
		cup = "E";
		m_nResultPose2[POSE2_MEASURE_CUP_SIZE] = 20;
	}
	else if (diff < 2375) {
		cup = "F";
		m_nResultPose2[POSE2_MEASURE_CUP_SIZE] = 22.5;
	}
	else if (diff < 2625) {
		cup = "G";
		m_nResultPose2[POSE2_MEASURE_CUP_SIZE] = 25;
	}
	else if (diff < 2875) {
		cup = "H";
		m_nResultPose2[POSE2_MEASURE_CUP_SIZE] = 27.5;
	}



	// mj::저장된 Measure 결과를 EgSizer로 옮긴다
	theSizer.Measure();


	// mj::ToDo::Create Pose2 Result Dialog
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame->m_pPose2ResultDlg == NULL)
	{
		pFrame->m_pPose2ResultDlg = new CPose2ResultDialog();
		pFrame->m_pPose2ResultDlg->Create(IDD_POSE2_RESULT_DIALOG);
	}
	CString str[NEW_RESULT_NUM_POSE2];
	for (int i = 0; i < NEW_RESULT_NUM_POSE2; ++i)
		str[i].Format("%.2lf ", theSizer.m_Results[i]);

	CPose2ResultDialog *pDlg = pFrame->m_pPose2ResultDlg;
	pDlg->InitDialog();
	pDlg->m_Pose2Result1 = str[0];
	pDlg->m_Pose2Result2 = str[1];
	pDlg->m_Pose2Result3 = str[2];
	pDlg->m_Pose2Result4 = str[3];
	pDlg->m_Pose2Result5 = str[4];
	pDlg->m_Pose2Result6 = str[5];
	pDlg->m_Pose2Result7 = str[6];
	pDlg->m_Pose2Result8 = str[7];
	pDlg->m_Pose2Result9 = str[8];
	pDlg->m_Pose2Result10 = str[9];
	pDlg->m_Pose2Result11 = str[10];
	pDlg->m_Pose2Result12 = str[11];
	pDlg->m_Pose2Result13 = str[12];
	pDlg->m_Pose2Result14 = str[13];
	pDlg->m_Pose2Result15 = str[14];
	pDlg->m_Pose2Result16 = str[15];
	pDlg->m_Pose2Result17 = str[16];
	pDlg->m_Pose2Result18 = str[17];
	pDlg->m_Pose2Result19 = str[18];
	pDlg->m_Pose2Result20 = str[19];
	pDlg->m_Pose2Result21 = str[20];
	pDlg->m_Pose2Result22 = str[21];
	pDlg->m_Pose2Result23 = str[22];
	pDlg->m_Pose2Result24 = str[23];
	pDlg->m_Pose2Result25 = str[24];
	pDlg->m_Pose2Result26 = str[25];
	pDlg->m_Pose2Result27 = str[26];
	pDlg->m_Pose2Result28 = str[27];
	pDlg->m_Pose2Result29 = str[28];
	pDlg->m_Pose2Result30 = str[29];
	pDlg->m_Pose2Result31 = str[30];
	pDlg->m_Pose2Result32 = str[31];
	pDlg->m_Pose2Result33 = str[32];
	pDlg->m_Pose2Result34 = str[33];
	pDlg->m_Pose2Result35 = str[34];
	pDlg->m_Pose2Result36 = str[35];
	pDlg->m_Pose2Result37 = str[36];
	pDlg->m_Pose2Result38 = str[37];
	pDlg->m_Pose2Result39 = str[38];
	pDlg->m_Pose2Result40 = str[39];
	pDlg->m_Pose2Result41 = str[40];
	pDlg->m_Pose2Result42 = str[41];
	pDlg->m_Pose2Result43 = str[42];
	pDlg->m_Pose2Result44 = str[43];
	pDlg->m_Pose2Result45 = str[44];
	pDlg->m_Pose2Result46 = str[45];
	pDlg->m_Pose2Result47 = str[46];
	pDlg->m_Pose2Result48 = str[47];
	pDlg->m_Pose2Result49 = str[48];
	pDlg->m_Pose2Result50 = str[49];
	pDlg->m_Pose2Result51 = str[50];
	pDlg->m_Pose2Result52 = str[51];
	pDlg->m_Pose2Result53 = str[52];
	pDlg->m_Pose2Result54 = str[53];
	pDlg->m_Pose2Result55 = str[54];
	pDlg->m_Pose2Result56 = str[55];
	pDlg->m_Pose2Result57 = str[56];
	pDlg->m_Pose2Result58 = str[57];
	pDlg->m_Pose2Result59 = str[58];
	pDlg->m_Pose2Result60 = str[59];
	pDlg->m_Pose2Result61 = str[60];
	pDlg->m_Pose2Result62 = str[61];
	pDlg->m_Pose2Result63 = str[62];
	pDlg->m_Pose2Result64 = str[63];
	pDlg->m_Pose2Result65 = str[64];
	pDlg->m_Pose2Result66 = str[65];
	pDlg->m_Pose2Result67 = str[66];
	pDlg->m_Pose2Result68 = str[67];
	pDlg->m_Pose2Result69 = str[68];
	pDlg->m_Pose2Result70 = str[69];
	pDlg->m_Pose2Result71 = str[70];
	pDlg->m_Pose2Result72 = str[71];
	pDlg->m_Pose2Result73 = str[72];
	pDlg->m_Pose2Result74 = str[73];
	pDlg->m_Pose2Result75 = str[74];
	pDlg->m_Pose2Result76 = str[75];
	pDlg->m_Pose2Result77 = str[76];
	pDlg->m_Pose2Result78 = str[77];
	pDlg->m_Pose2Result79 = str[78];

	pDlg->UpdateData(FALSE);
}

void CSizeKorea2020Doc::OnMeasurePose3()
{
	theSizer.Measure();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame->m_pPose3ResultDlg == NULL)
	{
		pFrame->m_pPose3ResultDlg = new CPose3ResultDialog();
		pFrame->m_pPose3ResultDlg->Create(IDD_POSE3_RESULT_DIALOG);
	}
	CString str[8];
	for (int i = 0; i < 8; ++i)
		str[i].Format("%.2lf ", theSizer.m_Results[i]);

	CPose3ResultDialog *pDlg = pFrame->m_pPose3ResultDlg;
	pDlg->InitDialog();
	pDlg->m_Pose3Result1 = str[0];
	pDlg->m_Pose3Result2 = str[1];
	pDlg->m_Pose3Result3 = str[2];
	pDlg->m_Pose3Result4 = str[3];
	pDlg->m_Pose3Result5 = str[4];
	pDlg->m_Pose3Result6 = str[5];
	pDlg->m_Pose3Result7 = str[6];
	pDlg->m_Pose3Result8 = str[7];
	pDlg->UpdateData(FALSE);
}

void CSizeKorea2020Doc::OnMeasurePose4()
{
	theSizer.Measure();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame->m_pPose4ResultDlg == NULL)
	{
		pFrame->m_pPose4ResultDlg = new CPose4ResultDialog();
		pFrame->m_pPose4ResultDlg->Create(IDD_POSE4_RESULT_DIALOG);
	}
	
	CString str[22];
	for (int i = 0; i < 22; ++i)
		str[i].Format("%.2lf ", theSizer.m_Results[i]);

	CPose4ResultDialog *pDlg = pFrame->m_pPose4ResultDlg;
	pDlg->InitDialog();
	pDlg->m_Pose4Result1 = str[0];
	pDlg->m_Pose4Result2 = str[1];
	pDlg->m_Pose4Result3 = str[2];
	pDlg->m_Pose4Result4 = str[3];
	pDlg->m_Pose4Result5 = str[4];
	pDlg->m_Pose4Result6 = str[5];
	pDlg->m_Pose4Result7 = str[6];
	pDlg->m_Pose4Result8 = str[7];
	pDlg->m_Pose4Result9 = str[8];
	pDlg->m_Pose4Result10 = str[9];
	pDlg->m_Pose4Result11 = str[10];
	pDlg->m_Pose4Result12 = str[11];
	pDlg->m_Pose4Result13 = str[12];
	pDlg->m_Pose4Result14 = str[13];
	pDlg->m_Pose4Result15 = str[14];
	pDlg->m_Pose4Result16 = str[15];
	pDlg->m_Pose4Result17 = str[16];
	pDlg->m_Pose4Result18 = str[17];
	pDlg->m_Pose4Result19 = str[18];
	pDlg->m_Pose4Result20 = str[19];
	pDlg->m_Pose4Result21 = str[20];
	pDlg->m_Pose4Result22 = str[21];
	pDlg->UpdateData(FALSE);
}

void CSizeKorea2020Doc::OnMeasurePose5()
{
	theSizer.Measure();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame->m_pPose3ResultDlg == NULL)
	{
		pFrame->m_pPose3ResultDlg = new CPose3ResultDialog();
		pFrame->m_pPose3ResultDlg->Create(IDD_POSE3_RESULT_DIALOG);
	}
	CString str[8];
	for (int i = 0; i < 8; ++i)
		str[i].Format("%.1lf ", theSizer.m_Results[i]);

	CPose3ResultDialog *pDlg = pFrame->m_pPose3ResultDlg;
	pDlg->InitDialog();
	pDlg->m_Pose3Result1 = str[0];
	pDlg->m_Pose3Result2 = str[1];
	pDlg->m_Pose3Result3 = str[2];
	pDlg->m_Pose3Result4 = str[3];
	pDlg->m_Pose3Result5 = str[4];
	pDlg->m_Pose3Result6 = str[5];
	pDlg->m_Pose3Result7 = str[6];
	pDlg->m_Pose3Result8 = str[7];
	pDlg->UpdateData(FALSE);
}


// CSizeKorea2020Doc serialization

void CSizeKorea2020Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CSizeKorea2020Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CSizeKorea2020Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSizeKorea2020Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSizeKorea2020Doc diagnostics

#ifdef _DEBUG
void CSizeKorea2020Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSizeKorea2020Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSizeKorea2020Doc commands


void CSizeKorea2020Doc::InitVariable() {
	if (m_Vertex) {
		delete[] m_Vertex;
		m_Vertex = NULL;
	}
	if (m_Color) {
		delete[] m_Color;
		m_Color = NULL;
	}
	if (m_Normal) {
		delete[] m_Normal;
		m_Normal = NULL;
	}
	if (m_FaceIdx) {
		delete[] m_FaceIdx;
		m_FaceIdx = NULL;
	}

	m_SlicePtList.DeleteAll();
	m_PtOnRefPlnList.DeleteAll();

	///////////////////////////////////////////////////////////////////////////
	int i = 0;
	for (i = 0; i < LANDMARK_NUM_POSE2; m_LandMarkPose2[i] = GLandMark(), i++);
	for (i = 0; i < LANDMARK_NUM_HELP; m_LandMarkHelp[i] = GLandMark(), i++);


	for (i = 0; i < RESULT_NUM_POSE2; m_ResultPathPose2[i].DeleteAll(), i++);

	if (!m_bSizeResult) {
		for (i = 0; i < RESULT_NUM_POSE2; m_ResultPose2[i] = 0.0f, i++);
	}

	if (m_pSlicesPtList) {
		delete[]	m_pSlicesPtList;
		m_pSlicesPtList = NULL;
	}
	if (m_pSliceAnglePtList) {
		delete[]	m_pSliceAnglePtList;
		m_pSliceAnglePtList = NULL;
	}


	if (m_pAlignedPointsV_X) {
		delete[]	m_pAlignedPointsV_X;
		m_pAlignedPointsV_X = NULL; // linked list that store points according to width.
	}
	if (m_pAlignedPointsV_Y) {
		delete[]	m_pAlignedPointsV_Y;
		m_pAlignedPointsV_Y = NULL; // linked list that store points according to height.
	}


	if (m_bLandmarkFind) {
		m_FRSilhouetteV.DeleteAll();
		m_FLSilhouetteV.DeleteAll();
		m_SFSilhouetteV.DeleteAll();
		m_SFSilhouetteV1.DeleteAll();
		m_SBSilhouetteV.DeleteAll();

		m_SagittalPtsB.DeleteAll();
		m_SagittalPtsF.DeleteAll();
		m_FNeckCrv.DeleteAll();

		m_ArmRInSilhouetteV.DeleteAll();
		m_ArmRFSilhouetteV.DeleteAll();
	}


	///////////////////////////////////////////////////////////////////////////////	
	m_vNum = 0;
	m_iNum = 0;
	m_MarkIdx = -1;
	m_LandmarkNumExport = 35;
	////////////////////////////////////////////////////////////////////////////
	m_Height = 0.0f;
	m_MinX = 10000.0f;
	m_MaxX = -10000.0f;
	m_MinY = 10000.0f;
	m_MaxY = -10000.0f;
	m_MinZ = 10000.0f;
	m_MaxZ = -10000.0f;
	m_MinXOrigin = 10000.0f;
	m_MaxXOrigin = -10000.0f;
	m_MinYOrigin = 10000.0f;
	m_MaxYOrigin = -10000.0f;
	m_MinZOrigin = 10000.0f;
	m_MaxZOrigin = -10000.0f;
	////////////////////////////////////////////////////////////////////////////
	m_bOpened = false;
	m_bSizeResult = false;

	//////////////////////////////////////////////////////////////////////////
	m_MarkIdx = -1;
	m_LandmarkNumExport = 35;
	m_ResultSizeNumExport = 70;
	//////////////////////////////////////////////////////////////////////////

	m_bReplaceResult_Underbust = false;

	RefAngle[0] = 0.34906f;   //목둘레 20
	RefAngle[1] = 1.48353f;   //팔 몸통분리 오른 85.0f,
	RefAngle[2] = -1.48353f;   //팔 몸통분리 왼  85.0f,
	RefAngle[3] = 1.48353f;    //팔, 몸통 분리각도: 허리, 엉덩이 높이 [3][4]
	RefAngle[4] = -1.48353f;
	RefAngle[5] = 1.54; //1.48353f;    //팔, 몸통 분리각도: 겨드랑아래 분리 [5][6]
	RefAngle[6] = -1.54;//-1.48353f;


	//------------------- For Auto Landmark --------------------
	//----------------------------------------------------------
	m_nCount_Y = 0;
	m_nCount_X = 0;
	m_nCount_Z = 0;
	m_HInterval = 2;


	m_bArrangedXY = false;
	m_bArrangedY = false;
	m_bArrangedX = false;
	m_bSizeResult = false;
	m_bLandmarkFind = false;
	m_bParam = false;


	m_Height = 0.0f;
	m_ShoulderAngleR = 0.0f;
	m_ShoulderAngleL = 0.0f;
	m_Obesity = 0.0f;
	m_ArmObesity = 0.0f;
	m_BustCObesity = 0.0f;
	m_UnderBustCObesity = 0.0f;
	m_Armslop = 0.0f;
	m_ArmslopDown = 0.0f;

	m_NeckBackAngle = 0.0f;
	m_NeckFrontAngle = 0.0f;
	m_NeckFSAngle = 0.0f;

	m_NeckFBVector = m_NeckBVector = m_NeckFVector = GVector3f(0.0f, 0.0f, 0.0f);

	m_Error = ASERR_OK;

	m_SlicesNum = 0;

	theSizer.Reset();
	theSlicer.Reset();
}

void CSizeKorea2020Doc::InitVariable_Landmark() {
	if (m_Vertex) {
		delete[] m_Vertex;
		m_Vertex = NULL;
	}
	if (m_Color) {
		delete[] m_Color;
		m_Color = NULL;
	}
	if (m_Normal) {
		delete[] m_Normal;
		m_Normal = NULL;
	}
	if (m_FaceIdx) {
		delete[] m_FaceIdx;
		m_FaceIdx = NULL;
	}

	m_SlicePtList.DeleteAll();
	m_PtOnRefPlnList.DeleteAll();

	///////////////////////////////////////////////////////////////////////////
	int i = 0;
	for (i = 0; i < LANDMARK_NUM_POSE2; m_LandMarkPose2[i] = GLandMark(), i++);
	for (i = 0; i < LANDMARK_NUM_HELP; m_LandMarkHelp[i] = GLandMark(), i++);


	for (i = 0; i < RESULT_NUM_POSE2; m_ResultPathPose2[i].DeleteAll(), i++);

	if (!m_bSizeResult) {
		for (i = 0; i < RESULT_NUM_POSE2; m_ResultPose2[i] = 0.0f, i++);
	}

	if (m_pSlicesPtList) {
		delete[]	m_pSlicesPtList;
		m_pSlicesPtList = NULL;
	}
	if (m_pSliceAnglePtList) {
		delete[]	m_pSliceAnglePtList;
		m_pSliceAnglePtList = NULL;
	}


	if (m_pAlignedPointsV_X) {
		delete[]	m_pAlignedPointsV_X;
		m_pAlignedPointsV_X = NULL; // linked list that store points according to width.
	}
	if (m_pAlignedPointsV_Y) {
		delete[]	m_pAlignedPointsV_Y;
		m_pAlignedPointsV_Y = NULL; // linked list that store points according to height.
	}


	if (m_bLandmarkFind) {
		m_FRSilhouetteV.DeleteAll();
		m_FLSilhouetteV.DeleteAll();
		m_SFSilhouetteV.DeleteAll();
		m_SFSilhouetteV1.DeleteAll();
		m_SBSilhouetteV.DeleteAll();

		m_SagittalPtsB.DeleteAll();
		m_SagittalPtsF.DeleteAll();
		m_FNeckCrv.DeleteAll();

		m_ArmRInSilhouetteV.DeleteAll();
		m_ArmRFSilhouetteV.DeleteAll();
	}


	///////////////////////////////////////////////////////////////////////////////	
	m_vNum = 0;
	m_iNum = 0;
	m_MarkIdx = -1;
	m_LandmarkNumExport = 35;
	////////////////////////////////////////////////////////////////////////////
	m_Height = 0.0f;
	m_MinX = 10000.0f;
	m_MaxX = -10000.0f;
	m_MinY = 10000.0f;
	m_MaxY = -10000.0f;
	m_MinZ = 10000.0f;
	m_MaxZ = -10000.0f;
	m_MinXOrigin = 10000.0f;
	m_MaxXOrigin = -10000.0f;
	m_MinYOrigin = 10000.0f;
	m_MaxYOrigin = -10000.0f;
	m_MinZOrigin = 10000.0f;
	m_MaxZOrigin = -10000.0f;
	////////////////////////////////////////////////////////////////////////////
	//m_bOpened	=  false;
	//m_bSizeResult	=  false;

	//////////////////////////////////////////////////////////////////////////
	m_MarkIdx = -1;
	m_LandmarkNumExport = 35;
	m_ResultSizeNumExport = 70;
	//////////////////////////////////////////////////////////////////////////

	m_bReplaceResult_Underbust = false;

	//RefAngle[0] = 0.34906f;   //목둘레 20
	//RefAngle[1] = 1.48353f;   //팔 몸통분리 오른 85.0f,
	//RefAngle[2] = 1.48353f;   //팔 몸통분리 왼  85.0f,
	//RefAngle[3] = 1.48353f;
	//RefAngle[4] = 1.48353f;


	//------------------- For Auto Landmark --------------------
	//----------------------------------------------------------
	m_nCount_Y = 0;
	m_nCount_X = 0;
	m_nCount_Z = 0;
	m_HInterval = 2;


	m_bArrangedXY = false;
	m_bArrangedY = false;
	m_bArrangedX = false;
	m_bLandmarkFind = false;
	m_bParam = false;


	m_Height = 0.0f;
	m_ShoulderAngleR = 0.0f;
	m_ShoulderAngleL = 0.0f;
	m_Obesity = 0.0f;
	m_ArmObesity = 0.0f;
	m_BustCObesity = 0.0f;
	m_UnderBustCObesity = 0.0f;
	m_Armslop = 0.0f;
	m_ArmslopDown = 0.0f;

	m_NeckBackAngle = 0.0f;
	m_NeckFrontAngle = 0.0f;
	m_NeckFSAngle = 0.0f;

	m_NeckFBVector = m_NeckBVector = m_NeckFVector = GVector3f(0.0f, 0.0f, 0.0f);

	m_Error = ASERR_OK;
}

void CSizeKorea2020Doc::BoundingBox() {
	int i;
	m_MinY = 10000.0f;
	m_MaxY = -10000.0f;
	for (i = 0; i < m_vNum; i++)
	{
		m_MinX = MIN(m_MinX, m_Vertex[i].X);
		m_MaxX = MAX(m_MaxX, m_Vertex[i].X);

		m_MinY = MIN(m_MinY, m_Vertex[i].Y);
		m_MaxY = MAX(m_MaxY, m_Vertex[i].Y);

		m_MinZ = MIN(m_MinZ, m_Vertex[i].Z);
		m_MaxZ = MAX(m_MaxZ, m_Vertex[i].Z);
	}

	m_MaxYOrigin = m_MaxY;
	m_MinYOrigin = m_MinY;
	m_MaxXOrigin = m_MaxX;
	m_MinXOrigin = m_MinX;
	m_MaxZOrigin = m_MaxZ;
	m_MinZOrigin = m_MinZ;

	m_Width = m_MaxX - m_MinX;
	m_Depth = m_MaxZ - m_MinZ;
	m_Height = m_MaxY - m_MinY;
}

void CSizeKorea2020Doc::UpdateOldLandMark(int ScanPose) {
	int i = 0;
	switch (ScanPose)
	{
	case 1:
	case 2:
		for (i = 0; i < LANDMARK_NUM_POSE2; m_OldLandMarkPose2[i] = m_LandMarkPose2[i], i++);
		break;
	}
}

int CSizeKorea2020Doc::GetNumberOfLandMarks() {
	if (m_bLandmarkFind)
		return m_LandmarkNumExport;
	else
		return 0;
}

int CSizeKorea2020Doc::GetNumberOfSizes() {
	if (m_bSizeResult)
		return m_ResultSizeNumExport;
	else
		return 0;
}
//
//void CSizeKorea2020Doc::GetLandMark(int i, float* xyz, char* name) {
//	if (m_bLandmarkFind)
//	{
//		if (i < m_LandmarkNumExport) {
//			xyz[0] = m_LandMarkPose2[i][0] + m_MinXOrigin;
//			xyz[1] = m_LandMarkPose2[i][1] + m_MinYOrigin;
//			xyz[2] = m_LandMarkPose2[i][2];
//			strcpy(name, m_LandMarkName[i]);
//		}
//		else {
//			xyz[0] = -1.0f;
//			xyz[1] = -1.0f;
//			xyz[2] = -1.0f;
//			strcpy(name, "");
//		}
//	}
//	else {
//		xyz[0] = -1.0f;
//		xyz[1] = -1.0f;
//		xyz[2] = -1.0f;
//		strcpy(name, "");
//	}
//}

//void CSizeKorea2020Doc::GetSize(int i, float* size, char* name) {
//	if (m_bSizeResult) {
//		*size = m_ResultPose2[i];
//		strcpy(name, m_SizeName[i]);
//	}
//	else
//		size = 0;
//}

void CSizeKorea2020Doc::InputMesh(int nNode, float* ppNodes, int nElement, int* ppElements) {
	if (m_bOpened) {
		if (m_bSizeResult)  InitVariable_Landmark();
		else InitVariable();
	}

	m_Vertex = new GVector3f[nNode];   	//*m_Color = new GVector3f[vnum];
	m_FaceIdx = new int[nElement];

	m_vNum = nNode;
	m_iNum = nElement;

	float min_y = 10000.0, max_y = -10000.0;
	float min_x = 10000.0, max_x = -10000.0;
	float min_z = 10000.0, max_z = -10000.0;

	for (int k = 0; k < nNode; k++)
	{
		m_Vertex[k][0] = ppNodes[k * 3];  //ppNodes++
		m_Vertex[k][1] = ppNodes[k * 3 + 1];
		m_Vertex[k][2] = ppNodes[k * 3 + 2];


		min_x = MIN(min_x, m_Vertex[k][0]);
		max_x = MAX(max_x, m_Vertex[k][0]);

		min_y = MIN(min_y, m_Vertex[k][1]);
		max_y = MAX(max_y, m_Vertex[k][1]);

		min_z = MIN(min_z, m_Vertex[k][2]);
		max_z = MAX(max_z, m_Vertex[k][2]);
	}

	for (int j = 0; j < nElement; j++)
	{
		m_FaceIdx[j] = *ppElements++;
	}

	m_bOpened = true;
	m_MaxY = max_y;
	m_MinY = min_y;
	m_MaxX = max_x;
	m_MinX = min_x;
	m_MaxZ = max_z;
	m_MinZ = min_z;
	m_MaxYOrigin = max_y;
	m_MinYOrigin = min_y;
	m_MaxXOrigin = max_x;
	m_MinXOrigin = min_x;
	m_MaxZOrigin = max_z;
	m_MinZOrigin = min_z;

	// 환경변수들을 default로
	RefAngle[0] = 20.0f;      //목둘레
	RefAngle[1] = 1.48353f;   //팔 몸통분리 오른 85.0f,
	RefAngle[2] = -1.48353f;   //팔 몸통분리 왼  85.0f,
	RefAngle[3] = 1.48353f;
	RefAngle[4] = -1.48353f;
	RefAngle[5] = 1.48353f;
	RefAngle[6] = -1.48353f;

	m_bOpened = true;

	if ((m_MaxX - m_MinX) < (m_MaxZ - m_MinZ)) {
		m_Error = ASERR_WRONGDATA;
	}
	else {
		OnArrange3DData();
	}
}

void CSizeKorea2020Doc::MeasureBody() {
	if (m_bOpened) {
		if (m_bLandmarkFind)
			OnMeasurePose2();
	}
}

void CSizeKorea2020Doc::SetManWoman(bool sex) {
	m_bWoman = sex;
}

// mj::Set gender here
void CSizeKorea2020Doc::OnSetGender()
{
	m_bWoman = !m_bWoman;
	theSizer.SetGender();
}

void CSizeKorea2020Doc::MoveModel(float dx, float dy, float dz) {
	for (long i = 0; i < m_vNum; i++)
	{
		m_Vertex[i].X += dx;
		m_Vertex[i].X += dy;
		m_Vertex[i].X += dz;
	}

	int num;

	for (int j = 0; j < m_nCount_X; j += 2)
	{
		num = m_pAlignedPointsV_X[j].m_nCount;

		for (int k = 0; k < num; k++)
		{
			m_pAlignedPointsV_X[j][k].X += dx;
			m_pAlignedPointsV_X[j][k].Y += dy;
			m_pAlignedPointsV_X[j][k].Z += dz;
		}

	}

	for (int j = 0; j < m_nCount_Y; j += 2)
	{
		num = m_pAlignedPointsV_Y[j].m_nCount;

		for (int k = 0; k < num; k++)
		{
			m_pAlignedPointsV_Y[j][k].X += dx;
			m_pAlignedPointsV_Y[j][k].Y += dy;
			m_pAlignedPointsV_Y[j][k].Z += dz;
		}

	}


	for (int j = 0; j < RESULT_NUM_POSE2; j++) {

		num = m_ResultPathPose2[j].m_nCount;

		for (int k = 0; k < num; k++)
		{
			m_ResultPathPose2[j][k].X += dx;
			m_ResultPathPose2[j][k].Y += dy;
			m_ResultPathPose2[j][k].Z += dz;
		}
	}

	num = m_FRSilhouetteV.m_nCount;
	for (int k = 0; k < num; k++)
	{
		m_FRSilhouetteV[k].X += dx;
		m_FRSilhouetteV[k].Y += dy;
		m_FRSilhouetteV[k].Z += dz;
	}

	num = m_FLSilhouetteV.m_nCount;
	for (int k = 0; k < num; k++)
	{
		m_FLSilhouetteV[k].X += dx;
		m_FLSilhouetteV[k].Y += dy;
		m_FLSilhouetteV[k].Z += dz;
	}

	num = m_SFSilhouetteV.m_nCount;
	for (int k = 0; k < num; k++)
	{
		m_SFSilhouetteV[k].X += dx;
		m_SFSilhouetteV[k].Y += dy;
		m_SFSilhouetteV[k].Z += dz;
	}

	num = m_SFSilhouetteV1.m_nCount;
	for (int k = 0; k < num; k++)
	{
		m_SFSilhouetteV1[k].X += dx;
		m_SFSilhouetteV1[k].Y += dy;
		m_SFSilhouetteV1[k].Z += dz;
	}

	num = m_SBSilhouetteV.m_nCount;
	for (int k = 0; k < num; k++)
	{
		m_SBSilhouetteV[k].X += dx;
		m_SBSilhouetteV[k].Y += dy;
		m_SBSilhouetteV[k].Z += dz;
	}

	num = m_SagittalPtsB.m_nCount;
	for (int k = 0; k < num; k++)
	{
		m_SagittalPtsB[k].X += dx;
		m_SagittalPtsB[k].Y += dy;
		m_SagittalPtsB[k].Z += dz;
	}

	num = m_SagittalPtsF.m_nCount;
	for (int k = 0; k < num; k++)
	{
		m_SagittalPtsF[k].X += dx;
		m_SagittalPtsF[k].Y += dy;
		m_SagittalPtsF[k].Z += dz;
	}

	num = m_FNeckCrv.m_nCount;
	for (int k = 0; k < num; k++)
	{
		m_FNeckCrv[k].X += dx;
		m_FNeckCrv[k].Y += dy;
		m_FNeckCrv[k].Z += dz;
	}
}

int  CSizeKorea2020Doc::GetErrorState() {
	return m_Error;
}

void CSizeKorea2020Doc::MakeSliceCrv() {
	GPlane lplane, rplane;
	double tan1 = tan(3.14159265 / 2.0 - RefAngle[5]);
	double tan2 = tan(3.14159265 / 2.0 + RefAngle[6]);
	rplane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkHelp[1]);
	lplane.MakePlane(GVector3f(1.0f, tan2, 0.0f), m_LandMarkHelp[2]);


	GObList<GVector3f> CrossPtList1, CrossPtList2;

	int crotchH = ROUND(m_LandMarkPose2[25].Y);

	if (m_SliceHeight < crotchH)
	{
		GObNode<GVector3f> *pNode = m_SlicePtList.m_pLeftEnd;

		while (pNode)
		{
			GVector3f pt = pNode->data;
			if (pt.X < m_LandMarkHelp[0].X)
				CrossPtList1.Insert(pt);
			else
				CrossPtList2.Insert(pt);

			pNode = pNode->m_pRight;
		}
	}
}

void CSizeKorea2020Doc::FindAllLandmarks()
{
	if (!m_bOpened)
		return;
	
	switch (m_ScanPose) 
	{
	case 1:
		theSizer.FindLandmarks();
		break;

	case 2:
		FindPose2Landmarks();
		// mj::EgSizer에 구해진 landmark를 할당하기 위해 호출
		theSizer.FindLandmarks();
		break;

	case 3:
		theSizer.FindLandmarks();
		break;

	case 4:
		theSizer.FindLandmarks();
		break;

	case 5:
		theSizer.FindLandmarks();
		break;

	default:
		break;
	}
	m_bLandmarkFind = true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//														POSE 2													 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CSizeKorea2020Doc::Pre_FindLandmarks() {
	if (!m_bOpened) return;

	if (!m_bArrangedXY)
		OnArrange3DData();

	// 한번 찾은 다음에 transform하고 다시찾을 경우에, FindLandmarks 함수 안에서 생성되었던, 기존 실루엣이나.. 등등을 지운다
	if (m_bLandmarkFind) {
		m_FRSilhouetteV.DeleteAll();
		m_FLSilhouetteV.DeleteAll();
		m_SBSilhouetteV.DeleteAll();
		m_SFSilhouetteV.DeleteAll();

		m_SagittalPtsF.DeleteAll();
		m_SagittalPtsB.DeleteAll();
		m_FNeckCrv.DeleteAll();

		m_ArmRInSilhouetteV.DeleteAll();
		m_ArmRFSilhouetteV.DeleteAll();
	}

	// find top head point
	FindTopHeadPoint();

	m_bLandmarkFind = true;

	FindCrotchTempV();

	FindArmEndV_Temp();

	if (m_LandMarkHelp[0].Y < (0.3f*m_Height)) { // 임시 샅점이 잘못 찾아지면 나간다.
		m_Error = ASERR_CROTCH;
		return;
	}

	FindArmPitTempV();      //---- temp armpit point

	if (m_LandMarkHelp[1].Y < (0.5f*m_Height)) { // 임시 겨드랑점이 잘못 찾아지면 나간다.
		m_Error = ASERR_ARMPIT;
		return;
	}

	Calc_RefAngleArmsV();

	MakeFSilhouetteV();    //---- silhouette 만들기
}


void CSizeKorea2020Doc::FindPose2Landmarks() {
	// mj::Delete global variables
	/*
	for (int i = 0; i < NEW_LANDMARK_NUM_POSE2; i++) {
		delete m_nLandmarkPose2[i];
	}
	*/

	Pre_FindLandmarks();
	// find hip point, calf, gluteal fold
	FindHipPointV();
	// find crotch point
	FindCrotchFB();
	// find arm end point
	FindArmEndV();
	// find wrist point
	FindWristPt_InV();        //- 이 함수 안에서 Calc_ArmSlop();를 호출해서 팔기울기 계산
	FindWristPt_FV();
	Calc_ArmC_Obesity_Temp();
	FindArmPitFoldV();
	FindArmPit_ArmpitFold();
	RefAngle[5] = atan((m_LandMarkPose2[9].Y - m_LandMarkHelp[42].Y) / (m_LandMarkPose2[9].X - m_LandMarkHelp[42].X));
	RefAngle[6] = atan((m_LandMarkPose2[10].Y - m_LandMarkHelp[43].Y) / (m_LandMarkPose2[10].X - m_LandMarkHelp[43].X));
	Calc_ArmC_Obesity();	   //- 이 함수 안에서 Calc_ArmSlop();를 호출
	// find bust point
	if (m_bWoman)
		FindBustPoint_SlopV();		  //downratio=0.67f;
	else
		FindBustPoint_Slop_mV();	  //downratio=0.685f;
	/// find under bust point
	//FindBustUnderPoint();
	FindBustUnderPointV();
	Calc_TorsoC_ObesityV();  //---- Determine How people fat by underbust.circum/height (waist.cirucm/height)
	FindNeckSideTempV1();
	if (m_Error)
		return;
	// 정중시상면 만들기
	MakeSagittalSection();
	// find under bust point 3
	if (m_bWoman)
		FindBustUnderPointC();	//참고점: 정중시상면
	//임시 목앞점
	FindNeckFrontTemp1();		// 여기서 턱안점도 찾는다.  참고점: 정중시상면

	//진동깊이점
	FindArmPitH_Back();			// 참고점: 정중시상면-뒤

	// find side neck point
	FindNeckSidePointV();


	// find back fneck, sneck point
	FindNeckBackSideFront();   //목옆점, 임시목앞점의 등쪽점 찾기, 목뒤벡터 구하기
	Calc_NeckAngles(); //목뒤점에 목뒤위각도, 목옆점z에 목앞옆각도 사용을 위해 미리 계산

	FindNeckBackPoint();

	if (!m_bWoman)
		FindBustPoint_Ratio_mV();


	// find waist back concave point
	FindWaistBack_ConcavePtV();     // 참고점: 측면실루엣

	// find front neck point
	FindNeckFrontPointV1();           // 참고점: 정중시상면-앞
	FindNeckFrontPoint_maxZV();      //목앞점 단면에서 앞으로 가장 튀어나온 좌우점 찾기 - 목밑둘레를 위해

	// 목옆점 Z조정
	FindNeckSidePoint_Z();
	FindNeckBackPoint();			// 조정된 목옆점으로 다시 목뒤점 설정
	FindNeckSidePoint_Z();			// 목뒤점으로 목옆점 다시 조정

	// find Thyroid neck point
	FindNeckThyroidPointV();			// 참고점: 정중시상면-앞

	// find shoulder point 1
	FindSP2_1V();
	FindShoulderPtConvexV();        //----임시 shoulder point  - 겨드랑점 탐색에 사용됨

	// find shoulder point 2
	FindShoulderPoint_SP2_ConvexV();

	FindArmpitConcavePtsV();  // 겨드랑 오목점들

	FindShoulderPointZ1_V();  //start

	FindShouderMid();

	// find armscye
	FindArmscyePt_1();

	// find abdominal porminence point
	FindAbdominalPtV();

	// find waist right point
	if (m_bWoman) FindWaistRightPt_wV();
	else 	FindWaistRightPt_mV();

	FindWaistF_B_LV();

	FindNavelPtV();
	FindNavel_RPtV();


	// find elbow points
	FindElbowPtV();

	// find knee point
	FindKneePtV();

	// find ankle point
	FindLegSmallPtV();

	// find ankle point
	FindAnklePtV();

	FindBackProjectingPt();

	FindFootInPt();

	FindShoulderPoint1();


	// mj::m_nLandmarkPose2에 m_LandMarkPose2를 이관
	m_nLandmarkPose2[POSE2_LANDMARK_TOPHEAD] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[1].X, m_LandMarkPose2[1].Y, m_LandMarkPose2[1].Z));


	m_nLandmarkPose2[POSE2_LANDMARK_NECK_THYROID] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[2].X, m_LandMarkPose2[2].Y, m_LandMarkPose2[2].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_NECK_FRONT] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[3].X, m_LandMarkPose2[3].Y, m_LandMarkPose2[3].Z));

	m_nLandmarkPose2[POSE2_LANDMARK_NECK_SIDE_R] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[4].X, m_LandMarkPose2[4].Y, m_LandMarkPose2[4].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_NECK_SIDE_L] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[5].X, m_LandMarkPose2[5].Y, m_LandMarkPose2[5].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_NECK_BACK] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[6].X, m_LandMarkPose2[6].Y, m_LandMarkPose2[6].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_SHOULDERSIDE_R] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[7].X, m_LandMarkPose2[7].Y, m_LandMarkPose2[7].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_SHOULDERSIDE_L] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[8].X, m_LandMarkPose2[8].Y, m_LandMarkPose2[8].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_ARMPITFOLD_FRONT_R] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[44].X, m_LandMarkPose2[44].Y, m_LandMarkPose2[44].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_ARMPITFOLD_BACK_R] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[45].X, m_LandMarkPose2[45].Y, m_LandMarkPose2[45].Z));

	// mj:: 겨드랑점을 앞뒤접힘점 중앙 수준으로 이동
	// 왼쪽 앞뒤접힘점이 없으므로 왼쪽 겨드랑점은 오른쪽 겨드랑점의 깊이수준으로 통일
	float armpitDepth = (m_nLandmarkPose2[POSE2_LANDMARK_ARMPITFOLD_FRONT_R]->m_Pos[2] + m_nLandmarkPose2[POSE2_LANDMARK_ARMPITFOLD_BACK_R]->m_Pos[2]) / 2;
	m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_R] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[9].X, m_LandMarkPose2[9].Y, m_LandMarkPose2[9].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_L] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[10].X, m_LandMarkPose2[10].Y, m_LandMarkPose2[10].Z));

	m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_R]->m_Pos[2] = armpitDepth;
	m_nLandmarkPose2[POSE2_LANDMARK_ARMPIT_L]->m_Pos[2] = armpitDepth;

	m_nLandmarkPose2[POSE2_LANDMARK_BP_R] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[16].X, m_LandMarkPose2[16].Y, m_LandMarkPose2[16].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_BP_L] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[17].X, m_LandMarkPose2[17].Y, m_LandMarkPose2[17].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_UNDERBUST] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[18].X, m_LandMarkPose2[18].Y, m_LandMarkPose2[18].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_WAIST] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[19].X, m_LandMarkPose2[19].Y, m_LandMarkPose2[19].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_WAIST_L] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[20].X, m_LandMarkPose2[20].Y, m_LandMarkPose2[20].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_WAIST_FRONT] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[21].X, m_LandMarkPose2[21].Y, m_LandMarkPose2[21].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_WAIST_BACK] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[22].X, m_LandMarkPose2[22].Y, m_LandMarkPose2[22].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_STOMACH_EXTRUDE] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[23].X, m_LandMarkPose2[23].Y, m_LandMarkPose2[23].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_NAVEL] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[35].X, m_LandMarkPose2[35].Y, m_LandMarkPose2[35].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_HIP_EXTRUDE] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[24].X, m_LandMarkPose2[24].Y, m_LandMarkPose2[24].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_CROTCH] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[25].X, m_LandMarkPose2[25].Y, m_LandMarkPose2[25].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_UNDERHIP] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[26].X, m_LandMarkPose2[26].Y, m_LandMarkPose2[26].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_KNEE_MID] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[27].X, m_LandMarkPose2[27].Y, m_LandMarkPose2[27].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_CALF_EXTRUDE] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[28].X, m_LandMarkPose2[28].Y, m_LandMarkPose2[28].Z));
	// m_nLandmarkPose2[POSE2_LANDMARK_ANKLE] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[30].X, m_LandMarkPose2[30].Y, m_LandMarkPose2[30].Z));

	m_nLandmarkPose2[POSE2_LANDMARK_WRIST] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[32].X, m_LandMarkPose2[32].Y, m_LandMarkPose2[32].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_WRIST_OUTSIDE] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[33].X, m_LandMarkPose2[33].Y, m_LandMarkPose2[33].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_HAND] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[34].X, m_LandMarkPose2[34].Y, m_LandMarkPose2[34].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_ELBOW] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[31].X, m_LandMarkPose2[31].Y, m_LandMarkPose2[31].Z));
	m_nLandmarkPose2[POSE2_LANDMARK_SHOULDER_MID] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[36].X, m_LandMarkPose2[36].Y, m_LandMarkPose2[36].Z));

	m_nLandmarkPose2[POSE2_LANDMARK_UNDERCALF] = FindClosestVert(m_ScanMesh, EgPos(m_LandMarkPose2[29].X, m_LandMarkPose2[29].Y, m_LandMarkPose2[29].Z));

	// 위치 이상으로 가쪽복사점 새로 찾는다 (20. 10. 4) 
	// Undercalf를 사용하므로 여기로 위치 이동
	m_nLandmarkPose2[POSE2_LANDMARK_ANKLE] = FindAnklePt();


	// 새로 추가된 측정점
	m_nLandmarkPose2[POSE2_LANDMARK_MIDRIFF] = FindMidriffPt();
	m_nLandmarkPose2[POSE2_LANDMARK_NAVEL_BACK] = FindNavelBackPt();
	m_nLandmarkPose2[POSE2_LANDMARK_TOPHIP] = FindTopHipPt();
	m_nLandmarkPose2[POSE2_LANDMARK_UPPERHIP] = FindUpperHipPt();
	m_nLandmarkPose2[POSE2_LANDMARK_THIGH_MID] = FindThighMidPt();
	m_nLandmarkPose2[POSE2_LANDMARK_UNDERKNEE] = FindUnderkneePt();
	m_nLandmarkPose2[POSE2_LANDMARK_CALF_EXTRUDE_FRONT] = FindCalfExtrudeFrontPt();
	m_nLandmarkPose2[POSE2_LANDMARK_ANKLE_INSIDE] = FindAnkleInsidePt();
	m_nLandmarkPose2[POSE2_LANDMARK_SHOULDER_R] = FindShoulderRPt();
	m_nLandmarkPose2[POSE2_LANDMARK_SHOULDER_L] = FindShoulderLPt();


	// 샅점이 뒤에 찍혀서 측정부위(샅앞길이, 샅뒤길이)에 문제가 생기므로 샅점을 자세 1과 같은 방법으로 구한다 (20. 9. 26)
	m_nLandmarkPose2[POSE2_LANDMARK_CROTCH] = FindCrotchPt();


	_cprintf("\nDone finding landmarks!\n");
}

///////////////////////////////////////////////
//			mj::New Landmarks(2020. 8)		 //
///////////////////////////////////////////////
// 마지막 수정일 : 2020. 11. 23
EgVertex *CSizeKorea2020Doc::FindAnklePt() {
	EgVertex *AnklePt = NULL;

	// 종아리 아래점 수준에서 slice해서 최소 x값을 가진 점을 seed point로
	EgSlicer theSlicer;
	theSlicer.Slice(m_ScanMesh, 1, m_nLandmarkPose2[POSE2_LANDMARK_UNDERCALF]->m_Pos[1] / m_MaxY, -1, 0, -1, 0, -1, 1, true);

	EgPos minX = EgPos(INFINITY, 0, 0);
	for (EgPos pos : theSlicer.m_CutLines[0]) {
		if (pos.m_Coords[0] < minX.m_Coords[0]) {
			minX = pos;
		}
	}

	// 3cm 근방에서 elliptic한 점
	AnklePt = SK_Elliptic_Vertex(FindClosestVert(m_ScanMesh, minX), 30);

	if (AnklePt != NULL) {
		m_nLandmarkPose2[POSE2_LANDMARK_ANKLE] = AnklePt;
	}
	else {
		_cprintf("Ankle NOT FOUND!\n");
		m_nLandmarkPose2[POSE2_LANDMARK_ANKLE] = new EgVertex();
	}

	return AnklePt;
}


EgVertex *CSizeKorea2020Doc::FindCrotchPt() {
	EgVertex *CrotchPt = NULL;
	EgVertex *StomachExtrudePt = m_nLandmarkPose2[POSE2_LANDMARK_STOMACH_EXTRUDE];

	// 배돌출점 높이 이하 ~ 붙어서 스캔된 다리 부분을 지나는 Sagittal slicer 생성.
	EgSlicer sagittalSlicer;
	sagittalSlicer.Slice(m_ScanMesh, 0, 0.5, -1, 1, -1, 1, -1, 1, false);

	// Sagittal slicer에서 최하단 점을 구하고,
	EgPos lowestCrotchPt;
	EgPos minY = EgPos(0, INFINITY, 0);

	for (EgPos pos : sagittalSlicer.m_CutLines[0]) {
		if (pos.m_Coords[1] < minY.m_Coords[1]) {
			lowestCrotchPt = pos;
			minY.m_Coords[1] = pos.m_Coords[1];
		}
	}

	// lowestCrotchPt를 기준으로 배돌출점 이하의 앞뒤로 나뉜 silhouette을 만든다.
	std::vector<EgPos> frontalSilhouette;
	for (EgPos pos : sagittalSlicer.m_CutLines[0]) {
		if (pos.m_Coords[2] > lowestCrotchPt.m_Coords[2] && pos.m_Coords[1] < StomachExtrudePt->m_Pos[1]) {
			frontalSilhouette.push_back(pos);
		}
	}

	// 최하단 점과 배돌출점을 지나는 곡선을 구한 후,
	EgLine tmp_l = EgLine(lowestCrotchPt, StomachExtrudePt->m_Pos);

	// frontal silhouette의 점 중 거리가 가장 먼 점을 샅점돌출 부위로 정의한다.
	EgPos CrotchExtrudePt = EgPos();
	double maxDist = -INFINITY;
	for (EgPos pos : frontalSilhouette) {
		double distance = dist(tmp_l, pos);
		if (distance > maxDist) {
			CrotchExtrudePt = pos;
			maxDist = distance;
		}
	}

	std::vector<EgPos> frontalSilhouette2;
	for (EgPos pos : frontalSilhouette) {
		if (pos.m_Coords[2] > lowestCrotchPt.m_Coords[2] && pos.m_Coords[1] < CrotchExtrudePt.m_Coords[1]) {
			frontalSilhouette2.push_back(pos);
		}
	}

	EgLine l = EgLine(lowestCrotchPt, CrotchExtrudePt);
	EgPos tmpCrotch;
	maxDist = -INFINITY;
	for (EgPos pos : frontalSilhouette2) {
		double distance = dist(l, pos);
		if (distance > maxDist) {
			tmpCrotch = pos;
			maxDist = distance;
		}
	}
	CrotchPt = FindClosestVert(m_ScanMesh, tmpCrotch);

	return CrotchPt;
}


EgVertex* CSizeKorea2020Doc::FindNeckFrontPt(std::vector<EgVertex *> sagittal) {
	EgVertex *FneckPt = NULL;


	EgVertex upperPt, lowerPt;
	float upperBound = m_MaxY * 0.833f; // 목앞점탐색범위 통계
	float lowerBound = m_MaxY * 0.792f; // 목앞점탐색범위 통계
	float minDisY = 10000.0f;
	float minDisY1 = 10000.0f;

	for (EgVertex *pos : sagittal) {
		float disY = ABS(upperBound - pos->m_Pos[1]);
		if (disY < minDisY) {
			upperPt = *pos;
			minDisY = disY;
		}

		float disY1 = ABS(lowerBound - pos->m_Pos[1]);
		if (disY1 < minDisY1) {
			lowerPt = *pos;
			minDisY1 = disY1;
		}
	}

	EgVertex tmpPt = upperPt;
	tmpPt.m_Pos.m_Coords[0] += 20.0f;
	EgPlane planeF(upperPt.m_Pos, tmpPt.m_Pos, lowerPt.m_Pos);
	float maxDis = -INFINITY;

	for (EgVertex* pos : sagittal) {
		if (pos->m_Pos[1] > lowerPt.m_Pos[1] && pos->m_Pos[1] < upperPt.m_Pos[1]) {
			float dis = dist(planeF, pos->m_Pos);

			if (dis > maxDis) {
				FneckPt = pos;
				maxDis = dis;
			}
		}
	}


	if (FneckPt == NULL)
		FneckPt = new EgVertex();

	return FneckPt;
}

EgVertex* CSizeKorea2020Doc::FindNeckThyroidPt(std::vector<EgVertex *> sagittal) {
	EgVertex *ThyroidPt = NULL;

	// Find Jaw
	EgVertex *FneckPt = m_nLandmarkPose2[POSE2_LANDMARK_NECK_FRONT];
	/*
	EgVertex *JawPt = new EgVertex();
	// Fneck 위에서 목앞점~pos랑 y축이 이루는 각이 제일 큰 점
	float maxAngle = -INFINITY;
	for (EgVertex *pos : sagittal) {
		if (pos->m_Pos[1] > FneckPt->m_Pos[1]) {
			EgVec3 y_axis = EgVec3(0, 1, 0);
			EgVec3 currVec = EgVec3(pos->m_Pos - FneckPt->m_Pos);

			float currAngle = angle(y_axis, currVec);
			if (currAngle > maxAngle) {
				maxAngle = currAngle;
				JawPt = pos;
			}

		}
	}

	float thyroidLevel = m_nLandmarkPose2[POSE2_LANDMARK_NECK_FRONT]->m_Pos[1] * 0.4f + JawPt->m_Pos[1] * 0.6f;

	float minDist = INFINITY;
	for (EgVertex *pos : sagittal) {
		float distance = ABS(pos->m_Pos[1] - thyroidLevel);

		if (distance < minDist) {
			minDist = distance;
			ThyroidPt = pos;
		}
	}
	*/

	// 목앞점에서 3cm 올라간 점
	ThyroidPt = FindClosestVert(m_ScanMesh, EgPos(FneckPt->m_Pos[0], FneckPt->m_Pos[1] + 30.0f, FneckPt->m_Pos[2]));

	if (ThyroidPt == NULL) {
		ThyroidPt = new EgVertex();
		_cprintf("\nThyroid null!\n");
	}


	return ThyroidPt;
}

EgVertex* CSizeKorea2020Doc::FindMidriffPt() {
	EgVertex *MidriffPt = NULL;

	// 젖가슴아래와 허리둘레수준 중간
	EgVertex *UnderBustPt = m_nLandmarkPose2[POSE2_LANDMARK_UNDERBUST],
		*WaistPt = m_nLandmarkPose2[POSE2_LANDMARK_WAIST];

	float level = (UnderBustPt->m_Pos[1] + WaistPt->m_Pos[1]) / 2;

	MidriffPt = FindClosestVert(m_ScanMesh, EgPos((m_MinX + m_MaxX)/2, level, m_MinZ));

	return MidriffPt;
}

EgVertex* CSizeKorea2020Doc::FindNavelBackPt() {
	EgVertex *NavelPt_Back = NULL;

	EgVertex *NavelPt = m_nLandmarkPose2[POSE2_LANDMARK_NAVEL];

	// 배꼽점의 뒷면으로 이동한 점
	EgPos NavelSeed_Back;
	EgLine rayBack(NavelPt->m_Pos, EgVec3(0, 0, 1));
	for (EgFace *face : m_ScanMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		EgPos tmpIntersection;
		if (intersect_line_triangle(p0, p1, p2, rayBack, tmpIntersection, true)) {
			NavelSeed_Back = tmpIntersection;
		}
	}

	NavelPt_Back = FindClosestVert(m_ScanMesh, NavelSeed_Back);

	return NavelPt_Back;
}

EgVertex* CSizeKorea2020Doc::FindTopHipPt() {
	EgVertex *TopHipPt = NULL;

	// 허리둘레수준과 엉덩이돌출수준의 중간
	EgVertex *WaistPt = m_nLandmarkPose2[POSE2_LANDMARK_WAIST],
		*HipExtrudePt = m_nLandmarkPose2[POSE2_LANDMARK_HIP_EXTRUDE];

	float level = (WaistPt->m_Pos[1] + HipExtrudePt->m_Pos[1]) / 2;

	TopHipPt = FindClosestVert(m_ScanMesh, EgPos((m_MinX + m_MaxX) / 2, level, m_MinZ));

	return TopHipPt;
}

EgVertex* CSizeKorea2020Doc::FindUpperHipPt() {
	EgVertex *UpperHipPt = NULL;

	// Top-hip수준과 허리둘레수준의 중간
	EgVertex *TopHipPt = m_nLandmarkPose2[POSE2_LANDMARK_TOPHIP],
		*WaistPt = m_nLandmarkPose2[POSE2_LANDMARK_WAIST];

	float level = (TopHipPt->m_Pos[1] + WaistPt->m_Pos[1]) / 2;

	UpperHipPt = FindClosestVert(m_ScanMesh, EgPos((m_MinX + m_MaxX) / 2, level, m_MinZ));


	return UpperHipPt;
}

EgVertex* CSizeKorea2020Doc::FindThighMidPt() {
	EgVertex *ThighMidPt = NULL;

	// 엉덩이돌출점과 무릎뼈가운데점 수준 상의 중간 위치
	EgVertex *HipExtrudePt = m_nLandmarkPose2[POSE2_LANDMARK_HIP_EXTRUDE],
		*KneeMidPt = m_nLandmarkPose2[POSE2_LANDMARK_KNEE_MID];

	EgPos levelPt = EgPos(m_MinX, (HipExtrudePt->m_Pos[1] + KneeMidPt->m_Pos[1])/2, (HipExtrudePt->m_Pos[2] + KneeMidPt->m_Pos[2])/ 2);
	EgLine rayRight(levelPt, EgVec3(1, 0, 0));

	EgPos ThighMidSeed = EgPos(INFINITY, 0, 0);
	for (EgFace *face : m_ScanMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		EgPos tmpIntersection;
		if (intersect_line_triangle(p0, p1, p2, rayRight, tmpIntersection, true)) {
			// 가장 오른쪽 점을 찾아야하므로 교차점들 중 최소 x값을 갖는 점으로 정의
			if (ThighMidSeed.m_Coords[0] > tmpIntersection.m_Coords[0]) {
				ThighMidSeed = tmpIntersection;
			}
		}
	}

	ThighMidPt = FindClosestVert(m_ScanMesh, ThighMidSeed);

	return ThighMidPt;
}

EgVertex* CSizeKorea2020Doc::FindUnderkneePt() {
	EgVertex *UnderkneePt = NULL;

	// 무릎뼈가운데점에서 2cm 아래로 내려온 점
	EgVertex *KneeMidPt = m_nLandmarkPose2[POSE2_LANDMARK_KNEE_MID];

	UnderkneePt = new EgVertex(*KneeMidPt);
	UnderkneePt->m_Pos[1] -= 20.0;
	UnderkneePt = FindClosestVert(m_ScanMesh, UnderkneePt->m_Pos);

	return UnderkneePt;
}

EgVertex* CSizeKorea2020Doc::FindCalfExtrudeFrontPt() {
	EgVertex *CalfExtrudeFrontPt = NULL;

	// 장딴지돌출점을 정면으로 이동한 점 (최대 z값을 갖는 점)
	EgVertex *CalfExtrudePt = m_nLandmarkPose2[POSE2_LANDMARK_CALF_EXTRUDE];
	EgSlicer CalfSlicer;
	CalfSlicer.Slice(m_ScanMesh, 1, CalfExtrudePt->m_Pos[1] / m_MaxY, -1, 0, -1, 1, -1, 1, true);

	EgPos maxZ(0, 0, -INFINITY);
	for (EgPos pos : CalfSlicer.m_CutLines[0]) {
		if (pos.m_Coords[2] > maxZ.m_Coords[2]) {
			maxZ = pos;
		}
	}

	CalfExtrudeFrontPt = FindClosestVert(m_ScanMesh, maxZ);

	return CalfExtrudeFrontPt;
}

EgVertex* CSizeKorea2020Doc::FindAnkleInsidePt()
{
	EgVertex *AnkleInsidePt = NULL;

	// 가쪽복사점에서 오른쪽으로 ray 
	EgVertex *AnklePt = m_nLandmarkPose2[POSE2_LANDMARK_ANKLE];
	EgLine rayRight(AnklePt->m_Pos, EgVec3(1, 0, 0));

	EgPos AnkleInsideSeed = AnklePt->m_Pos;

	// mj::가쪽복사점에서 쏜 x축으로 쏜 ray가 왼쪽 다리 안쪽 복사점에서만 교차
	for (EgFace *face : m_ScanMesh->m_pFaces)
	{
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);
		EgPos tmpIntersection;
		if (intersect_line_triangle(p0, p1, p2, rayRight, tmpIntersection, false))
		{
			if (tmpIntersection.m_Coords[0] > AnklePt->m_Pos[0])
			{
				if (tmpIntersection.m_Coords[0] < (m_MinX + m_MaxX) / 2)
					AnkleInsideSeed = tmpIntersection;
			}
		}
	}
	AnkleInsidePt = SK_Elliptic_Vertex(FindClosestVert(m_ScanMesh, AnkleInsideSeed), 10);

	return AnkleInsidePt;
}


EgVertex* CSizeKorea2020Doc::FindShoulderRPt() {
	EgVertex *ShoulderPt_R = NULL;

	EgVertex *ShoulderSidePt_R = m_nLandmarkPose2[POSE2_LANDMARK_SHOULDERSIDE_R];

	ShoulderPt_R = SK_Elliptic_Vertex(ShoulderSidePt_R, 10.0f);

	return ShoulderPt_R;
}

EgVertex* CSizeKorea2020Doc::FindShoulderLPt() {
	EgVertex *ShoulderPt_L = NULL;

	EgVertex *ShoulderSidePt_L = m_nLandmarkPose2[POSE2_LANDMARK_SHOULDERSIDE_L];

	ShoulderPt_L = SK_Elliptic_Vertex(ShoulderSidePt_L, 10.0f);

	return ShoulderPt_L;
}

///////////////////////////////////////////////
//					  END					 //
///////////////////////////////////////////////


// find crotch point
// 참고점 -볼기고랑점
void CSizeKorea2020Doc::FindCrotchTempV() {
	//* 샅점을 찾을때.. m_pAlignedPointsV_X로 찾는 방법
	// * 높이범위는 statiscally,  좌우범위는 발바닥 좌우끝점

	float upLimit = m_Height * 0.49f;

	GObNode<GVector3f>* pNode = m_pAlignedPointsV_Y[40].m_pLeftEnd;
	if (pNode == NULL || m_pAlignedPointsV_Y[40].m_nCount < 50)
		pNode = m_pAlignedPointsV_Y[42].m_pLeftEnd;


	double minx = 10000.0; double maxx = -10000.0;
	while (pNode)
	{ //시작높이에서 탐색할 좌,우 범위 찾기
		GVector3f p = pNode->data;
		maxx = MAX(p.X, maxx);
		minx = MIN(p.X, minx);
		pNode = pNode->m_pRight;
	}
	// determine the range of search w.r.t x-axis. 
	int start = ROUND(minx) + 10;
	int end = ROUND(maxx) - 10; //ROUND((maxx-m_MinX))-10;

	if (start % 2) start++;

	double min_max_y = -100000.0;
	GVector3f minV, min_maxV;

	for (int i = start; i < end; i += 2)
	{

		GObNode<GVector3f> *pNode = m_pAlignedPointsV_X[i].m_pLeftEnd;
		if (pNode) {
			double miny = 100000.0;
			while (pNode)
			{
				GVector3f v = pNode->data;
				if (v.Y < miny)
				{
					miny = v.Y;
					minV = v;
				}
				pNode = pNode->m_pRight;
			}
			if (miny > min_max_y)
			{
				min_max_y = miny;
				if (minV.Y < upLimit)
					min_maxV = minV;
			}
		}

	}

	if ((min_maxV.Y / m_Height) < 0.4f)
		min_maxV.Y = m_Height * 0.4f;

	m_LandMarkHelp[0] = min_maxV; // TempCrotchPt;
}

// 샅앞점
// 참고점 - 엉덩이돌출점, 볼기고랑점
void CSizeKorea2020Doc::FindCrotchFV() {
	// 샅종단면 만들기
	float upLimit = m_LandMarkPose2[24].Y;       //엉덩이돌출
	float downLimit = m_LandMarkPose2[26].Y - 20;  //볼기고랑점높이
	float midY = upLimit * 0.6f + downLimit * 0.4f;


	//볼기고랑 높이 앞쪽점
	GVector3f glutealFP;
	int underH = ROUND(downLimit);
	if (underH % 2 != 0) underH--;

	float max_z = -1000.0f;

	int dropY;
	if (m_bWoman) dropY = 8;
	else dropY = 40;

	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[underH - dropY].m_pLeftEnd;
	while (pNode)
	{
		GVector3f v = pNode->data;
		if (v.Z >= max_z) {
			max_z = v.Z;
			glutealFP = v;
		}
		pNode = pNode->m_pRight;
	}

	glutealFP.Z -= 40.0f;
	m_LandMarkHelp[17] = glutealFP;

	//샅앞뒤 중심  -볼고랑 뒤점(m_LandMarkPose2[26])과 앞점의 중간점
	GVector3f centerPt = (m_LandMarkPose2[26] + glutealFP) / 2.0f;


	//샅종단면 앞쪽점들
	GObList<GVector3f> CrossPtListF, CrossPtListB;
	GPlane pln;
	GVector3f tmpCrotchPt = m_LandMarkHelp[0];
	tmpCrotchPt.X = tmpCrotchPt.X;
	pln.MakePlane(GVector3f(1.0, 0.0, 0.0), tmpCrotchPt);

	for (int i = 0; i < m_iNum; i += 3)
	{
		GVector3f b0, b1, b2;
		b0 = m_Vertex[m_FaceIdx[i]];
		b1 = m_Vertex[m_FaceIdx[i + 1]];
		b2 = m_Vertex[m_FaceIdx[i + 2]];
		if (pln.IsAbovePlane(b0) && pln.IsAbovePlane(b1) && pln.IsAbovePlane(b2))
			continue;
		if (pln.IsBelowPlane(b0) && pln.IsBelowPlane(b1) && pln.IsBelowPlane(b2))
			continue;
		if (b0.Y > midY || b0.Y < downLimit || b0.Z < centerPt.Z)
			continue;

		if (pln.GetSignedDistance(b0) * pln.GetSignedDistance(b1) <= 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b1 - b0);
			CrossPtListF.Insert(CrossPt);
		}
		if (pln.GetSignedDistance(b1) * pln.GetSignedDistance(b2) <= 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b1, b2 - b1);
			CrossPtListF.Insert(CrossPt);
		}
		if (pln.GetSignedDistance(b2) * pln.GetSignedDistance(b0) <= 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b2 - b0);
			CrossPtListF.Insert(CrossPt);
		}
	}
	::getSortedPoint(CrossPtListF, 1, 0);
	pNode = CrossPtListF.m_pLeftEnd;
	int num = CrossPtListF.m_nCount;

	GVector3f ray, tangentFPt;
	GVector3f Hray = GVector3f(0.0f, 1.0f, 0.0f);
	float maxAngle = 0.0f;

	for (int i = 0; i < (num - 3); i++)
	{
		GVector3f pt = pNode->data;

		if (pt.Y < upLimit && pt.Y > downLimit)
		{
			ray = pt - glutealFP;
			ray.X = 0.0f;
			float angle = Hray.getAngle(ray);
			if (angle > maxAngle) {
				tangentFPt = pt;
				maxAngle = angle;
			}
		}

		if (pNode)  pNode = pNode->m_pRight;
	}

	// 앞 평면 구하기
	GPlane Fpln;
	float Fslop;
	GVector3f FNorm;
	if (tangentFPt.Y == glutealFP.Y) FNorm = GVector3f(0.0f, 1.0f, 0.0f);    //????
	else {
		Fslop = (tangentFPt.Z - glutealFP.Z) / (tangentFPt.Y - glutealFP.Y);
		FNorm = GVector3f(0.0f, -Fslop, 1.0f);
	}
	Fpln.MakePlane(FNorm, tangentFPt);

	float cz = (glutealFP.Z*0.9f) + (m_LandMarkPose2[26].Z*0.1f);

	// 앞뒤평면에서 먼 점(앞) 구하기
	pNode = CrossPtListF.m_pLeftEnd;
	float disF;
	float maxDisF = -1000.0f;
	GVector3f crotchFP;

	while (pNode)
	{
		GVector3f v = pNode->data;

		disF = Fpln.GetSignedDistance(v);

		if (disF > maxDisF && v.Z > cz) {
			crotchFP = v;
			maxDisF = disF;
		}

		pNode = pNode->m_pRight;

	}

	m_LandMarkHelp[16] = crotchFP;
}

// 샅뒤점을 찾은뒤 샅앞점과 비교
// 참고점 - 임시샅점, 엉덩이돌출점[24], 볼기고랑점[26]
//          볼기고랑앞점Help[17],
void CSizeKorea2020Doc::FindCrotch() {
	// 샅종단면 만들기
	float upLimit = m_LandMarkPose2[24].Y;    //엉덩이돌출
	float downLimit = m_LandMarkPose2[26].Y;  //볼기고랑점높이
	float midY = upLimit * 0.8f + downLimit * 0.2f;

	GObList<GVector3f>  CrossPtListB;
	GPlane plnB;
	GVector3f tmpCrotchPtB = m_LandMarkHelp[0];
	tmpCrotchPtB.X = tmpCrotchPtB.X - 12.0f;
	plnB.MakePlane(GVector3f(1.0, 0.0, 0.0), tmpCrotchPtB);

	for (int i = 0; i < m_iNum; i += 3)
	{
		GVector3f b0, b1, b2;
		b0 = m_Vertex[m_FaceIdx[i]];
		b1 = m_Vertex[m_FaceIdx[i + 1]];
		b2 = m_Vertex[m_FaceIdx[i + 2]];
		if (plnB.IsAbovePlane(b0) && plnB.IsAbovePlane(b1) && plnB.IsAbovePlane(b2))
			continue;
		if (plnB.IsBelowPlane(b0) && plnB.IsBelowPlane(b1) && plnB.IsBelowPlane(b2))
			continue;
		if (b0.Y > midY) //upLimit
			continue;
		if (b0.Y < downLimit)
			continue;

		if (plnB.GetSignedDistance(b0) * plnB.GetSignedDistance(b1) < 0)
		{
			GVector3f CrossPt = plnB.GetProjectionPt(b0, b1 - b0);
			CrossPtListB.Insert(CrossPt);
		}
		if (plnB.GetSignedDistance(b1) * plnB.GetSignedDistance(b2) < 0)
		{
			GVector3f CrossPt = plnB.GetProjectionPt(b1, b2 - b1);
			CrossPtListB.Insert(CrossPt);
		}
		if (plnB.GetSignedDistance(b2) * plnB.GetSignedDistance(b0) < 0)
		{
			GVector3f CrossPt = plnB.GetProjectionPt(b0, b2 - b0);
			CrossPtListB.Insert(CrossPt);
		}
	}

	//앞뒤 중심 구함
	GVector3f centerPt = (m_LandMarkHelp[17] + m_LandMarkPose2[26]) / 2.0f; //m_LandMarkHelp[17]=볼기고랑앞점, 

	//sorting by y - from small to big
	::getSortedPoint(CrossPtListB, 1, 0);

	// 앞뒤직선 구할 위아래 점 구하기 -샅종단면점 상에서 탐색
	int underH = ROUND(downLimit);
	int rem = underH % 4;
	if (rem != 0) underH -= rem;
	float minDisBY = 10000.0f;

	GVector3f hipBP, hipFP, glutealFP;

	GObNode<GVector3f> *pNode1 = CrossPtListB.m_pLeftEnd;
	if (pNode1 == NULL) 	return;

	while (pNode1)
	{
		GVector3f v = pNode1->data;
		float disY = ABS(v.Y - midY); //midY
		if (v.Z < centerPt.Z)
		{
			if (disY < minDisBY) {
				minDisBY = disY;
				hipBP = v;       // 엉덩이높이 뒤점
			}
		}

		pNode1 = pNode1->m_pRight;
	}

	// 뒤 평면 구하기
	hipBP.Z -= 20.0f;
	GPlane Bpln;
	float Bslop;
	GVector3f BNorm;
	if ((hipBP.Z - m_LandMarkPose2[26].Z) == -20.0f)  BNorm = GVector3f(0.0f, 0.0f, 1.0f);
	else {
		Bslop = (hipBP.Y - m_LandMarkPose2[26].Y) / (hipBP.Z - m_LandMarkPose2[26].Z + 20.0f);
		BNorm = GVector3f(0.0f, 1.0f, -Bslop);
	}

	Bpln.MakePlane(BNorm, hipBP);


	// 뒤평면에서 먼 점(뒤) 구하기
	pNode1 = CrossPtListB.m_pLeftEnd;
	float disB;
	float maxDisB = -1000.0f;
	GVector3f  crotchBP;

	while (pNode1)
	{
		GVector3f v = pNode1->data;

		if (v.Z < centerPt.Z) {	//뒤쪽점들의 거리 구하기
			disB = Bpln.GetSignedDistance(v);
			if (disB > maxDisB) {
				crotchBP = v;
				maxDisB = disB;
			}
		}
		pNode1 = pNode1->m_pRight;
	}

	m_LandMarkHelp[41] = crotchBP;
	GVector3f crotchFP = m_LandMarkHelp[16];

	float crotchZ = crotchFP.Z*0.3f + crotchBP.Z*0.7f;
	float crotchY;
	if (m_bWoman) {
		crotchY = crotchFP.Y;
	}
	//여자면 샅점높이를  샅앞점, 샅뒤점의 중간으로 함,  남자면 샅뒤점높이로 함 
	if (m_bWoman)
		m_LandMarkPose2[25] = GVector3f(crotchFP.X, crotchY, crotchZ);
	else
		m_LandMarkPose2[25] = GVector3f(m_LandMarkHelp[0].X, crotchBP.Y, crotchZ);
}

// 샅뒤점을 찾은뒤 샅앞점과 비교
// 참고점 - 임시샅점, 엉덩이돌출점[24], 볼기고랑점[26]
//          볼기고랑앞점Help[17],
void CSizeKorea2020Doc::FindCrotchFB() {
	// 샅종단면 만들기
	GVector3f  glutealP = m_LandMarkPose2[26];
	GVector3f  hipP = m_LandMarkPose2[24];

	float upLimit, downLimitF, downLimitB;
	if (m_bWoman) {
		downLimitF = glutealP.Y - 4.0f;   //볼기고랑점높이
		upLimit = hipP.Y + 4.0f; //엉덩이돌출, 여자는 엉덩이 돌출이 낮은 경우가 많으므로 조금 높여준다.
	}
	else {
		downLimitF = glutealP.Y - 40.0f;   //볼기고랑점높이보다 많이 아래
		upLimit = hipP.Y;       //엉덩이돌출
	}

	downLimitB = glutealP.Y - 4.0f;

	float midYF = upLimit * 0.6f + downLimitF * 0.4f;


	//볼기고랑 높이 앞쪽점 - 실제로 볼기고랑높이보다 20mm 정도 내려진 허벅지 앞점을 구한다.
	GVector3f glutealFP;
	int glutealFPH = ROUND(downLimitF - 20.0f);
	if (glutealFPH % 2 != 0) glutealFPH--;

	float max_z = -1000.0f;


	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[glutealFPH].m_pLeftEnd;
	while (pNode)
	{
		GVector3f v = pNode->data;
		if (v.Z >= max_z) {
			max_z = v.Z;
			glutealFP = v;
		}
		pNode = pNode->m_pRight;
	}

	//샅앞뒤 중심  -볼고랑 뒤점(m_LandMarkPose2[26])과 앞점의 중간점
	GVector3f centerPt = (glutealP + glutealFP) / 2.0f;
	if (m_bWoman) centerPt.Z = -20.0f;

	glutealFP.Z = glutealP.Z*0.3f + glutealFP.Z*0.7f;  //볼기고랑앞점의 Z위치를 볼기고랑 뒤점사이 거리의 1/4안쪽으로 수정

	m_LandMarkHelp[17] = glutealFP;


	//샅종단면 앞쪽점들
	GObList<GVector3f> CrossPtListF, CrossPtListB;
	GPlane pln;
	GVector3f tmpCrotchPt = m_LandMarkHelp[0];
	tmpCrotchPt.X -= 1.0f;
	pln.MakePlane(GVector3f(1.0, 0.0, 0.0), tmpCrotchPt);

	for (int i = 0; i < m_iNum; i += 3)
	{
		GVector3f b0, b1, b2;
		b0 = m_Vertex[m_FaceIdx[i]];
		b1 = m_Vertex[m_FaceIdx[i + 1]];
		b2 = m_Vertex[m_FaceIdx[i + 2]];
		if (pln.IsAbovePlane(b0) && pln.IsAbovePlane(b1) && pln.IsAbovePlane(b2))
			continue;
		if (pln.IsBelowPlane(b0) && pln.IsBelowPlane(b1) && pln.IsBelowPlane(b2))
			continue;
		if (b0.Y > midYF || b0.Y < downLimitF)
			continue;

		if (pln.GetSignedDistance(b0) * pln.GetSignedDistance(b1) <= 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b1 - b0);
			if (b0.Z > centerPt.Z) 	CrossPtListF.Insert(CrossPt);
			else                    CrossPtListB.Insert(CrossPt);
		}
		if (pln.GetSignedDistance(b1) * pln.GetSignedDistance(b2) <= 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b1, b2 - b1);
			if (b0.Z > centerPt.Z) 	CrossPtListF.Insert(CrossPt);
			else                    CrossPtListB.Insert(CrossPt);

		}
		if (pln.GetSignedDistance(b2) * pln.GetSignedDistance(b0) <= 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b2 - b0);
			if (b0.Z > centerPt.Z) 	CrossPtListF.Insert(CrossPt);
			else                    CrossPtListB.Insert(CrossPt);

		}
	}
	::getSortedPoint(CrossPtListF, 1, 0);
	::getSortedPoint(CrossPtListB, 1, 0);

	//---------앞접점 구하기
	pNode = CrossPtListF.m_pLeftEnd;
	int num = CrossPtListF.m_nCount;

	GVector3f ray, tangentFPt;
	GVector3f Hray = GVector3f(0.0f, 1.0f, 0.0f);
	float maxAngle = 0.0f;
	float downLimitTangentF = glutealFP.Y*0.8f + hipP.Y*0.2f;

	for (int i = 0; i < (num - 3); i++)
	{
		GVector3f pt = pNode->data;

		if (pt.Y < upLimit && pt.Y > downLimitTangentF &&  pt.Z > (glutealFP.Z - 20.f)) // &&  pt.Z >centerPt.Z  앞접점 구하는 범위: 위=엉덩이돌출, 아래=볼기고랑아래
		{
			ray = pt - glutealFP;
			ray.X = 0.0f;
			float angle = Hray.getAngle(ray);
			if (angle > maxAngle) {
				tangentFPt = pt;
				maxAngle = angle;
			}
		}

		if (pNode)  pNode = pNode->m_pRight;
	}

	// 앞 접평면 구하기
	GPlane Fpln;
	float Fslop;
	GVector3f FNorm;
	if (tangentFPt.Y == glutealFP.Y) FNorm = GVector3f(0.0f, 1.0f, 0.0f);    //????
	else {
		Fslop = (tangentFPt.Z - glutealFP.Z) / (tangentFPt.Y - glutealFP.Y);
		FNorm = GVector3f(0.0f, Fslop, -1.0f);
	}
	Fpln.MakePlane(FNorm, tangentFPt);

	float cz = (glutealFP.Z*0.9f) + (m_LandMarkPose2[26].Z*0.1f);

	// 앞뒤평면에서 먼 점(앞) 구하기
	pNode = CrossPtListF.m_pLeftEnd;
	float disF;
	float maxDisF = -1000.0f;
	GVector3f crotchFP;

	while (pNode)
	{
		GVector3f v = pNode->data;

		disF = Fpln.GetSignedDistance(v);

		if (disF > maxDisF &&  v.Y < tangentFPt.Y) { //v.Z>cz &&
			crotchFP = v;
			maxDisF = disF;
		}

		pNode = pNode->m_pRight;

	}

	//if(crotchF.Y ==0.0f) crotchF =
	m_LandMarkHelp[16] = crotchFP;



	//---------뒤접점 구하기
	pNode = CrossPtListB.m_pLeftEnd;
	num = CrossPtListB.m_nCount;

	glutealP.Y -= 4.0f;
	//glutealP.Z =  glutealP.Z*0.9f + glutealFP.Z*0.1f;  //볼기고랑점의 Z위치를 볼기고랑 앞점과의 거리의 1/4안쪽으로 수정

	GVector3f  tangentBPt;
	Hray = GVector3f(0.0f, 0.0f, -1.0f);
	float minAngle = 3.2f;
	float downLimitTangentB = glutealP.Y*0.9f + hipP.Y*0.1f;

	for (int i = 0; i < (num - 3); i++)
	{
		GVector3f pt = pNode->data;

		if (pt.Y < upLimit && pt.Y > downLimitTangentB)
		{
			ray = pt - glutealP;
			ray.X = 0.0f;
			float angle = Hray.getAngle(ray);
			if (angle < minAngle) {
				tangentBPt = pt;
				minAngle = angle;
			}
		}

		if (pNode)  pNode = pNode->m_pRight;
	}

	//  뒤 접평면 구하기
	GPlane Bpln;
	float Bslop;
	GVector3f BNorm;
	if (tangentBPt.Y == glutealP.Y) BNorm = GVector3f(0.0f, 1.0f, 0.0f);
	else {
		Bslop = (tangentBPt.Z - glutealP.Z) / (tangentBPt.Y - glutealP.Y);
		BNorm = GVector3f(0.0f, -Bslop, 1.0f);
	}
	Bpln.MakePlane(BNorm, tangentBPt);

	//        float cz= (glutealP.Z*0.9f)+(m_LandMarkPose2[26].Z*0.1f);

			// 뒤접뒤평면에서 먼 점(뒤) 구하기
	pNode = CrossPtListB.m_pLeftEnd;
	float disB;
	float maxDisB = -1000.0f;
	GVector3f crotchBP;

	while (pNode)
	{
		GVector3f v = pNode->data;

		if (v.Y< tangentBPt.Y && v.Y> downLimitB) {

			disB = Bpln.GetSignedDistance(v);

			if (disB > maxDisB) { //&& v.Z>cz
				crotchBP = v;
				maxDisB = disB;
			}
		}

		pNode = pNode->m_pRight;

	}

	m_LandMarkHelp[41] = crotchBP;

	if (crotchFP.Y == 0.0f && crotchBP.Y != 0.0f)
		crotchFP = crotchBP;
	if (crotchBP.Y == 0.0f && crotchFP.Y != 0.0f)
		crotchBP = crotchFP;

	float crotchZ = crotchFP.Z*0.3f + crotchBP.Z*0.7f;

	//여자면 샅점높이를  샅앞점, 샅뒤점의 중간으로 함,  남자면 샅뒤점높이로 함 
	if (m_bWoman)
		m_LandMarkPose2[25] = GVector3f(crotchFP.X, crotchFP.Y, crotchZ);
	else
		m_LandMarkPose2[25] = GVector3f(m_LandMarkHelp[0].X, crotchBP.Y, crotchZ);;

	// 에러로 샅점이 높이 0이 되면 임시샅점을 샅점으로 해준다.
	if (crotchFP.Y == 0.0f)
		m_LandMarkPose2[25] = m_LandMarkHelp[0];
}

// find armpit point
// 이때 팔 몸통 분리평면 2개(위, 아래)을 만들거나   or 층별로 분리점들을 저장해둔다.
void CSizeKorea2020Doc::FindArmPitTempV() {
	if (m_LandMarkPose2[34].Y == 0.0f || m_LandMarkPose2[35].Y == 0.0f)
		return;

	//오른쪽 겨드랑
	float startHf;
	if (m_LandMarkPose2[34].Y > m_LandMarkPose2[35].Y)
		startHf = m_LandMarkPose2[34].Y;
	else
		startHf = m_LandMarkPose2[35].Y;

	int startH = ROUND(startHf) + 90.0f;
	if (startH % 2 != 0) startH++;
	int end = ROUND((m_MaxY - m_MinY)*0.8f);
	int lower_boundL, lower_boundR, lower_bound;
	double xL0, xL1, xR2, xR3;

	if (startH % 2) startH++;

	//높이별로 x값으로 정렬 -> x간격이 일정(50) 이상되는 곳을 탐색시작높이로 정함
	for (int i = startH; i < end; i += 2) //by height
	{
		GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[i].m_pLeftEnd;
		int size = m_pAlignedPointsV_Y[i].m_nCount;

		if (size < 100) continue;

		double *pSlice = new double[size];
		int slice_index = 0;
		while (pNode)
		{
			GVector3f p = pNode->data;
			pSlice[slice_index] = p.X;
			pNode = pNode->m_pRight;
			slice_index++;
		}
		// start sorting by x.... from small to big
		for (int j = 0; j < size - 1; j++)
		{
			for (int k = j + 1; k < size; k++)
			{
				if (pSlice[j] > pSlice[k])
				{
					double imsi = pSlice[j];
					pSlice[j] = pSlice[k];
					pSlice[k] = imsi;
				}
			}
		}
		// ended sorting....  
		// Right
		double left_dist = 0.0;           //오른쪽도 같이 할 수 있을텐데 이 while문에서....
		for (int j = 1; j < size / 2; j++) //간격이 가장 큰 곳(팔과 몸통사이 거리) 찾기   - 왼쪽
		{
			double dist = pSlice[j] - pSlice[j - 1];
			if (dist > left_dist)
			{
				xR2 = pSlice[j - 1];
				xR3 = pSlice[j];
				left_dist = dist;
			}
		}
		if (left_dist < 50)   // 팔과 몸통 사이가 50 보다 작아지기 시작하면 아래쪽 경계 높이로 설정
		{
			lower_boundR = i;
			delete[] pSlice;
			break;
		}

		delete[] pSlice;
	} // end for loop.

	for (int i = startH; i < end; i += 2)
	{
		GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[i].m_pLeftEnd;
		int size = m_pAlignedPointsV_Y[i].m_nCount;

		if (size < 100) continue;

		double* pSlice = new double[size];
		int slice_index = 0;
		while (pNode)
		{
			GVector3f p = pNode->data;
			pSlice[slice_index] = p.X;
			pNode = pNode->m_pRight;
			slice_index++;
		}
		// start sorting.... from big to small
		for (int j = 0; j < size - 1; j++)
		{
			for (int k = j + 1; k < size; k++)
			{
				if (pSlice[j] > pSlice[k])
				{
					double imsi = pSlice[j];
					pSlice[j] = pSlice[k];
					pSlice[k] = imsi;
				}
			}
		}
		// ended sorting....
		// Left
		double right_dist = 0.0f;
		for (int j = size / 2; j < size; j++) //간격이 가장 큰 곳(팔과 몸통사이 거리) 찾기 -오른쪽
		{
			double dist = pSlice[j] - pSlice[j - 1];
			if (dist > right_dist)
			{
				xL0 = pSlice[j - 1];
				xL1 = pSlice[j];
				right_dist = dist;
			}
		}
		if (right_dist < 50)
		{
			lower_boundL = i;
			delete[] pSlice;
			break;
		}
		delete[] pSlice;
	} // end for loop.

	lower_bound = MIN(lower_boundR, lower_boundL);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// The only informations that we know are lower_bound, x0, x1, x2 and x3.
	// 왼쪽 겨드랑이점을 발견하기위한 루틴...
	///////////////////////////////////////////////////////////////////////////////	

	int startXL = ROUND(xL0 - m_MinX) - 10; // determine the range of search w.r.t x-axis.
	if (startXL % 2 != 0) startXL++;
	int endXL = ROUND(xL1 - m_MinX) + 10;
	double min_max_y = -100000.0;
	//long min_index, min_max_index;
	GVector3f min_p, min_max_p;
	for (int i = startXL; i < endXL; i += 2)
	{
		GObNode<GVector3f>* pNode = m_pAlignedPointsV_X[i].m_pLeftEnd;
		double miny = 100000.0;
		while (pNode)
		{
			GVector3f p = pNode->data;
			if (p.Y < miny && p.Y > lower_bound)
			{
				miny = p.Y;
				min_p = p;
			}
			pNode = pNode->m_pRight;
		}
		if (miny > min_max_y)
		{
			min_max_y = miny;
			min_max_p = min_p;
		}
	}
	m_LandMarkPose2[10] = min_max_p;
	m_LandMarkHelp[2] = min_max_p;


	///////////////////////////////////////////////////////////////////////////////
	//오른쪽 겨드랑이 점을 발견하기 위한 루틴...
	///////////////////////////////////////////////////////////////////////////////
	int startXR = ROUND(xR2 - m_MinX) - 10; // determine the range of search w.r.t x-axis.
	if (startXR % 2 != 0) startXR++;
	int endXR = ROUND(xR3 - m_MinX) + 10;
	min_max_y = -100000.0;
	for (int i = startXR; i < endXR; i += 2)
	{
		GObNode<GVector3f>* pNode = m_pAlignedPointsV_X[i].m_pLeftEnd; //종단면 vertex의 index를 가진 list
		double miny = 100000.0;
		while (pNode)
		{
			GVector3f p = pNode->data;
			if (p.Y < miny && p.Y > lower_bound)
			{
				miny = p.Y;
				min_p = p; // 종단면에서 가장 아래쪽 점 찾기
			}
			pNode = pNode->m_pRight;
		}
		if (miny > min_max_y) // min_max_y 란? miny 중에 가장 큰 것
		{
			min_max_y = miny;
			min_max_p = min_p;
		}
	}
	m_LandMarkHelp[1] = min_max_p;
	m_LandMarkPose2[9] = min_max_p;
}

// Find Armpit Fold - reentrancy 방법
// 참고점: 임시겨드랑점(m_LandMarkHelp[1])
void CSizeKorea2020Doc::FindArmPitFoldV() {
	// 임시 armpit 위의 단면들에서 중심을 구하고, 
	// 중심에서 방사형으로 선분을 비추어서 교차점이 2개 이상 되는 곳이 마지막으로 나타나는 단면이 armpit 단면정
	// 이 단면이 구해지면 여기서 convexhull을 만들어서 hull line에서 거리가 최대인 오목한 점이 겨드랑점
	/* 알고리즘 수정 -->
	 1. 단면의 점들을 정렬한다: 네구역 rightF, rightB, leftF, leftB 으로 나누어서 각각의 구역별로 각으로 정렬한다.
		이때 각 구역의 중심의 x 좌표는 은 앞뒤두께가 가장 작은 곳으로 한다.
	 2. 중심점에서 거리가 가장 작은 점이 오목점이다.
		이 오목점에서 수평선을 그엇을때 팔쪽에서 교차하는 선분이 없을때가 팔과 몸통이 갈라지는 점이다.
	*/

	int startH = ROUND(m_LandMarkHelp[1].Y);
	startH = startH - (startH % 2);
	int endH = startH + 160;
	GVector3f armpitV;
	GVector3f ArmpitRF, ArmpitRB, ArmpitLF, ArmpitLB;
	GVector3f ArmpitRF_B, ArmpitRB_F, ArmpitLF_B, ArmpitLB_F;    //이름의 의미 ArmpitRF_B = 겨드랑뒤분기점 높이의 겨드랑앞점
	GVector3f tmpRF, tmpRB, tmpLF, tmpLB;
	int RFHnum, RBHnum, LFHnum, LBHnum;   //RFHnum-> 이 높이에 오기까지 팔과 몸통이 겹치는 점이 없는 높이의 개수
	RFHnum = RBHnum = LFHnum = LBHnum = 0;
	GVector3f RFpit, RBpit, LFpit, LBpit;

	//각 단면에서 pit point구하기
	for (int i = startH; i < endH; i += 2)
	{
		// 중심구하기 -  점을 다 더해서 점 갯수로 나눈다.
		GVector3f sum, maxxP, minxP;
		maxxP.X = -10000.0f; minxP.X = 10000.0f;

		GObNode<GVector3f>* pNode1 = m_pAlignedPointsV_Y[i].m_pLeftEnd;
		int num = m_pAlignedPointsV_Y[i].m_nCount;

		while (pNode1) {

			GVector3f v = pNode1->data;
			sum = sum + v;

			if (v.X > maxxP.X) maxxP = v;
			if (v.X < minxP.X) minxP = v;

			pNode1 = pNode1->m_pRight;
		}

		GVector3f cp = sum / num; //단면중심 cp

		//좌, 우의 중심 구하기 rcp, lcp
		float ratiox;
		if (m_bWoman) ratiox = 0.19f; //|| m_ArmObesity 
		else ratiox = 0.21f;

		float amount = (maxxP.X - minxP.X)*ratiox;
		float rcx = minxP.X + amount;
		float lcx = maxxP.X - amount;

		GVector3f rcp = GVector3f(rcx, (float)i, minxP.Z);
		GVector3f lcp = GVector3f(lcx, (float)i, maxxP.Z);

		// 몸통/팔 사이 오목점들 구하기- 좌우중심에서 가장 가까운점을 오목점으로 한다. 
		pNode1 = m_pAlignedPointsV_Y[i].m_pLeftEnd;
		float RFmin, RBmin, LFmin, LBmin, dis;
		RFmin = RBmin = LFmin = LBmin = 10000.0f;
		while (pNode1) {

			GVector3f v = pNode1->data;

			if (v.X < cp.X) {				        // Right
				if (v.Z > (minxP.Z - 5.0f)) {		// Front
					dis = PT3_DISTANCE(rcp, v);
					if (dis < RFmin) {
						RFmin = dis; RFpit = v;
					}
				}
				else {							// Back
					dis = PT3_DISTANCE(rcp, v);
					if (dis < RBmin) {
						RBmin = dis; RBpit = v;
					}
				}
			}
			else {						// Left
				if (v.Z > maxxP.Z) {			// Front
					dis = PT3_DISTANCE(lcp, v);
					if (dis < LFmin) {
						LFmin = dis; LFpit = v;
					}
				}
				else {					 // Back
					dis = PT3_DISTANCE(lcp, v);
					if (dis < LBmin) {
						LBmin = dis; LBpit = v;
					}
				}
			}

			pNode1 = pNode1->m_pRight;
		}

		//	int R=0, L=0;
			//   if( ABS(RFpit.X - RBpit.X) > 50.0)  //앞,뒤 오목점의 x좌표차이가 50보다 크면 오류이다.
		//		R=1;
		//	if( ABS(LFpit.X - LBpit.X) > 50.0)
		//		L=1;

	//팔쪽점들 중에서 오목점들보다 바깥으로 튀어나온 점들이 없으면 겨드랑점접힘으로 설정
	// 4개의 오목점들 모두가 바깥으로 튀어나온 점이 없을때 loop를 마침

		pNode1 = m_pAlignedPointsV_Y[i].m_pLeftEnd;
		int  RFZnum, RBZnum, LFZnum, LBZnum;  //팔쪽점들중에서 오목점보다 z축 방향으로 바깥으로 나온 점의 개수
		RFZnum = RBZnum = LFZnum = LBZnum = 0;
		float RFZ, RBZ, LFZ, LBZ;
		RFZ = RFpit.Z + 2.0f;  RBZ = RBpit.Z - 1.0f; LFZ = LFpit.Z + 2.0f; LBZ = LBpit.Z - 1.0f;

		while (pNode1) {
			GVector3f v = pNode1->data;

			//	if (R==0) { 
			if (v.Z > minxP.Z) { // F, R			        
				if (v.X < RFpit.X) {
					if (v.Z >= RFZ) { RFZnum++; }
				}
			}
			else {			  // B, R			
				if (v.X < RBpit.X) {
					if (v.Z <= RBZ) { RBZnum++; }
				}
			}
			//	}

				//	if (L==0) {
			if (v.Z > maxxP.Z) {  // F, L			
				if (v.X > LFpit.X) {
					if (v.Z >= LFZ) { LFZnum++; }
				}
			}
			else {		  // B, L								  			
				if (v.X > LBpit.X) {
					if (v.Z <= LBZ) { LBZnum++; }
				}
			}
			//		}

			pNode1 = pNode1->m_pRight;
		}

		// RFZnum >=0 && RFZnum<6 RFHnum가 0일때, 즉 처음 분리점일때의 점만 AmrpitRF점으로 설정한다.
				// ArmpitRB_F = 겨드랑앞분기점 높이의 겨드랑뒤오목점
		if (RFZnum == 0 && RFHnum == 0) {
			ArmpitRF = RFpit;  RFHnum++;
			ArmpitRB_F = RBpit;
		}
		if (RBZnum == 0 && RBHnum == 0) {
			ArmpitRB = RBpit;  RBHnum++;
			ArmpitRF_B = RFpit;
		}
		if (LFZnum == 0 && LFHnum == 0) {
			ArmpitLF = LFpit;  LFHnum++;
			ArmpitLB_F = LBpit;
		}
		if (LBZnum == 0 && LBHnum == 0) {
			ArmpitLB = LBpit;  LBHnum++;
			ArmpitLF_B = LFpit;
		}

		if (i == startH + 2) {
			tmpRF = RFpit; tmpRB = RBpit;
			tmpLF = LFpit; tmpLB = LBpit;
		}

		if (RFHnum > 0 && RBHnum > 0 && LFHnum > 0 && LBHnum > 0)
			break;   //4군데에서 모두 팔과 몸통의 분기점을 찾았을때 나간다.

	} //for

	m_LandMarkPose2[44] = ArmpitRF;
	m_LandMarkPose2[45] = ArmpitRB;
	m_LandMarkPose2[46] = ArmpitLF;
	m_LandMarkPose2[47] = ArmpitLB;

	if (RFHnum == 0 || ArmpitRF.Y > (ArmpitRB.Y + 50.0))  //겨드랑분기점을 못찾았을 경우 || 앞분기점이 뒤분기점+50보다 높을때
		m_LandMarkPose2[44] = ArmpitRF_B;           //겨드랑앞점을 겨드랑뒤점 높이 겨드랑앞오목점으로 한다.
	if (RBHnum == 0)
		m_LandMarkPose2[45] = RBpit;

	if (LFHnum == 0 || ArmpitLF.Y > (ArmpitLB.Y + 50.0))
		m_LandMarkPose2[46] = ArmpitLF_B;
	if (LBHnum == 0)
		m_LandMarkPose2[47] = LBpit;

	//접힘점이 안찾아졌을 경우에는 임시 겨드랑점을 접힘점으로 한다.
	   /*       if( m_LandMarkPose2[44].Y < m_LandMarkHelp[1].Y )
						m_LandMarkPose2[44] = m_LandMarkHelp[1];   //tmpRF;
				if( m_LandMarkPose2[44].Y < m_LandMarkHelp[1].Y )
						m_LandMarkPose2[44] = m_LandMarkHelp[1];   //tmpRB;
				if( m_LandMarkPose2[46].Y < m_LandMarkHelp[2].Y )
						m_LandMarkPose2[46] = m_LandMarkHelp[2];  //tmpLF;
				if( m_LandMarkPose2[47].Y < m_LandMarkHelp[2].Y )
						m_LandMarkPose2[47] = m_LandMarkHelp[2];  //tmpLB;*/
						//접힘점이 안찾아졌을 경우에는 임시 겨드랑점을 접힘점 위 2mm 오목점으로 한다.
	if (m_LandMarkPose2[44].Y < m_LandMarkHelp[1].Y - 1.0f)
		m_LandMarkPose2[44] = tmpRF;   //tmpRF;
	if (m_LandMarkPose2[44].Y < m_LandMarkHelp[1].Y - 1.0f)
		m_LandMarkPose2[45] = tmpRB;   //tmpRB;
	if (m_LandMarkPose2[46].Y < m_LandMarkHelp[2].Y - 1.0f)
		m_LandMarkPose2[46] = tmpLF;  //tmpLF;
	if (m_LandMarkPose2[47].Y < m_LandMarkHelp[2].Y - 1.0f)
		m_LandMarkPose2[47] = tmpLB;  //tmpLB;
}

// Find Armpit Fold 를 이용해서 겨드랑점 찾기
// 참고점: 임시겨드랑점, 팔둘레
//       : 앞접힘점(42-R, 44-L), 뒤접힘점(43-R, 45-L)
void CSizeKorea2020Doc::FindArmPit_ArmpitFold() {
	GVector3f  ArmpitRF = m_LandMarkPose2[44];
	GVector3f  ArmpitRB = m_LandMarkPose2[44];
	GVector3f  ArmpitLF = m_LandMarkPose2[46];
	GVector3f  ArmpitLB = m_LandMarkPose2[47];


	if (m_ArmObesity < 215.0f) {  //215.0f  262
		m_LandMarkPose2[9] = m_LandMarkHelp[1];
		m_LandMarkPose2[10] = m_LandMarkHelp[2];
		m_LandMarkPose2[9].Z = (ArmpitRF.Z + ArmpitRB.Z)*0.5f;
		m_LandMarkPose2[10].Z = (ArmpitLF.Z + ArmpitLB.Z)*0.5f;
		return;
	}


	// Right Armpit
	float newZ = (ArmpitRF.Z + ArmpitRB.Z)*0.5f;
	float newX = (ArmpitRF.X*0.6f) + (ArmpitRB.X*0.4f);
	float newY;

	if (ArmpitRB.Y < ArmpitRF.Y) newY = ArmpitRB.Y*0.7f + ArmpitRF.Y*0.3f;
	else			    newY = ArmpitRB.Y*0.3f + ArmpitRF.Y*0.7f;

	m_LandMarkPose2[9] = GVector3f(newX, newY - 16.0f, newZ);

	// Left Armpit
	newZ = ArmpitLF.Z*0.5f + ArmpitLB.Z*0.5f;
	newX = ArmpitLF.X*0.6f + ArmpitLB.X*0.4f;

	if (ArmpitLB.Y < ArmpitLF.Y) newY = ArmpitLB.Y*0.7f + ArmpitLF.Y*0.3f;
	else		            newY = ArmpitLB.Y*0.3f + ArmpitLF.Y*0.7f;

	m_LandMarkPose2[10] = GVector3f(newX, newY - 16.0f, newZ);
}

void CSizeKorea2020Doc::FindArmpitConcavePtsV() {
	// 겨드랑높이에서의 좌우, 앞뒤 오목점 구하기
	int height = ROUND(m_LandMarkPose2[9].Y) + 8;
	if (height % 2) height--;
	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[height].m_pLeftEnd;

	GVector3f  rp = m_FRSilhouetteV[height / 2];
	GVector3f  lp = m_FLSilhouetteV[height / 2];

	float RX = rp.X + 50.0f;
	float RCZ = rp.Z;
	float LX = lp.X - 50.0f;
	float LCZ = lp.Z;
	float CX = (RX + LX) / 2.0f;
	float CRX = CX - 110.0f;
	float CLX = CX + 110.0f;
	//float minz = 1000.0f, maxz = -1000.0f;
	GVector3f maxzRFp = GVector3f(0.0f, 0.0f, -10000.0f);
	GVector3f minzLBp = GVector3f(0.0f, 0.0f, 10000.0f);
	GVector3f maxzLFp = GVector3f(0.0f, 0.0f, -10000.0f);
	GVector3f minzRBp = GVector3f(0.0f, 0.0f, 10000.0f);
	GVector3f armRFp, armRBp, armLFp, armLBp;
	float minDisRF, minDisRB, minDisLF, minDisLB;
	minDisRF = minDisRB = minDisLF = minDisLB = 10000.0f;

	//좌우 몸통의 최앞뒤점. 팔 앞뒤점 구하기
	while (pNode)
	{
		GVector3f pt = pNode->data;
		float disR = fabs(pt.X - RX);
		float disL = fabs(pt.X - LX);

		// right 
		if (pt.X < CRX) {
			// 몸통의 최 앞 뒤점 구하기
			if (pt.Z > maxzRFp.Z)  maxzRFp = pt;
			if (pt.Z < minzRBp.Z)  minzRBp = pt;

			// 팔 앞 뒤점 구하기
			if (pt.Z > RCZ) {
				if (disR < minDisRF) {
					armRFp = pt;
					minDisRF = disR;
				}
			}
			else {
				if (disR < minDisRB) {
					armRBp = pt;
					minDisRB = disR;
				}
			}
		}
		// left
		else if (pt.X > CLX) {
			if (pt.Z > maxzLFp.Z)  maxzLFp = pt;
			if (pt.Z < minzLBp.Z)  minzLBp = pt;

			// 팔 앞 뒤점 구하기
			if (pt.Z > LCZ) {
				if (disL < minDisLF) {
					armLFp = pt;
					minDisLF = disL;
				}
			}
			else {
				if (disL < minDisLB) {
					armLBp = pt;
					minDisLB = disL;
				}
			}
		}

		pNode = pNode->m_pRight;

	}

	float RFslop, RBslop, LFslop, LBslop;
	GVector3f normRF, normRB, normLF, normLB;

	if (armRFp.Z == maxzRFp.Z)
		normRF = GVector3f(0.0f, 0.0f, 1.0f);
	else {
		RFslop = (armRFp.X - maxzRFp.X) / (armRFp.Z - maxzRFp.Z);
		normRF = GVector3f(1.0f, 0.0f, -RFslop);
	}

	if (armRBp.Z == minzRBp.Z)
		normRB = GVector3f(0.0f, 0.0f, 1.0f);
	else {
		RBslop = (armRBp.X - minzRBp.X) / (armRBp.Z - minzRBp.Z);
		normRB = GVector3f(1.0f, 0.0f, -RBslop);
	}

	if (armLFp.Z == maxzLFp.Z)
		normLF = GVector3f(0.0f, 0.0f, 1.0f);
	else {
		LFslop = (armLFp.X - maxzLFp.X) / (armLFp.Z - maxzLFp.Z);
		normLF = GVector3f(-1.0f, 0.0f, LFslop);
	}

	if (armLBp.Z == minzLBp.Z)
		normLB = GVector3f(0.0f, 0.0f, 1.0f);
	else {
		LBslop = (armLBp.X - minzLBp.X) / (armLBp.Z - minzLBp.Z);
		normLB = GVector3f(-1.0f, 0.0f, LBslop);
	}

	GPlane plnRF, plnRB, plnLF, plnLB;
	plnRF.MakePlane(normRF, maxzRFp);
	plnRB.MakePlane(normRB, minzRBp);
	plnLF.MakePlane(normLF, maxzLFp);
	plnLB.MakePlane(normLB, minzLBp);

	float maxDisRF, maxDisRB, maxDisLF, maxDisLB;
	maxDisRF = maxDisRB = maxDisLF = maxDisLB = -1000.0f;

	pNode = m_pAlignedPointsV_Y[height].m_pLeftEnd;
	while (pNode)
	{
		GVector3f pt = pNode->data;

		//RF
		if (pt.X<maxzRFp.X && pt.X>armRFp.X && pt.Z > RCZ)
		{
			float dis = plnRF.GetSignedDistance(pt);
			if (dis > maxDisRF) {
				m_LandMarkHelp[62] = pt;
				maxDisRF = dis;
			}
		}


		//RB
		if (pt.X<maxzRFp.X && pt.X>armRFp.X && pt.Z < RCZ)
		{
			float dis = plnRB.GetSignedDistance(pt);
			if (dis > maxDisRB) {
				m_LandMarkHelp[63] = pt;
				maxDisRB = dis;
			}
		}
		//LF
		if (pt.X > maxzLFp.X && pt.X<armLFp.X && pt.Z>LCZ)
		{
			float dis = plnLF.GetSignedDistance(pt);
			if (dis > maxDisLF) {
				m_LandMarkHelp[64] = pt;
				maxDisLF = dis;
			}
		}
		//LB
		if (pt.X > maxzLFp.X && pt.X < armLFp.X && pt.Z < LCZ)
		{
			float dis = plnLB.GetSignedDistance(pt);
			if (dis > maxDisLB) {
				m_LandMarkHelp[65] = pt;
				maxDisLB = dis;
			}
		}
		pNode = pNode->m_pRight;
	}
}

// find arm end point
// 참고점:  샅점
void CSizeKorea2020Doc::FindArmEndV_Temp() {
	int underLimit = ROUND(m_LandMarkHelp[0].Y - 100.0f);
	int upLimit = ROUND(m_Height *  0.7f);  //0.424

	if (underLimit % 2 != 0) underLimit++;

	GVector3f tmp0(10000.0f, 0.0f, 0.0f);
	GVector3f tmp1(-10000.0f, 0.0f, 0.0f);

	for (int i = underLimit; i < upLimit; i += 4)
	{
		GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[i].m_pLeftEnd;

		while (pNode)
		{
			GVector3f pt = pNode->data;

			if (pt.X < tmp0.X)
				tmp0 = pt;
			if (pt.X > tmp1.X)
				tmp1 = pt;

			pNode = pNode->m_pRight;
		}
	}

	m_LandMarkPose2[34] = tmp0;  //right
	m_LandMarkPose2[35] = tmp1;  //left
}

// find arm end point
// 참고점:  임시 겨드랑점(m_LandMarkHelp[1], [2]), 샅점, 팔나눔각도 
void CSizeKorea2020Doc::FindArmEndV() {
	int underLimit = ROUND(m_Height * 0.330);
	int upLimit = ROUND(m_Height *  0.6);  //0.424

	if (underLimit % 2 != 0) underLimit++;

	double tan1 = tan(M_PI / 2.0 - RefAngle[3]);
	double tan2 = tan(M_PI / 2.0 + RefAngle[4]);

	GPlane plane0, plane1;
	plane0.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkHelp[42]);  //right
	plane1.MakePlane(GVector3f(1.0f, tan2, 0.0f), m_LandMarkHelp[43]);  //left

	GVector3f tmp0(0.0f, 10000.0f, 0.0f);
	GVector3f tmp1(0.0f, 10000.0f, 0.0f);

	for (int i = underLimit; i < upLimit; i += 2)
	{
		GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[i].m_pLeftEnd;

		while (pNode)
		{
			GVector3f pt = pNode->data;

			if (plane0.IsAbovePlane(pt)) //right
			{
				if (pt.Y < tmp0.Y)
					tmp0 = pt;
			}
			if (plane1.IsAbovePlane(pt)) //left
			{
				if (pt.Y < tmp1.Y)
					tmp1 = pt;
			}
			pNode = pNode->m_pRight;
		}
	}

	m_LandMarkPose2[34] = tmp0;  //right
	m_LandMarkPose2[35] = tmp1;  //left
}

// sp2구하기  
// 탐색범위를 목옆점에서 바깥쪽으로 80mm 이상에 있는 점들로 함
// 참고점: 목옆점[4][5], 겨드랑점[9][10], 정면실루엣
void CSizeKorea2020Doc::FindSP2_1V() {
	// sp2는 목옆점에서부터의 각을 구해서 가장 작은 각이 나오는 점으로 찾는다.
	GVector3f SNeckR = m_LandMarkPose2[4]; //오른 목옆점

	GObNode<GVector3f> *pNode1 = m_FRSilhouetteV.m_pLeftEnd;
	int num = m_FRSilhouetteV.m_nCount;

	float xlimit = m_LandMarkPose2[4].X*0.2 + m_LandMarkPose2[9].X*0.8; //m_LandMarkPose2[4].X-70.0f ; //r-sNeck  80
	float ylimitUp = m_LandMarkPose2[4].Y + 10.0f;
	float ylimitDown = m_LandMarkHelp[1].Y;   //r-armpit
	//float minDisX=1000.0f;
	GVector3f ray, SP2, armpitXpt;
	GVector3f Hray = GVector3f(-1.0f, 0.0f, 0.0f);
	float minAngle = 3.0f;

	for (int i = 0; i < (num - 3); i++)
	{
		GVector3f pt = pNode1->data;

		if (pt.Y < ylimitUp && pt.Y > ylimitDown && pt.X < xlimit)
		{
			ray = pt - SNeckR;
			ray.Z = 0.0f;
			float angle = Hray.getAngle(ray);
			if (angle < minAngle) {
				SP2 = pt;
				minAngle = angle;
			}
		}

		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	m_LandMarkHelp[3] = SP2;
	m_ShoulderAngleR = minAngle;
	m_ResultPose2[86] = (minAngle*180.0) / M_PI;

	//Left
	// sp2는 목옆점에서부터의 각을 구해서 가장 작은 각이 나오는 점으로 찾는다.
	GVector3f SNeckL = m_LandMarkPose2[5]; //왼 목옆점

	pNode1 = m_FLSilhouetteV.m_pLeftEnd;
	num = m_FLSilhouetteV.m_nCount;

	xlimit = m_LandMarkPose2[5].X*0.2 + m_LandMarkPose2[10].X*0.8;
	ylimitUp = m_LandMarkPose2[5].Y + 10.0f;
	ylimitDown = m_LandMarkHelp[2].Y;   //l-armpit
	//minDisX=1000.0f;
	Hray = GVector3f(1.0f, 0.0f, 0.0f);
	minAngle = 3.0f;

	for (int i = 0; i < (num - 3); i++)
	{
		GVector3f pt = pNode1->data;

		if (pt.Y < ylimitUp && pt.Y > ylimitDown && pt.X > xlimit)
		{
			ray = pt - SNeckL;
			ray.Z = 0.0f;
			float angle = Hray.getAngle(ray);
			if (angle < minAngle) {
				SP2 = pt;
				minAngle = angle;
			}

		}

		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	m_LandMarkHelp[4] = SP2;
	m_ShoulderAngleL = minAngle;
}

// 목옆 팔실루엣점(목옆점(4)과 겨드랑점(9)의 중간 위치) 직선에서 가장 먼점으로 찾는 방법
// 필요한 점 - 겨드랑점, 목옆점, 정면실루엣
void CSizeKorea2020Doc::FindShoulderPtConvexV() {
	// 겨드랑점과 목옆점을 이은 직선에서 가정 먼점, right
	if (m_LandMarkPose2[9] == GVector3f(0.0f, 0.0f, 0.0f))	return;
	if (m_LandMarkPose2[4] == GVector3f(0.0f, 0.0f, 0.0f))	return;
	GVector3f Armpit = m_LandMarkPose2[9];
	GVector3f SNeckR = m_LandMarkPose2[4];

	int startH = ROUND(Armpit.Y) + ((m_BustCObesity - 0.5)*200.0);
	int startHindex = startH / m_HInterval;
	GVector3f downPt = m_FRSilhouetteV[startHindex];

	GVector3f normal = GVector3f(-(downPt.Y - SNeckR.Y), (downPt.X - SNeckR.X), 0.0f);

	GPlane pln;
	pln.MakePlane(normal, downPt);

	GObNode<GVector3f> *pNode1 = m_FRSilhouetteV.m_pLeftEnd;
	int num = m_FRSilhouetteV.m_nCount;
	GVector3f tempShoulderPt;
	float maxDis = 0.0f;
	float limitRX = Armpit.X - 4.0f; //;

	for (int i = 0; i < (num - 3); i++)
	{
		GVector3f pt = pNode1->data;

		if (pt.Y > downPt.Y  && pt.X < limitRX)
		{
			float dis = ABS(pln.GetSignedDistance(pt));

			if (dis > maxDis) {
				maxDis = dis;
				m_LandMarkHelp[8] = pt;
			}
		}

		if (pNode1)  pNode1 = pNode1->m_pRight;
	}


	// left
	Armpit = m_LandMarkPose2[10];
	GVector3f SNeckL = m_LandMarkPose2[5];
	SNeckL.X += 20.0f;

	//비만일수록 어깨쪽으로 가도록 사선 아래 기준점을 올려준다.
	startH = ROUND(Armpit.Y) + 20;
	startHindex = startH / m_HInterval;
	downPt = m_FLSilhouetteV[startHindex];

	normal = GVector3f(-(downPt.Y - SNeckL.Y), (downPt.X - SNeckL.X), 0.0f);

	pln.MakePlane(normal, downPt);

	pNode1 = m_FLSilhouetteV.m_pLeftEnd;
	num = m_FLSilhouetteV.m_nCount;
	maxDis = 0.0f;
	limitRX = Armpit.X + 4.0f; //Armpit.X+4.0f;

	for (int i = 0; i < (num - 3); i++)
	{
		GVector3f pt = pNode1->data;

		if (pt.Y > downPt.Y && pt.X > limitRX)
		{
			float dis = ABS(pln.GetSignedDistance(pt));

			if (dis > maxDis) {
				maxDis = dis;
				m_LandMarkHelp[9] = pt;
			}
		}

		if (pNode1)  pNode1 = pNode1->m_pRight;
	}
}

// sp2 기울기를 이용하여 이보다 약간 큰 기울기에서 멈추는 방법
// 문제점- 어깨끈때문에 sp2가 제대로 안구해진다.
void CSizeKorea2020Doc::FindShoulderPoint1() {
	// 기울기 조사
	GObNode<GVector3f> *pNode1 = m_FRSilhouetteV.m_pLeftEnd;
	int num = m_FRSilhouetteV.m_nCount;
	GVector3f prePt, pt, nextPt, imsiPt;
	float dX1, dX2, maxAngleDiff = -1000.0f; // maxBv = -1000.0f,
	float slop1 = 0.0f, slop2 = 0.0f, preslop = 0.0f;

	float ylimit = m_LandMarkPose2[9].Y + 40.0f; // m_Height*0.76f; 
	int StartH = ROUND(m_LandMarkPose2[9].Y + 40.0f);
	if (StartH % 2) StartH--;
	int StartHindex = StartH / 2;

	prePt = m_FRSilhouetteV[StartHindex - 6];

	for (int i = StartHindex; i < (num - 4); i += 7)
	{
		pt = m_FRSilhouetteV[i];

		if (pt.X > (m_LandMarkPose2[4].X - 60.0f))
			break;

		dX1 = pt.X - prePt.X;
		slop1 = (pt.Y - prePt.Y) / dX1;
		float angle1 = atan(slop1);

		nextPt = m_FRSilhouetteV[i + 6];
		dX2 = nextPt.X - pt.X;
		slop2 = (nextPt.Y - pt.Y) / dX2;
		float angle2 = atan(slop2);

		float ds = sqrt((prePt.X - nextPt.X)*(prePt.X - nextPt.X) + (prePt.Y - nextPt.Y)*(prePt.Y - nextPt.Y));

		float diffAngle = angle1 - angle2;
		// float diffAngle= diffAngle1/ds;

		if (diffAngle > maxAngleDiff) { //다음기울기가 더 작아야한다. 점점 편평해지므로...  데이타가 안좋은경우를 대비해 slop2가 (-)가 되는따는 건너뛴다
			m_LandMarkHelp[63] = pt;
			maxAngleDiff = diffAngle;
		}

		prePt = pt;
	}
}

// 목옆점 겨드랑점 직선에서 가장 먼점과, SP2직선의 교점에서, 가장 가까운 점으로 찾는 방법
// 사용하는 점 - 목옆점, 임시 겨드랑점, sp2(Help[3]), spConvex, 겨드랑앞접힘점[42]
// m_LandMarkHelp[26] 팔축 위점.   m_LandMarkHelp[27] 팔축 아래점
void CSizeKorea2020Doc::FindShoulderPoint_SP2_ConvexV() {
	//----------------------------------------------------
	// Right
	//----------------------------------------------------
	// 1. sp2 기울기
	GVector3f SP2 = m_LandMarkHelp[3];
	float sp2Slop = tan(m_ShoulderAngleR);// -0.1f;
	float sp2C = SP2.Y - (SP2.X* sp2Slop);  // + (m_Obesity*2.0f);

	// 2. 겨드랑점과 목옆점을 이은 직선에서 가정 먼점에서 팔 아래 점과 직선
	GVector3f  tempShoulderPt = m_LandMarkHelp[8];
	float startH = tempShoulderPt.Y;
	int startHindex = ROUND(startH / 2);
	int downGap = 40 / 2;  // 탐색범위 아래쪽을 convex pt보다 20mm 아래쪽으로 한다.
	GVector3f armUpPt = m_FRSilhouetteV[startHindex];
	GVector3f armDownPt = m_FRSilhouetteV[startHindex - downGap];

	float armslop1;
	if (m_LandMarkHelp[26].X == m_LandMarkHelp[27].X)  armslop1 = 10000.0f;
	else {
		armslop1 = (m_LandMarkHelp[26].Y - m_LandMarkHelp[27].Y) / (m_LandMarkHelp[26].X - m_LandMarkHelp[27].X);
	}

	float armslop = armslop1 * 0.5f + sp2Slop * 0.5f;

	float armC = tempShoulderPt.Y - (tempShoulderPt.X* armslop);

	/*armslop = (m_LandMarkHelp[26].Y-m_LandMarkHelp[27].Y)/(m_LandMarkHelp[26].X-m_LandMarkHelp[27].X);
	float angle = atan(armslop);
	float dx= 27.0f/sin(angle);
	GVector3f bonePt = m_LandMarkHelp[26];
	bonePt.X -= dx;
	float armC = bonePt.Y - (bonePt.X* armslop) ;*/

	// 3.교차점 구하기
	float crossX = (sp2C - armC) / (armslop - sp2Slop);           //??? 
	float crossY = ((sp2C*armslop) - (sp2Slop*armC)) / (armslop - sp2Slop);
	GVector3f crossPt = GVector3f(crossX, crossY, SP2.Z);

	m_LandMarkHelp[10] = crossPt;

	if (SP2 == tempShoulderPt) m_LandMarkHelp[20] = SP2;

	//4. 'silhouette선'과 'cross pt와 겨드랑앞접힘을 이은 직선'과 고 교차하는 점 구하기
	GObNode<GVector3f> *pNode1 = m_FRSilhouetteV.m_pLeftEnd;
	int num = m_FRSilhouetteV.m_nCount;
	float limitXIn = SP2.X + 5.0f;
	float minDis = 1000.0f;
	GVector3f armfoldPt = m_LandMarkPose2[44];
	armfoldPt.X = m_LandMarkPose2[9].X;  //x좌표는 겨드랑점 것으로 한다

	// 선분중 교차하는 선분의 교차점을 구하는 방법
	float criticObesity;
	if (m_bWoman)  criticObesity = 0.5f;
	else criticObesity = 0.4f;

	armfoldPt.X += (m_BustCObesity - criticObesity)*200.0f; // 비만일수록 armfold의 x좌표를 안쪽으로 해준다

	if (armfoldPt.X < (crossPt.X - 10))
		armfoldPt.X = crossPt.X + 50.f;



	float BObesityLimit;
	if (m_bWoman) BObesityLimit = 0.602;
	else	BObesityLimit = 0.568;

	if (m_BustCObesity < BObesityLimit) {   //BustC 0.527 = BMI 20/ BustC 0.602 = BMI 25
		//마른체형들은 crossptHelp[10]와 겨드랑앞점힘선과의 선분과의 교차점으로 한다

		for (int i = 0; i < (num - 3); i++)
		{
			GVector3f pt, pt1;
			pt = pNode1->data;

			if (pNode1) {
				pNode1 = pNode1->m_pRight;
				pt1 = pNode1->data;

				if (pt.X < limitXIn && pt.Y > armDownPt.Y)
				{
					GVector3f intersectPt;
					if (::GetIntersectPointXY(m_LandMarkHelp[10], armfoldPt, pt1, pt, &intersectPt)) {
						intersectPt.Z = (pt.Z*0.5f) + (pt1.Z*0.5f);
						m_LandMarkHelp[20] = intersectPt;
					}
				}
			}

		}//for
	}
	else {  //비만체형들은 겨드랑앞점힘점을 좀 안쪽으로 옮겨준다		 

		armfoldPt.X += 10.0f;
		for (int i = 0; i < (num - 3); i++)
		{
			GVector3f pt, pt1;
			pt = pNode1->data;

			if (pNode1) {
				pNode1 = pNode1->m_pRight;
				pt1 = pNode1->data;

				if (pt.X < limitXIn && pt.Y > armDownPt.Y)
				{
					GVector3f intersectPt;
					if (::GetIntersectPointXY(m_LandMarkHelp[10], armfoldPt, pt1, pt, &intersectPt)) {
						intersectPt.Z = (pt.Z*0.5f) + (pt1.Z*0.5f);
						m_LandMarkHelp[20] = intersectPt;
					}
				}
			}

		}

	}


	//3-1 cross pt와 겨드랑점을 잇는 선과 front silhouette과의 교차점 구하기

	// Adjust -  60=r, 61 =l
	// error 인 경우 판단
	//  앞점힘보다 안쪽인 경우 - 앞점힘과 뒤접힘의 중간 위치ㅇ정도로 한다.
	//  뒤접힘보다 바깥이면서 한단계 위점과의 기울기가 1  이상인 경우
	float newx = 10000.0f;

	if (m_LandMarkHelp[20].X > m_LandMarkPose2[9].X)
		newx = (m_LandMarkPose2[44].X*0.4f) + (m_LandMarkPose2[44].X*0.6f);

	if (newx != 10000.0f)
	{
		pNode1 = m_FRSilhouetteV.m_pLeftEnd;
		float minDis = 10000.0f;
		for (int i = 0; i < (num - 3); i++)
		{
			GVector3f pt = pNode1->data;

			if (pt.Y < SP2.Y && pt.Y > armDownPt.Y)
			{
				float dis = fabs(pt.X - newx);

				if (dis < minDis) {
					minDis = dis;
					m_LandMarkHelp[20] = pt;
				}
			}

			if (pNode1)  pNode1 = pNode1->m_pRight;
		}
	}


	if (m_LandMarkHelp[20].Y == 0.0f)
		m_LandMarkPose2[7] = tempShoulderPt;
	else
		m_LandMarkPose2[7] = m_LandMarkHelp[20];  //z좌표 이동해주기 전 점을 임시로 어깨점으로 한다.


	 //----------------------------------------------------
	 // Left
	 //----------------------------------------------------
	 // 1. sp2 기울기
	SP2 = m_LandMarkHelp[4];
	sp2Slop = -tan(m_ShoulderAngleL);
	sp2C = SP2.Y - (SP2.X* sp2Slop);

	// 2. 겨드랑점과 목옆점을 이은 직선에서 가정 먼점에서 팔 아래 점과 직선
	tempShoulderPt = m_LandMarkHelp[9];
	startH = tempShoulderPt.Y;
	startHindex = ROUND(startH / 2);
	armUpPt = m_FLSilhouetteV[startHindex];
	armDownPt = m_FLSilhouetteV[startHindex - downGap];

	if ((armDownPt.X + 2.0f - armUpPt.X) == 0.0f) armslop = 10000.0f;
	else  armslop = (armDownPt.Y - armUpPt.Y) / (armDownPt.X + 2.0f - armUpPt.X);
	armC = armDownPt.Y - (armDownPt.X* armslop);

	//3. 교차점 구하기
	crossX = (sp2C - armC) / (armslop - sp2Slop);
	crossY = ((sp2C*armslop) - (sp2Slop*armC)) / (armslop - sp2Slop);  //??? 
	crossPt = GVector3f(crossX, crossY, SP2.Z);


	//3. cross pt에서 가장 가까운점 구하기
	pNode1 = m_FLSilhouetteV.m_pLeftEnd;
	minDis = 1000.0f;
	limitXIn = SP2.X - 10.0f;

	armfoldPt = m_LandMarkPose2[46];
	armfoldPt.X -= (m_BustCObesity - criticObesity)*200.0f; // 비만일수록 armfold의 x좌표를 안쪽으로 해준다
	if (armfoldPt.X > (crossPt.X + 10.0f))
		armfoldPt.X = crossPt.X - 50.f;

	if (m_UnderBustCObesity < 0.602) 	//마른체형들은 crosspt와 겨드랑앞점힘선과의 선분과의 교차점으로 한다
	{
		for (int i = 0; i < (num - 3); i++)
		{
			GVector3f pt, pt1;
			pt = pNode1->data;

			if (pNode1) {
				pNode1 = pNode1->m_pRight;
				pt1 = pNode1->data;

				if (pt.X > limitXIn && pt.Y > armDownPt.Y)
				{
					GVector3f intersectPt;
					if (::GetIntersectPointXY(crossPt, armfoldPt, pt1, pt, &intersectPt)) {
						intersectPt.Z = (pt.Z*0.5f) + (pt1.Z*0.5f);
						m_LandMarkHelp[21] = intersectPt;
					}
				}
			}

		}
	}
	else {
		// 3. cross pt에서 가장 가까운점 구하기
		armfoldPt.X -= 10.0f;
		for (int i = 0; i < (num - 3); i++)
		{
			GVector3f pt = pNode1->data;

			if (pt.X > limitXIn && pt.X < armDownPt.X)
			{
				float dis = SQRT(SQR(crossX - pt.X) + SQR(crossY - pt.Y)); //ABS(crossY-pt.Y);

				if (dis < minDis) {
					minDis = dis;
					m_LandMarkHelp[21] = pt;
				}
			}

			if (pNode1)  pNode1 = pNode1->m_pRight;

		}
	}

	if (m_LandMarkHelp[21].Y == 0.0f)
		m_LandMarkPose2[8] = tempShoulderPt;
	else
		m_LandMarkPose2[8] = m_LandMarkHelp[21];

}

//  어깨점 측면위치 결정
// 겨드랑접힘점 F[42], B[43]의 팔달린 방향에서의 이등분위치 겨드랑 달린방향 평면을 만들고,  어깨점 횡단면을 만들고 
// 어깨끝점에서 10mm 들어간 선분의 이등분점에서 수선을 그었을때 단면과 만나는 점
// 참고점-겨드랑앞뒤접힘점 F[42], B[43], 임시어깨점 Help R[20] L[21], 임시목앞점 Help[11]
void CSizeKorea2020Doc::FindShoulderPointZ1_V() {
	//-------------- 측면위치 결정------------------//	 
	//------------- Right
	GVector3f shPt = GVector3f((m_LandMarkHelp[20].X + 12.0f), m_LandMarkHelp[20].Y, m_LandMarkHelp[20].Z);

	// 겨드랑 달린방향 선분  x = armslopT*z + b
	float armslopT;
	if ((m_LandMarkPose2[44].Z == m_LandMarkPose2[44].Z))  armslopT = 10000.0f;
	else  armslopT = (m_LandMarkPose2[44].X - m_LandMarkPose2[44].X) / (m_LandMarkPose2[44].Z - m_LandMarkPose2[44].Z);
	if (armslopT < 0.0f) armslopT = 0.0f;  //뒤어깨인경우 바른어깨로  바꿔준다

	if (m_bWoman)     armslopT += 0.08749;   //0.08749 = 5도 더 기울인 것
	else      armslopT += 0.04;   //0.08749 = 5도 더 기울인 것
	float b = shPt.X - armslopT * shPt.Z;

	float FZ = shPt.Z + 100.0f;
	float FX = armslopT * FZ + b;

	float BZ = shPt.Z - 100.0f;
	float BX = armslopT * BZ + b;
	GVector3f FoutPt = GVector3f(FX, 0.0f, FZ);
	GVector3f BoutPt = GVector3f(BX, 0.0f, BZ);

	//어깨높이 황단면점 중에서 끝에서 횡단면 좌우 너비의 1/4되는 점들만 모음
	int shHeight = ROUND(m_LandMarkHelp[20].Y);
	//0.401425 =23도 /0.43633 = 25도 /0.48692f = 28도  0.471239 = 27도/ 0.50614=29도
	if (m_ShoulderAngleR > 0.471239f)
		shHeight -= -2.0f;

	if (shHeight % 2)
		shHeight++;
	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[shHeight].m_pLeftEnd;

	GObList<GVector3f> CrossPtListR;
	GObList<GVector3f> CrossPtListL;

	float Xlimit = (m_LandMarkHelp[21].X - m_LandMarkHelp[20].X)*0.2f;
	while (pNode) {
		GVector3f pt = pNode->data;

		if (pt.X < m_LandMarkHelp[11].X - Xlimit)
			CrossPtListR.Insert(pt);
		else if (pt.X > m_LandMarkHelp[11].X + Xlimit)
			CrossPtListL.Insert(pt);

		pNode = pNode->m_pRight;
	}


	CConvexHull cvx(CrossPtListR);
	cvx.m_HullPtList = CrossPtListR;
	cvx.ArrangeHullPtXZ();
	GObNode<GVector3f> *pNode1 = cvx.m_HullPtList.m_pLeftEnd;
	int num = cvx.m_HullPtList.m_nCount;


	// 어깨끝점 횡단면 선분중 교차하는 선분의 교차점을 구하는 방법
	GVector3f Fpt, Bpt, intersectPt;
	for (int i = 0; i < (num - 3); i++)
	{
		GVector3f pt, pt1;
		pt = pNode1->data;

		if (pNode1) {
			pNode1 = pNode1->m_pRight;
			pt1 = pNode1->data;

			if (::GetIntersectPointZX(FoutPt, BoutPt, pt1, pt, &intersectPt)) {
				intersectPt.Y = pt.Y;
				if (pt.Z > m_LandMarkHelp[20].Z)  //앞쪽점 
					Fpt = intersectPt;
				else
					Bpt = intersectPt;
			}
		}
	}

	m_LandMarkHelp[22] = Fpt;
	m_LandMarkHelp[23] = Bpt;

	GVector3f CP = (Bpt + Fpt)*0.5f;
	float slop = -1.0f / armslopT;
	b = CP.X - slop * CP.Z;
	float outZ = CP.Z + 100.0f;
	float outX = slop * outZ + b;
	GVector3f OutPt = GVector3f(outX, 0.0f, outZ);

	pNode1 = cvx.m_HullPtList.m_pLeftEnd;
	// 선분중 교차하는 선분의 교차점을 구하는 방법
	for (int i = 0; i < (num - 3); i++)
	{
		GVector3f pt, pt1;
		pt = pNode1->data;

		if (pNode1) {
			pNode1 = pNode1->m_pRight;
			pt1 = pNode1->data;

			if (pt.X < CP.X && pt1.X < CP.X)
			{
				GVector3f intersectPt;
				if (::GetIntersectPointZX(CP, OutPt, pt1, pt, &intersectPt)) {
					intersectPt.Y = CP.Y;
					m_LandMarkPose2[7] = intersectPt;
				}
			}
		}

	}
}

//어깨중앙점
void CSizeKorea2020Doc::FindShouderMid() {
	if (m_LandMarkPose2[4] == GLandMark(0.0f, 0.0f, 0.0f))  //목옆점
		return;
	if (m_LandMarkPose2[7] == GLandMark(0.0f, 0.0f, 0.0f))  //어깨가쪽점(r)
		return;

	GObList<GVector3f> CrossPtList;

	GLandMark a = m_LandMarkPose2[7]; // 어깨끝점
	GLandMark b = m_LandMarkPose2[4]; // 목옆점
	GLandMark c = (a + b) / 2.0f;
	GPlane refPln0, refPln1;
	GPlane pln(GVector3f(1.0f, 0.0f, 0.0f), c);

	refPln0.MakePlane(GVector3f(1.0f, 0.0f, 0.0f), b);
	refPln1.MakePlane(GVector3f(-1.0f, 0.0f, 0.0f), a);

	for (int i = 0; i < m_iNum; i += 3)
	{
		GVector3f v0 = m_Vertex[m_FaceIdx[i]];
		GVector3f v1 = m_Vertex[m_FaceIdx[i + 1]];
		GVector3f v2 = m_Vertex[m_FaceIdx[i + 2]];

		if (v0.Y < a.Y && v1.Y < a.Y && v2.Y < a.Y)
			continue;
		if (v0.Y > b.Y && v1.Y > b.Y && v2.Y > b.Y)
			continue;

		if (refPln0.IsAbovePlane(v0) && refPln0.IsAbovePlane(v1) && refPln0.IsAbovePlane(v2))
			continue;
		if (refPln1.IsAbovePlane(v0) && refPln1.IsAbovePlane(v1) && refPln1.IsAbovePlane(v2))
			continue;

		if (pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2))
			continue;
		if (pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2))
			continue;

		if (pln.GetSignedDistance(v0) * pln.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(v0, v1 - v0);
			CrossPtList.Insert(CrossPt);
		}
		if (pln.GetSignedDistance(v1) * pln.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(v1, v2 - v1);
			CrossPtList.Insert(CrossPt);
		}
		if (pln.GetSignedDistance(v2) * pln.GetSignedDistance(v0) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(v0, v2 - v0);
			CrossPtList.Insert(CrossPt);
		}
	}

	GVector3f M36(0.0f, -10000.0f, 0.0f);
	GObNode<GVector3f> *pNode = CrossPtList.m_pLeftEnd;
	while (pNode)
	{
		GVector3f pt = pNode->data;
		if (pt.Y > M36.Y)
			M36 = pt;
		pNode = pNode->m_pRight;
	}

	m_LandMarkPose2[36] = M36;
}

// Find Neck Front Pt (3)
// 관상면 z단면 만들어서 탐색
// 참고점:  목옆점, 임시목앞점(Help11), 정중시상면, 젖가슴아래비만도
void CSizeKorea2020Doc::FindNeckFrontPointV1() {
	if (m_BustCObesity < 0.57) {  //bmi 25일때  m_BustCObesity=0.602
		m_LandMarkPose2[3] = m_LandMarkHelp[11];
		return;
	}

	GVector3f FNeckPt = m_LandMarkHelp[11];
	GVector3f FNeckPt1 = FNeckPt;
	FNeckPt1.Y += 10.0f;

	float disYFNeckJaw = m_LandMarkHelp[34].Y - FNeckPt.Y;  //임시목앞점과 턱안점의 높이 차이


	if (disYFNeckJaw > 25.0f) FNeckPt1.Y += 5.0f;

	GPlane plnup, pln0, pln1, pln2, pln3, pln4;
	GVector3f SNeckR = m_LandMarkPose2[4];
	GVector3f SNeckL = m_LandMarkPose2[5];
	//좌우 목옆점의 z좌표를 같게하는 이유는, z좌표가 다를때 plnup이 기울어서  plnup이상인 점을제외시킬때 목둘레꺽임점도 제외되는일을 방지하기 위해서이다
	SNeckR.Z = SNeckL.Z = (SNeckR.Z + SNeckL.Z)*0.5f;
	SNeckR.Y = SNeckL.Y = (SNeckR.Y + SNeckL.Y)*0.5f + 20.0f;
	plnup.MakePlane(SNeckL, SNeckR, FNeckPt1); //좌우 목옆점과 임시 목앞점으로 형성되는 평면아래의 점들만

	float tmpZ0 = (SNeckR.Z*0.8) + (FNeckPt.Z*0.2f);
	float tmpZ1 = (SNeckR.Z*0.6) + (FNeckPt.Z*0.4f);
	float tmpZ2 = (SNeckR.Z*0.4) + (FNeckPt.Z*0.6f);
	float tmpZ3 = (SNeckR.Z*0.8) + (FNeckPt.Z*0.2f);
	float tmpZ4;
	//float tmpZ3 = FNeckPt.Z;

	//else  	tmpZ3=FNeckPt.Z+2.0f; // 비만하면 임시목앞점이 많이 위, 뒤쪽일 가능성이 많으므로 약간 앞쪽에 평면을 만듦
	// 숙인목이면 임시목앞점 그대로

	// 임시목앞점과 턱안점이 짧으면 세운목이면 좀더 앞쪽으로.. 
	if (disYFNeckJaw < 10.0f)
		tmpZ4 = FNeckPt.Z + 7.0f;
	else if (disYFNeckJaw < 25.0f)
		tmpZ4 = FNeckPt.Z + 4.0f;
	else if (disYFNeckJaw < 38.0f)
		tmpZ4 = FNeckPt.Z + 2.0f;
	else tmpZ4 = FNeckPt.Z;


	pln0.MakePlane(GVector3f(0.0, 0.0, 1.0), GVector3f(FNeckPt.X, FNeckPt.Y, tmpZ0));
	pln1.MakePlane(GVector3f(0.0, 0.0, 1.0), GVector3f(FNeckPt.X, FNeckPt.Y, tmpZ1));
	pln2.MakePlane(GVector3f(0.0, 0.0, 1.0), GVector3f(FNeckPt.X, FNeckPt.Y, tmpZ2));
	pln3.MakePlane(GVector3f(0.0, 0.0, 1.0), GVector3f(FNeckPt.X, FNeckPt.Y, tmpZ3));
	pln4.MakePlane(GVector3f(0.0, 0.0, 1.0), GVector3f(FNeckPt.X, FNeckPt.Y, tmpZ4));

	float low_height = FNeckPt.Y - 100.0f;
	//float upper_height = SNeckR.Y;
	float CX = FNeckPt.X;

	GObList<GVector3f> CrossPtListL[5], CrossPtListR[5];

	//목옆과 목앞 사이의 xy단면들을 만든다
	for (long l = 0L; l < m_iNum; l += 3)
	{
		GVector3f v0, v1, v2;
		v0 = m_Vertex[m_FaceIdx[l]];
		v1 = m_Vertex[m_FaceIdx[l + 1]];
		v2 = m_Vertex[m_FaceIdx[l + 2]];

		if (v0.Y < low_height   && v1.Y < low_height   && v2.Y < low_height)
			continue;

		if (plnup.IsAbovePlane(v0) && plnup.IsAbovePlane(v1) && plnup.IsAbovePlane(v2))
			continue;

		if (pln0.GetSignedDistance(v0) * pln0.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = pln0.GetProjectionPt(v0, v1 - v0);
			if (CrossPt.X > CX) CrossPtListL[0].Insert(CrossPt); //left
			else CrossPtListR[0].Insert(CrossPt);  //right
		}

		if (pln1.GetSignedDistance(v0) * pln1.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = pln1.GetProjectionPt(v0, v1 - v0);
			if (CrossPt.X > CX) CrossPtListL[1].Insert(CrossPt); //left
			else CrossPtListR[1].Insert(CrossPt);  //right
		}

		if (pln2.GetSignedDistance(v0) * pln2.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = pln2.GetProjectionPt(v0, v1 - v0);
			if (CrossPt.X > CX) CrossPtListL[2].Insert(CrossPt);
			else CrossPtListR[2].Insert(CrossPt);
		}

		if (pln3.GetSignedDistance(v0) * pln3.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = pln3.GetProjectionPt(v0, v1 - v0);
			if (CrossPt.X > CX) CrossPtListL[3].Insert(CrossPt);
			else CrossPtListR[3].Insert(CrossPt);
		}

		if (pln4.GetSignedDistance(v0) * pln4.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = pln4.GetProjectionPt(v0, v1 - v0);
			if (CrossPt.X > CX) CrossPtListL[4].Insert(CrossPt);
			else CrossPtListR[4].Insert(CrossPt);
		}

		//-------------------
		if (pln0.GetSignedDistance(v1) * pln0.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = pln0.GetProjectionPt(v1, v2 - v1);
			if (CrossPt.X > CX) CrossPtListL[0].Insert(CrossPt); //left
			else CrossPtListR[0].Insert(CrossPt);  //right
		}

		if (pln1.GetSignedDistance(v1) * pln1.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = pln1.GetProjectionPt(v1, v2 - v1);
			if (CrossPt.X > CX) CrossPtListL[1].Insert(CrossPt);
			else CrossPtListR[1].Insert(CrossPt);
		}

		if (pln2.GetSignedDistance(v1) * pln2.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = pln2.GetProjectionPt(v1, v2 - v1);
			if (CrossPt.X > CX) CrossPtListL[2].Insert(CrossPt);
			else CrossPtListR[2].Insert(CrossPt);
		}

		if (pln3.GetSignedDistance(v1) * pln3.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = pln3.GetProjectionPt(v1, v2 - v1);
			if (CrossPt.X > CX) CrossPtListL[3].Insert(CrossPt);
			else CrossPtListR[3].Insert(CrossPt);
		}

		if (pln4.GetSignedDistance(v1) * pln4.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = pln4.GetProjectionPt(v1, v2 - v1);
			if (CrossPt.X > CX) CrossPtListL[4].Insert(CrossPt);
			else CrossPtListR[4].Insert(CrossPt);
		}

		//-------------------
		if (pln0.GetSignedDistance(v1) * pln0.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = pln0.GetProjectionPt(v2, v0 - v2);
			if (CrossPt.X > CX) CrossPtListL[0].Insert(CrossPt); //left
			else CrossPtListR[0].Insert(CrossPt);  //right
		}

		if (pln1.GetSignedDistance(v2) * pln1.GetSignedDistance(v0) < 0)
		{
			GVector3f CrossPt = pln1.GetProjectionPt(v2, v0 - v2);
			if (CrossPt.X > CX) CrossPtListL[1].Insert(CrossPt);
			else CrossPtListR[1].Insert(CrossPt);
		}

		if (pln2.GetSignedDistance(v2) * pln2.GetSignedDistance(v0) < 0)
		{
			GVector3f CrossPt = pln2.GetProjectionPt(v2, v0 - v2);
			if (CrossPt.X > CX) CrossPtListL[2].Insert(CrossPt);
			else CrossPtListR[2].Insert(CrossPt);
		}

		if (pln3.GetSignedDistance(v2) * pln3.GetSignedDistance(v0) < 0)
		{
			GVector3f CrossPt = pln3.GetProjectionPt(v2, v0 - v2);
			if (CrossPt.X > CX) CrossPtListL[3].Insert(CrossPt);
			else CrossPtListR[3].Insert(CrossPt);
		}

		if (pln4.GetSignedDistance(v0) * pln4.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = pln4.GetProjectionPt(v2, v0 - v2);
			if (CrossPt.X > CX) CrossPtListL[4].Insert(CrossPt);
			else CrossPtListR[4].Insert(CrossPt);
		}
	}

	//  Y좌표 - 위에서 아래로 정렬 //0 : less->greater
	::getSortedPoint(CrossPtListL[0], 1, 1);
	::getSortedPoint(CrossPtListL[1], 1, 1);
	::getSortedPoint(CrossPtListL[2], 1, 1);
	::getSortedPoint(CrossPtListL[3], 0, 0); //0, 0
	::getSortedPoint(CrossPtListL[4], 0, 0);
	::getSortedPoint(CrossPtListR[0], 1, 1);
	::getSortedPoint(CrossPtListR[1], 1, 1);
	::getSortedPoint(CrossPtListR[2], 1, 1);
	::getSortedPoint(CrossPtListR[3], 0, 1);
	::getSortedPoint(CrossPtListR[4], 0, 1);
	int numL1 = CrossPtListL[1].m_nCount;
	int numL2 = CrossPtListL[2].m_nCount;
	int numL3 = CrossPtListL[3].m_nCount;
	int numR1 = CrossPtListR[1].m_nCount;
	int numR2 = CrossPtListR[2].m_nCount;
	int numR3 = CrossPtListR[3].m_nCount;


	//각 단면 왼, 오른쪽에서 기울기가 눕기 시작하는 점을 찾는다
	GVector3f L1, L2, L3, R1, R2, R3;
	GVector3f prePt;
	GObNode<GVector3f> *pNode1 = CrossPtListR[1].m_pLeftEnd;
	float slop = 100.0f, preslop = 100.0f; // , slopDiff=0.0f;
	int gap = 2, gap3 = 3;

	//Right 1
	for (long j = 0; j < (numR1 - 4); j += gap3)
	{
		GVector3f Pt = pNode1->data;

		if (j > 0) {
			if (Pt.X != prePt.X)
				slop = (Pt.Y - prePt.Y) / (Pt.X - prePt.X);
		}
		if (slop < 1.3f && slop > 0.0f) {
			R1 = prePt;
			break;
		}
		prePt = Pt;
		preslop = slop;

		pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;

	}
	//Right 2
	pNode1 = CrossPtListR[2].m_pLeftEnd;
	slop = 100.0f, preslop = 100.0f;
	for (int j = 0; j < (numR2 - 4); j += gap)
	{
		GVector3f Pt = pNode1->data;

		if (j > 0) {
			if (Pt.X != prePt.X)
				slop = (Pt.Y - prePt.Y) / (Pt.X - prePt.X);
		}
		if (slop < 1.5f && slop > 0.0f) {
			R2 = prePt;
			break;
		}
		prePt = Pt;
		preslop = slop;

		pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}
	//Right 3
	pNode1 = CrossPtListR[3].m_pLeftEnd;
	slop = 100.0f, preslop = 100.0f;
	float limitx = FNeckPt.X - 10.0f;
	float limitx1 = FNeckPt.X - 50.0f;
	for (int j = 0; j < (numR3 - 4); j += gap) //while(pNode1) //
	{
		GVector3f Pt = pNode1->data;
		float xdiff = ABS(Pt.X - prePt.X);
		if (j > 0) {
			if (Pt.X != prePt.X)
				slop = (Pt.Y - prePt.Y) / (Pt.X - prePt.X);
			//slopDiff = preslop - slop;
		}
		if (slop > -0.8f && slop < 1.5f && prePt.Y<(FNeckPt.Y + 6.0f) && prePt.X>limitx1 && prePt.X < limitx && xdiff < 5.0f) { //
			R3 = prePt;
			break;
		}
		prePt = Pt;
		preslop = slop;

		pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	//Left 3
	pNode1 = CrossPtListL[3].m_pLeftEnd;
	slop = 100.0f, preslop = 100.0f;
	limitx = FNeckPt.X + 10.0f;
	limitx1 = FNeckPt.X + 50.0f;
	for (int j = 0; j < (numL3 - 4); j += gap)
	{
		GVector3f Pt = pNode1->data;
		float xdiff = ABS(Pt.X - prePt.X);
		if (j > 0) {
			if (Pt.X != prePt.X)
				slop = (Pt.Y - prePt.Y) / (Pt.X - prePt.X);
		}
		if (slop < 0.8f && slop>-1.0f && prePt.Y<(FNeckPt.Y + 6.0f) && prePt.X>limitx && prePt.X < limitx1 && xdiff < 5.0f) {  //  
			L3 = prePt;  //(FNeckPt.Y+6.0)
			break;
		}
		prePt = Pt;
		preslop = slop;

		pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	//Left 2
	pNode1 = CrossPtListL[2].m_pLeftEnd;
	slop = 100.0f, preslop = 100.0f;
	for (int j = 0; j < (numL2 - 4); j += gap)
	{
		GVector3f Pt = pNode1->data;

		if (j > 0) {
			if (Pt.X != prePt.X)
				slop = (Pt.Y - prePt.Y) / (Pt.X - prePt.X);
		}
		if (slop < 0.0f  && slop>-1.5f) {
			L2 = prePt;
			break;
		}
		prePt = Pt;
		preslop = slop;

		pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	//Left 1
	pNode1 = CrossPtListL[1].m_pLeftEnd;
	slop = 100.0f, preslop = 100.0f;
	for (int j = 0; j < (numL1 - 4); j += gap)
	{
		GVector3f Pt = pNode1->data;

		if (j > 0) {
			if (Pt.X != prePt.X)
				slop = (Pt.Y - prePt.Y) / (Pt.X - prePt.X);
		}
		if (slop < 0.0f && slop>-1.3f) {
			L1 = prePt;
			break;
		}
		prePt = Pt;
		preslop = slop;

		pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	R3.Y -= 2.0f;  L3.Y -= 2.0f;
	m_LandMarkHelp[28] = R1;
	m_LandMarkHelp[29] = R2;
	m_LandMarkHelp[30] = R3;
	m_LandMarkHelp[31] = L1;
	m_LandMarkHelp[32] = L2;
	m_LandMarkHelp[33] = L3;

	// nurbs 곡선을 만든다
	REAL4 P[6];
	P[0][0] = R1[0];	P[0][1] = R1[1];   P[0][2] = R1[2];   P[0][3] = 1.0f;
	P[1][0] = R2[0];	P[1][1] = R2[1];   P[1][2] = R2[2];   P[1][3] = 1.0f;
	P[2][0] = R3[0];	P[2][1] = R3[1];   P[2][2] = R3[2];   P[2][3] = 1.0f;
	P[3][0] = L3[0];	P[3][1] = L3[1];   P[3][2] = L3[2];   P[3][3] = 1.0f;
	P[4][0] = L2[0];	P[4][1] = L2[1];   P[4][2] = L2[2];   P[4][3] = 1.0f;
	P[5][0] = L1[0];	P[5][1] = L1[1];   P[5][2] = L1[2];   P[5][3] = 1.0f;

	GNurbsCrv PathCrv;
	PathCrv.InterCrv(4, P, 5);


	// 곡선점들중에 임시 목앞점의 x위치와 가장 가까운 점을 구하여 mesh에 projection시켜 구한다
	int snum = 100;
	float minDis = 10000.0f;
	REAL4 v0, v1;
	GVector3f FNv, p0, p1;
	REAL min = PathCrv.GetMinKnotVal();
	REAL max = PathCrv.GetMaxKnotVal();
	for (int i = 0; i < snum; i++)
	{
		REAL u0 = min + ((max - min)*i) / (REAL)(snum);
		REAL u1 = min + ((max - min)*(i + 1)) / (REAL)(snum);

		PathCrv.Eval(u0, v0);
		PathCrv.Eval(u1, v1);

		p0 = GVector3f(v0[0], v0[1], v0[2]);
		p1 = GVector3f(v0[0], v0[1], v0[2]);

		m_FNeckCrv.Insert(p0);
		m_FNeckCrv.Insert(p1);

		float dis = (FNeckPt.X - v0[0])*(FNeckPt.X - v1[0]);
		if (dis < 0.0f) {
			GVector3f a = p0 * 0.5f;
			GVector3f b = p0 * 0.5f;
			FNv = a + b;
			//			FNv = (p0*0.5f) + (p1*0.5f) ;
			FNv.X = FNeckPt.X; //minDis=dis;
		}
	}

	//	m_LandMarkPose2[8] = FNv;

	GObList<GVector3f> CrossPtList;


	GPlane cutPln;  //GVector3f(FNv) 임시목앞점을 지나는 YZ평면(정중시상면)
	cutPln.MakePlane(GVector3f(1.0, 0.0, 0.0), m_LandMarkHelp[11]);
	float limitY = FNv[1] + 8.0f;
	float limitY1 = FNv[1] - 4.0f;
	float limitX = FNv[0] + 10.0f;
	float limitX1 = FNv[0] - 10.0f;
	float limitZ = FNv[2] - 50.0f;

	float dis;
	minDis = 10000.0f;

	for (int i = 0; i < m_iNum; i += 3)
	{
		GVector3f v0, v1, v2;
		v0 = m_Vertex[m_FaceIdx[i]];
		v1 = m_Vertex[m_FaceIdx[i + 1]];
		v2 = m_Vertex[m_FaceIdx[i + 2]];

		// culling.
		if (v0.Y > limitY || v0.Y < limitY1 || v0.X > limitX || v0.X < limitX1 || v0.Z < limitZ)
			continue;
		if (v1.Y > limitY || v1.Y < limitY1 || v1.X > limitX || v1.X < limitX1 || v0.Z < limitZ)
			continue;
		if (v2.Y > limitY || v2.Y < limitY1 || v2.X > limitX || v2.X < limitX1 || v0.Z < limitZ)
			continue;

		if (cutPln.GetSignedDistance(v0) * cutPln.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = cutPln.GetProjectionPt(v0, v1 - v0);
			dis = ABS(FNv.Y - CrossPt.Y); //ABS(FNeckPt.X-CrossPt.X);
			if (dis < minDis) {
				FNeckPt = CrossPt;
				minDis = dis;
			}
		}

		if (cutPln.GetSignedDistance(v1) * cutPln.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = cutPln.GetProjectionPt(v1, v2 - v1);
			dis = ABS(FNv.Y - CrossPt.Y); //ABS(FNeckPt.X-CrossPt.X);
			if (dis < minDis) {
				FNeckPt = CrossPt;
				minDis = dis;
			}
		}
		if (cutPln.GetSignedDistance(v2) * cutPln.GetSignedDistance(v0) < 0)
		{
			GVector3f CrossPt = cutPln.GetProjectionPt(v0, v2 - v0);
			dis = ABS(FNv.Y - CrossPt.Y); //ABS(FNeckPt.X-CrossPt.X);
			if (dis < minDis) {
				FNeckPt = CrossPt;
				minDis = dis;
			}
		}
	} //for

	if (FNeckPt.Y > m_LandMarkHelp[11].Y)  //목앞점이 목앞오목점보다 높으면 목앞오목점을 그냥 목앞점으로 한다.
		m_LandMarkPose2[3] = m_LandMarkHelp[11];
	else m_LandMarkPose2[3] = FNeckPt;
}

// Find Neck Front Pt - 목꺽임점을  사선으로부터의 거리로 만드는 방법  -아직수정안했음
// 관상면 z단면 만들어서 탐색
// 참고점:  목옆점, 임시목앞점(Help11), 정중시상면, 젖가슴아래비만도
void CSizeKorea2020Doc::FindNeckFrontPointV_1() {
	if (m_BustCObesity < 0.57) {  //bmi 25일때  m_BustCObesity=0.602
		m_LandMarkPose2[3] = m_LandMarkHelp[11];
		return;
	}

	GVector3f FNeckPt = m_LandMarkHelp[11];
	GVector3f FNeckPt1 = FNeckPt;
	FNeckPt1.Y += 10.0f;

	float disYFNeckJaw = m_LandMarkHelp[34].Y - FNeckPt.Y;  //임시목앞점과 턱안점의 높이 차이


	if (disYFNeckJaw > 25.0f) FNeckPt1.Y += 5.0f;

	GPlane plnup, pln1, pln2, pln3;
	GVector3f SNeckR = m_LandMarkPose2[4];
	GVector3f SNeckL = m_LandMarkPose2[5];
	//좌우 목옆점의 z좌표를 같게하는 이유는, z좌표가 다를때 plnup이 기울어서  plnup이상인 점을제외시킬때 목둘레꺽임점도 제외되는일을 방지하기 위해서이다
	SNeckR.Z = SNeckL.Z = (SNeckR.Z + SNeckL.Z)*0.5f;
	SNeckR.Y = SNeckL.Y = (SNeckR.Y + SNeckL.Y)*0.5f + 20.0f;
	plnup.MakePlane(SNeckL, SNeckR, FNeckPt1); //좌우 목옆점과 임시 목앞점으로 형성되는 평면아래의 점들만

	float tmpZ1 = (SNeckR.Z*0.65) + (FNeckPt.Z*0.35f);
	float tmpZ2 = (SNeckR.Z*0.3) + (FNeckPt.Z*0.7f);
	float tmpZ3;
	//float tmpZ3 = FNeckPt.Z;

	//else  	tmpZ3=FNeckPt.Z+2.0f; // 비만하면 임시목앞점이 많이 위, 뒤쪽일 가능성이 많으므로 약간 앞쪽에 평면을 만듦
	// 숙인목이면 임시목앞점 그대로

	// 임시목앞점과 턱안점이 짧으면 세운목이면 좀더 앞쪽으로.. 
	if (disYFNeckJaw < 10.0f)
		tmpZ3 = FNeckPt.Z + 7.0f;
	else if (disYFNeckJaw < 25.0f)
		tmpZ3 = FNeckPt.Z + 4.0f;
	else if (disYFNeckJaw < 38.0f)
		tmpZ3 = FNeckPt.Z + 2.0f;
	else tmpZ3 = FNeckPt.Z;


	pln1.MakePlane(GVector3f(0.0, 0.0, 1.0), GVector3f(FNeckPt.X, FNeckPt.Y, tmpZ1));
	pln2.MakePlane(GVector3f(0.0, 0.0, 1.0), GVector3f(FNeckPt.X, FNeckPt.Y, tmpZ2));
	pln3.MakePlane(GVector3f(0.0, 0.0, 1.0), GVector3f(FNeckPt.X, FNeckPt.Y, tmpZ3));

	float low_height = FNeckPt.Y - 100.0f;
	//float upper_height = SNeckR.Y;
	float CX = FNeckPt.X;

	GObList<GVector3f> CrossPtListL1, CrossPtListL2, CrossPtListL3,
		CrossPtListR1, CrossPtListR2, CrossPtListR3;

	//목옆과 목앞 사이의 xy단면들을 만든다
	for (long l = 0L; l < m_iNum; l += 3)
	{
		GVector3f v0, v1, v2;
		v0 = m_Vertex[m_FaceIdx[l]];
		v1 = m_Vertex[m_FaceIdx[l + 1]];
		v2 = m_Vertex[m_FaceIdx[l + 2]];

		if (v0.Y < low_height   && v1.Y < low_height   && v2.Y < low_height)
			continue;
		//if ( v0.Y > upper_height && v1.Y > upper_height && v2.Y > upper_height )
		//	continue;

		if (plnup.IsAbovePlane(v0) && plnup.IsAbovePlane(v1) && plnup.IsAbovePlane(v2))
			continue;

		if (pln1.GetSignedDistance(v0) * pln1.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = pln1.GetProjectionPt(v0, v1 - v0);
			if (CrossPt.X > CX) CrossPtListL1.Insert(CrossPt); //left
			else CrossPtListR1.Insert(CrossPt);  //right
		}

		if (pln2.GetSignedDistance(v0) * pln2.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = pln2.GetProjectionPt(v0, v1 - v0);
			if (CrossPt.X > CX) CrossPtListL2.Insert(CrossPt);
			else CrossPtListR2.Insert(CrossPt);
		}

		if (pln3.GetSignedDistance(v0) * pln3.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = pln3.GetProjectionPt(v0, v1 - v0);
			if (CrossPt.X > CX) CrossPtListL3.Insert(CrossPt);
			else CrossPtListR3.Insert(CrossPt);
		}

		//-------------------
		if (pln1.GetSignedDistance(v1) * pln1.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = pln1.GetProjectionPt(v1, v2 - v1);
			if (CrossPt.X > CX) CrossPtListL1.Insert(CrossPt);
			else CrossPtListR1.Insert(CrossPt);
		}

		if (pln2.GetSignedDistance(v1) * pln2.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = pln2.GetProjectionPt(v1, v2 - v1);
			if (CrossPt.X > CX) CrossPtListL2.Insert(CrossPt);
			else CrossPtListR2.Insert(CrossPt);
		}

		if (pln3.GetSignedDistance(v1) * pln3.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = pln3.GetProjectionPt(v1, v2 - v1);
			if (CrossPt.X > CX) CrossPtListL3.Insert(CrossPt);
			else CrossPtListR3.Insert(CrossPt);
		}


		//-------------------
		if (pln1.GetSignedDistance(v2) * pln1.GetSignedDistance(v0) < 0)
		{
			GVector3f CrossPt = pln1.GetProjectionPt(v2, v0 - v2);
			if (CrossPt.X > CX) CrossPtListL1.Insert(CrossPt);
			else CrossPtListR1.Insert(CrossPt);
		}

		if (pln2.GetSignedDistance(v2) * pln2.GetSignedDistance(v0) < 0)
		{
			GVector3f CrossPt = pln2.GetProjectionPt(v2, v0 - v2);
			if (CrossPt.X > CX) CrossPtListL2.Insert(CrossPt);
			else CrossPtListR2.Insert(CrossPt);
		}

		if (pln3.GetSignedDistance(v2) * pln3.GetSignedDistance(v0) < 0)
		{
			GVector3f CrossPt = pln3.GetProjectionPt(v2, v0 - v2);
			if (CrossPt.X > CX) CrossPtListL3.Insert(CrossPt);
			else CrossPtListR3.Insert(CrossPt);
		}

	}

	//  Y좌표 - 위에서 아래로 정렬
	::getSortedPoint(CrossPtListL1, 1, 1);
	::getSortedPoint(CrossPtListL2, 1, 1);
	::getSortedPoint(CrossPtListL3, 0, 0); //0, 0
	::getSortedPoint(CrossPtListR1, 1, 1);
	::getSortedPoint(CrossPtListR2, 1, 1);
	::getSortedPoint(CrossPtListR3, 0, 1);
	int numL1 = CrossPtListL1.m_nCount;
	int numL2 = CrossPtListL2.m_nCount;
	int numL3 = CrossPtListL3.m_nCount;
	int numR1 = CrossPtListR1.m_nCount;
	int numR2 = CrossPtListR2.m_nCount;
	int numR3 = CrossPtListR3.m_nCount;

	//각 단면 왼, 오른쪽에서 기울기가 눕기 시작하는 점을 찾는다
	GVector3f L1, L2, L3, R1, R2, R3;
	GVector3f prePt;
	GObNode<GVector3f> *pNode1 = CrossPtListR1.m_pLeftEnd;
	float slop = 100.0f, preslop = 100.0f; // , slopDiff=0.0f;
	int gap = 2, gap3 = 3;

	//Right 1
	for (long j = 0; j < (numR1 - 4); j += gap3)
	{
		GVector3f Pt = pNode1->data;

		if (j > 0) {
			if (Pt.X != prePt.X)
				slop = (Pt.Y - prePt.Y) / (Pt.X - prePt.X);
		}
		if (slop < 1.3f && slop > 0.0f) {
			R1 = prePt;
			break;
		}
		prePt = Pt;
		preslop = slop;

		pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;

	}
	//Right 2
	pNode1 = CrossPtListR2.m_pLeftEnd;
	slop = 100.0f, preslop = 100.0f;
	for (int j = 0; j < (numR2 - 4); j += gap)
	{
		GVector3f Pt = pNode1->data;

		if (j > 0) {
			if (Pt.X != prePt.X)
				slop = (Pt.Y - prePt.Y) / (Pt.X - prePt.X);
		}
		if (slop < 1.5f && slop > 0.0f) {
			R2 = prePt;
			break;
		}
		prePt = Pt;
		preslop = slop;

		pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}
	//Right 3
	pNode1 = CrossPtListR3.m_pLeftEnd;
	slop = 100.0f, preslop = 100.0f;
	float limitx = FNeckPt.X - 10.0f;
	float limitx1 = FNeckPt.X - 50.0f;
	for (int j = 0; j < (numR3 - 4); j += gap) //while(pNode1) //
	{
		GVector3f Pt = pNode1->data;
		float xdiff = ABS(Pt.X - prePt.X);
		if (j > 0) {
			if (Pt.X != prePt.X)
				slop = (Pt.Y - prePt.Y) / (Pt.X - prePt.X);
			//slopDiff = preslop - slop;
		}
		if (slop > -0.8f && slop < 1.5f && prePt.Y<(FNeckPt.Y + 6.0f) && prePt.X>limitx1 && prePt.X < limitx && xdiff < 5.0f) { //
			R3 = prePt;
			break;
		}
		prePt = Pt;
		preslop = slop;

		pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	//Left 3
	pNode1 = CrossPtListL3.m_pLeftEnd;
	slop = 100.0f, preslop = 100.0f;
	limitx = FNeckPt.X + 10.0f;
	limitx1 = FNeckPt.X + 50.0f;
	for (int j = 0; j < (numL3 - 4); j += gap)
	{
		GVector3f Pt = pNode1->data;
		float xdiff = ABS(Pt.X - prePt.X);
		if (j > 0) {
			if (Pt.X != prePt.X)
				slop = (Pt.Y - prePt.Y) / (Pt.X - prePt.X);
		}
		if (slop < 0.8f && slop>-1.0f && prePt.Y<(FNeckPt.Y + 6.0f) && prePt.X>limitx && prePt.X < limitx1 && xdiff < 5.0f) {  //  
			L3 = prePt;  //(FNeckPt.Y+6.0)
			break;
		}
		prePt = Pt;
		preslop = slop;

		pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	//Left 2
	pNode1 = CrossPtListL2.m_pLeftEnd;
	slop = 100.0f, preslop = 100.0f;
	for (int j = 0; j < (numL2 - 4); j += gap)
	{
		GVector3f Pt = pNode1->data;

		if (j > 0) {
			if (Pt.X != prePt.X)
				slop = (Pt.Y - prePt.Y) / (Pt.X - prePt.X);
		}
		if (slop < 0.0f  && slop>-1.5f) {
			L2 = prePt;
			break;
		}
		prePt = Pt;
		preslop = slop;

		pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	//Left 1
	pNode1 = CrossPtListL1.m_pLeftEnd;
	slop = 100.0f, preslop = 100.0f;
	for (int j = 0; j < (numL1 - 4); j += gap)
	{
		GVector3f Pt = pNode1->data;

		if (j > 0) {
			if (Pt.X != prePt.X)
				slop = (Pt.Y - prePt.Y) / (Pt.X - prePt.X);
		}
		if (slop < 0.0f && slop>-1.3f) {
			L1 = prePt;
			break;
		}
		prePt = Pt;
		preslop = slop;

		pNode1 = pNode1->m_pRight;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	R3.Y -= 2.0f;  L3.Y -= 2.0f;
	m_LandMarkHelp[28] = R1;
	m_LandMarkHelp[29] = R2;
	m_LandMarkHelp[30] = R3;
	m_LandMarkHelp[31] = L1;
	m_LandMarkHelp[32] = L2;
	m_LandMarkHelp[33] = L3;

	// nurbs 곡선을 만든다
	REAL4 P[6];
	P[0][0] = R1[0];	P[0][1] = R1[1];   P[0][2] = R1[2];   P[0][3] = 1.0f;
	P[1][0] = R2[0];	P[1][1] = R2[1];   P[1][2] = R2[2];   P[1][3] = 1.0f;
	P[2][0] = R3[0];	P[2][1] = R3[1];   P[2][2] = R3[2];   P[2][3] = 1.0f;
	P[3][0] = L3[0];	P[3][1] = L3[1];   P[3][2] = L3[2];   P[3][3] = 1.0f;
	P[4][0] = L2[0];	P[4][1] = L2[1];   P[4][2] = L2[2];   P[4][3] = 1.0f;
	P[5][0] = L1[0];	P[5][1] = L1[1];   P[5][2] = L1[2];   P[5][3] = 1.0f;

	GNurbsCrv PathCrv;
	PathCrv.InterCrv(4, P, 5);


	// 곡선점들중에 임시 목앞점의 x위치와 가장 가까운 점을 구하여 mesh에 projection시켜 구한다
	int snum = 100;
	float minDis = 10000.0f;
	REAL4 v0, v1;
	GVector3f FNv, p0, p1;
	REAL min = PathCrv.GetMinKnotVal();
	REAL max = PathCrv.GetMaxKnotVal();
	for (int i = 0; i < snum; i++)
	{
		REAL u0 = min + ((max - min)*i) / (REAL)(snum);
		REAL u1 = min + ((max - min)*(i + 1)) / (REAL)(snum);

		PathCrv.Eval(u0, v0);
		PathCrv.Eval(u1, v1);

		p0 = GVector3f(v0[0], v0[1], v0[2]);
		p1 = GVector3f(v0[0], v0[1], v0[2]);

		m_FNeckCrv.Insert(p0);
		m_FNeckCrv.Insert(p1);

		float dis = (FNeckPt.X - v0[0])*(FNeckPt.X - v1[0]);
		if (dis < 0.0f) {
			GVector3f a = p0 * 0.5f;
			GVector3f b = p0 * 0.5f;
			FNv = a + b;
			//			FNv = (p0*0.5f) + (p1*0.5f) ;
			FNv.X = FNeckPt.X; //minDis=dis;
		}
	}

	//	m_LandMarkPose2[8] = FNv;

	GObList<GVector3f> CrossPtList;


	GPlane cutPln;  //GVector3f(FNv) 임시목앞점을 지나는 YZ평면(정중시상면)
	cutPln.MakePlane(GVector3f(1.0, 0.0, 0.0), m_LandMarkHelp[11]);
	float limitY = FNv[1] + 8.0f;
	float limitY1 = FNv[1] - 4.0f;
	float limitX = FNv[0] + 10.0f;
	float limitX1 = FNv[0] - 10.0f;
	float limitZ = FNv[2] - 50.0f;

	float dis;
	minDis = 10000.0f;

	for (int i = 0; i < m_iNum; i += 3)
	{
		GVector3f v0, v1, v2;
		v0 = m_Vertex[m_FaceIdx[i]];
		v1 = m_Vertex[m_FaceIdx[i + 1]];
		v2 = m_Vertex[m_FaceIdx[i + 2]];

		// culling.
		if (v0.Y > limitY || v0.Y < limitY1 || v0.X > limitX || v0.X < limitX1 || v0.Z < limitZ)
			continue;
		if (v1.Y > limitY || v1.Y < limitY1 || v1.X > limitX || v1.X < limitX1 || v0.Z < limitZ)
			continue;
		if (v2.Y > limitY || v2.Y < limitY1 || v2.X > limitX || v2.X < limitX1 || v0.Z < limitZ)
			continue;

		if (cutPln.GetSignedDistance(v0) * cutPln.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = cutPln.GetProjectionPt(v0, v1 - v0);
			dis = ABS(FNv.Y - CrossPt.Y); //ABS(FNeckPt.X-CrossPt.X);
			if (dis < minDis) {
				FNeckPt = CrossPt;
				minDis = dis;
			}
		}

		if (cutPln.GetSignedDistance(v1) * cutPln.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = cutPln.GetProjectionPt(v1, v2 - v1);
			dis = ABS(FNv.Y - CrossPt.Y); //ABS(FNeckPt.X-CrossPt.X);
			if (dis < minDis) {
				FNeckPt = CrossPt;
				minDis = dis;
			}
		}
		if (cutPln.GetSignedDistance(v2) * cutPln.GetSignedDistance(v0) < 0)
		{
			GVector3f CrossPt = cutPln.GetProjectionPt(v0, v2 - v0);
			dis = ABS(FNv.Y - CrossPt.Y); //ABS(FNeckPt.X-CrossPt.X);
			if (dis < minDis) {
				FNeckPt = CrossPt;
				minDis = dis;
			}
		}
	} //for

	if (FNeckPt.Y > m_LandMarkHelp[11].Y)  //목앞점이 목앞오목점보다 높으면 목앞오목점을 그냥 목앞점으로 한다.
		m_LandMarkPose2[3] = m_LandMarkHelp[11];
	else m_LandMarkPose2[3] = FNeckPt;
}

//목앞점 단면에서 가장 앞의 점(좌,우 2개) 찾기 - 목밑둘레들 위해
void CSizeKorea2020Doc::FindNeckFrontPoint_maxZV() {
	if (m_LandMarkPose2[3].Y == 0.0f || !m_bArrangedXY)
		return;

	GVector3f FNeckPt = m_LandMarkPose2[3];

	//목앞점 단면에서 앞으로 가장 튀어나온 좌우점 찾기 - 목밑둘레들 위해
	int FNH = ROUND(FNeckPt.Y);
	int rem = FNH % m_HInterval;
	FNH = FNH - rem;
	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[FNH].m_pLeftEnd;
	float maxzR = -10000.0f;
	float maxzL = -10000.0f;
	while (pNode)
	{
		GVector3f pt = pNode->data;
		//GVector3f pt = m_Vertex[index];
		if (pt.Z > m_LandMarkHelp[35].Z)
		{
			if (pt.X > (FNeckPt.X + 6.0f)) //left
			{
				if (pt.X <(FNeckPt.X + 17.0f) && pt.Z > maxzL) {
					m_LandMarkHelp[47] = pt;   maxzL = pt.Z;
				}
			}

			if (pt.X < (FNeckPt.X - 6.0f))
			{
				if (pt.X > (FNeckPt.X - 17.0f) && pt.Z > maxzR) {
					m_LandMarkHelp[46] = pt;   maxzR = pt.Z;
				}
			}
		}

		pNode = pNode->m_pRight;
	}

	m_LandMarkHelp[46].Y = m_LandMarkHelp[47].Y = FNeckPt.Y; //높이가 높이간격에 맞게 해주었던 것을 원래 목앞점 높이로 바꿔
}

// Find Neck Front Pt (3)  jawInPt
// 참고점:  임시목옆점(35, 36),  정중시상면, m_maxZ
void CSizeKorea2020Doc::FindNeckFrontTemp1() {
	// Find Neck FrontTemp point
	// algorithm 0 :
	//얼굴앞점(upPt),  가슴앞점(downPt) 를 잇는 선분으로부터 먼 거리점을 찾는다

	GObNode<GVector3f> *pNode = m_SagittalPtsF.m_pLeftEnd;
	GVector3f FNeckTemp(0.0, 0.0, 10000.0);
	GVector3f FNeckPt(0.0, 0.0, 10000.0);

	float minDisY = 10000.0f;
	float minDisY1 = 10000.0f;
	GVector3f upPt, downPt;
	float upH = m_MaxY * 0.9f;
	float downH = m_MaxY * 0.65f;
	while (pNode)
	{
		GVector3f  pt = pNode->data;

		float disY = ABS(upH - pt.Y);
		if (disY < minDisY) {
			upPt = pt;
			minDisY = disY;
		}

		float disY1 = ABS(downH - pt.Y);
		if (disY1 < minDisY1) {
			downPt = pt;
			minDisY1 = disY1;
		}

		pNode = pNode->m_pRight;
	}

	GVector3f tmpPt = upPt;
	tmpPt.X += 20.0f;
	GPlane plnF(upPt, tmpPt, downPt);  //목앞오목점까지의 거리 구하기 위한 평면
	float maxDis = -10000.0f;

	pNode = m_SagittalPtsF.m_pLeftEnd;
	while (pNode)
	{
		GVector3f  pt = pNode->data;
		if (pt.Y > downPt.Y && pt.Y < upPt.Y)
		{
			float dis = plnF.GetSignedDistance(pt);

			if (dis > maxDis) {
				FNeckPt = pt;
				maxDis = dis;
			}
		}
		pNode = pNode->m_pRight;

	}

	m_LandMarkHelp[11] = FNeckPt; //temp FNeck
	m_LandMarkPose2[3] = FNeckPt;


	//Find Jaw point
	pNode = m_SagittalPtsF.m_pLeftEnd;
	int num = m_SagittalPtsF.m_nCount;

	float ylimitUp = FNeckPt.Y + 100.0f;
	GVector3f ray, jawPt;
	GVector3f Zray = GVector3f(0.0f, 0.0f, 1.0f);
	float minAngle = 3.0f;

	for (int i = 0; i < (num - 3); i++)
	{
		GVector3f pt = pNode->data;

		if (pt.Y < ylimitUp && pt.Y > FNeckPt.Y && pt.Z > (FNeckPt.Z - 20.0f))
		{
			ray = pt - FNeckPt;
			ray.X = 0.0f;
			float angle = Zray.getAngle(ray);
			if (angle < minAngle) {
				jawPt = pt;
				minAngle = angle;
			}
		}

		if (pNode)  pNode = pNode->m_pRight;
	}

	m_LandMarkHelp[34] = jawPt;
	jawPt.Z += 4.0f;

	// 임시목앞점, 턱끝점으로 이루어진 평면에서 가장 먼 점 구하기
	GPlane pln;
	GVector3f normal = GVector3f(0.0f, (jawPt.Z - FNeckPt.Z), -(jawPt.Y - FNeckPt.Y));
	pln.MakePlane(normal, jawPt);

	pNode = m_SagittalPtsF.m_pLeftEnd;
	maxDis = -1000.0f;

	for (int i = 0; i < (num - 3); i++)
	{
		GVector3f pt = pNode->data;

		if (pt.Y > FNeckPt.Y  && pt.Y<jawPt.Y && pt.Z > FNeckPt.Z)
		{
			float dis = ABS(pln.GetSignedDistance(pt));

			if (dis > maxDis) {
				maxDis = dis;
				m_LandMarkHelp[34] = pt;
			}
		}

		if (pNode)  pNode = pNode->m_pRight;
	}

	GVector3f  vf = m_LandMarkHelp[34] - FNeckPt;    //턱안점 - 임시목앞점 
	m_NeckFVector = vf;
}

//방패연골아래점, 목둘레평면 각도
//참고점: 목앞점(3), 턱안점(Help34), 목뒤점(6)
void CSizeKorea2020Doc::FindNeckThyroidPointV() {
	if (m_LandMarkPose2[3].Y == 0.0f || m_LandMarkPose2[6].Y == 0.0f || m_LandMarkHelp[34].Y == 0.0f)
		return;

	float  thyroidH = m_LandMarkPose2[3].Y*0.4f + m_LandMarkHelp[34].Y*0.6f;
	float   BNeckH = m_LandMarkPose2[6].Y + 12.0f;

	RefAngle[0] = -atan((thyroidH - BNeckH) / (m_LandMarkPose2[3].Z - m_LandMarkPose2[6].Z)); // ???

	GObNode<GVector3f> *pNode = m_SagittalPtsF.m_pLeftEnd;
	float minDis = 10000.0f;

	while (pNode)
	{
		GVector3f  pt = pNode->data;
		float dis = ABS(pt.Y - thyroidH);
		if (dis < minDis) {
			m_LandMarkPose2[2] = pt;
			minDis = dis;
		}
		pNode = pNode->m_pRight;
	}
}

// Find Neck side point (Right=4) (Left=5)
// 참고점: 머리정수리점[0], 겨드랑점[9,10], 정면실루엣, 턱안쪽점(Help34), 임시목옆점(Help35,36)
void CSizeKorea2020Doc::FindNeckSidePointV() {
	// 정수리점과 겨드랑점 높이 오른쪽점(최소 x좌표점)을 이은 직선과의 거리가 가장 먼점 Right Front silhouette로 hull을 만들고, 가장 긴 간격 직선에서 가장 먼점을 구한다. 

	float toShoulder = 80.0f + (0.1f*m_ArmObesity);
	int bottomH = ROUND(m_LandMarkPose2[9].Y + toShoulder);
	int bindex = bottomH / 2;

	float bottomY;

	//턱안점[34]과 임시목옆점[35] 중 더 높은 점은 아래 기준놋이로 한다-일반적으로 목옆점이 
	//더 아래이지만 비만인경우 턱안점이 더 아래인 경우도 있다
	if (m_LandMarkHelp[34].Y > m_LandMarkHelp[35].Y)
		bottomY = m_LandMarkHelp[34].Y;
	else bottomY = m_LandMarkHelp[35].Y;

	//목위거리의 비율을 정할때 
	// 목이 짧은 사람(턱안점이 임시목옆점에 가깝거나 더 아래인)은 아래로부터의 비율을 크게
	// 목이 긴 사람(턱안점이 임시목옆점보다 많이 높은)은 비율을 작게한다. 
	float ratio;
	float JawSnDis = m_LandMarkHelp[34].Y - m_LandMarkHelp[35].Y;
	if (JawSnDis < 5.0f)		ratio = 0.33;
	else if (JawSnDis < 18.0f)  ratio = 0.25;  //15.0
	else if (JawSnDis < 28.0f)  ratio = 0.2;
	else	ratio = 0.17;


	float topH = bottomY + (m_LandMarkPose2[1].Y - bottomY)*ratio;
	GVector3f topPt = GVector3f((m_LandMarkPose2[1].X), topH, m_LandMarkPose2[1].Z);
	topPt.X = m_LandMarkHelp[35].X*0.5 + m_LandMarkHelp[36].X*0.5;

	GVector3f bottomRPt = m_FRSilhouetteV[bindex];
	GVector3f bottomLPt = m_FLSilhouetteV[bindex];

	GPlane plnR, plnL;
	GVector3f normalR = GVector3f(1.0f, -(topPt.X - 40.0f - bottomRPt.X) / (topPt.Y - bottomRPt.Y), 0.0f); // ???
	GVector3f normalL = GVector3f(-1.0f, (topPt.X + 40.0f - bottomLPt.X) / (topPt.Y - bottomLPt.Y), 0.0f);
	plnR.MakePlane(normalR, bottomRPt);
	plnL.MakePlane(normalL, bottomLPt);

	//오른쪽
	int num = m_FRSilhouetteV.m_nCount;
	GObNode<GVector3f> *pNode1 = m_FRSilhouetteV.m_pLeftEnd;
	float a = (m_LandMarkPose2[10].X - m_LandMarkPose2[9].X)*0.17f;  //겨드랑점 사용
	float xlimitR = m_LandMarkPose2[9].X + a;	      //r-armpit
	float ylimitDown = m_LandMarkPose2[9].Y + 90.0f;
	float ylimitUp = m_LandMarkPose2[1].Y - 110.0f;   //top head
	float maxDis = -1000.0f;
	GVector3f prePt, prePt1, tempSNeckR, tempSNeckL;

	for (long i = 0; i < (num - 2); i++)
	{
		GVector3f pt = pNode1->data;

		if (pt.X > xlimitR && pt.Y > ylimitDown && pt.Y < ylimitUp)
		{
			float dis = plnR.GetSignedDistance(pt);
			if (dis > maxDis) {
				maxDis = dis;
				tempSNeckR = pt;
				prePt1 = prePt;
			}
		}
		prePt = pt;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	int sneckIndex = (ROUND(tempSNeckR.Y)) / 2;
	GVector3f upPt = m_FRSilhouetteV[sneckIndex + 1];
	GVector3f downPt = m_FRSilhouetteV[sneckIndex - 2];
	float slopUp = (upPt.Y - tempSNeckR.Y) / (upPt.X - tempSNeckR.X);  //???
	float slopDown = (downPt.Y - tempSNeckR.Y) / (downPt.X - tempSNeckR.X); //

	m_LandMarkPose2[4] = tempSNeckR;

	if (slopUp > 0.5f || slopUp <= 0.0f) {   //위쪽기울기가 0.9보다 크면 그대로...	
		if (slopDown < 0.8f)
			m_LandMarkPose2[4] = tempSNeckR;
		else {
			// m_LandMarkPose2[4] = downPt;
			m_LandMarkPose2[4] = prePt1;
			prePt1 = downPt;        //아래쪽기울기가 1보다 크면 아래점으로
		}
	}
	else {
		m_LandMarkPose2[4] = upPt;             //위쪽기울기가 0.8보다 작으면 위점으로
		prePt1 = tempSNeckR;
	}


	// 목앞쪽에 있는지 검사
	GPlane	pln1;
	pln1.MakePlane(GVector3f(0.0, 0.0, 1.0), m_LandMarkPose2[4]);
	GObList<GVector3f> CrossPtListRZ;
	float low_height = m_LandMarkPose2[4].Y - 10.0f;
	float upper_height = m_LandMarkPose2[4].Y;
	float limitX = m_LandMarkPose2[4].X + 2.0f;

	for (long l = 0L; l < m_iNum; l += 3)
	{
		GVector3f v0, v1, v2;
		v0 = m_Vertex[m_FaceIdx[l]];
		v1 = m_Vertex[m_FaceIdx[l + 1]];
		v2 = m_Vertex[m_FaceIdx[l + 2]];

		if (v0.Y < low_height   && v1.Y < low_height   && v2.Y < low_height)
			continue;
		if (v0.Y > upper_height && v1.Y > upper_height && v2.Y > upper_height)
			continue;
		if (v0.X > limitX && v1.X > limitX  && v2.X > limitX)
			continue;

		if (pln1.GetSignedDistance(v0) * pln1.GetSignedDistance(v1) <= 0)
		{
			GVector3f CrossPt = pln1.GetProjectionPt(v0, v1 - v0);
			CrossPtListRZ.Insert(CrossPt);
		}
		if (pln1.GetSignedDistance(v1) * pln1.GetSignedDistance(v2) <= 0)
		{
			GVector3f CrossPt = pln1.GetProjectionPt(v1, v2 - v1);
			CrossPtListRZ.Insert(CrossPt);
		}
		if (pln1.GetSignedDistance(v2) * pln1.GetSignedDistance(v0) <= 0)
		{
			GVector3f CrossPt = pln1.GetProjectionPt(v2, v0 - v2);
			CrossPtListRZ.Insert(CrossPt);
		}
	}

	::getSortedPoint(CrossPtListRZ, 1, 1);

	GVector3f bottomPt = CrossPtListRZ.m_pRightEnd->data;
	float slop = (m_LandMarkPose2[4].Y - bottomPt.Y) / (m_LandMarkPose2[4].X - bottomPt.X);
	if (slop > 1.8f || slop < 0.0f)
		m_LandMarkPose2[4] = prePt1;

	m_LandMarkHelp[48] = m_LandMarkPose2[4];

	//왼쪽
	num = m_FLSilhouetteV.m_nCount;
	pNode1 = m_FLSilhouetteV.m_pLeftEnd;
	xlimitR = m_LandMarkPose2[10].X - a;	   //l-armpit
	ylimitDown = m_LandMarkPose2[10].Y + 90.0f;
	ylimitUp = m_LandMarkPose2[1].Y - 120.0f;   //top head
	maxDis = -1000.0f;

	for (int i = 0; i < (num - 2); i++)
	{
		GVector3f pt = pNode1->data;

		if (pt.X < xlimitR && pt.Y > ylimitDown && pt.Y < ylimitUp)
		{
			float dis = plnL.GetSignedDistance(pt);
			if (dis > maxDis) {
				maxDis = dis;
				tempSNeckL = pt; //m_LandMarkPose2[5]
				prePt1 = prePt;
			}
		}
		prePt = pt;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}


	sneckIndex = (ROUND(tempSNeckL.Y)) / 2;
	upPt = m_FLSilhouetteV[sneckIndex + 1];
	downPt = m_FLSilhouetteV[sneckIndex - 2];
	slopUp = (upPt.Y - tempSNeckL.Y) / (upPt.X - tempSNeckL.X);
	slopDown = (downPt.Y - tempSNeckL.Y) / (downPt.X - tempSNeckL.X);//fabs((prePt1.Y-tempSNeckL.Y)/(prePt1.X-tempSNeckL.X));
	slopUp = fabs(slopUp);
	slopDown = fabs(slopDown);

	m_LandMarkPose2[5] = tempSNeckL;

	if (slopUp > 0.5f || slopUp <= 0.0f) {   //위쪽기울기가 0.9보다 크면 그대로...	
		if (slopDown < 0.8f)
			m_LandMarkPose2[5] = tempSNeckL;
		else {
			m_LandMarkPose2[5] = prePt1;
			//m_LandMarkPose2[5] = downPt; 
			prePt1 = downPt;        //아래쪽기울기가 1보다 크면 아래점으로
		}
	}
	else {
		m_LandMarkPose2[5] = upPt;             //위쪽기울기가 0.8보다 작으면 위점으로
		prePt1 = tempSNeckR;
	}

	/*if (slopUp<-0.5f || slopUp>=0.0f)  {   //위쪽기울기가 0.9보다 크면 그대로...
		//m_LandMarkPose2[4] = tempSNeckL;
		if (slopDown>-1.0f)
			m_LandMarkPose2[5] = tempSNeckL;
		else {
			m_LandMarkPose2[5] = prePt1;
			prePt1 = downPt;        //아래쪽기울기가 1보다 크면 아래점으로
		}
	}
	else {
		m_LandMarkPose2[5] = upPt;             //위쪽기울기가 0.8보다 작으면 위점으로
		prePt1 = tempSNeckL;
	}*/


	//	 if( (m_LandMarkPose2[5].Z -prePt1.Z) >50.0f) 
	//		 m_LandMarkPose2[5] = prePt1;

		 // 목앞쪽에 있는지 검사
	pln1.MakePlane(GVector3f(0.0, 0.0, 1.0), m_LandMarkPose2[5]);
	GObList<GVector3f> CrossPtListLZ; //CrossPtListL
	low_height = m_LandMarkPose2[5].Y - 8.0f;
	upper_height = m_LandMarkPose2[5].Y;
	limitX = m_LandMarkPose2[5].X - 2.0f;

	for (int l = 0L; l < m_iNum; l += 3)
	{
		GVector3f v0, v1, v2;
		v0 = m_Vertex[m_FaceIdx[l]];
		v1 = m_Vertex[m_FaceIdx[l + 1]];
		v2 = m_Vertex[m_FaceIdx[l + 2]];

		if (v0.Y < low_height   && v1.Y < low_height   && v2.Y < low_height)
			continue;
		if (v0.Y > upper_height && v1.Y > upper_height && v2.Y > upper_height)
			continue;
		if (v0.X < limitX && v1.X < limitX  && v2.X < limitX)
			continue;

		if (pln1.GetSignedDistance(v0) * pln1.GetSignedDistance(v1) <= 0)
		{
			GVector3f CrossPt = pln1.GetProjectionPt(v0, v1 - v0);
			CrossPtListLZ.Insert(CrossPt);
		}
		if (pln1.GetSignedDistance(v1) * pln1.GetSignedDistance(v2) <= 0)
		{
			GVector3f CrossPt = pln1.GetProjectionPt(v1, v2 - v1);
			CrossPtListLZ.Insert(CrossPt);
		}
		if (pln1.GetSignedDistance(v2) * pln1.GetSignedDistance(v0) <= 0)
		{
			GVector3f CrossPt = pln1.GetProjectionPt(v2, v0 - v2);
			CrossPtListLZ.Insert(CrossPt);
		}
	}

	::getSortedPoint(CrossPtListLZ, 1, 1);

	bottomPt = CrossPtListLZ.m_pRightEnd->data;
	slop = (m_LandMarkPose2[5].Y - bottomPt.Y) / (m_LandMarkPose2[5].X - bottomPt.X);
	if (slop<-1.5f || slop>0.0f)
		m_LandMarkPose2[5] = prePt1;

	m_LandMarkHelp[49] = m_LandMarkPose2[5];
}

//참고점
//목앞점(3), 목뒤점(6), 목옆점(4,5)
void CSizeKorea2020Doc::FindNeckSidePoint_Z() {
	//---------------Right
	double SneckRatioDepthR = (m_LandMarkPose2[3].Z - m_LandMarkPose2[4].Z) / (m_LandMarkPose2[3].Z - m_LandMarkPose2[6].Z);

	//newz = 실루엣z + 17.7221 + (목뒤기울기* -0.298) + (젖가슴둘레비만도*2.301) 
	if (m_bWoman) {
		if (SneckRatioDepthR < 0.486)
			m_LandMarkPose2[4].Z = m_LandMarkPose2[3].Z - 0.567 *(m_LandMarkPose2[3].Z - m_LandMarkPose2[6].Z); //0.547
		else if (SneckRatioDepthR > 0.727)
			m_LandMarkPose2[4].Z = m_LandMarkPose2[3].Z - 0.620 *(m_LandMarkPose2[3].Z - m_LandMarkPose2[6].Z); //0.667 
	}
	else {
		if (SneckRatioDepthR < 0.482)
			m_LandMarkPose2[4].Z = m_LandMarkPose2[3].Z - 0.545 *(m_LandMarkPose2[3].Z - m_LandMarkPose2[6].Z); //0.547
		else if (SneckRatioDepthR > 0.736)
			m_LandMarkPose2[4].Z = m_LandMarkPose2[3].Z - 0.673 *(m_LandMarkPose2[3].Z - m_LandMarkPose2[6].Z); //0.667 
	}

	//--------------- Left
	double SneckRatioDepthL = (m_LandMarkPose2[3].Z - m_LandMarkPose2[5].Z) / (m_LandMarkPose2[3].Z - m_LandMarkPose2[6].Z);

	//newz = 실루엣z + 17.7221 + (목뒤기울기* -0.298) + (젖가슴둘레비만도*2.301) 
	if (m_bWoman) {
		if (SneckRatioDepthL < 0.486)
			m_LandMarkPose2[5].Z = m_LandMarkPose2[3].Z - 0.547 *(m_LandMarkPose2[3].Z - m_LandMarkPose2[6].Z);
		else if (SneckRatioDepthL > 0.727)
			m_LandMarkPose2[5].Z = m_LandMarkPose2[3].Z - 0.620 *(m_LandMarkPose2[3].Z - m_LandMarkPose2[6].Z);
	}
	else {
		if (SneckRatioDepthR < 0.482)
			m_LandMarkPose2[4].Z = m_LandMarkPose2[3].Z - 0.545 *(m_LandMarkPose2[3].Z - m_LandMarkPose2[6].Z); //0.547
		else if (SneckRatioDepthR > 0.736)
			m_LandMarkPose2[4].Z = m_LandMarkPose2[3].Z - 0.673 *(m_LandMarkPose2[3].Z - m_LandMarkPose2[6].Z); //0.667 
	}
}

//참고점: 겨드랑점[9], 머리정수리점[1]
void CSizeKorea2020Doc::FindNeckSideTempV() {
	// 정수리점과 겨드랑점 높이 오른쪽점(최소 x좌표점)을 이은 직선과의 거리가 가장 먼점 Right Front silhouette로 hull을 만들고, 가장 긴 간격 직선에서 가장 먼점을 구한다. 

	   //임시겨드랑점으로하면 비만일때 너무 낮게 되므로 오류가 생긴다
	float toShoulder = 80.0f + (0.1f*m_ArmObesity);
	int bottomH = ROUND(m_LandMarkPose2[9].Y + toShoulder);
	int bindex = bottomH / 2;

	GVector3f topPt = GVector3f(m_LandMarkPose2[1].X, m_LandMarkPose2[1].Y - (m_Height*0.035f), m_LandMarkPose2[1].Z); //60-> (m_Height*0.3f)

	GVector3f bottomRPt = m_FRSilhouetteV[bindex];
	GVector3f bottomLPt = m_FLSilhouetteV[bindex];

	// 머리정수리점이 양쪽 겨드랑점보다 바깥이면 머리데이터가 이상한 것
	if (m_LandMarkPose2[1].X < m_LandMarkPose2[9].X || m_LandMarkPose2[1].X > m_LandMarkPose2[10].X)
		m_Error = ASERR_HEAD;


	GPlane plnR, plnL;
	GVector3f normalR = GVector3f(1.0f, -(topPt.X - 20.0f - bottomRPt.X) / (topPt.Y - bottomRPt.Y), 0.0f);
	GVector3f normalL = GVector3f(-1.0f, (topPt.X + 20.0f - bottomLPt.X) / (topPt.Y - bottomLPt.Y), 0.0f);
	plnR.MakePlane(normalR, bottomRPt);
	plnL.MakePlane(normalL, bottomLPt);

	//오른쪽
	int num = m_FRSilhouetteV.m_nCount;
	GObNode<GVector3f> *pNode1 = m_FRSilhouetteV.m_pLeftEnd;
	float a = (m_LandMarkPose2[10].X - m_LandMarkPose2[9].X)*0.17f;  //겨드랑점 사용
	float xlimit = m_LandMarkPose2[9].X + a;	      //r-armpit
	float ylimitDown = m_LandMarkPose2[9].Y + 80.0f;
	float ylimitUp = m_LandMarkPose2[1].Y - 100.0f;   //top head
	float maxDis = -1000.0f;
	GVector3f prePt, tempSNeckR, tempSNeckL;

	for (long i = 0; i < (num - 2); i++)
	{
		GVector3f pt = pNode1->data;

		if (pt.X > xlimit && pt.Y > ylimitDown && pt.Y < ylimitUp)
		{

			float dis = plnR.GetSignedDistance(pt);
			if (dis > maxDis) {
				maxDis = dis;
				m_LandMarkHelp[35] = pt; //prePt;
			}
		}
		prePt = pt;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	m_LandMarkPose2[4] = m_LandMarkHelp[35];

	num = m_FLSilhouetteV.m_nCount;
	pNode1 = m_FLSilhouetteV.m_pLeftEnd;
	xlimit = m_LandMarkPose2[10].X - a;	   //l-armpit
	ylimitDown = m_LandMarkPose2[10].Y + 80.0f;
	ylimitUp = m_LandMarkPose2[1].Y - 100.0f;   //top head
	maxDis = -1000.0f;

	for (int i = 0; i < (num - 2); i++)
	{
		GVector3f pt = pNode1->data;
		if (pt.X < xlimit && pt.Y > ylimitDown && pt.Y < ylimitUp)
		{
			float dis = plnL.GetSignedDistance(pt);
			if (dis > maxDis) {
				maxDis = dis;
				m_LandMarkHelp[36] = pt;
			}
		}

		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	m_LandMarkPose2[5] = m_LandMarkHelp[36];

}

///참고점: 겨드랑점[9], 머리정수리점[1]
void CSizeKorea2020Doc::FindNeckSideTempV1() {
	//목숙인 정도 파악: 임시 머리 뒤점- 등돌출점 잇는 선분 생성
	int upH = ROUND(m_MaxY - 70.0f);
	int upHindex = upH / 2;
	GVector3f upPt = m_SBSilhouetteV[upHindex];
	GVector3f upPtF = m_SFSilhouetteV1[upHindex];
	float headDepth = upPtF.Z - upPt.Z;

	//임시겨드랑점으로하면 비만일때 너무 낮게 되므로 오류가 생긴다
	float toShoulder = 80.0f + (0.1f*m_ArmObesity);
	int bottomH = ROUND(m_LandMarkPose2[9].Y + toShoulder);
	int bindex = bottomH / 2;
	GVector3f downPt = m_SBSilhouetteV[bindex];

	float slop;
	if (upPt.Z != downPt.Z)
		slop = (upPt.Y - downPt.Y) / (upPt.Z - downPt.Z);
	else
		slop = 0;

	GVector3f upMidPt = upPt;
	upMidPt.Z += headDepth / 2 + 30.0f;
	float b = upMidPt.Z - slop * upMidPt.Y;


	GVector3f topPt = GVector3f(m_LandMarkPose2[1].X, m_LandMarkPose2[1].Y - (m_Height*0.035f), m_LandMarkPose2[1].Z); //60-> (m_Height*0.3f)

	GVector3f bottomRPt = m_FRSilhouetteV[bindex];
	GVector3f bottomLPt = m_FLSilhouetteV[bindex];

	// 머리정수리점이 양쪽 겨드랑점보다 바깥이면 머리데이터가 이상한 것
	if (m_LandMarkPose2[1].X < m_LandMarkPose2[9].X || m_LandMarkPose2[1].X > m_LandMarkPose2[10].X)
		m_Error = ASERR_HEAD;

	GPlane plnR, plnL;
	GVector3f normalR = GVector3f(1.0f, -(topPt.X - 20.0f - bottomRPt.X) / (topPt.Y - bottomRPt.Y), 0.0f);
	GVector3f normalL = GVector3f(-1.0f, (topPt.X + 20.0f - bottomLPt.X) / (topPt.Y - bottomLPt.Y), 0.0f);
	plnR.MakePlane(normalR, bottomRPt);
	plnL.MakePlane(normalL, bottomLPt);

	//오른쪽
	int num = m_FRSilhouetteV.m_nCount;
	GObNode<GVector3f> *pNode1 = m_FRSilhouetteV.m_pLeftEnd;
	float a = (m_LandMarkPose2[10].X - m_LandMarkPose2[9].X)*0.17f;  //겨드랑점 사용
	float xlimit = m_LandMarkPose2[9].X + a;	      //r-armpit
	float ylimitDown = m_LandMarkPose2[9].Y + 80.0f;
	float ylimitUp = m_LandMarkPose2[1].Y - 100.0f;   //top head
	float maxDis = -1000.0f;
	GVector3f prePt, tempSNeckR, tempSNeckL;

	for (long i = 0; i < (num - 2); i++)
	{
		GVector3f pt = pNode1->data;

		if (pt.X > xlimit && pt.Y > ylimitDown && pt.Y < ylimitUp)
		{

			float dis = plnR.GetSignedDistance(pt);
			if (dis > maxDis) {
				int divideZ = slop * pt.Y + b;
				if (abs(pt.Z) < abs(divideZ)) {
					maxDis = dis;
					m_LandMarkHelp[35] = pt;
				}
			}
		}
		prePt = pt;
		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	m_LandMarkPose2[4] = m_LandMarkHelp[35];

	num = m_FLSilhouetteV.m_nCount;
	pNode1 = m_FLSilhouetteV.m_pLeftEnd;
	xlimit = m_LandMarkPose2[10].X - a;	   //l-armpit
	ylimitDown = m_LandMarkPose2[10].Y + 80.0f;
	ylimitUp = m_LandMarkPose2[1].Y - 100.0f;   //top head
	maxDis = -1000.0f;

	for (int i = 0; i < (num - 2); i++)
	{
		GVector3f pt = pNode1->data;
		if (pt.X < xlimit && pt.Y > ylimitDown && pt.Y < ylimitUp)
		{
			float dis = plnL.GetSignedDistance(pt);
			if (dis > maxDis) {
				int divideZ = slop * pt.Y + b;
				if (abs(pt.Z) < abs(divideZ)) {
					maxDis = dis;
					m_LandMarkHelp[36] = pt;
				}
			}
		}

		if (pNode1)  pNode1 = pNode1->m_pRight;
	}

	m_LandMarkPose2[5] = m_LandMarkHelp[36];
}

void CSizeKorea2020Doc::FindNeckSidePoint_Old() {
	long l = 0L;

	GPlane plane[2];
	plane[0].MakePlane(GVector3f(0.0, 1.05176, 1.0), m_LandMarkPose2[3]); //m_LandMarkPose2[3]=목앞점
	GObList<GVector3f> FrontNeckHole;

	for (l = 0L; l < m_iNum; l += 3)
	{
		GVector3f b0, b1, b2;
		b0 = m_Vertex[m_FaceIdx[l]];
		b1 = m_Vertex[m_FaceIdx[l + 1]];
		b2 = m_Vertex[m_FaceIdx[l + 2]];
		if (plane[0].IsAbovePlane(b0) && plane[0].IsAbovePlane(b1) && plane[0].IsAbovePlane(b2))
			continue;
		if (plane[0].IsBelowPlane(b0) && plane[0].IsBelowPlane(b1) && plane[0].IsBelowPlane(b2))
			continue;
		if (plane[0].GetSignedDistance(b0) * plane[0].GetSignedDistance(b1) < 0)
		{
			GVector3f CrossPt = plane[0].GetProjectionPt(b0, b1 - b0);
			if (CrossPt.Z < m_LandMarkPose2[3].Z)
			{
				FrontNeckHole.Insert(CrossPt);
			}
		}
		if (plane[0].GetSignedDistance(b1) * plane[0].GetSignedDistance(b2) < 0)
		{
			GVector3f CrossPt = plane[0].GetProjectionPt(b1, b2 - b1);
			if (CrossPt.Z < m_LandMarkPose2[3].Z)
			{
				FrontNeckHole.Insert(CrossPt);
			}
		}
		if (plane[0].GetSignedDistance(b2) * plane[0].GetSignedDistance(b0) < 0)
		{
			GVector3f CrossPt = plane[0].GetProjectionPt(b0, b2 - b0);
			if (CrossPt.Z < m_LandMarkPose2[3].Z)
			{
				FrontNeckHole.Insert(CrossPt);
			}
		}
	}

	GVector3f lneckPt(10000.0, 0.0, 0.0);
	GVector3f rneckPt(-10000.0, 0.0, 0.0);
	GObNode<GVector3f>* pNode1 = FrontNeckHole.m_pLeftEnd;
	while (pNode1)
	{
		GVector3f data = pNode1->data;
		if (data.X < lneckPt.X)
			lneckPt = data;
		if (data.X > rneckPt.X)
			rneckPt = data;
		pNode1 = pNode1->m_pRight;
	}
	m_LandMarkPose2[5] = lneckPt;
	m_LandMarkPose2[4] = rneckPt;
}

void CSizeKorea2020Doc::FindNeckCircRight() {
	GObNode<GVector3f> *pNode = m_ResultPathPose2[22].m_pLeftEnd;
	float minx = 1000.0f;

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (pt.X < minx) {
			m_LandMarkHelp[51] = pt;
			minx = pt.X;
		}

		pNode = pNode->m_pRight;
	}
}

//Find Neck Back point
//참고점: 목옆점 좌우/ 목앞,옆 등점 / 목뒤각도(m_NeckBackAngle)/ 정중시상면-뒤
void CSizeKorea2020Doc::FindNeckBackPoint() {
	if (m_LandMarkHelp[37].Y == 0.0f || m_LandMarkHelp[38].Y == 0.0f)
		return;

	GVector3f fneckBackP = m_LandMarkHelp[37];
	GVector3f sneckBackP = m_LandMarkHelp[38];

	float up = m_Height * 0.88;
	float down = m_LandMarkPose2[4].Y;  //m_Height * 0.81; // side neck

	if (up < (down + 2.0f)) up = down + 20.f;

	GVector3f sneckR = m_LandMarkPose2[4];
	sneckR.X = fneckBackP.X;

	float angleBetween = 90.0f + (m_NeckBackAngle*0.446f); //90.931f+(m_NeckBackAngle*0.444f);
	angleBetween = (3.14159*angleBetween) / 180.0f;

	float minAngle = 1000.0f, angle, diffAngle;
	GObNode<GVector3f> *pNode = m_SagittalPtsB.m_pLeftEnd;
	while (pNode)
	{
		GVector3f  pt = pNode->data;
		if (pt.Y < up && pt.Y >= down && pt.Z < m_LandMarkPose2[4].Z)
		{
			GVector3f  vs = pt - sneckR;

			angle = vs.getAngle(m_NeckBVector);

			//78.9=  80=1.39626f 90=1.5708 95=1.65806 98=1.710421 100=1.74532  105=1.83259 108=1.8675 110=1.91986
			diffAngle = fabs(angle - angleBetween);
			if (diffAngle < minAngle)// && angle>0.5f)
			{
				minAngle = diffAngle;
				m_LandMarkPose2[6] = pt;
			}
		}
		pNode = pNode->m_pRight;

	}
}

// 찾는점: 목앞등점, 목옆등점, 목옆등점위  / 목뒤기울기 벡터
// 참고점: 목앞점[3], 목옆점[4], 정중시상면-뒤
void CSizeKorea2020Doc::FindNeckBackSideFront() {
	if (m_LandMarkPose2[3].X == 0.0f || m_LandMarkPose2[4].X == 0.0f)
		return;

	GVector3f sneckR = m_LandMarkPose2[4];
	GVector3f fneck = m_LandMarkPose2[3];

	GVector3f sneckBackP, sneckBackupP, fneckBackP;

	GObNode<GVector3f> *pNode = m_SagittalPtsB.m_pLeftEnd;
	float minDisS = 10000.0f;
	float minDisSup = 10000.0f;
	float minDisF = 10000.0f;
	float fsmidy = (sneckR.Y*0.5f) + (fneck.Y*0.5f);

	while (pNode)
	{
		GVector3f  pt = pNode->data;
		float disS = ABS(pt.Y - sneckR.Y - 6); //-8
		float disSup = ABS(pt.Y - sneckR.Y - 28);
		float disF = ABS(pt.Y - fsmidy);  //fneck.Y-4
		if (disS < minDisS) {
			sneckBackP = pt;
			minDisS = disS;
		}
		if (disSup < minDisSup) {
			sneckBackupP = pt;
			minDisSup = disSup;
		}
		if (disF < minDisF) {
			fneckBackP = pt;
			minDisF = disF;
		}
		pNode = pNode->m_pRight;

	}

	//목뒤선 위점 -기울기로 찾기 ------------------- 2009.1.20추가

	pNode = m_SagittalPtsB.m_pLeftEnd;
	GVector3f  pt = pNode->data;
	while (pNode)
	{
		GVector3f  ptUp = pNode->data;

		if (ptUp.Y > (sneckR.Y + 5.0f) && (ptUp.Z - pt.Z) < -0.5f)
		{
			//float slop = (ptUp.Z-pt.Z)/(ptUp.Z-pt.Z);
			sneckBackupP = pt;
			break;
		}

		pt = ptUp;

		pNode = pNode->m_pRight;
		if (pNode)
			pNode = pNode->m_pRight;

	}
	//------------------------

	m_LandMarkHelp[37] = fneckBackP;
	m_LandMarkHelp[38] = sneckBackP;
	m_LandMarkHelp[39] = sneckBackupP;

	m_NeckBVector = sneckBackupP - sneckBackP;  //목옆위등점 -  목옆등점 

	GVector3f  hray = GVector3f(0.0f, 0.0f, 1.0f);
	GVector3f  vback = sneckBackupP - fneckBackP;    //목옆위등점 -  목앞등점 

	GVector3f a = vback * 0.5f;
	GVector3f b = m_NeckFVector * 0.5f;

	m_NeckFBVector = a + b;
}

void CSizeKorea2020Doc::FindHipPointV() {
	int upH = ROUND(m_Height * 0.53);
	int downH = ROUND(m_Height * 0.44);
	int upH1 = ROUND(m_Height * 0.211);
	int downH1 = ROUND(m_Height * 0.151);
	float minz = 10000.0f, minz1 = 10000.0f;

	GObNode<GVector3f> *pNode = m_SBSilhouetteV.m_pLeftEnd;
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (pt.Y<upH && pt.Y>downH)
		{
			if (pt.Z < minz) {
				m_LandMarkPose2[24] = pt; //hip
				minz = pt.Z;
			}
		}

		if (pt.Y<upH1 && pt.Y>downH1)
		{
			if (pt.Z < minz1) {
				m_LandMarkPose2[28] = pt; //Calf
				minz1 = pt.Z;
			}
		}
		pNode = pNode->m_pRight;
	}

	// 임시샅점 높이에서 임시샅점 과 오른쪽 허벅지 최측면점 구해서 그 사이에 볼기고랑점의 x좌표 설정
	double tan1 = tan(M_PI / 2.0 - RefAngle[3]);

	GPlane plane0;
	plane0.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkHelp[1]);  //right

	GVector3f tmp0(1000.0f, 0.0f, 0.0f);

	int tmpCrotchH = ROUND(m_LandMarkHelp[0].Y);
	if (tmpCrotchH % 2) tmpCrotchH++;

	pNode = m_pAlignedPointsV_Y[tmpCrotchH].m_pLeftEnd;
	if (pNode == NULL)
		pNode = m_pAlignedPointsV_Y[tmpCrotchH - 2].m_pLeftEnd;

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (plane0.IsBelowPlane(pt)) //right
		{
			if (pt.X < tmp0.X)
				tmp0 = pt;
		}

		pNode = pNode->m_pRight;
	}

	float glutealX = m_LandMarkHelp[0].X*0.7f + tmp0.X*0.3f;

	if (tmp0.X == 0.0f || glutealX > (m_LandMarkHelp[0].X + 200.0f))
		glutealX = m_LandMarkHelp[0].X + 60.0f;


	// 볼기고랑 종단면 만들기

	float upLimit = m_LandMarkPose2[24].Y - 40.0f;    //엉덩이돌출
	float downLimit = m_LandMarkPose2[24].Y - 200.0f;  //볼기고랑점높이
	float zLimit = m_LandMarkPose2[24].Z + 90.0f;
	GObList<GVector3f> CrossPtList;
	GPlane pln;
	GVector3f tmpPt = GVector3f(glutealX, 0.0f, 0.0f);
	pln.MakePlane(GVector3f(1.0, 0.0, 0.0), tmpPt);

	for (int i = 0; i < m_iNum; i += 3) //엉덩이돌출점 x 위치의 종단면 생성
	{
		GVector3f b0, b1, b2;
		b0 = m_Vertex[m_FaceIdx[i]];
		b1 = m_Vertex[m_FaceIdx[i + 1]];
		b2 = m_Vertex[m_FaceIdx[i + 2]];
		if (b0.Y > upLimit)
			continue;
		if (b0.Y < downLimit)
			continue;
		if (b0.Z > zLimit)
			continue;

		if (pln.IsAbovePlane(b0) && pln.IsAbovePlane(b1) && pln.IsAbovePlane(b2))
			continue;
		if (pln.IsBelowPlane(b0) && pln.IsBelowPlane(b1) && pln.IsBelowPlane(b2))
			continue;

		if (pln.GetSignedDistance(b0) * pln.GetSignedDistance(b1) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b1 - b0);
			CrossPtList.Insert(CrossPt);
		}
		if (pln.GetSignedDistance(b1) * pln.GetSignedDistance(b2) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b1, b2 - b1);
			CrossPtList.Insert(CrossPt);
		}
		if (pln.GetSignedDistance(b2) * pln.GetSignedDistance(b0) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b2 - b0);
			CrossPtList.Insert(CrossPt);
		}
	}

	::getSortedPoint(CrossPtList, 1, 1);

	pNode = CrossPtList.m_pLeftEnd;

	GVector3f upp = pNode->data;
	pNode = CrossPtList.m_pRightEnd;
	GVector3f downp = pNode->data;
	float slop;
	GVector3f norm;
	if (upp.Z == downp.Z)  norm = GVector3f(0.0f, 0.0f, 1.0f);
	else {
		slop = (upp.Y - downp.Y) / (upp.Z - downp.Z);
		norm = GVector3f(0.0f, 1.0f, -slop);
	}

	pln.MakePlane(norm, downp);
	float maxDis = -10.0f;
	pNode = pNode = CrossPtList.m_pLeftEnd;
	while (pNode)
	{
		GVector3f pt = pNode->data;

		float dis = pln.GetSignedDistance(pt);
		if (dis > maxDis) {
			m_LandMarkPose2[26] = pt; //gluteal fold
			maxDis = dis;
		}

		pNode = pNode->m_pRight;
	}
}

void CSizeKorea2020Doc::FindBustUnderPoint() {
	GVector3f bust = m_LandMarkPose2[16];
	int start = ROUND(bust.X - m_MinX) - 2;
	int end = start + 4;
	float zlimit = bust.Z - 100.0f;  //앞뒤나누는 기준
	float underH = bust.Y - 86.0f;   //직선만들 아래점 높이

	//  가슴돌출점 x 위치의 종단면 앞에서 앞직선만들 아래점 찾기
	float minDisY = 1000.0f;
	GVector3f downPt;

	for (int i = start; i < end; i++)
	{
		GObNode<GVector3f> *pNode = m_pAlignedPointsV_X[i].m_pLeftEnd;

		while (pNode) {
			GVector3f pt = pNode->data;

			if (pt.Z > zlimit) {
				float disY = ABS(underH - pt.Y);

				if (disY < minDisY) {
					downPt = pt;
					minDisY = disY;
				}
			}
			pNode = pNode->m_pRight;
		}
	}

	GVector3f normal = GVector3f(0.0f, -(bust.Z + 2 - downPt.Z), (bust.Y - 4 - downPt.Y));

	GPlane pln;
	pln.MakePlane(normal, downPt);
	float up = bust.Y;

	float maxDis = -10000.0f;
	for (int i = start; i < end; i++)
	{
		GObNode<GVector3f> *pNode = m_pAlignedPointsV_X[i].m_pLeftEnd;

		while (pNode)  //아래에서 위로 올라가는 중..
		{
			GVector3f pt = pNode->data;
			if (pt.Y<up && pt.Y>underH  && pt.Z > zlimit)
			{
				float dis = ABS(pln.GetSignedDistance(pt));

				if (dis > maxDis) {
					maxDis = dis;
					m_LandMarkPose2[18] = pt;
				}
			}
			pNode = pNode->m_pRight;
		}
	}
}

//가슴돌출점 위치의 시상면에서 찾는 방법
void CSizeKorea2020Doc::FindBustUnderPointV() {
	GVector3f bust = m_LandMarkPose2[16];
	int start = ROUND(bust.X - m_MinX) - 2;
	int end = start + 4;
	float zlimit = bust.Z - 100.0f;  //앞뒤나누는 기준
	float underH = bust.Y - 86.0f;   //직선만들 아래점 높이


	//  가슴돌출점 x 위치의 종단면 앞에서 앞직선만들 아래점 찾기
	float minDisY = 1000.0f;
	GVector3f downPt;

	for (int i = start; i < end; i++)
	{
		GObNode<GVector3f> *pNode = m_pAlignedPointsV_X[i].m_pLeftEnd;

		while (pNode) {
			GVector3f pt = pNode->data;

			if (pt.Z > zlimit) {
				float disY = ABS(underH - pt.Y);

				if (disY < minDisY) {
					downPt = pt;
					minDisY = disY;
				}
			}
			pNode = pNode->m_pRight;
		}
	}

	GVector3f normal = GVector3f(0.0f, -(bust.Z + 2 - downPt.Z), (bust.Y - 4 - downPt.Y));

	GPlane pln;
	pln.MakePlane(normal, downPt);
	float up = bust.Y;

	float maxDis = -10000.0f;
	for (int i = start; i < end; i++)
	{
		GObNode<GVector3f> *pNode = m_pAlignedPointsV_X[i].m_pLeftEnd;

		while (pNode)  //아래에서 위로 올라가는 중..
		{
			GVector3f pt = pNode->data;
			if (pt.Y<up && pt.Y>underH  && pt.Z > zlimit)
			{
				float dis = ABS(pln.GetSignedDistance(pt));

				if (dis > maxDis) {
					maxDis = dis;
					m_LandMarkPose2[18] = pt;
				}
			}
			pNode = pNode->m_pRight;
		}
	}
}

//  정중시상면(가운데)에서 젖가슴아래점 찾기,  젖가슴아래높이 단면에서 최전면점 찾기
//  참고점: 정중시상면
void CSizeKorea2020Doc::FindBustUnderPointC() {
	GVector3f bust = m_LandMarkPose2[16];
	GVector3f Ubust = m_LandMarkPose2[18];

	float underH = Ubust.Y - 40.0f;  //직선만들 아래점 높이

	//  가슴돌출점 x 위치의 종단면 앞에서 앞직선만들 아래점 찾기
	float minDisYdown = 1000.0f;
	float minDisYUB = 1000.0f;
	GVector3f upPt, downPt, UBFrontPt;

	GObNode<GVector3f> *pNode = m_SagittalPtsF.m_pLeftEnd;

	while (pNode) {

		GVector3f pt = pNode->data;

		float disYdown = ABS(underH - pt.Y);

		if (disYdown < minDisYdown) {
			downPt = pt;
			minDisYdown = disYdown;
		}

		float disYUB = ABS(Ubust.Y - pt.Y);

		if (disYUB < minDisYUB) {
			UBFrontPt = pt;
			minDisYUB = disYUB;
		}

		pNode = pNode->m_pRight;
	}

	m_LandMarkHelp[53] = UBFrontPt;

	upPt = bust;
	upPt.X = downPt.X;

	GVector3f normal = GVector3f(0.0f, -(upPt.Z - downPt.Z), (upPt.Y - downPt.Y));

	GPlane pln;
	pln.MakePlane(normal, downPt);
	float up = bust.Y;

	float maxDis = -10000.0f;

	pNode = m_SagittalPtsF.m_pLeftEnd;

	while (pNode)  //아래에서 위로 올라가는 중..
	{
		GVector3f pt = pNode->data;

		if (pt.Y<up && pt.Y>underH)
		{
			float dis = ABS(pln.GetSignedDistance(pt));

			if (dis > maxDis) {
				maxDis = dis;
				m_LandMarkHelp[52] = pt;
			}
		}

		pNode = pNode->m_pRight;
	}
}

// 젖꼭지점  위에서 아래로 내려가면서 기울기가 +가 되는 지점
// 참고점: 임시목앞점, 겨드랑점
void CSizeKorea2020Doc::FindBustPoint_SlopV() {
	// 남.여에 따라 탐색범위 다르게...
	float downratio = 0.6f;

	float up = m_Height * 0.76f;  //위쪽 범위는 남, 여 똑같다.
	float down = m_Height * downratio;
	//float maxz = -10000.0f;
	float preSlop = -1.0f;
	float slop = -1.0f;

	GObNode<GVector3f> *pNode1 = m_SFSilhouetteV.m_pRightEnd;
	//int count=0;
	GVector3f tempBP, prePt, ptC;

	while (pNode1)  //위에서 아래로 내려가는 중..
	{
		GVector3f pt0 = pNode1->data;

		if (pt0.Y<up && pt0.Y>down)
		{
			pNode1 = pNode1->m_pLeft;
			ptC = pNode1->data;
			pNode1 = pNode1->m_pLeft;  //실루엣은 높이 2m간격인데.. 4mm 간격으로..
			GVector3f pt1 = pNode1->data;

			if (pt0.Z - pt1.Z != 0)
				slop = (pt0.Y - pt1.Y) / (pt0.Z - pt1.Z);
			else
				slop = 0.0f;

			if (slop >= 0.0f && preSlop >= 0.0f) {
				tempBP = prePt;
				break;
			}
		}

		preSlop = slop;
		prePt = ptC;
		pNode1 = pNode1->m_pLeft;
	}

	m_LandMarkHelp[50] = tempBP;

	// BP높이의 너비 구하기
	GPlane lplane, rplane;
	double tan1 = tan(3.14159265 / 2.0 - RefAngle[5]);
	double tan2 = tan(3.14159265 / 2.0 + RefAngle[6]);
	rplane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkPose2[44]);
	lplane.MakePlane(GVector3f(1.0f, tan2, 0.0f), m_LandMarkPose2[46]);

	int BPH = ROUND(tempBP.Y);
	if (BPH % 2) BPH--;
	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[BPH].m_pLeftEnd;
	GVector3f minxp = GVector3f(10000.0f, 0.0f, 0.0f);
	GVector3f maxxp = GVector3f(-10000.0f, 0.0f, 0.0f);
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X > maxxp.X)	 maxxp = pt;
			if (pt.X < minxp.X)	 minxp = pt;
		}
		pNode = pNode->m_pRight;
	}
	float BW = maxxp.X - minxp.X;

	//  젖꼭지점 좌우 위치 조정하기
	float RBX, LBX;
	if (m_bWoman) {
		RBX = minxp.X + BW * 0.24f;  //242  rBP position
		LBX = minxp.X + BW * 0.76f;  //758  lBP position
	}
	else {
		RBX = minxp.X + BW * 0.193f;
		LBX = minxp.X + BW * 0.807f;
	}

	GVector3f BPBack = m_SBSilhouetteV[BPH / 2];
	pNode = m_pAlignedPointsV_Y[BPH].m_pLeftEnd;
	float minDisR = 10000.0f;
	float minDisL = 10000.0f;
	float CZ = (tempBP.Z + BPBack.Z)*0.5f;

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.Z > CZ) {
				float disR = ABS(pt.X - RBX);
				float disL = ABS(pt.X - LBX);
				if (disR < minDisR) {
					m_LandMarkPose2[16] = pt;
					minDisR = disR;
				}
				if (disL < minDisL) {
					m_LandMarkPose2[17] = pt;
					minDisL = disL;
				}
			}
		}
		pNode = pNode->m_pRight;
	}

	m_LandMarkPose2[16].X = RBX;
	m_LandMarkPose2[17].X = LBX;
}

// 참고점: 샅점[25], 목뒤점[6]
void CSizeKorea2020Doc::FindBustPoint_Slop_mV() {
	float downratio = 0.6f;

	float up = m_Height * 0.76f;  //위쪽 범위는 남, 여 똑같다.
	float down = m_Height * downratio;

	float rcx = m_LandMarkPose2[9].X*0.807f + m_LandMarkPose2[10].X*0.193f;
	//float zlimit = bust.Z-100.0f;  //앞뒤나누는 기준
	//float underH = bust.Y-100.0f;  //직선만들 아래점 높이


	//  가슴돌출점 x 위치의 종단면 앞에서 앞직선만들 아래점 찾기
	GVector3f normal = GVector3f(1.0f, 0.0f, 0.0f);
	GVector3f rcp = GVector3f(rcx, 0.0f, 0.0f);
	float cz = m_LandMarkPose2[9].Z;

	GPlane pln;
	pln.MakePlane(normal, rcp);

	GObList<GVector3f> CrossPtList;

	for (long i = 0L; i < m_iNum; i += 3)
	{
		GVector3f b0, b1, b2;
		b0 = m_Vertex[m_FaceIdx[i]];
		b1 = m_Vertex[m_FaceIdx[i + 1]];
		b2 = m_Vertex[m_FaceIdx[i + 2]];

		if (b0.Y > up)
			continue;
		if (b0.Y < down)
			continue;
		if (b0.Z < cz)
			continue;

		if (pln.IsAbovePlane(b0) && pln.IsAbovePlane(b1) && pln.IsAbovePlane(b2))
			continue;
		if (pln.IsBelowPlane(b0) && pln.IsBelowPlane(b1) && pln.IsBelowPlane(b2))
			continue;

		if (pln.GetSignedDistance(b0) * pln.GetSignedDistance(b1) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b1 - b0);
			CrossPtList.Insert(CrossPt);
		}
		if (pln.GetSignedDistance(b1) * pln.GetSignedDistance(b2) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b1, b2 - b1);
			CrossPtList.Insert(CrossPt);
		}
		if (pln.GetSignedDistance(b2) * pln.GetSignedDistance(b0) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b2 - b0);
			CrossPtList.Insert(CrossPt);
		}
	}

	//sorting by y - from big to small
	::getSortedPoint(CrossPtList, 1, 1);

	GObNode<GVector3f> *pNode1 = CrossPtList.m_pLeftEnd;

	GVector3f tempBP;
	GVector3f pt1;

	while (pNode1)  //위에서 아래로 내려가는 중..
	{
		GVector3f pt0 = pNode1->data;

		if (pt0.Y<up && pt0.Y>down)
		{
			if (pNode1) pNode1 = pNode1->m_pRight;
			if (pNode1) pNode1 = pNode1->m_pRight;
			if (pNode1) pNode1 = pNode1->m_pRight;
			if (pNode1) pNode1 = pNode1->m_pRight;
			// if(m_HInterval ==2)
			//	 pNode1 = pNode1->m_pLeft;  //실루엣은 높이 2m간격인데.. 4mm 간격으로..
			pt1 = pNode1->data;

			float slop;

			if (pt0.Z - pt1.Z != 0)
				slop = (pt0.Y - pt1.Y) / (pt0.Z - pt1.Z);
			else
				slop = 0.0f;

			if (slop >= 0) {	//&& count==0
				tempBP = pt0;
				break;
			}

			if (pNode1) pNode1 = pNode1->m_pLeft;
		}
		if (pNode1) pNode1 = pNode1->m_pRight;
	}

	// 오른쪽 BP높이의 너비 구하기
	GPlane lplane, rplane;
	double tan1 = tan(3.14159265 / 2.0 - RefAngle[5]);
	double tan2 = tan(3.14159265 / 2.0 + RefAngle[6]);
	rplane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkHelp[1]);
	lplane.MakePlane(GVector3f(1.0f, tan2, 0.0f), m_LandMarkHelp[2]);

	int BPH = ROUND(tempBP.Y);
	BPH = BPH - BPH % m_HInterval;
	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[BPH].m_pLeftEnd;
	GVector3f minxp = GVector3f(10000.0f, 0.0f, 0.0f);
	GVector3f maxxp = GVector3f(-10000.0f, 0.0f, 0.0f);
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X > maxxp.X)	 maxxp = pt;
			if (pt.X < minxp.X)	 minxp = pt;
		}
		pNode = pNode->m_pRight;
	}

	float BW = maxxp.X - minxp.X;

	//  젖꼭지점 좌우 위치 조정하기
	float RBX, LBX;
	if (m_bWoman) {
		RBX = minxp.X + BW * 0.2425f;  //rBP position
		LBX = minxp.X + BW * 0.7575f;  //lBP position
	}
	else {
		RBX = minxp.X + BW * 0.193f;
		LBX = minxp.X + BW * 0.807f;
	}

	GVector3f BPBack = m_SBSilhouetteV[BPH / m_HInterval];
	pNode = m_pAlignedPointsV_Y[BPH].m_pLeftEnd;
	float minDisR = 10000.0f;
	float minDisL = 10000.0f;
	float CZ = (tempBP.Z + BPBack.Z)*0.5f;
	while (pNode)
	{
		GVector3f pt = pNode->data;
		//GVector3f pt = m_Vertex[data];
		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.Z > CZ) {
				float disR = ABS(pt.X - RBX);
				float disL = ABS(pt.X - LBX);
				if (disR < minDisR) {
					m_LandMarkPose2[16] = pt;
					minDisR = disR;
				}
				if (disL < minDisL) {
					m_LandMarkPose2[17] = pt;
					minDisL = disL;
				}
			}
		}
		pNode = pNode->m_pRight;
	}
}

// 참고점: 샅점[25], 목뒤점[6]
void CSizeKorea2020Doc::FindBustPoint_Ratio_mV() {
	// 남자 가슴둘출점 평균 비율높이 0.678

	// 오른쪽 BP높이의 너비 구하기
	GPlane lplane, rplane;
	double tan1 = tan(3.14159265 / 2.0 - RefAngle[5]);
	double tan2 = tan(3.14159265 / 2.0 + RefAngle[6]);
	rplane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkHelp[1]);
	lplane.MakePlane(GVector3f(1.0f, tan2, 0.0f), m_LandMarkHelp[2]);

	int BPH = ROUND((m_LandMarkPose2[25].Y*0.322) + (m_LandMarkPose2[6].Y*0.678));
	BPH = BPH - BPH % m_HInterval;
	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[BPH].m_pLeftEnd;
	GVector3f minxp = GVector3f(10000.0f, 0.0f, 0.0f);
	GVector3f maxxp = GVector3f(-10000.0f, 0.0f, 0.0f);
	while (pNode)
	{
		GVector3f pt = pNode->data;
		//GVector3f pt = m_Vertex[data];
		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X > maxxp.X)	 maxxp = pt;
			if (pt.X < minxp.X)	 minxp = pt;
		}
		pNode = pNode->m_pRight;
	}

	float BW = maxxp.X - minxp.X;

	//  젖꼭지점 좌우 위치 조정하기
	float RBX, LBX;
	if (m_bWoman) {
		RBX = minxp.X + BW * 0.2425f;  //rBP position
		LBX = minxp.X + BW * 0.7575f;  //lBP position
	}
	else {
		RBX = minxp.X + BW * 0.193f;
		LBX = minxp.X + BW * 0.807f;
	}

	GVector3f BPBack = m_SBSilhouetteV[BPH / m_HInterval];
	pNode = m_pAlignedPointsV_Y[BPH].m_pLeftEnd;
	float minDisR = 10000.0f;
	float minDisL = 10000.0f;
	float CZ = (minxp.Z + maxxp.Z)*0.5f;
	while (pNode)
	{
		GVector3f pt = pNode->data;
		//GVector3f pt = m_Vertex[data];
		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.Z > CZ) {
				float disR = ABS(pt.X - RBX);
				float disL = ABS(pt.X - LBX);
				if (disR < minDisR) {
					m_LandMarkPose2[16] = pt;
					minDisR = disR;
				}
				if (disL < minDisL) {
					m_LandMarkPose2[17] = pt;
					minDisL = disL;
				}
			}
		}

		pNode = pNode->m_pRight;
	}
}

// 참고점: 젖가슴아래점(18), 샅점(25)
// 허리유형을 알아내서 유형에 따라 다른 방법 적용
// 허리앞점, 허리뒤점 다 찾기
void CSizeKorea2020Doc::FindWaistRightPt_wV() {
	GPlane lplane, rplane;
	float tan1 = (float)tan(M_PI / 2.0 - RefAngle[3]);
	float tan2 = (float)tan(M_PI / 2.0 + RefAngle[4]);

	rplane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkHelp[42]);
	lplane.MakePlane(GVector3f(1.0f, tan2, 0.0f), m_LandMarkHelp[43]);

	// 몸통정면에서 가장 오목한점 찾기 : 젖가슴아래점 높이 ~ 젖가슴아래점과 샅의 중간높이

	int upH = ROUND(m_LandMarkPose2[18].Y - 20.0f);
	if (m_LandMarkPose2[18].Y == 0.0f)  upH = ROUND(m_Height*0.691);
	int downH = ROUND(m_LandMarkPose2[25].Y);
	int middleH = ROUND((m_LandMarkPose2[25].Y*0.5f) + (m_LandMarkPose2[18].Y*0.5f));
	upH = upH - (upH % 2);
	downH = downH - (downH % 2);
	middleH = middleH - (middleH % 2);
	GVector3f RUBp, LUBp, RHp, LHp;
	GVector3f RWp = GVector3f(-1000.0f, 0.0f, 0.0f);
	GVector3f LWp = GVector3f(1000.0f, 0.0f, 0.0f);

	GVector3f maxxp = GVector3f(-1000.0f, 0.0f, 0.0f);
	GVector3f minxp = GVector3f(1000.0f, 0.0f, 0.0f);

	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[upH].m_pLeftEnd;
	//	if(m_pAlignedPoints_Y[upH].m_nCount<100) pNode = m_pAlignedPoints_Y[upH+m_HInterval].m_pLeftEnd;
	//	젖가슴아래높이에서 몸통좌우점 찾기
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X < minxp.X)	minxp = pt;
			if (pt.X > maxxp.X)	maxxp = pt;
		}
		pNode = pNode->m_pRight;
	}
	RUBp = minxp;
	LUBp = maxxp;
	//	젖가슴아래-샅 중간 높이에서 몸통좌우점 찾기
	pNode = m_pAlignedPointsV_Y[middleH].m_pLeftEnd;
	maxxp = GVector3f(-1000.0f, 0.0f, 0.0f);
	minxp = GVector3f(1000.0f, 0.0f, 0.0f);
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X < minxp.X)	minxp = pt;
			if (pt.X > maxxp.X)	maxxp = pt;
		}
		pNode = pNode->m_pRight;
	}
	RHp = minxp;
	LHp = maxxp;

	// 좌우 평면에서 가장 먼 수직거리로 오목점 찾기
	GPlane Rpln, Lpln;
	GVector3f RNorm, LNorm;
	float Rslop, Lslop;
	if (RHp.X == RUBp.X) RNorm = GVector3f(1.0f, 0.0f, 0.0f);
	else {
		Rslop = (RHp.Y - RUBp.Y) / (RHp.X - RUBp.X);
		RNorm = GVector3f(Rslop, -1.0f, 0.0f);
	}
	Rpln.MakePlane(RNorm, RUBp);

	if (LHp.X == LUBp.X)  LNorm = GVector3f(-1.0f, 0.0f, 0.0f);
	else {
		Lslop = (LHp.Y - LUBp.Y) / (LHp.X - LUBp.X);
		LNorm = GVector3f(Lslop, -1.0f, 0.0f);
	}
	Lpln.MakePlane(LNorm, LUBp);

	float maxdisR = -1000.0f;
	float maxdisL = -1000.0f;
	float disR, disL;

	// 몸통정면 실루엣 2mm 높이마다 만들기 : 젖가슴아래점 높이 ~ 샅높이
	GObList<GVector3f> RTorsoPtList;
	GObList<GVector3f> LTorsoPtList;
	for (int i = downH; i < upH; i += 2)
	{
		GVector3f rPt, lPt;
		GVector3f maxxp = GVector3f(-1000.0f, 0.0f, 0.0f);
		GVector3f minxp = GVector3f(1000.0f, 0.0f, 0.0f);

		GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[i].m_pLeftEnd;

		while (pNode)
		{
			GVector3f pt = pNode->data;

			if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
			{
				if (pt.X < minxp.X)
					minxp = pt;

				if (pt.X > maxxp.X)
					maxxp = pt;
			}
			pNode = pNode->m_pRight;
		}

		RTorsoPtList.Insert(minxp);
		LTorsoPtList.Insert(maxxp);

		if (i > middleH)	//좌우 직선에서 거리가 먼 점
		{
			disR = Rpln.GetSignedDistance(minxp); //ABS( Rpln.GetSignedDistance(minxp) ); //ABS
			disL = Lpln.GetSignedDistance(maxxp); //ABS( Lpln.GetSignedDistance(maxxp) ); //ABS

			if (maxdisR < disR) {
				maxdisR = disR;		RWp = minxp;
			}
			if (maxdisL < disL) {
				maxdisL = disL;     LWp = maxxp;
			}
		}
	}

	m_LandMarkHelp[18] = RWp;
	m_LandMarkHelp[19] = LWp;
	int RwaistType;

	// Right
	GVector3f RvectorUp = RWp - RUBp;      RvectorUp.Z = 0;
	GVector3f RvectorDown = RWp - RHp;      RvectorDown.Z = 0;
	GVector3f RvectorAll = RUBp - RHp;     RvectorAll.Z = 0;
	GVector3f RrefVector = GVector3f(1.0f, 0.0f, 0.0f);
	double RAngleUp = RvectorUp.getAngle(RrefVector);
	double RAngleDown = RvectorDown.getAngle(RrefVector);
	double RAngleSide = RvectorAll.getAngle(RrefVector);
	double RAngleConcave = RAngleUp + RAngleDown;

	//float disY=RUBp.Y-RWp.Y;
	//xx=0, HA=4,
	//HX=0, HH=2, AX=1,  AH=3, AA=4;
	// 샅높이-목뒤점 거리에 대한 샅높이-허리높이의 비율을 구해서 평균 +- 2시그맘범위를 벗어나면 
	// h체형으로 판단하여 통계치적용한다
	float TorsoDisY = (m_LandMarkPose2[6].Y - m_LandMarkPose2[25].Y);
	float Rwratio = (RWp.Y - m_LandMarkPose2[25].Y) / TorsoDisY;
	float limitup, limitdown;
	if (m_bWoman) {
		limitup = 0.468f;  //0.480f        455;
		limitdown = 0.377f;  //0.376  (385)  402;
	}
	else {
		limitup = 0.480f;
		limitdown = 0.362;
	}

	double Angle90 = M_PI * 0.5;  //0.517= 93/180

	if (RAngleSide > 1.383536) {	//H  85=1.383536/ 87=1.5184352 90=1.570795   0.0873 /2도=0.349065
		if (Rwratio > limitup || Rwratio < limitdown)  //가장 오목한 점 높이가  너무 위나 아래일때 H type으로 규정
			RwaistType = 2;					//HH
		else {
			if (RAngleUp < Angle90)
				RwaistType = 0;				//XH --
			else {
				if (RAngleConcave < (M_PI - 0.034))
					RwaistType = 2;			//HX 
				else  RwaistType = 3;		//HH
			}
		}
	}
	else {								//A
		if (Rwratio > limitup || Rwratio < limitdown) //if( disY<60.0f || disY>150.0f)
			RwaistType = 4;
		else {
			if (RAngleUp < Angle90)
				RwaistType = 1;				//XA--
			else {
				if (RAngleConcave < (M_PI - 0.034))
					RwaistType = 4;			//HA
				else  RwaistType = 5;		//AA
			}
		}
	}

	// 허리유형에따라 앞허리높이, 점 정하기

	// Left
	GVector3f LvectorUp = LWp - LUBp;   LvectorUp.Z = 0;
	GVector3f LvectorDown = LWp - LHp;   LvectorDown.Z = 0;
	GVector3f LvectorAll = LUBp - LHp;     LvectorAll.Z = 0;
	GVector3f LrefVector = GVector3f(-1.0f, 0.0f, 0.0f);
	double LAngleUp = LvectorUp.getAngle(LrefVector);
	double LAngleDown = LvectorDown.getAngle(LrefVector);
	double LAngleSide = LvectorAll.getAngle(LrefVector);
	double LAngleConcave = LAngleUp + LAngleDown;

	//	disY=LUBp.Y-LWp.Y; 

		//wratio = (m_LandMarkPose2[18].Y-RWp.Y)/TorsoDisY;
	float Lwratio = (LWp.Y - m_LandMarkPose2[25].Y) / TorsoDisY;
	int LwaistType;

	if (LAngleSide > 1.383536) {	//85=1.383536/ 87=1.5184352 0.0873 /2도=0.349065
		if (Lwratio > limitup || Lwratio < limitdown)  //가장 오목한 점 높이가 기준이하거나 이상이면 그냥 H type
			LwaistType = 2;					//HH
		else {
			if (LAngleUp < Angle90)
				LwaistType = 0;				//HX --
			else {
				if (LAngleConcave < (M_PI - 0.034))
					LwaistType = 2;			//HX --
				else  LwaistType = 3;		//HH
			}
		}
	}
	else {								   //A
		if (Lwratio<limitup || Lwratio>limitdown)
			LwaistType = 4;
		else {
			if (LAngleUp < Angle90)
				LwaistType = 1;				//XA--
			else {
				if (LAngleConcave < (M_PI - 0.034))
					LwaistType = 4;			//HA
				else  LwaistType = 5;		//AA
			}
		}
	}

	//  허리유형이 x이면 그곳이 허리점
	//  아니면 통계치 적용
	int wh, WaistType;
	if (RwaistType <= 1 || LwaistType <= 1) {
		WaistType = 0;
		if (RwaistType <= 1 && LwaistType > 1)
			wh = ROUND(RWp.Y - m_LandMarkPose2[25].Y);
		if (RwaistType > 1 && LwaistType <= 1)
			wh = ROUND(LWp.Y - m_LandMarkPose2[25].Y);
		if (RwaistType <= 1 && LwaistType <= 1) {
			wh = ROUND((LWp.Y*0.5) + (RWp.Y*0.5) - m_LandMarkPose2[25].Y + 2.0f);
		}
	}
	else {  //목뒤-샅거리기준
		WaistType = 2;
		if (m_bWoman) wh = TorsoDisY * 0.436;  //woman
		else			wh = TorsoDisY * 0.421;  //man 
	}

	m_ResultPose2[73] = WaistType;                                     // 허리유형


	// 허리옆점 R 넣기   
	// 이 높이는 샅점높이에서부터의 높이-지금쓰고 있는 데이바는 샅높이가 0으로 되고 있으므로
	int whIndex = wh / 2;

	// 만약 계산 허리높이가 겨드랑높이보다 높게 되는 경우에는 겨드랑높이보다 낮춰준다.
	if (whIndex > RTorsoPtList.m_nCount)
		whIndex = RTorsoPtList.m_nCount - 4;

	m_LandMarkPose2[19] = RTorsoPtList[whIndex]; //RWp; 
	m_LandMarkPose2[20] = LTorsoPtList[whIndex]; //LWp; 


	 // 허리옆점-L 찾기
	int whr = ROUND(m_LandMarkPose2[19].Y);
	whr = whr - (whr % 2);
	pNode = m_pAlignedPointsV_Y[whr].m_pLeftEnd;
	float maxX = -10000.0f;

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X > maxX) {
				m_LandMarkPose2[20] = pt;		 //허리옆점-L
				maxX = pt.X;
			}
		}

		pNode = pNode->m_pRight;
	}
}

// 참고점: 목뒤점(6), 샅점(25)
void CSizeKorea2020Doc::FindWaistRightPt_mV() {
	GPlane lplane, rplane;
	double tan1 = tan(M_PI / 2.0 - RefAngle[3]);
	double tan2 = tan(M_PI / 2.0 + RefAngle[4]);

	rplane.MakePlane(GVector3f(-1.0f, (float)tan1, 0.0f), m_LandMarkHelp[42]);
	lplane.MakePlane(GVector3f(1.0f, (float)tan2, 0.0f), m_LandMarkHelp[43]);

	// 몸통정면에서 가장 오목한점 찾기 : 젖가슴아래점 높이 ~ 젖가슴아래점과 샅의 중간높이
	int upH = (m_LandMarkPose2[25].Y*0.1f) + (m_LandMarkPose2[9].Y*0.9f);
	int downH = ROUND(m_LandMarkPose2[25].Y);
	int middleH = ROUND((m_LandMarkPose2[25].Y*0.8f) + (m_LandMarkPose2[6].Y*0.2f));
	upH = upH - (upH%m_HInterval);
	downH = downH - (downH%m_HInterval);
	middleH = middleH - (middleH%m_HInterval);
	GVector3f RUBp, LUBp, RHp, LHp;
	GVector3f RWp = GVector3f(-1000.0f, 0.0f, 0.0f);
	GVector3f LWp = GVector3f(1000.0f, 0.0f, 0.0f);

	GVector3f maxxp = GVector3f(-1000.0f, 0.0f, 0.0f);
	GVector3f minxp = GVector3f(1000.0f, 0.0f, 0.0f);

	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[upH].m_pLeftEnd;
	if (m_pAlignedPointsV_Y[upH].m_nCount < 100) pNode = m_pAlignedPointsV_Y[upH + m_HInterval].m_pLeftEnd;
	while (pNode)
	{
		GVector3f pt = pNode->data;
		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X < minxp.X)	minxp = pt;
			if (pt.X > maxxp.X)	maxxp = pt;
		}
		pNode = pNode->m_pRight;
	}
	RUBp = minxp;
	LUBp = maxxp;

	pNode = m_pAlignedPointsV_Y[middleH].m_pLeftEnd;
	maxxp = GVector3f(-1000.0f, 0.0f, 0.0f);
	minxp = GVector3f(1000.0f, 0.0f, 0.0f);
	while (pNode)
	{
		GVector3f pt = pNode->data;
		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X < minxp.X)	minxp = pt;
			if (pt.X > maxxp.X)	maxxp = pt;
		}
		pNode = pNode->m_pRight;
	}
	RHp = minxp;
	LHp = maxxp;

	// 좌우 평면에서 가장 먼 수직거리로 오목점 찾기
	GPlane Rpln, Lpln;
	GVector3f RNorm, LNorm;
	float Rslop = (RHp.Y - RUBp.Y) / (RHp.X - RUBp.X);
	RNorm = GVector3f(Rslop, -1.0f, 0.0f);
	Rpln.MakePlane(RNorm, RUBp);
	float Lslop = (LHp.Y - LUBp.Y) / (LHp.X - LUBp.X);
	LNorm = GVector3f(Lslop, -1.0f, 0.0f);
	Lpln.MakePlane(LNorm, LUBp);

	float maxdisR = -1000.0f;
	float maxdisL = -1000.0f;
	float disR, disL;

	GObList<GVector3f> RTorsoPtList;
	GObList<GVector3f> LTorsoPtList;
	for (int i = downH; i < upH; i += m_HInterval)
	{
		GVector3f rPt, lPt;
		GVector3f maxxp = GVector3f(-1000.0f, 0.0f, 0.0f);
		GVector3f minxp = GVector3f(1000.0f, 0.0f, 0.0f);

		GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[i].m_pLeftEnd;
		if (m_pAlignedPointsV_Y[i].m_nCount < 100) continue;

		while (pNode)
		{
			GVector3f pt = pNode->data;
			if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
			{
				if (pt.X < minxp.X)
					minxp = pt;

				if (pt.X > maxxp.X)
					maxxp = pt;
			}
			pNode = pNode->m_pRight;
		}

		RTorsoPtList.Insert(minxp);
		LTorsoPtList.Insert(maxxp);

		if (i > middleH)	//좌우 직선에서 거리가 먼 점
		{
			disR = ABS(Rpln.GetSignedDistance(minxp));
			disL = ABS(Lpln.GetSignedDistance(maxxp));

			if (maxdisR < disR) {
				maxdisR = disR;		RWp = minxp;
			}
			if (maxdisL < disL) {
				maxdisL = disL;     LWp = maxxp;
			}
		}
	}

	m_LandMarkHelp[18] = RWp;
	m_LandMarkHelp[19] = LWp;
	int RwaistType;

	// Right
	GVector3f RvectorUp = RWp - RUBp;      RvectorUp.Z = 0;
	GVector3f RvectorDown = RWp - RHp;      RvectorDown.Z = 0;
	GVector3f RvectorAll = RUBp - RHp;     RvectorAll.Z = 0;
	GVector3f RrefVector = GVector3f(1.0f, 0.0f, 0.0f);
	float RAngleUp = RvectorUp.getAngle(RrefVector);
	float RAngleDown = RvectorDown.getAngle(RrefVector);
	float RAngleSide = RvectorAll.getAngle(RrefVector);
	float RAngleConcave = RAngleUp + RAngleDown;

	float disY = RUBp.Y - RWp.Y;
	//xx=0, HA=4,
	//HX=0, HH=2, VX=1,  AH=3, AA=4;
	// 샅높이-목뒤점 거리에 대한 샅높이-허리높이의 비율을 구해서 평균 +- 2시그맘범위를 벗어나면 
	// h체형으로 판단하여 통계치적용한다
	float wratio = (RWp.Y - m_LandMarkPose2[25].Y) / (m_LandMarkPose2[6].Y - m_LandMarkPose2[25].Y);
	float limitup, limitdown;
	//man
	limitup = 0.450; //0.480f; 
	limitdown = 0.391; //0.362;

	if (RAngleSide < 1.65806) {	//85=1.383536/ 87=1.5184352 90=1.570795   0.0873 /2도=0.349065
		if (wratio > limitup || wratio < limitdown) //if( wratio<60.0f || disY>150.0f)  //가장 오목한 점 높이가 기준이하거나 이상이면 그냥 H type
			RwaistType = 2;					//HH
		else {
			if (RAngleUp < (M_PI*0.5f))
				RwaistType = 0;				//HX --
			else {
				if (RAngleConcave < (M_PI - 0.034))
					RwaistType = 0;			//HX --
				else  RwaistType = 2;		//HH
			}
		}
	}
	else {								//V
		if (wratio > limitup || wratio < limitdown) //if( disY<60.0f || disY>150.0f)
			RwaistType = 3;
		else {
			if (RAngleUp < (M_PI*0.5f))
				RwaistType = 1;				//XV--
			else {
				if (RAngleConcave < (M_PI - 0.034))
					RwaistType = 3;			//HV
				else  RwaistType = 4;		//AV
			}
		}
	}

	// 허리유형에따라 앞허리높이, 점 정하기
	// Left
	GVector3f LvectorUp = LWp - LUBp;   LvectorUp.Z = 0;
	GVector3f LvectorDown = LWp - LHp;   LvectorDown.Z = 0;
	GVector3f LvectorAll = LUBp - LHp;     LvectorAll.Z = 0;
	GVector3f LrefVector = GVector3f(-1.0f, 0.0f, 0.0f);
	float LAngleUp = LvectorUp.getAngle(LrefVector);
	float LAngleDown = LvectorDown.getAngle(LrefVector);
	float LAngleSide = LvectorAll.getAngle(LrefVector);
	float LAngleConcave = LAngleUp + LAngleDown;

	disY = LUBp.Y - LWp.Y;

	wratio = (LWp.Y - m_LandMarkPose2[25].Y) / (m_LandMarkPose2[6].Y - m_LandMarkPose2[25].Y);
	int LwaistType;

	if (LAngleSide < 1.65806) {	//85=1.383536/ 87=1.5184352 0.0873 /2도=0.349065
		if (wratio > limitup || wratio < limitdown)  //가장 오목한 점 높이가 기준이하거나 이상이면 그냥 H type
			LwaistType = 2;					//HH
		else {
			if (LAngleUp < (M_PI*0.5f))
				LwaistType = 0;				//HX --
			else {
				if (LAngleConcave < (M_PI - 0.034))
					LwaistType = 0;			//HX --
				else  LwaistType = 2;		//HH
			}
		}
	}
	else {								//V
		if (wratio > limitup || wratio < limitdown)
			LwaistType = 3;
		else {
			if (LAngleUp < (M_PI*0.5f))
				LwaistType = 1;				//XV--
			else {
				if (LAngleConcave < (M_PI - 0.034))
					LwaistType = 3;			//HV
				else  LwaistType = 4;		//AV
			}
		}
	}

	//  허리유형이 x이면 그곳이 허리점
	//  아니면 통계치 적용
	int wh, WaistType;
	if (RwaistType <= 1 || LwaistType <= 1) {
		WaistType = 0;
		if (RwaistType <= 1 && LwaistType > 1)
			wh = ROUND(RWp.Y - m_LandMarkPose2[25].Y);
		if (RwaistType > 1 && LwaistType <= 1)
			wh = ROUND(LWp.Y - m_LandMarkPose2[25].Y);
		if (RwaistType <= 1 && LwaistType <= 1) {
			wh = ROUND((LWp.Y*0.5) + (RWp.Y*0.5) - m_LandMarkPose2[25].Y);
		}
	}
	else {  //목뒤-샅거리기준  
		WaistType = 2;
		wh = (m_LandMarkPose2[6].Y - m_LandMarkPose2[25].Y)*0.420;  //man 
	}

	m_ResultPose2[73] = WaistType;                  // 허리유형

// 허리옆점 넣기   
	int whIndex = (wh) / m_HInterval;    //이 높이는 샅점높이에서부터의 높이-지금쓰고 있는 데이바는 샅높이가 0으로 되고 있으므로
	m_LandMarkPose2[19] = RTorsoPtList[whIndex]; //RWp; 
	m_LandMarkPose2[20] = LTorsoPtList[whIndex]; //LWp; 
}

//------ seek Waist Front, Back, Left Pt
void CSizeKorea2020Doc::FindWaistF_B_LV() {
	if (m_LandMarkPose2[9].Y == 0.0f || m_LandMarkPose2[10].Y == 0.0f || m_LandMarkPose2[19].Y == 0.0f)
		return;

	GPlane lplane, rplane;
	double tan1 = tan(M_PI / 2.0 - RefAngle[3]);
	double tan2 = tan(M_PI / 2.0 + RefAngle[4]);

	rplane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkHelp[42]);
	lplane.MakePlane(GVector3f(1.0f, tan2, 0.0f), m_LandMarkHelp[43]);


	//------ seek Waist Front, Back Pt	
	int wh = ROUND(m_LandMarkPose2[19].Y);
	wh = wh - (wh % 2);
	GObList<GVector3f> CrossPtList;   //허리둘레 점들
	GObNode<GVector3f> *pNode;

	if (m_bArrangedXY) {
		pNode = m_pAlignedPointsV_Y[wh].m_pLeftEnd;
	}
	else {
		GPlane pln;
		GVector3f tmpPt = GVector3f(0.0f, (float)wh, 0.0f);
		pln.MakePlane(GVector3f(0.0, 1.0, 0.0), tmpPt);
		float upLimit = (float)wh + 10.0f;
		float downLimit = (float)wh - 10.0f;

		for (int i = 0; i < m_iNum; i += 3)
		{
			GVector3f b0, b1, b2;
			b0 = m_Vertex[m_FaceIdx[i]];
			b1 = m_Vertex[m_FaceIdx[i + 1]];
			b2 = m_Vertex[m_FaceIdx[i + 2]];
			if (b0.Y > upLimit)
				continue;
			if (b0.Y < downLimit)
				continue;

			if (pln.IsAbovePlane(b0) && pln.IsAbovePlane(b1) && pln.IsAbovePlane(b2))
				continue;
			if (pln.IsBelowPlane(b0) && pln.IsBelowPlane(b1) && pln.IsBelowPlane(b2))
				continue;

			if (pln.GetSignedDistance(b0) * pln.GetSignedDistance(b1) < 0)
			{
				GVector3f CrossPt = pln.GetProjectionPt(b0, b1 - b0);
				CrossPtList.Insert(CrossPt);
			}
			if (pln.GetSignedDistance(b1) * pln.GetSignedDistance(b2) < 0)
			{
				GVector3f CrossPt = pln.GetProjectionPt(b1, b2 - b1);
				CrossPtList.Insert(CrossPt);
			}
			if (pln.GetSignedDistance(b2) * pln.GetSignedDistance(b0) < 0)
			{
				GVector3f CrossPt = pln.GetProjectionPt(b0, b2 - b0);
				CrossPtList.Insert(CrossPt);
			}
		}

		pNode = CrossPtList.m_pLeftEnd;
	}


	float minDisF = 10000.0f;
	float minDisB = 10000.0f;
	float maxX = -10000.0f;

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X > maxX) {
				m_LandMarkPose2[20] = pt;		 //허리옆점-L
				maxX = pt.X;
			}
		}

		pNode = pNode->m_pRight;
	}

	float waistCX = (m_LandMarkPose2[19].X + m_LandMarkPose2[20].X)*0.5f;
	float waistCZ = m_LandMarkPose2[19].Z;


	if (m_bArrangedXY) {
		pNode = m_pAlignedPointsV_Y[wh].m_pLeftEnd;
	}
	else {
		pNode = CrossPtList.m_pLeftEnd;
	}

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.Z > waistCZ) {
				float dis = ABS(pt.X - waistCX);
				if (dis < minDisF) {
					m_LandMarkPose2[21] = pt;		//허리앞점
					minDisF = dis;
				}
			}
			else {
				float dis = ABS(pt.X - waistCX);
				if (dis < minDisB) {
					m_LandMarkPose2[22] = pt;	  //허리뒤점
					minDisB = dis;
				}
			} //else
		} //if

		pNode = pNode->m_pRight;
	}//while
}

void CSizeKorea2020Doc::FindWaistBack_ConcavePtV() {
	int upW = ROUND(m_Height * 0.66);
	int downW = ROUND(m_Height * 0.54);
	float maxz = -10000.0f;

	GObNode<GVector3f> *pNode = m_SBSilhouetteV.m_pLeftEnd; //m_SagittalPtsB.m_pLeftEnd;

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (pt.Y<upW && pt.Y>downW)
		{
			if (pt.Z > maxz) {
				m_LandMarkHelp[7] = pt; //waist
				maxz = pt.Z;
			}
		}
		pNode = pNode->m_pRight;
	}

	m_ResultPose2[72] = m_LandMarkPose2[22].Y;  // 뒤허리높이
}

// 배돌출점[23]
// 참고점: 정중시상면, 임시겨드랑점[Help1,2], 샅점[25], 목뒤점[6], 젖가슴아래점[18]
void CSizeKorea2020Doc::FindAbdominalPtV() {
	CHECK(m_LandMarkHelp[1]);
	CHECK(m_LandMarkHelp[2]);

	GPlane lplane, rplane;
	double tan1 = tan(M_PI / 2.0 - RefAngle[3]);
	double tan2 = tan(M_PI / 2.0 + RefAngle[4]);
	rplane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkHelp[1]);
	lplane.MakePlane(GVector3f(1.0f, tan2, 0.0f), m_LandMarkHelp[2]);

	//  키기준 -  남.여에 따라 탐색범위 다르게...
	/*float downRatio, limitup;
	if (m_bWoman) { limitdown=0.49f;  limitup=0.61f; }
	else{	        limitdown=0.55f;  limitup=0.684f; }*/

	//  샅높이-목뒤점거리 기준 
	float limitup, limitdown;
	if (m_bWoman) {
		limitup = 0.30f;  //310	354
		limitdown = 0.222f; //222  178
	}
	else {
		limitup = 0.36f;   //355   0.399
		limitdown = 0.26f;  //266   0.221   244
	}

	float TorsoDisY = (m_LandMarkPose2[6].Y - m_LandMarkPose2[25].Y);


	float down = m_LandMarkPose2[25].Y + TorsoDisY * limitdown;
	float up = m_LandMarkPose2[25].Y + TorsoDisY * limitup;

	float maxz = -10000.0f;
	GObNode<GVector3f> *pNode = m_SFSilhouetteV1.m_pLeftEnd;

	while (pNode)
	{
		GVector3f pt = pNode->data;
		if (pt.Y > down  &&  pt.Y < up) {
			if (pt.Z > maxz)
			{
				maxz = pt.Z;
				m_LandMarkPose2[23] = pt;   //범위안에 배돌출점
			}
		}
		pNode = pNode->m_pRight;
	}

	pNode = m_SFSilhouetteV1.m_pLeftEnd;
	maxz = -10000.0f;
	while (pNode)
	{
		GVector3f pt = pNode->data;
		if (pt.Y > down  &&  pt.Y < m_LandMarkPose2[18].Y) {
			if (pt.Z > maxz)
			{
				maxz = pt.Z;
				m_LandMarkPose2[38] = pt;           //제일 배돌출점
			}
		}
		pNode = pNode->m_pRight;
	}

	GObList<GVector3f> CrossPtList;                 //배둘레 점들
	int abH = ROUND(m_LandMarkPose2[23].Y);
	if (abH % 2) abH++;
	GObNode<GVector3f> *pNode1 = m_pAlignedPointsV_Y[abH].m_pLeftEnd;
	if (m_pAlignedPointsV_Y[abH].m_nCount < 50) pNode1 = m_pAlignedPointsV_Y[abH + 2].m_pLeftEnd;

	GVector3f maxxp = GVector3f(-1000.0f, 0.0f, 0.0f);
	GVector3f minxp = GVector3f(1000.0f, 0.0f, 0.0f);
	while (pNode1)
	{
		GVector3f pt = pNode1->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			CrossPtList.Insert(pt);
			if (pt.X < minxp.X)	minxp = pt;
			if (pt.X > maxxp.X)	maxxp = pt;
		}
		pNode1 = pNode1->m_pRight;
	}

	minxp.Z = maxxp.Z;
	GVector3f abBackpt = m_SBSilhouetteV[abH / 2];
	abBackpt.X = m_LandMarkPose2[23].X;

	m_ResultPose2[10] = m_LandMarkPose2[23].Y;
	m_ResultPose2[74] = maxxp.X - minxp.X;
	m_ResultPose2[83] = m_LandMarkPose2[23].Z - abBackpt.Z;
	m_ResultPathPose2[74].Insert(maxxp);  //배너비경로
	m_ResultPathPose2[74].Insert(minxp);
	m_ResultPathPose2[83].Insert(m_LandMarkPose2[23]);  //배두께경로
	m_ResultPathPose2[83].Insert(abBackpt);


	CConvexHull cvx(CrossPtList);
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	m_ResultPose2[30] = cvx.GetHullLength();   //배둘레
	m_ResultPathPose2[30] = cvx.m_HullPtList;  //배둘레 경로
}

//무릎뼈가운데점
void CSizeKorea2020Doc::FindKneePtV() {
	//높이 범위 안에서 둘레가 최소인 위치
	int start = (int)m_Height * 0.23;
	int end = (int)m_Height * 0.29;
	if (start % 2) start++;

	int knee_height = 0;
	float min_length = 10000.0;

	for (int i = start; i < end; i += 2)
	{
		GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[i].m_pLeftEnd;
		GObList<GVector3f> temp;
		while (pNode)
		{
			GVector3f pt = pNode->data;
			if (pt.X < m_LandMarkPose2[25].X)
				temp.Insert(pt);
			pNode = pNode->m_pRight;
		}
		CConvexHull hull;
		hull.m_PtList = temp;
		hull.MakeConvexHullXZ();
		hull.ArrangeHullPtXZ();
		float length = hull.GetHullLength();
		if (length < min_length)
		{
			min_length = length;
			knee_height = i;
		}
	}
	//둘레가 최소인 위치보다 40mm 높은 위치
	int upDis;
	if (m_bWoman) upDis = ROUND(m_Height*0.007f) + 20;
	else upDis = ROUND(m_Height*0.008f) + 20;

	if (upDis % 2 != 0) upDis++;

	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[knee_height + upDis].m_pLeftEnd;
	GVector3f KneePt(0.0, 0.0, -10000.0);
	while (pNode)
	{
		GVector3f pt = pNode->data;
		if (pt.X < m_LandMarkHelp[0].X) {
			if (pt.Z > KneePt.Z)
				KneePt = pt;
		}
		pNode = pNode->m_pRight;
	}
	m_LandMarkPose2[27] = KneePt;
}

void CSizeKorea2020Doc::FindTopHeadPoint() {
	int i;
	GVector3f TopHeadPt(0.0, -100.0, 0.0);
	for (i = 0; i < m_vNum; i++)
	{
		if (m_Vertex[i].Y > TopHeadPt.Y)
			TopHeadPt = m_Vertex[i];
	}
	m_LandMarkPose2[1] = TopHeadPt;
}

// 겨드랑앞벽점, 뒤벽점 찾기 -  최전점, 최후점도 찾기
// 참고점: 임시목앞점 Help[11], 겨드랑점[9,10], 어깨가쪽점[7,8], 접힘점[42~45]
// 설정방법: 진동둘레를 먼저 만들고 진동룰레 선 상에서 설정
void CSizeKorea2020Doc::FindArmscyePt_1() {
	::GetResultPose2_25(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[25]);
	::GetResultPose2_88(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[88]);

	GVector3f LFArmscye, LBArmscye, RFArmscye, RBArmscye;

	// ------------------ Right -------------------
		//오른 진동둘레의 최전점 maxZ, 최후점 minZ 구하기
	GVector3f maxzPt, minzPt;
	maxzPt.Z = -10000.0f;   minzPt.Z = 10000.0f;
	GObNode<GVector3f> *pNode1 = m_ResultPathPose2[25].m_pLeftEnd; //오른쪽 진동둘레
	int num = m_ResultPathPose2[25].m_nCount;

	GVector3f pt;
	for (int i = 0; i < (num - 1); i++)
	{
		pt = pNode1->data;

		if (pt.Z > maxzPt.Z)  maxzPt = pt;
		if (pt.Z < minzPt.Z)  minzPt = pt;

		pNode1 = pNode1->m_pRight;

	}//for

	m_LandMarkHelp[54] = maxzPt;
	m_LandMarkHelp[55] = minzPt;

	float RCenterZ = maxzPt.Z*0.5f + minzPt.Z*0.5f;
	float RCenterY = (m_LandMarkPose2[9].Y*0.5f) + (m_LandMarkPose2[7].Y *0.5f) + 4.0f;


	GVector3f pt0 = m_ResultPathPose2[25][0]; // .m_pLeftEnd->data;   //진동둘레 시작점을 끝에 추가시켜서 closed가 되게 한다.
	GVector3f pt1 = m_ResultPathPose2[25][1];

	m_ResultPathPose2[25].Insert(pt0);
	m_ResultPathPose2[25].Insert(pt1);

	pNode1 = m_ResultPathPose2[25].m_pLeftEnd; //오른쪽 진동둘레
	num = m_ResultPathPose2[25].m_nCount;


	//  선분중 교차하는 선분의 교차점을 구하는 방법
	for (int i = 0; i < (num - 1); i++)
	{
		GVector3f pt, pt1;
		pt = pNode1->data;

		if (pNode1) {
			pNode1 = pNode1->m_pRight;
			pt1 = pNode1->data;

			if (pt.Z < RCenterZ)   //f
			{
				if ((pt.Y - RCenterY) * (pt1.Y - RCenterY) <= 0.0f) {
					RBArmscye = (pt + pt1) / 2.0f;
					RBArmscye.Y = RCenterY;
				}
			}
			else
			{
				if ((pt.Y - RCenterY) * (pt1.Y - RCenterY) <= 0.0f) {
					RFArmscye = (pt + pt1) / 2.0f;
					RFArmscye.Y = RCenterY;
				}
			}

		}//if

	}//for



   /*/겨드랑점 z좌표 조정  F:B = 0.531: 0.469
   m_LandMarkPose2[9].Z = m_LandMarkHelp[54].Z*0.469f + m_LandMarkHelp[55].Z*0.531f;

   //겨드랑아래 보조점 계산하기
   //float mf = -(maxzPt.Z - m_LandMarkPose2[10].Z)/(m_LandMarkHelp[54].Y - m_LandMarkPose2[10].Y);
   //float mb = -(minzPt.Z - m_LandMarkPose2[10].Z)/(m_LandMarkHelp[55].Y - m_LandMarkPose2[10].Y);
   GVector3f fcornerRdn = GVector3f( m_LandMarkPose2[9].X, m_LandMarkPose2[9].Y, m_LandMarkHelp[54].Z);
   GVector3f bcornerLdn = GVector3f( m_LandMarkPose2[9].X, m_LandMarkPose2[9].Y, m_LandMarkHelp[55].Z);
   GVector3f fcornerLup = GVector3f( m_LandMarkPose2[9].X, m_LandMarkHelp[54].Y, m_LandMarkPose2[9].Z);
   GVector3f bcornerRup = GVector3f( m_LandMarkPose2[9].X, m_LandMarkHelp[55].Y, m_LandMarkPose2[9].Z);

	GVector3f intersectPtF, intersectPtB, addptF, addptB;

   ::GetIntersectPointZY( maxzPt, m_LandMarkPose2[9], fcornerLup,  fcornerRdn, &intersectPtF);
   ::GetIntersectPointZY( minzPt, m_LandMarkPose2[9], bcornerRup,  bcornerLdn, &intersectPtB);

   intersectPtF.X = m_LandMarkHelp[54].X;
   intersectPtB.X = m_LandMarkHelp[55].X;

   addptF = GVector3f( (intersectPtF.X + fcornerRdn.X)*0.5f,
					   (intersectPtF.Y*0.4f + fcornerRdn.Y*0.6f),
					   (intersectPtF.Z*0.4f + fcornerRdn.Z*0.6f) );
   addptB = GVector3f( (intersectPtB.X + bcornerLdn.X)*0.5f,
					   (intersectPtB.Y*0.33f + bcornerLdn.Y*0.67f),
					   (intersectPtB.Z*0.33f + bcornerLdn.Z*0.67f) );

   m_LandMarkHelp[56]=addptF;
   m_LandMarkHelp[57]=addptB;*/


   //-------------- Left  -------------------
   //왼 진동둘레의 최전점 maxZ, 최후점 minZ 구하기
	maxzPt.Z = -10000.0f;   minzPt.Z = 10000.0f;
	pNode1 = m_ResultPathPose2[88].m_pLeftEnd; //왼쪽 진동둘레
	num = m_ResultPathPose2[88].m_nCount;

	for (int i = 0; i < (num - 1); i++)
	{
		pt = pNode1->data;

		if (pt.Z > maxzPt.Z)  maxzPt = pt;
		if (pt.Z < minzPt.Z)  minzPt = pt;

		pNode1 = pNode1->m_pRight;

	}

	float LCenterZ = maxzPt.Z*0.5f + minzPt.Z*0.5f; //m_LandMarkPose2[8].Z;
	float LCenterY = (m_LandMarkPose2[10].Y*0.5f) + (m_LandMarkPose2[8].Y *0.5f) + 4.0f;


	pt0 = m_ResultPathPose2[88][0]; // .m_pLeftEnd->data;   //진동둘레 시작점을 끝에 추가시켜서 closed가 되게 한다.
	pt1 = m_ResultPathPose2[88][1];

	m_ResultPathPose2[88].Insert(pt0);
	m_ResultPathPose2[88].Insert(pt1);

	pNode1 = m_ResultPathPose2[88].m_pLeftEnd; //왼쪽 진동둘레
	num = m_ResultPathPose2[88].m_nCount;

	//LFArmscye = pNode1->data;
	// 선분중 교차하는 선분의 교차점을 구하는 방법
	for (int i = 0; i < (num - 1); i++)
	{
		GVector3f pt, pt1;
		pt = pNode1->data;

		if (pNode1) {
			pNode1 = pNode1->m_pRight;
			pt1 = pNode1->data;

			if (pt.Z < LCenterZ)   //f
			{
				if ((pt.Y - LCenterY) * (pt1.Y - LCenterY) <= 0.0f) {
					LBArmscye = (pt + pt1) / 2.0f;
					LBArmscye.Y = LCenterY;
				}
			}
			else
			{
				if ((pt.Y - LCenterY) * (pt1.Y - LCenterY) <= 0.0f) {
					LFArmscye = (pt + pt1) / 2.0f;
					LFArmscye.Y = LCenterY;
				}
			}

		}//if

	}//for

	m_LandMarkPose2[12] = RFArmscye;
	m_LandMarkPose2[13] = LFArmscye;

	m_LandMarkPose2[14] = RBArmscye;
	m_LandMarkPose2[15] = LBArmscye;
}

// 팔꿈치점 설정 (31)
// 참고점: 겨드랑점(9), 손목안쪽점(32), 팔 나눔각도
void CSizeKorea2020Doc::FindElbowPtV() {
	GVector3f RArmPit = m_LandMarkPose2[9];
	GVector3f RWristOut = m_LandMarkPose2[32];
	GVector3f RElbowPt;
	GPlane rplane;
	double tan1 = tan(M_PI / 2.0 - RefAngle[3]);
	rplane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkHelp[1]);

	int elbowH = ROUND((RArmPit.Y*0.6f) + (RWristOut.Y*0.4f));
	elbowH = elbowH - (elbowH%m_HInterval);

	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[elbowH].m_pLeftEnd;
	GVector3f tmp(0.0, 0.0, 10000.0);
	GVector3f tmpZ(0.0, 0.0, 10000.0);
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (rplane.IsAbovePlane(pt))
		{
			if (pt.Z < tmpZ.Z)
				tmpZ = pt;

		}
		pNode = pNode->m_pRight;
	}

	m_LandMarkPose2[42] = tmpZ;


	pNode = m_pAlignedPointsV_Y[elbowH + 10].m_pLeftEnd;
	float limitX = tmpZ.X - 25.0f;

	if (pNode == NULL)
		pNode = m_pAlignedPointsV_Y[elbowH + 12].m_pLeftEnd;
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (rplane.IsAbovePlane(pt) && pt.X < limitX)
		{
			if (pt.Z < tmp.Z)
				tmp = pt;

		}
		pNode = pNode->m_pRight;
	}

	m_LandMarkPose2[31] = tmp;
}

// find Wrist right pt / WristIn(32), WristOut(33)
// 손바닥을 뒤로 한 자세에서의 손목점들..  안쪽-손목가쪽점/ 바깥쪽-손목안쪽점
// 참고점: 임시겨드랑점(r),  손끝점(r), 팔나눔각 
void CSizeKorea2020Doc::FindWristPt_InV() {
	if (m_LandMarkPose2[1].Y == 0.0f || m_LandMarkPose2[34].Y == 0.0f)
		return;

	GVector3f RArmPit = m_LandMarkHelp[1];  //임시 겨드랑점 사용
	GVector3f RArmEnd = m_LandMarkPose2[34];
	GVector3f RWristPt, HandConvexPt;      // HandPt-손끝에서 조금 올라간 손바닥높이 손안쪽점
	GPlane plane;
	int    start;
	double tan1 = tan(3.14159265 / 2.0 - RefAngle[3]);
	plane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), RArmPit); //팔분리평면
	float max_length = 0.0;

	int armMidH = ROUND((RArmEnd.Y*0.5f) + (RArmPit.Y*0.5f));   //손목가쪽점을 구하기 위한 직선위점
	if (armMidH % 2 != 0) armMidH++;
	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[armMidH].m_pLeftEnd;
	GVector3f armMidPt(-10000.0, 0.0, 0.0);      //겨드랑과 손끌점의 중간높이 팔안쪽점

	while (pNode)
	{
		GVector3f pt = pNode->data;
		if (plane.IsAbovePlane(pt))
		{
			if (pt.X > armMidPt.X)
				armMidPt = pt;
		}
		pNode = pNode->m_pRight;
	}


	GVector3f Vray = GVector3f(0.0f, -1.0f, 0.0f);
	GVector3f ray;
	float minAngle = 4.0f;
	int HandEndH = (int)RArmEnd.Y + 30.0f;
	if (HandEndH % 2) HandEndH++;

	int upLimitH = (int)(RArmEnd.Y*0.4f + armMidPt.Y*0.6f);

	for (int i = HandEndH; i < upLimitH; i += 2)
	{
		GVector3f pt = m_ArmRInSilhouetteV[i / 2];

		ray = pt - armMidPt;
		ray.Z = 0.0f;
		float angle = Vray.getAngle(ray);
		if (angle < minAngle) {
			HandConvexPt = pt;
			minAngle = angle;
		}

	}

	//m_LandMarkHelp[62] = HandConvexPt;   //손목가쪽점을 구하기 위한 직선아래점

	double tan2;
	GVector3f normal;
	GPlane plane2;

	if (HandConvexPt.X == armMidPt.X) normal = GVector3f(-1.0f, 0.0f, 0.0f);
	else {
		tan2 = (HandConvexPt.Y - armMidPt.Y) / (HandConvexPt.X - armMidPt.X);

		if (tan2 > 1000.0 || tan2 < -1000.0)
			normal = GVector3f(-1.0f, 0.0f, 0.0f);  // plane이 수직에 가까우면 normal을 아예  x축 방향으로 한다.
		else
			normal = GVector3f(-tan2, 1.0f, 0.0f);
	}

	plane2.MakePlane(normal, armMidPt); //오목점 거리기준 평면

	start = (int)HandConvexPt.Y;
	if (start % 2) start--;
	max_length = 0.0;
	GVector3f tmpWristpt, tmpWristpt1;


	for (int i = start; i < armMidH; i += 2)
	{

		GVector3f tmp = m_ArmRInSilhouetteV[i / 2];

		if (tmp.X == -10000.0) continue;
		if (i == start)
			tmpWristpt = tmp; //distance가 모두 - 부호일때(손데이터가 없는경우에 주로 이러함)는 그냥 시작높이 팔안쪽점점을 손목점으로 설정
		if (i == (start + 2))
			tmpWristpt1 = tmp;

		float length = plane2.GetSignedDistance(tmp);
		if (length > max_length)
		{
			max_length = length;
			RWristPt = tmp;
		}
	}

	if (max_length <= 0.0f)
		RWristPt = tmpWristpt;
	if (RWristPt.Y == 0.0f)   RWristPt = tmpWristpt1;

	m_LandMarkPose2[48] = RWristPt;  //손목가쪽점 - 3차원에서는 안쪽 - 앞쪽


	Calc_ArmSlopV();

	//--------- 손목안쪽점 구하기  (3차원 바깥에 - 뒤에 )
	// 팔각도 - 겨드랑점과 손목안쪽점으로
	//tan2 = (RWristPt.Y-RArmPit.Y)/(RWristPt.X-RArmPit.X);
	plane2;
	plane2.MakePlane(GVector3f(1.0f, m_ArmslopDown, 0.0f), RWristPt); //팔분리평면

	// 실루엣점에서 
	float down = ROUND(RWristPt.Y);
	float up = down + 80.0f;
	float min_dis = 1000.0f;

	GObNode<GVector3f> *pNode2 = m_FRSilhouetteV.m_pLeftEnd;
	while (pNode2)
	{
		GVector3f pt = pNode2->data;
		if (pt.Y > down && pt.Y < up) {
			float dis = ABS(plane2.GetSignedDistance(pt));
			if (dis < min_dis) {
				min_dis = dis;
				m_LandMarkPose2[32] = pt;   //손목안쪽점 - 3차원에서는 바깥점
			}
		}
		pNode2 = pNode2->m_pRight;
	}
}

//손바닥이 안쪽을 향하는 자세
// find Wrist right pt / WristIn(32), WristOut(33)
// 참고점: 임시겨드랑점(r),  손끝점(r), 팔나눔각 
void CSizeKorea2020Doc::FindWristPt_FV() {
	if (m_LandMarkPose2[1].Y == 0.0f || m_LandMarkPose2[34].Y == 0.0f)
		return;

	GVector3f RArmPit = m_LandMarkHelp[1];  //임시 겨드랑점 사용
	GVector3f RArmEnd = m_LandMarkPose2[34];
	GVector3f RWristPt, HandConvexPt;      // HandPt-손끝에서 조금 올라간 손바닥높이 손안쪽점
	GPlane plane;
	int    start;
	double tan1 = tan(3.14159265 / 2.0 - RefAngle[3]);
	plane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), RArmPit); //팔분리평면
	float max_length = 0.0;


	int armMidH = ROUND((RArmEnd.Y*0.5f) + (RArmPit.Y*0.5f));   //손목바깥점(앞쪽점)을 구하기 위한 직선위점
	if (armMidH % 2 != 0) armMidH++;
	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[armMidH].m_pLeftEnd;
	GVector3f armMidPt(0.0, 0.0, -1000.0);      //겨드랑과 손끌점의 중간높이 팔안쪽점

	while (pNode)
	{
		GVector3f pt = pNode->data;
		if (plane.IsAbovePlane(pt))
		{
			if (pt.Z > armMidPt.Z)
				armMidPt = pt;
		}
		pNode = pNode->m_pRight;
	}

	//손목바깥점을 구하기 위한 직선아래점
	GVector3f Vray = GVector3f(0.0f, -1.0f, 0.0f);
	GVector3f ray;
	float minAngle = -1.0f;
	int HandEndH = (int)RArmEnd.Y + 30.0f;
	if (HandEndH % 2) HandEndH++;

	int upLimitH = (int)(RArmEnd.Y*0.4f + armMidPt.Y*0.6f);

	for (int i = HandEndH; i < upLimitH; i += 2)
	{
		GVector3f pt = m_ArmRFSilhouetteV[i / 2];

		ray = pt - armMidPt;
		ray.X = 0.0f;
		float angle = Vray.getAngle(ray);
		if (angle > minAngle) {
			HandConvexPt = pt;
			minAngle = angle;
		}

	}

	m_LandMarkHelp[66] = HandConvexPt;

	double tan2;
	GVector3f normal;
	GPlane plane2;

	if (HandConvexPt.Z == armMidPt.Z) normal = GVector3f(0.0f, 0.0f, -1.0f);
	else {
		tan2 = (HandConvexPt.Y - armMidPt.Y) / (HandConvexPt.Z - armMidPt.Z);

		if (tan2 > 1000.0 || tan2 < -1000.0)
			normal = GVector3f(0.0f, 0.0f, -1.0f);  // plane이 수직에 가까우면 normal을 아예  x축 방향으로 한다.
		else
			normal = GVector3f(0.0f, -1.0f, tan2);
	}

	plane2.MakePlane(normal, armMidPt); //오목점 거리기준 평면

	start = (int)HandConvexPt.Y;
	if (start % 2) start--;
	max_length = 0.0;
	GVector3f tmpWristpt, tmpWristpt1;


	for (int i = start; i < armMidH; i += 2) {

		GVector3f tmp = m_ArmRFSilhouetteV[i / 2];

		if (tmp.Z == -10000.0) continue;
		if (i == start)
			tmpWristpt = tmp; //distance가 모두 - 부호일때(손데이터가 없는경우에 주로 이러함)는 그냥 시작높이 팔안쪽점점을 손목점으로 설정
		if (i == (start + 2))
			tmpWristpt1 = tmp;

		float length = plane2.GetSignedDistance(tmp);
		if (length > max_length)
		{
			max_length = length;
			RWristPt = tmp;
		}
	}

	if (max_length <= 0.0f)
		RWristPt = tmpWristpt;
	if (RWristPt.Y == 0.0f)   
		RWristPt = tmpWristpt1;

	m_LandMarkPose2[33] = RWristPt;  //손목가쪽점 - 3차원에서는 안쪽

	Calc_ArmSlopV();

	//--------- 손목안쪽점 구하기  (3차원 바깥에 - 뒤에)
	// 팔각도 - 겨드랑점과 손목안쪽점으로
	//tan2 = (RWristPt.Y-RArmPit.Y)/(RWristPt.X-RArmPit.X);
//	plane2;
//	plane2.MakePlane( GVector3f( 0.0f, 1.0, 0.0f ), RWristPt ); //팔분리평면

	// 실루엣점에서 
	int wristH = ROUND(RWristPt.Y);
	float minZ = 1000.0f;

	if (wristH % 2 != 0) wristH++;
	GObNode<GVector3f> *pNode2 = m_pAlignedPointsV_Y[wristH - 16].m_pLeftEnd;

	while (pNode2)
	{
		GVector3f pt = pNode2->data;
		if (plane.IsAbovePlane(pt)) {

			if (pt.Z < minZ) {
				minZ = pt.Z;
				m_LandMarkPose2[32] = pt;   //손목안쪽점 - 3차원에서는 뒤점
			}
		}
		pNode2 = pNode2->m_pRight;
	}
}

// 가쪽복사점 설정
//참고점: 장딴지앞점
void CSizeKorea2020Doc::FindAnklePtV() {
	// 만약 발이 스캔되지 않은 경우(발끝점과 장딴지 앞점 두께가 50)에는 맨 아래바깥점을 가쪽복사점으로 하고 나간다
	if ((m_LandMarkPose2[41].Z - m_LandMarkPose2[29].Z) < 50.0f) {
		m_LandMarkPose2[30] = m_FRSilhouetteV[5];
		m_LandMarkPose2[30].Z = m_LandMarkPose2[29].Z - 30.0f;
		m_LandMarkPose2[30].X += 5.0f;
		return;
	}

	int base_height = 0;
	GVector3f v0(10000.0, 0.0, 0.0);
	GVector3f v1(10000.0, 0.0, 0.0);

	int  down, up;  // -> 32mm, 88mm
	down = ROUND(0.015*m_Height) / m_HInterval;
	up = ROUND(m_LandMarkPose2[29].Y - 16) / m_HInterval;

	v0 = m_FRSilhouetteV[down];    // 4/2
	v1 = m_FRSilhouetteV[up];		// 150/2
	// 높이 4, 150 각각에서 가장 오른쪽 점을 찾는다.

	v0.Z = v1.Z = 0.0; // projection to xy-plane
	float m = (v1.Y - v0.Y) / (v1.X - v0.X);

	GObNode<GVector3f> *pNode = m_FRSilhouetteV.m_pLeftEnd;
	float max_dis = -10000.0f;

	//실루엣 점들 중 처음 만든 직선에서 가장 먼점 a을 구한다
	while (pNode)
	{
		GVector3f p = pNode->data;

		if (p.Y > v0.Y && p.Y < v1.Y  && p.Z < (m_LandMarkPose2[29].Z - 20.0f)) {

			float dis = fabs(m * p.X - p.Y - m * v0.X + v0.Y) / sqrt(m * m + 1);
			if (dis > max_dis)
			{
				max_dis = dis;
				base_height = ROUND(p.Y);
			}
		}
		pNode = pNode->m_pRight;
	}
	GVector3f AnklePt(10000.0, 0.0, 0.0);

	//a 위 30m 범위에서 가장 돌출된 점을 찾는다.

	base_height = (base_height / 2) + 1; //base_height가 종아리앞점과 가깝게 너무 높으면 내려준다
	if (base_height > (up - 10))
		base_height -= 30;
	//up -= 10;

	for (int i = base_height; i < up; i++)
	{
		GVector3f p = m_FRSilhouetteV[i];

		if (p.X < AnklePt.X)
			AnklePt = p;
	}
	m_LandMarkPose2[30] = AnklePt;
}
void CSizeKorea2020Doc::FindAnklePtV2() 
{
	// 만약 발이 스캔되지 않은 경우(발끝점과 장딴지 앞점 두께가 50)에는 맨 아래바깥점을 가쪽복사점으로 하고 나간다
	//if ((m_LandMarkPose2[41].Z - m_LandMarkPose2[29].Z) < 50.0f) 
	//{
	//	m_LandMarkPose2[30] = m_FRSilhouetteV[5];
	//	m_LandMarkPose2[30].Z = m_LandMarkPose2[29].Z - 30.0f;
	//	m_LandMarkPose2[30].X += 5.0f;
	//	return;
	//}

	// 탐색 높이 70mm
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), EgPos(0.0, 70.0, 0.0)));
	theSlicer.Slice(m_ScanMesh);
	if (theSlicer.m_CutLines.size() != 2)
	{
		m_LandMarkPose2[30] = GVector3f(0.0f, 0.0f, 0.0f);
		return;
	}

	EgPos p1 = theSlicer.GetCenterPos(0);
	EgPos p2 = theSlicer.GetCenterPos(1);
	std::vector<EgPos> CutLine = (p1[0] < p2[0]) ? theSlicer.m_CutLines[0] : theSlicer.m_CutLines[1];
	
	EgPos MinPt(INFINITY, 0.0, 0.0), MaxPt(-INFINITY, 0.0, 0.0);
	for (EgPos p : CutLine)
	{
		if (p[0] < MinPt[0])
			MinPt = p;
		if (p[0] > MaxPt[0])
			MaxPt = p;
	}
	m_LandMarkPose2[30] = GVector3f(MinPt[0], MinPt[1], MinPt[2]);

	
	//int base_height = 0;
	//GVector3f v0(10000.0, 0.0, 0.0);
	//GVector3f v1(10000.0, 0.0, 0.0);

	//int  down, up;  // -> 32mm, 88mm
	//down = ROUND(0.015*m_Height) / m_HInterval;
	//up = ROUND(m_LandMarkPose2[29].Y - 16) / m_HInterval;

	//v0 = m_FRSilhouetteV[down];    // 4/2
	//v1 = m_FRSilhouetteV[up];		// 150/2
	//// 높이 4, 150 각각에서 가장 오른쪽 점을 찾는다.

	//v0.Z = v1.Z = 0.0; // projection to xy-plane
	//float m = (v1.Y - v0.Y) / (v1.X - v0.X);

	//GObNode<GVector3f> *pNode = m_FRSilhouetteV.m_pLeftEnd;
	//float max_dis = -10000.0f;

	////실루엣 점들 중 처음 만든 직선에서 가장 먼점 a을 구한다
	//while (pNode)
	//{
	//	GVector3f p = pNode->data;

	//	if (p.Y > v0.Y && p.Y < v1.Y  && p.Z < (m_LandMarkPose2[29].Z - 20.0f)) {

	//		float dis = fabs(m * p.X - p.Y - m * v0.X + v0.Y) / sqrt(m * m + 1);
	//		if (dis > max_dis)
	//		{
	//			max_dis = dis;
	//			base_height = ROUND(p.Y);
	//		}
	//	}
	//	pNode = pNode->m_pRight;
	//}
	//

	////a 위 30m 범위에서 가장 돌출된 점을 찾는다.

	//base_height = (base_height / 2) + 1; //base_height가 종아리앞점과 가깝게 너무 높으면 내려준다
	//if (base_height > (up - 10))
	//	base_height -= 30;
	////up -= 10;

	//for (int i = base_height; i < up; i++)
	//{
	//	GVector3f p = m_FRSilhouetteV[i];

	//	if (p.X < AnklePt.X)
	//		AnklePt = p;
	//}
	GVector3f AnklePt(10000.0, 0.0, 0.0);
	m_LandMarkPose2[30] = AnklePt;
}

// 장딴지앞점 설정
// 참고점: 측면 실루엣
void CSizeKorea2020Doc::FindLegSmallPtV() {
	int down = ROUND(m_Height * 0.03);
	int up = ROUND(m_Height * 0.07);

	float downH = m_FRSilhouetteV[down / m_HInterval].Y;
	float upH = m_FRSilhouetteV[up / m_HInterval].Y;
	float minZ = 10000.0f;

	GObNode<GVector3f> *pNode = m_SFSilhouetteV.m_pLeftEnd;
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (pt.Y<upH && pt.Y>downH)
		{
			if (pt.Z < minZ) {
				m_LandMarkPose2[29] = pt; //gluteal fold
				minZ = pt.Z;
			}
		}

		pNode = pNode->m_pRight;
	}

	int h = ROUND(m_LandMarkPose2[29].Y);
	GObNode<GVector3f> *pNode1 = m_pAlignedPointsV_Y[h].m_pLeftEnd;
	GVector3f KneePt(0.0, 0.0, -10000.0);
	GObList<GVector3f> CrossPtList;
	while (pNode1)
	{
		GVector3f pt = pNode1->data;
		if (pt.X < m_LandMarkHelp[0].X) {
			CrossPtList.Insert(pt);  //종아리최소둘레점들
		}
		pNode1 = pNode1->m_pRight;
	}


	CConvexHull cvx(CrossPtList);
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	m_ResultPose2[36] = cvx.GetHullLength();   //종아래최소 둘레
	m_ResultPathPose2[36] = cvx.m_HullPtList;  //종아리최소 경로
}

//배꼽점[35]
//배돌출점(23) 찾은후에, 허리옆점R([19], [20])
void CSizeKorea2020Doc::FindNavelPtV() {
	int start = (int)(m_LandMarkPose2[23].Y) - 60; // 배돌출점
	int end = (int)(m_LandMarkPose2[19].Y) + 10; //허리옆점

	if (start % 2 != 0) start++;

	int navelPlaneNum = 0;
	GVector3f tmpNavel, NavelPt;

	float sideLimitZ = m_LandMarkPose2[19].Z*0.4f + m_LandMarkPose2[23].Z*0.6f;
	float LimitRX = m_LandMarkPose2[19].X;
	float LimitLX = m_LandMarkPose2[20].X;

	GObList<GVector3f> frontPtList;

	for (int i = start; i < end; i += 2)
	{
		GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[i].m_pLeftEnd;

		while (pNode)
		{
			// _cprintf("m_PAlignedPointsV_Y[%d].m_pLeftEnd : %f %f %f\n", i, m_pAlignedPointsV_Y[i].m_pLeftEnd->data.X, m_pAlignedPointsV_Y[i].m_pLeftEnd->data.Y, m_pAlignedPointsV_Y[i].m_pLeftEnd->data.Z);
			GVector3f pt = pNode->data;
			if (pt.Z > sideLimitZ && pt.X > LimitRX  && pt.X < LimitLX)
				frontPtList.Insert(pt);
			pNode = pNode->m_pRight;
		}

		int slopDiffNum = 0;

		if (frontPtList.m_nCount > 2)
		{
			::getSortedPoint(frontPtList, 0, 0); //x, less->large

			pNode = frontPtList.m_pLeftEnd;
			GVector3f Prept = pNode->data;
			pNode = pNode->m_pRight;
			float slop, preSlop, ppreSlop, slopX;
			preSlop = ppreSlop = 0.0f;

			while (pNode)
			{
				// _cprintf("pNodeData2 %f %f %f", pNode->data.X, pNode->data.Y, pNode->data.Z);
				GVector3f pt = pNode->data;
				if ((pt.X - Prept.X) > 6.0f && pt.Z != Prept.Z)    //pt.Z != Prept.Z && 같은점이면 다음점으로 넘어감
				{
					slop = (pt.X - Prept.X) / (pt.Z - Prept.Z);

					slopX = slop * preSlop;

					//if(pNode->m_pRight){
					//      pNode = pNode->m_pRight;
					//      GVector3f nextpt = pNode->data;
					//     if( (nextpt.X-pt.X)<4.0f ) {
					//             pNode = pNode->m_pRight;
					//             nextpt = pNode->data;
					//     }
					//     nextSlop = (pt.X-Prept.X)/(pt.Z-Prept.Z);
					//     slopXnext = slop*nextSlop;

					if (slopX < 0 && preSlop<0 && preSlop>-5.0f && preSlop < 0) {  //&& slopXnext>0 //pt.X<(m_LandMarkPose2[23].X+40.0f)
						slopDiffNum++;
						// if(slopDiffNum == 2 )
						NavelPt = Prept;
					}
					//}
					ppreSlop = preSlop;
					preSlop = slop;

					Prept = pt;

				}//if

				//else
				pNode = pNode->m_pRight;

			}//while
		}

		if (slopDiffNum > 0) {
			navelPlaneNum++;
			if (navelPlaneNum == 3) {
				m_LandMarkPose2[35] = NavelPt;
				return;
			}
		}

		frontPtList.DeleteAll();

	} //for


	if (navelPlaneNum < 3) {
		float minDisX = 10000.0f;
		GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[start + 80].m_pLeftEnd;
		while (pNode)
		{
			GVector3f pt = pNode->data;
			float disX = ABS(pt.X - m_LandMarkPose2[23].X);

			if (disX<minDisX && pt.Z>sideLimitZ) {
				minDisX = disX;
				tmpNavel = pt;
			}

			pNode = pNode->m_pRight;
		}
		m_LandMarkPose2[35] = tmpNavel;
	}


}

//------ seek navel Right Pt(39), BackPt(Help77)
void CSizeKorea2020Doc::FindNavel_RPtV() {
	GPlane lplane, rplane;
	double tan1 = tan(M_PI / 2.0 - RefAngle[3]);
	double tan2 = tan(M_PI / 2.0 + RefAngle[4]);

	rplane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkHelp[1]);
	lplane.MakePlane(GVector3f(1.0f, tan2, 0.0f), m_LandMarkHelp[2]);

	//------ seek Waist Front, Back Pt
	int nh = ROUND(m_LandMarkPose2[35].Y);
	nh = nh - (nh % 2);
	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[nh].m_pLeftEnd;
	float minZ = 10000.0f;
	float maxZ = -10000.0f;
	GVector3f backPt;

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.Z > maxZ) {
				maxZ = pt.Z;
			}
			if (pt.Z < minZ) {
				minZ = pt.Z;
				backPt = pt;
			}
		}

		pNode = pNode->m_pRight;
	}

	float CZ = (maxZ + minZ)*0.5f;

	pNode = m_pAlignedPointsV_Y[nh].m_pLeftEnd;
	float minDis = 10000.0f;
	float limitX = m_LandMarkPose2[35].X - 40.0f;

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X < limitX) {
				float  dis = ABS(pt.Z - CZ);

				if (dis < minDis) {
					m_LandMarkPose2[39] = pt;
					minDis = dis;
				}
			}
		}

		pNode = pNode->m_pRight;
	}

	m_LandMarkPose2[35].Z = CZ;
}

void CSizeKorea2020Doc::FindFootEndPt() {
	int upH = 30;
	int downH = 0;
	float minz = 10000.0f, maxz = -10000.0f;

	GObNode<GVector3f> *pNode = m_SBSilhouetteV.m_pLeftEnd;
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (pt.Y<upH && pt.Y>downH)
		{
			if (pt.Z < minz) {
				m_LandMarkPose2[39] = pt; //발뒤꿈치
				minz = pt.Z;
			}
		}

		pNode = pNode->m_pRight;
	}

	pNode = m_SFSilhouetteV.m_pLeftEnd;
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (pt.Y<upH && pt.Y>downH)
		{
			if (pt.Z > maxz) {
				m_LandMarkPose2[38] = pt; //발끝점-앞
				maxz = pt.Z;
			}
		}

		pNode = pNode->m_pRight;
	}
}

//참고점: 가쪽복사점[30]
void CSizeKorea2020Doc::FindFootInPt() {
	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[10].m_pLeftEnd;

	float limitLX = m_LandMarkHelp[0].X;     //임시샅점
	float limitRX = m_LandMarkPose2[30].X + 20.0f;
	float SZ = m_LandMarkPose2[30].Z;       //가쪽복사점

	float minDisZin = 10000.0f;
	float minDisZout = 10000.0f;

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (pt.X < limitLX && pt.X>limitRX) {
			float disZ = ABS(pt.Z - SZ);

			if (disZ < minDisZin) {
				m_LandMarkHelp[71] = pt;    //발안쪽 바닥점
				minDisZin = disZ;
			}
		}

		if (pt.X < limitRX) {
			float disZ = ABS(pt.Z - SZ);

			if (disZ < minDisZout) {
				m_LandMarkHelp[76] = pt;    //발바같 바닥점
				minDisZout = disZ;
			}
		}

		pNode = pNode->m_pRight;
	}

	m_LandMarkHelp[71].X += 10.0f;
	m_LandMarkHelp[71].Y = 0.0f;

	//      m_LandMarkHelp[76] = m_LandMarkPose2[30];   //발바같 바닥점
	m_LandMarkHelp[76].Y = 0.0f;
	m_LandMarkHelp[76].X -= 5.0f;
}

void CSizeKorea2020Doc::FindBackProjectingPt() {
	float upH = m_LandMarkPose2[6].Y;
	float downH = m_LandMarkPose2[19].Y;

	float minz = 10000.0f;

	GObNode<GVector3f> *pNode = m_SBSilhouetteV.m_pLeftEnd;
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (pt.Y<upH && pt.Y>downH)
		{
			if (pt.Z < minz) {
				m_LandMarkPose2[37] = pt;
				minz = pt.Z;
			}
		}

		pNode = pNode->m_pRight;
	}
}

//  허리옆점-옷 밴드위치
void CSizeKorea2020Doc::FindWaistR_SlacksBand() {
	GPlane lplane, rplane;
	double tan1 = tan(M_PI / 2.0 - RefAngle[3]);
	double tan2 = tan(M_PI / 2.0 + RefAngle[4]);

	rplane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkHelp[1]);
	lplane.MakePlane(GVector3f(1.0f, tan2, 0.0f), m_LandMarkHelp[2]);


	GVector3f RHp = m_LandMarkPose2[19]; //허리옆점-R
	GVector3f LHp = m_LandMarkPose2[20]; //허리옆점-L
	RHp.Y -= 30.0f;   RHp.X -= 5.0f;
	LHp.Y -= 30.0f;   LHp.X += 5.0f;

	GVector3f maxxp = GVector3f(-1000.0f, 0.0f, 0.0f);
	GVector3f minxp = GVector3f(1000.0f, 0.0f, 0.0f);

	int upH = ROUND(RHp.Y);    //허리
	int downH = ROUND(m_LandMarkPose2[24].Y + 20.0f);       //엉덩이
	if (downH % 1) downH += 1;

	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[downH].m_pLeftEnd;
	//	배꼽아래-60mm높이에서 몸통좌우점 찾기
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X < minxp.X)	minxp = pt;
			if (pt.X > maxxp.X)	maxxp = pt;
		}
		pNode = pNode->m_pRight;
	}
	GVector3f RdownP = minxp;
	GVector3f LdownP = maxxp;


	// 좌우 평면에서 가장 먼 수직거리로 오목점 찾기
	GPlane Rpln, Lpln;
	GVector3f RNorm, LNorm;
	float Rslop, Lslop;
	if (RHp.X == RdownP.X)
		RNorm = GVector3f(1.0f, 0.0f, 0.0f);
	else {
		Rslop = (RHp.Y - RdownP.Y) / (RHp.X - RdownP.X);
		RNorm = GVector3f(Rslop, -1.0f, 0.0f);
	}
	Rpln.MakePlane(RNorm, RdownP);

	if (LHp.X == LdownP.X)
		LNorm = GVector3f(-1.0f, 0.0f, 0.0f);
	else {
		Lslop = (LHp.Y - LdownP.Y) / (LHp.X - LdownP.X);
		LNorm = GVector3f(Lslop, -1.0f, 0.0f);
	}
	Lpln.MakePlane(LNorm, LdownP);

	float maxdisR = -1000.0f;
	float maxdisL = -1000.0f;
	float disR, disL;

	GObList<GVector3f> RTorsoPtList;
	GObList<GVector3f> LTorsoPtList;

	GVector3f RWp, LWp;
	for (int i = downH; i < upH; i += 2)
	{
		GVector3f rPt, lPt;
		GVector3f maxxp = GVector3f(-1000.0f, 0.0f, 0.0f);
		GVector3f minxp = GVector3f(1000.0f, 0.0f, 0.0f);

		GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[i].m_pLeftEnd;

		while (pNode)
		{
			GVector3f pt = pNode->data;

			if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
			{
				if (pt.X < minxp.X)
					minxp = pt;

				if (pt.X > maxxp.X)
					maxxp = pt;
			}
			pNode = pNode->m_pRight;
		}

		RTorsoPtList.Insert(minxp);
		LTorsoPtList.Insert(maxxp);

		//if( i>middleH)	//좌우 직선에서 거리가 먼 점
		//{
		disR = Rpln.GetSignedDistance(minxp); //ABS( Rpln.GetSignedDistance(minxp) );
		disL = Lpln.GetSignedDistance(maxxp); //ABS( Lpln.GetSignedDistance(maxxp) );

		if (maxdisR < disR) {
			maxdisR = disR;	   RWp = minxp;
		}
		if (maxdisL < disL) {
			maxdisL = disL;     LWp = maxxp;
		}
		//}

	}

	//m_LandMarkHelp[77] = RWp;
	//m_LandMarkHelp[78] = LWp;

	int AverageH = (RWp.Y + LWp.Y)*0.5f;
	if (AverageH % 2) AverageH++;

	pNode = m_pAlignedPointsV_Y[AverageH].m_pLeftEnd;
	maxxp = GVector3f(-1000.0f, 0.0f, 0.0f);
	minxp = GVector3f(1000.0f, 0.0f, 0.0f);
	GVector3f minzp = GVector3f(0.0f, 0.0f, 10000.0f);

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X < minxp.X)
				minxp = pt;

			if (pt.X > maxxp.X)
				maxxp = pt;

			if (pt.Z < minzp.Z)
				minzp = pt;
		}

		pNode = pNode->m_pRight;
	}

	m_LandMarkHelp[72] = minzp;
	m_LandMarkHelp[77] = minxp;
	m_LandMarkHelp[78] = maxxp;
}

// 참고점: 정중시상면-뒤
void CSizeKorea2020Doc::FindArmPitH_Back() {
	GObNode<GVector3f> *pNode = m_SagittalPtsB.m_pLeftEnd;
	int num = m_SagittalPtsB.m_nCount;
	GVector3f Fpt, Bpt, intersectPt;

	Fpt = Bpt = m_LandMarkPose2[9];  //Armpit
	Bpt.Z -= 600.0f;

	float up = Fpt.Y + 40.0f;
	float down = Fpt.Y - 40.0f;
	for (int i = 0; i < (num - 10); i++)
	{
		GVector3f pt, pt1;

		pt = pNode->data;

		if (pNode) {
			pNode = pNode->m_pRight;
			pt1 = pNode->data;

			if (pt.Y > up || pt.Y < down) continue;

			if (::GetIntersectPointZY(Fpt, Bpt, pt1, pt, &intersectPt)) {
				m_LandMarkPose2[11] = intersectPt;
				m_LandMarkPose2[11].X = pt.X;
			}
		}
	}
}

void CSizeKorea2020Doc::MakeFSilhouetteV() {
	GVector3f Rarmpit = m_LandMarkHelp[1]; //임시 겨드랑점
	GVector3f Larmpit = m_LandMarkHelp[2];
	double tan1 = tan(M_PI / 2.0 - RefAngle[3]);
	double tan2 = tan(M_PI / 2.0 + RefAngle[4]);
	GPlane pln0, pln1;
	pln0.MakePlane(GVector3f(-1.0f, (float)tan1, 0.0f), Rarmpit);
	pln1.MakePlane(GVector3f(1.0f, (float)tan2, 0.0f), Larmpit);


	GVector3f rPt, lPt, fPt, fPt1, bPt, ArmRInPt, ArmRFPt;
	ArmRInPt = GVector3f(-10000.0f, 0.0f, 0.0f);
	ArmRFPt = GVector3f(0.0f, 0.0f, -10000.0f);
	float CX = m_LandMarkHelp[0].X;
	float RX = Rarmpit.X*0.7f + Larmpit.X*0.3f;

	for (int h = 0; h < m_nCount_Y; h += 2)
	{
		float minx = 1000.0f, maxx = -1000.0f;
		float minz = 1000.0f, maxz = -1000.0f, maxz1 = -1000.0f;
		float minxArmR = 1000.0f, maxxArmR = -1000.0f, maxzArmR = -1000.0f;
		GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[h].m_pLeftEnd;

		while (pNode)
		{
			GVector3f pt = pNode->data;

			// Fornt sil
			if (pt.X < minx) {
				rPt = pt;
				minx = rPt.X;
			}
			if (pt.X > maxx) {
				lPt = pt;
				maxx = lPt.X;
			}

			// Side sil   인체의 오른쪽에서, 팔이 폭함되지 안게
			// 겨드랑이 위는 팔분리 평면에 상관없이 함
			if (pt.Y > Rarmpit.Y) {
				if (pt.Z < minz && pt.X < (CX - 10.0f)) { //back
					bPt = pt;
					minz = bPt.Z;
				}
				if (pt.Z > maxz && pt.X < RX) {  //front - 좌우위치 제한 실루엣
					fPt = pt;
					maxz = fPt.Z;
				}

				if (pt.Z > maxz1) {  //front
					fPt1 = pt;
					maxz1 = fPt1.Z;
				}


			}
			else {
				if (pln0.IsBelowPlane(pt) && pln1.IsBelowPlane(pt))
				{
					if (pt.Z < minz && pt.X < (CX - 10.0f)) { //back
						bPt = pt;
						minz = bPt.Z;
					}
					if (pt.Z > maxz && pt.X < RX) {  //front - 좌우위치 제한 실루엣
						fPt = pt;
						maxz = fPt.Z;
					}

					if (pt.Z > maxz1) {  //front
						fPt1 = pt;
						maxz1 = fPt1.Z;
					}

				}
			}

			if (pln0.IsAbovePlane(pt) && h < Rarmpit.Y) {

				if (pt.X > maxxArmR) {
					ArmRInPt = pt;
					maxxArmR = ArmRInPt.X;
				}

				if (pt.Z > maxzArmR) {
					ArmRFPt = pt;
					maxzArmR = ArmRFPt.Z;
				}
			}


			pNode = pNode->m_pRight;
		}

		m_FRSilhouetteV.Insert(rPt);
		m_FLSilhouetteV.Insert(lPt);
		m_SFSilhouetteV.Insert(fPt);
		m_SBSilhouetteV.Insert(bPt);

		m_ArmRInSilhouetteV.Insert(ArmRInPt);
		m_ArmRFSilhouetteV.Insert(ArmRFPt);

		m_SFSilhouetteV1.Insert(fPt1);
	}
}

// 시상면=중간 종단면 만들기, 좌우 목옆점의 중점으로...
// 참고점: 목옆점좌우, 샅점
void CSizeKorea2020Doc::MakeSagittalSection() {
	GVector3f sneckL = m_LandMarkHelp[35];
	GVector3f sneckR = m_LandMarkHelp[36];

	float cx = (sneckL.X + sneckR.X) / 2.0f - 1.0f;
	float cz = m_LandMarkHelp[35].Z + 10.0f;
	GPlane pln;
	GVector3f rsneckHPt;
	GVector3f cp = GVector3f(cx, 0.0f, 0.0f);
	GVector3f normal = GVector3f(1.0f, 0.0f, 0.0f);
	pln.MakePlane(normal, cp);
	//float minSNeckY = 1000.0f,  SNeckY = sneckR.Y;
	int down = m_LandMarkPose2[25].Y;
	GObList<GVector3f> CrossPtList;

	for (long i = 0L; i < m_iNum; i += 3)
	{
		GVector3f b0, b1, b2;
		b0 = m_Vertex[m_FaceIdx[i]];
		b1 = m_Vertex[m_FaceIdx[i + 1]];
		b2 = m_Vertex[m_FaceIdx[i + 2]];

		//if ( b0.Y > up )
		//	continue;
		if (b0.Y < down)
			continue;
		//if ( b0.Z > m_LandMarkPose2[4].Z )	continue;

		if (pln.IsAbovePlane(b0) && pln.IsAbovePlane(b1) && pln.IsAbovePlane(b2))
			continue;
		if (pln.IsBelowPlane(b0) && pln.IsBelowPlane(b1) && pln.IsBelowPlane(b2))
			continue;

		if (pln.GetSignedDistance(b0) * pln.GetSignedDistance(b1) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b1 - b0);
			if (b0.Z < cz) m_SagittalPtsB.Insert(CrossPt);
			else  m_SagittalPtsF.Insert(CrossPt);
		}
		if (pln.GetSignedDistance(b1) * pln.GetSignedDistance(b2) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b1, b2 - b1);
			if (b0.Z < cz) m_SagittalPtsB.Insert(CrossPt);
			else  m_SagittalPtsF.Insert(CrossPt);
		}
		if (pln.GetSignedDistance(b2) * pln.GetSignedDistance(b0) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b2 - b0);
			if (b0.Z < cz) m_SagittalPtsB.Insert(CrossPt);
			else  m_SagittalPtsF.Insert(CrossPt);
		}
	}

	//sorting by y - from small to big
	::getSortedPoint(m_SagittalPtsF, 1, 0);
	::getSortedPoint(m_SagittalPtsB, 1, 0);
}

// 임시 겨드랑점과 임시 손목점을 찾은후에
// 임시 겨드랑점을 지나는 팔둘레
void CSizeKorea2020Doc::Calc_ArmC_Obesity_Temp() {
	//arm circ 구하기
	//팔둘레 자르는 평면
	GVector3f normal = GVector3f((m_LandMarkHelp[26].X - m_LandMarkHelp[27].X), (m_LandMarkHelp[26].Y - m_LandMarkHelp[27].Y), 0.0f);
	GPlane armCPln;
	armCPln.MakePlane(normal, m_LandMarkHelp[24]);

	GObList<GVector3f> CrossPtList;

	for (int i = 0; i < m_iNum; i += 3)
	{
		GVector3f v0, v1, v2;
		v0 = m_Vertex[m_FaceIdx[i]];
		v1 = m_Vertex[m_FaceIdx[i + 1]];
		v2 = m_Vertex[m_FaceIdx[i + 2]];
		if (v0.Y < m_LandMarkHelp[24].Y && v1.Y < m_LandMarkHelp[24].Y && v2.Y < m_LandMarkHelp[24].Y)
			continue;
		//	if ( v0.X>maxx  && v1.X>maxx  && v2.X>maxx )
		//		continue;
		if (armCPln.IsAbovePlane(v0) && armCPln.IsAbovePlane(v1) && armCPln.IsAbovePlane(v2))
			continue;
		if (armCPln.IsBelowPlane(v0) && armCPln.IsBelowPlane(v1) && armCPln.IsBelowPlane(v2))
			continue;

		if (armCPln.GetSignedDistance(v0) * armCPln.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = armCPln.GetProjectionPt(v0, v1 - v0);
			CrossPtList.Insert(CrossPt);
		}
		if (armCPln.GetSignedDistance(v1) * armCPln.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = armCPln.GetProjectionPt(v1, v2 - v1);
			CrossPtList.Insert(CrossPt);
		}
		if (armCPln.GetSignedDistance(v2) * armCPln.GetSignedDistance(v0) < 0)
		{
			GVector3f CrossPt = armCPln.GetProjectionPt(v0, v2 - v0);
			CrossPtList.Insert(CrossPt);
		}
	}

	CConvexHull cvx(CrossPtList);
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	float armC = cvx.GetHullLength();

	m_ArmObesity = armC; //armC/m_Height; // 범위 0.198(0.275~ 0.121  0.250~0.147)
}

// 임시 겨드랑점과 임시 손목점을 찾은후에
// 참고점: 팔위중심[26], 팔아래중심[27], 앞접힘[42], 뒤첩힘[43]
// 실제 겨드랑점(9)을 지나는 팔둘레
void CSizeKorea2020Doc::Calc_ArmC_Obesity() {
	Calc_ArmSlopV();

	//arm circ 구하기
	//팔둘레 자르는 평면
	GVector3f normal = GVector3f((m_LandMarkHelp[26].X - m_LandMarkHelp[27].X), (m_LandMarkHelp[26].Y - m_LandMarkHelp[27].Y), 0.0f);
	GPlane armCPln, plnRL;
	GVector3f armpitR = m_LandMarkPose2[9];
	armpitR.Y -= 4.0f;
	armCPln.MakePlane(normal, armpitR);

	GVector3f tmpRF = GVector3f(m_LandMarkPose2[44].X - 3.0f, m_LandMarkPose2[44].Y, m_LandMarkPose2[44].Z);
	GVector3f tmpRB = GVector3f(m_LandMarkPose2[44].X - 2.0f, m_LandMarkPose2[44].Y, m_LandMarkPose2[44].Z);
	GVector3f tmpRC = (m_LandMarkPose2[44] + m_LandMarkPose2[44])*0.5;
	tmpRC.Y -= 10.0f;
	plnRL.MakePlane(tmpRF, tmpRB, tmpRC);

	GVector3f armSideR;
	armSideR.X = 10000.0f;

	GObList<GVector3f> CrossPtList;

	for (int i = 0; i < m_iNum; i += 3)
	{
		GVector3f v0, v1, v2;
		v0 = m_Vertex[m_FaceIdx[i]];
		v1 = m_Vertex[m_FaceIdx[i + 1]];
		v2 = m_Vertex[m_FaceIdx[i + 2]];
		if (v0.Y < m_LandMarkPose2[9].Y && v1.Y < m_LandMarkPose2[9].Y && v2.Y < m_LandMarkPose2[9].Y)
			continue;
		//	if ( v0.X>maxx  && v1.X>maxx  && v2.X>maxx )
		//		continue;
		if (armCPln.IsAbovePlane(v0) && armCPln.IsAbovePlane(v1) && armCPln.IsAbovePlane(v2))
			continue;
		if (armCPln.IsBelowPlane(v0) && armCPln.IsBelowPlane(v1) && armCPln.IsBelowPlane(v2))
			continue;
		if (plnRL.IsBelowPlane(v0) && plnRL.IsBelowPlane(v1) && plnRL.IsBelowPlane(v2))
			continue;


		if (armCPln.GetSignedDistance(v0) * armCPln.GetSignedDistance(v1) < 0)
		{
			GVector3f CrossPt = armCPln.GetProjectionPt(v0, v1 - v0);
			CrossPtList.Insert(CrossPt);
			if (CrossPt.X < armSideR.X) armSideR = CrossPt;
		}
		if (armCPln.GetSignedDistance(v1) * armCPln.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = armCPln.GetProjectionPt(v1, v2 - v1);
			CrossPtList.Insert(CrossPt);
			if (CrossPt.X < armSideR.X) armSideR = CrossPt;
		}
		if (armCPln.GetSignedDistance(v2) * armCPln.GetSignedDistance(v0) < 0)
		{
			GVector3f CrossPt = armCPln.GetProjectionPt(v0, v2 - v0);
			CrossPtList.Insert(CrossPt);
			if (CrossPt.X < armSideR.X) armSideR = CrossPt;
		}
	}

	m_LandMarkHelp[40] = armSideR;
	CConvexHull cvx(CrossPtList);
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	float armC = cvx.GetHullLength();

	m_ArmObesity = armC; //armC/m_Height; // 범위 0.198(0.275~ 0.121  0.250~0.147)

	m_ResultPose2[36] = m_ArmObesity;
	m_ResultPose2[38] = armC;
	m_ResultPathPose2[38] = cvx.m_HullPtList;
}

void CSizeKorea2020Doc::Calc_TorsoC_ObesityV() {
	GPlane lplane, rplane;
	double tan1 = tan(0);//tan( M_PI / 2.0 - RefAngle[5]); //tan(0)
	double tan2 = tan(0);//tan( M_PI / 2.0 + RefAngle[6]);	
	rplane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkPose2[44]); //m_LandMarkPose2[9]
	lplane.MakePlane(GVector3f(1.0f, tan2, 0.0f), m_LandMarkPose2[46]);

	//	if( !m_bLandmarkFind ) return;

	//----------------- 젖가슴둘레
	// 오른쪽 BP높이의 너비 구하기
	int BPH = ROUND(m_LandMarkPose2[16].Y);
	BPH = BPH - BPH % m_HInterval;
	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[BPH].m_pLeftEnd;
	GVector3f minxp = GVector3f(10000.0f, 0.0f, 0.0f);
	GVector3f maxxp = GVector3f(-10000.0f, 0.0f, 0.0f);
	GObList<GVector3f> CrossPtList;
	while (pNode)
	{
		GVector3f pt = pNode->data;
		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X > maxxp.X)	 maxxp = pt;
			if (pt.X < minxp.X)	 minxp = pt;
			CrossPtList.Insert(pt);
		}
		pNode = pNode->m_pRight;
	}

	/*float BW=maxxp.X-minxp.X;
	minxp.Z=maxxp.Z;
	GVector3f BPBack = m_SBSilhouetteV[BPH/m_HInterval];
	BPBack.X = m_LandMarkPose2[16].X;
	m_ResultPose2[71] = BW;									// 젖가슴너비
	m_ResultPose2[80] = m_LandMarkPose2[16].Z-BPBack.Z;     // 젖가슴두께
	m_ResultPathPose2[71].DeleteAll();
	m_ResultPathPose2[71].Insert( minxp );					// 젖가슴너비 경로
	m_ResultPathPose2[71].Insert( maxxp );
	m_ResultPathPose2[80].Insert( m_LandMarkPose2[16] );    // 젖가슴두께 경로
	m_ResultPathPose2[80].Insert( BPBack );*/

	CConvexHull cvx(CrossPtList);
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	m_ResultPose2[27] = cvx.GetHullLength();   //젖가슴둘레
	m_ResultPathPose2[27] = cvx.m_HullPtList;  //젖가슴둘레 경로
	m_BustCObesity = m_ResultPose2[27] / m_Height;

	//----------------- 젖가슴아래둘레
	BPH = ROUND(m_LandMarkPose2[18].Y);
	BPH = BPH - BPH % m_HInterval;
	pNode = m_pAlignedPointsV_Y[BPH].m_pLeftEnd;
	minxp = GVector3f(10000.0f, 0.0f, 0.0f);
	maxxp = GVector3f(-10000.0f, 0.0f, 0.0f);
	CrossPtList.DeleteAll();  // 젖가슴아래둘레구할점들
	while (pNode)
	{
		GVector3f pt = pNode->data;
		//GVector3f pt = m_Vertex[data];
		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X > maxxp.X)	 maxxp = pt;
			if (pt.X < minxp.X)	 minxp = pt;
			CrossPtList.Insert(pt);
		}
		pNode = pNode->m_pRight;
	}

	CConvexHull cvx1(CrossPtList);
	cvx1.MakeConvexHullXZ();
	cvx1.ArrangeHullPtXZ();
	double circ = cvx1.GetHullLength();   //젖가슴아래둘레

	m_UnderBustCObesity = circ / m_Height;
}

// 팔, 몸통 분리각도: 허리, 엉덩이 높이 [3][4]
// 참고점 -임시겨드랑점(m_LandMarkHelp[1]), 임시샅점(m_LandMarkHelp[0])
void CSizeKorea2020Doc::Calc_RefAngleArmsV() {
	//허리위치 중간점
	float waistHf = m_LandMarkHelp[1].Y*0.8f + m_LandMarkHelp[0].Y*0.2f;  // 임시겨드랑점 조금 아래
	int waistH = ROUND(waistHf);
	if (waistH % 2) waistH--;

	double x0 = 100000.0;
	double x1 = 100000.0;
	double centerRX_W, centerLX_W, centerRX_C, centerLX_C;

	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[waistH].m_pLeftEnd;
	int size = m_pAlignedPointsV_Y[waistH].m_nCount;

	if (size < 50) {
		pNode = m_pAlignedPointsV_Y[waistH + 2].m_pLeftEnd;
		size = m_pAlignedPointsV_Y[waistH + 2].m_nCount;
	}

	double *pSlice = new double[size];
	int slice_index = 0;
	while (pNode)
	{
		GVector3f p = pNode->data;
		pSlice[slice_index] = p.X;
		pNode = pNode->m_pRight;
		slice_index++;
	}
	// start sorting by x.... from small to big
	for (int j = 0; j < size - 1; j++)
	{
		for (int k = j + 1; k < size; k++)
		{
			if (pSlice[j] > pSlice[k])
			{
				double imsi = pSlice[j];
				pSlice[j] = pSlice[k];
				pSlice[k] = imsi;
			}
		}
	}
	// right max dis x mid  
	//double left_dist = 0.0;           //오른쪽도 같이 할 수 있을텐데 이 while문에서....
	for (int j = 1; j < size / 2; j++) //간격이 가장 큰 곳(팔과 몸통사이 거리) 찾기
	{
		double dist = pSlice[j] - pSlice[j - 1];
		if (dist > 15.0)
		{
			x0 = pSlice[j - 1];
			x1 = pSlice[j];
		}
	}

	if (x0 == 100000.0 || x1 == 100000.0) //간격 큰 부분이 없을때 임시겨드랑점과 같은 x위치점으로 설정
		centerRX_W = m_LandMarkHelp[1].X - 2.0f;
	else
		centerRX_W = (x0 + x1) / 2.0;

	//left  max dis x mid
	x0 = 100000.0;
	x1 = 100000.0;
	for (int j = size / 2; j < size; j++) //간격이 가장 큰 곳(팔과 몸통사이 거리) 찾기
	{
		double dist = pSlice[j] - pSlice[j - 1];
		if (dist > 15.0)
		{
			x0 = pSlice[j - 1];
			x1 = pSlice[j];
		}
	}

	m_LandMarkHelp[42].X = centerRX_W; //허리위치정도 팔 몸통 중간점 오른쪽
	m_LandMarkHelp[42].Y = waistHf;

	if (x0 == 100000.0 || x1 == 100000.0) //간격 큰 부분이 없을때 임시겨드랑점과 같은 x위치점으로 설정
		centerLX_W = m_LandMarkHelp[2].X + 2.0f;
	else
		centerLX_W = (x0 + x1) / 2.0;

	m_LandMarkHelp[43].X = centerLX_W; //샅위치정도 팔 몸통 중간점
	m_LandMarkHelp[43].Y = waistHf;

	delete[] pSlice;
	pSlice = NULL;

	RefAngle[5] = atan((waistHf - m_LandMarkHelp[1].Y) / (centerRX_W - m_LandMarkHelp[1].X));
	RefAngle[6] = atan((waistHf - m_LandMarkHelp[2].Y) / (centerLX_W - m_LandMarkHelp[2].X));

	//if ( RefAngle[5]> 1.14159 || RefAngle[5]<0.523 ) 
	//	RefAngle[5] = 1.396f;
//	if (  RefAngle[6]< -1.14159 || RefAngle[6]>-0.523  )
//		RefAngle[6] = -1.396f;

	//----------------------------------샅보다 조금 높은 점
	float crotchHf = m_LandMarkHelp[0].Y + 120.0f; //-200.0f;
	int crotchH = ROUND(crotchHf);
	if (crotchH % 2)
		crotchH++;

	x0 = 100000.0;
	x1 = 100000.0;

	pNode = m_pAlignedPointsV_Y[crotchH].m_pLeftEnd;
	size = m_pAlignedPointsV_Y[crotchH].m_nCount;

	if (size < 100) {
		pNode = m_pAlignedPointsV_Y[crotchH + 2].m_pLeftEnd;
		size = m_pAlignedPointsV_Y[crotchH + 2].m_nCount;
	}

	pSlice = new double[size];
	slice_index = 0;
	while (pNode)
	{
		GVector3f p = pNode->data;
		pSlice[slice_index] = p.X;
		pNode = pNode->m_pRight;
		slice_index++;
	}
	// start sorting by x.... from small to big
	for (int j = 0; j < size - 1; j++)
	{
		for (int k = j + 1; k < size; k++)
		{
			if (pSlice[j] > pSlice[k])
			{
				double imsi = pSlice[j];
				pSlice[j] = pSlice[k];
				pSlice[k] = imsi;
			}
		}
	}
	double crx = m_LandMarkHelp[0].X - 50.0f; //(pSlice[0]+pSlice[size-1])/2.0 - 50.0;
	double clx = m_LandMarkHelp[0].X + 50.0f; //(pSlice[0]+pSlice[size-1])/2.0 + 50.0;
	// right max dis x mid  
	//double left_dist = 0.0;           //오른쪽도 같이 할 수 있을텐데 이 while문에서....
	for (int j = 1; j < size / 2; j++) //간격이 가장 큰 곳(팔과 몸통사이 거리) 찾기
	{
		double dist = pSlice[j] - pSlice[j - 1];
		if (dist > 60.0  && pSlice[j] < crx)
		{
			x0 = pSlice[j - 1];
			x1 = pSlice[j];
		}
	}

	if (x0 == 100000.0 || x1 == 100000.0) //간격 큰 부분이 없을때 임시겨드랑점과 같은 x위치점으로 설정
		centerRX_C = m_LandMarkHelp[1].X - 50.0f;
	else
		centerRX_C = (x0 + x1) / 2.0;

	//left  max dis x mid
	for (int j = size / 2; j < size; j++) //간격이 가장 큰 곳(팔과 몸통사이 거리) 찾기
	{
		double dist = pSlice[j] - pSlice[j - 1];
		if (dist > 60.0  && pSlice[j - 1] > clx)
		{
			x0 = pSlice[j - 1];
			x1 = pSlice[j];
		}
	}


	if (x0 == 100000.0 || x1 == 100000.0) //간격 큰 부분이 없을때 임시겨드랑점과 같은 x위치점으로 설정
		centerLX_C = m_LandMarkHelp[2].X + 50.0f;
	else
		centerLX_C = (x0 + x1) / 2.0;


	delete[] pSlice;
	pSlice = NULL;

	RefAngle[3] = atan((waistHf - crotchHf) / (centerRX_W - centerRX_C));
	RefAngle[4] = atan((waistHf - crotchHf) / (centerLX_W - centerLX_C));
}

// 팔기울기 구하기(팔중심점위, 팔중심점아래, 팔안쪽점위, 팔안쪽점아래 찾음)
// 참고점 - 임시겨드랑점(1), 손목안쪽점(32), 몸통팔분리각도
void CSizeKorea2020Doc::Calc_ArmSlopV() {
	// 오른쪽
		// 팔 직선만들기 
		// 팔꿈치점(under)과, 임시겨드랑점(1 up) 단면의 중심구하기
	float wristH = ROUND(m_LandMarkPose2[33].Y + 4.0f);
	float armpitH = m_LandMarkHelp[1].Y;
	int   underH = ROUND(wristH + (armpitH - wristH)*0.6f); //팔중간점(손목아래와 겨드랑의 4/5지점)
	int   upH = ROUND(armpitH - 12.0f);
	int   rem = underH % m_HInterval;
	underH = underH - rem;
	rem = upH % m_HInterval;
	upH = upH - rem;
	GVector3f underCP, upCP, underInXP, upInXP, sum1, sum2;


	// 아래중심 
	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[underH].m_pLeftEnd;
	//int num = m_pAlignedPoints_Y[underH].m_nCount;
	float minx = 10000.0; 	float maxx = -10000.0;
	float minz = 10000.0;	float maxz = -10000.0;
	float limitXR = m_LandMarkHelp[1].X - 10.0f;
	while (pNode) {
		GVector3f p = pNode->data;
		if (p.X < limitXR) {
			if (p.X > maxx)		maxx = p.X; //m_Vertex[index].X;
			if (p.X < minx)		minx = p.X;
			if (p.Z > maxz)		maxz = p.Z;
			if (p.Z < minz)		minz = p.Z;
		}
		pNode = pNode->m_pRight;
	}
	float cx = (maxx + minx)*0.5;
	float cz = (maxz*0.6) + (minz*0.4);
	underCP = GVector3f(cx, (float)underH, cz + 100.0f);

	m_LandMarkHelp[27] = underCP;
	underInXP = GVector3f(maxx, (float)underH, cz);


	//위중심
	pNode = m_pAlignedPointsV_Y[upH].m_pLeftEnd;
	//num = m_pAlignedPointsV_Y[upH].m_nCount;
	minx = 10000.0; 	maxx = -10000.0;
	minz = 10000.0; 	maxz = -10000.0;
	limitXR = m_LandMarkHelp[1].X - 4.0f;
	while (pNode) {
		GVector3f p = pNode->data;
		if (p.X < limitXR) {
			if (p.X > maxx)		maxx = p.X;
			if (p.X < minx)		minx = p.X;
			if (p.Z > maxz)		maxz = p.Z;
			if (p.Z < minz)		minz = p.Z;
		}
		pNode = pNode->m_pRight;
	}
	cx = (maxx + minx)*0.5f;
	cz = (maxz*0.6f) + (minz*0.4f);
	upCP = GVector3f(cx, (float)upH, cz + 100.0f);
	upInXP = GVector3f(maxx, (float)upH, cz);

	//손목 중심
	pNode = m_pAlignedPointsV_Y[ROUND(wristH)].m_pLeftEnd;
	minx = 10000.0; 	maxx = -10000.0;
	minz = 10000.0; 	maxz = -10000.0;
	limitXR = m_LandMarkPose2[33].X + 8.0f;
	while (pNode) {
		GVector3f p = pNode->data;
		if (p.X < limitXR) {
			if (p.X > maxx)		maxx = p.X;
			if (p.X < minx)		minx = p.X;
			if (p.Z > maxz)		maxz = p.Z;
			if (p.Z < minz)		minz = p.Z;
		}
		pNode = pNode->m_pRight;
	}

	cx = (maxx + minx)*0.5f;
	cz = (maxz*0.6f) + (minz*0.4f);

	GVector3f wristCP = GVector3f(cx, (float)wristH, cz + 100.0f);

	m_LandMarkHelp[26] = upCP;
	m_LandMarkHelp[24] = upInXP;
	m_LandMarkHelp[25] = underInXP;


	if (upCP.X == underCP.X)
		m_Armslop = 10000.0f;
	else
		m_Armslop = (upCP.Y - underCP.Y) / (upCP.X - underCP.X);


	if (wristCP.X - underCP.X == 0.0f)
		m_ArmslopDown = 10000.0f;
	else
		m_ArmslopDown = (wristCP.Y - underCP.Y) / (wristCP.X - underCP.X);
}

// 목앞등점-목뒤위점 각,  목옆뒤점-목뒤윗점 각,  목앞뒤중간 벡터 각
// 목앞 벡터 각
void CSizeKorea2020Doc::Calc_NeckAngles() {
	if (m_LandMarkHelp[37].Y == 0.0f || m_LandMarkHelp[38].Y == 0.0f)
		return;

	GVector3f  FneckBackP = m_LandMarkHelp[37];      //목앞등점
	GVector3f  SneckBackP = m_LandMarkHelp[38];
	GVector3f  hray = GVector3f(0.0f, 0.0f, 1.0f);	 //목앞등점 +z - 목앞등점

	// 목앞등점-목뒤위점 각
	GVector3f BackUpP = m_LandMarkHelp[39]; //목뒤위점

	GVector3f vb = BackUpP - FneckBackP; //목뒤윗점 - 목앞등점

	// 목옆뒤점-목뒤윗점 각
	vb = BackUpP - SneckBackP;    //목옆위등점 -  목옆등점
	float angle2 = vb.getAngle(hray);
	m_NeckBackAngle = 90.0f - (180.0f*angle2) / 3.14159;
}

//side neck 찾는데 목뒤각도가 필요하기 때문에 임시목옆점으로 각도를 임시로 계산한다.
//목옆점 측면위치조정을 범위제한 방법으로 할때는 필요없는 함수다
void CSizeKorea2020Doc::Calc_NeckAngles_BackTemp() {
	GVector3f  FneckBackP = m_LandMarkHelp[37];      //목앞등점
	GVector3f  SneckBackP = m_LandMarkHelp[38];
	GVector3f  hray = GVector3f(0.0f, 0.0f, 1.0f);	 //목앞등점 +z - 목앞등점


	// 목앞등점-목옆뒤점 각			
	GVector3f vb = SneckBackP - FneckBackP; //목뒤윗점 - 목앞등점

	float angle1 = vb.getAngle(hray);
	m_NeckBackAngle = 90.0f - (180.0f*angle1) / 3.14159;
}


void CSizeKorea2020Doc::GetResultPose2_54_56() {
	if (!m_bLandmarkFind) return;

	//  시상단면 이용
	GObList<GVector3f> CrossPtList;
	GObList<GVector3f> CrossPtList1;
	float upH = m_LandMarkPose2[6].Y;        // 목뒤점
	float downH = m_LandMarkPose2[22].Y;      // 허리뒤점.
	float armpitH = m_LandMarkPose2[9].Y;     // 겨랑점

	GObNode<GVector3f> *pNode = m_SagittalPtsB.m_pLeftEnd;

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (pt.Y<upH && pt.Y>downH)
		{
			CrossPtList.Insert(pt); //back length
			if (pt.Y > armpitH)
				CrossPtList1.Insert(pt);
		}
		pNode = pNode->m_pRight;
	}

	CConvexHull cvx(CrossPtList);
	cvx.MakeConvexHullYZ();
	cvx.ArrangeHullPtY();
	m_ResultPathPose2[56] = cvx.m_HullPtList;
	m_ResultPose2[56] = (float)cvx.GetHullLength2();


	CConvexHull cvx1(CrossPtList1);
	cvx1.MakeConvexHullYZ();
	cvx1.ArrangeHullPtY();
	m_ResultPathPose2[54] = cvx1.m_HullPtList;
	m_ResultPose2[54] = (float)cvx1.GetHullLength2();
}

void CSizeKorea2020Doc::Measure_Circum_Arranged() {
	if (!m_bLandmarkFind) return;

	//----------------- 27 젖가슴둘레
	// 오른쪽 BP높이의 너비 구하기

	GPlane lplane, rplane;
	double tan1 = tan(3.14159265 / 2.0 - RefAngle[5]);  //Calc_RefAngleArms1V()에서 RefAngle[5][6] 구함
	double tan2 = tan(3.14159265 / 2.0 + RefAngle[6]);
	rplane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), m_LandMarkPose2[9]);
	lplane.MakePlane(GVector3f(1.0f, tan2, 0.0f), m_LandMarkPose2[10]);


	GObList<GVector3f> CrossPtList, CrossPtList1;

	int BPH = ROUND(m_LandMarkPose2[16].Y);
	BPH = BPH - BPH % 2;
	GObNode<GVector3f> *pNode = m_pAlignedPointsV_Y[BPH].m_pLeftEnd;
	GVector3f minxp = GVector3f(10000.0f, 0.0f, 0.0f);
	GVector3f maxxp = GVector3f(-10000.0f, 0.0f, 0.0f);
	GVector3f minzp = GVector3f(0.0f, 0.0f, 10000.0f);
	GVector3f maxzp = GVector3f(0.0f, 0.0f, -10000.0f);

	while (pNode)
	{
		GVector3f pt = pNode->data;
		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X > maxxp.X)	 maxxp = pt;
			if (pt.X < minxp.X)	 minxp = pt;
			if (pt.X < m_LandMarkPose2[25].X) {
				if (pt.Z > maxzp.Z)	 maxzp = pt;
				if (pt.Z < minzp.Z)	 minzp = pt;
			}
			CrossPtList.Insert(pt);
		}
		pNode = pNode->m_pRight;
	}

	minxp.Z = maxxp.Z;
	m_ResultPose2[71] = maxxp.X - minxp.X;
	m_ResultPathPose2[71].DeleteAll();
	m_ResultPathPose2[71].Insert(minxp);					// 젖가슴너비 경로
	m_ResultPathPose2[71].Insert(maxxp);

	minzp.X = maxzp.X;
	m_ResultPose2[80] = maxzp.Z - minzp.Z;      // 젖가슴두께
	m_ResultPathPose2[80].DeleteAll();
	m_ResultPathPose2[80].Insert(minzp);    // 젖가슴두께 경로
	m_ResultPathPose2[80].Insert(maxzp);


	CConvexHull cvx(CrossPtList);
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	m_ResultPose2[27] = (float)cvx.GetHullLength();   //젖가슴둘레
	m_ResultPathPose2[27].DeleteAll();
	m_ResultPathPose2[27] = cvx.m_HullPtList;  //젖가슴둘레 경로

	GObNode<GVector3f> *pNode1 = cvx.m_HullPtList.m_pLeftEnd;  //젖꼭지사이길이
	while (pNode1)
	{
		GVector3f pt = pNode1->data;

		if (pt.Z > minxp.Z && pt.X >= m_LandMarkPose2[16].X && pt.X <= m_LandMarkPose2[17].X)
			CrossPtList1.Insert(pt);
		pNode1 = pNode1->m_pRight;
	}
	CrossPtList1.Insert(m_LandMarkPose2[16]);
	//CrossPtList1.Insert(m_LandMarkPose2[17]);
	::getSortedPoint(CrossPtList1, 0, 0);
	m_ResultPose2[77] = ::getSortedPtLen(CrossPtList1);
	m_ResultPathPose2[77].DeleteAll();
	m_ResultPathPose2[77] = CrossPtList1;  //젖꼭지사이길이 경로


	//----------------- 28 젖가슴아래둘레
	CrossPtList.DeleteAll();
	BPH = ROUND(m_LandMarkPose2[18].Y);
	BPH = BPH - BPH % 2;
	pNode = m_pAlignedPointsV_Y[BPH].m_pLeftEnd;
	minxp = GVector3f(10000.0f, 0.0f, 0.0f);
	maxxp = GVector3f(-10000.0f, 0.0f, 0.0f);
	minzp = GVector3f(0.0f, 0.0f, 10000.0f);
	maxzp = GVector3f(0.0f, 0.0f, -10000.0f);

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X > maxxp.X)	 maxxp = pt;
			if (pt.X < minxp.X)	 minxp = pt;
			if (pt.X < m_LandMarkPose2[25].X) {
				if (pt.Z > maxzp.Z)	 maxzp = pt;
				if (pt.Z < minzp.Z)	 minzp = pt;
			}
			CrossPtList.Insert(pt);
		}
		pNode = pNode->m_pRight;
	}

	minxp.Z = maxxp.Z;
	m_ResultPose2[72] = maxxp.X - minxp.X;	    // 젖가슴아래너비
	m_ResultPathPose2[72].DeleteAll();
	m_ResultPathPose2[72].Insert(minxp);      // 젖가슴아래너비 경로
	m_ResultPathPose2[72].Insert(maxxp);

	minzp.X = maxzp.X;
	m_ResultPose2[81] = maxzp.Z - minzp.Z;     // 젖가슴아래두께
	m_ResultPathPose2[81].DeleteAll();
	m_ResultPathPose2[81].Insert(maxzp);    // 젖가슴아래두께 경로
	m_ResultPathPose2[81].Insert(minzp);

	CConvexHull cvx1(CrossPtList);
	cvx1.MakeConvexHullXZ();
	cvx1.ArrangeHullPtXZ();
	m_ResultPose2[28] = (float)cvx1.GetHullLength();   //젖가슴아래둘레
	m_ResultPathPose2[28].DeleteAll();
	m_ResultPathPose2[28] = cvx1.m_HullPtList;  //젖가슴아래둘레 경로


	//----------------29 허리둘레 
	CrossPtList.DeleteAll();
	// 팔 몸 분리각도
	tan1 = tan(3.14159265 / 2.0 - RefAngle[3]);
	tan2 = tan(3.14159265 / 2.0 + RefAngle[4]);
	rplane.MakePlane(GVector3f(-1.0f, (float)tan1, 0.0f), m_LandMarkHelp[42]);
	lplane.MakePlane(GVector3f(1.0f, (float)tan2, 0.0f), m_LandMarkHelp[43]);

	int	wh = ROUND(m_LandMarkPose2[19].Y);
	wh = wh - (wh % 2);
	pNode = m_pAlignedPointsV_Y[wh].m_pLeftEnd;
	minxp = GVector3f(10000.0f, 0.0f, 0.0f);
	maxxp = GVector3f(-10000.0f, 0.0f, 0.0f);
	minzp = GVector3f(0.0f, 0.0f, 10000.0f);
	maxzp = GVector3f(0.0f, 0.0f, -10000.0f);

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			if (pt.X > maxxp.X)	 maxxp = pt;
			if (pt.X < minxp.X)	 minxp = pt;
			if (pt.X < m_LandMarkPose2[25].X) {
				if (pt.Z > maxzp.Z)	 maxzp = pt;
				if (pt.Z < minzp.Z)	 minzp = pt;
			}
			CrossPtList.Insert(pt);
		}

		pNode = pNode->m_pRight;
	}

	int windex = wh / 2;
	minxp.Z = maxxp.Z;
	m_ResultPose2[73] = maxxp.X - minxp.X;   // 허리너비
	m_ResultPathPose2[73].DeleteAll();
	m_ResultPathPose2[73].Insert(maxxp);	   //허리너비경로
	m_ResultPathPose2[73].Insert(minxp);

	minzp.X = maxzp.X;
	m_ResultPose2[82] = maxzp.Z - minzp.Z;	// 허리두께
	m_ResultPathPose2[82].DeleteAll();
	m_ResultPathPose2[82].Insert(maxzp);	//허리두께경로
	m_ResultPathPose2[82].Insert(minzp);

	CConvexHull cvx2(CrossPtList);
	cvx2.MakeConvexHullXZ();
	cvx2.ArrangeHullPtXZ();
	m_ResultPose2[29] = (float)cvx2.GetHullLength();   //허리둘레
	m_ResultPathPose2[29].DeleteAll();
	m_ResultPathPose2[29] = cvx2.m_HullPtList;  //허리둘레 경로

	//-------------30 배둘레
	CrossPtList.DeleteAll();

	wh = ROUND(m_LandMarkPose2[23].Y);
	wh = wh - (wh % 2);
	pNode = m_pAlignedPointsV_Y[wh].m_pLeftEnd;

	minxp = GVector3f(10000.0f, 0.0f, 0.0f);
	maxxp = GVector3f(-10000.0f, 0.0f, 0.0f);
	minzp = GVector3f(0.0f, 0.0f, 10000.0f);
	maxzp = GVector3f(0.0f, 0.0f, -10000.0f);

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt)) {
			if (pt.X > maxxp.X)	 maxxp = pt;
			if (pt.X < minxp.X)	 minxp = pt;
			if (pt.X < m_LandMarkPose2[25].X) {
				if (pt.Z > maxzp.Z)	 maxzp = pt;
				if (pt.Z < minzp.Z)	 minzp = pt;
			}
			CrossPtList.Insert(pt);
		}

		pNode = pNode->m_pRight;
	}


	minxp.Z = maxxp.Z;
	m_ResultPose2[74] = maxxp.X - minxp.X;   // 배너비
	m_ResultPathPose2[74].DeleteAll();
	m_ResultPathPose2[74].Insert(maxxp);   // 배너비경로
	m_ResultPathPose2[74].Insert(minxp);

	minzp.X = maxzp.X;
	m_ResultPose2[83] = maxzp.Z - minzp.Z;			// 배두께
	m_ResultPathPose2[83].DeleteAll();
	m_ResultPathPose2[83].Insert(maxzp);			// 배두께경로
	m_ResultPathPose2[83].Insert(minzp);


	CConvexHull cvx12(CrossPtList);
	cvx12.MakeConvexHullXZ();
	cvx12.ArrangeHullPtXZ();
	m_ResultPose2[30] = (float)cvx12.GetHullLength();   //배둘레
	m_ResultPathPose2[30].DeleteAll();
	m_ResultPathPose2[30] = cvx12.m_HullPtList;  //배둘레 경로


	//-------------31 배꼽수준둘레 
	CrossPtList.DeleteAll();
	wh = ROUND(m_LandMarkPose2[35].Y);
	wh = wh - (wh % 2);
	pNode = m_pAlignedPointsV_Y[wh].m_pLeftEnd;

	minxp = GVector3f(10000.0f, 0.0f, 0.0f);
	maxxp = GVector3f(-10000.0f, 0.0f, 0.0f);
	minzp = GVector3f(0.0f, 0.0f, 10000.0f);
	maxzp = GVector3f(0.0f, 0.0f, -10000.0f);

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt)) {
			if (pt.X > maxxp.X)	 maxxp = pt;
			if (pt.X < minxp.X)	 minxp = pt;
			if (pt.X < m_LandMarkPose2[25].X) {
				if (pt.Z > maxzp.Z)	 maxzp = pt;
				if (pt.Z < minzp.Z)	 minzp = pt;
			}
			CrossPtList.Insert(pt);
		}

		pNode = pNode->m_pRight;
	}

	minxp.Z = maxxp.Z;
	m_ResultPose2[75] = maxxp.X - minxp.X;   // 배꼽너비
	m_ResultPathPose2[75].DeleteAll();
	m_ResultPathPose2[75].Insert(maxxp);   // 배꼽너비경로
	m_ResultPathPose2[75].Insert(minxp);

	minzp.X = maxzp.X;
	m_ResultPose2[84] = maxzp.Z - minzp.Z;	    // 배꼽두께
	m_ResultPathPose2[84].DeleteAll();
	m_ResultPathPose2[84].Insert(maxzp);		// 배꼽두께경로
	m_ResultPathPose2[84].Insert(minzp);

	CConvexHull cvx13(CrossPtList);
	cvx13.MakeConvexHullXZ();
	cvx13.ArrangeHullPtXZ();
	m_ResultPose2[31] = (float)cvx13.GetHullLength();   //배꼽허리둘레
	m_ResultPathPose2[31].DeleteAll();
	m_ResultPathPose2[31] = cvx13.m_HullPtList;  //배꼽허리둘레 경로



	//-----------------32 엉덩이둘레 점들
	CrossPtList.DeleteAll();
	int hipH = ROUND(m_LandMarkPose2[24].Y);
	hipH = hipH - (hipH % 2);
	pNode = m_pAlignedPointsV_Y[hipH].m_pLeftEnd;
	maxxp = GVector3f(-1000.0f, 0.0f, 0.0f);
	minxp = GVector3f(1000.0f, 0.0f, 0.0f);
	minzp = GVector3f(0.0f, 0.0f, 10000.0f);
	maxzp = GVector3f(0.0f, 0.0f, -10000.0f);

	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (lplane.IsBelowPlane(pt) && rplane.IsBelowPlane(pt))
		{
			CrossPtList.Insert(pt);
			if (pt.X < minxp.X)	minxp = pt;
			if (pt.X > maxxp.X)	maxxp = pt;
			if (pt.X < m_LandMarkPose2[25].X) {
				if (pt.Z < minzp.Z)	minzp = pt;
				if (pt.Z > maxzp.Z)	maxzp = pt;
			}
		}
		pNode = pNode->m_pRight;
	}

	minxp.Z = maxxp.Z;
	m_ResultPose2[76] = maxxp.X - minxp.X;
	m_ResultPathPose2[76].DeleteAll();
	m_ResultPathPose2[76].Insert(maxxp);				 //엉덩이너비경로
	m_ResultPathPose2[76].Insert(minxp);

	minzp.X = maxzp.X;
	m_ResultPose2[85] = maxzp.Z - minzp.Z;
	m_ResultPathPose2[85].DeleteAll();
	m_ResultPathPose2[85].Insert(maxzp);  //엉덩이두께경로
	m_ResultPathPose2[85].Insert(minzp);

	CConvexHull cvx0(CrossPtList);
	cvx0.MakeConvexHullXZ();
	cvx0.ArrangeHullPtXZ();
	m_ResultPose2[32] = (float)cvx0.GetHullLength();   //엉덩이둘레
	m_ResultPathPose2[32].DeleteAll();
	m_ResultPathPose2[32] = cvx0.m_HullPtList;  //엉덩이둘레 경로


	//-----------------33 넙다리둘레
	CrossPtList.DeleteAll();
	int glutealH = ROUND(m_LandMarkPose2[26].Y);  //볼기고랑점
	glutealH = glutealH - (glutealH % 2);
	pNode = m_pAlignedPointsV_Y[glutealH].m_pLeftEnd;
	//if(m_pAlignedPoints_Y[glutealH].m_nCount<100) pNode = m_pAlignedPoints_Y[glutealH+m_HInterval].m_pLeftEnd;
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (pt.X < m_LandMarkHelp[0].X  && rplane.IsBelowPlane(pt))
		{
			CrossPtList.Insert(pt);
		}
		pNode = pNode->m_pRight;
	}

	CConvexHull cvx3(CrossPtList);
	cvx3.MakeConvexHullXZ();
	cvx3.ArrangeHullPtXZ();
	m_ResultPose2[33] = (float)cvx3.GetHullLength();   //넙적다리 둘레
	m_ResultPathPose2[33].DeleteAll();
	m_ResultPathPose2[33] = cvx3.m_HullPtList;  //넙적다리 경로


	//-----------------34 무릎둘레
	CrossPtList.DeleteAll();
	int kneeH = ROUND(m_LandMarkPose2[27].Y);
	kneeH = kneeH - (kneeH % 2);
	pNode = m_pAlignedPointsV_Y[kneeH].m_pLeftEnd;

	while (pNode)
	{
		GVector3f pt = pNode->data;
		if (pt.X < m_LandMarkHelp[0].X) {
			CrossPtList.Insert(pt);
		}
		pNode = pNode->m_pRight;
	}

	CConvexHull cvx4(CrossPtList);
	cvx4.MakeConvexHullXZ();
	cvx4.ArrangeHullPtXZ();
	m_ResultPose2[34] = (float)cvx4.GetHullLength();   //무릎 둘레
	m_ResultPathPose2[34].DeleteAll();
	m_ResultPathPose2[34] = cvx4.m_HullPtList;  //무릎 경로


	//------------ 35 장딴지둘레
	CrossPtList.DeleteAll();
	int calfH = ROUND(m_LandMarkPose2[28].Y);  //장딴지점
	calfH = calfH - (calfH % 2);
	pNode = m_pAlignedPointsV_Y[calfH].m_pLeftEnd;
	//if(m_pAlignedPointsV_Y[calfH].m_nCount<100) pNode = m_pAlignedPointsV_Y[calfH+m_HInterval].m_pLeftEnd;
	while (pNode)
	{
		GVector3f pt = pNode->data;

		if (pt.X < m_LandMarkHelp[0].X  && rplane.IsBelowPlane(pt))
		{
			CrossPtList.Insert(pt);
		}
		pNode = pNode->m_pRight;
	}

	CConvexHull cvx5(CrossPtList);
	cvx5.MakeConvexHullXZ();
	cvx5.ArrangeHullPtXZ();
	m_ResultPose2[35] = (float)cvx5.GetHullLength();   //장딴지 둘레
	m_ResultPathPose2[35].DeleteAll();
	m_ResultPathPose2[35] = cvx5.m_HullPtList;  //장딴지 경로


	//------------ 36 종아래최소 둘레
	CrossPtList.DeleteAll();
	int h = ROUND(m_LandMarkPose2[29].Y);
	h = h - (h % 2);
	pNode = m_pAlignedPointsV_Y[h].m_pLeftEnd;
	GVector3f KneePt(0.0, 0.0, -10000.0);
	while (pNode)
	{
		GVector3f pt = pNode->data;
		if (pt.X < m_LandMarkHelp[0].X) {
			CrossPtList.Insert(pt);  //종아리최소둘레점들
		}
		pNode = pNode->m_pRight;
	}

	CConvexHull cvx6(CrossPtList);
	cvx6.MakeConvexHullXZ();
	cvx6.ArrangeHullPtXZ();
	m_ResultPose2[36] = (float)cvx6.GetHullLength();   //종아래최소 둘레
	m_ResultPathPose2[36].DeleteAll();
	m_ResultPathPose2[36] = cvx6.m_HullPtList;  //종아리최소 경로
}

void CSizeKorea2020Doc::Measure_Circum_UnArranged() {
	// 측정항목 27_77_71_80 : 젓가슴둘레, 젖꼭지사이길이,젖가슴 너비, 젖가슴 두께.
	::GetResultPose2_27_77_71_80(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[27], &m_ResultPathPose2[77], &m_ResultPathPose2[71], &m_ResultPathPose2[80]);

	// 측정항목 28_72_81 : 젖가슴 아래 둘레, 젖가슴 아래 너비, 젖가슴 아래 두께.
	::GetResultPose2_28_72_81(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[28], &m_ResultPathPose2[72], &m_ResultPathPose2[81]);

	// 측정항목 29_73_82 : 허리둘레, 허리 너비, 허리 두께.
	::GetResultPose2_29_73_82(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[29], &m_ResultPathPose2[73], &m_ResultPathPose2[82]);

	// 측정항목 30_74_83 : 배둘레, 배너비, 배두께
	::GetResultPose2_30_74_83(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[30], &m_ResultPathPose2[59], &m_ResultPathPose2[83]);

	// 측정항목 31_75_84 : 배꼽수준허리둘레, 너비, 두께
	::GetResultPose2_31_75_84(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[31], &m_ResultPathPose2[75], &m_ResultPathPose2[84]);

	// 측정항목 32_76_85 : 엉덩이둘레, 엉덩이너비, 엉덩이두께
	::GetResultPose2_32_76_85(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_LandMarkHelp, m_OldLandMarkPose2, &m_ResultPathPose2[32], &m_ResultPathPose2[76], &m_ResultPathPose2[85]);

	// 측정항목 33 : 넙다리둘레
	::GetResultPose2_33(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[33]);

	// 측정항목 34 : 무릎둘레
	::GetResultPose2_34(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[34]);

	// 측정항목 35 : 장딴지 둘레
	::GetResultPose2_35(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[35]);

	// 측정항목 36 : 종아리 최소둘레
	::GetResultPose2_36(m_ResultPose2, m_Vertex, m_vNum, m_FaceIdx, m_iNum, m_LandMarkPose2, m_OldLandMarkPose2, &m_ResultPathPose2[36]);
}

void CSizeKorea2020Doc::Measure_LengthSurface(GVector3f vStart, GVector3f vEnd, GVector3f v3, int vnum, int parallelPlane, int ForB, float *Result, int resultIndex, GObList<GVector3f> *CvxPt) {
	GPlane CutPln;	// 길이가 놓여질 평면.
	GPlane RefPln;	// 앞뒤좌우 구분 평면.

	if (vnum == 2)   //1: 두점을 지나는 길이 -> 임의의 한 점을 만들어줌
	{
		v3 = (vStart + vEnd) / 2.0f;

		switch (parallelPlane)  // 다른 한점을 구할때 어느평면과 평행한지를 1=x축에 평행, 2=y축에 평행 3=z축에 평행
		{
		case 1:  v3.X += 10.0f;  break; // side view   x축에 평행l
		case 2:  v3.Y += 10.0f;  break; // top view
		case 3:  v3.Z += 10.0f;  break; // front view
		case 4:  v3.Z -= 10.0f;  break; // back view
		}
	}
	else if (vnum == 1)
	{
		v3 = vStart;
		switch (parallelPlane)  // 다른 한점을 구할때 어느평면과 평행한지를 1=x, 2=y, 3=z
		{
		case 1:  v3.X += 10.0f;    vEnd.Z = vStart.Z; break;
		case 2:  v3.Y += 10.0f;    vEnd.Z = vStart.Z; break;
		case 3:  v3.Z += 10.0f;    vEnd.X = vStart.X; break;
		}
	}

	CutPln.MakePlane(vStart, v3, vEnd);

	// ref plane은 cutpln의 아래점을 제외시키기 위한 평면
	// ref plane의 normal은  CutPln의 normal과 start, end 두점벡터의 외적으로 한다.
	//GVector3f tmp = (vStart+vEnd)/2.0f; tmp.X += 100.0f;
	GVector3f refNorm = (CutPln.GetNormal()) ^ (vStart - vEnd);     //GVector3f n = ( b - a )^( c - a );
	refNorm.normalize();
	RefPln.MakePlane(refNorm, vStart);

	float maxX, minX, maxZ, minZ, maxY, minY;
	maxX = maxZ = maxY = -10000.0f;
	minX = minZ = minY = 10000.0f;

	GVector3f v[3];
	v[0] = vStart; v[1] = v3; v[2] = vEnd;

	for (int k = 0; k < 3; k++)
	{
		if (v[k].X > maxX) maxX = v[k].X;
		if (v[k].Y > maxY) maxY = v[k].Y;
		if (v[k].Z > maxZ) maxZ = v[k].Z;

		if (v[k].X < minX) minX = v[k].X;
		if (v[k].Y < minY) minY = v[k].Y;
		if (v[k].Z < minZ) minZ = v[k].Z;
	}


	GObList<GVector3f> CrossPtList;
	for (long i = 0L; i < m_iNum; i += 3)
	{
		GVector3f v0, v1, v2;
		v0 = m_Vertex[m_FaceIdx[i]];
		v1 = m_Vertex[m_FaceIdx[i + 1]];
		v2 = m_Vertex[m_FaceIdx[i + 2]];

		if (RefPln.IsBelowPlane(v0) || RefPln.IsBelowPlane(v1) || RefPln.IsBelowPlane(v2))
			continue;

		if (CutPln.IsAbovePlane(v0) && CutPln.IsAbovePlane(v1) && CutPln.IsAbovePlane(v2))
			continue;
		if (CutPln.IsBelowPlane(v0) && CutPln.IsBelowPlane(v1) && CutPln.IsBelowPlane(v2))
			continue;

		if (v0.X > maxX || v1.X > maxX || v2.X > maxX || v0.X < minX || v1.X < minX || v2.X < minX)
			continue;
		if (v0.Y > maxY || v1.Y > maxY || v2.Y > maxY || v0.Y < minY || v1.Y < minY || v2.Y < minY)
			continue;
		if (v0.Z > maxZ || v1.Z > maxZ || v2.Z > maxZ || v0.Z < minZ || v1.Z < minZ || v2.Z < minZ)
			continue;

		if (CutPln.GetSignedDistance(v0) * CutPln.GetSignedDistance(v1) <= 0)  //만약 v0, v1의 x좌표가 같으면? 
		{
			GVector3f CrossPt = CutPln.GetProjectionPt(v0, v1 - v0);
			CrossPtList.Insert(CrossPt);
			/*if(CrossPt.X<minX) minX = CrossPt.X;
			if(CrossPt.Y<minY) minY = CrossPt.Y;
			if(CrossPt.Z<minZ) minZ = CrossPt.Z;
			if(CrossPt.X>maxX) maxX = CrossPt.X;
			if(CrossPt.Y>maxY) maxY = CrossPt.Y;
			if(CrossPt.Z>maxZ) maxZ = CrossPt.Z;*/
		}
		if (CutPln.GetSignedDistance(v1) * CutPln.GetSignedDistance(v2) <= 0)
		{
			GVector3f CrossPt = CutPln.GetProjectionPt(v1, v2 - v1);
			CrossPtList.Insert(CrossPt);
			/*if(CrossPt.X<minX) minX = CrossPt.X;
			if(CrossPt.Y<minY) minY = CrossPt.Y;
			if(CrossPt.Z<minZ) minZ = CrossPt.Z;
			if(CrossPt.X>maxX) maxX = CrossPt.X;
			if(CrossPt.Y>maxY) maxY = CrossPt.Y;
			if(CrossPt.Z>maxZ) maxZ = CrossPt.Z;*/
		}
		if (CutPln.GetSignedDistance(v2) * CutPln.GetSignedDistance(v0) <= 0)
		{
			GVector3f CrossPt = CutPln.GetProjectionPt(v0, v2 - v0);
			CrossPtList.Insert(CrossPt);
			/*if(CrossPt.X<minX) minX = CrossPt.X;
			if(CrossPt.Y<minY) minY = CrossPt.Y;
			if(CrossPt.Z<minZ) minZ = CrossPt.Z;
			if(CrossPt.X>maxX) maxX = CrossPt.X;
			if(CrossPt.Y>maxY) maxY = CrossPt.Y;
			if(CrossPt.Z>maxZ) maxZ = CrossPt.Z;*/
		}


	} //for


	float    disX = maxX - minX;
	float    disY = maxY - minY;
	float    disZ = maxZ - minZ;


	//x,y,z 방향중에 길이 거리가 큰 순서 알기
	int biggestDis;
	int secondDis;
	int shortestDis;
	if (disX > disY)
	{
		if (disX > disZ) {
			biggestDis = 1; //x
			if (disY > disZ) { secondDis = 2;  shortestDis = 3; }
			else { secondDis = 3; shortestDis = 2; }
		}
		else { //z>x>y
			biggestDis = 3; //z
			secondDis = 1;
			shortestDis = 2;
		}
	}
	else
	{ //disY > disX
		if (disY > disZ) {
			biggestDis = 2;  //y
			if (disX > disZ) { secondDis = 1; shortestDis = 3; }
			else { secondDis = 3; shortestDis = 1; }
		}
		else {  // z>y>x
			biggestDis = 3;      //z
			secondDis = 2;
			shortestDis = 1;
		}
	}

	CrossPtList.Insert(vStart);
	CrossPtList.Insert(vEnd);

	CConvexHull cvx(CrossPtList);
	GVector3f addPt;

	int startIndex = -1;

	// 길이 경로에서 vStart과 vEnd 직선으로부터의 최대거리를 알아봄 - 나중에 convexHull만들때 점을 추개해야할지 판단하기 위해...
	GObNode<GVector3f>* pNode = CrossPtList.m_pLeftEnd;
	float maxDis = -1000.0f;

	while (pNode)
	{
		GVector3f v = pNode->data;

		float dis = RefPln.GetSignedDistance(v);

		if (dis > maxDis) maxDis = dis;

		pNode = pNode->m_pRight;
	}

	GVector3f tmp = vStart - vEnd;
	float LengthStartEnd = tmp.getNorm();
	/*if(parallelPlane)
	{
		switch(parallelPlane)  // 다른 한점을 구할때 어느평면과 평행한지를 1=x, 2=y, 3=z
		{
		case 1:
			if(disX < disY) {  //세로로 더 길때  XY으로 convexhull만듦
				addPt=GVector3f(vStart.X+300.0f, vStart.Y, vStart.Z);  //side
				cvx.m_PtList.Insert( addPt ); cvx.MakeConvexHullXY();
				if( cvx.m_HullPtList.Search( vStart) <0 )  cvx.m_HullPtList.Insert( vStart );
				cvx.ArrangeHullPtXY();
			}
			else{               //가로로 더 길때 XZ로~~
				addPt=GVector3f(vStart.X+300.0f, vStart.Y, vStart.Z);  //s
				cvx.m_PtList.Insert( addPt ); cvx.MakeConvexHullXZ();
				if( cvx.m_HullPtList.Search( vStart) <0 )  cvx.m_HullPtList.Insert( vStart );
				cvx.ArrangeHullPtXZ();
			}
			break;
		case 2:
			addPt=GVector3f(vStart.X, vStart.Y-300.0f, vStart.Z);  //top
			cvx.m_PtList.Insert( addPt ); cvx.MakeConvexHullXY();
			if( cvx.m_HullPtList.Search( vStart) <0 )  cvx.m_HullPtList.Insert( vStart );
			cvx.ArrangeHullPtXY();
			break;
		case 3:
			if(disX < disY) {  //세로로 더 길때  YZ으로 convexhull
				addPt=GVector3f(vStart.X, vStart.Y, vStart.Z-300.0f);  //f
				cvx.m_PtList.Insert( addPt ); cvx.MakeConvexHullYZ();
				if( cvx.m_HullPtList.Search( vStart) <0 )  cvx.m_HullPtList.Insert( vStart );
				cvx.ArrangeHullPtYZ1();
			}
			else  {           //가로로 더 길때
				addPt=GVector3f(vStart.X, vStart.Y, vStart.Z-300.0f);
				cvx.m_PtList.Insert( addPt ); cvx.MakeConvexHullXZ();
				if( cvx.m_HullPtList.Search( vStart) <0 )  cvx.m_HullPtList.Insert( vStart );
				cvx.ArrangeHullPtXZ();
			}
			break;
		case 4:
			if(disX < disY) {  //세로로 더 길때
				addPt=GVector3f(vStart.X, vStart.Y, vStart.Z+300.0f);  //b
				cvx.m_PtList.Insert( addPt );    cvx.MakeConvexHullYZ();
				if( cvx.m_HullPtList.Search( vStart) <0 )  cvx.m_HullPtList.Insert( vStart );
				cvx.ArrangeHullPtYZ();
			}
			else  {             //가로로 더 길때
				addPt=GVector3f(vStart.X, vStart.Y, vStart.Z+300.0f);
				cvx.m_PtList.Insert( addPt );   cvx.MakeConvexHullXZ();
				if( cvx.m_HullPtList.Search( vStart) <0 )  cvx.m_HullPtList.Insert( vStart );
				cvx.ArrangeHullPtXZ1();
			}
			break;
		}
			   cvx.m_HullPtList.Delete(addPt);

	}
	else{*/
	// 길이가 직선에 가까운 모양이면 convexhull을 잘 만들기 위해 점을 하나 추가한다.
	float ratioLength = LengthStartEnd / maxDis;

	if (ratioLength > 2.0f) {
		addPt = (vStart + vEnd)*0.5f + RefPln.n*(-200.0f);
		cvx.m_PtList.Insert(addPt);
	}

	if (shortestDis == 3) {  //XY		
		cvx.MakeConvexHullXY();
		if (cvx.m_HullPtList.Search(vStart) < 0) {
			cvx.m_HullPtList.Insert(vStart);
			//	int a=0;
		}
		if (CutPln.n.Z > 0)
			cvx.ArrangeHullPtXY();
		else
			cvx.ArrangeHullPtYX();
	}
	else if (shortestDis == 2) {  //XZ
		cvx.MakeConvexHullXZ();
		if (cvx.m_HullPtList.Search(vStart) < 0)
			cvx.m_HullPtList.Insert(vStart);
		if (CutPln.n.Y > 0)
			cvx.ArrangeHullPtZX();
		else
			cvx.ArrangeHullPtXZ();
	}
	else if (shortestDis == 1) { //ZY
		cvx.MakeConvexHullYZ();
		if (cvx.m_HullPtList.Search(vStart) < 0)
			cvx.m_HullPtList.Insert(vStart);
		if (CutPln.n.X > 0)
			cvx.ArrangeHullPtYZ();
		else
			cvx.ArrangeHullPtZY();
	}


	if (ratioLength > 2.0f) {
		cvx.m_HullPtList.Delete(addPt);
	}

	//}

	//vStart가 시작점이 되도록 점 재정렬        
	CrossPtList.DeleteAll();

	startIndex = -1;

	startIndex = cvx.m_HullPtList.Search(vStart);

	if (startIndex > 0) {
		int num = cvx.m_HullPtList.m_nCount;

		for (int k = startIndex; k < num; k++)
		{
			GVector3f pt = cvx.m_HullPtList[k];

			CrossPtList.Insert(pt);
		}

		for (int k = 0; k < startIndex; k++)
		{
			GVector3f pt = cvx.m_HullPtList[k];

			CrossPtList.Insert(pt);
		}

		(*CvxPt) = CrossPtList;
		Result[resultIndex] = ::getSortedPtLen(CrossPtList);
	}

	else {
		(*CvxPt) = cvx.m_HullPtList;
		Result[resultIndex] = cvx.GetHullLength2();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//														END														 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//												Arrange Data													 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSizeKorea2020Doc::Analyze3DStructY() {
	if (!m_bOpened) return;
	if (m_bArrangedY) return;

	int Height = m_MaxY - m_MinY;
	int countY = ROUND(Height);
	if (countY % 2 == 1) countY++;
	//	int nCount_Y = countY+6;

	int num[30], j = 0;
	for (long i = 40000; i < 45000; i += 200)
	{
		// set the mininum value of y to zero. 
		//if( i > m_vNum ) continue;
		float y = m_Vertex[i].Y - m_MinY;

		int height = ROUND(y);

		num[j] = height;

		j++;
	}

	int rem04 = num[0] % 4;
	int rem14 = num[1] % 4;
	int rem24 = num[2] % 4;
	int rem34 = num[3] % 4;
	if (rem04 == 0 && rem14 == 0 && rem24 == 0 && rem34 == 0)  m_HInterval = 4;
	if (rem04 == 2 || rem14 == 2 || rem24 == 2 || rem34 == 2)  m_HInterval = 2;
}

void CSizeKorea2020Doc::Arrange3DDataY() {
	// TODO: Add your command handler code here
	if (!m_bOpened) return;
	if (m_bArrangedY) return;

	//	Analyze3DStructY();

		/*****************************************************************
		 * algorithm
		 * 1. determine the size of m_pAlignedPoints_Y
		 * 2. arrange the points by height
		 * 3. rotate the point croud to be front direction
		 *****************************************************************/

		 //CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

		 // 1. determine the size of m_pAlignedPoints_Y 
	m_Height = m_MaxY - m_MinY;
	int countY = ROUND(m_Height);
	if (countY % 2 == 1) countY++;
	m_nCount_Y = countY + 6;

	// 2. arrange the points by height 

	for (long i = 0; i < m_vNum; i++)
	{
		// set the mininum value of y to zero. 
		m_Vertex[i].Y -= m_MinY;
	}

	m_MaxY -= m_MinY;
	m_MinY = 0.0f;

	m_bArrangedY = true;
}

void CSizeKorea2020Doc::Arrange3DDataY_V() {
	if (!m_bOpened) return;
	//if ( m_bArrangedY) return;

	m_Height = m_MaxY - m_MinY;
	int countY = ROUND(m_Height);
	if (countY % 2 == 1) countY++;
	m_nCount_Y = countY + 2;
	m_pAlignedPointsV_Y = new GObList<GVector3f>[m_nCount_Y];

	GVector3f normY = GVector3f(0.0f, 1.0f, 0.0f);

	GPlane Hpln;
	int Height = (int)m_Height;


	for (int h = 0; h < Height; h += 2)
	{
		float hf = (float)h;
		Hpln.MakePlane(normY, GVector3f(0.0f, hf, 0.0f));
		float upH = hf + 6.0f;
		float downH = hf - 6.0f;

		GVector3f v;
		for (int i = 0; i < m_iNum; i += 3)
		{
			GVector3f v0, v1, v2;
			v0 = m_Vertex[m_FaceIdx[i]];
			v1 = m_Vertex[m_FaceIdx[i + 1]];
			v2 = m_Vertex[m_FaceIdx[i + 2]];

			//v01= v12= v20 = 0;
			// culling.
			if (v0.Y > upH && v1.Y > upH && v2.Y > upH)
				continue;
			if (v0.Y < downH && v1.Y < downH && v2.Y < downH)
				continue;

			if (Hpln.IsAbovePlane(v0) && Hpln.IsAbovePlane(v1) && Hpln.IsAbovePlane(v2))
				continue;
			if (Hpln.IsBelowPlane(v0) && Hpln.IsBelowPlane(v1) && Hpln.IsBelowPlane(v2))
				continue;


			//높이랑 같은 점이 2개 있으면?  1개 있으면?  continue
			if (v0.Y == hf && v1.Y == hf) {
				m_pAlignedPointsV_Y[h].Insert(v0);
				m_pAlignedPointsV_Y[h].Insert(v1);
				continue;
			}
			else if (v1.Y == hf && v2.Y == hf) {
				m_pAlignedPointsV_Y[h].Insert(v1);
				m_pAlignedPointsV_Y[h].Insert(v2);
				continue;
			}
			else if (v2.Y == hf && v0.Y == hf) {
				m_pAlignedPointsV_Y[h].Insert(v2);
				m_pAlignedPointsV_Y[h].Insert(v0);
				continue;
			}
			else if (v0.Y == hf) {
				m_pAlignedPointsV_Y[h].Insert(v0);  continue;
			}
			else if (v1.Y == hf) {
				m_pAlignedPointsV_Y[h].Insert(v1);  continue;
			}
			else if (v2.Y == hf) {
				m_pAlignedPointsV_Y[h].Insert(v2);  continue;
			}
			else {

				if (Hpln.GetSignedDistance(v0) * Hpln.GetSignedDistance(v1) <= 0)
				{
					GVector3f CrossPt = Hpln.GetProjectionPt(v0, v1 - v0);
					//if( m_pAlignedPointsV_Y[h].Search(CrossPt) ) 
					m_pAlignedPointsV_Y[h].Insert(CrossPt);
				}

				if (Hpln.GetSignedDistance(v1) * Hpln.GetSignedDistance(v2) <= 0)
				{
					GVector3f CrossPt = Hpln.GetProjectionPt(v0, v1 - v0);
					//if( m_pAlignedPointsV_Y[h].Search(CrossPt) ) 
					m_pAlignedPointsV_Y[h].Insert(CrossPt);
				}
				if (Hpln.GetSignedDistance(v2) * Hpln.GetSignedDistance(v0) <= 0)
				{
					GVector3f CrossPt = Hpln.GetProjectionPt(v0, v1 - v0);
					//if( m_pAlignedPointsV_Y[h].Search(CrossPt)  ) 
					m_pAlignedPointsV_Y[h].Insert(CrossPt);
				}

			} //else

		} //for

	} //for
}

void CSizeKorea2020Doc::Arrange3DDataX() {
	if (!m_bOpened) return;
	if (!m_bArrangedY) return;
	if (m_bArrangedX) return;


	m_nCount_X = ROUND((m_MaxX - m_MinX)) + 1;

	for (long i = 0; i < m_vNum; i++)
	{
		m_Vertex[i].X -= m_MinX;    // x좌표치 min값이 0이되도록 해줌
	}

	m_MaxX -= m_MinX;
	m_MinX = 0.0f;

	m_bArrangedX = true;
}

void CSizeKorea2020Doc::Arrange3DDataX_V() {
	if (!m_bOpened) return;
	if (!m_bArrangedY) return;
	if (m_bArrangedX) return;

	m_nCount_X = ROUND((m_MaxX - m_MinX)) + 1;
	m_pAlignedPointsV_X = new GObList<GVector3f>[m_nCount_X];

	GPlane pln;
	GVector3f norm = GVector3f(1.0f, 0.0f, 0.0f);

	for (int j = 0; j < m_nCount_X; j += 2)
	{
		float xf = (float)j;
		pln.MakePlane(norm, GVector3f(xf, 0.0f, 0.0f));
		float Rx = xf - 10.0f;
		float Lx = xf + 10.0f;

		for (long i = 0L; i < m_iNum; i += 3)
		{
			GVector3f v0, v1, v2;
			v0 = m_Vertex[m_FaceIdx[i]];
			v1 = m_Vertex[m_FaceIdx[i + 1]];
			v2 = m_Vertex[m_FaceIdx[i + 2]];


			if (v0.X < Rx && v1.X < Rx && v2.X < Rx)
				continue;
			if (v0.X > Lx && v1.X > Lx && v2.X > Lx)
				continue;

			if (pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2))
				continue;
			if (pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2))
				continue;

			if (v0.X == xf && v1.X == xf) {
				m_pAlignedPointsV_X[j].Insert(v0);
				m_pAlignedPointsV_X[j].Insert(v1);
				continue;
			}
			else if (v1.X == xf && v2.X == xf) {
				m_pAlignedPointsV_X[j].Insert(v1);
				m_pAlignedPointsV_X[j].Insert(v2);
				continue;
			}
			else if (v2.X == xf && v0.X == xf) {
				m_pAlignedPointsV_X[j].Insert(v2);
				m_pAlignedPointsV_X[j].Insert(v0);
				continue;
			}
			else if (v0.X == xf) {
				m_pAlignedPointsV_X[j].Insert(v0);  continue;
			}
			else if (v1.X == xf) {
				m_pAlignedPointsV_X[j].Insert(v1);  continue;
			}
			else if (v2.X == xf) {
				m_pAlignedPointsV_X[j].Insert(v2);  continue;
			}
			else {

				if (pln.GetSignedDistance(v0) * pln.GetSignedDistance(v1) < 0)  //만약 v0, v1의 x좌표가 같으면?
				{
					GVector3f CrossPt = pln.GetProjectionPt(v0, v1 - v0);
					m_pAlignedPointsV_X[j].Insert(CrossPt);
				}
				if (pln.GetSignedDistance(v1) * pln.GetSignedDistance(v2) < 0)
				{
					GVector3f CrossPt = pln.GetProjectionPt(v1, v2 - v1);
					m_pAlignedPointsV_X[j].Insert(CrossPt);
				}
				if (pln.GetSignedDistance(v2) * pln.GetSignedDistance(v0) < 0)
				{
					GVector3f CrossPt = pln.GetProjectionPt(v0, v2 - v0);
					m_pAlignedPointsV_X[j].Insert(CrossPt);
				}
			} //else
		} //for

	}

	m_bArrangedX = true;
}

/*!
*	\brief	모델의 정점의 (x, y) 최소 좌표를 (0, 0)으로 변경한다.
*	\note	왜일까?
*/
void CSizeKorea2020Doc::Arrange3DDataXY_to0() 
{
	if (!m_bOpened) 
		return;
	if (m_bArrangedY) 
		return;

	m_Height = m_MaxY - m_MinY;
	int countY = ROUND(m_Height);
	if (countY % 2 == 1) 
		countY++;
	m_nCount_Y = countY + 6;
	m_nCount_X = ROUND((m_MaxX - m_MinX)) + 1;

	for (int i = 0; i < m_vNum; i++)
	{
		// set the mininum value of y to zero. 
		m_Vertex[i].Y -= m_MinY;
		m_Vertex[i].X -= m_MinX;
		//m_Vertex[i].Z -= m_MinZ ; 
	}

	// mj::Move m_ScanMesh too
	if (m_ScanMesh != NULL) 
	{
		for (EgVertex *v : m_ScanMesh->m_pVerts)
		{
			v->m_Pos[0] -= m_MinX;
			v->m_Pos[1] -= m_MinY;
		}
		// mj::Update Bounding box
		m_ScanMesh->UpdateBndBox();
	}

	m_MaxY -= m_MinY;
	m_MinY = 0.0f;

	m_MaxX -= m_MinX;
	m_MinX = 0.0f;
}

void CSizeKorea2020Doc::Arrange3DDataXYZ_to0()
{
	// 스캔 모델이 없다면 리턴한다.
	if (!m_bOpened)
		return;

	// 스캔 모델의 최소좌표를 (0, 0, 0)으로 설정한다.
	for (EgVertex *v : m_ScanMesh->m_pVerts)
		v->m_Pos -= cast_vec3(m_ScanMesh->m_BndBox.m_MinPt);
	m_ScanMesh->UpdateBndBox();
	m_ScanMesh->UpdateVertexBuf();

	// 어디서 필요한가?
	m_Height = m_MaxY - m_MinY;
	int countY = ROUND(m_Height);
	if (countY % 2 == 1)
		countY++;
	m_nCount_Y = countY + 6;
	m_nCount_X = ROUND((m_MaxX - m_MinX)) + 1;	
}

void CSizeKorea2020Doc::Arrange3DDataXY_V() {
	if (!m_bOpened) return;

	m_nCount_X = ROUND((m_MaxX - m_MinX)) + 1;
	m_pAlignedPointsV_X = new GObList<GVector3f>[m_nCount_X];

	m_Height = m_MaxY - m_MinY;
	int countY = ROUND(m_Height);
	if (countY % 2 == 1) countY++;
	m_nCount_Y = countY + 2;
	m_pAlignedPointsV_Y = new GObList<GVector3f>[m_nCount_Y];

	//double minx, maxx, miny, maxy;

	for (int k = 0; k < m_iNum; k += 3)
	{
		GVector3f v0, v1, v2;
		v0 = m_Vertex[m_FaceIdx[k]];
		v1 = m_Vertex[m_FaceIdx[k + 1]];
		v2 = m_Vertex[m_FaceIdx[k + 2]];

		float newx, newy, newz;
		double mx, my, mz, cx, cy, cz;
		int start, end;
		GVector3f newp;

		//----------AlignX   
		//AlignX v0, v1	

		if (v0.X == v1.X) //두점의 x좌표가 같을때
		{
			int i = v0.X;
			float diff = v0.X - i;
			if (diff == 0 && i % 2 == 0) {
				m_pAlignedPointsV_X[i].Insert(v0);
				m_pAlignedPointsV_X[i].Insert(v1);
			}
		}
		else {    //두점의 x좌표가 같지 않을때
			my = (v0.Y - v1.Y) / (v0.X - v1.X);   //xy평면의 기울기
			cy = v0.Y - my * v0.X;
			mz = (v0.Z - v1.Z) / (v0.X - v1.X);   //zx평면의 기울기
			cz = v0.Z - mz * v0.X;

			if (v0.X < v1.X) {
				start = (int)v0.X;
				end = (int)v1.X;
			}
			else {
				start = (int)v1.X;
				end = (int)v0.X;
			}
			for (int i = start + 1; i < end + 1; i++)  //v0.X, v1.X중에서 작은것부터 큰 것까지에서 x가 정수일때의 y,z값을 구함
			{
				if (i % 2 == 0) {
					newy = my * (double)i + cy;
					newz = mz * (double)i + cz;
					newp = GVector3f((float)i, newy, newz);
					m_pAlignedPointsV_X[i].Insert(newp);
				}
			}
		}

		//AlignX v2, v0

		if (v2.X == v0.X) //두점의 x좌표가 같을때
		{
			int i = v2.X;
			if (v2.X - i == 0 && i % 2 == 0) {
				m_pAlignedPointsV_X[i].Insert(v2);
				m_pAlignedPointsV_X[i].Insert(v0);
			}
		}
		else {
			my = (v2.Y - v0.Y) / (v2.X - v0.X);
			cy = v2.Y - my * v2.X;
			mz = (v2.Z - v0.Z) / (v2.X - v0.X);
			cz = v2.Z - mz * v2.X;

			if (v2.X < v0.X) {
				start = (int)v2.X;
				end = (int)v0.X;
			}
			else {
				start = (int)v0.X;
				end = (int)v2.X;
			}
			for (int i = start + 1; i < end + 1; i++)
			{
				if (i % 2 == 0) {
					newy = my * (double)i + cy;
					newz = mz * (double)i + cz;
					newp = GVector3f((float)i, newy, newz);
					m_pAlignedPointsV_X[i].Insert(newp);
				}
			}
		}

		//----------AlignY   
		//AlignY v0, v1		
		if (v0.Y == v1.Y) //두점의 y좌표가 같을때
		{
			int i = v0.Y;
			if (v0.Y - i == 0 && i % 2 == 0) {
				m_pAlignedPointsV_Y[i].Insert(v0);
				m_pAlignedPointsV_Y[i].Insert(v1);
			}

		}
		else {
			mx = (v0.X - v1.X) / (v0.Y - v1.Y);
			cx = v0.X - mx * v0.Y;
			mz = (v0.Z - v1.Z) / (v0.Y - v1.Y);
			cz = v0.Z - mz * v0.Y;
			if (v0.Y < v1.Y) {
				start = (int)v0.Y;
				end = (int)v1.Y;
			}
			else {
				start = (int)v1.Y;
				end = (int)v0.Y;
			}
			for (int i = start + 1; i < end + 1; i++)
			{
				if (i % 2 == 0) {
					newx = mx * (double)i + cx;
					newz = mz * (double)i + cz;
					newp = GVector3f(newx, (float)i, newz);
					m_pAlignedPointsV_Y[i].Insert(newp);
				}
			}
		}

		//AlignY v2, v0		
		if (v2.Y == v0.Y) //두점의 y좌표가 같을때
		{
			int i = v2.Y;
			if (v2.Y - i == 0 && i % 2 == 0) {
				m_pAlignedPointsV_Y[i].Insert(v2);
				m_pAlignedPointsV_Y[i].Insert(v0);
			}
		}
		else {
			mx = (v2.X - v0.X) / (v2.Y - v0.Y);
			cx = v2.X - mx * v2.Y;
			mz = (v2.Z - v0.Z) / (v2.Y - v0.Y);
			cz = v2.Z - mz * v2.Y;
			if (v2.Y < v0.Y) {
				start = (int)v2.Y;
				end = (int)v0.Y;
			}
			else {
				start = (int)v0.Y;
				end = (int)v2.Y;
			}
			for (int i = start + 1; i < end + 1; i++)
			{
				if (i % 2 == 0) {
					newx = mx * (double)i + cx;
					newz = mz * (double)i + cz;
					newp = GVector3f(newx, (float)i, newz);
					m_pAlignedPointsV_Y[i].Insert(newp);
				}
			}
		}

	}
}

void CSizeKorea2020Doc::OnArrange3DData() {
	Arrange3DDataXY_V();
	m_bArrangedXY = true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//														END														 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void CSizeKorea2020Doc::PointsToLine(GObNode<GVector3f>* pNode, GObList<GVector3f> *CvxPt, int plane) {
	float minx, maxx, miny, maxy, minz, maxz;
	minx = miny = minz = 1000000.0f;
	maxx = maxy = maxz = -1000000.0f;
	GVector3f minxP, maxxP, minyP, maxyP, minzP, maxzP;

	GObNode<GVector3f> *pNode1 = pNode;
	GObList<GVector3f> CrvPtList;

	while (pNode)
	{
		GVector3f v = pNode->data;

		if (v.X < minxP.X)
			minxP = v;
		if (v.Y < minyP.Y)
			minyP = v;
		if (v.Z < minzP.Z)
			minzP = v;

		if (v.X > maxxP.X)
			maxxP = v;
		if (v.Y > maxyP.Y)
			maxyP = v;
		if (v.Z > maxzP.Z)
			maxzP = v;

		pNode = pNode->m_pRight;
	}

	//	int min, max, Row, Column, Rstart, Cstart;

		/*switch(plane) //0=XY, 1=YZ, 2=ZX
		{
		case: 0
			  Rstart = ROUND(minx);
			  Cstart = ROUND(miny);
			  Row = ROUND(maxx) -ROUND(minx);
			  Column = ROUND(maxy) -ROUND(miny);
		case: 1
			  Column = ROUND(maxz) -ROUND(minz);
			  Row = ROUND(maxy) -ROUND(miny);
			  Rstart = ROUND(miny);
			  Cstart = ROUND(minz);
		case: 2
			  Column = ROUND(maxx) -ROUND(minx);
			  Row = ROUND(maxz) -ROUND(minz);
			  Rstart = ROUND(minz);
			  Cstart = ROUND(minx);
		default;
		}*/

	GVector3f StartP;
	switch (plane) //0=XY, 1=YZ, 2=ZX
	{
	case 0: 	    StartP = maxyP;  break;
	case 1:			StartP = maxyP;  break;
	case 2:			StartP = maxxP;  break;
	}


	int i, j, m, n;
	i = ROUND(StartP.Y);
	j = ROUND(StartP.Z);
	CrvPtList.Insert(StartP);
	int exist, exist1, exist2, exist3, exist4, exist5, exist6, exist7, exist8;
	exist = exist1 = exist2 = exist3 = exist4 = exist5 = exist6 = exist7 = exist8 = 0;

	while (pNode1)
	{
		GVector3f v = pNode1->data;

		m = ROUND(v.X);
		n = ROUND(v.Z);

		if (m == i - 1 && n == j - 1)  exist++; exist1 = 1;
		if (m == i - 1 && n == j)    exist++; exist2 = 1;
		if (m == i - 1 && n == j + 1)  exist++; exist3 = 1;
		if (m == i && n == j + 1)  exist++; exist4 = 1;
		if (m == i + 1 && n == j + 1)  exist++; exist5 = 1;
		if (m == i + 1 && n == j)    exist++; exist6 = 1;
		if (m == i + 1 && n == j - 1)  exist++; exist7 = 1;
		if (m == i && n == j - 1)  exist++; exist8 = 1;

		if (exist) {

			int e = CrvPtList.Search(v);
			if (!e)
			{
				CrvPtList.Insert(v);
				i = m;
				j = n;
			}
		}
		else {
		}

		//
		exist = 0;
		pNode1 = pNode1->m_pRight;

	}
}

void CSizeKorea2020Doc::TransformModel(float rx, float ry, float rz, float sx, float sy, float sz) {
	float tx, ty, tz;
	float sin_rx, sin_ry, sin_rz;
	float cos_rx, cos_ry, cos_rz;
	float mtrx[3][3];

	rx = rx * 3.141592 / 180.0f;
	ry = ry * 3.141592 / 180.0f;
	rz = rz * 3.141592 / 180.0f;
	sin_rx = sin(rx); sin_ry = sin(ry); sin_rz = sin(rz);
	cos_rx = cos(rx); cos_ry = cos(ry); cos_rz = cos(rz);
	tx = 0.0f;
	ty = 0.0f;
	tz = 0.0f;

	mtrx[0][0] = sx * (cos_ry*cos_rz);
	mtrx[0][1] = -cos_ry * sin_rz;
	mtrx[0][2] = sin_ry;
	mtrx[1][0] = sin_rx * sin_ry*cos_rz + cos_rx * sin_rz;
	mtrx[1][1] = sy * (-sin_rx * sin_ry*sin_rz + cos_rx * cos_rz);
	mtrx[1][2] = -sin_rx * cos_ry;
	mtrx[2][0] = -cos_rx * sin_ry*cos_rz + sin_rx * sin_rz;
	mtrx[2][1] = cos_rx * sin_ry*sin_rz + sin_rx * cos_rz;
	mtrx[2][2] = sz * (cos_rx*cos_ry);

	float min_y = 10000.0, max_y = -10000.0;
	float min_x = 10000.0, max_x = -10000.0;
	float min_z = 10000.0, max_z = -10000.0;

	for (int i = 0; i < m_vNum; i++)
	{
		GVector3f pt = m_Vertex[i];
		m_Vertex[i][0] = (mtrx[0][0] * pt[0] + mtrx[0][1] * pt[1] + mtrx[0][2] * pt[2]) + tx;
		m_Vertex[i][1] = (mtrx[1][0] * pt[0] + mtrx[1][1] * pt[1] + mtrx[1][2] * pt[2]) + ty;
		m_Vertex[i][2] = (mtrx[2][0] * pt[0] + mtrx[2][1] * pt[1] + mtrx[2][2] * pt[2]) + tz;

		min_x = MIN(min_x, m_Vertex[i][0]);
		max_x = MAX(max_x, m_Vertex[i][0]);

		min_y = MIN(min_y, m_Vertex[i][1]);
		max_y = MAX(max_y, m_Vertex[i][1]);

		min_z = MIN(min_z, m_Vertex[i][2]);
		max_z = MAX(max_z, m_Vertex[i][2]);
	}

	m_MaxY = max_y;
	m_MinY = min_y;
	m_MaxX = max_x;
	m_MinX = min_x;
	m_MaxZ = max_z;
	m_MinZ = min_z;
	m_MaxYOrigin = max_y;
	m_MinYOrigin = min_y;
	m_MaxXOrigin = max_x;
	m_MinXOrigin = min_x;
	m_MaxZOrigin = max_z;
	m_MinZOrigin = min_z;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//												MENU HANDLER													 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
*	\brief	불러온 obj 파일에서 모델에 대한 정보를 추출하는 함수
*	\note	마지막 수정일: 2020-08-18
*
*	\return	성공하면 true, 아니면 false를 반환한다.
*/
bool CSizeKorea2020Doc::ImportObj() 
{
	// 정점의 수와 삼각형을 구성하는 인덱스 수를 계산한다.
	m_vNum = m_ScanMesh->m_pVerts.size();
	m_iNum = m_ScanMesh->m_pFaces.size() * 3;

	// 모델의 정점, 칼라, 법선, 인덱스 배열을 동적으로 할당한다.
	m_Vertex = new GVector3f[m_vNum];
	m_Color = new GVector3f[m_vNum];
	m_Normal = new GVector3f[m_vNum];
	m_FaceIdx = new int[m_iNum];

	// 모델의 정점, 색상, 법선 배열을 초기화한다.
	for (int i = 0; i < m_vNum; i++) 
	{
		m_Vertex[i].X = m_ScanMesh->m_pVerts[i]->m_Pos[0];
		m_Vertex[i].Y = m_ScanMesh->m_pVerts[i]->m_Pos[1];
		m_Vertex[i].Z = m_ScanMesh->m_pVerts[i]->m_Pos[2];
		
		m_Color[i].X = 0.3;
		m_Color[i].Y = 0.3;
		m_Color[i].Z = 0.3;

		m_Normal[i].X = m_ScanMesh->m_pNormals[i]->m_Dir[0];
		m_Normal[i].Y = m_ScanMesh->m_pNormals[i]->m_Dir[1];
		m_Normal[i].Z = m_ScanMesh->m_pNormals[i]->m_Dir[2];
	}

	// 삼각형 인덱스 배열을 초기화한다.
	for (int i = 0, k = 0; i < m_ScanMesh->GetNumFace(); i++, k += 3)
	{
		int *idx = m_ScanMesh->m_pFaces[i]->GetVertexIdxs();
		m_FaceIdx[k] = idx[0];
		m_FaceIdx[k + 1] = idx[1];
		m_FaceIdx[k + 2] = idx[2];
	}

	// 모델의 경계 상자를 설정한다.
	m_MinX = m_ScanMesh->m_BndBox.m_MinPt[0];
	m_MinY = m_ScanMesh->m_BndBox.m_MinPt[1];
	m_MinZ = m_ScanMesh->m_BndBox.m_MinPt[2];
	m_MaxX = m_ScanMesh->m_BndBox.m_MaxPt[0];
	m_MaxY = m_ScanMesh->m_BndBox.m_MaxPt[1];
	m_MaxZ = m_ScanMesh->m_BndBox.m_MaxPt[2];
	return true;
}

void CSizeKorea2020Doc::EgMesh2RawData()
{
	// 정점의 수와 삼각형을 구성하는 인덱스 수를 계산한다.
	m_vNum = m_ScanMesh->m_pVerts.size();
	m_iNum = m_ScanMesh->m_pFaces.size() * 3;

	// 모델의 정점, 칼라, 법선, 인덱스 배열을 동적으로 할당한다.
	m_Vertex = new GVector3f[m_vNum];
	m_Color = new GVector3f[m_vNum];
	m_Normal = new GVector3f[m_vNum];
	m_FaceIdx = new int[m_iNum];

	// 모델의 정점, 색상, 법선 배열을 초기화한다.
	for (int i = 0; i < m_vNum; i++)
	{
		m_Vertex[i].X = m_ScanMesh->m_pVerts[i]->m_Pos[0];
		m_Vertex[i].Y = m_ScanMesh->m_pVerts[i]->m_Pos[1];
		m_Vertex[i].Z = m_ScanMesh->m_pVerts[i]->m_Pos[2];

		m_Color[i].X = 0.3;
		m_Color[i].Y = 0.3;
		m_Color[i].Z = 0.3;

		m_Normal[i].X = m_ScanMesh->m_pNormals[i]->m_Dir[0];
		m_Normal[i].Y = m_ScanMesh->m_pNormals[i]->m_Dir[1];
		m_Normal[i].Z = m_ScanMesh->m_pNormals[i]->m_Dir[2];
	}

	// 삼각형 인덱스 배열을 초기화한다.
	for (int i = 0, k = 0; i < m_ScanMesh->GetNumFace(); i++, k += 3)
	{
		int *idx = m_ScanMesh->m_pFaces[i]->GetVertexIdxs();
		m_FaceIdx[k] = idx[0];
		m_FaceIdx[k + 1] = idx[1];
		m_FaceIdx[k + 2] = idx[2];
	}

	// 모델의 경계 상자를 설정한다.
	m_MinX = m_ScanMesh->m_BndBox.m_MinPt[0];
	m_MinY = m_ScanMesh->m_BndBox.m_MinPt[1];
	m_MinZ = m_ScanMesh->m_BndBox.m_MinPt[2];
	m_MaxX = m_ScanMesh->m_BndBox.m_MaxPt[0];
	m_MaxY = m_ScanMesh->m_BndBox.m_MaxPt[1];
	m_MaxZ = m_ScanMesh->m_BndBox.m_MaxPt[2];
	m_Width = m_MaxX - m_MinX;
	m_Height = m_MaxY - m_MinY;
	m_Depth = m_MaxZ - m_MinZ;
}

// mj::멤버 변수들을 이용해 EgMesh에 할당한다
bool CSizeKorea2020Doc::Copy2EgMesh() {
	_cprintf("Copying to m_ScanMesh...\n");

	// Build up m_ScanMesh
	m_ScanMesh = new EgMesh((LPCTSTR)m_FileName);

	_cprintf("Copying to %d vertices... ", m_vNum);
	for (int i = 0; i < m_vNum; i++)
	{
		m_ScanMesh->AddVertex(new EgVertex(m_Vertex[i].X, m_Vertex[i].Y, m_Vertex[i].Z));
		m_ScanMesh->AddNormal(new EgNormal(m_Normal[i].X, m_Normal[i].Y, m_Normal[i].Z));
	}
	_cprintf("Final vert num : %d\n", m_ScanMesh->m_pVerts.size());


	_cprintf("Copying to %d faces... \n", m_iNum / 3);
	for (int i = 0; i < m_iNum; i += 3) {

		EgVertex *V[3] = { 0 };
		V[0] = m_ScanMesh->m_pVerts[m_FaceIdx[i]];
		V[1] = m_ScanMesh->m_pVerts[m_FaceIdx[i + 1]];
		V[2] = m_ScanMesh->m_pVerts[m_FaceIdx[i + 2]];

		EgTexel *T[3] = { 0 };
		EgNormal *N[3] = { 0 };

		std::string grp;
		EgFace *f = new EgFace(V[0], V[1], V[2], T[0], T[1], T[2], N[0], N[1], N[2], NULL, grp);
		m_ScanMesh->AddFace(f);
	}
	_cprintf("Final face num : %d\n", m_ScanMesh->m_pFaces.size());

	_cprintf("Setting up bounding box...\n");
	m_ScanMesh->m_BndBox = EgBox(m_MinX, m_MinY, m_MinZ, m_MaxX, m_MaxY, m_MaxZ);


	// 하프에지 자료구조를 초기화 해야 함.
	m_ScanMesh->InitEdgeMate();

	_cprintf("End copying m_ScanMesh");
	return true;
}

void CSizeKorea2020Doc::InitPoseInfoDialog()
{
	// 자세3 이라면
	if (m_ScanPose == 3 || m_ScanPose == 5)
	{
		GVector3f MaxZPt(0.0, 0.0, -INFINITY);
		for (int i = 0; i < m_vNum; ++i)
		{
			if (m_Vertex[i].Z > MaxZPt.Z)
				MaxZPt = m_Vertex[i];
		}
		// 자세3(구) 라면 자세5로 변경한다.
		if (MaxZPt.X > (m_MinX + m_MaxX) * 0.5)
			m_ScanPose = 5;
		else
			m_ScanPose = 3;
	}

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->ShowPane(&pFrame->m_Pose1InfoDlg, FALSE, FALSE, TRUE);
	pFrame->ShowPane(&pFrame->m_Pose2InfoDlg, FALSE, FALSE, TRUE);
	pFrame->ShowPane(&pFrame->m_Pose3InfoDlg, FALSE, FALSE, TRUE);
	pFrame->ShowPane(&pFrame->m_Pose4InfoDlg, FALSE, FALSE, TRUE);
	pFrame->ShowPane(&pFrame->m_Pose5InfoDlg, FALSE, FALSE, TRUE);
	pFrame->m_EditModelDlg.UpdateDlgData();
	switch (m_ScanPose)
	{
	case 1:
		pFrame->ShowPane(&pFrame->m_Pose1InfoDlg, TRUE, FALSE, TRUE);
		theSizer.Init(1, m_ScanMesh);
		break;

	case 2:
		pFrame->ShowPane(&pFrame->m_Pose2InfoDlg, TRUE, FALSE, TRUE);
		theSizer.Init(2, m_ScanMesh);
		break;

	case 3:
		pFrame->ShowPane(&pFrame->m_Pose3InfoDlg, TRUE, FALSE, TRUE);
		theSizer.Init(3, m_ScanMesh);
		break;

	case 4:
		pFrame->ShowPane(&pFrame->m_Pose4InfoDlg, TRUE, FALSE, TRUE);
		theSizer.Init(4, m_ScanMesh);
		break;

	case 5:
		pFrame->ShowPane(&pFrame->m_Pose5InfoDlg, TRUE, FALSE, TRUE);
		theSizer.Init(5, m_ScanMesh);
		break;
	}
	// mj::모델 불러오기 화면에서 편집창 숨기기
	// pFrame->ShowPane(&pFrame->m_EditModelDlg, TRUE, FALSE, TRUE);
	pFrame->RecalcLayout();
}

void CSizeKorea2020Doc::OnFileOpen()
{
	// 파일 열기 대화상자를 출력한다.
	//LPCTSTR filter = _T("obj file(ascii)(*.obj)|*.obj|ply file(Ascii Rapid Form Only)(*.ply)|*.ply|txt file(*.txt)|*.txt|iv file(ascii)(*.iv)|*.iv|mesh file(*.msh)|*.msh|vrml file(*.wrl)|*.wrl|wrb file(*.wrb)|*.wrb|All Files (*.*)|*.*||");
	LPCTSTR filter = _T("obj file(ascii)(*.obj)|*.obj|ply file(Ascii Rapid Form Only)(*.ply)|*.ply|iv file(ascii)(*.iv)|*.iv|All Files (*.*)|*.*||");
	CFileDialog FileDlg(true, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (FileDlg.DoModal() != IDOK)
		return;

	// 기존에 오픈된 모델이 있다면 변수를 초기화한다.
	if (m_bOpened)
		InitVariable();

	// 열기 대화 상자로부터 파일 이름과 확장자를 구한다.
	m_FileName = CT2A(FileDlg.GetPathName());
	int idxFilter = FileDlg.m_ofn.nFilterIndex;

	// 선택한 파일을 불러온다.
	BeginWaitCursor();
	CSizeKorea2020View *pView = GetView();
	switch (idxFilter)
	{
	case 1: // *.obj 파일
		// 텍스처 로딩을 위해 OpenGL 렌더링 모드로 진입한다.
		wglMakeCurrent(pView->m_hDC, pView->m_hRC);
		{
			if (m_ScanMesh = SK_Import_Obj(m_FileName))
				m_bOpened = true;
			else
				m_bOpened = false;
		}
		wglMakeCurrent(NULL, NULL);
		break;

	case 2: // *.ply 파일
		if (m_ScanMesh = SK_Import_Ply(m_FileName))
			m_bOpened = true;
		else
			m_bOpened = false;
		//m_bOpened = ::read_ply_asc(m_FileName, &m_Vertex, &m_Color, &m_Normal, &m_FaceIdx, &m_vNum, &m_iNum, &m_MaxY, &m_MinY, &m_MaxX, &m_MinX);
		break;

	case 4: // *.iv 파일
		if (m_ScanMesh = SK_Import_Iv(m_FileName))
			m_bOpened = true;
		else
			m_bOpened = false;
		//m_bOpened = ::read_iv_asc(m_FileName, &m_Vertex, &m_Color, &m_Normal, &m_FaceIdx, &m_vNum, &m_iNum, &m_MaxY, &m_MinY, &m_MaxX, &m_MinX);
		break;

	case 5: // *.msh 파일
		//m_ScanMesh = SK_Import_Msh(m_FileName);
		//if (m_ScanMesh != NULL)
		//	m_bOpened = true;
		//else
		//	m_bOpened = false;
		m_bOpened = ::read_msh(m_FileName, &m_Vertex, &m_Color, &m_Normal, &m_FaceIdx, &m_vNum, &m_iNum);
		break;

	case 6: // *.vrml97 파일
		//m_ScanMesh = SK_Import_Vrml97(m_FileName);
		//if (m_ScanMesh != NULL)
		//	m_bOpened = true;
		//else
		//	m_bOpened = false;
		m_bOpened = ::read_vrml(m_FileName, &m_Vertex, &m_Color, &m_Normal, &m_FaceIdx, &m_vNum, &m_iNum, &m_MaxY, &m_MinY, &m_MaxX, &m_MinX);
		break;

	case 7: // *.wrb 파일
		//m_ScanMesh = SK_Import_Wrb(m_FileName);
		//if (m_ScanMesh != NULL)
		//	m_bOpened = true;
		//else
		//	m_bOpened = false;
		m_bOpened = ::read_wrb(m_FileName, &m_Vertex, &m_Color, &m_Normal, &m_FaceIdx, &m_vNum, &m_iNum, &m_MaxY, &m_MinY, &m_MaxX, &m_MinX);
		break;
	}
	EndWaitCursor();

	// InitVariable()과 중복....
	{
		//RefAngle[0] = 0.34906f;    //목둘레
		//RefAngle[1] = 1.48353f;    //팔 몸통분리 오른 85.0f,
		//RefAngle[2] = -1.48353f;   //팔 몸통분리 왼  85.0f,
		//RefAngle[3] = 1.48353f;
		//RefAngle[4] = -1.48353f;
		//RefAngle[5] = 1.54f;		//1.48353f;  
		//RefAngle[6] = -1.54f;		//-1.48353f;
	}

	// 모델 정점의 최소 좌표를 (0, 0, 0)으로 변환한다.
	Arrange3DDataXYZ_to0();	

	// m_ScanMesh의 데이터를 m_Vertex, m_Color, m_Nomal...등에 복사한다.
	EgMesh2RawData();

	// 적당한 카메라 위치를 설정한다.
	GetView()->ReCalcViewParameter();

	// 자세에 따라 적절한 랜드마크 대화상자를 표시한다.
	CSelectPoseDialog dlg;
	if (dlg.DoModal() == IDOK)
		m_ScanPose = dlg.m_ScanPose + 1;
	else
		m_ScanPose = 0;
	InitPoseInfoDialog();

	UpdateAllViews(NULL);
}


void CSizeKorea2020Doc::OnFileSave()
{
	// 파일의 확장자 필터와 인덱스를 정의하고,
	//LPCTSTR filter = _T("obj file(ascii)(*.obj)|*.obj|ply file(Ascii Rapid Form Only)(*.ply)|*.ply|txt file(*.txt)|*.txt|iv file(ascii)(*.iv)|*.iv|mesh file(*.msh)|*.msh|vrml file(*.wrl)|*.wrl|wrb file(*.wrb)|*.wrb|All Files (*.*)|*.*||");
	LPCTSTR filter = _T("obj file(ascii)(*.obj)|*.obj|ply file(Ascii Rapid Form Only)(*.ply)|*.ply||");

	// 파일 저장 대화상자를 출력하고,
	CFileDialog FileDlg(false, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (FileDlg.DoModal() != IDOK)
		return;

	// 모델을 저장한다.
	BeginWaitCursor();
	CString FileName = FileDlg.GetPathName();

	int idxFilter = FileDlg.m_ofn.nFilterIndex;
	switch (idxFilter)
	{
	case 1: // *.obj 파일
		FileName = FileDlg.GetPathName(); // get file path.
		if (FileName.Find(".obj") == -1)
			FileName += ".obj";
		::export_obj(m_ScanMesh, FileName);
		break;

	case 2: // *.ply 파일
		FileName = FileDlg.GetPathName(); // get file path.
		if (FileName.Find(".ply") == -1)
			FileName += ".ply";
		write_ply_asc(FileName, m_Vertex, m_Color, m_FaceIdx, m_vNum, m_iNum);
		break;

	case 4: // *.iv 파일
		break;

	case 5: // *.msh 파일
		break;

	case 6: // *.vrml97 파일
		break;

	case 7: // *.wrb 파일
		break;
	}
	EndWaitCursor();
}

void CSizeKorea2020Doc::OnModelLandmarkSave()
{
	// 모델 저장하기
	OnFileSave();

	// 랜드마크 저장하기
	OnExportLandmark();
}

void CSizeKorea2020Doc::OnFindLandmark()
{
	// TODO: Add your command handler code here
	if (!m_bArrangedXY)
		OnArrange3DData();

	FindAllLandmarks();

	BoundingBox();
	GetView()->ReCalcViewParameter();
	UpdateAllViews(NULL);
}

void CSizeKorea2020Doc::OnMeasure()
{
	// TODO: Add your command handler code here
	BeginWaitCursor();

	switch (m_ScanPose)
	{
	case 1:
		OnMeasurePose1();
		break;

	case 2:
		OnMeasurePose2();
		break;

	case 3:
		OnMeasurePose3();
		break;

	case 4:
		OnMeasurePose4();
		break;

	case 5:
		OnMeasurePose5();
		break;

	default:
		return;
	}
	EndWaitCursor();
	m_bSizeResult = true;
	GetView()->Invalidate(false);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													  END														 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSizeKorea2020Doc::OnImportLandmark()
{
	CString strFileName;
	LPCTSTR filter = "Landmark file(ascii)(*.lnd)|*.lnd||";
	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (FileDlg.DoModal() != IDOK)
		return;

	strFileName = FileDlg.GetPathName(); // get file path.
	if (strFileName.Find(".lnd") == -1)
		strFileName += ".lnd";

	FILE *fp = NULL;
	fopen_s(&fp, strFileName, "r");

	if (m_ScanPose == 1)
	{
		char buffer[1024];
		double x, y, z;
		// 측정점과 보조점을 분리해서 불러온다
		int idxNum = 0;
		int helpIdx = 0;
		while (fscanf_s(fp, "%s%lf%lf%lf", buffer, 1024, &x, &y, &z) != EOF)
		{
			EgPos p(x, y, z);

			// 측정점 import
			if (idxNum < LANDMARK_NUM_POSE1) {
				int idx = theSizer.GetLandmarkIdx(std::string(buffer));
				theSizer.m_Landmarks[idx] = FindClosestVert(theSizer.m_pMesh, p);
			}
			// 보조점 import
			else {
				m_LandmarkHelpPose1[helpIdx] = FindClosestVert(theSizer.m_pMesh, p);
				helpIdx++;
			}

			idxNum++;
		}
	}
	else if (m_ScanPose == 2)
	{
		char buffer[1024];
		double x, y, z;
		int idxNum = 0;
		int preNum = 0;
		int helpIdx = 0;
		while (fscanf_s(fp, "%s%lf%lf%lf", buffer, 1024, &x, &y, &z) != EOF)
		{
			EgPos p(x, y, z);

			if (idxNum < NEW_LANDMARK_NUM_POSE2) {
				int idx = theSizer.GetLandmarkIdx(std::string(buffer));
				theSizer.m_Landmarks[idx] = FindClosestVert(theSizer.m_pMesh, p);
				m_nLandmarkPose2[idx] = theSizer.m_Landmarks[idx];
			}
			else if (idxNum < NEW_LANDMARK_NUM_POSE2 + LANDMARK_NUM_POSE2) {
				m_LandMarkPose2[preNum] = GVector3f(x, y, z);
				preNum++;
			}
			else {
				m_LandMarkHelp[helpIdx] = GLandMark(x, y, z);
				helpIdx++;
			}
			idxNum++;
		}


		// m_LandMarkPose2에 할당
		m_LandMarkPose2[1] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_TOPHEAD]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_TOPHEAD]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_TOPHEAD]->m_Pos[2]);
		m_LandMarkPose2[2] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_NECK_THYROID]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_NECK_THYROID]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_NECK_THYROID]->m_Pos[2]);
		m_LandMarkPose2[3] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_NECK_FRONT]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_NECK_FRONT]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_NECK_FRONT]->m_Pos[2]);
		m_LandMarkPose2[4] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_NECK_SIDE_R]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_NECK_SIDE_R]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_NECK_SIDE_R]->m_Pos[2]);
		m_LandMarkPose2[5] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_NECK_SIDE_L]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_NECK_SIDE_L]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_NECK_SIDE_L]->m_Pos[2]);

		m_LandMarkPose2[6] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_NECK_BACK]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_NECK_BACK]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_NECK_BACK]->m_Pos[2]);
		m_LandMarkPose2[7] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_SHOULDERSIDE_R]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_SHOULDERSIDE_R]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_SHOULDERSIDE_R]->m_Pos[2]);
		m_LandMarkPose2[8] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_SHOULDERSIDE_L]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_SHOULDERSIDE_L]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_SHOULDERSIDE_L]->m_Pos[2]);
		m_LandMarkPose2[44] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_ARMPITFOLD_FRONT_R]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_ARMPITFOLD_FRONT_R]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_ARMPITFOLD_FRONT_R]->m_Pos[2]);
		m_LandMarkPose2[45] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_ARMPITFOLD_BACK_R]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_ARMPITFOLD_BACK_R]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_ARMPITFOLD_BACK_R]->m_Pos[2]);
		m_LandMarkPose2[9] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_ARMPIT_R]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_ARMPIT_R]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_ARMPIT_R]->m_Pos[2]);
		m_LandMarkPose2[10] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_ARMPIT_L]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_ARMPIT_L]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_ARMPIT_L]->m_Pos[2]);
		m_LandMarkPose2[16] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_BP_R]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_BP_R]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_BP_R]->m_Pos[2]);
		m_LandMarkPose2[17] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_BP_L]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_BP_L]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_BP_L]->m_Pos[2]);
		m_LandMarkPose2[18] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_UNDERBUST]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_UNDERBUST]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_UNDERBUST]->m_Pos[2]);
		m_LandMarkPose2[19] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_WAIST]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_WAIST]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_WAIST]->m_Pos[2]);
		m_LandMarkPose2[20] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_WAIST_L]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_WAIST_L]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_WAIST_L]->m_Pos[2]);
		m_LandMarkPose2[21] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_WAIST_FRONT]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_WAIST_FRONT]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_WAIST_FRONT]->m_Pos[2]);
		m_LandMarkPose2[22] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_WAIST_BACK]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_WAIST_BACK]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_WAIST_BACK]->m_Pos[2]);
		m_LandMarkPose2[23] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_STOMACH_EXTRUDE]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_STOMACH_EXTRUDE]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_STOMACH_EXTRUDE]->m_Pos[2]);
		m_LandMarkPose2[35] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_NAVEL]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_NAVEL]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_NAVEL]->m_Pos[2]);
		m_LandMarkPose2[24] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_HIP_EXTRUDE]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_HIP_EXTRUDE]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_HIP_EXTRUDE]->m_Pos[2]);
		m_LandMarkPose2[25] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_CROTCH]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_CROTCH]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_CROTCH]->m_Pos[2]);
		m_LandMarkPose2[26] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_UNDERHIP]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_UNDERHIP]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_UNDERHIP]->m_Pos[2]);
		m_LandMarkPose2[27] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_KNEE_MID]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_KNEE_MID]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_KNEE_MID]->m_Pos[2]);
		m_LandMarkPose2[28] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_CALF_EXTRUDE]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_CALF_EXTRUDE]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_CALF_EXTRUDE]->m_Pos[2]);
		m_LandMarkPose2[32] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_WRIST]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_WRIST]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_WRIST]->m_Pos[2]);
		m_LandMarkPose2[33] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_WRIST_OUTSIDE]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_WRIST_OUTSIDE]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_WRIST_OUTSIDE]->m_Pos[2]);
		m_LandMarkPose2[34] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_HAND]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_HAND]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_HAND]->m_Pos[2]);
		m_LandMarkPose2[31] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_ELBOW]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_ELBOW]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_ELBOW]->m_Pos[2]);
		m_LandMarkPose2[36] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_SHOULDER_MID]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_SHOULDER_MID]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_SHOULDER_MID]->m_Pos[2]);
		m_LandMarkPose2[29] = GVector3f(theSizer.m_Landmarks[POSE2_LANDMARK_UNDERCALF]->m_Pos[0], theSizer.m_Landmarks[POSE2_LANDMARK_UNDERCALF]->m_Pos[1], theSizer.m_Landmarks[POSE2_LANDMARK_UNDERCALF]->m_Pos[2]);
	}
	else if (m_ScanPose == 3)
	{
		char buffer[1024];
		double x, y, z;
		while (fscanf_s(fp, "%s%lf%lf%lf", buffer, 1024, &x, &y, &z) != EOF)
		{
			EgPos p(x, y, z);
			int idx = theSizer.GetLandmarkIdx(std::string(buffer));
			theSizer.m_Landmarks[idx] = FindClosestVert(theSizer.m_pMesh, p);
		}
	}
	else if (m_ScanPose == 4)
	{
		char buffer[1024];
		double x, y, z;
		while (fscanf_s(fp, "%s%lf%lf%lf", buffer, 1024, &x, &y, &z) != EOF)
		{
			EgPos p(x, y, z);
			int idx = theSizer.GetLandmarkIdx(std::string(buffer));
			theSizer.m_Landmarks[idx] = FindClosestVert(theSizer.m_pMesh, p);
		}
	}
	else if (m_ScanPose == 5)
	{
		char buffer[1024];
		double x, y, z;
		while (fscanf_s(fp, "%s%lf%lf%lf", buffer, 1024, &x, &y, &z) != EOF)
		{
			EgPos p(x, y, z);
			int idx = theSizer.GetLandmarkIdx(std::string(buffer));
			theSizer.m_Landmarks[idx] = FindClosestVert(theSizer.m_pMesh, p);
		}
	}
	fclose(fp);
	GetView()->Invalidate(false);
}

void CSizeKorea2020Doc::OnExportLandmark()
{
	CString strFileName;
	LPCTSTR filter = "Landmark file(ascii)(*.lnd)|*.lnd||";
	CFileDialog FileDlg(false, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (FileDlg.DoModal() != IDOK)
		return;

	strFileName = FileDlg.GetPathName(); // get file path.
	if (strFileName.Find(".lnd") == -1)
		strFileName += ".lnd";

	FILE *fp = NULL;
	fopen_s(&fp, strFileName, "w");	//fprintf( fp, "%s %d\n", "ScanPose:", m_ScanPose );

	if (m_ScanPose == 1)
	{
		int LandMarkNum = (int)theSizer.m_Landmarks.size();
		_cprintf("LandmarkNum : %d\n", LandMarkNum);
		for (int i = 0; i < LandMarkNum; ++i)
		{
			if (theSizer.m_Landmarks[i] != NULL) {
				EgPos p = theSizer.m_Landmarks[i]->m_Pos;

				fprintf(fp, "%s\t%lf %lf %lf\n", LandmarkPose1[i].c_str(), p[0], p[1], p[2]);
			}
		}

		// 보조점들도 같이 저장한다
		for (int i = 0; i < LANDMARK_HELP_NUM_POSE1; i++) {
			_cprintf("Saving landmark help pose1... %d\n", i);
			if (m_LandmarkHelpPose1[i] != NULL) {
				EgPos p = m_LandmarkHelpPose1[i]->m_Pos;

				fprintf(fp, "%s\t%lf %lf %lf\n", LandmarkHelpPose1[i].c_str(), p[0], p[1], p[2]);
			}
		}
	}
	else if (m_ScanPose == 2)
	{
		int LandMarkNum = (int)theSizer.m_Landmarks.size();
		for (int i = 0; i < LandMarkNum; ++i)
		{
			EgPos p = theSizer.m_Landmarks[i]->m_Pos;
			fprintf(fp, "%s\t%lf %lf %lf\n", LandmarkPose2[i].c_str(), p[0], p[1], p[2]);
		}

		// 기존의 측정점들도 같이 저장한다
		for (int i = 0; i < LANDMARK_NUM_POSE2; i++) {
			GVector3f v = m_LandMarkPose2[i];
			fprintf(fp, "%s\t%lf %lf %lf\n", "기존측정점", v[0], v[1], v[2]);
		}

		// 보조점들도 같이 저장한다
		for (int i = 0; i < LANDMARK_NUM_HELP; i++) {
			GLandMark l = m_LandMarkHelp[i];
			fprintf(fp, "%s\t%lf %lf %lf\n", "보조점", l.X, l.Y, l.Z);
		}
	}
	else if (m_ScanPose == 3)
	{
		int LandMarkNum = (int)theSizer.m_Landmarks.size();
		for (int i = 0; i < LandMarkNum; ++i)
		{
			EgPos p = theSizer.m_Landmarks[i]->m_Pos;
			fprintf(fp, "%s\t%lf %lf %lf\n", LandmarkPose3[i].c_str(), p[0], p[1], p[2]);
		}
	}
	else if (m_ScanPose == 4)
	{
		int LandMarkNum = (int)theSizer.m_Landmarks.size();
		for (int i = 0; i < LandMarkNum; ++i)
		{
			EgPos p = theSizer.m_Landmarks[i]->m_Pos;
			fprintf(fp, "%s\t%lf %lf %lf\n", LandmarkPose4[i].c_str(), p[0], p[1], p[2]);
		}
	}
	else if (m_ScanPose == 5)
	{
		int LandMarkNum = (int)theSizer.m_Landmarks.size();
		for (int i = 0; i < LandMarkNum; ++i)
		{
			EgPos p = theSizer.m_Landmarks[i]->m_Pos;
			fprintf(fp, "%s\t%lf %lf %lf\n", LandmarkPose5[i].c_str(), p[0], p[1], p[2]);
		}
	}
	fclose(fp);
}


void CSizeKorea2020Doc::OnSelectPose()
{
	// 자세 선택 대화상자를 출력한다.
	CSelectPoseDialog dlg;
	if (dlg.DoModal() == IDOK)
		m_ScanPose = dlg.m_ScanPose + 1;
	else
		m_ScanPose = 0;

	// 자세3 이라면
	if (m_ScanPose == 3)
	{
		GVector3f MaxZPt(0.0, 0.0, -INFINITY);
		for (int i = 0; i < m_vNum; ++i)
		{
			if (m_Vertex[i].Z > MaxZPt.Z)
				MaxZPt = m_Vertex[i];
		}
		// 자세3(구) 라면 자세5로 변경한다.
		if (MaxZPt.X > (m_MinX + m_MaxX) * 0.5)
			m_ScanPose = 5;
	}

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->ShowPane(&pFrame->m_Pose1InfoDlg, FALSE, FALSE, TRUE);
	pFrame->ShowPane(&pFrame->m_Pose2InfoDlg, FALSE, FALSE, TRUE);
	pFrame->ShowPane(&pFrame->m_Pose3InfoDlg, FALSE, FALSE, TRUE);
	pFrame->ShowPane(&pFrame->m_Pose4InfoDlg, FALSE, FALSE, TRUE);
	pFrame->ShowPane(&pFrame->m_Pose5InfoDlg, FALSE, FALSE, TRUE);
	pFrame->m_EditModelDlg.UpdateDlgData();
	switch (m_ScanPose)
	{
	case 1:
		pFrame->ShowPane(&pFrame->m_Pose1InfoDlg, TRUE, FALSE, TRUE);
		theSizer.Init(1, m_ScanMesh);
		break;

	case 2:
		pFrame->ShowPane(&pFrame->m_Pose2InfoDlg, TRUE, FALSE, TRUE);
		theSizer.Init(2, m_ScanMesh);
		break;

	case 3:
		pFrame->ShowPane(&pFrame->m_Pose3InfoDlg, TRUE, FALSE, TRUE);
		theSizer.Init(3, m_ScanMesh);
		break;

	case 4:
		pFrame->ShowPane(&pFrame->m_Pose4InfoDlg, TRUE, FALSE, TRUE);
		theSizer.Init(4, m_ScanMesh);
		break;

	case 5:
		pFrame->ShowPane(&pFrame->m_Pose5InfoDlg, TRUE, FALSE, TRUE);
		theSizer.Init(5, m_ScanMesh);
		break;
	}
	// mj::모델 불러오기 화면에서 편집창 숨기기
	// pFrame->ShowPane(&pFrame->m_EditModelDlg, TRUE, FALSE, TRUE);
	pFrame->RecalcLayout();
	UpdateAllViews(NULL);
}


// mj::Gender tool bar 버튼 토글
void CSizeKorea2020Doc::OnUpdateSetGender(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (!theSizer.m_bWoman)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}
