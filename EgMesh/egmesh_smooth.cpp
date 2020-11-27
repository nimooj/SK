#include "..\usr\include\egmesh\egmesh.h"

/*! 
*	\brief 메쉬 전체에 스무딩을 수행한다. 
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*/
void get_smooth_mesh(EgMesh *pMesh)
{
	printf("Start mesh smoothing...\n");

	// 각각의 정점에 대하여 1링 이웃 정점의 무게 중심을 구한다.
	std::vector<EgPos> Cogs;
	for (EgVertex *v : pMesh->m_pVerts)
		Cogs.push_back(v->GetOneRingVertCenter());

	// 각각의 정점을 무게 중심으로 이동한다.
	int vidx = 0;
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_Pos = Cogs[vidx++];

	// 경계 상자와 렌더링 버퍼를 갱신한다.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal();
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();

	printf("Mesh smoothing is completed...\n\n");
}

/*!
*	\brief 메쉬 선택 영역에 대한 스무딩을 수행한다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*	\param Faces[in] 선택된 영역의 삼각형 리스트
*/
void get_smooth_mesh_region(EgMesh *pMesh, std::vector<EgFace *> &Faces)
{
	printf("Start mesh smoothing for the selected region...\n");

	// 선택 삼각형의 정점 집합을 구한다.
	std::vector<EgVertex *> Verts;
	::get_verts(Faces, Verts);

	// 선택된 정점의 무게 중심을 구한다.
	std::vector<EgPos> Cogs;
	for (EgVertex *v : Verts)
		Cogs.push_back(v->GetOneRingVertCenter());

	// 선택된 정점을 무게 중심으로 이동한다.
	int vidx = 0;
	for (EgVertex *v : Verts)
		v->m_Pos = Cogs[vidx++];

	// 삼각형 법선을 사용한다면
	if (pMesh->IsFaceNormal())
	{
		// 선택 영역의 각 삼각형에 대하여 삼각형 법선을 설정한다.
		for (EgFace *f : Faces)
			f->m_pEdge->m_pNormal->m_Dir = f->GetFaceNormal(true);
	}
	else // 정점 법선을 사용한다면
	{
		std::map<EgFace *, EgVec3> FaceNormalMap;
		for (EgFace *f : Faces)
			FaceNormalMap[f] = f->GetFaceNormal(true);

		for (EgVertex *v : Verts)
		{
			v->m_pEdges[0]->m_pNormal->m_Dir.Set(0.0, 0.0, 0.0);
			for (EgEdge *e : v->m_pEdges)
				e->m_pNormal->m_Dir += FaceNormalMap[e->m_pFace];
		}

		for (EgVertex *v : Verts)
			v->m_pEdges[0]->m_pNormal->m_Dir.Normalize();
	}

	// 경계 상자와 렌더링 버퍼를 갱신한다.
	pMesh->UpdateBndBox();
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();

	printf("Mesh smoothing is complete...\n\n");
}

/*!
*	\brief 메쉬 경계를 고정한 채 스무딩을 수행한다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*/
void get_smooth_mesh_interior(EgMesh *pMesh)
{
	printf("Start mesh smoothing...\n");

	// 각각의 정점에 대하여 1링 이웃 정점의 무게 중심을 구한다.
	std::vector<EgPos> Cogs;
	for (EgVertex *v : pMesh->m_pVerts)
		Cogs.push_back(v->GetOneRingVertCenter());

	// 각각의 정점을 무게 중심으로 이동한다.
	int vidx = 0;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (!v->IsBndry())
			v->m_Pos = Cogs[vidx];
		vidx++;
	}

	// 경계 상자와 렌더링 버퍼를 갱신한다.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal();
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();

	printf("Mesh smoothing complete...\n\n");
}

/*!
*	\brief 메쉬 경계에 스무딩을 수행한다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*/
void get_smooth_mesh_boundary(EgMesh *pMesh)
{
	printf("Start mesh boundary smoothing...\n");

	// 코너 삼각형을 제거한다 (정점의 인덱스 갱신됨)
	::delete_corner_faces(pMesh);

	// 메쉬의 경계 정점의 리스트를 구한다.
	std::vector<std::vector<EgVertex *>> HoleList;
	int NumHoles = ::get_bndry_verts(pMesh, HoleList, false, false);
	if (NumHoles == -1)
	{
		printf("\tNon-manifold vertices exist...\n");
		printf("Mesh boundary smoothing incomplete...\n\n");
		return;
	}

	// 각각의 경계에 대하여
	for (std::vector<EgVertex *> &BndryVerts : HoleList)
	{
		std::vector<EgPos> NewPos;
		int NumVerts = (int)BndryVerts.size();
		for (int j = 0; j < NumVerts; ++j)
		{
			EgPos pi = (j == 0) ? BndryVerts[NumVerts - 1]->m_Pos : BndryVerts[j - 1]->m_Pos;
			EgPos pk = (j == NumVerts - 1) ? BndryVerts[0]->m_Pos : BndryVerts[j + 1]->m_Pos;
			EgPos p = lerp(pi, pk, 0.5);
			NewPos.push_back(p);
			BndryVerts[j]->m_Pos = p;
		}
	}

	// 경계 상자와 렌더링 버퍼를 갱신한다.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal();
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();

	printf("Mesh boundary smoothing complete...\n\n");
}

/*!
*	\brief 메쉬 법선에 스무딩을 수행한다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*/
void get_smooth_mesh_normal(EgMesh *pMesh)
{
	printf("Start mesh normal smoothing...\n");

	// 정점 법선을 계산한다.
	if (pMesh->IsFaceNormal())
		pMesh->UpdateNormal(SMOOTH);
	pMesh->UpdateVertIdx();

	// 각각의 정점에 대하여 1링 정점의 평균 법선을 구한다.
	std::vector<EgVec3> AvgNormals;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		EgVec3 N;
		for (EgEdge *e : v->m_pEdges)
		{
			int nidx = e->m_pNext->m_pVert->m_Idx;
			N += pMesh->m_pNormals[nidx]->m_Dir;
		}
		AvgNormals.push_back(N.Normalize());
	}

	// 메쉬의 법선을 재설정 한다.
	int nidx = 0;
	for (EgNormal *n : pMesh->m_pNormals)
		n->m_Dir = AvgNormals[nidx++];

	// 렌더링 버퍼를 갱신한다.
	pMesh->UpdateNormalBuf();

	printf("Mesh normal smoothing complete...\n\n");
}

/*!
*	\brief	주어진 경계 조건을 만족하면서 메쉬의 면적을 최소화하는 fairing을 수행한다.
*
*	\param	pMesh[in] 대상 메쉬
*/
void get_fair_mesh_membrane(EgMesh *pMesh)
{
	printf("Start mesh fairing (membrane)...\n");

	// 반시계 방향으로 정렬된 경계 정점을 구한다.
	std::vector<std::vector<EgVertex *>> BndryList;
	int NumBndry = ::get_bndry_verts(pMesh, BndryList, false, false);

	// Non-manifold 메쉬거나, 경계가 없는 메쉬라면 리턴한다.
	if (NumBndry == -1 || NumBndry == 0)
	{
		printf("\tMesh fairing error...\n");
		printf("Mesh fairing incomplete...\n");
		return;
	}

	// 모든 경계 정점의 리스트를 구한다.
	std::vector<EgVertex *> BndryVerts;
	for (int i = 0; i < NumBndry; ++i)
		for (EgVertex *v : BndryList[i])
			BndryVerts.push_back(v);

	// 경계 정점의 개수를 구한다.
	int n = (int)BndryVerts.size();

	// 메쉬 정점의 순서를 내부정점-경계정점으로 구분한다.
	std::vector<EgVertex *> IntrVerts;
	int m = ::get_intr_verts(pMesh, IntrVerts);
	for (EgVertex *v : BndryVerts)
		IntrVerts.push_back(v);
	pMesh->m_pVerts.clear();
	pMesh->m_pVerts = IntrVerts;

	// Laplace 행렬을 구한다.
	Eigen::SparseMatrix<float> D;	// D = (m + n) by (m + n)
	Eigen::SparseMatrix<float> M;	// M = (m + n) by (m + n)
	::get_laplacian_matrix(pMesh, D, M);

	// 선형 시스템 Ax = b를 구성하기 위한 계수 행렬 A를 계산한다.
	Eigen::SparseMatrix<float> A, B;
	A = M.topLeftCorner(m, m);
	B = M.topRightCorner(m, n);
	A.makeCompressed();
	printf("\tMatrix A is constructed...\n");

	// 경계 조건(결정된 미지수 값)을 설정한다.
	Eigen::VectorXf y1(n), y2(n), y3(n);
	for (int i = 0; i < n; ++i)
	{
		y1(i) = (float)BndryVerts[i]->m_Pos[0];
		y2(i) = (float)BndryVerts[i]->m_Pos[1];
		y3(i) = (float)BndryVerts[i]->m_Pos[2];
	}

	// 선형 시스템 Ax = b의 상수 벡터(b1, b2, b3)를 구한다.
	Eigen::VectorXf b1(m), b2(m), b3(m);
	b1 = -B * y1;
	b2 = -B * y2;
	b3 = -B * y3;
	printf("\tConstant vector b is constructed...\n");

	// 선형시스템 Ax = b의 해(x1, x2, x3)를 구한다.
	Eigen::VectorXf x1(m), x2(m), x3(m);
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>> solver;
	solver.compute(A);
	x1 = solver.solve(b1);
	x2 = solver.solve(b2);
	x3 = solver.solve(b3);

	// 내부 정점의 파라미터 값을 설정한다.
	for (int i = 0; i < m; ++i)
		IntrVerts[i]->m_Pos.Set(x1[i], x2[i], x3[i]);

	// 렌더링 버퍼를 초기화 한다.
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();
	pMesh->m_TexelBuf.clear();
	printf("Mesh fairing complete...\n");	
}

/*!
*	\brief	선택 영역의 면적을 최소화하는 fairing을 수행한다.
*
*	\param	pMesh[in] 대상 메쉬
*	\param	Faces[in] 선택 영역의 삼각형 리스트
*/
void get_fair_mesh_membrane(EgMesh *pMesh, std::vector<EgFace *> &Faces)
{
	// 선택된 삼각형으로 메쉬를 생성하여 fairing을 수행하고
	EgMesh *pSubMesh = ::create_mesh_from_slt_faces(pMesh, Faces, false);
	::get_fair_mesh_membrane(pSubMesh);

	// 정점의 좌표를 갱신한다.
	int NumFace = pSubMesh->GetNumFace();
	for (int i = 0; i < NumFace; ++i)
	{
		EgFace *f = pSubMesh->m_pFaces[i];
		Faces[i]->GetVertex(0)->m_Pos = f->GetVertex(0)->m_Pos;
		Faces[i]->GetVertex(1)->m_Pos = f->GetVertex(1)->m_Pos;
		Faces[i]->GetVertex(2)->m_Pos = f->GetVertex(2)->m_Pos;
	}
	
	// 렌더링 버퍼를 초기화 한다.
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();
	pMesh->m_TexelBuf.clear();
}

/*!
*	\brief	주어진 경계 조건을 만족하면서 메쉬의 곡률을 최소화하는 fairing을 수행한다.
*
*	\param	pMesh[in] 대상 메쉬
*/
void get_fair_mesh_thin_plate(EgMesh *pMesh)
{
	printf("Start mesh fairing (thin plate)...\n");

	// 반시계 방향으로 정렬된 경계 정점을 구한다.
	std::vector<std::vector<EgVertex *>> BndryList;
	int NumBndry = ::get_bndry_verts(pMesh, BndryList, false, false);

	// Non-manifold 메쉬거나, 경계가 없는 메쉬라면 리턴한다.
	if (NumBndry == -1 || NumBndry == 0)
	{
		printf("\tMesh fairing error...\n");
		printf("Mesh fairing incomplete...\n");
		return;
	}

	// 모든 경계 정점의 리스트를 구한다.
	std::vector<EgVertex *> BndryVerts;
	for (int i = 0; i < NumBndry; ++i)
		for (EgVertex *v : BndryList[i])
			BndryVerts.push_back(v);

	// 경계 정점의 영역을 메쉬 안쪽으로 1링 확장한다.
	std::vector<EgVertex *> Tmp;
	for (EgVertex *v : BndryVerts)
	{
		for (EgEdge *e : v->m_pEdges)
		{
			if (e->m_pNext->m_pVert->IsBndry() == false)
				Tmp.push_back(e->m_pNext->m_pVert);
		}
	}
	// 중복된 정점을 제거한다.
	sort(Tmp.begin(), Tmp.end());
	Tmp.erase(unique(Tmp.begin(), Tmp.end()), Tmp.end());
	for (EgVertex *v : Tmp)
		BndryVerts.push_back(v);

	// 확장된 경계 정점(제약 조건을 갖는 정점)의 수를 구한다.
	int n = (int)BndryVerts.size();

	// 내부 정점을 구한다.
	std::vector<EgVertex *> IntrVerts;
	for (EgVertex *v : BndryVerts)
		v->m_Idx = -1;
	for (EgVertex *v : pMesh->m_pVerts)
		if (v->m_Idx != -1)
			IntrVerts.push_back(v);

	// 내부 정점(자유 정점)의 수를 구한다.
	int m = (int)IntrVerts.size();

	// 메쉬 정점의 순서를 내부정점-경계정점으로 구분한다.
	for (EgVertex *v : BndryVerts)
		IntrVerts.push_back(v);
	pMesh->m_pVerts.clear();
	pMesh->m_pVerts = IntrVerts;

	// Laplace 행렬을 구한다.
	Eigen::SparseMatrix<float> D;	// D = (m + n) by (m + n)
	Eigen::SparseMatrix<float> M;	// M = (m + n) by (m + n)
	::get_laplacian_matrix(pMesh, D, M);

	Eigen::SparseMatrix<float> L;	// L = (m + n) by (m + n)
	L = M * D * M;

	// 선형 시스템 Ax = b를 구성하기 위한 계수 행렬 A를 계산한다.
	Eigen::SparseMatrix<float> A, B;
	A = L.topLeftCorner(m, m);
	B = L.topRightCorner(m, n);
	A.makeCompressed();
	printf("\tMatrix A is constructed...\n");

	// 경계 조건(결정된 미지수 값)을 설정한다.
	Eigen::VectorXf y1(n), y2(n), y3(n);
	for (int i = 0; i < n; ++i)
	{
		y1(i) = (float)BndryVerts[i]->m_Pos[0];
		y2(i) = (float)BndryVerts[i]->m_Pos[1];
		y3(i) = (float)BndryVerts[i]->m_Pos[2];
	}

	// 선형 시스템 Ax = b의 상수 벡터(b1, b2, b3)를 구한다.
	Eigen::VectorXf b1(m), b2(m), b3(m);
	b1 = -B * y1;
	b2 = -B * y2;
	b3 = -B * y3;
	printf("\tConstant vector b is constructed...\n");

	// 선형시스템 Ax = b의 해(x1, x2, x3)를 구한다.
	Eigen::VectorXf x1(m), x2(m), x3(m);
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>> solver;
	solver.compute(A);
	x1 = solver.solve(b1);
	x2 = solver.solve(b2);
	x3 = solver.solve(b3);

	// 내부 정점의 파라미터 값을 설정한다.
	for (int i = 0; i < m; ++i)
		IntrVerts[i]->m_Pos.Set(x1[i], x2[i], x3[i]);

	// 렌더링 버퍼를 초기화 한다.
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();
	pMesh->m_TexelBuf.clear();
	printf("Mesh fairing complete...\n");
}

/*!
*	\brief	선택 영역의 곡률합을 최소화하는 fairing을 수행한다.
*
*	\param	pMesh[in] 대상 메쉬
*	\param	Faces[in] 선택 영역의 삼각형 리스트
*/
void get_fair_mesh_thin_plate(EgMesh *pMesh, std::vector<EgFace *> &Faces)
{
	// 선택된 삼각형으로 메쉬를 생성하여 fairing을 수행하고
	EgMesh *pSubMesh = ::create_mesh_from_slt_faces(pMesh, Faces, false);
	::get_fair_mesh_thin_plate(pSubMesh);

	// 정점의 좌표를 갱신한다.
	int NumFace = pSubMesh->GetNumFace();
	for (int i = 0; i < NumFace; ++i)
	{
		EgFace *f = pSubMesh->m_pFaces[i];
		Faces[i]->GetVertex(0)->m_Pos = f->GetVertex(0)->m_Pos;
		Faces[i]->GetVertex(1)->m_Pos = f->GetVertex(1)->m_Pos;
		Faces[i]->GetVertex(2)->m_Pos = f->GetVertex(2)->m_Pos;
	}

	// 렌더링 버퍼를 초기화 한다.
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();
	pMesh->m_TexelBuf.clear();
}

/*! 
*	\brief 삼각형을 분할하여 세분화된 메쉬를 만든다.
*	\note 원하는 만큼 분할 후에 InitEdgeMate() 함수를 호출해 주어야 한다.
*	
*	\param pMesh[in] 분할할 삼각 메쉬
*/
void get_refined_mesh(EgMesh *pMesh)
{
	// 각각의 삼각형을 4개로 분할하여 새로운 삼각형 리스트를 생성한다.
	std::vector<EgFace *> NewFaces;
	std::map<std::string, EgVertex *> VertMap;
	for (EgFace *f : pMesh->m_pFaces)
	{
		EgVertex *v0 = f->GetVertex(0);
		EgVertex *v1 = f->GetVertex(1);
		EgVertex *v2 = f->GetVertex(2);
		v0->DeleteEdge(f->GetEdge(0));
		v1->DeleteEdge(f->GetEdge(1));
		v2->DeleteEdge(f->GetEdge(2));
		std::string edge01 = cast_str(v0->m_Idx) + std::string("-") + cast_str(v1->m_Idx);
		std::string edge10 = cast_str(v1->m_Idx) + std::string("-") + cast_str(v0->m_Idx);
		std::string edge12 = cast_str(v1->m_Idx) + std::string("-") + cast_str(v2->m_Idx);
		std::string edge21 = cast_str(v2->m_Idx) + std::string("-") + cast_str(v1->m_Idx);
		std::string edge20 = cast_str(v2->m_Idx) + std::string("-") + cast_str(v0->m_Idx);
		std::string edge02 = cast_str(v0->m_Idx) + std::string("-") + cast_str(v2->m_Idx);

		// 에지 (vidx0, vidx1) 또는 (vidx1, vidx0)의 중점이 추가되지 않았다면,
		if (VertMap[edge01] == NULL && VertMap[edge10] == NULL)
		{
			// 에지의 중점을 구하여 정점 리스트에 추가하고,
			EgPos q = lerp(v0->m_Pos, v1->m_Pos, 0.5);
			EgVertex *v = new EgVertex(q);
			pMesh->AddVertex(v);

			// 에지의 중점의 포인터를 기록한다.
			VertMap[edge01] = VertMap[edge10] = v;
		}

		// 에지 (vidx1, vidx2) 또는 (vidx2, vidx1)의 중점이 추가되지 않았다면,
		if (VertMap[edge12] == NULL && VertMap[edge21] == NULL)
		{
			// 에지의 중점을 구하여 정점 리스트에 추가하고,
			EgPos q = lerp(v1->m_Pos, v2->m_Pos, 0.5);
			EgVertex *v = new EgVertex(q);
			pMesh->AddVertex(v);

			// 에지의 중점의 포인터를 기록한다.
			VertMap[edge12] = VertMap[edge21] = v;
		}

		// 에지 (vidx2, vidx0) 또는 (vidx0, vidx2)의 중점이 추가되지 않았다면,
		if (VertMap[edge20] == NULL && VertMap[edge02] == NULL)
		{
			// 에지의 중점을 구하여 정점 리스트에 추가하고,
			EgPos q = lerp(v2->m_Pos, v0->m_Pos, 0.5);
			EgVertex *v = new EgVertex(q);
			pMesh->AddVertex(v);

			// 에지의 중점의 포인터를 기록한다.
			VertMap[edge20] = VertMap[edge02] = v;
		}

		// 에지 중점의 포인터를 구하여
		EgVertex *v3 = VertMap[edge01];
		EgVertex *v4 = VertMap[edge12];
		EgVertex *v5 = VertMap[edge20];

		// 삼각형을 4개의 부분삼각형으로 분할하여 메쉬에 추가한다.
		std::string GrpName = f->m_GroupName;
		EgMaterial *pMtl = f->m_pMaterial;
		EgFace *f0 = new EgFace(v0, v3, v5, NULL, NULL, NULL, NULL, NULL, NULL, pMtl, GrpName);
		EgFace *f1 = new EgFace(v3, v1, v4, NULL, NULL, NULL, NULL, NULL, NULL, pMtl, GrpName);
		EgFace *f2 = new EgFace(v3, v4, v5, NULL, NULL, NULL, NULL, NULL, NULL, pMtl, GrpName);
		EgFace *f3 = new EgFace(v5, v4, v2, NULL, NULL, NULL, NULL, NULL, NULL, pMtl, GrpName);
		NewFaces.push_back(f0);
		NewFaces.push_back(f1);
		NewFaces.push_back(f2);
		NewFaces.push_back(f3);
	}
	pMesh->m_pFaces = NewFaces;
	pMesh->UpdateFaceIdx();
	pMesh->m_VertexBuf.clear();
}

