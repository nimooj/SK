#pragma once

#include "../EgMath/egmath.h"

class EgVertex;
class EgFace;
class EgMesh;

/*!	\brief 메쉬 정점의 N-링 이웃 정점을 구하는 함수 */
int get_neighbor_verts(EgMesh *pMesh, EgVertex *pVert, int N, std::vector<std::vector<EgVertex *>> &RingVerts);

/*!	\brief 메쉬 정점의 N-링 이웃 삼각형을 구하는 함수 */
int get_neighbor_faces(EgMesh *pMesh, EgVertex *pVert, int N, std::vector<EgFace *> &Faces);

/*!	\brief 메쉬의 경계(홀) 정점 리스트를 찾는 함수 */
int get_bndry_verts(EgMesh *pMesh, std::vector<std::vector<EgVertex *>> &BndryVerts, bool bClockWise, bool bSortByLen);

/*!	\brief 메쉬의 경계(홀) 정점 리스트를 찾는 함수 */
int get_bndry_verts(EgMesh *pMesh, std::vector<EgVertex *> &BndryVerts);

/*!	\brief 경계(홀) 정점에 연결된 삼각형 리스트를 찾는 함수 */
int get_bndry_faces(std::vector<EgVertex *> &BndryVerts, std::vector<EgFace *> &BndryFaces);

/*!	\brief 메쉬의 경계 정점에 연결된 삼각형 리스트를 찾는 함수 */
int get_bndry_faces(EgMesh *pMesh, std::vector<EgFace *> &BndryFaces);

/*!	\brief 메쉬의 내부 정점 리스트를 찾는 함수 */
int get_intr_verts(EgMesh *pMesh, std::vector<EgVertex *> &IntrVerts);

/*!	\brief 선택 삼각형의 정점을 찾는 함수 */
int get_verts(std::vector<EgFace *> &Faces, std::vector<EgVertex *> &Verts);

/*!	\brief 선택 삼각형의 내부 정점과 외부 정점을 찾는 함수 */
int get_verts(std::vector<EgFace *> &Faces, std::vector<EgVertex *> &IntrVerts, std::vector<EgVertex *> &BndryVerts);

/*!	\brief 메쉬의 정점을 내부-경계 순으로 재배열 하는 함수 */
int rearrange_verts(EgMesh *pMesh);

