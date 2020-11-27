#include "..\usr\include\EgMesh\egmesh.h"

/*!
*	\brief	다음 평면 절단 정보을 찾는다.
*	\note	마지막 수정일: 2020-06-24
*
*	\param	pStFace[in]		시작 삼각형에 대한 포인터
*	\param	e[in]			현재 교차점이 놓인 에지 대한 포인터
*	\param	t[in]			에지 상에서 현재 교차점의 위치 파라미터(0.0 < t <= 1.0)
*	\param	Pln[in]			경로가 놓일 평면에 대한 포인터
*	\param	CutInfo[out]	절단 평면을 아래서 위로 통과하는 에지의 절단 정보가 저장된다.
*
*	\return	결과 값(-1: 미확인 에러, 0: 홀에 의해 경로가 끊김, 1: 시작점으로 되돌아옴)을 반환한다.
*/
static int find_next_planar_cut(EgFace *pStFace, EgEdge *e, float t, EgPlane *Pln, std::vector<std::pair<EgEdge *, float>> &CutInfo)
{
	// 종료조건: 경로가 끊겼다면 0을 반환한다.
	if (e == NULL)
		return 0;

	// 종료조건: 시작 삼각형으로 돌아왔다면 1을 반환한다.
	if (e->m_pFace == pStFace)
		return 1;

	// 현재 교차점이 에지의 끝점이라면
	if (EQ(t, 1.0f, 1.0e-6))
	{
		// 종료조건: 끝점을 포함한 삼각형에 도달했다면 마지막 경로를 추가하고 1을 반환한다.
		EgVertex *vk = EV(e);
		for (EgEdge *ei : vk->m_pEdges)
		{
			// 종료조건: 시작점으로 돌아왔다면 1을 반환한다.
			if (ei->m_pFace == pStFace)
				return 1;
		}

		// 1-링 경계 에지를 검사하여 절단 경로가 통과할 다음 에지를 찾는다.
		for (EgEdge *ei : vk->m_pEdges)
		{
			// 검사 에지가 경계 에지인 경우 생략한다.
			if (ei->m_pNext->IsBndry())
				continue;

			// 평면 경로가 통과할 다음 에지를 찾는다.
			if (intersect_plane_edge(Pln, ei->m_pNext, &t))
			{
				ei->m_Cost = 0.0;
				CutInfo.push_back(std::make_pair(ei->m_pNext, t));
				return find_next_planar_cut(pStFace, ei->m_pNext->m_pMate, 1.0f - t, Pln, CutInfo);
			}
		}

		// 경로가 끊겼다면 0을 반환한다.
		return 0;
	}
	else
	{
		// 다른 두 에지를 조사하여 절단 경로를 추가한다.
		for (int i = 0; i < 2; ++i)
		{
			e = e->m_pNext;
			if (intersect_plane_edge(Pln, e, &t))
			{
				e->m_Cost = 0.0;
				CutInfo.push_back(std::make_pair(e, t));
				return find_next_planar_cut(pStFace, e->m_pMate, 1.0f - t, Pln, CutInfo);
			}
		}
	}

	// 미확인 에러인 경우 -1를 반환한다.
	return -1;
}

/*!
*	\brief	평면과 메쉬의 교차점(정보)을 구하는 함수
*	\note	마지막 수정일: 2020-06-28
*
*	\param	pMesh[in]			메쉬에 대한 포인터
*	\param	Pln[in]				평면에 대한 포인터
*	\param	CutPtList[out]		폴리라인 단위로 교차점의 위치가 저장된다.
*	\param	CutInfoList[out]	폴리라인 단위로 교차 정보가 저장된다.
*/
void intersect_plane_mesh(
	EgMesh *pMesh, EgPlane *Pln,
	std::vector<std::vector<EgPos>> &CutPtList,
	std::vector<std::vector<std::pair<EgEdge *, float>>> &CutInfoList)
{
	// 결과를 초기화 한다.
	CutPtList.clear();
	CutInfoList.clear();

	// 평면과 교차하는 모든 에지를 구한다.
	std::vector<EgEdge *> CutEdges;
	for (EgFace *f : pMesh->m_pFaces)
	{
		for (int i = 0; i < 3; ++i)
		{
			EgEdge *e = f->GetEdge(i);
			if (intersect_plane_edge(Pln, e))
			{
				e->m_Cost = 1.0;
				CutEdges.push_back(e);
			}
		}
	}

	// 열린 폴리라인의 시작 에지를 구한다.
	std::vector<EgEdge *> SeedEdges;
	for (EgEdge *e : CutEdges)
	{
		EgEdge *e0 = e->m_pNext;
		EgEdge *e1 = e->m_pNext->m_pNext;
		if (e0->m_pMate != NULL && e0->m_pMate->m_Cost == 1.0)
			continue;
		if (e1->m_pMate != NULL && e1->m_pMate->m_Cost == 1.0)
			continue;
		SeedEdges.push_back(e);
	}

	// 열린 폴리라인들이 있다면
	if (!SeedEdges.empty())
	{
		// 각 시작 에지에 대하여
		for (EgEdge *e : SeedEdges)
		{
			// 시작 교차 정보를 추가하고
			float t;
			e->m_Cost = 0.0;
			std::vector <std::pair<EgEdge *, float>> SubCutInfo;
			intersect_plane_edge(Pln, e, &t);
			SubCutInfo.push_back(std::make_pair(e, t));

			// 열린 폴리라인을 구한다.
			int result = find_next_planar_cut(e->m_pFace, e->m_pMate, 1.0f - t, Pln, SubCutInfo);
			if (result != -1)
			{
				// 폴리라인의 교차 정보를 추가하고
				CutInfoList.push_back(SubCutInfo);

				// 폴리라인 별로 교차점을 구성한다.
				std::vector<EgPos> CutPts;
				for (std::pair<EgEdge *, float> &info : SubCutInfo)
					CutPts.push_back(::lerp(POS(SV(info.first)), POS(EV(info.first)), info.second));
				CutPtList.push_back(CutPts);
			}
		}
	}

	// 닫힌 폴리라인들을 구한다.
	for (EgEdge *e : CutEdges)
	{
		// 열린 폴리라인을 구하는 과정에서 이미 처리된 교차 에지라면 스킵한다.
		if (e->m_Cost == 0.0)
			continue;

		// 시작 교차 정보를 추가하고
		float t;
		e->m_Cost = 0.0;
		std::vector <std::pair<EgEdge *, float>> SubCutInfo;
		intersect_plane_edge(Pln, e, &t);
		SubCutInfo.push_back(std::make_pair(e, t));

		// 닫힌 폴리라인을 구한다.
		int result = find_next_planar_cut(e->m_pFace, e->m_pMate, 1.0f - t, Pln, SubCutInfo);
		if (result != -1)
		{
			// 폴리라인의 교차 정보를 추가하고
			CutInfoList.push_back(SubCutInfo);

			// 폴리라인 별로 교차점을 구성한다.
			std::vector<EgPos> CutPts;
			for (std::pair<EgEdge *, float> &info : SubCutInfo)
				CutPts.push_back(::lerp(POS(SV(info.first)), POS(EV(info.first)), info.second));

			// 시작점을 추가하여 닫힌 폴리라인을 생성한다.
			CutPts.push_back(CutPts[0]);
			CutPtList.push_back(CutPts);
		}
	}
}
