#include <algorithm>
#include <direct.h>
#include "..\usr\include\EgMesh\egmesh.h"

/*!
*	\brief 객체의 클래스 이름을 반환한다.
*
*	\return 객체의 클래스 이름을 반환한다.
*/
std::string EgObject::GetClassName() 
{ 
	return std::string("EgObject");
}

/*!
*	\brief 객체가 포함된 메쉬의 포인터를 반환한다.
*
*	\return 객체가 포함된 메쉬의 포인터를 반환한다.
*/
EgMesh *EgObject::GetMesh()
{
	return NULL;
}

/*!
*	\brief 월드 좌표계에서 표현된 객체의 변환 정보를 반환한다.
*
*	\return 월드 좌표계에서 표현된 객체의 변환 정보를 반환한다.
*/
EgTrans EgObject::GetTrans()
{
	return EgTrans();
}

/*!
*	\brief 선택된 객체를 편집한다.
*
*	\param EC 편집 좌표계
*	\param t 편집될 객체의 이동 변위
*	\param q 편집될 객체의 회전 변위
*	\param s 편집될 객체의 스케일 변위
*	\param bFirst 편집 객체들 중 첫 번째 객체에 대한 편집 여부
*/
void EgObject::Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst)
{
}

/*!
*	\brief 편집할 객체를 렌더링 한다.
*/
void EgObject::RenderForEdit(float r, float g, float b)
{
}

/*!
*	\brief 객체의 클래스 이름을 반환한다.
*
*	\return 객체의 클래스 이름을 반환한다.
*/
std::string EgVertex::GetClassName()
{
	return std::string("EgVertex");
}

/*!
*	\brief 정점이 포함된 메쉬의 포인터를 반환한다.
*
*	\return 정점이 포함된 메쉬의 포인터를 반환한다.
*/
EgMesh *EgVertex::GetMesh()
{
	return m_pMesh;
}

/*!
*	\brief 정점의 모델링 좌표계를 반환한다.
*
*	\return 정점의 모델링 좌표계를 반환한다.
*/
EgTrans EgVertex::GetTrans()
{
	return m_pMesh->m_MC * EgTrans(cast_vec3(m_Pos));
}

/*!
*	\brief 정점이 속한 메쉬를 편집한다.
*
*	\param EC 편집툴의 좌표계
*	\param t 정점의 이동 변위
*	\param q 정점의 회전 변위
*	\param s 정점의 스케일 변위
*	\param bFirst 편집 객체들 중 첫 번째 객체에 대한 편집 여부
*/
void EgVertex::Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst)
{
	// 편집 변환을 위한 정적 변수를 선언하고,
	static EgTrans MC2MC;

	// 편집 객체의 클래스 이름을 구하여,
	std::string ClsName = m_pMesh->GetClassName();

	// EgMesh 객체의 정점일 경우,
	if (ClsName == "EgMesh")
	{
		// 확장 영역에 대한 편집 변환이 아니라면 편집 변환을 계산하고,
		if (bFirst)
			MC2MC = inv(m_pMesh->m_MC) * EC * EgTrans(t, q) * inv(EC) * m_pMesh->m_MC;

		// 편집된 정점의 좌표를 계산하여,
		EgPos pt = MC2MC * m_Pos;

		// 스케일 변환을 수행하고,
		if (s[0] != 1.0) pt[0] *= s[0];
		if (s[1] != 1.0) pt[1] *= s[1];
		if (s[2] != 1.0) pt[2] *= s[2];

		// 최종 정점의 위치를 수정하고,
		m_Pos = ::lerp(m_Pos, pt, m_EditWgt);
	}
}

/*!
*	\brief 편집 정점을 렌더링 한다.
*
*	\param r 색상의 red 성분
*	\param g 색상의 green 성분
*	\param b 색상의 blue 성분
*/
void EgVertex::RenderForEdit(float r, float g, float b)
{
	// 현재 좌표계: 월드(장면)
	glPushMatrix();
	{
		// 정점을 포함한 메쉬 객체의 모델링 좌표계로 변환
		glMultMatrixd(cast_arr(cast_mat(m_pMesh->m_MC)));

		int RenderMode = 0;
		switch (RenderMode)
		{		
		case 0: // 포인트 렌더링
			glDisable(GL_LIGHTING);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_POINT_SMOOTH);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//glColor4f(0.2627, 1.0, 0.64, 0.5);
			glColor4f(0, 0, 1, 1.0);
			glPointSize(9.0);
			glBegin(GL_POINTS);
			glVertex3dv(m_Pos.m_Coords);
			glEnd();

			// 블렌딩 모드에서 나온다.
			glDisable(GL_BLEND);
			glDisable(GL_POINT_SMOOTH);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
			break;

		// 구 렌더링
		case 1:
		{
			// 선택된 정점을 그린다.
			double size = m_pMesh->m_BndBox.GetDiagonal() * 0.003;
			glTranslated(m_Pos[0], m_Pos[1], m_Pos[2]);
			EgMaterial mtl_blue("blue");
			mtl_blue.SetAmbient(0.2f, 0.2f, 0.2f);
			mtl_blue.SetDiffuse(0.0f, 0.0f, 1.0f);
			mtl_blue.SetSpecular(0.5f, 0.5f, 0.5f);
			mtl_blue.Enable();
			glutSolidSphere(size, 10, 10);
		}
		break;
		}
	}
	glPopMatrix();
}

/*!
*	\brief 생성자
*
*	\param x 정점의 x좌표
*	\param y 정점의 y좌표
*	\param z 정점의 z좌표
*/
EgVertex::EgVertex(double x, double y, double z)
{
	m_pMesh = NULL;
	m_Idx = -1;
	m_Pos = EgPos(x, y, z);
	m_Color = EgVec3(0.0, 0.0, 0.0);
	m_EditWgt = 0.0f;
}

/*!
*	\brief 생성자
*
*	\param Pos 정점의 좌표
*/
EgVertex::EgVertex(EgPos Pos)
{
	m_pMesh = NULL;
	m_Idx = -1;
	m_Pos = Pos;
	m_Color = EgVec3(0.0, 0.0, 0.0);
	m_EditWgt = 0.0f;
}

/*!
*	\brief 소멸자
*/
EgVertex::~EgVertex()
{
}

/*!
*	\brief 정점의 차수(정점과 연결된 에지의 수)를 계산한다.
*
*	\return 정점 차수를 반환한다.
*/
int EgVertex::GetDegree()
{
	int Degree = 0;
	// 정점에서 시작하는 각각의 에지에 대하여
	for (EgEdge *e : m_pEdges)
	{
		// 정점의 차수를 증가시키고,
		Degree++;
		
		// 경계 정점일 경우도 차수에 포함한다.
		if (e->m_pNext->m_pNext->m_pMate == NULL)
			Degree++;
	}

	// 정점의 차수를 반환한다.
	return Degree;
}

/*!
*	\brief 정점의 1링 이웃 정점을 구한다.
*
*	\param Verts 1링 이웃 정점들의 포인터가 저장된다[out].
*
*	\return 1링을 구성하는 정점의 수(degree)를 반환한다.
*/
int EgVertex::GetOneRingVert(std::vector<EgVertex *> &Verts)
{
	// 배열을 초기화 하고
	Verts.clear();

	// 정점에서 시작하는 각각의 에지에 대하여
	for (EgEdge *e : m_pEdges)
	{
		// 에지의 끝점을 배열에 추가하고
		Verts.push_back(e->m_pNext->m_pVert);

		// 경계 정점일 경우도 배열에 추가한다.
		if (e->m_pNext->m_pNext->m_pMate == NULL)
			Verts.push_back(e->m_pNext->m_pNext->m_pVert);
	}

	// 1링 정점의 수를 반환한다.
	return (int)Verts.size();
}

/*!
*	\brief 정점의 1링 경계 에지의 포인터를 찾는다.
*
*	\param Edges 경계 에지들의 포인터가 저장된다[out].
*
*	\return 정점의 1링 경계 에지의 수를 반환한다.
*/
int EgVertex::GetOneRingBndryEdge(std::vector<EgEdge *> &Edges)
{
	// 리스트를 초기화 하고
	Edges.clear();

	// 정점에서 시작하는 각각의 에지에 대하여
	for (EgEdge *e : m_pEdges)
		Edges.push_back(e->m_pNext); // 경계 에지를 리스트에 추가한다.

	// 1링 경계 에지의 수를 반환한다.
	return (int)Edges.size();
}

/*!
*	\brief 정점을 공유하는 1링 삼각형들의 포인터를 찾는다.
*
*	\param Faces 정점을 공유하는 삼각형들의 포인터가 저장된다[out].
*
*	\return 정점을 공유하는 1링 삼각형의 수를 반환한다.
*/
int EgVertex::GetOneRingFace(std::vector<EgFace *> &Faces)
{
	// 배열을 초기화 하고
	Faces.clear();

	// 정점에서 시작하는 각각의 에지가 포함된 삼각형을 구한다.
	for (EgEdge *e : m_pEdges)
		Faces.push_back(e->m_pFace);

	// 삼각형의 수를 반환한다.
	return (int)Faces.size();
}

/* !
*	\brief 경계 정점 여부를 조사한다.
*
*	\return 경계 정점이면 true, 아니면 false를 반환한다.
*/
bool EgVertex::IsBndry()
{
	// 정점에서 시작하는 각각의 에지에 대하여
	for (EgEdge *e : m_pEdges)
	{
		// 하나라도 경계 에지라면 경계 정점이므로 true를 반환한다.
		if (e->IsBndry())
			return true;
	}

	// 경계 에지가 없다면 false를 반환한다.
	return false;
}

/*!
*	\brief	정점이 오목한 영역에 있는지 판단한다.
*
*	\return	정점이 오목한 영역에 있으면 true, 아니면 false를 반환한다.
*/
bool EgVertex::IsConcave()
{
	EgPos p = m_Pos;
	EgPos q = GetOneRingVertCenter();
	EgVec3 N = GetAvgNormal();
	double cs = (q - p) * N;
	if (cs > 1.0e-6)
		return true;	
	
	return false;
}

/*!
*	\brief	해당 정점이 정점 v와 연결되어 있는지 조사한다.
*
*	\param	v 조사할 정점
*
*	\return	해당 정점이 v와 연결되어 있으면 해당 에지를 반환하고, 아니면 NULL을 반환한다.
*/
EgEdge *EgVertex::IsAdjacent(EgVertex *v)
{
	// 현재 정점에서 시작하는 각각의 에지에 대하여
	for (EgEdge *e : m_pEdges)
	{
		// 다음 에지의 시작 정점이 v와 같다면 해당 에지를 반환한다.
		if (e->m_pNext->m_pVert == v)
			return e;
	}
	return NULL;
}

/*!
*	\brief	정점의 평균 단위 법선을 구한다.
*	
*	\return 정점의 평균 단위 법선을 반환한다.
*/
EgVec3 EgVertex::GetAvgNormal()
{
	if (m_pEdges.empty())
		return EgVec3(0.0, 0.0, 0.0);

	// 방법 1
	EgVec3 N;
	for (EgEdge *e : m_pEdges)
		N += e->m_pNormal->m_Dir;
	N.Normalize();

	// 방법 2
	//EgVec3 N;
	//for (EgEdge *e : m_pEdges)
	//{
	//	double angle = e->m_pNext->GetAngle(true);
	//	N += angle * e->m_pNormal->m_Dir;
	//}
	//N.Normalize();

	return N;
}

/*!
*	\breif	정점의 1-링 정점들의 무게중심을 반환한다.
*
*	\return 정점의 1-링 정점들의 무게중심을 반환한다.
*/
EgPos EgVertex::GetOneRingVertCenter()
{
	// 정점의 1링 이웃 정점의 무게중심을 구한다.
	EgVec3 cog;
	std::vector<EgVertex *> Verts;
	int Degree = GetOneRingVert(Verts);
	for (EgVertex *v : Verts)
		cog += cast_vec3(v->m_Pos);
	cog /= (double)Degree;

	return cast_pos(cog);
}

/*!
*	\brief	정점의 위치를 perturbation 한다
*
*	\param	rate[in]	무게 중심 방향으로 움직일 비율
*
*	\return	변경된 정점의 위치를 반환한다.
*/
EgPos EgVertex::Perturbate(double rate)
{
	EgPos P = GetOneRingVertCenter();
	m_Pos = m_Pos + (P - m_Pos) * rate;

	return m_Pos;
}

/*!
*	\brief	정점에서 시작하는 에지 리스트에서 에지를 제거한다.
*
*	\param	e 제거할 에지의 포인터
*
*	\return	제거 후, 정점에서 시작하는 에지의 수를 반환한다(제거할 에지가 없는 경우 -1을 반환)
*/
int EgVertex::DeleteEdge(EgEdge *e)
{
	std::vector<EgEdge *>::iterator it;
	for (it = m_pEdges.begin(); it != m_pEdges.end(); ++it)
	{
		if (*it == e)
		{
			m_pEdges.erase(it);
			return (int)m_pEdges.size();
		}
	}

	return -1;
}

/*!
*	\brief 생성자
*
*	\param u 수평 방향 텍스처 좌표
*	\param v 수직 방향 텍스처 좌표
*/
EgTexel::EgTexel(double u, double v)
{
	m_Idx = -1;
	m_UV[0] = u;
	m_UV[1] = v;
}

/*!
*	\brief 소멸자
*/
EgTexel::~EgTexel()
{
}

/*!	\brief 생성자
*
*	\param nx 법선의 x 좌표
*	\param ny 법선의 y 좌표
*	\param nz 법선의 z 좌표
*/
EgNormal::EgNormal(double nx, double ny, double nz)
{
	m_Idx = -1;
	m_Dir = EgVec3(nx, ny, nz);
}

/*!	\brief 생성자
*
*	\param Dir 법선의 방향
*/
EgNormal::EgNormal(EgVec3 Dir)
{
	m_Idx = -1;
	m_Dir = Dir;
}

/*!
*	\brief 소멸자
*/
EgNormal::~EgNormal()
{
}

/*!
*	\brief 생성자
*
*	\param pVert 에지의 시작점에 할당된 정점에 대한 포인터
*	\param pTexel 에지의 시작점에 할당된 텍스처 정점에 대한 포인터
*	\param pNormal 에지의 시작에 할당된 법선에 대한 포인터
*/
EgEdge::EgEdge(EgVertex *pVert, EgTexel *pTexel, EgNormal *pNormal)
{
	// 정점, 텍셀, 법선 정보를 에지의 시작점에 할당한다.
	m_pVert = pVert;
	m_pTexel = pTexel;
	m_pNormal = pNormal;	

	// 다음 에지, 반대편 에지, 에지가 속한 삼각형에 대한 포인터를 초기화한다.
	m_pNext = NULL;
	m_pMate = NULL;
	m_pFace = NULL;

	// 시작점의 정점에 현재 에지를 추가한다.
	m_pVert->m_pEdges.push_back(this);

	// 단순화에 필요한 에지 제거 비용을 초기화 한다.
	m_Cost = 0.0;
}

/*!
*	\brief 소멸자
*/
EgEdge::~EgEdge()
{
}

/*! 
*	\brief 에지와 메이트 에지를 포함한 삼각형을 구한다.
*
*	\param Faces 삼각형 포인터가 저장된다.
*
*	\return 메이트 에지가 있는 경우 2, 메이트 에지가 없는 경우 1이 반환된다.
*/
int EgEdge::GetIncidentFace(std::vector<EgFace *> &Faces)
{
	Faces.push_back(m_pFace);
	if (m_pMate != NULL)
		Faces.push_back(m_pMate->m_pFace);

	return (int)Faces.size();
}

/*!
*	\brief 에지가 경계 에지인지 조사한다.
*
*	\return 경계 에지이면 true, 아니면 false를 반환한다.
*/
bool EgEdge::IsBndry()
{
	return (m_pMate == NULL);
}

/*!
*	\brief 에지가 텍스처 경계 에지인지 조사한다.
*
*	\return 텍스처 경계 에지이면 true, 아니면 false를 반환한다.
*/
bool EgEdge::IsTexBndry()
{
	return (IsBndry() || m_pTexel != m_pMate->m_pNext->m_pTexel);
}

/*!
*	\brief	에지가 제거 가능한지 검사한다.
*
*	\return 에지가 제거가능할 경우 true, 아니면 false를 반환한다.
*/
bool EgEdge::IsCollapsible()
{
	// 에지의 시작점(v1)과 끝점(v2)을 구한다.
	EgVertex *v1 = m_pVert;
	EgVertex *v2 = m_pNext->m_pVert;

	// v1과 v2가 경계 정점인데, 에지가 경계 에지가 아닌 경우 false를 반환한다.
	if (v1->IsBndry() && v2->IsBndry() && !IsBndry())
		return false;

	// v1과 v2의 1링 정점의 집합 A와 B를 구한다.
	std::vector<EgVertex *> A, B;
	v1->GetOneRingVert(A);
	v2->GetOneRingVert(B);

	// A와 B의 교집합을 구한다.
	std::vector<EgVertex *> C;
	for (EgVertex *a : A)
	{
		for (EgVertex *b : B)
		{
			if (a == b)
			{
				C.push_back(a);
				break;
			}
		}
	}

	// 에지 제거의 조건을 만족할 경우 true를 반환한다.
	if (m_pMate == NULL && C.size() == 1)
		return true;
	if (m_pMate != NULL && C.size() == 2)
		return true;

	// 에지 제거 조건을 만족하지 못하는 경우 false를 반환한다.
	return false;
}

/*!
*	\brief 삼각형에서 에지가 마주보고 있는 각도를 계산한다.
*	\warning 0 < 사이각 < 180
*
*	\param bRadian 각도의 형태(true: 라디안, false: 각도)
*
*	\return 삼각형에서 에지가 마주보고 있는 각도를 반환한다.
*/
double EgEdge::GetAngle(bool bRadian)
{
	// 에지가 포함된 삼각형에서 세 점과 법선 벡터를 구한다.
	EgPos p = m_pVert->m_Pos;
	EgPos q = m_pNext->m_pVert->m_Pos;
	EgPos r = m_pNext->m_pNext->m_pVert->m_Pos;
	EgVec3 N = m_pFace->GetFaceNormal(true);

	// 에지가 마주보고 있는 각도를 계산하여 반환한다.
	return (bRadian) ? angle(p - r, q - r, N, true) : angle(p - r, q - r, N, false);
}

/*!
*	\brief 삼각형에서 에지가 마주보고 있는 각도의 코탄젠트 값을 계산한다.
*
*	\return 삼각형에서 에지가 마주보고 있는 각도의 코탄젠트 값을 반환한다.
*/
double EgEdge::GetCotangent()
{
	// 에지가 포함된 삼각형에서 세 점과 법선 벡터를 구한다.
	EgPos p = m_pVert->m_Pos;
	EgPos q = m_pNext->m_pVert->m_Pos;
	EgPos r = m_pNext->m_pNext->m_pVert->m_Pos;
	EgVec3 rp = p - r;
	EgVec3 rq = q - r;

	// 에지가 마주보고 있는 각도의 코탄젠트 값을 반환한다.
	return (rp * rq) / norm(rp ^ rq);
}

/*!
*	\brief	에지의 길이를 구한다.
*
*	\return 에지의 길이를 반환한다.
*/
double EgEdge::GetLength()
{
	return dist(m_pVert->m_Pos, m_pNext->m_pVert->m_Pos);
}

/*!
*	\brief	에지 방향으로 법곡률을 구한다.
*
*	\return 계산된 법곡률을 반환한다.
*/
double EgEdge::GetNormalCurvature()
{
	EgVertex *vi = m_pVert;
	EgVertex *vj = m_pNext->m_pVert;
	EgVec3 ni = vi->GetAvgNormal();
	double ret = 2.0 * (vi->m_Pos - vj->m_Pos) * ni / norm_sq(vi->m_Pos - vj->m_Pos);
	
	return ret;
}

/*!
*	\brief 객체의 클래스 이름을 반환한다.
*
*	\return 객체의 클래스 이름을 반환한다.
*/
std::string EgFace::GetClassName()
{
	return std::string("EgFace");
}

/*!
*	\brief 삼각형이 포함된 메쉬의 포인터를 반환한다.
*
*	\return 삼각형이 포함된 메쉬의 포인터를 반환한다.
*/
EgMesh *EgFace::GetMesh()
{
	return m_pEdge->m_pVert->m_pMesh;
}

/*!
*	\brief 삼각형의 무게중심에서 정의된 좌표계를 반환한다.
*
*	\return 삼각형의 무게중심에서 정의된 좌표계(월드 좌표계 기준)를 반환한다.
*/
EgTrans EgFace::GetTrans()
{
	return EgTrans(cast_vec3(GetCOG(false)));
}

/*!
*	\brief 삼각형과 삼각형이 포함된 객체을 편집한다.
*
*	\param EC 편집툴의 좌표계
*	\param t 삼각형의 이동 변위
*	\param q 삼각형의 회전 변위
*	\param s 삼각형의 스케일 변위
*	\param bFirst 편집 객체들 중 첫 번째 객체에 대한 편집 여부
*/
void EgFace::Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst)
{
	EgMesh *pMesh = GetMesh();
	if (pMesh->GetClassName() != "EgMesh")
		return;

	// 편집 변환을 위한 정적 변수를 선언하고,
	static EgTrans MC2MC;
	static std::vector<bool> VertFlag;

	// 여러개의 편집 객체들 중 첫 번째 편집이라면
	if (bFirst)
	{
		int NumVert = pMesh->GetNumVert();
		VertFlag.clear();
		VertFlag.assign(NumVert, false);

		// 편집 변환을 계산하고,
		MC2MC = inv(pMesh->m_MC) * EC * EgTrans(t, q) * inv(EC) * pMesh->m_MC;
	}	

	// 삼각형의 세 정점의 좌표를 편집한다.
	int *vIdx = GetVertexIdxs();
	if (VertFlag[vIdx[0]] == false)
	{
		VertFlag[vIdx[0]] = true;
		pMesh->m_pVerts[vIdx[0]]->m_Pos = MC2MC * pMesh->m_pVerts[vIdx[0]]->m_Pos;
	}
	if (VertFlag[vIdx[1]] == false)
	{
		VertFlag[vIdx[1]] = true;
		pMesh->m_pVerts[vIdx[1]]->m_Pos = MC2MC * pMesh->m_pVerts[vIdx[1]]->m_Pos;
	}
	if (VertFlag[vIdx[2]] == false)
	{
		VertFlag[vIdx[2]] = true;
		pMesh->m_pVerts[vIdx[2]]->m_Pos = MC2MC * pMesh->m_pVerts[vIdx[2]]->m_Pos;
	}
}

/*!
*	\brief 편집 삼각형을 렌더링 한다.
*
*	\param r 색상의 red 성분
*	\param g 색상의 green 성분
*	\param b 색상의 blue 성분
*/
void EgFace::RenderForEdit(float r, float g, float b)
{
	// 블렌딩 기능을 활성하고, 조명을 비활성화 한다.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LIGHTING);	
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0.3f, 0.3f);

	EgMesh *pMesh = GetMesh();

	// 현재 좌표계: 월드(장면)
	glPushMatrix();
	{
		// 메쉬 좌표계로 변환
		glMultMatrixd(cast_arr(cast_mat(pMesh->m_MC)));

		//glColor3d(0.2627, 1.0, 0.64);
		//glColor4d(0.2627, 1.0, 0.64, 0.37);
		glColor4d(0.5, 0.5, 0.9, 0.5);
		//glColor4d(227.0/255.0, 220.0/255.0, 193.0/255.0, 0.37);
		int *vidx = GetVertexIdxs();
		glBegin(GL_TRIANGLES);
		{
			glVertex3dv(pMesh->m_pVerts[vidx[0]]->m_Pos.m_Coords);
			glVertex3dv(pMesh->m_pVerts[vidx[1]]->m_Pos.m_Coords);
			glVertex3dv(pMesh->m_pVerts[vidx[2]]->m_Pos.m_Coords);
		}
		glEnd();

		//glColor4f(0.0, 0.0, 0.0, 0.5);
		//glLineWidth(1.0);
		//glBegin(GL_LINE_LOOP);
		//{
		//	glVertex3dv(pMesh->m_pVerts[vidx[0]]->m_Pos.V);
		//	glVertex3dv(pMesh->m_pVerts[vidx[1]]->m_Pos.V);
		//	glVertex3dv(pMesh->m_pVerts[vidx[2]]->m_Pos.V);
		//}
		//glEnd();
		//glLineWidth(1.0);
	}
	glPopMatrix();

	// 오프셋 기능을 비활성하고, 조명을 활성화 한다.
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glDisable(GL_POLYGON_OFFSET_FILL);
}

/*!
*	\brief 생성자
*
*	\param e0 삼각형의 첫 번째 에지
*	\param e1 삼각형의 두 번째 에지
*	\param e2 삼각형의 세 번째 에지
*	\param pMtl 삼각형이 사용하는 재질에 대한 포인터
*	\param GroupName 삼각형이 속한 그룹의 이름
*/
EgFace::EgFace(EgEdge *e0, EgEdge *e1, EgEdge *e2, EgMaterial *pMtl, std::string GroupName)
{
	// 삼각형의 인덱스를 초기화 하고 시작 에지를 설정한다.
	m_Idx = -1;
	m_pEdge = e0;

	// 에지의 연결관계를 생성한다.
	e0->m_pNext = e1;
	e1->m_pNext = e2;
	e2->m_pNext = e0;
	e0->m_pFace = e1->m_pFace = e2->m_pFace = this;	

	// 재질과 삼각형이 속한 그룹의 이름을 설정한다.
	m_pMaterial = pMtl;
	m_GroupName = GroupName;
}

/*!
*	\brief 생성자
*
*	\param v0 삼각형의 첫 번째 정점
*	\param v1 삼각형의 두 번째 정점
*	\param v2 삼각형의 세 번째 정점
*	\param t0 삼각형의 첫 번째 정점의 텍스처 좌표
*	\param t1 삼각형의 두 번째 정점의 텍스처 좌표
*	\param t2 삼각형의 세 번째 정점의 텍스처 좌표
*	\param n0 삼각형의 첫 번째 정점의 법선
*	\param n1 삼각형의 두 번째 정점의 법선
*	\param n2 삼각형의 세 번째 정점의 법선
*	\param pMtl 삼각형이 사용하는 재질에 대한 포인터
*	\param GroupName 삼각형이 속한 그룹의 이름
*/
EgFace::EgFace(
	EgVertex *v0, EgVertex *v1, EgVertex *v2, 
	EgTexel *t0, EgTexel *t1, EgTexel *t2, 
	EgNormal *n0, EgNormal *n1, EgNormal *n2, 
	EgMaterial * pMtl, std::string GroupName)
{
	// 삼각형의 인덱스를 초기화 하고
	m_Idx = -1;

	// 각 정점에서 시작하는 3개의 에지를 생성하고 시작 에지를 설정한다.
	EgEdge *e0 = new EgEdge(v0, t0, n0);
	EgEdge *e1 = new EgEdge(v1, t1, n1);
	EgEdge *e2 = new EgEdge(v2, t2, n2);
	m_pEdge = e0;

	// 에지의 연결관계를 생성한다.
	e0->m_pNext = e1;
	e1->m_pNext = e2;
	e2->m_pNext = e0;
	e0->m_pFace = e1->m_pFace = e2->m_pFace = this;

	// 재질과 삼각형이 속한 그룹의 이름을 설정한다.
	m_pMaterial = pMtl;
	m_GroupName = GroupName;	
}

/*!
*	\brief 소멸자
*/
EgFace::~EgFace()
{
	delete m_pEdge->m_pNext->m_pNext;
	delete m_pEdge->m_pNext;
	delete m_pEdge;
	m_pEdge = NULL;
}

/*!
*	\brief 삼각형에서 정점의 좌표를 반환한다.
*
*	\param vidx 삼각형을 구성하는 정점 인덱스(0, 1, 2)
*
*	\return 인덱스에 대응하는 정점의 좌표를 반환한다.
*/
EgPos EgFace::GetVertexPos(int vidx)
{
	return GetEdge(vidx)->m_pVert->m_Pos;
}

/*!
*	\brief 삼각형 정점의 포인터를 반환한다.
*
*	\param vIdx 삼각형을 구성하는 정점의 인덱스(0, 1, 2)
*
*	\return 인덱스에 대응되는 정점의 포인터를 반환한다.
*/
EgVertex *EgFace::GetVertex(int vIdx)
{
	return GetEdge(vIdx)->m_pVert;
}

/*!
*	\brief 삼각형에서 하프에지에 할당된 텍스처 좌표를 반환한다.
*
*	\param tidx 삼각형을 구성하는 하프에지 인덱스(0, 1, 2)
*
*	\return 인덱스에 대응하는 텍스처 좌표를 반환한다.
*/
EgVec2 EgFace::GetTexelCoord(int tidx)
{
	return GetEdge(tidx)->m_pTexel->m_UV;
}

/*!
*	\brief 삼각형에서 텍스처 정점의 포인터를 반환한다.
*
*	\param tidx 삼각형을 구성하는 텍스처 정점의 인덱스(0, 1, 2)
*
*	\return 인덱스에 대응되는 텍스처 정점의 포인터를 반환한다.
*/
EgTexel *EgFace::GetTexel(int tidx)
{
	return GetEdge(tidx)->m_pTexel;
}

/*!
*	\brief 삼각형에서 하프에지에 할당된 법선의 방향을 반환한다.
*
*	\param nidx 삼각형을 구성하는 하프에지 인덱스(0, 1, 2)
*	\param bLocal 객체의 모델링 좌표계에서 표현된 법선일 경우 true, 월드 좌표계에서 표현될 경우 false
*
*	\return 인덱스에 대응하는 법선의 방향을 반환한다.
*/
EgVec3 EgFace::GetNormalDir(int nidx, bool bLocal)
{
	EgVec3 N = GetEdge(nidx)->m_pNormal->m_Dir;
	if (bLocal)
		return N;
	else
		return GetMesh()->m_MC * N;
}

/*!
*	\brief 삼각형에서 법선의 포인터를 반환한다.
*
*	\param nIdx 삼각형을 구성하는 법선의 인덱스(0, 1, 2)
*
*	\return 인덱스에 대응하는 법선의 포인터를 반환한다.
*/
EgNormal *EgFace::GetNormal(int nidx)
{
	return GetEdge(nidx)->m_pNormal;
}

/*!
*	\brief 삼각형의 단위 법선벡터를 계산한다.
*
*	\param bLocal 객체의 모델링 좌표계에서 표현된 법선일 경우 true, 월드 좌표계에서 표현될 경우 false
*
*	\return 삼각형의 단위 법선벡터를 반환한다.
*/
EgVec3 EgFace::GetFaceNormal(bool bLocal)
{
	// 삼각형의 세 정점의 위치를 구하여
	EgPos p0 = m_pEdge->m_pVert->m_Pos;
	EgPos p1 = m_pEdge->m_pNext->m_pVert->m_Pos;
	EgPos p2 = m_pEdge->m_pNext->m_pNext->m_pVert->m_Pos;

	// 단위 길이의 법선을 구하여 반환한다.
	EgVec3 N = ((p1 - p0) ^ (p2 - p0)).Normalize();
	if (bLocal)
		return N;
	else
		return GetMesh()->m_MC * N;
}

/*!
*	\brief 삼각형 에지 포인터를 반환한다.
*
*	\param eIdx 삼각형을 구성하는 에지의 인덱스(0, 1, 2)
*
*	\return 인덱스에 대응되는 에지의 포인터를 반환한다.
*/
EgEdge *EgFace::GetEdge(int eIdx)
{
	switch (eIdx)
	{
	case 0:
		return m_pEdge;
	case 1:
		return m_pEdge->m_pNext;
	case 2:
		return m_pEdge->m_pNext->m_pNext;
	}

	return NULL;
}

/*!
*	\brief	삼각형을 구성하는 정점의 인덱스 리스트를 반환한다.
*
*	\return 삼각형을 구성하는 정점의 인덱스 배열에 대한 포인터를 반환한다.
*/
int *EgFace::GetVertexIdxs()
{
	static int Idxs[3] = { 0 };
	Idxs[0] = m_pEdge->m_pVert->m_Idx;
	Idxs[1] = m_pEdge->m_pNext->m_pVert->m_Idx;
	Idxs[2] = m_pEdge->m_pNext->m_pNext->m_pVert->m_Idx;

	return Idxs;
}

/*!
*	\brief	삼각형을 구성하는 텍스처 정점의 인덱스 리스트를 반환한다.
*
*	\return 삼각형을 구성하는 텍스처 정점의 인덱스 배열에 대한 포인터를 반환한다.
*/
int *EgFace::GetTexelIdxs()
{
	static int Idxs[3] = { 0 };
	if (m_pEdge->m_pTexel == NULL)
		return NULL;

	Idxs[0] = m_pEdge->m_pTexel->m_Idx;
	Idxs[1] = m_pEdge->m_pNext->m_pTexel->m_Idx;
	Idxs[2] = m_pEdge->m_pNext->m_pNext->m_pTexel->m_Idx;

	return Idxs;
}

/*!
*	\brief	삼각형을 구성하는 법선의 인덱스 리스트를 반환한다.
*
*	\return 삼각형을 구성하는 법선의 인덱스 배열에 대한 포인터를 반환한다.
*/
int *EgFace::GetNormalIdxs()
{
	static int Idxs[3] = { 0 };
	Idxs[0] = m_pEdge->m_pNormal->m_Idx;
	Idxs[1] = m_pEdge->m_pNext->m_pNormal->m_Idx;
	Idxs[2] = m_pEdge->m_pNext->m_pNext->m_pNormal->m_Idx;

	return Idxs;
}

/*!
*	\brief 삼각형에서 에지의 인덱스를 반환한다.
*
*	\param e 찾을 에지에 대한 포인터
*
*	\return 삼각형에서 에지의 인덱스를 반환한다.
*/
int EgFace::GetEdgeIdx(EgEdge *e)
{
	if (e == m_pEdge)
		return 0;
	if (e == m_pEdge->m_pNext)
		return 1;
	if (e == m_pEdge->m_pNext->m_pNext)
		return 2;

	return -1;
}

/*!
*	\brief 삼각형의 면적을 계산한다.
*
*	\return 계산된 면적을 반환한다.
*/
double EgFace::GetArea()
{
	EgVec3 a = GetVertex(1)->m_Pos - GetVertex(0)->m_Pos;
	EgVec3 b = GetVertex(2)->m_Pos - GetVertex(0)->m_Pos;
	return norm(a ^ b) * 0.5;
}

/*!
*	\brief	삼각형의 둘레의 길이를 계산한다.
*
*	\return	계산된 둘레의 길이를 반환한다.
*/
double EgFace::GetPerimeter()
{
	EgPos p0 = GetVertex(0)->m_Pos;
	EgPos p1 = GetVertex(1)->m_Pos;
	EgPos p2 = GetVertex(2)->m_Pos;

	return dist(p0, p1) + dist(p1, p2) + dist(p2, p0);
}

/*!
*	\brief 삼각형의 무게 중심을 계산한다.
*
*	\param bLocal 객체의 모델링 좌표계에서 표현된 경우 true, 월드 좌표계에서 표현될 경우 false
*
*	\return 삼각형의 무게 중심의 위치를 반환한다.
*/
EgPos EgFace::GetCOG(bool bLocal)
{
	EgPos p0 = GetVertexPos(0);
	EgPos p1 = GetVertexPos(1);
	EgPos p2 = GetVertexPos(2);
	EgPos p = cast_pos((cast_vec3(p0) + cast_vec3(p1) + cast_vec3(p2)) / 3.0);

	if (bLocal)
		return p;
	else
		return GetMesh()->m_MC * p;
}

/*!
*	\brief 삼각형 에지를 공유한 삼각형을 구한다.
*
*	\param Faces 에지를 공유한 삼각형의 포인터가 저장된다.
*
*	\return 에지를 공유한 삼각형의 개수(0, 1, 2, 3)를 반환한다.
*/
int EgFace::GetAdjacentFace(std::vector<EgFace *> &Faces)
{
	Faces.clear();

	if (m_pEdge->m_pMate != NULL)
		Faces.push_back(m_pEdge->m_pMate->m_pFace);
	if (m_pEdge->m_pNext->m_pMate != NULL)
		Faces.push_back(m_pEdge->m_pNext->m_pMate->m_pFace);
	if (m_pEdge->m_pNext->m_pNext->m_pMate != NULL)
		Faces.push_back(m_pEdge->m_pNext->m_pNext->m_pMate->m_pFace);

	return (int)Faces.size();
}

/*!
*	\brief 삼각형이 경계 삼각형인지 조사한다.
*
*	\return	경계 삼각형이면 true, 아니면 false를 반환한다.
*/
bool EgFace::IsBndryFace()
{
	for (int i = 0; i < 3; ++i)
		if (GetVertex(i)->IsBndry())
			return true;
	return false;
}

/*!
*	\brief 객체의 클래스 이름을 반환한다.
*
*	\return 객체의 클래스 이름을 반환한다.
*/
std::string EgFacePt::GetClassName()
{
	return std::string("EgFacePt");
}

/*!
*	\brief 선택점이 포함된 메쉬의 포인터를 반환한다.
*
*	\return 선택점이 포함된 메쉬의 포인터를 반환한다.
*/
EgMesh *EgFacePt::GetMesh()
{
	return m_pFace->GetMesh();
}

/*!
*	\brief 선택점의 로컬 좌표계를 반환한다.
*
*	\return 선택점의 로컬 좌표계를 반환한다.
*/
EgTrans EgFacePt::GetTrans()
{
	return GetMesh()->GetTrans() * EgTrans(cast_vec3(Pos()));
}

/*!
*	\brief 선택점이 속한 메쉬를 편집한다.
*
*	\param EC[in] 편집툴의 좌표계
*	\param t[in] 정점의 이동 변위
*	\param q[in] 정점의 회전 변위
*	\param s[in] 정점의 스케일 변위
*	\param bFirst[in] 편집 객체들 중 첫 번째 객체에 대한 편집 여부
*/
void EgFacePt::Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst)
{
	m_pFace->Edit(EC, t, q, s, bFirst);
}

/*!
*	\brief 선택점을 렌더링 한다.
*
*	\param r[in] 색상의 red 성분
*	\param g[in] 색상의 green 성분
*	\param b[in] 색상의 blue 성분
*/
void EgFacePt::RenderForEdit(float r, float g, float b)
{
	// 현재 좌표계: 월드(장면)
	glPushMatrix();
	{
		// 점을 포함한 메쉬 객체의 모델링 좌표계로 변환
		EgMesh *pMesh = GetMesh();
		glMultMatrixd(cast_arr(cast_mat(pMesh->m_MC)));

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glColor4f(0.2627, 1.0, 0.64, 0.5);
		glColor4f(0, 0, 1, 1.0);
		glPointSize(9.0);
		glBegin(GL_POINTS);
		glVertex3dv(Pos().m_Coords);
		glEnd();

		// 블렌딩 모드에서 나온다.
		glDisable(GL_BLEND);
		glDisable(GL_POINT_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();
}

/*!
*	\brief 생성자
*
*	\param pFace[in]	점이 속한 삼각형에 대한 포인터
*	\param Coords[in]	점의 barycentric 좌표
*/
EgFacePt::EgFacePt(EgFace *pFace, EgVec3 Coords)
{
	m_pFace = pFace;
	m_Coords = Coords;
}

/*!
*	\brief 복사 생성자
*
*	\param cpy[in] 복사할 객체의 포인터
*
*	\return 복사된 자신을 반환한다.
*/
EgFacePt::EgFacePt(const EgFacePt &cpy)
{
	m_pFace = cpy.m_pFace;
	m_Coords = cpy.m_Coords;
}

/*!
*	\brief 소멸자
*/
EgFacePt::~EgFacePt()
{
}

/*!
*	\brief 대입 연산자
*
*	\param rhs[in] 대입할 오른쪽 피연산자
*
*	\return 대입된 자신을 반환한다.
*/
EgFacePt &EgFacePt::operator=(const EgFacePt &rhs)
{
	m_pFace = rhs.m_pFace;
	m_Coords = rhs.m_Coords;

	return *this;
}

/*!
*	\brief 삼각형 내부점의 3차원 위치를 계산한다.
*
*	\return 계산된 3차원 위치를 반환한다.
*/
EgPos EgFacePt::Pos()
{
	EgPos p0 = P0(m_pFace);
	EgPos p1 = P1(m_pFace);
	EgPos p2 = P2(m_pFace);
	return p2 + (p0 - p2) * m_Coords[0] + (p1 - p2) * m_Coords[1];
}

/*!
*	\brief	삼각형 코너 정점 여부를 조사한다.
*
*	\return 삼각형의 정점이면 true, 아니면 false를 반환한다.
*/
bool EgFacePt::IsVertex()
{
	if (EQ(m_Coords[0], 1.0, 1.0e-6) && EQ(m_Coords[1], 0.0, 1.0e-6) && EQ(m_Coords[2], 0.0, 1.0e-6))
		return true;
	if (EQ(m_Coords[0], 0.0, 1.0e-6) && EQ(m_Coords[1], 1.0, 1.0e-6) && EQ(m_Coords[2], 0.0, 1.0e-6))
		return true;
	if (EQ(m_Coords[0], 0.0, 1.0e-6) && EQ(m_Coords[1], 0.0, 1.0e-6) && EQ(m_Coords[2], 1.0, 1.0e-6))
		return true;

	return false;
}

/*!
*	\brief 삼각형 정점까지의 거리를 계산한다.
*
*	\param vidx 타겟 정점의 인덱스
*
*	\return 계산된 거리를 반환한다.
*/
double EgFacePt::Dist2Vert(int vidx)
{
	switch (vidx)
	{
	case 0: return ::dist(Pos(), P0(m_pFace));
	case 1: return ::dist(Pos(), P1(m_pFace));
	case 2: return ::dist(Pos(), P2(m_pFace));
	default: return 0.0;
	}
}

/*!
*	\brief	가장 가까운 정점을 찾는다.
*
*	\return 가장 가까운 정점의 포인터를 반환한다.
*/
EgVertex *EgFacePt::GetClosestVert()
{
	EgPos p = Pos();
	EgVertex *v = NULL;
	double min_d = INFINITY;

	for (int i = 0; i < 3; ++i)
	{
		double d = ::dist(p, m_pFace->GetVertexPos(i));
		if (d < min_d)
		{
			min_d = d;
			v = m_pFace->GetVertex(i);
		}
	}

	return v;
}

/*!
*	\brief 생성자
*/
EgTexture::EgTexture()
{
	m_FileName = "";
	m_TexId = 0;
}

/*!
*	\brief 소멸자
*/
EgTexture::~EgTexture()
{
	// 생성된 OpenGL 텍스처가 있다면 제거한다.
	if (m_TexId != 0)
	{
		glDeleteTextures(1, &m_TexId);
		m_TexId = 0;
	}
}

/*!
*	\brief 텍스처 파일로부터 OpenGL 텍스처를 생성한다.
*
*	\param fname 텍스처 파일명
*
*	\return 성공이면 true, 실패면 false를 반환한다.
*/
bool EgTexture::LoadTexture(const char *fname)
{
	// 현재 작업 디렉터리를 절대 경로로 출력한다.
	//char cwd[1024] = { 0 };
	//_getcwd(cwd, 1024);
	//printf("\tCurrent working directory = %s\n", cwd);

	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// or, GL_NEAREST
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// or, GL_NEAREST
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);		// or, GL_REPLACE

	// ILUT 라이브러리를 이용하여 이지미 파일을 OPENGL 텍스처 형태로 로드한다.
	m_TexId = ilutGLLoadImage((char *)fname);
	glDisable(GL_TEXTURE_2D);

	if (m_TexId < 1)
	{
		printf("\tLoading %s failed...\n", fname);
		return false;
	}
	else
	{
		m_FileName = std::string(fname);
		printf("\tLoading %s succeeded...\n", fname);
		return true;
	}
}

/*!
*	\brief 생성자
*
*	\param Name 생성할 재질의 이름
*/
EgMaterial::EgMaterial(std::string Name)
{
	m_Idx = -1;
	m_Name = Name;
	SetAmbient(0.2f, 0.2f, 0.2f, 1.0f);
	SetDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
	SetSpecular(0.7f, 0.7f, 0.7f, 1.0f);
	m_Shiness = 32.0f;
}

/*!
*	\brief 복사 생성자
*
*	\param cpy 복사할 재질
*/
EgMaterial::EgMaterial(const EgMaterial &cpy)
{
	m_Idx = -1;
	m_Name = cpy.m_Name;
	ARR_COPY(m_Ambient, cpy.m_Ambient, 4);
	ARR_COPY(m_Diffuse, cpy.m_Diffuse, 4);
	ARR_COPY(m_Specular, cpy.m_Specular, 4);
	m_Shiness = cpy.m_Shiness;
	m_pTexture = cpy.m_pTexture;
}

/*!
*	\brief 소멸자
*/
EgMaterial::~EgMaterial()
{
}

/*!
*	\brief 재질의 주변광 반사 계수를 설정한다.
*
*	\param r Red 성분에 대한 반사계수
*	\param g Green 성분에 대한 반사계수
*	\param b Blue 성분에 대한 반사계수
*	\param a Alpha 블렌딩을 위한 알파 값
*/
void EgMaterial::SetAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_Ambient[0] = r;
	m_Ambient[1] = g;
	m_Ambient[2] = b;
	m_Ambient[3] = a;
}

/*!
*	\brief 재질의 난반사 계수를 설정한다.
*
*	\param r Red 성분에 대한 반사계수
*	\param g Green 성분에 대한 반사계수
*	\param b Blue 성분에 대한 반사계수
*	\param a Alpha 블렌딩을 위한 알파 값
*/
void EgMaterial::SetDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_Diffuse[0] = r;
	m_Diffuse[1] = g;
	m_Diffuse[2] = b;
	m_Diffuse[3] = a;
}

/*!
*	\brief 재질의 전반사 계수를 설정한다.
*
*	\param r Red 전반사 성분에 대한 반사계수
*	\param g Green 전반사 성분에 대한 반사계수
*	\param b Blue 전반사 성분에 대한 반사계수
*	\param a Alpha 블렌딩을 위한 알파 값
*/
void EgMaterial::SetSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_Specular[0] = r;
	m_Specular[1] = g;
	m_Specular[2] = b;
	m_Specular[3] = a;
}

/*!
*	\brief 재질의 전반사 지수를 설정한다.
*
*	\param s 재질의 전반사 지수
*/
void EgMaterial::SetShiness(GLfloat s)
{
	m_Shiness = s;	
}

/*!
*	\brief 재질의 텍스처를 설정한다.
*
*	\param pTexture 설정할 텍스처에 대한 포인터
*/
void EgMaterial::SetTexture(EgTexture *pTexture)
{
	m_pTexture = pTexture;
}

/*!
*	\brief 재질을 활성화 한다.
*
*	\param bTexture 텍스처 적용 여부
*/
void EgMaterial::Enable()
{
	// 알파 값이 1이 아니라면 블렌딩을 활성화하고,
	if (m_Diffuse[3] != 1.0f)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else // 아니면 블렌딩을 비활성화 한다.
		glDisable(GL_BLEND);

	// 삼각형의 앞면과 뒷면에 동일한 재질을 적용한다.
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_Ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_Diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_Specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_Shiness);
}

/*!
*	\brief 재질의 비활성화 한다.
*/
void EgMaterial::Disable()
{
	glDisable(GL_BLEND);
}

/*!
*	\brief 생성자
*
*	\param minx 경계 상자의 최소점의 x 좌표
*	\param miny 경계 상자의 최소점의 y 좌표
*	\param minz 경계 상자의 최소점의 z 좌표
*	\param maxx 경계 상자의 최대점의 x 좌표
*	\param maxy 경계 상자의 최대점의 y 좌표
*	\param maxz 경계 상자의 최대점의 z 좌표
*/
EgBox::EgBox(double minx, double miny, double minz, double maxx, double maxy, double maxz)
{
	m_pMesh = NULL;
	m_MinPt.Set(minx, miny, minz);
	m_MaxPt.Set(maxx, maxy, maxz);
}

/*!
*	\brief 복사 생성자
*
*	\param cpy 복사될 객체
*/
EgBox::EgBox(const EgBox &cpy)
{
	m_pMesh = NULL;
	m_MinPt = cpy.m_MinPt;
	m_MaxPt = cpy.m_MaxPt;
}

/*!
*	\brief 대입 연산자
*
*	\param rhs 대입될 객체
*
*	\return 대입된 자신을 반환한다.
*/
EgBox &EgBox::operator =(const EgBox &rhs)
{
	m_pMesh = NULL;
	m_MinPt = rhs.m_MinPt;
	m_MaxPt = rhs.m_MaxPt;

	return *this;
}

/*!
*	\brief 월드 좌표계에서 표현된 경계 상자의 모델링 좌표계를 반환한다.
*
*	\return 월드 좌표계에서 표현된 경계 상자의 모델링 좌표계를 반환한다.
*/
EgTrans EgBox::GetMC()
{
	EgVec3 t;
	EgQuat q;
	EgPos p = ::lerp(m_MinPt, m_MaxPt, 0.5);
	if (m_pMesh != NULL)
	{
		t = cast_vec3(m_pMesh->m_MC * p);
		q = m_pMesh->m_MC.m_Q;
	}
	else
		t = cast_vec3(p);

	return EgTrans(t, q);
}

/*!
*	\brief 경계상자의 x축 방향의 너비를 반환한다.
*/
double EgBox::GetWidth()
{
	return ABS(m_MaxPt[0] - m_MinPt[0]);
}

/*!
*	\brief 경계상자의 y축 방향의 높이를 반환한다.
*/
double EgBox::GetHeight()
{
	return ABS(m_MaxPt[1] - m_MinPt[1]);
}

/*!
*	\brief 경계상자의 z축 방향의 깊이를 반환한다.
*/
double EgBox::GetDepth()
{
	return ABS(m_MaxPt[2] - m_MinPt[2]);
}

/*!
*	\brief 경계상자의 대각선의 길이를 반환한다.
*/
double EgBox::GetDiagonal()
{
	return dist(m_MinPt, m_MaxPt);
}

/*!
*	\brief 경계상자의 8개의 모서리 정점을 반환한다.
*	\note
*
*           (2) *------------* (3)
*             + |          + |
*           +   |        +   |
*     (6) *------------* (7) |
*         |     |      |     |
*         | (0) *------|-----* (1)
*         |   +        |   +
*         | +          | +
*     (4) *------------* (5)
*
*	\param Pt 경계상자의 8개의 모서리 위치가 저장된다.
*	\param bLocal 모서리 정점의 좌표계(true: 모델링 좌표계, false: 월드 좌표계)
*/
void EgBox::GetCornerPts(EgPos *Pt, bool bLocal)
{
	Pt[0] = m_MinPt;
	Pt[1] = EgPos(m_MaxPt[0], m_MinPt[1], m_MinPt[2]);
	Pt[2] = EgPos(m_MinPt[0], m_MaxPt[1], m_MinPt[2]);
	Pt[3] = EgPos(m_MaxPt[0], m_MaxPt[1], m_MinPt[2]);

	Pt[4] = EgPos(m_MinPt[0], m_MinPt[1], m_MaxPt[2]);
	Pt[5] = EgPos(m_MaxPt[0], m_MinPt[1], m_MaxPt[2]);
	Pt[6] = EgPos(m_MinPt[0], m_MaxPt[1], m_MaxPt[2]);
	Pt[7] = m_MaxPt;

	if (!bLocal)
	{
		for (int i = 0; i < 8; ++i)
			Pt[i] = m_pMesh->m_MC * Pt[i];
	}
}

/*!
*	\brief 경계 상자를 렌더링 한다.
*
*	\param r 경계 상자 테두리의 빨간색 성분
*	\param g 경계 상자 테두리의 초록색 성분
*	\param b 경계 상자 테두리의 파란색 성분
*	\param width 경계 상자 테두리의 두께
*/
void EgBox::Render(double r, double g, double b, double width)
{
	// 현재 좌표계: 월드(장면)
	glPushMatrix();
	{
		// 경계 상자의 모델링 좌표계로 변환
		glMultMatrixd(cast_arr(cast_mat(m_pMesh->m_MC)));

		EgPos p = m_MinPt;
		EgPos q = m_MaxPt;
		double dx = (q[0] - p[0]) * 0.25;
		double dy = (q[1] - p[1]) * 0.25;
		double dz = (q[2] - p[2]) * 0.25;

		glDisable(GL_LIGHTING);
		glColor3d(r, g, b);
		glLineWidth((float)width);
		glBegin(GL_LINES);
		{
			glVertex3d(p[0], p[1], p[2]);
			glVertex3d(p[0] + dx, p[1], p[2]);
			glVertex3d(p[0], p[1], p[2]);
			glVertex3d(p[0], p[1] + dy, p[2]);
			glVertex3d(p[0], p[1], p[2]);
			glVertex3d(p[0], p[1], p[2] + dz);

			glVertex3d(p[0], p[1], q[2]);
			glVertex3d(p[0] + dx, p[1], q[2]);
			glVertex3d(p[0], p[1], q[2]);
			glVertex3d(p[0], p[1] + dy, q[2]);
			glVertex3d(p[0], p[1], q[2]);
			glVertex3d(p[0], p[1], q[2] - dz);

			glVertex3d(q[0], p[1], q[2]);
			glVertex3d(q[0] - dx, p[1], q[2]);
			glVertex3d(q[0], p[1], q[2]);
			glVertex3d(q[0], p[1] + dy, q[2]);
			glVertex3d(q[0], p[1], q[2]);
			glVertex3d(q[0], p[1], q[2] - dz);

			glVertex3d(q[0], p[1], p[2]);
			glVertex3d(q[0] - dx, p[1], p[2]);
			glVertex3d(q[0], p[1], p[2]);
			glVertex3d(q[0], p[1] + dy, p[2]);
			glVertex3d(q[0], p[1], p[2]);
			glVertex3d(q[0], p[1], p[2] + dz);

			glVertex3d(q[0], q[1], q[2]);
			glVertex3d(q[0] - dx, q[1], q[2]);
			glVertex3d(q[0], q[1], q[2]);
			glVertex3d(q[0], q[1] - dy, q[2]);
			glVertex3d(q[0], q[1], q[2]);
			glVertex3d(q[0], q[1], q[2] - dz);

			glVertex3d(q[0], q[1], p[2]);
			glVertex3d(q[0] - dx, q[1], p[2]);
			glVertex3d(q[0], q[1], p[2]);
			glVertex3d(q[0], q[1] - dy, p[2]);
			glVertex3d(q[0], q[1], p[2]);
			glVertex3d(q[0], q[1], p[2] + dz);

			glVertex3d(p[0], q[1], p[2]);
			glVertex3d(p[0] + dx, q[1], p[2]);
			glVertex3d(p[0], q[1], p[2]);
			glVertex3d(p[0], q[1] - dy, p[2]);
			glVertex3d(p[0], q[1], p[2]);
			glVertex3d(p[0], q[1], p[2] + dz);

			glVertex3d(p[0], q[1], q[2]);
			glVertex3d(p[0] + dx, q[1], q[2]);
			glVertex3d(p[0], q[1], q[2]);
			glVertex3d(p[0], q[1] - dy, q[2]);
			glVertex3d(p[0], q[1], q[2]);
			glVertex3d(p[0], q[1], q[2] - dz);
			glEnd();
		}

		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glLineWidth(1.0f);
	}
	glPopMatrix();
}

/*!
*	\brief 객체의 클래스 이름을 반환한다.
*
*	\return 객체의 클래스 이름을 반환한다.
*/
std::string EgMesh::GetClassName() 
{ 
	return std::string("EgMesh");
}

/*!
*	\brief 자신을 복사한다.
*
*	\return 자신을 복사한 객체의 포인터를 반환한다.
*/
EgMesh *EgMesh::CopyObject() 
{ 
	return new EgMesh(*this); 
}

/*!
*	\brief 객체가 포함된 메쉬의 포인터를 반환한다.
*
*	\return 객체가 포함된 메쉬의 포인터를 반환한다.
*/
EgMesh *EgMesh::GetMesh()
{
	return this;
}

/*!
*	\brief 월드 좌표계에서 표현된 객체의 모델링 좌표계를 반환한다.
*
*	\return 월드 좌표계에서 표현된 객체의 모델링 좌표계를 반환한다.
*/
EgTrans EgMesh::GetTrans()
{
	return m_MC;
}

/*!
*	\brief 선택된 객체를 편집한다.
*
*	\param EC 편집 툴의 좌표계
*	\param t 편집될 객체의 이동 변위
*	\param q 편집될 객체의 회전 변위
*	\param s 편집될 객체의 스케일 변위
*	\param bFirst 편집 객체들 중 첫 번째 객체에 대한 편집 여부
*/
void EgMesh::Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst)
{
	// 메쉬의 모델링 좌표계를 편집한다.
	EgTrans EC2MC = inv(EC) * m_MC;
	m_MC = EC * EgTrans(t, q) * EC2MC;

	// 크기 변환을 수행한다면,
	if (s != EgVec3(1.0, 1.0, 1.0))
	{
		// 메쉬의 각 정점에 대하여 크기 변환을 수행하고,
		for (EgVertex *v : m_pVerts)
		{
			v->m_Pos[0] *= s[0];
			v->m_Pos[1] *= s[1];
			v->m_Pos[2] *= s[2];
		}

		// 경계 상자에 대해서도 크기 변환을 수행한다.
		m_BndBox.m_MinPt[0] *= s[0];
		m_BndBox.m_MinPt[1] *= s[1];
		m_BndBox.m_MinPt[2] *= s[2];
		m_BndBox.m_MaxPt[0] *= s[0];
		m_BndBox.m_MaxPt[1] *= s[1];
		m_BndBox.m_MaxPt[2] *= s[2];

		// 정점의 위치가 변경되어 버퍼를 갱신한다.
		UpdateVertexBuf();
	}
}

/*!
*	\brief	메쉬의 모델링 좌표계를 변경한다.
*
*	\param RefType 변경될 모델링 좌표계의 타입(0: 월드 좌표계, 1: AABB, 2: OBB)
*/
void EgMesh::UpdateMC(int RefType)
{
	if (RefType == 0)
	{
		// 월드 좌표계에서 표현된 정점의 좌표를 구하고,
		for (EgVertex *v : m_pVerts)
			v->m_Pos = m_MC * v->m_Pos;
		
		// 모델링 좌표계를 월드 좌표계로 설정하고,
		m_MC = EgTrans();
	}
	else if (RefType == 1)
	{
		// 월드 좌표계를 모델링 좌표계로 설정하고,
		UpdateMC(0);

		// 경계 상자의 중심의 좌표를 구하고,
		EgPos p = m_BndBox.m_MinPt + (m_BndBox.m_MaxPt - m_BndBox.m_MinPt) * 0.5;

		// 메쉬의 모델링 좌표계를 설정한다.
		m_MC = EgTrans(cast_vec3(p), EgQuat());

		// 새로운 모델링 좌표계에서 표현된 정점의 좌표를 계산한다.
		EgTrans invMC = inv(m_MC);
		for (EgVertex *v : m_pVerts)
			v->m_Pos = invMC * v->m_Pos;
	}
	else if (RefType == 2)
	{
		// 주성분 분석을 위한 변수를 선언하고,
		int NumVert = GetNumVert();
		EgMat Data(NumVert, 3), PCs(3, 3);
		EgVec Sigma(3), Mean(3);

		// 각각의 정점에 대하여,
		int r = 0;
		for (EgVertex *v : m_pVerts)
		{
			// 월드 좌표계에서 표현된 정점의 좌표를 구하고,
			v->m_Pos = m_MC * v->m_Pos;

			// 행 벡터 형태로 행렬에 추가한 후,
			Data.SetRowVec(r++, cast_vec(v->m_Pos));
		}

		// 주성분 분석을 수행하여
		pca(Data, PCs, Sigma, Mean);

		// 모델링 좌표계를 설정한다.
		EgVec3 origin = cast_vec3(Mean);
		EgVec3 x_axis = cast_vec3(PCs.GetColVec(0)).Normalize();
		EgVec3 y_axis = cast_vec3(PCs.GetColVec(1)).Normalize();
		EgQuat q(x_axis, y_axis);
		m_MC = EgTrans(origin, q);

		// 메쉬의 모델링 좌표계에서 표현된 정점의 좌표를 계산한다.
		EgTrans invMC = inv(m_MC);
		for (EgVertex *v : m_pVerts)
			v->m_Pos = invMC * v->m_Pos;
	}

	// 정점 버퍼와 경계 상자를 갱신힌다.
	UpdateVertexBuf();
	UpdateBndBox();
}

/*!
*	\brief	메쉬가 수정된 내용에 따라 메쉬를 갱신한다.
*/
void EgMesh::UpdateMesh()
{
}

/*!
*	\brief 선택된 객체를 렌더링 한다.
*
*	\param r 색상의 red 성분
*	\param g 색상의 green 성분
*	\param b 색상의 blue 성분
*/
void EgMesh::RenderForEdit(float r, float g, float b)
{
	m_BndBox.Render(r, g, b, 1.0);
}

/*!
*	\brief 생성자
*
*	\param Name 생성할 메쉬의 이름
*/
EgMesh::EgMesh(std::string Name)
{
	m_Name = Name;
	m_MC = EgTrans();
	m_BndBox.m_pMesh = this;
	m_RenderType = (RENDER_POLYGON | RENDER_BACK_FACE);
	m_ShadeType = SHADE_BY_LIGHT;
	m_DisplayListOfFaceId = 0;
	m_DisplayListOfBndry = 0;
}

/*!
*	\brief 복사 생성자
*
*	\param cpy 복사될 메쉬 객체
*/
EgMesh::EgMesh(const EgMesh &cpy)
{
	// 이름과 모델링 좌표계를 복사한다.
	m_Name = cpy.m_Name + "_copy";
	m_MC = cpy.m_MC;

	// 정점의 리스트를 복사한다.
	for (EgVertex *v : cpy.m_pVerts)
		AddVertex(new EgVertex(v->m_Pos));

	// 텍스처 정점의 리스트를 복사한다.
	for (EgTexel *t : cpy.m_pTexels)
		AddTexel(new EgTexel(t->m_UV[0], t->m_UV[1]));

	// 법선의 정보를 복사한다.
	for (EgNormal *n : cpy.m_pNormals)
		AddNormal(new EgNormal(n->m_Dir));

	// 메쉬에 포함된 텍스처를 복사한다.
	for (EgTexture *t : cpy.m_pTextures)
	{
		EgTexture *pTexture = ::create_texture(t->m_FileName);
		if (pTexture != NULL)
			AddTexture(pTexture);
	}

	// 메쉬에 포함된 재질을 복사한다.
	for (EgMaterial *m : cpy.m_pMaterials)
	{
		// 재질을 복사하여 메쉬에 추가하고,
		EgMaterial *pMtl = new EgMaterial(*m);
		AddMaterial(pMtl);

		// 재질에 텍스처를 연결한다.
		if (m->m_pTexture != NULL)
			pMtl->m_pTexture = GetTexture(m->m_pTexture->m_FileName.c_str());
	}
	
	// 삼각형을 복사한다.
	for (EgFace *f : cpy.m_pFaces)
	{
		int *vidx = f->GetVertexIdxs();
		int *tidx = f->GetTexelIdxs();
		int *nidx = f->GetNormalIdxs();
		EgMaterial *pMtl = NULL;
		if (f->m_pMaterial != NULL)
			pMtl = GetMaterial(f->m_pMaterial->m_Name);

		if (tidx == NULL)
		{
			AddFace(new EgFace(
				m_pVerts[vidx[0]], m_pVerts[vidx[1]], m_pVerts[vidx[2]], 
				NULL, NULL, NULL, 
				m_pNormals[nidx[0]], m_pNormals[nidx[1]], m_pNormals[nidx[2]], 
				pMtl, f->m_GroupName));
		}
		else
		{
			AddFace(new EgFace(
				m_pVerts[vidx[0]], m_pVerts[vidx[1]], m_pVerts[vidx[2]],
				m_pTexels[tidx[0]], m_pTexels[tidx[1]], m_pTexels[tidx[2]],
				m_pNormals[nidx[0]], m_pNormals[nidx[1]], m_pNormals[nidx[2]],
				pMtl, f->m_GroupName));
		}		
	}
	
	// 에지의 연결관계를 구성하고 
	InitEdgeMate();

	// 렌더링 속성을 초기화 하고
	m_RenderType = cpy.m_RenderType;
	m_ShadeType = cpy.m_ShadeType;
	m_VertexBuf.clear();
	m_NormalBuf.clear();
	m_TexelBuf.clear();

	// 경계 상자를 복사하고
	m_BndBox = cpy.m_BndBox;
	m_BndBox.m_pMesh = this;

	// 디스플레이 리스트를 초기화 한다.
	m_DisplayListOfFaceId = 0;
	m_DisplayListOfBndry = 0;
}

/*!
*	\brief 소멸자
*/
EgMesh::~EgMesh()
{
	// 제거되는 메쉬의 이름을 출력한다.
	printf("%s is deleted...\n\n", m_Name.c_str());

	// 정점, 텍셀, 노말, 삼각형, 재질, 텍스처 리스트를 제거한다.
	for (EgVertex *v : m_pVerts)
		delete v;
	for (EgTexel *t : m_pTexels)
		delete t;
	for (EgNormal *n : m_pNormals)
		delete n;
	for (EgFace *f : m_pFaces)
		delete f;
	for (EgMaterial *m : m_pMaterials)
		delete m;
	for (EgTexture *tex : m_pTextures)
		delete tex;

	// 디스플레이 리스트를 제거한다. 
	if (m_DisplayListOfFaceId != 0)
		glDeleteLists(m_DisplayListOfFaceId, 1);
	if (m_DisplayListOfBndry != 0)
		glDeleteLists(m_DisplayListOfBndry, 1);
}

/*!
*	\brief 메쉬에 정점을 추가한다.
*
*	\param pVert 추가할 정점에 대한 포인터[in]
*/
void EgMesh::AddVertex(EgVertex *pVert)
{
	// 정점의 인덱스를 설정하고 정점 배열에 추가한다.
	pVert->m_pMesh = this;
	pVert->m_Idx = (int)m_pVerts.size();
	m_pVerts.push_back(pVert);
}

/*!
*	\brief 메쉬에 텍셀을 추가한다.
*
*	\param pTexel 추가할 텍셀에 대한 포인터[in]
*/
void EgMesh::AddTexel(EgTexel *pTexel)
{
	// 텍설의 인덱스를 설정하고 텍셀 배열에 추가한다.
	pTexel->m_Idx = (int)m_pTexels.size();
	m_pTexels.push_back(pTexel);
}

/*!
*	\brief 메쉬에 법선을 추가한다.
*
*	\param pNormal 추가할 법선에 대한 포인터[in]
*/
void EgMesh::AddNormal(EgNormal *pNormal)
{
	// 법선의 인덱스를 설정하고 법선 배열에 추가한다.
	pNormal->m_Idx = (int)m_pNormals.size();
	m_pNormals.push_back(pNormal);
}

/*!
*	\brief 메쉬에 삼각형을 추가한다.
*
*	\param pFace 추가할 삼각형에 대한 포인터[in]
*/
void EgMesh::AddFace(EgFace *pFace)
{
	// 삼각형의 인덱스를 설정하고 삼각형 배열에 추가한다.
	pFace->m_Idx = (int)m_pFaces.size();
	m_pFaces.push_back(pFace);
}

/*!
*	\brief 메쉬에 재질을 추가한다.
*
*	\param pMaterial 추가할 재질에 대한 포인터[in]
*/
void EgMesh::AddMaterial(EgMaterial *pMaterial)
{
	// 재질의 인덱스를 설정하고 재질 배열에 추가한다.
	pMaterial->m_Idx = (int)m_pMaterials.size();
	m_pMaterials.push_back(pMaterial);
}

/*!
*	\brief 메쉬에 텍스처를 추가한다.
*
*	\param pTexture 추가할 텍스처에 대한 포인터[in]
*/
void EgMesh::AddTexture(EgTexture *pTexture)
{
	m_pTextures.push_back(pTexture);
}

/*!
*	\brief	메쉬의 삼각형(3개의 하프에지 포함)을 제거한다.
*	\note	삼각형 제거 과정에서 생긴 사용되지 않는 정점/텍셀/법선은 제거되지 않는다.
*			남은 정점/텍셀/법선을 제거하기 위해서는 delete_unused_*() 함수를 추가로 호출해야 한다.
*
*	\param	Faces 제거할 삼각형의 포인터 리스트
*
*	\return 남은 삼각형의 개수를 반환한다.
*/
int EgMesh::DeleteFaces(std::vector<EgFace *> &Faces)
{
	// 각각의 삼각형에 대하여
	for (EgFace *f : Faces)
	{
		for (int i = 0; i < 3; ++i)
		{
			// 각각의 에지에 대하여
			EgEdge *e = f->GetEdge(i);

			// 경계 에지가 아니라면 반대편 에지의 정보를 갱신하고,
			if (!e->IsBndry())
				e->m_pMate->m_pMate = NULL;

			// 시작 정점의 에지 집합에서 현재 에지를 제거한다.
			e->m_pVert->DeleteEdge(e);
		}

		// 제거할 삼각형임을 기록한다.
		f->m_Idx = -1;
	}

	// 새로운 삼각형 리스트를 정의하고
	std::vector<EgFace *> NewFaces;

	// 각각의 기존 삼각형에 대하여
	int fidx = 0;
	for (EgFace *f : m_pFaces)
	{
		if (f->m_Idx == -1) // 제거할 삼각형이라면 제거한다.
			delete f;
		else				// 남은 삼각형은 새로운 리스트에 추가한다.
		{
			f->m_Idx = fidx++;
			NewFaces.push_back(f);			
		}
	}

	// 삼각형의 리스트를 갱신한다.
	m_pFaces = NewFaces;

	// 렌더링 버퍼를 초기화 한다.
	m_VertexBuf.clear();
	m_NormalBuf.clear();
	m_TexelBuf.clear();

	// 남은 삼각형의 개수를 반환한다.
	return (int)m_pFaces.size();
}

/*!
*	\brief	메쉬 정점들을 제거한다.
*	\note	정점이 포함된 삼각형들도 함께 제거된다.
*			정점 제거 과정에서 생긴 사용되지 않는 텍셀/법선은 제거되지 않는다.
*			남은 텍셀/법선을 제거하기 위해서는 delete_unused_*() 함수를 추가로 호출해야 한다.
*
*	\param Verts 제거할 정점의 포인터 리스트
*
*	\return 남은 정점의 개수를 반환한다.
*/
int EgMesh::DeleteVerts(std::vector<EgVertex *> &Verts)
{
	// 정점이 포함된 삼각형 리스트를 정의한다.
	std::vector<EgFace *> Faces;

	// 정점을 공유하는 모든 삼각형을 구한다.
	for (EgVertex *v : Verts)
	{
		// 제거할 정점임을 기록한다.
		v->m_Idx = -1;

		// 정점을 공유한 모든 삼각형을 구한다.
		std::vector<EgFace *> tmp;
		v->GetOneRingFace(tmp);
		for (EgFace *f : tmp)
			Faces.push_back(f);
	}

	// 중복된 삼각형 객체를 제거한다.
	sort(Faces.begin(), Faces.end());
	Faces.erase(unique(Faces.begin(), Faces.end()), Faces.end());

	// 각각의 삼각형을 제거한다.
	DeleteFaces(Faces);

	// 사용되는 정점으로 구성될 새로운 리스트를 정의하고,
	std::vector<EgVertex *> NewVerts;

	// 각각의 정점에 대하여
	for (EgVertex *v : m_pVerts)
	{
		if (v->m_Idx == -1) // 제거할 정점은 제거하고
			delete v;
		else // 사용된 정점이라면 리스트에 추가한다.
			NewVerts.push_back(v);
	}

	// 정점의 리스트를 업데이트 한다.
	m_pVerts = NewVerts;
	int NumVert = UpdateVertIdx();

	// 남은 정점의 수를 반환한다.
	return NumVert;
}

/*!
*	\brief	메쉬의 에지를 제거한다.
*	\note	에지가 포함된 삼각형들도 함께 제거된다.
*			에지 제거 과정에서 생긴 사용되지 않는 정점/텍셀/법선은 제거되지 않는다.
*			남은 정점/텍셀/법선을 제거하기 위해서는 delete_unused_*() 함수를 추가로 호출해야 한다.
*
*	\param	pEdge 제거할 에지의 포인터 배열
*
*	\return 남은 삼각형의 수를 반환한다.
*/
int EgMesh::DeleteEdges(std::vector<EgEdge *> &Edges)
{
	// 각각의 에지가 포함된 삼각형 집합을 구한다.
	std::vector<EgFace *> Faces;
	for (EgEdge *e : Edges)
		Faces.push_back(e->m_pFace);

	// 중복된 객체를 제거한다.
	sort(Faces.begin(), Faces.end());
	Faces.erase(unique(Faces.begin(), Faces.end()), Faces.end());

	// 삼각형을 제거한다.
	return DeleteFaces(Faces);
}

/*!
*	\brief 메쉬 정점의 인덱스(0, 1, 2, ...)를 갱신한다.
*
*	\return 정점의 개수를 반환한다.
*/
int EgMesh::UpdateVertIdx()
{
	int vidx = 0;
	for (EgVertex *v : m_pVerts)
		v->m_Idx = vidx++;

	return vidx;
}

/*!
*	\brief 메쉬에서 텍셀의 인덱스를 갱신한다
*
*	\return 텍셀의 개수를 반환한다.
*/
int EgMesh::UpdateTexIdx()
{
	int tidx = 0;
	for (EgTexel *t : m_pTexels)
		t->m_Idx = tidx++;

	return tidx;
}

/*!
*	\brief 메쉬에서 법선의 인덱스를 갱신한다
*
*	\return 법선의 개수를 반환한다.
*/
int EgMesh::UpdateNormalIdx()
{
	int nidx = 0;
	for (EgNormal *n : m_pNormals)
		n->m_Idx = nidx++;

	return nidx;
}

/*!
*	\brief 메쉬에서 삼각형의 인덱스를 갱신한다.
*
*	\return 삼각형의 개수를 반환한다.
*/
int EgMesh::UpdateFaceIdx()
{
	int fidx = 0;
	for (EgFace *f : m_pFaces)
		f->m_Idx = fidx++;

	return fidx;
}

/*!
*	\brief 메쉬에서 재질의 인덱스를 갱신한다.
*
*	\return 재질의 개수를 반환한다.
*/
int EgMesh::UpdateMaterialIdx()
{
	int midx = 0;
	for (EgMaterial *m : m_pMaterials)
		m->m_Idx = midx++;

	return midx;
}

/*!
*	\brief	메쉬의 반대편 에지(Mate) 정보를 구성한다.
*/
void EgMesh::InitEdgeMate()
{
	// 메쉬의 모든 에지의 메이트 정보를 초기화 한다.
	for (EgFace *f : m_pFaces)
	{
		f->GetEdge(0)->m_pMate = NULL;
		f->GetEdge(1)->m_pMate = NULL;
		f->GetEdge(2)->m_pMate = NULL;
	}

	// 각각의 정점에 대하여
	for (EgVertex *v : m_pVerts)
	{
		// 정점에서 시작하는 각각의 에지에 대하여
		for (EgEdge *e1 : v->m_pEdges)
		{
			// 에지의 다음 정점에서 시작하는 각각의 에지에 대하여
			for (EgEdge *e2 : e1->m_pNext->m_pVert->m_pEdges)
			{
				// e2가 e1의 mate 에지라면
				if (IS_MATE_EDGE(e1, e2))
				{
					// mate 정보를 설정한다.
					e1->m_pMate = e2;
					e2->m_pMate = e1;
					break;
				}
			}
		}
	}
}

/*!
*	\brief 정점에서 시작하는 에지의 반대편 에지 정보를 갱신한다.

*	\param pVert 정점의 포인터
*/
void EgMesh::UpdateEdgeMate(EgVertex *pVert)
{
	// 정점에서 시작하는 각각의 에지 e1에 대하여
	for (EgEdge *e1 : pVert->m_pEdges)
	{
		// 다음 에지의 정점에서 시작하는 각각의 에지 e2에 대하여
		for (EgEdge *e2 : e1->m_pNext->m_pVert->m_pEdges)
		{
			// e1과 e2가 mate 관계라면
			if (IS_MATE_EDGE(e1, e2))
			{
				// mate 정보를 설정한다.
				e1->m_pMate = e2;
				e2->m_pMate = e1;
				break;
			}
		}
	}
}

/*!
*	\brief	메쉬가 삼각형 법선을 사용하고 있는지 조사한다.
*
*	\return	삼각형 법선을 사용하면 true, 아니면 false를 반환한다.
*/
bool EgMesh::IsFaceNormal()
{
	if (m_pNormals.size() == m_pFaces.size())
		return true;
	return false;
}

/*!
*	\brief	기존 법선 배열(m_pNormal)을 제거하고, 새로운 m_pNormal을 구성한다.
*
*	\param	NormalType 법선의 형태를 지정한다(ASIS: 기존 형태, FLAT: 삼각형 법선, SMOOTH: 정점 법선)
*/
void EgMesh::UpdateNormal(TypeNormal NormalType)
{
	// 기존 법선을 사용한다면,
	if (NormalType == ASIS)
		NormalType = (GetNumNormal() == GetNumFace()) ? FLAT : SMOOTH;

	// 기존의 법선 리스트를 제거한다.
	for (EgNormal *n : m_pNormals)
		delete n;
	m_pNormals.clear();

	// 삼각형 법선을 사용한다면
	if (NormalType == FLAT)
	{
		// 각각의 삼각형에 대하여
		for (EgFace *f : m_pFaces)
		{
			// 법선을 생성하여 리스트에 추가한다.
			EgNormal *pNormal = new EgNormal();
			AddNormal(pNormal);

			// 각각의 에지에 삼각형 법선을 할당한다.
			pNormal->m_Dir = f->GetFaceNormal(true);
			f->GetEdge(0)->m_pNormal = pNormal;
			f->GetEdge(1)->m_pNormal = pNormal;
			f->GetEdge(2)->m_pNormal = pNormal;
		}
	}
	else  // 정점 법선을 사용한다면
	{
		// 각각의 정점에 대하여
		for (EgVertex *v : m_pVerts)
		{
			// 정점 법선을 생성하고 리스트에 추가한다.
			EgNormal *pNormal = new EgNormal();
			AddNormal(pNormal);

			// 정점에서 시작하는 각각의 에지에 대하여 정점 법선을 할당한다.
			for (EgEdge *e : v->m_pEdges)
				e->m_pNormal = pNormal;
		}

		// 각각의 삼각형에 대하여
		for (EgFace *f : m_pFaces)
		{
			// 삼각형의 법선을 계산하여, 각각의 에지의 법선에 누적한다.
			EgVec3 N = f->GetFaceNormal(true);
			f->GetEdge(0)->m_pNormal->m_Dir += N;
			f->GetEdge(1)->m_pNormal->m_Dir += N;
			f->GetEdge(2)->m_pNormal->m_Dir += N;
		}

		// 누적된 법선을 정규화 한다.
		for (EgNormal *n : m_pNormals)
			n->m_Dir.Normalize();
	}

	// 법선 버퍼를 초기화 한다.
	m_NormalBuf.clear();
}

/*!
*	\brief 메쉬의 크기를 대각선 기준으로 정규화 한다.
*/
void EgMesh::Normalize()
{
	EgVec3 p(1000000.0, 1000000.0, 1000000.0);
	EgVec3 q(-1000000.0, -1000000.0, -1000000.0);
	for (EgVertex *v : m_pVerts)
	{
		EgPos r = v->m_Pos;
		p[0] = MIN(p[0], r[0]);
		p[1] = MIN(p[1], r[1]);
		p[2] = MIN(p[2], r[2]);
		q[0] = MAX(q[0], r[0]);
		q[1] = MAX(q[1], r[1]);
		q[2] = MAX(q[2], r[2]);
	}

	// 경계 상자 대각선의 길이를 초기화 한다.
	double size = norm(q - p);
	for (EgVertex *v : m_pVerts)
	{
		v->m_Pos[0] = v->m_Pos[0] / size;
		v->m_Pos[1] = v->m_Pos[1] / size;
		v->m_Pos[2] = v->m_Pos[2] / size;
	}
}

/*!
*	\brief 메쉬의 정점의 개수를 반환한다.
*
*	\return 메쉬의 정점의 개수를 반환한다.
*/
int EgMesh::GetNumVert()
{
	return (int)m_pVerts.size();
}

/*!
*	\brief 메쉬의 텍셀의 개수를 반환한다.
*
*	\return 메쉬의 텍셀의 개수를 반환한다.
*/
int EgMesh::GetNumTexel()
{
	return (int)m_pTexels.size();
}

/*!
*	\brief 메쉬의 법선의 개수를 반환한다.
*
*	\return 메쉬의 법선의 개수를 반환한다.
*/
int EgMesh::GetNumNormal()
{
	return (int)m_pNormals.size();
}

/*!
*	\brief 메쉬의 삼각형의 개수를 반환한다.
*
*	\return 메쉬의 삼각형의 개수를 반환한다.
*/
int EgMesh::GetNumFace()
{
	return (int)m_pFaces.size();
}

/*!
*	\brief 메쉬의 재질의 개수를 반환한다.
*
*	\return 메쉬의 재질의 개수를 반환한다.
*/
int EgMesh::GetNumMtrl()
{
	return (int)m_pMaterials.size();
}

/*!
*	\brief 메쉬를 구성하는 에지의 최소 길이를 구한다.
*
*	\return 에지의 최소 길이를 반환한다.
*/
double EgMesh::GetMinEdgeLen()
{
	double len = 1.0e+10;
	for (EgFace *f : m_pFaces)
		for (int i = 0; i < 3; ++i)
			len = MIN(len, f->GetEdge(i)->GetLength());
	return len;
}

/*!
*	\brief 메쉬를 구성하는 에지의 최대 길이를 구한다.
*
*	\return 에지의 최대 길이를 반환한다.
*/
double EgMesh::GetMaxEdgeLen()
{
	double len = 1.0e-10;
	for (EgFace *f : m_pFaces)
		for (int i = 0; i < 3; ++i)
			len = MAX(len, f->GetEdge(i)->GetLength());
	return len;
}

/*!
*	\brief 메쉬를 구성하는 에지의 평균 길이를 구한다.
*
*	\return 에지의 평균 길이를 반환한다.
*/
double EgMesh::GetAvgEdgeLen()
{
	double len = 0.0;
	for (EgFace *f : m_pFaces)
		len += f->GetPerimeter();

	return len / (GetNumFace() * 3.0);
}

/*!
*	\brief 메쉬의 Idx번째 정점의 포인터를 찾는다.
*
*	\param Idx 정점의 인덱스(0부터 시작)
*
*	\return 메쉬의 Idx번째 정점의 포인터를 반환한다.
*/
EgVertex *EgMesh::GetVert(int Idx)
{
	if (m_pVerts.empty() || Idx < 0 || Idx > m_pVerts.size() - 1)
		return NULL;

	return m_pVerts[Idx];
}

/*!
*	\brief 메쉬의 Idx번째 텍셀의 포인터를 찾는다.
*
*	\param Idx 텍셀의 인덱스(0부터 시작)
*
*	\return 메쉬의 Idx번째 텍셀의 포인터를 반환한다.
*/
EgTexel *EgMesh::GetTexel(int Idx)
{
	if (m_pTexels.empty() || Idx < 0 || Idx > m_pTexels.size() - 1)
		return NULL;

	return m_pTexels[Idx];
}

/*!
*	\brief 메쉬의 Idx번째 법선의 포인터를 찾는다.
*
*	\param Idx 법선의 인덱스(0부터 시작)
*
*	\return 메쉬의 Idx번째 법선의 포인터를 반환한다.
*/
EgNormal *EgMesh::GetNormal(int Idx)
{
	if (m_pNormals.empty() || Idx < 0 || Idx > m_pNormals.size() - 1)
		return NULL;

	return m_pNormals[Idx];
}

/*!
*	\brief 메쉬의 Idx번째 삼각형의 포인터를 찾는다.
*
*	\param Idx 삼각형의 인덱스(0부터 시작)
*
*	\return 메쉬의 Idx번째 삼각형의 포인터를 반환한다.
*/
EgFace *EgMesh::GetFace(int Idx)
{
	if (m_pFaces.empty() || Idx < 0 || Idx > m_pFaces.size() - 1)
		return NULL;

	return m_pFaces[Idx];
}

/*!
*	\brief Idx 번째 재질을 찾는다.
*
*	\param Idx 재질의 인덱스
*
*	\return Idx 번째 재질의 포인터를 반환한다.
*/
EgMaterial *EgMesh::GetMaterial(int Idx)
{
	if (m_pMaterials.empty() || Idx < 0 || Idx > m_pMaterials.size() - 1)
		return NULL;

	return m_pMaterials[Idx];
}

/*!
*	\brief 주어진 이름을 갖는 재질을 찾는다.
*
*	\param Name 재질의 이름
*
*	\return 주어진 이름을 갖는 재질의 포인터를 반환한다.
*/
EgMaterial *EgMesh::GetMaterial(std::string Name)
{
	for (EgMaterial *m : m_pMaterials)
	{
		if (m->m_Name == Name)
			return m;
	}

	return NULL;
}

/*!
*	\brief 주어진 이름을 갖는 텍스처를 찾는다.
*
*	\param Name 텍스처 파일명
*
*	\return 주어진 이름을 갖는 텍스처의 포인터를 반환한다.
*/
EgTexture *EgMesh::GetTexture(std::string Name)
{
	for (EgTexture *t : m_pTextures)
	{
		if (t->m_FileName == Name)
			return t;
	}

	return NULL;
}

/*!
*	\brief 메쉬의 텍스처 사용 여부를 조사한다.
*
*	\return 메쉬가 텍스처를 사용하면 true, 아니면 false를 반환한다.
*/
bool EgMesh::HasTexture()
{
	if (!m_pTexels.empty())
		return true;
	else
		return false;
}

/*!
*	\brief	메쉬의 삼각형에 새로운 재질을 설정한다.
*
*	\param	pMtl 설정한 재질에 대한 포인터
*/
void EgMesh::SetMaterial(EgMaterial *pMtl)
{
	for (EgFace *f : m_pFaces)
		f->m_pMaterial = pMtl;

	// 정점 버퍼를 초기화 한다.
	m_VertexBuf.clear();
}

/*!
*	\brief	메쉬 정점의 색상을 가우스 곡률을 이용하여 설정한다.
*
*	\param	Ks[in] 정점별 계산된 가우스 곡률
*	\param	Range[in] 가시화(사용)할 가우스 곡률의 범위(0 ~ 1.0)
*/
void EgMesh::SetVertColorByGaussCurvature(std::vector<double> &Ks, double Range)
{
	// 메쉬의 쉐이딩을 정점의 색으로 설정한다.
	m_ShadeType = SHADE_BY_VERT_COLOR;

	// 양수 곡률과 음수 곡률을 구분하여 저장한다.
	std::vector<double> Pos, Neg;
	for (int i = 0; i < NUM(Ks); ++i)
	{
		if (Ks[i] > 0.0)
			Pos.push_back(Ks[i]);
		else
			Neg.push_back(Ks[i]);
	}

	// 곡률을 오름차순으로 정렬한다.
	sort(Pos.begin(), Pos.end());
	sort(Neg.begin(), Neg.end());

	// 곡률에서 입력 범위의 최소값과 최대값을 구하고 출력한다. 
	double NegMin = (Neg.empty()) ? 0.0 : Neg[(int)((Neg.size() - 1) * (1.0 - Range))];
	double PosMax = (Pos.empty()) ? 0.0 : Pos[(int)((Pos.size() - 1) * Range)];
	printf("\tSelected range = %.2lf%%\n", Range * 100);
	printf("\tColor mapping range = [%.10lf(Red) ~ %.10lf(Green)]\n", NegMin, 0.0);
	printf("\tColor mapping range = [%.10lf(Green) ~ %.10lf(Blue)]\n\n", 0.0, PosMax);

	// 각각의 정점에 대하여
	for (size_t i = 0; i < Ks.size(); ++i)
	{
		// 음수 곡률은 Red ~ Green으로 색상을 맵핑하여,
		if (Ks[i] < 0.0)
			m_pVerts[i]->m_Color = get_color_map(Ks[i], NegMin, 0.0, EgVec3(1.0, 0.0, 0.0), EgVec3(0.0, 1.0, 0.0));			
		else // 양수 곡률은 Green ~ Blue으로 색상을 맵핑하고,
			m_pVerts[i]->m_Color = get_color_map(Ks[i], 0.0, PosMax, EgVec3(0.0, 1.0, 0.0), EgVec3(0.0, 0.0, 1.0));			
	}
}

/*!
*	\brief	메쉬 정점의 색상을 평균 곡률의 절대값을 이용하여 설정한다.
*
*	\param	Hs[in] 정점별 계산된 평균 곡률의 절대값
*	\param	Range[in] 가시화(사용)할 평균 곡률의 범위(0 ~ 1.0)
*/
void EgMesh::SetVertColorByMeanCurvature(std::vector<double> &Hs, double Range)
{
	// 메쉬의 쉐이딩을 정점의 색으로 설정한다.
	m_ShadeType = SHADE_BY_VERT_COLOR;

	// 평균 곡률 배열을 복사하여 오름 차순으로 정렬한다.
	std::vector<double> Hs_Copy = Hs;
	sort(Hs_Copy.begin(), Hs_Copy.end());

	// 곡률에서 입력 범위의 최소값과 최대값을 구하고 출력한다. 
	double Max = (Hs_Copy.empty()) ? 0.0 : Hs_Copy[(int)((Hs_Copy.size() - 1) * Range)];
	printf("\tSelected range = %.2lf%%\n", Range * 100);
	printf("\tColor mapping range = [%.10lf(Red) ~ %.10lf(Blue)]\n\n", 0.0, Max);

	// 각각의 정점에 대하여
	for (size_t i = 0; i < Hs.size(); ++i)
		m_pVerts[i]->m_Color = get_color_map(Hs[i], 0.0, Max, EgVec3(1.0, 0.0, 0.0), EgVec3(0.0, 1.0, 0.0), EgVec3(0.0, 0.0, 1.0));
}

/*!
*	\brief	메쉬 정점의 색상을 하모닉 필드를 이용하여 설정한다.
*
*	\param	Fs 정점별 계산된 하모닉 필드의 값
*	\param	bRainbow 무지개 칼라 맵 사용여부
*/
void EgMesh::SetVertColorByHarmonicField(std::vector<double> &Fs, bool bRainbow)
{
	// 메쉬의 쉐이딩을 정점의 색으로 설정한다.
	m_ShadeType = SHADE_BY_VERT_COLOR;

	int NumVert = GetNumVert();
	for (int i = 0; i < NumVert; ++i)
	{
		if (bRainbow)
			m_pVerts[i]->m_Color = ::get_color_map_from_rainbow(Fs[i]);
		else
			m_pVerts[i]->m_Color = ::get_color_map(Fs[i], 0.0, 1.0, EgVec3(0.0, 0.0, 1.0), EgVec3(0.0, 1.0, 0.0), EgVec3(1.0, 0.0, 0.0));
	}
}

/*!
*	\brief	메쉬 정점의 색상을 법선을 이용하여 설정한다.
*/
void EgMesh::SetVertColorByNormal()
{
	// 메쉬의 쉐이딩을 정점의 색으로 설정한다.
	m_ShadeType = SHADE_BY_VERT_COLOR;
	for (EgVertex *v : m_pVerts)
	{
		EgEdge *e = v->m_pEdges[0];
		double r = ABS(e->m_pNormal->m_Dir[0]);
		double g = ABS(e->m_pNormal->m_Dir[1]);
		double b = ABS(e->m_pNormal->m_Dir[2]);
		v->m_Color.Set(r, g, b);
	}
}

/*!
*	\brief	메쉬 정점의 색상을 디폴트 조명 모델을 이용하여 설정한다.
*/
void EgMesh::SetVertColorByLight()
{
	m_ShadeType = SHADE_BY_LIGHT;
}

/*!
*	\brief 메쉬를 렌더링 한다.
*/
void EgMesh::Render()
{
	// 삼각형 후면 렌더링을 한다면
	if (m_RenderType & RENDER_BACK_FACE)
	{
		glDisable(GL_CULL_FACE);
	}
	else // 삼각형 후면 렌더링을 하지 않으면
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	// 월드 좌표계에서
	glPushMatrix();
	{
		// 객체의 모델링 좌표계로의 변환하고,
		glMultMatrixd(cast_arr(cast_mat(m_MC)));

		// 폴리곤을 렌더링 한다면
		if (m_RenderType & RENDER_POLYGON)
		{
			// 조명 모드라면 조명 모델로 렌더링 하고
			if (m_ShadeType == SHADE_BY_LIGHT)
				RenderPolygon();

			// 텍스처 모드라면 텍스처 맵핑을 하고
			if (m_ShadeType == SHADE_BY_TEXTURE)
				RenderTexture();

			// 정점의 색상 모드라면 정점의 색으로 렌더링한다.
			if (m_ShadeType == SHADE_BY_VERT_COLOR)
				RenderVertColor();
		}

		// 추가적으로 포인트를 렌더링을 한다.
		if (m_RenderType & RENDER_POINT)
			RenderPoint();

		// 추가적으로 와이어프레임을 렌더링 한다.
		if (m_RenderType & RENDER_WIRE)
			RenderWire();

		// 추가적으로 메쉬의 경계(홀)을 렌더링 한다.
		if (m_DisplayListOfBndry != 0)
			glCallList(m_DisplayListOfBndry);
	}
	glPopMatrix();
}

/*!
*	\brief 메쉬를 삼각형으로 렌더링 한다.
*/
void EgMesh::RenderPolygon()
{
	// 정점과 법선 버퍼를 재생성 한다.
	if (m_VertexBuf.empty())
		UpdateVertexBuf();
	if (m_NormalBuf.empty())
		UpdateNormalBuf();

	// 폴리곤 오프셋 활성화 하고,
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	// 정점과 노말 버퍼를 지정하고,
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	// 재질에 따른 폴리곤 그룹을 렌더링 한다.
	static EgMaterial Mtl("default");
	std::map<std::string, std::vector<float>>::iterator it0 = m_VertexBuf.begin();
	std::map<std::string, std::vector<float>>::iterator it1 = m_NormalBuf.begin();
	for (; it0 != m_VertexBuf.end(); it0++, it1++)
	{
		// 재질을 활성화 하여
		EgMaterial *pMtl = GetMaterial(it0->first);
		if (pMtl == NULL)
			pMtl = &Mtl;
		pMtl->Enable();

		// 정점, 법선 배열을 지정하여 삼각형으로 렌더링 하고
		glVertexPointer(3, GL_FLOAT, 0, &(it0->second[0]));
		glNormalPointer(GL_FLOAT, 0, &(it1->second[0]));
		glDrawArrays(GL_TRIANGLES, 0, (int)it0->second.size() / 3);

		// 재질을 비활성화 한다.
		pMtl->Disable();
	}

	// 정점과 노말 버퍼를 비활성화 한다.
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	// 폴리곤 오프셋을 비활성화 한다.
	glDisable(GL_POLYGON_OFFSET_FILL);
}

/*!
*	\brief 메쉬를 텍스처로 렌더링 한다.
*/
void EgMesh::RenderTexture()
{
	// 정점, 법선, 텍스처 버퍼를 재생성 한다.
	if (m_VertexBuf.empty())
		UpdateVertexBuf();
	if (m_NormalBuf.empty())
		UpdateNormalBuf();
	if (m_TexelBuf.empty())
		UpdateTexelBuf();

	// 텍스처 매핑 옵션을 설정하고
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// 폴리곤 오프셋 활성화 하고
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	// 정점, 법선, 텍스처 버퍼를 활성화 한다
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// 재질에 따른 폴리곤 그룹을 렌더링 한다.
	static EgMaterial Mtl("default");
	std::map<std::string, std::vector<float>>::iterator it0 = m_VertexBuf.begin();
	std::map<std::string, std::vector<float>>::iterator it1 = m_NormalBuf.begin();
	std::map<std::string, std::vector<float>>::iterator it2 = m_TexelBuf.begin();
	for (; it0 != m_VertexBuf.end(); it0++, it1++, it2++)
	{
		// 재질을 활성화 하여
		EgMaterial *pMtl = GetMaterial(it0->first);
		if (pMtl == NULL)
			pMtl = &Mtl;
		pMtl->Enable();
		
		if (pMtl->m_pTexture != NULL)
			glBindTexture(GL_TEXTURE_2D, pMtl->m_pTexture->m_TexId);

		// 정점, 법선 배열을 지정하여 삼각형으로 렌더링 하고
		glVertexPointer(3, GL_FLOAT, 0, &(it0->second[0]));
		glNormalPointer(GL_FLOAT, 0, &(it1->second[0]));
		glTexCoordPointer(2, GL_FLOAT, 0, &(it2->second[0]));
		glDrawArrays(GL_TRIANGLES, 0, (int)it0->second.size() / 3);

		// 재질을 비활성화 한다.
		pMtl->Disable();
	}

	// 정점과 노말 버퍼를 해제한다.
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// 폴리곤 오프셋을 비활성화 한다.
	glDisable(GL_POLYGON_OFFSET_FILL);

	// 텍스처 기능을 비활성화 한다.
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

/*!
*	\brief 정점의 색상으로 모델을 렌더링한다.
*/
void EgMesh::RenderVertColor()
{
	// 조명을 비활성화 한다.
	glDisable(GL_LIGHTING);

	// 폴리곤 오프셋 활성화 하고,
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	// 각각의 삼각형을 렌더링 한다.
	glBegin(GL_TRIANGLES);
	{
		for (EgFace *f : m_pFaces)
		{
			glColor3dv(f->GetVertex(0)->m_Color.m_Coords);
			glVertex3dv(f->GetVertex(0)->m_Pos.m_Coords);

			glColor3dv(f->GetVertex(1)->m_Color.m_Coords);
			glVertex3dv(f->GetVertex(1)->m_Pos.m_Coords);

			glColor3dv(f->GetVertex(2)->m_Color.m_Coords);
			glVertex3dv(f->GetVertex(2)->m_Pos.m_Coords);
		}
	}
	glEnd();

	// 조명을 다시 활성화 한다.
	glPointSize(1.0f);
	glEnable(GL_LIGHTING);

	// 폴리곤 오프셋을 비활성화 한다.
	glDisable(GL_POLYGON_OFFSET_FILL);
}

/*!
*	\brief 메쉬의 정점을 렌더링 한다.
*/
void EgMesh::RenderPoint()
{
	// 조명을 비활성화 하고 점의 색상과 크기를 설정한다.
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(3.0f);

	// 각각의 정점을 렌더링 한다.
	glBegin(GL_POINTS);
	{
		for (EgVertex *v : m_pVerts)
			glVertex3dv(v->m_Pos.m_Coords);
	}
	glEnd();

	// 점의 크기를 초기화 하고 조명을 다시 활성화 한다.
	glPointSize(1.0f);
	glEnable(GL_LIGHTING);
}

/*!
*	\brief 메쉬를 와이어프레임으로 렌더링 한다.
*/
void EgMesh::RenderWire()
{
	// 정점과 법선 버퍼를 재생성 한다.
	if (m_VertexBuf.empty())
		UpdateVertexBuf();

	// 폴리곤 렌더링 모드를 라인으로 지정하고, 조명을 끄고, 색상을 지정한 후,
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_LIGHTING);
	//glColor4f(227 / 255.0f, 220 / 255.0f, 193 / 255.0f, 1.0f);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glLineWidth(1.0f);

	// 정점 버퍼를 활성화 하고
	glEnableClientState(GL_VERTEX_ARRAY);

	// 재질에 따른 폴리곤 그룹을 렌더링 한다.
	std::map<std::string, std::vector<float>>::iterator it;
	for (it = m_VertexBuf.begin(); it != m_VertexBuf.end(); it++)
	{
		// 정점 배열을 지정하여 삼각형으로 렌더링 한다.
		glVertexPointer(3, GL_FLOAT, 0, &(it->second[0]));
		glDrawArrays(GL_TRIANGLES, 0, (int)it->second.size() / 3);
	}

	// 정점 버퍼를 비활성화 한다
	glDisableClientState(GL_VERTEX_ARRAY);

	// 폴리곤 렌더링 모드를 재설정하고, 조명을 활성화 한다.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
}

/*!
*	\brief 메쉬의 경계를 렌더링하는 디스플레이 리스트를 생성한다.
*/
void EgMesh::CreateDisplayListOfBndry()
{
	// 기존 디스플레이 리스트를 제거한다.
	DeleteDisplayListOfBndry();	
	
	// 경계 정점의 리스트를 구하여,
	std::vector<std::vector<EgVertex *>> BndryVerts;

	// Non-manifold 정점을 포함하거나, 에지의 홀이 불명확 하다면
	if (::get_bndry_verts(this, BndryVerts, false, true) < 0)
	{
		// 새로운 디스플레이 리스트를 생성한다.
		m_DisplayListOfBndry = glGenLists(1);
		glNewList(m_DisplayListOfBndry, GL_COMPILE_AND_EXECUTE);
		{
			// 조명을 비활성화 하고 선의 색상과 두께를 설정한다.
			glDisable(GL_LIGHTING);
			glColor3f(1.0f, 0.0f, 0.0f);
			glLineWidth(2.5);

			// 경계 에지를 렌더링 한다.
			glBegin(GL_LINES);
			for (int i = 0; i < (int)BndryVerts[0].size(); i += 2)
			{
				glVertex3dv(BndryVerts[0][i]->m_Pos.m_Coords);
				glVertex3dv(BndryVerts[0][i + 1]->m_Pos.m_Coords);
			}
			glEnd();

			// 선의 두께를 초기화 하고 조명을 다시 활성화 한다.
			glLineWidth(1.0);
			glEnable(GL_LIGHTING);
		}
		glEndList();

	}
	else // 여러개의 홀이 존재한다면 각각의 경계(홀)을 서로 다른 색상으로 렌더링 한다.
	{
		// 새로운 디스플레이 리스트를 생성한다.
		int cidx = 0;
		m_DisplayListOfBndry = glGenLists(1);
		glNewList(m_DisplayListOfBndry, GL_COMPILE_AND_EXECUTE);
		{
			// 조명을 비활성화 하고 선의 색상과 두께를 설정한다.
			glDisable(GL_LIGHTING);
			glLineWidth(2.5);

			for (std::vector<EgVertex *> &Bndry : BndryVerts)
			{
				EgVec3 C = ::get_color_sequence(cidx++);
				glColor3dv(C.m_Coords);
				glBegin(GL_LINE_LOOP);
				for (EgVertex *v : Bndry)
					glVertex3dv(v->m_Pos.m_Coords);
				glEnd();
			}

			// 선의 두께를 초기화 하고 조명을 다시 활성화 한다.
			glLineWidth(1.0);
			glEnable(GL_LIGHTING);

		}
		glEndList();
	}
}

/*!
*	\brief 메쉬의 경계를 렌더링하는 디스플레이 리스트를 제거한다.
*/
void EgMesh::DeleteDisplayListOfBndry()
{
	if (m_DisplayListOfBndry != 0)
		glDeleteLists(m_DisplayListOfBndry, 1);
	m_DisplayListOfBndry = 0;
}

/*!
*	\brief 정점 버퍼를 갱신한다.
*/
void EgMesh::UpdateVertexBuf()
{
	// 기존의 정점 버퍼를 초기화 한다.
	m_VertexBuf.clear();

	// 각각의 삼각형에 대하여
	for (EgFace *f : m_pFaces)
	{
		// 재질명을 구하여
		std::string MtlName = (f->m_pMaterial == NULL) ? "default" : f->m_pMaterial->m_Name;

		// 삼각형의 세 정점의 좌표를 재질명에 따라 구분하여 정점 버퍼에 저장한다.
		for (int i = 0; i < 3; ++i)
		{
			EgVertex *v = f->GetVertex(i);
			m_VertexBuf[MtlName].push_back((float)v->m_Pos[0]);
			m_VertexBuf[MtlName].push_back((float)v->m_Pos[1]);
			m_VertexBuf[MtlName].push_back((float)v->m_Pos[2]);
		}
	}

	// 기존 디스플레이 리스트를 제거한다.
	if (m_DisplayListOfFaceId != 0)
	{
		glDeleteLists(m_DisplayListOfFaceId, 1);
		m_DisplayListOfFaceId = 0;
	}
	if (m_DisplayListOfBndry != 0)
	{
		glDeleteLists(m_DisplayListOfBndry, 1);
		m_DisplayListOfBndry = 0;
	}
}

/*!
*	\brief 법선 버퍼를 갱신한다.
*/
void EgMesh::UpdateNormalBuf()
{
	// 기존의 법선 버퍼를 초기화 한다.
	m_NormalBuf.clear();

	// 각각의 삼각형에 대하여
	for (EgFace *f : m_pFaces)
	{
		// 재질명을 구하여
		std::string MtlName = (f->m_pMaterial == NULL) ? "default" : f->m_pMaterial->m_Name;

		// 삼각형의 세 법선의 좌표를 재질명에 따라 구분하여 법선 버퍼에 저장한다.
		for (int i = 0; i < 3; ++i)
		{
			EgNormal *n = f->GetNormal(i);
			m_NormalBuf[MtlName].push_back((float)n->m_Dir[0]);
			m_NormalBuf[MtlName].push_back((float)n->m_Dir[1]);
			m_NormalBuf[MtlName].push_back((float)n->m_Dir[2]);
		}
	}
}

/*!
*	\brief 텍스처 버퍼를 갱신한다.
*/
void EgMesh::UpdateTexelBuf()
{
	// 텍스처 좌표가 없다면 리턴한다.
	if (m_pTexels.empty())
		return;

	// 기존 텍스처 버퍼를 초기화 한다.
	m_TexelBuf.clear();

	// 각각의 삼각형에 대하여
	for (EgFace *f : m_pFaces)
	{
		// 재질명을 구하여
		std::string MtlName = (f->m_pMaterial == NULL) ? "default" : f->m_pMaterial->m_Name;

		// 삼각형의 세 텍스처 좌표를 재질명에 따라 구분하여 텍스처 버퍼에 저장한다.
		for (int i = 0; i < 3; ++i)
		{
			EgTexel *t = f->GetTexel(i);
			m_TexelBuf[MtlName].push_back((float)t->m_UV[0]);
			m_TexelBuf[MtlName].push_back((float)t->m_UV[1]);
		}
	}
}

/*!
*	\brief 메쉬를 둘러싸는 경계 상자를 갱신한다.
*/
void EgMesh::UpdateBndBox()
{
	// 메쉬 정점이 없다면 리턴한다.
	if (GetNumVert() == 0)
	{
		m_BndBox.m_MinPt = m_BndBox.m_MaxPt = EgPos(0.0, 0.0, 0.0);
		return;
	}

	// 메쉬 정점의 각 축에 대한 최대/최소 좌표를 구한다.
	m_BndBox.m_MinPt = m_BndBox.m_MaxPt = m_pVerts[0]->m_Pos;
	for (EgVertex *v : m_pVerts)
	{
		m_BndBox.m_MinPt[0] = MIN(m_BndBox.m_MinPt[0], v->m_Pos[0]);
		m_BndBox.m_MinPt[1] = MIN(m_BndBox.m_MinPt[1], v->m_Pos[1]);
		m_BndBox.m_MinPt[2] = MIN(m_BndBox.m_MinPt[2], v->m_Pos[2]);

		m_BndBox.m_MaxPt[0] = MAX(m_BndBox.m_MaxPt[0], v->m_Pos[0]);
		m_BndBox.m_MaxPt[1] = MAX(m_BndBox.m_MaxPt[1], v->m_Pos[1]);
		m_BndBox.m_MaxPt[2] = MAX(m_BndBox.m_MaxPt[2], v->m_Pos[2]);
	}
}

/*!
*	\biref	폴리 라인(Polyline)의 각 정점의 곡률을 계산한다.
*	\note	마지막 수정일: 2020-08-01
*	\note	양 끝점의 곡률은 0으로 정한다.
*
*	\param	Pts[in]					폴리 라인의 정점 리시트
*	\param	Ks[out]					각 정점의 곡률이 저장된다.
*	\param	bSignedCurvature[in]	곡률 부호를 고려하면 true, 아니면 false
*	\param	Up[in]					폴리 라인이 놓인 평면의 법선 벡터
*/
void get_curvature(std::vector<EgPos> &Pts, std::vector<double> &Ks, bool bSignedCurvature, EgVec3 Up)
{
	int NumPt = NUM(Pts);
	Ks.assign(NumPt, 0.0);
	for (int i = 1; i < NumPt - 1; ++i)
	{
		EgPos p = Pts[i - 1];
		EgPos q = Pts[i];
		EgPos r = Pts[i + 1];

		EgVec3 d = q - p;
		EgVec3 e = r - q;
		double nd = norm(d);
		double ne = norm(e);
		Ks[i] = norm(2.0 / (nd + ne) * (e / ne - d / nd));

		if (bSignedCurvature)
		{
			double theta = angle(d, e, Up);
			Ks[i] = (theta < 180.0) ? Ks[i] : -Ks[i];
		}
	}
}

/*!
*	\brief	정점을 중심으로 주어진 반경 이내의 삼각형의 면적을 계산한다.
*
*	\param	pVert[in]	중심 정점에 대한 포인터
*	\param	Radius[in]	반경
*
*	\return	구해진 면적을 반환한다.
*/
double get_local_area(EgVertex *pVert, double Radius)
{
	// 측지 거리장을 이용하여 국소 영역을 구한다.
	std::vector<double> Distances;
	EgMesh *pMesh = pVert->GetMesh();
	::get_geodesic_dist_field(pMesh, pVert, Distances, Radius);

	// 국소 영역 내의 삼각형의 면적을 구하여 누적한다.
	double area = 0.0;
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 삼각형 세 점의 iso-value를 구하여
		int *vidx = f->GetVertexIdxs();
		double d0 = Distances[vidx[0]];
		double d1 = Distances[vidx[1]];
		double d2 = Distances[vidx[2]];

		// 반경 외의 삼각형이라면 스킵한다.
		if (d0 >= Radius && d1 >= Radius && d2 >= Radius)
			continue;

		// 반경 내의 삼각형이라면 전체 면적을 더한다.
		if (d0 <= Radius && d1 <= Radius && d2 <= Radius)
		{
			area += f->GetArea();
			continue;
		}

		// 삼각형이 경계에 걸친 경우, 삼각형 세 정점의 좌표를 구하고
		EgPos v0 = pMesh->m_pVerts[vidx[0]]->m_Pos;
		EgPos v1 = pMesh->m_pVerts[vidx[1]]->m_Pos;
		EgPos v2 = pMesh->m_pVerts[vidx[2]]->m_Pos;

		// 경계가 v0v1, v0v2 에지를 지나는 경우
		if (d0 < Radius && d1 > Radius && d2 > Radius)
		{
			double s = (Radius - d0) / (d1 - d0);
			double t = (Radius - d0) / (d2 - d0);
			EgVec3 a = s * (v1 - v0);
			EgVec3 b = t * (v2 - v0);
			area += norm(a ^ b) * 0.5;
			continue;
		}
		if (d1 < Radius && d2 < Radius && d0 > Radius)
		{
			double s = (Radius - d2) / (d0 - d2);
			double t = (Radius - d1) / (d0 - d1);
			EgVec3 a = (1 - s) * (v2 - v0);
			EgVec3 b = (1 - t) * (v1 - v0);
			area += f->GetArea();
			area -= norm(b ^ a) * 0.5;
			continue;
		}

		// 경계가 v1v0, v1v2 에지를 지나는 경우
		if (d1 < Radius && d2 > Radius && d0 > Radius)
		{
			double s = (Radius - d1) / (d2 - d1);
			double t = (Radius - d1) / (d0 - d1);
			EgVec3 a = s * (v2 - v1);
			EgVec3 b = t * (v0 - v1);
			area += norm(a ^ b) * 0.5;
			continue;
		}
		if (d2 < Radius && d0 < Radius && d1 > Radius)
		{
			double s = (Radius - d0) / (d1 - d0);
			double t = (Radius - d2) / (d1 - d2);
			EgVec3 a = (1 - s) * (v0 - v1);
			EgVec3 b = (1 - t) * (v2 - v1);
			area += f->GetArea();
			area -= norm(b ^ a) * 0.5;
			continue;
		}

		// 경계가 v2v0, v2v1 에지를 지나는 경우
		if (d2 < Radius && d0 > Radius && d1 > Radius)
		{
			double s = (Radius - d2) / (d0 - d2);
			double t = (Radius - d2) / (d1 - d2);
			EgVec3 a = s * (v0 - v2);
			EgVec3 b = t * (v1 - v2);
			area += norm(a ^ b) * 0.5;
			continue;
		}
		if (d0 < Radius && d1 < Radius && d2 > Radius)
		{
			double s = (Radius - d1) / (d2 - d1);
			double t = (Radius - d0) / (d2 - d0);
			EgVec3 a = (1 - s) * (v1 - v2);
			EgVec3 b = (1 - t) * (v0 - v2);
			area += f->GetArea();
			area -= norm(b ^ a) * 0.5;
			continue;
		}
	}

	// 누적된 삼각형의 면적을 반환한다.
	return area;
}
