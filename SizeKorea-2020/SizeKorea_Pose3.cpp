#include "pch.h"
#include "SizeKoreaUtil.h"

extern EgSlicer theSlicer;
extern EgSizer theSizer;

std::vector<std::string> LandmarkPose3 = {
	"엉덩이돌출점",				// 0
	"배돌출점",					// 1
	"엉덩이최대둘레수준점",			// 2
	"왼쪽주먹가운데점",			// 3
	"오른쪽주먹가운데점",			// 4
	"오른쪽등부위돌출점",			// 5
	"왼쪽팔꿈치아래점",			// 6
	"왼쪽팔꿈치뒤점",				// 7
	"오른쪽어깨점"				// 8
};

std::vector<std::string> ResultPose3 = {
	"엉덩이최대둘레높이",			// 0
	"굽힌팔꿈치높이",				// 1
	"엉덩이둘레",					// 2
	"엉덩이최대둘레",				// 3
	"배돌출점기준엉덩이둘레",		// 4
	"팔꿈치주먹수평길이",			// 5
	"벽면앞으로뻗은주먹수평길이",	// 6
	"벽면어깨수평길이"				// 7
};

/*!
*	\brief	엉덩이돌출점(수준)을 찾는다.
*	\note	마지막 수정일: 2020-06-23
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	엉덩이돌출점의 포인터를 반환한다.
*/
EgVertex *SK_Pose3_Buttock_Protrusion(EgMesh *pMesh)
{
	// X축 중간 좌표를 구한다.
	double cntX = pMesh->m_BndBox.m_MinPt[0] * 0.65 + pMesh->m_BndBox.m_MaxPt[0] * 0.35;

	// Y축 최소/최대 좌표를 구한다.
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];

	// 탐색 높이를 결정한다. 
	double low = minY + (maxY - minY) * 0.4;
	double high = minY + (maxY - minY) * 0.55;
	double minZ = INFINITY;

	// 최소 Z좌표를 갖는 정점을 찾아 반환한다.
	EgVertex *pResult = NULL;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] > low && v->m_Pos[1] < high)
		{
			if (v->m_Pos[2] < minZ && v->m_Pos[0] < cntX)
			{
				minZ = v->m_Pos[2];
				pResult = v;
			}
		}
	}
	return pResult;
}

/*!
*	\brief	배돌출점을 찾는다.
*	\note	마지막 수정일: 2020-07-08
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	배돌출점의 포인터를 반환한다.
*/
EgVertex *SK_Pose3_Abdominal_Protrusion(EgMesh *pMesh)
{
	// X, Y축 최소/최대 좌표를 구한다.
	double minX = pMesh->m_BndBox.m_MinPt[0];
	double maxX = pMesh->m_BndBox.m_MaxPt[0];
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];

	// 탐색 높이를 결정한다. 
	double low = minY + (maxY - minY) * 0.5;
	double high = minY + (maxY - minY) * 0.6;

	// 탐색 폭을 결정한다. (cntX - 10cm ~ cntX + 10cm)
	double cntX = (minX + maxX) * 0.5;

	// 최대 z 좌표를 갖는 정점을 찾아 반환한다.
	double maxZ = -INFINITY;
	EgVertex *pResult = NULL;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] > low && v->m_Pos[1] < high &&
			ABS(v->m_Pos[0] - cntX) < 100.0)
		{
			if (v->m_Pos[2] > maxZ)
			{
				maxZ = v->m_Pos[2];
				pResult = v;
			}
		}
	}
	return pResult;
}

/*!
*	\brief	엉덩이최대둘레수준(점)을 찾는다.
*	\note	마지막 수정일: 2020-06-26
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*	\param	Path[out]	절단 경로가 저장된다.
*
*	\return	엉덩이최대둘레수준(점)의 포인터를 반환한다.
*/
EgVertex *SK_Pose3_Buttock_Max_Perimeter(EgMesh *pMesh, std::vector<EgPos> &Path)
{
	// Y축 최소/최대 좌표를 구한다.
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];

	// 샅수준점 ~ 엉덩이돌출점 사이의 탐색 높이를 결정한다. 
	EgVertex *BP = SK_Pose3_Buttock_Protrusion(pMesh);
	double low = minY + (maxY - minY) * 0.4;
	double high = BP->m_Pos[1];

	EgVertex *pResult = NULL;
	double maxPerimeter = -INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// 탐색 범위의 정점에 대하여
		if (v->m_Pos[1] > low && v->m_Pos[1] < high)
		{
			if (ABS(BP->m_Pos[0] - v->m_Pos[0]) < 1 &&
				ABS(BP->m_Pos[2] - v->m_Pos[2]) < 10)
			{
				// 최대 둘레의 정보를 구한다.
				std::vector<EgPos> tmp;
				EgPlane Pln(EgVec3(0.0, 1.0, 0.0), v->m_Pos);
				double perimeter = ::SK_Planar_Perimeter(v, &Pln, tmp, true);
				if (perimeter >= maxPerimeter)
				{
					maxPerimeter = perimeter;
					pResult = v;
					Path = tmp;
				}
			}
		}
	}

	// 엉덩이최대둘레수준점을 반환한다.
	return pResult;
}

/*
*	\brief	오른쪽 주먹가운데점을 찾는 함수
*
*	\param	pMesh[in]	대상 메쉬에 대한 포인터
*
*	\return	오른쪽 주먹가운데점의 위치를 반환한다.
*/
EgVertex *SK_Pose3_Right_Fist_Center(EgMesh *pMesh)
{
	double minZ = 0.7, maxZ = 1.0, z_star;
	double prevLen = INFINITY;
	EgPos CntPos;
	for (int i = 0; i < 10; ++i)
	{
		double z = (double)i / (9.0) * (maxZ - minZ) + minZ;
		double len = theSlicer.CvxSlice(pMesh, 2, z, -1.0, 0.0, 0.0, 1.0, -1.0, 1.0);
		if (len == -1.0)
			continue;

		//printf("Len = %lf\n", len);
		if (len > prevLen)
		{
			z_star = z;
			CntPos = theSlicer.GetCenterPos(0);
			break;
		}
		else
			prevLen = len;
	}

	// 주먹 중심을 지나는 절단면의 형상을 구한다.
	double len = theSlicer.CvxSlice(pMesh, 2, (z_star + 1.0) * 0.5, -1.0, 0.0, 0.0, 1.0, -1.0, 1.0);
	if (len == -1.0)
	{
		printf("[Error]: SK_Pose5_Right_Fist_Center()...\n");
		return NULL;
	}

	// 절단면의 좌표 중 최소 X좌표를 갖는 점을 선택한다.
	EgPos FistCenter(INFINITY, 0.0, 0.0);
	for (EgPos p : theSlicer.m_CutLines[0])
	{
		if (p[0] < FistCenter[0])
			FistCenter = p;
	}

	// 오른쪽 주먹가운데점을 반환한다.
	return FindClosestVert(pMesh, FistCenter);
}

/*
*	\brief	왼쪽 주먹가운데점을 찾는 함수
*
*	\param	pMesh[in]	대상 메쉬에 대한 포인터
*
*	\return	왼쪽 주먹가운데점의 위치를 반환한다.
*/
EgVertex *SK_Pose3_Left_Fist_Center(EgMesh *pMesh)
{
	double minZ = 0.7, maxZ = 1.0, z_star;
	double prevLen = INFINITY;
	EgPos CntPos;
	for (int i = 0; i < 10; ++i)
	{
		double z = (double)i / (9.0) * (maxZ - minZ) + minZ;
		double len = theSlicer.CvxSlice(pMesh, 2, z, 0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
		if (len == -1.0)
			continue;

		//printf("Len = %lf\n", len);
		if (len > prevLen)
		{
			z_star = z;
			CntPos = theSlicer.GetCenterPos(0);
			break;
		}
		else
			prevLen = len;
	}

	// 주먹 중심을 지나는 절단면의 형상을 구한다.
	double len = theSlicer.CvxSlice(pMesh, 2, (z_star + 1.0) * 0.5, 0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	if (len == -1.0)
	{
		printf("[Error]: SK_Pose3_Left_Fist_Center()...\n");
		return NULL;
	}

	// 절단면의 좌표 중 최대 X좌표를 갖는 점을 선택한다.
	EgPos FistCenter(-INFINITY, 0.0, 0.0);
	for (EgPos p : theSlicer.m_CutLines[0])
	{
		if (p[0] > FistCenter[0])
			FistCenter = p;
	}

	// 왼쪽 주먹가운데점을 반환한다.
	return FindClosestVert(pMesh, FistCenter);
}

/*
*	\brief	왼쪽 팔꿈치아래점을 찾는 함수
*	\note	마지막 수정일: 2020-11-29
*
*	\param	pMesh[in]	대상 메쉬에 대한 포인터
*
*	\return	왼쪽 팔꿈치아래점의 위치를 반환한다.
*/
EgVertex *SK_Pose3_Bottom_Olecranon(EgMesh *pMesh)
{
	EgVertex *pSeedVert = theSizer.GetLandmark("왼쪽팔꿈치뒤점");
	if (pSeedVert == NULL)
		return NULL;

	pSeedVert = SK_Extreme_Vertex(pSeedVert, 30.0, 3, 1);
	return pSeedVert;
	// 이전 버전
	//for (int i = 0; i < 10; ++i)
	//{
	//	double z = 0.7 + 0.3 * i / 9.0;
	//	double Len = theSlicer.CvxSlice(pMesh, 2, z, 0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	//	if (Len != -1.0)
	//		break;
	//}

	//// 직선을 생성한다.
	//EgPos CntPos = theSlicer.GetCenterPos(0);
	//EgLine ray(CntPos, EgVec3(0.0, 0.0, -1.0));
	//EgPos SeedPt(0.0, 0.0, INFINITY);
	//for (EgFace *f : pMesh->m_pFaces)
	//{
	//	EgPos p0 = f->GetVertexPos(0);
	//	EgPos p1 = f->GetVertexPos(1);
	//	EgPos p2 = f->GetVertexPos(2);
	//	EgPos tmp;
	//	if (::intersect_line_triangle(p0, p1, p2, ray, tmp, false))
	//	{
	//		if (tmp[2] < SeedPt[2])
	//			SeedPt = tmp;
	//	}
	//}

	//EgVertex *pSeedVert = FindClosestVert(pMesh, SeedPt);
	//pSeedVert = SK_Extreme_Vertex(pSeedVert, 30.0, 3, 2);
	//return pSeedVert;
}

/*
*	\brief	왼쪽 팔꿈치뒤점을 찾는 함수
*	\note	마지막 수정일: 2020-11-29
*
*	\param	pMesh[in]	대상 메쉬에 대한 포인터
*
*	\return	왼쪽 팔꿈치뒤점의 위치를 반환한다.
*/
EgVertex *SK_Pose3_Rear_Olecranon(EgMesh *pMesh)
{
	double minZ = 0.7, maxZ = 1.0, z_star;
	double prevLen = INFINITY;
	EgPos prevCntPos, CntPos;
	for (int i = 0; i < 10; ++i)
	{
		double z = minZ + (maxZ - minZ) * (double)i / (9.0);
		double len = theSlicer.CvxSlice(pMesh, 2, z, 0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
		if (len == -1.0)
			continue;

		if (len > prevLen)
		{
			z_star = z;
			CntPos = theSlicer.GetCenterPos(0);
			break;
		}
		else
		{
			prevLen = len;
			prevCntPos = theSlicer.GetCenterPos(0);
		}
	}

	// 직선을 생성한다.
	EgLine ray(CntPos, EgVec3(0.0, 0.0, -1.0));
	EgPos SeedPt(0.0, 0.0, INFINITY);
	for (EgFace *f : pMesh->m_pFaces)
	{
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);
		EgPos tmp;
		if (::intersect_line_triangle(p0, p1, p2, ray, tmp, false))
		{
			if (tmp[2] < SeedPt[2])
				SeedPt = tmp;
		}
	}

	EgVertex *pSeedVert = FindClosestVert(pMesh, SeedPt);
	pSeedVert = SK_Elliptic_Vertex(pSeedVert, 50.0);
	pSeedVert = SK_Extreme_Vertex(pSeedVert, 20.0, 5);
	return pSeedVert;
}

/*!
*	\brief	오른쪽 등부위돌출점 찾는 함수
*
*	\param	pMesh[in]	대상 메쉬에 대한 포인터
*
*	\return	오른쪽 등부위돌출점의 좌표를 반환한다.
*/
EgVertex *SK_Pose3_Right_Back_Protrusion(EgMesh *pMesh)
{
	// 메쉬를 절단한다.
	double Len = theSlicer.CvxSlice(pMesh, 2, 0.7, -1.0, 0.0, 0.0, 1.0, 0.0, 1.0);
	if (Len == -1.0)
	{
		printf("[Error]: SK_Pose3_Right_Back_Protrusion()...\n");
		return NULL;
	}

	// 직선을 생성한다.
	EgLine ray(theSlicer.GetCenterPos(0), EgVec3(0.0, 0.0, -1.0));

	// 직선과 메쉬의 교차점(최소 Z값을 갖는)을 구한다.
	EgPos SeedPt(0.0, 0.0, INFINITY);
	for (EgFace *f : pMesh->m_pFaces)
	{
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);
		EgPos tmp; // 교차점
		if (::intersect_line_triangle(p0, p1, p2, ray, tmp, false))
		{
			if (tmp[2] < SeedPt[2])
				SeedPt = tmp;
		}
	}

	// 교차점 부근에서 최소 Z값을 갖는 정점을 찾는다.
	EgVertex *pResult = FindClosestVert(pMesh, SeedPt);
	return SK_Extreme_Vertex(pResult, 100.0, 5);
}

/*!
*	\brief	오른쪽 어깨점을 찾는 함수
*
*	\param	pMesh[in]		대상 메쉬에 대한 포인터
*
*	\return	오른쪽 어깨점의 좌표를 반환한다.
*/
EgVertex *SK_Pose3_Right_Acromion(EgMesh *pMesh)
{
	// 오른팔의 절단면의 중심 3개를 구한다.
	std::vector<EgPos> CntPos;
	for (int i = 0; i < 10; ++i)
	{
		// z = 0.4 ~ 1.0 범위에서 적당한 절단면을 구한다.
		double z = 0.4 + 0.6 * i / 9.0;
		double Len = theSlicer.CvxSlice(pMesh, 2, z, -1.0, -0.5, 0.0, 1.0, -1.0, 1.0);
		if (Len != -1.0)
		{
			CntPos.push_back(theSlicer.GetCenterPos(0));
			if (NUM(CntPos) == 2)
				break;
		}
	}

	// 절단면의 중심들을 지나는 절단 평면을 설정한다.
	EgPos MidPos = lerp(CntPos[0], CntPos[1], 0.5);
	MidPos[1] -= 50.0;
	theSlicer.SetCutPlane(EgPlane(CntPos[0], MidPos, CntPos[1]));
	theSlicer.CvxSlice(pMesh);

	// 절단면의 y 좌표가 최대인 점을 오른쪽 어깨점으로 결정한다.
	EgPos Acromion(0.0, -INFINITY, 0.0);
	for (EgPos &pt : theSlicer.m_CutLines[0])
		if (pt[1] > Acromion[1] && pt[2] < CntPos[0][2])
			Acromion = pt;

	// 오른쪽 어깨점을 반환한다.
	EgVertex *Ret = FindClosestVert(pMesh, Acromion);
	return SK_Extreme_Vertex(Ret, 30.0, 5);
	//return FindClosestVert(pMesh, Acromion);
}
// 이전 버전
//EgVertex *SK_Pose3_Right_Acromion(EgMesh *pMesh)
//{
//	for (int i = 0; i < 10; ++i)
//	{
//		// z = 0.7 ~ 1.0 범위에서 적당한 절단면을 구한다.
//		double z = 0.7 + 0.3 * i / 9.0;
//		double Len = theSlicer.CvxSlice(pMesh, 2, z, -1.0, 0.0, 0.0, 1.0, -1.0, 1.0);
//		if (Len != -1.0)
//			break;
//	}
//
//	// 절단 중심을 지나고 x 축을 법선으로 하는 절단 평면을 설정한다.
//	EgPos p = theSlicer.GetCenterPos(0);
//	theSlicer.SetCutPlane(EgPlane(EgVec3(1.0, 0.0, 0.0), p));
//	theSlicer.CvxSlice(pMesh);
//
//	// 절단면의 y 좌표가 최대인 점을 오른쪽 어깨점으로 결정한다.
//	EgPos Acromion(0.0, -INFINITY, 0.0);
//	for (EgPos &pt : theSlicer.m_CutLines[0])
//		if (pt[1] > Acromion[1] && pt[2] < p[2])
//			Acromion = pt;
//
//	// 오른쪽 어깨점을 반환한다.
//	return FindClosestVert(pMesh, Acromion);
//}

