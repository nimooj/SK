#pragma once
// 10���� API ����

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
*	\brief	������ ���� �Ÿ� ������ ǥ���ϴ� Ŭ����
*	\note	������ ������: 2020-07-31
*
*	\author ������
*/
class EgGeodesic
{
public:
	/*! \brief ������ ���� ������ */
	EgVertex *m_pVert;

	/*! \brief �ҽ� ���������� �ִ� �Ÿ� */
	double m_Distance;

public:
	// ������ �� �Ҹ���
	EgGeodesic(EgVertex *pVert, double Dist);
	EgGeodesic(const EgGeodesic &cpy);
	~EgGeodesic();

	// ���� ������
	EgGeodesic &operator =(const EgGeodesic &rhs);

	// ������ �Լ�
	friend bool operator >(const EgGeodesic &lhs, const EgGeodesic &rhs);
};

/*! \brief �ҽ� �������� �� �������� ���� �Ÿ��� �����ϴ� �ּ��� �ڷ� ���� */
typedef std::priority_queue<EgGeodesic, std::vector<EgGeodesic>, std::greater<EgGeodesic>> GeodesicHeap;

/*! \brief �ҽ� �������� ��� �Ÿ� �̳��� ���������� ���� �Ÿ��� ����ϴ� �Լ� */
double get_geodesic_dist_field(EgMesh *pMesh, EgVertex *S, std::vector<double> &Distances, double BndryDistance);

/*! \brief �ҽ� �������� Ÿ�� ���������� �Ÿ� ���� �������� ���� �Ÿ��� ����ϴ� �Լ� */
double get_geodesic_dist_field(EgMesh *pMesh, EgVertex *S, EgVertex *T, std::vector<double> &Distances);

/*! \brief �ﰢ�� ���������� ��� �Ÿ� �̳��� �� ���������� �ּ� �Ÿ��� ����ϴ� �Լ� */
double get_geodesic_dist_field(EgMesh *pMesh, EgFacePt *S, std::vector<double> &Distances, double BndryDistance);

/*! \brief �ﰢ�� ���������� Ÿ�� ���������� �Ÿ� ���� �������� ���� �Ÿ����� ����ϴ� �Լ� */
double get_geodesic_dist_field(EgMesh *pMesh, EgFacePt *S, EgFacePt *T, std::vector<double> &Distances);

/*! \brief ���õ� ����/�ﰢ���鿡�� �޽��� �� ���������� �����Ÿ��� ����ϴ� �Լ� */
double get_geodesic_dist_field(EgMesh *pMesh, std::vector<EgObject *> &SrcList, std::vector<double> &Distances, double BndryDistance);

/*! \brief �ҽ� �������� ��� �Ÿ� �̳��� ���������� ������ ���� �ּ� �Ÿ��� ����ϴ� �Լ� */
double get_edge_dist_field(EgMesh *pMesh, EgVertex *S, std::vector<double> &Distances, double BndryDistance);

/*! \brief �ҽ� �������� ��� �Ÿ� �̳��� ���������� ���� �Ÿ��� ����ϴ� �Լ� */
int get_topology_dist_field(EgMesh *pMesh, EgVertex *S, std::vector<int> &Distances, int BndryDistance);

/*! \brief �޽� ������ �� ���� ������ �ִ� ��θ� ���ϴ� �Լ� */
double get_shortest_path(EgVertex *S, EgVertex *E, std::vector<double> &Distances, std::vector<EgPos> &Path, std::vector<std::pair<EgEdge *, float>> &CutInfo);

/*! \brief �޽� ������ ������ �� ��ġ ������ �ִ� ��θ� ���ϴ� �Լ� */
double get_shortest_path(EgFacePt *S, EgFacePt *E, std::vector<double> &Distances, std::vector<EgPos> &Path, std::vector<std::pair<EgEdge *, float>> &CutInfo);

/*!	\brief ��� ������ ���� ������ �� ��ġ�� �����ϴ� ���� ��ο� �Ÿ��� ����ϴ� �Լ� */
double get_planar_path(EgFacePt *S, EgFacePt *E, EgPlane *Pln, std::vector<EgPos> &Path, std::vector<std::pair<EgEdge *, float>> &CutInfo);

/*!	\brief ��� ������ ���� �ѷ� ��ο� �Ÿ��� ����ϴ� �Լ� */
double get_planar_path_closed(EgFacePt *P, EgPlane *Pln, std::vector<EgPos> &Path, std::vector<std::pair<EgEdge *, float>> &CutInfo);

/*!	\brief	���� ������ ���� ������ ���ϴ� �Լ� */
bool intersect_plane_edge(EgPlane *Pln, EgEdge *e, float *t = NULL);
