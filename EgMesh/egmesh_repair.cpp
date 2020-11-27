#include "..\usr\include\egmesh\egmesh.h"
#include <cstring>
#include <vector>
#include <map>

/*!
*	\brief �޽��� non-manifold ������ ã�´�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*	\param Verts[out] Non-manifold ������ ����ȴ�.
*
*	\return Non-manifold ������ ������ ��ȯ�Ѵ�.
*/
int find_non_manifold_verts(EgMesh *pMesh, std::vector<EgVertex *> &Verts)
{
	// ������ ����Ʈ�� �ʱ�ȭ �Ѵ�.
	Verts.clear();
	if (pMesh == NULL)
		return 0;
	
	// �޽� �� ���� V�� ���Ͽ�
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// V���� �����ϴ� ������ ����� �ﰢ�� ���� Ŭ�����͸� �Ѵ�.
		std::map<EgEdge *, int> EdgeVisitMap;
		std::vector<std::vector<EgEdge *>> EdgeClusterList;
		for (EgEdge *ei : v->m_pEdges)
		{
			// �̸� Ŭ�����͸��� ������� �����Ѵ�.
			if (EdgeVisitMap[ei] == 1)
				continue;

			// Ŭ�����͸��� ���� ���� ej�� ���Ѵ�.
			EgEdge *ej = ei;
			while (true)
			{
				if (ej->IsBndry())
					break;
				ej = ej->m_pMate->m_pNext;
				if (ej == ei)
					break;
			}

			// ���� ej�� �������� �ݽð� �������� Ŭ�����͸��� �����Ѵ�. 
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

		// Ŭ������ ���� 2�� �̻��̶�� non-manifold �������� �߰��Ѵ�.
		if (EdgeClusterList.size() >= 2)
			Verts.push_back(v);
	}

	// non-manifold ������ ������ ��ȯ�Ѵ�.
	return (int)Verts.size();
}

/*!
*	\brief Non-manifold ������ ã�´�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*	\param Faces[out] Non-manifold ������ �����ϴ� �ﰢ���� ����ȴ�.
*
*	\return Non-manifold �ﰢ���� ������ ��ȯ�Ѵ�.
*/
int find_non_manifold_edges(EgMesh *pMesh, std::vector<EgFace *> &Faces)
{
	if (pMesh == NULL)
		return 0;

	// ������ ������ ���Ͽ�
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// �������� �����ϴ� ������ ������ ���Ͽ�
		for (EgEdge *e1 : v->m_pEdges)
		{
			std::vector<EgFace *> tmp;
			// ������ ���� �������� �����ϴ� ������ ������ ���Ͽ�
			for (EgEdge *e2 : e1->m_pNext->m_pVert->m_pEdges)
			{
				// e2�� e1�� mate �������, e2�� �ﰢ���� tmp�� �߰��Ѵ�.
				if (IS_MATE_EDGE(e1, e2))
					tmp.push_back(e2->m_pFace);
			}

			// �� �� �̻��� ����Ʈ ������ �ִٸ�
			if (tmp.size() > 1)
			{
				// ������ ����Ʈ ������ ���Ե� �ﰢ���� ����Ʈ�� �߰��Ѵ�.
				for (EgFace *f : tmp)
					Faces.push_back(f);
			}
		}
	}

	// �ߺ��� �ﰢ�� ��ü�� �����Ѵ�.
	sort(Faces.begin(), Faces.end());
	Faces.erase(unique(Faces.begin(), Faces.end()), Faces.end());

	return (int)Faces.size();
}

/*!
*	\brief ��ȭ�� �ﰢ���� ã�´�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*	\param Faces[out] ��ȭ�� �ﰢ������ ����ȴ�.
*	\param rate[in] ��ȭ�� �ﰢ���� �Ǵ��ϱ� ���� �޽��� �ﰢ�� ��� ������� ����
*
*	\return ��ȭ�� �ﰢ���� ������ ��ȯ�Ѵ�.
*/
int find_degenerated_faces(EgMesh *pMesh, std::vector<EgFace *> &Faces, double rate)
{
	if (pMesh == NULL)
		return 0;
	Faces.clear();

	// �޽� �ﰢ���� ��� ������ ����Ѵ�.
	double avgArea = 0.0;
	for (EgFace *f : pMesh->m_pFaces)
		avgArea += f->GetArea();
	avgArea /= pMesh->GetNumFace();

	// ������ �ﰢ���� ���Ͽ�
	double eps = avgArea * rate;
	for (EgFace *f : pMesh->m_pFaces)
	{
		// �ﰢ���� ������ ����Ͽ�
		double area = f->GetArea();

		// eps ���� �۴ٸ� ����Ʈ�� �߰��Ѵ�.
		if (ABS(area) < eps)
			Faces.push_back(f);
	}

	// ��ȭ�� �ﰢ���� ������ ��ȯ�Ѵ�.
	return (int)Faces.size();
}

/*!
*	\brief �޽��� ������ �ʴ� ������ ã�´�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*	\param Verts[out] �̻�� ������ ����ȴ�.
*
*	\return ������ �ʴ� ������ ���� ��ȯ�Ѵ�.
*/
int find_unused_verts(EgMesh *pMesh, std::vector<EgVertex *> &Verts)
{
	if (pMesh == NULL)
		return 0;

	// ������ ���ٸ� �����Ѵ�.
	if (pMesh->m_pVerts.empty())
		return 0;

	// ������ ����Ʈ�� �ʱ�ȭ �Ѵ�.
	Verts.clear();

	// ��� ������ �ε����� -1�� �ʱ�ȭ �Ѵ�.
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_Idx = -1;

	// ������ �ﰢ������
	for (EgFace *f : pMesh->m_pFaces)
	{
		// ������ ����ϴ� ������ �ε����� 1�� ǥ���Ѵ�.
		for (int i = 0; i < 3; ++i)
			f->GetVertex(i)->m_Idx = 1;
	}

	// ������ ������ ���Ͽ�
	int vidx = 0;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Idx == -1) // �̻�� �����̶�� Vert�� �߰��ϰ�,
			Verts.push_back(v);

		// ������ �ε����� �����Ѵ�.
		v->m_Idx = vidx++;
	}

	// �̻�� ������ ���� ��ȯ�Ѵ�.
	return (int)Verts.size();
}

/*!
*	\brief �޽����� ������ �ʴ� �ؼ��� ã�´�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*	\param Texels[out] �̻�� �ؼ��� ����ȴ�.
*
*	\return ������ �ʴ� �ؼ��� ���� ��ȯ�Ѵ�.
*/
int find_unused_texels(EgMesh *pMesh, std::vector<EgTexel *> &Texels)
{
	if (pMesh == NULL)
		return 0;

	// �ؼ��� ���ٸ� �����Ѵ�.
	if (pMesh->m_pTexels.empty())
		return 0;

	// �ؼ� ����Ʈ�� �ʱ�ȭ �Ѵ�.
	Texels.clear();

	// ��� �ؼ��� �ε����� -1�� �ʱ�ȭ�Ѵ�.
	for (EgTexel *t : pMesh->m_pTexels)
		t->m_Idx = -1;

	// ������ �ﰢ������
	for (EgFace *f : pMesh->m_pFaces)
	{
		// ������ ����ϴ� �ؼ��� �ε����� 1�� ǥ���Ѵ�.
		for (int i = 0; i < 3; ++i)
			f->GetTexel(i)->m_Idx = 1;
	}

	// ������ �ؼ��� ���Ͽ�
	int tidx = 0;
	for (EgTexel *t : pMesh->m_pTexels)
	{
		if (t->m_Idx == -1) // �̻�� �ؼ��̶�� ����Ʈ�� �߰��ϰ�,
			Texels.push_back(t);

		t->m_Idx = tidx++;
	}

	// ���ŵ� �ؼ��� ���� ��ȯ�Ѵ�.
	return (int)Texels.size();
}

/*!
*	\brief �޽����� ������ �ʴ� ������ ã�´�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*	\param Normals[out] �̻�� ������ ����ȴ�.
*
*	\return ������ �ʴ� ������ ���� ��ȯ�Ѵ�.
*/
int find_unused_normals(EgMesh *pMesh, std::vector<EgNormal *> &Normals)
{
	if (pMesh == NULL)
		return 0;

	// ������ ���ٸ� �����Ѵ�.
	if (pMesh->m_pNormals.empty())
		return 0;

	// ���� ����Ʈ�� �ʱ�ȭ �Ѵ�.
	Normals.clear();

	// ��� ������ �ε����� -1�� �ʱ�ȭ �Ѵ�.
	for (EgNormal *n : pMesh->m_pNormals)
		n->m_Idx = -1;

	// ������ �ﰢ������
	for (EgFace *f : pMesh->m_pFaces)
	{
		// ������ ����ϴ� ������ �ε����� 1�� ǥ���Ѵ�.
		for (int i = 0; i < 3; ++i)
			f->GetNormal(i)->m_Idx = 1;
	}

	// ������ ������ ���Ͽ�
	int nidx = 0;
	for (EgNormal *n : pMesh->m_pNormals)
	{
		if (n->m_Idx == -1) // �̻��� �����̶�� ����Ʈ�� �߰��ϰ�,
			Normals.push_back(n);

		n->m_Idx = nidx++;
	}

	// ���ŵ� ������ ���� ��ȯ�Ѵ�.
	return (int)Normals.size();
}

/*!
*	\brief Non-manifold ������ �����Ѵ�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*
*	\return ���ŵ� Non-manifold ������ ������ ��ȯ�Ѵ�.
*/
int delete_non_manifold_verts(EgMesh *pMesh)
{
	if (pMesh == NULL)
		return 0;

	// Non-manifold ������ ����Ʈ�� ���Ѵ�.
	std::vector<EgVertex *> Verts;
	int NumVert = ::find_non_manifold_verts(pMesh, Verts);

	// Non-manifold ������ �����ϰ�, 
	pMesh->DeleteVerts(Verts);

	// ���ŵ� ������ ���� ��ȯ�Ѵ�.
	return NumVert;
}

/*!
*	\brief Non-manifold ������ �����Ѵ�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*
*	\return ������ Non-manifold ������ ������ �ﰢ���� ������ ��ȯ�Ѵ�.
*/
int delete_non_manifold_edges(EgMesh *pMesh)
{
	if (pMesh == NULL)
		return 0;

	// Non-manifold ������ ������ �ﰢ���� ���ϰ�
	std::vector<EgFace *> Faces;
	int NumFace = find_non_manifold_edges(pMesh, Faces);

	// Non-manifold �ﰢ���� �����ϰ�
	pMesh->DeleteFaces(Faces);

	// ���ŵ� �ﰢ���� ������ ��ȯ�Ѵ�.
	return NumFace;
}

/*!
*	\brief ��ȭ�� �ﰢ���� �����Ѵ�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*
*	\return ������ �ﰢ���� ������ ��ȯ�Ѵ�.
*/
int delete_degenerated_faces(EgMesh *pMesh)
{
	if (pMesh == NULL)
		return 0;

	//��ȭ�� �ﰢ���� ���Ͽ�,
	std::vector<EgFace *> Faces;
	int NumFace = find_degenerated_faces(pMesh, Faces);

	// �ﰢ���� �����ϰ�,
	pMesh->DeleteFaces(Faces);

	// ���ŵ� �ﰢ���� ������ ��ȯ�Ѵ�.
	return NumFace;
}

/*!
*	\brief �޽����� ������ �ʴ� ������ �����Ѵ�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*
*	\return ���ŵ� ������ ���� ��ȯ�Ѵ�.
*/
int delete_unused_verts(EgMesh *pMesh)
{
	if (pMesh == NULL)
		return 0;

	// ������ ���ٸ� �����Ѵ�.
	if (pMesh->m_pVerts.empty())
		return 0;

	// ��� ������ �ε����� -1�� �ʱ�ȭ �Ѵ�.
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_Idx = -1;

	// ������ �ﰢ������
	for (EgFace *f : pMesh->m_pFaces)
	{
		// ������ ����ϴ� ������ �ε����� 1�� ǥ���Ѵ�.
		for (int i = 0; i < 3; ++i)
			f->GetVertex(i)->m_Idx = 1;
	}

	// ���Ǵ� �������� ������ ���ο� ����Ʈ�� �����ϰ�,
	std::vector<EgVertex *> NewVerts;

	// ������ ������ ���Ͽ�
	int vidx = 0;
	int rmv_cnt = 0;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Idx == 1) // ���� �����̶�� ����Ʈ�� �߰��ϰ�,
		{
			v->m_Idx = vidx++;
			NewVerts.push_back(v);
		}
		else // ������ ���� �����̶�� �����Ѵ�.
		{
			rmv_cnt++;
			delete v;
		}
	}

	// ������ ����Ʈ�� ������Ʈ �Ѵ�.
	pMesh->m_pVerts = NewVerts;

	// ���ŵ� ������ ���� ��ȯ�Ѵ�.
	return rmv_cnt;
}

/*!
*	\brief �޽����� ������ �ʴ� �ؼ��� �����Ѵ�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*
*	\return ���ŵ� �ؼ��� ���� ��ȯ�Ѵ�.
*/
int delete_unused_texels(EgMesh *pMesh)
{
	if (pMesh == NULL)
		return 0;

	// �ؼ��� ���ٸ� �����Ѵ�.
	if (pMesh->m_pTexels.empty())
		return 0;

	// ��� �ؼ��� �ε����� -1�� �ʱ�ȭ�Ѵ�.
	for (EgTexel *t : pMesh->m_pTexels)
		t->m_Idx = -1;

	// ������ �ﰢ������
	for (EgFace *f : pMesh->m_pFaces)
	{
		// ������ ����ϴ� �ؼ��� �ε����� 1�� ǥ���Ѵ�.
		for (int i = 0; i < 3; ++i)
			f->GetTexel(i)->m_Idx = 1;
	}

	// ���Ǵ� �ؼ��� ������ ���ο� ����Ʈ�� �����ϰ�,
	std::vector<EgTexel *> NewTexels;

	// ������ �ؼ��� ���Ͽ�
	int tidx = 0;
	int rmv_cnt = 0;
	for (EgTexel *t : pMesh->m_pTexels)
	{
		if (t->m_Idx == 1) // ���� �ؼ��̶�� ����Ʈ�� �߰��ϰ�,
		{
			t->m_Idx = tidx++;
			NewTexels.push_back(t);
		}
		else // ������ �ʴ� �ؼ��̶�� �����Ѵ�.
		{
			rmv_cnt++;
			delete t;
		}
	}

	// �ؼ��� ����Ʈ�� ������Ʈ �Ѵ�.
	pMesh->m_pTexels = NewTexels;

	// ���ŵ� �ؼ��� ���� ��ȯ�Ѵ�.
	return rmv_cnt;
}

/*!
*	\brief �޽����� ������ �ʴ� ������ �����Ѵ�.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*
*	\return ���ŵ� ������ ���� ��ȯ�Ѵ�.
*/
int delete_unused_normals(EgMesh *pMesh)
{
	if (pMesh == NULL)
		return 0;

	// ������ ���ٸ� �����Ѵ�.
	if (pMesh->m_pNormals.empty())
		return 0;

	// ��� ������ �ε����� -1�� �ʱ�ȭ �Ѵ�.
	for (EgNormal *n : pMesh->m_pNormals)
		n->m_Idx = -1;

	// ������ �ﰢ������
	for (EgFace *f : pMesh->m_pFaces)
	{
		// ������ ����ϴ� ������ �ε����� 1�� ǥ���Ѵ�.
		for (int i = 0; i < 3; ++i)
			f->GetNormal(i)->m_Idx = 1;
	}

	// ���Ǵ� �������� ������ ���ο� ����Ʈ�� �����ϰ�,
	std::vector<EgNormal *> NewNormals;

	// ������ ������ ���Ͽ�
	int nidx = 0;
	int rmv_cnt = 0;
	for (EgNormal *n : pMesh->m_pNormals)
	{
		if (n->m_Idx == 1) // ���� �����̶�� ����Ʈ�� �߰��ϰ�,
		{
			n->m_Idx = nidx++;
			NewNormals.push_back(n);
		}
		else // ������ ���� �����̶�� �����Ѵ�.
		{
			rmv_cnt++;
			delete n;
		}
	}

	// ������ ����Ʈ�� ������Ʈ �Ѵ�.
	pMesh->m_pNormals = NewNormals;

	// ���ŵ� ������ ���� ��ȯ�Ѵ�.
	return rmv_cnt;
}

/*!
*	\brief �׸��忡�� �ߺ��� ������ �����Ѵ�.
*
*	\param pGrid[IN/OUT] ������ �����(��) �׸���(10 x 10 x 10������ 1�������� ǥ��)
*	\param v[in] ������ ������ ������
*	\param Box[in] ������ ���Ե� �޽��� ��� ����
*	\param bDuplicate[out] �ߺ��� �����̸� true����, ���ο� �����̸� false���� ����ȴ�.
*
*	\return �ߺ��� �����̸� ���� ������ �����͸� ��ȯ�ϰ�, �ƴϸ� ���� ������ �����Ͽ� �����͸� ��ȯ�Ѵ�.
*/
static EgVertex *find_duplicate_vertex(std::vector<EgVertex *> *pGrid, EgVertex *v, EgBox &Box, bool &bDuplicate)
{
	// ������ ��ġ, �������� �ڳ���, �������� ũ�⸦ ���Ѵ�.
	double x = v->m_Pos[0];
	double y = v->m_Pos[1];
	double z = v->m_Pos[2];
	double ox = Box.m_MinPt[0];
	double oy = Box.m_MinPt[1];
	double oz = Box.m_MinPt[2];
	double sx = Box.GetWidth();
	double sy = Box.GetHeight();
	double sz = Box.GetDepth();

	// ������ ���Ե�(��) �׸����� �ε����� ���Ѵ�.
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

	// 3���� �׸��� �ε����� 1�������� �����Ͽ�,
	int gidx = idx[0] * 100 + idx[1] * 10 + idx[2];

	// �ش� �׸��忡 ���Ե� ������ ������ ������ ���Ͽ�
	for (EgVertex *v0 : pGrid[gidx])
	{
		// ������ �ߺ��Ǿ��ٸ� ���� ������ �����͸� ��ȯ�Ѵ�.
		//if (v0->m_Pos == v->m_Pos)
		if (dist_sq(v0->m_Pos, v->m_Pos) < 0.00001)
		{
			bDuplicate = true;
			return v0;
		}
	}

	// ���ο� �����̶�� �����Ͽ� ���ο� ������ �����Ͽ� �׸��忡 �߰��Ѵ�.
	EgVertex *nv = new EgVertex(v->m_Pos[0], v->m_Pos[1], v->m_Pos[2]);
	pGrid[gidx].push_back(nv);

	// ���� ������ ������ �����͸� ��ȯ�Ѵ�.
	bDuplicate = false;
	return nv;
}

/*!
*	\brief �޽����� �ߺ��� ������ �ϳ��� ��ģ��.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*
*	\return �޽��� ���� ������ ���� ��ȯ�Ѵ�.
*/
int delete_duplicate_verts(EgMesh *pMesh)
{
	// ���յ� ������ �ﰢ���� ������ ������ �����Ѵ�.
	std::vector<EgFace *> NewFaces;
	std::vector<EgVertex *> Grid[1000];

	// ������ �ﰢ���� ���Ͽ�
	for (EgFace *f : pMesh->m_pFaces)
	{
		// �� ������ ���Ͽ�
		EgVertex *v0 = f->GetVertex(0);
		EgVertex *v1 = f->GetVertex(1);
		EgVertex *v2 = f->GetVertex(2);

		// �ߺ��� �����̶�� ���� ������ �����͸� ���ϰ�, �ƴϸ� ���ο� ������ �����Ѵ�.
		bool bDuplicate;
		v0 = find_duplicate_vertex(Grid, v0, pMesh->m_BndBox, bDuplicate);
		v1 = find_duplicate_vertex(Grid, v1, pMesh->m_BndBox, bDuplicate);
		v2 = find_duplicate_vertex(Grid, v2, pMesh->m_BndBox, bDuplicate);

		// ���� �ﰢ���� �ؼ��� ������ ���Ͽ� ������ �����ϰ�
		EgTexel *t0 = f->GetEdge(0)->m_pTexel;
		EgTexel *t1 = f->GetEdge(1)->m_pTexel;
		EgTexel *t2 = f->GetEdge(2)->m_pTexel;
		EgNormal *n0 = f->GetEdge(0)->m_pNormal;
		EgNormal *n1 = f->GetEdge(1)->m_pNormal;
		EgNormal *n2 = f->GetEdge(2)->m_pNormal;
		EgEdge *e0 = new EgEdge(v0, t0, n0);
		EgEdge *e1 = new EgEdge(v1, t1, n1);
		EgEdge *e2 = new EgEdge(v2, t2, n2);

		// ���ο� �ﰢ���� �����Ͽ� ����Ʈ�� �߰��Ѵ�.
		EgFace *nf = new EgFace(e0, e1, e2, f->m_pMaterial, f->m_GroupName);
		NewFaces.push_back(nf);
	}

	// ���� ������ ����Ʈ�� �ﰢ�� ����Ʈ�� �����Ѵ�.
	for (EgVertex *v : pMesh->m_pVerts)
		delete v;
	for (EgFace *f : pMesh->m_pFaces)
		delete f;
	pMesh->m_pVerts.clear();
	pMesh->m_pFaces.clear();

	// ���ο� ������ ����Ʈ�� �ﰢ�� ����Ʈ�� �����Ѵ�.
	for (int i = 0; i < 1000; ++i)
		for (EgVertex *v : Grid[i])
			pMesh->AddVertex(v);
	for (EgFace *f : NewFaces)
		pMesh->AddFace(f);

	// ������ �������, ����, �����ڸ� �����Ѵ�.
	pMesh->InitEdgeMate();
	pMesh->UpdateNormal(SMOOTH);
	pMesh->UpdateBndBox();

	// ������ ���۸� �ʱ�ȭ �Ѵ�.
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();
	pMesh->m_TexelBuf.clear();

	// ������ ������ ��ȯ�Ѵ�.
	return pMesh->GetNumVert();
}

/*! 
*	\brief	�� ���� ��� ��� ������ ������ (�ڳ�) �ﰢ���� �����Ѵ�. 
*	
*	\param	pMesh[in] ��� �޽�
*
*	\return ���ŵ� �ڳ� �ﰢ���� ���� ��ȯ�Ѵ�.
*/
int delete_corner_faces(EgMesh *pMesh)
{
	// �ڳ� �ﰢ���� ���Ͽ� �޽����� �����Ѵ�.
	std::vector<EgFace *> SpikeFaces;
	for (EgFace *f : pMesh->m_pFaces)
	{
		if (f->GetVertex(0)->IsBndry() && 
			f->GetVertex(1)->IsBndry() && 
			f->GetVertex(2)->IsBndry())
			SpikeFaces.push_back(f);
	}
	pMesh->DeleteFaces(SpikeFaces);

	// �̻�� ������ ������ �����ϰ�
	::delete_unused_verts(pMesh);
	::delete_unused_normals(pMesh);
	
	// ���ŵ� �ﰢ�� ���� ��ȯ�Ѵ�.
	return (int)SpikeFaces.size();
}

