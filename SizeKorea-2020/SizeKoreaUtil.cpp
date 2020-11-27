#include "pch.h"
#include "SizeKoreaUtil.h"


EgSlicer theSlicer;
EgSizer theSizer;

/**********************/
/* EgSlicer 클래스 구현 */
/**********************/
/*!
*	\brief	생성자
*	\note	마지막 수정일: 2020-07-08
*/
EgSlicer::EgSlicer()
{
}

/*!
*	\brief	생성자
*	\note	마지막 수정일: 2020-07-08
*
*	\param	CutPln[in]	절단 평면
*/
EgSlicer::EgSlicer(EgPlane CutPln)
{
	m_CutPln = CutPln;
}

/*!
*	\brief	복사 생성자
*	\note	마지막 수정일: 2020-07-08
*
*	\param	cpy[in]		복사될 객체
*/
EgSlicer::EgSlicer(const EgSlicer &cpy)
{
	m_CutPln = cpy.m_CutPln;
	m_CutLines = cpy.m_CutLines;
}

/*!
*	\brief	소멸자
*	\note	마지막 수정일: 2020-07-08
*/
EgSlicer::~EgSlicer()
{
}

/*!
*	\brief	대입 연산자
*	\note	마지막 수정일: 2020-07-08
*
*	\param	rhs[in]		대입될 객체
*
*	\return	대입된 자신을 반환한다.
*/
EgSlicer &EgSlicer::operator=(const EgSlicer &rhs)
{
	m_CutPln = rhs.m_CutPln;
	m_CutLines = rhs.m_CutLines;
	return *this;
}

/*!
*	\brief	절단 평면을 설정한다.
*	\note	마지막 수정일: 2020-07-08
*
*	\param	CutPln[in]	설정할 절단 평면
*/
void EgSlicer::SetCutPlane(EgPlane CutPln)
{
	m_CutPln = CutPln;
}

/*!
*	\brief	메쉬를 절단하고 절단선을 길이에 따라 내림차순으로 정렬한다.
*	\note	마지막 수정일: 2020-07-08
*
*	\param	pMesh[in]	대상 메쉬에 대한 포인터
*
*	\return	절단 후, 연결된 절단선의 수를 반환한다.
*/
int EgSlicer::Slice(EgMesh *pMesh)
{
	// 절단면과 절단선 정보를 구한다.
	std::vector<std::vector<std::pair<EgEdge *, float>>> CutInfoList;
	::intersect_plane_mesh(pMesh, &m_CutPln, m_CutLines, CutInfoList);

	// 절단선의 길이에 따라 내림 차순으로 정렬한다.
	SortByLength(true);

	// 연결된 절단선의 수를 반환한다.
	return NUM(m_CutLines);
}

/*!
*	\brief	메쉬를 절단하고 특정 영역내에 포함된 절단선을 선택한다
*	\note	마지막 수정일: 2020-07-26
*
*	\param	pMesh[in]	대상 메쉬에 대한 포인터
*	\param	Pivot[in]	영역의 중심점
*	\param	dx[in]		영역의 중심점에서 x 방향 범위
*	\param	dy[in]		영역의 중심점에서 y 방향 범위
*	\param	dz[in]		영역의 중심점에서 z 방향 범위
*
*	\return	절단 후, 연결된 절단선의 수를 반환한다.
*/
int EgSlicer::Slice(EgMesh *pMesh, EgPos Pivot, double dx, double dy, double dz)
{
	// 메쉬를 절단하고,
	Slice(pMesh);

	// 영역을 정의하고,
	double minX = Pivot[0] - dx;
	double maxX = Pivot[0] + dx;
	double minY = Pivot[1] - dy;
	double maxY = Pivot[1] + dy;
	double minZ = Pivot[2] - dz;
	double maxZ = Pivot[2] + dz;

	// 영역내에 포함된 절단선을 선별하여
	std::vector<std::vector<EgPos>> NewCutLines;
	for (std::vector<EgPos> &Cut : m_CutLines)
	{
		bool flag = true;
		for (EgPos &p : Cut)
		{
			if (p[0] < minX || p[0] > maxX)
				flag = false;

			if (p[1] < minY || p[1] > maxY)
				flag = false;

			if (p[2] < minZ || p[2] > maxZ)
				flag = false;

			if (flag == false)
				break;
		}

		if (flag == true)
			NewCutLines.push_back(Cut);
	}

	// 절단선을 갱신하고, 절단선의 수를 반환한다.
	m_CutLines.clear();
	m_CutLines = NewCutLines;
	return NUM(m_CutLines);
}

/*!
*	\brief	평면 절단 후 주어진 조건을 만족하는 절단선을 구한다.
*	\note	마지막 수정일: 2020-07-11
*
*	\param	pMesh[in]		대상 메쉬에 대한 포인터
*	\param	PlnType[in]		절단 평면 타입(0: Sagittal 평면(X축 법선), 1: Transverse 평면(Y축 법선), 2: Coronal 평면(Z축 법선))
*	\param	Param[in]		탐색 영역내에서 평면의 상대적 위치(0.0 ~ 1.0)
*	\param	MinX			메쉬 경계 상자 기준 X 축 탐색 범위 최소값(-1.0 ~ 1.0)
*	\param	MaxX			메쉬 경계 상자 기준 X 축 탐색 범위 최대값(-1.0 ~ 1.0)
*	\param	MinY			메쉬 경계 상자 기준 Y 축 탐색 범위 최소값(-1.0 ~ 1.0)
*	\param	MaxY			메쉬 경계 상자 기준 Y 축 탐색 범위 최대값(-1.0 ~ 1.0)
*	\param	MinZ			메쉬 경계 상자 기준 Z 축 탐색 범위 최소값(-1.0 ~ 1.0)
*	\param	MaxZ			메쉬 경계 상자 기준 Z 축 탐색 범위 최대값(-1.0 ~ 1.0)
*	\param	bClosedCut[in]	닫힌 절단선만 선택할지 여부
*
*	\return	탐색 영역내에 포함된 절단선의 수를 반환한다.
*/
int EgSlicer::Slice(
	EgMesh *pMesh,
	int PlnType,
	double Param,
	double MinX, double MaxX,
	double MinY, double MaxY,
	double MinZ, double MaxZ,
	bool bClosedCut)
{
	// 예외 처리를 한다.
	if (PlnType < 0 || PlnType > 2)
	{
		printf("[Error]: SK_Cross_Section(...)...\n");
		return -1;
	}

	// 탐색 영역을 구하고
	EgPos p = pMesh->m_BndBox.m_MinPt;
	EgPos q = pMesh->m_BndBox.m_MaxPt;
	double x0 = ((1.0 - MinX) * p[0] + (1.0 + MinX) * q[0]) * 0.5;
	double x1 = ((1.0 - MaxX) * p[0] + (1.0 + MaxX) * q[0]) * 0.5;
	double y0 = ((1.0 - MinY) * p[1] + (1.0 + MinY) * q[1]) * 0.5;
	double y1 = ((1.0 - MaxY) * p[1] + (1.0 + MaxY) * q[1]) * 0.5;
	double z0 = ((1.0 - MinZ) * p[2] + (1.0 + MinZ) * q[2]) * 0.5;
	double z1 = ((1.0 - MaxZ) * p[2] + (1.0 + MaxZ) * q[2]) * 0.5;

	// 탐색 영역내의 객체의 경계를 구한다.
	p.Set(INFINITY, INFINITY, INFINITY);
	q.Set(-INFINITY, -INFINITY, -INFINITY);
	for (EgVertex *v : pMesh->m_pVerts)
	{
		double x = v->m_Pos[0];
		double y = v->m_Pos[1];
		double z = v->m_Pos[2];
		if (x < x0 || x > x1 || y < y0 || y > y1 || z < z0 || z > z1)
			continue;
		p[0] = MIN(p[0], x);
		p[1] = MIN(p[1], y);
		p[2] = MIN(p[2], z);
		q[0] = MAX(q[0], x);
		q[1] = MAX(q[1], y);
		q[2] = MAX(q[2], z);
	}

	// 절단 평면을 설정한다.
	if (PlnType == 0)
		m_CutPln = EgPlane(EgVec3(1.0, 0.0, 0.0), lerp(p, q, Param));
	else if (PlnType == 1)
		m_CutPln = EgPlane(EgVec3(0.0, 1.0, 0.0), lerp(p, q, Param));
	else
		m_CutPln = EgPlane(EgVec3(0.0, 0.0, 1.0), lerp(p, q, Param));

	// 절단선의 리스트를 구하여, 탐색 영역내의 절단선만 선택한다.
	int NumCut = Slice(pMesh);
	std::vector<std::vector<EgPos>> SltCutList;
	for (int i = 0; i < NumCut; ++i)
	{
		if (bClosedCut && !IsClosed(i))
			continue;

		int NumPts = NUM(m_CutLines[i]);
		for (int j = 0; j < NumPts; ++j)
		{
			EgPos pt = m_CutLines[i][j];
			if (pt[0] > x0 && pt[0] < x1 && pt[1] > y0 && pt[1] < y1 && pt[2] > z0 && pt[2] < z1)
			{
				SltCutList.push_back(m_CutLines[i]);
				break;
			}
		}
	}

	// 조건을 만족하는 절단선의 수를 반환한다.
	m_CutLines = SltCutList;
	return NUM(m_CutLines);
}

/*!
*	\brief	평면 절단 후 절단점들의 Convex hull을 구한다.
*	\note	마지막 수정일: 2020-09-07
*
*	\param	pMesh[in]		대상 메쉬에 대한 포인터
*	\param	PlnType[in]		절단 평면 타입(0: Sagittal 평면(X축 법선), 1: Transverse 평면(Y축 법선), 2: Coronal 평면(Z축 법선))
*	\param	Param[in]		탐색 영역내의 객체가 포함된 영역에서 평면의 상대적 위치(0.0 ~ 1.0)
*	\param	MinX			메쉬 경계 상자 기준 X 축 탐색 범위 최소값(-1.0 ~ 1.0)
*	\param	MaxX			메쉬 경계 상자 기준 X 축 탐색 범위 최대값(-1.0 ~ 1.0)
*	\param	MinY			메쉬 경계 상자 기준 Y 축 탐색 범위 최소값(-1.0 ~ 1.0)
*	\param	MaxY			메쉬 경계 상자 기준 Y 축 탐색 범위 최대값(-1.0 ~ 1.0)
*	\param	MinZ			메쉬 경계 상자 기준 Z 축 탐색 범위 최소값(-1.0 ~ 1.0)
*	\param	MaxZ			메쉬 경계 상자 기준 Z 축 탐색 범위 최대값(-1.0 ~ 1.0)
*
*	\return Convexhull의 길이를 반환한다.
*/
double EgSlicer::CvxSlice(EgMesh *pMesh, int PlnType, double Param, double MinX, double MaxX, double MinY, double MaxY, double MinZ, double MaxZ)
{
	// 예외 처리를 한다.
	if (PlnType < 0 || PlnType > 2)
	{
		_cprintf("[Error]: EgSlicer::CvxSlice()...\n");
		return -1.0;
	}

	// 탐색 영역을 구하고
	EgPos p = pMesh->m_BndBox.m_MinPt;
	EgPos q = pMesh->m_BndBox.m_MaxPt;
	double x0 = ((1.0 - MinX) * p[0] + (1.0 + MinX) * q[0]) * 0.5;
	double x1 = ((1.0 - MaxX) * p[0] + (1.0 + MaxX) * q[0]) * 0.5;
	double y0 = ((1.0 - MinY) * p[1] + (1.0 + MinY) * q[1]) * 0.5;
	double y1 = ((1.0 - MaxY) * p[1] + (1.0 + MaxY) * q[1]) * 0.5;
	double z0 = ((1.0 - MinZ) * p[2] + (1.0 + MinZ) * q[2]) * 0.5;
	double z1 = ((1.0 - MaxZ) * p[2] + (1.0 + MaxZ) * q[2]) * 0.5;

	// 탐색 영역내의 객체의 경계를 구한다.
	p.Set(INFINITY, INFINITY, INFINITY);
	q.Set(-INFINITY, -INFINITY, -INFINITY);
	for (EgVertex *v : pMesh->m_pVerts)
	{
		double x = v->m_Pos[0];
		double y = v->m_Pos[1];
		double z = v->m_Pos[2];
		if (x < x0 || x > x1 || y < y0 || y > y1 || z < z0 || z > z1)
			continue;
		p[0] = MIN(p[0], x);
		p[1] = MIN(p[1], y);
		p[2] = MIN(p[2], z);
		q[0] = MAX(q[0], x);
		q[1] = MAX(q[1], y);
		q[2] = MAX(q[2], z);
	}
	x0 = p[0];
	y0 = p[1];
	z0 = p[2];
	x1 = q[0];
	y1 = q[1];
	z1 = q[2];

	// 절단 평면을 설정한다.
	if (PlnType == 0)
		m_CutPln = EgPlane(EgVec3(1.0, 0.0, 0.0), lerp(p, q, Param));
	else if (PlnType == 1)
		m_CutPln = EgPlane(EgVec3(0.0, 1.0, 0.0), lerp(p, q, Param));
	else
		m_CutPln = EgPlane(EgVec3(0.0, 0.0, 1.0), lerp(p, q, Param));

	// 절단점의 리스트를 구한다.
	std::vector<EgPos> CutPts;
	for (EgFace *f : pMesh->m_pFaces)
	{
		for (int i = 0; i < 3; ++i)
		{
			EgPos v = f->GetVertexPos(i);
			if (v[0] < x0 || v[0] > x1)
				break;
			if (v[1] < y0 || v[1] > y1)
				break;
			if (v[2] < z0 || v[2] > z1)
				break;

			EgEdge *e = f->GetEdge(i);
			float t;
			if (::intersect_plane_edge(&m_CutPln, e, &t))
			{
				EgPos pt = SV(e)->m_Pos + t * (EV(e)->m_Pos - SV(e)->m_Pos);
				CutPts.push_back(pt);
			}
		}
	}

	std::vector<EgPos> CvxPts;
	double len = SK_ConvexHull(&m_CutPln, CutPts, CvxPts);

	m_CutLines.clear();
	m_CutLines.push_back(CvxPts);

	return len;
}


/*!
*	\brief	평면 절단 후 절단점들의 Convex hull을 구한다.
*	\note	마지막 수정일: 2020-09-07
*
*	\param	pMesh[in]		대상 메쉬에 대한 포인터
*/
void EgSlicer::CvxSlice(EgMesh *pMesh)
{
	// 절단점의 리스트를 구한다.
	std::vector<EgPos> CutPts;
	for (EgFace *f : pMesh->m_pFaces)
	{
		for (int i = 0; i < 3; ++i)
		{
			EgEdge *e = f->GetEdge(i);
			float t;
			if (::intersect_plane_edge(&m_CutPln, e, &t))
			{
				EgPos pt = SV(e)->m_Pos + t * (EV(e)->m_Pos - SV(e)->m_Pos);
				CutPts.push_back(pt);
			}
		}
	}

	std::vector<EgPos> CvxPts;
	double len = SK_ConvexHull(&m_CutPln, CutPts, CvxPts);

	m_CutLines.clear();
	m_CutLines.push_back(CvxPts);
}

/*!
*	\brief	연결된 절단선의 수를 반환한다.
*	\note	마지막 수정일: 2020-07-08
*
*	\return	연결된 절단선의 수를 반환한다.
*/
int EgSlicer::GetNumCutLine()
{
	return NUM(m_CutLines);
}

/*!
*	\brief	절단선을 길이에 따라 정렬한다.
*	\note	마지막 수정일: 2020-07-08
*
*	\param	bDescend[in]	정렬 옵션(내림차순: true, 오름차순: false)
*/
void EgSlicer::SortByLength(bool bDescend)
{
	// 각 절단선의 길이를 구한다.
	int NumCuts = GetNumCutLine();
	std::vector<double> Lengths;
	for (int i = 0; i < NumCuts; ++i)
		Lengths.push_back(GetLength(i));

	// 절단선을 옵션에 따라 정렬한다.
	for (int i = 0; i < NumCuts; ++i)
	{
		for (int j = i + 1; j < NumCuts; ++j)
		{
			// 내림 차수이라면
			if (bDescend)
			{
				if (Lengths[i] < Lengths[j])
				{
					SWAP(double, Lengths[i], Lengths[j]);
					SWAP(std::vector<EgPos>, m_CutLines[i], m_CutLines[j]);
				}
			}
			else // 오름 차순이라면
			{
				if (Lengths[i] > Lengths[j])
				{
					SWAP(double, Lengths[i], Lengths[j]);
					SWAP(std::vector<EgPos>, m_CutLines[i], m_CutLines[j]);
				}
			}
		}
	}
}

/*!
*	\brief	절단선의 중심을 구한다.
*	\note	마지막 수정일: 2020-07-08
*
*	\param	idx[in]		절단선의 인덱스
*
*	\return	절단선의 중심을 반환한다.
*/
EgPos EgSlicer::GetCenterPos(int idx)
{
	int NumCuts = NUM(m_CutLines);
	if (idx < 0 || idx > NumCuts - 1)
	{
		printf("[Error]: EgSlicer::GetCenterPos(...)...\n");
		return EgPos();
	}

	EgVec3 CntPt;
	int NumPts = (int)m_CutLines[idx].size();
	for (int i = 0; i < NumPts; ++i)
		CntPt += cast_vec3(m_CutLines[idx][i]);

	CntPt /= (double)NumPts;
	return cast_pos(CntPt);
}

/*!
*	\brief	절단선의 길이를 구한다.
*	\note	마지막 수정일: 2020-07-08
*
*	\param	idx[in]		절단선의 인덱스
*
*	\return	절단선의 길이를 반환한다.
*/
double EgSlicer::GetLength(int idx)
{
	int NumCuts = NUM(m_CutLines);
	if (idx < 0 || idx > NumCuts - 1)
	{
		printf("[Error]: EgSlicer::GetLength(...)...\n");
		return 0.0;
	}

	double Len = 0.0;
	int NumPts = (int)m_CutLines[idx].size();
	for (int i = 0; i < NumPts - 1; ++i)
		Len += dist(m_CutLines[idx][i], m_CutLines[idx][i + 1]);
	return Len;
}

/*!
*	\brief	절단선이 닫혀 있는지 조사한다.
*	\note	마지막 수정일: 2020-07-08
*
*	\param	idx[in]		절단선의 인덱스
*
*	\return	절단선이 닫혀 있으면 true, 열려 있으면 false를 반환한다.
*/
bool EgSlicer::IsClosed(int idx)
{
	int NumCuts = NUM(m_CutLines);
	if (idx < 0 || idx > NumCuts - 1)
	{
		printf("[Error]: EgSlicer::IsClosed(...)...\n");
		return false;
	}

	int NumPts = (int)m_CutLines[idx].size();
	return (m_CutLines[idx][0] == m_CutLines[idx][NumPts - 1]) ? true : false;
}

/*!
*	\brief	절단면의 각 절단선을 렌더링한다.
*	\note	마지막 수정일: 2020-07-08
*/
void EgSlicer::Render()
{
	glDisable(GL_LIGHTING);
	{
		int NumCuts = GetNumCutLine();
		glLineWidth(3.0f);
		glPointSize(5.0f);
		for (int i = 0; i < NumCuts; ++i)
		{
			double t = (double)i / double(NumCuts);
			EgVec3 C = ::get_color_map_from_rainbow(t);
			glColor3dv(C.m_Coords);

			// 절단선을 렌더링 한다.
			glBegin(GL_LINE_STRIP);
			for (int j = 0; j < NUM(m_CutLines[i]); ++j)
			{
				//double t = (double)j / NUM(m_CutLines[i]);
				//EgVec3 C = ::get_color_map_from_rainbow(t);
				//glColor3dv(C.m_Coords);
				glVertex3dv(m_CutLines[i][j].m_Coords);
			}
			glEnd();

			// 부호를 고려하여 오름차순으로 검사.
			std::vector<EgPos> SortedPts;
			SK_Sort_By_Curvature(m_CutLines[i], SortedPts, false, true, theSlicer.m_CutPln.N(true));

			glPointSize(4.0f);
			glBegin(GL_POINTS);
			glColor3d(1.0, 1.0, 1.0);
			for (int j = 0; j < NUM(m_CutLines[i]); ++j)
				glVertex3dv(m_CutLines[i][j].m_Coords);
			glEnd();

			glPointSize(6.0f);
			glBegin(GL_POINTS);
			for (int j = 0; j < MIN(NUM(SortedPts), 10); ++j)
				//for (int j = 0; j < NUM(SortedPts); ++j)
			{
				//double t = (double)j / NUM(Pts);
				//EgVec3 C = ::get_color_map_from_rainbow(t);
				//glColor3dv(C.m_Coords);
				glColor3d(0.0, 0.0, 0.0);
				glVertex3dv(SortedPts[j].m_Coords);
			}
			glEnd();


			//// 절단점을 렌더링한다.
			//glColor3f(0.0f, 0.0f, 0.0f);			
			//glBegin(GL_POINTS);
			//for (int j = 0; j < NUM(m_CutLines[i]); ++j)
			//	glVertex3dv(m_CutLines[i][j].m_Coords);
			//glEnd();
		}
		glLineWidth(1.0f);
		glPointSize(1.0f);
	}
	glEnable(GL_LIGHTING);
}

/*!
*	\brief	초기화 한다.
*	\note	마지막 수정일: 2020-07-13
*/
void EgSlicer::Reset()
{
	m_CutLines.clear();
}

//////////////////////////////////////////////////////////////////////////
static bool SK_Import_Obj_Mtl(EgMesh *pMesh, const char *fname)
{
	// 생성할 재질 변수를 정의한다.
	EgMaterial *pMtl = NULL;

	// 파일을 열고, 
	FILE *fp;
	fopen_s(&fp, fname, "r");
	if (!fp) // 실패하면 false를 반환한다.
	{
		printf("\tImporting %s failed...\n", fname);
		return false;
	}
	else
		printf("\tImporting %s...\n", fname);

	// 파일의 끝까지 한 단어씩 읽어, tag 배열에 저장한다.
	char tag[256];
	while (fscanf_s(fp, "%s", tag, 256) != EOF)
	{
		// newmtl (new material) 태그라면,
		if (!strcmp(tag, "newmtl"))
		{
			// 재질의 이름을 읽고,
			char tmp[256];
			fscanf_s(fp, "%s", tmp, 256);

			// 재질을 생성하여, 메쉬의 재질 리스트에 추가한다.
			pMtl = new EgMaterial(tmp);
			pMesh->AddMaterial(pMtl);
		}

		// Ka (ambient coefficients) 태그라면,
		if (!strcmp(tag, "Ka"))
		{
			// ambient 성분을 읽어서 재질을 설정한다.
			GLfloat c[3];
			fscanf_s(fp, "%f%f%f", &c[0], &c[1], &c[2]);
			pMtl->SetAmbient(c[0], c[1], c[2]);
		}

		// Kd (diffuse coefficients) 태그라면,
		if (!strcmp(tag, "Kd"))
		{
			// diffuse 성분을 읽어서 재질을 설정한다.
			GLfloat c[3];
			fscanf_s(fp, "%f%f%f", &c[0], &c[1], &c[2]);
			pMtl->SetDiffuse(c[0], c[1], c[2]);
		}

		// Ks (specular coefficients) 태그라면,
		if (!strcmp(tag, "Ks"))
		{
			// specular 성분을 읽어서 재질을 설정한다.
			GLfloat c[3];
			fscanf_s(fp, "%f%f%f", &c[0], &c[1], &c[2]);
			pMtl->SetSpecular(c[0], c[1], c[2]);
		}

		// map_Kd (diffuse texture file) 태그라면,
		if (!strcmp(tag, "map_Kd"))
		{
			// 텍스처 파일을 읽는다.
			char tex_name[512];
			fscanf_s(fp, "%s", tex_name, 512);

			// 이미 로드된 텍스처라면 기존의 텍스처를 설정한다.
			pMtl->m_pTexture = pMesh->GetTexture(tex_name);

			// 새로운 텍스처라면
			if (pMtl->m_pTexture == NULL)
			{
				// 새로운 텍스처를 생성하여 메쉬에 추가하고, 재질에 설정한다.
				EgTexture *pTexture = ::create_texture(tex_name);
				if (pTexture != NULL)
				{
					pMesh->AddTexture(pTexture);
					pMtl->SetTexture(pTexture);
				}
			}
		}
	}

	// 파일을 닫는다.
	fclose(fp);
	return true;
}

EgMesh *SK_Import_Obj(CString FileName)
{
	// 파일을 열고 실패하면 false를 반환한다.
	FILE *fp;
	std::string fname((LPCTSTR)FileName);
	fopen_s(&fp, fname.c_str(), "r");
	if (!fp)
	{
		_cprintf("Importing %s failed...\n", fname.c_str());
		return NULL;
	}
	else
		_cprintf("Importing %s...\n", fname.c_str());

	// 현재 작업 디렉터리를 절대 경로로 출력한다.
	char cwd[1024] = { 0 };
	_getcwd(cwd, 1024);
	_cprintf("\tCurrent working directory = %s\n", cwd);

	// 모델 검색 디렉터리로 이동하고, 절대 경로로 출력한다.
	std::string model_dir = get_file_path(fname);
	_chdir(model_dir.c_str());
	char msd[1024] = { 0 };
	_getcwd(msd, 1024);
	_cprintf("\tModel directory = %s\n\n", msd);

	// 파일의 이름(경로 및 확장자 제외)을 구하여 메쉬를 생성한다.
	std::string MeshName = get_file_name(fname);
	EgMesh *pMesh = ::create_mesh(MeshName);
	_cprintf("\tMesh name = %s\n", MeshName.c_str());

	// 로딩에 필요한 임시 변수들을 선언한다.
	char tag[1024];
	bool bTexture = false;
	bool bNormal = false;
	std::string GroupName;
	EgMaterial *pCurrMtl = NULL;

	// 파일의 끝까지 한 단어씩 읽으면서 tag 배열에 저장한다.
	while (fscanf_s(fp, "%s", tag, 1024) != EOF)
	{
		// mtllib (material library) 태그라면,
		if (!strcmp(tag, "mtllib"))
		{
			// 재질 파일명(주로 상대 경로)을 구하여
			char mtl_fname[1024];
			fscanf_s(fp, "%s", mtl_fname, 1024);

			// 재질 파일을 읽는다. 
			SK_Import_Obj_Mtl(pMesh, mtl_fname);
		}

		// v (vertex) 태그라면,
		if (!strcmp(tag, "v"))
		{
			// 좌표(x, y, z)를 읽어 정점을 생성하고 메쉬에 추가한다.
			double x, y, z;
			fscanf_s(fp, "%lf%lf%lf", &x, &y, &z);
			EgVertex *pVert = ::create_vertex(x, y, z);
			pMesh->AddVertex(pVert);
		}

		// vt (vertex texture) 태그라면,
		if (!strcmp(tag, "vt"))
		{
			// 텍스처 좌표(u, v)를 읽어 텍셀을 생성하고 메쉬에 추가한다.
			bTexture = true;
			double u, v;
			fscanf_s(fp, "%lf%lf", &u, &v);
			EgTexel *pTexel = ::create_texel(u, v);
			pMesh->AddTexel(pTexel);
		}

		// vn (vertex normal) 태그라면,
		if (!strcmp(tag, "vn"))
		{
			// 법선의 방향(nx, ny, nz)을 읽어 법선을 생성하고 메쉬에 추가한다.
			bNormal = true;
			double nx, ny, nz;
			fscanf_s(fp, "%lf%lf%lf", &nx, &ny, &nz);
			EgNormal *pNormal = ::create_normal(nx, ny, nz);
			pMesh->AddNormal(pNormal);
		}

		// g (group) 태그라면,
		if (!strcmp(tag, "g"))
		{
			// 그룹의 이름을 읽는다.
			char grp_name[256];
			fscanf_s(fp, "%s", grp_name, 256);
			GroupName = std::string(grp_name);
		}

		// usemtl (use material) 태그라면,
		if (!strcmp(tag, "usemtl"))
		{
			// 메쉬의 재질 리스트에서 해당 재질을 찾는다.
			char mtl_name[256];
			fscanf_s(fp, "%s", mtl_name, 256);
			pCurrMtl = pMesh->GetMaterial(mtl_name);
		}

		// f (face) 태그라면,
		if (!strcmp(tag, "f"))
		{
			// 폴리곤을 구성하는 정점, 텍셀, 노말의 인덱스를 저장하는 변수를 선언하고,
			std::vector<int> vIdxs, tIdxs, nIdxs;

			// 한 줄을 읽어서,
			char line[256];
			fgets(line, 256, fp);
			char *pData = line;

			// 한 줄을 모두 읽을 때까지
			while (true)
			{
				// 불필요한 공백과 줄빠꿈을 건너띄고,
				while (*pData == ' ' || *pData == '\n')
					pData++;

				// 한 줄을 모두 읽었다면 반복문을 빠져 나온다.
				if (!strcmp(pData, ""))
					break;

				// 인덱스 정보를 buffer로 읽은 후,
				char buffer[256];	// buffer[] = "vidx/tidx/nidx" 
				sscanf_s(pData, "%s", buffer, 256);
				pData += strlen(buffer);

				// 인덱스 정보를 저장할 변수를 선언하고
				char sep;
				int vidx, tidx, nidx;

				// 정점, 텍셀, 노말 인덱스가 모두 있는 경우 (f 1/1/1/ 2/2/2 3/3/3 ...)
				if (bTexture && bNormal)
				{
					// 모든 인덱스를 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d%c%d%c%d", &vidx, &sep, 1, &tidx, &sep, 1, &nidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					tidx = (tidx < 0) ? -tidx : tidx;
					nidx = (nidx < 0) ? -nidx : nidx;
					vIdxs.push_back(vidx - 1);
					tIdxs.push_back(tidx - 1);
					nIdxs.push_back(nidx - 1);
					continue;
				}

				// 정점과 텍셀 인덱스만 있는 경우 (f 1/1 2/2 3/3 ...)
				if (bTexture && !bNormal)
				{
					// 정점과 텍셀 인덱스만 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d%c%d", &vidx, &sep, 1, &tidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					tidx = (tidx < 0) ? -tidx : tidx;
					vIdxs.push_back(vidx - 1);
					tIdxs.push_back(tidx - 1);
					continue;
				}

				// 정점과 노말 인덱스만 있는 경우 (f 1//1 2//2 3//3 ...)
				if (!bTexture && bNormal)
				{
					// 정점과 노말 인덱스만 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d%c%c%d", &vidx, &sep, 1, &sep, 1, &nidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					nidx = (nidx < 0) ? -nidx : nidx;
					vIdxs.push_back(vidx - 1);
					nIdxs.push_back(nidx - 1);
					continue;
				}

				// 정점의 인덱스만 있는 경우 (f 1 2 3 ...)
				if (!bTexture && !bNormal)
				{
					// 정점의 인덱스만 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d", &vidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					vIdxs.push_back(vidx - 1);
					continue;
				}
			}

			// 폴리곤을 삼각형으로 나누어 메쉬에 추가한다.
			int PolygonSize = (int)vIdxs.size(); // 삼각형(PolygonSize = 3), 사각형(PolygonSize = 4), ...
			for (int i = 0; i < PolygonSize - 2; ++i)
			{
				// 삼각형의 정점을 구하고,
				EgVertex *V[3] = { 0 };
				V[0] = pMesh->GetVert(vIdxs[0]);
				V[1] = pMesh->GetVert(vIdxs[i + 1]);
				V[2] = pMesh->GetVert(vIdxs[i + 2]);

				// 각 정점의 텍셀 정보를 구하고,
				EgTexel *T[3] = { 0 };
				if (bTexture)
				{
					T[0] = pMesh->GetTexel(tIdxs[0]);
					T[1] = pMesh->GetTexel(tIdxs[i + 1]);
					T[2] = pMesh->GetTexel(tIdxs[i + 2]);
				}

				// 각 정점의 법선 정보를 구하고,
				EgNormal *N[3] = { 0 };
				if (bNormal)
				{
					N[0] = pMesh->GetNormal(nIdxs[0]);
					N[1] = pMesh->GetNormal(nIdxs[i + 1]);
					N[2] = pMesh->GetNormal(nIdxs[i + 2]);
				}
				else // 법선 정보가 없다면 삼각형 법선으로 대체한다.
				{
					EgVec3 n = ((V[1]->m_Pos - V[0]->m_Pos) ^ (V[2]->m_Pos - V[0]->m_Pos)).Normalize();
					N[0] = N[1] = N[2] = ::create_normal(n[0], n[1], n[2]);
					pMesh->AddNormal(N[0]);
				}

				// 삼각형을 생성하여 메쉬에 추가한다.
				EgFace *f = new EgFace(V[0], V[1], V[2], T[0], T[1], T[2], N[0], N[1], N[2], pCurrMtl, GroupName);
				pMesh->AddFace(f);
			}
		}
	}

	// 파일을 닫는다.
	fclose(fp);

	// 하프에지에 반대편 에지의 정보를 갱신한다.
	pMesh->InitEdgeMate();

	// 메쉬의 경계 상자를 계산한다.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal(SMOOTH);

	// 텍스처 유무에 따라 쉐이딩 타입을 설정한다.
	if (bTexture)
		pMesh->m_ShadeType = SHADE_BY_TEXTURE;
	_cprintf("Importing OBJ mesh complete...\n\n");

	// 모델 검색 디렉터리에서 원래 작업 디렉터리로 복귀한다.
	_chdir(cwd);

	// 임포트된 메쉬의 포인터를 반환한다.
	return pMesh;
}

EgMesh *SK_Import_Ply(CString FileName)
{
	// 파일을 오픈한다.
	FILE *fp;
	fopen_s(&fp, FileName, "r");
	if (!fp)
	{
		_cprintf("Ply file open failed...\n");
		return NULL;
	}

	// 메쉬를 생성한다.
	EgMesh *pMesh = ::create_mesh(std::string(FileName));

	// 파일 헤더를 읽는다.
	int NumVert = 0, NumFace = 0;
	char buffer[1024];
	while (fscanf_s(fp, "%s", buffer, 1024) != EOF)
	{
		// element 태그라면,
		if (!strcmp(buffer, "element"))
		{
			// 다음 태그를 읽는다.
			fscanf_s(fp, "%s", buffer, 1024);

			// vertex 태그라면,
			if (!strcmp(buffer, "vertex"))
				fscanf_s(fp, "%d", &NumVert);

			// face 태그라면,
			if (!strcmp(buffer, "face"))
				fscanf_s(fp, "%d", &NumFace);
		}
		// end_header 태그라면
		if (!strcmp(buffer, "end_header"))
			break;
	}

	// 정점의 좌표를 읽는다.
	for (int i = 0; i < NumVert; ++i)
	{
		double x, y, z;
		fscanf_s(fp, "%lf%lf%lf", &x, &y, &z);
		EgVertex *v = ::create_vertex(x, y, z);
		pMesh->AddVertex(v);
	}

	// 삼각형의 인덱스를 읽는다.
	for (int i = 0; i < NumFace; ++i)
	{
		int tmp, idx0, idx1, idx2;
		fscanf_s(fp, "%d%d%d%d", &tmp, &idx0, &idx1, &idx2);
		EgVertex *v0 = pMesh->GetVert(idx0);
		EgVertex *v1 = pMesh->GetVert(idx1);
		EgVertex *v2 = pMesh->GetVert(idx2);
		EgFace *f = ::create_face(v0, v1, v2, NULL, "");
		pMesh->AddFace(f);
	}

	// 파일을 닫는다.
	fclose(fp);

	// 하프에지에 반대편 에지의 정보를 갱신한다.
	pMesh->InitEdgeMate();

	// 메쉬의 경계 상자를 계산한다.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal(SMOOTH);

	// 텍스처 유무에 따라 쉐이딩 타입을 설정한다.
	pMesh->m_ShadeType = SHADE_BY_LIGHT;
	_cprintf("Importing PLY mesh complete...\n\n");

	// 메쉬를 리턴한다.
	return pMesh;
}

EgMesh *SK_Import_Iv(CString FileName)
{
	// 파일을 오픈한다.
	FILE *fp;
	fopen_s(&fp, FileName, "r");
	if (!fp)
	{
		_cprintf("Iv file open failed...\n");
		return NULL;
	}

	// 메쉬를 생성한다.
	EgMesh *pMesh = ::create_mesh(std::string(FileName));

	// 정점 리스트의 시작 위치로 이동한다.
	char line_buffer[1024];
	while (true)
	{
		fgets(line_buffer, 1024, fp);
		CString str(line_buffer);
		str.TrimLeft();
		if (str == "[\n")
			break;
	}

	// 정점을 생성하여 메쉬에 추가한다.
	while (true)
	{
		double x, y, z;
		fgets(line_buffer, 1024, fp);
		CString str(line_buffer);
		str.TrimLeft();
		if (str == "]\n")
			break;

		sscanf_s(line_buffer, "%lf%lf%lf", &x, &y, &z);
		EgVertex *v = ::create_vertex(x, y, z);
		pMesh->AddVertex(v);
	}

	// 삼각형 리스트의 시작 위치로 이동한다.
	while (true)
	{
		fgets(line_buffer, 1024, fp);
		CString str(line_buffer);
		str.TrimLeft();
		if (str == "[\n")
			break;
	}

	// 삼각형을 생성하여 메쉬에 추가한다.
	while (true)
	{
		fgets(line_buffer, 1024, fp);
		CString str(line_buffer);
		str.TrimLeft();
		if (str == "]\n")
			break;

		int vidx0, vidx1, vidx2;
		fgets(line_buffer, 1024, fp);
		sscanf_s(line_buffer, "%d,%d,%d", &vidx0, &vidx1, &vidx2);

		EgVertex *v0 = pMesh->GetVert(vidx0);
		EgVertex *v1 = pMesh->GetVert(vidx1);
		EgVertex *v2 = pMesh->GetVert(vidx2);
		EgFace *f = ::create_face(v0, v1, v2, NULL, "");
		pMesh->AddFace(f);
	}

	// 파일을 닫는다.
	fclose(fp);

	// 하프에지에 반대편 에지의 정보를 갱신한다.
	pMesh->InitEdgeMate();

	// 메쉬의 경계 상자를 계산한다.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal(SMOOTH);

	// 텍스처 유무에 따라 쉐이딩 타입을 설정한다.
	pMesh->m_ShadeType = SHADE_BY_LIGHT;
	_cprintf("Importing Iv mesh complete...\n\n");

	// 메쉬를 리턴한다.
	return pMesh;
}

EgMesh *SK_Import_Msh(CString FileName)
{
	return nullptr;
}

EgMesh *SK_Import_Vrml97(CString FileName)
{
	return nullptr;
}

EgMesh * SK_Import_Wrb(CString FileName)
{
	return nullptr;
}

/*!
*	\brief	입력 위치와 가장 가까운 정점을 찾는다.
*	\note	마지막 수정일: 2020-07-15
*
*	\param	pMesh[in]	대상 메쉬
*	\param	p[in]		입력 위치
*
*	\return	입력 위치와 가장 가까운 정점의 포인터를 반환한다.
*/
EgVertex *FindClosestVert(EgMesh *pMesh, EgPos p)
{
	EgVertex *pRet = NULL;
	double minDist = INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		double d = dist_sq(p, v->m_Pos);
		if (d < minDist)
		{
			minDist = d;
			pRet = v;
		}
	}
	return pRet;
}

/*!
*	\brief	메쉬 위의 두 점 사이의 최단 경로와 거리를 계산한다.
*	\note	마지막 수정일: 2020-05-29 (윤승현)
*
*	\param	P[in]		최단 경로의 시작점
*	\param	Q[in]		최단 경로의 끝점
*	\param	Path[out]	최단 경로를 구성하는 에지 교차점들이 저장된다.
*
*	\return	최단 경로의 길이를 반환한다.
*/
double SK_Shortest_Distance(EgFacePt *P, EgFacePt *Q, std::vector<EgPos> &Path)
{
	// 시작점에서의 측지 거리장을 생성한다.
	std::vector<double> Distances;
	EgMesh *pMesh = P->m_pFace->GetMesh();
	::get_geodesic_dist_field(pMesh, P, Q, Distances);

	// 최단 경로와 길이를 계산하여 반환한다.
	Path.clear();
	std::vector<std::pair<EgEdge *, float>> CutInfo;
	return ::get_shortest_path(P, Q, Distances, Path, CutInfo);
}

/*!
*	\brief	메쉬 위의 두 정점 사이의 최단 경로와 거리를 계산한다.
*	\note	마지막 수정일: 2020-06-18 (윤승현)
*
*	\param	P[in]		최단 경로의 시작 정점
*	\param	Q[in]		최단 경로의 끝 정점
*	\param	Path[out]	최단 경로를 구성하는 에지 교차점들이 저장된다.
*
*	\return	최단 경로의 길이를 반환한다.
*/
double SK_Shortest_Distance(EgVertex *P, EgVertex *Q, std::vector<EgPos> &Path)
{
	// 시작점에서의 측지 거리장을 생성한다.
	std::vector<double> Distances;
	::get_geodesic_dist_field(P->m_pMesh, P, Q, Distances);

	// 최단 경로와 길이를 계산하여 반환한다.
	Path.clear();
	std::vector<std::pair<EgEdge *, float>> CutInfo;
	return ::get_shortest_path(P, Q, Distances, Path, CutInfo);
}

/*!
*	\brief	메쉬 위의 두 점 사이의 평면에 의한 절단 경로와 거리를 구하는 함수
*	\note	마지막 수정일: 2020-05-30 (윤승현)
*
*	\param	P[in]			경로의 시작점
*	\param	Q[in]			경로의 끝점
*	\param	Pln[in]			절단 평면에 대한 포인터
*	\param	Path[out]		절단 경로를 구성하는 에지 교차점들이 저장된다.
*	\param	bConvex[in]		절단 경로의 convexhull 사용 여부
*
*	\return	평면 절단에 의한 경로의 거리를 반환한다(경로에 문제가 있다면 -1.0을 반환한다).
*/
double SK_Planar_Distance(EgFacePt *P, EgFacePt *Q, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex)
{
	// 평면 절단 경로와 길이를 구한다.
	Path.clear();
	std::vector<std::pair<EgEdge *, float>> CutInfo;
	double Dist = ::get_planar_path(P, Q, Pln, Path, CutInfo);

	// 경로에 문제가 있다면 -1.0을 반환한다.
	if (Dist == -1.0)
	{
		return -1.0;
	}

	// Convex 경로를 사용한다면
	if (bConvex)
	{
		// Convex 경로를 구하고 거리를 계산한다.
		std::vector<EgPos> CvxPts;
		Dist = SK_ConvexHull_Open(Pln, Path, CvxPts);
		Path = CvxPts;
	}

	// 거리를 반환한다.
	return Dist;
}

/*!
*	\brief	메쉬 위의 두 정점 사이의 평면에 의한 절단 경로와 거리를 구하는 함수
*	\note	마지막 수정일: 2020-06-18 (윤승현)
*
*	\param	P[in]			경로의 시작 정점
*	\param	Q[in]			경로의 끝 정점
*	\param	Pln[in]			절단 평면에 대한 포인터
*	\param	Path[out]		절단 경로를 구성하는 에지 교차점들이 저장된다.
*	\param	bConvex[in]		절단 경로의 convexhull 사용 여부
*
*	\return	평면 절단에 의한 경로의 거리를 반환한다(경로에 문제가 있다면 -1.0을 반환한다).
*/
double SK_Planar_Distance(EgVertex *P, EgVertex *Q, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex)
{
	EgFacePt PF;
	PF.m_pFace = P->m_pEdges[0]->m_pFace;
	if (P == PF.m_pFace->GetVertex(0))
		PF.m_Coords.Set(1.0, 0.0, 0.0);
	if (P == PF.m_pFace->GetVertex(1))
		PF.m_Coords.Set(0.0, 1.0, 0.0);
	if (P == PF.m_pFace->GetVertex(2))
		PF.m_Coords.Set(0.0, 0.0, 1.0);

	EgFacePt QF;
	QF.m_pFace = Q->m_pEdges[0]->m_pFace;
	if (Q == QF.m_pFace->GetVertex(0))
		QF.m_Coords.Set(1.0, 0.0, 0.0);
	if (Q == QF.m_pFace->GetVertex(1))
		QF.m_Coords.Set(0.0, 1.0, 0.0);
	if (Q == QF.m_pFace->GetVertex(2))
		QF.m_Coords.Set(0.0, 0.0, 1.0);

	// 평면 절단 경로와 길이를 구한다.
	Path.clear();
	std::vector<std::pair<EgEdge *, float>> CutInfo;
	double Dist = ::get_planar_path(&PF, &QF, Pln, Path, CutInfo);

	// 경로에 문제가 있다면 -1.0을 반환한다.
	if (Dist == -1.0)
	{
		return -1.0;
	}

	// Convex 경로를 사용한다면
	if (bConvex)
	{
		// Convex 경로를 구하고 거리를 계산한다.
		std::vector<EgPos> CvxPts;
		Dist = SK_ConvexHull_Open(Pln, Path, CvxPts);		
		Path = CvxPts;
	}

	// 거리를 반환한다.
	return Dist;
}

/*!
*	\brief	메쉬 위의 두 정점 사이의 평면에 의한 절단 경로와 거리를 구하는 함수
*	\note	마지막 수정일: 2020-10-14 (윤승현)
*
*	\param	P[in]			경로의 시작 정점
*	\param	Q[in]			경로의 끝 정점
*	\param	Path[out]		절단 경로를 구성하는 에지 교차점들이 저장된다.
*	\param	bConvex[in]		절단 경로의 convexhull 사용 여부
*
*	\return	평면 절단에 의한 경로의 거리를 반환한다(경로에 문제가 있다면 -1.0을 반환한다).
*/
double SK_Planar_Distance(EgVertex *P, EgVertex *Q, std::vector<EgPos> &Path, bool bConvex)
{
	// 두 점 P와 Q를 지나는 최단 경로를 구한다.
	std::vector<EgPos> sPath;
	if (SK_Shortest_Distance(P, Q, sPath) == -1.0)
	{
		printf("Error in SK_Planar_Distance...\n");
		return -1.0;
	}

	// 최단 경로를 피팅하는 평면의 방정식을 구한다.
	int m = NUM(sPath);
	EgMat A(m, 3);
	EgVec x(3), b(m);
	for (int i = 0; i < m; ++i)
	{
		A.SetRowVec(i, EgVec(3, sPath[i][0], sPath[i][1], sPath[i][2]));
		b[i] = -1.0;
	}
	::lin_sys_sv(A, x, b);

	EgVec3 z_axis(x[0], x[1], x[2]);
	EgVec3 x_axis = (Q->m_Pos - P->m_Pos).Normalize();
	EgVec3 y_axis = (z_axis ^ x_axis).Normalize();
	z_axis = x_axis ^ y_axis;

	EgPlane CutPln1(z_axis, P->m_Pos);
	EgPlane CutPln2(-z_axis, P->m_Pos);
	double f = CutPln1.Eval(Q->m_Pos);
	printf("f = %lf\n", f);

	std::vector<EgPos> Path1, Path2;
	double d1 = SK_Planar_Distance(P, Q, &CutPln1, Path1, false);
	double d2 = SK_Planar_Distance(P, Q, &CutPln2, Path2, false);
	if (d1 == -1.0 && d2 == -1.0)
	{
		printf("Error in SK_Planar_Distance...\n");
		return -1.0;
	}
	else if (d1 == -1.0 && d2 != -1.0)
		return SK_Planar_Distance(P, Q, &CutPln2, Path, bConvex);
	else if (d1 != -1.0 && d2 == -1.0)
		return SK_Planar_Distance(P, Q, &CutPln1, Path, bConvex);
	else if (d1 < d2)
		return SK_Planar_Distance(P, Q, &CutPln1, Path, bConvex);
	else if (d2 < d1)
		return SK_Planar_Distance(P, Q, &CutPln2, Path, bConvex);

	printf("Error in SK_Planar_Distance...\n");
	return -1.0;
}

/*!
*	\brief	메쉬 위의 한 점을 지나는 둘레의 길이를 구하는 함수
*	\note	마지막 수정일: 2020-05-26 (윤승현)
*
*	\param	P[in]			삼각형 내부의 한 점
*	\param	Pln[in]			절단평면에 대한 포인터
*	\param	Path[out]		경로의 에지 교차점들이 저장된다.
*	\param	bConvex[in]		둘레 경로의 convexhull 사용 여부
*
*	\return	둘레의 길이를 반환한다.
*/
double SK_Planar_Perimeter(EgFacePt *P, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex)
{
	// 둘레의 길이를 계산한다.
	std::vector<std::pair<EgEdge *, float>> CutInfo;
	double Dist = ::get_planar_path_closed(P, Pln, Path, CutInfo);

	// 경로에 문제가 있다면 -1.0을 반환한다.
	if (Dist == -1.0)
		return -1.0;

	// Convex 경로를 사용한다면
	if (bConvex)
	{
		// Convex 경로를 구하고 길이를 계산하여
		std::vector<EgPos> CvxPts;
		Dist = SK_ConvexHull(Pln, Path, CvxPts);
		Path = CvxPts;
	}

	// 둘레의 길이를 반환한다.
	return Dist;
}

/*!
*	\brief	메쉬 정점을 지나는 둘레의 길이를 구하는 함수
*	\note	마지막 수정일: 2020-06-22 (윤승현)
*
*	\param	P[in]			메쉬 정점
*	\param	Pln[in]			절단평면에 대한 포인터
*	\param	CutPts[out]		경로의 에지 교차점들이 저장된다.
*	\param	bConvex[in]		둘레 경로의 convexhull 사용 여부
*
*	\return	둘레의 길이를 반환한다.
*/
double SK_Planar_Perimeter(EgVertex *P, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex)
{
	EgFacePt PF;
	PF.m_pFace = P->m_pEdges[0]->m_pFace;
	if (P == PF.m_pFace->GetVertex(0))
		PF.m_Coords.Set(0.9, 0.05, 0.05);
	if (P == PF.m_pFace->GetVertex(1))
		PF.m_Coords.Set(0.05, 0.9, 0.05);
	if (P == PF.m_pFace->GetVertex(2))
		PF.m_Coords.Set(0.05, 0.05, 0.9);
	EgPos p = PF.Pos();
	Pln->m_Coords[3] -= Pln->Eval(p);

	// 둘레의 길이를 계산한다.
	std::vector<std::pair<EgEdge *, float>> CutInfo;
	double Dist = ::get_planar_path_closed(&PF, Pln, Path, CutInfo);

	// 경로에 문제가 있다면 -1.0을 반환한다.
	if (Dist == -1.0)
		return -1.0;

	// Convex 경로를 사용한다면
	if (bConvex)
	{
		// Convex 경로를 구하고 길이를 계산하여
		std::vector<EgPos> CvxPts;
		Dist = SK_ConvexHull(Pln, Path, CvxPts);
		Path = CvxPts;
	}

	// 둘레의 길이를 반환한다.
	return Dist;
}

/*!
*	\brief	선분 pq 위쪽 점들 중에서 convexhull을 구성하는 점을 추가한다.
*	\note	마지막 수정일: 2020-05-29
*
*	\param	p[in]			선분의 시작점
*	\param	q[in]			선분의 끝점
*	\param	Pts[in]			전체 점 배열
*	\param	CvxPts[out]		현재까지 찾은 Convexhull을 구성하는 점들의 배열
*/
void AddUpperHullPt(EgPos p, EgPos q, std::vector<EgPos> &Pts, std::vector<EgPos> &CvxPts)
{
	EgPos r;
	double m = (q[1] - p[1]) / (q[0] - p[0]);
	double max_d = -INFINITY;
	bool bFound = false;
	for (EgPos pt : Pts)
	{
		double f = m * (pt[0] - p[0]) + p[1] - pt[1];
		if (EQ_ZERO(f, 1.0e-6))			
			continue;
		if (f > 0.0)
			continue;

		double d = ABS(m * (pt[0] - p[0]) + p[1] - pt[1]) / SQRT(SQR(m) + 1.0);
		if (d > max_d)
		{
			max_d = d;
			r = pt;
			bFound = true;
		}
	}
	if (bFound == false)
		return;
	CvxPts.push_back(r);
	AddUpperHullPt(p, r, Pts, CvxPts);
	AddUpperHullPt(r, q, Pts, CvxPts);
}

/*!
*	\brief	선분 pq 아래쪽 점들 중에서 convexhull을 구성하는 점을 추가한다.
*	\note	마지막 수정일: 2020-10-07
*
*	\param	p[in]			선분의 시작점
*	\param	q[in]			선분의 끝점
*	\param	Pts[in]			전체 점 배열
*	\param	CvxPts[out]		현재까지 찾은 Convexhull을 구성하는 점들의 배열
*/
void AddLowerHullPt(EgPos p, EgPos q, std::vector<EgPos> &Pts, std::vector<EgPos> &CvxPts)
{
	EgPos r;
	bool bFound = false;
	double m = (q[1] - p[1]) / (q[0] - p[0]);
	double max_d = -INFINITY;

	EgVec2 u = ortho(EgVec2(q[0] - p[0], q[1] - p[1]), true);
	for (EgPos pt : Pts)
	{
		EgVec2 v(pt[0] - p[0], pt[1] - p[1]);
		double theta = angle(u, v);
		if (EQ(theta, 90.0, 1.0e-6) || theta < 90.0)
			continue;

		double d = ABS(m * (pt[0] - p[0]) + p[1] - pt[1]) / SQRT(SQR(m) + 1);
		if (d > max_d)
		{
			max_d = d;
			r = pt;
			bFound = true;
		}
	}
	if (bFound == false)
		return;

	CvxPts.push_back(r);
	AddLowerHullPt(p, r, Pts, CvxPts);
	AddLowerHullPt(r, q, Pts, CvxPts);
}

/*!
*	\brief	절단 평면 위에 놓인 교차점들의 convexhull을 구한다.
*	\note	마지막 수정일: 2020-05-28 (윤승현)
*
*	\param	Pln[in]			절단 평면에 대한 포인터
*	\param	Pts[in]			절단 평면 위의 교차점 배열
*	\param	CvxPts[out]		Convexhull을 구성하는 점들의 배열
*
*	\return Convexhull의 둘레 길이를 반환한다.
*/
double SK_ConvexHull(EgPlane *Pln, std::vector<EgPos> &Pts, std::vector<EgPos> &CvxPts)
{
	// 법선을 z축으로 갖는 절단 평면의 로컬 좌표계를 구한다.
	EgVec3 z_axis = Pln->N(true);
	EgQuat q;
	q.SetFromArcBall(EgVec3(0.0, 0.0, 1.0), z_axis);
	EgVec3 o;
	for (EgPos &p : Pts)
		o = o + cast_vec3(p);
	o = o / (double)(Pts.size());

	// 교차 점들의 로컬 좌표를 구한다.
	EgTrans X(o, q);
	EgTrans invX = inv(X);
	std::vector<EgPos> LocalPts;
	for (EgPos &p : Pts)
		LocalPts.push_back(invX * p);

	// 로컬 좌표계에서 convexhull을 구성하는 점들을 구한다.
	EgPos MinPt(INFINITY, 0.0, 0.0), MaxPt(-INFINITY, 0.0, 0.0);
	for (EgPos &p : LocalPts)
	{
		if (p[0] < MinPt[0])
			MinPt = p;
		if (p[0] > MaxPt[0])
			MaxPt = p;
	}
	CvxPts.push_back(MinPt);
	CvxPts.push_back(MaxPt);
	AddUpperHullPt(MinPt, MaxPt, LocalPts, CvxPts);
	AddLowerHullPt(MinPt, MaxPt, LocalPts, CvxPts);

	// Convexhull 정점을 반시계 방향으로 정렬한다.
	for (EgPos &p : CvxPts)
		o = o + cast_vec3(p);
	o = o / (double)(CvxPts.size());
	std::vector<double> Angles;
	for (EgPos &p : CvxPts)
	{
		EgVec3 v = (p - cast_pos(o)).Normalize();
		double theta = angle(EgVec3(1.0, 0.0, 0.0), v, EgVec3(0.0, 0.0, 1.0));
		Angles.push_back(theta);
	}
	for (int i = 0; i < NUM(Angles) - 1; ++i)
	{
		for (int j = i + 1; j < NUM(Angles); ++j)
		{
			if (Angles[i] < Angles[j])
			{
				SWAP(double, Angles[i], Angles[j]);
				SWAP(EgPos, CvxPts[i], CvxPts[j]);
			}
		}
	}

	// 시작점을 추가하여 폐경로를 만든다.
	CvxPts.push_back(CvxPts[0]);

	// 로컬 좌표를 전역 좌표로 변환한다.
	for (EgPos &p : CvxPts)
		p = X * p;

	// 둘레의 길이를 구하여 반환한다.
	double Dist = 0.0;
	for (int i = 0; i < NUM(CvxPts) - 1; ++i)
		Dist += dist(CvxPts[i], CvxPts[i + 1]);
	return Dist;
}

/*!
*	\brief	절단 평면 위에 놓인 교차점들의 열린 convexhull을 구한다.
*	\note	마지막 수정일: 2020-10-14
*
*	\param	Pln[in]			절단 평면에 대한 포인터
*	\param	Pts[in]			절단 평면 위의 교차점 배열
*	\param	CvxPts[out]		Convexhull을 구성하는 점들의 배열
*
*	\return	Convexhull의 길이를 반환한다.
*/
double SK_ConvexHull_Open(EgPlane *Pln, std::vector<EgPos> &Pts, std::vector<EgPos> &CvxPts)
{
	// 법선을 z축으로 갖는 절단 평면의 로컬 좌표계를 구한다.
	EgVec3 z_axis = Pln->N(true);
	EgVec3 x_axis = (Pts[NUM(Pts) - 1] - Pts[0]).Normalize();
	EgQuat q;
	q.SetFromFrameZX(z_axis, x_axis);

	EgVec3 o;
	for (EgPos &p : Pts)
		o = o + cast_vec3(p);
	o = o / (double)(Pts.size());

	// 교차 점들의 로컬 좌표를 구한다.
	EgTrans X(o, q);
	EgTrans invX = inv(X);
	std::vector<EgPos> LocalPts;
	for (EgPos &p : Pts)
		LocalPts.push_back(invX * p);

	// 로컬 좌표계에서 convexhull을 구성하는 점들을 구한다.
	EgPos p0 = LocalPts[0];
	EgPos q0 = LocalPts[NUM(LocalPts) - 1];
	std::vector<EgPos> tmpCvxPts;
	tmpCvxPts.push_back(p0);
	tmpCvxPts.push_back(q0);
	AddLowerHullPt(p0, q0, LocalPts, tmpCvxPts);

	// tmpCvxPts는 LocalPts의 부분집합이므로, tmpCvxPts의 정점을 LocalPts순으로 정렬한다.
	for (EgPos p : LocalPts)
	{
		for (EgPos q : tmpCvxPts)
		{
			if (p == q)
			{
				CvxPts.push_back(p);
				break;
			}
		}
	}

	// 로컬 좌표를 전역 좌표로 변환한다.
	for (EgPos &p : CvxPts)
		p = X * p;

	// 둘레의 길이를 구하여 반환한다.
	double Dist = 0.0;
	int NumPts = NUM(CvxPts);
	for (int i = 0; i < NumPts - 1; ++i)
		Dist += dist(CvxPts[i], CvxPts[i + 1]);
	return Dist;
}

/*!
*	\brief	주어진 정점 주변의 영역을 검사하여 극 정점(extream vertex)을 찾는다.
*	\note	마지막 수정일: 2020-06-23
*
*	\param	pSeedVert[in]	탐색을 시작할 정점에 대한 포인터
*	\param	Radius[in]		탐색할 영역의 반지름
*	\param	PtType[in]		탐색할 극 정점의 형태(x축 최대점(0)/최소점(1), y축 최대점(2)/최소점(3), z축 최대점(4)/최소점(5)
*	\param	Depth[in]		탐색을 반복할 회수
*
*	\return	극 정점의 포인터를 반환한다.
*/
EgVertex *SK_Extreme_Vertex(EgVertex *pSeedVert, double Radius, int PtType, int Depth)
{
	// 종료 조건
	if (Depth == 0)
		return pSeedVert;

	// 측지 거리장을 이용하여 탐색 영역을 구한다.
	std::vector<double> Distances;
	EgMesh *pMesh = pSeedVert->GetMesh();
	::get_geodesic_dist_field(pMesh, pSeedVert, Distances, Radius);

	// 탐색 영역의 정점의 좌표를 조사하여 극점을 구한다.
	EgVertex *pVert = NULL;
	double value = (PtType % 2 == 0) ? (-INFINITY) : (INFINITY);
	for (int i = 0; i < NUM(Distances); ++i)
	{
		// 탐색 영역 밖의 정점이면 스킵한다.
		if (Distances[i] == INFINITY)
			continue;

		EgPos p = pMesh->m_pVerts[i]->m_Pos;
		if (PtType % 2 == 0)	// 최대값
		{
			if (p[PtType / 2] > value)
			{
				value = p[PtType / 2];
				pVert = pMesh->m_pVerts[i];
			}
		}
		else // 최소값
		{
			if (p[PtType / 2] < value)
			{
				value = p[PtType / 2];
				pVert = pMesh->m_pVerts[i];
			}
		}
	}

	// 재귀 호출을 수행한다.
	return SK_Extreme_Vertex(pVert, Radius, PtType, Depth - 1);
}

/*!
*	\brief	주어진 정점 주변의 영역을 검사하여 타원점(elliptic point)을 찾는다.
*	\note	마지막 수정일: 2020-08-02
*
*	\param	pSeedVert[in]	탐색을 시작할 정점에 대한 포인터
*	\param	Radius[in]		탐색할 영역의 반지름
*
*	\return	구해진 정점의 포인터를 반환한다
*/
EgVertex *SK_Elliptic_Vertex(EgVertex *pSeedVert, double Radius)
{
	// 측지 거리장을 이용하여 탐색 영역을 구한다.
	EgMesh *pMesh = pSeedVert->GetMesh();
	EgMesh *pSubMesh = ::create_mesh_from_geodesic_dist(pMesh, pSeedVert, 2 * Radius);

	std::vector<double> Distances;
	EgVertex *pVert = FindClosestVert(pSubMesh, pSeedVert->m_Pos);
	::get_geodesic_dist_field(pSubMesh, pVert, Distances, Radius);

	// 탐색 영역에 포함된 정점에 대하여 최소 면적을 갖는 정점을 선택하여 반환한다.
	EgPos pos;
	double minArea = INFINITY;
	for (EgVertex *v : pSubMesh->m_pVerts)
	{
		if (Distances[v->m_Idx] == INFINITY)
			continue;

		double area = ::get_local_area(v, Radius);
		if (area < minArea)
		{
			minArea = area;
			pos = v->m_Pos;
		}
	}
	delete pSubMesh;
	return FindClosestVert(pMesh, pos);
}

/*!
*	\brief	주어진 정점 주변의 영역을 검사하여 평탄점(planar point)을 찾는다.
*	\note	마지막 수정일: 2020-08-02
*
*	\param	pSeedVert[in]	탐색을 시작할 정점에 대한 포인터
*	\param	Radius[in]		탐색할 영역의 반지름
*
*	\return	구해진 정점의 포인터를 반환한다
*/
EgVertex *SK_Planar_Vertex(EgVertex *pSeedVert, double Radius)
{
	// 측지 거리장을 이용하여 탐색 영역을 구한다.
	std::vector<double> Distances;
	EgMesh *pMesh = pSeedVert->GetMesh();
	::get_geodesic_dist_field(pMesh, pSeedVert, Distances, Radius);

	// 탐색 영역에 포함된 정점에 대하여 평면 면적과 차이가 가장 적은 정점을 선택하여 반환한다.
	EgVertex *theVert = NULL;
	double theArea = M_PI * SQR(Radius);
	double minError = INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (Distances[v->m_Idx] == INFINITY)
			continue;

		double area = ::get_local_area(v, Radius);
		double error = ABS(area - theArea);
		if (error < minError)
		{
			minError = error;
			theVert = v;
		}
	}
	return theVert;
}

/*!
*	\brief	주어진 정점 주변의 영역을 검사하여 쌍곡점(hyperbolic point)을 찾는다.
*	\note	마지막 수정일: 2020-08-02
*
*	\param	pSeedVert[in]	탐색을 시작할 정점에 대한 포인터
*	\param	Radius[in]		탐색할 영역의 반지름
*
*	\return	구해진 정점의 포인터를 반환한다
*/
EgVertex *SK_Hyperbolic_Vertex(EgVertex *pSeedVert, double Radius)
{
	// 측지 거리장을 이용하여 탐색 영역을 구한다.
	EgMesh *pMesh = pSeedVert->GetMesh();
	EgMesh *pSubMesh = ::create_mesh_from_geodesic_dist(pMesh, pSeedVert, 2 * Radius);

	std::vector<double> Distances;
	EgVertex *pVert = FindClosestVert(pSubMesh, pSeedVert->m_Pos);
	::get_geodesic_dist_field(pSubMesh, pVert, Distances, Radius);

	// 탐색 영역에 포함된 정점에 대하여 최대 면적을 갖는 정점을 선택하여 반환한다.
	EgPos pos;
	double maxArea = -INFINITY;
	for (EgVertex *v : pSubMesh->m_pVerts)
	{
		if (Distances[v->m_Idx] == INFINITY)
			continue;

		double area = ::get_local_area(v, Radius);
		if (area > maxArea)
		{
			maxArea = area;
			pos = v->m_Pos;
		}
	}
	delete pSubMesh;
	return FindClosestVert(pMesh, pos);
}

/*!
*	\brief	폴리 라인을 구성하는 정점을 곡률에 따라 정렬한다.
*	\note	폴리 라인에 대한 스무딩을 적용하고 곡률을 계산한다.
*	\note	마지막 수정일: 2020-08-03
*
*	\param	Pts[in]					2D 폴리 라인을 구성하는 정점의 리스트
*	\param	SortedPts[out]			곡률에 따라 정렬된 정점들이 저장된다.
*	\param	bDescend[in]			정렬 방식(내림 차순: true, 오름 차순: false)
*	\param	bSignedCurvature[in]	곡률 부호를 고려하면 true, 아니면 false
*	\param	Up[in]					폴리 라인이 놓은 평면의 법선 벡터(곡률 부호를 고려할 경우 사용됨)
*/
void SK_Sort_By_Curvature(std::vector<EgPos> Pts, std::vector<EgPos> &SortedPts, bool bDescend, bool bSignedCurvature, EgVec3 Up)
{
	// 입력 정점을 스무딩 한다.
	int NumPt = NUM(Pts);
	std::vector<EgPos> SmoothPts = Pts;
	for (int i = 1; i < NumPt - 1; ++i)
		SmoothPts[i] = lerp(Pts[i - 1], Pts[i + 1], 0.5);

	// 스무딩된 폴리 라인의 각 정점의 곡률을 구한다.
	std::vector<double> Ks;
	::get_curvature(SmoothPts, Ks, bSignedCurvature, Up);

	// 곡률에 따라 입력 정점을 정렬한다.	
	for (int i = 0; i < NumPt - 1; ++i)
	{
		for (int j = i + 1; j < NumPt; ++j)
		{
			if (bDescend) // 큰순->작은순
			{
				if (Ks[i] < Ks[j])
				{
					SWAP(double, Ks[i], Ks[j]);
					SWAP(EgPos, Pts[i], Pts[j]);
				}
			}
			else // 작은순->큰순
			{
				if (Ks[i] > Ks[j])
				{
					SWAP(double, Ks[i], Ks[j]);
					SWAP(EgPos, Pts[i], Pts[j]);
				}
			}
		}
	}

	// 정렬되 정점을 복사한다.
	SortedPts = Pts;
}

/*! 
*	\brief	폴리 라인의 정점에서 음수 곡률을 갖는 정점을 구한다.
*
*	\param	Pts[in]		입력 정점
*	\param	ResPts[out]	음수 곡률을 갖는 정점들이 저장됨
*	\param	bSort[in]	크기순으로 정렬할지 여부
*	\param	Up[in]		정점들이 놓이 평면의 법선 벡터(부호 결정에 사용됨)
*/
void SK_Find_Negative_Curvature(std::vector<EgPos> Pts, std::vector<EgPos> &ResPts, bool bSort, EgVec3 Up)
{
	// 입력 정점을 스무딩 한다.
	int NumPt = NUM(Pts);
	std::vector<EgPos> SmoothPts = Pts;
	for (int i = 1; i < NumPt - 1; ++i)
		SmoothPts[i] = lerp(Pts[i - 1], Pts[i + 1], 0.5);

	// 스무딩된 폴리 라인의 각 정점의 곡률을 구한다.
	std::vector<double> Ks;
	::get_curvature(SmoothPts, Ks, true, Up);

	// 곡률의 크기에 따라 입력 정점을 정렬(음수->양수, 작은순->큰순)한다.	
	if (bSort)
	{
		for (int i = 0; i < NumPt - 1; ++i)
		{
			for (int j = i + 1; j < NumPt; ++j)
			{
				if (Ks[i] > Ks[j])
				{
					SWAP(double, Ks[i], Ks[j]);
					SWAP(EgPos, Pts[i], Pts[j]);
				}
			}
		}
	}

	// 곡률이 음수인 정점만 추출한다.
	ResPts.clear();
	for (int i = 0; i < NumPt; ++i)
		if (Ks[i] < 0.0)
			ResPts.push_back(Pts[i]);
}

/*!
*	\brief	폴리 라인의 정점에서 양수 곡률을 갖는 정점을 구한다.
*
*	\param	Pts[in]		입력 정점
*	\param	ResPts[out]	양수 곡률을 갖는 정점들이 저장됨
*	\param	bSort[in]	크기순으로 정렬할지 여부
*	\param	Up[in]		정점들이 놓이 평면의 법선 벡터(부호 결정에 사용됨)
*/
void SK_Find_Positive_Curvature(std::vector<EgPos> Pts, std::vector<EgPos> &ResPts, bool bSort, EgVec3 Up)
{
	// 입력 정점을 스무딩 한다.
	int NumPt = NUM(Pts);
	std::vector<EgPos> SmoothPts = Pts;
	for (int i = 1; i < NumPt - 1; ++i)
		SmoothPts[i] = lerp(Pts[i - 1], Pts[i + 1], 0.5);

	// 스무딩된 폴리 라인의 각 정점의 곡률을 구한다.
	std::vector<double> Ks;
	::get_curvature(SmoothPts, Ks, true, Up);

	// 곡률의 크기에 따라 입력 정점을 정렬(음수->양수, 작은순->큰순)한다.	
	if (bSort)
	{
		for (int i = 0; i < NumPt - 1; ++i)
		{
			for (int j = i + 1; j < NumPt; ++j)
			{
				if (Ks[i] > Ks[j])
				{
					SWAP(double, Ks[i], Ks[j]);
					SWAP(EgPos, Pts[i], Pts[j]);
				}
			}
		}
	}

	// 곡률이 양수인 정점만 추출한다.
	ResPts.clear();
	for (int i = 0; i < NumPt; ++i)
		if (Ks[i] > 0.0)
			ResPts.push_back(Pts[i]);
}