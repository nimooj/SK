#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <crtdbg.h>
#include <direct.h>
#include "EgSizer.h"
#include "SizeKorea_Pose3.h"
#include "SizeKorea_Pose4.h"
#include "SizeKorea_Pose5.h"

/*!
*	\class	EgSlicer
*	\brief	메쉬를 평면으로 절단한 단면을 표현하는 클래스
*
*	\author 윤승현(shyun@dongguk.edu)
*	\date	8 July 2020
*/
class EgSlicer
{
public:
	/*!	\biref	절단 평면 */
	EgPlane m_CutPln;

	/*!	\biref	연결된 절단선 리스트 */
	std::vector<std::vector<EgPos>> m_CutLines;

public:
	// 생성자 및 소멸자
	EgSlicer();
	EgSlicer(EgPlane CutPln);
	EgSlicer(const EgSlicer &cpy);
	~EgSlicer();

public:
	// 대입 연산자
	EgSlicer &operator =(const EgSlicer &rhs);

	// 멤버 함수
	void SetCutPlane(EgPlane CutPln);
	int Slice(EgMesh *pMesh);
	int Slice(EgMesh *pMesh, EgPos Pivot, double dx, double dy, double dz);
	int Slice(EgMesh *pMesh, int PlnType, double Param, double MinX, double MaxX, double MinY, double MaxY, double MinZ, double MaxZ, bool bClosedCut);
	double CvxSlice(EgMesh *pMesh, int PlnType, double Param, double MinX, double MaxX, double MinY, double MaxY, double MinZ, double MaxZ);
	void CvxSlice(EgMesh *pMesh);
	int GetNumCutLine();
	void SortByLength(bool bDescend = true);
	EgPos GetCenterPos(int idx);
	double GetLength(int idx);
	bool IsClosed(int idx);
	void Render();
	void Reset();
};

/*!	\brief	파일 임포트 함수들 */
EgMesh *SK_Import_Obj(CString FileName);
EgMesh *SK_Import_Ply(CString FileName);
EgMesh *SK_Import_Iv(CString FileName);
EgMesh *SK_Import_Msh(CString FileName);
EgMesh *SK_Import_Vrml97(CString FileName);
EgMesh *SK_Import_Wrb(CString FileName);

/*!	\brief	입력 위치와 가장 가까운 정점을 찾는다. */
EgVertex *FindClosestVert(EgMesh *pMesh, EgPos p);

/*! \brief 메쉬 위의 두 점 사이의 측지(최단) 거리를 구하는 함수 */
double SK_Shortest_Distance(EgFacePt *P, EgFacePt *Q, std::vector<EgPos> &Path);
double SK_Shortest_Distance(EgVertex *P, EgVertex *Q, std::vector<EgPos> &Path);

/*! \brief 메쉬 위의 두 점 사이의 평면 절단 거리를 구하는 함수 */
double SK_Planar_Distance(EgFacePt *P, EgFacePt *Q, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex);
double SK_Planar_Distance(EgVertex *P, EgVertex *Q, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex);
double SK_Planar_Distance(EgVertex *P, EgVertex *Q, std::vector<EgPos> &Path, bool bConvex);

/*! \brief 메쉬 위의 한 점을 지나는 둘레의 길이를 구하는 함수 */
double SK_Planar_Perimeter(EgFacePt *P, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex);
double SK_Planar_Perimeter(EgVertex *P, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex);

/*!	\brief	절단 평면 위에 놓인 교차점들의 convexhull을 구한다. */
double SK_ConvexHull(EgPlane *Pln, std::vector<EgPos> &Pts, std::vector<EgPos> &CvxPts);

/*!	\brief	절단 평면 위에 놓인 교차점들의 열린 convexhull을 구한다. */
double SK_ConvexHull_Open(EgPlane *Pln, std::vector<EgPos> &Pts, std::vector<EgPos> &CvxPts);

/*! \brief	주어진 정점 주변의 영역을 검사하여 극 정점(extream vertex)을 찾는다. */
EgVertex *SK_Extreme_Vertex(EgVertex *pSeedVert, double Radius, int PtType, int depth = 1);

/*!	\brief	주어진 정점 주변의 영역을 검사하여 타원점(elliptic point)을 찾는다. */
EgVertex *SK_Elliptic_Vertex(EgVertex *pSeedVert, double Radius);

/*!	\brief	주어진 정점 주변의 영역을 검사하여 평탄점(planar point)을 찾는다. */
EgVertex *SK_Planar_Vertex(EgVertex *pSeedVert, double Radius);

/*!	\brief	주어진 정점 주변의 영역을 검사하여 쌍곡점(hyperbolic point)을 찾는다. */
EgVertex *SK_Hyperbolic_Vertex(EgVertex *pSeedVert, double Radius);

/*! \brief	폴리 라인의 정점을 곡률에 따라 정렬한다. */
void SK_Sort_By_Curvature(std::vector<EgPos> Pts, std::vector<EgPos> &SortedPts, bool bDescend, bool bSignedCurvature, EgVec3 Up);

/*! \brief	폴리 라인의 정점에서 음수 곡률을 갖는 정점을 구한다. */
void SK_Find_Negative_Curvature(std::vector<EgPos> Pts, std::vector<EgPos> &ResPts, bool bSort, EgVec3 Up);

/*! \brief	폴리 라인의 정점에서 양수 곡률을 갖는 정점을 구한다. */
void SK_Find_Positive_Curvature(std::vector<EgPos> Pts, std::vector<EgPos> &ResPts, bool bSort, EgVec3 Up);
