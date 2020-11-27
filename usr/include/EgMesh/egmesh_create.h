#pragma once
// 13개의 API 제공

#include "../EgMath/egmath.h"

class EgVertex;
class EgTexel;
class EgNormal;
class EgFace;
class EgTexture;
class EgMaterial;
class EgMesh;

/*! \brief 메쉬 객체를 생성한다. */
EgMesh *create_mesh(std::string MeshName);

/*! \brief 메쉬 정점을 생성한다. */
EgVertex *create_vertex(double x, double y, double z);

/*! \brief 텍스처 좌표를 생성한다. */
EgTexel *create_texel(double s, double t);

/*! \brief 2D 텍스처를 생성한다. */
EgNormal *create_normal(double nx, double ny, double nz);

/*! \brief 메쉬 삼각형을 생성한다. */
EgFace *create_face(EgVertex *v0, EgVertex *v1, EgVertex *v2, EgMaterial *pMtl = NULL, std::string GroupName = "");

/*! \brief 메쉬 삼각형을 생성한다. */
EgFace *create_face(EgVertex *v0, EgVertex *v1, EgVertex *v2, EgTexel *t0, EgTexel *t1, EgTexel *t2, EgMaterial *pMtl, std::string GroupName = "");

/*! \brief 메쉬 삼각형을 생성한다. */
EgFace *create_face(EgVertex *v0, EgVertex *v1, EgVertex *v2, EgTexel *t0, EgTexel *t1, EgTexel *t2, EgNormal *n0, EgNormal *n1, EgNormal *n2, EgMaterial *pMtl, std::string GroupName = "");

/*! \brief 삼각형의 재질을 생성한다. */
EgMaterial *create_material(std::string mtl_name, float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, float ns);

/*!	\brief 임의의 재질을 생성하여 반환하는 함수 */
EgMaterial *create_material_random();

/*! \brief 2D 텍스처를 생성한다. */
EgTexture *create_texture(std::string file_name);

/*! \brief 경계 정점의 내부를 채우는 초기 메쉬를 생성한다. */
EgMesh *create_mesh_from_bndry_verts(std::vector<EgVertex *> &Verts);

/*! \brief 다수의 메쉬를 병합한 메쉬를 생성한다. */
EgMesh *create_mesh_by_merging(std::vector<EgMesh *> &MeshList);

/*! \brief 정점에서 주어진 반경이내의 삼각형으로 메쉬를 생성한다. */
EgMesh *create_mesh_from_geodesic_dist(EgMesh *pMesh, EgVertex *S, double R);