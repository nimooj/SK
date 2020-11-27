#include "..\usr\include\egmesh\egmesh.h"

/*!
*	\brief 내부 정점의 가중치를 계산한다.
*
*	\param Verts[in] 내부 정점의 배열
*/
static void compute_edge_weight(std::vector<EgVertex *> &Verts)
{
	// 각각의 정점에 대하여
	for (EgVertex *v : Verts)
	{
		// 에지의 가중치를 구하고
		double tot_wgt = 0.0;
		for (EgEdge *e : v->m_pEdges)
		{
			EgVertex *v0 = SV(e);
			EgVertex *v1 = EV(e->m_pMate->m_pNext);
			EgVertex *v2 = EV(e);
			EgVertex *v3 = EV(e->m_pNext);
			EgVec3 a = (v1->m_Pos - v0->m_Pos).Normalize();
			EgVec3 b = (v2->m_Pos - v0->m_Pos).Normalize();
			EgVec3 c = (v3->m_Pos - v0->m_Pos).Normalize();
			double tan_half_alpha = (1.0 - a * b) / norm(a ^ b);
			double tan_half_beta = (1.0 - b * c) / norm(b ^ c);

			// 내부 정점과 연결된 가중치를 계산하여 반환한다.
			e->m_Cost = (tan_half_alpha + tan_half_beta) / dist(v0->m_Pos, v2->m_Pos);
			tot_wgt += e->m_Cost;
		}

		// 에지 가중치의 합이 1이 되도록 정규화 한다.
		for (EgEdge *e : v->m_pEdges)
			e->m_Cost /= tot_wgt;
	}
}

/*!
*	\brief 메쉬 경계 정점을 2차원 정의역의 경계에 매핑한다.
*
*	\param BndryVerts[in] 경계 정점의 배열
*	\param BndryType[in] 2차원 정의역 형태(0: 단위 정사각형, 1: 단위 원)
*/
static void compute_bndry_map(std::vector<EgVertex *> &BndryVerts, int BndryType)
{
	// 경계의 길이를 계산하고,
	double BndryLen = 0.0;
	int NumBndryVerts = (int)BndryVerts.size();
	std::vector<EgPos> BndryPos;
	for (int i = 0; i < NumBndryVerts; ++i)
	{
		BndryLen += dist(BndryVerts[i]->m_Pos, BndryVerts[(i + 1) % NumBndryVerts]->m_Pos);
		BndryPos.push_back(BndryVerts[i]->m_Pos);
	}

	// 경계가 정사각형 타입이라면,
	if (BndryType == 0)
	{
		// 경계 시작점의 파라미터 값을 할당하고, 한 변의 길이를 구한다.
		BndryVerts[0]->m_Pos.Set(0.0, 0.0, 0.0);

		// 경계 정점의 좌표를 결정한다.
		double CurrLen = 0.0;
		int side = 0;
		for (int i = 1; i < NumBndryVerts; ++i)
		{
			CurrLen += dist(BndryPos[i - 1], BndryPos[i]);
			double t = CurrLen / BndryLen;
			if (t > 0.0 && t < 0.25)
			{
				BndryVerts[i]->m_Pos.Set(t / 0.25, 0.0, 0.0);
			}
			else if (t >= 0.25 && t < 0.5)
			{
				if (side == 0)
				{
					side++;
					BndryVerts[i]->m_Pos.Set(1.0, 0.0, 0.0);
				}
				else
					BndryVerts[i]->m_Pos.Set(1.0, (t - 0.25) / 0.25, 0.0);
			}
			else if (t >= 0.5 && t < 0.75)
			{
				if (side == 1)
				{
					side++;
					BndryVerts[i]->m_Pos.Set(1.0, 1.0, 0.0);
				}
				else
					BndryVerts[i]->m_Pos.Set((0.75 - t) / 0.25, 1.0, 0.0);
			}
			else if (t >= 0.75 && t <= 1.0)
			{
				if (side == 2)
				{
					side++;
					BndryVerts[i]->m_Pos.Set(0.0, 1.0, 0.0);
				}
				else
					BndryVerts[i]->m_Pos.Set(0.0, (1.0 - t) / 0.25, 0.0);
			}
		}
	}
	else // 경계가 원 타입이라면,
	{
		// 경계 시작점의 파라미터 값을 할당하고, 한 변의 길이를 구한다.
		BndryVerts[0]->m_Pos.Set(1.0, 0.0, 0.0);

		// 경계 정점의 좌표를 결정한다.
		double CurrLen = 0.0;
		for (int i = 1; i < NumBndryVerts; ++i)
		{
			CurrLen += dist(BndryPos[i - 1], BndryPos[i]);
			double theta = 2.0 * M_PI * (CurrLen / BndryLen);
			BndryVerts[i]->m_Pos.Set(cos(theta), sin(theta), 0.0);
		}
	}
}

/*!
*	\brief 두 정점을 연결하는 에지의 가중치를 반환한다.
*
*	\param vi 에지의 시작 정점
*	\param vj 에지의 끝 정점
*
*	\return 두 정점이 연결된 경우 에지의 가중치, 그렇지 않으면 -1.0을 반환한다.
*/
static double get_edge_weight(EgVertex *vi, EgVertex *vj)
{
	for (EgEdge *e : vi->m_pEdges)
	{
		if (EV(e) == vj)
			return e->m_Cost;
	}
	return -1.0;
}

/*!
*	\brief 2차원 정의역 매개화된 메쉬를 생성한다.
*
*	\param pMesh[in] 대상 메쉬
*	\param BndryType[in] 2차원 정의역 형태(0: 단위 정사각형, 1: 단위 원)
*
*	\return 매개화된 2차원 메쉬를 반환한다.
*/
EgMesh *get_unfold_mesh(EgMesh *pMesh, int BndryType)
{
	print_msg("Mesh unfolding started...\n");
	clock_t st = clock();
	
	// 입력 메쉬를 복제하여 2D 매개화 결과 메쉬를 생성한다.
	EgMesh *pRetMesh = new EgMesh(*pMesh);
	pRetMesh->m_Name = pMesh->m_Name + "_unfold";

	// 메쉬 정점을 내부-경계 순으로 재배열 한다.
	int NumBndry = ::rearrange_verts(pRetMesh);
	if (NumBndry != 1)
	{
		print_msg("\tInterior holes exist...\n");
		print_msg("Unfolding incomplete...\n\n");
		delete pRetMesh;
		return NULL;
	}

	// 내부 정점과 연결된 에지의 가중치를 구한다.
	std::vector<EgVertex *> IntrVerts;
	int NumIntrVert = ::get_intr_verts(pRetMesh, IntrVerts);
	compute_edge_weight(IntrVerts);

	// 선형 시스템 Ax = b를 구성하기 위한 계수 행렬 A를 계산한다.
	Eigen::SparseMatrix<float, Eigen::RowMajor> A(NumIntrVert, NumIntrVert);
	A.setZero();
	for (EgVertex *v : IntrVerts)
	{
		// 에지의 시작 정점의 인덱스를 구하고
		int r = v->m_Idx;
		for (EgEdge *e : v->m_pEdges)
		{
			// 에지의 끝 정점의 인덱스를 구하여
			int c = e->m_pNext->m_pVert->m_Idx;

			// 내부 정점을 연결하는 에지라면 행렬에 가중치 값을 할당한다.
			if (c < NumIntrVert)
				A.insert(r, c) = -(float)(e->m_Cost);
		}
		A.insert(r, r) = 1.0f;
	}
	A.makeCompressed();

	// 경계 정점 리스트를 구하여 2D Convex 경계로 매핑한다.
	std::vector<EgVertex *> BndryVerts;
	int NumBndryVerts = ::get_bndry_verts(pRetMesh, BndryVerts);
	compute_bndry_map(BndryVerts, BndryType);

	// 선형 시스템의 상수 벡터(b1, b2)를 구한다.
	Eigen::VectorXf b1(NumIntrVert), b2(NumIntrVert);
	b1.setZero();
	b2.setZero();

	// 각각의 경계 정점에 대하여
	for (EgVertex *vj : BndryVerts)
	{
		// 각각의 1링 이웃 정점에 대하여
		std::vector<EgVertex *> Verts;
		vj->GetOneRingVert(Verts);
		for (EgVertex *vi : Verts)
		{
			// 경계 정점이 아니라며,
			if (!vi->IsBndry())
			{
				// 가중치를 구하여 상수 벡터에 추가한다.
				double wgt = get_edge_weight(vi, vj);
				if (wgt != -1.0)
				{
					b1(vi->m_Idx) += (float)(wgt * vj->m_Pos[0]);
					b2(vi->m_Idx) += (float)(wgt * vj->m_Pos[1]);
				}
			}
		}
	}
	print_msg("\tAx = b is constructed(%u ms)...\n", clock() - st);

	// 선형시스템의 해(x1, x2)를 구한다.
	st = clock();
	Eigen::VectorXf x1(NumIntrVert), x2(NumIntrVert);
	Eigen::SparseLU<Eigen::SparseMatrix<float, Eigen::RowMajor>> solver;
	solver.compute(A);
	x1 = solver.solve(b1);
	x2 = solver.solve(b2);

	// 내부 정점의 파라미터 값을 설정한다.
	for (int i = 0; i < NumIntrVert; ++i)
		IntrVerts[i]->m_Pos.Set(x1[i], x2[i], 0.0);

	print_msg("\tUnknowns (u, v) are found(%u ms)...\n", clock() - st);
	print_msg("Unfolding complete...\n\n");

	// 경계 상자, 법선, 렌더링 버퍼를 갱신한다.
	pRetMesh->UpdateBndBox();
	pRetMesh->UpdateNormal(FLAT);
	pRetMesh->m_VertexBuf.clear();
	pRetMesh->m_NormalBuf.clear();

	// 매개화된 메쉬를 반환한다.
	return pRetMesh;
}
