#pragma once
#include "pch.h"

#include "SizeKorea_Pose1.h"
#include "MainFrm.h"

extern bool m_bWoman;
extern float m_MinX, m_MaxX;
extern float m_MinY, m_MaxY;
extern float m_MinZ, m_MaxZ;

EgVertex* m_LandmarkPose1[LANDMARK_NUM_POSE1];
EgVertex* m_LandmarkHelpPose1[LANDMARK_HELP_NUM_POSE1];


std::vector<EgVertex*> m_SagittalPoints_Front, m_SagittalPoints_Back;
std::vector<EgVertex*> m_CoronalPoints_Right, m_CoronalPoints_Left;

REAL	m_ResultPose1[RESULT_NUM_POSE1];
BOOL	m_ResultPathViewPose1[RESULT_NUM_POSE1];
std::vector<EgPos>	m_ResultPathPose1[RESULT_NUM_POSE1];

void printPos(EgPos pos);
void printVert(EgVertex *v);
bool sortPosByY(EgPos a, EgPos b);
bool sortPointByY(EgVertex* a, EgVertex* b);
bool sortPointByZ(EgVertex* a, EgVertex* b);


// mj :: �ڼ�1, �ڼ�2 ���� ������ ���� ���� ���� (20. 12. 8)
extern std::vector<bool> m_bFindLandmark;


CSizeKorea2020View* GetView() 
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	return (CSizeKorea2020View*) pFrame->GetActiveView();
}

std::vector<std::string> LandmarkPose1 = {
	"�Ӹ�������", // 1
	"���ʸ���", // 2
	"�γ���", // 3
	"�����", // 4
	"����", // 5
	"�����", // 6
	"�����(����)", // 7
	"�����(��)", // 8
	"���������(����)", // 9
	"���������(��)", // 10
	"�ܵ����", // 11
	"��������(����)", // 12
	"��������(��)", // 13
	"�������Ʒ���", // 14
	"�㸮��", // 15
	"�㸮����", // 16
	"�㸮����", // 17
	"�����", // 18
	"��ŵ���", // 19
	"�赹����", // 20
	"�����̵�����", // 21
	"����", // 22
	"��������", // 23
	"�����������", // 24
	"����������", // 25
	"�����������", // 26
	"���ʺ�����", // 27
	"�ճ���", // 28
	"�ܵ���ں���(����)", // 29
	"�ܵ���ں���(��)", // 30
	"�ܵ���պ���(����)", // 31
	"�ܵ���պ���(��)", // 32
	"�ܵ����������(����)", // 33
	"�ܵ����������(��)", // 34
	"�ܵ����������(����)", // 35
	"�ܵ����������(��)", // 36
	"������", // 37
	"�������", // 38
	"�ո������", // 39
	"�̵帮��������", // 40
	"Top-hip������", // 41
	"Upper-hip������" // 42
};

std::vector<std::string> LandmarkHelpPose1 = {
	"�ڳ���", // 0
	"�ܵ����(L)", // 1
	"�ܵ������(R)", // 2
	"�ܵ������(L)", // 3
	"�ܵ������(R)", // 4
	"�ܵ������(L)", // 5
	"���ִ뵹����", // 6
	"����(L)" // 7
};

std::vector<std::string> ResultPose1 = {
	"Ű",
	"������",
	"�γ�����",
	"��ڳ���",
	"�񿷳���",
	"��ճ���",
	"�������",
	"������ʳ���",
	"�ܵ������",
	"����������",
	"�������Ʒ�����",
	"�̵帮������",
	"�㸮����",
	"��ż����㸮����",
	"�����",
	"Upper-hip����",
	"Top-hip����",
	"�����̳���",
	"������",
	"����������",
	"�������������",
	"��������",
	"���������",
	"���ʺ������",
	"�ճ�����",
	"�������",
	"������ʱ���",
	"������̱���",
	"������ʻ��̱���",
	"��ھ������",
	"��ھ�����ʱ���",
	"��ھ�����̱���",
	"�ܵ���ں����̱���",
	"�ܵ�����������̱���",
	"�ܵ���պ����̱���",
	"�ܵ�����������̱���",
	"���������̼������",
	"�ܵ���յ����������̱���",
	"���߽ɱ���",
	"��ż��ؾ��߽ɱ���",
	"������������",
	"��(������)�㸮�ѷ�������",
	"�������������",
	"����������㸮�ѷ�������",
	"��ڵ�����ܵ�����ر���",
	"�񿷵��㸮�ѷ�������",
	"�����",
	"��ż��ص����",
	"��ڿ��ݱ���",
	"�ѱ���",
	"���ȱ���(�����)",
	"���ȱ���(���������)",
	"�ȱ���(�����)",
	"�ȱ���(���������)",
	"�����������",
	"�����̼�������",
	"��ڼո���ʱ���(�ȳ���)",
	"�Ҵٸ���������",
	"����赹��β�",
	"���鰡���β�",
	"�������β�",
	"�����ʾ�����ʰ�簢",
	"���ʾ�����ʰ�簢",
	"�����ʾ����簢",
	"���ʾ����簢"
};

void printPos(EgPos pos) {
	_cprintf("%f %f %f\n", pos.m_Coords[0], pos.m_Coords[1], pos.m_Coords[2]);
}

void printVert(EgVertex *v) {
	_cprintf("%f %f %f\n", v->m_Pos[0], v->m_Pos[1], v->m_Pos[2]);
}

bool sortPosByY(EgPos a, EgPos b) {
	return a.m_Coords[1] > b.m_Coords[1];
}

// mj::Sort EgPos.Y by descending order
bool sortPointByY(EgVertex* a, EgVertex* b) {
	return a->m_Pos[1] > b->m_Pos[1];
}

// mj::Sort EgPos.Z by ascending order
bool sortPointByZ(EgVertex* a, EgVertex* b) {
	return a->m_Pos[2] < b->m_Pos[2];
}


/********************************************************************************************************************/

///////////////////////////////////////  ������::�Ӹ������� 
// ������ ������ : 2020. 7. 11
void Pose1_FindTopHeadPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_TOPHEAD])
		return;

	EgVertex *TopHeadPt = NULL;

	EgPos maxY(0, -INFINITY, 0);
	for (EgVertex *vertex : pMesh->m_pVerts) {
		if (vertex->m_Pos[1] > maxY.m_Coords[1]) {
			TopHeadPt = vertex;
			maxY.m_Coords[1] = vertex->m_Pos[1];
		}
	}

	if (TopHeadPt != NULL) {
		m_LandmarkPose1[POSE1_LANDMARK_TOPHEAD] = TopHeadPt;
		_cprintf("Top Head Pt : ");
		printVert(TopHeadPt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_TOPHEAD] = NULL;
		_cprintf("Top Head Pt : NOT FOUND!\n");
	}
}


///////////////////////////////////////  Make sagittal/coronal plane
// ������ ������ : 2020. 11. 21
void Pose1_MakeSilhouette(EgMesh *pMesh) {
	m_SagittalPoints_Front.clear();
	m_SagittalPoints_Back.clear();

	m_CoronalPoints_Right.clear();
	m_CoronalPoints_Left.clear();

	_cprintf("Making Sagittal Silhouette...");
	///////////////////////////////////////  Make sagittal plane :: �Ӹ������� �� front, �� back
	// ��ü��Ī�� �� �´� ���, sagittal plane�� ���߾ӿ� ��ġ���� �ʴ� ������ ����

	EgVertex* TopHeadPt = m_LandmarkPose1[POSE1_LANDMARK_TOPHEAD];
	for (EgFace *face : pMesh->m_pFaces) {
		EgVertex *v0 = face->GetVertex(0), *v1 = face->GetVertex(1), *v2 = face->GetVertex(2);
		// Sagittal cutting plane
		// �Ӹ��������� ���߾��� �ƴҼ��� �ֱ� ������ (maxX + minX)/2�� �߾����� ģ��
		EgPlane planeS(EgVec3(1.0, 0.0, 0.0), EgPos((m_MaxX + m_MinX) / 2, TopHeadPt->m_Pos[1], TopHeadPt->m_Pos[2]));

		// Sagittal
		if (planeS.IsAbovePlane(v0->m_Pos) && planeS.IsAbovePlane(v1->m_Pos) && planeS.IsAbovePlane(v2->m_Pos))
			continue;
		else if (planeS.IsBelowPlane(v0->m_Pos) && planeS.IsBelowPlane(v1->m_Pos) && planeS.IsBelowPlane(v2->m_Pos))
			continue;

		EgVertex *intersection = NULL;

		// Sagittal
		if (dist(planeS, v0->m_Pos) * dist(planeS, v1->m_Pos) < 0) {
			// Project v0
			intersection = FindClosestVert(pMesh, proj(v0->m_Pos, planeS));
		}
		else if (dist(planeS, v1->m_Pos) * dist(planeS, v2->m_Pos) < 0) {
			// Project v1
			intersection = FindClosestVert(pMesh, proj(v1->m_Pos, planeS));
		}
		else if (dist(planeS, v2->m_Pos) * dist(planeS, v0->m_Pos) < 0) {
			// Project v2
			intersection = FindClosestVert(pMesh, proj(v2->m_Pos, planeS));
		}

		// Sagittal
		// intersection���� front�� �ִ� z��, back�� �ּ� z���� ���� ���� Ž���Ͽ� silhouette������ �����Ѵ� (20. 11. 21)
		if (intersection != NULL) {
			if (intersection->m_Pos[2] < TopHeadPt->m_Pos[2]) {
				m_SagittalPoints_Back.push_back(SK_Extreme_Vertex(intersection, 30.0, 4, 1));
			}
			else {
				m_SagittalPoints_Front.push_back(SK_Extreme_Vertex(intersection, 30.0, 5, 1));
			}
		}
	}
	// Sagittal
	std::sort(m_SagittalPoints_Front.begin(), m_SagittalPoints_Front.end(), sortPointByY);
	std::sort(m_SagittalPoints_Back.begin(), m_SagittalPoints_Back.end(), sortPointByY); 
	_cprintf("\n%d %d\n", m_SagittalPoints_Front.size(), m_SagittalPoints_Back.size());


	///////////////////////////////////////  Make coronal plane By minmax (20. 8. 27)
	////////////////////////////////////// => Shoulder, Sneck, Waist���� ���
	/*
	// 5mm �� Slice down�ϸ鼭 minmax�� coronal silhouette�� �����Ѵ�
	_cprintf("Making Coronal Silhouette...");
	EgSlicer Slicer;
	for (float level = m_MaxY; level >= m_MinY; level -= 5.0f) {
		Slicer.Slice(pMesh, 1, level / m_MaxY, -1, 1, -1, 1, -1, 1, true);

		EgPos minX(INFINITY, 0, 0), maxX(-INFINITY, 0, 0);
		for (int cutLineNum = 0; cutLineNum < Slicer.GetNumCutLine(); cutLineNum++) {
			for (EgPos pos : Slicer.m_CutLines[cutLineNum]) {
				if (pos.m_Coords[0] < minX.m_Coords[0]) {
					minX = pos;
				}

				if (pos.m_Coords[0] > maxX.m_Coords[0]) {
					maxX = pos;
				}
			}
			// TopHead �������� �¿츦 ������
			if (minX != EgPos(INFINITY, 0, 0) && minX.m_Coords[0] <= TopHeadPt->m_Pos[0])
				m_CoronalPoints_Right.push_back(FindClosestVert(pMesh, minX));
			if (maxX != EgPos(-INFINITY, 0, 0) && maxX.m_Coords[0] > TopHeadPt->m_Pos[0])
				m_CoronalPoints_Left.push_back(FindClosestVert(pMesh, maxX));
		}
	}
	_cprintf("\n%d %d\n", m_CoronalPoints_Right.size(), m_CoronalPoints_Left.size());
	*/
}


/////////////////////////////////////// ������::�ܵ����������(R/L)
/////////////////////////////////////// ������::�ܵ����������(R/L)
/////////////////////////////////////// ������::�ܵ���� (R)
/////////////////////////////////////// ������::�ܵ���� (L)
/////////////////////////////////////// ������::�ܵ������ (R/L)
/////////////////////////////////////// ������::�ܵ������ (R/L)
// ������ ������ : 2020. 10. 27
void Pose1_FindArmpitPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_ARMPIT]
		&& !m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_FRONT_R]
		&& !m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_FRONT_L]
		&& !m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_BACK_R]
		&& !m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_BACK_L])
		return;

	EgVertex *TopHeadPt = m_LandmarkPose1[POSE1_LANDMARK_TOPHEAD];

	if (TopHeadPt == NULL) {
		_cprintf("Error in finding Armpit... Exiting...\n");
	}

	EgVertex *ArmpitRPt = NULL,
		*ArmpitLPt = NULL;

	EgVertex *ArmpitFoldR = NULL,
		*ArmpitFoldL = NULL,
		*ArmpitBackFoldR = NULL,
		*ArmpitBackFoldL = NULL;

	EgVertex *ArmpitFrontPt_R = NULL,
		*ArmpitFrontPt_L = NULL,
		*ArmpitBackPt_R = NULL,
		*ArmpitBackPt_L = NULL;

	// �ܵ������ ���� ���ġ
	float upperArmpitBound = m_MaxY * 0.77;
	float lowerArmpitBound = m_MaxY * 0.71;

	// ���� ��� (20. 10. 6)
	// upperArmpitBound ~ lowerArmpitBound ���̸� ���������� slice�� ��
	std::vector<EgPos> SortedPts;
	std::vector<EgPos> armpitCandidates;
	EgSlicer armpitSlicer;
	for (double level = 0.77; level >= 0.71; level -= 0.01) {
		// x ���� ����, z ��� �������� Ž��
		armpitSlicer.Slice(pMesh, 1, level, -1, 0, -1, 1, 0, 1, true);
		SK_Sort_By_Curvature(armpitSlicer.m_CutLines[0], SortedPts, true, true, EgVec3(0, 1, 0));

		// ��� ���� 8���� �ִ´�
		// �յ� ��ȯ��, ������� ���, �ܵ���յ��¿�
		armpitCandidates.push_back(SortedPts[0]);
		armpitCandidates.push_back(SortedPts[1]);
		armpitCandidates.push_back(SortedPts[2]);
		armpitCandidates.push_back(SortedPts[3]);
		armpitCandidates.push_back(SortedPts[4]);
		armpitCandidates.push_back(SortedPts[5]);
		armpitCandidates.push_back(SortedPts[6]);
		armpitCandidates.push_back(SortedPts[7]);
	}
	armpitSlicer.SortByLength(true);
	EgPos CenterPt = armpitSlicer.GetCenterPos(0);

	float upper = 0.9;
	float lower = 0.6;

	std::vector<EgPos> front_R, front_L, back_R, back_L;

	while (front_R.empty()) {
		for (EgPos pos : armpitCandidates) {
			float lowerX = (m_MinX + m_MaxX) / 2 - ((m_MaxX - m_MinX) * upper)/2;
			float upperX = (m_MinX + m_MaxX) / 2 - ((m_MaxX - m_MinX) * lower)/2;

			if (pos.m_Coords[0] < upperX && pos.m_Coords[0] > lowerX && pos.m_Coords[2] > CenterPt.m_Coords[2])
				front_R.push_back(pos);
		}

		lower -= 0.03;
	}
	lower = 0.6;
	while (back_R.empty()) {
		for (EgPos pos : armpitCandidates) {
			float lowerX = (m_MinX + m_MaxX) / 2 - ((m_MaxX - m_MinX) * upper)/2;
			float upperX = (m_MinX + m_MaxX) / 2 - ((m_MaxX - m_MinX) * lower)/2;

			if (pos.m_Coords[0] < upperX && pos.m_Coords[0] > lowerX && pos.m_Coords[2] < CenterPt.m_Coords[2])
				back_R.push_back(pos);
		}

		lower -= 0.03;
	}

	lower = 0.6;
	while (front_L.empty()) {
		for (EgPos pos : armpitCandidates) {
			float upperX = (m_MinX + m_MaxX) / 2 + ((m_MaxX - m_MinX) * upper)/2;
			float lowerX = (m_MinX + m_MaxX) / 2 + ((m_MaxX - m_MinX) * lower)/2;

			if (pos.m_Coords[0] < upperX && pos.m_Coords[0] > lowerX && pos.m_Coords[2] > CenterPt.m_Coords[2])
				front_L.push_back(pos);
		}

		lower -= 0.03;
	}
	lower = 0.6;
	while (back_L.empty()) {
		for (EgPos pos : armpitCandidates) {
			float upperX = (m_MinX + m_MaxX) / 2 + ((m_MaxX - m_MinX) * upper)/2;
			float lowerX = (m_MinX + m_MaxX) / 2 + ((m_MaxX - m_MinX) * lower)/2;

			if (pos.m_Coords[0] < upperX && pos.m_Coords[0] > lowerX && pos.m_Coords[2] < CenterPt.m_Coords[2])
				back_L.push_back(pos);
		}

		lower -= 0.03;
	}


	//////// �ܵ����������(R)
	if (m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_FRONT_R]) {
		if (!front_R.empty()) {
			// �ĺ����� ���� �߾� ���� ����
			std::sort(front_R.begin(), front_R.end(), sortPosByY);
			EgPos first = front_R[0];
			EgPos last = front_R[front_R.size() - 1];

			ArmpitRPt = FindClosestVert(pMesh, EgPos((first.m_Coords[0] + last.m_Coords[0]) / 2, (first.m_Coords[1] + last.m_Coords[1]) / 2, (first.m_Coords[2] + last.m_Coords[2]) / 2));
			ArmpitRPt = SK_Hyperbolic_Vertex(ArmpitRPt, 30);

			// �ܵ����������(R)
			ArmpitFoldR = FindClosestVert(pMesh, EgPos(ArmpitRPt->m_Pos[0], ArmpitRPt->m_Pos[1] + 15, ArmpitRPt->m_Pos[2]));
			if (ArmpitFoldR->m_Pos[1] < ArmpitRPt->m_Pos[1])
				ArmpitFoldR->m_Pos[1] = ArmpitRPt->m_Pos[1] + 15;
			if (ArmpitFoldR != NULL) {
				m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_R] = ArmpitFoldR;
				_cprintf("Armpit Fold Pt Front (R) : ");
				printVert(ArmpitFoldR);
			}
			else {
				m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_R] = NULL;
				_cprintf("Armpit Fold Pt Front (R) : NOT FOUND!\n");

			}
		}
		else {
			_cprintf("Armpit Fold Pt Front (R) : Not Found ! Replacing... ");
			EgPos minX = EgPos(INFINITY, 0, 0);
			for (EgPos pos : SortedPts) {
				if (pos.m_Coords[0] < minX.m_Coords[0] && pos.m_Coords[2] && pos.m_Coords[2] > (m_MinZ + m_MaxZ) / 2) {
					minX = pos;
				}
			}
			m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_R] = FindClosestVert(pMesh, minX);
			_cprintf("Armpit Fold Pt Front (R) : ");
			printVert(ArmpitFoldR);
		}

	}
	// ������Ʈ�� �ܵ����������(R)�� ����� ���, ArmpitRPt�� ArmpitFoldR�� ���� ������Ʈ ���ش� (20. 12. 8)
	else {
		ArmpitFoldR = m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_R];

		// ��¥�� �ڿ��� ���Ӱ� allocate���ֹǷ� ���⼱ ArmpitFoldR�� ����Ű�� �Ѵ� (20. 12. 8)
		ArmpitRPt = ArmpitFoldR;
	}

	//////// �ܵ����������(L)
	if (m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_FRONT_L]) {
		if (!front_L.empty()) {
			// �ĺ����� ���� �߾� ���� ����
			std::sort(front_L.begin(), front_L.end(), sortPosByY);
			EgPos first = front_L[0];
			EgPos last = front_L[front_L.size() - 1];

			ArmpitLPt = FindClosestVert(pMesh, EgPos((first.m_Coords[0] + last.m_Coords[0]) / 2, (first.m_Coords[1] + last.m_Coords[1]) / 2, (first.m_Coords[2] + last.m_Coords[2]) / 2));
			ArmpitLPt = SK_Hyperbolic_Vertex(ArmpitLPt, 30);

			// �ܵ����������(L)
			ArmpitFoldL = FindClosestVert(pMesh, EgPos(ArmpitLPt->m_Pos[0], ArmpitLPt->m_Pos[1] + 15, ArmpitLPt->m_Pos[2]));

			if (ArmpitFoldL != NULL) {
				m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_L] = ArmpitFoldL;
				_cprintf("Armpit Fold Pt Front (L) : ");
				printVert(ArmpitFoldL);
			}
			else {
				m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_L] = NULL;
				_cprintf("Armpit Fold Pt Front (L) : NOT FOUND!\n");
			}
		}
		else {
			_cprintf("Armpit Fold Pt Front (L) : Not Found ! Replacing... ");

			EgPos maxX = EgPos(-INFINITY, 0, 0);
			for (EgPos pos : SortedPts) {
				if (pos.m_Coords[0] > maxX.m_Coords[0] && pos.m_Coords[2] && pos.m_Coords[2] > (m_MinZ + m_MaxZ) / 2) {
					maxX = pos;
				}
			}
			m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_L] = FindClosestVert(pMesh, maxX);
			_cprintf("Armpit Fold Pt Front (L) : ");
			printVert(ArmpitFoldL);
		}
	}
	// ������Ʈ�� �ܵ����������(L)�� ����� ���, ArmpitLPt�� ArmpitFoldL�� ���� ������Ʈ ���ش� (20. 12. 8)
	else {
		ArmpitFoldL = m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_L];
		// ��¥�� �ڿ��� ���Ӱ� allocate���ֹǷ� ���⼱ ArmpitFoldL�� ����Ű�� �Ѵ� (20. 12. 8)
		ArmpitLPt = ArmpitFoldL;
	}

	//////// �ܵ����������(R)
	if (m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_BACK_R]) {
		if (!back_R.empty()) {
			std::sort(back_R.begin(), back_R.end(), sortPosByY);
			EgPos first = back_R[0];
			EgPos last = back_R[back_R.size() - 1];

			ArmpitBackFoldR = FindClosestVert(pMesh, EgPos((first.m_Coords[0] + last.m_Coords[0]) / 2, (first.m_Coords[1] + last.m_Coords[1]) / 2, (first.m_Coords[2] + last.m_Coords[2]) / 2));
			ArmpitBackFoldR = SK_Hyperbolic_Vertex(ArmpitBackFoldR, 50);

			if (ArmpitBackFoldR->m_Pos[1] < ArmpitRPt->m_Pos[1])
				ArmpitBackFoldR = FindClosestVert(pMesh, EgPos(ArmpitBackFoldR->m_Pos[0], ArmpitRPt->m_Pos[1] + 20, ArmpitBackFoldR->m_Pos[2]));
			if (ArmpitBackFoldR != NULL) {
				m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_BACK_R] = ArmpitBackFoldR;
				_cprintf("Armpit Fold Pt Back (R) : ");
				printVert(ArmpitBackFoldR);
			}
			else {
				m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_BACK_R] = NULL;
				_cprintf("Armpit Fold Pt Back (R) : NOT FOUND!\n");
			}
		}
		else {
			_cprintf("Armpit Fold Pt Back (R) : Not Found!\n");
		}
	}

	//////// �ܵ����������(L)
	if (m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_BACK_L]) {
		if (!back_L.empty()) {
			std::sort(back_L.begin(), back_L.end(), sortPosByY);
			EgPos first = back_L[0];
			EgPos last = back_L[back_L.size() - 1];

			ArmpitBackFoldL = FindClosestVert(pMesh, EgPos((first.m_Coords[0] + last.m_Coords[0]) / 2, (first.m_Coords[1] + last.m_Coords[1]) / 2, (first.m_Coords[2] + last.m_Coords[2]) / 2));
			ArmpitBackFoldL = SK_Hyperbolic_Vertex(ArmpitBackFoldL, 50);

			if (ArmpitBackFoldL->m_Pos[1] < ArmpitLPt->m_Pos[1])
				ArmpitBackFoldL = FindClosestVert(pMesh, EgPos(ArmpitBackFoldL->m_Pos[0], ArmpitLPt->m_Pos[1] + 20, ArmpitBackFoldL->m_Pos[2]));

			if (ArmpitBackFoldL != NULL) {
				m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_BACK_L] = ArmpitBackFoldL;
				_cprintf("Armpit Fold Pt Back (L) : ");
				printVert(ArmpitBackFoldL);
			}
			else {
				m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_BACK_L] = NULL;
				_cprintf("Armpit Fold Pt Back (L) : NOT FOUND!\n");

			}
		}
		else {
			_cprintf("Armpit Fold Pt Back (L) : Not Found!\n");
		}
	}

	/////////////////////////////////////////////////////// �ܵ���յ���(R/L)

	// �ܵ�������� �յڷ� ray�� ���� ������ ���������κ��� FindClosestVert
	// ������
	EgLine rayArmpitR = EgLine(EgPos(ArmpitRPt->m_Pos[0], ArmpitRPt->m_Pos[1], (ArmpitFoldR->m_Pos[2] + ArmpitBackFoldR->m_Pos[2])/2), EgVec3(0, 0, 1));
	// ����
	EgLine rayArmpitL = EgLine(EgPos(ArmpitLPt->m_Pos[0], ArmpitLPt->m_Pos[1], (ArmpitFoldL->m_Pos[2] + ArmpitBackFoldL->m_Pos[2])/2), EgVec3(0, 0, 1));

	EgPos tmpArmpitFrontPt_R = ArmpitRPt->m_Pos, tmpArmpitBackPt_R = ArmpitRPt->m_Pos, 
		tmpArmpitFrontPt_L = ArmpitLPt->m_Pos, tmpArmpitBackPt_L = ArmpitLPt->m_Pos;
	for (EgFace *face : pMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		EgPos tmpIntersection;
		// �ܵ������(R)
		// �ܵ������(R)
		if (intersect_line_triangle(p0, p1, p2, rayArmpitR, tmpIntersection, false)) {
			if (tmpIntersection.m_Coords[2] > (ArmpitFoldR->m_Pos[2] + ArmpitBackFoldR->m_Pos[2])/2)
				tmpArmpitFrontPt_R = tmpIntersection;
			else 
				tmpArmpitBackPt_R = tmpIntersection;
		}

		// �ܵ������(L)
		// �ܵ������(L)
		if (intersect_line_triangle(p0, p1, p2, rayArmpitL, tmpIntersection, false)) {
			if (tmpIntersection.m_Coords[2] > (ArmpitFoldL->m_Pos[2] + ArmpitBackFoldL->m_Pos[2])/2)
				tmpArmpitFrontPt_L = tmpIntersection;
			else 
				tmpArmpitBackPt_L = tmpIntersection;
		}
	}


	/////// �ܵ������(R)
	ArmpitFrontPt_R = FindClosestVert(pMesh, tmpArmpitFrontPt_R);
	if (ArmpitFrontPt_R != NULL) {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_R] = ArmpitFrontPt_R;
		_cprintf("Armpit Pt Front (R) : ");
		printVert(ArmpitFrontPt_R);
	}
	else {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_R] = NULL;
		_cprintf("Armpit Pt Front (R) : NOT FOUND!\n");
	}


	/////// �ܵ������(L)
	ArmpitFrontPt_L = FindClosestVert(pMesh, tmpArmpitFrontPt_L);
	if (ArmpitFrontPt_L != NULL) {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_L] = ArmpitFrontPt_L;
		_cprintf("Armpit Pt Front (L) : ");
		printVert(ArmpitFrontPt_L);
	}
	else {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_L] = NULL;
		_cprintf("Armpit Pt Front (L) : NOT FOUND!\n");
	}


	/////// �ܵ������(R)
	ArmpitBackPt_R = FindClosestVert(pMesh, tmpArmpitBackPt_R);
	if (ArmpitBackPt_R != NULL) {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_BACK_R] = ArmpitBackPt_R;
		_cprintf("Armpit Pt Back (R) : ");
		printVert(ArmpitBackPt_R);
	}
	else {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_BACK_R] = NULL;
		_cprintf("Armpit Pt Back (R) : NOT FOUND!\n");
	}


	/////// �ܵ������(L)
	ArmpitBackPt_L = FindClosestVert(pMesh, tmpArmpitBackPt_L);
	if (ArmpitBackPt_L != NULL) {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_BACK_L] = ArmpitBackPt_L;
		_cprintf("Armpit Pt Back (L) : ");
		printVert(ArmpitBackPt_L);
	}
	else {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_BACK_L] = NULL;
		_cprintf("Armpit Pt Back (L) : NOT FOUND!\n");
	}




	//////// �ܵ����(R)
	// �ܵ������ z���� �յ��������� �߾�
	if (m_bFindLandmark[POSE1_LANDMARK_ARMPIT]) {
		if (ArmpitFoldR != NULL && ArmpitBackFoldR != NULL) {
			m_LandmarkPose1[POSE1_LANDMARK_ARMPIT] = new EgVertex(
				ArmpitRPt->m_Pos[0],
				ArmpitRPt->m_Pos[1],
				(m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_R]->m_Pos[2] + m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_BACK_R]->m_Pos[2]) / 2
			);
			_cprintf("Armpit Pt (R) : ");
			printVert(m_LandmarkPose1[POSE1_LANDMARK_ARMPIT]);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_ARMPIT] = NULL;
			_cprintf("Armpit Pt (R) : NOT FOUND!\n");
		}
	}


	///////// �ܵ����(L)
	if (ArmpitLPt != NULL) {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_L] = new EgVertex(
			ArmpitLPt->m_Pos[0],
			ArmpitLPt->m_Pos[1],
			(m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_L]->m_Pos[2] + m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_BACK_L]->m_Pos[2]) / 2
		);
		_cprintf("Armpit Pt (L) : ");
		printVert(m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_L]);
	}
	else {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_L] = NULL;
		_cprintf("Armpit Pt (L) : NOT FOUND!\n");
	}
}


/////////////////////////////////////// ������::����� (R/L)
/////////////////////////////////////// ������::���������  (R/L)
// ������ ������ : 2020. 10. 26
void Pose1_FindShoulderPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_SHOULDER_R]
		&& !m_bFindLandmark[POSE1_LANDMARK_SHOULDER_L]
		&& !m_bFindLandmark[POSE1_LANDMARK_SHOULDERSIDE_R]
		&& !m_bFindLandmark[POSE1_LANDMARK_SHOULDERSIDE_L])
		return;

	EgVertex *ArmpitR = m_LandmarkPose1[POSE1_LANDMARK_ARMPIT],
		*ArmpitL = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_L],
		*ArmpitFrontR = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_R],
		*ArmpitFrontL = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_L],
		*ArmpitBackR = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_BACK_R],
		*ArmpitBackL = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_BACK_L],
		*ArmpitFoldFront_R = m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_R],
		*ArmpitFoldFront_L = m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_L],
		*ArmpitFoldBack_R = m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_BACK_R],
		*ArmpitFoldBack_L = m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_BACK_L],
		*TopHeadPt = m_LandmarkPose1[POSE1_LANDMARK_TOPHEAD];

	if (ArmpitR == NULL || ArmpitL == NULL || ArmpitFrontR == NULL || ArmpitFrontL == NULL 
		|| ArmpitBackR == NULL || ArmpitBackL == NULL || TopHeadPt== NULL 
		|| ArmpitFoldFront_R == NULL || ArmpitFoldBack_R == NULL
		|| ArmpitFoldFront_L == NULL || ArmpitFoldBack_L == NULL) {
		_cprintf("Error in finding Shoulder... Exiting...\n");
		return;
	}

	EgVertex *ShoulderRightPt = NULL,
		*ShoulderLeftPt = NULL,
		*ShoulderSideRightPt = NULL,
			*ShoulderSideLeftPt = NULL;

	EgSlicer theSlicer;
	std::vector<EgPos> Pts;
	std::vector<EgPos> SortedPts;
	std::vector<EgPos> Path;

	EgVertex *v;
	EgPos maxY(0, -INFINITY, 0);
	EgPos minX(INFINITY, 0, 0);

	// �����(R)�� ã�ƾ��� ���� ����
	if (m_bFindLandmark[POSE1_LANDMARK_SHOULDER_R]) {
		theSlicer.SetCutPlane(EgPlane(EgVec3(0, 1, 0), EgPos(ArmpitR->m_Pos[0], ArmpitR->m_Pos[1] + 50, ArmpitR->m_Pos[2])));
		theSlicer.Slice(pMesh);

		for (EgPos p : theSlicer.m_CutLines[0]) {
			if (p[0] < (m_MinX + m_MaxX) / 2) {
				Pts.push_back(p);
			}
		}
		SK_Sort_By_Curvature(Pts, SortedPts, true, false, EgVec3());

		v = FindClosestVert(pMesh, SortedPts[0]);
		SK_Planar_Perimeter(v, &EgPlane(EgVec3(1, 0, 0), v->m_Pos), Path, false);

		for (EgPos pos : Path) {
			if (pos.m_Coords[1] > maxY.m_Coords[1]) {
				maxY = pos;
			}
		}

		ShoulderRightPt = FindClosestVert(pMesh, maxY);

		ShoulderRightPt = SK_Elliptic_Vertex(ShoulderRightPt, 50.0);
		theSlicer.SetCutPlane(EgPlane(EgVec3(0, 1, 0), ShoulderRightPt->m_Pos));
		theSlicer.Slice(pMesh);

		for (EgPos pos : theSlicer.m_CutLines[0]) {
			minX = (pos[0] < minX[0]) ? pos : minX;
		}

		ShoulderRightPt = FindClosestVert(pMesh, minX);
	}
	// �����(R)�� ���� ���� ���� �� ShoulderRightPt �Ҵ����ش� (20. 12. 8)
	else {
		ShoulderRightPt = m_LandmarkPose1[POSE1_LANDMARK_SHOULDER_R];
	}
	

	//////////////// �����(R), ���������(R)
	if (ShoulderRightPt != NULL) {
		if (m_bFindLandmark[POSE1_LANDMARK_SHOULDER_R]) { 
			m_LandmarkPose1[POSE1_LANDMARK_SHOULDER_R] = ShoulderRightPt;
			_cprintf("Shoulder (R) : ");
			printVert(ShoulderRightPt);
		}

		if (m_bFindLandmark[POSE1_LANDMARK_SHOULDERSIDE_R]) {
			m_LandmarkPose1[POSE1_LANDMARK_SHOULDERSIDE_R] = FindClosestVert(pMesh,
				EgPos(ShoulderRightPt->m_Pos[0],
					ShoulderRightPt->m_Pos[1],
					ArmpitR->m_Pos[2])
			);
			_cprintf("ShoulderSide (R) : ");
			printVert(m_LandmarkPose1[POSE1_LANDMARK_SHOULDERSIDE_R]);
		}
	}
	else {
		_cprintf("Shoulder (R) : NOT FOUND! Replacing... \n");
		theSlicer.SetCutPlane(EgPlane(EgVec3(0, 1, 0), ArmpitR->m_Pos));
		theSlicer.Slice(pMesh);

		EgPos minX = EgPos(INFINITY, 0, 0);
		for (EgPos pos : theSlicer.m_CutLines[0]) {
			if (pos.m_Coords[0] < minX.m_Coords[0]) {
				minX = pos;
			}
		}
		ShoulderRightPt = FindClosestVert(pMesh, minX);
		ShoulderRightPt = SK_Elliptic_Vertex(ShoulderRightPt, 30);
		m_LandmarkPose1[POSE1_LANDMARK_SHOULDER_R] = ShoulderRightPt;
		_cprintf("Shoulder (R) : ");
		printVert(ShoulderRightPt);


		_cprintf("ShoulderSide (R) : NOT FOUND! Replacing... \n");
		m_LandmarkPose1[POSE1_LANDMARK_SHOULDERSIDE_R] = FindClosestVert(pMesh,
			EgPos(ShoulderRightPt->m_Pos[0], 
				ShoulderRightPt->m_Pos[1],
				ArmpitR->m_Pos[2])
			);
		_cprintf("ShoulderSide (R) : ");
		printVert(m_LandmarkPose1[POSE1_LANDMARK_SHOULDERSIDE_R]);
	}



	// �����(L)�� ã�ƾ��� ���� ����
	if (m_bFindLandmark[POSE1_LANDMARK_SHOULDER_L]) {
		theSlicer.SetCutPlane(EgPlane(EgVec3(0, 1, 0), EgPos(ArmpitL->m_Pos[0], ArmpitL->m_Pos[1] + 50, ArmpitL->m_Pos[2])));
		theSlicer.Slice(pMesh);

		Pts.clear();
		for (EgPos p : theSlicer.m_CutLines[0]) {
			if (p[0] > (m_MinX + m_MaxX) / 2) {
				Pts.push_back(p);
			}
		}
		SortedPts.clear();
		SK_Sort_By_Curvature(Pts, SortedPts, true, false, EgVec3());

		v = FindClosestVert(pMesh, SortedPts[0]);
		Path.clear();
		SK_Planar_Perimeter(v, &EgPlane(EgVec3(1, 0, 0), v->m_Pos), Path, false);

		maxY = EgPos(0, -INFINITY, 0);
		for (EgPos pos : Path) {
			if (pos.m_Coords[1] > maxY.m_Coords[1]) {
				maxY = pos;
			}
		}


		ShoulderLeftPt = FindClosestVert(pMesh, maxY);
		ShoulderLeftPt = SK_Elliptic_Vertex(ShoulderLeftPt, 50.0);
		theSlicer.SetCutPlane(EgPlane(EgVec3(0, 1, 0), ShoulderLeftPt->m_Pos));
		theSlicer.Slice(pMesh);

		EgPos maxX = EgPos(-INFINITY, 0, 0);
		for (EgPos pos : theSlicer.m_CutLines[0]) {
			maxX = (pos[0] > maxX[0]) ? pos : maxX;
		}

		_cprintf("maxY (L): %f %f %f\n", maxY[0], maxY[1], maxY[2]);
		_cprintf("Elliptic (L): %f %f %f\n", ShoulderLeftPt->m_Pos[0], ShoulderLeftPt->m_Pos[1], ShoulderLeftPt->m_Pos[2]);
		_cprintf("maxX (L): %f %f %f\n", maxX[0], maxX[1], maxX[2]);

		ShoulderLeftPt = FindClosestVert(pMesh, maxX);
	}
	// �����(L)�� ���� ���� ���� ��, ShoulderLeftPt �Ҵ� �ʿ� (20. 12. 8)
	else {
		ShoulderLeftPt = m_LandmarkPose1[POSE1_LANDMARK_SHOULDER_L];
	}


	//////////////// �����(L), ���������(L)
	if (ShoulderLeftPt != NULL) {
		if (m_bFindLandmark[POSE1_LANDMARK_SHOULDER_L]) {
			m_LandmarkPose1[POSE1_LANDMARK_SHOULDER_L] = ShoulderLeftPt;
			_cprintf("Shoulder (L) : ");
			printVert(ShoulderLeftPt);
		}

		if (m_bFindLandmark[POSE1_LANDMARK_SHOULDERSIDE_L]) {
			m_LandmarkPose1[POSE1_LANDMARK_SHOULDERSIDE_L] = FindClosestVert(pMesh,
				EgPos(ShoulderLeftPt->m_Pos[0],
					ShoulderLeftPt->m_Pos[1],
					ArmpitL->m_Pos[2])
			);
			_cprintf("ShoulderSide (L) : ");
			printVert(m_LandmarkPose1[POSE1_LANDMARK_SHOULDERSIDE_L]);
		}
	}
	else {
		_cprintf("Shoulder (L) : NOT FOUND! Replacing... \n");
		theSlicer.SetCutPlane(EgPlane(EgVec3(0, 1, 0), ArmpitL->m_Pos));
		theSlicer.Slice(pMesh);

		EgPos maxX = EgPos(-INFINITY, 0, 0);
		for (EgPos pos : theSlicer.m_CutLines[0]) {
			if (pos.m_Coords[0] > maxX.m_Coords[0]) {
				maxX = pos;
			}
		}
		ShoulderLeftPt = FindClosestVert(pMesh, maxX);
		ShoulderLeftPt = SK_Elliptic_Vertex(ShoulderLeftPt, 30);
		m_LandmarkPose1[POSE1_LANDMARK_SHOULDER_L] = ShoulderLeftPt;
		_cprintf("Shoulder (L) : ");
		printVert(ShoulderLeftPt);

		_cprintf("ShoulderSide (L) : NOT FOUND!\n");
		m_LandmarkPose1[POSE1_LANDMARK_SHOULDERSIDE_L] = FindClosestVert(pMesh,
			EgPos(ShoulderLeftPt->m_Pos[0], 
				ShoulderLeftPt->m_Pos[1],
				ArmpitL->m_Pos[2])
			);

		_cprintf("ShoulderSide (L) : ");
		printVert(m_LandmarkPose1[POSE1_LANDMARK_SHOULDERSIDE_L]);
	}
}


/////////////////////////////////////// ������::���� 
// ������ ������ : 2020. 11. 21
void Pose1_FindSideNeckPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_NECK_SIDE])
		return;

	EgVertex *NeckSidePt = NULL,
		*NeckSideLeftPt = NULL;

	EgVertex *TopHeadPt = m_LandmarkPose1[POSE1_LANDMARK_TOPHEAD],
		*ShoulderSidePt = m_LandmarkPose1[POSE1_LANDMARK_SHOULDERSIDE_R];

	if (TopHeadPt == NULL || ShoulderSidePt == NULL) {
		_cprintf("Error in finding Side neck... Exiting...\n");
		return;
	}

	std::vector<EgPos> sneckCoronal_right, sneckCoronal_left;
	EgSlicer theSlicer;
	for (float level = (m_MaxY + ShoulderSidePt->m_Pos[1])/2; level > ShoulderSidePt->m_Pos[1]; level -= 5.0f) {
		theSlicer.Slice(pMesh, 1, level / m_MaxY, -1, 1, -1, 1, -1, 1, true);
		theSlicer.SortByLength(true);

		EgPos minX(INFINITY, 0, 0), maxX(-INFINITY, 0, 0);

		for (EgPos pos : theSlicer.m_CutLines[0]) {
			if (pos.m_Coords[0] < minX.m_Coords[0]) {
				minX = pos;
			}

			if (pos.m_Coords[0] > maxX.m_Coords[0]) {
				maxX = pos;
			}
		}

		// TopHead �������� �¿츦 ������
		if (minX != EgPos(INFINITY, 0, 0) && minX.m_Coords[0] <= TopHeadPt->m_Pos[0])
			sneckCoronal_right.push_back(minX);
		if (maxX != EgPos(-INFINITY, 0, 0) && maxX.m_Coords[0] > TopHeadPt->m_Pos[0])
			sneckCoronal_left.push_back(maxX);
	}


	// ����������� ���������� �մ� �������κ��� ���� �� �Ƿ翧���� �ӽø������� �����ϰ�
	EgPos NeckSidePt_tmp = EgPos();

	double maxDist = -INFINITY;
	EgLine tmp_l = EgLine(TopHeadPt->m_Pos, ShoulderSidePt->m_Pos);

	for (EgPos pos : sneckCoronal_right) {
		// ������~������� ������ �Ƿ翧 ���� ��
		if (pos[0] > ShoulderSidePt->m_Pos[0] && pos[0] < TopHeadPt->m_Pos[0]
			&& pos[1] > ShoulderSidePt->m_Pos[1] && pos[1] < TopHeadPt->m_Pos[1]) {
			// tmp_l�� �Ÿ��� ���� �� ��	
			double distance = dist(tmp_l, pos);

			if (distance > maxDist) {
				maxDist = distance;
				NeckSidePt_tmp = pos;
			}
		}
	}

	// �ӽø����� ���������� 1/4 �������� �ٽ� ���� ������� ������ ���Ѵ�.
	EgPos TopHeadPt_Quater = TopHeadPt->m_Pos;
	TopHeadPt_Quater.m_Coords[0] = NeckSidePt_tmp.m_Coords[0];
	TopHeadPt_Quater.m_Coords[1] = (3 * NeckSidePt_tmp.m_Coords[1] + TopHeadPt->m_Pos[1]) / 4;

	EgLine l = EgLine(TopHeadPt_Quater, ShoulderSidePt->m_Pos);

	EgPos NeckSideSeedPt = EgPos();
	maxDist = -INFINITY;
	for (EgPos pos : sneckCoronal_right) {
		if (pos[0] > ShoulderSidePt->m_Pos[0] && pos[0] < TopHeadPt_Quater.m_Coords[0]
			&& pos[1] > ShoulderSidePt->m_Pos[1] && pos[1] < TopHeadPt_Quater.m_Coords[1]) {
			double distance = dist(l, pos);

			if (distance > maxDist) {
				maxDist = distance;
				NeckSideSeedPt = pos;
			}
		}
	}

	NeckSidePt = FindClosestVert(pMesh, NeckSideSeedPt);

	// ���� ������ NeckSidePt���� ray�� ���� ������ ������
	EgPos NeckSidePt_L = EgPos();
	EgLine raySneck(NeckSidePt->m_Pos, EgVec3(-1, 0, 0));
	for (EgFace *face : pMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		EgPos tmpIntersection;
		if (intersect_line_triangle(p0, p1, p2, raySneck, tmpIntersection, true)) {
			NeckSidePt_L = tmpIntersection;
			break;
		}
	}

	if (NeckSidePt != NULL) {
		m_LandmarkPose1[POSE1_LANDMARK_NECK_SIDE] = NeckSidePt;
		_cprintf("Neck Side Pt (R) : ");
		printVert(NeckSidePt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_NECK_SIDE] = NULL;
		_cprintf("Neck Side Pt (R) : NOT FOUND!\n");
	}


	NeckSideLeftPt = FindClosestVert(pMesh, NeckSidePt_L);
	if (NeckSideLeftPt != NULL) {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_NECK_SIDE_L] = NeckSideLeftPt;
		_cprintf("Neck Side Pt (L) : ");
		printVert(NeckSideLeftPt);

	}
	else {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_NECK_SIDE_L] = NULL;
		_cprintf("Neck Side Pt (L) : NOT FOUND!\n");
	}
}


///////////////////////////////////////  ������::�����
// ������ ������ : 2020. 12. 3
void Pose1_FindFneckPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_NECK_FRONT])
		return;

	EgVertex *FneckPt = NULL;

	EgVertex *SneckPt = m_LandmarkPose1[POSE1_LANDMARK_NECK_SIDE],
		*SneckPt_L = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_NECK_SIDE_L];

	if (SneckPt == NULL || SneckPt_L == NULL) {
		_cprintf("Error in finding Fneck... Exiting...\n");
	}


	EgVertex upperPt, lowerPt;
	// float upperBound = m_MaxY * 0.83f; // �����Ž������ ���
	float upperBound = (SneckPt->m_Pos[1] <= SneckPt_L->m_Pos[1]) ? SneckPt->m_Pos[1] : SneckPt_L->m_Pos[1]; // �׻� ���� �ؿ� ��ġ�Ѵٰ� ����
	float lowerBound = m_MaxY * 0.79f; // �����Ž������ ���
	if (lowerBound > upperBound)
		lowerBound = upperBound - 50;

	std::vector<EgPos> fneckSilhouette;
	EgPos upper0, upper1, lower;
	for (EgVertex *v : m_SagittalPoints_Front) {
		EgPos pos = v->m_Pos;
		// SagittalPoints�� ������ ����ؼ� upperBound���� ���� �� ���������� Ž��
		if (pos.m_Coords[1] <= upperBound + 50 && pos.m_Coords[1] >= lowerBound)
			fneckSilhouette.push_back(pos);
	}
	sort(fneckSilhouette.begin(), fneckSilhouette.end(), sortPosByY);

	upper0 = upper1 = fneckSilhouette[0];
	lower = fneckSilhouette[fneckSilhouette.size() - 1];
	upper0.m_Coords[0] -= 10.0f;
	upper1.m_Coords[0] += 10.0f;

	_cprintf("upper Y : %f, lowerY : %f\n", upper0[1], lower[1]);

	EgPlane pln(upper0, upper1, lower);
	float maxDist = -INFINITY;
	EgPos tmpFneckPt; 
	for (EgPos pos : fneckSilhouette) {
		float thisDist = dist(pln, pos);

		if (thisDist > maxDist) {
			maxDist = thisDist;
			tmpFneckPt = pos;
		}
	}

	_cprintf("tmpFneckPt : %f %f %f\n", tmpFneckPt[0], tmpFneckPt[1], tmpFneckPt[2]);

	std::vector<EgPos> neckCtrl;
	neckCtrl.push_back(SneckPt->m_Pos);
	neckCtrl.push_back(SneckPt_L->m_Pos);

	// �������� 1cm ���� �տ��� �����Ѵ�
	float level = 10;
	EgSlicer theSlicer;

	int loopCount = 0; // level�� �ʹ� Ŀ���� ������ ��� ���� ������ ctrl pt�� �� �������� �ǵ��� ����
	while (neckCtrl.size() < 6 && loopCount < 3) {
		theSlicer.SetCutPlane(EgPlane(EgVec3(0, 0, 1), EgPos(SneckPt->m_Pos[0], SneckPt->m_Pos[1], SneckPt->m_Pos[2] + level)));
		theSlicer.Slice(pMesh);
		theSlicer.SortByLength(true);

		EgPos CenterPt = theSlicer.GetCenterPos(0);
		std::vector<EgPos> neckCoronal_right, neckCoronal_left;
		for (EgPos pos : theSlicer.m_CutLines[0]) {
			if (pos.m_Coords[1] <= upperBound && pos.m_Coords[1] >= lowerBound) {
				if (pos.m_Coords[0] < CenterPt.m_Coords[0])
					neckCoronal_right.push_back(pos);
				else
					neckCoronal_left.push_back(pos);
			}
		}
		std::sort(neckCoronal_right.begin(), neckCoronal_right.end(), sortPosByY);
		std::sort(neckCoronal_left.begin(), neckCoronal_left.end(), sortPosByY);

		float slope;
		int i;
		for (i = 1; i < neckCoronal_right.size(); i++) {
			slope = ABS((neckCoronal_right[i].m_Coords[1] - neckCoronal_right[i - 1].m_Coords[1])/(neckCoronal_right[i].m_Coords[0] - neckCoronal_right[i - 1].m_Coords[0]));

			if (slope < 1)
				break;
		}
		neckCtrl.push_back(neckCoronal_right[i - 1]);

		for (i = 1; i < neckCoronal_left.size(); i++) {
			slope = ABS((neckCoronal_left[i].m_Coords[1] - neckCoronal_left[i - 1].m_Coords[1])/(neckCoronal_left[i].m_Coords[0] - neckCoronal_left[i - 1].m_Coords[0]));

			if (slope < 1)
				break;
		}
		neckCtrl.push_back(neckCoronal_left[i - 1]);

		level += 20.0;
		loopCount++;
	}


	REAL4 P[6];
	P[0][0] = neckCtrl[0][0];	P[0][1] = neckCtrl[0][1];   P[0][2] = neckCtrl[0][2];   P[0][3] = 1.0f;
	P[1][0] = neckCtrl[2][0];	P[2][1] = neckCtrl[2][1];   P[2][2] = neckCtrl[2][2];   P[2][3] = 1.0f;
	P[2][0] = neckCtrl[4][0];	P[4][1] = neckCtrl[4][1];   P[4][2] = neckCtrl[4][2];   P[4][3] = 1.0f;

	P[3][0] = neckCtrl[5][0];	P[5][1] = neckCtrl[5][1];   P[5][2] = neckCtrl[5][2];   P[5][3] = 1.0f;
	P[4][0] = neckCtrl[3][0];	P[3][1] = neckCtrl[3][1];   P[3][2] = neckCtrl[3][2];   P[3][3] = 1.0f;
	P[5][0] = neckCtrl[1][0];	P[1][1] = neckCtrl[1][1];   P[1][2] = neckCtrl[1][2];   P[1][3] = 1.0f;

	GNurbsCrv PathCrv;
	PathCrv.InterCrv(4, P, 5);

	// ������߿� �ӽ� ������� x��ġ�� ���� ����� ���� ���Ͽ� mesh�� projection���� ���Ѵ�
	std::vector<EgPos> fneckCurv;
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

		fneckCurv.push_back(EgPos(p0[0], p0[1],p0[2]));
		fneckCurv.push_back(EgPos(p1[0], p1[1],p1[2]));

		float dis = (tmpFneckPt[0] - v0[0])*(tmpFneckPt[0] - v1[0]);
		if (dis < 0.0f) {
			GVector3f a = p0 * 0.5f;
			GVector3f b = p0 * 0.5f;
			FNv = a + b;
			//			FNv = (p0*0.5f) + (p1*0.5f) ;
			FNv.X = tmpFneckPt[0]; //minDis=dis;
		}
	}

	// GetView()->SetPoint(fneckCurv);

	// GObList<GVector3f> CrossPtList;
	std::vector<EgPos> CrossPtList;
	GPlane cutPln;  //GVector3f(FNv) �ӽø������ ������ YZ���(���߽û��)

	// 
	EgPos tmpFneckForCurv;

	cutPln.MakePlane(GVector3f(1.0, 0.0, 0.0), GVector3f(tmpFneckPt[0], tmpFneckPt[1], tmpFneckPt[2]));
	float limitY = FNv[1] + 8.0f;
	float limitY1 = FNv[1] - 4.0f;
	float limitX = FNv[0] + 10.0f;
	float limitX1 = FNv[0] - 10.0f;
	float limitZ = FNv[2] - 50.0f;

	float dis;
	minDis = 10000.0f;

	for (EgFace *f : pMesh->m_pFaces)
	{
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);

		GVector3f v0, v1, v2;
		v0 = GVector3f(p0[0], p0[1], p0[2]);
		v1 = GVector3f(p1[0], p1[1], p1[2]);
		v2 = GVector3f(p2[0], p2[1], p2[2]);

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
				// FneckPt = FindClosestVert(pMesh, EgPos(CrossPt.X, CrossPt.Y, CrossPt.Z));
				CrossPtList.push_back(FindClosestVert(pMesh, EgPos(CrossPt.X, CrossPt.Y, CrossPt.Z))->m_Pos);
				minDis = dis;
			}
		}

		if (cutPln.GetSignedDistance(v1) * cutPln.GetSignedDistance(v2) < 0)
		{
			GVector3f CrossPt = cutPln.GetProjectionPt(v1, v2 - v1);
			dis = ABS(FNv.Y - CrossPt.Y); //ABS(FNeckPt.X-CrossPt.X);
			if (dis < minDis) {
				// FneckPt = FindClosestVert(pMesh, EgPos(CrossPt.X, CrossPt.Y, CrossPt.Z));
				CrossPtList.push_back(FindClosestVert(pMesh, EgPos(CrossPt.X, CrossPt.Y, CrossPt.Z))->m_Pos);
				minDis = dis;
			}
		}
		if (cutPln.GetSignedDistance(v2) * cutPln.GetSignedDistance(v0) < 0)
		{
			GVector3f CrossPt = cutPln.GetProjectionPt(v0, v2 - v0);
			dis = ABS(FNv.Y - CrossPt.Y); //ABS(FNeckPt.X-CrossPt.X);
			if (dis < minDis) {
				// FneckPt = FindClosestVert(pMesh, EgPos(CrossPt.X, CrossPt.Y, CrossPt.Z));
				CrossPtList.push_back(FindClosestVert(pMesh, EgPos(CrossPt.X, CrossPt.Y, CrossPt.Z))->m_Pos);
				minDis = dis;
			}
		}
	} //for

	if (!CrossPtList.empty()) {
		EgPos maxZ = EgPos(0, 0, -INFINITY);
		for (EgPos pos : CrossPtList) {
			if (pos.m_Coords[2] > maxZ.m_Coords[2]) {
				maxZ = pos;
			}
		}

		// ����� �̵�
		maxZ.m_Coords[0] = (m_MinX + m_MaxX) / 2;
		FneckPt = FindClosestVert(pMesh, maxZ);
	}


	if (FneckPt != NULL) {
		if (FneckPt->m_Pos[1] > tmpFneckPt.m_Coords[1]) {
			// ����� �̵�
			tmpFneckPt.m_Coords[0] = (m_MinX + m_MaxX) / 2;
			FneckPt = FindClosestVert(pMesh, tmpFneckPt);
		}

		m_LandmarkPose1[POSE1_LANDMARK_NECK_FRONT] = FneckPt;
		_cprintf("Neck Front Pt : ");
		printVert(FneckPt);
	}
	else {
		_cprintf("Neck Front Pt : NOT FOUND!");
		if (tmpFneckPt != NULL) {
			_cprintf(" Replacing...\n");

			FneckPt = FindClosestVert(pMesh, tmpFneckPt);
			m_LandmarkPose1[POSE1_LANDMARK_NECK_FRONT] = FneckPt;
			_cprintf("Neck Front Pt : ");
			printVert(FneckPt);
		}
		else {
			_cprintf("\n");
			m_LandmarkPose1[POSE1_LANDMARK_NECK_FRONT] = NULL;
		}
	}
}


/////////////////////////////////////// ������::�ڳ���
// ������ ������ : 2020. 10. 11
// ToDo::�Լ��� Ƣ����ִ� ��� �ִ�z���� �Լ��� �� ���� �ִ� ����
// ToDo::����ũ ������ �ڳ��� ��ġ�� �߸� ã������ ���� (20. 11. 21)
void Pose1_FindNosePt(EgMesh *pMesh) {
	EgVertex *TopHeadPt = m_LandmarkPose1[POSE1_LANDMARK_TOPHEAD],
		*FneckPt = m_LandmarkPose1[POSE1_LANDMARK_NECK_FRONT];

	EgVertex *NosePt = NULL;

	if (TopHeadPt == NULL || FneckPt == NULL) {
		_cprintf("Error in finding Nose... Exiting...\n");
		return;
	}

	EgPos maxZ(0, 0, -INFINITY);
	for (EgVertex *v : m_SagittalPoints_Front) {
		if (v->m_Pos[1] > FneckPt->m_Pos[1]) {
			if (v->m_Pos[2] > maxZ.m_Coords[2]) {
				NosePt = v;
				maxZ = v->m_Pos;
			}
		}
	}


	if (NosePt != NULL) {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_NOSE] = NosePt;
		_cprintf("Nose Pt : ");
		printVert(NosePt);
	}
	else {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_NOSE] = NULL;
		_cprintf("Nose Pt : Not Found!\n");
	}
}


/////////////////////////////////////// ������::�γ���
// ������ ������ : 2020. 12. 3
// ����� �������� ���߾ӿ��� Ž��
void Pose1_FindJawPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_JAW])
		return;

	EgVertex *JawPt = NULL;
	EgVertex *FneckPt = m_LandmarkPose1[POSE1_LANDMARK_NECK_FRONT],
		*NosePt = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_NOSE];

	if (FneckPt == NULL || NosePt == NULL) {
		_cprintf("Error in finding Jaw... Exiting...\n");
		return;
	}

	float maxAngle = -INFINITY;
	EgLine FneckPt_Y(EgPos(0, FneckPt->m_Pos[1], FneckPt->m_Pos[2]), EgPos(0, FneckPt->m_Pos[1] + 50, FneckPt->m_Pos[2]));
	EgPos tmpJawPt;

	for (EgVertex *pos : m_SagittalPoints_Front) {
		// �����~�ڳ��� ���̿� �ִ� ���� �߿���
		if (pos->m_Pos[1] > FneckPt->m_Pos[1] + 30 && pos->m_Pos[1] < NosePt->m_Pos[1]) {
			// ���� (��������κ��� y�� ������ ���Ϳ� �����~pos�� �̷�� ������ ������ �ִ밡 �Ǵ� ��)
			EgLine ray(EgPos(0, FneckPt->m_Pos[1], FneckPt->m_Pos[2]), EgPos(0, pos->m_Pos[1], pos->m_Pos[2]));
			float thisAngle = angle(FneckPt_Y.m_Dir, ray.m_Dir, false);

			if (thisAngle > maxAngle) {
				maxAngle = thisAngle;
				tmpJawPt = pos->m_Pos;
			}
		}
	}
	JawPt = FindClosestVert(pMesh, tmpJawPt);

	if (JawPt != NULL) {
		m_LandmarkPose1[POSE1_LANDMARK_JAW] = JawPt;
		_cprintf("Jaw Pt : ");
		printVert(JawPt);
	}
	else {
		_cprintf("Jaw Pt : NOT FOUND! Replacing... \n");
		JawPt = FindClosestVert(pMesh, EgPos(NosePt->m_Pos[0], NosePt->m_Pos[1] - 60, NosePt->m_Pos[2]));
		m_LandmarkPose1[POSE1_LANDMARK_JAW] = JawPt;
		_cprintf("Jaw Pt : ");
		printVert(JawPt);
	}
}


/////////////////////////////////////// ������::�����̵�����
/////////////////////////////////////// ������::�����������
// ������ ������ : 2020. 10. 11
void Pose1_FindHipnCalfExtrudePt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_HIP_EXTRUDE] && !m_bFindLandmark[POSE1_LANDMARK_CALF_EXTRUDE])
		return;

	EgVertex *HipExtrudePt = NULL, *CalfExtrudePt = NULL;

	EgSlicer upperSlicer, lowerSlicer;
	EgPos hip_minZ(0, 0, INFINITY), calf_minZ(0, 0, INFINITY);

	if (m_bFindLandmark[POSE1_LANDMARK_HIP_EXTRUDE]) {
		// �����̵����� : ��� Ž�� ����
		for (float i = 1.0f; i >= 0.5; i -= 0.1f) {
			int upperCount = upperSlicer.Slice(pMesh, 1, i, -1.0, 1.0, -1.0, 0.1, -1.0, 1.0, true);

			// ���ܸ鿡 ���� ���Ե� �� �����Ƿ� ���ܸ� �ѷ� ���̷� ������ ��,
			upperSlicer.SortByLength(true); // true: ��������

			// �ִ� �ѷ� ����(����ѷ�)�� ���� (upperSlicer.m_CutLines[0])
			// �� �� �ּ� z ���� ���� ���� �����̵��������� ����
			for (EgPos p : upperSlicer.m_CutLines[0]) {
				if (p.m_Coords[2] < hip_minZ.m_Coords[2] && p.m_Coords[0] < upperSlicer.GetCenterPos(0).m_Coords[0]) {
					hip_minZ = p;
				}
			}
		}
		HipExtrudePt = FindClosestVert(pMesh, hip_minZ);
		if (HipExtrudePt != NULL) {
			HipExtrudePt = SK_Extreme_Vertex(HipExtrudePt, 50.0f, 5, 1);
			m_LandmarkPose1[POSE1_LANDMARK_HIP_EXTRUDE] = HipExtrudePt;
			_cprintf("Hip Extrude Pt : ");
			printVert(HipExtrudePt);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_HIP_EXTRUDE] = NULL;
			_cprintf("Hip Extrude Pt : NOT FOUND!\n");
		}
	}


	if (m_bFindLandmark[POSE1_LANDMARK_CALF_EXTRUDE]) {
		// ����������� : �ϴ� Ž�� ����
		// Ž�� ������ ���Ƿ� 0.1���� �����Ѵ�
		// B/C �޲�ġ�� ã������ ��찡 �־ ����
		for (float i = 0.5f; i > 0.1f; i -= 0.1f) {
			int lowerCount = lowerSlicer.Slice(pMesh, 1, i, -1.0, 1.0, -1.0, 0.1, -1.0, 1.0, true);

			lowerSlicer.SortByLength(true);
			int rightIdx = 0;
			if (lowerSlicer.GetNumCutLine() > 1) {
				if (lowerSlicer.GetCenterPos(1).m_Coords[0] < lowerSlicer.GetCenterPos(0).m_Coords[0])
					rightIdx = 1;
			}

			for (EgPos p : lowerSlicer.m_CutLines[rightIdx]) {
				if (p.m_Coords[2] < calf_minZ.m_Coords[2] && p.m_Coords[0] < lowerSlicer.GetCenterPos(rightIdx).m_Coords[0]) {
					calf_minZ = p;
				}
			}
		}

		CalfExtrudePt = FindClosestVert(pMesh, calf_minZ);

		if (CalfExtrudePt != NULL) {
			CalfExtrudePt = SK_Extreme_Vertex(CalfExtrudePt, 50.0f, 5, 1);
			m_LandmarkPose1[POSE1_LANDMARK_CALF_EXTRUDE] = CalfExtrudePt;
			_cprintf("Calf Extrude Pt : ");
			printVert(CalfExtrudePt);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_CALF_EXTRUDE] = NULL;
			_cprintf("Calf Extrude Pt : NOT FOUND!\n");
		}
	}
}


/////////////////////////////////////// ������::��������
// ������ ������ : 2020. 10. 19
void Pose1_FindUnderHipPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_UNDERHIP])
		return;

	EgVertex *UnderHipPt = NULL;
	EgVertex *HipExtrudePt = m_LandmarkPose1[POSE1_LANDMARK_HIP_EXTRUDE],
		*CalfExtrudePt = m_LandmarkPose1[POSE1_LANDMARK_CALF_EXTRUDE];

	if (HipExtrudePt == NULL || CalfExtrudePt == NULL) {
		_cprintf("Error in finding Under hip... Exiting...\n");
		return;
	}
	
	EgSlicer theSlicer;
	theSlicer.SetCutPlane(EgPlane(EgVec3(1, 0, 0), HipExtrudePt->m_Pos));
	theSlicer.Slice(pMesh);
	theSlicer.SortByLength(true);

	std::vector<EgPos> underhipSilhouette, SortedPts;
	for (EgPos pos : theSlicer.m_CutLines[0]) {
		if (pos.m_Coords[1] < HipExtrudePt->m_Pos[1] && pos.m_Coords[1] > (HipExtrudePt->m_Pos[1] + CalfExtrudePt->m_Pos[1]) / 2
			&& pos.m_Coords[2] < theSlicer.GetCenterPos(0).m_Coords[2])
			underhipSilhouette.push_back(pos);
	}

	SK_Sort_By_Curvature(underhipSilhouette, SortedPts, true, false, EgVec3());
	UnderHipPt = FindClosestVert(pMesh, SortedPts[0]);

	if (UnderHipPt != NULL) {
		m_LandmarkPose1[POSE1_LANDMARK_UNDERHIP] = UnderHipPt;
		_cprintf("Under Hip Pt : ");
		printVert(UnderHipPt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_UNDERHIP] = UnderHipPt;
		_cprintf("Under Hip Pt : NOT FOUND!\n");
	}
}


/////////////////////////////////////// ������::�����(��/��)
// ������ ������ : 2020. 10. 19
void Pose1_FindNavelPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_NAVEL] && !m_bFindLandmark[POSE1_LANDMARK_NAVEL_BACK])
		return;

	EgVertex *NavelPt = NULL,
		*NavelPt_Back = NULL;

	EgLine ray(EgPos((m_MinX + m_MaxX) / 2, m_MaxY * 0.6, m_MaxZ), EgVec3(0, 0, -1));
	if (m_bFindLandmark[POSE1_LANDMARK_NAVEL]) {
		for (EgFace *f : pMesh->m_pFaces) {
			EgPos p0 = f->GetVertexPos(0);
			EgPos p1 = f->GetVertexPos(1);
			EgPos p2 = f->GetVertexPos(2);

			EgPos tmpIntersection;
			if (intersect_line_triangle(p0, p1, p2, ray, tmpIntersection, true)) {
				NavelPt = FindClosestVert(pMesh, tmpIntersection);
			}
		}

		//////////// �����(��)
		if (NavelPt != NULL) {
			// Hyperbolic���� ã���� ���� ���� ���� ���� ã�����Ƿ� Elliptic�� ã�� ���� ����
			NavelPt = SK_Elliptic_Vertex(NavelPt, 50.0);
			m_LandmarkPose1[POSE1_LANDMARK_NAVEL] = NavelPt;
			_cprintf("Navel Pt : ");
			printVert(NavelPt);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_NAVEL] = NULL;
			_cprintf("Navel Pt : NOT FOUND!\n");
		}
	}
	// ������� ���� �������� ��, NavelPt �Ҵ� �ʿ� (20. 12. 8)
	else {
		NavelPt = m_LandmarkPose1[POSE1_LANDMARK_NAVEL];
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////

	if (m_bFindLandmark[POSE1_LANDMARK_NAVEL_BACK]) {
		// ��ž������� �ڷ� ray�� ���� ������ ��
		EgPos tmpNavelPt_Back;
		ray = EgLine(NavelPt->m_Pos, EgVec3(0, 0, 1));
		for (EgFace *face : pMesh->m_pFaces) {
			EgPos p0 = face->GetVertexPos(0);
			EgPos p1 = face->GetVertexPos(1);
			EgPos p2 = face->GetVertexPos(2);

			EgPos tmpIntersection;
			if (intersect_line_triangle(p0, p1, p2, ray, tmpIntersection)) {
				tmpNavelPt_Back = tmpIntersection;
			}
		}

		//////////// �����(��)
		NavelPt_Back = FindClosestVert(pMesh, tmpNavelPt_Back);
		if (NavelPt_Back != NULL) {
			m_LandmarkPose1[POSE1_LANDMARK_NAVEL_BACK] = NavelPt_Back;
			_cprintf("Navel Pt Back : ");
			printVert(NavelPt_Back);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_NAVEL_BACK] = NULL;
			_cprintf("Navel Pt Back : NOT FOUND!\n");
		}
	}
}


/////////////////////////////////////// ������::�赹����
// ������ ������ : 2020. 10. 24
// �赹���� Ž������ �� ���� ū z���� ���� ��
void Pose1_FindStomachExtrudePt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_STOMACH_EXTRUDE])
		return;

	EgVertex* StomachExtrudePt = NULL;

	float upperStomachBound = m_LandmarkPose1[POSE1_LANDMARK_NAVEL]->m_Pos[1] + 10.0;
	float lowerStomachBound = m_LandmarkPose1[POSE1_LANDMARK_NAVEL]->m_Pos[1] - 100.0;

	// float upperStomachBound = m_MaxY * 0.63; // �赹���� Ž������ ���
	// float lowerStomachBound = m_MaxY * 0.49; // �赹���� Ž������ �ϴ�

	EgLine ray(EgPos((m_MinX + m_MaxX)/2, (upperStomachBound + lowerStomachBound)/2, m_MaxZ), EgVec3(0, 0, -1));

	for (EgFace *f : pMesh->m_pFaces) {
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);

		EgPos tmpIntersection;
		if (intersect_line_triangle(p0, p1, p2, ray, tmpIntersection, true)) {
			StomachExtrudePt = FindClosestVert(pMesh, tmpIntersection);
		}
	}

	if (StomachExtrudePt != NULL) {
		StomachExtrudePt = SK_Extreme_Vertex(StomachExtrudePt, abs(upperStomachBound - lowerStomachBound)/2, 4, 1);

		m_LandmarkPose1[POSE1_LANDMARK_STOMACH_EXTRUDE] = StomachExtrudePt;
		_cprintf("Stomach Pt : ");
		printVert(StomachExtrudePt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_STOMACH_EXTRUDE] = NULL;
		_cprintf("Stomach Pt : NOT FOUND!\n");
	}
}


/////////////////////////////////////// ������::�����������
/////////////////////////////////////// ������::����������
/////////////////////////////////////// ������::������
// ������ ������ : 2020. 11. 23
void Pose1_FindKneePt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_KNEE_MID]
		&& !m_bFindLandmark[POSE1_LANDMARK_KNEE_SIDE]
		&& !m_bFindLandmark[POSE1_LANDMARK_POPLITEUS])
		return;

	EgVertex *KneePt = NULL,
		*KneeSidePt = NULL,
		*PopliteusPt = NULL;
	EgVertex *CalfExtrudePt = m_LandmarkPose1[POSE1_LANDMARK_CALF_EXTRUDE];

	if (CalfExtrudePt == NULL) {
		_cprintf("Error in finding Knee... Exiting...\n");
		return;
	}

	std::vector<EgPos> kneeLevelSection;
	EgSlicer kneeSlicer;
	int rightIdx = 0;
	float minPerimeter = INFINITY;
	EgPos maxZ = EgPos(0, 0, -INFINITY);
	EgPos maxX = EgPos(-INFINITY, 0, 0);

	for (float i = 0.29; i >= 0.23; i -= 0.01) {
		int sectionNum = kneeSlicer.Slice(pMesh, 1, i, -1, 1, -1, 1, -1, 1, true);
		rightIdx = 0;

		if (sectionNum > 1) {
			if (kneeSlicer.GetCenterPos(1).m_Coords[0] < kneeSlicer.GetCenterPos(0).m_Coords[0])
				rightIdx = 1;
		}

		float perimeter = kneeSlicer.GetLength(rightIdx);

		if (perimeter < minPerimeter) {
			minPerimeter = perimeter;
			kneeLevelSection = kneeSlicer.m_CutLines[rightIdx];
		}
	}

	// ������ �ܸ��� �ִ� z���� ���� �����κ��� 40mm(GSizer�� ��) �ݰ濡�� �ִ� z���� ���� ��
	for (EgPos pos : kneeLevelSection) {
		if (pos.m_Coords[0] < (m_MinX + m_MaxX) / 2 && pos.m_Coords[2] > kneeSlicer.GetCenterPos(rightIdx).m_Coords[2]) {
			if (pos.m_Coords[0] > maxX.m_Coords[0]) {
				if (pos.m_Coords[2] > maxZ.m_Coords[2]) {
					maxX = pos;
					maxZ = pos;
				}
			}
		}
	}

	if (m_bFindLandmark[POSE1_LANDMARK_KNEE_MID]) {
		EgPos tmpKneePt = maxZ;
		tmpKneePt.m_Coords[1] += 40.0;
		KneePt = FindClosestVert(pMesh, tmpKneePt);

		/////// �����������
		if (KneePt != NULL) {
			KneePt = SK_Extreme_Vertex(KneePt, 10.0, 4, 1);

			m_LandmarkPose1[POSE1_LANDMARK_KNEE_MID] = KneePt;
			_cprintf("Knee Pt : ");
			printVert(KneePt);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_KNEE_MID] = NULL;
			_cprintf("Knee Pt : NOT FOUND!\n");
		}
	}
	else {
		KneePt = m_LandmarkPose1[POSE1_LANDMARK_KNEE_MID];
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// kneeLevelSection���� �ִ� x���� ���� ���� seed point�� �ݰ� 5cm �� �ִ� x���� ���� ��
	if (m_bFindLandmark[POSE1_LANDMARK_KNEE_SIDE]) {
		KneeSidePt = FindClosestVert(pMesh, maxX);

		/////// ����������
		if (KneeSidePt != NULL) {
			KneeSidePt = SK_Extreme_Vertex(KneeSidePt, 20.0f, 0, 1);


			m_LandmarkPose1[POSE1_LANDMARK_KNEE_SIDE] = KneeSidePt;
			_cprintf("Knee Side Pt : ");
			printVert(KneeSidePt);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_KNEE_SIDE] = NULL;
			_cprintf("Knee Side Pt : NOT FOUND!\n");
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (m_bFindLandmark[POSE1_LANDMARK_POPLITEUS]) {
		EgLine rayPopliteus(KneePt->m_Pos, EgVec3(0, 0, 1));

		// ��������������� �������� ray�� ���� ������ ���������� 1cm ������ ������ ��
		EgPos tmpPopliteusPt;
		for (EgFace *face : pMesh->m_pFaces) {
			EgPos p0 = face->GetVertexPos(0);
			EgPos p1 = face->GetVertexPos(1);
			EgPos p2 = face->GetVertexPos(2);

			EgPos tmpIntersection;
			if (intersect_line_triangle(p0, p1, p2, rayPopliteus, tmpIntersection, true)) {
				tmpPopliteusPt = tmpIntersection;
			}
		}

		tmpPopliteusPt.m_Coords[1] -= 10.0;

		/////// ������
		PopliteusPt = FindClosestVert(pMesh, tmpPopliteusPt);
		if (PopliteusPt != NULL) {
			m_LandmarkPose1[POSE1_LANDMARK_POPLITEUS] = PopliteusPt;
			_cprintf("Popliteus pt : ");
			printVert(PopliteusPt);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_POPLITEUS] = NULL;
			_cprintf("Popliteus pt : NOT FOUND!\n");
		}
	}
}


/////////////////////////////////////// ������::����
// ������ ������ : 2020. 10. 19
// ���� �ؿ� �� �ٸ��� �پ ��ĵ�� �������� ���ϴ������� ���� ���� ���� �κб����� �������� �Ÿ��� ���� �� ��
void Pose1_FindCrotchPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_CROTCH])
		return;

	EgVertex *CrotchPt = NULL;
	EgVertex *StomachExtrudePt = m_LandmarkPose1[POSE1_LANDMARK_STOMACH_EXTRUDE],
		*KneePt = m_LandmarkPose1[POSE1_LANDMARK_KNEE_MID];

	if (StomachExtrudePt == NULL || KneePt == NULL) {
		_cprintf("Error in finding Crotch... Exiting...\n");
		return;
	}

	// �赹���� ���� ���� ~ �پ ��ĵ�� �ٸ� �κ��� ������ Sagittal slicer ����.
	EgSlicer sagittalSlicer;
	sagittalSlicer.Slice(pMesh, 0, 0.5, -1, 1, -1, 1, -1, 1, false);

	// Sagittal slicer���� ���ϴ� ���� ���ϰ�,
	EgPos lowestCrotchPt;
	EgPos minY = EgPos(0, INFINITY, 0);

	for (EgPos pos : sagittalSlicer.m_CutLines[0]) {
		if (pos.m_Coords[1] < minY.m_Coords[1]) {
			lowestCrotchPt = pos;
			minY.m_Coords[1] = pos.m_Coords[1];
		}
	}

	// lowestCrotchPt�� �������� �赹���� ������ �յڷ� ���� silhouette�� �����.
	std::vector<EgPos> frontalSilhouette;
	for (EgPos pos : sagittalSlicer.m_CutLines[0]) {
		if (pos.m_Coords[2] > sagittalSlicer.GetCenterPos(0).m_Coords[2] && pos.m_Coords[2] > lowestCrotchPt.m_Coords[2] 
			&& pos.m_Coords[1] < StomachExtrudePt->m_Pos[1] && pos.m_Coords[1] > KneePt->m_Pos[1] + 200.0) {
			frontalSilhouette.push_back(pos);
		}
	}

	// ���ϴ� ���� �赹������ ������ ��� ���� ��,
	EgLine tmp_l = EgLine(lowestCrotchPt, StomachExtrudePt->m_Pos);

	// frontal silhouette�� �� �� �Ÿ��� ���� �� ���� �������� ������ �����Ѵ�.
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
	CrotchPt = FindClosestVert(pMesh, tmpCrotch);

	// ���������� ������ ���� �����Ƿ� ������ ���� �׻� �տ��� ���ϵ��� ó��
	EgLine ray(EgPos(CrotchPt->m_Pos[0], CrotchPt->m_Pos[1], m_MaxZ), EgVec3(0, 0, -1));
	for (EgFace *f : pMesh->m_pFaces) {
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);

		EgPos tmpIntersection;
		if (intersect_line_triangle(p0, p1, p2, ray, tmpIntersection, true)) {
			CrotchPt = FindClosestVert(pMesh, tmpIntersection);
		}
	}

	if (CrotchPt != NULL) {
		m_LandmarkPose1[POSE1_LANDMARK_CROTCH] = CrotchPt;
		_cprintf("Crotch Pt : ");
		printVert(CrotchPt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_CROTCH] = NULL;
		_cprintf("Crotch Pt : NOT FOUND!\n");

	}
}


/////////////////////////////////////// ������::�����
// ������ ������ : 2020. 10. 12
void Pose1_FindBackNeckPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_NECK_BACK])
		return;

	EgVertex *BneckPt = NULL;
	EgVertex *FneckPt = m_LandmarkPose1[POSE1_LANDMARK_NECK_FRONT],
		*SideNeckPt = m_LandmarkPose1[POSE1_LANDMARK_NECK_SIDE];

	if (FneckPt == NULL || SideNeckPt == NULL) {
		_cprintf("Error in finding Back neck... Exiting...\n");
		return;
	}

	// ���� ����� Ž�� ����
	float upperBneckBound = SideNeckPt->m_Pos[1] + 33;
	float lowerBneckBound = SideNeckPt->m_Pos[1] - 10;

	EgLine ray(FneckPt->m_Pos, EgVec3(0, 0, -1));
	EgPos minZ = EgPos(0, 0, INFINITY);
	for (EgFace *f : pMesh->m_pFaces) {
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);

		EgPos tmpIntersection;
		if (intersect_line_triangle(p0, p1, p2, ray, tmpIntersection, false)) {
			if (tmpIntersection.m_Coords[2] < minZ.m_Coords[2]) {
				minZ = tmpIntersection;
			}
		}
	}
	BneckPt = FindClosestVert(pMesh, minZ);

	// ����� Ž�� ���� ���� �̵�
	if (BneckPt->m_Pos[1] < lowerBneckBound || BneckPt->m_Pos[1] > upperBneckBound) {
		BneckPt = FindClosestVert(pMesh, EgPos(BneckPt->m_Pos[0], (upperBneckBound + lowerBneckBound) / 2, BneckPt->m_Pos[2]));
	}


	if (BneckPt != NULL) {
		m_LandmarkPose1[POSE1_LANDMARK_NECK_BACK] = BneckPt;
		_cprintf("Back Neck Pt : ");
		printVert(BneckPt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_NECK_BACK] = NULL;
		_cprintf("Back Neck Pt : NOT FOUND!\n");
	}
}


/////////////////////////////////////// ������::�㸮(��)��
/////////////////////////////////////// ������::�㸮����
/////////////////////////////////////// ������::�㸮����
// ������ ������ : 2020. 12. 3
void Pose1_FindWaistPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_WAIST]
		&& !m_bFindLandmark[POSE1_LANDMARK_WAIST_FRONT]
		&& !m_bFindLandmark[POSE1_LANDMARK_WAIST_BACK])
		return;

	EgVertex *WaistPt = NULL,
		*WaistFrontPt = NULL,
		*WaistBackPt = NULL;

	EgVertex *ArmpitPt_Front_R = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_R],
		*ArmpitPt_Front_L = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_L];

	if (ArmpitPt_Front_R == NULL || ArmpitPt_Front_L == NULL) {
		_cprintf("Error in finding Waist... Exiting...\n");
		return;
	}

	float upperWaistBound = m_MaxY * 0.66;
	float lowerWaistBound = m_MaxY * 0.57;

	if (m_bFindLandmark[POSE1_LANDMARK_WAIST]) {
		std::vector<EgPos> waistCoronal;
		EgSlicer theSlicer;
		for (float level = upperWaistBound; level >= lowerWaistBound; level -= 5.0f) {
			theSlicer.Slice(pMesh, 1, level / m_MaxY, -1, 1, -1, 1, -1, 1, true);
			theSlicer.Slice(pMesh);
			theSlicer.SortByLength(true);

			EgPos minX(INFINITY, 0, 0);

			for (EgPos pos : theSlicer.m_CutLines[0]) {
				if (pos.m_Coords[0] < minX.m_Coords[0]) {
					minX = pos;
				}
			}

			if (minX != EgPos(INFINITY, 0, 0) && minX.m_Coords[0] <= theSlicer.GetCenterPos(0).m_Coords[0] && minX.m_Coords[0] >= ArmpitPt_Front_R->m_Pos[0])
				waistCoronal.push_back(minX);
		}


		// upperWaistBound ~ waistSilhouette ~ lowerWaistBound �� ������ ���� ū �� || �����㸮�� ��� ������ 0�� �� ?
		double maxAngle = -INFINITY;
		for (EgPos pos : waistCoronal) {
			// upperWaistBound ~ pos
			EgVec3 upper2pos = upperWaistBound - pos;

			// pos ~ lowerWaistBound
			EgVec3 pos2lower = lowerWaistBound - pos;

			upper2pos.m_Coords[2] = 0.0;
			pos2lower.m_Coords[2] = 0.0;

			float currAngle = angle(upper2pos, pos2lower, false);
			if (currAngle > maxAngle) {
				maxAngle = currAngle;
				WaistPt = FindClosestVert(pMesh, pos);
			}
		}

		if (WaistPt != NULL) {
			// �㸮�� �ʹ� ���ų� ���� ��� ���ġ�� �����Ѵ�
			float upperBound, lowerBound, avgBound;
			if (m_bWoman) {
				upperBound = 0.652;
				lowerBound = 0.578;
				avgBound = 0.615;
			}
			else {
				upperBound = 0.649;
				lowerBound = 0.575;
				avgBound = 0.612;
			}

			if (WaistPt->m_Pos[1] > m_MaxY * upperBound || WaistPt->m_Pos[1] < m_MaxY * lowerBound) {
				WaistPt->m_Pos[1] = m_MaxY * avgBound;
			}

			m_LandmarkPose1[POSE1_LANDMARK_WAIST] = WaistPt;
			_cprintf("Waist Pt : ");
			printVert(WaistPt);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_WAIST] = NULL;
			_cprintf("Waist Pt : NOT FOUND!\n");
		}
	}
	// �㸮���� ���� ���� ���, WaistPt�� �Ҵ����ش� (20. 12. 8)
	else {
		WaistPt = m_LandmarkPose1[POSE1_LANDMARK_WAIST];
	}


	EgPos tmpWaistFrontPt, tmpWaistBackPt;
	EgPos maxZ = EgPos(0, 0, -INFINITY), minZ = EgPos(0, 0, INFINITY);
	if (m_bFindLandmark[POSE1_LANDMARK_WAIST_FRONT] || m_bFindLandmark[POSE1_LANDMARK_WAIST_BACK]) {
		// �㸮(��)���� ������ Ⱦ�ܸ��� EgSlicer�� ����������� 
		EgPlane waistLevelPlane = EgPlane(EgVec3(0.0, 1.0, 0.0), WaistPt->m_Pos);
		EgSlicer waistSlicer;
		waistSlicer.SetCutPlane(waistLevelPlane);

		waistSlicer.Slice(pMesh);

		for (EgPos pos : waistSlicer.m_CutLines[0]) {
			if (pos.m_Coords[2] > maxZ.m_Coords[2]) {
				maxZ.m_Coords[2] = pos.m_Coords[2];
				tmpWaistFrontPt = pos;
			}

			if (pos.m_Coords[2] < minZ.m_Coords[2]) {
				minZ.m_Coords[2] = pos.m_Coords[2];
				tmpWaistBackPt = pos;
			}
		}
	}

	if (m_bFindLandmark[POSE1_LANDMARK_WAIST_FRONT]) {
		// ��� �������� �̵�
		tmpWaistFrontPt.m_Coords[0] = (m_MaxX + m_MinX) / 2;
		WaistFrontPt = FindClosestVert(pMesh, tmpWaistFrontPt);
		if (WaistFrontPt != NULL) {
			m_LandmarkPose1[POSE1_LANDMARK_WAIST_FRONT] = WaistFrontPt;
			_cprintf("Waist Front Pt : ");
			printVert(WaistFrontPt);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_WAIST_FRONT] = NULL;
			_cprintf("Waist Front Pt : NOT FOUND!\n");
		}
	}


	if (m_bFindLandmark[POSE1_LANDMARK_WAIST_BACK]) {
		// ��� �������� �̵�
		tmpWaistBackPt.m_Coords[0] = (m_MaxX + m_MinX) / 2;
		WaistBackPt = FindClosestVert(pMesh, tmpWaistBackPt);
		if (WaistBackPt != NULL) {
			m_LandmarkPose1[POSE1_LANDMARK_WAIST_BACK] = WaistBackPt;
			_cprintf("Waist Back Pt : ");
			printVert(WaistBackPt);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_WAIST_BACK] = NULL;
			_cprintf("Waist Back Pt : NOT FOUND!\n");
		}
	}
}


/////////////////////////////////////// ������::���ʺ�����
// ������ ������ : 2020. 11. 23
void Pose1_FindAnklePt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_ANKLE])
		return;

	EgVertex *AnklePt = NULL;

	// (����� ������:�ٴ�)=2:1 �������� ������ ���ܸ� �ѷ� ���̰� ���� ���� ������ �ּ� x���� ���� ��
	EgPos CalfExtrudePt = m_LandmarkPose1[POSE1_LANDMARK_CALF_EXTRUDE]->m_Pos;
	float lowerAnkleLevel = m_MaxY * 0.02;

	if (CalfExtrudePt == EgPos()) {
		_cprintf("Error in finding Ankle... Exiting...\n");
		return;
	}


	// 10mm ������ �������鼭 ã�´� 
	EgSlicer ankleSlicer;

	float minCirc = INFINITY;
	float startLvl = (m_MinY + CalfExtrudePt.m_Coords[1]) / 2;
	float minCircLvl = startLvl;

	for (float level = startLvl; level > lowerAnkleLevel; level -= 10.0) {
		ankleSlicer.Slice(pMesh, 1, level / m_MaxY, -1, 0, -1, 0, -1, 1, true);

		float thisCirc = ankleSlicer.GetLength(0);

		if (thisCirc < minCirc) {
			minCirc = thisCirc;
			minCircLvl = level;
		}
	}

	ankleSlicer.Slice(pMesh, 1, minCircLvl / m_MaxY, -1, 0, -1, 0, -1, 1, true);

	// �ּ� x���� ���� ��
	EgPos minX = EgPos(INFINITY, 0, 0);
	for (EgPos pos : ankleSlicer.m_CutLines[0]) {
		if (pos.m_Coords[0] < minX.m_Coords[0]) {
			minX = pos;
		}
	}

	AnklePt = FindClosestVert(pMesh, minX);

	// if (furthestPt != NULL) {
	if (AnklePt != NULL) {
		AnklePt = SK_Elliptic_Vertex(AnklePt, 30);

		m_LandmarkPose1[POSE1_LANDMARK_ANKLE] = AnklePt;
		_cprintf("Ankle Pt : ");
		printVert(AnklePt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_ANKLE] = NULL;
		_cprintf("Ankle Pt : NOT FOUND!\n");
	}
}


/////////////////////////////////////// ������::��������
// ������ ������ : 2020. 11. 21
void Pose1_FindBustPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_BP_R] && !m_bFindLandmark[POSE1_LANDMARK_BP_L])
		return;

	EgVertex *BustPt_R = NULL, 
		*BustPt_L = NULL;

	float upperBustBound = m_MaxY * 0.76;
	float lowerBustBound = m_MaxY * 0.67;

	float centerX = (m_MaxX + m_MinX) / 2;

	/////// Female
	// �ʺ� ���� ��ġ�� �� ����
	float BP_distance_avg = 0, chestWidth_avg = 0;
	// ���������̼������
	float lowerWidthBound = 0, upperWidthBound = 0;

	if (m_bWoman) {
		BP_distance_avg = 156; // (unit: mm)
		// �������ʺ�
		chestWidth_avg = BP_distance_avg / 0.520;

		lowerWidthBound = centerX - (chestWidth_avg * 0.26);
		upperWidthBound = centerX + (chestWidth_avg * 0.26);
	}
	else {
		/////// Male
		BP_distance_avg = 199;
		chestWidth_avg = BP_distance_avg / 0.614;

		lowerWidthBound = centerX - (chestWidth_avg * 0.307);
		upperWidthBound = centerX + (chestWidth_avg * 0.307);
	}

	// ���� �� �ʺ� Ž�� ���� ������ Elliptic Ž��
	// ���� ������ lowerWidthBound���� ������ �������� Ž��
	EgPos midPt_AllWay_R = EgPos(lowerWidthBound, (lowerBustBound + upperBustBound) / 2, 0);
	// ���� ������ upperWidthbound~�¿��������� ���� �������� Ž��
	EgPos midPt_AllWay_L = EgPos(upperWidthBound, (lowerBustBound + upperBustBound) / 2, 0);

	EgLine rayBustR(midPt_AllWay_R, EgVec3(0, 0, -1));
	EgLine rayBustL(midPt_AllWay_L, EgVec3(0, 0, -1));

	EgPos rightSeedPt = EgPos(), leftSeedPt = EgPos();
	for (EgFace *face : pMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		EgPos tmpIntersection = EgPos();
		if (intersect_line_triangle(p0, p1, p2, rayBustR, tmpIntersection, false)) {
			if (tmpIntersection.m_Coords[2] > (m_MinZ + m_MaxZ)/2)
				rightSeedPt = tmpIntersection;
		}
		if (intersect_line_triangle(p0, p1, p2, rayBustL, tmpIntersection, false)) {
			if (tmpIntersection.m_Coords[2] > (m_MinZ + m_MaxZ)/2)
				leftSeedPt = tmpIntersection;
		}
	}

	if (m_bFindLandmark[POSE1_LANDMARK_BP_R]) {
		BustPt_R = FindClosestVert(pMesh, rightSeedPt);
		// 7cm �ݰ��� Ž�� (3->7cm; 2020. 11. 21)
		if (BustPt_R != NULL) {
			BustPt_R = SK_Elliptic_Vertex(BustPt_R, 70);
			m_LandmarkPose1[POSE1_LANDMARK_BP_R] = BustPt_R;
			_cprintf("Bust Pt R : ");
			printVert(BustPt_R);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_BP_R] = NULL;
			_cprintf("Bust Pt R : NOT FOUND!\n");
		}
	}


	if (m_bFindLandmark[POSE1_LANDMARK_BP_L]) {
		BustPt_L = FindClosestVert(pMesh, leftSeedPt);
		if (BustPt_L != NULL) {
			BustPt_L = SK_Elliptic_Vertex(BustPt_L, 70);
			m_LandmarkPose1[POSE1_LANDMARK_BP_L] = BustPt_L;
			_cprintf("Bust Pt L : ");
			printVert(BustPt_L);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_BP_L] = NULL;
			_cprintf("Bust Pt L : NOT FOUND!\n");
		}
	}
}


//////////////////////////////////////////////// ��ġ ����
/////////////////////////////////////// ������::�ܵ���� (R/L)
/////////////////////////////////////// ������::�ܵ������ (R/L)
/////////////////////////////////////// ������::�ܵ������ (R/L)
/////////////////////////////////////// ������::�ܵ����������(R/L)
/////////////////////////////////////// ������::�ܵ����������(R/L)
// ������ ������ : 2020. 10. 27
void Pose1_ModifyArmpitWithShoudler(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_ARMPIT]
		&& !m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_FRONT_R]
		&& !m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_FRONT_L]
		&& !m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_BACK_R]
		&& !m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_BACK_L])
		return;

	EgVertex *BustPt = m_LandmarkPose1[POSE1_LANDMARK_BP_R],
		*ShoulderSidePt_R = m_LandmarkPose1[POSE1_LANDMARK_SHOULDERSIDE_R],
		*ShoulderSidePt_L = m_LandmarkPose1[POSE1_LANDMARK_SHOULDERSIDE_L],
		*ArmpitPt_R = m_LandmarkPose1[POSE1_LANDMARK_ARMPIT],
		*ArmpitPt_L = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_L],
		*ArmpitPt_Front_R = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_R],
		*ArmpitPt_Back_R = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_BACK_R],
		*ArmpitPt_Front_L = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_L],
		*ArmpitPt_Back_L = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_BACK_L],
		*ArmpitFoldPt_Front_R = m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_R],
		*ArmpitFoldPt_Back_R = m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_BACK_R],
		*ArmpitFoldPt_Front_L = m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_L],
		*ArmpitFoldPt_Back_L = m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_BACK_L];


	if (BustPt == NULL || ShoulderSidePt_R == NULL || ShoulderSidePt_L == NULL || ArmpitPt_R == NULL || ArmpitPt_L == NULL) {
		_cprintf("Error in modifying Armpit... Exiting...\n");
	}

	EgPlane pln(EgVec3(0, 1, 0), BustPt->m_Pos);
	std::vector<EgPos> tmpBpCirc;
	SK_Planar_Perimeter(
		BustPt,
		&pln,
		tmpBpCirc,
		false
	);

	// ���� �پ��ִ� �κ� ����
	std::vector<EgPos> bpCirc;
	for (EgPos pos : tmpBpCirc) {
		if (pos.m_Coords[0] >= ArmpitPt_R->m_Pos[0] && pos.m_Coords[0] <= ArmpitPt_L->m_Pos[0]) {
			bpCirc.push_back(pos);
		}
	}

	float bustCirc = 0;
	if (!bpCirc.empty()) {
		for (int i = 0; i < bpCirc.size() - 1; i++) {
			bustCirc += dist(bpCirc[i], bpCirc[i + 1]);
		}
	}


	float shoulderSide2armpit = 0;
	if (m_bWoman) {
		shoulderSide2armpit = 0.069 * bustCirc + 59.3;
	}
	else {
		shoulderSide2armpit = 0.077 * bustCirc + 57.7;
	}


	// ���� �ܵ����~��������������� �Ÿ��� ���ؼ�
	float currLevel_R = ArmpitPt_R->m_Pos[1],
		currLevel_L = ArmpitPt_L->m_Pos[1];


	// radius�� �� Ŭ ���, radius�� ������ ������(2mm ���� ����) ������ ������
	if (shoulderSide2armpit > ShoulderSidePt_R->m_Pos[1] - currLevel_R) {
		while(ABS((ShoulderSidePt_R->m_Pos[1] - currLevel_R) - shoulderSide2armpit) > 1.0) {
			currLevel_R -= 1.0;
		}
	}
	// radius�� �� ���� ���, radius�� ������ ������(2mm ���� ����) ������ �ø���
	else if (shoulderSide2armpit < ShoulderSidePt_R->m_Pos[1] - currLevel_R) {
		while (ABS((ShoulderSidePt_R->m_Pos[1] - currLevel_R)- shoulderSide2armpit) > 1.0) {
			currLevel_R += 1.0;
		}
	}


	if (shoulderSide2armpit > ShoulderSidePt_L->m_Pos[1] - currLevel_L) {
		while(ABS((ShoulderSidePt_L->m_Pos[1] - currLevel_L) - shoulderSide2armpit) > 1.0) {
			currLevel_L -= 1.0;
		}
	}
	// radius�� �� ���� ���, radius�� ������ ������(2mm ���� ����) ������ �ø���
	else if (shoulderSide2armpit < ShoulderSidePt_L->m_Pos[1] - currLevel_L) {
		while(ABS((ShoulderSidePt_L->m_Pos[1] - currLevel_L) - shoulderSide2armpit) > 1.0) {
			currLevel_L += 1.0;
		}
	}


	////////////////////  �ܵ���� (R)
	if (m_bFindLandmark[POSE1_LANDMARK_ARMPIT]) {
		// Mesh Vertex�� �ƴϹǷ� �׳� ��ǥ ���� �ٲ��ش� (20. 12. 8)
		// ArmpitPt_R = new EgVertex(ArmpitPt_R->m_Pos[0], currLevel_R, ArmpitPt_R->m_Pos[2]);
		ArmpitPt_R->m_Pos[1] = currLevel_R;
		m_LandmarkPose1[POSE1_LANDMARK_ARMPIT] = ArmpitPt_R;
		_cprintf("------- Modified Armpit (R) : ");
		printVert(ArmpitPt_R);

		////////////////////  �ܵ������ (R)
		ArmpitPt_Front_R = FindClosestVert(pMesh, EgPos(ArmpitPt_Front_R->m_Pos[0], currLevel_R, ArmpitPt_Front_R->m_Pos[2]));
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_R] = ArmpitPt_Front_R;
		_cprintf("------- Modified Armpit Front (R) : ");
		printVert(ArmpitPt_Front_R);


		////////////////////  �ܵ������ (R)
		ArmpitPt_Back_R = FindClosestVert(pMesh, EgPos(ArmpitPt_Back_R->m_Pos[0], currLevel_R, ArmpitPt_Back_R->m_Pos[2]));
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_BACK_R] = ArmpitPt_Back_R;
		_cprintf("------- Modified Armpit Back (R) : ");
		printVert(ArmpitPt_Back_R);
	}

	////////////////////  �ܵ���������� (R)
	if (m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_FRONT_R]) {
		ArmpitFoldPt_Front_R = FindClosestVert(pMesh, EgPos(ArmpitFoldPt_Front_R->m_Pos[0], currLevel_R + 15, ArmpitFoldPt_Front_R->m_Pos[2]));
		m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_R] = ArmpitFoldPt_Front_R;
		_cprintf("------- Modified Armpit Fold Front (R) : ");
		printVert(ArmpitFoldPt_Front_R);
	}

	////////////////////  �ܵ���������� (R)
	if (m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_BACK_R]) {
		ArmpitFoldPt_Back_R = FindClosestVert(pMesh, EgPos(ArmpitFoldPt_Back_R->m_Pos[0], currLevel_R + 20, ArmpitFoldPt_Back_R->m_Pos[2]));
		m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_BACK_R] = ArmpitFoldPt_Back_R;
		_cprintf("------- Modified Armpit Fold Back (R) : ");
		printVert(ArmpitFoldPt_Back_R);
	}



	////////////////////  �ܵ���� (L)
	// �׳� �ܵ����(R)�� ������ ��쿡 ���� �ٲ��ش� (20. 12. 8)
	if (m_bFindLandmark[POSE1_LANDMARK_ARMPIT]) {
		// Mesh Vertex�� �ƴϹǷ� �׳� ��ǥ ���� �ٲ��ش� (20. 12. 8)
		// ArmpitPt_L = new EgVertex(ArmpitPt_L->m_Pos[0], currLevel_L, ArmpitPt_L->m_Pos[2]);
		ArmpitPt_L->m_Pos[1] = currLevel_L;
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_L] = ArmpitPt_L;
		_cprintf("------- Modified Armpit (L) : ");
		printVert(ArmpitPt_L);

		////////////////////  �ܵ������ (L)
		ArmpitPt_Front_L = FindClosestVert(pMesh, EgPos(ArmpitPt_Front_L->m_Pos[0], currLevel_L, ArmpitPt_Front_L->m_Pos[2]));
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_L] = ArmpitPt_Front_L;
		_cprintf("------- Modified Armpit Front (L) : ");
		printVert(ArmpitPt_Front_L);

		////////////////////  �ܵ������ (L)
		ArmpitPt_Back_L = FindClosestVert(pMesh, EgPos(ArmpitPt_Back_L->m_Pos[0], currLevel_L, ArmpitPt_Back_L->m_Pos[2]));
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_BACK_L] = ArmpitPt_Back_L;
		_cprintf("------- Modified Armpit Back (L) : ");
		printVert(ArmpitPt_Back_L);
	}

	////////////////////  �ܵ���������� (L)
	if (m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_FRONT_L]) {
		ArmpitFoldPt_Front_L = FindClosestVert(pMesh, EgPos(ArmpitFoldPt_Front_L->m_Pos[0], currLevel_L + 15, ArmpitFoldPt_Front_L->m_Pos[2]));
		m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_FRONT_L] = ArmpitFoldPt_Front_L;
		_cprintf("------- Modified Armpit Fold Front (L) : ");
		printVert(ArmpitFoldPt_Front_L);
	}

	////////////////////  �ܵ���������� (L)
	if (m_bFindLandmark[POSE1_LANDMARK_ARMPITFOLD_BACK_L]) {
		ArmpitFoldPt_Back_L = FindClosestVert(pMesh, EgPos(ArmpitFoldPt_Back_L->m_Pos[0], currLevel_L + 20, ArmpitFoldPt_Back_L->m_Pos[2]));
		m_LandmarkPose1[POSE1_LANDMARK_ARMPITFOLD_BACK_L] = ArmpitFoldPt_Back_L;
		_cprintf("------- Modified Armpit Fold Back (L) : ");
		printVert(ArmpitFoldPt_Back_L);
	}
}


/////////////////////////////////////// ������::�ܵ���պ���(R/L)
/////////////////////////////////////// ������::�ܵ���ں���(R/L)
// ������ ������ : 2020. 7. 28
void Pose1_FindArmpitWall(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_ARMPITWALL_FRONT_R]
		&& !m_bFindLandmark[POSE1_LANDMARK_ARMPITWALL_FRONT_L]
		&& !m_bFindLandmark[POSE1_LANDMARK_ARMPITWALL_BACK_R]
		&& !m_bFindLandmark[POSE1_LANDMARK_ARMPITWALL_BACK_L])
		return;

	EgVertex *ShoulderSidePt_R = m_LandmarkPose1[POSE1_LANDMARK_SHOULDERSIDE_R],
		*ShoulderSidePt_L = m_LandmarkPose1[POSE1_LANDMARK_SHOULDERSIDE_L],
		*ArmpitPt_Front_R = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_R],
		*ArmpitPt_Front_L = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_FRONT_L],
		*ArmpitPt_Back_R = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_BACK_R],
		*ArmpitPt_Back_L = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_ARMPIT_BACK_L];

	EgVertex *ArmpitWallPt_Front_R = NULL,
		*ArmpitWallPt_Front_L = NULL,
		*ArmpitWallPt_Back_R = NULL,
		*ArmpitWallPt_Back_L = NULL;

	if (ShoulderSidePt_R == NULL 
		|| ShoulderSidePt_L == NULL
		|| ArmpitPt_Front_R == NULL
		|| ArmpitPt_Front_L == NULL
		|| ArmpitPt_Back_R == NULL
		|| ArmpitPt_Back_L == NULL) {
		_cprintf("Error in finding Armpit wall... Exiting...\n");
		return;
	}

	// ����������� �ܵ����|������ ������������ �� �� ������ ������ ������ Ž��
	EgPos pos_ArmpitWallPt_Front_R = EgPos(
		(ShoulderSidePt_R->m_Pos[0] + ArmpitPt_Front_R->m_Pos[0]) / 2,
		(ShoulderSidePt_R->m_Pos[1] + ArmpitPt_Front_R->m_Pos[1]) / 2,
		(ShoulderSidePt_R->m_Pos[2] + ArmpitPt_Front_R->m_Pos[2]) / 2
	),
		pos_ArmpitWallPt_Front_L = EgPos(
		(ShoulderSidePt_L->m_Pos[0] + ArmpitPt_Front_L->m_Pos[0]) / 2,
			(ShoulderSidePt_L->m_Pos[1] + ArmpitPt_Front_L->m_Pos[1]) / 2,
			(ShoulderSidePt_L->m_Pos[2] + ArmpitPt_Front_L->m_Pos[2]) / 2
		),
		pos_ArmpitWallPt_Back_R = EgPos(
		(ShoulderSidePt_R->m_Pos[0] + ArmpitPt_Back_R->m_Pos[0]) / 2,
			(ShoulderSidePt_R->m_Pos[1] + ArmpitPt_Back_R->m_Pos[1]) / 2,
			(ShoulderSidePt_R->m_Pos[2] + ArmpitPt_Back_R->m_Pos[2]) / 2
		),
		pos_ArmpitWallPt_Back_L = EgPos(
		(ShoulderSidePt_L->m_Pos[0] + ArmpitPt_Back_L->m_Pos[0]) / 2,
			(ShoulderSidePt_L->m_Pos[1] + ArmpitPt_Back_L->m_Pos[1]) / 2,
			(ShoulderSidePt_L->m_Pos[2] + ArmpitPt_Back_L->m_Pos[2]) / 2
		);


	EgLine rayFrontR = EgLine(pos_ArmpitWallPt_Front_R, EgVec3(0, 0, -1));
	EgLine rayFrontL = EgLine(pos_ArmpitWallPt_Front_L, EgVec3(0, 0, -1));

	EgLine rayBackR = EgLine(pos_ArmpitWallPt_Back_R, EgVec3(0, 0, 1));
	EgLine rayBackL = EgLine(pos_ArmpitWallPt_Back_L, EgVec3(0, 0, 1));

	EgPos tmpArmpitWallPt_Front_R, tmpArmpitWallPt_Front_L, tmpArmpitWallPt_Back_R, tmpArmpitWallPt_Back_L;
	for (EgFace* face : pMesh->m_pFaces) {
		EgPos p0 = face->GetVertexPos(0);
		EgPos p1 = face->GetVertexPos(1);
		EgPos p2 = face->GetVertexPos(2);

		EgPos tmpIntersection;
		if (intersect_line_triangle(p0, p1, p2, rayFrontR, tmpIntersection)) {
			tmpArmpitWallPt_Front_R = tmpIntersection;
		}

		if (intersect_line_triangle(p0, p1, p2, rayFrontL, tmpIntersection)) {
			tmpArmpitWallPt_Front_L = tmpIntersection;
		}

		if (intersect_line_triangle(p0, p1, p2, rayBackR, tmpIntersection)) {
			tmpArmpitWallPt_Back_R = tmpIntersection;
		}

		if (intersect_line_triangle(p0, p1, p2, rayBackL, tmpIntersection)) {
			tmpArmpitWallPt_Back_L = tmpIntersection;
		}
	}

	if (m_bFindLandmark[POSE1_LANDMARK_ARMPITWALL_FRONT_R]) {
		ArmpitWallPt_Front_R = FindClosestVert(pMesh, tmpArmpitWallPt_Front_R);
		if (ArmpitWallPt_Front_R != NULL) {
			m_LandmarkPose1[POSE1_LANDMARK_ARMPITWALL_FRONT_R] = ArmpitWallPt_Front_R;
			_cprintf("Armpit Wall Pt Front (R) : ");
			printVert(ArmpitWallPt_Front_R);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_ARMPITWALL_FRONT_R] = NULL;
			_cprintf("Armpit Wall Pt Front (R) : NOT FOUND!\n");
		}
	}

	if (m_bFindLandmark[POSE1_LANDMARK_ARMPITWALL_FRONT_L]) {
		ArmpitWallPt_Front_L = FindClosestVert(pMesh, tmpArmpitWallPt_Front_L);
		if (ArmpitWallPt_Front_L != NULL) {
			m_LandmarkPose1[POSE1_LANDMARK_ARMPITWALL_FRONT_L] = ArmpitWallPt_Front_L;
			_cprintf("Armpit Wall Pt Front (L) : ");
			printVert(ArmpitWallPt_Front_L);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_ARMPITWALL_FRONT_L] = NULL;
			_cprintf("Armpit Wall Pt Front (L) : NOT FOUND!\n");
		}
	}

	if (m_bFindLandmark[POSE1_LANDMARK_ARMPITWALL_BACK_R]) {
		ArmpitWallPt_Back_R = FindClosestVert(pMesh, tmpArmpitWallPt_Back_R);
		if (ArmpitWallPt_Back_R != NULL) {
			m_LandmarkPose1[POSE1_LANDMARK_ARMPITWALL_BACK_R] = ArmpitWallPt_Back_R;
			_cprintf("Armpit Wall Pt Back (R) : ");
			printVert(ArmpitWallPt_Back_R);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_ARMPITWALL_BACK_R] = NULL;
			_cprintf("Armpit Wall Pt Back (R) : NOT FOUND!\n");
		}
	}

	if (m_bFindLandmark[POSE1_LANDMARK_ARMPITWALL_BACK_L]) {
		ArmpitWallPt_Back_L = FindClosestVert(pMesh, tmpArmpitWallPt_Back_L);
		if (ArmpitWallPt_Back_L != NULL) {
			m_LandmarkPose1[POSE1_LANDMARK_ARMPITWALL_BACK_L] = ArmpitWallPt_Back_L;
			_cprintf("Armpit Wall Pt Back (L) : ");
			printVert(ArmpitWallPt_Back_L);
		}
		else {
			m_LandmarkPose1[POSE1_LANDMARK_ARMPITWALL_BACK_L] = NULL;
			_cprintf("Armpit Wall Pt Back (L) : NOT FOUND!\n");
		}
	}
}


/////////////////////////////////////// ������::�������Ʒ���
// ������ ������ : 2020. 9. 11
void Pose1_FindUnderBustPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_UNDERBUST])
		return;

	EgVertex *UnderBustPt = NULL;
	EgVertex *BustPt = m_LandmarkPose1[POSE1_LANDMARK_BP_R];

	if (BustPt == NULL) {
		_cprintf("Error in finding Under bust... Exiting...\n");
		return;
	}

	EgPos tmpBustPt = BustPt->m_Pos;

	// �����������κ��� ��������-�������Ʒ� �Ÿ� ��� ���� �߾�(55mm) ���� ��������
	// BustPt->m_Pos[1] -= 55.0f;
	// BustPt = FindClosestVert(pMesh, BustPt->m_Pos);
	tmpBustPt.m_Coords[1] -= 55.0f;

	// �ű⼭���� ���ġ �Ÿ� ���� �� (�� 20mm) �̳��� Ž�������� Hyperbolic�� ���� ã�´�
	if (BustPt != NULL) {
		// UnderBustPt = SK_Hyperbolic_Vertex(BustPt, 15);
		UnderBustPt = SK_Hyperbolic_Vertex(FindClosestVert(pMesh, tmpBustPt), 15);
		m_LandmarkPose1[POSE1_LANDMARK_UNDERBUST] = UnderBustPt;
		_cprintf("UnderBust Pt : ");
		printVert(UnderBustPt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_UNDERBUST] = NULL;
		_cprintf("UnderBust Pt : NOT FOUND!\n");
	}
}


/////////////////////////////////////// ������::�ճ���
// ������ ������ : 2020. 12. 3
void Pose1_FindHandPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_HAND])
		return;

	EgVertex *NavelPt = m_LandmarkPose1[POSE1_LANDMARK_NAVEL],
		*ArmpitPt = m_LandmarkPose1[POSE1_LANDMARK_ARMPIT];

	if (NavelPt == NULL || ArmpitPt == NULL) {
		_cprintf("Error in finding Hand... Exiting...\n");
		return;
	}

	// ����� - 5cm�� ������ y���� �������� �ϴ� ������� ���ܸ��� ���ϰ�
	EgPos minX(INFINITY, 0, 0);
	EgSlicer theSlicer;
	theSlicer.SetCutPlane(EgPlane(EgVec3(0, 1, 0), EgPos(NavelPt->m_Pos[0], NavelPt->m_Pos[1] - 50, NavelPt->m_Pos[2])));
	theSlicer.CvxSlice(pMesh);

	// ������ �� �ּ� x��ǥ�� ���� ������ ã�´�.
	for (EgPos p : theSlicer.m_CutLines[0])
		if (p[0] < minX[0])
			minX = p;
	EgVertex *HandPt = FindClosestVert(pMesh, minX);

	// ����� �ﰢ���� Ž���Ͽ� �ּ� y��ǥ�� ���� ������ ã�´�.
	if (HandPt != NULL) {
		HandPt = SK_Extreme_Vertex(HandPt, 30, 3, 11);

		m_LandmarkPose1[POSE1_LANDMARK_HAND] = HandPt;
		_cprintf("Hand Pt : ");
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_HAND] = NULL;
		_cprintf("Hand Pt : NOT FOUND!\n");
	}
}


/////////////////////////////////////// ������::�ո������
// ������ ������ : 2020. 10. 19
void Pose1_FindWristPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_WRIST])
		return;

	EgVertex *WristPt = NULL;
	EgVertex *HandPt = m_LandmarkPose1[POSE1_LANDMARK_HAND],
		*ArmpitPt = m_LandmarkPose1[POSE1_LANDMARK_ARMPIT];

	if (HandPt == NULL || ArmpitPt == NULL) {
		_cprintf("Error in finding Wrist... Exiting...\n");
		return;
	}

	if (ArmpitPt->m_Pos[1] < HandPt->m_Pos[1])
		return;

	float upper = 300.0;
	float lower = 120.0;

	int rightIdx = 0;
	float minCirc = INFINITY, minCircLevel = lower;
	EgSlicer theSlicer;
	for (float level = lower; level <= upper; level += 10.0) {
		theSlicer.SetCutPlane(EgPlane(EgVec3(0, 1, 0), EgPos(HandPt->m_Pos[0], HandPt->m_Pos[1] + level, HandPt->m_Pos[2])));
		theSlicer.Slice(pMesh);
		theSlicer.SortByLength(false);

		rightIdx = 0;
		if (theSlicer.GetCenterPos(1).m_Coords[0] < theSlicer.GetCenterPos(0).m_Coords[0])
			rightIdx = 1;

		if (theSlicer.GetLength(rightIdx) < minCirc) {
			minCirc = theSlicer.GetLength(rightIdx);
			minCircLevel = level;
		}
	}

	theSlicer.SetCutPlane(EgPlane(EgVec3(0, 1, 0), EgPos(HandPt->m_Pos[0], HandPt->m_Pos[1] + minCircLevel, HandPt->m_Pos[2])));
	theSlicer.Slice(pMesh);
	theSlicer.SortByLength(false);

	rightIdx = 0;
	if (theSlicer.GetNumCutLine() > 1) {
		if (theSlicer.GetCenterPos(1).m_Coords[0] < theSlicer.GetCenterPos(0).m_Coords[0])
			rightIdx = 1;
	}

	// ������ �� �ּ� x��ǥ�� ���� ������ ã�´�.
	EgPos minX = EgPos(INFINITY, 0, 0);
	for (EgPos p : theSlicer.m_CutLines[rightIdx])
		if (p[0] < minX[0])
			minX = p;


	WristPt = FindClosestVert(pMesh, minX);

	if (WristPt != NULL) {
		m_LandmarkPose1[POSE1_LANDMARK_WRIST] = WristPt;
		_cprintf("Wrist Pt : ");
		printVert(WristPt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_WRIST] = NULL;
		_cprintf("Wrist Pt : NOT FOUND!\n");
	}
}


/////////////////////////////////////// ������::�������
// ������ ������ : 2020. 10. 19
void Pose1_FindElbowPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_ELBOW])
		return;

	EgVertex *ElbowPt = NULL;
	EgVertex *WristPt = m_LandmarkPose1[POSE1_LANDMARK_WRIST],
		*ArmpitPt = m_LandmarkPose1[POSE1_LANDMARK_ARMPIT];

	if (WristPt == NULL || ArmpitPt == NULL) {
		_cprintf("Error in finding Elbow... Exiting...\n");
		return;
	}

	EgPos HalfPt = ArmpitPt->m_Pos;
	HalfPt.m_Coords[1] = (WristPt->m_Pos[1] + ArmpitPt->m_Pos[1]) / 2;

	EgSlicer theSlicer;
	theSlicer.SetCutPlane(EgPlane(EgVec3(0, 1, 0), HalfPt));
	theSlicer.CvxSlice(pMesh);

	// ������ �� �ּ� x��ǥ�� ���� ������ ã�´�.
	EgPos minX = EgPos(INFINITY, 0, 0);
	for (EgPos p : theSlicer.m_CutLines[0])
		if (p[0] < minX[0])
			minX = p;

	ElbowPt = FindClosestVert(pMesh, minX);

	if (ElbowPt != NULL) {
		// ElbowPt = SK_Elliptic_Vertex(ElbowPt, 30);
		ElbowPt = SK_Extreme_Vertex(ElbowPt, 10, 5, 1);

		m_LandmarkPose1[POSE1_LANDMARK_ELBOW] = ElbowPt;
		_cprintf("Elbow Pt : ");
		printVert(ElbowPt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_ELBOW] = NULL;
		_cprintf("Elbow Pt : NOT FOUND!\n");
	}
}


/////////////////////////////////////// ������::���ʸ���
// ������ ������ : 2020. 10. 12
void Pose1_FindEyePt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_EYE])
		return;

	EgVertex *EyePt = NULL;
	EgVertex *TopHeadPt = m_LandmarkPose1[POSE1_LANDMARK_TOPHEAD],
		*NosePt = m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_NOSE];

	if (NosePt == NULL) {
		_cprintf("Error in finding Eye... Exiting...\n");
		return;
	}

	EgPos MidEyePt = NosePt->m_Pos;
	MidEyePt.m_Coords[1] += 70;
	MidEyePt = FindClosestVert(pMesh, MidEyePt)->m_Pos;

	if (MidEyePt != NULL)
		MidEyePt = SK_Hyperbolic_Vertex(FindClosestVert(pMesh, MidEyePt), 20.0)->m_Pos;


	EyePt = FindClosestVert(pMesh, EgPos(MidEyePt.m_Coords[0] - 40, MidEyePt.m_Coords[1], MidEyePt.m_Coords[2]));


	if (EyePt != NULL) {
		m_LandmarkPose1[POSE1_LANDMARK_EYE] = EyePt;
		_cprintf("Eye Pt : ");
		printVert(EyePt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_EYE] = NULL;
		_cprintf("Eye Pt : NOT FOUND!\n");
	}
}


/////////////////////////////////////// ������::�̵帮����
// ������ ������ : 2020. 9. 17
void Pose1_FindMidRiffPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_MIDRIFF])
		return;

	EgVertex *MidRiffPt = NULL;
	// �������Ʒ��� �㸮�ѷ����� �߰�
	EgVertex *UnderBustPt = m_LandmarkPose1[POSE1_LANDMARK_UNDERBUST],
		*WaistPt = m_LandmarkPose1[POSE1_LANDMARK_WAIST];

	if (UnderBustPt == NULL || WaistPt == NULL) {
		_cprintf("Error in finding Mid riff... Exiting...\n");
		return;
	}

	float level = (UnderBustPt->m_Pos[1] + WaistPt->m_Pos[1]) / 2;

	MidRiffPt = FindClosestVert(pMesh, EgPos((m_MinX + m_MaxX)/2, level, m_MinZ));

	if (MidRiffPt != NULL) {
		m_LandmarkPose1[POSE1_LANDMARK_MIDRIFF] = MidRiffPt;
		_cprintf("MidRiff Pt : ");
		printVert(MidRiffPt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_MIDRIFF] = NULL;
		_cprintf("MidRiff Pt : NOT FOUND!\n");
	}
}


/////////////////////////////////////// ������::Top hip��
// ������ ������ : 2020. 9. 17
void Pose1_FindTopHipPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_TOPHIP])
		return;

	EgVertex *TopHipPt = NULL;
	// �㸮�ѷ����ذ� �����̵�������� �߰�
	EgVertex *WaistPt = m_LandmarkPose1[POSE1_LANDMARK_WAIST],
		*HipExtrudePt = m_LandmarkPose1[POSE1_LANDMARK_HIP_EXTRUDE];

	if (WaistPt == NULL || HipExtrudePt == NULL) {
		_cprintf("Error in finding Top hip... Exiting...\n");
		return;
	}

	float level = (WaistPt->m_Pos[1] + HipExtrudePt->m_Pos[1]) / 2;

	TopHipPt = FindClosestVert(pMesh, EgPos((m_MinX + m_MaxX)/2, level, m_MinZ));

	if (TopHipPt != NULL) {
		m_LandmarkPose1[POSE1_LANDMARK_TOPHIP] = TopHipPt;
		_cprintf("Top Hip Pt : ");
		printVert(TopHipPt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_TOPHIP] = NULL;
		_cprintf("Top Hip Pt : NOT FOUND!\n");
	}
}


/////////////////////////////////////// ������::Upper hip��
// ������ ������ : 2020. 9. 17
void Pose1_FindUpperHipPt(EgMesh *pMesh) {
	if (!m_bFindLandmark[POSE1_LANDMARK_UPPERHIP])
		return;

	EgVertex *UpperHipPt = NULL;
	// Top-hip���ذ� �㸮�ѷ������� �߰�
	EgVertex *TopHipPt = m_LandmarkPose1[POSE1_LANDMARK_TOPHIP],
		*WaistPt = m_LandmarkPose1[POSE1_LANDMARK_WAIST];

	if (TopHipPt == NULL || WaistPt == NULL) {
		_cprintf("Error in finding Upper hip... Exiting...\n");
		return;
	}

	float level = (TopHipPt->m_Pos[1] + WaistPt->m_Pos[1]) / 2;

	UpperHipPt = FindClosestVert(pMesh, EgPos((m_MinX + m_MaxX)/2, level, m_MinZ));

	if (UpperHipPt != NULL) {
		m_LandmarkPose1[POSE1_LANDMARK_UPPERHIP] = UpperHipPt;
		_cprintf("Upper Hip Pt : ");
		printVert(UpperHipPt);
	}
	else {
		m_LandmarkPose1[POSE1_LANDMARK_UPPERHIP] = NULL;
		_cprintf("Upper Hip Pt : NOT FOUND!\n");
	}
}


/////////////////////////////////////// ������::�����
// ������ ������ : 2020. 8. 20
void Pose1_FindBackExtrudePt(EgMesh *pMesh) {
	EgVertex *BackExtrudePt = NULL;
	EgVertex *BneckPt = m_LandmarkPose1[POSE1_LANDMARK_NECK_BACK],
		*WaistPt = m_LandmarkPose1[POSE1_LANDMARK_WAIST];

	if (BneckPt == NULL || WaistPt == NULL) {
		_cprintf("Error in finding Back extrude... Exiting...\n");
		return;
	}

	float upperBackBound = BneckPt->m_Pos[1];
	float lowerBackBound = WaistPt->m_Pos[1];

	EgPos minZ = EgPos(0, 0, INFINITY);
	for (EgVertex *pos : m_SagittalPoints_Back) {
		if (pos->m_Pos[1] > lowerBackBound && pos->m_Pos[1] < upperBackBound) {
			if (pos->m_Pos[2] < minZ.m_Coords[2])
				minZ = pos->m_Pos;
		}
	}

	BackExtrudePt = FindClosestVert(pMesh, minZ);
	
	if (BackExtrudePt != NULL) {
		BackExtrudePt = SK_Extreme_Vertex(BackExtrudePt, 150.0, 5, 1);

		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_BACK_EXTRUDE] = BackExtrudePt;
		_cprintf("Back Extrude Pt : ");
		printVert(BackExtrudePt);
	}
	else {
		m_LandmarkHelpPose1[POSE1_LANDMARK_HELP_BACK_EXTRUDE] = NULL;
		_cprintf("Back Extrude Pt : NOT FOUND!\n");
	}
}

/********************************************************************************************************************/
