#include "..\usr\include\egmesh\egmesh.h"

/*!
*	\biref	�޽� ��ü�� �����Ѵ�.
*
*	\param	MeshName[in] ������ �޽� ��ü�� �̸�
*
*	\return ������ �޽��� �����͸� ��ȯ�Ѵ�.
*/
EgMesh *create_mesh(std::string MeshName)
{
	return new EgMesh(MeshName);
}

/*!
*	\biref	�޽� ������ �����Ѵ�.
*
*	\param	x[in] ������ x ��ǥ
*	\param	y[in] ������ y ��ǥ
*	\param	z[in] ������ z ��ǥ
*
*	\return ������ ������ �����͸� ��ȯ�Ѵ�.
*/
EgVertex *create_vertex(double x, double y, double z)
{
	return new EgVertex(x, y, z);
}

/*!
*	\biref	�ؽ�ó ��ǥ�� �����Ѵ�.
*
*	\param	s[in] �ؽ�ó s ��ǥ
*	\param	t[in] �ؽ�ó t ��ǥ
*
*	\return ������ �ؽ�ó ��ǥ�� �����͸� ��ȯ�Ѵ�.
*/
EgTexel *create_texel(double s, double t)
{
	return new EgTexel(s, t);
}

/*!
*	\biref	�޽� ������ �����Ѵ�.
*
*	\param	nx[in] ������ x ��ǥ
*	\param	ny[in] ������ y ��ǥ
*	\param	nz[in] ������ z ��ǥ
*
*	\return ������ ������ �����͸� ��ȯ�Ѵ�.
*/
EgNormal *create_normal(double nx, double ny, double nz)
{
	return new EgNormal(nx, ny, nz);
}

/*!
*	\brief �޽� �ﰢ���� �����Ѵ�.
*
*	\param v0[in] �ﰢ���� ù ��° ����
*	\param v1[in] �ﰢ���� �� ��° ����
*	\param v2[in] �ﰢ���� �� ��° ����
*	\param pMtl[in] �ﰢ���� ����ϴ� ������ ���� ������
*	\param GroupName[in] �ﰢ���� ���� �׷��� �̸�
*/
EgFace *create_face(EgVertex *v0, EgVertex *v1, EgVertex *v2, EgMaterial *pMtl, std::string GroupName)
{
	return new EgFace(v0, v1, v2, NULL, NULL, NULL, NULL, NULL, NULL, pMtl, GroupName);
}

/*!
*	\brief �޽� �ﰢ���� �����Ѵ�.
*
*	\param v0[in] �ﰢ���� ù ��° ����
*	\param v1[in] �ﰢ���� �� ��° ����
*	\param v2[in] �ﰢ���� �� ��° ����
*	\param t0[in] �ﰢ���� ù ��° ������ �ؽ�ó ��ǥ
*	\param t1[in] �ﰢ���� �� ��° ������ �ؽ�ó ��ǥ
*	\param t2[in] �ﰢ���� �� ��° ������ �ؽ�ó ��ǥ
*	\param pMtl[in] �ﰢ���� ����ϴ� ������ ���� ������
*	\param GroupName[in] �ﰢ���� ���� �׷��� �̸�
*/
EgFace *create_face(EgVertex *v0, EgVertex *v1, EgVertex *v2, EgTexel *t0, EgTexel *t1, EgTexel *t2, EgMaterial *pMtl, std::string GroupName)
{
	return new EgFace(v0, v1, v2, t0, t1, t2, NULL, NULL, NULL, pMtl, GroupName);
}

/*!
*	\brief �޽� �ﰢ���� �����Ѵ�.
*
*	\param v0[in] �ﰢ���� ù ��° ����
*	\param v1[in] �ﰢ���� �� ��° ����
*	\param v2[in] �ﰢ���� �� ��° ����
*	\param t0[in] �ﰢ���� ù ��° ������ �ؽ�ó ��ǥ
*	\param t1[in] �ﰢ���� �� ��° ������ �ؽ�ó ��ǥ
*	\param t2[in] �ﰢ���� �� ��° ������ �ؽ�ó ��ǥ
*	\param n0[in] �ﰢ���� ù ��° ������ ����
*	\param n1[in] �ﰢ���� �� ��° ������ ����
*	\param n2[in] �ﰢ���� �� ��° ������ ����
*	\param pMtl[in] �ﰢ���� ����ϴ� ������ ���� ������
*	\param GroupName[in] �ﰢ���� ���� �׷��� �̸�
*/
EgFace *create_face(EgVertex *v0, EgVertex *v1, EgVertex *v2, EgTexel *t0, EgTexel *t1, EgTexel *t2, EgNormal *n0, EgNormal *n1, EgNormal *n2, EgMaterial *pMtl, std::string GroupName)
{
	return new EgFace(v0, v1, v2, t0, t1, t2, n0, n1, n2, pMtl, GroupName);
}

/*!
*	\biref	�޽� ������ �����Ѵ�.
*
*	\param	mtl_name[in] ������ ������ �̸�
*	\param	ar[in] �ֺ��� red �ݻ���
*	\param	ag[in] �ֺ��� green �ݻ���
*	\param	ab[in] �ֺ��� blue �ݻ���
*	\param	dr[in] ���ݻ� red �ݻ���
*	\param	dg[in] ���ݻ� green �ݻ���
*	\param	db[in] ���ݻ� blue �ݻ���
*	\param	sr[in] ���ݻ� red �ݻ���
*	\param	sg[in] ���ݻ� green �ݻ���
*	\param	sb[in] ���ݻ� blue �ݻ���
*	\param	ns[in] ���ݻ� ����
*
*	\return ������ ������ �����͸� ��ȯ�Ѵ�.
*/
EgMaterial *create_material(std::string mtl_name, float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, float ns)
{
	EgMaterial *pMtl = new EgMaterial(mtl_name);
	pMtl->SetAmbient(ar, ag, ab);
	pMtl->SetDiffuse(dr, dg, db);
	pMtl->SetSpecular(sr, sg, sb);
	pMtl->SetShiness(ns);
	return pMtl;
}

/*!
*	\brief ������ ������ �����Ѵ�.
*
*	\return ������ ������ �����͸� ��ȯ�Ѵ�.
*/
EgMaterial *create_material_random()
{
	static int mtl_idx = 0;
	static bool bInit = false;
	if (!bInit)
	{
		srand((unsigned)time(NULL));
		bInit = true;
	}

	float r = (float)rand() / (float)RAND_MAX;
	float g = (float)rand() / (float)RAND_MAX;
	float b = (float)rand() / (float)RAND_MAX;

	std::string MtlName = "Mtl_" + cast_str(mtl_idx++);
	EgMaterial *pMtl = new EgMaterial(MtlName);
	pMtl->SetAmbient(r * 0.2f, g * 0.2f, b * 0.2f);
	pMtl->SetDiffuse(r, g, b);
	pMtl->SetSpecular(0.7f, 0.7f, 0.7f);

	return pMtl;
}

/*!
*	\biref	2D �ؽ�ó�� �����Ѵ�.
*
*	\param	file_name[in] �ؽ�ó ������ �̸�
*
*	\return ������ �׽�ó�� �����͸� ��ȯ�Ѵ�.
*/
EgTexture *create_texture(std::string file_name)
{
	EgTexture *pTexture = new EgTexture();
	if (pTexture->LoadTexture(file_name.c_str()))
	{
		return pTexture;
	}
	else
	{
		delete pTexture;
		return NULL;
	}	
}

/*!
*	\biref	��� ������ ä��� �ʱ� �޽��� �����Ѵ�.
*
*	\param	Verts[in] ��� ������ ����Ʈ
*
*	\return ������ �޽��� ��ȯ�Ѵ�.
*/
EgMesh *create_mesh_from_bndry_verts(std::vector<EgVertex *> &Verts)
{
	// ��� �޽��� �����Ѵ�.
	static int id = 0;
	EgMesh *pMesh = ::create_mesh("Patch_" + cast_str(id++));
	
	// ��� ������ �����Ͽ� �޽��� �߰��Ѵ�.
	for (EgVertex *v : Verts)
	{
		EgVertex *_v = ::create_vertex(v->m_Pos[0], v->m_Pos[1], v->m_Pos[2]);
		_v->m_Color = v->GetAvgNormal();
		pMesh->AddVertex(_v);
	}

	// ��� ������ ����Ʈ�� �����ϰ�, ���(Ȧ)�� ���θ� ä���.
	std::vector<EgVertex *> BndryVerts = pMesh->m_pVerts;
	while (true)
	{
		// ���� ��� ������ ���� ���ϰ�,
		int NumVert = (int)BndryVerts.size();

		// ��� ������ 3�� ���Ҵٸ�,
		if (NumVert == 3)
		{
			// �ﰢ���� �����Ͽ� �޽��� �߰��ϰ�, �ݺ��� �����Ѵ�.
			EgFace *f = ::create_face(BndryVerts[0], BndryVerts[1], BndryVerts[2]);
			pMesh->AddFace(f);
			break;
		}

		// ���հ��� �ּ��� ��� ������ ã�´�.
		EgVertex *v0, *v1, *v2;
		double min_angle = INFINITY;
		std::vector<EgVertex *>::iterator min_it;

		// ������ ��� ������ ���Ͽ�
		for (int j = 0; j < NumVert; ++j)
		{
			// �̿��� �� �������� ã��,
			EgVertex *vi = (j == 0) ? BndryVerts[NumVert - 1] : BndryVerts[j - 1];
			EgVertex *vj = BndryVerts[j];
			EgVertex *vk = (j == NumVert - 1) ? BndryVerts[0] : BndryVerts[j + 1];

			// �̿��� �𼭸��� ���հ��� ����Ͽ�
			EgVec3 e0 = vi->m_Pos - vj->m_Pos;
			EgVec3 e1 = vk->m_Pos - vj->m_Pos;
			EgVec3 N0 = (vj->m_Color == EgVec3(0.0f, 0.0f, 0.0f)) ? (e1 ^ e0).Normalize() : vj->m_Color;
			double theta = angle(e1, e0, N0, false);

			// �ּ� ���հ��� ���� �� ������ ã�´�.
			if (theta < min_angle)
			{
				min_angle = theta;
				v0 = vi;
				v1 = vj;
				v2 = vk;
				min_it = BndryVerts.begin() + j;
			}
		}

		// ���հ��� �ּ��� �� �������� �ﰢ���� �����Ͽ� �޽��� �߰��Ѵ�.
		EgFace *f = ::create_face(v0, v1, v2);
		pMesh->AddFace(f);

		// ���� ���� ���հ��� ���� ������ ������� ����Ʈ���� �����Ѵ�.
		BndryVerts.erase(min_it);
	}

	// ������ �޽��� ������ ������ �߰��ϰ� �����Ѵ�.
	EgMaterial *pMtl = ::create_material_random();
	pMesh->AddMaterial(pMtl);
	pMesh->SetMaterial(pMtl);
	
	// �޽��� ���� ����, ����, �������� ������ �����ϰ�,
	pMesh->InitEdgeMate();
	pMesh->UpdateNormal(FLAT);
	pMesh->UpdateBndBox();

	// ��� �޽��� ��ȯ�Ѵ�.
	return pMesh;
}

/*!
*	\biref	�ټ��� �޽��� �����Ͽ� ���ο� �޽��� �����Ѵ�.
*	\note	���������� ���յǰ� ������ �ؽ�ó ������ ���յ��� �ʴ´�.
*
*	\param	MeshList[in] ������ �޽�����Ʈ
*
*	\return ������ �޽��� ��ȯ�Ѵ�.
*/
EgMesh *create_mesh_by_merging(std::vector<EgMesh *> &MeshList)
{
	printf("Merging multiple meshes...\n");

	// ��� �޽��� ���� 2�� �̸��̶�� �����Ѵ�.
	if (MeshList.size() < 2)
	{
		printf("Merging multiple meshes incomplete...\n\n");
		return NULL;
	}

	// ���ο� �̸��� ���� ��� �޽��� �����Ѵ�.
	static int id = 0;
	EgMesh *pRetMesh = ::create_mesh("Merged Mesh_" + cast_str(id++));

	// ������ �޽��� ���Ͽ�
	for (EgMesh *pMesh : MeshList)
	{
		// �� �޽��� ���� ������ �ε����� �����Ѵ�.
		int BaseIdx = pRetMesh->GetNumVert();

		// ������ �����Ͽ� �޽��� �߰��Ѵ�.
		pMesh->UpdateVertIdx();
		for (EgVertex *v : pMesh->m_pVerts)
			pRetMesh->AddVertex(::create_vertex(v->m_Pos[0], v->m_Pos[1], v->m_Pos[2]));

		// �ﰢ���� �����Ͽ� �޽��� �߰��Ѵ�.
		for (EgFace *f : pMesh->m_pFaces)
		{
			int *vIdx = f->GetVertexIdxs();
			EgVertex *v0 = pRetMesh->GetVert(BaseIdx + vIdx[0]);
			EgVertex *v1 = pRetMesh->GetVert(BaseIdx + vIdx[1]);
			EgVertex *v2 = pRetMesh->GetVert(BaseIdx + vIdx[2]);
			EgFace *f = ::create_face(v0, v1, v2);
			pRetMesh->AddFace(f);
		}
	}

	// �޽��� ����, ����, �������� ������ �����Ѵ�.
	pRetMesh->InitEdgeMate();
	pRetMesh->UpdateNormal(FLAT);
	pRetMesh->UpdateBndBox();
	printf("Merging multiple meshes complete...\n\n");

	// ��� �޽��� ��ȯ�Ѵ�.
	return pRetMesh;
}

/*!
*	\brief �������� �־��� �ݰ��̳��� �ﰢ������ �޽��� �����Ѵ�.
*
*	\param	pMesh[in]	��� �޽�
*	\param	S[in]		�ҽ� ����
*	\param	R[in]		�ݰ�
*
*	\return	�ҽ� �������� �ݰ� R�̳��� ���Ե� �ﰢ������ ���ο� �޽��� �����Ͽ� ��ȯ�Ѵ�.
*/
EgMesh *create_mesh_from_geodesic_dist(EgMesh *pMesh, EgVertex *S, double R)
{
	// �޽��� �����Ѵ�.
	static int id = 0;
	EgMesh *pRetMesh = ::create_mesh(pMesh->m_Name + std::string("_sub_mesh_") + cast_str(id++));

	// �ҽ� �������� �ݰ� R�̳��� �ﰢ�� ������ ���Ѵ�.
	std::vector<double> Distances;
	::get_geodesic_dist_field(pMesh, S, Distances, R);
	std::vector<EgFace *> Faces;
	for (EgFace *f : pMesh->m_pFaces)
	{
		for (int i = 0; i < 3; ++i)
		{
			int vidx = f->GetVertex(i)->m_Idx;
			if (Distances[vidx] < INFINITY)
			{
				Faces.push_back(f);
				break;
			}
		}
	}

	// �� �ﰢ���� ���Ͽ�
	for (EgFace *f : Faces)
	{
		// ������ �����Ͽ� �߰��ϰ�
		EgVertex *v0 = new EgVertex(POS(V0(f)));
		EgVertex *v1 = new EgVertex(POS(V1(f)));
		EgVertex *v2 = new EgVertex(POS(V2(f)));
		pRetMesh->AddVertex(v0);
		pRetMesh->AddVertex(v1);
		pRetMesh->AddVertex(v2);

		// �ﰢ���� �߰��Ѵ�.		
		pRetMesh->AddFace(::create_face(v0, v1, v2));
	}

	// �ߺ��� ������ �����ϰ� ��� �޽��� ��ȯ�Ѵ�.
	::delete_duplicate_verts(pRetMesh);
	pRetMesh->InitEdgeMate();
	pRetMesh->UpdateBndBox();
	pRetMesh->UpdateNormal(FLAT);
	return pRetMesh;
}

