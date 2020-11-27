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
*	\brief	�޽��� ������� ������ �ܸ��� ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	8 July 2020
*/
class EgSlicer
{
public:
	/*!	\biref	���� ��� */
	EgPlane m_CutPln;

	/*!	\biref	����� ���ܼ� ����Ʈ */
	std::vector<std::vector<EgPos>> m_CutLines;

public:
	// ������ �� �Ҹ���
	EgSlicer();
	EgSlicer(EgPlane CutPln);
	EgSlicer(const EgSlicer &cpy);
	~EgSlicer();

public:
	// ���� ������
	EgSlicer &operator =(const EgSlicer &rhs);

	// ��� �Լ�
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

/*!	\brief	���� ����Ʈ �Լ��� */
EgMesh *SK_Import_Obj(CString FileName);
EgMesh *SK_Import_Ply(CString FileName);
EgMesh *SK_Import_Iv(CString FileName);
EgMesh *SK_Import_Msh(CString FileName);
EgMesh *SK_Import_Vrml97(CString FileName);
EgMesh *SK_Import_Wrb(CString FileName);

/*!	\brief	�Է� ��ġ�� ���� ����� ������ ã�´�. */
EgVertex *FindClosestVert(EgMesh *pMesh, EgPos p);

/*! \brief �޽� ���� �� �� ������ ����(�ִ�) �Ÿ��� ���ϴ� �Լ� */
double SK_Shortest_Distance(EgFacePt *P, EgFacePt *Q, std::vector<EgPos> &Path);
double SK_Shortest_Distance(EgVertex *P, EgVertex *Q, std::vector<EgPos> &Path);

/*! \brief �޽� ���� �� �� ������ ��� ���� �Ÿ��� ���ϴ� �Լ� */
double SK_Planar_Distance(EgFacePt *P, EgFacePt *Q, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex);
double SK_Planar_Distance(EgVertex *P, EgVertex *Q, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex);
double SK_Planar_Distance(EgVertex *P, EgVertex *Q, std::vector<EgPos> &Path, bool bConvex);

/*! \brief �޽� ���� �� ���� ������ �ѷ��� ���̸� ���ϴ� �Լ� */
double SK_Planar_Perimeter(EgFacePt *P, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex);
double SK_Planar_Perimeter(EgVertex *P, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex);

/*!	\brief	���� ��� ���� ���� ���������� convexhull�� ���Ѵ�. */
double SK_ConvexHull(EgPlane *Pln, std::vector<EgPos> &Pts, std::vector<EgPos> &CvxPts);

/*!	\brief	���� ��� ���� ���� ���������� ���� convexhull�� ���Ѵ�. */
double SK_ConvexHull_Open(EgPlane *Pln, std::vector<EgPos> &Pts, std::vector<EgPos> &CvxPts);

/*! \brief	�־��� ���� �ֺ��� ������ �˻��Ͽ� �� ����(extream vertex)�� ã�´�. */
EgVertex *SK_Extreme_Vertex(EgVertex *pSeedVert, double Radius, int PtType, int depth = 1);

/*!	\brief	�־��� ���� �ֺ��� ������ �˻��Ͽ� Ÿ����(elliptic point)�� ã�´�. */
EgVertex *SK_Elliptic_Vertex(EgVertex *pSeedVert, double Radius);

/*!	\brief	�־��� ���� �ֺ��� ������ �˻��Ͽ� ��ź��(planar point)�� ã�´�. */
EgVertex *SK_Planar_Vertex(EgVertex *pSeedVert, double Radius);

/*!	\brief	�־��� ���� �ֺ��� ������ �˻��Ͽ� �ְ���(hyperbolic point)�� ã�´�. */
EgVertex *SK_Hyperbolic_Vertex(EgVertex *pSeedVert, double Radius);

/*! \brief	���� ������ ������ ����� ���� �����Ѵ�. */
void SK_Sort_By_Curvature(std::vector<EgPos> Pts, std::vector<EgPos> &SortedPts, bool bDescend, bool bSignedCurvature, EgVec3 Up);

/*! \brief	���� ������ �������� ���� ����� ���� ������ ���Ѵ�. */
void SK_Find_Negative_Curvature(std::vector<EgPos> Pts, std::vector<EgPos> &ResPts, bool bSort, EgVec3 Up);

/*! \brief	���� ������ �������� ��� ����� ���� ������ ���Ѵ�. */
void SK_Find_Positive_Curvature(std::vector<EgPos> Pts, std::vector<EgPos> &ResPts, bool bSort, EgVec3 Up);
