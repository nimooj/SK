#include "..\usr\include\egmesh\egmesh.h"
#include <cstring>
#include <vector>
#include <map>

/*!
*	\biref 정점의 가우스 곡률을 계산한다.
*	\note 경계 정점의 경우 계산 불가
*
*	\param pVert[in] 대상 정점
*
*	\return 계산된 가우스 곡률을 반환한다.
*/
double get_gaussian_curvature(EgVertex *pVert)
{
	// 경계 정점의 경우 0.0을 반환한다.
	if (pVert->IsBndry())
		return 0.0;

	// 정점 주변의 각도를 모두 더한다.
	double sum_theta = 0.0;
	for (EgEdge *e : pVert->m_pEdges)
		sum_theta += e->m_pNext->GetAngle(true);

	// 가우스 곡률을 계산하여 반환한다.
	double ret = (2 * M_PI - sum_theta);
	if (EQ(ret, 0.0, 1.0e-6))
		ret = 0.0;
	else
		ret /= ::get_mixed_voronoi_cell_area(pVert);
	return ret;
}

/*!
*	\biref 메쉬의 가우스 곡률을 계산한다.
*
*	\param pMesh[in] 대상 메쉬
*	\param Ks[out] i번째 정점의 가우스 곡률이 Ks[i]에 저장된다.
*	\param min[out] 최소 가우스 곡률이 저장된다.
*	\param max[out] 최대 가우스 곡률이 저장된다.
*/
void get_gaussian_curvature(EgMesh *pMesh, std::vector<double> &Ks, double &min, double &max)
{
	// 변수를 초기화 한다.
	clock_t st = clock();
	min = 1e+10;
	max = 1e-10;
	Ks.clear();

	// 각각의 정점에 대하여
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// 가우스 곡률을 계산하여
		double k = ::get_gaussian_curvature(v);

		// 최대, 최소를 구하고 곡률 배열에 추가한다.
		min = MIN(k, min);
		max = MAX(k, max);
		Ks.push_back(k);
	}

	// 수행 시간과 곡률의 범위를 출력한다.
	printf("Total Elapsed time for GetGaussCurvature = %u ms \n", clock() - st);
	printf("Gaussian Curvature Range = [%.5lf ~ %.5lf]\n", min, max);
}

/*!
*	\biref 정점의 평균 곡률의 절대값을 계산한다.
*	\note 경계 정점의 경우 계산 불가
*
*	\param pVert[in] 대상 정점
*
*	\return 계산된 평균 곡률의 절대값을 반환한다.
*/
double get_mean_curvature(EgVertex *pVert)
{
	// 평균 곡률 벡터를 정의하고,
	EgVec3 L;

	// 정점에서 시작하는 각각의 에지 e에 대하여
	for (EgEdge *e : pVert->m_pEdges)
	{
		// Cotangent 가중치를 계산한다.
		double beta = e->GetAngle(true);
		double weight = 0.0;
		if (e->IsBndry())
			weight = cos(beta) / sin(beta);
		else
		{
			double alpha = e->m_pMate->GetAngle(true);
			weight = (cos(alpha) / sin(alpha) + cos(beta) / sin(beta)) / 2.0;
		}

		// 평균 곡률 벡터를 계산한다.
		L = L + weight * (POS(EV(e)) - POS(pVert));

		// 경계 에지에 대한 예외처리를 한다.
		if (e->m_pNext->m_pNext->m_pMate == NULL)
		{
			beta = e->m_pNext->m_pNext->GetAngle(true);
			weight = cos(beta) / sin(beta);
			L = L + weight * (POS(EV(e->m_pNext)) - POS(pVert));
		}
	}
	// 계산된 평균 곡률 벡터를 국소 면적의 크기로 나눈다(정규화).
	L /= ::get_mixed_voronoi_cell_area(pVert);

	// 평균 곡률 절대값을 계산한다.
	double ret = 0.5 * norm(L);
	if (EQ(ret, 0.0, 1.0e-7))
		ret = 0.0;

	return ret;
}

/*!
*	\biref 메쉬 각 정점의 평균 곡률의 절대값을 계산한다.
*
*	\param pMesh[in] 대상 메쉬
*	\param Hs[out] i번째 정점의 평균 곡률의 절대값이 Hs[i]에 저장된다.
*	\param min[out] 최소 평균 곡률의 절대값이 저장된다.
*	\param max[out] 최대 평균 곡률의 절대값이 저장된다.
*/
void get_mean_curvature(EgMesh *pMesh, std::vector<double> &Hs, double &min, double &max)
{
	// 변수를 초기화한다.
	clock_t st = clock();
	min = 1e+10;
	max = 1e-10;
	Hs.clear();

	// 각각의 정점에 대하여
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// 평균 곡률을 계산하여
		double h = ::get_mean_curvature(v);

		// 최대, 최소를 구하고 곡률 배열에 추가한다.
		min = MIN(h, min);
		max = MAX(h, max);
		Hs.push_back(h);
	}

	// 수행 시간과 곡률의 범위를 출력한다.
	printf("Total Elapsed time for GetMeanCurvature = %u ms \n", clock() - st);
	printf("Mean Curvature Range = [%.5lf ~ %.5lf]\n", min, max);
}

/*!
*	\brief 정점의 국소 영역 경계점의 리스트를 생성한다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*	\param BndryPts[out] 각 정점의 국소 영역 경계점이 저장된다.
*/
void get_mixed_voronoi_cell_bndry(EgMesh *pMesh, std::vector<std::vector<EgPos>> &BndryPts)
{
	for (EgVertex *v : pMesh->m_pVerts)
	{
		std::vector<EgPos> Points;

		// 중심 정점에서 시작하는 각각의 에지에 대하여
		for (EgEdge *e : v->m_pEdges)
		{
			// 삼각형을 구성하는 pqr의 좌표를 구한다.
			EgPos p = v->m_Pos;
			EgPos q = e->m_pNext->m_pVert->m_Pos;
			EgPos r = e->m_pNext->m_pNext->m_pVert->m_Pos;

			// 정점 p와 q의 중점을 추가하고
			Points.push_back(lerp(p, q, 0.5));

			// 외심(둔각 삼각형이라면 중점으로 대체)을 추가하고
			switch (is_obtuse_tri(p, q, r))
			{
			case 0:
				Points.push_back(circumcenter(p, q, r));
				break;
			case 1:
				Points.push_back(lerp(q, r, 0.5));
				break;
			case 2:
				Points.push_back(lerp(r, p, 0.5));
				break;
			case 3:
				Points.push_back(lerp(p, q, 0.5));
				break;
			}

			// 정점 p와 r의 중점을 추가한다
			Points.push_back(lerp(p, r, 0.5));
		}
		BndryPts.push_back(Points);
	}
}

/*!
*	\brief 정점의 국소 영역의 면적을 계산한다.
*	\note  참고 논문: Discrete Differential-Geometry Operators for Triangulated 2-Manifolds
*
*	\param v[in] 정점에 대한 포인터
*
*	\return 국소 영역의 면적을 반환한다.
*/
double get_mixed_voronoi_cell_area(EgVertex *v)
{
	// 결과 값을 0으로 초기화 한다.
	double Area = 0.0;

	// 정점을 공유한 각각의 삼각형에 대하여
	for (EgEdge *e : v->m_pEdges)
	{
		// 삼각형을 구성하는 pqr의 좌표를 구한다.
		EgFace *f = e->m_pFace;
		EgPos p = v->m_Pos;
		EgPos q = e->m_pNext->m_pVert->m_Pos;
		EgPos r = e->m_pNext->m_pNext->m_pVert->m_Pos;
		double T = 0.0, cot_q = 0.0, cot_r = 0.0;
		
		// 삼각형의 형태를 분석하여 
		switch (is_obtuse_tri(p, q, r))
		{
		case 0:	// 예각 삼각형이라면 보로노이 면적을 구하고,
			cot_q = (r - q) * (p - q) / norm((r - q) ^ (p - q));
			cot_r = (p - r) * (q - r) / norm((p - r) ^ (q - r));
			T = (norm_sq(q - p) * cot_r + norm_sq(r - p) * cot_q) / 8.0;
			break;

		case 1: // 점 p가 둔각이라면 
			T = f->GetArea() / 2.0;
			break;

		case 2:	// 점 q 또는 r이 둔각이라면
		case 3:
			T = f->GetArea() / 4.0;
			break;
		}

		// 삼각형의 부분면적을 누적한다.
		Area += T;
	}

	// 국소 영역의 면적을 반환한다.
	return Area;
}

/*!
*	\brief	메쉬의 Cotangent 라플라시안 행렬을 계산한다.
*
*	\param	pMesh[in] 대상 메쉬
*	\param	D[out] 국소영역의 면적의 역수(1/A_i)가 저장된다.
*	\param	M[out] 라플라시안 행렬이 저장된다.
*/
void get_laplacian_matrix(EgMesh *pMesh, Eigen::SparseMatrix<float> &D, Eigen::SparseMatrix<float> &M)
{
	// 수행 시간 측정을 위해서 현재 clock을 구한다.
	clock_t st = clock();

	// 행렬의 크기를 재설정하고 0으로 초기화 한다.
	int NumVert = pMesh->UpdateVertIdx();
	D.resize(NumVert, NumVert);
	M.resize(NumVert, NumVert);

	// 모든 에지의 가중치를 0으로 초기화 한다.
	for (EgFace *f : pMesh->m_pFaces)
		f->m_pEdge->m_Cost = f->m_pEdge->m_pNext->m_Cost = f->m_pEdge->m_pNext->m_pNext->m_Cost = 0.0;

	// 각각의 정점에 대하여 연결된 정점과의 에지 가중치를 구한다.
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// 행렬의 열 인덱스를 구하고
		int j = v->m_Idx;

		// 정점에 연결된 각각의 에지에 대하여
		double tot_weight = 0.0;
		for (EgEdge *e : v->m_pEdges)
		{
			// 행렬의 행 인덱스를 구하고
			int i = e->m_pNext->m_pVert->m_Idx;

			// 가중치(w_ji)가 계산되지 않은 새로운 에지라면
			double w_ji = e->m_Cost;
			if (w_ji == 0.0)
			{
				// 경계 에지가 아니라면
				if (!e->IsBndry())
				{
					// 두 각으로 가중치를 구하고
					double cot_beta = e->GetCotangent();
					double cot_alpha = e->m_pMate->GetCotangent();
					w_ji = (cot_alpha + cot_beta) / 2.0;
					e->m_Cost = e->m_pMate->m_Cost = w_ji;
				}
				else // 경계 에지라면
				{
					// 한 각으로 가중치를 구하고
					e->m_Cost = w_ji = e->GetCotangent();
				}
			}

			// Column major 순으로 가중치를 추가하고, 총합을 계산한다.
			M.coeffRef(i, j) = (float)w_ji;	// 어차피 대칭 행렬이므로 상관 없음
			tot_weight += w_ji;

			// 경계 에지에 대한 예외처리를 한다.
			if (e->m_pNext->m_pNext->m_pMate == NULL)
			{
				// 행렬의 행 인덱스를 구하여 한 각으로 가중치를 구하고
				i = e->m_pNext->m_pNext->m_pVert->m_Idx;				
				w_ji = e->m_pNext->m_pNext->GetCotangent();
				e->m_pNext->m_pNext->m_Cost = w_ji;

				// Column major 순으로 가중치를 추가하고, 총합을 계산한다.
				M.coeffRef(i, j) = (float)w_ji;
				tot_weight += w_ji;
			}
		}

		// 행렬 D와 M의 대각선 원소를 기록한다.
		double area = ::get_mixed_voronoi_cell_area(v);
		D.coeffRef(j, j) = 1.0f / (float)area;
		M.coeffRef(j, j) = -(float)tot_weight;
	}

	// 모든 에지의 가중치를 0으로 초기화 한다.
	//for (EgFace *f : pMesh->m_pFaces)
	//	f->m_pEdge->m_Cost = f->m_pEdge->m_pNext->m_Cost = f->m_pEdge->m_pNext->m_pNext->m_Cost = 0.0;

	// 수행 시간을 출력한다.
	printf("\tElapsed time for Laplace matrix construction = %u ms\n", clock() - st);
}

/*!
*	\brief	특징을 반영한 라플라스 행렬을 계산한다.
*
*	\param	pMesh[in] 대상 메쉬
*	\param	M[out] 라플라시안 행렬이 저장된다.
*/
void get_laplacian_matrix_feature(EgMesh *pMesh, Eigen::SparseMatrix<float> &M)
{
	// 수행 시간 측정을 위해서 현재 clock을 구한다.
	clock_t st = clock();

	// 정점의 수(m)를 구하여, 행렬(m by m)의 크기를 재설정 한다.
	int m = pMesh->UpdateVertIdx();
	M.resize(m, m);

	// 각각의 정점에 대하여 오목성과 가우스곡률을 저장한다.
	std::vector<bool> Concavity;
	std::vector<double> Kappa;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		Concavity.push_back(v->IsConcave());
		double k = ::get_gaussian_curvature(v);
		Kappa.push_back(ABS(k)); // 계산 시간 큼		
	}

	// 각각의 정점에 대하여 연결된 정점과의 에지 가중치를 구한다.
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// 행렬의 열 인덱스를 구하고
		int j = v->m_Idx;

		// 정점에 연결된 각각의 에지에 대하여
		double tot_weight = 0.0;
		for (EgEdge *e : v->m_pEdges)
		{
			// 행렬의 행 인덱스를 구하고
			int i = e->m_pNext->m_pVert->m_Idx;
			double weight = e->GetLength() / (Kappa[j] + Kappa[i] + 1.0e-5);
			if (Concavity[j] || Concavity[i])
				weight *= 0.01;

			// Column major 순으로 가중치를 추가하고, 총합을 계산한다.
			M.coeffRef(i, j) = (float)weight;
			tot_weight += weight;

			// 경계 에지에 대한 예외처리를 한다.
			if (e->m_pNext->m_pNext->m_pMate == NULL)
			{
				// 행렬의 행 인덱스를 구하고
				i = e->m_pNext->m_pNext->m_pVert->m_Idx;
				double weight = e->m_pNext->m_pNext->GetLength() / (Kappa[j] + Kappa[i] + 1.0e-5);
				if (Concavity[j] || Concavity[i])
					weight *= 0.01;

				// Column major 순으로 가중치를 추가하고, 총합을 계산한다.
				M.coeffRef(i, j) = (float)weight;
				tot_weight += weight;
			}
		}

		// 행렬 M의 대각선 원소를 기록한다.
		M.coeffRef(j, j) = -(float)tot_weight;
	}

	// 수행 시간을 출력한다.
	printf("\tElapsed time for Laplace matrix construction = %u ms\n", clock() - st);
}

/*!
*	\brief	특징을 반영한 라플라스 행렬을 계산한다.
*
*	\param	pMesh[in] 대상 메쉬
*	\param	Verts[in] 부분 영역에 대한 정점의 리스트(비제약 정점 + 제약 정점 순으로 저장됨)
*	\param	M[out] 라플라시안 행렬이 저장된다.
*/
void get_laplacian_matrix_feature(EgMesh *pMesh, std::vector<EgVertex *> &Verts, Eigen::SparseMatrix<float> &M)
{
	// 수행 시간 측정을 위해서 현재 clock을 구한다.
	clock_t st = clock();

	// 모든 정점의 인덱스를 -1로 초기화 한다.
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_Idx = -1;

	// 부분 영역의 정점의 수(m)를 구하여, 행렬(m by m)의 크기를 재설정 한다.
	int m = 0;
	for (EgVertex *v : Verts)
		v->m_Idx = m++;
	M.resize(m, m);

	// 부분 영역의 정점에 대하여 오목성과 가우스곡률을 저장한다.
	std::vector<bool> Concavity;
	std::vector<double> Kappa;
	for (EgVertex *v : Verts)
	{
		Concavity.push_back(v->IsConcave());
		double k = ::get_gaussian_curvature(v);
		Kappa.push_back(ABS(k));
	}

	// 각각의 정점에 대하여 연결된 정점과의 에지 가중치를 구한다.
	for (EgVertex *v : Verts)
	{
		// 행렬의 열 인덱스를 구하고
		int j = v->m_Idx;

		// 정점에 연결된 각각의 에지에 대하여
		double tot_weight = 0.0;
		for (EgEdge *e : v->m_pEdges)
		{
			// 행렬의 행 인덱스를 구하여 선택 영역에 포함된 정점이 아니면 건너띤다.
			int i = e->m_pNext->m_pVert->m_Idx;
			if (i == -1)
				continue;

			// 특징이 반영된 가중치를 구한다.
			double w_ji = e->GetLength() / (Kappa[j] + Kappa[i] + 1.0e-5);
			if (Concavity[j] || Concavity[i])
				w_ji *= 0.01;

			// Column major 순으로 가중치를 추가하고, 총합을 계산한다.
			M.coeffRef(i, j) = (float)w_ji;
			tot_weight += w_ji;

			// 경계 에지에 대한 예외처리를 한다.
			if (e->m_pNext->m_pNext->m_pMate == NULL)
			{
				// 행렬의 행 인덱스를 구하여 선택 영역에 포함된 정점이 아니면 건너띤다.
				i = e->m_pNext->m_pNext->m_pVert->m_Idx;
				if (i == -1)
					continue;
				
				// 특징이 반영된 가중치를 구한다.
				double w_ji = e->m_pNext->m_pNext->GetLength() / (Kappa[j] + Kappa[i] + 1.0e-5);
				if (Concavity[j] || Concavity[i])
					w_ji *= 0.01;

				// Column major 순으로 가중치를 추가하고, 총합을 계산한다.
				M.coeffRef(i, j) = (float)w_ji;
				tot_weight += w_ji;
			}
		}

		// 행렬 M의 대각선 원소를 기록한다.
		M.coeffRef(j, j) = -(float)tot_weight;
	}

	// 정점의 인덱스를 복구하고 수행 시간을 출력한다.
	pMesh->UpdateVertIdx();
	printf("\tElapsed time for Laplace matrix construction = %u ms\n", clock() - st);
}

/*!
*	\brief	메쉬의 각 삼각형에서 그래디언트를 계산한다.
*
*	\param	pMesh[in] 대상 메쉬
*	\param	Values[in] 각 정점에 할당된 값
*	\param	Gradients[out] 계산된 각 삼각형의 그래디언트가 저장된다.
*	\param	bNormalize[in] 그래디언트의 정규화 여부
*/
void get_gradient_field(EgMesh *pMesh, std::vector<double> &Values, std::vector<EgVec3> &Gradients, bool bNormalize)
{
	// 결과를 초기화 하고
	Gradients.clear();

	// 각각의 삼각형에 대하여
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 세 정점 v0, v1, v2와 정점에 할당된 값 f0, f1, f2를 구한다.
		EgVertex *v0 = f->GetVertex(0);
		EgVertex *v1 = f->GetVertex(1);
		EgVertex *v2 = f->GetVertex(2);
		double f0 = Values[v0->m_Idx];
		double f1 = Values[v1->m_Idx];
		double f2 = Values[v2->m_Idx];

		// 삼각형의 법선을 구하여 그래디언트 방향을 계산한다.
		EgVec3 N = f->GetFaceNormal(true);
		EgVec3 e0 = v2->m_Pos - v1->m_Pos;
		EgVec3 e1 = v0->m_Pos - v2->m_Pos;
		EgVec3 e2 = v1->m_Pos - v0->m_Pos;
		EgVec3 Grad = f0 * (N ^ e0) + f1 * (N ^ e1) + f2 * (N ^ e2);

		// 정규화를 수행하여 결과에 저장한다.
		if (bNormalize)
			Grad.Normalize();
		Gradients.push_back(Grad);
	}
}

