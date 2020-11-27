#include "pch.h"
#include "SizeKoreaUtil.h"


EgSlicer theSlicer;
EgSizer theSizer;

/**********************/
/* EgSlicer Ŭ���� ���� */
/**********************/
/*!
*	\brief	������
*	\note	������ ������: 2020-07-08
*/
EgSlicer::EgSlicer()
{
}

/*!
*	\brief	������
*	\note	������ ������: 2020-07-08
*
*	\param	CutPln[in]	���� ���
*/
EgSlicer::EgSlicer(EgPlane CutPln)
{
	m_CutPln = CutPln;
}

/*!
*	\brief	���� ������
*	\note	������ ������: 2020-07-08
*
*	\param	cpy[in]		����� ��ü
*/
EgSlicer::EgSlicer(const EgSlicer &cpy)
{
	m_CutPln = cpy.m_CutPln;
	m_CutLines = cpy.m_CutLines;
}

/*!
*	\brief	�Ҹ���
*	\note	������ ������: 2020-07-08
*/
EgSlicer::~EgSlicer()
{
}

/*!
*	\brief	���� ������
*	\note	������ ������: 2020-07-08
*
*	\param	rhs[in]		���Ե� ��ü
*
*	\return	���Ե� �ڽ��� ��ȯ�Ѵ�.
*/
EgSlicer &EgSlicer::operator=(const EgSlicer &rhs)
{
	m_CutPln = rhs.m_CutPln;
	m_CutLines = rhs.m_CutLines;
	return *this;
}

/*!
*	\brief	���� ����� �����Ѵ�.
*	\note	������ ������: 2020-07-08
*
*	\param	CutPln[in]	������ ���� ���
*/
void EgSlicer::SetCutPlane(EgPlane CutPln)
{
	m_CutPln = CutPln;
}

/*!
*	\brief	�޽��� �����ϰ� ���ܼ��� ���̿� ���� ������������ �����Ѵ�.
*	\note	������ ������: 2020-07-08
*
*	\param	pMesh[in]	��� �޽��� ���� ������
*
*	\return	���� ��, ����� ���ܼ��� ���� ��ȯ�Ѵ�.
*/
int EgSlicer::Slice(EgMesh *pMesh)
{
	// ���ܸ�� ���ܼ� ������ ���Ѵ�.
	std::vector<std::vector<std::pair<EgEdge *, float>>> CutInfoList;
	::intersect_plane_mesh(pMesh, &m_CutPln, m_CutLines, CutInfoList);

	// ���ܼ��� ���̿� ���� ���� �������� �����Ѵ�.
	SortByLength(true);

	// ����� ���ܼ��� ���� ��ȯ�Ѵ�.
	return NUM(m_CutLines);
}

/*!
*	\brief	�޽��� �����ϰ� Ư�� �������� ���Ե� ���ܼ��� �����Ѵ�
*	\note	������ ������: 2020-07-26
*
*	\param	pMesh[in]	��� �޽��� ���� ������
*	\param	Pivot[in]	������ �߽���
*	\param	dx[in]		������ �߽������� x ���� ����
*	\param	dy[in]		������ �߽������� y ���� ����
*	\param	dz[in]		������ �߽������� z ���� ����
*
*	\return	���� ��, ����� ���ܼ��� ���� ��ȯ�Ѵ�.
*/
int EgSlicer::Slice(EgMesh *pMesh, EgPos Pivot, double dx, double dy, double dz)
{
	// �޽��� �����ϰ�,
	Slice(pMesh);

	// ������ �����ϰ�,
	double minX = Pivot[0] - dx;
	double maxX = Pivot[0] + dx;
	double minY = Pivot[1] - dy;
	double maxY = Pivot[1] + dy;
	double minZ = Pivot[2] - dz;
	double maxZ = Pivot[2] + dz;

	// �������� ���Ե� ���ܼ��� �����Ͽ�
	std::vector<std::vector<EgPos>> NewCutLines;
	for (std::vector<EgPos> &Cut : m_CutLines)
	{
		bool flag = true;
		for (EgPos &p : Cut)
		{
			if (p[0] < minX || p[0] > maxX)
				flag = false;

			if (p[1] < minY || p[1] > maxY)
				flag = false;

			if (p[2] < minZ || p[2] > maxZ)
				flag = false;

			if (flag == false)
				break;
		}

		if (flag == true)
			NewCutLines.push_back(Cut);
	}

	// ���ܼ��� �����ϰ�, ���ܼ��� ���� ��ȯ�Ѵ�.
	m_CutLines.clear();
	m_CutLines = NewCutLines;
	return NUM(m_CutLines);
}

/*!
*	\brief	��� ���� �� �־��� ������ �����ϴ� ���ܼ��� ���Ѵ�.
*	\note	������ ������: 2020-07-11
*
*	\param	pMesh[in]		��� �޽��� ���� ������
*	\param	PlnType[in]		���� ��� Ÿ��(0: Sagittal ���(X�� ����), 1: Transverse ���(Y�� ����), 2: Coronal ���(Z�� ����))
*	\param	Param[in]		Ž�� ���������� ����� ����� ��ġ(0.0 ~ 1.0)
*	\param	MinX			�޽� ��� ���� ���� X �� Ž�� ���� �ּҰ�(-1.0 ~ 1.0)
*	\param	MaxX			�޽� ��� ���� ���� X �� Ž�� ���� �ִ밪(-1.0 ~ 1.0)
*	\param	MinY			�޽� ��� ���� ���� Y �� Ž�� ���� �ּҰ�(-1.0 ~ 1.0)
*	\param	MaxY			�޽� ��� ���� ���� Y �� Ž�� ���� �ִ밪(-1.0 ~ 1.0)
*	\param	MinZ			�޽� ��� ���� ���� Z �� Ž�� ���� �ּҰ�(-1.0 ~ 1.0)
*	\param	MaxZ			�޽� ��� ���� ���� Z �� Ž�� ���� �ִ밪(-1.0 ~ 1.0)
*	\param	bClosedCut[in]	���� ���ܼ��� �������� ����
*
*	\return	Ž�� �������� ���Ե� ���ܼ��� ���� ��ȯ�Ѵ�.
*/
int EgSlicer::Slice(
	EgMesh *pMesh,
	int PlnType,
	double Param,
	double MinX, double MaxX,
	double MinY, double MaxY,
	double MinZ, double MaxZ,
	bool bClosedCut)
{
	// ���� ó���� �Ѵ�.
	if (PlnType < 0 || PlnType > 2)
	{
		printf("[Error]: SK_Cross_Section(...)...\n");
		return -1;
	}

	// Ž�� ������ ���ϰ�
	EgPos p = pMesh->m_BndBox.m_MinPt;
	EgPos q = pMesh->m_BndBox.m_MaxPt;
	double x0 = ((1.0 - MinX) * p[0] + (1.0 + MinX) * q[0]) * 0.5;
	double x1 = ((1.0 - MaxX) * p[0] + (1.0 + MaxX) * q[0]) * 0.5;
	double y0 = ((1.0 - MinY) * p[1] + (1.0 + MinY) * q[1]) * 0.5;
	double y1 = ((1.0 - MaxY) * p[1] + (1.0 + MaxY) * q[1]) * 0.5;
	double z0 = ((1.0 - MinZ) * p[2] + (1.0 + MinZ) * q[2]) * 0.5;
	double z1 = ((1.0 - MaxZ) * p[2] + (1.0 + MaxZ) * q[2]) * 0.5;

	// Ž�� �������� ��ü�� ��踦 ���Ѵ�.
	p.Set(INFINITY, INFINITY, INFINITY);
	q.Set(-INFINITY, -INFINITY, -INFINITY);
	for (EgVertex *v : pMesh->m_pVerts)
	{
		double x = v->m_Pos[0];
		double y = v->m_Pos[1];
		double z = v->m_Pos[2];
		if (x < x0 || x > x1 || y < y0 || y > y1 || z < z0 || z > z1)
			continue;
		p[0] = MIN(p[0], x);
		p[1] = MIN(p[1], y);
		p[2] = MIN(p[2], z);
		q[0] = MAX(q[0], x);
		q[1] = MAX(q[1], y);
		q[2] = MAX(q[2], z);
	}

	// ���� ����� �����Ѵ�.
	if (PlnType == 0)
		m_CutPln = EgPlane(EgVec3(1.0, 0.0, 0.0), lerp(p, q, Param));
	else if (PlnType == 1)
		m_CutPln = EgPlane(EgVec3(0.0, 1.0, 0.0), lerp(p, q, Param));
	else
		m_CutPln = EgPlane(EgVec3(0.0, 0.0, 1.0), lerp(p, q, Param));

	// ���ܼ��� ����Ʈ�� ���Ͽ�, Ž�� �������� ���ܼ��� �����Ѵ�.
	int NumCut = Slice(pMesh);
	std::vector<std::vector<EgPos>> SltCutList;
	for (int i = 0; i < NumCut; ++i)
	{
		if (bClosedCut && !IsClosed(i))
			continue;

		int NumPts = NUM(m_CutLines[i]);
		for (int j = 0; j < NumPts; ++j)
		{
			EgPos pt = m_CutLines[i][j];
			if (pt[0] > x0 && pt[0] < x1 && pt[1] > y0 && pt[1] < y1 && pt[2] > z0 && pt[2] < z1)
			{
				SltCutList.push_back(m_CutLines[i]);
				break;
			}
		}
	}

	// ������ �����ϴ� ���ܼ��� ���� ��ȯ�Ѵ�.
	m_CutLines = SltCutList;
	return NUM(m_CutLines);
}

/*!
*	\brief	��� ���� �� ���������� Convex hull�� ���Ѵ�.
*	\note	������ ������: 2020-09-07
*
*	\param	pMesh[in]		��� �޽��� ���� ������
*	\param	PlnType[in]		���� ��� Ÿ��(0: Sagittal ���(X�� ����), 1: Transverse ���(Y�� ����), 2: Coronal ���(Z�� ����))
*	\param	Param[in]		Ž�� �������� ��ü�� ���Ե� �������� ����� ����� ��ġ(0.0 ~ 1.0)
*	\param	MinX			�޽� ��� ���� ���� X �� Ž�� ���� �ּҰ�(-1.0 ~ 1.0)
*	\param	MaxX			�޽� ��� ���� ���� X �� Ž�� ���� �ִ밪(-1.0 ~ 1.0)
*	\param	MinY			�޽� ��� ���� ���� Y �� Ž�� ���� �ּҰ�(-1.0 ~ 1.0)
*	\param	MaxY			�޽� ��� ���� ���� Y �� Ž�� ���� �ִ밪(-1.0 ~ 1.0)
*	\param	MinZ			�޽� ��� ���� ���� Z �� Ž�� ���� �ּҰ�(-1.0 ~ 1.0)
*	\param	MaxZ			�޽� ��� ���� ���� Z �� Ž�� ���� �ִ밪(-1.0 ~ 1.0)
*
*	\return Convexhull�� ���̸� ��ȯ�Ѵ�.
*/
double EgSlicer::CvxSlice(EgMesh *pMesh, int PlnType, double Param, double MinX, double MaxX, double MinY, double MaxY, double MinZ, double MaxZ)
{
	// ���� ó���� �Ѵ�.
	if (PlnType < 0 || PlnType > 2)
	{
		_cprintf("[Error]: EgSlicer::CvxSlice()...\n");
		return -1.0;
	}

	// Ž�� ������ ���ϰ�
	EgPos p = pMesh->m_BndBox.m_MinPt;
	EgPos q = pMesh->m_BndBox.m_MaxPt;
	double x0 = ((1.0 - MinX) * p[0] + (1.0 + MinX) * q[0]) * 0.5;
	double x1 = ((1.0 - MaxX) * p[0] + (1.0 + MaxX) * q[0]) * 0.5;
	double y0 = ((1.0 - MinY) * p[1] + (1.0 + MinY) * q[1]) * 0.5;
	double y1 = ((1.0 - MaxY) * p[1] + (1.0 + MaxY) * q[1]) * 0.5;
	double z0 = ((1.0 - MinZ) * p[2] + (1.0 + MinZ) * q[2]) * 0.5;
	double z1 = ((1.0 - MaxZ) * p[2] + (1.0 + MaxZ) * q[2]) * 0.5;

	// Ž�� �������� ��ü�� ��踦 ���Ѵ�.
	p.Set(INFINITY, INFINITY, INFINITY);
	q.Set(-INFINITY, -INFINITY, -INFINITY);
	for (EgVertex *v : pMesh->m_pVerts)
	{
		double x = v->m_Pos[0];
		double y = v->m_Pos[1];
		double z = v->m_Pos[2];
		if (x < x0 || x > x1 || y < y0 || y > y1 || z < z0 || z > z1)
			continue;
		p[0] = MIN(p[0], x);
		p[1] = MIN(p[1], y);
		p[2] = MIN(p[2], z);
		q[0] = MAX(q[0], x);
		q[1] = MAX(q[1], y);
		q[2] = MAX(q[2], z);
	}
	x0 = p[0];
	y0 = p[1];
	z0 = p[2];
	x1 = q[0];
	y1 = q[1];
	z1 = q[2];

	// ���� ����� �����Ѵ�.
	if (PlnType == 0)
		m_CutPln = EgPlane(EgVec3(1.0, 0.0, 0.0), lerp(p, q, Param));
	else if (PlnType == 1)
		m_CutPln = EgPlane(EgVec3(0.0, 1.0, 0.0), lerp(p, q, Param));
	else
		m_CutPln = EgPlane(EgVec3(0.0, 0.0, 1.0), lerp(p, q, Param));

	// �������� ����Ʈ�� ���Ѵ�.
	std::vector<EgPos> CutPts;
	for (EgFace *f : pMesh->m_pFaces)
	{
		for (int i = 0; i < 3; ++i)
		{
			EgPos v = f->GetVertexPos(i);
			if (v[0] < x0 || v[0] > x1)
				break;
			if (v[1] < y0 || v[1] > y1)
				break;
			if (v[2] < z0 || v[2] > z1)
				break;

			EgEdge *e = f->GetEdge(i);
			float t;
			if (::intersect_plane_edge(&m_CutPln, e, &t))
			{
				EgPos pt = SV(e)->m_Pos + t * (EV(e)->m_Pos - SV(e)->m_Pos);
				CutPts.push_back(pt);
			}
		}
	}

	std::vector<EgPos> CvxPts;
	double len = SK_ConvexHull(&m_CutPln, CutPts, CvxPts);

	m_CutLines.clear();
	m_CutLines.push_back(CvxPts);

	return len;
}


/*!
*	\brief	��� ���� �� ���������� Convex hull�� ���Ѵ�.
*	\note	������ ������: 2020-09-07
*
*	\param	pMesh[in]		��� �޽��� ���� ������
*/
void EgSlicer::CvxSlice(EgMesh *pMesh)
{
	// �������� ����Ʈ�� ���Ѵ�.
	std::vector<EgPos> CutPts;
	for (EgFace *f : pMesh->m_pFaces)
	{
		for (int i = 0; i < 3; ++i)
		{
			EgEdge *e = f->GetEdge(i);
			float t;
			if (::intersect_plane_edge(&m_CutPln, e, &t))
			{
				EgPos pt = SV(e)->m_Pos + t * (EV(e)->m_Pos - SV(e)->m_Pos);
				CutPts.push_back(pt);
			}
		}
	}

	std::vector<EgPos> CvxPts;
	double len = SK_ConvexHull(&m_CutPln, CutPts, CvxPts);

	m_CutLines.clear();
	m_CutLines.push_back(CvxPts);
}

/*!
*	\brief	����� ���ܼ��� ���� ��ȯ�Ѵ�.
*	\note	������ ������: 2020-07-08
*
*	\return	����� ���ܼ��� ���� ��ȯ�Ѵ�.
*/
int EgSlicer::GetNumCutLine()
{
	return NUM(m_CutLines);
}

/*!
*	\brief	���ܼ��� ���̿� ���� �����Ѵ�.
*	\note	������ ������: 2020-07-08
*
*	\param	bDescend[in]	���� �ɼ�(��������: true, ��������: false)
*/
void EgSlicer::SortByLength(bool bDescend)
{
	// �� ���ܼ��� ���̸� ���Ѵ�.
	int NumCuts = GetNumCutLine();
	std::vector<double> Lengths;
	for (int i = 0; i < NumCuts; ++i)
		Lengths.push_back(GetLength(i));

	// ���ܼ��� �ɼǿ� ���� �����Ѵ�.
	for (int i = 0; i < NumCuts; ++i)
	{
		for (int j = i + 1; j < NumCuts; ++j)
		{
			// ���� �����̶��
			if (bDescend)
			{
				if (Lengths[i] < Lengths[j])
				{
					SWAP(double, Lengths[i], Lengths[j]);
					SWAP(std::vector<EgPos>, m_CutLines[i], m_CutLines[j]);
				}
			}
			else // ���� �����̶��
			{
				if (Lengths[i] > Lengths[j])
				{
					SWAP(double, Lengths[i], Lengths[j]);
					SWAP(std::vector<EgPos>, m_CutLines[i], m_CutLines[j]);
				}
			}
		}
	}
}

/*!
*	\brief	���ܼ��� �߽��� ���Ѵ�.
*	\note	������ ������: 2020-07-08
*
*	\param	idx[in]		���ܼ��� �ε���
*
*	\return	���ܼ��� �߽��� ��ȯ�Ѵ�.
*/
EgPos EgSlicer::GetCenterPos(int idx)
{
	int NumCuts = NUM(m_CutLines);
	if (idx < 0 || idx > NumCuts - 1)
	{
		printf("[Error]: EgSlicer::GetCenterPos(...)...\n");
		return EgPos();
	}

	EgVec3 CntPt;
	int NumPts = (int)m_CutLines[idx].size();
	for (int i = 0; i < NumPts; ++i)
		CntPt += cast_vec3(m_CutLines[idx][i]);

	CntPt /= (double)NumPts;
	return cast_pos(CntPt);
}

/*!
*	\brief	���ܼ��� ���̸� ���Ѵ�.
*	\note	������ ������: 2020-07-08
*
*	\param	idx[in]		���ܼ��� �ε���
*
*	\return	���ܼ��� ���̸� ��ȯ�Ѵ�.
*/
double EgSlicer::GetLength(int idx)
{
	int NumCuts = NUM(m_CutLines);
	if (idx < 0 || idx > NumCuts - 1)
	{
		printf("[Error]: EgSlicer::GetLength(...)...\n");
		return 0.0;
	}

	double Len = 0.0;
	int NumPts = (int)m_CutLines[idx].size();
	for (int i = 0; i < NumPts - 1; ++i)
		Len += dist(m_CutLines[idx][i], m_CutLines[idx][i + 1]);
	return Len;
}

/*!
*	\brief	���ܼ��� ���� �ִ��� �����Ѵ�.
*	\note	������ ������: 2020-07-08
*
*	\param	idx[in]		���ܼ��� �ε���
*
*	\return	���ܼ��� ���� ������ true, ���� ������ false�� ��ȯ�Ѵ�.
*/
bool EgSlicer::IsClosed(int idx)
{
	int NumCuts = NUM(m_CutLines);
	if (idx < 0 || idx > NumCuts - 1)
	{
		printf("[Error]: EgSlicer::IsClosed(...)...\n");
		return false;
	}

	int NumPts = (int)m_CutLines[idx].size();
	return (m_CutLines[idx][0] == m_CutLines[idx][NumPts - 1]) ? true : false;
}

/*!
*	\brief	���ܸ��� �� ���ܼ��� �������Ѵ�.
*	\note	������ ������: 2020-07-08
*/
void EgSlicer::Render()
{
	glDisable(GL_LIGHTING);
	{
		int NumCuts = GetNumCutLine();
		glLineWidth(3.0f);
		glPointSize(5.0f);
		for (int i = 0; i < NumCuts; ++i)
		{
			double t = (double)i / double(NumCuts);
			EgVec3 C = ::get_color_map_from_rainbow(t);
			glColor3dv(C.m_Coords);

			// ���ܼ��� ������ �Ѵ�.
			glBegin(GL_LINE_STRIP);
			for (int j = 0; j < NUM(m_CutLines[i]); ++j)
			{
				//double t = (double)j / NUM(m_CutLines[i]);
				//EgVec3 C = ::get_color_map_from_rainbow(t);
				//glColor3dv(C.m_Coords);
				glVertex3dv(m_CutLines[i][j].m_Coords);
			}
			glEnd();

			// ��ȣ�� ����Ͽ� ������������ �˻�.
			std::vector<EgPos> SortedPts;
			SK_Sort_By_Curvature(m_CutLines[i], SortedPts, false, true, theSlicer.m_CutPln.N(true));

			glPointSize(4.0f);
			glBegin(GL_POINTS);
			glColor3d(1.0, 1.0, 1.0);
			for (int j = 0; j < NUM(m_CutLines[i]); ++j)
				glVertex3dv(m_CutLines[i][j].m_Coords);
			glEnd();

			glPointSize(6.0f);
			glBegin(GL_POINTS);
			for (int j = 0; j < MIN(NUM(SortedPts), 10); ++j)
				//for (int j = 0; j < NUM(SortedPts); ++j)
			{
				//double t = (double)j / NUM(Pts);
				//EgVec3 C = ::get_color_map_from_rainbow(t);
				//glColor3dv(C.m_Coords);
				glColor3d(0.0, 0.0, 0.0);
				glVertex3dv(SortedPts[j].m_Coords);
			}
			glEnd();


			//// �������� �������Ѵ�.
			//glColor3f(0.0f, 0.0f, 0.0f);			
			//glBegin(GL_POINTS);
			//for (int j = 0; j < NUM(m_CutLines[i]); ++j)
			//	glVertex3dv(m_CutLines[i][j].m_Coords);
			//glEnd();
		}
		glLineWidth(1.0f);
		glPointSize(1.0f);
	}
	glEnable(GL_LIGHTING);
}

/*!
*	\brief	�ʱ�ȭ �Ѵ�.
*	\note	������ ������: 2020-07-13
*/
void EgSlicer::Reset()
{
	m_CutLines.clear();
}

//////////////////////////////////////////////////////////////////////////
static bool SK_Import_Obj_Mtl(EgMesh *pMesh, const char *fname)
{
	// ������ ���� ������ �����Ѵ�.
	EgMaterial *pMtl = NULL;

	// ������ ����, 
	FILE *fp;
	fopen_s(&fp, fname, "r");
	if (!fp) // �����ϸ� false�� ��ȯ�Ѵ�.
	{
		printf("\tImporting %s failed...\n", fname);
		return false;
	}
	else
		printf("\tImporting %s...\n", fname);

	// ������ ������ �� �ܾ �о�, tag �迭�� �����Ѵ�.
	char tag[256];
	while (fscanf_s(fp, "%s", tag, 256) != EOF)
	{
		// newmtl (new material) �±׶��,
		if (!strcmp(tag, "newmtl"))
		{
			// ������ �̸��� �а�,
			char tmp[256];
			fscanf_s(fp, "%s", tmp, 256);

			// ������ �����Ͽ�, �޽��� ���� ����Ʈ�� �߰��Ѵ�.
			pMtl = new EgMaterial(tmp);
			pMesh->AddMaterial(pMtl);
		}

		// Ka (ambient coefficients) �±׶��,
		if (!strcmp(tag, "Ka"))
		{
			// ambient ������ �о ������ �����Ѵ�.
			GLfloat c[3];
			fscanf_s(fp, "%f%f%f", &c[0], &c[1], &c[2]);
			pMtl->SetAmbient(c[0], c[1], c[2]);
		}

		// Kd (diffuse coefficients) �±׶��,
		if (!strcmp(tag, "Kd"))
		{
			// diffuse ������ �о ������ �����Ѵ�.
			GLfloat c[3];
			fscanf_s(fp, "%f%f%f", &c[0], &c[1], &c[2]);
			pMtl->SetDiffuse(c[0], c[1], c[2]);
		}

		// Ks (specular coefficients) �±׶��,
		if (!strcmp(tag, "Ks"))
		{
			// specular ������ �о ������ �����Ѵ�.
			GLfloat c[3];
			fscanf_s(fp, "%f%f%f", &c[0], &c[1], &c[2]);
			pMtl->SetSpecular(c[0], c[1], c[2]);
		}

		// map_Kd (diffuse texture file) �±׶��,
		if (!strcmp(tag, "map_Kd"))
		{
			// �ؽ�ó ������ �д´�.
			char tex_name[512];
			fscanf_s(fp, "%s", tex_name, 512);

			// �̹� �ε�� �ؽ�ó��� ������ �ؽ�ó�� �����Ѵ�.
			pMtl->m_pTexture = pMesh->GetTexture(tex_name);

			// ���ο� �ؽ�ó���
			if (pMtl->m_pTexture == NULL)
			{
				// ���ο� �ؽ�ó�� �����Ͽ� �޽��� �߰��ϰ�, ������ �����Ѵ�.
				EgTexture *pTexture = ::create_texture(tex_name);
				if (pTexture != NULL)
				{
					pMesh->AddTexture(pTexture);
					pMtl->SetTexture(pTexture);
				}
			}
		}
	}

	// ������ �ݴ´�.
	fclose(fp);
	return true;
}

EgMesh *SK_Import_Obj(CString FileName)
{
	// ������ ���� �����ϸ� false�� ��ȯ�Ѵ�.
	FILE *fp;
	std::string fname((LPCTSTR)FileName);
	fopen_s(&fp, fname.c_str(), "r");
	if (!fp)
	{
		_cprintf("Importing %s failed...\n", fname.c_str());
		return NULL;
	}
	else
		_cprintf("Importing %s...\n", fname.c_str());

	// ���� �۾� ���͸��� ���� ��η� ����Ѵ�.
	char cwd[1024] = { 0 };
	_getcwd(cwd, 1024);
	_cprintf("\tCurrent working directory = %s\n", cwd);

	// �� �˻� ���͸��� �̵��ϰ�, ���� ��η� ����Ѵ�.
	std::string model_dir = get_file_path(fname);
	_chdir(model_dir.c_str());
	char msd[1024] = { 0 };
	_getcwd(msd, 1024);
	_cprintf("\tModel directory = %s\n\n", msd);

	// ������ �̸�(��� �� Ȯ���� ����)�� ���Ͽ� �޽��� �����Ѵ�.
	std::string MeshName = get_file_name(fname);
	EgMesh *pMesh = ::create_mesh(MeshName);
	_cprintf("\tMesh name = %s\n", MeshName.c_str());

	// �ε��� �ʿ��� �ӽ� �������� �����Ѵ�.
	char tag[1024];
	bool bTexture = false;
	bool bNormal = false;
	std::string GroupName;
	EgMaterial *pCurrMtl = NULL;

	// ������ ������ �� �ܾ �����鼭 tag �迭�� �����Ѵ�.
	while (fscanf_s(fp, "%s", tag, 1024) != EOF)
	{
		// mtllib (material library) �±׶��,
		if (!strcmp(tag, "mtllib"))
		{
			// ���� ���ϸ�(�ַ� ��� ���)�� ���Ͽ�
			char mtl_fname[1024];
			fscanf_s(fp, "%s", mtl_fname, 1024);

			// ���� ������ �д´�. 
			SK_Import_Obj_Mtl(pMesh, mtl_fname);
		}

		// v (vertex) �±׶��,
		if (!strcmp(tag, "v"))
		{
			// ��ǥ(x, y, z)�� �о� ������ �����ϰ� �޽��� �߰��Ѵ�.
			double x, y, z;
			fscanf_s(fp, "%lf%lf%lf", &x, &y, &z);
			EgVertex *pVert = ::create_vertex(x, y, z);
			pMesh->AddVertex(pVert);
		}

		// vt (vertex texture) �±׶��,
		if (!strcmp(tag, "vt"))
		{
			// �ؽ�ó ��ǥ(u, v)�� �о� �ؼ��� �����ϰ� �޽��� �߰��Ѵ�.
			bTexture = true;
			double u, v;
			fscanf_s(fp, "%lf%lf", &u, &v);
			EgTexel *pTexel = ::create_texel(u, v);
			pMesh->AddTexel(pTexel);
		}

		// vn (vertex normal) �±׶��,
		if (!strcmp(tag, "vn"))
		{
			// ������ ����(nx, ny, nz)�� �о� ������ �����ϰ� �޽��� �߰��Ѵ�.
			bNormal = true;
			double nx, ny, nz;
			fscanf_s(fp, "%lf%lf%lf", &nx, &ny, &nz);
			EgNormal *pNormal = ::create_normal(nx, ny, nz);
			pMesh->AddNormal(pNormal);
		}

		// g (group) �±׶��,
		if (!strcmp(tag, "g"))
		{
			// �׷��� �̸��� �д´�.
			char grp_name[256];
			fscanf_s(fp, "%s", grp_name, 256);
			GroupName = std::string(grp_name);
		}

		// usemtl (use material) �±׶��,
		if (!strcmp(tag, "usemtl"))
		{
			// �޽��� ���� ����Ʈ���� �ش� ������ ã�´�.
			char mtl_name[256];
			fscanf_s(fp, "%s", mtl_name, 256);
			pCurrMtl = pMesh->GetMaterial(mtl_name);
		}

		// f (face) �±׶��,
		if (!strcmp(tag, "f"))
		{
			// �������� �����ϴ� ����, �ؼ�, �븻�� �ε����� �����ϴ� ������ �����ϰ�,
			std::vector<int> vIdxs, tIdxs, nIdxs;

			// �� ���� �о,
			char line[256];
			fgets(line, 256, fp);
			char *pData = line;

			// �� ���� ��� ���� ������
			while (true)
			{
				// ���ʿ��� ����� �ٺ����� �ǳʶ��,
				while (*pData == ' ' || *pData == '\n')
					pData++;

				// �� ���� ��� �о��ٸ� �ݺ����� ���� ���´�.
				if (!strcmp(pData, ""))
					break;

				// �ε��� ������ buffer�� ���� ��,
				char buffer[256];	// buffer[] = "vidx/tidx/nidx" 
				sscanf_s(pData, "%s", buffer, 256);
				pData += strlen(buffer);

				// �ε��� ������ ������ ������ �����ϰ�
				char sep;
				int vidx, tidx, nidx;

				// ����, �ؼ�, �븻 �ε����� ��� �ִ� ��� (f 1/1/1/ 2/2/2 3/3/3 ...)
				if (bTexture && bNormal)
				{
					// ��� �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d%c%d%c%d", &vidx, &sep, 1, &tidx, &sep, 1, &nidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					tidx = (tidx < 0) ? -tidx : tidx;
					nidx = (nidx < 0) ? -nidx : nidx;
					vIdxs.push_back(vidx - 1);
					tIdxs.push_back(tidx - 1);
					nIdxs.push_back(nidx - 1);
					continue;
				}

				// ������ �ؼ� �ε����� �ִ� ��� (f 1/1 2/2 3/3 ...)
				if (bTexture && !bNormal)
				{
					// ������ �ؼ� �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d%c%d", &vidx, &sep, 1, &tidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					tidx = (tidx < 0) ? -tidx : tidx;
					vIdxs.push_back(vidx - 1);
					tIdxs.push_back(tidx - 1);
					continue;
				}

				// ������ �븻 �ε����� �ִ� ��� (f 1//1 2//2 3//3 ...)
				if (!bTexture && bNormal)
				{
					// ������ �븻 �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d%c%c%d", &vidx, &sep, 1, &sep, 1, &nidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					nidx = (nidx < 0) ? -nidx : nidx;
					vIdxs.push_back(vidx - 1);
					nIdxs.push_back(nidx - 1);
					continue;
				}

				// ������ �ε����� �ִ� ��� (f 1 2 3 ...)
				if (!bTexture && !bNormal)
				{
					// ������ �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d", &vidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					vIdxs.push_back(vidx - 1);
					continue;
				}
			}

			// �������� �ﰢ������ ������ �޽��� �߰��Ѵ�.
			int PolygonSize = (int)vIdxs.size(); // �ﰢ��(PolygonSize = 3), �簢��(PolygonSize = 4), ...
			for (int i = 0; i < PolygonSize - 2; ++i)
			{
				// �ﰢ���� ������ ���ϰ�,
				EgVertex *V[3] = { 0 };
				V[0] = pMesh->GetVert(vIdxs[0]);
				V[1] = pMesh->GetVert(vIdxs[i + 1]);
				V[2] = pMesh->GetVert(vIdxs[i + 2]);

				// �� ������ �ؼ� ������ ���ϰ�,
				EgTexel *T[3] = { 0 };
				if (bTexture)
				{
					T[0] = pMesh->GetTexel(tIdxs[0]);
					T[1] = pMesh->GetTexel(tIdxs[i + 1]);
					T[2] = pMesh->GetTexel(tIdxs[i + 2]);
				}

				// �� ������ ���� ������ ���ϰ�,
				EgNormal *N[3] = { 0 };
				if (bNormal)
				{
					N[0] = pMesh->GetNormal(nIdxs[0]);
					N[1] = pMesh->GetNormal(nIdxs[i + 1]);
					N[2] = pMesh->GetNormal(nIdxs[i + 2]);
				}
				else // ���� ������ ���ٸ� �ﰢ�� �������� ��ü�Ѵ�.
				{
					EgVec3 n = ((V[1]->m_Pos - V[0]->m_Pos) ^ (V[2]->m_Pos - V[0]->m_Pos)).Normalize();
					N[0] = N[1] = N[2] = ::create_normal(n[0], n[1], n[2]);
					pMesh->AddNormal(N[0]);
				}

				// �ﰢ���� �����Ͽ� �޽��� �߰��Ѵ�.
				EgFace *f = new EgFace(V[0], V[1], V[2], T[0], T[1], T[2], N[0], N[1], N[2], pCurrMtl, GroupName);
				pMesh->AddFace(f);
			}
		}
	}

	// ������ �ݴ´�.
	fclose(fp);

	// ���������� �ݴ��� ������ ������ �����Ѵ�.
	pMesh->InitEdgeMate();

	// �޽��� ��� ���ڸ� ����Ѵ�.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal(SMOOTH);

	// �ؽ�ó ������ ���� ���̵� Ÿ���� �����Ѵ�.
	if (bTexture)
		pMesh->m_ShadeType = SHADE_BY_TEXTURE;
	_cprintf("Importing OBJ mesh complete...\n\n");

	// �� �˻� ���͸����� ���� �۾� ���͸��� �����Ѵ�.
	_chdir(cwd);

	// ����Ʈ�� �޽��� �����͸� ��ȯ�Ѵ�.
	return pMesh;
}

EgMesh *SK_Import_Ply(CString FileName)
{
	// ������ �����Ѵ�.
	FILE *fp;
	fopen_s(&fp, FileName, "r");
	if (!fp)
	{
		_cprintf("Ply file open failed...\n");
		return NULL;
	}

	// �޽��� �����Ѵ�.
	EgMesh *pMesh = ::create_mesh(std::string(FileName));

	// ���� ����� �д´�.
	int NumVert = 0, NumFace = 0;
	char buffer[1024];
	while (fscanf_s(fp, "%s", buffer, 1024) != EOF)
	{
		// element �±׶��,
		if (!strcmp(buffer, "element"))
		{
			// ���� �±׸� �д´�.
			fscanf_s(fp, "%s", buffer, 1024);

			// vertex �±׶��,
			if (!strcmp(buffer, "vertex"))
				fscanf_s(fp, "%d", &NumVert);

			// face �±׶��,
			if (!strcmp(buffer, "face"))
				fscanf_s(fp, "%d", &NumFace);
		}
		// end_header �±׶��
		if (!strcmp(buffer, "end_header"))
			break;
	}

	// ������ ��ǥ�� �д´�.
	for (int i = 0; i < NumVert; ++i)
	{
		double x, y, z;
		fscanf_s(fp, "%lf%lf%lf", &x, &y, &z);
		EgVertex *v = ::create_vertex(x, y, z);
		pMesh->AddVertex(v);
	}

	// �ﰢ���� �ε����� �д´�.
	for (int i = 0; i < NumFace; ++i)
	{
		int tmp, idx0, idx1, idx2;
		fscanf_s(fp, "%d%d%d%d", &tmp, &idx0, &idx1, &idx2);
		EgVertex *v0 = pMesh->GetVert(idx0);
		EgVertex *v1 = pMesh->GetVert(idx1);
		EgVertex *v2 = pMesh->GetVert(idx2);
		EgFace *f = ::create_face(v0, v1, v2, NULL, "");
		pMesh->AddFace(f);
	}

	// ������ �ݴ´�.
	fclose(fp);

	// ���������� �ݴ��� ������ ������ �����Ѵ�.
	pMesh->InitEdgeMate();

	// �޽��� ��� ���ڸ� ����Ѵ�.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal(SMOOTH);

	// �ؽ�ó ������ ���� ���̵� Ÿ���� �����Ѵ�.
	pMesh->m_ShadeType = SHADE_BY_LIGHT;
	_cprintf("Importing PLY mesh complete...\n\n");

	// �޽��� �����Ѵ�.
	return pMesh;
}

EgMesh *SK_Import_Iv(CString FileName)
{
	// ������ �����Ѵ�.
	FILE *fp;
	fopen_s(&fp, FileName, "r");
	if (!fp)
	{
		_cprintf("Iv file open failed...\n");
		return NULL;
	}

	// �޽��� �����Ѵ�.
	EgMesh *pMesh = ::create_mesh(std::string(FileName));

	// ���� ����Ʈ�� ���� ��ġ�� �̵��Ѵ�.
	char line_buffer[1024];
	while (true)
	{
		fgets(line_buffer, 1024, fp);
		CString str(line_buffer);
		str.TrimLeft();
		if (str == "[\n")
			break;
	}

	// ������ �����Ͽ� �޽��� �߰��Ѵ�.
	while (true)
	{
		double x, y, z;
		fgets(line_buffer, 1024, fp);
		CString str(line_buffer);
		str.TrimLeft();
		if (str == "]\n")
			break;

		sscanf_s(line_buffer, "%lf%lf%lf", &x, &y, &z);
		EgVertex *v = ::create_vertex(x, y, z);
		pMesh->AddVertex(v);
	}

	// �ﰢ�� ����Ʈ�� ���� ��ġ�� �̵��Ѵ�.
	while (true)
	{
		fgets(line_buffer, 1024, fp);
		CString str(line_buffer);
		str.TrimLeft();
		if (str == "[\n")
			break;
	}

	// �ﰢ���� �����Ͽ� �޽��� �߰��Ѵ�.
	while (true)
	{
		fgets(line_buffer, 1024, fp);
		CString str(line_buffer);
		str.TrimLeft();
		if (str == "]\n")
			break;

		int vidx0, vidx1, vidx2;
		fgets(line_buffer, 1024, fp);
		sscanf_s(line_buffer, "%d,%d,%d", &vidx0, &vidx1, &vidx2);

		EgVertex *v0 = pMesh->GetVert(vidx0);
		EgVertex *v1 = pMesh->GetVert(vidx1);
		EgVertex *v2 = pMesh->GetVert(vidx2);
		EgFace *f = ::create_face(v0, v1, v2, NULL, "");
		pMesh->AddFace(f);
	}

	// ������ �ݴ´�.
	fclose(fp);

	// ���������� �ݴ��� ������ ������ �����Ѵ�.
	pMesh->InitEdgeMate();

	// �޽��� ��� ���ڸ� ����Ѵ�.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal(SMOOTH);

	// �ؽ�ó ������ ���� ���̵� Ÿ���� �����Ѵ�.
	pMesh->m_ShadeType = SHADE_BY_LIGHT;
	_cprintf("Importing Iv mesh complete...\n\n");

	// �޽��� �����Ѵ�.
	return pMesh;
}

EgMesh *SK_Import_Msh(CString FileName)
{
	return nullptr;
}

EgMesh *SK_Import_Vrml97(CString FileName)
{
	return nullptr;
}

EgMesh * SK_Import_Wrb(CString FileName)
{
	return nullptr;
}

/*!
*	\brief	�Է� ��ġ�� ���� ����� ������ ã�´�.
*	\note	������ ������: 2020-07-15
*
*	\param	pMesh[in]	��� �޽�
*	\param	p[in]		�Է� ��ġ
*
*	\return	�Է� ��ġ�� ���� ����� ������ �����͸� ��ȯ�Ѵ�.
*/
EgVertex *FindClosestVert(EgMesh *pMesh, EgPos p)
{
	EgVertex *pRet = NULL;
	double minDist = INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		double d = dist_sq(p, v->m_Pos);
		if (d < minDist)
		{
			minDist = d;
			pRet = v;
		}
	}
	return pRet;
}

/*!
*	\brief	�޽� ���� �� �� ������ �ִ� ��ο� �Ÿ��� ����Ѵ�.
*	\note	������ ������: 2020-05-29 (������)
*
*	\param	P[in]		�ִ� ����� ������
*	\param	Q[in]		�ִ� ����� ����
*	\param	Path[out]	�ִ� ��θ� �����ϴ� ���� ���������� ����ȴ�.
*
*	\return	�ִ� ����� ���̸� ��ȯ�Ѵ�.
*/
double SK_Shortest_Distance(EgFacePt *P, EgFacePt *Q, std::vector<EgPos> &Path)
{
	// ������������ ���� �Ÿ����� �����Ѵ�.
	std::vector<double> Distances;
	EgMesh *pMesh = P->m_pFace->GetMesh();
	::get_geodesic_dist_field(pMesh, P, Q, Distances);

	// �ִ� ��ο� ���̸� ����Ͽ� ��ȯ�Ѵ�.
	Path.clear();
	std::vector<std::pair<EgEdge *, float>> CutInfo;
	return ::get_shortest_path(P, Q, Distances, Path, CutInfo);
}

/*!
*	\brief	�޽� ���� �� ���� ������ �ִ� ��ο� �Ÿ��� ����Ѵ�.
*	\note	������ ������: 2020-06-18 (������)
*
*	\param	P[in]		�ִ� ����� ���� ����
*	\param	Q[in]		�ִ� ����� �� ����
*	\param	Path[out]	�ִ� ��θ� �����ϴ� ���� ���������� ����ȴ�.
*
*	\return	�ִ� ����� ���̸� ��ȯ�Ѵ�.
*/
double SK_Shortest_Distance(EgVertex *P, EgVertex *Q, std::vector<EgPos> &Path)
{
	// ������������ ���� �Ÿ����� �����Ѵ�.
	std::vector<double> Distances;
	::get_geodesic_dist_field(P->m_pMesh, P, Q, Distances);

	// �ִ� ��ο� ���̸� ����Ͽ� ��ȯ�Ѵ�.
	Path.clear();
	std::vector<std::pair<EgEdge *, float>> CutInfo;
	return ::get_shortest_path(P, Q, Distances, Path, CutInfo);
}

/*!
*	\brief	�޽� ���� �� �� ������ ��鿡 ���� ���� ��ο� �Ÿ��� ���ϴ� �Լ�
*	\note	������ ������: 2020-05-30 (������)
*
*	\param	P[in]			����� ������
*	\param	Q[in]			����� ����
*	\param	Pln[in]			���� ��鿡 ���� ������
*	\param	Path[out]		���� ��θ� �����ϴ� ���� ���������� ����ȴ�.
*	\param	bConvex[in]		���� ����� convexhull ��� ����
*
*	\return	��� ���ܿ� ���� ����� �Ÿ��� ��ȯ�Ѵ�(��ο� ������ �ִٸ� -1.0�� ��ȯ�Ѵ�).
*/
double SK_Planar_Distance(EgFacePt *P, EgFacePt *Q, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex)
{
	// ��� ���� ��ο� ���̸� ���Ѵ�.
	Path.clear();
	std::vector<std::pair<EgEdge *, float>> CutInfo;
	double Dist = ::get_planar_path(P, Q, Pln, Path, CutInfo);

	// ��ο� ������ �ִٸ� -1.0�� ��ȯ�Ѵ�.
	if (Dist == -1.0)
	{
		return -1.0;
	}

	// Convex ��θ� ����Ѵٸ�
	if (bConvex)
	{
		// Convex ��θ� ���ϰ� �Ÿ��� ����Ѵ�.
		std::vector<EgPos> CvxPts;
		Dist = SK_ConvexHull_Open(Pln, Path, CvxPts);
		Path = CvxPts;
	}

	// �Ÿ��� ��ȯ�Ѵ�.
	return Dist;
}

/*!
*	\brief	�޽� ���� �� ���� ������ ��鿡 ���� ���� ��ο� �Ÿ��� ���ϴ� �Լ�
*	\note	������ ������: 2020-06-18 (������)
*
*	\param	P[in]			����� ���� ����
*	\param	Q[in]			����� �� ����
*	\param	Pln[in]			���� ��鿡 ���� ������
*	\param	Path[out]		���� ��θ� �����ϴ� ���� ���������� ����ȴ�.
*	\param	bConvex[in]		���� ����� convexhull ��� ����
*
*	\return	��� ���ܿ� ���� ����� �Ÿ��� ��ȯ�Ѵ�(��ο� ������ �ִٸ� -1.0�� ��ȯ�Ѵ�).
*/
double SK_Planar_Distance(EgVertex *P, EgVertex *Q, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex)
{
	EgFacePt PF;
	PF.m_pFace = P->m_pEdges[0]->m_pFace;
	if (P == PF.m_pFace->GetVertex(0))
		PF.m_Coords.Set(1.0, 0.0, 0.0);
	if (P == PF.m_pFace->GetVertex(1))
		PF.m_Coords.Set(0.0, 1.0, 0.0);
	if (P == PF.m_pFace->GetVertex(2))
		PF.m_Coords.Set(0.0, 0.0, 1.0);

	EgFacePt QF;
	QF.m_pFace = Q->m_pEdges[0]->m_pFace;
	if (Q == QF.m_pFace->GetVertex(0))
		QF.m_Coords.Set(1.0, 0.0, 0.0);
	if (Q == QF.m_pFace->GetVertex(1))
		QF.m_Coords.Set(0.0, 1.0, 0.0);
	if (Q == QF.m_pFace->GetVertex(2))
		QF.m_Coords.Set(0.0, 0.0, 1.0);

	// ��� ���� ��ο� ���̸� ���Ѵ�.
	Path.clear();
	std::vector<std::pair<EgEdge *, float>> CutInfo;
	double Dist = ::get_planar_path(&PF, &QF, Pln, Path, CutInfo);

	// ��ο� ������ �ִٸ� -1.0�� ��ȯ�Ѵ�.
	if (Dist == -1.0)
	{
		return -1.0;
	}

	// Convex ��θ� ����Ѵٸ�
	if (bConvex)
	{
		// Convex ��θ� ���ϰ� �Ÿ��� ����Ѵ�.
		std::vector<EgPos> CvxPts;
		Dist = SK_ConvexHull_Open(Pln, Path, CvxPts);		
		Path = CvxPts;
	}

	// �Ÿ��� ��ȯ�Ѵ�.
	return Dist;
}

/*!
*	\brief	�޽� ���� �� ���� ������ ��鿡 ���� ���� ��ο� �Ÿ��� ���ϴ� �Լ�
*	\note	������ ������: 2020-10-14 (������)
*
*	\param	P[in]			����� ���� ����
*	\param	Q[in]			����� �� ����
*	\param	Path[out]		���� ��θ� �����ϴ� ���� ���������� ����ȴ�.
*	\param	bConvex[in]		���� ����� convexhull ��� ����
*
*	\return	��� ���ܿ� ���� ����� �Ÿ��� ��ȯ�Ѵ�(��ο� ������ �ִٸ� -1.0�� ��ȯ�Ѵ�).
*/
double SK_Planar_Distance(EgVertex *P, EgVertex *Q, std::vector<EgPos> &Path, bool bConvex)
{
	// �� �� P�� Q�� ������ �ִ� ��θ� ���Ѵ�.
	std::vector<EgPos> sPath;
	if (SK_Shortest_Distance(P, Q, sPath) == -1.0)
	{
		printf("Error in SK_Planar_Distance...\n");
		return -1.0;
	}

	// �ִ� ��θ� �����ϴ� ����� �������� ���Ѵ�.
	int m = NUM(sPath);
	EgMat A(m, 3);
	EgVec x(3), b(m);
	for (int i = 0; i < m; ++i)
	{
		A.SetRowVec(i, EgVec(3, sPath[i][0], sPath[i][1], sPath[i][2]));
		b[i] = -1.0;
	}
	::lin_sys_sv(A, x, b);

	EgVec3 z_axis(x[0], x[1], x[2]);
	EgVec3 x_axis = (Q->m_Pos - P->m_Pos).Normalize();
	EgVec3 y_axis = (z_axis ^ x_axis).Normalize();
	z_axis = x_axis ^ y_axis;

	EgPlane CutPln1(z_axis, P->m_Pos);
	EgPlane CutPln2(-z_axis, P->m_Pos);
	double f = CutPln1.Eval(Q->m_Pos);
	printf("f = %lf\n", f);

	std::vector<EgPos> Path1, Path2;
	double d1 = SK_Planar_Distance(P, Q, &CutPln1, Path1, false);
	double d2 = SK_Planar_Distance(P, Q, &CutPln2, Path2, false);
	if (d1 == -1.0 && d2 == -1.0)
	{
		printf("Error in SK_Planar_Distance...\n");
		return -1.0;
	}
	else if (d1 == -1.0 && d2 != -1.0)
		return SK_Planar_Distance(P, Q, &CutPln2, Path, bConvex);
	else if (d1 != -1.0 && d2 == -1.0)
		return SK_Planar_Distance(P, Q, &CutPln1, Path, bConvex);
	else if (d1 < d2)
		return SK_Planar_Distance(P, Q, &CutPln1, Path, bConvex);
	else if (d2 < d1)
		return SK_Planar_Distance(P, Q, &CutPln2, Path, bConvex);

	printf("Error in SK_Planar_Distance...\n");
	return -1.0;
}

/*!
*	\brief	�޽� ���� �� ���� ������ �ѷ��� ���̸� ���ϴ� �Լ�
*	\note	������ ������: 2020-05-26 (������)
*
*	\param	P[in]			�ﰢ�� ������ �� ��
*	\param	Pln[in]			������鿡 ���� ������
*	\param	Path[out]		����� ���� ���������� ����ȴ�.
*	\param	bConvex[in]		�ѷ� ����� convexhull ��� ����
*
*	\return	�ѷ��� ���̸� ��ȯ�Ѵ�.
*/
double SK_Planar_Perimeter(EgFacePt *P, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex)
{
	// �ѷ��� ���̸� ����Ѵ�.
	std::vector<std::pair<EgEdge *, float>> CutInfo;
	double Dist = ::get_planar_path_closed(P, Pln, Path, CutInfo);

	// ��ο� ������ �ִٸ� -1.0�� ��ȯ�Ѵ�.
	if (Dist == -1.0)
		return -1.0;

	// Convex ��θ� ����Ѵٸ�
	if (bConvex)
	{
		// Convex ��θ� ���ϰ� ���̸� ����Ͽ�
		std::vector<EgPos> CvxPts;
		Dist = SK_ConvexHull(Pln, Path, CvxPts);
		Path = CvxPts;
	}

	// �ѷ��� ���̸� ��ȯ�Ѵ�.
	return Dist;
}

/*!
*	\brief	�޽� ������ ������ �ѷ��� ���̸� ���ϴ� �Լ�
*	\note	������ ������: 2020-06-22 (������)
*
*	\param	P[in]			�޽� ����
*	\param	Pln[in]			������鿡 ���� ������
*	\param	CutPts[out]		����� ���� ���������� ����ȴ�.
*	\param	bConvex[in]		�ѷ� ����� convexhull ��� ����
*
*	\return	�ѷ��� ���̸� ��ȯ�Ѵ�.
*/
double SK_Planar_Perimeter(EgVertex *P, EgPlane *Pln, std::vector<EgPos> &Path, bool bConvex)
{
	EgFacePt PF;
	PF.m_pFace = P->m_pEdges[0]->m_pFace;
	if (P == PF.m_pFace->GetVertex(0))
		PF.m_Coords.Set(0.9, 0.05, 0.05);
	if (P == PF.m_pFace->GetVertex(1))
		PF.m_Coords.Set(0.05, 0.9, 0.05);
	if (P == PF.m_pFace->GetVertex(2))
		PF.m_Coords.Set(0.05, 0.05, 0.9);
	EgPos p = PF.Pos();
	Pln->m_Coords[3] -= Pln->Eval(p);

	// �ѷ��� ���̸� ����Ѵ�.
	std::vector<std::pair<EgEdge *, float>> CutInfo;
	double Dist = ::get_planar_path_closed(&PF, Pln, Path, CutInfo);

	// ��ο� ������ �ִٸ� -1.0�� ��ȯ�Ѵ�.
	if (Dist == -1.0)
		return -1.0;

	// Convex ��θ� ����Ѵٸ�
	if (bConvex)
	{
		// Convex ��θ� ���ϰ� ���̸� ����Ͽ�
		std::vector<EgPos> CvxPts;
		Dist = SK_ConvexHull(Pln, Path, CvxPts);
		Path = CvxPts;
	}

	// �ѷ��� ���̸� ��ȯ�Ѵ�.
	return Dist;
}

/*!
*	\brief	���� pq ���� ���� �߿��� convexhull�� �����ϴ� ���� �߰��Ѵ�.
*	\note	������ ������: 2020-05-29
*
*	\param	p[in]			������ ������
*	\param	q[in]			������ ����
*	\param	Pts[in]			��ü �� �迭
*	\param	CvxPts[out]		������� ã�� Convexhull�� �����ϴ� ������ �迭
*/
void AddUpperHullPt(EgPos p, EgPos q, std::vector<EgPos> &Pts, std::vector<EgPos> &CvxPts)
{
	EgPos r;
	double m = (q[1] - p[1]) / (q[0] - p[0]);
	double max_d = -INFINITY;
	bool bFound = false;
	for (EgPos pt : Pts)
	{
		double f = m * (pt[0] - p[0]) + p[1] - pt[1];
		if (EQ_ZERO(f, 1.0e-6))			
			continue;
		if (f > 0.0)
			continue;

		double d = ABS(m * (pt[0] - p[0]) + p[1] - pt[1]) / SQRT(SQR(m) + 1.0);
		if (d > max_d)
		{
			max_d = d;
			r = pt;
			bFound = true;
		}
	}
	if (bFound == false)
		return;
	CvxPts.push_back(r);
	AddUpperHullPt(p, r, Pts, CvxPts);
	AddUpperHullPt(r, q, Pts, CvxPts);
}

/*!
*	\brief	���� pq �Ʒ��� ���� �߿��� convexhull�� �����ϴ� ���� �߰��Ѵ�.
*	\note	������ ������: 2020-10-07
*
*	\param	p[in]			������ ������
*	\param	q[in]			������ ����
*	\param	Pts[in]			��ü �� �迭
*	\param	CvxPts[out]		������� ã�� Convexhull�� �����ϴ� ������ �迭
*/
void AddLowerHullPt(EgPos p, EgPos q, std::vector<EgPos> &Pts, std::vector<EgPos> &CvxPts)
{
	EgPos r;
	bool bFound = false;
	double m = (q[1] - p[1]) / (q[0] - p[0]);
	double max_d = -INFINITY;

	EgVec2 u = ortho(EgVec2(q[0] - p[0], q[1] - p[1]), true);
	for (EgPos pt : Pts)
	{
		EgVec2 v(pt[0] - p[0], pt[1] - p[1]);
		double theta = angle(u, v);
		if (EQ(theta, 90.0, 1.0e-6) || theta < 90.0)
			continue;

		double d = ABS(m * (pt[0] - p[0]) + p[1] - pt[1]) / SQRT(SQR(m) + 1);
		if (d > max_d)
		{
			max_d = d;
			r = pt;
			bFound = true;
		}
	}
	if (bFound == false)
		return;

	CvxPts.push_back(r);
	AddLowerHullPt(p, r, Pts, CvxPts);
	AddLowerHullPt(r, q, Pts, CvxPts);
}

/*!
*	\brief	���� ��� ���� ���� ���������� convexhull�� ���Ѵ�.
*	\note	������ ������: 2020-05-28 (������)
*
*	\param	Pln[in]			���� ��鿡 ���� ������
*	\param	Pts[in]			���� ��� ���� ������ �迭
*	\param	CvxPts[out]		Convexhull�� �����ϴ� ������ �迭
*
*	\return Convexhull�� �ѷ� ���̸� ��ȯ�Ѵ�.
*/
double SK_ConvexHull(EgPlane *Pln, std::vector<EgPos> &Pts, std::vector<EgPos> &CvxPts)
{
	// ������ z������ ���� ���� ����� ���� ��ǥ�踦 ���Ѵ�.
	EgVec3 z_axis = Pln->N(true);
	EgQuat q;
	q.SetFromArcBall(EgVec3(0.0, 0.0, 1.0), z_axis);
	EgVec3 o;
	for (EgPos &p : Pts)
		o = o + cast_vec3(p);
	o = o / (double)(Pts.size());

	// ���� ������ ���� ��ǥ�� ���Ѵ�.
	EgTrans X(o, q);
	EgTrans invX = inv(X);
	std::vector<EgPos> LocalPts;
	for (EgPos &p : Pts)
		LocalPts.push_back(invX * p);

	// ���� ��ǥ�迡�� convexhull�� �����ϴ� ������ ���Ѵ�.
	EgPos MinPt(INFINITY, 0.0, 0.0), MaxPt(-INFINITY, 0.0, 0.0);
	for (EgPos &p : LocalPts)
	{
		if (p[0] < MinPt[0])
			MinPt = p;
		if (p[0] > MaxPt[0])
			MaxPt = p;
	}
	CvxPts.push_back(MinPt);
	CvxPts.push_back(MaxPt);
	AddUpperHullPt(MinPt, MaxPt, LocalPts, CvxPts);
	AddLowerHullPt(MinPt, MaxPt, LocalPts, CvxPts);

	// Convexhull ������ �ݽð� �������� �����Ѵ�.
	for (EgPos &p : CvxPts)
		o = o + cast_vec3(p);
	o = o / (double)(CvxPts.size());
	std::vector<double> Angles;
	for (EgPos &p : CvxPts)
	{
		EgVec3 v = (p - cast_pos(o)).Normalize();
		double theta = angle(EgVec3(1.0, 0.0, 0.0), v, EgVec3(0.0, 0.0, 1.0));
		Angles.push_back(theta);
	}
	for (int i = 0; i < NUM(Angles) - 1; ++i)
	{
		for (int j = i + 1; j < NUM(Angles); ++j)
		{
			if (Angles[i] < Angles[j])
			{
				SWAP(double, Angles[i], Angles[j]);
				SWAP(EgPos, CvxPts[i], CvxPts[j]);
			}
		}
	}

	// �������� �߰��Ͽ� ���θ� �����.
	CvxPts.push_back(CvxPts[0]);

	// ���� ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
	for (EgPos &p : CvxPts)
		p = X * p;

	// �ѷ��� ���̸� ���Ͽ� ��ȯ�Ѵ�.
	double Dist = 0.0;
	for (int i = 0; i < NUM(CvxPts) - 1; ++i)
		Dist += dist(CvxPts[i], CvxPts[i + 1]);
	return Dist;
}

/*!
*	\brief	���� ��� ���� ���� ���������� ���� convexhull�� ���Ѵ�.
*	\note	������ ������: 2020-10-14
*
*	\param	Pln[in]			���� ��鿡 ���� ������
*	\param	Pts[in]			���� ��� ���� ������ �迭
*	\param	CvxPts[out]		Convexhull�� �����ϴ� ������ �迭
*
*	\return	Convexhull�� ���̸� ��ȯ�Ѵ�.
*/
double SK_ConvexHull_Open(EgPlane *Pln, std::vector<EgPos> &Pts, std::vector<EgPos> &CvxPts)
{
	// ������ z������ ���� ���� ����� ���� ��ǥ�踦 ���Ѵ�.
	EgVec3 z_axis = Pln->N(true);
	EgVec3 x_axis = (Pts[NUM(Pts) - 1] - Pts[0]).Normalize();
	EgQuat q;
	q.SetFromFrameZX(z_axis, x_axis);

	EgVec3 o;
	for (EgPos &p : Pts)
		o = o + cast_vec3(p);
	o = o / (double)(Pts.size());

	// ���� ������ ���� ��ǥ�� ���Ѵ�.
	EgTrans X(o, q);
	EgTrans invX = inv(X);
	std::vector<EgPos> LocalPts;
	for (EgPos &p : Pts)
		LocalPts.push_back(invX * p);

	// ���� ��ǥ�迡�� convexhull�� �����ϴ� ������ ���Ѵ�.
	EgPos p0 = LocalPts[0];
	EgPos q0 = LocalPts[NUM(LocalPts) - 1];
	std::vector<EgPos> tmpCvxPts;
	tmpCvxPts.push_back(p0);
	tmpCvxPts.push_back(q0);
	AddLowerHullPt(p0, q0, LocalPts, tmpCvxPts);

	// tmpCvxPts�� LocalPts�� �κ������̹Ƿ�, tmpCvxPts�� ������ LocalPts������ �����Ѵ�.
	for (EgPos p : LocalPts)
	{
		for (EgPos q : tmpCvxPts)
		{
			if (p == q)
			{
				CvxPts.push_back(p);
				break;
			}
		}
	}

	// ���� ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
	for (EgPos &p : CvxPts)
		p = X * p;

	// �ѷ��� ���̸� ���Ͽ� ��ȯ�Ѵ�.
	double Dist = 0.0;
	int NumPts = NUM(CvxPts);
	for (int i = 0; i < NumPts - 1; ++i)
		Dist += dist(CvxPts[i], CvxPts[i + 1]);
	return Dist;
}

/*!
*	\brief	�־��� ���� �ֺ��� ������ �˻��Ͽ� �� ����(extream vertex)�� ã�´�.
*	\note	������ ������: 2020-06-23
*
*	\param	pSeedVert[in]	Ž���� ������ ������ ���� ������
*	\param	Radius[in]		Ž���� ������ ������
*	\param	PtType[in]		Ž���� �� ������ ����(x�� �ִ���(0)/�ּ���(1), y�� �ִ���(2)/�ּ���(3), z�� �ִ���(4)/�ּ���(5)
*	\param	Depth[in]		Ž���� �ݺ��� ȸ��
*
*	\return	�� ������ �����͸� ��ȯ�Ѵ�.
*/
EgVertex *SK_Extreme_Vertex(EgVertex *pSeedVert, double Radius, int PtType, int Depth)
{
	// ���� ����
	if (Depth == 0)
		return pSeedVert;

	// ���� �Ÿ����� �̿��Ͽ� Ž�� ������ ���Ѵ�.
	std::vector<double> Distances;
	EgMesh *pMesh = pSeedVert->GetMesh();
	::get_geodesic_dist_field(pMesh, pSeedVert, Distances, Radius);

	// Ž�� ������ ������ ��ǥ�� �����Ͽ� ������ ���Ѵ�.
	EgVertex *pVert = NULL;
	double value = (PtType % 2 == 0) ? (-INFINITY) : (INFINITY);
	for (int i = 0; i < NUM(Distances); ++i)
	{
		// Ž�� ���� ���� �����̸� ��ŵ�Ѵ�.
		if (Distances[i] == INFINITY)
			continue;

		EgPos p = pMesh->m_pVerts[i]->m_Pos;
		if (PtType % 2 == 0)	// �ִ밪
		{
			if (p[PtType / 2] > value)
			{
				value = p[PtType / 2];
				pVert = pMesh->m_pVerts[i];
			}
		}
		else // �ּҰ�
		{
			if (p[PtType / 2] < value)
			{
				value = p[PtType / 2];
				pVert = pMesh->m_pVerts[i];
			}
		}
	}

	// ��� ȣ���� �����Ѵ�.
	return SK_Extreme_Vertex(pVert, Radius, PtType, Depth - 1);
}

/*!
*	\brief	�־��� ���� �ֺ��� ������ �˻��Ͽ� Ÿ����(elliptic point)�� ã�´�.
*	\note	������ ������: 2020-08-02
*
*	\param	pSeedVert[in]	Ž���� ������ ������ ���� ������
*	\param	Radius[in]		Ž���� ������ ������
*
*	\return	������ ������ �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Elliptic_Vertex(EgVertex *pSeedVert, double Radius)
{
	// ���� �Ÿ����� �̿��Ͽ� Ž�� ������ ���Ѵ�.
	EgMesh *pMesh = pSeedVert->GetMesh();
	EgMesh *pSubMesh = ::create_mesh_from_geodesic_dist(pMesh, pSeedVert, 2 * Radius);

	std::vector<double> Distances;
	EgVertex *pVert = FindClosestVert(pSubMesh, pSeedVert->m_Pos);
	::get_geodesic_dist_field(pSubMesh, pVert, Distances, Radius);

	// Ž�� ������ ���Ե� ������ ���Ͽ� �ּ� ������ ���� ������ �����Ͽ� ��ȯ�Ѵ�.
	EgPos pos;
	double minArea = INFINITY;
	for (EgVertex *v : pSubMesh->m_pVerts)
	{
		if (Distances[v->m_Idx] == INFINITY)
			continue;

		double area = ::get_local_area(v, Radius);
		if (area < minArea)
		{
			minArea = area;
			pos = v->m_Pos;
		}
	}
	delete pSubMesh;
	return FindClosestVert(pMesh, pos);
}

/*!
*	\brief	�־��� ���� �ֺ��� ������ �˻��Ͽ� ��ź��(planar point)�� ã�´�.
*	\note	������ ������: 2020-08-02
*
*	\param	pSeedVert[in]	Ž���� ������ ������ ���� ������
*	\param	Radius[in]		Ž���� ������ ������
*
*	\return	������ ������ �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Planar_Vertex(EgVertex *pSeedVert, double Radius)
{
	// ���� �Ÿ����� �̿��Ͽ� Ž�� ������ ���Ѵ�.
	std::vector<double> Distances;
	EgMesh *pMesh = pSeedVert->GetMesh();
	::get_geodesic_dist_field(pMesh, pSeedVert, Distances, Radius);

	// Ž�� ������ ���Ե� ������ ���Ͽ� ��� ������ ���̰� ���� ���� ������ �����Ͽ� ��ȯ�Ѵ�.
	EgVertex *theVert = NULL;
	double theArea = M_PI * SQR(Radius);
	double minError = INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (Distances[v->m_Idx] == INFINITY)
			continue;

		double area = ::get_local_area(v, Radius);
		double error = ABS(area - theArea);
		if (error < minError)
		{
			minError = error;
			theVert = v;
		}
	}
	return theVert;
}

/*!
*	\brief	�־��� ���� �ֺ��� ������ �˻��Ͽ� �ְ���(hyperbolic point)�� ã�´�.
*	\note	������ ������: 2020-08-02
*
*	\param	pSeedVert[in]	Ž���� ������ ������ ���� ������
*	\param	Radius[in]		Ž���� ������ ������
*
*	\return	������ ������ �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Hyperbolic_Vertex(EgVertex *pSeedVert, double Radius)
{
	// ���� �Ÿ����� �̿��Ͽ� Ž�� ������ ���Ѵ�.
	EgMesh *pMesh = pSeedVert->GetMesh();
	EgMesh *pSubMesh = ::create_mesh_from_geodesic_dist(pMesh, pSeedVert, 2 * Radius);

	std::vector<double> Distances;
	EgVertex *pVert = FindClosestVert(pSubMesh, pSeedVert->m_Pos);
	::get_geodesic_dist_field(pSubMesh, pVert, Distances, Radius);

	// Ž�� ������ ���Ե� ������ ���Ͽ� �ִ� ������ ���� ������ �����Ͽ� ��ȯ�Ѵ�.
	EgPos pos;
	double maxArea = -INFINITY;
	for (EgVertex *v : pSubMesh->m_pVerts)
	{
		if (Distances[v->m_Idx] == INFINITY)
			continue;

		double area = ::get_local_area(v, Radius);
		if (area > maxArea)
		{
			maxArea = area;
			pos = v->m_Pos;
		}
	}
	delete pSubMesh;
	return FindClosestVert(pMesh, pos);
}

/*!
*	\brief	���� ������ �����ϴ� ������ ����� ���� �����Ѵ�.
*	\note	���� ���ο� ���� �������� �����ϰ� ����� ����Ѵ�.
*	\note	������ ������: 2020-08-03
*
*	\param	Pts[in]					2D ���� ������ �����ϴ� ������ ����Ʈ
*	\param	SortedPts[out]			����� ���� ���ĵ� �������� ����ȴ�.
*	\param	bDescend[in]			���� ���(���� ����: true, ���� ����: false)
*	\param	bSignedCurvature[in]	��� ��ȣ�� ����ϸ� true, �ƴϸ� false
*	\param	Up[in]					���� ������ ���� ����� ���� ����(��� ��ȣ�� ����� ��� ����)
*/
void SK_Sort_By_Curvature(std::vector<EgPos> Pts, std::vector<EgPos> &SortedPts, bool bDescend, bool bSignedCurvature, EgVec3 Up)
{
	// �Է� ������ ������ �Ѵ�.
	int NumPt = NUM(Pts);
	std::vector<EgPos> SmoothPts = Pts;
	for (int i = 1; i < NumPt - 1; ++i)
		SmoothPts[i] = lerp(Pts[i - 1], Pts[i + 1], 0.5);

	// �������� ���� ������ �� ������ ����� ���Ѵ�.
	std::vector<double> Ks;
	::get_curvature(SmoothPts, Ks, bSignedCurvature, Up);

	// ����� ���� �Է� ������ �����Ѵ�.	
	for (int i = 0; i < NumPt - 1; ++i)
	{
		for (int j = i + 1; j < NumPt; ++j)
		{
			if (bDescend) // ū��->������
			{
				if (Ks[i] < Ks[j])
				{
					SWAP(double, Ks[i], Ks[j]);
					SWAP(EgPos, Pts[i], Pts[j]);
				}
			}
			else // ������->ū��
			{
				if (Ks[i] > Ks[j])
				{
					SWAP(double, Ks[i], Ks[j]);
					SWAP(EgPos, Pts[i], Pts[j]);
				}
			}
		}
	}

	// ���ĵ� ������ �����Ѵ�.
	SortedPts = Pts;
}

/*! 
*	\brief	���� ������ �������� ���� ����� ���� ������ ���Ѵ�.
*
*	\param	Pts[in]		�Է� ����
*	\param	ResPts[out]	���� ����� ���� �������� �����
*	\param	bSort[in]	ũ������� �������� ����
*	\param	Up[in]		�������� ���� ����� ���� ����(��ȣ ������ ����)
*/
void SK_Find_Negative_Curvature(std::vector<EgPos> Pts, std::vector<EgPos> &ResPts, bool bSort, EgVec3 Up)
{
	// �Է� ������ ������ �Ѵ�.
	int NumPt = NUM(Pts);
	std::vector<EgPos> SmoothPts = Pts;
	for (int i = 1; i < NumPt - 1; ++i)
		SmoothPts[i] = lerp(Pts[i - 1], Pts[i + 1], 0.5);

	// �������� ���� ������ �� ������ ����� ���Ѵ�.
	std::vector<double> Ks;
	::get_curvature(SmoothPts, Ks, true, Up);

	// ����� ũ�⿡ ���� �Է� ������ ����(����->���, ������->ū��)�Ѵ�.	
	if (bSort)
	{
		for (int i = 0; i < NumPt - 1; ++i)
		{
			for (int j = i + 1; j < NumPt; ++j)
			{
				if (Ks[i] > Ks[j])
				{
					SWAP(double, Ks[i], Ks[j]);
					SWAP(EgPos, Pts[i], Pts[j]);
				}
			}
		}
	}

	// ����� ������ ������ �����Ѵ�.
	ResPts.clear();
	for (int i = 0; i < NumPt; ++i)
		if (Ks[i] < 0.0)
			ResPts.push_back(Pts[i]);
}

/*!
*	\brief	���� ������ �������� ��� ����� ���� ������ ���Ѵ�.
*
*	\param	Pts[in]		�Է� ����
*	\param	ResPts[out]	��� ����� ���� �������� �����
*	\param	bSort[in]	ũ������� �������� ����
*	\param	Up[in]		�������� ���� ����� ���� ����(��ȣ ������ ����)
*/
void SK_Find_Positive_Curvature(std::vector<EgPos> Pts, std::vector<EgPos> &ResPts, bool bSort, EgVec3 Up)
{
	// �Է� ������ ������ �Ѵ�.
	int NumPt = NUM(Pts);
	std::vector<EgPos> SmoothPts = Pts;
	for (int i = 1; i < NumPt - 1; ++i)
		SmoothPts[i] = lerp(Pts[i - 1], Pts[i + 1], 0.5);

	// �������� ���� ������ �� ������ ����� ���Ѵ�.
	std::vector<double> Ks;
	::get_curvature(SmoothPts, Ks, true, Up);

	// ����� ũ�⿡ ���� �Է� ������ ����(����->���, ������->ū��)�Ѵ�.	
	if (bSort)
	{
		for (int i = 0; i < NumPt - 1; ++i)
		{
			for (int j = i + 1; j < NumPt; ++j)
			{
				if (Ks[i] > Ks[j])
				{
					SWAP(double, Ks[i], Ks[j]);
					SWAP(EgPos, Pts[i], Pts[j]);
				}
			}
		}
	}

	// ����� ����� ������ �����Ѵ�.
	ResPts.clear();
	for (int i = 0; i < NumPt; ++i)
		if (Ks[i] > 0.0)
			ResPts.push_back(Pts[i]);
}