#include "..\usr\include\egmesh\egmesh.h"

/*!
*	\brief	메쉬의 정점의 N-링 이웃 정점을 구하는 함수
*
*	\param	pMesh[in] 대상 메쉬
*	\param	pVert[in] 조사할 정점
*	\param	N[in] 조사할 이웃 정점의 최대 링수
*	\param	RingVerts[out] 결과가 저장됨(i-링 이웃 정점은 RingVerts[i][0], ..., RingVerts[i][m]에 저장됨)
*
*	\return 실제 구해진 최대 링수를 반환한다(N 보다 작을 수도 있음)
*/
int get_neighbor_verts(EgMesh *pMesh, EgVertex *pVert, int N, std::vector<std::vector<EgVertex *>> &RingVerts)
{
	// 수행 시간을 측정한다.
	clock_t st = clock();

	// 각 정점까지의 거리를 무한대로 초기화 한다.
	std::vector<int> Distances;
	Distances.assign(pMesh->GetNumVert(), 10000000);

	// 최소힙을 생성하고 0으로 초기화된 소스 정점의 정보를 추가한다.
	GeodesicHeap MinHeap;
	Distances[pVert->m_Idx] = 0;
	MinHeap.push(EgGeodesic(pVert, 0.0));

	// 최소힙의 정보가 모두 처리될 때까지
	while (!MinHeap.empty())
	{
		// 소스 정점에서 가장 까가운 정점의 정보를 꺼내서
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// 이미 처리된 정보라면 스킵한다.
		int vidx = info.m_pVert->m_Idx;
		if ((int)info.m_Distance > Distances[vidx])
			continue;

		// 정점의 1링 정점 vj의 최단 거리를 갱신한다.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// Dijkstra 알고리즘을 적용한다.
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

			// 새로운 최단 거리를 찾은 경우
			int dj = MIN(d1, d2);
			if (dj < Distances[vj->m_Idx])
			{
				// 정점의 최단 거리를 갱신하고
				Distances[vj->m_Idx] = dj;
				if (dj <= N)
					MinHeap.push(EgGeodesic(vj, (double)dj));
			}
		}
	}

	// 계산 결과를 리스트에 저장한다.
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

	// 실제 조사된 링수를 반환한다.
	for (int i = N; i >= 0; --i)
		if (!RingVerts[i].empty())
			return N;

	return -1;
}

/*!
*   \brief	메쉬 경계의 N-링 이웃 정점을 구하는 함수
*   \note	수행 후, 모든 정점의 m_EditWgt는 0으로 초기화된다.
*
*   \param  pMesh[in]		대상 메쉬
*   \param  BndryVerts[in]	조사할 경계 정점 리스트
*   \param  N[in]			조사할 이웃 정점의 최대 링수 (음수인 경우 전체 메쉬를 조사한다.)
*   \param  RingVerts[out]	결과가 저장됨(i-링 이웃 정점은 RingVerts[i][0], ..., RingVerts[i][m]에 저장됨)
*
*   \return 실제 구해진 최대 링수를 반환한다(N 보다 작을 수도 있음)
*/
int get_neighbor_verts(EgMesh *pMesh, std::vector<EgVertex *> &BndryVerts, int N, std::vector<std::vector<EgVertex *>> &RingVerts)
{
	// 수행 시간을 측정한다.
	clock_t st = clock();

	// 조사할 정점 리스트가 경계가 아니라면 리턴한다.
	for (EgVertex *v : BndryVerts)
	{
		if (!v->IsBndry())
		{
			std::cout << "get_neighbor_verts return -1" << std::endl;
			return -1;
		}
	}

	// 미방문 정점을 표시한다.
	int i = -1;
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_EditWgt = (float)i;

	RingVerts.clear();
	RingVerts.push_back(std::vector<EgVertex *>());
	for (EgVertex *v : BndryVerts)
		RingVerts[0].push_back(v);

	while (true)
	{
		// N-링만큼 조사했다면 루프를 빠져나간다.
		if (++i == N)
			break;

		// i-링 경계를 기록한다.
		for (EgVertex *v : RingVerts[i])
			v->m_EditWgt = (float)i;

		// i-링 경계의 1링 정점을 구하여,
		std::vector<EgVertex *> NextRingVerts;
		for (EgVertex *vi : RingVerts[i])
		{
			std::vector<EgVertex *> Verts;
			vi->GetOneRingVert(Verts);
			for (EgVertex *vj : Verts)
				if (vj->m_EditWgt == -1.0f)
					NextRingVerts.push_back(vj);
		}

		// 더이상 방문할 정점이 없다면 루프를 빠져나간다.
		if (NextRingVerts.empty())
			break;

		UNIQUE(NextRingVerts);

		// (i+1)-링 경계 정점으로 설정한다.
		RingVerts.push_back(std::vector<EgVertex *>());
		for (EgVertex *v : NextRingVerts)
			RingVerts[i + 1].push_back(v);
	}

	// 가중치를 초기화한다.
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_EditWgt = 0.0f;

	//printf("Elapsed time for finding neighbor vertices of boundary= %u ms\n", clock() - st);

	// 실제 조사된 링수를 반환한다.
	return NUM(RingVerts) - 1;
}

/*!
*	\brief	메쉬 정점의 N-링 삼각형을 구하는 함수
*
*	\param	pMesh[in] 대상 메쉬
*	\param	pVert[in] 조사할 정점
*	\param	N[in] 조사할 이웃 정점의 최대 링수
*	\param	Faces[out] 삼각형이 저장됨
*
*	\return 선택된 삼각형의 수를 반환한다.
*/
int get_neighbor_faces(EgMesh *pMesh, EgVertex *pVert, int N, std::vector<EgFace *> &Faces)
{
	// 수행 시간을 측정한다.
	clock_t st = clock();

	// 각 정점까지의 거리를 무한대로 초기화 한다.
	std::vector<int> Distances;
	Distances.assign(pMesh->GetNumVert(), 10000000);

	// 최소힙을 생성하고 0으로 초기화된 소스 정점의 정보를 추가한다.
	GeodesicHeap MinHeap;
	Distances[pVert->m_Idx] = 0;
	MinHeap.push(EgGeodesic(pVert, 0.0));

	// 최소힙의 정보가 모두 처리될 때까지
	while (!MinHeap.empty())
	{
		// 소스 정점에서 가장 까가운 정점의 정보를 꺼내서
		EgGeodesic info = MinHeap.top();
		MinHeap.pop();

		// 이미 처리된 정보라면 스킵한다.
		int vidx = info.m_pVert->m_Idx;
		if ((int)info.m_Distance > Distances[vidx])
			continue;

		// 정점의 1링 정점 vj의 최단 거리를 갱신한다.
		EgVertex *vi = info.m_pVert;
		for (EgEdge *e : vi->m_pEdges)
		{
			// Dijkstra 알고리즘을 적용한다.
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

			// 새로운 최단 거리를 찾은 경우
			int dj = MIN(d1, d2);
			if (dj < Distances[vj->m_Idx])
			{
				// 정점의 최단 거리를 갱신하고
				Distances[vj->m_Idx] = dj;
				if (dj <= N)
					MinHeap.push(EgGeodesic(vj, (double)dj));
			}
		}
	}

	// 계산 결과를 리스트에 저장한다.
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
*	\brief	메쉬 경계(홀) 정점의 리스트를 구한다.
*
*	\param	pMesh[in] 대상 메쉬
*	\param	BndryVerts[out] 경계(홀) 정점이 저장될 리스트
*	\param	bClockWise[in] true인 경우 경계(홀) 정점이 시계(반시계) 방향으로, false인 경우 경계(홀) 정점이 반시계(시계) 방향으로 저장된다.
*	\param	bSortByLen[in] true인 경우 경계(홀)의 길이에 따라 오름차순으로 정렬된다.
*
*	\return 경계(홀)의 수를 반환한다(non-manifold 정점이 있다면 -1을 반환한다).
*/
int get_bndry_verts(EgMesh *pMesh, std::vector<std::vector<EgVertex *>> &BndryVerts, bool bClockWise, bool bSortByLen)
{
	clock_t st = clock();
	// 결과를 저장할 리스트를 초기화한다.
	BndryVerts.clear();

	// 정점의 인덱스를 갱신한다.
	pMesh->UpdateVertIdx();

	// 경계 에지에 대하여 <시작점의 인덱스, 끝점의 인덱스>로 표현된 맵을 구성한다.
	std::map<int, int> BndryVertIdxPairs;
	int NumBndryEdges = 0;
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 각 삼각형의 정점의 인덱스를 구하여
		int vidx0 = f->GetVertex(0)->m_Idx;
		int vidx1 = f->GetVertex(1)->m_Idx;
		int vidx2 = f->GetVertex(2)->m_Idx;

		// 경계 에지라면 맵에 추가한다.
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

	// 비다양체 정점이 포함되어 있다면 -1을 반환한다.
	if (NumBndryEdges != (int)BndryVertIdxPairs.size())
	{
		std::vector<EgVertex *> Temp;
		// 각각의 삼각형에 대하여
		for (EgFace *f : pMesh->m_pFaces)
		{
			// 첫 번째 에지가 경계 에지라면
			if (f->GetEdge(0)->IsBndry())
			{
				// 첫 번째 에지의 시작점과 끝점을 추가한다.
				Temp.push_back(f->GetVertex(0));
				Temp.push_back(f->GetVertex(1));
			}

			// 두 번째 에지가 경계 에지라면
			if (f->GetEdge(1)->IsBndry())
			{
				// 두 번째 에지의 시작점과 끝점을 추가한다.
				Temp.push_back(f->GetVertex(1));
				Temp.push_back(f->GetVertex(2));
			}

			// 세 번째 에지가 경계 에지라면
			if (f->GetEdge(2)->IsBndry())
			{
				// 세 번째 에지의 시작점과 끝점을 추가한다.
				Temp.push_back(f->GetVertex(2));
				Temp.push_back(f->GetVertex(0));
			}
		}

		// 경계 정점 리스트에 추가하고, -1을 반환한다.
		BndryVerts.push_back(Temp);
		return -1;
	}

	// 경계 정점들을 반시계 방향으로 정렬한다.
	while (!BndryVertIdxPairs.empty())
	{
		// 각각의 경계(홀)의 시작 에지를 구하여 
		std::map<int, int>::iterator it = BndryVertIdxPairs.begin();

		// 각 경계(홀)에 대하여 정점의 리스트를 구성한다.
		std::vector<int> tmp;
		tmp.push_back(it->first);
		do
		{
			// 끝점의 인덱스를 추가하고, 
			tmp.push_back(it->second);

			// 추가된 끝점을 시작점으로 갖는 경계 에지를 구한다.
			it = BndryVertIdxPairs.find(tmp.back());

			// 추가된 끝점을 시작점으로 하는 경계 에지가 없거나,
			// 경계의 시작점으로 되돌아 왔다면(루프가 생성) 반복을 마친다
		} while (it != BndryVertIdxPairs.end() && it->second != tmp.front());

		// 찾아진 경계 에지를 제거한다.
		for (int key : tmp)
			BndryVertIdxPairs.erase(key);

		// 각 경계 정점의 인덱스로 경계 정점을 찾아서 
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

		// 경계 정점 리스트에 추가한다.
		BndryVerts.push_back(EachBndryVerts);
	}

	// 경계(홀)을 길이에 따라 정렬한다면
	if (bSortByLen)
	{
		// 각각의 경계(홀)에 대하여 길이를 구한다.
		std::vector<double> BndryLen;
		for (std::vector<EgVertex *> &EachBndry : BndryVerts)
		{
			double len = ::get_hole_perimeter(EachBndry, true);
			BndryLen.push_back(len);
		}

		// 경계(홀)의 길이에 따라 정렬한다.
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

	// 홀의 개수를 반환한다.
	return (int)BndryVerts.size();
}

/*!
*	\brief	메쉬 경계(홀) 정점의 리스트를 구한다.
*	\note	Manifold 또는 non-manifold 구분 없이 경계 정점을 찾는다.
*
*	\param	pMesh[in] 대상 메쉬
*	\param	BndryVerts[out] 경계(홀) 정점이 저장된다.
*
*	\return 경계 정점의 수를 반환한다.
*/
int get_bndry_verts(EgMesh *pMesh, std::vector<EgVertex *> &BndryVerts)
{
	// 결과를 저장할 리스트를 초기화한다.
	BndryVerts.clear();

	// 각 경계 정점의 리스트를 구한다.
	std::vector<std::vector<EgVertex *>> BndryList;

	// Non-manifold 라면
	if (::get_bndry_verts(pMesh, BndryList, false, false) == -1)
	{
		// 각 경계 구분없이 정점을 추가한다.
		BndryVerts = BndryList[0];
	}
	else // Manifold 라면 
	{
		// 각 경계별로 정점을 추가한다.
		for (std::vector<EgVertex *> &Bndry : BndryList)
			for (EgVertex *v : Bndry)
				BndryVerts.push_back(v);
	}

	// 경계 정점의 수를 반환한다.
	return (int)BndryVerts.size();
}

/*!
*	\brief	메쉬 경계(홀) 삼각형 리스트를 구한다.
*
*	\param	BndryVerts[in] 경계(홀) 정점의 리스트
*	\param	BndryFaces[out] 경계(홀) 정점에 연결된 삼각형 리스트가 저장된다.
*
*	\return 삼각형의 수를 반환한다.
*/
int get_bndry_faces(std::vector<EgVertex *> &BndryVerts, std::vector<EgFace *> &BndryFaces)
{
	clock_t st = clock();
	// 결과를 저장할 리스트를 초기화한다.
	BndryFaces.clear();

	// 각각의 경계 정점에 대하여
	for (EgVertex *v : BndryVerts)
	{
		// 정점에서 시작하는 각각의 에지가 포함된 삼각형을 추가한다.
		for (EgEdge *e : v->m_pEdges)
			BndryFaces.push_back(e->m_pFace);
	}

	// 중복으로 추가된 삼각형을 제거한다.
	sort(BndryFaces.begin(), BndryFaces.end());
	BndryFaces.erase(unique(BndryFaces.begin(), BndryFaces.end()), BndryFaces.end());

	// 삼각형의 수를 반환한다.
	return (int)BndryFaces.size();
}

/*!
*	\brief	메쉬의 경계 정점에 연결된 삼각형 리스트를 찾는 함수
*
*	\param	pMesh[in] 대상 메쉬
*	\param	BndryFaces[out] 경계 정점에 연결된 삼각형의 리스트가 저장됨
*
*	\return	삼각형의 수를 반환한다.
*/
int get_bndry_faces(EgMesh *pMesh, std::vector<EgFace *> &BndryFaces)
{
	// 결과 값을 초기화 하고
	BndryFaces.clear();

	// 경계 에지를 포함한 삼각형을 리스트에 추가한다.
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
*	\brief 메쉬 내부 정점의 리스트를 구한다.
*
*	\param pMesh[in] 대상 메쉬
*	\param IntrVerts[out] 내부 정점이 저장된다.
*
*	\return 내부 정점의 수를 반환한다.
*/
int get_intr_verts(EgMesh *pMesh, std::vector<EgVertex *> &IntrVerts)
{
	// 리스트를 초기화 하고,
	IntrVerts.clear();

	// 경계 정점이 아니면 리스트에 추가한다.
	for (EgVertex *v : pMesh->m_pVerts)
		if (v->IsBndry() == false)
			IntrVerts.push_back(v);

	// 내부 정점의 수를 반환한다.
	return (int)IntrVerts.size();
}

/*!
*	\brief 선택 삼각형의 정점의 리스트를 구한다.
*
*	\param Faces[in] 선택된 삼각형의 리스트
*	\param Verts[out] 선택된 삼각형의 정점이 저장된다.
*
*	\return 정점의 수를 반환한다.
*/
int get_verts(std::vector<EgFace *> &Faces, std::vector<EgVertex *> &Verts)
{
	// 선택 삼각형의 정점 집합을 구한다.
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
*	\brief 선택 삼각형의 내부 정점과 외부 정점의 리스트를 구한다.
*
*	\param Faces[in] 선택된 삼각형의 리스트
*	\param IntrVerts[out] 선택된 삼각형의 내부 정점이 저장된다.
*	\param BndryVerts[out] 선택된 삼각형의 외부 정점이 저장된다.
*
*	\return 정점의 수를 반환한다.
*/
int get_verts(std::vector<EgFace *> &Faces, std::vector<EgVertex *> &IntrVerts, std::vector<EgVertex *> &BndryVerts)
{
	// 결과를 초기화 하고
	IntrVerts.clear();
	BndryVerts.clear();

	// 선택 삼각형을 기록하고 인덱스를 저장한다.
	std::vector<int> FaceIdxs;
	for (EgFace *f : Faces)
	{
		FaceIdxs.push_back(f->m_Idx);
		f->m_Idx = -1;
	}

	// 선택 삼각형의 정점 집합을 구한다.
	std::vector<EgVertex *> Verts;
	::get_verts(Faces, Verts);

	// 각각의 정점에 대하여
	for (EgVertex *v : Verts)
	{
		bool bBndry = false;
		for (EgEdge *e : v->m_pEdges)
		{
			// 경계 정점이라면 경계 정점 리스트에 추가한다.
			if (FACE(e)->m_Idx == -1)
			{
				BndryVerts.push_back(v);
				bBndry = true;
				break;
			}
		}
		// 경계 정점이 아니라면 내부 정점 리스트에 추가한다.
		if (!bBndry)
			IntrVerts.push_back(v);
	}

	// 선택 삼각형의 인덱스를 복원한다.
	int idx = 0;
	for (EgFace *f : Faces)
		f->m_Idx = FaceIdxs[idx++];

	// 선택 삼각형의 정점 수를 반환한다.
	return (int)Verts.size();
}

/*!
*	\brief 메쉬의 정점을 내부-경계 순으로 재배열 한다.
*
*	\param pMesh[IN/OUT] 대상 메쉬
*
*	\return 경계의 수를 반환한다.
*/
int rearrange_verts(EgMesh *pMesh)
{
	// 내부 정점과 경계 정점의 리스트를 구한다.
	std::vector<EgVertex *> IntrVerts;
	std::vector<std::vector<EgVertex *>> BndryList;
	::get_intr_verts(pMesh, IntrVerts);
	int NumBndry = ::get_bndry_verts(pMesh, BndryList, false, false);

	// Non-manifold 메쉬라면 리턴한다.
	if (NumBndry == -1)
	{
		printf("\t[Error]: Non-manifold mesh...\n");
		return NumBndry;
	}

	// 모든 경계 정점을 내부정점 리스트의 끝에 추가한다.
	for (int i = 0; i < NumBndry; ++i)
		for (EgVertex *v : BndryList[i])
			IntrVerts.push_back(v);

	// 메쉬 정점의 리스트를 갱신하고,
	pMesh->m_pVerts.swap(IntrVerts);
	pMesh->UpdateVertIdx();

	// 경계의 수를 반환한다.
	return NumBndry;
}
