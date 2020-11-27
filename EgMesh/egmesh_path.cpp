#include "..\usr\include\EgMesh\egmesh.h"
#include <cstring>
#include <vector>
#include <map>
#include <direct.h>

// 측지 경로 구하는 함수들
static double propagate_geodesic_dist_field(GeodesicHeap &MinHeap, std::vector<double> &Distances, double BndryDistance);
static double propagate_geodesic_dist_field(GeodesicHeap &MinHeap, std::vector<double> &Distances, EgVertex *T);
static double propagate_geodesic_dist_field(GeodesicHeap &MinHeap, std::vector<double> &Distances, EgFacePt *T);

static float get_geodesic_dist(EgVertex *vj, EgEdge *e, std::vector<double> &Distances);
static float get_geodesic_dist(EgPos pk, EgEdge *e, float *t, std::vector<double> &Distances);

// 평면 절단 경로 구하는 함수들
static int find_next_planar_path_closed(EgFacePt *P, EgEdge *e, float t, EgPlane *Pln, std::vector<std::pair<EgEdge *, float>> &CutInfo);

/*!
*	\brief	생성자
*	\note	마지막 수정일: 2020-07-31
*
*	\param	pVert[in]	정점에 대한 포인터
*	\param	Dist[in]	소스 정점까지의 거리
*/
EgGeodesic::EgGeodesic(EgVertex *pVert, double Dist)
{
	m_pVert = pVert;
	m_Distance = Dist;
}

/*!
*	\brief	복사 생성자
*	\note	마지막 수정일: 2020-07-31
*
*	\param	cpy[in]		복사될 객체
*/
EgGeodesic::EgGeodesic(const EgGeodesic &cpy)
{
	m_pVert = cpy.m_pVert;
	m_Distance = cpy.m_Distance;
}

/*!
*	\brief	소멸자
*	\note	마지막 수정일: 2020-07-31
*/
EgGeodesic::~EgGeodesic()
{
}

/*!
*	\brief	대입 연산자
*	\note	마지막 수정일: 2020-07-31
*
*	\param	rhs[in] 대입할 객체
*
*	\return 대입된 자신을 반환한다.
*/
EgGeodesic &EgGeodesic::operator =(const EgGeodesic &rhs)
{
	m_pVert = rhs.m_pVert;
	m_Distance = rhs.m_Distance;
	return *this;
}

/*!
*	\brief	비교 연산자
*	\note	마지막 수정일: 2020-07-31
*
*	\param	lhs[in]		왼쪽 객체
*	\param	rhs[in]		오른쪽 객체
*
*	\return 왼쪽 객체가 오른쪽 객체보다 크면 true, 아니면 false를 반환한다.
*/
bool operator >(const EgGeodesic &lhs, const EgGeodesic &rhs)
{
	return (lhs.m_Distance > rhs.m_Distance);
}

/*!
*	\brief	소스 정점에서 경계 거리 이내의 삼각형(교차 삼각형 포함)의 각 정점까지의 최소 거리를 계산한다.
*	\note	경계 외부 삼각형의 정점까지의 거리는 INFINITY로 설정된다.
*	\note	마지막 수정일: 2020-08-31
*
*	\param	pMesh[in]			대상 메쉬
*	\param	S[in]				소스 정점에 대한 포인터
*	\param	Distances[out]		소스 정점에서 i번째 정점까지의 거리가 Distances[i]에 저장된다.
*	\param	BndryDistance[in]	검사할 경계 거리(음수인 경우 전체 메쉬를 계산한다)
*
*	\return 실제 구해진 최대 거리(>= BndryDistance)를 반환한다.
*/
double get_geodesic_dist_field(EgMesh *pMesh, EgVertex *S, std::vector<double> &Distances, double BndryDistance)
{
	// 수행 시간을 측정한다.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// 소스를 제외한 각 정점까지의 거리를 무한대로 초기화 한다.
	Distances.assign(pMesh->GetNumVert(), INFINITY);
	Distances[S->m_Idx] = 0.0;

	// 소스 정점의 측지 거리 정보를 최소힙에 추가한다.
	GeodesicHeap MinHeap;
	MinHeap.push(EgGeodesic(S, 0.0));

	// 측지 거리장을 전파한다.
	double MaxDistance = propagate_geodesic_dist_field(MinHeap, Distances, BndryDistance);

	// 계산 결과를 출력한다.
#ifdef _DEBUG
	printf("Input boundary distance = %lf\n", BndryDistance);
	printf("Maximum distance = %lf\n", MaxDistance);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// 실제 구해진 최대 거리(>= BndryDistance)를 반환한다.
	return MaxDistance;
}

/*!
*	\brief	최소힙의 정점들에 대하여 측지 거리장을 전파한다.
*	\note	마지막 수정일: 2020-08-31
*
*	\param	MinHeap[in]			초기화된 최소힙
*	\param	Distances[out]		소스 정점에서 i번째 정점까지의 거리가 Distances[i]에 저장된다.
*	\param	BndryDistance[in]	검사할 경계 거리(음수인 경우 전체 메쉬를 계산한다)
*
*	\return 실제 구해진 최대 거리(>= 경계 거리)를 반환한다.
*/
static double propagate_geodesic_dist_field(GeodesicHeap &MinHeap, std::vector<double> &Distances, double BndryDistance)
{
	// 최소힙의 정보가 모두 처리될 때까지
	double MaxDistance = 0.0;
	while (!MinHeap.empty())
	{
		// 소스 정점에서 가장 까가운 정점의 정보를 꺼내서
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// 이미 갱신된 데이터라면 스킵한다.
		int vidx = info.m_pVert->m_Idx;
		if (info.m_Distance > Distances[vidx])
			continue;

		// 1링 정점의 최단 거리를 갱신한다.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// 정점 vj까지의 거리를 계산하여
			EgVertex *vj = EV(e);
			float d1 = get_geodesic_dist(vj, e->m_pNext->m_pNext, Distances);
			float d2 = (e->m_pMate != NULL) ? get_geodesic_dist(vj, e->m_pMate->m_pNext, Distances) : INFINITY;
			float dj = MIN(d1, d2);

			// 새로운 최단 거리를 찾은 경우
			if (dj < Distances[vj->m_Idx])
			{
				// 정점의 최단 거리를 갱신하고
				Distances[vj->m_Idx] = dj;

				if (BndryDistance < 0.0)	// 모든 정점을 조사한다면 무조건 힙에 추가하고
					MinHeap.push(EgGeodesic(vj, dj));
				else if (dj < BndryDistance) // 아니라면 경계 거리에 포함된 정점만 힙에 추가한다.
					MinHeap.push(EgGeodesic(vj, dj));

				// 발견된 최대 거리를 갱신한다.
				MaxDistance = MAX(MaxDistance, dj);
			}

			// 예외 처리: 경계 정점일 경우도 처리한다.
			if (e->m_pNext->m_pNext->m_pMate == NULL)
			{
				EgVertex *vj = e->m_pNext->m_pNext->m_pVert;
				float dj = get_geodesic_dist(vj, e, Distances);

				// 새로운 최단 거리를 찾은 경우
				if (dj < Distances[vj->m_Idx])
				{
					// 정점의 최단 거리를 갱신하고
					Distances[vj->m_Idx] = dj;

					if (BndryDistance < 0.0)	// 모든 정점을 조사한다면 무조건 힙에 추가하고
						MinHeap.push(EgGeodesic(vj, dj));
					else if (dj < BndryDistance) // 아니라면 경계 거리에 포함된 정점만 힙에 추가한다.
						MinHeap.push(EgGeodesic(vj, dj));

					// 발견된 최대 거리를 갱신한다.
					MaxDistance = MAX(MaxDistance, dj);
				}
			}
		}
	}

	// 최대 거리를 반환한다.
	return MaxDistance;
}

/*!
*	\brief	정점(vj)에서 소스 정점까지 최단 거리 dj를 계산하는 함수
*	\note	마지막 수정일: 2020-07-31
*
*	\param	vj[in]	대상 정점
*	\param	e[in]	정점 vj와 마주보는 에지 e = (vk, vi)
*
*	\return	최단 거리를 반환한다.
*/
static float get_geodesic_dist(EgVertex *vj, EgEdge *e, std::vector<double> &Distances)
{
	// 삼각형의 세 정점과 선분의 길이를 계산하고
	EgVertex *vi = EV(e);
	EgVertex *vk = SV(e);
	float dij_sq = (float)dist_sq(vi->m_Pos, vj->m_Pos);
	float djk_sq = (float)dist_sq(vj->m_Pos, vk->m_Pos);
	float dki_sq = (float)dist_sq(vk->m_Pos, vi->m_Pos);
	float dij = SQRT(dij_sq);
	float djk = SQRT(djk_sq);
	float dki = SQRT(dki_sq);

	// 소스 정점에서 vj까지의 거리 dj를 최소 에지 거리로 초기화 한다.
	float di = (float)Distances[vi->m_Idx];
	float dk = (float)Distances[vk->m_Idx];
	float dj = (di + dij < dk + djk) ? (di + dij) : (dk + djk);

	// 가상의 소스 정점 vs가 존재한다면
	if (ABS(dk - di) < dki && dki < dk + di)
	{
		// 2차원 평면에 매핑한 vs(x0, y0), vk(0, 0), vi(-dki, 0), vj(x1, y1)의 좌표를 구하고,
		float x0 = (SQR(di) - SQR(dk) - dki_sq) / (2 * dki);
		float y0 = SQRT(SQR(dk) - SQR(x0));
		float x1 = (dij_sq - djk_sq - dki_sq) / (2 * dki);
		float y1 = -SQRT(djk_sq - SQR(x1));

		// 선분 (vs, vk)와 (vj, vi)의 교차점의 x좌표를 구하여
		float x = (x0 * y1 - x1 * y0) / (y1 - y0);

		// 교차점이 선분 (vi, vk)사이에 존재한다면 거리 dj를 갱신한다.
		if (x > -dki && x < 0)
			dj = SQRT(SQR(x1 - x0) + SQR(y1 - y0));
	}

	// 계산된 거리를 반환한다.
	return dj;
}

/*!
*	\brief	소스 정점에서 타겟 정점까지의 거리 내의 정점들의 측지 거리장을 계산한다.
*	\note	타겟 정점까지 거리보다 먼 정점들의 거리는 INFINITY로 설정된다.
*	\note	마지막 수정일: 2020-08-31
*
*	\param	pMesh[in]		대상 메쉬
*	\param	S[in]			소스 정점에 대한 포인터
*	\param	T[in]			타겟 정점에 대한 포인터
*	\param	Distances[out]	소스 정점에서 i번째 정점까지의 거리가 Distances[i]에 저장된다.
*
*	\return 실제 구해진 최대 거리(>= 경계 거리)를 반환한다.
*/
double get_geodesic_dist_field(EgMesh *pMesh, EgVertex *S, EgVertex *T, std::vector<double> &Distances)
{
	// 수행 시간을 측정한다.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// 소스 정점에서 각 정점까지의 거리를 무한대로 초기화 한다.
	Distances.assign(pMesh->GetNumVert(), INFINITY);
	Distances[S->m_Idx] = 0.0;

	// 소스 정점의 정보를 최소힙에 추가한다.
	GeodesicHeap MinHeap;
	MinHeap.push(EgGeodesic(S, 0.0));

	// 타겟 정점이 포함될 때까지 측지 거리장을 계산한다.
	double MaxDistance = propagate_geodesic_dist_field(MinHeap, Distances, T);

	// 계산 결과를 출력한다.
#ifdef _DEBUG
	printf("Maximum distance = %lf\n", MaxDistance);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// 실제 구해진 최대 거리(>= 경계 거리)를 반환한다.
	return MaxDistance;
}

/*!
*	\brief	타겟 정점이 포함될 때까지 측지 거리장을 전파한다.
*	\note	마지막 수정일: 2020-07-31
*
*	\param	MinHeap[in]		초기화된 최소힙
*	\param	Distances[out]	소스 정점에서 i번째 정점까지의 거리가 Distances[i]에 저장된다.
*	\param	T[in]			타겟 정점에 대한 포인터
*
*	\return 실제 구해진 최대 거리(>= 경계 거리)를 반환한다.
*/
static double propagate_geodesic_dist_field(GeodesicHeap &MinHeap, std::vector<double> &Distances, EgVertex *T)
{
	// 최소힙의 정보가 모두 처리될 때까지
	double MaxDistance = 0.0;
	while (!MinHeap.empty())
	{
		// 소스 정점에서 가장 까가운 정점의 정보를 꺼내서
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// 이미 갱신된 데이터라면 스킵한다.
		int vidx = info.m_pVert->m_Idx;
		if (info.m_Distance > Distances[vidx])
			continue;

		// 정점의 1링 정점의 최단 거리를 갱신한다.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// 정점 vj까지의 거리를 계산하여
			EgVertex *vj = EV(e);
			float d1 = get_geodesic_dist(vj, e->m_pNext->m_pNext, Distances);
			float d2 = (e->m_pMate != NULL) ? get_geodesic_dist(vj, e->m_pMate->m_pNext, Distances) : INFINITY;
			float dj = MIN(d1, d2);

			// 새로운 최단 거리를 찾은 경우
			if (dj < Distances[vj->m_Idx])
			{
				// 정점의 최단 거리를 갱신하고, 새로운 정보를 힙에 추가한다.
				Distances[vj->m_Idx] = dj;
				MinHeap.push(EgGeodesic(vj, dj));

				// 발견된 최대 거리를 갱신한다.
				MaxDistance = MAX(MaxDistance, dj);
			}

			// 예외 처리: 경계 정점일 경우도 처리한다.
			if (e->m_pNext->m_pNext->m_pMate == NULL)
			{
				EgVertex *vj = e->m_pNext->m_pNext->m_pVert;
				float dj = get_geodesic_dist(vj, e, Distances);

				// 새로운 최단 거리를 찾은 경우
				if (dj < Distances[vj->m_Idx])
				{
					// 정점의 최단 거리를 갱신하고, 새로운 정보를 힙에 추가한다.
					Distances[vj->m_Idx] = dj;
					MinHeap.push(EgGeodesic(vj, dj));

					// 발견된 최대 거리를 갱신한다.
					MaxDistance = MAX(MaxDistance, dj);
				}
			}
		}

		// 타겟 정점이 처리되었다면 중단한다.
		if (vi == T)
			break;
	}

	// 최대 거리를 반환한다.
	return MaxDistance;
}

/*!
*	\brief	삼각형 내부점에서 경계 거리 이내의 각 정점까지의 최소 거리를 계산한다.
*	\note	경계 외부의 삼각형 정점들까지의 거리는 INFINITY로 설정된다.
*	\note	마지막 수정일: 2020-08-31
*
*	\param	pMesh[in]			대상 메쉬
*	\param	S[in]				삼각형 내부점에 대한 포인터
*	\param	Distances[out]		i번째 정점까지의 거리가 Distances[i]에 저장된다.
*	\param	BndryDistance[in]	검사할 경계 거리(음수인 경우 전체 메쉬를 계산한다)
*
*	\return 실제 구해진 최대 거리(>= 경계 거리)를 반환한다.
*/
double get_geodesic_dist_field(EgMesh *pMesh, EgFacePt *S, std::vector<double> &Distances, double BndryDistance)
{
	// 수행 시간을 측정한다.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// 소스 정점에서 각 정점까지의 거리를 무한대로 초기화 한다.
	Distances.assign(pMesh->GetNumVert(), INFINITY);

	// 내부점에서 삼각형의 각 정점까지의 거리를 구하여 힙에 추가한다.
	EgPos p = S->Pos();
	GeodesicHeap MinHeap;
	for (int i = 0; i < 3; ++i)
	{
		EgVertex *v = S->m_pFace->GetVertex(i);
		double d = dist(p, v->m_Pos);
		Distances[v->m_Idx] = d;
		MinHeap.push(EgGeodesic(v, d));
	}

	// 측지 거리장을 전파한다.
	double MaxDistance = propagate_geodesic_dist_field(MinHeap, Distances, BndryDistance);

	// 계산 결과를 출력한다.
#ifdef _DEBUG
	printf("Boundary distance = %lf\n", BndryDistance);
	printf("Maximum distance = %lf\n", MaxDistance);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// 실제 구해진 최대 거리(>= 경계 거리)를 반환한다.
	return MaxDistance;
}

/*!
*	\brief	삼각형 내부점에서 타겟 정점까지의 거리 내의 정점들의 측지 거리장을 계산하는 함수
*	\note	타겟 정점까지 거리보다 먼 정점들의 거리는 INFINITY로 설정된다.
*	\note	마지막 수정일: 2020-08-31
*
*	\param	pMesh[in]		대상 메쉬
*	\param	S[in]			소스 점에 대한 포인터
*	\param	T[in]			타겟 점에 대한 포인터
*	\param	Distances[out]	P에서 i번째 정점까지의 거리가 Distances[i]에 저장된다.
*
*	\return 실제 구해진 최대 거리(>= 경계 거리)를 반환한다.
*/
double get_geodesic_dist_field(EgMesh *pMesh, EgFacePt *S, EgFacePt *T, std::vector<double> &Distances)
{
	// 수행 시간을 측정한다.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// 소스 정점에서 각 정점까지의 거리를 무한대로 초기화 한다.
	Distances.assign(pMesh->GetNumVert(), INFINITY);

	// 내부점에서 삼각형의 각 정점까지의 거리를 구하여 힙에 추가한다.
	EgPos p = S->Pos();
	GeodesicHeap MinHeap;
	for (int i = 0; i < 3; ++i)
	{
		EgVertex *v = S->m_pFace->GetVertex(i);
		double d = dist(p, v->m_Pos);
		Distances[v->m_Idx] = d;
		MinHeap.push(EgGeodesic(v, d));
	}

	// 측지 거리장을 전파한다.
	double MaxDistance = propagate_geodesic_dist_field(MinHeap, Distances, T);

	// 계산 결과를 출력한다.
#ifdef _DEBUG
	printf("Maximum distance = %lf\n", MaxDistance);
	printf("Elapsed time for geodesic field = %u ms\n\n", clock() - st);
#endif

	// 실제 구해진 최대 거리(>= 경계 거리)를 반환한다.
	return MaxDistance;
}

/*!
*	\brief	타겟 정점이 포함될 때까지 측지 거리장을 전파한다.
*	\note	마지막 수정일: 2020-07-31
*
*	\param	MinHeap[in]		초기화된 최소힙
*	\param	Distances[out]	소스 정점에서 i번째 정점까지의 거리가 Distances[i]에 저장된다.
*	\param	T[in]			타겟 점에 대한 포인터
*
*	\return 실제 구해진 최대 거리(>= 경계 거리)를 반환한다.
*/
static double propagate_geodesic_dist_field(GeodesicHeap &MinHeap, std::vector<double> &Distances, EgFacePt *T)
{
	// 타겟 점이 포한된 삼각형의 세 정점을 구한다.
	EgVertex *T0 = T->m_pFace->GetVertex(0);
	EgVertex *T1 = T->m_pFace->GetVertex(1);
	EgVertex *T2 = T->m_pFace->GetVertex(2);

	// 최소힙의 정보가 모두 처리될 때까지
	double MaxDistance = 0.0;
	while (!MinHeap.empty())
	{
		// 소스 정점에서 가장 까가운 정점의 정보를 꺼내서
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// 이미 갱신된 데이터라면 스킵한다.
		int vidx = info.m_pVert->m_Idx;
		if (info.m_Distance > Distances[vidx])
			continue;

		// 정점의 1링 정점의 최단 거리를 갱신한다.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// 정점 vj까지의 거리를 계산하여
			EgVertex *vj = EV(e);
			float d1 = get_geodesic_dist(vj, e->m_pNext->m_pNext, Distances);
			float d2 = (e->m_pMate != NULL) ? get_geodesic_dist(vj, e->m_pMate->m_pNext, Distances) : INFINITY;
			float dj = MIN(d1, d2);

			// 새로운 최단 거리를 찾은 경우
			if (dj < Distances[vj->m_Idx])
			{
				// 정점의 최단 거리를 갱신하고, 새로운 정보를 힙에 추가한다.
				Distances[vj->m_Idx] = dj;
				MinHeap.push(EgGeodesic(vj, dj));

				// 발견된 최대 거리를 갱신한다.
				MaxDistance = MAX(MaxDistance, dj);
			}

			// 예외 처리: 경계 정점일 경우도 처리한다.
			if (e->m_pNext->m_pNext->m_pMate == NULL)
			{
				EgVertex *vj = e->m_pNext->m_pNext->m_pVert;
				float dj = get_geodesic_dist(vj, e, Distances);

				// 새로운 최단 거리를 찾은 경우
				if (dj < Distances[vj->m_Idx])
				{
					// 정점의 최단 거리를 갱신하고, 새로운 정보를 힙에 추가한다.
					Distances[vj->m_Idx] = dj;
					MinHeap.push(EgGeodesic(vj, dj));

					// 발견된 최대 거리를 갱신한다.
					MaxDistance = MAX(MaxDistance, dj);
				}
			}
		}

		// 타겟 점이 포한된 삼각형의 세 정점이 모두 처리되면 중단한다.
		if (vi == T0)
			T0 = NULL;
		if (vi == T1)
			T1 = NULL;
		if (vi == T2)
			T2 = NULL;
		if (T0 == NULL && T1 == NULL && T2 == NULL)
			break;
	}

	// 최대 거리를 반환한다.
	return MaxDistance;
}

/*!
*	\brief	소스 정점(삼각형)들에서 경계 거리(BndryDistance) 이내의 삼각형(교차 포함)의 각 정점까지의 최소 거리를 계산한다.
*	\note	경계 외부의 삼각형 정점들까지의 거리는 INFINITY로 설정된다.
*	\note	마지막 수정일: 2020-08-31
*
*	\param	pMesh[in]			대상 메쉬
*	\param	SrcList[in]			메쉬의 소스 정점(삼각형) 리스트
*	\param	Distances[out]		i번째 정점까지의 거리가 Distances[i]에 저장된다.
*	\param	BndryDistance[in]	검사할 경계 거리(음수인 경우 전체 메쉬를 계산한다)
*
*	\return 실제 구해진 최대 거리(>= 경계 거리)를 반환한다.
*/
double get_geodesic_dist_field(EgMesh *pMesh, std::vector<EgObject *> &SrcList, std::vector<double> &Distances, double BndryDistance)
{
	// 수행 시간을 측정한다.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// 정점, 삼각형, 삼각형 내부점이 선택된 경우가 아니라면 리턴한다.
	std::string ClsName = SrcList[0]->GetClassName();
	if (ClsName != "EgVertex" && ClsName != "EgFace" && ClsName != "EgFacePt")
		return -1;

	// 각 정점까지의 거리를 무한대로 초기화 한다.
	Distances.assign(pMesh->GetNumVert(), INFINITY);

	// 최소힙을 생성한다.
	GeodesicHeap MinHeap;

	// 선택된 객체가 정점이라면,
	if (ClsName == "EgVertex")
	{
		// 선택된 각 정점의 거리를 0으로 초기화하고 힙에 추가한다.
		for (EgObject *obj : SrcList)
		{
			EgVertex *v = (EgVertex *)obj;
			Distances[v->m_Idx] = 0.0;
			MinHeap.push(EgGeodesic(v, 0.0));
		}
	}

	// 선택된 객체가 삼각형이라면,
	if (ClsName == "EgFace")
	{
		// 선택된 각 삼각형의 정점의 거리를 0으로 초기화 하고 힙에 추가한다.
		for (EgObject *obj : SrcList)
		{
			EgFace *f = (EgFace *)obj;
			for (int i = 0; i < 3; ++i)
			{
				EgVertex *v = f->GetVertex(i);
				Distances[v->m_Idx] = 0.0;
				MinHeap.push(EgGeodesic(v, 0.0));
			}
		}
	}

	// 선택된 객체가 삼각형 내부점 이라면,
	if (ClsName == "EgFacePt")
	{
		// 각각의 내부점에 대하여
		for (EgObject *obj : SrcList)
		{
			// 내부점에서 삼각형의 각 정점까지의 거리를 구하여 힙에 추가한다.
			EgFacePt *Pt = (EgFacePt *)obj;
			EgFace *f = Pt->m_pFace;
			for (int i = 0; i < 3; ++i)
			{
				EgVertex *v = f->GetVertex(i);
				double d = Pt->Dist2Vert(i);
				Distances[v->m_Idx] = d;
				MinHeap.push(EgGeodesic(v, d));
			}
		}
	}

	// 측지 거리장을 전파한다.
	double MaxDistance = propagate_geodesic_dist_field(MinHeap, Distances, BndryDistance);

	// 계산 결과를 출력한다.
#ifdef _DEBUG
	printf("Boundary distance = %lf\n", BndryDistance);
	printf("Maximum distance = %lf\n", MaxDistance);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// 실제 구해진 최대 거리(>= 경계 거리)를 반환한다.
	return MaxDistance;
}

/*!
*	\brief	소스 정점에서 경계 거리(BndryDistance) 이내의 삼각형(교차 삼각형 포함)의 각 정점까지 에지를 따르는 최소 거리를 계산한다.
*	\note	경계 외부 삼각형의 정점들까지의 거리는 INFINITY로 설정된다.
*	\note	마지막 수정일: 2020-08-31
*
*	\param	pMesh[in]			대상 메쉬
*	\param	S[in]				소스 정점에 대한 포인터
*	\param	Distances[out]		소스 정점에서 i번째 정점까지의 에지 거리가 Distances[i]에 저장된다.
*	\param	BndryDistance[in]	검사할 경계 거리(음수인 경우 전체 메쉬를 계산한다)
*
*	\return 실제 구해진 최대 거리(>= 경계 거리)를 반환한다.
*/
double get_edge_dist_field(EgMesh *pMesh, EgVertex *S, std::vector<double> &Distances, double BndryDistance)
{
	// 수행 시간을 측정한다.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// 각 정점까지의 거리를 무한대로 초기화 한다.
	Distances.assign(pMesh->GetNumVert(), INFINITY);

	// 최소힙을 생성하고 0으로 초기화된 소스 정점의 정보를 추가한다.
	GeodesicHeap MinHeap;
	Distances[S->m_Idx] = 0.0;
	MinHeap.push(EgGeodesic(S, 0.0));

	// 최소힙의 정보가 모두 처리될 때까지
	double MaxDistance = 0.0;
	while (!MinHeap.empty())
	{
		// 소스 정점에서 가장 까가운 정점의 정보를 꺼내서
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// 이미 처리된 정보라면 스킵한다.
		int vidx = info.m_pVert->m_Idx;
		if (info.m_Distance > Distances[vidx])
			continue;

		// 정점의 1링 정점 vj의 최단 거리를 갱신한다.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// Dijkstra 알고리즘을 적용한다.
			EgVertex *vj = EV(e);
			EgVertex *vk = EV(e->m_pNext);
			float di = (float)Distances[vi->m_Idx];
			float dk = (float)Distances[vk->m_Idx];
			float dij = (float)dist(vi->m_Pos, vj->m_Pos);
			float djk = (float)dist(vj->m_Pos, vk->m_Pos);
			float d1 = (di + dij < dk + djk) ? (di + dij) : (dk + djk);
			float d2 = INFINITY;
			if (!e->IsBndry())
			{
				vk = EV(e->m_pMate->m_pNext);
				dk = (float)Distances[vk->m_Idx];
				djk = (float)dist(vj->m_Pos, vk->m_Pos);
				d2 = (di + dij < dk + djk) ? (di + dij) : (dk + djk);
			}

			// 새로운 최단 거리를 찾은 경우
			float dj = MIN(d1, d2);
			if (dj < Distances[vj->m_Idx])
			{
				// 정점의 최단 거리를 갱신하고
				Distances[vj->m_Idx] = dj;

				if (BndryDistance < 0.0)	// 모든 정점을 조사한다면 무조건 힙에 추가하고
					MinHeap.push(EgGeodesic(vj, dj));
				else if (dj < BndryDistance) // 아니라면 경계 거리에 포함된 정점만 힙에 추가한다.
					MinHeap.push(EgGeodesic(vj, dj));

				// 발견된 최대 거리를 갱신한다.
				MaxDistance = MAX(MaxDistance, dj);
			}
		}
	}

	// 계산 결과를 출력한다.
#ifdef _DEBUG
	printf("Boundary distance = %lf\n", BndryDistance);
	printf("Maximum distance = %lf\n", MaxDistance);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// 실제 구해진 최대 거리(>= 경계 거리)를 반환한다.
	return MaxDistance;
}

/*!
*	\brief	소스 정점에서 경계 거리 이내의 정점까지의 위상 거리를 계산하는 함수
*
*	\param	pMesh[in]			대상 메쉬
*	\param	S[in]				소스 정점
*	\param	Distances[out]		소스 정점에서 i번째 정점까지의 위상 거리가 Distances[i]에 저장된다.
*	\param	BndryDistance[in]	검사할 경계 위상 거리(음수인 경우 전체 메쉬를 계산한다)
*
*	\return 실제 구해진 최대 위상 거리(>= 경계 거리)를 반환한다.
*/
int get_topology_dist_field(EgMesh *pMesh, EgVertex *S, std::vector<int> &Distances, int BndryDistance)
{
	// 수행 시간을 측정한다.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// 각 정점까지의 거리를 무한대로 초기화 한다.
	Distances.assign(pMesh->GetNumVert(), INT_MAX);

	// 최소힙을 생성하고 0으로 초기화된 소스 정점의 정보를 추가한다.
	GeodesicHeap MinHeap;
	Distances[S->m_Idx] = 0;
	MinHeap.push(EgGeodesic(S, 0.0));

	// 최소힙의 정보가 모두 처리될 때까지
	int MaxDistance = 0;
	while (!MinHeap.empty())
	{
		// 소스 정점에서 가장 까가운 정점의 정보를 꺼내서
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// 이미 처리된 정보라면 스킵한다.
		int vidx = info.m_pVert->m_Idx;
		if ((int)info.m_Distance > Distances[vidx])
			continue;

		// 정점의 1링 정점 vj의 최단 거리를 갱신한다.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// Dijkstra 알고리즘을 적용한다.
			EgVertex *vj = EV(e);
			EgVertex *vk = EV(e->m_pNext);
			int di = Distances[vi->m_Idx];
			int dk = Distances[vk->m_Idx];
			int d1 = (di < dk) ? (di + 1) : (dk + 1);
			int d2 = 10000000;
			if (!e->IsBndry())
			{
				vk = EV(e->m_pMate->m_pNext);
				dk = Distances[vk->m_Idx];
				d2 = (di < dk) ? (di + 1) : (dk + 1);
			}

			// 새로운 최단 거리를 찾은 경우
			int dj = MIN(d1, d2);
			if (dj < Distances[vj->m_Idx])
			{
				// 정점의 최단 거리를 갱신하고
				Distances[vj->m_Idx] = dj;

				if (BndryDistance < 0.0)	// 모든 정점을 조사한다면 무조건 힙에 추가하고
					MinHeap.push(EgGeodesic(vj, (double)dj));
				else if (dj < BndryDistance) // 아니라면 경계 거리에 포함된 정점만 힙에 추가한다.
					MinHeap.push(EgGeodesic(vj, (double)dj));

				// 발견된 최대 거리를 갱신한다.
				MaxDistance = MAX(MaxDistance, dj);
			}
		}
	}

	// 계산 결과를 출력한다.
#ifdef _DEBUG
	printf("Boundary distance = %d\n", BndryDistance);
	printf("Maximum distance = %d\n", MaxDistance);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// 실제 구해진 최대 위상 거리를 반환한다.
	return MaxDistance;
}

/*!
*	\brief	두 정점 사이의 최단 경로와 교차 정보를 구하는 함수
*	\note	마지막 수정일: 2020-09-01
*
*	\param	S[in]			시작 정점에 대한 포인터
*	\param	E[in]			끝 정점에 대한 포인터
*	\param	Distances[in]	시작 정점 기준의 측지 거리장
*	\param	Path[out]		끝 정점에서 소스 정점으로 경로를 구성하는 교차점의 위치가 저장된다.
*	\param	CutInfo[out]	끝 정점에서 소스 정점으로 최단 경로의 교차 정보가 저장된다.
*
*	\return 최단 거리를 반환한다.
*/
double get_shortest_path(
	EgVertex *S,
	EgVertex *E,
	std::vector<double> &Distances,
	std::vector<EgPos> &Path,
	std::vector<std::pair<EgEdge *, float>> &CutInfo)
{
	// 시작 정점과 끝 정점이 같으면 리턴한다.
	if (S == E)
		return 0.0;

#ifdef _DEBUG
	clock_t st = clock();
#endif

	// 역추적을 통해 끝 정점에서 시작 정점까지의 최단 경로를 찾는다.	
	CutInfo.clear();
	EgEdge *e = E->m_pEdges[0];
	float t = 0.0f;
	while (true)
	{
		// 종료 조건
		if (EV(e) == S && !CutInfo.empty())
		{
			// 마지막 정보를 추가하고 종료한다.
			CutInfo.push_back(std::make_pair(e, 1.0f));
			break;
		}

		// 현재 교차점이 정점이라면
		if (EQ(t, 0.0f, 1.0e-5) || EQ(t, 1.0f, 1.0e-5))
		{
			// 현재 교차점을 구하여, 마지막 호출을 검사한다.
			EgVertex *vk = EQ(t, 0.0f, 1.0e-5) ? SV(e) : EV(e);
			EgEdge *ei = NULL, *ej = NULL;
			for (EgEdge *e : vk->m_pEdges)
			{
				// 검사 에지를 구하여
				ej = e->m_pNext;

				// 검사 에지 중 소스 정점에서 나오는 에지가 있다면
				if (SV(ej) == S)
					ei = ej = e;

				// 검사 에지 중 소스 정점으로 들어가는 에지가 있다면
				if (EV(ej) == S)
				{
					ei = e;
					break;
				}
			}
			if (ei != NULL)
			{
				// 마지막 정보를 추가하고 종료한다.
				CutInfo.push_back(std::make_pair(ei, 0.0f));
				CutInfo.push_back(std::make_pair(ej, 1.0f));
				break;
			}

			// 1-링 경계 에지를 검사하여 최단 경로가 통과할 다음 에지를 찾는다.
			EgEdge *e_star = NULL;
			float t_star, min_dist = INFINITY;
			for (EgEdge *ei : vk->m_pEdges)
			{
				// 검사 에지가 경계 에지인 경우 생략한다(예외 처리).
				EgEdge *ej = ei->m_pNext;
				if (ej->IsBndry())
					continue;

				// 최단 경로가 통과할 다음 에지를 찾는다.
				float tmp, d = get_geodesic_dist(POS(vk), ej, &tmp, Distances);
				if (d < min_dist || (EQ(d, min_dist, 1.0e-5) && tmp < t_star))
				{
					min_dist = d;
					t_star = tmp;
					e_star = ej;
				}
			}

			// 최단 경로에 교차점을 추가하고, 다음 에지를 검사한다(*).
			CutInfo.push_back(std::make_pair(e_star->m_pNext->m_pNext, 0.0f));
			t_star = EQ(t_star, 0.0f, 1.0e-5) ? 0.0f : t_star;
			t_star = EQ(t_star, 1.0f, 1.0e-5) ? 1.0f : t_star;
			e = e_star->m_pMate;
			t = 1.0f - t_star;
			continue;
		}
		else // 현재 교차점이 에지 위의 점이라면
		{
			// 최단 경로에 교차 정보를 추가하고
			CutInfo.push_back(std::make_pair(e, t));

			// 검사할 두 에지 e1, e2를 구하고
			EgEdge *e1 = e->m_pNext;
			EgEdge *e2 = e->m_pNext->m_pNext;

			// 소스 정점이 포함된 삼각형에 도달한 경우
			if (EV(e1) == S) // 또는 SV(e2) == S
			{
				// 마지막 교차 정보를 추가하고 종료한다.
				CutInfo.push_back(std::make_pair(e1, 1.0f));
				break;
			}

			// 검사할 에지가 모두 경계 에지라면(예외 처리)
			if (e1->IsBndry() && e2->IsBndry())
			{
				// 마지막 교차 정보를 제거하고
				CutInfo.pop_back();
#ifdef _DEBUG
				printf("Warning: two edges are boundaries !!!\n");
#endif
				// 현재 에지의 양끝 정점 중 소스 정점과 가까운 정점에서 다시 최단 경로를 조사한다.
				if (Distances[SV(e)->m_Idx] < Distances[EV(e)->m_Idx])
					t = 0.0f;
				else
				{
					e = e1;
					t = 0.0f;
				}
				continue;
			}

			// 두 에지를 통과하는 거리를 계산한다.
			EgPos pk = ::lerp(POS(SV(e)), POS(SV(e->m_pNext)), t);
			float t1, t2;

			// e2만 경계 에지인 경우 e1방향을 선택
			if (!e1->IsBndry() && e2->IsBndry())
			{
				get_geodesic_dist(pk, e1, &t1, Distances);
				t1 = EQ(t1, 0.0f, 1.0e-5) ? 0.0f : t1;
				t1 = EQ(t1, 1.0f, 1.0e-5) ? 1.0f : t1;
				e = e1->m_pMate;
				t = 1.0f - t1;
				continue;
			}
			// e1만 경계 에지인 경우 e2방향을 선택
			else if (e1->IsBndry() && !e2->IsBndry())
			{
				get_geodesic_dist(pk, e2, &t2, Distances);
				t2 = EQ(t2, 0.0f, 1.0e-5) ? 0.0f : t2;
				t2 = EQ(t2, 1.0f, 1.0e-5) ? 1.0f : t2;
				e = e2->m_pMate;
				t = 1.0f - t2;
				continue;
			}
			else // 모두 경계 에지가 아니라면
			{
				// 짧은 경로의 에지를 선택한다.	
				float d1 = get_geodesic_dist(pk, e1, &t1, Distances);
				float d2 = get_geodesic_dist(pk, e2, &t2, Distances);
				t1 = EQ(t1, 0.0f, 1.0e-5) ? 0.0f : t1;
				t1 = EQ(t1, 1.0f, 1.0e-5) ? 1.0f : t1;
				t2 = EQ(t2, 0.0f, 1.0e-5) ? 0.0f : t2;
				t2 = EQ(t2, 1.0f, 1.0e-5) ? 1.0f : t2;
				e = (d1 < d2) ? e1->m_pMate : e2->m_pMate;
				t = (d1 < d2) ? (1.0f - t1) : (1.0f - t2);
				continue;
			}
		}
	}

	// 절단점 리스트를 구하여
	Path.clear();
	for (std::pair<EgEdge *, float> &info : CutInfo)
		Path.push_back(::lerp(POS(SV(info.first)), POS(EV(info.first)), info.second));

	// 최단 거리를 계산하고
	double MinDist = 0.0;
	for (int i = 0; i < NUM(Path) - 1; ++i)
		MinDist += dist(Path[i], Path[i + 1]);

	// 최단 길이와 계산 시간을 출력한 후
#ifdef _DEBUG
	printf("Shortest distances = %lf\n", MinDist);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// 최단 거리를 반환한다.
	return MinDist;
}

/*!
*	\brief	현재 위치에서 가상의 소스 정점까지 최단 거리를 계산하는 함수
*	\note	마지막 수정일: 2020-05-26
*
*	\param	pk[in]			현재 위치
*	\param	e[in]			검사할 에지 e = (vi, vj)
*	\param	t[out]			에지와 교차점의 파라미터가 저장된다.
*	\param	Distances[in]	소스 정점에 대한 측지 거리장
*
*	\return	소스 정점까지의 최단 거리를 반환한다.
*/
static float get_geodesic_dist(EgPos pk, EgEdge *e, float *t, std::vector<double> &Distances)
{
	// 삼각형의 세 정점과 선분의 길이를 계산하고
	EgVertex *vi = SV(e);
	EgVertex *vj = EV(e);
	float dij_sq = (float)dist_sq(vi->m_Pos, vj->m_Pos);
	float djk_sq = (float)dist_sq(vj->m_Pos, pk);
	float dki_sq = (float)dist_sq(pk, vi->m_Pos);
	float dij = SQRT(dij_sq);
	float djk = SQRT(djk_sq);
	float dki = SQRT(dki_sq);

	// 소스 정점에서 vi까지의 거리 di를 최소 에지 거리로 초기화 한다.
	float di = (float)Distances[vi->m_Idx];
	float dj = (float)Distances[vj->m_Idx];
	float dk = (di + dki < dj + djk) ? (di + dki) : (dj + djk);
	*t = (di + dki < dj + djk) ? 0.0f : 1.0f;

	// 가상의 소스 정점 vs가 존재한다면
	if (ABS(di - dj) < dij && dij < di + dj)
	{
		// 2차원 평면에 매핑한 vs(x0, y0), vi(0, 0), vj(-dij, 0), pk(x1, y1)의 좌표를 구하고,
		float x0 = (SQR(dj) - SQR(di) - dij_sq) / (2 * dij);
		float y0 = SQRT(SQR(di) - SQR(x0));
		float x1 = (djk_sq - dki_sq - dij_sq) / (2 * dij);
		float y1 = -SQRT(dki_sq - SQR(x1));

		// 선분 (vs, vk)와 (vj, vi)의 교차점의 x좌표를 구하여
		if (EQ(y0, y1, 1.0e-6))
			printf("Error in get_geodesic_dist()...\n");
		float x = (x0 * y1 - x1 * y0) / (y1 - y0);

		// 교차점이 선분 (vj, vi)사이에 존재한다면 거리 di를 갱신한다.
		if (x > -dij && x < 0.0)
		{
			dk = SQRT(SQR(x1 - x0) + SQR(y1 - y0));
			*t = -x / dij;
		}
	}

	// 계산된 거리를 반환한다.
	return dk;
}

/*!
*	\brief	메쉬 위의 임의의 두 점 사이의 최단 경로와 교차 정보를 구하는 함수
*	\note	마지막 수정일: 2020-08-31
*
*	\param	S[in]			시작점에 대한 포인터
*	\param	E[in]			끝점에 대한 포인터
*	\param	Distances[in]	소스점 기준의 측지 거리장
*	\param	Path[out]		타겟점에서 소스점으로 경로를 구성하는 교차점의 위치가 저장된다.
*	\param	CutInfo[out]	타겟점에서 소스점으로 최단 경로의 교차 정보가 저장된다.
*
*	\return 최단 거리를 반환한다.
*/
double get_shortest_path(
	EgFacePt *S,
	EgFacePt *E,
	std::vector<double> &Distances,
	std::vector<EgPos> &Path,
	std::vector<std::pair<EgEdge *, float>> &CutInfo)
{
	// 동일한 삼각형 내에 있다면 리턴한다.
	if (S->m_pFace == E->m_pFace)
		return -1.0;

	// 수행 시간을 측정한다.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// 시작 에지를 구한다.
	float t = 0.0f;
	EgEdge *e = NULL;
	float min_d = INFINITY;
	for (int i = 0; i < 3; ++i)
	{
		float ti;
		EgEdge *ei = E->m_pFace->GetEdge(i);
		float d = (ei->m_pMate == NULL) ? INFINITY : get_geodesic_dist(E->Pos(), ei, &ti, Distances);
		if (d < min_d)
		{
			min_d = d;
			t = 1.0f - ti;
			e = ei->m_pMate;
		}
	}

	// 역추적을 통해 E에서 S까지의 최단 경로의 정보를 찾는다. 
	CutInfo.clear();
	EgFace *pSrcFace = S->m_pFace;
	while (true)
	{
		// 종료 조건: 소스점이 포함된 삼각형에 도달했다면
		if (e->m_pFace == pSrcFace)
		{
			// 최단 경로에 교차 정보를 추가하고 종료한다.
			CutInfo.push_back(std::make_pair(e, t));
			break;
		}

		// 현재 교차점이 정점이라면
		if (EQ(t, 0.0f, 1.0e-5) || EQ(t, 1.0f, 1.0e-5))
		{
			// 현재 교차점을 구하여
			EgVertex *vk = EQ(t, 0.0f, 1.0e-5) ? SV(e) : EV(e);
			EgEdge *ei = NULL;
			for (EgEdge *e : vk->m_pEdges)
			{
				// 소스 정점을 포함한 삼각형에 현재 교차점이 포함된다면
				if (e->m_pFace == pSrcFace)
				{
					ei = e;
					break;
				}
			}
			if (ei != NULL)
			{
				// 현재 교차점을 추가하고 종료한다.
				CutInfo.push_back(std::make_pair(ei, 0.0f));
				break;
			}

			// 1-링 경계 에지를 검사하여 최단 경로가 통과할 다음 에지를 찾는다.
			EgEdge *e_star = NULL;
			float t_star, min_dist = INFINITY;
			for (EgEdge *ei : vk->m_pEdges)
			{
				// 검사 에지가 경계 에지인 경우 생략한다(예외 처리).
				EgEdge *ej = ei->m_pNext;
				if (ej->IsBndry())
					continue;

				// 최단 경로가 통과할 다음 에지를 찾는다.
				float tmp, d = get_geodesic_dist(POS(vk), ej, &tmp, Distances);
				if (d < min_dist || (EQ(d, min_dist, 1.0e-5) && tmp < t_star))
				{
					min_dist = d;
					t_star = tmp;
					e_star = ej;
				}
			}

			// 최단 경로에 교차점을 추가하고, 다음 에지를 검사한다(*).
			CutInfo.push_back(std::make_pair(e_star->m_pNext->m_pNext, 0.0f));
			t_star = EQ(t_star, 0.0f, 1.0e-5) ? 0.0f : t_star;
			t_star = EQ(t_star, 1.0f, 1.0e-5) ? 1.0f : t_star;
			e = e_star->m_pMate;
			t = 1.0f - t_star;
			continue;
		}
		else // 현재 교차점이 에지의 내부점이라면
		{
			// 검사할 두 에지 e1, e2를 구하고
			EgEdge *e1 = e->m_pNext;
			EgEdge *e2 = e->m_pNext->m_pNext;

			// 최단 경로에 교차 정보를 추가하고(*)
			CutInfo.push_back(std::make_pair(e, t));

			// 검사할 에지가 모두 경계 에지라면(예외 처리)
			if (e1->IsBndry() && e2->IsBndry())
			{
				// 마지막 교차 정보를 제거하고
				printf("Warning: two edges are boundaries !!!\n");
				CutInfo.pop_back();

				// 현재 에지의 양끝 정점 중 소스 정점과 가까운 정점에서 다시 최단 경로를 조사한다.
				if (Distances[SV(e)->m_Idx] < Distances[EV(e)->m_Idx])
					t = 0.0f;
				else
				{
					e = e1;
					t = 0.0f;
				}
				continue;
			}

			// 두 에지를 통과하는 거리를 계산한다.
			EgPos pk = ::lerp(POS(SV(e)), POS(EV(e)), t);
			float t1, t2;

			// e2만 경계 에지인 경우
			if (!e1->IsBndry() && e2->IsBndry())
			{
				get_geodesic_dist(pk, e1, &t1, Distances);
				t1 = EQ(t1, 0.0f, 1.0e-6) ? 0.0f : t1;
				t1 = EQ(t1, 1.0f, 1.0e-6) ? 1.0f : t1;
				e = e1->m_pMate;
				t = 1.0f - t1;
				continue;
			}
			// e1만 경계 에지인 경우
			else if (e1->IsBndry() && !e2->IsBndry())
			{
				get_geodesic_dist(pk, e2, &t2, Distances);
				t2 = EQ(t2, 0.0f, 1.0e-5) ? 0.0f : t2;
				t2 = EQ(t2, 1.0f, 1.0e-5) ? 1.0f : t2;
				e = e2->m_pMate;
				t = 1.0f - t2;
				continue;
			}
			else // 모두 경계 에지가 아니라면
			{
				// 짧은 경로의 에지를 선택한다.	
				float d1 = get_geodesic_dist(pk, e1, &t1, Distances);
				float d2 = get_geodesic_dist(pk, e2, &t2, Distances);
				t1 = EQ(t1, 0.0f, 1.0e-5) ? 0.0f : t1;
				t1 = EQ(t1, 1.0f, 1.0e-5) ? 1.0f : t1;
				t2 = EQ(t2, 0.0f, 1.0e-5) ? 0.0f : t2;
				t2 = EQ(t2, 1.0f, 1.0e-5) ? 1.0f : t2;
				if (d1 < d2)
				{
					e = e1->m_pMate;
					t = 1.0f - t1;
				}
				else
				{
					e = e2->m_pMate;
					t = 1.0f - t2;
				}
				continue;
			}
		}
	}

	// 절단점 리스트를 완성한다.
	Path.clear();
	Path.push_back(E->Pos());
	for (std::pair<EgEdge *, float> info : CutInfo)
		Path.push_back(::lerp(POS(SV(info.first)), POS(EV(info.first)), info.second));
	Path.push_back(S->Pos());

	// 최단 거리를 계산하고
	double MinDist = 0.0;
	for (int i = 0; i < (int)Path.size() - 1; ++i)
		MinDist += dist(Path[i], Path[i + 1]);

	// 최단 길이와 계산 시간을 출력한 후
#ifdef _DEBUG
	printf("Shortest distances = %lf\n", MinDist);
	printf("Elapsed time for back-tracing = %u ms\n\n", clock() - st);
#endif

	// 최단 거리를 반환한다.
	return MinDist;
}

/*!
*	\brief	두 점을 연결하는 평면 절단 경로와 거리를 계산한다.
*	\note	마지막 수정일: 2020-06-24
*
*	\param	S[in]			시작점에 대한 포인터
*	\param	E[in]			끝점에 대한 포인터
*	\param	Pln[in]			절단 평면에 대한 포인터(법선 방향에 따라 시작점에서 절단 방향이 정해진다).
*	\param	Path[out]		절단 경로를 구성하는 에지 교차점이 저장된다.
*	\param	CutInfo[out]	절단 평면을 아래서 위로 통과하는 에지의 절단 정보가 저장된다.
*
*	\return 경로의 길이를 반환한다(경로가 끊어지거나 에러가 발생하면 -1.0을 반환한다).
*/
double get_planar_path(EgFacePt *S, EgFacePt *E, EgPlane *Pln, std::vector<EgPos> &Path, std::vector<std::pair<EgEdge*, float>> &CutInfo)
{
	// 수행 시간을 측정한다.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// 결과를 초기화 한다.
	Path.clear();
	CutInfo.clear();

	// 시작점과 끝점이 동일 삼각형 내부점이라면 두 점 사이의 거리를 반환한다.
	if (S->m_pFace == E->m_pFace)
	{
		Path.push_back(S->Pos());
		Path.push_back(E->Pos());
		return ::dist(Path[0], Path[1]);
	}

	// 절단 평면과 교차하는 시작 에지를 찾는다.
	float t;
	EgEdge *StEdge = NULL;

	// 시작점이 정점이라면
	if (S->IsVertex())
	{
		// 1-링 경계 에지를 조사하여 시작 에지를 구하고, 시작점의 정보를 수정한다. 
		EgVertex *v = S->GetClosestVert();
		for (EgEdge *e : v->m_pEdges)
		{
			if (intersect_plane_edge(Pln, e->m_pNext, &t))
			{
				StEdge = e->m_pNext;
				S->m_pFace = e->m_pFace;
				for (int i = 0; i < 3; ++i)
					S->m_Coords[i] = (v == e->m_pFace->GetVertex(i)) ? 1.0 : 0.0;
				break;
			}
		}
	}
	else // 시작점이 삼각형 내부점이라면
	{
		// 세 에지를 검사하여 시작 에지를 구한다.
		for (int i = 0; i < 3; ++i)
		{
			if (intersect_plane_edge(Pln, S->m_pFace->GetEdge(i), &t))
			{
				StEdge = S->m_pFace->GetEdge(i);
				break;
			}
		}
	}

	// 절단 평면과 교차하는 마지막 에지를 찾는다.
	EgEdge *EdEdge = NULL;
	EgPlane rPln = Pln->GetReversePlane();

	// 끝점이 정점이라면
	if (E->IsVertex())
	{
		// 1-링 경계 에지를 조사하여 끝점의 정보를 수정한다. 
		EgVertex *v = E->GetClosestVert();
		for (EgEdge *e : v->m_pEdges)
		{
			if (intersect_plane_edge(&rPln, e->m_pNext, &t))
			{
				EdEdge = e->m_pNext;
				E->m_pFace = e->m_pFace;
				for (int i = 0; i < 3; ++i)
					E->m_Coords[i] = (v == e->m_pFace->GetVertex(i)) ? 1.0 : 0.0;
				break;
			}
		}
	}
	else // 끝점이 삼각형 내부점이라면
	{
		// 세 에지를 검사하여 종료 에지를 구한다.
		for (int i = 0; i < 3; ++i)
		{
			if (intersect_plane_edge(&rPln, E->m_pFace->GetEdge(i), &t))
			{
				EdEdge = E->m_pFace->GetEdge(i);
				break;
			}
		}
	}

	// 시작 또는 끝 에지를 못 찾았다면
	if (StEdge == NULL) // 미확인 에러
	{
		printf("[Error]: 시작 에지를 못 찾음...\n");
		return -1.0;
	}
	if (EdEdge == NULL) // 미확인 에러
	{
		printf("[Error]: 끝 에지를 못 찾음...\n");
		return -1.0;
	}

	// 첫 번째 절단 정보를 추가한다.
	intersect_plane_edge(Pln, StEdge, &t);
	CutInfo.push_back(std::make_pair(StEdge, t));

	// 다음 절단 정보를 찾는다.
	EgEdge *e = StEdge->m_pMate;
	t = 1.0f - t;
	while (true)
	{
		// 비정상 종료: 경로가 끊겼다면 -1을 반환한다.
		if (e == NULL)
		{
			CutInfo.clear();
			//printf("[Error]: 평면 절단 경로에 홀 존재...\n");
			return -1.0;
		}

		// 비정상 종료: 시작점으로 돌아왔다면 -1을 반환한다.
		if (e->m_pFace == S->m_pFace)
		{
			CutInfo.clear();
			//printf("[Error]: 평면 절단 경로가 떨어져 있음...\n");
			return -1.0;
		}

		// 정상 종료: 마지막 에지에 도달했다면 종료한다.
		if (e == EdEdge)
			break;

		// 현재 교차점이 에지의 끝점이라면
		if (EQ(t, 1.0f, 1.0e-6))
		{
			// 정상 종료: 끝점이 포함된 삼각형에 도달했다면 종료한다.
			EgVertex *vk = EV(e);
			if (vk == SV(EdEdge) || vk == EV(EdEdge))
				break;

			for (EgEdge *ei : vk->m_pEdges)
			{
				// 비정상 종료: 시작점으로 돌아왔다면 -1.0을 반환한다.
				if (ei->m_pFace == S->m_pFace)
				{
					CutInfo.clear();
					//printf("[Error]: 평면 절단 경로가 떨어져 있음...\n");
					return -1.0;
				}
			}

			// 1-링 경계 에지를 검사하여 절단 경로가 통과할 다음 에지를 찾는다.
			bool success = false;
			for (EgEdge *ei : vk->m_pEdges)
			{
				// 검사 에지가 경계 에지인 경우 생략한다.
				if (ei->m_pNext->IsBndry())
					continue;

				// 평면 경로가 통과할 다음 에지를 찾는다.
				if (intersect_plane_edge(Pln, ei->m_pNext, &t))
				{
					CutInfo.push_back(std::make_pair(ei->m_pNext, t));
					e = ei->m_pNext->m_pMate;
					t = 1.0f - t;
					success = true;
					break;
				}
			}

			// 경로가 끊겼다면 -1을 반환한다.
			if (!success)
			{
				printf("[Error]: 알려지지 않은 에러...\n");
				return -1.0;
			}
			continue;
		}
		else // 교차점이 에지 위의 점이라면
		{
			// 다른 두 에지를 조사하여 절단 경로를 추가한다.
			bool success = false;
			for (int i = 0; i < 2; ++i)
			{
				e = e->m_pNext;
				if (intersect_plane_edge(Pln, e, &t))
				{
					CutInfo.push_back(std::make_pair(e, t));
					e = e->m_pMate;
					t = 1.0f - t;
					success = true;
					break;
				}
			}

			// 경로가 끊겼다면 -1을 반환한다.
			if (!success)
			{
				printf("[Error]: 알려지지 않은 에러...\n");
				return -1.0;
			}
			continue;
		}
	}

	// 절단 정보로부터 절단 경로를 구하고
	Path.push_back(S->Pos());
	for (std::pair<EgEdge *, float> &info : CutInfo)
		Path.push_back(::lerp(POS(SV(info.first)), POS(EV(info.first)), info.second));
	Path.push_back(E->Pos());

	// 길이를 계산하여 반환한다.
	double MinDist = 0.0;
	for (int i = 0; i < NUM(Path) - 1; ++i)
		MinDist += dist(Path[i], Path[i + 1]);
	return MinDist;
}

/*!
*	\brief	평면과 에지의 교차 정보를 구하는 함수
*	\note	마지막 수정일: 2020-06-29
*	\note	중복 교차를 피하기 위해 [시작점, 끝점) 형태의 vertex spliting을 사용
*
*	\param	Pln[in]		평면에 대한 포인터
*	\param	e[in]		조사할 에지 e=(P, Q)에 대한 포인터
*	\param	t[out]		에지가 평면을 아래서 위로 교차할 경우, 교차점의 파라미터 값(0.0 <= t < 1.0)이 저장된다.
*
*	\return	에지가 평면을 아래서 위로 교차할 경우 true, 그렇지 않으면 false를 반환한다.
*/
bool intersect_plane_edge(EgPlane *Pln, EgEdge *e, float *t)
{
	// 에지의 시작점(P)과 끝점(Q)을 구한다.
	EgPos P = POS(SV(e));
	EgPos Q = POS(EV(e));

	// P가 평면 상에 있고, Q가 평면 위쪽에 있는 경우
	if (Pln->IsOnPlane(P) && Pln->IsAbovePlane(Q))
	{
		if (t != NULL)
			*t = 0.0f;
		return true;
	}
	else if (Pln->IsBelowPlane(P) && Pln->IsAbovePlane(Q)) // P가 평면 아래쪽에 있고, Q가 평면 위쪽에 있는 경우
	{
		double denominator = Pln->N() * (Q - P);
		if (t != NULL)
			*t = (float)(-(Pln->Eval(P)) / denominator);
		return true;
	}
	else if (Pln->IsOnPlane(P) && Pln->IsOnPlane(Q)) // 에지가 평면에 포함되어 있는 경우
	{
		if (t != NULL)
			*t = -1.0f;
		printf("[Warning]: 절단 평면 위에 에지 놓임...\n");
		return false;
	}
	else // 이외의 경우는 false를 반환한다.
	{
		if (t != NULL)
			*t = -1.0f;
		return false;
	}
}

/*!	\brief	평면 절단을 통해 둘레 경로와 길이를 계산하는 함수
*	\note	마지막 수정일: 2020-06-22
*
*	\param	S[in]			절단 경로의 시작점에 대한 포인터
*	\param	Pln[in]			절단 평면에 대한 포인터
*	\param	Path[out]		경로를 구성하는 교차점의 위치가 저장된다.
*	\param	CutInfo[out]	절단 평면을 아래서 위로 통과하는 에지의 절단 정보가 저장된다.
*
*	\return 경로가 존재하면 둘레의 길이를 없다면 -1.0을 반환한다.
*/
double get_planar_path_closed(EgFacePt *S, EgPlane *Pln, std::vector<EgPos> &Path, std::vector<std::pair<EgEdge*, float>> &CutInfo)
{
	// 수행 시간을 측정한다.
	clock_t st = clock();
	Path.clear();
	CutInfo.clear();

	// 절단 평면과 교차하는 시작 에지를 찾는다.
	float t;
	EgEdge *StEdge = NULL;

	// 시작점이 정점이라면
	if (S->IsVertex())
	{
		// 1-링 주변 에지를 조사하여 시작 에지를 구하고, 시작점의 정보를 수정한다. 
		EgVertex *v = S->GetClosestVert();
		for (EgEdge *e : v->m_pEdges)
		{
			if (intersect_plane_edge(Pln, e->m_pNext, &t))
			{
				StEdge = e->m_pNext;
				S->m_pFace = e->m_pFace;
				for (int i = 0; i < 3; ++i)
					S->m_Coords[i] = (v == e->m_pFace->GetVertex(i)) ? 1.0 : 0.0;
				break;
			}
		}
	}
	else // 시작점이 삼각형 내부점이라면
	{
		// 세 에지를 검사하여 시작 에지를 구한다.
		for (int i = 0; i < 3; ++i)
		{
			if (intersect_plane_edge(Pln, S->m_pFace->GetEdge(i), &t))
			{
				StEdge = S->m_pFace->GetEdge(i);
				break;
			}
		}
	}

	// 시작 에지를 못 찾았다면
	if (StEdge == NULL) // 미확인 에러
	{
		printf("[Error]: 시작 에지를 못 찾음...\n");
		return -1.0;
	}

	// 첫 번째 절단 정보를 추가하고, 다음 절단 정보를 찾는다.
	intersect_plane_edge(Pln, StEdge, &t);
	CutInfo.push_back(std::make_pair(StEdge, t));
	int result = find_next_planar_path_closed(S, StEdge->m_pMate, 1.0f - t, Pln, CutInfo);

	// 문제가 생긴 경우 에러 메시지와 -1.0을 반환한다.
	switch (result)
	{
	case -1:
		CutInfo.clear();
		printf("[Error]: 미확인 에러...\n");
		return -1.0;

	case 0:
		CutInfo.clear();
		printf("[Error]: 평면 절단에 홀 존재...\n");
		return -1.0;
	}

	// 절단 정보로 절단 경로를 구하고.
	Path.push_back(S->Pos());
	for (std::pair<EgEdge *, float> info : CutInfo)
		Path.push_back(::lerp(POS(SV(info.first)), POS(EV(info.first)), info.second));
	Path.push_back(S->Pos());

	// 길이를 계산하여 반환한다.
	double MinDist = 0.0;
	for (int i = 0; i < NUM(Path) - 1; ++i)
		MinDist += dist(Path[i], Path[i + 1]);
	return MinDist;
}

/*!
*	\brief	평면 절단에 의한 둘레 경로를 구하는 함수
*	\note	마지막 수정일: 2020-06-22
*
*	\param	S[in]			경로의 시작점에 대한 포인터
*	\param	e[in]			현재 교차점이 놓인 에지 대한 포인터
*	\param	t[in]			에지 상에서 현재 교차점의 위치 파라미터(0.0 <= t <= 1.0)
*	\param	Pln[in]			경로가 놓일 평면에 대한 포인터
*	\param	CutInfo[out]	둘레 경로의 교차 정보가 저장된다.
*
*	\return	결과 값(-1: 미확인 에러, 0: 홀에 의해 경로가 끊김, 1: 다음 경로를 찾음)을 반환한다.
*/
static int find_next_planar_path_closed(EgFacePt *S, EgEdge *e, float t, EgPlane *Pln, std::vector<std::pair<EgEdge *, float>> &CutInfo)
{
	// 종료조건(실패): 경로가 끊겼다면 0을 반환한다.
	if (e == NULL)
		return 0;

	// 종료조건(성공): 시작점으로 돌아왔다면 1을 반환한다.
	if (e->m_pFace == S->m_pFace)
		return 1;

	// 현재 교차점이 에지의 끝점이라면
	if (EQ(t, 1.0f, 1.0e-6))
	{
		// 현재 교차점을 구하여, 마지막 호출 여부를 검사한다.
		EgVertex *vk = EV(e);
		for (EgEdge *ei : vk->m_pEdges)
		{
			// 종료조건(성공): 시작점으로 돌아왔다면 1을 반환한다.
			if (ei->m_pFace == S->m_pFace)
				return 1;
		}

		// 1-링 경계 에지를 검사하여 절단 경로가 통과할 다음 에지를 찾는다.
		for (EgEdge *ei : vk->m_pEdges)
		{
			// 검사 에지가 경계 에지인 경우 생략한다.
			if (ei->m_pNext->IsBndry())
				continue;

			// 절단 경로가 통과할 다음 에지를 찾는다.
			if (intersect_plane_edge(Pln, ei->m_pNext, &t))
			{
				CutInfo.push_back(std::make_pair(ei->m_pNext, t));
				return find_next_planar_path_closed(S, ei->m_pNext->m_pMate, 1.0f - t, Pln, CutInfo);
			}
		}

		// 종료조건(실패): 경로가 끊겼다면 -1을 반환한다.
		return -1;
	}
	else
	{
		// 다른 두 에지를 조사하여 절단 경로를 추가하고, 다음 경로를 찾는다.
		for (int i = 0; i < 2; ++i)
		{
			e = e->m_pNext;
			if (intersect_plane_edge(Pln, e, &t))
			{
				CutInfo.push_back(std::make_pair(e, t));
				return find_next_planar_path_closed(S, e->m_pMate, 1.0f - t, Pln, CutInfo);
			}
		}
	}

	// 미확인 에러인 경우 -1를 반환한다.
	return -1;
}




