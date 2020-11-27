#include "..\usr\include\egmesh\egmesh.h"
#include <cstring>
#include <vector>
#include <map>

/*!
*	\brief 메쉬의 non-manifold 정점을 찾는다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*	\param Verts[out] Non-manifold 정점이 저장된다.
*
*	\return Non-manifold 정점의 개수를 반환한다.
*/
int find_non_manifold_verts(EgMesh *pMesh, std::vector<EgVertex *> &Verts)
{
	// 정점의 리스트를 초기화 한다.
	Verts.clear();
	if (pMesh == NULL)
		return 0;
	
	// 메쉬 각 정점 V에 대하여
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// V에서 시작하는 에지를 연결된 삼각형 별로 클러스터링 한다.
		std::map<EgEdge *, int> EdgeVisitMap;
		std::vector<std::vector<EgEdge *>> EdgeClusterList;
		for (EgEdge *ei : v->m_pEdges)
		{
			// 미리 클러스터링된 에지라면 생략한다.
			if (EdgeVisitMap[ei] == 1)
				continue;

			// 클러스터링의 시작 에지 ej를 구한다.
			EgEdge *ej = ei;
			while (true)
			{
				if (ej->IsBndry())
					break;
				ej = ej->m_pMate->m_pNext;
				if (ej == ei)
					break;
			}

			// 에지 ej를 시작으로 반시계 방향으로 클러스터링을 수행한다. 
			EdgeClusterList.push_back(std::vector<EgEdge *>());
			int idx = (int)EdgeClusterList.size() - 1;
			EgEdge *ek = ej;
			while (true)
			{
				EdgeVisitMap[ek] = 1;
				EdgeClusterList[idx].push_back(ek);
				if (ek->m_pNext->m_pNext->IsBndry())
					break;
				ek = ek->m_pNext->m_pNext->m_pMate;
				if (ek == ej)
					break;
			}
		}

		// 클러스터 수가 2개 이상이라면 non-manifold 정점으로 추가한다.
		if (EdgeClusterList.size() >= 2)
			Verts.push_back(v);
	}

	// non-manifold 정점의 개수를 반환한다.
	return (int)Verts.size();
}

/*!
*	\brief Non-manifold 에지를 찾는다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*	\param Faces[out] Non-manifold 에지를 포함하는 삼각형이 저장된다.
*
*	\return Non-manifold 삼각형의 개수를 반환한다.
*/
int find_non_manifold_edges(EgMesh *pMesh, std::vector<EgFace *> &Faces)
{
	if (pMesh == NULL)
		return 0;

	// 각각의 정점에 대하여
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// 정점에서 시작하는 각각의 에지에 대하여
		for (EgEdge *e1 : v->m_pEdges)
		{
			std::vector<EgFace *> tmp;
			// 에지의 다음 정점에서 시작하는 각각의 에지에 대하여
			for (EgEdge *e2 : e1->m_pNext->m_pVert->m_pEdges)
			{
				// e2가 e1의 mate 에지라면, e2의 삼각형을 tmp에 추가한다.
				if (IS_MATE_EDGE(e1, e2))
					tmp.push_back(e2->m_pFace);
			}

			// 두 개 이상의 메이트 에지가 있다면
			if (tmp.size() > 1)
			{
				// 각각의 메이트 에지가 포함된 삼각형을 리스트에 추가한다.
				for (EgFace *f : tmp)
					Faces.push_back(f);
			}
		}
	}

	// 중복된 삼각형 객체를 제거한다.
	sort(Faces.begin(), Faces.end());
	Faces.erase(unique(Faces.begin(), Faces.end()), Faces.end());

	return (int)Faces.size();
}

/*!
*	\brief 퇴화된 삼각형을 찾는다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*	\param Faces[out] 퇴화된 삼각형들이 저장된다.
*	\param rate[in] 퇴화된 삼각형을 판단하기 위한 메쉬의 삼각형 평균 면적대비 비율
*
*	\return 퇴화된 삼각형의 개수를 반환한다.
*/
int find_degenerated_faces(EgMesh *pMesh, std::vector<EgFace *> &Faces, double rate)
{
	if (pMesh == NULL)
		return 0;
	Faces.clear();

	// 메쉬 삼각형의 평균 면적을 계산한다.
	double avgArea = 0.0;
	for (EgFace *f : pMesh->m_pFaces)
		avgArea += f->GetArea();
	avgArea /= pMesh->GetNumFace();

	// 각각의 삼각형에 대하여
	double eps = avgArea * rate;
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 삼각형의 면적을 계산하여
		double area = f->GetArea();

		// eps 보다 작다면 리스트에 추가한다.
		if (ABS(area) < eps)
			Faces.push_back(f);
	}

	// 퇴화된 삼각형의 개수를 반환한다.
	return (int)Faces.size();
}

/*!
*	\brief 메쉬의 사용되지 않는 정점을 찾는다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*	\param Verts[out] 미사용 정점이 저장된다.
*
*	\return 사용되지 않는 정점의 수를 반환한다.
*/
int find_unused_verts(EgMesh *pMesh, std::vector<EgVertex *> &Verts)
{
	if (pMesh == NULL)
		return 0;

	// 정점이 없다면 리턴한다.
	if (pMesh->m_pVerts.empty())
		return 0;

	// 정점의 리스트를 초기화 한다.
	Verts.clear();

	// 모든 정점의 인덱스를 -1로 초기화 한다.
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_Idx = -1;

	// 각각의 삼각형마다
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 에지가 사용하는 정점의 인덱스를 1로 표기한다.
		for (int i = 0; i < 3; ++i)
			f->GetVertex(i)->m_Idx = 1;
	}

	// 각각의 정점에 대하여
	int vidx = 0;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Idx == -1) // 미사용 정점이라면 Vert에 추가하고,
			Verts.push_back(v);

		// 정점의 인덱스를 설정한다.
		v->m_Idx = vidx++;
	}

	// 미사용 정점의 수를 반환한다.
	return (int)Verts.size();
}

/*!
*	\brief 메쉬에서 사용되지 않는 텍셀을 찾는다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*	\param Texels[out] 미사용 텍셀이 저장된다.
*
*	\return 사용되지 않는 텍셀의 수를 반환한다.
*/
int find_unused_texels(EgMesh *pMesh, std::vector<EgTexel *> &Texels)
{
	if (pMesh == NULL)
		return 0;

	// 텍셀이 없다면 리턴한다.
	if (pMesh->m_pTexels.empty())
		return 0;

	// 텍셀 리스트를 초기화 한다.
	Texels.clear();

	// 모든 텍셀의 인덱스를 -1로 초기화한다.
	for (EgTexel *t : pMesh->m_pTexels)
		t->m_Idx = -1;

	// 각각의 삼각형마다
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 에지가 사용하는 텍셀의 인덱스를 1로 표기한다.
		for (int i = 0; i < 3; ++i)
			f->GetTexel(i)->m_Idx = 1;
	}

	// 각각의 텍셀에 대하여
	int tidx = 0;
	for (EgTexel *t : pMesh->m_pTexels)
	{
		if (t->m_Idx == -1) // 미사용 텍셀이라면 리스트에 추가하고,
			Texels.push_back(t);

		t->m_Idx = tidx++;
	}

	// 제거된 텍셀의 수를 반환한다.
	return (int)Texels.size();
}

/*!
*	\brief 메쉬에서 사용되지 않는 법선을 찾는다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*	\param Normals[out] 미사용 법선이 저장된다.
*
*	\return 사용되지 않는 법선의 수를 반환한다.
*/
int find_unused_normals(EgMesh *pMesh, std::vector<EgNormal *> &Normals)
{
	if (pMesh == NULL)
		return 0;

	// 법선이 없다면 리턴한다.
	if (pMesh->m_pNormals.empty())
		return 0;

	// 법선 리스트를 초기화 한다.
	Normals.clear();

	// 모든 법선의 인덱스를 -1로 초기화 한다.
	for (EgNormal *n : pMesh->m_pNormals)
		n->m_Idx = -1;

	// 각각의 삼각형마다
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 에지가 사용하는 법선의 인덱스를 1로 표기한다.
		for (int i = 0; i < 3; ++i)
			f->GetNormal(i)->m_Idx = 1;
	}

	// 각각의 법선에 대하여
	int nidx = 0;
	for (EgNormal *n : pMesh->m_pNormals)
	{
		if (n->m_Idx == -1) // 미사용된 법선이라면 리스트에 추가하고,
			Normals.push_back(n);

		n->m_Idx = nidx++;
	}

	// 제거된 법선의 수를 반환한다.
	return (int)Normals.size();
}

/*!
*	\brief Non-manifold 정점을 제거한다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*
*	\return 제거된 Non-manifold 정점의 개수를 반환한다.
*/
int delete_non_manifold_verts(EgMesh *pMesh)
{
	if (pMesh == NULL)
		return 0;

	// Non-manifold 정점의 리스트를 구한다.
	std::vector<EgVertex *> Verts;
	int NumVert = ::find_non_manifold_verts(pMesh, Verts);

	// Non-manifold 정점을 제거하고, 
	pMesh->DeleteVerts(Verts);

	// 제거된 정점의 수를 반환한다.
	return NumVert;
}

/*!
*	\brief Non-manifold 에지를 제거한다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*
*	\return 제거한 Non-manifold 에지를 포함한 삼각형의 개수를 반환한다.
*/
int delete_non_manifold_edges(EgMesh *pMesh)
{
	if (pMesh == NULL)
		return 0;

	// Non-manifold 에지를 포함한 삼각형을 구하고
	std::vector<EgFace *> Faces;
	int NumFace = find_non_manifold_edges(pMesh, Faces);

	// Non-manifold 삼각형을 제거하고
	pMesh->DeleteFaces(Faces);

	// 제거된 삼각형의 개수를 반환한다.
	return NumFace;
}

/*!
*	\brief 퇴화된 삼각형을 제거한다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*
*	\return 제거한 삼각형의 개수를 반환한다.
*/
int delete_degenerated_faces(EgMesh *pMesh)
{
	if (pMesh == NULL)
		return 0;

	//퇴화된 삼각형을 구하여,
	std::vector<EgFace *> Faces;
	int NumFace = find_degenerated_faces(pMesh, Faces);

	// 삼각형을 제거하고,
	pMesh->DeleteFaces(Faces);

	// 제거된 삼각형의 개수를 반환한다.
	return NumFace;
}

/*!
*	\brief 메쉬에서 사용되지 않는 정점을 제거한다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*
*	\return 제거된 정점의 수를 반환한다.
*/
int delete_unused_verts(EgMesh *pMesh)
{
	if (pMesh == NULL)
		return 0;

	// 정점이 없다면 리턴한다.
	if (pMesh->m_pVerts.empty())
		return 0;

	// 모든 정점의 인덱스를 -1로 초기화 한다.
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_Idx = -1;

	// 각각의 삼각형마다
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 에지가 사용하는 정점의 인덱스를 1로 표기한다.
		for (int i = 0; i < 3; ++i)
			f->GetVertex(i)->m_Idx = 1;
	}

	// 사용되는 정점으로 구성될 새로운 리스트를 정의하고,
	std::vector<EgVertex *> NewVerts;

	// 각각의 정점에 대하여
	int vidx = 0;
	int rmv_cnt = 0;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Idx == 1) // 사용된 정점이라면 리스트에 추가하고,
		{
			v->m_Idx = vidx++;
			NewVerts.push_back(v);
		}
		else // 사용되지 않은 정점이라면 제거한다.
		{
			rmv_cnt++;
			delete v;
		}
	}

	// 정점의 리스트를 업데이트 한다.
	pMesh->m_pVerts = NewVerts;

	// 제거된 정점의 수를 반환한다.
	return rmv_cnt;
}

/*!
*	\brief 메쉬에서 사용되지 않는 텍셀을 제거한다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*
*	\return 제거된 텍셀의 수를 반환한다.
*/
int delete_unused_texels(EgMesh *pMesh)
{
	if (pMesh == NULL)
		return 0;

	// 텍셀이 없다면 리턴한다.
	if (pMesh->m_pTexels.empty())
		return 0;

	// 모든 텍셀의 인덱스를 -1로 초기화한다.
	for (EgTexel *t : pMesh->m_pTexels)
		t->m_Idx = -1;

	// 각각의 삼각형마다
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 에지가 사용하는 텍셀의 인덱스를 1로 표기한다.
		for (int i = 0; i < 3; ++i)
			f->GetTexel(i)->m_Idx = 1;
	}

	// 사용되는 텍셀로 구성될 새로운 리스트를 정의하고,
	std::vector<EgTexel *> NewTexels;

	// 각각의 텍셀에 대하여
	int tidx = 0;
	int rmv_cnt = 0;
	for (EgTexel *t : pMesh->m_pTexels)
	{
		if (t->m_Idx == 1) // 사용된 텍셀이라면 리스트에 추가하고,
		{
			t->m_Idx = tidx++;
			NewTexels.push_back(t);
		}
		else // 사용되지 않는 텍셀이라면 제거한다.
		{
			rmv_cnt++;
			delete t;
		}
	}

	// 텍셀의 리스트를 업데이트 한다.
	pMesh->m_pTexels = NewTexels;

	// 제거된 텍셀의 수를 반환한다.
	return rmv_cnt;
}

/*!
*	\brief 메쉬에서 사용되지 않는 법선을 제거한다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*
*	\return 제거된 법선의 수를 반환한다.
*/
int delete_unused_normals(EgMesh *pMesh)
{
	if (pMesh == NULL)
		return 0;

	// 법선이 없다면 리턴한다.
	if (pMesh->m_pNormals.empty())
		return 0;

	// 모든 법선의 인덱스를 -1로 초기화 한다.
	for (EgNormal *n : pMesh->m_pNormals)
		n->m_Idx = -1;

	// 각각의 삼각형마다
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 에지가 사용하는 법선의 인덱스를 1로 표기한다.
		for (int i = 0; i < 3; ++i)
			f->GetNormal(i)->m_Idx = 1;
	}

	// 사용되는 법선으로 구성될 새로운 리스트를 정의하고,
	std::vector<EgNormal *> NewNormals;

	// 각각의 법선에 대하여
	int nidx = 0;
	int rmv_cnt = 0;
	for (EgNormal *n : pMesh->m_pNormals)
	{
		if (n->m_Idx == 1) // 사용된 법선이라면 리스트에 추가하고,
		{
			n->m_Idx = nidx++;
			NewNormals.push_back(n);
		}
		else // 사용되지 않은 법선이라면 제거한다.
		{
			rmv_cnt++;
			delete n;
		}
	}

	// 법선의 리스트를 업데이트 한다.
	pMesh->m_pNormals = NewNormals;

	// 제거된 법선의 수를 반환한다.
	return rmv_cnt;
}

/*!
*	\brief 그리드에서 중복된 정점을 조사한다.
*
*	\param pGrid[IN/OUT] 정점이 저장된(될) 그리드(10 x 10 x 10이지만 1차원으로 표현)
*	\param v[in] 조사할 정점의 포인터
*	\param Box[in] 정점이 포함된 메쉬의 경계 상자
*	\param bDuplicate[out] 중복된 정점이면 true값이, 새로운 정점이면 false값이 저장된다.
*
*	\return 중복된 정점이면 최초 정점의 포인터를 반환하고, 아니면 새로 정점을 생성하여 포인터를 반환한다.
*/
static EgVertex *find_duplicate_vertex(std::vector<EgVertex *> *pGrid, EgVertex *v, EgBox &Box, bool &bDuplicate)
{
	// 정점의 위치, 경계상자의 코너점, 경계상자의 크기를 구한다.
	double x = v->m_Pos[0];
	double y = v->m_Pos[1];
	double z = v->m_Pos[2];
	double ox = Box.m_MinPt[0];
	double oy = Box.m_MinPt[1];
	double oz = Box.m_MinPt[2];
	double sx = Box.GetWidth();
	double sy = Box.GetHeight();
	double sz = Box.GetDepth();

	// 정점이 포함된(될) 그리드의 인덱스를 구한다.
	int idx[3] = { 9, 9, 9 };
	for (int i = 0; i < 9; ++i)
	{
		double st = ox + i * sx / 9.0;
		double ed = ox + (i + 1) * sx / 9.0;
		if (x >= st && x < ed)
		{
			idx[0] = i;
			break;
		}
	}
	for (int i = 0; i < 9; ++i)
	{
		double st = oy + i * sy / 9.0;
		double ed = oy + (i + 1) * sy / 9.0;
		if (y >= st && y < ed)
		{
			idx[1] = i;
			break;
		}
	}
	for (int i = 0; i < 9; ++i)
	{
		double st = oz + i * sz / 9.0;
		double ed = oz + (i + 1) * sz / 9.0;
		if (z >= st && z < ed)
		{
			idx[2] = i;
			break;
		}
	}

	// 3차원 그리드 인덱스를 1차원으로 매핑하여,
	int gidx = idx[0] * 100 + idx[1] * 10 + idx[2];

	// 해당 그리드에 포함된 기존의 각각의 정점에 대하여
	for (EgVertex *v0 : pGrid[gidx])
	{
		// 정점이 중복되었다면 최초 정점의 포인터를 반환한다.
		//if (v0->m_Pos == v->m_Pos)
		if (dist_sq(v0->m_Pos, v->m_Pos) < 0.00001)
		{
			bDuplicate = true;
			return v0;
		}
	}

	// 새로운 정점이라면 복사하여 새로운 정점을 생성하여 그리드에 추가한다.
	EgVertex *nv = new EgVertex(v->m_Pos[0], v->m_Pos[1], v->m_Pos[2]);
	pGrid[gidx].push_back(nv);

	// 새로 생성한 정점의 포인터를 반환한다.
	bDuplicate = false;
	return nv;
}

/*!
*	\brief 메쉬에서 중복된 정점을 하나로 합친다.
*
*	\param pMesh[in] 대상 메쉬에 대한 포인터
*
*	\return 메쉬의 최종 정점의 수를 반환한다.
*/
int delete_duplicate_verts(EgMesh *pMesh)
{
	// 병합된 정점과 삼각형을 저장할 변수를 선언한다.
	std::vector<EgFace *> NewFaces;
	std::vector<EgVertex *> Grid[1000];

	// 각각의 삼각형에 대하여
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 세 정점을 구하여
		EgVertex *v0 = f->GetVertex(0);
		EgVertex *v1 = f->GetVertex(1);
		EgVertex *v2 = f->GetVertex(2);

		// 중복된 정점이라며 기존 정점의 포인터를 구하고, 아니면 새로운 정점을 생성한다.
		bool bDuplicate;
		v0 = find_duplicate_vertex(Grid, v0, pMesh->m_BndBox, bDuplicate);
		v1 = find_duplicate_vertex(Grid, v1, pMesh->m_BndBox, bDuplicate);
		v2 = find_duplicate_vertex(Grid, v2, pMesh->m_BndBox, bDuplicate);

		// 이전 삼각형의 텍셀과 법선을 구하여 에지를 생성하고
		EgTexel *t0 = f->GetEdge(0)->m_pTexel;
		EgTexel *t1 = f->GetEdge(1)->m_pTexel;
		EgTexel *t2 = f->GetEdge(2)->m_pTexel;
		EgNormal *n0 = f->GetEdge(0)->m_pNormal;
		EgNormal *n1 = f->GetEdge(1)->m_pNormal;
		EgNormal *n2 = f->GetEdge(2)->m_pNormal;
		EgEdge *e0 = new EgEdge(v0, t0, n0);
		EgEdge *e1 = new EgEdge(v1, t1, n1);
		EgEdge *e2 = new EgEdge(v2, t2, n2);

		// 새로운 삼각형을 생성하여 리스트에 추가한다.
		EgFace *nf = new EgFace(e0, e1, e2, f->m_pMaterial, f->m_GroupName);
		NewFaces.push_back(nf);
	}

	// 기존 정점의 리스트와 삼각형 리스트를 제거한다.
	for (EgVertex *v : pMesh->m_pVerts)
		delete v;
	for (EgFace *f : pMesh->m_pFaces)
		delete f;
	pMesh->m_pVerts.clear();
	pMesh->m_pFaces.clear();

	// 새로운 정점의 리스트와 삼각형 리스트를 생성한다.
	for (int i = 0; i < 1000; ++i)
		for (EgVertex *v : Grid[i])
			pMesh->AddVertex(v);
	for (EgFace *f : NewFaces)
		pMesh->AddFace(f);

	// 에지의 연결관계, 법선, 경계상자를 갱신한다.
	pMesh->InitEdgeMate();
	pMesh->UpdateNormal(SMOOTH);
	pMesh->UpdateBndBox();

	// 렌더링 버퍼를 초기화 한다.
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();
	pMesh->m_TexelBuf.clear();

	// 정점의 개수를 반환한다.
	return pMesh->GetNumVert();
}

/*! 
*	\brief	세 점이 모두 경계 정점인 뾰족한 (코너) 삼각형을 제거한다. 
*	
*	\param	pMesh[in] 대상 메쉬
*
*	\return 제거된 코너 삼각형의 수를 반환한다.
*/
int delete_corner_faces(EgMesh *pMesh)
{
	// 코너 삼각형을 구하여 메쉬에서 제거한다.
	std::vector<EgFace *> SpikeFaces;
	for (EgFace *f : pMesh->m_pFaces)
	{
		if (f->GetVertex(0)->IsBndry() && 
			f->GetVertex(1)->IsBndry() && 
			f->GetVertex(2)->IsBndry())
			SpikeFaces.push_back(f);
	}
	pMesh->DeleteFaces(SpikeFaces);

	// 미사용 정점과 법선을 제거하고
	::delete_unused_verts(pMesh);
	::delete_unused_normals(pMesh);
	
	// 제거된 삼각형 수를 반환한다.
	return (int)SpikeFaces.size();
}

