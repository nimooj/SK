#include "pch.h"
#include "SizeKoreaUtil.h"


extern EgSlicer theSlicer;
extern EgSizer theSizer;

std::vector<std::string> LandmarkPose4 = {
	"머리마루점",						// 1
	"눈초리점",						// 2
	"목뒤점",						// 3
	"오른쪽어깨가쪽점",				// 4
	"왼쪽어깨가쪽점",					// 5
	"오른쪽어깨점",					// 6
	"왼쪽어깨점",						// 7
	"앉은엉덩이뒤돌출점",				// 8
	"팔꿈치가운데점",					// 9
	"손목안쪽점",						// 10
	"손끝점",						// 11
	"앉은면수준점",					// 12
	"앉은넙다리위점",					// 13
	"무릎뼈위점",						// 14
	"앉은오금점",						// 15
	"오른쪽어깨세모근점",				// 16
	"왼쪽어깨세모근점",				// 17
	"오른쪽팔꿈치가쪽점",				// 18
	"왼쪽팔꿈치가쪽점",				// 19
	"오른쪽앉은엉덩이사이최대돌출수준점",	// 20
	"왼쪽앉은엉덩이사이최대돌출수준점",	// 21
	"앉은무릎앞점",					// 22
	"팔꿈치뒤점",						// 23
	"팔꿈치아래점",					// 24
	"앉은배돌출점"					// 25
};

std::vector<std::string> ResultPose4 = {
	"앉은키",						// 1
	"앉은눈높이",						// 2
	"앉은목뒤높이",					// 3
	"앉은어깨높이",					// 4
	"앉은팔꿈치높이",					// 5
	"앉은넙다리높이",					// 6
	"앉은무릎높이",					// 7
	"앉은오금높이",					// 8
	"어깨사이너비",					// 9
	"어깨가쪽사이너비",				// 10
	"위팔사이너비",					// 11
	"팔꿈치사이너비",					// 12
	"앉은엉덩이너비",					// 13
	"앉은넙다리두께",					// 14
	"앉은배두께",						// 15
	"앉은엉덩이배두께",				// 16
	"굽힌팔길이",						// 17
	"앉은엉덩이오금수평길이",			// 18
	"앉은엉덩이무릎(앞)수평길이",		// 19
	"(팔굽힌)위팔수직길이",			// 20
	"(팔굽힌)아래팔수평길이",			// 21
	"(팔굽힌)팔꿈치손끝수평길이"		// 22
};

/*!
*	\brief	머리마루점을 찾는 함수
*	\note	마지막 수정일: 2020-07-19
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	머리마루점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Head_Top(EgMesh *pMesh)
{
	EgVertex *HT = NULL;
	double minY = -INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] > minY)
		{
			HT = v;
			minY = v->m_Pos[1];
		}
	}
	return HT;
}

/*!
*	\brief	앉은엉덩이뒤돌출점 찾는 함수
*	\note	마지막 수정일 : 2020-07-19
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	앉은엉덩이뒤돌출점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Buttock_Protrusion(EgMesh *pMesh)
{
	// 앉은무릎앞점이 발견되지 않았다면 NULL을 반환한다.
	EgVertex *RF = theSizer.GetLandmark("오른쪽앉은엉덩이사이최대돌출수준점");
	if (RF == NULL)
		return NULL;

	// 탐색 범위를 설정한다.
	double minY = RF->m_Pos[1] - 20;
	double maxY = RF->m_Pos[1] + 20;

	EgVertex *BP = NULL;
	double minZ = INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] > minY &&
			v->m_Pos[1] < maxY &&
			v->m_Pos[2] < minZ)
		{
			minZ = v->m_Pos[2];
			BP = v;
		}
	}

	return BP;
}

/*!
*	\brief	팔꿈치가운데점을 찾는 함수
*	\note	마지막 수정일 : 2020-07-19
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	팔꿈치가운데점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Center_Olecranon(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("손목안쪽점");
	if (pRef == NULL)
		return NULL;

	EgPos p = pRef->m_Pos;
	p[2] -= 100.0;
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 0.0, 1.0), p));
	theSlicer.Slice(pMesh, p, 200, 200, 10.0);
	EgPos c0 = theSlicer.GetCenterPos(0);

	p[2] -= 50.0;
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 0.0, 1.0), p));
	theSlicer.Slice(pMesh, p, 200, 200, 10.0);
	EgPos c1 = theSlicer.GetCenterPos(0);

	EgVertex *tmp = NULL;
	EgLine ray(c0, c1);
	for (EgFace *f : pMesh->m_pFaces)
	{
		EgPos q;
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);
		if (::intersect_line_triangle(p0, p1, p2, ray, q, false))
		{
			if (q[2] < c1[2])
			{
				tmp = f->GetVertex(0);
				break;
			}
		}
	}

	if (tmp == NULL)
		return NULL;
	return ::SK_Elliptic_Vertex(tmp, 50);;
	//return pMesh->m_pVerts[26477];
}

/*!
*	\brief	(오른쪽)팔꿈치뒤점을 찾는 함수
*	\note	마지막 수정일 : 2020-07-26
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	팔꿈치뒤점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Rear_Olecranon_Sit(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("팔꿈치가운데점");
	if (pRef == NULL)
		return NULL;

	return SK_Extreme_Vertex(pRef, 40, 5);
	//return pMesh->m_pVerts[26690];
}

/*!
*	\brief	팔꿈치아래점을 찾는 함수
*	\note	마지막 수정일 : 2020-07-19
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	팔꿈치아래점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Bottom_Olecranon_Sit(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("팔꿈치가운데점");
	if (pRef == NULL)
		return NULL;

	return SK_Extreme_Vertex(pRef, 40, 3);
	//return pMesh->m_pVerts[26552];
}

/*!
*	\brief	오른쪽 팔꿈치가쪽점을 찾는 함수
*	\note	마지막 수정일 : 2020-07-19
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	오른쪽 팔꿈치가쪽점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Lateral_Humeral_Epicondyle_Right(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("팔꿈치가운데점");
	if (pRef == NULL)
		return NULL;

	return SK_Extreme_Vertex(pRef, 40, 1);
	//return pMesh->m_pVerts[26205];
}

/*!
*	\brief	왼쪽 팔꿈치가쪽점을 찾는 함수
*	\note	마지막 수정일 : 2020-07-19
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	왼쪽 팔꿈치가쪽점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Lateral_Humeral_Epicondyle_Left(EgMesh *pMesh)
{
	EgPos pt(-INFINITY, 0.0, 0.0);
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];
	double y0 = minY + (maxY - minY) * 0.45;
	double y1 = minY + (maxY - minY) * 0.6;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] < y0 || v->m_Pos[1] > y1)
			continue;

		if (v->m_Pos[0] > pt[0])
			pt = v->m_Pos;
	}
	return FindClosestVert(pMesh, pt);
}
//EgVertex *SK_Pose4_Lateral_Humeral_Epicondyle_Left(EgMesh *pMesh)
//{
//	EgVertex *pRef = theSizer.GetLandmark("오른쪽팔꿈치가쪽점");
//	if (pRef == NULL)
//		return NULL;
//
//	EgPos pt(-INFINITY, 0.0, 0.0);
//	EgLine ray(pRef->m_Pos, EgVec3(1.0, 0.0, 0.0));
//	for (EgFace *f : pMesh->m_pFaces)
//	{
//		EgPos q;
//		EgPos p0 = f->GetVertexPos(0);
//		EgPos p1 = f->GetVertexPos(1);
//		EgPos p2 = f->GetVertexPos(2);
//		if (::intersect_line_triangle(p0, p1, p2, ray, q, false))
//		{
//			if (q[0] > pt[0])
//			{
//				pt = q;
//			}
//		}
//	}
//	return FindClosestVert(pMesh, pt);
//}


/*!
*	\brief	오른쪽 어깨가쪽점을 찾는 함수
*	\note	마지막 수정일 : 2020-08-03
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	오른쪽 어깨가쪽점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Lateral_Shoulder_Right(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("손끝점");
	if (pRef == NULL)
		return NULL;

	EgPos p = pRef->m_Pos;
	for (int i = 0; i < 5; ++i)
	{
		p[1] += 50.0;
		theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), p));
		int NumCut = theSlicer.Slice(pMesh);
		if (NumCut == 1)
			break;
	}

	if (theSlicer.GetNumCutLine() != 1)
		return NULL;

	double cntX = (pMesh->m_BndBox.m_MinPt[0] + pMesh->m_BndBox.m_MaxPt[0]) * 0.5;
	std::vector<EgPos> Pts;
	for (EgPos &p : theSlicer.m_CutLines[0])
	{
		if (p[0] < cntX)
			Pts.push_back(p);
	}

	std::vector<EgPos> SortedPts;
	SK_Sort_By_Curvature(Pts, SortedPts, true, false, EgVec3());

	EgVertex *v = FindClosestVert(pMesh, SortedPts[0]);
	std::vector<EgPos> Path;
	SK_Planar_Perimeter(v, &EgPlane(EgVec3(1.0, 0.0, 0.0), v->m_Pos), Path, false);

	EgPos minPt(0.0, -INFINITY, 0.0);
	for (EgPos &p : Path)
	{
		if (p[1] > minPt[1]) {
			minPt = p;
			_cprintf("%f %f %f\n", p[0], p[1], p[2]);
		}
	}

	return FindClosestVert(pMesh, minPt);
	//return pMesh->m_pVerts[32983];
}

/*!
*	\brief	왼쪽 어깨가쪽점을 찾는 함수
*	\note	마지막 수정일: 2020-07-19
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	왼쪽 어깨가쪽점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Lateral_Shoulder_Left(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("손끝점");
	if (pRef == NULL)
		return NULL;

	EgPos p = pRef->m_Pos;
	for (int i = 0; i < 5; ++i)
	{
		p[1] += 50.0;
		theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), p));
		int NumCut = theSlicer.Slice(pMesh);
		if (NumCut == 1)
			break;
	}

	if (theSlicer.GetNumCutLine() != 1)
		return NULL;

	double cntX = (pMesh->m_BndBox.m_MinPt[0] + pMesh->m_BndBox.m_MaxPt[0]) * 0.5;
	std::vector<EgPos> Pts;
	for (EgPos &p : theSlicer.m_CutLines[0])
	{
		if (p[0] > cntX)
			Pts.push_back(p);
	}
	std::vector<EgPos> SortedPts;
	SK_Sort_By_Curvature(Pts, SortedPts, true, false, EgVec3());

	EgVertex *v = FindClosestVert(pMesh, SortedPts[0]);
	std::vector<EgPos> Path;
	SK_Planar_Perimeter(v, &EgPlane(EgVec3(1.0, 0.0, 0.0), v->m_Pos), Path, false);

	EgPos minPt(0.0, -INFINITY, 0.0);
	for (EgPos &p : Path)
	{
		if (p[1] > minPt[1])
			minPt = p;
	}

	return FindClosestVert(pMesh, minPt);
	//return pMesh->m_pVerts[45923];
}

/*!
*	\brief	오른쪽어깨점을 찾는 함수
*	\note	마지막 수정일 : 2020-08-07
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	오른쪽 어깨점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Acromion_Right(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("오른쪽어깨가쪽점");
	if (pRef == NULL)
		return NULL;

	EgVertex *AR = SK_Elliptic_Vertex(pRef, 50.0);
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), AR->m_Pos));
	theSlicer.Slice(pMesh);

	EgPos minXPos(INFINITY, 0.0, 0.0);
	for (EgPos &p : theSlicer.m_CutLines[0])
		minXPos = (p[0] < minXPos[0]) ? p : minXPos;

	// 오른쪽 어깨점 찾음
	AR = FindClosestVert(pMesh, minXPos);

	// 오른쪽 어깨가쪽점 수정
	int idx = theSizer.GetLandmarkIdx("오른쪽어깨가쪽점");
	theSizer.m_Landmarks[idx] = SK_Extreme_Vertex(AR, 10.0, 2, 1);
	return AR;
}

/*!
*	\brief	왼쪽 어깨점을 찾는 함수
*	\note	마지막 수정일 : 2020-08-07
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	왼쪽 어깨점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Acromion_Left(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("왼쪽어깨가쪽점");
	if (pRef == NULL)
		return NULL;

	EgVertex *AL = SK_Elliptic_Vertex(pRef, 50.0);
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), AL->m_Pos));
	theSlicer.Slice(pMesh);

	EgPos maxXPos(-INFINITY, 0.0, 0.0);
	for (EgPos &p : theSlicer.m_CutLines[0])
		maxXPos = (p[0] > maxXPos[0]) ? p : maxXPos;

	// 왼쪽 어깨점 찾음
	AL = FindClosestVert(pMesh, maxXPos);

	// 왼쪽 어깨가쪽점 수정
	int idx = theSizer.GetLandmarkIdx("왼쪽어깨가쪽점");
	theSizer.m_Landmarks[idx] = SK_Extreme_Vertex(AL, 10.0, 2, 1);
	return AL;
}

/*!
*	\brief	오른쪽어깨세모근점을 찾는 함수
*	\note	마지막 수정일 : 2020-08-07
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	오른쪽어깨세모근점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Deltoid_Right(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("오른쪽어깨점");
	if (pRef == NULL)
		return NULL;

	//EgVertex *DR = SK_Elliptic_Vertex(pRef, 50.0);
	EgPos p = pRef->m_Pos;
	p[1] -= 60.0;
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), p));
	theSlicer.Slice(pMesh);

	EgPos minXPos(INFINITY, 0.0, 0.0);
	for (EgPos &p : theSlicer.m_CutLines[0])
		minXPos = (p[0] < minXPos[0]) ? p : minXPos;

	return FindClosestVert(pMesh, minXPos);
	//return pMesh->m_pVerts[28196];
}

/*!
*	\brief	왼쪽어깨세모근점을 찾는 함수
*	\note	마지막 수정일 : 2020-08-07
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	왼쪽어깨세모근점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Deltoid_Left(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("왼쪽어깨점");
	if (pRef == NULL)
		return NULL;

	EgPos p = pRef->m_Pos;
	p[1] -= 60.0;
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), p));
	theSlicer.Slice(pMesh);

	EgPos maxXPos(-INFINITY, 0.0, 0.0);
	for (EgPos &p : theSlicer.m_CutLines[0])
		maxXPos = (p[0] > maxXPos[0]) ? p : maxXPos;

	return FindClosestVert(pMesh, maxXPos);
	//return pMesh->m_pVerts[41886];
}

/*!
*	\brief	눈초리점을 찾는 함수
*	\note	마지막 수정일: 2020-08-07
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	눈초리점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Ectocanthus(EgMesh *pMesh)
{
	EgVertex *pRef1 = theSizer.GetLandmark("머리마루점");
	EgVertex *pRef2 = theSizer.GetLandmark("목뒤점");
	if (pRef1 == NULL || pRef2 == NULL)
		return NULL;

	// Step 1: 후보 코끝점을 구한다.
	EgPos maxZPt(0.0, 0.0, -INFINITY);
	for (EgVertex *v : pMesh->m_pVerts)	
		if (v->m_Pos[1] > pRef2->m_Pos[1] + 50.0 && v->m_Pos[2] > maxZPt[2])
			maxZPt = v->m_Pos;

	// 후보 코끝점을 지나고 x축에 수직한 평면으로 자른다.
	theSlicer.SetCutPlane(EgPlane(EgVec3(1.0, 0.0, 0.0), maxZPt));
	theSlicer.Slice(pMesh);

	// 절단점 중에서 
	std::vector<EgPos> Pts;
	for (EgPos p : theSlicer.m_CutLines[0])
	{
		// 높이가 (코끝점 + 1.0cm ~ 머리마루점 - 10.0cm) 사이에 있고
		if (p[1] > maxZPt[1] + 10.0 && p[1] < pRef1->m_Pos[1] - 100.0)
		{
			// 머리마루점 + 2.0cm 앞쪽에 있는 절단점을 모은다.
			if (p[2] > pRef1->m_Pos[2] + 20.0)
				Pts.push_back(p);
		}
	}

	// 음수 곡률을 갖는 절단점 들을 구별한다.
	std::vector<EgPos> NegPts;
	SK_Find_Negative_Curvature(Pts, NegPts, true, EgVec3(1.0, 0.0, 0.0));

	// 가장 작은 음수 곡률을 갖는 점을 지나고 y축을 법선으로 갖는 평면으로 절단한다.
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), NegPts[0]));
	theSlicer.CvxSlice(pMesh);

	// Step 2: 절단점 중에서 가장 큰 z값을 갖는 코끝점을 구한다.
	EgPos NosePt(0.0, 0.0, -INFINITY);
	for (EgPos p : theSlicer.m_CutLines[0])
		if (p[2] > NosePt[2])
			NosePt = p;

	// 코끝점을 지나고 x축에 수직한 평면으로 자른다.
	theSlicer.SetCutPlane(EgPlane(EgVec3(1.0, 0.0, 0.0), NosePt));
	theSlicer.Slice(pMesh);

	Pts.clear();
	for (EgPos p : theSlicer.m_CutLines[0])
	{
		// 높이가 (코끝점 + 1.0cm ~ 머리마루점 - 10.0cm) 사이에 있고
		if (p[1] > maxZPt[1] + 10.0 && p[1] < pRef1->m_Pos[1] - 100.0)
		{
			// 머리마루점 + 2.0cm 앞쪽에 있는 절단점을 모은다.
			if (p[2] > pRef1->m_Pos[2] + 20.0)
				Pts.push_back(p);
		}
	}

	// 음수 곡률을 갖는 점들을 구별한다.
	NegPts.clear();
	SK_Find_Negative_Curvature(Pts, NegPts, true, EgVec3(1.0, 0.0, 0.0));

	// 음수 곡률을 갖는 정들중에서 z좌표가 가장 작은 점을 구한다.
	EgPos minZPt(0.0, 0.0, INFINITY);
	for (EgPos p : NegPts)
	{
		if (dist(p, NosePt) < 10.0)
		{
			if (p[2] < minZPt[2])
				minZPt = p;
		}
	}
	
	// 오른쪽 눈의 위치로 이동한다.
	minZPt[0] -= 40.0;
	EgLine ray(minZPt, EgVec3(0.0, 0.0, -1.0));

	EgPos q(0.0, 0.0, -INFINITY);
	for (EgFace *f : pMesh->m_pFaces)
	{
		EgPos tmp;
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);
		if (::intersect_line_triangle(p0, p1, p2, ray, tmp, false))
		{
			double d = dist(minZPt, tmp);
			if (d > 0.0001 && tmp[2] > q[2])
				q = tmp;
		}
	}

	return FindClosestVert(pMesh, q);
}
// 이전 
//EgVertex *SK_Pose4_Ectocanthus(EgMesh *pMesh)
//{
//	EgVertex *pRef1 = theSizer.GetLandmark("머리마루점");
//	EgVertex *pRef2 = theSizer.GetLandmark("목뒤점");
//	if (pRef1 == NULL || pRef2 == NULL)
//		return NULL;
//
//	EgVertex *NoseVert = NULL;
//	EgPos maxZPt(0.0, 0.0, -INFINITY);
//	for (EgVertex *v : pMesh->m_pVerts)
//	{
//		// 목뒤점 위쪽 정점에 대하여
//		if (v->m_Pos[1] > pRef2->m_Pos[1])
//		{
//			if (v->m_Pos[2] > maxZPt[2])
//			{
//				NoseVert = v;
//				maxZPt = v->m_Pos;
//			}
//		}
//	}
//
//	// 코끝점을 지나는 평면으로 자른다.
//	theSlicer.SetCutPlane(EgPlane(EgVec3(1.0, 0.0, 0.0), maxZPt));
//	theSlicer.Slice(pMesh);
//
//	std::vector<EgPos> Pts;
//	for (EgPos p : theSlicer.m_CutLines[0])
//	{
//		if (p[1] > maxZPt[1] && p[2] > pRef1->m_Pos[2])
//			Pts.push_back(p);
//	}
//
//	std::vector<EgPos> SortedPts;
//	SK_Sort_By_Curvature(Pts, SortedPts, false, true, EgVec3(1.0, 0.0, 0.0));
//
//	EgPos pt(SortedPts[0][0] - 40.0, SortedPts[0][1], SortedPts[0][2]);
//	EgLine ray(pt, EgVec3(0.0, 0.0, -1.0));
//
//	EgPos q(0.0, 0.0, -INFINITY);
//	for (EgFace *f : pMesh->m_pFaces)
//	{
//		EgPos tmp;
//		EgPos p0 = f->GetVertexPos(0);
//		EgPos p1 = f->GetVertexPos(1);
//		EgPos p2 = f->GetVertexPos(2);
//		if (::intersect_line_triangle(p0, p1, p2, ray, tmp, false))
//		{
//			double d = dist(pt, tmp);
//			if (d > 0.0001 && tmp[2] > q[2])
//				q = tmp;
//		}
//	}
//
//	return FindClosestVert(pMesh, q);
//}

/*!
*	\brief	목뒤점을 찾는 함수
*	\note	마지막 수정일 : 2020-08-16
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	목뒤점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Cervicale(EgMesh *pMesh)
{
	// 목옆점을 찾는다.
	// 오른쪽어깨점높이 ~ 머리마루점높이의 오른쪽 실루엣을 구한다.
	EgVertex *pRef1 = theSizer.GetLandmark("머리마루점");
	EgVertex *pRef2 = theSizer.GetLandmark("오른쪽어깨점");
	EgVertex *pRef3 = theSizer.GetLandmark("왼쪽어깨점");
	if (pRef1 == NULL || pRef2 == NULL || pRef3 == NULL)
		return NULL;

	theSlicer.SetCutPlane(EgPlane(pRef1->m_Pos, pRef2->m_Pos, pRef3->m_Pos));
	theSlicer.Slice(pMesh);
	std::vector<EgPos> Silhouette;

	for (EgPos p : theSlicer.m_CutLines[0])
	{
		if (p[0] < pRef1->m_Pos[0] && p[1] > pRef2->m_Pos[1])
			Silhouette.push_back(p);
	}

	double maxDist = 0.0;
	EgPos maxPt;
	EgLine line(pRef2->m_Pos, pRef1->m_Pos);
	for (EgPos p : Silhouette)
	{
		double d = dist(line, p);

		if (d > maxDist)
		{
			maxDist = d;
			maxPt = p;
		}
	}

	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), maxPt));
	theSlicer.Slice(pMesh);
	EgPos minPt(0.0, 0.0, INFINITY);
	for (EgPos p : theSlicer.m_CutLines[0])
	{
		if (p[2] < minPt[2])
			minPt = p;
	}

	return FindClosestVert(pMesh, minPt);
	//return pMesh->m_pVerts[34076];
}

/*!
*	\brief	손목안쪽점을 찾는 함수
*	\note	마지막 수정일 : 2020-10-22
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	손목안쪽점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Ulnar_Styloid(EgMesh *pMesh)
{
	// 손끝점이 발견되지 않았다면 NULL을 반환한다.
	EgVertex *HD = theSizer.GetLandmark("손끝점");
	if (HD == NULL)
		return NULL;

	// 탐색 범위를 설정한다.
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];
	double minZ = pMesh->m_BndBox.m_MinPt[2];
	double maxZ = pMesh->m_BndBox.m_MaxPt[2];
	double y0 = 2 * (HD->m_Pos[1] - 100.0 - minY) / (maxY - minY) - 1.0;
	double y1 = 2 * (HD->m_Pos[1] + 100.0 - minY) / (maxY - minY) - 1.0;
	double z0 = 2 * (HD->m_Pos[2] - 250.0 - minZ) / (maxZ - minZ) - 1.0;
	double z1 = 1.0;
	double prevLen = INFINITY;
	std::vector<EgPos> PrevCut;
	EgPos minPt(0.0, INFINITY, 0.0);
	int snum = 20;
	for (int i = 0; i < snum; ++i)
	{
		double z = (double)i / (snum - 1);
		int NumCut = theSlicer.Slice(pMesh, 2, z, -1.0, 0.0, y0, y1, z0, z1, true);
		if (NumCut != 1)
			continue;

		PrevCut = theSlicer.m_CutLines[0];
		double Len = theSlicer.GetLength(0);
		if (Len > prevLen)
		{
			for (EgPos p : PrevCut)
			{
				if (p[1] < minPt[1])
					minPt = p;
			}
			break;
		}
		else
			prevLen = Len;
	}

	return FindClosestVert(pMesh, minPt);
}

/*!
*	\brief	(오른쪽)손끝점을 찾는 함수
*	\note	마지막 수정일 : 2020-07-20, 정확도(상)
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	(오른쪽)손끝점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Dactylion(EgMesh *pMesh)
{
	// Y축 최소/최대 좌표를 구한다.
	double minX = pMesh->m_BndBox.m_MinPt[0];
	double maxX = pMesh->m_BndBox.m_MaxPt[0];
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];

	// 탐색 영역을 결정한다.
	double low = minY + (maxY - minY) * 0.45;
	double high = minY + (maxY - minY) * 0.6;
	double center = (minX + maxX) * 0.5;

	// 탐색 영역내에서 최대 Z 좌표를 갖는 정점을 찾는다.
	EgVertex *AK = NULL;
	double maxZ = -INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[0] < center &&
			v->m_Pos[1] > low && v->m_Pos[1] < high &&
			v->m_Pos[2] > maxZ)
		{
			AK = v;
			maxZ = v->m_Pos[2];
		}
	}
	return AK;
}

/*!
*	\brief	앉은면수준점을 찾는 함수
*	\note	마지막 수정일 : 2020-09-24
*	\note	머리마루점을 먼저 찾아야 한다.
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	앉은면수준점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Base_Sit(EgMesh *pMesh)
{
	// 절단 평면 설정: Z = 7cm
	EgPlane Pln(EgVec3(0.0, 0.0, 1.0), EgPos(0.0, 0.0, 70));
	theSlicer.SetCutPlane(Pln);
	theSlicer.CvxSlice(pMesh);

	if (theSlicer.m_CutLines.empty())
	{
		_cprintf("앉은면수준면 에러...\n");
		return NULL;
	}

	EgPos MinPt(0.0, INFINITY, 0.0);
	for (EgPos &p : theSlicer.m_CutLines[0])
	{
		if (p[1] < MinPt[1])
			MinPt = p;
	}

	return FindClosestVert(pMesh, MinPt);
}
// 이전 버전: 2020-07-19
//EgVertex *SK_Pose4_Base_Sit(EgMesh *pMesh)
//{
//	// 머리마루점이 발견되지 않았다면 NULL을 반환한다.
//	EgVertex *HT = theSizer.GetLandmark("머리마루점");
//	if (HT == NULL)
//		return NULL;
//
//	// 머리마루점을 지나 -Y 축 방향의 직선을 생성한다.
//	EgLine ray(HT->m_Pos, EgVec3(0.0, -1.0, 0.0));
//
//	EgVertex *BS = NULL;
//	double minDist = INFINITY;
//	for (EgFace *f : pMesh->m_pFaces)
//	{
//		EgPos q;
//		EgPos p0 = f->GetVertexPos(0);
//		EgPos p1 = f->GetVertexPos(1);
//		EgPos p2 = f->GetVertexPos(2);
//		if (::intersect_line_triangle(p0, p1, p2, ray, q, false))
//		{
//			double d = dist(HT->m_Pos, q);
//			if (d > 0.0001 && d < minDist)
//			{
//				minDist = d;
//				BS = f->GetVertex(0);
//			}
//		}
//	}
//	return BS;
//}

/*!
*	\brief	앉은넙다리위점을 찾는 함수
*	\note	마지막 수정일 : 2020-07-24
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	앉은넙다리위점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Superior_Thigh_Sit(EgMesh *pMesh)
{
	// 앉은무릎앞점이 발견되지 않았다면 NULL을 반환한다.
	EgVertex *AK = theSizer.GetLandmark("앉은무릎앞점");
	if (AK == NULL)
		return NULL;

	EgLine ray(AK->m_Pos, EgVec3(0.0, 0.0, -1.0));
	EgPos q(0.0, 0.0, INFINITY);
	for (EgFace *f : pMesh->m_pFaces)
	{
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);
		EgPos tmp;
		if (::intersect_line_triangle(p0, p1, p2, ray, tmp, false))
		{
			double d = dist(AK->m_Pos, tmp);
			if (d > 0.0001 && tmp[2] < q[2])
				q = tmp;
		}
	}

	std::vector<EgPos> IsPts;
	EgPos p = lerp(AK->m_Pos, q, 0.5);
	int snum = 5;
	for (int i = 0; i < snum; ++i)
	{
		EgVec3 v(i / (double)(snum - 1) - 0.5, 1.0, 0.0);
		EgLine ray(p, v);
		double minDist = INFINITY;
		EgPos FirstPt;
		for (EgFace *f : pMesh->m_pFaces)
		{
			EgPos p0 = f->GetVertexPos(0);
			EgPos p1 = f->GetVertexPos(1);
			EgPos p2 = f->GetVertexPos(2);
			double param;
			if (::intersect_line_triangle2(p0, p1, p2, ray, param, false) && param > 0.0)
			{
				double d = dist(p, ray.Eval(param));
				if (d < minDist)
				{
					minDist = d;
					FirstPt = ray.Eval(param);
				}
			}
		}
		IsPts.push_back(FirstPt);
	}

	if (IsPts.empty())
		return NULL;

	EgPos MaxPt(0.0, -INFINITY, 0.0);
	for (EgPos p : IsPts)
	{
		if (p[1] > MaxPt[1])
			MaxPt = p;
	}

	return FindClosestVert(pMesh, MaxPt);
}

/*!
*	\brief	무릎뼈위점을 찾는 함수
*	\note	마지막 수정일 : 2020-07-19
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	무릎뼈위점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Superior_Patella_Sit(EgMesh *pMesh)
{
	// 앉은오금점이 발견되지 않았다면 NULL을 반환한다.
	EgVertex *pRef1 = theSizer.GetLandmark("앉은무릎앞점");
	if (pRef1 == NULL)
		return NULL;

	EgVertex *SPS = SK_Extreme_Vertex(pRef1, 30.0, 2, 1);
	return SPS;
	//return pMesh->m_pVerts[16045];
}

/*!
*	\brief	앉은오금점을 찾는 함수
*	\note	마지막 수정일 : 2020-07-20, 정확도(상)
*	\note	앉은무릎앞점을 먼저 찾아야 한다.
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	앉은오금점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Posterior_Juncture_Calf_Thigh(EgMesh *pMesh)
{
	// 앉은무릎앞점이 발견되지 않았다면 NULL을 반환한다.
	EgVertex *AK = theSizer.GetLandmark("앉은무릎앞점");
	if (AK == NULL)
		return NULL;

	EgVertex *PJCT = NULL;
	int snum = 10;
	double minDist = INFINITY;
	for (int i = 0; i < snum; ++i)
	{
		EgVec3 v(0.0, -i / (double)(snum - 1), -1.0);
		EgLine ray(AK->m_Pos, v);
		for (EgFace *f : pMesh->m_pFaces)
		{
			EgPos q;
			EgPos p0 = f->GetVertexPos(0);
			EgPos p1 = f->GetVertexPos(1);
			EgPos p2 = f->GetVertexPos(2);
			if (::intersect_line_triangle(p0, p1, p2, ray, q, false))
			{
				double d = dist(AK->m_Pos, q);
				if (d > 0.0001 && d < minDist)
				{
					minDist = d;
					PJCT = f->GetVertex(0);
				}
			}
		}
	}

	return PJCT;
	//return pMesh->m_pVerts[14261];
}

/*!
*	\brief	(오른쪽)앉은엉덩이사이최대돌출수준점을 찾는 함수
*	\note	마지막 수정일 : 2020-07-22
*	\note	앉은무릎앞점을 먼저 찾아야 한다.
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	(오른쪽)앉은엉덩이사이최대돌출수준점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Max_Hip_Width_Right(EgMesh *pMesh)
{
	// 앉은무릎앞점과 머리마루점이 발견되지 않았다면 NULL을 반환한다.
	EgVertex *AK = theSizer.GetLandmark("앉은무릎앞점");
	EgVertex *HT = theSizer.GetLandmark("머리마루점");
	if (AK == NULL || HT == NULL)
		return NULL;

	// 탐색 범위를 설정한다.
	double minY = AK->m_Pos[1] - 150;
	double maxY = AK->m_Pos[1] + 150;

	EgVertex *MHW_R = NULL;
	double minX = INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] > minY &&
			v->m_Pos[1] < maxY &&
			v->m_Pos[2] < HT->m_Pos[2] &&
			v->m_Pos[0] < minX)
		{
			minX = v->m_Pos[0];
			MHW_R = v;
		}
	}

	return MHW_R;
}

/*!
*	\brief	(왼쪽)앉은엉덩이사이최대돌출수준점을 찾는 함수
*	\note	마지막 수정일 : 2020-07-22
*	\note	앉은무릎앞점을 먼저 찾아야 한다.
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	(왼쪽)앉은엉덩이사이최대돌출수준점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Max_Hip_Width_Left(EgMesh *pMesh)
{
	// 앉은무릎앞점과 머리마루점이 발견되지 않았다면 NULL을 반환한다.
	EgVertex *AK = theSizer.GetLandmark("앉은무릎앞점");
	EgVertex *HT = theSizer.GetLandmark("머리마루점");
	if (AK == NULL || HT == NULL)
		return NULL;

	// 탐색 범위를 설정한다.
	double minY = AK->m_Pos[1] - 150;
	double maxY = AK->m_Pos[1] + 150;

	EgVertex *MHW_L = NULL;
	double maxX = -INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] > minY &&
			v->m_Pos[1] < maxY &&
			v->m_Pos[2] < HT->m_Pos[2] &&
			v->m_Pos[0] > maxX)
		{
			maxX = v->m_Pos[0];
			MHW_L = v;
		}
	}

	return MHW_L;
	//return pMesh->m_pVerts[373];
}

/*!
*	\brief	(오른쪽)앉은무릎앞점을 찾는 함수
*	\note	마지막 수정일 : 2020-07-20, 정확도(상)
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	(오른쪽)앉은무릎앞점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Anterior_Knee_Sit(EgMesh *pMesh)
{
	// Y축 최소/최대 좌표를 구한다.
	double minX = pMesh->m_BndBox.m_MinPt[0];
	double maxX = pMesh->m_BndBox.m_MaxPt[0];
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];

	// 탐색 영역을 결정한다.
	double low = minY + (maxY - minY) * 0.2;
	double high = minY + (maxY - minY) * 0.5;
	double center = (minX + maxX) * 0.5;

	// 탐색 영역내에서 최대 Z 좌표를 갖는 정점을 찾는다.
	EgVertex *AK = NULL;
	double maxZ = -INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[0] < center &&
			v->m_Pos[1] > low && v->m_Pos[1] < high &&
			v->m_Pos[2] > maxZ)
		{
			AK = v;
			maxZ = v->m_Pos[2];
		}
	}
	return AK;
	//return pMesh->m_pVerts[14999];
}

/*!
*	\brief	앉은배돌출점을 찾는 함수
*	\note	마지막 수정일 : 2020-10-22
*
*	\param	pMesh[in]	스캔 메쉬에 대한 포인터
*
*	\return	앉은배돌출점의 포인터를 반환한다
*/
EgVertex *SK_Pose4_Abdominal_Protrusion_Sit(EgMesh *pMesh)
{
	// 참조점: P: 머리마루점, Q: 넙다리위점, R: 팔꿈치아래점
	EgVertex *P = theSizer.GetLandmark("머리마루점");
	EgVertex *Q = theSizer.GetLandmark("앉은넙다리위점");
	EgVertex *R = theSizer.GetLandmark("팔꿈치아래점");

	// 참조점들이 발견되지 않았다면 NULL을 반환한다.
	if (P == NULL || Q == NULL || R == NULL)
		return NULL;

	// 머리마루점을 지나고 x축을 법선으로하는 평면으로 메쉬를 자른다.
	theSlicer.SetCutPlane(EgPlane(EgVec3(1.0, 0.0, 0.0), P->m_Pos));
	theSlicer.Slice(pMesh);

	// (넙다리위점 + 2cm) ~ (팔꿈치아래점 - 2cm) 높이에서 z값이 최대인 점을 찾는다.
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];
	double y0 = minY + (maxY - minY) * 0.44;
	double y1 = minY + (maxY - minY) * 0.46;
	EgPos maxPt(0.0, 0.0, -INFINITY);
	for (EgPos p : theSlicer.m_CutLines[0])
		if (p[1] > Q->m_Pos[1] + 20.0 && p[1] < R->m_Pos[1] - 20.0 && p[2] > maxPt[2])
			maxPt = p;

	// 특징점을 반환한다.
	return FindClosestVert(pMesh, maxPt);
}
