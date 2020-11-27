#include "..\usr\include\egmesh\egmesh.h"
#include <cstring>
#include <vector>
#include <map>

/*!
*	\biref ������ ���콺 ����� ����Ѵ�.
*	\note ��� ������ ��� ��� �Ұ�
*
*	\param pVert[in] ��� ����
*
*	\return ���� ���콺 ����� ��ȯ�Ѵ�.
*/
double get_gaussian_curvature(EgVertex *pVert)
{
	// ��� ������ ��� 0.0�� ��ȯ�Ѵ�.
	if (pVert->IsBndry())
		return 0.0;

	// ���� �ֺ��� ������ ��� ���Ѵ�.
	double sum_theta = 0.0;
	for (EgEdge *e : pVert->m_pEdges)
		sum_theta += e->m_pNext->GetAngle(true);

	// ���콺 ����� ����Ͽ� ��ȯ�Ѵ�.
	double ret = (2 * M_PI - sum_theta);
	if (EQ(ret, 0.0, 1.0e-6))
		ret = 0.0;
	else
		ret /= ::get_mixed_voronoi_cell_area(pVert);
	return ret;
}

/*!
*	\biref �޽��� ���콺 ����� ����Ѵ�.
*
*	\param pMesh[in] ��� �޽�
*	\param Ks[out] i��° ������ ���콺 ����� Ks[i]�� ����ȴ�.
*	\param min[out] �ּ� ���콺 ����� ����ȴ�.
*	\param max[out] �ִ� ���콺 ����� ����ȴ�.
*/
void get_gaussian_curvature(EgMesh *pMesh, std::vector<double> &Ks, double &min, double &max)
{
	// ������ �ʱ�ȭ �Ѵ�.
	clock_t st = clock();
	min = 1e+10;
	max = 1e-10;
	Ks.clear();

	// ������ ������ ���Ͽ�
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// ���콺 ����� ����Ͽ�
		double k = ::get_gaussian_curvature(v);

		// �ִ�, �ּҸ� ���ϰ� ��� �迭�� �߰��Ѵ�.
		min = MIN(k, min);
		max = MAX(k, max);
		Ks.push_back(k);
	}

	// ���� �ð��� ����� ������ ����Ѵ�.
	printf("Total Elapsed time for GetGaussCurvature = %u ms \n", clock() - st);
	printf("Gaussian Curvature Range = [%.5lf ~ %.5lf]\n", min, max);
}

/*!
*	\biref ������ ��� ����� ���밪�� ����Ѵ�.
*	\note ��� ������ ��� ��� �Ұ�
*
*	\param pVert[in] ��� ����
*
*	\return ���� ��� ����� ���밪�� ��ȯ�Ѵ�.
*/
double get_mean_curvature(EgVertex *pVert)
{
	// ��� ��� ���͸� �����ϰ�,
	EgVec3 L;

	// �������� �����ϴ� ������ ���� e�� ���Ͽ�
	for (EgEdge *e : pVert->m_pEdges)
	{
		// Cotangent ����ġ�� ����Ѵ�.
		double beta = e->GetAngle(true);
		double weight = 0.0;
		if (e->IsBndry())
			weight = cos(beta) / sin(beta);
		else
		{
			double alpha = e->m_pMate->GetAngle(true);
			weight = (cos(alpha) / sin(alpha) + cos(beta) / sin(beta)) / 2.0;
		}

		// ��� ��� ���͸� ����Ѵ�.
		L = L + weight * (POS(EV(e)) - POS(pVert));

		// ��� ������ ���� ����ó���� �Ѵ�.
		if (e->m_pNext->m_pNext->m_pMate == NULL)
		{
			beta = e->m_pNext->m_pNext->GetAngle(true);
			weight = cos(beta) / sin(beta);
			L = L + weight * (POS(EV(e->m_pNext)) - POS(pVert));
		}
	}
	// ���� ��� ��� ���͸� ���� ������ ũ��� ������(����ȭ).
	L /= ::get_mixed_voronoi_cell_area(pVert);

	// ��� ��� ���밪�� ����Ѵ�.
	double ret = 0.5 * norm(L);
	if (EQ(ret, 0.0, 1.0e-7))
		ret = 0.0;

	return ret;
}

/*!
*	\biref �޽� �� ������ ��� ����� ���밪�� ����Ѵ�.
*
*	\param pMesh[in] ��� �޽�
*	\param Hs[out] i��° ������ ��� ����� ���밪�� Hs[i]�� ����ȴ�.
*	\param min[out] �ּ� ��� ����� ���밪�� ����ȴ�.
*	\param max[out] �ִ� ��� ����� ���밪�� ����ȴ�.
*/
void get_mean_curvature(EgMesh *pMesh, std::vector<double> &Hs, double &min, double &max)
{
	// ������ �ʱ�ȭ�Ѵ�.
	clock_t st = clock();
	min = 1e+10;
	max = 1e-10;
	Hs.clear();

	// ������ ������ ���Ͽ�
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// ��� ����� ����Ͽ�
		double h = ::get_mean_curvature(v);

		// �ִ�, �ּҸ� ���ϰ� ��� �迭�� �߰��Ѵ�.
		min = MIN(h, min);
		max = MAX(h, max);
		Hs.push_back(h);
	}

	// ���� �ð��� ����� ������ ����Ѵ�.
	printf("Total Elapsed time for GetMeanCurvature = %u ms \n", clock() - st);
	printf("Mean Curvature Range = [%.5lf ~ %.5lf]\n", min, max);
}

/*!
*	\brief ������ ���� ���� ������� ����Ʈ�� �����Ѵ�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*	\param BndryPts[out] �� ������ ���� ���� ������� ����ȴ�.
*/
void get_mixed_voronoi_cell_bndry(EgMesh *pMesh, std::vector<std::vector<EgPos>> &BndryPts)
{
	for (EgVertex *v : pMesh->m_pVerts)
	{
		std::vector<EgPos> Points;

		// �߽� �������� �����ϴ� ������ ������ ���Ͽ�
		for (EgEdge *e : v->m_pEdges)
		{
			// �ﰢ���� �����ϴ� pqr�� ��ǥ�� ���Ѵ�.
			EgPos p = v->m_Pos;
			EgPos q = e->m_pNext->m_pVert->m_Pos;
			EgPos r = e->m_pNext->m_pNext->m_pVert->m_Pos;

			// ���� p�� q�� ������ �߰��ϰ�
			Points.push_back(lerp(p, q, 0.5));

			// �ܽ�(�а� �ﰢ���̶�� �������� ��ü)�� �߰��ϰ�
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

			// ���� p�� r�� ������ �߰��Ѵ�
			Points.push_back(lerp(p, r, 0.5));
		}
		BndryPts.push_back(Points);
	}
}

/*!
*	\brief ������ ���� ������ ������ ����Ѵ�.
*	\note  ���� ��: Discrete Differential-Geometry Operators for Triangulated 2-Manifolds
*
*	\param v[in] ������ ���� ������
*
*	\return ���� ������ ������ ��ȯ�Ѵ�.
*/
double get_mixed_voronoi_cell_area(EgVertex *v)
{
	// ��� ���� 0���� �ʱ�ȭ �Ѵ�.
	double Area = 0.0;

	// ������ ������ ������ �ﰢ���� ���Ͽ�
	for (EgEdge *e : v->m_pEdges)
	{
		// �ﰢ���� �����ϴ� pqr�� ��ǥ�� ���Ѵ�.
		EgFace *f = e->m_pFace;
		EgPos p = v->m_Pos;
		EgPos q = e->m_pNext->m_pVert->m_Pos;
		EgPos r = e->m_pNext->m_pNext->m_pVert->m_Pos;
		double T = 0.0, cot_q = 0.0, cot_r = 0.0;
		
		// �ﰢ���� ���¸� �м��Ͽ� 
		switch (is_obtuse_tri(p, q, r))
		{
		case 0:	// ���� �ﰢ���̶�� ���γ��� ������ ���ϰ�,
			cot_q = (r - q) * (p - q) / norm((r - q) ^ (p - q));
			cot_r = (p - r) * (q - r) / norm((p - r) ^ (q - r));
			T = (norm_sq(q - p) * cot_r + norm_sq(r - p) * cot_q) / 8.0;
			break;

		case 1: // �� p�� �а��̶�� 
			T = f->GetArea() / 2.0;
			break;

		case 2:	// �� q �Ǵ� r�� �а��̶��
		case 3:
			T = f->GetArea() / 4.0;
			break;
		}

		// �ﰢ���� �κи����� �����Ѵ�.
		Area += T;
	}

	// ���� ������ ������ ��ȯ�Ѵ�.
	return Area;
}

/*!
*	\brief	�޽��� Cotangent ���ö�þ� ����� ����Ѵ�.
*
*	\param	pMesh[in] ��� �޽�
*	\param	D[out] ���ҿ����� ������ ����(1/A_i)�� ����ȴ�.
*	\param	M[out] ���ö�þ� ����� ����ȴ�.
*/
void get_laplacian_matrix(EgMesh *pMesh, Eigen::SparseMatrix<float> &D, Eigen::SparseMatrix<float> &M)
{
	// ���� �ð� ������ ���ؼ� ���� clock�� ���Ѵ�.
	clock_t st = clock();

	// ����� ũ�⸦ �缳���ϰ� 0���� �ʱ�ȭ �Ѵ�.
	int NumVert = pMesh->UpdateVertIdx();
	D.resize(NumVert, NumVert);
	M.resize(NumVert, NumVert);

	// ��� ������ ����ġ�� 0���� �ʱ�ȭ �Ѵ�.
	for (EgFace *f : pMesh->m_pFaces)
		f->m_pEdge->m_Cost = f->m_pEdge->m_pNext->m_Cost = f->m_pEdge->m_pNext->m_pNext->m_Cost = 0.0;

	// ������ ������ ���Ͽ� ����� �������� ���� ����ġ�� ���Ѵ�.
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// ����� �� �ε����� ���ϰ�
		int j = v->m_Idx;

		// ������ ����� ������ ������ ���Ͽ�
		double tot_weight = 0.0;
		for (EgEdge *e : v->m_pEdges)
		{
			// ����� �� �ε����� ���ϰ�
			int i = e->m_pNext->m_pVert->m_Idx;

			// ����ġ(w_ji)�� ������ ���� ���ο� �������
			double w_ji = e->m_Cost;
			if (w_ji == 0.0)
			{
				// ��� ������ �ƴ϶��
				if (!e->IsBndry())
				{
					// �� ������ ����ġ�� ���ϰ�
					double cot_beta = e->GetCotangent();
					double cot_alpha = e->m_pMate->GetCotangent();
					w_ji = (cot_alpha + cot_beta) / 2.0;
					e->m_Cost = e->m_pMate->m_Cost = w_ji;
				}
				else // ��� �������
				{
					// �� ������ ����ġ�� ���ϰ�
					e->m_Cost = w_ji = e->GetCotangent();
				}
			}

			// Column major ������ ����ġ�� �߰��ϰ�, ������ ����Ѵ�.
			M.coeffRef(i, j) = (float)w_ji;	// ������ ��Ī ����̹Ƿ� ��� ����
			tot_weight += w_ji;

			// ��� ������ ���� ����ó���� �Ѵ�.
			if (e->m_pNext->m_pNext->m_pMate == NULL)
			{
				// ����� �� �ε����� ���Ͽ� �� ������ ����ġ�� ���ϰ�
				i = e->m_pNext->m_pNext->m_pVert->m_Idx;				
				w_ji = e->m_pNext->m_pNext->GetCotangent();
				e->m_pNext->m_pNext->m_Cost = w_ji;

				// Column major ������ ����ġ�� �߰��ϰ�, ������ ����Ѵ�.
				M.coeffRef(i, j) = (float)w_ji;
				tot_weight += w_ji;
			}
		}

		// ��� D�� M�� �밢�� ���Ҹ� ����Ѵ�.
		double area = ::get_mixed_voronoi_cell_area(v);
		D.coeffRef(j, j) = 1.0f / (float)area;
		M.coeffRef(j, j) = -(float)tot_weight;
	}

	// ��� ������ ����ġ�� 0���� �ʱ�ȭ �Ѵ�.
	//for (EgFace *f : pMesh->m_pFaces)
	//	f->m_pEdge->m_Cost = f->m_pEdge->m_pNext->m_Cost = f->m_pEdge->m_pNext->m_pNext->m_Cost = 0.0;

	// ���� �ð��� ����Ѵ�.
	printf("\tElapsed time for Laplace matrix construction = %u ms\n", clock() - st);
}

/*!
*	\brief	Ư¡�� �ݿ��� ���ö� ����� ����Ѵ�.
*
*	\param	pMesh[in] ��� �޽�
*	\param	M[out] ���ö�þ� ����� ����ȴ�.
*/
void get_laplacian_matrix_feature(EgMesh *pMesh, Eigen::SparseMatrix<float> &M)
{
	// ���� �ð� ������ ���ؼ� ���� clock�� ���Ѵ�.
	clock_t st = clock();

	// ������ ��(m)�� ���Ͽ�, ���(m by m)�� ũ�⸦ �缳�� �Ѵ�.
	int m = pMesh->UpdateVertIdx();
	M.resize(m, m);

	// ������ ������ ���Ͽ� ���񼺰� ���콺����� �����Ѵ�.
	std::vector<bool> Concavity;
	std::vector<double> Kappa;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		Concavity.push_back(v->IsConcave());
		double k = ::get_gaussian_curvature(v);
		Kappa.push_back(ABS(k)); // ��� �ð� ŭ		
	}

	// ������ ������ ���Ͽ� ����� �������� ���� ����ġ�� ���Ѵ�.
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// ����� �� �ε����� ���ϰ�
		int j = v->m_Idx;

		// ������ ����� ������ ������ ���Ͽ�
		double tot_weight = 0.0;
		for (EgEdge *e : v->m_pEdges)
		{
			// ����� �� �ε����� ���ϰ�
			int i = e->m_pNext->m_pVert->m_Idx;
			double weight = e->GetLength() / (Kappa[j] + Kappa[i] + 1.0e-5);
			if (Concavity[j] || Concavity[i])
				weight *= 0.01;

			// Column major ������ ����ġ�� �߰��ϰ�, ������ ����Ѵ�.
			M.coeffRef(i, j) = (float)weight;
			tot_weight += weight;

			// ��� ������ ���� ����ó���� �Ѵ�.
			if (e->m_pNext->m_pNext->m_pMate == NULL)
			{
				// ����� �� �ε����� ���ϰ�
				i = e->m_pNext->m_pNext->m_pVert->m_Idx;
				double weight = e->m_pNext->m_pNext->GetLength() / (Kappa[j] + Kappa[i] + 1.0e-5);
				if (Concavity[j] || Concavity[i])
					weight *= 0.01;

				// Column major ������ ����ġ�� �߰��ϰ�, ������ ����Ѵ�.
				M.coeffRef(i, j) = (float)weight;
				tot_weight += weight;
			}
		}

		// ��� M�� �밢�� ���Ҹ� ����Ѵ�.
		M.coeffRef(j, j) = -(float)tot_weight;
	}

	// ���� �ð��� ����Ѵ�.
	printf("\tElapsed time for Laplace matrix construction = %u ms\n", clock() - st);
}

/*!
*	\brief	Ư¡�� �ݿ��� ���ö� ����� ����Ѵ�.
*
*	\param	pMesh[in] ��� �޽�
*	\param	Verts[in] �κ� ������ ���� ������ ����Ʈ(������ ���� + ���� ���� ������ �����)
*	\param	M[out] ���ö�þ� ����� ����ȴ�.
*/
void get_laplacian_matrix_feature(EgMesh *pMesh, std::vector<EgVertex *> &Verts, Eigen::SparseMatrix<float> &M)
{
	// ���� �ð� ������ ���ؼ� ���� clock�� ���Ѵ�.
	clock_t st = clock();

	// ��� ������ �ε����� -1�� �ʱ�ȭ �Ѵ�.
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_Idx = -1;

	// �κ� ������ ������ ��(m)�� ���Ͽ�, ���(m by m)�� ũ�⸦ �缳�� �Ѵ�.
	int m = 0;
	for (EgVertex *v : Verts)
		v->m_Idx = m++;
	M.resize(m, m);

	// �κ� ������ ������ ���Ͽ� ���񼺰� ���콺����� �����Ѵ�.
	std::vector<bool> Concavity;
	std::vector<double> Kappa;
	for (EgVertex *v : Verts)
	{
		Concavity.push_back(v->IsConcave());
		double k = ::get_gaussian_curvature(v);
		Kappa.push_back(ABS(k));
	}

	// ������ ������ ���Ͽ� ����� �������� ���� ����ġ�� ���Ѵ�.
	for (EgVertex *v : Verts)
	{
		// ����� �� �ε����� ���ϰ�
		int j = v->m_Idx;

		// ������ ����� ������ ������ ���Ͽ�
		double tot_weight = 0.0;
		for (EgEdge *e : v->m_pEdges)
		{
			// ����� �� �ε����� ���Ͽ� ���� ������ ���Ե� ������ �ƴϸ� �ǳʶ��.
			int i = e->m_pNext->m_pVert->m_Idx;
			if (i == -1)
				continue;

			// Ư¡�� �ݿ��� ����ġ�� ���Ѵ�.
			double w_ji = e->GetLength() / (Kappa[j] + Kappa[i] + 1.0e-5);
			if (Concavity[j] || Concavity[i])
				w_ji *= 0.01;

			// Column major ������ ����ġ�� �߰��ϰ�, ������ ����Ѵ�.
			M.coeffRef(i, j) = (float)w_ji;
			tot_weight += w_ji;

			// ��� ������ ���� ����ó���� �Ѵ�.
			if (e->m_pNext->m_pNext->m_pMate == NULL)
			{
				// ����� �� �ε����� ���Ͽ� ���� ������ ���Ե� ������ �ƴϸ� �ǳʶ��.
				i = e->m_pNext->m_pNext->m_pVert->m_Idx;
				if (i == -1)
					continue;
				
				// Ư¡�� �ݿ��� ����ġ�� ���Ѵ�.
				double w_ji = e->m_pNext->m_pNext->GetLength() / (Kappa[j] + Kappa[i] + 1.0e-5);
				if (Concavity[j] || Concavity[i])
					w_ji *= 0.01;

				// Column major ������ ����ġ�� �߰��ϰ�, ������ ����Ѵ�.
				M.coeffRef(i, j) = (float)w_ji;
				tot_weight += w_ji;
			}
		}

		// ��� M�� �밢�� ���Ҹ� ����Ѵ�.
		M.coeffRef(j, j) = -(float)tot_weight;
	}

	// ������ �ε����� �����ϰ� ���� �ð��� ����Ѵ�.
	pMesh->UpdateVertIdx();
	printf("\tElapsed time for Laplace matrix construction = %u ms\n", clock() - st);
}

/*!
*	\brief	�޽��� �� �ﰢ������ �׷����Ʈ�� ����Ѵ�.
*
*	\param	pMesh[in] ��� �޽�
*	\param	Values[in] �� ������ �Ҵ�� ��
*	\param	Gradients[out] ���� �� �ﰢ���� �׷����Ʈ�� ����ȴ�.
*	\param	bNormalize[in] �׷����Ʈ�� ����ȭ ����
*/
void get_gradient_field(EgMesh *pMesh, std::vector<double> &Values, std::vector<EgVec3> &Gradients, bool bNormalize)
{
	// ����� �ʱ�ȭ �ϰ�
	Gradients.clear();

	// ������ �ﰢ���� ���Ͽ�
	for (EgFace *f : pMesh->m_pFaces)
	{
		// �� ���� v0, v1, v2�� ������ �Ҵ�� �� f0, f1, f2�� ���Ѵ�.
		EgVertex *v0 = f->GetVertex(0);
		EgVertex *v1 = f->GetVertex(1);
		EgVertex *v2 = f->GetVertex(2);
		double f0 = Values[v0->m_Idx];
		double f1 = Values[v1->m_Idx];
		double f2 = Values[v2->m_Idx];

		// �ﰢ���� ������ ���Ͽ� �׷����Ʈ ������ ����Ѵ�.
		EgVec3 N = f->GetFaceNormal(true);
		EgVec3 e0 = v2->m_Pos - v1->m_Pos;
		EgVec3 e1 = v0->m_Pos - v2->m_Pos;
		EgVec3 e2 = v1->m_Pos - v0->m_Pos;
		EgVec3 Grad = f0 * (N ^ e0) + f1 * (N ^ e1) + f2 * (N ^ e2);

		// ����ȭ�� �����Ͽ� ����� �����Ѵ�.
		if (bNormalize)
			Grad.Normalize();
		Gradients.push_back(Grad);
	}
}

