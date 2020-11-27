#pragma once
// 9�� API ����

#include "../EgMath/egmath.h"
#include <queue>
#include <functional>

class EgObject;
class EgVertex;
class EgTexel;
class EgNormal;
class EgEdge;
class EgFace;
class EgFacePt;
class EgMesh;
class EgMaterial;
class EgBox;

/*! \brief ������ ���ڿ��� ��ȯ�Ͽ� ��ȯ�Ѵ�. */
std::string cast_str(int i);

/*! \brief ������ �Ǽ��� ���ڿ��� ��ȯ�Ͽ� ��ȯ�Ѵ�. */
std::string cast_str(double v);

/*! \brief ������ �Ǽ��� ���ڿ��� ��ȯ�Ͽ� ��ȯ�Ѵ�. */
std::string cast_str(float v);

/*!	\brief 1 �������� ���ε� ���� ���� ����ϴ� �Լ� */
EgVec3 get_color_map(double val, double min, double max, EgVec3 Cmin, EgVec3 Cmax);

/*!	\brief 2 �������� ���ε� ���� ���� ����ϴ� �Լ� */
EgVec3 get_color_map(double val, double min, double max, EgVec3 Cmin, EgVec3 Cmid, EgVec3 Cmax);

/*!	\brief �������� ���ε� ���� ���� ����ϴ� �Լ� */
EgVec3 get_color_map_from_rainbow(double t);

/*!	\brief ������ ������ ��ȯ�ϴ� �Լ� */
EgVec3 get_color_random();

/*!	\brief �ε����� ���� ������ ������ ��ȯ�ϴ� �Լ� */
EgVec3 get_color_sequence(int idx);

/*!	\brief ���(Ȧ)�� ���̸� ����ϴ� �Լ� */
double get_hole_perimeter(std::vector<EgVertex *> &BndryVerts, bool bClosed);

/*!	\brief �޽� ������ �Լ����� �̿��Ͽ� iso-line ������ �� ������ ���ϴ� �Լ� */
void get_iso_line_pts(EgMesh *pMesh, std::vector<double> &Values, double MinValue, double MaxValue, int NumValue, std::vector<std::vector<EgPos>> &IsoLines);

/*!	\brief �޽����� �ߺ��� ������ �ϳ��� ��ģ��. */
int delete_duplicate_verts(EgMesh *pMesh);