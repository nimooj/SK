#pragma once

/*!	\brief ���� �޽��� ������(����)�� ���ϴ� �Լ� */
void intersect_plane_mesh(EgMesh *pMesh, EgPlane *Pln, std::vector<std::vector<EgPos>> &CutPtList, std::vector<std::vector<std::pair<EgEdge *, float>>> &CutInfoList);



