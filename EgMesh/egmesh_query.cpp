#include "..\usr\include\egmesh\egmesh.h"

/*!
*	\brief	�޽��� ������ N-�� �̿� ������ ���ϴ� �Լ�
*
*	\param	pMesh[in] ��� �޽�
*	\param	pVert[in] ������ ����
*	\param	N[in] ������ �̿� ������ �ִ� ����
*	\param	RingVerts[out] ����� �����(i-�� �̿� ������ RingVerts[i][0], ..., RingVerts[i][m]�� �����)
*
*	\return ���� ������ �ִ� ������ ��ȯ�Ѵ�(N ���� ���� ���� ����)
*/
int get_neighbor_verts(EgMesh *pMesh, EgVertex *pVert, int N, std::vector<std::vector<EgVertex *>> &RingVerts)
{
	// ���� �ð��� �����Ѵ�.
	clock_t st = clock();

	// �� ���������� �Ÿ��� ���Ѵ�� �ʱ�ȭ �Ѵ�.
	std::vector<int> Distances;
	Distances.assign(pMesh->GetNumVert(), 10000000);

	// �ּ����� �����ϰ� 0���� �ʱ�ȭ�� �ҽ� ������ ������ �߰��Ѵ�.
	GeodesicHeap MinHeap;
	Distances[pVert->m_Idx] = 0;
	MinHeap.push(EgGeodesic(pVert, 0.0));

	// �ּ����� ������ ��� ó���� ������
	while (!MinHeap.empty())
	{
		// �ҽ� �������� ���� ��� ������ ������ ������
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// �̹� ó���� ������� ��ŵ�Ѵ�.
		int vidx = info.m_pVert->m_Idx;
		if ((int)info.m_Distance > Distances[vidx])
			continue;

		// ������ 1�� ���� vj�� �ִ� �Ÿ��� �����Ѵ�.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// Dijkstra �˰����� �����Ѵ�.
			EgVertex *vj = EV(e);
			EgVertex *vk = EV(e->m_pNext);
			int di = Distances[vi->m_Idx];
			int dk = Distances[vk->m_Idx];
			int d1 = (di < dk) ? (di + 1) : (dk + 1);
			int d2 = 10000000;
			if (!e->IsBndry())
			{
				vk = EV(e->m_pMate->m_pNext);
				dk = Distances[vk->m_Idx];
				d2 = (di < dk) ? (di + 1) : (dk + 1);
			}

			// ���ο� �ִ� �Ÿ��� ã�� ���
			int dj = MIN(d1, d2);
			if (dj < Distances[vj->m_Idx])
			{
				// ������ �ִ� �Ÿ��� �����ϰ�
				Distances[vj->m_Idx] = dj;
				if (dj <= N)
					MinHeap.push(EgGeodesic(vj, (double)dj));
			}
		}
	}

	// ��� ����� ����Ʈ�� �����Ѵ�.
	RingVerts.clear();
	RingVerts.assign(N + 1, std::vector<EgVertex *>());
	for (EgVertex *v : pMesh->m_pVerts)
	{
		int k = Distances[v->m_Idx];
		if (k <= N)
		{
			RingVerts[k].push_back(v);
			//v->m_EditWgt = (float)k;
		}
		//else
			//v->m_EditWgt = 0.0f;
	}
	//printf("Elapsed time for finding neighbor vertices = %u ms\n", clock() - st);

	// ���� ����� ������ ��ȯ�Ѵ�.
	for (int i = N; i >= 0; --i)
		if (!RingVerts[i].empty())
			return N;

	return -1;
}

/*!
*   \brief	�޽� ����� N-�� �̿� ������ ���ϴ� �Լ�
*   \note	���� ��, ��� ������ m_EditWgt�� 0���� �ʱ�ȭ�ȴ�.
*
*   \param  pMesh[in]		��� �޽�
*   \param  BndryVerts[in]	������ ��� ���� ����Ʈ
*   \param  N[in]			������ �̿� ������ �ִ� ���� (������ ��� ��ü �޽��� �����Ѵ�.)
*   \param  RingVerts[out]	����� �����(i-�� �̿� ������ RingVerts[i][0], ..., RingVerts[i][m]�� �����)
*
*   \return ���� ������ �ִ� ������ ��ȯ�Ѵ�(N ���� ���� ���� ����)
*/
int get_neighbor_verts(EgMesh *pMesh, std::vector<EgVertex *> &BndryVerts, int N, std::vector<std::vector<EgVertex *>> &RingVerts)
{
	// ���� �ð��� �����Ѵ�.
	clock_t st = clock();

	// ������ ���� ����Ʈ�� ��谡 �ƴ϶�� �����Ѵ�.
	for (EgVertex *v : BndryVerts)
	{
		if (!v->IsBndry())
		{
			std::cout << "get_neighbor_verts return -1" << std::endl;
			return -1;
		}
	}

	// �̹湮 ������ ǥ���Ѵ�.
	int i = -1;
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_EditWgt = (float)i;

	RingVerts.clear();
	RingVerts.push_back(std::vector<EgVertex *>());
	for (EgVertex *v : BndryVerts)
		RingVerts[0].push_back(v);

	while (true)
	{
		// N-����ŭ �����ߴٸ� ������ ����������.
		if (++i == N)
			break;

		// i-�� ��踦 ����Ѵ�.
		for (EgVertex *v : RingVerts[i])
			v->m_EditWgt = (float)i;

		// i-�� ����� 1�� ������ ���Ͽ�,
		std::vector<EgVertex *> NextRingVerts;
		for (EgVertex *vi : RingVerts[i])
		{
			std::vector<EgVertex *> Verts;
			vi->GetOneRingVert(Verts);
			for (EgVertex *vj : Verts)
				if (vj->m_EditWgt == -1.0f)
					NextRingVerts.push_back(vj);
		}

		// ���̻� �湮�� ������ ���ٸ� ������ ����������.
		if (NextRingVerts.empty())
			break;

		UNIQUE(NextRingVerts);

		// (i+1)-�� ��� �������� �����Ѵ�.
		RingVerts.push_back(std::vector<EgVertex *>());
		for (EgVertex *v : NextRingVerts)
			RingVerts[i + 1].push_back(v);
	}

	// ����ġ�� �ʱ�ȭ�Ѵ�.
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_EditWgt = 0.0f;

	//printf("Elapsed time for finding neighbor vertices of boundary= %u ms\n", clock() - st);

	// ���� ����� ������ ��ȯ�Ѵ�.
	return NUM(RingVerts) - 1;
}

/*!
*	\brief	�޽� ������ N-�� �ﰢ���� ���ϴ� �Լ�
*
*	\param	pMesh[in] ��� �޽�
*	\param	pVert[in] ������ ����
*	\param	N[in] ������ �̿� ������ �ִ� ����
*	\param	Faces[out] �ﰢ���� �����
*
*	\return ���õ� �ﰢ���� ���� ��ȯ�Ѵ�.
*/
int get_neighbor_faces(EgMesh *pMesh, EgVertex *pVert, int N, std::vector<EgFace *> &Faces)
{
	// ���� �ð��� �����Ѵ�.
	clock_t st = clock();

	// �� ���������� �Ÿ��� ���Ѵ�� �ʱ�ȭ �Ѵ�.
	std::vector<int> Distances;
	Distances.assign(pMesh->GetNumVert(), 10000000);

	// �ּ����� �����ϰ� 0���� �ʱ�ȭ�� �ҽ� ������ ������ �߰��Ѵ�.
	GeodesicHeap MinHeap;
	Distances[pVert->m_Idx] = 0;
	MinHeap.push(EgGeodesic(pVert, 0.0));

	// �ּ����� ������ ��� ó���� ������
	while (!MinHeap.empty())
	{
		// �ҽ� �������� ���� ��� ������ ������ ������
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// �̹� ó���� ������� ��ŵ�Ѵ�.
		int vidx = info.m_pVert->m_Idx;
		if ((int)info.m_Distance > Distances[vidx])
			continue;

		// ������ 1�� ���� vj�� �ִ� �Ÿ��� �����Ѵ�.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// Dijkstra �˰����� �����Ѵ�.
			EgVertex *vj = EV(e);
			EgVertex *vk = EV(e->m_pNext);
			int di = Distances[vi->m_Idx];
			int dk = Distances[vk->m_Idx];
			int d1 = (di < dk) ? (di + 1) : (dk + 1);
			int d2 = 10000000;
			if (!e->IsBndry())
			{
				vk = EV(e->m_pMate->m_pNext);
				dk = Distances[vk->m_Idx];
				d2 = (di < dk) ? (di + 1) : (dk + 1);
			}

			// ���ο� �ִ� �Ÿ��� ã�� ���
			int dj = MIN(d1, d2);
			if (dj < Distances[vj->m_Idx])
			{
				// ������ �ִ� �Ÿ��� �����ϰ�
				Distances[vj->m_Idx] = dj;
				if (dj <= N)
					MinHeap.push(EgGeodesic(vj, (double)dj));
			}
		}
	}

	// ��� ����� ����Ʈ�� �����Ѵ�.
	Faces.clear();
	for (EgFace *f : pMesh->m_pFaces)
	{
		int k0 = Distances[V0(f)->m_Idx];
		int k1 = Distances[V1(f)->m_Idx];
		int k2 = Distances[V2(f)->m_Idx];
		if (k0 <= N || k1 <= N || k2 <= N)
			Faces.push_back(f);
	}
	return NUM(Faces);
}

/*!
*	\brief	�޽� ���(Ȧ) ������ ����Ʈ�� ���Ѵ�.
*
*	\param	pMesh[in] ��� �޽�
*	\param	BndryVerts[out] ���(Ȧ) ������ ����� ����Ʈ
*	\param	bClockWise[in] true�� ��� ���(Ȧ) ������ �ð�(�ݽð�) ��������, false�� ��� ���(Ȧ) ������ �ݽð�(�ð�) �������� ����ȴ�.
*	\param	bSortByLen[in] true�� ��� ���(Ȧ)�� ���̿� ���� ������������ ���ĵȴ�.
*
*	\return ���(Ȧ)�� ���� ��ȯ�Ѵ�(non-manifold ������ �ִٸ� -1�� ��ȯ�Ѵ�).
*/
int get_bndry_verts(EgMesh *pMesh, std::vector<std::vector<EgVertex *>> &BndryVerts, bool bClockWise, bool bSortByLen)
{
	clock_t st = clock();
	// ����� ������ ����Ʈ�� �ʱ�ȭ�Ѵ�.
	BndryVerts.clear();

	// ������ �ε����� �����Ѵ�.
	pMesh->UpdateVertIdx();

	// ��� ������ ���Ͽ� <�������� �ε���, ������ �ε���>�� ǥ���� ���� �����Ѵ�.
	std::map<int, int> BndryVertIdxPairs;
	int NumBndryEdges = 0;
	for (EgFace *f : pMesh->m_pFaces)
	{
		// �� �ﰢ���� ������ �ε����� ���Ͽ�
		int vidx0 = f->GetVertex(0)->m_Idx;
		int vidx1 = f->GetVertex(1)->m_Idx;
		int vidx2 = f->GetVertex(2)->m_Idx;

		// ��� ������� �ʿ� �߰��Ѵ�.
		if (f->GetEdge(0)->IsBndry())
		{
			BndryVertIdxPairs[vidx0] = vidx1;
			NumBndryEdges++;
		}

		if (f->GetEdge(1)->IsBndry())
		{
			BndryVertIdxPairs[vidx1] = vidx2;
			NumBndryEdges++;
		}

		if (f->GetEdge(2)->IsBndry())
		{
			BndryVertIdxPairs[vidx2] = vidx0;
			NumBndryEdges++;
		}
	}

	// ��پ�ü ������ ���ԵǾ� �ִٸ� -1�� ��ȯ�Ѵ�.
	if (NumBndryEdges != (int)BndryVertIdxPairs.size())
	{
		std::vector<EgVertex *> Temp;
		// ������ �ﰢ���� ���Ͽ�
		for (EgFace *f : pMesh->m_pFaces)
		{
			// ù ��° ������ ��� �������
			if (f->GetEdge(0)->IsBndry())
			{
				// ù ��° ������ �������� ������ �߰��Ѵ�.
				Temp.push_back(f->GetVertex(0));
				Temp.push_back(f->GetVertex(1));
			}

			// �� ��° ������ ��� �������
			if (f->GetEdge(1)->IsBndry())
			{
				// �� ��° ������ �������� ������ �߰��Ѵ�.
				Temp.push_back(f->GetVertex(1));
				Temp.push_back(f->GetVertex(2));
			}

			// �� ��° ������ ��� �������
			if (f->GetEdge(2)->IsBndry())
			{
				// �� ��° ������ �������� ������ �߰��Ѵ�.
				Temp.push_back(f->GetVertex(2));
				Temp.push_back(f->GetVertex(0));
			}
		}

		// ��� ���� ����Ʈ�� �߰��ϰ�, -1�� ��ȯ�Ѵ�.
		BndryVerts.push_back(Temp);
		return -1;
	}

	// ��� �������� �ݽð� �������� �����Ѵ�.
	while (!BndryVertIdxPairs.empty())
	{
		// ������ ���(Ȧ)�� ���� ������ ���Ͽ� 
		std::map<int, int>::iterator it = BndryVertIdxPairs.begin();

		// �� ���(Ȧ)�� ���Ͽ� ������ ����Ʈ�� �����Ѵ�.
		std::vector<int> tmp;
		tmp.push_back(it->first);
		do
		{
			// ������ �ε����� �߰��ϰ�, 
			tmp.push_back(it->second);

			// �߰��� ������ ���������� ���� ��� ������ ���Ѵ�.
			it = BndryVertIdxPairs.find(tmp.back());

			// �߰��� ������ ���������� �ϴ� ��� ������ ���ų�,
			// ����� ���������� �ǵ��� �Դٸ�(������ ����) �ݺ��� ��ģ��
		} while (it != BndryVertIdxPairs.end() && it->second != tmp.front());

		// ã���� ��� ������ �����Ѵ�.
		for (int key : tmp)
			BndryVertIdxPairs.erase(key);

		// �� ��� ������ �ε����� ��� ������ ã�Ƽ� 
		std::vector<EgVertex *> EachBndryVerts;
		for (int i = 0; i < (int)tmp.size(); ++i)
		{
			int vidx;
			if (bClockWise)
				vidx = tmp[tmp.size() - 1 - i];
			else
				vidx = tmp[i];
			EachBndryVerts.push_back(pMesh->m_pVerts[vidx]);
		}

		// ��� ���� ����Ʈ�� �߰��Ѵ�.
		BndryVerts.push_back(EachBndryVerts);
	}

	// ���(Ȧ)�� ���̿� ���� �����Ѵٸ�
	if (bSortByLen)
	{
		// ������ ���(Ȧ)�� ���Ͽ� ���̸� ���Ѵ�.
		std::vector<double> BndryLen;
		for (std::vector<EgVertex *> &EachBndry : BndryVerts)
		{
			double len = ::get_hole_perimeter(EachBndry, true);
			BndryLen.push_back(len);
		}

		// ���(Ȧ)�� ���̿� ���� �����Ѵ�.
		int NumBndry = (int)BndryLen.size();
		for (int i = 0; i < NumBndry - 1; ++i)
		{
			for (int j = i + 1; j < NumBndry; ++j)
			{
				if (BndryLen[i] > BndryLen[j])
				{
					SWAP(double, BndryLen[i], BndryLen[j]);
					BndryVerts[i].swap(BndryVerts[j]);
				}
			}
		}
	}

	// Ȧ�� ������ ��ȯ�Ѵ�.
	return (int)BndryVerts.size();
}

/*!
*	\brief	�޽� ���(Ȧ) ������ ����Ʈ�� ���Ѵ�.
*	\note	Manifold �Ǵ� non-manifold ���� ���� ��� ������ ã�´�.
*
*	\param	pMesh[in] ��� �޽�
*	\param	BndryVerts[out] ���(Ȧ) ������ ����ȴ�.
*
*	\return ��� ������ ���� ��ȯ�Ѵ�.
*/
int get_bndry_verts(EgMesh *pMesh, std::vector<EgVertex *> &BndryVerts)
{
	// ����� ������ ����Ʈ�� �ʱ�ȭ�Ѵ�.
	BndryVerts.clear();

	// �� ��� ������ ����Ʈ�� ���Ѵ�.
	std::vector<std::vector<EgVertex *>> BndryList;

	// Non-manifold ���
	if (::get_bndry_verts(pMesh, BndryList, false, false) == -1)
	{
		// �� ��� ���о��� ������ �߰��Ѵ�.
		BndryVerts = BndryList[0];
	}
	else // Manifold ��� 
	{
		// �� ��躰�� ������ �߰��Ѵ�.
		for (std::vector<EgVertex *> &Bndry : BndryList)
			for (EgVertex *v : Bndry)
				BndryVerts.push_back(v);
	}

	// ��� ������ ���� ��ȯ�Ѵ�.
	return (int)BndryVerts.size();
}

/*!
*	\brief	�޽� ���(Ȧ) �ﰢ�� ����Ʈ�� ���Ѵ�.
*
*	\param	BndryVerts[in] ���(Ȧ) ������ ����Ʈ
*	\param	BndryFaces[out] ���(Ȧ) ������ ����� �ﰢ�� ����Ʈ�� ����ȴ�.
*
*	\return �ﰢ���� ���� ��ȯ�Ѵ�.
*/
int get_bndry_faces(std::vector<EgVertex *> &BndryVerts, std::vector<EgFace *> &BndryFaces)
{
	clock_t st = clock();
	// ����� ������ ����Ʈ�� �ʱ�ȭ�Ѵ�.
	BndryFaces.clear();

	// ������ ��� ������ ���Ͽ�
	for (EgVertex *v : BndryVerts)
	{
		// �������� �����ϴ� ������ ������ ���Ե� �ﰢ���� �߰��Ѵ�.
		for (EgEdge *e : v->m_pEdges)
			BndryFaces.push_back(e->m_pFace);
	}

	// �ߺ����� �߰��� �ﰢ���� �����Ѵ�.
	sort(BndryFaces.begin(), BndryFaces.end());
	BndryFaces.erase(unique(BndryFaces.begin(), BndryFaces.end()), BndryFaces.end());

	// �ﰢ���� ���� ��ȯ�Ѵ�.
	return (int)BndryFaces.size();
}

/*!
*	\brief	�޽��� ��� ������ ����� �ﰢ�� ����Ʈ�� ã�� �Լ�
*
*	\param	pMesh[in] ��� �޽�
*	\param	BndryFaces[out] ��� ������ ����� �ﰢ���� ����Ʈ�� �����
*
*	\return	�ﰢ���� ���� ��ȯ�Ѵ�.
*/
int get_bndry_faces(EgMesh *pMesh, std::vector<EgFace *> &BndryFaces)
{
	// ��� ���� �ʱ�ȭ �ϰ�
	BndryFaces.clear();

	// ��� ������ ������ �ﰢ���� ����Ʈ�� �߰��Ѵ�.
	for (EgFace *f : pMesh->m_pFaces)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (f->GetEdge(i)->IsBndry())
			{
				BndryFaces.push_back(f);
				break;
			}
		}
	}
	return (int)BndryFaces.size();
}

/*!
*	\brief �޽� ���� ������ ����Ʈ�� ���Ѵ�.
*
*	\param pMesh[in] ��� �޽�
*	\param IntrVerts[out] ���� ������ ����ȴ�.
*
*	\return ���� ������ ���� ��ȯ�Ѵ�.
*/
int get_intr_verts(EgMesh *pMesh, std::vector<EgVertex *> &IntrVerts)
{
	// ����Ʈ�� �ʱ�ȭ �ϰ�,
	IntrVerts.clear();

	// ��� ������ �ƴϸ� ����Ʈ�� �߰��Ѵ�.
	for (EgVertex *v : pMesh->m_pVerts)
		if (v->IsBndry() == false)
			IntrVerts.push_back(v);

	// ���� ������ ���� ��ȯ�Ѵ�.
	return (int)IntrVerts.size();
}

/*!
*	\brief ���� �ﰢ���� ������ ����Ʈ�� ���Ѵ�.
*
*	\param Faces[in] ���õ� �ﰢ���� ����Ʈ
*	\param Verts[out] ���õ� �ﰢ���� ������ ����ȴ�.
*
*	\return ������ ���� ��ȯ�Ѵ�.
*/
int get_verts(std::vector<EgFace *> &Faces, std::vector<EgVertex *> &Verts)
{
	// ���� �ﰢ���� ���� ������ ���Ѵ�.
	Verts.clear();
	for (EgFace *f : Faces)
	{
		Verts.push_back(V0(f));
		Verts.push_back(V1(f));
		Verts.push_back(V2(f));
	}
	sort(Verts.begin(), Verts.end());
	Verts.erase(unique(Verts.begin(), Verts.end()), Verts.end());

	return (int)Verts.size();
}

/*!
*	\brief ���� �ﰢ���� ���� ������ �ܺ� ������ ����Ʈ�� ���Ѵ�.
*
*	\param Faces[in] ���õ� �ﰢ���� ����Ʈ
*	\param IntrVerts[out] ���õ� �ﰢ���� ���� ������ ����ȴ�.
*	\param BndryVerts[out] ���õ� �ﰢ���� �ܺ� ������ ����ȴ�.
*
*	\return ������ ���� ��ȯ�Ѵ�.
*/
int get_verts(std::vector<EgFace *> &Faces, std::vector<EgVertex *> &IntrVerts, std::vector<EgVertex *> &BndryVerts)
{
	// ����� �ʱ�ȭ �ϰ�
	IntrVerts.clear();
	BndryVerts.clear();

	// ���� �ﰢ���� ����ϰ� �ε����� �����Ѵ�.
	std::vector<int> FaceIdxs;
	for (EgFace *f : Faces)
	{
		FaceIdxs.push_back(f->m_Idx);
		f->m_Idx = -1;
	}

	// ���� �ﰢ���� ���� ������ ���Ѵ�.
	std::vector<EgVertex *> Verts;
	::get_verts(Faces, Verts);

	// ������ ������ ���Ͽ�
	for (EgVertex *v : Verts)
	{
		bool bBndry = false;
		for (EgEdge *e : v->m_pEdges)
		{
			// ��� �����̶�� ��� ���� ����Ʈ�� �߰��Ѵ�.
			if (FACE(e)->m_Idx == -1)
			{
				BndryVerts.push_back(v);
				bBndry = true;
				break;
			}
		}
		// ��� ������ �ƴ϶�� ���� ���� ����Ʈ�� �߰��Ѵ�.
		if (!bBndry)
			IntrVerts.push_back(v);
	}

	// ���� �ﰢ���� �ε����� �����Ѵ�.
	int idx = 0;
	for (EgFace *f : Faces)
		f->m_Idx = FaceIdxs[idx++];

	// ���� �ﰢ���� ���� ���� ��ȯ�Ѵ�.
	return (int)Verts.size();
}

/*!
*	\brief �޽��� ������ ����-��� ������ ��迭 �Ѵ�.
*
*	\param pMesh[IN/OUT] ��� �޽�
*
*	\return ����� ���� ��ȯ�Ѵ�.
*/
int rearrange_verts(EgMesh *pMesh)
{
	// ���� ������ ��� ������ ����Ʈ�� ���Ѵ�.
	std::vector<EgVertex *> IntrVerts;
	std::vector<std::vector<EgVertex *>> BndryList;
	::get_intr_verts(pMesh, IntrVerts);
	int NumBndry = ::get_bndry_verts(pMesh, BndryList, false, false);

	// Non-manifold �޽���� �����Ѵ�.
	if (NumBndry == -1)
	{
		printf("\t[Error]: Non-manifold mesh...\n");
		return NumBndry;
	}

	// ��� ��� ������ �������� ����Ʈ�� ���� �߰��Ѵ�.
	for (int i = 0; i < NumBndry; ++i)
		for (EgVertex *v : BndryList[i])
			IntrVerts.push_back(v);

	// �޽� ������ ����Ʈ�� �����ϰ�,
	pMesh->m_pVerts.swap(IntrVerts);
	pMesh->UpdateVertIdx();

	// ����� ���� ��ȯ�Ѵ�.
	return NumBndry;
}
