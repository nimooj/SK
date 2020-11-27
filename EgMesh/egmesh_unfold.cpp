#include "..\usr\include\egmesh\egmesh.h"

/*!
*	\brief ���� ������ ����ġ�� ����Ѵ�.
*
*	\param Verts[in] ���� ������ �迭
*/
static void compute_edge_weight(std::vector<EgVertex *> &Verts)
{
	// ������ ������ ���Ͽ�
	for (EgVertex *v : Verts)
	{
		// ������ ����ġ�� ���ϰ�
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

			// ���� ������ ����� ����ġ�� ����Ͽ� ��ȯ�Ѵ�.
			e->m_Cost = (tan_half_alpha + tan_half_beta) / dist(v0->m_Pos, v2->m_Pos);
			tot_wgt += e->m_Cost;
		}

		// ���� ����ġ�� ���� 1�� �ǵ��� ����ȭ �Ѵ�.
		for (EgEdge *e : v->m_pEdges)
			e->m_Cost /= tot_wgt;
	}
}

/*!
*	\brief �޽� ��� ������ 2���� ���ǿ��� ��迡 �����Ѵ�.
*
*	\param BndryVerts[in] ��� ������ �迭
*	\param BndryType[in] 2���� ���ǿ� ����(0: ���� ���簢��, 1: ���� ��)
*/
static void compute_bndry_map(std::vector<EgVertex *> &BndryVerts, int BndryType)
{
	// ����� ���̸� ����ϰ�,
	double BndryLen = 0.0;
	int NumBndryVerts = (int)BndryVerts.size();
	std::vector<EgPos> BndryPos;
	for (int i = 0; i < NumBndryVerts; ++i)
	{
		BndryLen += dist(BndryVerts[i]->m_Pos, BndryVerts[(i + 1) % NumBndryVerts]->m_Pos);
		BndryPos.push_back(BndryVerts[i]->m_Pos);
	}

	// ��谡 ���簢�� Ÿ���̶��,
	if (BndryType == 0)
	{
		// ��� �������� �Ķ���� ���� �Ҵ��ϰ�, �� ���� ���̸� ���Ѵ�.
		BndryVerts[0]->m_Pos.Set(0.0, 0.0, 0.0);

		// ��� ������ ��ǥ�� �����Ѵ�.
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
	else // ��谡 �� Ÿ���̶��,
	{
		// ��� �������� �Ķ���� ���� �Ҵ��ϰ�, �� ���� ���̸� ���Ѵ�.
		BndryVerts[0]->m_Pos.Set(1.0, 0.0, 0.0);

		// ��� ������ ��ǥ�� �����Ѵ�.
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
*	\brief �� ������ �����ϴ� ������ ����ġ�� ��ȯ�Ѵ�.
*
*	\param vi ������ ���� ����
*	\param vj ������ �� ����
*
*	\return �� ������ ����� ��� ������ ����ġ, �׷��� ������ -1.0�� ��ȯ�Ѵ�.
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
*	\brief 2���� ���ǿ� �Ű�ȭ�� �޽��� �����Ѵ�.
*
*	\param pMesh[in] ��� �޽�
*	\param BndryType[in] 2���� ���ǿ� ����(0: ���� ���簢��, 1: ���� ��)
*
*	\return �Ű�ȭ�� 2���� �޽��� ��ȯ�Ѵ�.
*/
EgMesh *get_unfold_mesh(EgMesh *pMesh, int BndryType)
{
	print_msg("Mesh unfolding started...\n");
	clock_t st = clock();
	
	// �Է� �޽��� �����Ͽ� 2D �Ű�ȭ ��� �޽��� �����Ѵ�.
	EgMesh *pRetMesh = new EgMesh(*pMesh);
	pRetMesh->m_Name = pMesh->m_Name + "_unfold";

	// �޽� ������ ����-��� ������ ��迭 �Ѵ�.
	int NumBndry = ::rearrange_verts(pRetMesh);
	if (NumBndry != 1)
	{
		print_msg("\tInterior holes exist...\n");
		print_msg("Unfolding incomplete...\n\n");
		delete pRetMesh;
		return NULL;
	}

	// ���� ������ ����� ������ ����ġ�� ���Ѵ�.
	std::vector<EgVertex *> IntrVerts;
	int NumIntrVert = ::get_intr_verts(pRetMesh, IntrVerts);
	compute_edge_weight(IntrVerts);

	// ���� �ý��� Ax = b�� �����ϱ� ���� ��� ��� A�� ����Ѵ�.
	Eigen::SparseMatrix<float, Eigen::RowMajor> A(NumIntrVert, NumIntrVert);
	A.setZero();
	for (EgVertex *v : IntrVerts)
	{
		// ������ ���� ������ �ε����� ���ϰ�
		int r = v->m_Idx;
		for (EgEdge *e : v->m_pEdges)
		{
			// ������ �� ������ �ε����� ���Ͽ�
			int c = e->m_pNext->m_pVert->m_Idx;

			// ���� ������ �����ϴ� ������� ��Ŀ� ����ġ ���� �Ҵ��Ѵ�.
			if (c < NumIntrVert)
				A.insert(r, c) = -(float)(e->m_Cost);
		}
		A.insert(r, r) = 1.0f;
	}
	A.makeCompressed();

	// ��� ���� ����Ʈ�� ���Ͽ� 2D Convex ���� �����Ѵ�.
	std::vector<EgVertex *> BndryVerts;
	int NumBndryVerts = ::get_bndry_verts(pRetMesh, BndryVerts);
	compute_bndry_map(BndryVerts, BndryType);

	// ���� �ý����� ��� ����(b1, b2)�� ���Ѵ�.
	Eigen::VectorXf b1(NumIntrVert), b2(NumIntrVert);
	b1.setZero();
	b2.setZero();

	// ������ ��� ������ ���Ͽ�
	for (EgVertex *vj : BndryVerts)
	{
		// ������ 1�� �̿� ������ ���Ͽ�
		std::vector<EgVertex *> Verts;
		vj->GetOneRingVert(Verts);
		for (EgVertex *vi : Verts)
		{
			// ��� ������ �ƴ϶��,
			if (!vi->IsBndry())
			{
				// ����ġ�� ���Ͽ� ��� ���Ϳ� �߰��Ѵ�.
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

	// �����ý����� ��(x1, x2)�� ���Ѵ�.
	st = clock();
	Eigen::VectorXf x1(NumIntrVert), x2(NumIntrVert);
	Eigen::SparseLU<Eigen::SparseMatrix<float, Eigen::RowMajor>> solver;
	solver.compute(A);
	x1 = solver.solve(b1);
	x2 = solver.solve(b2);

	// ���� ������ �Ķ���� ���� �����Ѵ�.
	for (int i = 0; i < NumIntrVert; ++i)
		IntrVerts[i]->m_Pos.Set(x1[i], x2[i], 0.0);

	print_msg("\tUnknowns (u, v) are found(%u ms)...\n", clock() - st);
	print_msg("Unfolding complete...\n\n");

	// ��� ����, ����, ������ ���۸� �����Ѵ�.
	pRetMesh->UpdateBndBox();
	pRetMesh->UpdateNormal(FLAT);
	pRetMesh->m_VertexBuf.clear();
	pRetMesh->m_NormalBuf.clear();

	// �Ű�ȭ�� �޽��� ��ȯ�Ѵ�.
	return pRetMesh;
}
