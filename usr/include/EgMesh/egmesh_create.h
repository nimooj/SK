#pragma once
// 13���� API ����

#include "../EgMath/egmath.h"

class EgVertex;
class EgTexel;
class EgNormal;
class EgFace;
class EgTexture;
class EgMaterial;
class EgMesh;

/*! \brief �޽� ��ü�� �����Ѵ�. */
EgMesh *create_mesh(std::string MeshName);

/*! \brief �޽� ������ �����Ѵ�. */
EgVertex *create_vertex(double x, double y, double z);

/*! \brief �ؽ�ó ��ǥ�� �����Ѵ�. */
EgTexel *create_texel(double s, double t);

/*! \brief 2D �ؽ�ó�� �����Ѵ�. */
EgNormal *create_normal(double nx, double ny, double nz);

/*! \brief �޽� �ﰢ���� �����Ѵ�. */
EgFace *create_face(EgVertex *v0, EgVertex *v1, EgVertex *v2, EgMaterial *pMtl = NULL, std::string GroupName = "");

/*! \brief �޽� �ﰢ���� �����Ѵ�. */
EgFace *create_face(EgVertex *v0, EgVertex *v1, EgVertex *v2, EgTexel *t0, EgTexel *t1, EgTexel *t2, EgMaterial *pMtl, std::string GroupName = "");

/*! \brief �޽� �ﰢ���� �����Ѵ�. */
EgFace *create_face(EgVertex *v0, EgVertex *v1, EgVertex *v2, EgTexel *t0, EgTexel *t1, EgTexel *t2, EgNormal *n0, EgNormal *n1, EgNormal *n2, EgMaterial *pMtl, std::string GroupName = "");

/*! \brief �ﰢ���� ������ �����Ѵ�. */
EgMaterial *create_material(std::string mtl_name, float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, float ns);

/*!	\brief ������ ������ �����Ͽ� ��ȯ�ϴ� �Լ� */
EgMaterial *create_material_random();

/*! \brief 2D �ؽ�ó�� �����Ѵ�. */
EgTexture *create_texture(std::string file_name);

/*! \brief ��� ������ ���θ� ä��� �ʱ� �޽��� �����Ѵ�. */
EgMesh *create_mesh_from_bndry_verts(std::vector<EgVertex *> &Verts);

/*! \brief �ټ��� �޽��� ������ �޽��� �����Ѵ�. */
EgMesh *create_mesh_by_merging(std::vector<EgMesh *> &MeshList);

/*! \brief �������� �־��� �ݰ��̳��� �ﰢ������ �޽��� �����Ѵ�. */
EgMesh *create_mesh_from_geodesic_dist(EgMesh *pMesh, EgVertex *S, double R);