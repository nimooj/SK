#include <algorithm>
#include <direct.h>
#include "..\usr\include\EgMesh\egmesh.h"

/*!
*	\brief ��ü�� Ŭ���� �̸��� ��ȯ�Ѵ�.
*
*	\return ��ü�� Ŭ���� �̸��� ��ȯ�Ѵ�.
*/
std::string EgObject::GetClassName() 
{ 
	return std::string("EgObject");
}

/*!
*	\brief ��ü�� ���Ե� �޽��� �����͸� ��ȯ�Ѵ�.
*
*	\return ��ü�� ���Ե� �޽��� �����͸� ��ȯ�Ѵ�.
*/
EgMesh *EgObject::GetMesh()
{
	return NULL;
}

/*!
*	\brief ���� ��ǥ�迡�� ǥ���� ��ü�� ��ȯ ������ ��ȯ�Ѵ�.
*
*	\return ���� ��ǥ�迡�� ǥ���� ��ü�� ��ȯ ������ ��ȯ�Ѵ�.
*/
EgTrans EgObject::GetTrans()
{
	return EgTrans();
}

/*!
*	\brief ���õ� ��ü�� �����Ѵ�.
*
*	\param EC ���� ��ǥ��
*	\param t ������ ��ü�� �̵� ����
*	\param q ������ ��ü�� ȸ�� ����
*	\param s ������ ��ü�� ������ ����
*	\param bFirst ���� ��ü�� �� ù ��° ��ü�� ���� ���� ����
*/
void EgObject::Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst)
{
}

/*!
*	\brief ������ ��ü�� ������ �Ѵ�.
*/
void EgObject::RenderForEdit(float r, float g, float b)
{
}

/*!
*	\brief ��ü�� Ŭ���� �̸��� ��ȯ�Ѵ�.
*
*	\return ��ü�� Ŭ���� �̸��� ��ȯ�Ѵ�.
*/
std::string EgVertex::GetClassName()
{
	return std::string("EgVertex");
}

/*!
*	\brief ������ ���Ե� �޽��� �����͸� ��ȯ�Ѵ�.
*
*	\return ������ ���Ե� �޽��� �����͸� ��ȯ�Ѵ�.
*/
EgMesh *EgVertex::GetMesh()
{
	return m_pMesh;
}

/*!
*	\brief ������ �𵨸� ��ǥ�踦 ��ȯ�Ѵ�.
*
*	\return ������ �𵨸� ��ǥ�踦 ��ȯ�Ѵ�.
*/
EgTrans EgVertex::GetTrans()
{
	return m_pMesh->m_MC * EgTrans(cast_vec3(m_Pos));
}

/*!
*	\brief ������ ���� �޽��� �����Ѵ�.
*
*	\param EC �������� ��ǥ��
*	\param t ������ �̵� ����
*	\param q ������ ȸ�� ����
*	\param s ������ ������ ����
*	\param bFirst ���� ��ü�� �� ù ��° ��ü�� ���� ���� ����
*/
void EgVertex::Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst)
{
	// ���� ��ȯ�� ���� ���� ������ �����ϰ�,
	static EgTrans MC2MC;

	// ���� ��ü�� Ŭ���� �̸��� ���Ͽ�,
	std::string ClsName = m_pMesh->GetClassName();

	// EgMesh ��ü�� ������ ���,
	if (ClsName == "EgMesh")
	{
		// Ȯ�� ������ ���� ���� ��ȯ�� �ƴ϶�� ���� ��ȯ�� ����ϰ�,
		if (bFirst)
			MC2MC = inv(m_pMesh->m_MC) * EC * EgTrans(t, q) * inv(EC) * m_pMesh->m_MC;

		// ������ ������ ��ǥ�� ����Ͽ�,
		EgPos pt = MC2MC * m_Pos;

		// ������ ��ȯ�� �����ϰ�,
		if (s[0] != 1.0) pt[0] *= s[0];
		if (s[1] != 1.0) pt[1] *= s[1];
		if (s[2] != 1.0) pt[2] *= s[2];

		// ���� ������ ��ġ�� �����ϰ�,
		m_Pos = ::lerp(m_Pos, pt, m_EditWgt);
	}
}

/*!
*	\brief ���� ������ ������ �Ѵ�.
*
*	\param r ������ red ����
*	\param g ������ green ����
*	\param b ������ blue ����
*/
void EgVertex::RenderForEdit(float r, float g, float b)
{
	// ���� ��ǥ��: ����(���)
	glPushMatrix();
	{
		// ������ ������ �޽� ��ü�� �𵨸� ��ǥ��� ��ȯ
		glMultMatrixd(cast_arr(cast_mat(m_pMesh->m_MC)));

		int RenderMode = 0;
		switch (RenderMode)
		{		
		case 0: // ����Ʈ ������
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

			// ���� ��忡�� ���´�.
			glDisable(GL_BLEND);
			glDisable(GL_POINT_SMOOTH);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
			break;

		// �� ������
		case 1:
		{
			// ���õ� ������ �׸���.
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
*	\brief ������
*
*	\param x ������ x��ǥ
*	\param y ������ y��ǥ
*	\param z ������ z��ǥ
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
*	\brief ������
*
*	\param Pos ������ ��ǥ
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
*	\brief �Ҹ���
*/
EgVertex::~EgVertex()
{
}

/*!
*	\brief ������ ����(������ ����� ������ ��)�� ����Ѵ�.
*
*	\return ���� ������ ��ȯ�Ѵ�.
*/
int EgVertex::GetDegree()
{
	int Degree = 0;
	// �������� �����ϴ� ������ ������ ���Ͽ�
	for (EgEdge *e : m_pEdges)
	{
		// ������ ������ ������Ű��,
		Degree++;
		
		// ��� ������ ��쵵 ������ �����Ѵ�.
		if (e->m_pNext->m_pNext->m_pMate == NULL)
			Degree++;
	}

	// ������ ������ ��ȯ�Ѵ�.
	return Degree;
}

/*!
*	\brief ������ 1�� �̿� ������ ���Ѵ�.
*
*	\param Verts 1�� �̿� �������� �����Ͱ� ����ȴ�[out].
*
*	\return 1���� �����ϴ� ������ ��(degree)�� ��ȯ�Ѵ�.
*/
int EgVertex::GetOneRingVert(std::vector<EgVertex *> &Verts)
{
	// �迭�� �ʱ�ȭ �ϰ�
	Verts.clear();

	// �������� �����ϴ� ������ ������ ���Ͽ�
	for (EgEdge *e : m_pEdges)
	{
		// ������ ������ �迭�� �߰��ϰ�
		Verts.push_back(e->m_pNext->m_pVert);

		// ��� ������ ��쵵 �迭�� �߰��Ѵ�.
		if (e->m_pNext->m_pNext->m_pMate == NULL)
			Verts.push_back(e->m_pNext->m_pNext->m_pVert);
	}

	// 1�� ������ ���� ��ȯ�Ѵ�.
	return (int)Verts.size();
}

/*!
*	\brief ������ 1�� ��� ������ �����͸� ã�´�.
*
*	\param Edges ��� �������� �����Ͱ� ����ȴ�[out].
*
*	\return ������ 1�� ��� ������ ���� ��ȯ�Ѵ�.
*/
int EgVertex::GetOneRingBndryEdge(std::vector<EgEdge *> &Edges)
{
	// ����Ʈ�� �ʱ�ȭ �ϰ�
	Edges.clear();

	// �������� �����ϴ� ������ ������ ���Ͽ�
	for (EgEdge *e : m_pEdges)
		Edges.push_back(e->m_pNext); // ��� ������ ����Ʈ�� �߰��Ѵ�.

	// 1�� ��� ������ ���� ��ȯ�Ѵ�.
	return (int)Edges.size();
}

/*!
*	\brief ������ �����ϴ� 1�� �ﰢ������ �����͸� ã�´�.
*
*	\param Faces ������ �����ϴ� �ﰢ������ �����Ͱ� ����ȴ�[out].
*
*	\return ������ �����ϴ� 1�� �ﰢ���� ���� ��ȯ�Ѵ�.
*/
int EgVertex::GetOneRingFace(std::vector<EgFace *> &Faces)
{
	// �迭�� �ʱ�ȭ �ϰ�
	Faces.clear();

	// �������� �����ϴ� ������ ������ ���Ե� �ﰢ���� ���Ѵ�.
	for (EgEdge *e : m_pEdges)
		Faces.push_back(e->m_pFace);

	// �ﰢ���� ���� ��ȯ�Ѵ�.
	return (int)Faces.size();
}

/* !
*	\brief ��� ���� ���θ� �����Ѵ�.
*
*	\return ��� �����̸� true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool EgVertex::IsBndry()
{
	// �������� �����ϴ� ������ ������ ���Ͽ�
	for (EgEdge *e : m_pEdges)
	{
		// �ϳ��� ��� ������� ��� �����̹Ƿ� true�� ��ȯ�Ѵ�.
		if (e->IsBndry())
			return true;
	}

	// ��� ������ ���ٸ� false�� ��ȯ�Ѵ�.
	return false;
}

/*!
*	\brief	������ ������ ������ �ִ��� �Ǵ��Ѵ�.
*
*	\return	������ ������ ������ ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
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
*	\brief	�ش� ������ ���� v�� ����Ǿ� �ִ��� �����Ѵ�.
*
*	\param	v ������ ����
*
*	\return	�ش� ������ v�� ����Ǿ� ������ �ش� ������ ��ȯ�ϰ�, �ƴϸ� NULL�� ��ȯ�Ѵ�.
*/
EgEdge *EgVertex::IsAdjacent(EgVertex *v)
{
	// ���� �������� �����ϴ� ������ ������ ���Ͽ�
	for (EgEdge *e : m_pEdges)
	{
		// ���� ������ ���� ������ v�� ���ٸ� �ش� ������ ��ȯ�Ѵ�.
		if (e->m_pNext->m_pVert == v)
			return e;
	}
	return NULL;
}

/*!
*	\brief	������ ��� ���� ������ ���Ѵ�.
*	
*	\return ������ ��� ���� ������ ��ȯ�Ѵ�.
*/
EgVec3 EgVertex::GetAvgNormal()
{
	if (m_pEdges.empty())
		return EgVec3(0.0, 0.0, 0.0);

	// ��� 1
	EgVec3 N;
	for (EgEdge *e : m_pEdges)
		N += e->m_pNormal->m_Dir;
	N.Normalize();

	// ��� 2
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
*	\breif	������ 1-�� �������� �����߽��� ��ȯ�Ѵ�.
*
*	\return ������ 1-�� �������� �����߽��� ��ȯ�Ѵ�.
*/
EgPos EgVertex::GetOneRingVertCenter()
{
	// ������ 1�� �̿� ������ �����߽��� ���Ѵ�.
	EgVec3 cog;
	std::vector<EgVertex *> Verts;
	int Degree = GetOneRingVert(Verts);
	for (EgVertex *v : Verts)
		cog += cast_vec3(v->m_Pos);
	cog /= (double)Degree;

	return cast_pos(cog);
}

/*!
*	\brief	������ ��ġ�� perturbation �Ѵ�
*
*	\param	rate[in]	���� �߽� �������� ������ ����
*
*	\return	����� ������ ��ġ�� ��ȯ�Ѵ�.
*/
EgPos EgVertex::Perturbate(double rate)
{
	EgPos P = GetOneRingVertCenter();
	m_Pos = m_Pos + (P - m_Pos) * rate;

	return m_Pos;
}

/*!
*	\brief	�������� �����ϴ� ���� ����Ʈ���� ������ �����Ѵ�.
*
*	\param	e ������ ������ ������
*
*	\return	���� ��, �������� �����ϴ� ������ ���� ��ȯ�Ѵ�(������ ������ ���� ��� -1�� ��ȯ)
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
*	\brief ������
*
*	\param u ���� ���� �ؽ�ó ��ǥ
*	\param v ���� ���� �ؽ�ó ��ǥ
*/
EgTexel::EgTexel(double u, double v)
{
	m_Idx = -1;
	m_UV[0] = u;
	m_UV[1] = v;
}

/*!
*	\brief �Ҹ���
*/
EgTexel::~EgTexel()
{
}

/*!	\brief ������
*
*	\param nx ������ x ��ǥ
*	\param ny ������ y ��ǥ
*	\param nz ������ z ��ǥ
*/
EgNormal::EgNormal(double nx, double ny, double nz)
{
	m_Idx = -1;
	m_Dir = EgVec3(nx, ny, nz);
}

/*!	\brief ������
*
*	\param Dir ������ ����
*/
EgNormal::EgNormal(EgVec3 Dir)
{
	m_Idx = -1;
	m_Dir = Dir;
}

/*!
*	\brief �Ҹ���
*/
EgNormal::~EgNormal()
{
}

/*!
*	\brief ������
*
*	\param pVert ������ �������� �Ҵ�� ������ ���� ������
*	\param pTexel ������ �������� �Ҵ�� �ؽ�ó ������ ���� ������
*	\param pNormal ������ ���ۿ� �Ҵ�� ������ ���� ������
*/
EgEdge::EgEdge(EgVertex *pVert, EgTexel *pTexel, EgNormal *pNormal)
{
	// ����, �ؼ�, ���� ������ ������ �������� �Ҵ��Ѵ�.
	m_pVert = pVert;
	m_pTexel = pTexel;
	m_pNormal = pNormal;	

	// ���� ����, �ݴ��� ����, ������ ���� �ﰢ���� ���� �����͸� �ʱ�ȭ�Ѵ�.
	m_pNext = NULL;
	m_pMate = NULL;
	m_pFace = NULL;

	// �������� ������ ���� ������ �߰��Ѵ�.
	m_pVert->m_pEdges.push_back(this);

	// �ܼ�ȭ�� �ʿ��� ���� ���� ����� �ʱ�ȭ �Ѵ�.
	m_Cost = 0.0;
}

/*!
*	\brief �Ҹ���
*/
EgEdge::~EgEdge()
{
}

/*! 
*	\brief ������ ����Ʈ ������ ������ �ﰢ���� ���Ѵ�.
*
*	\param Faces �ﰢ�� �����Ͱ� ����ȴ�.
*
*	\return ����Ʈ ������ �ִ� ��� 2, ����Ʈ ������ ���� ��� 1�� ��ȯ�ȴ�.
*/
int EgEdge::GetIncidentFace(std::vector<EgFace *> &Faces)
{
	Faces.push_back(m_pFace);
	if (m_pMate != NULL)
		Faces.push_back(m_pMate->m_pFace);

	return (int)Faces.size();
}

/*!
*	\brief ������ ��� �������� �����Ѵ�.
*
*	\return ��� �����̸� true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool EgEdge::IsBndry()
{
	return (m_pMate == NULL);
}

/*!
*	\brief ������ �ؽ�ó ��� �������� �����Ѵ�.
*
*	\return �ؽ�ó ��� �����̸� true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool EgEdge::IsTexBndry()
{
	return (IsBndry() || m_pTexel != m_pMate->m_pNext->m_pTexel);
}

/*!
*	\brief	������ ���� �������� �˻��Ѵ�.
*
*	\return ������ ���Ű����� ��� true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool EgEdge::IsCollapsible()
{
	// ������ ������(v1)�� ����(v2)�� ���Ѵ�.
	EgVertex *v1 = m_pVert;
	EgVertex *v2 = m_pNext->m_pVert;

	// v1�� v2�� ��� �����ε�, ������ ��� ������ �ƴ� ��� false�� ��ȯ�Ѵ�.
	if (v1->IsBndry() && v2->IsBndry() && !IsBndry())
		return false;

	// v1�� v2�� 1�� ������ ���� A�� B�� ���Ѵ�.
	std::vector<EgVertex *> A, B;
	v1->GetOneRingVert(A);
	v2->GetOneRingVert(B);

	// A�� B�� �������� ���Ѵ�.
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

	// ���� ������ ������ ������ ��� true�� ��ȯ�Ѵ�.
	if (m_pMate == NULL && C.size() == 1)
		return true;
	if (m_pMate != NULL && C.size() == 2)
		return true;

	// ���� ���� ������ �������� ���ϴ� ��� false�� ��ȯ�Ѵ�.
	return false;
}

/*!
*	\brief �ﰢ������ ������ ���ֺ��� �ִ� ������ ����Ѵ�.
*	\warning 0 < ���̰� < 180
*
*	\param bRadian ������ ����(true: ����, false: ����)
*
*	\return �ﰢ������ ������ ���ֺ��� �ִ� ������ ��ȯ�Ѵ�.
*/
double EgEdge::GetAngle(bool bRadian)
{
	// ������ ���Ե� �ﰢ������ �� ���� ���� ���͸� ���Ѵ�.
	EgPos p = m_pVert->m_Pos;
	EgPos q = m_pNext->m_pVert->m_Pos;
	EgPos r = m_pNext->m_pNext->m_pVert->m_Pos;
	EgVec3 N = m_pFace->GetFaceNormal(true);

	// ������ ���ֺ��� �ִ� ������ ����Ͽ� ��ȯ�Ѵ�.
	return (bRadian) ? angle(p - r, q - r, N, true) : angle(p - r, q - r, N, false);
}

/*!
*	\brief �ﰢ������ ������ ���ֺ��� �ִ� ������ ��ź��Ʈ ���� ����Ѵ�.
*
*	\return �ﰢ������ ������ ���ֺ��� �ִ� ������ ��ź��Ʈ ���� ��ȯ�Ѵ�.
*/
double EgEdge::GetCotangent()
{
	// ������ ���Ե� �ﰢ������ �� ���� ���� ���͸� ���Ѵ�.
	EgPos p = m_pVert->m_Pos;
	EgPos q = m_pNext->m_pVert->m_Pos;
	EgPos r = m_pNext->m_pNext->m_pVert->m_Pos;
	EgVec3 rp = p - r;
	EgVec3 rq = q - r;

	// ������ ���ֺ��� �ִ� ������ ��ź��Ʈ ���� ��ȯ�Ѵ�.
	return (rp * rq) / norm(rp ^ rq);
}

/*!
*	\brief	������ ���̸� ���Ѵ�.
*
*	\return ������ ���̸� ��ȯ�Ѵ�.
*/
double EgEdge::GetLength()
{
	return dist(m_pVert->m_Pos, m_pNext->m_pVert->m_Pos);
}

/*!
*	\brief	���� �������� ������� ���Ѵ�.
*
*	\return ���� ������� ��ȯ�Ѵ�.
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
*	\brief ��ü�� Ŭ���� �̸��� ��ȯ�Ѵ�.
*
*	\return ��ü�� Ŭ���� �̸��� ��ȯ�Ѵ�.
*/
std::string EgFace::GetClassName()
{
	return std::string("EgFace");
}

/*!
*	\brief �ﰢ���� ���Ե� �޽��� �����͸� ��ȯ�Ѵ�.
*
*	\return �ﰢ���� ���Ե� �޽��� �����͸� ��ȯ�Ѵ�.
*/
EgMesh *EgFace::GetMesh()
{
	return m_pEdge->m_pVert->m_pMesh;
}

/*!
*	\brief �ﰢ���� �����߽ɿ��� ���ǵ� ��ǥ�踦 ��ȯ�Ѵ�.
*
*	\return �ﰢ���� �����߽ɿ��� ���ǵ� ��ǥ��(���� ��ǥ�� ����)�� ��ȯ�Ѵ�.
*/
EgTrans EgFace::GetTrans()
{
	return EgTrans(cast_vec3(GetCOG(false)));
}

/*!
*	\brief �ﰢ���� �ﰢ���� ���Ե� ��ü�� �����Ѵ�.
*
*	\param EC �������� ��ǥ��
*	\param t �ﰢ���� �̵� ����
*	\param q �ﰢ���� ȸ�� ����
*	\param s �ﰢ���� ������ ����
*	\param bFirst ���� ��ü�� �� ù ��° ��ü�� ���� ���� ����
*/
void EgFace::Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst)
{
	EgMesh *pMesh = GetMesh();
	if (pMesh->GetClassName() != "EgMesh")
		return;

	// ���� ��ȯ�� ���� ���� ������ �����ϰ�,
	static EgTrans MC2MC;
	static std::vector<bool> VertFlag;

	// �������� ���� ��ü�� �� ù ��° �����̶��
	if (bFirst)
	{
		int NumVert = pMesh->GetNumVert();
		VertFlag.clear();
		VertFlag.assign(NumVert, false);

		// ���� ��ȯ�� ����ϰ�,
		MC2MC = inv(pMesh->m_MC) * EC * EgTrans(t, q) * inv(EC) * pMesh->m_MC;
	}	

	// �ﰢ���� �� ������ ��ǥ�� �����Ѵ�.
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
*	\brief ���� �ﰢ���� ������ �Ѵ�.
*
*	\param r ������ red ����
*	\param g ������ green ����
*	\param b ������ blue ����
*/
void EgFace::RenderForEdit(float r, float g, float b)
{
	// ���� ����� Ȱ���ϰ�, ������ ��Ȱ��ȭ �Ѵ�.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LIGHTING);	
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0.3f, 0.3f);

	EgMesh *pMesh = GetMesh();

	// ���� ��ǥ��: ����(���)
	glPushMatrix();
	{
		// �޽� ��ǥ��� ��ȯ
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

	// ������ ����� ��Ȱ���ϰ�, ������ Ȱ��ȭ �Ѵ�.
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glDisable(GL_POLYGON_OFFSET_FILL);
}

/*!
*	\brief ������
*
*	\param e0 �ﰢ���� ù ��° ����
*	\param e1 �ﰢ���� �� ��° ����
*	\param e2 �ﰢ���� �� ��° ����
*	\param pMtl �ﰢ���� ����ϴ� ������ ���� ������
*	\param GroupName �ﰢ���� ���� �׷��� �̸�
*/
EgFace::EgFace(EgEdge *e0, EgEdge *e1, EgEdge *e2, EgMaterial *pMtl, std::string GroupName)
{
	// �ﰢ���� �ε����� �ʱ�ȭ �ϰ� ���� ������ �����Ѵ�.
	m_Idx = -1;
	m_pEdge = e0;

	// ������ ������踦 �����Ѵ�.
	e0->m_pNext = e1;
	e1->m_pNext = e2;
	e2->m_pNext = e0;
	e0->m_pFace = e1->m_pFace = e2->m_pFace = this;	

	// ������ �ﰢ���� ���� �׷��� �̸��� �����Ѵ�.
	m_pMaterial = pMtl;
	m_GroupName = GroupName;
}

/*!
*	\brief ������
*
*	\param v0 �ﰢ���� ù ��° ����
*	\param v1 �ﰢ���� �� ��° ����
*	\param v2 �ﰢ���� �� ��° ����
*	\param t0 �ﰢ���� ù ��° ������ �ؽ�ó ��ǥ
*	\param t1 �ﰢ���� �� ��° ������ �ؽ�ó ��ǥ
*	\param t2 �ﰢ���� �� ��° ������ �ؽ�ó ��ǥ
*	\param n0 �ﰢ���� ù ��° ������ ����
*	\param n1 �ﰢ���� �� ��° ������ ����
*	\param n2 �ﰢ���� �� ��° ������ ����
*	\param pMtl �ﰢ���� ����ϴ� ������ ���� ������
*	\param GroupName �ﰢ���� ���� �׷��� �̸�
*/
EgFace::EgFace(
	EgVertex *v0, EgVertex *v1, EgVertex *v2, 
	EgTexel *t0, EgTexel *t1, EgTexel *t2, 
	EgNormal *n0, EgNormal *n1, EgNormal *n2, 
	EgMaterial * pMtl, std::string GroupName)
{
	// �ﰢ���� �ε����� �ʱ�ȭ �ϰ�
	m_Idx = -1;

	// �� �������� �����ϴ� 3���� ������ �����ϰ� ���� ������ �����Ѵ�.
	EgEdge *e0 = new EgEdge(v0, t0, n0);
	EgEdge *e1 = new EgEdge(v1, t1, n1);
	EgEdge *e2 = new EgEdge(v2, t2, n2);
	m_pEdge = e0;

	// ������ ������踦 �����Ѵ�.
	e0->m_pNext = e1;
	e1->m_pNext = e2;
	e2->m_pNext = e0;
	e0->m_pFace = e1->m_pFace = e2->m_pFace = this;

	// ������ �ﰢ���� ���� �׷��� �̸��� �����Ѵ�.
	m_pMaterial = pMtl;
	m_GroupName = GroupName;	
}

/*!
*	\brief �Ҹ���
*/
EgFace::~EgFace()
{
	delete m_pEdge->m_pNext->m_pNext;
	delete m_pEdge->m_pNext;
	delete m_pEdge;
	m_pEdge = NULL;
}

/*!
*	\brief �ﰢ������ ������ ��ǥ�� ��ȯ�Ѵ�.
*
*	\param vidx �ﰢ���� �����ϴ� ���� �ε���(0, 1, 2)
*
*	\return �ε����� �����ϴ� ������ ��ǥ�� ��ȯ�Ѵ�.
*/
EgPos EgFace::GetVertexPos(int vidx)
{
	return GetEdge(vidx)->m_pVert->m_Pos;
}

/*!
*	\brief �ﰢ�� ������ �����͸� ��ȯ�Ѵ�.
*
*	\param vIdx �ﰢ���� �����ϴ� ������ �ε���(0, 1, 2)
*
*	\return �ε����� �����Ǵ� ������ �����͸� ��ȯ�Ѵ�.
*/
EgVertex *EgFace::GetVertex(int vIdx)
{
	return GetEdge(vIdx)->m_pVert;
}

/*!
*	\brief �ﰢ������ ���������� �Ҵ�� �ؽ�ó ��ǥ�� ��ȯ�Ѵ�.
*
*	\param tidx �ﰢ���� �����ϴ� �������� �ε���(0, 1, 2)
*
*	\return �ε����� �����ϴ� �ؽ�ó ��ǥ�� ��ȯ�Ѵ�.
*/
EgVec2 EgFace::GetTexelCoord(int tidx)
{
	return GetEdge(tidx)->m_pTexel->m_UV;
}

/*!
*	\brief �ﰢ������ �ؽ�ó ������ �����͸� ��ȯ�Ѵ�.
*
*	\param tidx �ﰢ���� �����ϴ� �ؽ�ó ������ �ε���(0, 1, 2)
*
*	\return �ε����� �����Ǵ� �ؽ�ó ������ �����͸� ��ȯ�Ѵ�.
*/
EgTexel *EgFace::GetTexel(int tidx)
{
	return GetEdge(tidx)->m_pTexel;
}

/*!
*	\brief �ﰢ������ ���������� �Ҵ�� ������ ������ ��ȯ�Ѵ�.
*
*	\param nidx �ﰢ���� �����ϴ� �������� �ε���(0, 1, 2)
*	\param bLocal ��ü�� �𵨸� ��ǥ�迡�� ǥ���� ������ ��� true, ���� ��ǥ�迡�� ǥ���� ��� false
*
*	\return �ε����� �����ϴ� ������ ������ ��ȯ�Ѵ�.
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
*	\brief �ﰢ������ ������ �����͸� ��ȯ�Ѵ�.
*
*	\param nIdx �ﰢ���� �����ϴ� ������ �ε���(0, 1, 2)
*
*	\return �ε����� �����ϴ� ������ �����͸� ��ȯ�Ѵ�.
*/
EgNormal *EgFace::GetNormal(int nidx)
{
	return GetEdge(nidx)->m_pNormal;
}

/*!
*	\brief �ﰢ���� ���� �������͸� ����Ѵ�.
*
*	\param bLocal ��ü�� �𵨸� ��ǥ�迡�� ǥ���� ������ ��� true, ���� ��ǥ�迡�� ǥ���� ��� false
*
*	\return �ﰢ���� ���� �������͸� ��ȯ�Ѵ�.
*/
EgVec3 EgFace::GetFaceNormal(bool bLocal)
{
	// �ﰢ���� �� ������ ��ġ�� ���Ͽ�
	EgPos p0 = m_pEdge->m_pVert->m_Pos;
	EgPos p1 = m_pEdge->m_pNext->m_pVert->m_Pos;
	EgPos p2 = m_pEdge->m_pNext->m_pNext->m_pVert->m_Pos;

	// ���� ������ ������ ���Ͽ� ��ȯ�Ѵ�.
	EgVec3 N = ((p1 - p0) ^ (p2 - p0)).Normalize();
	if (bLocal)
		return N;
	else
		return GetMesh()->m_MC * N;
}

/*!
*	\brief �ﰢ�� ���� �����͸� ��ȯ�Ѵ�.
*
*	\param eIdx �ﰢ���� �����ϴ� ������ �ε���(0, 1, 2)
*
*	\return �ε����� �����Ǵ� ������ �����͸� ��ȯ�Ѵ�.
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
*	\brief	�ﰢ���� �����ϴ� ������ �ε��� ����Ʈ�� ��ȯ�Ѵ�.
*
*	\return �ﰢ���� �����ϴ� ������ �ε��� �迭�� ���� �����͸� ��ȯ�Ѵ�.
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
*	\brief	�ﰢ���� �����ϴ� �ؽ�ó ������ �ε��� ����Ʈ�� ��ȯ�Ѵ�.
*
*	\return �ﰢ���� �����ϴ� �ؽ�ó ������ �ε��� �迭�� ���� �����͸� ��ȯ�Ѵ�.
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
*	\brief	�ﰢ���� �����ϴ� ������ �ε��� ����Ʈ�� ��ȯ�Ѵ�.
*
*	\return �ﰢ���� �����ϴ� ������ �ε��� �迭�� ���� �����͸� ��ȯ�Ѵ�.
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
*	\brief �ﰢ������ ������ �ε����� ��ȯ�Ѵ�.
*
*	\param e ã�� ������ ���� ������
*
*	\return �ﰢ������ ������ �ε����� ��ȯ�Ѵ�.
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
*	\brief �ﰢ���� ������ ����Ѵ�.
*
*	\return ���� ������ ��ȯ�Ѵ�.
*/
double EgFace::GetArea()
{
	EgVec3 a = GetVertex(1)->m_Pos - GetVertex(0)->m_Pos;
	EgVec3 b = GetVertex(2)->m_Pos - GetVertex(0)->m_Pos;
	return norm(a ^ b) * 0.5;
}

/*!
*	\brief	�ﰢ���� �ѷ��� ���̸� ����Ѵ�.
*
*	\return	���� �ѷ��� ���̸� ��ȯ�Ѵ�.
*/
double EgFace::GetPerimeter()
{
	EgPos p0 = GetVertex(0)->m_Pos;
	EgPos p1 = GetVertex(1)->m_Pos;
	EgPos p2 = GetVertex(2)->m_Pos;

	return dist(p0, p1) + dist(p1, p2) + dist(p2, p0);
}

/*!
*	\brief �ﰢ���� ���� �߽��� ����Ѵ�.
*
*	\param bLocal ��ü�� �𵨸� ��ǥ�迡�� ǥ���� ��� true, ���� ��ǥ�迡�� ǥ���� ��� false
*
*	\return �ﰢ���� ���� �߽��� ��ġ�� ��ȯ�Ѵ�.
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
*	\brief �ﰢ�� ������ ������ �ﰢ���� ���Ѵ�.
*
*	\param Faces ������ ������ �ﰢ���� �����Ͱ� ����ȴ�.
*
*	\return ������ ������ �ﰢ���� ����(0, 1, 2, 3)�� ��ȯ�Ѵ�.
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
*	\brief �ﰢ���� ��� �ﰢ������ �����Ѵ�.
*
*	\return	��� �ﰢ���̸� true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool EgFace::IsBndryFace()
{
	for (int i = 0; i < 3; ++i)
		if (GetVertex(i)->IsBndry())
			return true;
	return false;
}

/*!
*	\brief ��ü�� Ŭ���� �̸��� ��ȯ�Ѵ�.
*
*	\return ��ü�� Ŭ���� �̸��� ��ȯ�Ѵ�.
*/
std::string EgFacePt::GetClassName()
{
	return std::string("EgFacePt");
}

/*!
*	\brief �������� ���Ե� �޽��� �����͸� ��ȯ�Ѵ�.
*
*	\return �������� ���Ե� �޽��� �����͸� ��ȯ�Ѵ�.
*/
EgMesh *EgFacePt::GetMesh()
{
	return m_pFace->GetMesh();
}

/*!
*	\brief �������� ���� ��ǥ�踦 ��ȯ�Ѵ�.
*
*	\return �������� ���� ��ǥ�踦 ��ȯ�Ѵ�.
*/
EgTrans EgFacePt::GetTrans()
{
	return GetMesh()->GetTrans() * EgTrans(cast_vec3(Pos()));
}

/*!
*	\brief �������� ���� �޽��� �����Ѵ�.
*
*	\param EC[in] �������� ��ǥ��
*	\param t[in] ������ �̵� ����
*	\param q[in] ������ ȸ�� ����
*	\param s[in] ������ ������ ����
*	\param bFirst[in] ���� ��ü�� �� ù ��° ��ü�� ���� ���� ����
*/
void EgFacePt::Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst)
{
	m_pFace->Edit(EC, t, q, s, bFirst);
}

/*!
*	\brief �������� ������ �Ѵ�.
*
*	\param r[in] ������ red ����
*	\param g[in] ������ green ����
*	\param b[in] ������ blue ����
*/
void EgFacePt::RenderForEdit(float r, float g, float b)
{
	// ���� ��ǥ��: ����(���)
	glPushMatrix();
	{
		// ���� ������ �޽� ��ü�� �𵨸� ��ǥ��� ��ȯ
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

		// ���� ��忡�� ���´�.
		glDisable(GL_BLEND);
		glDisable(GL_POINT_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();
}

/*!
*	\brief ������
*
*	\param pFace[in]	���� ���� �ﰢ���� ���� ������
*	\param Coords[in]	���� barycentric ��ǥ
*/
EgFacePt::EgFacePt(EgFace *pFace, EgVec3 Coords)
{
	m_pFace = pFace;
	m_Coords = Coords;
}

/*!
*	\brief ���� ������
*
*	\param cpy[in] ������ ��ü�� ������
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgFacePt::EgFacePt(const EgFacePt &cpy)
{
	m_pFace = cpy.m_pFace;
	m_Coords = cpy.m_Coords;
}

/*!
*	\brief �Ҹ���
*/
EgFacePt::~EgFacePt()
{
}

/*!
*	\brief ���� ������
*
*	\param rhs[in] ������ ������ �ǿ�����
*
*	\return ���Ե� �ڽ��� ��ȯ�Ѵ�.
*/
EgFacePt &EgFacePt::operator=(const EgFacePt &rhs)
{
	m_pFace = rhs.m_pFace;
	m_Coords = rhs.m_Coords;

	return *this;
}

/*!
*	\brief �ﰢ�� �������� 3���� ��ġ�� ����Ѵ�.
*
*	\return ���� 3���� ��ġ�� ��ȯ�Ѵ�.
*/
EgPos EgFacePt::Pos()
{
	EgPos p0 = P0(m_pFace);
	EgPos p1 = P1(m_pFace);
	EgPos p2 = P2(m_pFace);
	return p2 + (p0 - p2) * m_Coords[0] + (p1 - p2) * m_Coords[1];
}

/*!
*	\brief	�ﰢ�� �ڳ� ���� ���θ� �����Ѵ�.
*
*	\return �ﰢ���� �����̸� true, �ƴϸ� false�� ��ȯ�Ѵ�.
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
*	\brief �ﰢ�� ���������� �Ÿ��� ����Ѵ�.
*
*	\param vidx Ÿ�� ������ �ε���
*
*	\return ���� �Ÿ��� ��ȯ�Ѵ�.
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
*	\brief	���� ����� ������ ã�´�.
*
*	\return ���� ����� ������ �����͸� ��ȯ�Ѵ�.
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
*	\brief ������
*/
EgTexture::EgTexture()
{
	m_FileName = "";
	m_TexId = 0;
}

/*!
*	\brief �Ҹ���
*/
EgTexture::~EgTexture()
{
	// ������ OpenGL �ؽ�ó�� �ִٸ� �����Ѵ�.
	if (m_TexId != 0)
	{
		glDeleteTextures(1, &m_TexId);
		m_TexId = 0;
	}
}

/*!
*	\brief �ؽ�ó ���Ϸκ��� OpenGL �ؽ�ó�� �����Ѵ�.
*
*	\param fname �ؽ�ó ���ϸ�
*
*	\return �����̸� true, ���и� false�� ��ȯ�Ѵ�.
*/
bool EgTexture::LoadTexture(const char *fname)
{
	// ���� �۾� ���͸��� ���� ��η� ����Ѵ�.
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

	// ILUT ���̺귯���� �̿��Ͽ� ������ ������ OPENGL �ؽ�ó ���·� �ε��Ѵ�.
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
*	\brief ������
*
*	\param Name ������ ������ �̸�
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
*	\brief ���� ������
*
*	\param cpy ������ ����
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
*	\brief �Ҹ���
*/
EgMaterial::~EgMaterial()
{
}

/*!
*	\brief ������ �ֺ��� �ݻ� ����� �����Ѵ�.
*
*	\param r Red ���п� ���� �ݻ���
*	\param g Green ���п� ���� �ݻ���
*	\param b Blue ���п� ���� �ݻ���
*	\param a Alpha ������ ���� ���� ��
*/
void EgMaterial::SetAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_Ambient[0] = r;
	m_Ambient[1] = g;
	m_Ambient[2] = b;
	m_Ambient[3] = a;
}

/*!
*	\brief ������ ���ݻ� ����� �����Ѵ�.
*
*	\param r Red ���п� ���� �ݻ���
*	\param g Green ���п� ���� �ݻ���
*	\param b Blue ���п� ���� �ݻ���
*	\param a Alpha ������ ���� ���� ��
*/
void EgMaterial::SetDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_Diffuse[0] = r;
	m_Diffuse[1] = g;
	m_Diffuse[2] = b;
	m_Diffuse[3] = a;
}

/*!
*	\brief ������ ���ݻ� ����� �����Ѵ�.
*
*	\param r Red ���ݻ� ���п� ���� �ݻ���
*	\param g Green ���ݻ� ���п� ���� �ݻ���
*	\param b Blue ���ݻ� ���п� ���� �ݻ���
*	\param a Alpha ������ ���� ���� ��
*/
void EgMaterial::SetSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_Specular[0] = r;
	m_Specular[1] = g;
	m_Specular[2] = b;
	m_Specular[3] = a;
}

/*!
*	\brief ������ ���ݻ� ������ �����Ѵ�.
*
*	\param s ������ ���ݻ� ����
*/
void EgMaterial::SetShiness(GLfloat s)
{
	m_Shiness = s;	
}

/*!
*	\brief ������ �ؽ�ó�� �����Ѵ�.
*
*	\param pTexture ������ �ؽ�ó�� ���� ������
*/
void EgMaterial::SetTexture(EgTexture *pTexture)
{
	m_pTexture = pTexture;
}

/*!
*	\brief ������ Ȱ��ȭ �Ѵ�.
*
*	\param bTexture �ؽ�ó ���� ����
*/
void EgMaterial::Enable()
{
	// ���� ���� 1�� �ƴ϶�� ������ Ȱ��ȭ�ϰ�,
	if (m_Diffuse[3] != 1.0f)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else // �ƴϸ� ������ ��Ȱ��ȭ �Ѵ�.
		glDisable(GL_BLEND);

	// �ﰢ���� �ո�� �޸鿡 ������ ������ �����Ѵ�.
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_Ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_Diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_Specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_Shiness);
}

/*!
*	\brief ������ ��Ȱ��ȭ �Ѵ�.
*/
void EgMaterial::Disable()
{
	glDisable(GL_BLEND);
}

/*!
*	\brief ������
*
*	\param minx ��� ������ �ּ����� x ��ǥ
*	\param miny ��� ������ �ּ����� y ��ǥ
*	\param minz ��� ������ �ּ����� z ��ǥ
*	\param maxx ��� ������ �ִ����� x ��ǥ
*	\param maxy ��� ������ �ִ����� y ��ǥ
*	\param maxz ��� ������ �ִ����� z ��ǥ
*/
EgBox::EgBox(double minx, double miny, double minz, double maxx, double maxy, double maxz)
{
	m_pMesh = NULL;
	m_MinPt.Set(minx, miny, minz);
	m_MaxPt.Set(maxx, maxy, maxz);
}

/*!
*	\brief ���� ������
*
*	\param cpy ����� ��ü
*/
EgBox::EgBox(const EgBox &cpy)
{
	m_pMesh = NULL;
	m_MinPt = cpy.m_MinPt;
	m_MaxPt = cpy.m_MaxPt;
}

/*!
*	\brief ���� ������
*
*	\param rhs ���Ե� ��ü
*
*	\return ���Ե� �ڽ��� ��ȯ�Ѵ�.
*/
EgBox &EgBox::operator =(const EgBox &rhs)
{
	m_pMesh = NULL;
	m_MinPt = rhs.m_MinPt;
	m_MaxPt = rhs.m_MaxPt;

	return *this;
}

/*!
*	\brief ���� ��ǥ�迡�� ǥ���� ��� ������ �𵨸� ��ǥ�踦 ��ȯ�Ѵ�.
*
*	\return ���� ��ǥ�迡�� ǥ���� ��� ������ �𵨸� ��ǥ�踦 ��ȯ�Ѵ�.
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
*	\brief �������� x�� ������ �ʺ� ��ȯ�Ѵ�.
*/
double EgBox::GetWidth()
{
	return ABS(m_MaxPt[0] - m_MinPt[0]);
}

/*!
*	\brief �������� y�� ������ ���̸� ��ȯ�Ѵ�.
*/
double EgBox::GetHeight()
{
	return ABS(m_MaxPt[1] - m_MinPt[1]);
}

/*!
*	\brief �������� z�� ������ ���̸� ��ȯ�Ѵ�.
*/
double EgBox::GetDepth()
{
	return ABS(m_MaxPt[2] - m_MinPt[2]);
}

/*!
*	\brief �������� �밢���� ���̸� ��ȯ�Ѵ�.
*/
double EgBox::GetDiagonal()
{
	return dist(m_MinPt, m_MaxPt);
}

/*!
*	\brief �������� 8���� �𼭸� ������ ��ȯ�Ѵ�.
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
*	\param Pt �������� 8���� �𼭸� ��ġ�� ����ȴ�.
*	\param bLocal �𼭸� ������ ��ǥ��(true: �𵨸� ��ǥ��, false: ���� ��ǥ��)
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
*	\brief ��� ���ڸ� ������ �Ѵ�.
*
*	\param r ��� ���� �׵θ��� ������ ����
*	\param g ��� ���� �׵θ��� �ʷϻ� ����
*	\param b ��� ���� �׵θ��� �Ķ��� ����
*	\param width ��� ���� �׵θ��� �β�
*/
void EgBox::Render(double r, double g, double b, double width)
{
	// ���� ��ǥ��: ����(���)
	glPushMatrix();
	{
		// ��� ������ �𵨸� ��ǥ��� ��ȯ
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
*	\brief ��ü�� Ŭ���� �̸��� ��ȯ�Ѵ�.
*
*	\return ��ü�� Ŭ���� �̸��� ��ȯ�Ѵ�.
*/
std::string EgMesh::GetClassName() 
{ 
	return std::string("EgMesh");
}

/*!
*	\brief �ڽ��� �����Ѵ�.
*
*	\return �ڽ��� ������ ��ü�� �����͸� ��ȯ�Ѵ�.
*/
EgMesh *EgMesh::CopyObject() 
{ 
	return new EgMesh(*this); 
}

/*!
*	\brief ��ü�� ���Ե� �޽��� �����͸� ��ȯ�Ѵ�.
*
*	\return ��ü�� ���Ե� �޽��� �����͸� ��ȯ�Ѵ�.
*/
EgMesh *EgMesh::GetMesh()
{
	return this;
}

/*!
*	\brief ���� ��ǥ�迡�� ǥ���� ��ü�� �𵨸� ��ǥ�踦 ��ȯ�Ѵ�.
*
*	\return ���� ��ǥ�迡�� ǥ���� ��ü�� �𵨸� ��ǥ�踦 ��ȯ�Ѵ�.
*/
EgTrans EgMesh::GetTrans()
{
	return m_MC;
}

/*!
*	\brief ���õ� ��ü�� �����Ѵ�.
*
*	\param EC ���� ���� ��ǥ��
*	\param t ������ ��ü�� �̵� ����
*	\param q ������ ��ü�� ȸ�� ����
*	\param s ������ ��ü�� ������ ����
*	\param bFirst ���� ��ü�� �� ù ��° ��ü�� ���� ���� ����
*/
void EgMesh::Edit(EgTrans EC, EgVec3 t, EgQuat q, EgVec3 s, bool bFirst)
{
	// �޽��� �𵨸� ��ǥ�踦 �����Ѵ�.
	EgTrans EC2MC = inv(EC) * m_MC;
	m_MC = EC * EgTrans(t, q) * EC2MC;

	// ũ�� ��ȯ�� �����Ѵٸ�,
	if (s != EgVec3(1.0, 1.0, 1.0))
	{
		// �޽��� �� ������ ���Ͽ� ũ�� ��ȯ�� �����ϰ�,
		for (EgVertex *v : m_pVerts)
		{
			v->m_Pos[0] *= s[0];
			v->m_Pos[1] *= s[1];
			v->m_Pos[2] *= s[2];
		}

		// ��� ���ڿ� ���ؼ��� ũ�� ��ȯ�� �����Ѵ�.
		m_BndBox.m_MinPt[0] *= s[0];
		m_BndBox.m_MinPt[1] *= s[1];
		m_BndBox.m_MinPt[2] *= s[2];
		m_BndBox.m_MaxPt[0] *= s[0];
		m_BndBox.m_MaxPt[1] *= s[1];
		m_BndBox.m_MaxPt[2] *= s[2];

		// ������ ��ġ�� ����Ǿ� ���۸� �����Ѵ�.
		UpdateVertexBuf();
	}
}

/*!
*	\brief	�޽��� �𵨸� ��ǥ�踦 �����Ѵ�.
*
*	\param RefType ����� �𵨸� ��ǥ���� Ÿ��(0: ���� ��ǥ��, 1: AABB, 2: OBB)
*/
void EgMesh::UpdateMC(int RefType)
{
	if (RefType == 0)
	{
		// ���� ��ǥ�迡�� ǥ���� ������ ��ǥ�� ���ϰ�,
		for (EgVertex *v : m_pVerts)
			v->m_Pos = m_MC * v->m_Pos;
		
		// �𵨸� ��ǥ�踦 ���� ��ǥ��� �����ϰ�,
		m_MC = EgTrans();
	}
	else if (RefType == 1)
	{
		// ���� ��ǥ�踦 �𵨸� ��ǥ��� �����ϰ�,
		UpdateMC(0);

		// ��� ������ �߽��� ��ǥ�� ���ϰ�,
		EgPos p = m_BndBox.m_MinPt + (m_BndBox.m_MaxPt - m_BndBox.m_MinPt) * 0.5;

		// �޽��� �𵨸� ��ǥ�踦 �����Ѵ�.
		m_MC = EgTrans(cast_vec3(p), EgQuat());

		// ���ο� �𵨸� ��ǥ�迡�� ǥ���� ������ ��ǥ�� ����Ѵ�.
		EgTrans invMC = inv(m_MC);
		for (EgVertex *v : m_pVerts)
			v->m_Pos = invMC * v->m_Pos;
	}
	else if (RefType == 2)
	{
		// �ּ��� �м��� ���� ������ �����ϰ�,
		int NumVert = GetNumVert();
		EgMat Data(NumVert, 3), PCs(3, 3);
		EgVec Sigma(3), Mean(3);

		// ������ ������ ���Ͽ�,
		int r = 0;
		for (EgVertex *v : m_pVerts)
		{
			// ���� ��ǥ�迡�� ǥ���� ������ ��ǥ�� ���ϰ�,
			v->m_Pos = m_MC * v->m_Pos;

			// �� ���� ���·� ��Ŀ� �߰��� ��,
			Data.SetRowVec(r++, cast_vec(v->m_Pos));
		}

		// �ּ��� �м��� �����Ͽ�
		pca(Data, PCs, Sigma, Mean);

		// �𵨸� ��ǥ�踦 �����Ѵ�.
		EgVec3 origin = cast_vec3(Mean);
		EgVec3 x_axis = cast_vec3(PCs.GetColVec(0)).Normalize();
		EgVec3 y_axis = cast_vec3(PCs.GetColVec(1)).Normalize();
		EgQuat q(x_axis, y_axis);
		m_MC = EgTrans(origin, q);

		// �޽��� �𵨸� ��ǥ�迡�� ǥ���� ������ ��ǥ�� ����Ѵ�.
		EgTrans invMC = inv(m_MC);
		for (EgVertex *v : m_pVerts)
			v->m_Pos = invMC * v->m_Pos;
	}

	// ���� ���ۿ� ��� ���ڸ� ��������.
	UpdateVertexBuf();
	UpdateBndBox();
}

/*!
*	\brief	�޽��� ������ ���뿡 ���� �޽��� �����Ѵ�.
*/
void EgMesh::UpdateMesh()
{
}

/*!
*	\brief ���õ� ��ü�� ������ �Ѵ�.
*
*	\param r ������ red ����
*	\param g ������ green ����
*	\param b ������ blue ����
*/
void EgMesh::RenderForEdit(float r, float g, float b)
{
	m_BndBox.Render(r, g, b, 1.0);
}

/*!
*	\brief ������
*
*	\param Name ������ �޽��� �̸�
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
*	\brief ���� ������
*
*	\param cpy ����� �޽� ��ü
*/
EgMesh::EgMesh(const EgMesh &cpy)
{
	// �̸��� �𵨸� ��ǥ�踦 �����Ѵ�.
	m_Name = cpy.m_Name + "_copy";
	m_MC = cpy.m_MC;

	// ������ ����Ʈ�� �����Ѵ�.
	for (EgVertex *v : cpy.m_pVerts)
		AddVertex(new EgVertex(v->m_Pos));

	// �ؽ�ó ������ ����Ʈ�� �����Ѵ�.
	for (EgTexel *t : cpy.m_pTexels)
		AddTexel(new EgTexel(t->m_UV[0], t->m_UV[1]));

	// ������ ������ �����Ѵ�.
	for (EgNormal *n : cpy.m_pNormals)
		AddNormal(new EgNormal(n->m_Dir));

	// �޽��� ���Ե� �ؽ�ó�� �����Ѵ�.
	for (EgTexture *t : cpy.m_pTextures)
	{
		EgTexture *pTexture = ::create_texture(t->m_FileName);
		if (pTexture != NULL)
			AddTexture(pTexture);
	}

	// �޽��� ���Ե� ������ �����Ѵ�.
	for (EgMaterial *m : cpy.m_pMaterials)
	{
		// ������ �����Ͽ� �޽��� �߰��ϰ�,
		EgMaterial *pMtl = new EgMaterial(*m);
		AddMaterial(pMtl);

		// ������ �ؽ�ó�� �����Ѵ�.
		if (m->m_pTexture != NULL)
			pMtl->m_pTexture = GetTexture(m->m_pTexture->m_FileName.c_str());
	}
	
	// �ﰢ���� �����Ѵ�.
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
	
	// ������ ������踦 �����ϰ� 
	InitEdgeMate();

	// ������ �Ӽ��� �ʱ�ȭ �ϰ�
	m_RenderType = cpy.m_RenderType;
	m_ShadeType = cpy.m_ShadeType;
	m_VertexBuf.clear();
	m_NormalBuf.clear();
	m_TexelBuf.clear();

	// ��� ���ڸ� �����ϰ�
	m_BndBox = cpy.m_BndBox;
	m_BndBox.m_pMesh = this;

	// ���÷��� ����Ʈ�� �ʱ�ȭ �Ѵ�.
	m_DisplayListOfFaceId = 0;
	m_DisplayListOfBndry = 0;
}

/*!
*	\brief �Ҹ���
*/
EgMesh::~EgMesh()
{
	// ���ŵǴ� �޽��� �̸��� ����Ѵ�.
	printf("%s is deleted...\n\n", m_Name.c_str());

	// ����, �ؼ�, �븻, �ﰢ��, ����, �ؽ�ó ����Ʈ�� �����Ѵ�.
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

	// ���÷��� ����Ʈ�� �����Ѵ�. 
	if (m_DisplayListOfFaceId != 0)
		glDeleteLists(m_DisplayListOfFaceId, 1);
	if (m_DisplayListOfBndry != 0)
		glDeleteLists(m_DisplayListOfBndry, 1);
}

/*!
*	\brief �޽��� ������ �߰��Ѵ�.
*
*	\param pVert �߰��� ������ ���� ������[in]
*/
void EgMesh::AddVertex(EgVertex *pVert)
{
	// ������ �ε����� �����ϰ� ���� �迭�� �߰��Ѵ�.
	pVert->m_pMesh = this;
	pVert->m_Idx = (int)m_pVerts.size();
	m_pVerts.push_back(pVert);
}

/*!
*	\brief �޽��� �ؼ��� �߰��Ѵ�.
*
*	\param pTexel �߰��� �ؼ��� ���� ������[in]
*/
void EgMesh::AddTexel(EgTexel *pTexel)
{
	// �ؼ��� �ε����� �����ϰ� �ؼ� �迭�� �߰��Ѵ�.
	pTexel->m_Idx = (int)m_pTexels.size();
	m_pTexels.push_back(pTexel);
}

/*!
*	\brief �޽��� ������ �߰��Ѵ�.
*
*	\param pNormal �߰��� ������ ���� ������[in]
*/
void EgMesh::AddNormal(EgNormal *pNormal)
{
	// ������ �ε����� �����ϰ� ���� �迭�� �߰��Ѵ�.
	pNormal->m_Idx = (int)m_pNormals.size();
	m_pNormals.push_back(pNormal);
}

/*!
*	\brief �޽��� �ﰢ���� �߰��Ѵ�.
*
*	\param pFace �߰��� �ﰢ���� ���� ������[in]
*/
void EgMesh::AddFace(EgFace *pFace)
{
	// �ﰢ���� �ε����� �����ϰ� �ﰢ�� �迭�� �߰��Ѵ�.
	pFace->m_Idx = (int)m_pFaces.size();
	m_pFaces.push_back(pFace);
}

/*!
*	\brief �޽��� ������ �߰��Ѵ�.
*
*	\param pMaterial �߰��� ������ ���� ������[in]
*/
void EgMesh::AddMaterial(EgMaterial *pMaterial)
{
	// ������ �ε����� �����ϰ� ���� �迭�� �߰��Ѵ�.
	pMaterial->m_Idx = (int)m_pMaterials.size();
	m_pMaterials.push_back(pMaterial);
}

/*!
*	\brief �޽��� �ؽ�ó�� �߰��Ѵ�.
*
*	\param pTexture �߰��� �ؽ�ó�� ���� ������[in]
*/
void EgMesh::AddTexture(EgTexture *pTexture)
{
	m_pTextures.push_back(pTexture);
}

/*!
*	\brief	�޽��� �ﰢ��(3���� �������� ����)�� �����Ѵ�.
*	\note	�ﰢ�� ���� �������� ���� ������ �ʴ� ����/�ؼ�/������ ���ŵ��� �ʴ´�.
*			���� ����/�ؼ�/������ �����ϱ� ���ؼ��� delete_unused_*() �Լ��� �߰��� ȣ���ؾ� �Ѵ�.
*
*	\param	Faces ������ �ﰢ���� ������ ����Ʈ
*
*	\return ���� �ﰢ���� ������ ��ȯ�Ѵ�.
*/
int EgMesh::DeleteFaces(std::vector<EgFace *> &Faces)
{
	// ������ �ﰢ���� ���Ͽ�
	for (EgFace *f : Faces)
	{
		for (int i = 0; i < 3; ++i)
		{
			// ������ ������ ���Ͽ�
			EgEdge *e = f->GetEdge(i);

			// ��� ������ �ƴ϶�� �ݴ��� ������ ������ �����ϰ�,
			if (!e->IsBndry())
				e->m_pMate->m_pMate = NULL;

			// ���� ������ ���� ���տ��� ���� ������ �����Ѵ�.
			e->m_pVert->DeleteEdge(e);
		}

		// ������ �ﰢ������ ����Ѵ�.
		f->m_Idx = -1;
	}

	// ���ο� �ﰢ�� ����Ʈ�� �����ϰ�
	std::vector<EgFace *> NewFaces;

	// ������ ���� �ﰢ���� ���Ͽ�
	int fidx = 0;
	for (EgFace *f : m_pFaces)
	{
		if (f->m_Idx == -1) // ������ �ﰢ���̶�� �����Ѵ�.
			delete f;
		else				// ���� �ﰢ���� ���ο� ����Ʈ�� �߰��Ѵ�.
		{
			f->m_Idx = fidx++;
			NewFaces.push_back(f);			
		}
	}

	// �ﰢ���� ����Ʈ�� �����Ѵ�.
	m_pFaces = NewFaces;

	// ������ ���۸� �ʱ�ȭ �Ѵ�.
	m_VertexBuf.clear();
	m_NormalBuf.clear();
	m_TexelBuf.clear();

	// ���� �ﰢ���� ������ ��ȯ�Ѵ�.
	return (int)m_pFaces.size();
}

/*!
*	\brief	�޽� �������� �����Ѵ�.
*	\note	������ ���Ե� �ﰢ���鵵 �Բ� ���ŵȴ�.
*			���� ���� �������� ���� ������ �ʴ� �ؼ�/������ ���ŵ��� �ʴ´�.
*			���� �ؼ�/������ �����ϱ� ���ؼ��� delete_unused_*() �Լ��� �߰��� ȣ���ؾ� �Ѵ�.
*
*	\param Verts ������ ������ ������ ����Ʈ
*
*	\return ���� ������ ������ ��ȯ�Ѵ�.
*/
int EgMesh::DeleteVerts(std::vector<EgVertex *> &Verts)
{
	// ������ ���Ե� �ﰢ�� ����Ʈ�� �����Ѵ�.
	std::vector<EgFace *> Faces;

	// ������ �����ϴ� ��� �ﰢ���� ���Ѵ�.
	for (EgVertex *v : Verts)
	{
		// ������ �������� ����Ѵ�.
		v->m_Idx = -1;

		// ������ ������ ��� �ﰢ���� ���Ѵ�.
		std::vector<EgFace *> tmp;
		v->GetOneRingFace(tmp);
		for (EgFace *f : tmp)
			Faces.push_back(f);
	}

	// �ߺ��� �ﰢ�� ��ü�� �����Ѵ�.
	sort(Faces.begin(), Faces.end());
	Faces.erase(unique(Faces.begin(), Faces.end()), Faces.end());

	// ������ �ﰢ���� �����Ѵ�.
	DeleteFaces(Faces);

	// ���Ǵ� �������� ������ ���ο� ����Ʈ�� �����ϰ�,
	std::vector<EgVertex *> NewVerts;

	// ������ ������ ���Ͽ�
	for (EgVertex *v : m_pVerts)
	{
		if (v->m_Idx == -1) // ������ ������ �����ϰ�
			delete v;
		else // ���� �����̶�� ����Ʈ�� �߰��Ѵ�.
			NewVerts.push_back(v);
	}

	// ������ ����Ʈ�� ������Ʈ �Ѵ�.
	m_pVerts = NewVerts;
	int NumVert = UpdateVertIdx();

	// ���� ������ ���� ��ȯ�Ѵ�.
	return NumVert;
}

/*!
*	\brief	�޽��� ������ �����Ѵ�.
*	\note	������ ���Ե� �ﰢ���鵵 �Բ� ���ŵȴ�.
*			���� ���� �������� ���� ������ �ʴ� ����/�ؼ�/������ ���ŵ��� �ʴ´�.
*			���� ����/�ؼ�/������ �����ϱ� ���ؼ��� delete_unused_*() �Լ��� �߰��� ȣ���ؾ� �Ѵ�.
*
*	\param	pEdge ������ ������ ������ �迭
*
*	\return ���� �ﰢ���� ���� ��ȯ�Ѵ�.
*/
int EgMesh::DeleteEdges(std::vector<EgEdge *> &Edges)
{
	// ������ ������ ���Ե� �ﰢ�� ������ ���Ѵ�.
	std::vector<EgFace *> Faces;
	for (EgEdge *e : Edges)
		Faces.push_back(e->m_pFace);

	// �ߺ��� ��ü�� �����Ѵ�.
	sort(Faces.begin(), Faces.end());
	Faces.erase(unique(Faces.begin(), Faces.end()), Faces.end());

	// �ﰢ���� �����Ѵ�.
	return DeleteFaces(Faces);
}

/*!
*	\brief �޽� ������ �ε���(0, 1, 2, ...)�� �����Ѵ�.
*
*	\return ������ ������ ��ȯ�Ѵ�.
*/
int EgMesh::UpdateVertIdx()
{
	int vidx = 0;
	for (EgVertex *v : m_pVerts)
		v->m_Idx = vidx++;

	return vidx;
}

/*!
*	\brief �޽����� �ؼ��� �ε����� �����Ѵ�
*
*	\return �ؼ��� ������ ��ȯ�Ѵ�.
*/
int EgMesh::UpdateTexIdx()
{
	int tidx = 0;
	for (EgTexel *t : m_pTexels)
		t->m_Idx = tidx++;

	return tidx;
}

/*!
*	\brief �޽����� ������ �ε����� �����Ѵ�
*
*	\return ������ ������ ��ȯ�Ѵ�.
*/
int EgMesh::UpdateNormalIdx()
{
	int nidx = 0;
	for (EgNormal *n : m_pNormals)
		n->m_Idx = nidx++;

	return nidx;
}

/*!
*	\brief �޽����� �ﰢ���� �ε����� �����Ѵ�.
*
*	\return �ﰢ���� ������ ��ȯ�Ѵ�.
*/
int EgMesh::UpdateFaceIdx()
{
	int fidx = 0;
	for (EgFace *f : m_pFaces)
		f->m_Idx = fidx++;

	return fidx;
}

/*!
*	\brief �޽����� ������ �ε����� �����Ѵ�.
*
*	\return ������ ������ ��ȯ�Ѵ�.
*/
int EgMesh::UpdateMaterialIdx()
{
	int midx = 0;
	for (EgMaterial *m : m_pMaterials)
		m->m_Idx = midx++;

	return midx;
}

/*!
*	\brief	�޽��� �ݴ��� ����(Mate) ������ �����Ѵ�.
*/
void EgMesh::InitEdgeMate()
{
	// �޽��� ��� ������ ����Ʈ ������ �ʱ�ȭ �Ѵ�.
	for (EgFace *f : m_pFaces)
	{
		f->GetEdge(0)->m_pMate = NULL;
		f->GetEdge(1)->m_pMate = NULL;
		f->GetEdge(2)->m_pMate = NULL;
	}

	// ������ ������ ���Ͽ�
	for (EgVertex *v : m_pVerts)
	{
		// �������� �����ϴ� ������ ������ ���Ͽ�
		for (EgEdge *e1 : v->m_pEdges)
		{
			// ������ ���� �������� �����ϴ� ������ ������ ���Ͽ�
			for (EgEdge *e2 : e1->m_pNext->m_pVert->m_pEdges)
			{
				// e2�� e1�� mate �������
				if (IS_MATE_EDGE(e1, e2))
				{
					// mate ������ �����Ѵ�.
					e1->m_pMate = e2;
					e2->m_pMate = e1;
					break;
				}
			}
		}
	}
}

/*!
*	\brief �������� �����ϴ� ������ �ݴ��� ���� ������ �����Ѵ�.

*	\param pVert ������ ������
*/
void EgMesh::UpdateEdgeMate(EgVertex *pVert)
{
	// �������� �����ϴ� ������ ���� e1�� ���Ͽ�
	for (EgEdge *e1 : pVert->m_pEdges)
	{
		// ���� ������ �������� �����ϴ� ������ ���� e2�� ���Ͽ�
		for (EgEdge *e2 : e1->m_pNext->m_pVert->m_pEdges)
		{
			// e1�� e2�� mate ������
			if (IS_MATE_EDGE(e1, e2))
			{
				// mate ������ �����Ѵ�.
				e1->m_pMate = e2;
				e2->m_pMate = e1;
				break;
			}
		}
	}
}

/*!
*	\brief	�޽��� �ﰢ�� ������ ����ϰ� �ִ��� �����Ѵ�.
*
*	\return	�ﰢ�� ������ ����ϸ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool EgMesh::IsFaceNormal()
{
	if (m_pNormals.size() == m_pFaces.size())
		return true;
	return false;
}

/*!
*	\brief	���� ���� �迭(m_pNormal)�� �����ϰ�, ���ο� m_pNormal�� �����Ѵ�.
*
*	\param	NormalType ������ ���¸� �����Ѵ�(ASIS: ���� ����, FLAT: �ﰢ�� ����, SMOOTH: ���� ����)
*/
void EgMesh::UpdateNormal(TypeNormal NormalType)
{
	// ���� ������ ����Ѵٸ�,
	if (NormalType == ASIS)
		NormalType = (GetNumNormal() == GetNumFace()) ? FLAT : SMOOTH;

	// ������ ���� ����Ʈ�� �����Ѵ�.
	for (EgNormal *n : m_pNormals)
		delete n;
	m_pNormals.clear();

	// �ﰢ�� ������ ����Ѵٸ�
	if (NormalType == FLAT)
	{
		// ������ �ﰢ���� ���Ͽ�
		for (EgFace *f : m_pFaces)
		{
			// ������ �����Ͽ� ����Ʈ�� �߰��Ѵ�.
			EgNormal *pNormal = new EgNormal();
			AddNormal(pNormal);

			// ������ ������ �ﰢ�� ������ �Ҵ��Ѵ�.
			pNormal->m_Dir = f->GetFaceNormal(true);
			f->GetEdge(0)->m_pNormal = pNormal;
			f->GetEdge(1)->m_pNormal = pNormal;
			f->GetEdge(2)->m_pNormal = pNormal;
		}
	}
	else  // ���� ������ ����Ѵٸ�
	{
		// ������ ������ ���Ͽ�
		for (EgVertex *v : m_pVerts)
		{
			// ���� ������ �����ϰ� ����Ʈ�� �߰��Ѵ�.
			EgNormal *pNormal = new EgNormal();
			AddNormal(pNormal);

			// �������� �����ϴ� ������ ������ ���Ͽ� ���� ������ �Ҵ��Ѵ�.
			for (EgEdge *e : v->m_pEdges)
				e->m_pNormal = pNormal;
		}

		// ������ �ﰢ���� ���Ͽ�
		for (EgFace *f : m_pFaces)
		{
			// �ﰢ���� ������ ����Ͽ�, ������ ������ ������ �����Ѵ�.
			EgVec3 N = f->GetFaceNormal(true);
			f->GetEdge(0)->m_pNormal->m_Dir += N;
			f->GetEdge(1)->m_pNormal->m_Dir += N;
			f->GetEdge(2)->m_pNormal->m_Dir += N;
		}

		// ������ ������ ����ȭ �Ѵ�.
		for (EgNormal *n : m_pNormals)
			n->m_Dir.Normalize();
	}

	// ���� ���۸� �ʱ�ȭ �Ѵ�.
	m_NormalBuf.clear();
}

/*!
*	\brief �޽��� ũ�⸦ �밢�� �������� ����ȭ �Ѵ�.
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

	// ��� ���� �밢���� ���̸� �ʱ�ȭ �Ѵ�.
	double size = norm(q - p);
	for (EgVertex *v : m_pVerts)
	{
		v->m_Pos[0] = v->m_Pos[0] / size;
		v->m_Pos[1] = v->m_Pos[1] / size;
		v->m_Pos[2] = v->m_Pos[2] / size;
	}
}

/*!
*	\brief �޽��� ������ ������ ��ȯ�Ѵ�.
*
*	\return �޽��� ������ ������ ��ȯ�Ѵ�.
*/
int EgMesh::GetNumVert()
{
	return (int)m_pVerts.size();
}

/*!
*	\brief �޽��� �ؼ��� ������ ��ȯ�Ѵ�.
*
*	\return �޽��� �ؼ��� ������ ��ȯ�Ѵ�.
*/
int EgMesh::GetNumTexel()
{
	return (int)m_pTexels.size();
}

/*!
*	\brief �޽��� ������ ������ ��ȯ�Ѵ�.
*
*	\return �޽��� ������ ������ ��ȯ�Ѵ�.
*/
int EgMesh::GetNumNormal()
{
	return (int)m_pNormals.size();
}

/*!
*	\brief �޽��� �ﰢ���� ������ ��ȯ�Ѵ�.
*
*	\return �޽��� �ﰢ���� ������ ��ȯ�Ѵ�.
*/
int EgMesh::GetNumFace()
{
	return (int)m_pFaces.size();
}

/*!
*	\brief �޽��� ������ ������ ��ȯ�Ѵ�.
*
*	\return �޽��� ������ ������ ��ȯ�Ѵ�.
*/
int EgMesh::GetNumMtrl()
{
	return (int)m_pMaterials.size();
}

/*!
*	\brief �޽��� �����ϴ� ������ �ּ� ���̸� ���Ѵ�.
*
*	\return ������ �ּ� ���̸� ��ȯ�Ѵ�.
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
*	\brief �޽��� �����ϴ� ������ �ִ� ���̸� ���Ѵ�.
*
*	\return ������ �ִ� ���̸� ��ȯ�Ѵ�.
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
*	\brief �޽��� �����ϴ� ������ ��� ���̸� ���Ѵ�.
*
*	\return ������ ��� ���̸� ��ȯ�Ѵ�.
*/
double EgMesh::GetAvgEdgeLen()
{
	double len = 0.0;
	for (EgFace *f : m_pFaces)
		len += f->GetPerimeter();

	return len / (GetNumFace() * 3.0);
}

/*!
*	\brief �޽��� Idx��° ������ �����͸� ã�´�.
*
*	\param Idx ������ �ε���(0���� ����)
*
*	\return �޽��� Idx��° ������ �����͸� ��ȯ�Ѵ�.
*/
EgVertex *EgMesh::GetVert(int Idx)
{
	if (m_pVerts.empty() || Idx < 0 || Idx > m_pVerts.size() - 1)
		return NULL;

	return m_pVerts[Idx];
}

/*!
*	\brief �޽��� Idx��° �ؼ��� �����͸� ã�´�.
*
*	\param Idx �ؼ��� �ε���(0���� ����)
*
*	\return �޽��� Idx��° �ؼ��� �����͸� ��ȯ�Ѵ�.
*/
EgTexel *EgMesh::GetTexel(int Idx)
{
	if (m_pTexels.empty() || Idx < 0 || Idx > m_pTexels.size() - 1)
		return NULL;

	return m_pTexels[Idx];
}

/*!
*	\brief �޽��� Idx��° ������ �����͸� ã�´�.
*
*	\param Idx ������ �ε���(0���� ����)
*
*	\return �޽��� Idx��° ������ �����͸� ��ȯ�Ѵ�.
*/
EgNormal *EgMesh::GetNormal(int Idx)
{
	if (m_pNormals.empty() || Idx < 0 || Idx > m_pNormals.size() - 1)
		return NULL;

	return m_pNormals[Idx];
}

/*!
*	\brief �޽��� Idx��° �ﰢ���� �����͸� ã�´�.
*
*	\param Idx �ﰢ���� �ε���(0���� ����)
*
*	\return �޽��� Idx��° �ﰢ���� �����͸� ��ȯ�Ѵ�.
*/
EgFace *EgMesh::GetFace(int Idx)
{
	if (m_pFaces.empty() || Idx < 0 || Idx > m_pFaces.size() - 1)
		return NULL;

	return m_pFaces[Idx];
}

/*!
*	\brief Idx ��° ������ ã�´�.
*
*	\param Idx ������ �ε���
*
*	\return Idx ��° ������ �����͸� ��ȯ�Ѵ�.
*/
EgMaterial *EgMesh::GetMaterial(int Idx)
{
	if (m_pMaterials.empty() || Idx < 0 || Idx > m_pMaterials.size() - 1)
		return NULL;

	return m_pMaterials[Idx];
}

/*!
*	\brief �־��� �̸��� ���� ������ ã�´�.
*
*	\param Name ������ �̸�
*
*	\return �־��� �̸��� ���� ������ �����͸� ��ȯ�Ѵ�.
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
*	\brief �־��� �̸��� ���� �ؽ�ó�� ã�´�.
*
*	\param Name �ؽ�ó ���ϸ�
*
*	\return �־��� �̸��� ���� �ؽ�ó�� �����͸� ��ȯ�Ѵ�.
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
*	\brief �޽��� �ؽ�ó ��� ���θ� �����Ѵ�.
*
*	\return �޽��� �ؽ�ó�� ����ϸ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool EgMesh::HasTexture()
{
	if (!m_pTexels.empty())
		return true;
	else
		return false;
}

/*!
*	\brief	�޽��� �ﰢ���� ���ο� ������ �����Ѵ�.
*
*	\param	pMtl ������ ������ ���� ������
*/
void EgMesh::SetMaterial(EgMaterial *pMtl)
{
	for (EgFace *f : m_pFaces)
		f->m_pMaterial = pMtl;

	// ���� ���۸� �ʱ�ȭ �Ѵ�.
	m_VertexBuf.clear();
}

/*!
*	\brief	�޽� ������ ������ ���콺 ����� �̿��Ͽ� �����Ѵ�.
*
*	\param	Ks[in] ������ ���� ���콺 ���
*	\param	Range[in] ����ȭ(���)�� ���콺 ����� ����(0 ~ 1.0)
*/
void EgMesh::SetVertColorByGaussCurvature(std::vector<double> &Ks, double Range)
{
	// �޽��� ���̵��� ������ ������ �����Ѵ�.
	m_ShadeType = SHADE_BY_VERT_COLOR;

	// ��� ����� ���� ����� �����Ͽ� �����Ѵ�.
	std::vector<double> Pos, Neg;
	for (int i = 0; i < NUM(Ks); ++i)
	{
		if (Ks[i] > 0.0)
			Pos.push_back(Ks[i]);
		else
			Neg.push_back(Ks[i]);
	}

	// ����� ������������ �����Ѵ�.
	sort(Pos.begin(), Pos.end());
	sort(Neg.begin(), Neg.end());

	// ������� �Է� ������ �ּҰ��� �ִ밪�� ���ϰ� ����Ѵ�. 
	double NegMin = (Neg.empty()) ? 0.0 : Neg[(int)((Neg.size() - 1) * (1.0 - Range))];
	double PosMax = (Pos.empty()) ? 0.0 : Pos[(int)((Pos.size() - 1) * Range)];
	printf("\tSelected range = %.2lf%%\n", Range * 100);
	printf("\tColor mapping range = [%.10lf(Red) ~ %.10lf(Green)]\n", NegMin, 0.0);
	printf("\tColor mapping range = [%.10lf(Green) ~ %.10lf(Blue)]\n\n", 0.0, PosMax);

	// ������ ������ ���Ͽ�
	for (size_t i = 0; i < Ks.size(); ++i)
	{
		// ���� ����� Red ~ Green���� ������ �����Ͽ�,
		if (Ks[i] < 0.0)
			m_pVerts[i]->m_Color = get_color_map(Ks[i], NegMin, 0.0, EgVec3(1.0, 0.0, 0.0), EgVec3(0.0, 1.0, 0.0));			
		else // ��� ����� Green ~ Blue���� ������ �����ϰ�,
			m_pVerts[i]->m_Color = get_color_map(Ks[i], 0.0, PosMax, EgVec3(0.0, 1.0, 0.0), EgVec3(0.0, 0.0, 1.0));			
	}
}

/*!
*	\brief	�޽� ������ ������ ��� ����� ���밪�� �̿��Ͽ� �����Ѵ�.
*
*	\param	Hs[in] ������ ���� ��� ����� ���밪
*	\param	Range[in] ����ȭ(���)�� ��� ����� ����(0 ~ 1.0)
*/
void EgMesh::SetVertColorByMeanCurvature(std::vector<double> &Hs, double Range)
{
	// �޽��� ���̵��� ������ ������ �����Ѵ�.
	m_ShadeType = SHADE_BY_VERT_COLOR;

	// ��� ��� �迭�� �����Ͽ� ���� �������� �����Ѵ�.
	std::vector<double> Hs_Copy = Hs;
	sort(Hs_Copy.begin(), Hs_Copy.end());

	// ������� �Է� ������ �ּҰ��� �ִ밪�� ���ϰ� ����Ѵ�. 
	double Max = (Hs_Copy.empty()) ? 0.0 : Hs_Copy[(int)((Hs_Copy.size() - 1) * Range)];
	printf("\tSelected range = %.2lf%%\n", Range * 100);
	printf("\tColor mapping range = [%.10lf(Red) ~ %.10lf(Blue)]\n\n", 0.0, Max);

	// ������ ������ ���Ͽ�
	for (size_t i = 0; i < Hs.size(); ++i)
		m_pVerts[i]->m_Color = get_color_map(Hs[i], 0.0, Max, EgVec3(1.0, 0.0, 0.0), EgVec3(0.0, 1.0, 0.0), EgVec3(0.0, 0.0, 1.0));
}

/*!
*	\brief	�޽� ������ ������ �ϸ�� �ʵ带 �̿��Ͽ� �����Ѵ�.
*
*	\param	Fs ������ ���� �ϸ�� �ʵ��� ��
*	\param	bRainbow ������ Į�� �� ��뿩��
*/
void EgMesh::SetVertColorByHarmonicField(std::vector<double> &Fs, bool bRainbow)
{
	// �޽��� ���̵��� ������ ������ �����Ѵ�.
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
*	\brief	�޽� ������ ������ ������ �̿��Ͽ� �����Ѵ�.
*/
void EgMesh::SetVertColorByNormal()
{
	// �޽��� ���̵��� ������ ������ �����Ѵ�.
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
*	\brief	�޽� ������ ������ ����Ʈ ���� ���� �̿��Ͽ� �����Ѵ�.
*/
void EgMesh::SetVertColorByLight()
{
	m_ShadeType = SHADE_BY_LIGHT;
}

/*!
*	\brief �޽��� ������ �Ѵ�.
*/
void EgMesh::Render()
{
	// �ﰢ�� �ĸ� �������� �Ѵٸ�
	if (m_RenderType & RENDER_BACK_FACE)
	{
		glDisable(GL_CULL_FACE);
	}
	else // �ﰢ�� �ĸ� �������� ���� ������
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	// ���� ��ǥ�迡��
	glPushMatrix();
	{
		// ��ü�� �𵨸� ��ǥ����� ��ȯ�ϰ�,
		glMultMatrixd(cast_arr(cast_mat(m_MC)));

		// �������� ������ �Ѵٸ�
		if (m_RenderType & RENDER_POLYGON)
		{
			// ���� ����� ���� �𵨷� ������ �ϰ�
			if (m_ShadeType == SHADE_BY_LIGHT)
				RenderPolygon();

			// �ؽ�ó ����� �ؽ�ó ������ �ϰ�
			if (m_ShadeType == SHADE_BY_TEXTURE)
				RenderTexture();

			// ������ ���� ����� ������ ������ �������Ѵ�.
			if (m_ShadeType == SHADE_BY_VERT_COLOR)
				RenderVertColor();
		}

		// �߰������� ����Ʈ�� �������� �Ѵ�.
		if (m_RenderType & RENDER_POINT)
			RenderPoint();

		// �߰������� ���̾��������� ������ �Ѵ�.
		if (m_RenderType & RENDER_WIRE)
			RenderWire();

		// �߰������� �޽��� ���(Ȧ)�� ������ �Ѵ�.
		if (m_DisplayListOfBndry != 0)
			glCallList(m_DisplayListOfBndry);
	}
	glPopMatrix();
}

/*!
*	\brief �޽��� �ﰢ������ ������ �Ѵ�.
*/
void EgMesh::RenderPolygon()
{
	// ������ ���� ���۸� ����� �Ѵ�.
	if (m_VertexBuf.empty())
		UpdateVertexBuf();
	if (m_NormalBuf.empty())
		UpdateNormalBuf();

	// ������ ������ Ȱ��ȭ �ϰ�,
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	// ������ �븻 ���۸� �����ϰ�,
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	// ������ ���� ������ �׷��� ������ �Ѵ�.
	static EgMaterial Mtl("default");
	std::map<std::string, std::vector<float>>::iterator it0 = m_VertexBuf.begin();
	std::map<std::string, std::vector<float>>::iterator it1 = m_NormalBuf.begin();
	for (; it0 != m_VertexBuf.end(); it0++, it1++)
	{
		// ������ Ȱ��ȭ �Ͽ�
		EgMaterial *pMtl = GetMaterial(it0->first);
		if (pMtl == NULL)
			pMtl = &Mtl;
		pMtl->Enable();

		// ����, ���� �迭�� �����Ͽ� �ﰢ������ ������ �ϰ�
		glVertexPointer(3, GL_FLOAT, 0, &(it0->second[0]));
		glNormalPointer(GL_FLOAT, 0, &(it1->second[0]));
		glDrawArrays(GL_TRIANGLES, 0, (int)it0->second.size() / 3);

		// ������ ��Ȱ��ȭ �Ѵ�.
		pMtl->Disable();
	}

	// ������ �븻 ���۸� ��Ȱ��ȭ �Ѵ�.
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	// ������ �������� ��Ȱ��ȭ �Ѵ�.
	glDisable(GL_POLYGON_OFFSET_FILL);
}

/*!
*	\brief �޽��� �ؽ�ó�� ������ �Ѵ�.
*/
void EgMesh::RenderTexture()
{
	// ����, ����, �ؽ�ó ���۸� ����� �Ѵ�.
	if (m_VertexBuf.empty())
		UpdateVertexBuf();
	if (m_NormalBuf.empty())
		UpdateNormalBuf();
	if (m_TexelBuf.empty())
		UpdateTexelBuf();

	// �ؽ�ó ���� �ɼ��� �����ϰ�
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// ������ ������ Ȱ��ȭ �ϰ�
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	// ����, ����, �ؽ�ó ���۸� Ȱ��ȭ �Ѵ�
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// ������ ���� ������ �׷��� ������ �Ѵ�.
	static EgMaterial Mtl("default");
	std::map<std::string, std::vector<float>>::iterator it0 = m_VertexBuf.begin();
	std::map<std::string, std::vector<float>>::iterator it1 = m_NormalBuf.begin();
	std::map<std::string, std::vector<float>>::iterator it2 = m_TexelBuf.begin();
	for (; it0 != m_VertexBuf.end(); it0++, it1++, it2++)
	{
		// ������ Ȱ��ȭ �Ͽ�
		EgMaterial *pMtl = GetMaterial(it0->first);
		if (pMtl == NULL)
			pMtl = &Mtl;
		pMtl->Enable();
		
		if (pMtl->m_pTexture != NULL)
			glBindTexture(GL_TEXTURE_2D, pMtl->m_pTexture->m_TexId);

		// ����, ���� �迭�� �����Ͽ� �ﰢ������ ������ �ϰ�
		glVertexPointer(3, GL_FLOAT, 0, &(it0->second[0]));
		glNormalPointer(GL_FLOAT, 0, &(it1->second[0]));
		glTexCoordPointer(2, GL_FLOAT, 0, &(it2->second[0]));
		glDrawArrays(GL_TRIANGLES, 0, (int)it0->second.size() / 3);

		// ������ ��Ȱ��ȭ �Ѵ�.
		pMtl->Disable();
	}

	// ������ �븻 ���۸� �����Ѵ�.
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// ������ �������� ��Ȱ��ȭ �Ѵ�.
	glDisable(GL_POLYGON_OFFSET_FILL);

	// �ؽ�ó ����� ��Ȱ��ȭ �Ѵ�.
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

/*!
*	\brief ������ �������� ���� �������Ѵ�.
*/
void EgMesh::RenderVertColor()
{
	// ������ ��Ȱ��ȭ �Ѵ�.
	glDisable(GL_LIGHTING);

	// ������ ������ Ȱ��ȭ �ϰ�,
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	// ������ �ﰢ���� ������ �Ѵ�.
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

	// ������ �ٽ� Ȱ��ȭ �Ѵ�.
	glPointSize(1.0f);
	glEnable(GL_LIGHTING);

	// ������ �������� ��Ȱ��ȭ �Ѵ�.
	glDisable(GL_POLYGON_OFFSET_FILL);
}

/*!
*	\brief �޽��� ������ ������ �Ѵ�.
*/
void EgMesh::RenderPoint()
{
	// ������ ��Ȱ��ȭ �ϰ� ���� ����� ũ�⸦ �����Ѵ�.
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(3.0f);

	// ������ ������ ������ �Ѵ�.
	glBegin(GL_POINTS);
	{
		for (EgVertex *v : m_pVerts)
			glVertex3dv(v->m_Pos.m_Coords);
	}
	glEnd();

	// ���� ũ�⸦ �ʱ�ȭ �ϰ� ������ �ٽ� Ȱ��ȭ �Ѵ�.
	glPointSize(1.0f);
	glEnable(GL_LIGHTING);
}

/*!
*	\brief �޽��� ���̾����������� ������ �Ѵ�.
*/
void EgMesh::RenderWire()
{
	// ������ ���� ���۸� ����� �Ѵ�.
	if (m_VertexBuf.empty())
		UpdateVertexBuf();

	// ������ ������ ��带 �������� �����ϰ�, ������ ����, ������ ������ ��,
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_LIGHTING);
	//glColor4f(227 / 255.0f, 220 / 255.0f, 193 / 255.0f, 1.0f);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glLineWidth(1.0f);

	// ���� ���۸� Ȱ��ȭ �ϰ�
	glEnableClientState(GL_VERTEX_ARRAY);

	// ������ ���� ������ �׷��� ������ �Ѵ�.
	std::map<std::string, std::vector<float>>::iterator it;
	for (it = m_VertexBuf.begin(); it != m_VertexBuf.end(); it++)
	{
		// ���� �迭�� �����Ͽ� �ﰢ������ ������ �Ѵ�.
		glVertexPointer(3, GL_FLOAT, 0, &(it->second[0]));
		glDrawArrays(GL_TRIANGLES, 0, (int)it->second.size() / 3);
	}

	// ���� ���۸� ��Ȱ��ȭ �Ѵ�
	glDisableClientState(GL_VERTEX_ARRAY);

	// ������ ������ ��带 �缳���ϰ�, ������ Ȱ��ȭ �Ѵ�.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
}

/*!
*	\brief �޽��� ��踦 �������ϴ� ���÷��� ����Ʈ�� �����Ѵ�.
*/
void EgMesh::CreateDisplayListOfBndry()
{
	// ���� ���÷��� ����Ʈ�� �����Ѵ�.
	DeleteDisplayListOfBndry();	
	
	// ��� ������ ����Ʈ�� ���Ͽ�,
	std::vector<std::vector<EgVertex *>> BndryVerts;

	// Non-manifold ������ �����ϰų�, ������ Ȧ�� �Ҹ�Ȯ �ϴٸ�
	if (::get_bndry_verts(this, BndryVerts, false, true) < 0)
	{
		// ���ο� ���÷��� ����Ʈ�� �����Ѵ�.
		m_DisplayListOfBndry = glGenLists(1);
		glNewList(m_DisplayListOfBndry, GL_COMPILE_AND_EXECUTE);
		{
			// ������ ��Ȱ��ȭ �ϰ� ���� ����� �β��� �����Ѵ�.
			glDisable(GL_LIGHTING);
			glColor3f(1.0f, 0.0f, 0.0f);
			glLineWidth(2.5);

			// ��� ������ ������ �Ѵ�.
			glBegin(GL_LINES);
			for (int i = 0; i < (int)BndryVerts[0].size(); i += 2)
			{
				glVertex3dv(BndryVerts[0][i]->m_Pos.m_Coords);
				glVertex3dv(BndryVerts[0][i + 1]->m_Pos.m_Coords);
			}
			glEnd();

			// ���� �β��� �ʱ�ȭ �ϰ� ������ �ٽ� Ȱ��ȭ �Ѵ�.
			glLineWidth(1.0);
			glEnable(GL_LIGHTING);
		}
		glEndList();

	}
	else // �������� Ȧ�� �����Ѵٸ� ������ ���(Ȧ)�� ���� �ٸ� �������� ������ �Ѵ�.
	{
		// ���ο� ���÷��� ����Ʈ�� �����Ѵ�.
		int cidx = 0;
		m_DisplayListOfBndry = glGenLists(1);
		glNewList(m_DisplayListOfBndry, GL_COMPILE_AND_EXECUTE);
		{
			// ������ ��Ȱ��ȭ �ϰ� ���� ����� �β��� �����Ѵ�.
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

			// ���� �β��� �ʱ�ȭ �ϰ� ������ �ٽ� Ȱ��ȭ �Ѵ�.
			glLineWidth(1.0);
			glEnable(GL_LIGHTING);

		}
		glEndList();
	}
}

/*!
*	\brief �޽��� ��踦 �������ϴ� ���÷��� ����Ʈ�� �����Ѵ�.
*/
void EgMesh::DeleteDisplayListOfBndry()
{
	if (m_DisplayListOfBndry != 0)
		glDeleteLists(m_DisplayListOfBndry, 1);
	m_DisplayListOfBndry = 0;
}

/*!
*	\brief ���� ���۸� �����Ѵ�.
*/
void EgMesh::UpdateVertexBuf()
{
	// ������ ���� ���۸� �ʱ�ȭ �Ѵ�.
	m_VertexBuf.clear();

	// ������ �ﰢ���� ���Ͽ�
	for (EgFace *f : m_pFaces)
	{
		// �������� ���Ͽ�
		std::string MtlName = (f->m_pMaterial == NULL) ? "default" : f->m_pMaterial->m_Name;

		// �ﰢ���� �� ������ ��ǥ�� ������ ���� �����Ͽ� ���� ���ۿ� �����Ѵ�.
		for (int i = 0; i < 3; ++i)
		{
			EgVertex *v = f->GetVertex(i);
			m_VertexBuf[MtlName].push_back((float)v->m_Pos[0]);
			m_VertexBuf[MtlName].push_back((float)v->m_Pos[1]);
			m_VertexBuf[MtlName].push_back((float)v->m_Pos[2]);
		}
	}

	// ���� ���÷��� ����Ʈ�� �����Ѵ�.
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
*	\brief ���� ���۸� �����Ѵ�.
*/
void EgMesh::UpdateNormalBuf()
{
	// ������ ���� ���۸� �ʱ�ȭ �Ѵ�.
	m_NormalBuf.clear();

	// ������ �ﰢ���� ���Ͽ�
	for (EgFace *f : m_pFaces)
	{
		// �������� ���Ͽ�
		std::string MtlName = (f->m_pMaterial == NULL) ? "default" : f->m_pMaterial->m_Name;

		// �ﰢ���� �� ������ ��ǥ�� ������ ���� �����Ͽ� ���� ���ۿ� �����Ѵ�.
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
*	\brief �ؽ�ó ���۸� �����Ѵ�.
*/
void EgMesh::UpdateTexelBuf()
{
	// �ؽ�ó ��ǥ�� ���ٸ� �����Ѵ�.
	if (m_pTexels.empty())
		return;

	// ���� �ؽ�ó ���۸� �ʱ�ȭ �Ѵ�.
	m_TexelBuf.clear();

	// ������ �ﰢ���� ���Ͽ�
	for (EgFace *f : m_pFaces)
	{
		// �������� ���Ͽ�
		std::string MtlName = (f->m_pMaterial == NULL) ? "default" : f->m_pMaterial->m_Name;

		// �ﰢ���� �� �ؽ�ó ��ǥ�� ������ ���� �����Ͽ� �ؽ�ó ���ۿ� �����Ѵ�.
		for (int i = 0; i < 3; ++i)
		{
			EgTexel *t = f->GetTexel(i);
			m_TexelBuf[MtlName].push_back((float)t->m_UV[0]);
			m_TexelBuf[MtlName].push_back((float)t->m_UV[1]);
		}
	}
}

/*!
*	\brief �޽��� �ѷ��δ� ��� ���ڸ� �����Ѵ�.
*/
void EgMesh::UpdateBndBox()
{
	// �޽� ������ ���ٸ� �����Ѵ�.
	if (GetNumVert() == 0)
	{
		m_BndBox.m_MinPt = m_BndBox.m_MaxPt = EgPos(0.0, 0.0, 0.0);
		return;
	}

	// �޽� ������ �� �࿡ ���� �ִ�/�ּ� ��ǥ�� ���Ѵ�.
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
*	\biref	���� ����(Polyline)�� �� ������ ����� ����Ѵ�.
*	\note	������ ������: 2020-08-01
*	\note	�� ������ ����� 0���� ���Ѵ�.
*
*	\param	Pts[in]					���� ������ ���� ����Ʈ
*	\param	Ks[out]					�� ������ ����� ����ȴ�.
*	\param	bSignedCurvature[in]	��� ��ȣ�� ����ϸ� true, �ƴϸ� false
*	\param	Up[in]					���� ������ ���� ����� ���� ����
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
*	\brief	������ �߽����� �־��� �ݰ� �̳��� �ﰢ���� ������ ����Ѵ�.
*
*	\param	pVert[in]	�߽� ������ ���� ������
*	\param	Radius[in]	�ݰ�
*
*	\return	������ ������ ��ȯ�Ѵ�.
*/
double get_local_area(EgVertex *pVert, double Radius)
{
	// ���� �Ÿ����� �̿��Ͽ� ���� ������ ���Ѵ�.
	std::vector<double> Distances;
	EgMesh *pMesh = pVert->GetMesh();
	::get_geodesic_dist_field(pMesh, pVert, Distances, Radius);

	// ���� ���� ���� �ﰢ���� ������ ���Ͽ� �����Ѵ�.
	double area = 0.0;
	for (EgFace *f : pMesh->m_pFaces)
	{
		// �ﰢ�� �� ���� iso-value�� ���Ͽ�
		int *vidx = f->GetVertexIdxs();
		double d0 = Distances[vidx[0]];
		double d1 = Distances[vidx[1]];
		double d2 = Distances[vidx[2]];

		// �ݰ� ���� �ﰢ���̶�� ��ŵ�Ѵ�.
		if (d0 >= Radius && d1 >= Radius && d2 >= Radius)
			continue;

		// �ݰ� ���� �ﰢ���̶�� ��ü ������ ���Ѵ�.
		if (d0 <= Radius && d1 <= Radius && d2 <= Radius)
		{
			area += f->GetArea();
			continue;
		}

		// �ﰢ���� ��迡 ��ģ ���, �ﰢ�� �� ������ ��ǥ�� ���ϰ�
		EgPos v0 = pMesh->m_pVerts[vidx[0]]->m_Pos;
		EgPos v1 = pMesh->m_pVerts[vidx[1]]->m_Pos;
		EgPos v2 = pMesh->m_pVerts[vidx[2]]->m_Pos;

		// ��谡 v0v1, v0v2 ������ ������ ���
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

		// ��谡 v1v0, v1v2 ������ ������ ���
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

		// ��谡 v2v0, v2v1 ������ ������ ���
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

	// ������ �ﰢ���� ������ ��ȯ�Ѵ�.
	return area;
}
