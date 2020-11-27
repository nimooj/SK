#pragma once

#include "../EgMath/egmath.h"

class EgVertex;
class EgFace;
class EgMesh;

/*!	\brief �޽� ������ N-�� �̿� ������ ���ϴ� �Լ� */
int get_neighbor_verts(EgMesh *pMesh, EgVertex *pVert, int N, std::vector<std::vector<EgVertex *>> &RingVerts);

/*!	\brief �޽� ������ N-�� �̿� �ﰢ���� ���ϴ� �Լ� */
int get_neighbor_faces(EgMesh *pMesh, EgVertex *pVert, int N, std::vector<EgFace *> &Faces);

/*!	\brief �޽��� ���(Ȧ) ���� ����Ʈ�� ã�� �Լ� */
int get_bndry_verts(EgMesh *pMesh, std::vector<std::vector<EgVertex *>> &BndryVerts, bool bClockWise, bool bSortByLen);

/*!	\brief �޽��� ���(Ȧ) ���� ����Ʈ�� ã�� �Լ� */
int get_bndry_verts(EgMesh *pMesh, std::vector<EgVertex *> &BndryVerts);

/*!	\brief ���(Ȧ) ������ ����� �ﰢ�� ����Ʈ�� ã�� �Լ� */
int get_bndry_faces(std::vector<EgVertex *> &BndryVerts, std::vector<EgFace *> &BndryFaces);

/*!	\brief �޽��� ��� ������ ����� �ﰢ�� ����Ʈ�� ã�� �Լ� */
int get_bndry_faces(EgMesh *pMesh, std::vector<EgFace *> &BndryFaces);

/*!	\brief �޽��� ���� ���� ����Ʈ�� ã�� �Լ� */
int get_intr_verts(EgMesh *pMesh, std::vector<EgVertex *> &IntrVerts);

/*!	\brief ���� �ﰢ���� ������ ã�� �Լ� */
int get_verts(std::vector<EgFace *> &Faces, std::vector<EgVertex *> &Verts);

/*!	\brief ���� �ﰢ���� ���� ������ �ܺ� ������ ã�� �Լ� */
int get_verts(std::vector<EgFace *> &Faces, std::vector<EgVertex *> &IntrVerts, std::vector<EgVertex *> &BndryVerts);

/*!	\brief �޽��� ������ ����-��� ������ ��迭 �ϴ� �Լ� */
int rearrange_verts(EgMesh *pMesh);

