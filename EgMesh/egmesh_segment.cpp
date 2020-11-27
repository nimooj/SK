#pragma once

#include "..\usr\include\egmesh\egmesh.h"


/*!
*	\brief	메쉬에서 concave한 정점을 찾는다.
*
*	\param	pMesh[in] 대상 메쉬
*	\param	Verts[out] concave한 정점이 저장된다.
*
*	\return concave한 정점의 개수를 반환한다.
*/
int get_concave_verts(EgMesh *pMesh, std::vector<EgVertex *> &Verts)
{
	// 정점의 리스트를 초기화 한다.
	Verts.clear();

	// 각각의 정점에 대하여
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// Concave 조건을 만족하면 리스트에 추가한다.
		if (v->IsConcave())
			Verts.push_back(v);
	}

	// Concave한 정점의 수를 반환한다.
	return (int)Verts.size();
}

/*!
*	\brief	메쉬에서 concave한 삼각형을 찾는다.
*
*	\param	pMesh[in] 대상 메쉬
*	\param	Faces[out] concave한 삼각형이 저장된다.
*
*	\return concave한 삼각형의 수를 반환한다.
*/
int get_concave_faces(EgMesh *pMesh, std::vector<EgFace *> &Faces)
{
	// 삼각형 리스트를 초기화 한다.
	Faces.clear();

	// 각각의 삼각형에 대하여
	for (EgFace *f : pMesh->m_pFaces)
	{
		int num_concave_verts = 3;
		// 각각의 정점에 대하여
		for (int i = 0; i < 3; ++i)
		{
			EgVertex *v = f->GetVertex(i);
			if (!v->IsConcave())
				num_concave_verts--;
		}
		if (num_concave_verts > 1)
			Faces.push_back(f);
	}

	// Concave한 삼각형 수를 반환한다.
	return (int)Faces.size();
}

/*! 
*	\brief 메쉬의 제약 정점으로 하모닉 필드를 구성한다. 
*
*	\param pMesh[in] 대상 메쉬
*	\param OneVerts[in] 제약값 1을 갖는 정점의 리스트
*	\param HalfVerts[in] 제약값 0.5을 갖는 정점의 리스트
*	\param ZeroVerts[in] 제약값 0을 갖는 정점의 리스트
*	\param Field[out] i번째 정점의 하모닉 필드값이 Field[i]에 저장된다.
*	\param bBndryCstr[in] 경계 정점의 자동 제약 조건(0.5) 사용 여부
*	\param bFeature[in] 특징 기반 필드 생성 여부
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

	// 제약 정점의 중복을 제거하고 초기화 한다.
	UNIQUE(OneVerts);
	UNIQUE(ZeroVerts);
	UNIQUE(HalfVerts);
	if (bBndryCstr)
	{
		std::vector<EgVertex *> BndryVerts;
		::get_bndry_verts(pMesh, BndryVerts);
		HalfVerts.insert(HalfVerts.end(), BndryVerts.begin(), BndryVerts.end());
	}
	
	// 제약 정점의 수를 구한다.
	int n = NUM(OneVerts) + NUM(HalfVerts) + NUM(ZeroVerts);
	
	// 제약 정점에 할당된 값으로 벡터를 생성한다.
	Eigen::VectorXf y(n);

	// 1 값을 갖는 제약 정점을 추가한다.
	int idx = 0;
	for (EgVertex *v : OneVerts)
	{
		v->m_Idx = -1;
		y.coeffRef(idx++) = 1.0f;
	}

	// 0.5 값을 갖는 제약 정점을 추가한다.
	for (EgVertex *v : HalfVerts)
	{
		v->m_Idx = -1;
		y.coeffRef(idx++) = 0.5f;
	}

	// 0 값을 갖는 제약 정점을 추가한다.	
	for (EgVertex *v : ZeroVerts)
	{
		v->m_Idx = -1;
		y.coeffRef(idx++) = 0.0f;
	}

	// 제약 조건이 없는 정점의 리스트를 생성한다.
	std::vector<EgVertex *> NewVerts;
	for (EgVertex *v : pMesh->m_pVerts)
		if (v->m_Idx != -1)
			NewVerts.push_back(v);

	// 제약 조건이 없는 정점의 수를 구한다.
	int m = (int)NewVerts.size();

	// 제약 조건 정점들을 뒤쪽에 추가한다.
	NewVerts.insert(NewVerts.end(), OneVerts.begin(), OneVerts.end());
	NewVerts.insert(NewVerts.end(), HalfVerts.begin(), HalfVerts.end());
	NewVerts.insert(NewVerts.end(), ZeroVerts.begin(), ZeroVerts.end());

	// 메쉬의 정점 집합을 비제약 정점 + 제약 정점의 순으로 재배열한다.
	pMesh->m_pVerts.swap(NewVerts);

	// 제약 정점의 수를 출력한다.
	printf("\tNumber of free vertices = %d\n", m);
	printf("\tNumber of constraint vertices = %d\n", n);
	printf("\t\tNumber of vertices with constraint 1.0 = %d\n", NUM(OneVerts));
	printf("\t\tNumber of vertices with constraint 0.5 = %d\n", NUM(HalfVerts));
	printf("\t\tNumber of vertices with constraint 0.0 = %d\n", NUM(ZeroVerts));

	// Laplace 행렬을 구한다.
	Eigen::SparseMatrix<float> D, M;	// M = (m + n) by (m + n)
	if (bFeature)
		::get_laplacian_matrix_feature(pMesh, M);
	else
		::get_laplacian_matrix(pMesh, D, M);

	// 선형 시스템 Ax = b를 구성하기 위한 계수 행렬 A를 계산한다.
	Eigen::SparseMatrix<float> A, B;
	A = M.topLeftCorner(m, m);
	B = M.topRightCorner(m, n);
	A.makeCompressed();
	B.makeCompressed();

	// 선형 시스템 Ax = b의 상수 벡터 b를 구한다.
	Eigen::VectorXf b(m);
	b = -B * y;
	printf("\tLaplace system (Ax = b) is constructed...\n");

	// 선형시스템 Ax = b의 해를 구한다.
	clock_t st = clock();
	Eigen::VectorXf x(m);
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>> solver;
	solver.compute(A);
	x = solver.solve(b);
	printf("\tElapsed time for Ax = b is %u ms\n", clock() - st);

	// 계산된 하모닉 필드의 값을 리스트에 저장한다.
	for (int i = 0; i < m; ++i)
		Field.push_back((double)x[i]);
	for (int i = 0; i < n; ++i)
		Field.push_back((double)y[i]);

	printf("Computing mesh harmonic field complete...\n");
}

/*!
*	\brief 정점 주변 영역의 하모닉 필드를 구성한다.
*
*	\param pMesh[in] 대상 메쉬
*	\param pVert[in] 선택 정점
*	\param NumRing[in] 주변 영역의 링 수
*	\param Field[out] i번째 정점의 하모닉 필드값이 Field[i]에 저장된다.
*/
void get_harmonic_field(EgMesh *pMesh, EgVertex *pVert, int NumRing, std::vector<double> &Field)
{
	// 대상 메쉬가 바뀌었거나, 신규 호출이라면 필드값을 초기화 한다.
	if (Field.size() != pMesh->GetNumVert())
	{
		Field.swap(std::vector<double>());
		Field.assign(pMesh->GetNumVert(), 0.0);
	}

	// 정점 주변의 후보 선택 영역을 구한다.
	std::vector<std::vector<EgVertex *>> RingVerts;
	int N = ::get_neighbor_verts(pMesh, pVert, NumRing, RingVerts);

	// 후보 선택 영역의 정점을 (비제약 정점, 제약 정점(1, 0)) 순으로 재배열 한다.
	std::vector<EgVertex *> SltVerts;

	// 제약 조건이 없는 정점을 추가하고, 그 수(m)를 구한다.
	int k = (int)(N * 0.1);
	k = MAX(k, 1);
	for (int i = k; i < N; ++i)
		for (EgVertex *v : RingVerts[i])
			SltVerts.push_back(v);
	int m = (int)SltVerts.size();

	// 제약 조건이 1인 정점을 추가하고, 그 수(l)를 구한다.
	for (int i = 0; i < k; ++i)
		for (EgVertex *v : RingVerts[i])
			SltVerts.push_back(v);
	int l = (int)SltVerts.size() - m;

	// 제약 조건이 0인 정점을 추가하고, 그 수를(n) 구한다.
	for (EgVertex *v : RingVerts[N])
		SltVerts.push_back(v);
	int n = (int)SltVerts.size() - m - l;

	// 제약 정점의 값(1, 0)으로 벡터를 생성한다.
	Eigen::VectorXf y(l + n);
	for (int i = 0; i < l + n; ++i)
		y.coeffRef(i) = (i < l) ? 1.0f : 0.0f;

	// Laplace 행렬을 구한다.
	Eigen::SparseMatrix<float> M;	// M = (m + l + n) by (m + l + n)
	::get_laplacian_matrix_feature(pMesh, SltVerts, M);

	// 선형 시스템 Ax = b를 구성하기 위한 계수 행렬 A를 계산한다.
	Eigen::SparseMatrix<float> A, B;
	A = M.topLeftCorner(m, m);
	B = M.topRightCorner(m, l + n);
	A.makeCompressed();
	B.makeCompressed();

	// 선형 시스템 Ax = b의 상수 벡터 b를 구한다.
	Eigen::VectorXf b(m);
	b = -B * y;

	// 선형시스템 Ax = b의 해를 구한다.
	clock_t st = clock();
	Eigen::VectorXf x(m);
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>> solver;
	solver.compute(A);
	x = solver.solve(b);
	printf("\tElapsed time for Ax = b is %u ms\n", clock() - st);

	// 새로운 값과 기존값 중 큰 값을 선택한다.
	for (int i = 0; i < m + l; ++i)
	{
		int vidx = SltVerts[i]->m_Idx;
		Field[vidx] = (i < m) ? MAX(Field[vidx], (double)x(i)) : 1.0;
	}

	// 하모닉 필드 가시화를 위한 코드
	//pMesh->m_ShadeType = SHADE_BY_VERT_COLOR;
	//for (EgVertex *v : pMesh->m_pVerts)
	//	v->m_Color.Set(0.5, 0.5, 0.5);
	//for (int i = 0; i < m + l; ++i)
	//	SltVerts[i]->m_Color = (i < m) ? ::get_color_map_from_rainbow(x[i]) : get_color_map_from_rainbow(1.0);
}

/*!
*	\brief 생성된 하모닉 필드로 영역을 선택한다.
*
*	\param pMesh[in] 대상 메쉬
*	\param HF[in] 생성된 하모닉 필드
*	\param Thres[in] 선택 영역을 결정하기 위한 하모닉 필드의 임계값(Thres 이상인 정점만 선택됨)
*	\param SltFaces[out] 선택된 삼각형이 저장된다.
*	\param SltBndryPts[out] 선택 경계점이 저장된다.
*/
void select_region_by_harmonic_field(EgMesh *pMesh, std::vector<double> &HF, double Thres, 
									 std::vector<EgFace *> &SltFaces, std::vector<EgPos> &SltBndryPts)
{
	// 결과를 초기화 한다.
	SltFaces.clear();
	SltBndryPts.clear();

	// 각각의 삼각형에 대하여
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 세 정점의 하모닉 필드값이 임계값 이상이면 선택 삼각형에 추가한다.
		EgVertex *v0 = f->GetVertex(0);
		EgVertex *v1 = f->GetVertex(1);
		EgVertex *v2 = f->GetVertex(2);
		double w0 = HF[v0->m_Idx];
		double w1 = HF[v1->m_Idx];
		double w2 = HF[v2->m_Idx];
		if (w0 > Thres && w1 > Thres && w2 > Thres)
			SltFaces.push_back(f);

		EgPos p, q;
		// 정점 v0가 선택 영역에 포함된 경우
		if (w0 > Thres && w1 < Thres && w2 < Thres)
		{
			double s = (w0 - Thres) / (w0 - w1);
			double t = (w0 - Thres) / (w0 - w2);
			p = v0->m_Pos + s * (v1->m_Pos - v0->m_Pos);
			q = v0->m_Pos + t * (v2->m_Pos - v0->m_Pos);
		}

		// 정점 v1이 선택 영역에 포함된 경우
		if (w1 > Thres && w2 < Thres && w0 < Thres)
		{
			double s = (w1 - Thres) / (w1 - w2);
			double t = (w1 - Thres) / (w1 - w0);
			p = v1->m_Pos + s * (v2->m_Pos - v1->m_Pos);
			q = v1->m_Pos + t * (v0->m_Pos - v1->m_Pos);
		}

		// 정점 v2가 선택 영역에 포함된 경우
		if (w2 > Thres && w0 < Thres && w1 < Thres)
		{
			double s = (w2 - Thres) / (w2 - w0);
			double t = (w2 - Thres) / (w2 - w1);
			p = v2->m_Pos + s * (v0->m_Pos - v2->m_Pos);
			q = v2->m_Pos + t * (v1->m_Pos - v2->m_Pos);
		}

		// 정점 (v1, v2)가 선택 영역에 포함된 경우
		if (w1 > Thres && w2 > Thres && w0 < Thres)
		{
			double s = (w2 - Thres) / (w2 - w0);
			double t = (w1 - Thres) / (w1 - w0);
			p = v2->m_Pos + s * (v0->m_Pos - v2->m_Pos);
			q = v1->m_Pos + t * (v0->m_Pos - v1->m_Pos);
		}

		// 정점 (v0, v2)가 선택 영역에 포함된 경우
		if (w0 > Thres && w2 > Thres && w1 < Thres)
		{
			double s = (w0 - Thres) / (w0 - w1);
			double t = (w2 - Thres) / (w2 - w1);
			p = v0->m_Pos + s * (v1->m_Pos - v0->m_Pos);
			q = v2->m_Pos + t * (v1->m_Pos - v2->m_Pos);
		}

		// 정점 (v0, v1)이 선택 영역에 포함된 경우
		if (w0 > Thres && w1 > Thres && w2 < Thres)
		{
			double s = (w1 - Thres) / (w1 - w2);
			double t = (w0 - Thres) / (w0 - w2);
			p = v1->m_Pos + s * (v2->m_Pos - v1->m_Pos);
			q = v0->m_Pos + t * (v2->m_Pos - v0->m_Pos);
		}

		// 선택 영역의 경계 정점을 추가한다.
		SltBndryPts.push_back(p);
		SltBndryPts.push_back(q);
	}
}

/*!
*	\brief 메쉬의 특징 영역을 선택한다.
*
*	\param pMesh[in] 대상 메쉬
*	\param pVert[in] 선택을 위한 시작 정점에 대한 포인터
*	\param NumRing[in] 후보 영역을 정의하는 주변 정점의 링(ring) 수
*	\param Thres[in] 선택 영역을 결정하기 위한 하모닉 필드의 임계값(Thres 이상인 정점만 선택됨)
*	\param Option[in] 선택 옵션을 나타낸다(0: 새로운 선택, 1: 추가 선택, 2: 제거 선택)
*	\param SltFaces[out] 선택된 삼각형이 저장된다.
*	\param SltBndryPts[out] 선택 경계 라인이 저장된다.
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
	// 미지원 옵션이면 리턴한다.
	if (Option < 0 || Option > 2 || NumRing < 1)
	{
		printf("Not supported option...\n");
		return;
	}

	// 결과를 초기화 한다.
	SltFaces.swap(std::vector<EgFace *>());
	SltBndryPts.swap(std::vector<EgPos>());

	// 새로운 선택이라면 정점에 저장된 기존의 하모닉 필드 값을 초기화 한다.
	if (Option == 0)
		for (EgVertex *v : pMesh->m_pVerts)
			v->m_Color[0] = 0.0;
	
	// 정점 주변의 후보 선택 영역을 구한다.
	std::vector<std::vector<EgVertex *>> RingVerts;
	int N = ::get_neighbor_verts(pMesh, pVert, NumRing, RingVerts);

	// 후보 선택 영역의 정점을 (비제약 정점, 제약 정점(1, 0)) 순으로 재배열 한다.
	std::vector<EgVertex *> SltVerts;

	// 제약 조건이 없는 정점을 추가하고, 그 수(m)를 구한다.
	int k = (int)(N * 0.1);
	k = MAX(k, 1);
	for (int i = k; i < N; ++i)
		for (EgVertex *v : RingVerts[i])
			SltVerts.push_back(v);
	int m = (int)SltVerts.size();

	// 제약 조건이 1인 정점을 추가하고, 그 수(l)를 구한다.
	for (int i = 0; i < k; ++i)
		for (EgVertex *v : RingVerts[i])
			SltVerts.push_back(v);
	int l = (int)SltVerts.size() - m;

	// 제약 조건이 0인 정점을 추가하고, 그 수를(n) 구한다.
	for (EgVertex *v : RingVerts[N])
		SltVerts.push_back(v);
	int n = (int)SltVerts.size() - m - l;

	// 제약 정점의 값(1, 0)으로 벡터를 생성한다.
	Eigen::VectorXf y(l + n);
	for (int i = 0; i < l + n; ++i)
		y.coeffRef(i) = (i < l) ? 1.0f : 0.0f;

	// Laplace 행렬을 구한다.
	Eigen::SparseMatrix<float> M;	// M = (m + l + n) by (m + l + n)
	::get_laplacian_matrix_feature(pMesh, SltVerts, M);

	// 선형 시스템 Ax = b를 구성하기 위한 계수 행렬 A를 계산한다.
	Eigen::SparseMatrix<float> A, B;
	A = M.topLeftCorner(m, m);
	B = M.topRightCorner(m, l + n);
	A.makeCompressed();
	B.makeCompressed();

	// 선형 시스템 Ax = b의 상수 벡터 b를 구한다.
	Eigen::VectorXf b(m);
	b = -B * y;

	// 선형시스템 Ax = b의 해를 구한다.
	clock_t st = clock();
	Eigen::VectorXf x(m);
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>> solver;
	solver.compute(A);
	x = solver.solve(b);
	printf("\tElapsed time for Ax = b is %u ms\n", clock() - st);

	// 제거 선택 옵션이라면
	if (Option == 2)
	{
		for (int i = 0; i < m + l; ++i)
			SltVerts[i]->m_Color[0] = 0.0;
	}
	else
	{
		// 새로운 값과 기존값 중 큰 값을 선택한다.
		for (int i = 0; i < m + l; ++i) 
			SltVerts[i]->m_Color[0] = (i < m) ? MAX(SltVerts[i]->m_Color[0], (double)x(i)) : 1.0;
	}
	
	// 각각의 삼각형에 대하여
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 세 정점의 하모닉 필드값이 임계값 이상이면 선택 삼각형에 추가한다.
		EgVertex *v0 = f->GetVertex(0);
		EgVertex *v1 = f->GetVertex(1);
		EgVertex *v2 = f->GetVertex(2);
		double w0 = v0->m_Color[0];
		double w1 = v1->m_Color[0];
		double w2 = v2->m_Color[0];
		if (w0 > Thres && w1 > Thres && w2 > Thres)
			SltFaces.push_back(f);		

		EgPos p, q;
		// 정점 v0가 선택 영역에 포함된 경우
		if (w0 > Thres && w1 < Thres && w2 < Thres)
		{
			double s = (w0 - Thres) / (w0 - w1);
			double t = (w0 - Thres) / (w0 - w2);
			p = v0->m_Pos + s * (v1->m_Pos - v0->m_Pos);
			q = v0->m_Pos + t * (v2->m_Pos - v0->m_Pos);
		}

		// 정점 v1이 선택 영역에 포함된 경우
		if (w1 > Thres && w2 < Thres && w0 < Thres)
		{
			double s = (w1 - Thres) / (w1 - w2);
			double t = (w1 - Thres) / (w1 - w0);
			p = v1->m_Pos + s * (v2->m_Pos - v1->m_Pos);
			q = v1->m_Pos + t * (v0->m_Pos - v1->m_Pos);
		}

		// 정점 v2가 선택 영역에 포함된 경우
		if (w2 > Thres && w0 < Thres && w1 < Thres)
		{
			double s = (w2 - Thres) / (w2 - w0);
			double t = (w2 - Thres) / (w2 - w1);
			p = v2->m_Pos + s * (v0->m_Pos - v2->m_Pos);
			q = v2->m_Pos + t * (v1->m_Pos - v2->m_Pos);
		}

		// 정점 (v1, v2)가 선택 영역에 포함된 경우
		if (w1 > Thres && w2 > Thres && w0 < Thres)
		{
			double s = (w2 - Thres) / (w2 - w0);
			double t = (w1 - Thres) / (w1 - w0);
			p = v2->m_Pos + s * (v0->m_Pos - v2->m_Pos);
			q = v1->m_Pos + t * (v0->m_Pos - v1->m_Pos);
		}

		// 정점 (v0, v2)가 선택 영역에 포함된 경우
		if (w0 > Thres && w2 > Thres && w1 < Thres)
		{
			double s = (w0 - Thres) / (w0 - w1);
			double t = (w2 - Thres) / (w2 - w1);
			p = v0->m_Pos + s * (v1->m_Pos - v0->m_Pos);
			q = v2->m_Pos + t * (v1->m_Pos - v2->m_Pos);
		}

		// 정점 (v0, v1)이 선택 영역에 포함된 경우
		if (w0 > Thres && w1 > Thres && w2 < Thres)
		{
			double s = (w1 - Thres) / (w1 - w2);
			double t = (w0 - Thres) / (w0 - w2);
			p = v1->m_Pos + s * (v2->m_Pos - v1->m_Pos);
			q = v0->m_Pos + t * (v2->m_Pos - v0->m_Pos);
		}

		// 선택 영역의 경계 정점을 추가한다.
		SltBndryPts.push_back(p);
		SltBndryPts.push_back(q);
	}

	// 하모닉 필드 가시화를 위한 코드
	//pMesh->m_ShadeType = SHADE_BY_VERT_COLOR;
	//for (EgVertex *v : pMesh->m_pVerts)
	//	v->m_Color.Set(0.5, 0.5, 0.5);
	//for (int i = 0; i < m; ++i)
	//	SltVerts[i]->m_Color = ::get_color_map_from_rainbow(x[i]);
}
