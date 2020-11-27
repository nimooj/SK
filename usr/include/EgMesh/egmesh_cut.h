#pragma once

/*!	\brief 평면과 메쉬의 교차점(정보)를 구하는 함수 */
void intersect_plane_mesh(EgMesh *pMesh, EgPlane *Pln, std::vector<std::vector<EgPos>> &CutPtList, std::vector<std::vector<std::pair<EgEdge *, float>>> &CutInfoList);



