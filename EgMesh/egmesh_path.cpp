#include "..\usr\include\EgMesh\egmesh.h"
#include <cstring>
#include <vector>
#include <map>
#include <direct.h>

// ���� ��� ���ϴ� �Լ���
static double propagate_geodesic_dist_field(GeodesicHeap &MinHeap, std::vector<double> &Distances, double BndryDistance);
static double propagate_geodesic_dist_field(GeodesicHeap &MinHeap, std::vector<double> &Distances, EgVertex *T);
static double propagate_geodesic_dist_field(GeodesicHeap &MinHeap, std::vector<double> &Distances, EgFacePt *T);

static float get_geodesic_dist(EgVertex *vj, EgEdge *e, std::vector<double> &Distances);
static float get_geodesic_dist(EgPos pk, EgEdge *e, float *t, std::vector<double> &Distances);

// ��� ���� ��� ���ϴ� �Լ���
static int find_next_planar_path_closed(EgFacePt *P, EgEdge *e, float t, EgPlane *Pln, std::vector<std::pair<EgEdge *, float>> &CutInfo);

/*!
*	\brief	������
*	\note	������ ������: 2020-07-31
*
*	\param	pVert[in]	������ ���� ������
*	\param	Dist[in]	�ҽ� ���������� �Ÿ�
*/
EgGeodesic::EgGeodesic(EgVertex *pVert, double Dist)
{
	m_pVert = pVert;
	m_Distance = Dist;
}

/*!
*	\brief	���� ������
*	\note	������ ������: 2020-07-31
*
*	\param	cpy[in]		����� ��ü
*/
EgGeodesic::EgGeodesic(const EgGeodesic &cpy)
{
	m_pVert = cpy.m_pVert;
	m_Distance = cpy.m_Distance;
}

/*!
*	\brief	�Ҹ���
*	\note	������ ������: 2020-07-31
*/
EgGeodesic::~EgGeodesic()
{
}

/*!
*	\brief	���� ������
*	\note	������ ������: 2020-07-31
*
*	\param	rhs[in] ������ ��ü
*
*	\return ���Ե� �ڽ��� ��ȯ�Ѵ�.
*/
EgGeodesic &EgGeodesic::operator =(const EgGeodesic &rhs)
{
	m_pVert = rhs.m_pVert;
	m_Distance = rhs.m_Distance;
	return *this;
}

/*!
*	\brief	�� ������
*	\note	������ ������: 2020-07-31
*
*	\param	lhs[in]		���� ��ü
*	\param	rhs[in]		������ ��ü
*
*	\return ���� ��ü�� ������ ��ü���� ũ�� true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool operator >(const EgGeodesic &lhs, const EgGeodesic &rhs)
{
	return (lhs.m_Distance > rhs.m_Distance);
}

/*!
*	\brief	�ҽ� �������� ��� �Ÿ� �̳��� �ﰢ��(���� �ﰢ�� ����)�� �� ���������� �ּ� �Ÿ��� ����Ѵ�.
*	\note	��� �ܺ� �ﰢ���� ���������� �Ÿ��� INFINITY�� �����ȴ�.
*	\note	������ ������: 2020-08-31
*
*	\param	pMesh[in]			��� �޽�
*	\param	S[in]				�ҽ� ������ ���� ������
*	\param	Distances[out]		�ҽ� �������� i��° ���������� �Ÿ��� Distances[i]�� ����ȴ�.
*	\param	BndryDistance[in]	�˻��� ��� �Ÿ�(������ ��� ��ü �޽��� ����Ѵ�)
*
*	\return ���� ������ �ִ� �Ÿ�(>= BndryDistance)�� ��ȯ�Ѵ�.
*/
double get_geodesic_dist_field(EgMesh *pMesh, EgVertex *S, std::vector<double> &Distances, double BndryDistance)
{
	// ���� �ð��� �����Ѵ�.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// �ҽ��� ������ �� ���������� �Ÿ��� ���Ѵ�� �ʱ�ȭ �Ѵ�.
	Distances.assign(pMesh->GetNumVert(), INFINITY);
	Distances[S->m_Idx] = 0.0;

	// �ҽ� ������ ���� �Ÿ� ������ �ּ����� �߰��Ѵ�.
	GeodesicHeap MinHeap;
	MinHeap.push(EgGeodesic(S, 0.0));

	// ���� �Ÿ����� �����Ѵ�.
	double MaxDistance = propagate_geodesic_dist_field(MinHeap, Distances, BndryDistance);

	// ��� ����� ����Ѵ�.
#ifdef _DEBUG
	printf("Input boundary distance = %lf\n", BndryDistance);
	printf("Maximum distance = %lf\n", MaxDistance);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// ���� ������ �ִ� �Ÿ�(>= BndryDistance)�� ��ȯ�Ѵ�.
	return MaxDistance;
}

/*!
*	\brief	�ּ����� �����鿡 ���Ͽ� ���� �Ÿ����� �����Ѵ�.
*	\note	������ ������: 2020-08-31
*
*	\param	MinHeap[in]			�ʱ�ȭ�� �ּ���
*	\param	Distances[out]		�ҽ� �������� i��° ���������� �Ÿ��� Distances[i]�� ����ȴ�.
*	\param	BndryDistance[in]	�˻��� ��� �Ÿ�(������ ��� ��ü �޽��� ����Ѵ�)
*
*	\return ���� ������ �ִ� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
*/
static double propagate_geodesic_dist_field(GeodesicHeap &MinHeap, std::vector<double> &Distances, double BndryDistance)
{
	// �ּ����� ������ ��� ó���� ������
	double MaxDistance = 0.0;
	while (!MinHeap.empty())
	{
		// �ҽ� �������� ���� ��� ������ ������ ������
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// �̹� ���ŵ� �����Ͷ�� ��ŵ�Ѵ�.
		int vidx = info.m_pVert->m_Idx;
		if (info.m_Distance > Distances[vidx])
			continue;

		// 1�� ������ �ִ� �Ÿ��� �����Ѵ�.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// ���� vj������ �Ÿ��� ����Ͽ�
			EgVertex *vj = EV(e);
			float d1 = get_geodesic_dist(vj, e->m_pNext->m_pNext, Distances);
			float d2 = (e->m_pMate != NULL) ? get_geodesic_dist(vj, e->m_pMate->m_pNext, Distances) : INFINITY;
			float dj = MIN(d1, d2);

			// ���ο� �ִ� �Ÿ��� ã�� ���
			if (dj < Distances[vj->m_Idx])
			{
				// ������ �ִ� �Ÿ��� �����ϰ�
				Distances[vj->m_Idx] = dj;

				if (BndryDistance < 0.0)	// ��� ������ �����Ѵٸ� ������ ���� �߰��ϰ�
					MinHeap.push(EgGeodesic(vj, dj));
				else if (dj < BndryDistance) // �ƴ϶�� ��� �Ÿ��� ���Ե� ������ ���� �߰��Ѵ�.
					MinHeap.push(EgGeodesic(vj, dj));

				// �߰ߵ� �ִ� �Ÿ��� �����Ѵ�.
				MaxDistance = MAX(MaxDistance, dj);
			}

			// ���� ó��: ��� ������ ��쵵 ó���Ѵ�.
			if (e->m_pNext->m_pNext->m_pMate == NULL)
			{
				EgVertex *vj = e->m_pNext->m_pNext->m_pVert;
				float dj = get_geodesic_dist(vj, e, Distances);

				// ���ο� �ִ� �Ÿ��� ã�� ���
				if (dj < Distances[vj->m_Idx])
				{
					// ������ �ִ� �Ÿ��� �����ϰ�
					Distances[vj->m_Idx] = dj;

					if (BndryDistance < 0.0)	// ��� ������ �����Ѵٸ� ������ ���� �߰��ϰ�
						MinHeap.push(EgGeodesic(vj, dj));
					else if (dj < BndryDistance) // �ƴ϶�� ��� �Ÿ��� ���Ե� ������ ���� �߰��Ѵ�.
						MinHeap.push(EgGeodesic(vj, dj));

					// �߰ߵ� �ִ� �Ÿ��� �����Ѵ�.
					MaxDistance = MAX(MaxDistance, dj);
				}
			}
		}
	}

	// �ִ� �Ÿ��� ��ȯ�Ѵ�.
	return MaxDistance;
}

/*!
*	\brief	����(vj)���� �ҽ� �������� �ִ� �Ÿ� dj�� ����ϴ� �Լ�
*	\note	������ ������: 2020-07-31
*
*	\param	vj[in]	��� ����
*	\param	e[in]	���� vj�� ���ֺ��� ���� e = (vk, vi)
*
*	\return	�ִ� �Ÿ��� ��ȯ�Ѵ�.
*/
static float get_geodesic_dist(EgVertex *vj, EgEdge *e, std::vector<double> &Distances)
{
	// �ﰢ���� �� ������ ������ ���̸� ����ϰ�
	EgVertex *vi = EV(e);
	EgVertex *vk = SV(e);
	float dij_sq = (float)dist_sq(vi->m_Pos, vj->m_Pos);
	float djk_sq = (float)dist_sq(vj->m_Pos, vk->m_Pos);
	float dki_sq = (float)dist_sq(vk->m_Pos, vi->m_Pos);
	float dij = SQRT(dij_sq);
	float djk = SQRT(djk_sq);
	float dki = SQRT(dki_sq);

	// �ҽ� �������� vj������ �Ÿ� dj�� �ּ� ���� �Ÿ��� �ʱ�ȭ �Ѵ�.
	float di = (float)Distances[vi->m_Idx];
	float dk = (float)Distances[vk->m_Idx];
	float dj = (di + dij < dk + djk) ? (di + dij) : (dk + djk);

	// ������ �ҽ� ���� vs�� �����Ѵٸ�
	if (ABS(dk - di) < dki && dki < dk + di)
	{
		// 2���� ��鿡 ������ vs(x0, y0), vk(0, 0), vi(-dki, 0), vj(x1, y1)�� ��ǥ�� ���ϰ�,
		float x0 = (SQR(di) - SQR(dk) - dki_sq) / (2 * dki);
		float y0 = SQRT(SQR(dk) - SQR(x0));
		float x1 = (dij_sq - djk_sq - dki_sq) / (2 * dki);
		float y1 = -SQRT(djk_sq - SQR(x1));

		// ���� (vs, vk)�� (vj, vi)�� �������� x��ǥ�� ���Ͽ�
		float x = (x0 * y1 - x1 * y0) / (y1 - y0);

		// �������� ���� (vi, vk)���̿� �����Ѵٸ� �Ÿ� dj�� �����Ѵ�.
		if (x > -dki && x < 0)
			dj = SQRT(SQR(x1 - x0) + SQR(y1 - y0));
	}

	// ���� �Ÿ��� ��ȯ�Ѵ�.
	return dj;
}

/*!
*	\brief	�ҽ� �������� Ÿ�� ���������� �Ÿ� ���� �������� ���� �Ÿ����� ����Ѵ�.
*	\note	Ÿ�� �������� �Ÿ����� �� �������� �Ÿ��� INFINITY�� �����ȴ�.
*	\note	������ ������: 2020-08-31
*
*	\param	pMesh[in]		��� �޽�
*	\param	S[in]			�ҽ� ������ ���� ������
*	\param	T[in]			Ÿ�� ������ ���� ������
*	\param	Distances[out]	�ҽ� �������� i��° ���������� �Ÿ��� Distances[i]�� ����ȴ�.
*
*	\return ���� ������ �ִ� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
*/
double get_geodesic_dist_field(EgMesh *pMesh, EgVertex *S, EgVertex *T, std::vector<double> &Distances)
{
	// ���� �ð��� �����Ѵ�.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// �ҽ� �������� �� ���������� �Ÿ��� ���Ѵ�� �ʱ�ȭ �Ѵ�.
	Distances.assign(pMesh->GetNumVert(), INFINITY);
	Distances[S->m_Idx] = 0.0;

	// �ҽ� ������ ������ �ּ����� �߰��Ѵ�.
	GeodesicHeap MinHeap;
	MinHeap.push(EgGeodesic(S, 0.0));

	// Ÿ�� ������ ���Ե� ������ ���� �Ÿ����� ����Ѵ�.
	double MaxDistance = propagate_geodesic_dist_field(MinHeap, Distances, T);

	// ��� ����� ����Ѵ�.
#ifdef _DEBUG
	printf("Maximum distance = %lf\n", MaxDistance);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// ���� ������ �ִ� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
	return MaxDistance;
}

/*!
*	\brief	Ÿ�� ������ ���Ե� ������ ���� �Ÿ����� �����Ѵ�.
*	\note	������ ������: 2020-07-31
*
*	\param	MinHeap[in]		�ʱ�ȭ�� �ּ���
*	\param	Distances[out]	�ҽ� �������� i��° ���������� �Ÿ��� Distances[i]�� ����ȴ�.
*	\param	T[in]			Ÿ�� ������ ���� ������
*
*	\return ���� ������ �ִ� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
*/
static double propagate_geodesic_dist_field(GeodesicHeap &MinHeap, std::vector<double> &Distances, EgVertex *T)
{
	// �ּ����� ������ ��� ó���� ������
	double MaxDistance = 0.0;
	while (!MinHeap.empty())
	{
		// �ҽ� �������� ���� ��� ������ ������ ������
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// �̹� ���ŵ� �����Ͷ�� ��ŵ�Ѵ�.
		int vidx = info.m_pVert->m_Idx;
		if (info.m_Distance > Distances[vidx])
			continue;

		// ������ 1�� ������ �ִ� �Ÿ��� �����Ѵ�.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// ���� vj������ �Ÿ��� ����Ͽ�
			EgVertex *vj = EV(e);
			float d1 = get_geodesic_dist(vj, e->m_pNext->m_pNext, Distances);
			float d2 = (e->m_pMate != NULL) ? get_geodesic_dist(vj, e->m_pMate->m_pNext, Distances) : INFINITY;
			float dj = MIN(d1, d2);

			// ���ο� �ִ� �Ÿ��� ã�� ���
			if (dj < Distances[vj->m_Idx])
			{
				// ������ �ִ� �Ÿ��� �����ϰ�, ���ο� ������ ���� �߰��Ѵ�.
				Distances[vj->m_Idx] = dj;
				MinHeap.push(EgGeodesic(vj, dj));

				// �߰ߵ� �ִ� �Ÿ��� �����Ѵ�.
				MaxDistance = MAX(MaxDistance, dj);
			}

			// ���� ó��: ��� ������ ��쵵 ó���Ѵ�.
			if (e->m_pNext->m_pNext->m_pMate == NULL)
			{
				EgVertex *vj = e->m_pNext->m_pNext->m_pVert;
				float dj = get_geodesic_dist(vj, e, Distances);

				// ���ο� �ִ� �Ÿ��� ã�� ���
				if (dj < Distances[vj->m_Idx])
				{
					// ������ �ִ� �Ÿ��� �����ϰ�, ���ο� ������ ���� �߰��Ѵ�.
					Distances[vj->m_Idx] = dj;
					MinHeap.push(EgGeodesic(vj, dj));

					// �߰ߵ� �ִ� �Ÿ��� �����Ѵ�.
					MaxDistance = MAX(MaxDistance, dj);
				}
			}
		}

		// Ÿ�� ������ ó���Ǿ��ٸ� �ߴ��Ѵ�.
		if (vi == T)
			break;
	}

	// �ִ� �Ÿ��� ��ȯ�Ѵ�.
	return MaxDistance;
}

/*!
*	\brief	�ﰢ�� ���������� ��� �Ÿ� �̳��� �� ���������� �ּ� �Ÿ��� ����Ѵ�.
*	\note	��� �ܺ��� �ﰢ�� ����������� �Ÿ��� INFINITY�� �����ȴ�.
*	\note	������ ������: 2020-08-31
*
*	\param	pMesh[in]			��� �޽�
*	\param	S[in]				�ﰢ�� �������� ���� ������
*	\param	Distances[out]		i��° ���������� �Ÿ��� Distances[i]�� ����ȴ�.
*	\param	BndryDistance[in]	�˻��� ��� �Ÿ�(������ ��� ��ü �޽��� ����Ѵ�)
*
*	\return ���� ������ �ִ� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
*/
double get_geodesic_dist_field(EgMesh *pMesh, EgFacePt *S, std::vector<double> &Distances, double BndryDistance)
{
	// ���� �ð��� �����Ѵ�.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// �ҽ� �������� �� ���������� �Ÿ��� ���Ѵ�� �ʱ�ȭ �Ѵ�.
	Distances.assign(pMesh->GetNumVert(), INFINITY);

	// ���������� �ﰢ���� �� ���������� �Ÿ��� ���Ͽ� ���� �߰��Ѵ�.
	EgPos p = S->Pos();
	GeodesicHeap MinHeap;
	for (int i = 0; i < 3; ++i)
	{
		EgVertex *v = S->m_pFace->GetVertex(i);
		double d = dist(p, v->m_Pos);
		Distances[v->m_Idx] = d;
		MinHeap.push(EgGeodesic(v, d));
	}

	// ���� �Ÿ����� �����Ѵ�.
	double MaxDistance = propagate_geodesic_dist_field(MinHeap, Distances, BndryDistance);

	// ��� ����� ����Ѵ�.
#ifdef _DEBUG
	printf("Boundary distance = %lf\n", BndryDistance);
	printf("Maximum distance = %lf\n", MaxDistance);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// ���� ������ �ִ� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
	return MaxDistance;
}

/*!
*	\brief	�ﰢ�� ���������� Ÿ�� ���������� �Ÿ� ���� �������� ���� �Ÿ����� ����ϴ� �Լ�
*	\note	Ÿ�� �������� �Ÿ����� �� �������� �Ÿ��� INFINITY�� �����ȴ�.
*	\note	������ ������: 2020-08-31
*
*	\param	pMesh[in]		��� �޽�
*	\param	S[in]			�ҽ� ���� ���� ������
*	\param	T[in]			Ÿ�� ���� ���� ������
*	\param	Distances[out]	P���� i��° ���������� �Ÿ��� Distances[i]�� ����ȴ�.
*
*	\return ���� ������ �ִ� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
*/
double get_geodesic_dist_field(EgMesh *pMesh, EgFacePt *S, EgFacePt *T, std::vector<double> &Distances)
{
	// ���� �ð��� �����Ѵ�.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// �ҽ� �������� �� ���������� �Ÿ��� ���Ѵ�� �ʱ�ȭ �Ѵ�.
	Distances.assign(pMesh->GetNumVert(), INFINITY);

	// ���������� �ﰢ���� �� ���������� �Ÿ��� ���Ͽ� ���� �߰��Ѵ�.
	EgPos p = S->Pos();
	GeodesicHeap MinHeap;
	for (int i = 0; i < 3; ++i)
	{
		EgVertex *v = S->m_pFace->GetVertex(i);
		double d = dist(p, v->m_Pos);
		Distances[v->m_Idx] = d;
		MinHeap.push(EgGeodesic(v, d));
	}

	// ���� �Ÿ����� �����Ѵ�.
	double MaxDistance = propagate_geodesic_dist_field(MinHeap, Distances, T);

	// ��� ����� ����Ѵ�.
#ifdef _DEBUG
	printf("Maximum distance = %lf\n", MaxDistance);
	printf("Elapsed time for geodesic field = %u ms\n\n", clock() - st);
#endif

	// ���� ������ �ִ� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
	return MaxDistance;
}

/*!
*	\brief	Ÿ�� ������ ���Ե� ������ ���� �Ÿ����� �����Ѵ�.
*	\note	������ ������: 2020-07-31
*
*	\param	MinHeap[in]		�ʱ�ȭ�� �ּ���
*	\param	Distances[out]	�ҽ� �������� i��° ���������� �Ÿ��� Distances[i]�� ����ȴ�.
*	\param	T[in]			Ÿ�� ���� ���� ������
*
*	\return ���� ������ �ִ� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
*/
static double propagate_geodesic_dist_field(GeodesicHeap &MinHeap, std::vector<double> &Distances, EgFacePt *T)
{
	// Ÿ�� ���� ���ѵ� �ﰢ���� �� ������ ���Ѵ�.
	EgVertex *T0 = T->m_pFace->GetVertex(0);
	EgVertex *T1 = T->m_pFace->GetVertex(1);
	EgVertex *T2 = T->m_pFace->GetVertex(2);

	// �ּ����� ������ ��� ó���� ������
	double MaxDistance = 0.0;
	while (!MinHeap.empty())
	{
		// �ҽ� �������� ���� ��� ������ ������ ������
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// �̹� ���ŵ� �����Ͷ�� ��ŵ�Ѵ�.
		int vidx = info.m_pVert->m_Idx;
		if (info.m_Distance > Distances[vidx])
			continue;

		// ������ 1�� ������ �ִ� �Ÿ��� �����Ѵ�.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// ���� vj������ �Ÿ��� ����Ͽ�
			EgVertex *vj = EV(e);
			float d1 = get_geodesic_dist(vj, e->m_pNext->m_pNext, Distances);
			float d2 = (e->m_pMate != NULL) ? get_geodesic_dist(vj, e->m_pMate->m_pNext, Distances) : INFINITY;
			float dj = MIN(d1, d2);

			// ���ο� �ִ� �Ÿ��� ã�� ���
			if (dj < Distances[vj->m_Idx])
			{
				// ������ �ִ� �Ÿ��� �����ϰ�, ���ο� ������ ���� �߰��Ѵ�.
				Distances[vj->m_Idx] = dj;
				MinHeap.push(EgGeodesic(vj, dj));

				// �߰ߵ� �ִ� �Ÿ��� �����Ѵ�.
				MaxDistance = MAX(MaxDistance, dj);
			}

			// ���� ó��: ��� ������ ��쵵 ó���Ѵ�.
			if (e->m_pNext->m_pNext->m_pMate == NULL)
			{
				EgVertex *vj = e->m_pNext->m_pNext->m_pVert;
				float dj = get_geodesic_dist(vj, e, Distances);

				// ���ο� �ִ� �Ÿ��� ã�� ���
				if (dj < Distances[vj->m_Idx])
				{
					// ������ �ִ� �Ÿ��� �����ϰ�, ���ο� ������ ���� �߰��Ѵ�.
					Distances[vj->m_Idx] = dj;
					MinHeap.push(EgGeodesic(vj, dj));

					// �߰ߵ� �ִ� �Ÿ��� �����Ѵ�.
					MaxDistance = MAX(MaxDistance, dj);
				}
			}
		}

		// Ÿ�� ���� ���ѵ� �ﰢ���� �� ������ ��� ó���Ǹ� �ߴ��Ѵ�.
		if (vi == T0)
			T0 = NULL;
		if (vi == T1)
			T1 = NULL;
		if (vi == T2)
			T2 = NULL;
		if (T0 == NULL && T1 == NULL && T2 == NULL)
			break;
	}

	// �ִ� �Ÿ��� ��ȯ�Ѵ�.
	return MaxDistance;
}

/*!
*	\brief	�ҽ� ����(�ﰢ��)�鿡�� ��� �Ÿ�(BndryDistance) �̳��� �ﰢ��(���� ����)�� �� ���������� �ּ� �Ÿ��� ����Ѵ�.
*	\note	��� �ܺ��� �ﰢ�� ����������� �Ÿ��� INFINITY�� �����ȴ�.
*	\note	������ ������: 2020-08-31
*
*	\param	pMesh[in]			��� �޽�
*	\param	SrcList[in]			�޽��� �ҽ� ����(�ﰢ��) ����Ʈ
*	\param	Distances[out]		i��° ���������� �Ÿ��� Distances[i]�� ����ȴ�.
*	\param	BndryDistance[in]	�˻��� ��� �Ÿ�(������ ��� ��ü �޽��� ����Ѵ�)
*
*	\return ���� ������ �ִ� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
*/
double get_geodesic_dist_field(EgMesh *pMesh, std::vector<EgObject *> &SrcList, std::vector<double> &Distances, double BndryDistance)
{
	// ���� �ð��� �����Ѵ�.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// ����, �ﰢ��, �ﰢ�� �������� ���õ� ��찡 �ƴ϶�� �����Ѵ�.
	std::string ClsName = SrcList[0]->GetClassName();
	if (ClsName != "EgVertex" && ClsName != "EgFace" && ClsName != "EgFacePt")
		return -1;

	// �� ���������� �Ÿ��� ���Ѵ�� �ʱ�ȭ �Ѵ�.
	Distances.assign(pMesh->GetNumVert(), INFINITY);

	// �ּ����� �����Ѵ�.
	GeodesicHeap MinHeap;

	// ���õ� ��ü�� �����̶��,
	if (ClsName == "EgVertex")
	{
		// ���õ� �� ������ �Ÿ��� 0���� �ʱ�ȭ�ϰ� ���� �߰��Ѵ�.
		for (EgObject *obj : SrcList)
		{
			EgVertex *v = (EgVertex *)obj;
			Distances[v->m_Idx] = 0.0;
			MinHeap.push(EgGeodesic(v, 0.0));
		}
	}

	// ���õ� ��ü�� �ﰢ���̶��,
	if (ClsName == "EgFace")
	{
		// ���õ� �� �ﰢ���� ������ �Ÿ��� 0���� �ʱ�ȭ �ϰ� ���� �߰��Ѵ�.
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

	// ���õ� ��ü�� �ﰢ�� ������ �̶��,
	if (ClsName == "EgFacePt")
	{
		// ������ �������� ���Ͽ�
		for (EgObject *obj : SrcList)
		{
			// ���������� �ﰢ���� �� ���������� �Ÿ��� ���Ͽ� ���� �߰��Ѵ�.
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

	// ���� �Ÿ����� �����Ѵ�.
	double MaxDistance = propagate_geodesic_dist_field(MinHeap, Distances, BndryDistance);

	// ��� ����� ����Ѵ�.
#ifdef _DEBUG
	printf("Boundary distance = %lf\n", BndryDistance);
	printf("Maximum distance = %lf\n", MaxDistance);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// ���� ������ �ִ� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
	return MaxDistance;
}

/*!
*	\brief	�ҽ� �������� ��� �Ÿ�(BndryDistance) �̳��� �ﰢ��(���� �ﰢ�� ����)�� �� �������� ������ ������ �ּ� �Ÿ��� ����Ѵ�.
*	\note	��� �ܺ� �ﰢ���� ����������� �Ÿ��� INFINITY�� �����ȴ�.
*	\note	������ ������: 2020-08-31
*
*	\param	pMesh[in]			��� �޽�
*	\param	S[in]				�ҽ� ������ ���� ������
*	\param	Distances[out]		�ҽ� �������� i��° ���������� ���� �Ÿ��� Distances[i]�� ����ȴ�.
*	\param	BndryDistance[in]	�˻��� ��� �Ÿ�(������ ��� ��ü �޽��� ����Ѵ�)
*
*	\return ���� ������ �ִ� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
*/
double get_edge_dist_field(EgMesh *pMesh, EgVertex *S, std::vector<double> &Distances, double BndryDistance)
{
	// ���� �ð��� �����Ѵ�.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// �� ���������� �Ÿ��� ���Ѵ�� �ʱ�ȭ �Ѵ�.
	Distances.assign(pMesh->GetNumVert(), INFINITY);

	// �ּ����� �����ϰ� 0���� �ʱ�ȭ�� �ҽ� ������ ������ �߰��Ѵ�.
	GeodesicHeap MinHeap;
	Distances[S->m_Idx] = 0.0;
	MinHeap.push(EgGeodesic(S, 0.0));

	// �ּ����� ������ ��� ó���� ������
	double MaxDistance = 0.0;
	while (!MinHeap.empty())
	{
		// �ҽ� �������� ���� ��� ������ ������ ������
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// �̹� ó���� ������� ��ŵ�Ѵ�.
		int vidx = info.m_pVert->m_Idx;
		if (info.m_Distance > Distances[vidx])
			continue;

		// ������ 1�� ���� vj�� �ִ� �Ÿ��� �����Ѵ�.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// Dijkstra �˰����� �����Ѵ�.
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

			// ���ο� �ִ� �Ÿ��� ã�� ���
			float dj = MIN(d1, d2);
			if (dj < Distances[vj->m_Idx])
			{
				// ������ �ִ� �Ÿ��� �����ϰ�
				Distances[vj->m_Idx] = dj;

				if (BndryDistance < 0.0)	// ��� ������ �����Ѵٸ� ������ ���� �߰��ϰ�
					MinHeap.push(EgGeodesic(vj, dj));
				else if (dj < BndryDistance) // �ƴ϶�� ��� �Ÿ��� ���Ե� ������ ���� �߰��Ѵ�.
					MinHeap.push(EgGeodesic(vj, dj));

				// �߰ߵ� �ִ� �Ÿ��� �����Ѵ�.
				MaxDistance = MAX(MaxDistance, dj);
			}
		}
	}

	// ��� ����� ����Ѵ�.
#ifdef _DEBUG
	printf("Boundary distance = %lf\n", BndryDistance);
	printf("Maximum distance = %lf\n", MaxDistance);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// ���� ������ �ִ� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
	return MaxDistance;
}

/*!
*	\brief	�ҽ� �������� ��� �Ÿ� �̳��� ���������� ���� �Ÿ��� ����ϴ� �Լ�
*
*	\param	pMesh[in]			��� �޽�
*	\param	S[in]				�ҽ� ����
*	\param	Distances[out]		�ҽ� �������� i��° ���������� ���� �Ÿ��� Distances[i]�� ����ȴ�.
*	\param	BndryDistance[in]	�˻��� ��� ���� �Ÿ�(������ ��� ��ü �޽��� ����Ѵ�)
*
*	\return ���� ������ �ִ� ���� �Ÿ�(>= ��� �Ÿ�)�� ��ȯ�Ѵ�.
*/
int get_topology_dist_field(EgMesh *pMesh, EgVertex *S, std::vector<int> &Distances, int BndryDistance)
{
	// ���� �ð��� �����Ѵ�.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// �� ���������� �Ÿ��� ���Ѵ�� �ʱ�ȭ �Ѵ�.
	Distances.assign(pMesh->GetNumVert(), INT_MAX);

	// �ּ����� �����ϰ� 0���� �ʱ�ȭ�� �ҽ� ������ ������ �߰��Ѵ�.
	GeodesicHeap MinHeap;
	Distances[S->m_Idx] = 0;
	MinHeap.push(EgGeodesic(S, 0.0));

	// �ּ����� ������ ��� ó���� ������
	int MaxDistance = 0;
	while (!MinHeap.empty())
	{
		// �ҽ� �������� ���� ��� ������ ������ ������
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// �̹� ó���� ������� ��ŵ�Ѵ�.
		int vidx = info.m_pVert->m_Idx;
		if ((int)info.m_Distance > Distances[vidx])
			continue;

		// ������ 1�� ���� vj�� �ִ� �Ÿ��� �����Ѵ�.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// Dijkstra �˰����� �����Ѵ�.
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

			// ���ο� �ִ� �Ÿ��� ã�� ���
			int dj = MIN(d1, d2);
			if (dj < Distances[vj->m_Idx])
			{
				// ������ �ִ� �Ÿ��� �����ϰ�
				Distances[vj->m_Idx] = dj;

				if (BndryDistance < 0.0)	// ��� ������ �����Ѵٸ� ������ ���� �߰��ϰ�
					MinHeap.push(EgGeodesic(vj, (double)dj));
				else if (dj < BndryDistance) // �ƴ϶�� ��� �Ÿ��� ���Ե� ������ ���� �߰��Ѵ�.
					MinHeap.push(EgGeodesic(vj, (double)dj));

				// �߰ߵ� �ִ� �Ÿ��� �����Ѵ�.
				MaxDistance = MAX(MaxDistance, dj);
			}
		}
	}

	// ��� ����� ����Ѵ�.
#ifdef _DEBUG
	printf("Boundary distance = %d\n", BndryDistance);
	printf("Maximum distance = %d\n", MaxDistance);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// ���� ������ �ִ� ���� �Ÿ��� ��ȯ�Ѵ�.
	return MaxDistance;
}

/*!
*	\brief	�� ���� ������ �ִ� ��ο� ���� ������ ���ϴ� �Լ�
*	\note	������ ������: 2020-09-01
*
*	\param	S[in]			���� ������ ���� ������
*	\param	E[in]			�� ������ ���� ������
*	\param	Distances[in]	���� ���� ������ ���� �Ÿ���
*	\param	Path[out]		�� �������� �ҽ� �������� ��θ� �����ϴ� �������� ��ġ�� ����ȴ�.
*	\param	CutInfo[out]	�� �������� �ҽ� �������� �ִ� ����� ���� ������ ����ȴ�.
*
*	\return �ִ� �Ÿ��� ��ȯ�Ѵ�.
*/
double get_shortest_path(
	EgVertex *S,
	EgVertex *E,
	std::vector<double> &Distances,
	std::vector<EgPos> &Path,
	std::vector<std::pair<EgEdge *, float>> &CutInfo)
{
	// ���� ������ �� ������ ������ �����Ѵ�.
	if (S == E)
		return 0.0;

#ifdef _DEBUG
	clock_t st = clock();
#endif

	// �������� ���� �� �������� ���� ���������� �ִ� ��θ� ã�´�.	
	CutInfo.clear();
	EgEdge *e = E->m_pEdges[0];
	float t = 0.0f;
	while (true)
	{
		// ���� ����
		if (EV(e) == S && !CutInfo.empty())
		{
			// ������ ������ �߰��ϰ� �����Ѵ�.
			CutInfo.push_back(std::make_pair(e, 1.0f));
			break;
		}

		// ���� �������� �����̶��
		if (EQ(t, 0.0f, 1.0e-5) || EQ(t, 1.0f, 1.0e-5))
		{
			// ���� �������� ���Ͽ�, ������ ȣ���� �˻��Ѵ�.
			EgVertex *vk = EQ(t, 0.0f, 1.0e-5) ? SV(e) : EV(e);
			EgEdge *ei = NULL, *ej = NULL;
			for (EgEdge *e : vk->m_pEdges)
			{
				// �˻� ������ ���Ͽ�
				ej = e->m_pNext;

				// �˻� ���� �� �ҽ� �������� ������ ������ �ִٸ�
				if (SV(ej) == S)
					ei = ej = e;

				// �˻� ���� �� �ҽ� �������� ���� ������ �ִٸ�
				if (EV(ej) == S)
				{
					ei = e;
					break;
				}
			}
			if (ei != NULL)
			{
				// ������ ������ �߰��ϰ� �����Ѵ�.
				CutInfo.push_back(std::make_pair(ei, 0.0f));
				CutInfo.push_back(std::make_pair(ej, 1.0f));
				break;
			}

			// 1-�� ��� ������ �˻��Ͽ� �ִ� ��ΰ� ����� ���� ������ ã�´�.
			EgEdge *e_star = NULL;
			float t_star, min_dist = INFINITY;
			for (EgEdge *ei : vk->m_pEdges)
			{
				// �˻� ������ ��� ������ ��� �����Ѵ�(���� ó��).
				EgEdge *ej = ei->m_pNext;
				if (ej->IsBndry())
					continue;

				// �ִ� ��ΰ� ����� ���� ������ ã�´�.
				float tmp, d = get_geodesic_dist(POS(vk), ej, &tmp, Distances);
				if (d < min_dist || (EQ(d, min_dist, 1.0e-5) && tmp < t_star))
				{
					min_dist = d;
					t_star = tmp;
					e_star = ej;
				}
			}

			// �ִ� ��ο� �������� �߰��ϰ�, ���� ������ �˻��Ѵ�(*).
			CutInfo.push_back(std::make_pair(e_star->m_pNext->m_pNext, 0.0f));
			t_star = EQ(t_star, 0.0f, 1.0e-5) ? 0.0f : t_star;
			t_star = EQ(t_star, 1.0f, 1.0e-5) ? 1.0f : t_star;
			e = e_star->m_pMate;
			t = 1.0f - t_star;
			continue;
		}
		else // ���� �������� ���� ���� ���̶��
		{
			// �ִ� ��ο� ���� ������ �߰��ϰ�
			CutInfo.push_back(std::make_pair(e, t));

			// �˻��� �� ���� e1, e2�� ���ϰ�
			EgEdge *e1 = e->m_pNext;
			EgEdge *e2 = e->m_pNext->m_pNext;

			// �ҽ� ������ ���Ե� �ﰢ���� ������ ���
			if (EV(e1) == S) // �Ǵ� SV(e2) == S
			{
				// ������ ���� ������ �߰��ϰ� �����Ѵ�.
				CutInfo.push_back(std::make_pair(e1, 1.0f));
				break;
			}

			// �˻��� ������ ��� ��� �������(���� ó��)
			if (e1->IsBndry() && e2->IsBndry())
			{
				// ������ ���� ������ �����ϰ�
				CutInfo.pop_back();
#ifdef _DEBUG
				printf("Warning: two edges are boundaries !!!\n");
#endif
				// ���� ������ �糡 ���� �� �ҽ� ������ ����� �������� �ٽ� �ִ� ��θ� �����Ѵ�.
				if (Distances[SV(e)->m_Idx] < Distances[EV(e)->m_Idx])
					t = 0.0f;
				else
				{
					e = e1;
					t = 0.0f;
				}
				continue;
			}

			// �� ������ ����ϴ� �Ÿ��� ����Ѵ�.
			EgPos pk = ::lerp(POS(SV(e)), POS(SV(e->m_pNext)), t);
			float t1, t2;

			// e2�� ��� ������ ��� e1������ ����
			if (!e1->IsBndry() && e2->IsBndry())
			{
				get_geodesic_dist(pk, e1, &t1, Distances);
				t1 = EQ(t1, 0.0f, 1.0e-5) ? 0.0f : t1;
				t1 = EQ(t1, 1.0f, 1.0e-5) ? 1.0f : t1;
				e = e1->m_pMate;
				t = 1.0f - t1;
				continue;
			}
			// e1�� ��� ������ ��� e2������ ����
			else if (e1->IsBndry() && !e2->IsBndry())
			{
				get_geodesic_dist(pk, e2, &t2, Distances);
				t2 = EQ(t2, 0.0f, 1.0e-5) ? 0.0f : t2;
				t2 = EQ(t2, 1.0f, 1.0e-5) ? 1.0f : t2;
				e = e2->m_pMate;
				t = 1.0f - t2;
				continue;
			}
			else // ��� ��� ������ �ƴ϶��
			{
				// ª�� ����� ������ �����Ѵ�.	
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

	// ������ ����Ʈ�� ���Ͽ�
	Path.clear();
	for (std::pair<EgEdge *, float> &info : CutInfo)
		Path.push_back(::lerp(POS(SV(info.first)), POS(EV(info.first)), info.second));

	// �ִ� �Ÿ��� ����ϰ�
	double MinDist = 0.0;
	for (int i = 0; i < NUM(Path) - 1; ++i)
		MinDist += dist(Path[i], Path[i + 1]);

	// �ִ� ���̿� ��� �ð��� ����� ��
#ifdef _DEBUG
	printf("Shortest distances = %lf\n", MinDist);
	printf("Elapsed time = %u ms\n\n", clock() - st);
#endif

	// �ִ� �Ÿ��� ��ȯ�Ѵ�.
	return MinDist;
}

/*!
*	\brief	���� ��ġ���� ������ �ҽ� �������� �ִ� �Ÿ��� ����ϴ� �Լ�
*	\note	������ ������: 2020-05-26
*
*	\param	pk[in]			���� ��ġ
*	\param	e[in]			�˻��� ���� e = (vi, vj)
*	\param	t[out]			������ �������� �Ķ���Ͱ� ����ȴ�.
*	\param	Distances[in]	�ҽ� ������ ���� ���� �Ÿ���
*
*	\return	�ҽ� ���������� �ִ� �Ÿ��� ��ȯ�Ѵ�.
*/
static float get_geodesic_dist(EgPos pk, EgEdge *e, float *t, std::vector<double> &Distances)
{
	// �ﰢ���� �� ������ ������ ���̸� ����ϰ�
	EgVertex *vi = SV(e);
	EgVertex *vj = EV(e);
	float dij_sq = (float)dist_sq(vi->m_Pos, vj->m_Pos);
	float djk_sq = (float)dist_sq(vj->m_Pos, pk);
	float dki_sq = (float)dist_sq(pk, vi->m_Pos);
	float dij = SQRT(dij_sq);
	float djk = SQRT(djk_sq);
	float dki = SQRT(dki_sq);

	// �ҽ� �������� vi������ �Ÿ� di�� �ּ� ���� �Ÿ��� �ʱ�ȭ �Ѵ�.
	float di = (float)Distances[vi->m_Idx];
	float dj = (float)Distances[vj->m_Idx];
	float dk = (di + dki < dj + djk) ? (di + dki) : (dj + djk);
	*t = (di + dki < dj + djk) ? 0.0f : 1.0f;

	// ������ �ҽ� ���� vs�� �����Ѵٸ�
	if (ABS(di - dj) < dij && dij < di + dj)
	{
		// 2���� ��鿡 ������ vs(x0, y0), vi(0, 0), vj(-dij, 0), pk(x1, y1)�� ��ǥ�� ���ϰ�,
		float x0 = (SQR(dj) - SQR(di) - dij_sq) / (2 * dij);
		float y0 = SQRT(SQR(di) - SQR(x0));
		float x1 = (djk_sq - dki_sq - dij_sq) / (2 * dij);
		float y1 = -SQRT(dki_sq - SQR(x1));

		// ���� (vs, vk)�� (vj, vi)�� �������� x��ǥ�� ���Ͽ�
		if (EQ(y0, y1, 1.0e-6))
			printf("Error in get_geodesic_dist()...\n");
		float x = (x0 * y1 - x1 * y0) / (y1 - y0);

		// �������� ���� (vj, vi)���̿� �����Ѵٸ� �Ÿ� di�� �����Ѵ�.
		if (x > -dij && x < 0.0)
		{
			dk = SQRT(SQR(x1 - x0) + SQR(y1 - y0));
			*t = -x / dij;
		}
	}

	// ���� �Ÿ��� ��ȯ�Ѵ�.
	return dk;
}

/*!
*	\brief	�޽� ���� ������ �� �� ������ �ִ� ��ο� ���� ������ ���ϴ� �Լ�
*	\note	������ ������: 2020-08-31
*
*	\param	S[in]			�������� ���� ������
*	\param	E[in]			������ ���� ������
*	\param	Distances[in]	�ҽ��� ������ ���� �Ÿ���
*	\param	Path[out]		Ÿ�������� �ҽ������� ��θ� �����ϴ� �������� ��ġ�� ����ȴ�.
*	\param	CutInfo[out]	Ÿ�������� �ҽ������� �ִ� ����� ���� ������ ����ȴ�.
*
*	\return �ִ� �Ÿ��� ��ȯ�Ѵ�.
*/
double get_shortest_path(
	EgFacePt *S,
	EgFacePt *E,
	std::vector<double> &Distances,
	std::vector<EgPos> &Path,
	std::vector<std::pair<EgEdge *, float>> &CutInfo)
{
	// ������ �ﰢ�� ���� �ִٸ� �����Ѵ�.
	if (S->m_pFace == E->m_pFace)
		return -1.0;

	// ���� �ð��� �����Ѵ�.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// ���� ������ ���Ѵ�.
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

	// �������� ���� E���� S������ �ִ� ����� ������ ã�´�. 
	CutInfo.clear();
	EgFace *pSrcFace = S->m_pFace;
	while (true)
	{
		// ���� ����: �ҽ����� ���Ե� �ﰢ���� �����ߴٸ�
		if (e->m_pFace == pSrcFace)
		{
			// �ִ� ��ο� ���� ������ �߰��ϰ� �����Ѵ�.
			CutInfo.push_back(std::make_pair(e, t));
			break;
		}

		// ���� �������� �����̶��
		if (EQ(t, 0.0f, 1.0e-5) || EQ(t, 1.0f, 1.0e-5))
		{
			// ���� �������� ���Ͽ�
			EgVertex *vk = EQ(t, 0.0f, 1.0e-5) ? SV(e) : EV(e);
			EgEdge *ei = NULL;
			for (EgEdge *e : vk->m_pEdges)
			{
				// �ҽ� ������ ������ �ﰢ���� ���� �������� ���Եȴٸ�
				if (e->m_pFace == pSrcFace)
				{
					ei = e;
					break;
				}
			}
			if (ei != NULL)
			{
				// ���� �������� �߰��ϰ� �����Ѵ�.
				CutInfo.push_back(std::make_pair(ei, 0.0f));
				break;
			}

			// 1-�� ��� ������ �˻��Ͽ� �ִ� ��ΰ� ����� ���� ������ ã�´�.
			EgEdge *e_star = NULL;
			float t_star, min_dist = INFINITY;
			for (EgEdge *ei : vk->m_pEdges)
			{
				// �˻� ������ ��� ������ ��� �����Ѵ�(���� ó��).
				EgEdge *ej = ei->m_pNext;
				if (ej->IsBndry())
					continue;

				// �ִ� ��ΰ� ����� ���� ������ ã�´�.
				float tmp, d = get_geodesic_dist(POS(vk), ej, &tmp, Distances);
				if (d < min_dist || (EQ(d, min_dist, 1.0e-5) && tmp < t_star))
				{
					min_dist = d;
					t_star = tmp;
					e_star = ej;
				}
			}

			// �ִ� ��ο� �������� �߰��ϰ�, ���� ������ �˻��Ѵ�(*).
			CutInfo.push_back(std::make_pair(e_star->m_pNext->m_pNext, 0.0f));
			t_star = EQ(t_star, 0.0f, 1.0e-5) ? 0.0f : t_star;
			t_star = EQ(t_star, 1.0f, 1.0e-5) ? 1.0f : t_star;
			e = e_star->m_pMate;
			t = 1.0f - t_star;
			continue;
		}
		else // ���� �������� ������ �������̶��
		{
			// �˻��� �� ���� e1, e2�� ���ϰ�
			EgEdge *e1 = e->m_pNext;
			EgEdge *e2 = e->m_pNext->m_pNext;

			// �ִ� ��ο� ���� ������ �߰��ϰ�(*)
			CutInfo.push_back(std::make_pair(e, t));

			// �˻��� ������ ��� ��� �������(���� ó��)
			if (e1->IsBndry() && e2->IsBndry())
			{
				// ������ ���� ������ �����ϰ�
				printf("Warning: two edges are boundaries !!!\n");
				CutInfo.pop_back();

				// ���� ������ �糡 ���� �� �ҽ� ������ ����� �������� �ٽ� �ִ� ��θ� �����Ѵ�.
				if (Distances[SV(e)->m_Idx] < Distances[EV(e)->m_Idx])
					t = 0.0f;
				else
				{
					e = e1;
					t = 0.0f;
				}
				continue;
			}

			// �� ������ ����ϴ� �Ÿ��� ����Ѵ�.
			EgPos pk = ::lerp(POS(SV(e)), POS(EV(e)), t);
			float t1, t2;

			// e2�� ��� ������ ���
			if (!e1->IsBndry() && e2->IsBndry())
			{
				get_geodesic_dist(pk, e1, &t1, Distances);
				t1 = EQ(t1, 0.0f, 1.0e-6) ? 0.0f : t1;
				t1 = EQ(t1, 1.0f, 1.0e-6) ? 1.0f : t1;
				e = e1->m_pMate;
				t = 1.0f - t1;
				continue;
			}
			// e1�� ��� ������ ���
			else if (e1->IsBndry() && !e2->IsBndry())
			{
				get_geodesic_dist(pk, e2, &t2, Distances);
				t2 = EQ(t2, 0.0f, 1.0e-5) ? 0.0f : t2;
				t2 = EQ(t2, 1.0f, 1.0e-5) ? 1.0f : t2;
				e = e2->m_pMate;
				t = 1.0f - t2;
				continue;
			}
			else // ��� ��� ������ �ƴ϶��
			{
				// ª�� ����� ������ �����Ѵ�.	
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

	// ������ ����Ʈ�� �ϼ��Ѵ�.
	Path.clear();
	Path.push_back(E->Pos());
	for (std::pair<EgEdge *, float> info : CutInfo)
		Path.push_back(::lerp(POS(SV(info.first)), POS(EV(info.first)), info.second));
	Path.push_back(S->Pos());

	// �ִ� �Ÿ��� ����ϰ�
	double MinDist = 0.0;
	for (int i = 0; i < (int)Path.size() - 1; ++i)
		MinDist += dist(Path[i], Path[i + 1]);

	// �ִ� ���̿� ��� �ð��� ����� ��
#ifdef _DEBUG
	printf("Shortest distances = %lf\n", MinDist);
	printf("Elapsed time for back-tracing = %u ms\n\n", clock() - st);
#endif

	// �ִ� �Ÿ��� ��ȯ�Ѵ�.
	return MinDist;
}

/*!
*	\brief	�� ���� �����ϴ� ��� ���� ��ο� �Ÿ��� ����Ѵ�.
*	\note	������ ������: 2020-06-24
*
*	\param	S[in]			�������� ���� ������
*	\param	E[in]			������ ���� ������
*	\param	Pln[in]			���� ��鿡 ���� ������(���� ���⿡ ���� ���������� ���� ������ ��������).
*	\param	Path[out]		���� ��θ� �����ϴ� ���� �������� ����ȴ�.
*	\param	CutInfo[out]	���� ����� �Ʒ��� ���� ����ϴ� ������ ���� ������ ����ȴ�.
*
*	\return ����� ���̸� ��ȯ�Ѵ�(��ΰ� �������ų� ������ �߻��ϸ� -1.0�� ��ȯ�Ѵ�).
*/
double get_planar_path(EgFacePt *S, EgFacePt *E, EgPlane *Pln, std::vector<EgPos> &Path, std::vector<std::pair<EgEdge*, float>> &CutInfo)
{
	// ���� �ð��� �����Ѵ�.
#ifdef _DEBUG
	clock_t st = clock();
#endif

	// ����� �ʱ�ȭ �Ѵ�.
	Path.clear();
	CutInfo.clear();

	// �������� ������ ���� �ﰢ�� �������̶�� �� �� ������ �Ÿ��� ��ȯ�Ѵ�.
	if (S->m_pFace == E->m_pFace)
	{
		Path.push_back(S->Pos());
		Path.push_back(E->Pos());
		return ::dist(Path[0], Path[1]);
	}

	// ���� ���� �����ϴ� ���� ������ ã�´�.
	float t;
	EgEdge *StEdge = NULL;

	// �������� �����̶��
	if (S->IsVertex())
	{
		// 1-�� ��� ������ �����Ͽ� ���� ������ ���ϰ�, �������� ������ �����Ѵ�. 
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
	else // �������� �ﰢ�� �������̶��
	{
		// �� ������ �˻��Ͽ� ���� ������ ���Ѵ�.
		for (int i = 0; i < 3; ++i)
		{
			if (intersect_plane_edge(Pln, S->m_pFace->GetEdge(i), &t))
			{
				StEdge = S->m_pFace->GetEdge(i);
				break;
			}
		}
	}

	// ���� ���� �����ϴ� ������ ������ ã�´�.
	EgEdge *EdEdge = NULL;
	EgPlane rPln = Pln->GetReversePlane();

	// ������ �����̶��
	if (E->IsVertex())
	{
		// 1-�� ��� ������ �����Ͽ� ������ ������ �����Ѵ�. 
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
	else // ������ �ﰢ�� �������̶��
	{
		// �� ������ �˻��Ͽ� ���� ������ ���Ѵ�.
		for (int i = 0; i < 3; ++i)
		{
			if (intersect_plane_edge(&rPln, E->m_pFace->GetEdge(i), &t))
			{
				EdEdge = E->m_pFace->GetEdge(i);
				break;
			}
		}
	}

	// ���� �Ǵ� �� ������ �� ã�Ҵٸ�
	if (StEdge == NULL) // ��Ȯ�� ����
	{
		printf("[Error]: ���� ������ �� ã��...\n");
		return -1.0;
	}
	if (EdEdge == NULL) // ��Ȯ�� ����
	{
		printf("[Error]: �� ������ �� ã��...\n");
		return -1.0;
	}

	// ù ��° ���� ������ �߰��Ѵ�.
	intersect_plane_edge(Pln, StEdge, &t);
	CutInfo.push_back(std::make_pair(StEdge, t));

	// ���� ���� ������ ã�´�.
	EgEdge *e = StEdge->m_pMate;
	t = 1.0f - t;
	while (true)
	{
		// ������ ����: ��ΰ� ����ٸ� -1�� ��ȯ�Ѵ�.
		if (e == NULL)
		{
			CutInfo.clear();
			//printf("[Error]: ��� ���� ��ο� Ȧ ����...\n");
			return -1.0;
		}

		// ������ ����: ���������� ���ƿԴٸ� -1�� ��ȯ�Ѵ�.
		if (e->m_pFace == S->m_pFace)
		{
			CutInfo.clear();
			//printf("[Error]: ��� ���� ��ΰ� ������ ����...\n");
			return -1.0;
		}

		// ���� ����: ������ ������ �����ߴٸ� �����Ѵ�.
		if (e == EdEdge)
			break;

		// ���� �������� ������ �����̶��
		if (EQ(t, 1.0f, 1.0e-6))
		{
			// ���� ����: ������ ���Ե� �ﰢ���� �����ߴٸ� �����Ѵ�.
			EgVertex *vk = EV(e);
			if (vk == SV(EdEdge) || vk == EV(EdEdge))
				break;

			for (EgEdge *ei : vk->m_pEdges)
			{
				// ������ ����: ���������� ���ƿԴٸ� -1.0�� ��ȯ�Ѵ�.
				if (ei->m_pFace == S->m_pFace)
				{
					CutInfo.clear();
					//printf("[Error]: ��� ���� ��ΰ� ������ ����...\n");
					return -1.0;
				}
			}

			// 1-�� ��� ������ �˻��Ͽ� ���� ��ΰ� ����� ���� ������ ã�´�.
			bool success = false;
			for (EgEdge *ei : vk->m_pEdges)
			{
				// �˻� ������ ��� ������ ��� �����Ѵ�.
				if (ei->m_pNext->IsBndry())
					continue;

				// ��� ��ΰ� ����� ���� ������ ã�´�.
				if (intersect_plane_edge(Pln, ei->m_pNext, &t))
				{
					CutInfo.push_back(std::make_pair(ei->m_pNext, t));
					e = ei->m_pNext->m_pMate;
					t = 1.0f - t;
					success = true;
					break;
				}
			}

			// ��ΰ� ����ٸ� -1�� ��ȯ�Ѵ�.
			if (!success)
			{
				printf("[Error]: �˷����� ���� ����...\n");
				return -1.0;
			}
			continue;
		}
		else // �������� ���� ���� ���̶��
		{
			// �ٸ� �� ������ �����Ͽ� ���� ��θ� �߰��Ѵ�.
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

			// ��ΰ� ����ٸ� -1�� ��ȯ�Ѵ�.
			if (!success)
			{
				printf("[Error]: �˷����� ���� ����...\n");
				return -1.0;
			}
			continue;
		}
	}

	// ���� �����κ��� ���� ��θ� ���ϰ�
	Path.push_back(S->Pos());
	for (std::pair<EgEdge *, float> &info : CutInfo)
		Path.push_back(::lerp(POS(SV(info.first)), POS(EV(info.first)), info.second));
	Path.push_back(E->Pos());

	// ���̸� ����Ͽ� ��ȯ�Ѵ�.
	double MinDist = 0.0;
	for (int i = 0; i < NUM(Path) - 1; ++i)
		MinDist += dist(Path[i], Path[i + 1]);
	return MinDist;
}

/*!
*	\brief	���� ������ ���� ������ ���ϴ� �Լ�
*	\note	������ ������: 2020-06-29
*	\note	�ߺ� ������ ���ϱ� ���� [������, ����) ������ vertex spliting�� ���
*
*	\param	Pln[in]		��鿡 ���� ������
*	\param	e[in]		������ ���� e=(P, Q)�� ���� ������
*	\param	t[out]		������ ����� �Ʒ��� ���� ������ ���, �������� �Ķ���� ��(0.0 <= t < 1.0)�� ����ȴ�.
*
*	\return	������ ����� �Ʒ��� ���� ������ ��� true, �׷��� ������ false�� ��ȯ�Ѵ�.
*/
bool intersect_plane_edge(EgPlane *Pln, EgEdge *e, float *t)
{
	// ������ ������(P)�� ����(Q)�� ���Ѵ�.
	EgPos P = POS(SV(e));
	EgPos Q = POS(EV(e));

	// P�� ��� �� �ְ�, Q�� ��� ���ʿ� �ִ� ���
	if (Pln->IsOnPlane(P) && Pln->IsAbovePlane(Q))
	{
		if (t != NULL)
			*t = 0.0f;
		return true;
	}
	else if (Pln->IsBelowPlane(P) && Pln->IsAbovePlane(Q)) // P�� ��� �Ʒ��ʿ� �ְ�, Q�� ��� ���ʿ� �ִ� ���
	{
		double denominator = Pln->N() * (Q - P);
		if (t != NULL)
			*t = (float)(-(Pln->Eval(P)) / denominator);
		return true;
	}
	else if (Pln->IsOnPlane(P) && Pln->IsOnPlane(Q)) // ������ ��鿡 ���ԵǾ� �ִ� ���
	{
		if (t != NULL)
			*t = -1.0f;
		printf("[Warning]: ���� ��� ���� ���� ����...\n");
		return false;
	}
	else // �̿��� ���� false�� ��ȯ�Ѵ�.
	{
		if (t != NULL)
			*t = -1.0f;
		return false;
	}
}

/*!	\brief	��� ������ ���� �ѷ� ��ο� ���̸� ����ϴ� �Լ�
*	\note	������ ������: 2020-06-22
*
*	\param	S[in]			���� ����� �������� ���� ������
*	\param	Pln[in]			���� ��鿡 ���� ������
*	\param	Path[out]		��θ� �����ϴ� �������� ��ġ�� ����ȴ�.
*	\param	CutInfo[out]	���� ����� �Ʒ��� ���� ����ϴ� ������ ���� ������ ����ȴ�.
*
*	\return ��ΰ� �����ϸ� �ѷ��� ���̸� ���ٸ� -1.0�� ��ȯ�Ѵ�.
*/
double get_planar_path_closed(EgFacePt *S, EgPlane *Pln, std::vector<EgPos> &Path, std::vector<std::pair<EgEdge*, float>> &CutInfo)
{
	// ���� �ð��� �����Ѵ�.
	clock_t st = clock();
	Path.clear();
	CutInfo.clear();

	// ���� ���� �����ϴ� ���� ������ ã�´�.
	float t;
	EgEdge *StEdge = NULL;

	// �������� �����̶��
	if (S->IsVertex())
	{
		// 1-�� �ֺ� ������ �����Ͽ� ���� ������ ���ϰ�, �������� ������ �����Ѵ�. 
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
	else // �������� �ﰢ�� �������̶��
	{
		// �� ������ �˻��Ͽ� ���� ������ ���Ѵ�.
		for (int i = 0; i < 3; ++i)
		{
			if (intersect_plane_edge(Pln, S->m_pFace->GetEdge(i), &t))
			{
				StEdge = S->m_pFace->GetEdge(i);
				break;
			}
		}
	}

	// ���� ������ �� ã�Ҵٸ�
	if (StEdge == NULL) // ��Ȯ�� ����
	{
		printf("[Error]: ���� ������ �� ã��...\n");
		return -1.0;
	}

	// ù ��° ���� ������ �߰��ϰ�, ���� ���� ������ ã�´�.
	intersect_plane_edge(Pln, StEdge, &t);
	CutInfo.push_back(std::make_pair(StEdge, t));
	int result = find_next_planar_path_closed(S, StEdge->m_pMate, 1.0f - t, Pln, CutInfo);

	// ������ ���� ��� ���� �޽����� -1.0�� ��ȯ�Ѵ�.
	switch (result)
	{
	case -1:
		CutInfo.clear();
		printf("[Error]: ��Ȯ�� ����...\n");
		return -1.0;

	case 0:
		CutInfo.clear();
		printf("[Error]: ��� ���ܿ� Ȧ ����...\n");
		return -1.0;
	}

	// ���� ������ ���� ��θ� ���ϰ�.
	Path.push_back(S->Pos());
	for (std::pair<EgEdge *, float> info : CutInfo)
		Path.push_back(::lerp(POS(SV(info.first)), POS(EV(info.first)), info.second));
	Path.push_back(S->Pos());

	// ���̸� ����Ͽ� ��ȯ�Ѵ�.
	double MinDist = 0.0;
	for (int i = 0; i < NUM(Path) - 1; ++i)
		MinDist += dist(Path[i], Path[i + 1]);
	return MinDist;
}

/*!
*	\brief	��� ���ܿ� ���� �ѷ� ��θ� ���ϴ� �Լ�
*	\note	������ ������: 2020-06-22
*
*	\param	S[in]			����� �������� ���� ������
*	\param	e[in]			���� �������� ���� ���� ���� ������
*	\param	t[in]			���� �󿡼� ���� �������� ��ġ �Ķ����(0.0 <= t <= 1.0)
*	\param	Pln[in]			��ΰ� ���� ��鿡 ���� ������
*	\param	CutInfo[out]	�ѷ� ����� ���� ������ ����ȴ�.
*
*	\return	��� ��(-1: ��Ȯ�� ����, 0: Ȧ�� ���� ��ΰ� ����, 1: ���� ��θ� ã��)�� ��ȯ�Ѵ�.
*/
static int find_next_planar_path_closed(EgFacePt *S, EgEdge *e, float t, EgPlane *Pln, std::vector<std::pair<EgEdge *, float>> &CutInfo)
{
	// ��������(����): ��ΰ� ����ٸ� 0�� ��ȯ�Ѵ�.
	if (e == NULL)
		return 0;

	// ��������(����): ���������� ���ƿԴٸ� 1�� ��ȯ�Ѵ�.
	if (e->m_pFace == S->m_pFace)
		return 1;

	// ���� �������� ������ �����̶��
	if (EQ(t, 1.0f, 1.0e-6))
	{
		// ���� �������� ���Ͽ�, ������ ȣ�� ���θ� �˻��Ѵ�.
		EgVertex *vk = EV(e);
		for (EgEdge *ei : vk->m_pEdges)
		{
			// ��������(����): ���������� ���ƿԴٸ� 1�� ��ȯ�Ѵ�.
			if (ei->m_pFace == S->m_pFace)
				return 1;
		}

		// 1-�� ��� ������ �˻��Ͽ� ���� ��ΰ� ����� ���� ������ ã�´�.
		for (EgEdge *ei : vk->m_pEdges)
		{
			// �˻� ������ ��� ������ ��� �����Ѵ�.
			if (ei->m_pNext->IsBndry())
				continue;

			// ���� ��ΰ� ����� ���� ������ ã�´�.
			if (intersect_plane_edge(Pln, ei->m_pNext, &t))
			{
				CutInfo.push_back(std::make_pair(ei->m_pNext, t));
				return find_next_planar_path_closed(S, ei->m_pNext->m_pMate, 1.0f - t, Pln, CutInfo);
			}
		}

		// ��������(����): ��ΰ� ����ٸ� -1�� ��ȯ�Ѵ�.
		return -1;
	}
	else
	{
		// �ٸ� �� ������ �����Ͽ� ���� ��θ� �߰��ϰ�, ���� ��θ� ã�´�.
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

	// ��Ȯ�� ������ ��� -1�� ��ȯ�Ѵ�.
	return -1;
}




