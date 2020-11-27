#include "..\usr\include\egmesh\egmesh.h"
#include <cstring>
#include <vector>
#include <map>

/*!
*	\brief 정수를 문자열로 변환하여 반환한다.
*
*	\param i 문자열로 변환될 정수
*
*	\return 문자열로 변환된 정수
*/
std::string cast_str(int i)
{
	char buffer[128];
	_itoa_s(i, buffer, 10);

	return std::string(buffer);
}

/*!
*	\brief 실수를 문자열로 변환하여 반환한다.
*
*	\param v 문자열로 변환될 실수
*
*	\return 문자열로 변환된 실수
*/
std::string cast_str(float v)
{
	char buffer[128];
	sprintf_s(buffer, "%f", v);

	return std::string(buffer);
}

/*!
*	\brief 구간에서 맵핑된 색상 값을 계산한다.
*
*	\param val 맵핑할 값
*	\param min 구간의 시작값
*	\param max 구간의 끝값
*	\param Cmin 시작값의 색상
*	\param Cman 끝값의 색상
*
*	\return 맵핑된 색상을 반환한다.
*/
EgVec3 get_color_map(double val, double min, double max, EgVec3 Cmin, EgVec3 Cmax)
{
	if (EQ(min, max, 1.0e-7))
		return Cmin;

	if (val <= min)
		return Cmin;
	if (val >= max)
		return Cmax;

	double t = (val - min) / (max - min);
	return (1.0 - t) * Cmin + t * Cmax;
}

/*!
*	\brief 구간에서 맵핑된 색상 값을 계산한다.
*
*	\param val 맵핑할 값
*	\param min 구간의 시작값
*	\param max 구간의 끝값
*	\param Cmin 시작값의 색상
*	\param Cmin 중간값의 색상
*	\param Cman 끝값의 색상
*
*	\return 맵핑된 색상을 반환한다.
*/
EgVec3 get_color_map(double val, double min, double max, EgVec3 Cmin, EgVec3 Cmid, EgVec3 Cmax)
{
	if (EQ(min, max, 1.0e-7))
		return Cmin;

	if (val <= min)
		return Cmin;
	if (val >= max)
		return Cmax;

	double t = (val - min) / (max - min);
	if (t <= 0.5)
		return Cmin + 2 * t * (Cmid - Cmin);

	return Cmid + 2 * (t - 0.5) * (Cmax - Cmid);
}

/*!
*	\brief 무지개 색상에서 보간된 색을 얻는다.
*
*	\param t 색상 파라미터: 0(빨강) <= t <= 1.0(보라)
*
*	\return 보간된 중간 색상을 반환한다.
*/
EgVec3 get_color_map_from_rainbow(double t)
{
	int X = (int)(6 * t);

	// 0.0 <= t <= 1.0
	t = 6.0 * t - X;

	EgVec3 C0, C1, C;
	switch (X)
	{
	case 0:	// 빨강 ~ 주황
		C0.Set(255.0, 0.0, 0.0);
		C1.Set(255.0, 127.0, 0.0);
		C = (1.0 - t) * C0 + t * C1;
		break;

	case 1:	// 주황 ~ 노랑
		C0.Set(255.0, 127.0, 0.0);
		C1.Set(255.0, 255.0, 0.0);
		C = (1.0 - t) * C0 + t * C1;
		break;

	case 2:	// 노랑 ~ 초록
		C0.Set(255.0, 255.0, 0.0);
		C1.Set(0.0, 255.0, 0.0);
		C = (1.0 - t) * C0 + t * C1;
		break;

	case 3:	// 초록 ~ 파랑
		C0.Set(0.0, 255.0, 0.0);
		C1.Set(0.0, 0.0, 255.0);
		C = (1.0 - t) * C0 + t * C1;
		break;

	case 4:	// 파랑 ~ 남색
		C0.Set(0.0, 0.0, 255.0);
		C1.Set(75.0, 0.0, 130.0);
		C = (1.0 - t) * C0 + t * C1;
		break;

	case 5:	// 남색 ~ 보라
		C0.Set(75.0, 0.0, 130.0);
		C1.Set(148.0, 0.0, 211.0);
		C = (1.0 - t) * C0 + t * C1;
		break;

	case 6:	// 보라
		C.Set(148.0, 0.0, 211.0);
		break;
	}

	// 범위 밖은 검정색을 할당한다.
	if (t < 0.0 || t > 1.0)
		C.Set(0.0, 0.0, 0.0);
	C /= 255.0;

	return C;
}

/*!
*	\brief 임의의 색상을 조합한다.
*
*	\return 임의의 색상을 반환한다.
*/
EgVec3 get_color_random()
{
	double r = rand() / (double)RAND_MAX;
	double g = rand() / (double)RAND_MAX;
	double b = rand() / (double)RAND_MAX;

	return EgVec3(r, g, b);
}

/*!
*	\brief 인덱스에 따라 정해진 색상을 반환한다.
*
*	\idx 색상의 인덱스
*
*	\return 인덱스에 따라 정해진 색상을 반환한다.
*/
EgVec3 get_color_sequence(int idx)
{
	idx = idx % 7;
	switch (idx)
	{
	case 0:
		return EgVec3(0.0, 0.0, 1.0);

	case 1:
		return EgVec3(0.0, 1.0, 0.0);

	case 2:
		return EgVec3(0.0, 1.0, 1.0);

	case 3:
		return EgVec3(1.0, 0.0, 0.0);

	case 4:
		return EgVec3(1.0, 0.0, 1.0);

	case 5:
		return EgVec3(1.0, 1.0, 0.0);

	case 6:
		return EgVec3(1.0, 1.0, 1.0);
	}

	return EgVec3(0.5, 0.5, 0.5);
}

/*!
*	\brief	경계(홀)의 길이를 계산한다.
*
*	\param	BndryVerts 경계(홀)을 구성하는 정점의 리스트
*	\param	bClosed 경계 정점의 폐곡선 구성 여부
*
*	\return 계산된 정점의 길이를 반환한다.
*/
double get_hole_perimeter(std::vector<EgVertex *> &BndryVerts, bool bClosed)
{
	double len = 0.0;
	if (bClosed) // 폐곡선이라면
	{
		int NumVerts = (int)BndryVerts.size();
		for (int i = 0; i < NumVerts; ++i)
		{
			EgVertex *v0 = BndryVerts[i];
			EgVertex *v1 = (i == NumVerts - 1) ? BndryVerts[0] : BndryVerts[i + 1];
			len += ::dist_sq(v0->m_Pos, v1->m_Pos);
		}
	}
	else // 폐곡선이 아니라면
	{
		int NumVerts = (int)BndryVerts.size();
		for (int i = 0; i < NumVerts - 1; ++i)
			len += ::dist_sq(BndryVerts[i]->m_Pos, BndryVerts[i + 1]->m_Pos);
	}

	// 둘레 길이를 반환한다.
	return len;
}

/*!	\brief	메쉬 정점에 함수값을 이용하여 iso-line 선분의 양 끝점을 구하는 함수
*
*	\param	pMesh[in] 대상 메쉬에 대한 포인터
*	\param	Values[in] 메쉬 i번째 정점의 함수 값 Values[i]
*	\param	MinValue[in] 조사할 iso value의 최소값
*	\param	MaxValue[in] 조사할 iso value의 최대값
*	\param	NumIsoValues[in] 추출할 iso value의 개수
*	\param	IsoLines[out] iso-line 선분의 양 끝점이 저장됨
*/
void get_iso_line_pts(EgMesh *pMesh, std::vector<double> &Values, double MinValue, double MaxValue, int NumValue, std::vector<std::vector<EgPos>> &IsoLines)
{
	// 결과를 초기화 한다.
	IsoLines.clear();
	IsoLines.assign(NumValue, std::vector<EgPos>());

	// 각각의 iso-value에 대하여
	for (int i = 0; i < NumValue; ++i)
	{
		// Iso-value의 값을 계산하여
		double IsoValue = MinValue + (double)i / (double)(NumValue - 1) * (MaxValue - MinValue);

		// 각각의 삼각형에 대하여
		for (EgFace *f : pMesh->m_pFaces)
		{
			// 삼각형 세 점의 iso-value를 구하여
			int *vidx = f->GetVertexIdxs();
			double d0 = Values[vidx[0]];
			double d1 = Values[vidx[1]];
			double d2 = Values[vidx[2]];

			// Iso-value가 삼각형과 교차하지 않으면 스킵한다.
			if ((d0 < IsoValue && d1 < IsoValue && d2 < IsoValue) ||
				(d0 > IsoValue && d1 > IsoValue && d2 > IsoValue))
				continue;

			// 삼각형 세 정점의 좌표를 구하고
			EgPos v0 = pMesh->m_pVerts[vidx[0]]->m_Pos;
			EgPos v1 = pMesh->m_pVerts[vidx[1]]->m_Pos;
			EgPos v2 = pMesh->m_pVerts[vidx[2]]->m_Pos;

			// iso value가 v0v1, v0v2 에지를 절단하는 경우
			if (d0 < IsoValue && d1 > IsoValue && d2 > IsoValue)
			{
				double s = (IsoValue - d0) / (d1 - d0);
				double t = (IsoValue - d0) / (d2 - d0);
				IsoLines[i].push_back(v0 + s * (v1 - v0));
				IsoLines[i].push_back(v0 + t * (v2 - v0));
				continue;
			}
			if (d1 < IsoValue && d2 < IsoValue && d0 > IsoValue)
			{
				double s = (IsoValue - d2) / (d0 - d2);
				double t = (IsoValue - d1) / (d0 - d1);
				IsoLines[i].push_back(v2 + s * (v0 - v2));
				IsoLines[i].push_back(v1 + t * (v0 - v1));
				continue;
			}

			// iso value가 v1v0, v1v2 에지를 절단하는 경우
			if (d1 < IsoValue && d2 > IsoValue && d0 > IsoValue)
			{
				double s = (IsoValue - d1) / (d2 - d1);
				double t = (IsoValue - d1) / (d0 - d1);
				IsoLines[i].push_back(v1 + s * (v2 - v1));
				IsoLines[i].push_back(v1 + t * (v0 - v1));
				continue;
			}
			if (d2 < IsoValue && d0 < IsoValue && d1 > IsoValue)
			{
				double s = (IsoValue - d0) / (d1 - d0);
				double t = (IsoValue - d2) / (d1 - d2);
				IsoLines[i].push_back(v0 + s * (v1 - v0));
				IsoLines[i].push_back(v2 + t * (v1 - v2));
				continue;
			}

			// iso value가 v2v0, v2v1 에지를 절단하는 경우
			if (d2 < IsoValue && d0 > IsoValue && d1 > IsoValue)
			{
				double s = (IsoValue - d2) / (d0 - d2);
				double t = (IsoValue - d2) / (d1 - d2);
				IsoLines[i].push_back(v2 + s * (v0 - v2));
				IsoLines[i].push_back(v2 + t * (v1 - v2));
				continue;
			}
			if (d0 < IsoValue && d1 < IsoValue && d2 > IsoValue)
			{
				double s = (IsoValue - d1) / (d2 - d1);
				double t = (IsoValue - d0) / (d2 - d0);
				IsoLines[i].push_back(v1 + s * (v2 - v1));
				IsoLines[i].push_back(v0 + t * (v2 - v0));
				continue;
			}

			if (EQ(IsoValue, d0, 1.0e-7))
			{
				IsoLines[i].push_back(v0);
				IsoLines[i].push_back(v0);
				continue;
			}
			if (EQ(IsoValue, d1, 1.0e-7))
			{
				IsoLines[i].push_back(v1);
				IsoLines[i].push_back(v1);
				continue;
			}
			if (EQ(IsoValue, d2, 1.0e-7))
			{
				IsoLines[i].push_back(v2);
				IsoLines[i].push_back(v2);
				continue;
			}

			printf("Exceptional cases !!!\n");
			printf("IsoValue = %lf, d0 = %lf, d1 = %lf, d2 = %lf\n\n", IsoValue, d0, d1, d2);
		}
	}
}

/*!
*	\brief 그리드에서 중복된 정점을 조사한다.
*
*	\param pGrid[IN/OUT] 정점이 저장된(될) 그리드(10 x 10 x 10이지만 1차원으로 표현)
*	\param v[in] 조사할 정점의 포인터
*	\param Box[in] 정점이 포함된 메쉬의 경계 상자
*	\param bDuplicate[out] 중복된 정점이면 true값이, 새로운 정점이면 false값이 저장된다.
*
*	\return 중복된 정점이면 최초 정점의 포인터를 반환하고, 아니면 새로 정점을 생성하여 포인터를 반환한다.
*/
static EgVertex *find_duplicate_vertex(std::vector<EgVertex *> *pGrid, EgVertex *v, EgBox &Box, bool &bDuplicate)
{
	// 정점의 위치, 경계상자의 코너점, 경계상자의 크기를 구한다.
	double x = v->m_Pos[0];
	double y = v->m_Pos[1];
	double z = v->m_Pos[2];
	double ox = Box.m_MinPt[0];
	double oy = Box.m_MinPt[1];
	double oz = Box.m_MinPt[2];
	double sx = Box.GetWidth();
	double sy = Box.GetHeight();
	double sz = Box.GetDepth();

	// 정점이 포함된(될) 그리드의 인덱스를 구한다.
	int idx[3] = { 9, 9, 9 };
	for (int i = 0; i < 9; ++i)
	{
		double st = ox + i * sx / 9.0;
		double ed = ox + (i + 1) * sx / 9.0;
		if (x >= st && x < ed)
		{
			idx[0] = i;
			break;
		}
	}
	for (int i = 0; i < 9; ++i)
	{
		double st = oy + i * sy / 9.0;
		double ed = oy + (i + 1) * sy / 9.0;
		if (y >= st && y < ed)
		{
			idx[1] = i;
			break;
		}
	}
	for (int i = 0; i < 9; ++i)
	{
		double st = oz + i * sz / 9.0;
		double ed = oz + (i + 1) * sz / 9.0;
		if (z >= st && z < ed)
		{
			idx[2] = i;
			break;
		}
	}

	// 3차원 그리드 인덱스를 1차원으로 매핑하여,
	int gidx = idx[0] * 100 + idx[1] * 10 + idx[2];

	// 해당 그리드에 포함된 기존의 각각의 정점에 대하여
	for (EgVertex *v0 : pGrid[gidx])
	{
		// 정점이 중복되었다면 최초 정점의 포인터를 반환한다.
		//if (v0->m_Pos == v->m_Pos)
		if (dist_sq(v0->m_Pos, v->m_Pos) < 0.00001)
		{
			bDuplicate = true;
			return v0;
		}
	}

	// 새로운 정점이라면 복사하여 새로운 정점을 생성하여 그리드에 추가한다.
	EgVertex *nv = new EgVertex(v->m_Pos[0], v->m_Pos[1], v->m_Pos[2]);
	pGrid[gidx].push_back(nv);

	// 새로 생성한 정점의 포인터를 반환한다.
	bDuplicate = false;
	return nv;
}

/*!
*	\brief 메쉬에서 중복된 정점을 하나로 합친다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*
*	\return 메쉬의 최종 정점의 수를 반환한다.
*/
int delete_duplicate_verts(EgMesh *pMesh)
{
	// 병합된 정점과 삼각형을 저장할 변수를 선언한다.
	std::vector<EgFace *> NewFaces;
	std::vector<EgVertex *> Grid[1000];

	// 각각의 삼각형에 대하여
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 세 정점을 구하여
		EgVertex *v0 = f->GetVertex(0);
		EgVertex *v1 = f->GetVertex(1);
		EgVertex *v2 = f->GetVertex(2);

		// 중복된 정점이라며 기존 정점의 포인터를 구하고, 아니면 새로운 정점을 생성한다.
		bool bDuplicate;
		v0 = find_duplicate_vertex(Grid, v0, pMesh->m_BndBox, bDuplicate);
		v1 = find_duplicate_vertex(Grid, v1, pMesh->m_BndBox, bDuplicate);
		v2 = find_duplicate_vertex(Grid, v2, pMesh->m_BndBox, bDuplicate);

		// 이전 삼각형의 텍셀과 법선을 구하여 에지를 생성하고
		EgTexel *t0 = f->GetEdge(0)->m_pTexel;
		EgTexel *t1 = f->GetEdge(1)->m_pTexel;
		EgTexel *t2 = f->GetEdge(2)->m_pTexel;
		EgNormal *n0 = f->GetEdge(0)->m_pNormal;
		EgNormal *n1 = f->GetEdge(1)->m_pNormal;
		EgNormal *n2 = f->GetEdge(2)->m_pNormal;
		EgEdge *e0 = new EgEdge(v0, t0, n0);
		EgEdge *e1 = new EgEdge(v1, t1, n1);
		EgEdge *e2 = new EgEdge(v2, t2, n2);

		// 새로운 삼각형을 생성하여 리스트에 추가한다.
		EgFace *nf = new EgFace(e0, e1, e2, f->m_pMaterial, f->m_GroupName);
		NewFaces.push_back(nf);
	}

	// 기존 정점의 리스트와 삼각형 리스트를 제거한다.
	for (EgVertex *v : pMesh->m_pVerts)
		delete v;
	for (EgFace *f : pMesh->m_pFaces)
		delete f;
	pMesh->m_pVerts.clear();
	pMesh->m_pFaces.clear();

	// 새로운 정점의 리스트와 삼각형 리스트를 생성한다.
	for (int i = 0; i < 1000; ++i)
		for (EgVertex *v : Grid[i])
			pMesh->AddVertex(v);
	for (EgFace *f : NewFaces)
		pMesh->AddFace(f);

	// 에지의 연결관계, 법선, 경계상자를 갱신한다.
	pMesh->InitEdgeMate();
	pMesh->UpdateNormal(SMOOTH);
	pMesh->UpdateBndBox();

	// 렌더링 버퍼를 초기화 한다.
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();
	pMesh->m_TexelBuf.clear();

	// 정점의 개수를 반환한다.
	return pMesh->GetNumVert();
}


