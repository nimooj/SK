#include "..\usr\include\EgMesh\egmesh.h"

/*!
*	\brief	���� ��� ���� ������ ã�´�.
*	\note	������ ������: 2020-06-24
*
*	\param	pStFace[in]		���� �ﰢ���� ���� ������
*	\param	e[in]			���� �������� ���� ���� ���� ������
*	\param	t[in]			���� �󿡼� ���� �������� ��ġ �Ķ����(0.0 < t <= 1.0)
*	\param	Pln[in]			��ΰ� ���� ��鿡 ���� ������
*	\param	CutInfo[out]	���� ����� �Ʒ��� ���� ����ϴ� ������ ���� ������ ����ȴ�.
*
*	\return	��� ��(-1: ��Ȯ�� ����, 0: Ȧ�� ���� ��ΰ� ����, 1: ���������� �ǵ��ƿ�)�� ��ȯ�Ѵ�.
*/
static int find_next_planar_cut(EgFace *pStFace, EgEdge *e, float t, EgPlane *Pln, std::vector<std::pair<EgEdge *, float>> &CutInfo)
{
	// ��������: ��ΰ� ����ٸ� 0�� ��ȯ�Ѵ�.
	if (e == NULL)
		return 0;

	// ��������: ���� �ﰢ������ ���ƿԴٸ� 1�� ��ȯ�Ѵ�.
	if (e->m_pFace == pStFace)
		return 1;

	// ���� �������� ������ �����̶��
	if (EQ(t, 1.0f, 1.0e-6))
	{
		// ��������: ������ ������ �ﰢ���� �����ߴٸ� ������ ��θ� �߰��ϰ� 1�� ��ȯ�Ѵ�.
		EgVertex *vk = EV(e);
		for (EgEdge *ei : vk->m_pEdges)
		{
			// ��������: ���������� ���ƿԴٸ� 1�� ��ȯ�Ѵ�.
			if (ei->m_pFace == pStFace)
				return 1;
		}

		// 1-�� ��� ������ �˻��Ͽ� ���� ��ΰ� ����� ���� ������ ã�´�.
		for (EgEdge *ei : vk->m_pEdges)
		{
			// �˻� ������ ��� ������ ��� �����Ѵ�.
			if (ei->m_pNext->IsBndry())
				continue;

			// ��� ��ΰ� ����� ���� ������ ã�´�.
			if (intersect_plane_edge(Pln, ei->m_pNext, &t))
			{
				ei->m_Cost = 0.0;
				CutInfo.push_back(std::make_pair(ei->m_pNext, t));
				return find_next_planar_cut(pStFace, ei->m_pNext->m_pMate, 1.0f - t, Pln, CutInfo);
			}
		}

		// ��ΰ� ����ٸ� 0�� ��ȯ�Ѵ�.
		return 0;
	}
	else
	{
		// �ٸ� �� ������ �����Ͽ� ���� ��θ� �߰��Ѵ�.
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

	// ��Ȯ�� ������ ��� -1�� ��ȯ�Ѵ�.
	return -1;
}

/*!
*	\brief	���� �޽��� ������(����)�� ���ϴ� �Լ�
*	\note	������ ������: 2020-06-28
*
*	\param	pMesh[in]			�޽��� ���� ������
*	\param	Pln[in]				��鿡 ���� ������
*	\param	CutPtList[out]		�������� ������ �������� ��ġ�� ����ȴ�.
*	\param	CutInfoList[out]	�������� ������ ���� ������ ����ȴ�.
*/
void intersect_plane_mesh(
	EgMesh *pMesh, EgPlane *Pln,
	std::vector<std::vector<EgPos>> &CutPtList,
	std::vector<std::vector<std::pair<EgEdge *, float>>> &CutInfoList)
{
	// ����� �ʱ�ȭ �Ѵ�.
	CutPtList.clear();
	CutInfoList.clear();

	// ���� �����ϴ� ��� ������ ���Ѵ�.
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

	// ���� ���������� ���� ������ ���Ѵ�.
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

	// ���� �������ε��� �ִٸ�
	if (!SeedEdges.empty())
	{
		// �� ���� ������ ���Ͽ�
		for (EgEdge *e : SeedEdges)
		{
			// ���� ���� ������ �߰��ϰ�
			float t;
			e->m_Cost = 0.0;
			std::vector <std::pair<EgEdge *, float>> SubCutInfo;
			intersect_plane_edge(Pln, e, &t);
			SubCutInfo.push_back(std::make_pair(e, t));

			// ���� ���������� ���Ѵ�.
			int result = find_next_planar_cut(e->m_pFace, e->m_pMate, 1.0f - t, Pln, SubCutInfo);
			if (result != -1)
			{
				// ���������� ���� ������ �߰��ϰ�
				CutInfoList.push_back(SubCutInfo);

				// �������� ���� �������� �����Ѵ�.
				std::vector<EgPos> CutPts;
				for (std::pair<EgEdge *, float> &info : SubCutInfo)
					CutPts.push_back(::lerp(POS(SV(info.first)), POS(EV(info.first)), info.second));
				CutPtList.push_back(CutPts);
			}
		}
	}

	// ���� �������ε��� ���Ѵ�.
	for (EgEdge *e : CutEdges)
	{
		// ���� ���������� ���ϴ� �������� �̹� ó���� ���� ������� ��ŵ�Ѵ�.
		if (e->m_Cost == 0.0)
			continue;

		// ���� ���� ������ �߰��ϰ�
		float t;
		e->m_Cost = 0.0;
		std::vector <std::pair<EgEdge *, float>> SubCutInfo;
		intersect_plane_edge(Pln, e, &t);
		SubCutInfo.push_back(std::make_pair(e, t));

		// ���� ���������� ���Ѵ�.
		int result = find_next_planar_cut(e->m_pFace, e->m_pMate, 1.0f - t, Pln, SubCutInfo);
		if (result != -1)
		{
			// ���������� ���� ������ �߰��ϰ�
			CutInfoList.push_back(SubCutInfo);

			// �������� ���� �������� �����Ѵ�.
			std::vector<EgPos> CutPts;
			for (std::pair<EgEdge *, float> &info : SubCutInfo)
				CutPts.push_back(::lerp(POS(SV(info.first)), POS(EV(info.first)), info.second));

			// �������� �߰��Ͽ� ���� ���������� �����Ѵ�.
			CutPts.push_back(CutPts[0]);
			CutPtList.push_back(CutPts);
		}
	}
}
