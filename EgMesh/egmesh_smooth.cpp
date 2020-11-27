#include "..\usr\include\egmesh\egmesh.h"

/*! 
*	\brief �޽� ��ü�� �������� �����Ѵ�. 
*
*	\param pMesh[in] ��� �޽��� ���� ������
*/
void get_smooth_mesh(EgMesh *pMesh)
{
	printf("Start mesh smoothing...\n");

	// ������ ������ ���Ͽ� 1�� �̿� ������ ���� �߽��� ���Ѵ�.
	std::vector<EgPos> Cogs;
	for (EgVertex *v : pMesh->m_pVerts)
		Cogs.push_back(v->GetOneRingVertCenter());

	// ������ ������ ���� �߽����� �̵��Ѵ�.
	int vidx = 0;
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_Pos = Cogs[vidx++];

	// ��� ���ڿ� ������ ���۸� �����Ѵ�.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal();
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();

	printf("Mesh smoothing is completed...\n\n");
}

/*!
*	\brief �޽� ���� ������ ���� �������� �����Ѵ�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*	\param Faces[in] ���õ� ������ �ﰢ�� ����Ʈ
*/
void get_smooth_mesh_region(EgMesh *pMesh, std::vector<EgFace *> &Faces)
{
	printf("Start mesh smoothing for the selected region...\n");

	// ���� �ﰢ���� ���� ������ ���Ѵ�.
	std::vector<EgVertex *> Verts;
	::get_verts(Faces, Verts);

	// ���õ� ������ ���� �߽��� ���Ѵ�.
	std::vector<EgPos> Cogs;
	for (EgVertex *v : Verts)
		Cogs.push_back(v->GetOneRingVertCenter());

	// ���õ� ������ ���� �߽����� �̵��Ѵ�.
	int vidx = 0;
	for (EgVertex *v : Verts)
		v->m_Pos = Cogs[vidx++];

	// �ﰢ�� ������ ����Ѵٸ�
	if (pMesh->IsFaceNormal())
	{
		// ���� ������ �� �ﰢ���� ���Ͽ� �ﰢ�� ������ �����Ѵ�.
		for (EgFace *f : Faces)
			f->m_pEdge->m_pNormal->m_Dir = f->GetFaceNormal(true);
	}
	else // ���� ������ ����Ѵٸ�
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

	// ��� ���ڿ� ������ ���۸� �����Ѵ�.
	pMesh->UpdateBndBox();
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();

	printf("Mesh smoothing is complete...\n\n");
}

/*!
*	\brief �޽� ��踦 ������ ä �������� �����Ѵ�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*/
void get_smooth_mesh_interior(EgMesh *pMesh)
{
	printf("Start mesh smoothing...\n");

	// ������ ������ ���Ͽ� 1�� �̿� ������ ���� �߽��� ���Ѵ�.
	std::vector<EgPos> Cogs;
	for (EgVertex *v : pMesh->m_pVerts)
		Cogs.push_back(v->GetOneRingVertCenter());

	// ������ ������ ���� �߽����� �̵��Ѵ�.
	int vidx = 0;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (!v->IsBndry())
			v->m_Pos = Cogs[vidx];
		vidx++;
	}

	// ��� ���ڿ� ������ ���۸� �����Ѵ�.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal();
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();

	printf("Mesh smoothing complete...\n\n");
}

/*!
*	\brief �޽� ��迡 �������� �����Ѵ�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*/
void get_smooth_mesh_boundary(EgMesh *pMesh)
{
	printf("Start mesh boundary smoothing...\n");

	// �ڳ� �ﰢ���� �����Ѵ� (������ �ε��� ���ŵ�)
	::delete_corner_faces(pMesh);

	// �޽��� ��� ������ ����Ʈ�� ���Ѵ�.
	std::vector<std::vector<EgVertex *>> HoleList;
	int NumHoles = ::get_bndry_verts(pMesh, HoleList, false, false);
	if (NumHoles == -1)
	{
		printf("\tNon-manifold vertices exist...\n");
		printf("Mesh boundary smoothing incomplete...\n\n");
		return;
	}

	// ������ ��迡 ���Ͽ�
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

	// ��� ���ڿ� ������ ���۸� �����Ѵ�.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal();
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();

	printf("Mesh boundary smoothing complete...\n\n");
}

/*!
*	\brief �޽� ������ �������� �����Ѵ�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*/
void get_smooth_mesh_normal(EgMesh *pMesh)
{
	printf("Start mesh normal smoothing...\n");

	// ���� ������ ����Ѵ�.
	if (pMesh->IsFaceNormal())
		pMesh->UpdateNormal(SMOOTH);
	pMesh->UpdateVertIdx();

	// ������ ������ ���Ͽ� 1�� ������ ��� ������ ���Ѵ�.
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

	// �޽��� ������ �缳�� �Ѵ�.
	int nidx = 0;
	for (EgNormal *n : pMesh->m_pNormals)
		n->m_Dir = AvgNormals[nidx++];

	// ������ ���۸� �����Ѵ�.
	pMesh->UpdateNormalBuf();

	printf("Mesh normal smoothing complete...\n\n");
}

/*!
*	\brief	�־��� ��� ������ �����ϸ鼭 �޽��� ������ �ּ�ȭ�ϴ� fairing�� �����Ѵ�.
*
*	\param	pMesh[in] ��� �޽�
*/
void get_fair_mesh_membrane(EgMesh *pMesh)
{
	printf("Start mesh fairing (membrane)...\n");

	// �ݽð� �������� ���ĵ� ��� ������ ���Ѵ�.
	std::vector<std::vector<EgVertex *>> BndryList;
	int NumBndry = ::get_bndry_verts(pMesh, BndryList, false, false);

	// Non-manifold �޽��ų�, ��谡 ���� �޽���� �����Ѵ�.
	if (NumBndry == -1 || NumBndry == 0)
	{
		printf("\tMesh fairing error...\n");
		printf("Mesh fairing incomplete...\n");
		return;
	}

	// ��� ��� ������ ����Ʈ�� ���Ѵ�.
	std::vector<EgVertex *> BndryVerts;
	for (int i = 0; i < NumBndry; ++i)
		for (EgVertex *v : BndryList[i])
			BndryVerts.push_back(v);

	// ��� ������ ������ ���Ѵ�.
	int n = (int)BndryVerts.size();

	// �޽� ������ ������ ��������-����������� �����Ѵ�.
	std::vector<EgVertex *> IntrVerts;
	int m = ::get_intr_verts(pMesh, IntrVerts);
	for (EgVertex *v : BndryVerts)
		IntrVerts.push_back(v);
	pMesh->m_pVerts.clear();
	pMesh->m_pVerts = IntrVerts;

	// Laplace ����� ���Ѵ�.
	Eigen::SparseMatrix<float> D;	// D = (m + n) by (m + n)
	Eigen::SparseMatrix<float> M;	// M = (m + n) by (m + n)
	::get_laplacian_matrix(pMesh, D, M);

	// ���� �ý��� Ax = b�� �����ϱ� ���� ��� ��� A�� ����Ѵ�.
	Eigen::SparseMatrix<float> A, B;
	A = M.topLeftCorner(m, m);
	B = M.topRightCorner(m, n);
	A.makeCompressed();
	printf("\tMatrix A is constructed...\n");

	// ��� ����(������ ������ ��)�� �����Ѵ�.
	Eigen::VectorXf y1(n), y2(n), y3(n);
	for (int i = 0; i < n; ++i)
	{
		y1(i) = (float)BndryVerts[i]->m_Pos[0];
		y2(i) = (float)BndryVerts[i]->m_Pos[1];
		y3(i) = (float)BndryVerts[i]->m_Pos[2];
	}

	// ���� �ý��� Ax = b�� ��� ����(b1, b2, b3)�� ���Ѵ�.
	Eigen::VectorXf b1(m), b2(m), b3(m);
	b1 = -B * y1;
	b2 = -B * y2;
	b3 = -B * y3;
	printf("\tConstant vector b is constructed...\n");

	// �����ý��� Ax = b�� ��(x1, x2, x3)�� ���Ѵ�.
	Eigen::VectorXf x1(m), x2(m), x3(m);
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>> solver;
	solver.compute(A);
	x1 = solver.solve(b1);
	x2 = solver.solve(b2);
	x3 = solver.solve(b3);

	// ���� ������ �Ķ���� ���� �����Ѵ�.
	for (int i = 0; i < m; ++i)
		IntrVerts[i]->m_Pos.Set(x1[i], x2[i], x3[i]);

	// ������ ���۸� �ʱ�ȭ �Ѵ�.
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();
	pMesh->m_TexelBuf.clear();
	printf("Mesh fairing complete...\n");	
}

/*!
*	\brief	���� ������ ������ �ּ�ȭ�ϴ� fairing�� �����Ѵ�.
*
*	\param	pMesh[in] ��� �޽�
*	\param	Faces[in] ���� ������ �ﰢ�� ����Ʈ
*/
void get_fair_mesh_membrane(EgMesh *pMesh, std::vector<EgFace *> &Faces)
{
	// ���õ� �ﰢ������ �޽��� �����Ͽ� fairing�� �����ϰ�
	EgMesh *pSubMesh = ::create_mesh_from_slt_faces(pMesh, Faces, false);
	::get_fair_mesh_membrane(pSubMesh);

	// ������ ��ǥ�� �����Ѵ�.
	int NumFace = pSubMesh->GetNumFace();
	for (int i = 0; i < NumFace; ++i)
	{
		EgFace *f = pSubMesh->m_pFaces[i];
		Faces[i]->GetVertex(0)->m_Pos = f->GetVertex(0)->m_Pos;
		Faces[i]->GetVertex(1)->m_Pos = f->GetVertex(1)->m_Pos;
		Faces[i]->GetVertex(2)->m_Pos = f->GetVertex(2)->m_Pos;
	}
	
	// ������ ���۸� �ʱ�ȭ �Ѵ�.
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();
	pMesh->m_TexelBuf.clear();
}

/*!
*	\brief	�־��� ��� ������ �����ϸ鼭 �޽��� ����� �ּ�ȭ�ϴ� fairing�� �����Ѵ�.
*
*	\param	pMesh[in] ��� �޽�
*/
void get_fair_mesh_thin_plate(EgMesh *pMesh)
{
	printf("Start mesh fairing (thin plate)...\n");

	// �ݽð� �������� ���ĵ� ��� ������ ���Ѵ�.
	std::vector<std::vector<EgVertex *>> BndryList;
	int NumBndry = ::get_bndry_verts(pMesh, BndryList, false, false);

	// Non-manifold �޽��ų�, ��谡 ���� �޽���� �����Ѵ�.
	if (NumBndry == -1 || NumBndry == 0)
	{
		printf("\tMesh fairing error...\n");
		printf("Mesh fairing incomplete...\n");
		return;
	}

	// ��� ��� ������ ����Ʈ�� ���Ѵ�.
	std::vector<EgVertex *> BndryVerts;
	for (int i = 0; i < NumBndry; ++i)
		for (EgVertex *v : BndryList[i])
			BndryVerts.push_back(v);

	// ��� ������ ������ �޽� �������� 1�� Ȯ���Ѵ�.
	std::vector<EgVertex *> Tmp;
	for (EgVertex *v : BndryVerts)
	{
		for (EgEdge *e : v->m_pEdges)
		{
			if (e->m_pNext->m_pVert->IsBndry() == false)
				Tmp.push_back(e->m_pNext->m_pVert);
		}
	}
	// �ߺ��� ������ �����Ѵ�.
	sort(Tmp.begin(), Tmp.end());
	Tmp.erase(unique(Tmp.begin(), Tmp.end()), Tmp.end());
	for (EgVertex *v : Tmp)
		BndryVerts.push_back(v);

	// Ȯ��� ��� ����(���� ������ ���� ����)�� ���� ���Ѵ�.
	int n = (int)BndryVerts.size();

	// ���� ������ ���Ѵ�.
	std::vector<EgVertex *> IntrVerts;
	for (EgVertex *v : BndryVerts)
		v->m_Idx = -1;
	for (EgVertex *v : pMesh->m_pVerts)
		if (v->m_Idx != -1)
			IntrVerts.push_back(v);

	// ���� ����(���� ����)�� ���� ���Ѵ�.
	int m = (int)IntrVerts.size();

	// �޽� ������ ������ ��������-����������� �����Ѵ�.
	for (EgVertex *v : BndryVerts)
		IntrVerts.push_back(v);
	pMesh->m_pVerts.clear();
	pMesh->m_pVerts = IntrVerts;

	// Laplace ����� ���Ѵ�.
	Eigen::SparseMatrix<float> D;	// D = (m + n) by (m + n)
	Eigen::SparseMatrix<float> M;	// M = (m + n) by (m + n)
	::get_laplacian_matrix(pMesh, D, M);

	Eigen::SparseMatrix<float> L;	// L = (m + n) by (m + n)
	L = M * D * M;

	// ���� �ý��� Ax = b�� �����ϱ� ���� ��� ��� A�� ����Ѵ�.
	Eigen::SparseMatrix<float> A, B;
	A = L.topLeftCorner(m, m);
	B = L.topRightCorner(m, n);
	A.makeCompressed();
	printf("\tMatrix A is constructed...\n");

	// ��� ����(������ ������ ��)�� �����Ѵ�.
	Eigen::VectorXf y1(n), y2(n), y3(n);
	for (int i = 0; i < n; ++i)
	{
		y1(i) = (float)BndryVerts[i]->m_Pos[0];
		y2(i) = (float)BndryVerts[i]->m_Pos[1];
		y3(i) = (float)BndryVerts[i]->m_Pos[2];
	}

	// ���� �ý��� Ax = b�� ��� ����(b1, b2, b3)�� ���Ѵ�.
	Eigen::VectorXf b1(m), b2(m), b3(m);
	b1 = -B * y1;
	b2 = -B * y2;
	b3 = -B * y3;
	printf("\tConstant vector b is constructed...\n");

	// �����ý��� Ax = b�� ��(x1, x2, x3)�� ���Ѵ�.
	Eigen::VectorXf x1(m), x2(m), x3(m);
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>> solver;
	solver.compute(A);
	x1 = solver.solve(b1);
	x2 = solver.solve(b2);
	x3 = solver.solve(b3);

	// ���� ������ �Ķ���� ���� �����Ѵ�.
	for (int i = 0; i < m; ++i)
		IntrVerts[i]->m_Pos.Set(x1[i], x2[i], x3[i]);

	// ������ ���۸� �ʱ�ȭ �Ѵ�.
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();
	pMesh->m_TexelBuf.clear();
	printf("Mesh fairing complete...\n");
}

/*!
*	\brief	���� ������ ������� �ּ�ȭ�ϴ� fairing�� �����Ѵ�.
*
*	\param	pMesh[in] ��� �޽�
*	\param	Faces[in] ���� ������ �ﰢ�� ����Ʈ
*/
void get_fair_mesh_thin_plate(EgMesh *pMesh, std::vector<EgFace *> &Faces)
{
	// ���õ� �ﰢ������ �޽��� �����Ͽ� fairing�� �����ϰ�
	EgMesh *pSubMesh = ::create_mesh_from_slt_faces(pMesh, Faces, false);
	::get_fair_mesh_thin_plate(pSubMesh);

	// ������ ��ǥ�� �����Ѵ�.
	int NumFace = pSubMesh->GetNumFace();
	for (int i = 0; i < NumFace; ++i)
	{
		EgFace *f = pSubMesh->m_pFaces[i];
		Faces[i]->GetVertex(0)->m_Pos = f->GetVertex(0)->m_Pos;
		Faces[i]->GetVertex(1)->m_Pos = f->GetVertex(1)->m_Pos;
		Faces[i]->GetVertex(2)->m_Pos = f->GetVertex(2)->m_Pos;
	}

	// ������ ���۸� �ʱ�ȭ �Ѵ�.
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();
	pMesh->m_TexelBuf.clear();
}

/*! 
*	\brief �ﰢ���� �����Ͽ� ����ȭ�� �޽��� �����.
*	\note ���ϴ� ��ŭ ���� �Ŀ� InitEdgeMate() �Լ��� ȣ���� �־�� �Ѵ�.
*	
*	\param pMesh[in] ������ �ﰢ �޽�
*/
void get_refined_mesh(EgMesh *pMesh)
{
	// ������ �ﰢ���� 4���� �����Ͽ� ���ο� �ﰢ�� ����Ʈ�� �����Ѵ�.
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

		// ���� (vidx0, vidx1) �Ǵ� (vidx1, vidx0)�� ������ �߰����� �ʾҴٸ�,
		if (VertMap[edge01] == NULL && VertMap[edge10] == NULL)
		{
			// ������ ������ ���Ͽ� ���� ����Ʈ�� �߰��ϰ�,
			EgPos q = lerp(v0->m_Pos, v1->m_Pos, 0.5);
			EgVertex *v = new EgVertex(q);
			pMesh->AddVertex(v);

			// ������ ������ �����͸� ����Ѵ�.
			VertMap[edge01] = VertMap[edge10] = v;
		}

		// ���� (vidx1, vidx2) �Ǵ� (vidx2, vidx1)�� ������ �߰����� �ʾҴٸ�,
		if (VertMap[edge12] == NULL && VertMap[edge21] == NULL)
		{
			// ������ ������ ���Ͽ� ���� ����Ʈ�� �߰��ϰ�,
			EgPos q = lerp(v1->m_Pos, v2->m_Pos, 0.5);
			EgVertex *v = new EgVertex(q);
			pMesh->AddVertex(v);

			// ������ ������ �����͸� ����Ѵ�.
			VertMap[edge12] = VertMap[edge21] = v;
		}

		// ���� (vidx2, vidx0) �Ǵ� (vidx0, vidx2)�� ������ �߰����� �ʾҴٸ�,
		if (VertMap[edge20] == NULL && VertMap[edge02] == NULL)
		{
			// ������ ������ ���Ͽ� ���� ����Ʈ�� �߰��ϰ�,
			EgPos q = lerp(v2->m_Pos, v0->m_Pos, 0.5);
			EgVertex *v = new EgVertex(q);
			pMesh->AddVertex(v);

			// ������ ������ �����͸� ����Ѵ�.
			VertMap[edge20] = VertMap[edge02] = v;
		}

		// ���� ������ �����͸� ���Ͽ�
		EgVertex *v3 = VertMap[edge01];
		EgVertex *v4 = VertMap[edge12];
		EgVertex *v5 = VertMap[edge20];

		// �ﰢ���� 4���� �κлﰢ������ �����Ͽ� �޽��� �߰��Ѵ�.
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

