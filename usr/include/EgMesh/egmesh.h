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

// Ŭ���� ����
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
*	\brief ���� ������ ��ü�� �⺻ Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date 01 Jan 2001
*/
class EgObject
{
public:
	// �����Լ� ������
	virtual std::string GetClassName();
	virtual	EgMesh *GetMesh();
	virtual EgTrans GetTrans();
	virtual void Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst);
	virtual void RenderForEdit(float r, float g, float b);

public:
	// ������ �� �Ҹ���
	EgObject() {};
	virtual ~EgObject() {};
};

/*!
*	\class EgVertex
*	\brief �޽��� �����ϴ� ������ ǥ���ϴ� Ŭ����
*
*	\author ����ȣ, ������
*	\date 25 Jan 2018
*/
class EgVertex : public EgObject
{
public:
	// �����Լ� ������
	virtual std::string GetClassName();
	virtual	EgMesh *GetMesh();
	virtual EgTrans GetTrans();
	virtual void Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst);
	virtual void RenderForEdit(float r, float g, float b);

public:
	/*! \brief ������ ���Ե� �޽��� ������ */
	EgMesh *m_pMesh;

	/*! \brief ������ �ε��� */
	int m_Idx;

	/*! \brief ������ 3���� ��ǥ */
	EgPos m_Pos;

	/*! \brief ������ ����(��� ���� ����ȭ�� ���), �ϸ�� �ʵ尪 */
	EgVec3 m_Color;

	/*! \brief ������ ���� ����ġ */
	float m_EditWgt;

	/*! \brief �������� �����ϴ� �����鿡 ���� ������ �迭 */
	std::vector<EgEdge *> m_pEdges;

public:
	// ������ �� �Ҹ���
	EgVertex(double x = 0.0, double y = 0.0, double z = 0.0);
	EgVertex(EgPos Pos);
	virtual ~EgVertex();

	// ���� ���� ���� �Լ���
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
*	\brief �޽��� �ؽ�ó ������ ǥ���ϴ� Ŭ����
*
*	\author ����ȣ, ������
*	\date 27 Jun 2018
*/
class EgTexel
{
public:
	/*! \brief �ؽ�ó ������ �ε��� */
	int m_Idx;

	/*! \brief �ؽ�ó ��ǥ */
	EgVec2 m_UV;

public:
	// ������ �� �Ҹ���
	EgTexel(double u = 0.0, double v = 0.0);
	virtual ~EgTexel();
};

/*!
*	\class EgNormal
*	\brief �޽��� ���� ������ ǥ���ϴ� Ŭ����
*
*	\author ����ȣ, ������
*	\date 27 Jun 2018
*/
class EgNormal
{
public:
	/*! \brief ������ �ε��� */
	int m_Idx;

	/*! \brief ������ ���� */
	EgVec3 m_Dir;

public:
	// ������ �� �Ҹ���
	EgNormal(double nx = 0.0, double ny = 0.0, double nz = 0.0);
	EgNormal(EgVec3 Dir);
	virtual ~EgNormal();
};

/*!
*	\class EgEdge
*	\brief �ﰢ���� �����ϴ� ���⼺ ����(Half Edge)�� ǥ���ϴ� Ŭ����
*
*	\author ����ȣ, ������
*	\date 25 Jan 2018
*/
class EgEdge
{
public:
	/*! \brief ������ ���� ������ ���� ������ */
	EgVertex *m_pVert;

	/*! \brief ������ ���������� �ؼ��� ���� ������ */
	EgTexel *m_pTexel;

	/*! \brief ������ ���������� ������ ���� ������ */
	EgNormal *m_pNormal;

	/*! \brief �ﰢ���� �����ϴ� ���� ������ ���� ������ */
	EgEdge *m_pNext;

	/*! \brief ������ �ﰢ���� �ݴ��� ������ ���� ������ */
	EgEdge *m_pMate;

	/*! \brief ������ ���Ե� �ﰢ���� ���� ������ */
	EgFace *m_pFace;

	/*! \brief ������ �ο��� ���(����ġ, ��� ����) ���� ����, �̻�� ���� = -1.0 */
	double m_Cost;

public:
	// ������ �� �Ҹ���
	EgEdge(EgVertex *pVert, EgTexel *pTexel, EgNormal *pNormal);
	virtual ~EgEdge();

	// ����Լ�
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
*	\brief �޽��� �ﰢ���� ǥ���ϴ� Ŭ����
*
*	\author ����ȣ, ������
*	\date 25 Jan 2018
*/
class EgFace : public EgObject
{
public:
	// �����Լ� ������
	virtual std::string GetClassName();
	virtual	EgMesh *GetMesh();
	virtual EgTrans GetTrans();
	virtual void Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst);
	virtual void RenderForEdit(float r, float g, float b);

public:
	/*! \brief �ﰢ���� �ε��� */
	int m_Idx;

	/*! \brief �ﰢ���� ���� ������ ���� ������ */
	EgEdge *m_pEdge;

	/*! \brief �ﰢ���� ����ϴ� ������ ���� ������ */
	EgMaterial *m_pMaterial;

	/*! \brief �ﰢ���� ���� �׷��� �̸� */
	std::string m_GroupName;

public:
	// ������ �� �Ҹ���
	EgFace(EgEdge *e0, EgEdge *e1, EgEdge *e2, EgMaterial *pMtl, std::string GroupName);
	EgFace(EgVertex *v0, EgVertex *v1, EgVertex *v2, 
		   EgTexel *t0, EgTexel *t1, EgTexel *t2, 
		   EgNormal *n0, EgNormal *n1, EgNormal *n2,
		   EgMaterial *pMtl, std::string GroupName);
	virtual ~EgFace();

	// ��� �Լ�
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
*	\brief �޽� �ﰢ�� ������ ���� ��ġ�� ǥ���ϴ� Ŭ����
*
*	\author ������
*	\date 8 Dec. 2019
*/
class EgFacePt : public EgObject
{
public:
	// �����Լ� ������
	virtual std::string GetClassName();
	virtual	EgMesh *GetMesh();
	virtual EgTrans GetTrans();
	virtual void Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst);
	virtual void RenderForEdit(float r, float g, float b);

public:
	/*! \biref ���� ���� �ﰢ���� ���� ������ */
	EgFace *m_pFace;

	/*! \biref ���� barycentric ��ǥ */
	EgVec3 m_Coords;

public:
	// ������ �� �Ҹ���
	EgFacePt(EgFace *pFace = NULL, EgVec3 Coords = EgVec3());
	EgFacePt(const EgFacePt &cpy);
	virtual ~EgFacePt();

	// ���� ������ 
	EgFacePt &operator =(const EgFacePt &rhs);

	// ��� �Լ�
	EgPos Pos();
	bool IsVertex();
	double Dist2Vert(int vidx);
	EgVertex *GetClosestVert();
};


/*!
*	\class EgTexture
*	\brief �޽��� ����ϴ� �ؽ�ó�� ǥ���ϴ� Ŭ����
*
*	\author ������
*	\date 29 Nov. 2018
*/
class EgTexture
{
public:
	/*! brief �ؽ�ó�� ������ �̸�(Ȯ��� ����) */
	std::string m_FileName;

	/*! brief OpenGL �ؽ�ó�� ������ ��� ���̵�(1���� ����) */
	GLuint m_TexId;

public:
	// ������ �� �Ҹ���
	EgTexture();
	~EgTexture();

	// ��� �Լ�
	bool LoadTexture(const char *fname);
};

/*!
*	\class EgMaterial
*	\brief �޽��� ����ϴ� ������ ǥ���ϴ� Ŭ����
*
*	\author ����ȣ, ������
*	\date 2 Jul. 2018
*/
class EgMaterial
{
public:
	/*! \brief ������ �ε��� */
	int m_Idx;

	/*! \brief ������ �̸� */
	std::string m_Name;

	/*! \brief ������ �ֺ��� �ݻ� ��� */
	GLfloat m_Ambient[4];

	/*! \brief ������ ���ݻ� ��� */
	GLfloat m_Diffuse[4];

	/*! \brief ������ ���ݻ� ��� */
	GLfloat m_Specular[4];

	/*! \brief ������ ���ݻ� ���� */
	GLfloat m_Shiness;

	/*! \brief ������ ����ϴ� �ؽ�ó�� ���� ������(�ؽ�ó�� ���� ������ ���� �� �� ����) */
	EgTexture *m_pTexture;

public:
	// ������ �� �Ҹ���
	EgMaterial(std::string Name);
	EgMaterial(const EgMaterial &cpy);
	virtual ~EgMaterial();

	// ���� ���� �Լ�
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
*	\brief	3���� ��鿡�� ���� ��ǥ���� �࿡ ������ �ٿ�� �ڽ��� ǥ���ϴ� Ŭ����
*	\note	������ ������(21 Aug. 2016)
*	\author ������(shyun@dongguk.edu)
*	\date	29 Jan. 2015
*/
class EgBox
{
public:
	// ������ ���
	/*! \brief ��� ������ ��� �޽� */
	EgMesh *m_pMesh;

	/*! \brief ��� ������ �ּ���(�޽��� �𵨸� ��ǥ�迡�� ǥ��) */
	EgPos m_MinPt;

	/*! \brief ��� ������ �ִ���(�޽��� �𵨸� ��ǥ�迡�� ǥ��) */
	EgPos m_MaxPt;

public:
	// ������ �� �Ҹ���
	EgBox(double minx = 0, double miny = 0, double minz = 0, double maxx = 0, double maxy = 0, double maxz = 0);
	EgBox(const EgBox &cpy);
	~EgBox() {}

	// ���� ������ 
	EgBox &operator =(const EgBox &rhs);

	// ��� �Լ�
	EgTrans GetMC();
	double GetWidth();
	double GetHeight();
	double GetDepth();
	double GetDiagonal();
	void GetCornerPts(EgPos *Pt, bool bLocal = true);

	// ������ ���� �Լ�
	void Render(double r, double g, double b, double width);
};

/*! \brief �޽��� �߰� ������ Ÿ�� ������ */
#define RENDER_POLYGON			0x0001
#define RENDER_WIRE				0x0002
#define RENDER_POINT			0x0004
#define RENDER_NORMAL			0x0008
#define RENDER_BACK_FACE		0x0010

/*! \brief �޽��� ���̵� Ÿ�� ������ */
enum TypeShade
{
	SHADE_BY_LIGHT = 0,
	SHADE_BY_TEXTURE,
	SHADE_BY_VERT_COLOR,
};

/*! \brief �޽��� ���� Ÿ�� ������ */
enum TypeNormal
{
	ASIS = 0,
	FLAT = 1,
	SMOOTH = 2,
};

/*!
*	\class EgMesh
*	\brief �ﰢ������ ǥ���� 3���� �޽��� ǥ���ϴ� Ŭ����
*
*	\author ����ȣ, ������
*	\date 25 Jan 2018
*/
class EgMesh : public EgObject
{
public:
	// �����Լ� ������
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
	/*! \brief �޽��� �̸� */
	std::string m_Name;

	/*! \brief �޽��� ���ǵ� �𵨸� ��ǥ�� */
	EgTrans m_MC;

	/*! \brief ������ ������ �迭 */
	std::vector<EgVertex *> m_pVerts;

	/*! \brief �ؽ�ó ������ ������ �迭 */
	std::vector<EgTexel *> m_pTexels;

	/*! \brief ������ ������ �迭 */
	std::vector<EgNormal *> m_pNormals;

	/*! \brief �ﰢ���� ������ �迭 */
	std::vector<EgFace *> m_pFaces;

	/*! \brief ������ ������ �迭 */
	std::vector<EgMaterial *> m_pMaterials;

	/*! \brief �ؽ�ó�� ������ �迭(���� ������ ���� ����) */
	std::vector<EgTexture *> m_pTextures;

	/*! \brief �޽��� ��� ����*/
	EgBox m_BndBox;

	/*! \brief ������ �ɼ�: RENDER_POLYGON, RENDER_WIRE, ... */
	unsigned int m_RenderType;

	/*! \brief �޽��� ���̵� Ÿ��: SHADE_BY_LIGHT, SHADE_BY_TEXTURE, SHADE_BY_COLOR, ... */
	TypeShade m_ShadeType;

	/*! \brief ��� �������� ���� ���� ����: (������, ������ ��ǥ �迭)�� ������ */
	std::map<std::string, std::vector<float>> m_VertexBuf;

	/*! \brief ��� �������� ���� ���� ����: (������, ������ ��ǥ �迭)�� ������ */
	std::map<std::string, std::vector<float>> m_NormalBuf;

	/*! \brief ��� �������� ���� �ؽ�ó ��ǥ ����: (������, �ؽ�ó ��ǥ �迭)�� ������ */
	std::map<std::string, std::vector<float>> m_TexelBuf;

	/*! \brief �ﰢ�� ���̵�� �������� ���÷��� ����Ʈ */
	GLuint m_DisplayListOfFaceId;

	/*! \brief �ﰢ�� ���(Ȧ)�� �������� ���÷��� ����Ʈ */
	GLuint m_DisplayListOfBndry;	

public:
	// ������ �� �Ҹ���
	EgMesh(std::string Name);
	EgMesh(const EgMesh &cpy);
	virtual ~EgMesh();

	// �޽��� ���� ��Ҹ� �߰��ϴ� �Լ���
	void AddVertex(EgVertex *pVert);
	void AddTexel(EgTexel *pTexel);
	void AddNormal(EgNormal *pNormal);
	void AddFace(EgFace *pFace);
	void AddMaterial(EgMaterial *pMaterial);
	void AddTexture(EgTexture *pTexture);

	// �޽��� ���� ��Ҹ� �����ϴ� �Լ�(�߿�: ���۵��� ���������� �޽��� ����)
	int DeleteFaces(std::vector<EgFace *> &Faces);
	int DeleteVerts(std::vector<EgVertex *> &Verts);
	int DeleteEdges(std::vector<EgEdge *> &Edges);

	// �޽��� ����/�ؼ�/����/�ﰢ��/���� �ε����� �ʱ�ȭ�ϴ� �Լ�
	int UpdateVertIdx();
	int UpdateTexIdx();
	int UpdateNormalIdx();
	int UpdateFaceIdx();
	int UpdateMaterialIdx();

	// �޽��� ���� ���� ������ �����ϴ� �Լ���
	void InitEdgeMate();
	void UpdateEdgeMate(EgVertex *pVert);

	// �޽� ���� ������ ó���ϴ� �Լ���
	bool IsFaceNormal();
	void UpdateNormal(TypeNormal NormalType = ASIS);

	// �޽��� ũ�⸦ ����ȭ �ϴ� �Լ�
	void Normalize();

	// �޽��� ���� ���� �Լ���
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

	// �޽� �Ӽ� ���� �Լ�
	void SetMaterial(EgMaterial *pMtl);
	void SetVertColorByGaussCurvature(std::vector<double> &Ks, double Range);
	void SetVertColorByMeanCurvature(std::vector<double> &Hs, double Range);
	void SetVertColorByHarmonicField(std::vector<double> &Fs, bool bRainbow);
	void SetVertColorByNormal();
	void SetVertColorByLight();
		
	// ������ ���� �Լ�
	void RenderPolygon();		// ����, �븻 ���� ���
	void RenderTexture();		// ����, �븻, �ؽ�ó ���� ���
	void RenderVertColor();
	void RenderPoint();
	void RenderWire();			// ���� ���� ���

	// ���÷��� ����Ʈ ���� �� ���� �Լ�
	void CreateDisplayListOfBndry();
	void DeleteDisplayListOfBndry();
	
	// ������ ���� ���� �Լ�
	void UpdateVertexBuf();
	void UpdateNormalBuf();
	void UpdateTexelBuf();
	void UpdateBndBox();
};

/*!	\biref ���� ������ ����� ����Ѵ�. */
void get_curvature(std::vector<EgPos> &Pts, std::vector<double> &Ks, bool bSignedCurvature, EgVec3 Up);

/*! \brief ������ �߽����� �־��� �ݰ� �̳��� �ﰢ���� ������ ����Ѵ�. */
double get_local_area(EgVertex *pVert, double Radius);