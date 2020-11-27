#pragma once

#include <vector>
#include <list>
#include <map>
#include "../EgMath/egmath.h"
#include "../GL/freeglut.h"
//#include "../GL/glut.h"
#include "../IL/ilut.h"
#include "../Eigen/Eigenvalues"
#include "../Eigen/Sparse"
#include "egmesh_macro.h"
#include "egmesh_create.h"
#include "egmesh_query.h"
#include "egmesh_path.h"
#include "egmesh_cut.h"
#include "egmesh_util.h"
#include "egmesh_io.h"

// 클래스 선언
class EgObject;
class EgVertex;
class EgTexel;
class EgNormal;
class EgEdge;
class EgFace;
class EgFacePt;
class EgTexture;
class EgMaterial;
class EgBox;
class EgMesh;

/*!
*	\class EgObject
*	\brief 편집 가능한 객체의 기본 클래스
*
*	\author 윤승현(shyun@dongguk.edu)
*	\date 01 Jan 2001
*/
class EgObject
{
public:
	// 가상함수 재정의
	virtual std::string GetClassName();
	virtual	EgMesh *GetMesh();
	virtual EgTrans GetTrans();
	virtual void Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst);
	virtual void RenderForEdit(float r, float g, float b);

public:
	// 생성자 및 소멸자
	EgObject() {};
	virtual ~EgObject() {};
};

/*!
*	\class EgVertex
*	\brief 메쉬를 구성하는 정점을 표현하는 클래스
*
*	\author 박정호, 윤승현
*	\date 25 Jan 2018
*/
class EgVertex : public EgObject
{
public:
	// 가상함수 재정의
	virtual std::string GetClassName();
	virtual	EgMesh *GetMesh();
	virtual EgTrans GetTrans();
	virtual void Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst);
	virtual void RenderForEdit(float r, float g, float b);

public:
	/*! \brief 정점이 포함된 메쉬의 포인터 */
	EgMesh *m_pMesh;

	/*! \brief 정점의 인덱스 */
	int m_Idx;

	/*! \brief 정점의 3차원 좌표 */
	EgPos m_Pos;

	/*! \brief 정점의 색상(곡률 등의 가시화에 사용), 하모닉 필드값 */
	EgVec3 m_Color;

	/*! \brief 정점의 편집 가중치 */
	float m_EditWgt;

	/*! \brief 정점에서 시작하는 에지들에 대한 포인터 배열 */
	std::vector<EgEdge *> m_pEdges;

public:
	// 생성자 및 소멸자
	EgVertex(double x = 0.0, double y = 0.0, double z = 0.0);
	EgVertex(EgPos Pos);
	virtual ~EgVertex();

	// 위상 정보 관련 함수들
	int GetDegree();
	int GetOneRingVert(std::vector<EgVertex *> &Verts);
	int GetOneRingFace(std::vector<EgFace *> &Faces);
	int GetOneRingBndryEdge(std::vector<EgEdge *> &Edges);	
	bool IsBndry();
	bool IsConcave();
	EgEdge *IsAdjacent(EgVertex *v);
	EgVec3 GetAvgNormal();
	EgPos GetOneRingVertCenter();
	EgPos Perturbate(double rate);
	int DeleteEdge(EgEdge *e);	
};

/*!
*	\class EgTexel
*	\brief 메쉬의 텍스처 정점을 표현하는 클래스
*
*	\author 박정호, 윤승현
*	\date 27 Jun 2018
*/
class EgTexel
{
public:
	/*! \brief 텍스처 정점의 인덱스 */
	int m_Idx;

	/*! \brief 텍스처 좌표 */
	EgVec2 m_UV;

public:
	// 생성자 및 소멸자
	EgTexel(double u = 0.0, double v = 0.0);
	virtual ~EgTexel();
};

/*!
*	\class EgNormal
*	\brief 메쉬의 법선 정보를 표현하는 클래스
*
*	\author 박정호, 윤승현
*	\date 27 Jun 2018
*/
class EgNormal
{
public:
	/*! \brief 법선의 인덱스 */
	int m_Idx;

	/*! \brief 법선의 방향 */
	EgVec3 m_Dir;

public:
	// 생성자 및 소멸자
	EgNormal(double nx = 0.0, double ny = 0.0, double nz = 0.0);
	EgNormal(EgVec3 Dir);
	virtual ~EgNormal();
};

/*!
*	\class EgEdge
*	\brief 삼각형을 구성하는 방향성 에지(Half Edge)를 표현하는 클래스
*
*	\author 박정호, 윤승현
*	\date 25 Jan 2018
*/
class EgEdge
{
public:
	/*! \brief 에지의 시작 정점에 대한 포인터 */
	EgVertex *m_pVert;

	/*! \brief 에지의 시작점에서 텍셀에 대한 포인터 */
	EgTexel *m_pTexel;

	/*! \brief 에지의 시작점에서 법선에 대한 포인터 */
	EgNormal *m_pNormal;

	/*! \brief 삼각형을 구성하는 다음 에지에 대한 포인터 */
	EgEdge *m_pNext;

	/*! \brief 인접한 삼각형의 반대편 에지에 대한 포인터 */
	EgEdge *m_pMate;

	/*! \brief 에지가 포함된 삼각형에 대한 포인터 */
	EgFace *m_pFace;

	/*! \brief 에지에 부여된 비용(가중치, 사용 여부) 등의 정보, 미사용 에지 = -1.0 */
	double m_Cost;

public:
	// 생성자 및 소멸자
	EgEdge(EgVertex *pVert, EgTexel *pTexel, EgNormal *pNormal);
	virtual ~EgEdge();

	// 멤버함수
	int	GetIncidentFace(std::vector<EgFace *> &Faces);
	bool IsBndry();
	bool IsTexBndry();
	bool IsCollapsible();
	double GetAngle(bool bRadian);
	double GetCotangent();
	double GetLength();
	double GetNormalCurvature();
};

/*!
*	\class EgFace
*	\brief 메쉬의 삼각형을 표현하는 클래스
*
*	\author 박정호, 윤승현
*	\date 25 Jan 2018
*/
class EgFace : public EgObject
{
public:
	// 가상함수 재정의
	virtual std::string GetClassName();
	virtual	EgMesh *GetMesh();
	virtual EgTrans GetTrans();
	virtual void Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst);
	virtual void RenderForEdit(float r, float g, float b);

public:
	/*! \brief 삼각형의 인덱스 */
	int m_Idx;

	/*! \brief 삼각형의 시작 에지에 대한 포인터 */
	EgEdge *m_pEdge;

	/*! \brief 삼각형이 사용하는 재질에 대한 포인터 */
	EgMaterial *m_pMaterial;

	/*! \brief 삼각형이 속한 그룹의 이름 */
	std::string m_GroupName;

public:
	// 생성자 및 소멸자
	EgFace(EgEdge *e0, EgEdge *e1, EgEdge *e2, EgMaterial *pMtl, std::string GroupName);
	EgFace(EgVertex *v0, EgVertex *v1, EgVertex *v2, 
		   EgTexel *t0, EgTexel *t1, EgTexel *t2, 
		   EgNormal *n0, EgNormal *n1, EgNormal *n2,
		   EgMaterial *pMtl, std::string GroupName);
	virtual ~EgFace();

	// 멤버 함수
	EgPos GetVertexPos(int vidx);
	EgVertex *GetVertex(int vidx);		
	EgVec2 GetTexelCoord(int tidx);
	EgTexel *GetTexel(int tidx);	
	EgNormal *GetNormal(int nidx);
	EgVec3 GetNormalDir(int nidx, bool bLocal);	
	EgVec3 GetFaceNormal(bool bLocal);
	EgEdge *GetEdge(int eidx);
	int *GetVertexIdxs();
	int *GetTexelIdxs();
	int *GetNormalIdxs();
	int GetEdgeIdx(EgEdge *pEdge);
	double GetArea();
	double GetPerimeter();
	EgPos GetCOG(bool bLocal);
	int GetAdjacentFace(std::vector<EgFace *> &Faces);
	bool IsBndryFace();
};

/*!
*	\class EgFacePt
*	\brief 메쉬 삼각형 내부의 점의 위치를 표현하는 클래스
*
*	\author 윤승현
*	\date 8 Dec. 2019
*/
class EgFacePt : public EgObject
{
public:
	// 가상함수 재정의
	virtual std::string GetClassName();
	virtual	EgMesh *GetMesh();
	virtual EgTrans GetTrans();
	virtual void Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst);
	virtual void RenderForEdit(float r, float g, float b);

public:
	/*! \biref 점이 놓인 삼각형에 대한 포인터 */
	EgFace *m_pFace;

	/*! \biref 점의 barycentric 좌표 */
	EgVec3 m_Coords;

public:
	// 생성자 및 소멸자
	EgFacePt(EgFace *pFace = NULL, EgVec3 Coords = EgVec3());
	EgFacePt(const EgFacePt &cpy);
	virtual ~EgFacePt();

	// 대입 연산자 
	EgFacePt &operator =(const EgFacePt &rhs);

	// 멤버 함수
	EgPos Pos();
	bool IsVertex();
	double Dist2Vert(int vidx);
	EgVertex *GetClosestVert();
};


/*!
*	\class EgTexture
*	\brief 메쉬가 사용하는 텍스처를 표현하는 클래스
*
*	\author 윤승현
*	\date 29 Nov. 2018
*/
class EgTexture
{
public:
	/*! brief 텍스처의 파일의 이름(확장명 포함) */
	std::string m_FileName;

	/*! brief OpenGL 텍스처가 생성된 경우 아이디(1부터 시작) */
	GLuint m_TexId;

public:
	// 생성자 및 소멸자
	EgTexture();
	~EgTexture();

	// 멤버 함수
	bool LoadTexture(const char *fname);
};

/*!
*	\class EgMaterial
*	\brief 메쉬가 사용하는 재질을 표현하는 클래스
*
*	\author 박정호, 윤승현
*	\date 2 Jul. 2018
*/
class EgMaterial
{
public:
	/*! \brief 재질의 인덱스 */
	int m_Idx;

	/*! \brief 재질의 이름 */
	std::string m_Name;

	/*! \brief 재질의 주변광 반사 계수 */
	GLfloat m_Ambient[4];

	/*! \brief 재질의 난반사 계수 */
	GLfloat m_Diffuse[4];

	/*! \brief 재질의 전반사 계수 */
	GLfloat m_Specular[4];

	/*! \brief 재질의 전반사 지수 */
	GLfloat m_Shiness;

	/*! \brief 재질이 사용하는 텍스처에 대한 포인터(텍스처는 여러 재질이 공유 할 수 있음) */
	EgTexture *m_pTexture;

public:
	// 생성자 및 소멸자
	EgMaterial(std::string Name);
	EgMaterial(const EgMaterial &cpy);
	virtual ~EgMaterial();

	// 재질 설정 함수
	void SetAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
	void SetDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
	void SetSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
	void SetShiness(GLfloat s);
	void SetTexture(EgTexture *pTexture);
	void Enable();
	void Disable();
};

/*!
*	\class	EgBox
*	\brief	3차원 장면에서 월드 좌표계의 축에 평행한 바운딩 박스를 표현하는 클래스
*	\note	마지막 수정일(21 Aug. 2016)
*	\author 윤승현(shyun@dongguk.edu)
*	\date	29 Jan. 2015
*/
class EgBox
{
public:
	// 데이터 멤버
	/*! \brief 경계 상자의 대상 메쉬 */
	EgMesh *m_pMesh;

	/*! \brief 경계 상자의 최소점(메쉬의 모델링 좌표계에서 표현) */
	EgPos m_MinPt;

	/*! \brief 경계 상자의 최대점(메쉬의 모델링 좌표계에서 표현) */
	EgPos m_MaxPt;

public:
	// 생성자 및 소멸자
	EgBox(double minx = 0, double miny = 0, double minz = 0, double maxx = 0, double maxy = 0, double maxz = 0);
	EgBox(const EgBox &cpy);
	~EgBox() {}

	// 대입 연산자 
	EgBox &operator =(const EgBox &rhs);

	// 멤버 함수
	EgTrans GetMC();
	double GetWidth();
	double GetHeight();
	double GetDepth();
	double GetDiagonal();
	void GetCornerPts(EgPos *Pt, bool bLocal = true);

	// 렌더링 관련 함수
	void Render(double r, double g, double b, double width);
};

/*! \brief 메쉬의 추가 렌더링 타입 열거형 */
#define RENDER_POLYGON			0x0001
#define RENDER_WIRE				0x0002
#define RENDER_POINT			0x0004
#define RENDER_NORMAL			0x0008
#define RENDER_BACK_FACE		0x0010

/*! \brief 메쉬의 쉐이딩 타입 열거형 */
enum TypeShade
{
	SHADE_BY_LIGHT = 0,
	SHADE_BY_TEXTURE,
	SHADE_BY_VERT_COLOR,
};

/*! \brief 메쉬의 법선 타입 열거형 */
enum TypeNormal
{
	ASIS = 0,
	FLAT = 1,
	SMOOTH = 2,
};

/*!
*	\class EgMesh
*	\brief 삼각형으로 표현된 3차원 메쉬를 표현하는 클래스
*
*	\author 박정호, 윤승현
*	\date 25 Jan 2018
*/
class EgMesh : public EgObject
{
public:
	// 가상함수 재정의
	virtual std::string GetClassName();
	virtual EgMesh *CopyObject();
	virtual	EgMesh *GetMesh();
	virtual EgTrans GetTrans();
	virtual void Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst);
	virtual void UpdateMesh();
	virtual void UpdateMC(int type);
	virtual void RenderForEdit(float r, float g, float b);
	virtual void Render();

public:
	/*! \brief 메쉬의 이름 */
	std::string m_Name;

	/*! \brief 메쉬가 정의된 모델링 좌표계 */
	EgTrans m_MC;

	/*! \brief 정점의 포인터 배열 */
	std::vector<EgVertex *> m_pVerts;

	/*! \brief 텍스처 정점의 포인터 배열 */
	std::vector<EgTexel *> m_pTexels;

	/*! \brief 법선의 포인터 배열 */
	std::vector<EgNormal *> m_pNormals;

	/*! \brief 삼각형의 포인터 배열 */
	std::vector<EgFace *> m_pFaces;

	/*! \brief 재질의 포인터 배열 */
	std::vector<EgMaterial *> m_pMaterials;

	/*! \brief 텍스처의 포인터 배열(여러 재질이 공유 가능) */
	std::vector<EgTexture *> m_pTextures;

	/*! \brief 메쉬의 경계 상자*/
	EgBox m_BndBox;

	/*! \brief 렌더링 옵션: RENDER_POLYGON, RENDER_WIRE, ... */
	unsigned int m_RenderType;

	/*! \brief 메쉬의 쉐이딩 타입: SHADE_BY_LIGHT, SHADE_BY_TEXTURE, SHADE_BY_COLOR, ... */
	TypeShade m_ShadeType;

	/*! \brief 고속 렌더링을 위한 정점 버퍼: (재질명, 정점의 좌표 배열)로 구성됨 */
	std::map<std::string, std::vector<float>> m_VertexBuf;

	/*! \brief 고속 렌더링을 위한 법선 버퍼: (재질명, 법선의 좌표 배열)로 구성됨 */
	std::map<std::string, std::vector<float>> m_NormalBuf;

	/*! \brief 고속 렌더링을 위한 텍스처 좌표 버퍼: (재질명, 텍스처 좌표 배열)로 구성됨 */
	std::map<std::string, std::vector<float>> m_TexelBuf;

	/*! \brief 삼각형 아이디로 렌더링된 디스플레이 리스트 */
	GLuint m_DisplayListOfFaceId;

	/*! \brief 삼각형 경계(홀)로 렌더링된 디스플레이 리스트 */
	GLuint m_DisplayListOfBndry;	

public:
	// 생성자 및 소멸자
	EgMesh(std::string Name);
	EgMesh(const EgMesh &cpy);
	virtual ~EgMesh();

	// 메쉬에 구성 요소를 추가하는 함수들
	void AddVertex(EgVertex *pVert);
	void AddTexel(EgTexel *pTexel);
	void AddNormal(EgNormal *pNormal);
	void AddFace(EgFace *pFace);
	void AddMaterial(EgMaterial *pMaterial);
	void AddTexture(EgTexture *pTexture);

	// 메쉬의 구성 요소를 제거하는 함수(중요: 오작동시 비정상적인 메쉬를 생성)
	int DeleteFaces(std::vector<EgFace *> &Faces);
	int DeleteVerts(std::vector<EgVertex *> &Verts);
	int DeleteEdges(std::vector<EgEdge *> &Edges);

	// 메쉬의 정점/텍셀/법선/삼각형/재질 인덱스를 초기화하는 함수
	int UpdateVertIdx();
	int UpdateTexIdx();
	int UpdateNormalIdx();
	int UpdateFaceIdx();
	int UpdateMaterialIdx();

	// 메쉬의 하프 에지 구조를 생성하는 함수들
	void InitEdgeMate();
	void UpdateEdgeMate(EgVertex *pVert);

	// 메쉬 법선 정보를 처리하는 함수들
	bool IsFaceNormal();
	void UpdateNormal(TypeNormal NormalType = ASIS);

	// 메쉬의 크기를 정규화 하는 함수
	void Normalize();

	// 메쉬의 정보 추출 함수들
	int GetNumVert();
	int GetNumTexel();
	int GetNumNormal();
	int GetNumFace();
	int GetNumMtrl();
	double GetAvgEdgeLen();
	double GetMaxEdgeLen();
	double GetMinEdgeLen();
	EgVertex *GetVert(int Idx);
	EgTexel *GetTexel(int Idx);
	EgNormal *GetNormal(int Idx);
	EgFace *GetFace(int Idx);
	EgMaterial *GetMaterial(int Idx);
	EgMaterial *GetMaterial(std::string Name);
	EgTexture *GetTexture(std::string Name);
	bool HasTexture();

	// 메쉬 속성 설정 함수
	void SetMaterial(EgMaterial *pMtl);
	void SetVertColorByGaussCurvature(std::vector<double> &Ks, double Range);
	void SetVertColorByMeanCurvature(std::vector<double> &Hs, double Range);
	void SetVertColorByHarmonicField(std::vector<double> &Fs, bool bRainbow);
	void SetVertColorByNormal();
	void SetVertColorByLight();
		
	// 렌더링 관련 함수
	void RenderPolygon();		// 정점, 노말 버퍼 사용
	void RenderTexture();		// 정점, 노말, 텍스처 버퍼 사용
	void RenderVertColor();
	void RenderPoint();
	void RenderWire();			// 정점 버퍼 사용

	// 디스플레이 리스트 생성 및 제거 함수
	void CreateDisplayListOfBndry();
	void DeleteDisplayListOfBndry();
	
	// 렌더링 버퍼 관련 함수
	void UpdateVertexBuf();
	void UpdateNormalBuf();
	void UpdateTexelBuf();
	void UpdateBndBox();
};

/*!	\biref 폴리 라인의 곡률을 계산한다. */
void get_curvature(std::vector<EgPos> &Pts, std::vector<double> &Ks, bool bSignedCurvature, EgVec3 Up);

/*! \brief 정점을 중심으로 주어진 반경 이내의 삼각형의 면적을 계산한다. */
double get_local_area(EgVertex *pVert, double Radius);