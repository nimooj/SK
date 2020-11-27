#pragma once
// 10개의 API 제공

#include <queue>
#include "..\EgMath\egmath.h"

class EgObject;
class EgVertex;
class EgTexel;
class EgFace;
class EgEdge;
class EgMesh;
class EgFacePt;

/*!
*	\class	EgGeodesic
*	\brief	정점의 측지 거리 정보를 표현하는 클래스
*	\note	마지막 수정일: 2020-07-31
*
*	\author 윤승현
*/
class EgGeodesic
{
public:
	/*! \brief 정점에 대한 포인터 */
	EgVertex *m_pVert;

	/*! \brief 소스 정점까지의 최단 거리 */
	double m_Distance;

public:
	// 생성자 및 소멸자
	EgGeodesic(EgVertex *pVert, double Dist);
	EgGeodesic(const EgGeodesic &cpy);
	~EgGeodesic();

	// 대입 연산자
	EgGeodesic &operator =(const EgGeodesic &rhs);

	// 프렌드 함수
	friend bool operator >(const EgGeodesic &lhs, const EgGeodesic &rhs);
};

/*! \brief 소스 정점에서 각 정점까지 측지 거리를 저장하는 최소힙 자료 구조 */
typedef std::priority_queue<EgGeodesic, std::vector<EgGeodesic>, std::greater<EgGeodesic>> GeodesicHeap;

/*! \brief 소스 정점에서 경계 거리 이내의 정점까지의 측지 거리를 계산하는 함수 */
double get_geodesic_dist_field(EgMesh *pMesh, EgVertex *S, std::vector<double> &Distances, double BndryDistance);

/*! \brief 소스 정점에서 타겟 정점까지의 거리 내의 정점들의 측지 거리를 계산하는 함수 */
double get_geodesic_dist_field(EgMesh *pMesh, EgVertex *S, EgVertex *T, std::vector<double> &Distances);

/*! \brief 삼각형 내부점에서 경계 거리 이내의 각 정점까지의 최소 거리를 계산하는 함수 */
double get_geodesic_dist_field(EgMesh *pMesh, EgFacePt *S, std::vector<double> &Distances, double BndryDistance);

/*! \brief 삼각형 내부점에서 타겟 정점까지의 거리 내의 정점들의 측지 거리장을 계산하는 함수 */
double get_geodesic_dist_field(EgMesh *pMesh, EgFacePt *S, EgFacePt *T, std::vector<double> &Distances);

/*! \brief 선택된 정점/삼각형들에서 메쉬의 각 정점까지의 측지거리를 계산하는 함수 */
double get_geodesic_dist_field(EgMesh *pMesh, std::vector<EgObject *> &SrcList, std::vector<double> &Distances, double BndryDistance);

/*! \brief 소스 정점에서 경계 거리 이내의 정점까지의 에지를 따른 최소 거리를 계산하는 함수 */
double get_edge_dist_field(EgMesh *pMesh, EgVertex *S, std::vector<double> &Distances, double BndryDistance);

/*! \brief 소스 정점에서 경계 거리 이내의 정점까지의 위상 거리를 계산하는 함수 */
int get_topology_dist_field(EgMesh *pMesh, EgVertex *S, std::vector<int> &Distances, int BndryDistance);

/*! \brief 메쉬 위에서 두 정점 사이의 최단 경로를 구하는 함수 */
double get_shortest_path(EgVertex *S, EgVertex *E, std::vector<double> &Distances, std::vector<EgPos> &Path, std::vector<std::pair<EgEdge *, float>> &CutInfo);

/*! \brief 메쉬 위에서 임의의 두 위치 사이의 최단 경로를 구하는 함수 */
double get_shortest_path(EgFacePt *S, EgFacePt *E, std::vector<double> &Distances, std::vector<EgPos> &Path, std::vector<std::pair<EgEdge *, float>> &CutInfo);

/*!	\brief 평면 절단을 통해 임의의 두 위치를 연결하는 절단 경로와 거리를 계산하는 함수 */
double get_planar_path(EgFacePt *S, EgFacePt *E, EgPlane *Pln, std::vector<EgPos> &Path, std::vector<std::pair<EgEdge *, float>> &CutInfo);

/*!	\brief 평면 절단을 통해 둘레 경로와 거리를 계산하는 함수 */
double get_planar_path_closed(EgFacePt *P, EgPlane *Pln, std::vector<EgPos> &Path, std::vector<std::pair<EgEdge *, float>> &CutInfo);

/*!	\brief	평면과 에지의 교차 정보를 구하는 함수 */
bool intersect_plane_edge(EgPlane *Pln, EgEdge *e, float *t = NULL);
