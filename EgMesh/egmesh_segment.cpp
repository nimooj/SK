#pragma once

#include "..\usr\include\egmesh\egmesh.h"


/*!
*	\brief	�޽����� concave�� ������ ã�´�.
*
*	\param	pMesh[in] ��� �޽�
*	\param	Verts[out] concave�� ������ ����ȴ�.
*
*	\return concave�� ������ ������ ��ȯ�Ѵ�.
*/
int get_concave_verts(EgMesh *pMesh, std::vector<EgVertex *> &Verts)
{
	// ������ ����Ʈ�� �ʱ�ȭ �Ѵ�.
	Verts.clear();

	// ������ ������ ���Ͽ�
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// Concave ������ �����ϸ� ����Ʈ�� �߰��Ѵ�.
		if (v->IsConcave())
			Verts.push_back(v);
	}

	// Concave�� ������ ���� ��ȯ�Ѵ�.
	return (int)Verts.size();
}

/*!
*	\brief	�޽����� concave�� �ﰢ���� ã�´�.
*
*	\param	pMesh[in] ��� �޽�
*	\param	Faces[out] concave�� �ﰢ���� ����ȴ�.
*
*	\return concave�� �ﰢ���� ���� ��ȯ�Ѵ�.
*/
int get_concave_faces(EgMesh *pMesh, std::vector<EgFace *> &Faces)
{
	// �ﰢ�� ����Ʈ�� �ʱ�ȭ �Ѵ�.
	Faces.clear();

	// ������ �ﰢ���� ���Ͽ�
	for (EgFace *f : pMesh->m_pFaces)
	{
		int num_concave_verts = 3;
		// ������ ������ ���Ͽ�
		for (int i = 0; i < 3; ++i)
		{
			EgVertex *v = f->GetVertex(i);
			if (!v->IsConcave())
				num_concave_verts--;
		}
		if (num_concave_verts > 1)
			Faces.push_back(f);
	}

	// Concave�� �ﰢ�� ���� ��ȯ�Ѵ�.
	return (int)Faces.size();
}

/*! 
*	\brief �޽��� ���� �������� �ϸ�� �ʵ带 �����Ѵ�. 
*
*	\param pMesh[in] ��� �޽�
*	\param OneVerts[in] ���ప 1�� ���� ������ ����Ʈ
*	\param HalfVerts[in] ���ప 0.5�� ���� ������ ����Ʈ
*	\param ZeroVerts[in] ���ప 0�� ���� ������ ����Ʈ
*	\param Field[out] i��° ������ �ϸ�� �ʵ尪�� Field[i]�� ����ȴ�.
*	\param bBndryCstr[in] ��� ������ �ڵ� ���� ����(0.5) ��� ����
*	\param bFeature[in] Ư¡ ��� �ʵ� ���� ����
*/
void get_harmonic_field(
	EgMesh *pMesh, 
	std::vector<EgVertex *> &OneVerts,
	std::vector<EgVertex *> &HalfVerts,
	std::vector<EgVertex *> &ZeroVerts,
	std::vector<double> &Field, 
	bool bBndryCstr, 
	bool bFeature)
{
	printf("Computing mesh harmonic field...\n");
	Field.clear();

	// ���� ������ �ߺ��� �����ϰ� �ʱ�ȭ �Ѵ�.
	UNIQUE(OneVerts);
	UNIQUE(ZeroVerts);
	UNIQUE(HalfVerts);
	if (bBndryCstr)
	{
		std::vector<EgVertex *> BndryVerts;
		::get_bndry_verts(pMesh, BndryVerts);
		HalfVerts.insert(HalfVerts.end(), BndryVerts.begin(), BndryVerts.end());
	}
	
	// ���� ������ ���� ���Ѵ�.
	int n = NUM(OneVerts) + NUM(HalfVerts) + NUM(ZeroVerts);
	
	// ���� ������ �Ҵ�� ������ ���͸� �����Ѵ�.
	Eigen::VectorXf y(n);

	// 1 ���� ���� ���� ������ �߰��Ѵ�.
	int idx = 0;
	for (EgVertex *v : OneVerts)
	{
		v->m_Idx = -1;
		y.coeffRef(idx++) = 1.0f;
	}

	// 0.5 ���� ���� ���� ������ �߰��Ѵ�.
	for (EgVertex *v : HalfVerts)
	{
		v->m_Idx = -1;
		y.coeffRef(idx++) = 0.5f;
	}

	// 0 ���� ���� ���� ������ �߰��Ѵ�.	
	for (EgVertex *v : ZeroVerts)
	{
		v->m_Idx = -1;
		y.coeffRef(idx++) = 0.0f;
	}

	// ���� ������ ���� ������ ����Ʈ�� �����Ѵ�.
	std::vector<EgVertex *> NewVerts;
	for (EgVertex *v : pMesh->m_pVerts)
		if (v->m_Idx != -1)
			NewVerts.push_back(v);

	// ���� ������ ���� ������ ���� ���Ѵ�.
	int m = (int)NewVerts.size();

	// ���� ���� �������� ���ʿ� �߰��Ѵ�.
	NewVerts.insert(NewVerts.end(), OneVerts.begin(), OneVerts.end());
	NewVerts.insert(NewVerts.end(), HalfVerts.begin(), HalfVerts.end());
	NewVerts.insert(NewVerts.end(), ZeroVerts.begin(), ZeroVerts.end());

	// �޽��� ���� ������ ������ ���� + ���� ������ ������ ��迭�Ѵ�.
	pMesh->m_pVerts.swap(NewVerts);

	// ���� ������ ���� ����Ѵ�.
	printf("\tNumber of free vertices = %d\n", m);
	printf("\tNumber of constraint vertices = %d\n", n);
	printf("\t\tNumber of vertices with constraint 1.0 = %d\n", NUM(OneVerts));
	printf("\t\tNumber of vertices with constraint 0.5 = %d\n", NUM(HalfVerts));
	printf("\t\tNumber of vertices with constraint 0.0 = %d\n", NUM(ZeroVerts));

	// Laplace ����� ���Ѵ�.
	Eigen::SparseMatrix<float> D, M;	// M = (m + n) by (m + n)
	if (bFeature)
		::get_laplacian_matrix_feature(pMesh, M);
	else
		::get_laplacian_matrix(pMesh, D, M);

	// ���� �ý��� Ax = b�� �����ϱ� ���� ��� ��� A�� ����Ѵ�.
	Eigen::SparseMatrix<float> A, B;
	A = M.topLeftCorner(m, m);
	B = M.topRightCorner(m, n);
	A.makeCompressed();
	B.makeCompressed();

	// ���� �ý��� Ax = b�� ��� ���� b�� ���Ѵ�.
	Eigen::VectorXf b(m);
	b = -B * y;
	printf("\tLaplace system (Ax = b) is constructed...\n");

	// �����ý��� Ax = b�� �ظ� ���Ѵ�.
	clock_t st = clock();
	Eigen::VectorXf x(m);
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>> solver;
	solver.compute(A);
	x = solver.solve(b);
	printf("\tElapsed time for Ax = b is %u ms\n", clock() - st);

	// ���� �ϸ�� �ʵ��� ���� ����Ʈ�� �����Ѵ�.
	for (int i = 0; i < m; ++i)
		Field.push_back((double)x[i]);
	for (int i = 0; i < n; ++i)
		Field.push_back((double)y[i]);

	printf("Computing mesh harmonic field complete...\n");
}

/*!
*	\brief ���� �ֺ� ������ �ϸ�� �ʵ带 �����Ѵ�.
*
*	\param pMesh[in] ��� �޽�
*	\param pVert[in] ���� ����
*	\param NumRing[in] �ֺ� ������ �� ��
*	\param Field[out] i��° ������ �ϸ�� �ʵ尪�� Field[i]�� ����ȴ�.
*/
void get_harmonic_field(EgMesh *pMesh, EgVertex *pVert, int NumRing, std::vector<double> &Field)
{
	// ��� �޽��� �ٲ���ų�, �ű� ȣ���̶�� �ʵ尪�� �ʱ�ȭ �Ѵ�.
	if (Field.size() != pMesh->GetNumVert())
	{
		Field.swap(std::vector<double>());
		Field.assign(pMesh->GetNumVert(), 0.0);
	}

	// ���� �ֺ��� �ĺ� ���� ������ ���Ѵ�.
	std::vector<std::vector<EgVertex *>> RingVerts;
	int N = ::get_neighbor_verts(pMesh, pVert, NumRing, RingVerts);

	// �ĺ� ���� ������ ������ (������ ����, ���� ����(1, 0)) ������ ��迭 �Ѵ�.
	std::vector<EgVertex *> SltVerts;

	// ���� ������ ���� ������ �߰��ϰ�, �� ��(m)�� ���Ѵ�.
	int k = (int)(N * 0.1);
	k = MAX(k, 1);
	for (int i = k; i < N; ++i)
		for (EgVertex *v : RingVerts[i])
			SltVerts.push_back(v);
	int m = (int)SltVerts.size();

	// ���� ������ 1�� ������ �߰��ϰ�, �� ��(l)�� ���Ѵ�.
	for (int i = 0; i < k; ++i)
		for (EgVertex *v : RingVerts[i])
			SltVerts.push_back(v);
	int l = (int)SltVerts.size() - m;

	// ���� ������ 0�� ������ �߰��ϰ�, �� ����(n) ���Ѵ�.
	for (EgVertex *v : RingVerts[N])
		SltVerts.push_back(v);
	int n = (int)SltVerts.size() - m - l;

	// ���� ������ ��(1, 0)���� ���͸� �����Ѵ�.
	Eigen::VectorXf y(l + n);
	for (int i = 0; i < l + n; ++i)
		y.coeffRef(i) = (i < l) ? 1.0f : 0.0f;

	// Laplace ����� ���Ѵ�.
	Eigen::SparseMatrix<float> M;	// M = (m + l + n) by (m + l + n)
	::get_laplacian_matrix_feature(pMesh, SltVerts, M);

	// ���� �ý��� Ax = b�� �����ϱ� ���� ��� ��� A�� ����Ѵ�.
	Eigen::SparseMatrix<float> A, B;
	A = M.topLeftCorner(m, m);
	B = M.topRightCorner(m, l + n);
	A.makeCompressed();
	B.makeCompressed();

	// ���� �ý��� Ax = b�� ��� ���� b�� ���Ѵ�.
	Eigen::VectorXf b(m);
	b = -B * y;

	// �����ý��� Ax = b�� �ظ� ���Ѵ�.
	clock_t st = clock();
	Eigen::VectorXf x(m);
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>> solver;
	solver.compute(A);
	x = solver.solve(b);
	printf("\tElapsed time for Ax = b is %u ms\n", clock() - st);

	// ���ο� ���� ������ �� ū ���� �����Ѵ�.
	for (int i = 0; i < m + l; ++i)
	{
		int vidx = SltVerts[i]->m_Idx;
		Field[vidx] = (i < m) ? MAX(Field[vidx], (double)x(i)) : 1.0;
	}

	// �ϸ�� �ʵ� ����ȭ�� ���� �ڵ�
	//pMesh->m_ShadeType = SHADE_BY_VERT_COLOR;
	//for (EgVertex *v : pMesh->m_pVerts)
	//	v->m_Color.Set(0.5, 0.5, 0.5);
	//for (int i = 0; i < m + l; ++i)
	//	SltVerts[i]->m_Color = (i < m) ? ::get_color_map_from_rainbow(x[i]) : get_color_map_from_rainbow(1.0);
}

/*!
*	\brief ������ �ϸ�� �ʵ�� ������ �����Ѵ�.
*
*	\param pMesh[in] ��� �޽�
*	\param HF[in] ������ �ϸ�� �ʵ�
*	\param Thres[in] ���� ������ �����ϱ� ���� �ϸ�� �ʵ��� �Ӱ谪(Thres �̻��� ������ ���õ�)
*	\param SltFaces[out] ���õ� �ﰢ���� ����ȴ�.
*	\param SltBndryPts[out] ���� ������� ����ȴ�.
*/
void select_region_by_harmonic_field(EgMesh *pMesh, std::vector<double> &HF, double Thres, 
									 std::vector<EgFace *> &SltFaces, std::vector<EgPos> &SltBndryPts)
{
	// ����� �ʱ�ȭ �Ѵ�.
	SltFaces.clear();
	SltBndryPts.clear();

	// ������ �ﰢ���� ���Ͽ�
	for (EgFace *f : pMesh->m_pFaces)
	{
		// �� ������ �ϸ�� �ʵ尪�� �Ӱ谪 �̻��̸� ���� �ﰢ���� �߰��Ѵ�.
		EgVertex *v0 = f->GetVertex(0);
		EgVertex *v1 = f->GetVertex(1);
		EgVertex *v2 = f->GetVertex(2);
		double w0 = HF[v0->m_Idx];
		double w1 = HF[v1->m_Idx];
		double w2 = HF[v2->m_Idx];
		if (w0 > Thres && w1 > Thres && w2 > Thres)
			SltFaces.push_back(f);

		EgPos p, q;
		// ���� v0�� ���� ������ ���Ե� ���
		if (w0 > Thres && w1 < Thres && w2 < Thres)
		{
			double s = (w0 - Thres) / (w0 - w1);
			double t = (w0 - Thres) / (w0 - w2);
			p = v0->m_Pos + s * (v1->m_Pos - v0->m_Pos);
			q = v0->m_Pos + t * (v2->m_Pos - v0->m_Pos);
		}

		// ���� v1�� ���� ������ ���Ե� ���
		if (w1 > Thres && w2 < Thres && w0 < Thres)
		{
			double s = (w1 - Thres) / (w1 - w2);
			double t = (w1 - Thres) / (w1 - w0);
			p = v1->m_Pos + s * (v2->m_Pos - v1->m_Pos);
			q = v1->m_Pos + t * (v0->m_Pos - v1->m_Pos);
		}

		// ���� v2�� ���� ������ ���Ե� ���
		if (w2 > Thres && w0 < Thres && w1 < Thres)
		{
			double s = (w2 - Thres) / (w2 - w0);
			double t = (w2 - Thres) / (w2 - w1);
			p = v2->m_Pos + s * (v0->m_Pos - v2->m_Pos);
			q = v2->m_Pos + t * (v1->m_Pos - v2->m_Pos);
		}

		// ���� (v1, v2)�� ���� ������ ���Ե� ���
		if (w1 > Thres && w2 > Thres && w0 < Thres)
		{
			double s = (w2 - Thres) / (w2 - w0);
			double t = (w1 - Thres) / (w1 - w0);
			p = v2->m_Pos + s * (v0->m_Pos - v2->m_Pos);
			q = v1->m_Pos + t * (v0->m_Pos - v1->m_Pos);
		}

		// ���� (v0, v2)�� ���� ������ ���Ե� ���
		if (w0 > Thres && w2 > Thres && w1 < Thres)
		{
			double s = (w0 - Thres) / (w0 - w1);
			double t = (w2 - Thres) / (w2 - w1);
			p = v0->m_Pos + s * (v1->m_Pos - v0->m_Pos);
			q = v2->m_Pos + t * (v1->m_Pos - v2->m_Pos);
		}

		// ���� (v0, v1)�� ���� ������ ���Ե� ���
		if (w0 > Thres && w1 > Thres && w2 < Thres)
		{
			double s = (w1 - Thres) / (w1 - w2);
			double t = (w0 - Thres) / (w0 - w2);
			p = v1->m_Pos + s * (v2->m_Pos - v1->m_Pos);
			q = v0->m_Pos + t * (v2->m_Pos - v0->m_Pos);
		}

		// ���� ������ ��� ������ �߰��Ѵ�.
		SltBndryPts.push_back(p);
		SltBndryPts.push_back(q);
	}
}

/*!
*	\brief �޽��� Ư¡ ������ �����Ѵ�.
*
*	\param pMesh[in] ��� �޽�
*	\param pVert[in] ������ ���� ���� ������ ���� ������
*	\param NumRing[in] �ĺ� ������ �����ϴ� �ֺ� ������ ��(ring) ��
*	\param Thres[in] ���� ������ �����ϱ� ���� �ϸ�� �ʵ��� �Ӱ谪(Thres �̻��� ������ ���õ�)
*	\param Option[in] ���� �ɼ��� ��Ÿ����(0: ���ο� ����, 1: �߰� ����, 2: ���� ����)
*	\param SltFaces[out] ���õ� �ﰢ���� ����ȴ�.
*	\param SltBndryPts[out] ���� ��� ������ ����ȴ�.
*/
void select_region_by_harmonic_field(
	EgMesh *pMesh, 
	EgVertex *pVert, 
	int NumRing,
	double Thres,
	int Option,
	std::vector<EgFace *> &SltFaces,
	std::vector<EgPos> &SltBndryPts)
{
	// ������ �ɼ��̸� �����Ѵ�.
	if (Option < 0 || Option > 2 || NumRing < 1)
	{
		printf("Not supported option...\n");
		return;
	}

	// ����� �ʱ�ȭ �Ѵ�.
	SltFaces.swap(std::vector<EgFace *>());
	SltBndryPts.swap(std::vector<EgPos>());

	// ���ο� �����̶�� ������ ����� ������ �ϸ�� �ʵ� ���� �ʱ�ȭ �Ѵ�.
	if (Option == 0)
		for (EgVertex *v : pMesh->m_pVerts)
			v->m_Color[0] = 0.0;
	
	// ���� �ֺ��� �ĺ� ���� ������ ���Ѵ�.
	std::vector<std::vector<EgVertex *>> RingVerts;
	int N = ::get_neighbor_verts(pMesh, pVert, NumRing, RingVerts);

	// �ĺ� ���� ������ ������ (������ ����, ���� ����(1, 0)) ������ ��迭 �Ѵ�.
	std::vector<EgVertex *> SltVerts;

	// ���� ������ ���� ������ �߰��ϰ�, �� ��(m)�� ���Ѵ�.
	int k = (int)(N * 0.1);
	k = MAX(k, 1);
	for (int i = k; i < N; ++i)
		for (EgVertex *v : RingVerts[i])
			SltVerts.push_back(v);
	int m = (int)SltVerts.size();

	// ���� ������ 1�� ������ �߰��ϰ�, �� ��(l)�� ���Ѵ�.
	for (int i = 0; i < k; ++i)
		for (EgVertex *v : RingVerts[i])
			SltVerts.push_back(v);
	int l = (int)SltVerts.size() - m;

	// ���� ������ 0�� ������ �߰��ϰ�, �� ����(n) ���Ѵ�.
	for (EgVertex *v : RingVerts[N])
		SltVerts.push_back(v);
	int n = (int)SltVerts.size() - m - l;

	// ���� ������ ��(1, 0)���� ���͸� �����Ѵ�.
	Eigen::VectorXf y(l + n);
	for (int i = 0; i < l + n; ++i)
		y.coeffRef(i) = (i < l) ? 1.0f : 0.0f;

	// Laplace ����� ���Ѵ�.
	Eigen::SparseMatrix<float> M;	// M = (m + l + n) by (m + l + n)
	::get_laplacian_matrix_feature(pMesh, SltVerts, M);

	// ���� �ý��� Ax = b�� �����ϱ� ���� ��� ��� A�� ����Ѵ�.
	Eigen::SparseMatrix<float> A, B;
	A = M.topLeftCorner(m, m);
	B = M.topRightCorner(m, l + n);
	A.makeCompressed();
	B.makeCompressed();

	// ���� �ý��� Ax = b�� ��� ���� b�� ���Ѵ�.
	Eigen::VectorXf b(m);
	b = -B * y;

	// �����ý��� Ax = b�� �ظ� ���Ѵ�.
	clock_t st = clock();
	Eigen::VectorXf x(m);
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>> solver;
	solver.compute(A);
	x = solver.solve(b);
	printf("\tElapsed time for Ax = b is %u ms\n", clock() - st);

	// ���� ���� �ɼ��̶��
	if (Option == 2)
	{
		for (int i = 0; i < m + l; ++i)
			SltVerts[i]->m_Color[0] = 0.0;
	}
	else
	{
		// ���ο� ���� ������ �� ū ���� �����Ѵ�.
		for (int i = 0; i < m + l; ++i) 
			SltVerts[i]->m_Color[0] = (i < m) ? MAX(SltVerts[i]->m_Color[0], (double)x(i)) : 1.0;
	}
	
	// ������ �ﰢ���� ���Ͽ�
	for (EgFace *f : pMesh->m_pFaces)
	{
		// �� ������ �ϸ�� �ʵ尪�� �Ӱ谪 �̻��̸� ���� �ﰢ���� �߰��Ѵ�.
		EgVertex *v0 = f->GetVertex(0);
		EgVertex *v1 = f->GetVertex(1);
		EgVertex *v2 = f->GetVertex(2);
		double w0 = v0->m_Color[0];
		double w1 = v1->m_Color[0];
		double w2 = v2->m_Color[0];
		if (w0 > Thres && w1 > Thres && w2 > Thres)
			SltFaces.push_back(f);		

		EgPos p, q;
		// ���� v0�� ���� ������ ���Ե� ���
		if (w0 > Thres && w1 < Thres && w2 < Thres)
		{
			double s = (w0 - Thres) / (w0 - w1);
			double t = (w0 - Thres) / (w0 - w2);
			p = v0->m_Pos + s * (v1->m_Pos - v0->m_Pos);
			q = v0->m_Pos + t * (v2->m_Pos - v0->m_Pos);
		}

		// ���� v1�� ���� ������ ���Ե� ���
		if (w1 > Thres && w2 < Thres && w0 < Thres)
		{
			double s = (w1 - Thres) / (w1 - w2);
			double t = (w1 - Thres) / (w1 - w0);
			p = v1->m_Pos + s * (v2->m_Pos - v1->m_Pos);
			q = v1->m_Pos + t * (v0->m_Pos - v1->m_Pos);
		}

		// ���� v2�� ���� ������ ���Ե� ���
		if (w2 > Thres && w0 < Thres && w1 < Thres)
		{
			double s = (w2 - Thres) / (w2 - w0);
			double t = (w2 - Thres) / (w2 - w1);
			p = v2->m_Pos + s * (v0->m_Pos - v2->m_Pos);
			q = v2->m_Pos + t * (v1->m_Pos - v2->m_Pos);
		}

		// ���� (v1, v2)�� ���� ������ ���Ե� ���
		if (w1 > Thres && w2 > Thres && w0 < Thres)
		{
			double s = (w2 - Thres) / (w2 - w0);
			double t = (w1 - Thres) / (w1 - w0);
			p = v2->m_Pos + s * (v0->m_Pos - v2->m_Pos);
			q = v1->m_Pos + t * (v0->m_Pos - v1->m_Pos);
		}

		// ���� (v0, v2)�� ���� ������ ���Ե� ���
		if (w0 > Thres && w2 > Thres && w1 < Thres)
		{
			double s = (w0 - Thres) / (w0 - w1);
			double t = (w2 - Thres) / (w2 - w1);
			p = v0->m_Pos + s * (v1->m_Pos - v0->m_Pos);
			q = v2->m_Pos + t * (v1->m_Pos - v2->m_Pos);
		}

		// ���� (v0, v1)�� ���� ������ ���Ե� ���
		if (w0 > Thres && w1 > Thres && w2 < Thres)
		{
			double s = (w1 - Thres) / (w1 - w2);
			double t = (w0 - Thres) / (w0 - w2);
			p = v1->m_Pos + s * (v2->m_Pos - v1->m_Pos);
			q = v0->m_Pos + t * (v2->m_Pos - v0->m_Pos);
		}

		// ���� ������ ��� ������ �߰��Ѵ�.
		SltBndryPts.push_back(p);
		SltBndryPts.push_back(q);
	}

	// �ϸ�� �ʵ� ����ȭ�� ���� �ڵ�
	//pMesh->m_ShadeType = SHADE_BY_VERT_COLOR;
	//for (EgVertex *v : pMesh->m_pVerts)
	//	v->m_Color.Set(0.5, 0.5, 0.5);
	//for (int i = 0; i < m; ++i)
	//	SltVerts[i]->m_Color = ::get_color_map_from_rainbow(x[i]);
}
