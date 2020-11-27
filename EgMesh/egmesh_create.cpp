#include "..\usr\include\egmesh\egmesh.h"

/*!
*	\biref	메쉬 객체를 생성한다.
*
*	\param	MeshName[in] 생성할 메쉬 객체의 이름
*
*	\return 생성된 메쉬의 포인터를 반환한다.
*/
EgMesh *create_mesh(std::string MeshName)
{
	return new EgMesh(MeshName);
}

/*!
*	\biref	메쉬 정점을 생성한다.
*
*	\param	x[in] 정점의 x 좌표
*	\param	y[in] 정점의 y 좌표
*	\param	z[in] 정점의 z 좌표
*
*	\return 생성된 정점의 포인터를 반환한다.
*/
EgVertex *create_vertex(double x, double y, double z)
{
	return new EgVertex(x, y, z);
}

/*!
*	\biref	텍스처 좌표를 생성한다.
*
*	\param	s[in] 텍스처 s 좌표
*	\param	t[in] 텍스처 t 좌표
*
*	\return 생성된 텍스처 좌표의 포인터를 반환한다.
*/
EgTexel *create_texel(double s, double t)
{
	return new EgTexel(s, t);
}

/*!
*	\biref	메쉬 법선을 생성한다.
*
*	\param	nx[in] 법선의 x 좌표
*	\param	ny[in] 법선의 y 좌표
*	\param	nz[in] 법선의 z 좌표
*
*	\return 생성된 법선의 포인터를 반환한다.
*/
EgNormal *create_normal(double nx, double ny, double nz)
{
	return new EgNormal(nx, ny, nz);
}

/*!
*	\brief 메쉬 삼각형을 생성한다.
*
*	\param v0[in] 삼각형의 첫 번째 정점
*	\param v1[in] 삼각형의 두 번째 정점
*	\param v2[in] 삼각형의 세 번째 정점
*	\param pMtl[in] 삼각형이 사용하는 재질에 대한 포인터
*	\param GroupName[in] 삼각형이 속한 그룹의 이름
*/
EgFace *create_face(EgVertex *v0, EgVertex *v1, EgVertex *v2, EgMaterial *pMtl, std::string GroupName)
{
	return new EgFace(v0, v1, v2, NULL, NULL, NULL, NULL, NULL, NULL, pMtl, GroupName);
}

/*!
*	\brief 메쉬 삼각형을 생성한다.
*
*	\param v0[in] 삼각형의 첫 번째 정점
*	\param v1[in] 삼각형의 두 번째 정점
*	\param v2[in] 삼각형의 세 번째 정점
*	\param t0[in] 삼각형의 첫 번째 정점의 텍스처 좌표
*	\param t1[in] 삼각형의 두 번째 정점의 텍스처 좌표
*	\param t2[in] 삼각형의 세 번째 정점의 텍스처 좌표
*	\param pMtl[in] 삼각형이 사용하는 재질에 대한 포인터
*	\param GroupName[in] 삼각형이 속한 그룹의 이름
*/
EgFace *create_face(EgVertex *v0, EgVertex *v1, EgVertex *v2, EgTexel *t0, EgTexel *t1, EgTexel *t2, EgMaterial *pMtl, std::string GroupName)
{
	return new EgFace(v0, v1, v2, t0, t1, t2, NULL, NULL, NULL, pMtl, GroupName);
}

/*!
*	\brief 메쉬 삼각형을 생성한다.
*
*	\param v0[in] 삼각형의 첫 번째 정점
*	\param v1[in] 삼각형의 두 번째 정점
*	\param v2[in] 삼각형의 세 번째 정점
*	\param t0[in] 삼각형의 첫 번째 정점의 텍스처 좌표
*	\param t1[in] 삼각형의 두 번째 정점의 텍스처 좌표
*	\param t2[in] 삼각형의 세 번째 정점의 텍스처 좌표
*	\param n0[in] 삼각형의 첫 번째 정점의 법선
*	\param n1[in] 삼각형의 두 번째 정점의 법선
*	\param n2[in] 삼각형의 세 번째 정점의 법선
*	\param pMtl[in] 삼각형이 사용하는 재질에 대한 포인터
*	\param GroupName[in] 삼각형이 속한 그룹의 이름
*/
EgFace *create_face(EgVertex *v0, EgVertex *v1, EgVertex *v2, EgTexel *t0, EgTexel *t1, EgTexel *t2, EgNormal *n0, EgNormal *n1, EgNormal *n2, EgMaterial *pMtl, std::string GroupName)
{
	return new EgFace(v0, v1, v2, t0, t1, t2, n0, n1, n2, pMtl, GroupName);
}

/*!
*	\biref	메쉬 재질을 생성한다.
*
*	\param	mtl_name[in] 생성할 재질의 이름
*	\param	ar[in] 주변광 red 반사계수
*	\param	ag[in] 주변광 green 반사계수
*	\param	ab[in] 주변광 blue 반사계수
*	\param	dr[in] 난반사 red 반사계수
*	\param	dg[in] 난반사 green 반사계수
*	\param	db[in] 난반사 blue 반사계수
*	\param	sr[in] 전반사 red 반사계수
*	\param	sg[in] 전반사 green 반사계수
*	\param	sb[in] 전반사 blue 반사계수
*	\param	ns[in] 전반사 지수
*
*	\return 생성된 재질의 포인터를 반환한다.
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
*	\brief 임의의 재질을 생성한다.
*
*	\return 생성된 재질의 포인터를 반환한다.
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
*	\biref	2D 텍스처를 생성한다.
*
*	\param	file_name[in] 텍스처 파일의 이름
*
*	\return 생성된 테스처의 포인터를 반환한다.
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
*	\biref	경계 정점을 채우는 초기 메쉬를 생성한다.
*
*	\param	Verts[in] 경계 정점의 리스트
*
*	\return 생성된 메쉬를 반환한다.
*/
EgMesh *create_mesh_from_bndry_verts(std::vector<EgVertex *> &Verts)
{
	// 결과 메쉬를 생성한다.
	static int id = 0;
	EgMesh *pMesh = ::create_mesh("Patch_" + cast_str(id++));
	
	// 경계 정점을 복사하여 메쉬에 추가한다.
	for (EgVertex *v : Verts)
	{
		EgVertex *_v = ::create_vertex(v->m_Pos[0], v->m_Pos[1], v->m_Pos[2]);
		_v->m_Color = v->GetAvgNormal();
		pMesh->AddVertex(_v);
	}

	// 경계 정점의 리스트를 복사하고, 경계(홀)의 내부를 채운다.
	std::vector<EgVertex *> BndryVerts = pMesh->m_pVerts;
	while (true)
	{
		// 남은 경계 정점의 수를 구하고,
		int NumVert = (int)BndryVerts.size();

		// 경계 정점이 3개 남았다면,
		if (NumVert == 3)
		{
			// 삼각형을 생성하여 메쉬에 추가하고, 반복을 종료한다.
			EgFace *f = ::create_face(BndryVerts[0], BndryVerts[1], BndryVerts[2]);
			pMesh->AddFace(f);
			break;
		}

		// 사잇각이 최소인 경계 정점을 찾는다.
		EgVertex *v0, *v1, *v2;
		double min_angle = INFINITY;
		std::vector<EgVertex *>::iterator min_it;

		// 각각의 경계 정점에 대하여
		for (int j = 0; j < NumVert; ++j)
		{
			// 이웃한 세 정점들을 찾아,
			EgVertex *vi = (j == 0) ? BndryVerts[NumVert - 1] : BndryVerts[j - 1];
			EgVertex *vj = BndryVerts[j];
			EgVertex *vk = (j == NumVert - 1) ? BndryVerts[0] : BndryVerts[j + 1];

			// 이웃한 모서리의 사잇각을 계산하여
			EgVec3 e0 = vi->m_Pos - vj->m_Pos;
			EgVec3 e1 = vk->m_Pos - vj->m_Pos;
			EgVec3 N0 = (vj->m_Color == EgVec3(0.0f, 0.0f, 0.0f)) ? (e1 ^ e0).Normalize() : vj->m_Color;
			double theta = angle(e1, e0, N0, false);

			// 최소 사잇각을 갖는 세 정점을 찾는다.
			if (theta < min_angle)
			{
				min_angle = theta;
				v0 = vi;
				v1 = vj;
				v2 = vk;
				min_it = BndryVerts.begin() + j;
			}
		}

		// 사잇각이 최소인 세 정점으로 삼각형을 생성하여 메쉬에 추가한다.
		EgFace *f = ::create_face(v0, v1, v2);
		pMesh->AddFace(f);

		// 가장 작은 사잇각을 갖는 정점을 경계정점 리스트에서 제거한다.
		BndryVerts.erase(min_it);
	}

	// 생성된 메쉬에 임의의 재질을 추가하고 설정한다.
	EgMaterial *pMtl = ::create_material_random();
	pMesh->AddMaterial(pMtl);
	pMesh->SetMaterial(pMtl);
	
	// 메쉬의 인접 에지, 법선, 경계상자의 정보를 갱신하고,
	pMesh->InitEdgeMate();
	pMesh->UpdateNormal(FLAT);
	pMesh->UpdateBndBox();

	// 결과 메쉬를 반환한다.
	return pMesh;
}

/*!
*	\biref	다수의 메쉬를 병합하여 새로운 메쉬를 생성한다.
*	\note	기하정보만 병합되고 재질과 텍스처 정보를 병합되지 않는다.
*
*	\param	MeshList[in] 병합할 메쉬리스트
*
*	\return 생성된 메쉬를 반환한다.
*/
EgMesh *create_mesh_by_merging(std::vector<EgMesh *> &MeshList)
{
	printf("Merging multiple meshes...\n");

	// 대상 메쉬의 수가 2개 미만이라면 리턴한다.
	if (MeshList.size() < 2)
	{
		printf("Merging multiple meshes incomplete...\n\n");
		return NULL;
	}

	// 새로운 이름을 갖는 결과 메쉬를 생성한다.
	static int id = 0;
	EgMesh *pRetMesh = ::create_mesh("Merged Mesh_" + cast_str(id++));

	// 각각의 메쉬에 대하여
	for (EgMesh *pMesh : MeshList)
	{
		// 각 메쉬의 시작 정점의 인덱스를 설정한다.
		int BaseIdx = pRetMesh->GetNumVert();

		// 정점을 복사하여 메쉬에 추가한다.
		pMesh->UpdateVertIdx();
		for (EgVertex *v : pMesh->m_pVerts)
			pRetMesh->AddVertex(::create_vertex(v->m_Pos[0], v->m_Pos[1], v->m_Pos[2]));

		// 삼각형을 복사하여 메쉬에 추가한다.
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

	// 메쉬의 에지, 법선, 경계상자의 정보를 갱신한다.
	pRetMesh->InitEdgeMate();
	pRetMesh->UpdateNormal(FLAT);
	pRetMesh->UpdateBndBox();
	printf("Merging multiple meshes complete...\n\n");

	// 결과 메쉬를 반환한다.
	return pRetMesh;
}

/*!
*	\brief 정점에서 주어진 반경이내의 삼각형으로 메쉬를 생성한다.
*
*	\param	pMesh[in]	대상 메쉬
*	\param	S[in]		소스 정점
*	\param	R[in]		반경
*
*	\return	소스 정점에서 반경 R이내에 포함된 삼각형으로 새로운 메쉬를 생성하여 반환한다.
*/
EgMesh *create_mesh_from_geodesic_dist(EgMesh *pMesh, EgVertex *S, double R)
{
	// 메쉬를 생성한다.
	static int id = 0;
	EgMesh *pRetMesh = ::create_mesh(pMesh->m_Name + std::string("_sub_mesh_") + cast_str(id++));

	// 소스 정점에서 반경 R이내의 삼각형 집합을 구한다.
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

	// 각 삼각형에 대하여
	for (EgFace *f : Faces)
	{
		// 정점을 복사하여 추가하고
		EgVertex *v0 = new EgVertex(POS(V0(f)));
		EgVertex *v1 = new EgVertex(POS(V1(f)));
		EgVertex *v2 = new EgVertex(POS(V2(f)));
		pRetMesh->AddVertex(v0);
		pRetMesh->AddVertex(v1);
		pRetMesh->AddVertex(v2);

		// 삼각형을 추가한다.		
		pRetMesh->AddFace(::create_face(v0, v1, v2));
	}

	// 중복된 정점을 제거하고 결과 메쉬를 반환한다.
	::delete_duplicate_verts(pRetMesh);
	pRetMesh->InitEdgeMate();
	pRetMesh->UpdateBndBox();
	pRetMesh->UpdateNormal(FLAT);
	return pRetMesh;
}

