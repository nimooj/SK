#pragma once
// 9개 API 제공

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

/*! \brief 정수를 문자열로 변환하여 반환한다. */
std::string cast_str(int i);

/*! \brief 배정도 실수를 문자열로 변환하여 반환한다. */
std::string cast_str(double v);

/*! \brief 단정도 실수를 문자열로 변환하여 반환한다. */
std::string cast_str(float v);

/*!	\brief 1 구간에서 맵핑된 색상 값을 계산하는 함수 */
EgVec3 get_color_map(double val, double min, double max, EgVec3 Cmin, EgVec3 Cmax);

/*!	\brief 2 구간에서 맵핑된 색상 값을 계산하는 함수 */
EgVec3 get_color_map(double val, double min, double max, EgVec3 Cmin, EgVec3 Cmid, EgVec3 Cmax);

/*!	\brief 무지개에 맵핑된 색상 값을 계산하는 함수 */
EgVec3 get_color_map_from_rainbow(double t);

/*!	\brief 임의의 색상을 반환하는 함수 */
EgVec3 get_color_random();

/*!	\brief 인덱스에 따라 정해진 색상을 반환하는 함수 */
EgVec3 get_color_sequence(int idx);

/*!	\brief 경계(홀)의 길이를 계산하는 함수 */
double get_hole_perimeter(std::vector<EgVertex *> &BndryVerts, bool bClosed);

/*!	\brief 메쉬 정점에 함수값을 이용하여 iso-line 선분의 양 끝점을 구하는 함수 */
void get_iso_line_pts(EgMesh *pMesh, std::vector<double> &Values, double MinValue, double MaxValue, int NumValue, std::vector<std::vector<EgPos>> &IsoLines);

/*!	\brief 메쉬에서 중복된 정점을 하나로 합친다. */
int delete_duplicate_verts(EgMesh *pMesh);