#include "..\usr\include\egmesh\egmesh.h"
#include <cstring>
#include <vector>
#include <map>

/*!
*	\brief ������ ���ڿ��� ��ȯ�Ͽ� ��ȯ�Ѵ�.
*
*	\param i ���ڿ��� ��ȯ�� ����
*
*	\return ���ڿ��� ��ȯ�� ����
*/
std::string cast_str(int i)
{
	char buffer[128];
	_itoa_s(i, buffer, 10);

	return std::string(buffer);
}

/*!
*	\brief �Ǽ��� ���ڿ��� ��ȯ�Ͽ� ��ȯ�Ѵ�.
*
*	\param v ���ڿ��� ��ȯ�� �Ǽ�
*
*	\return ���ڿ��� ��ȯ�� �Ǽ�
*/
std::string cast_str(float v)
{
	char buffer[128];
	sprintf_s(buffer, "%f", v);

	return std::string(buffer);
}

/*!
*	\brief �������� ���ε� ���� ���� ����Ѵ�.
*
*	\param val ������ ��
*	\param min ������ ���۰�
*	\param max ������ ����
*	\param Cmin ���۰��� ����
*	\param Cman ������ ����
*
*	\return ���ε� ������ ��ȯ�Ѵ�.
*/
EgVec3 get_color_map(double val, double min, double max, EgVec3 Cmin, EgVec3 Cmax)
{
	if (EQ(min, max, 1.0e-7))
		return Cmin;

	if (val <= min)
		return Cmin;
	if (val >= max)
		return Cmax;

	double t = (val - min) / (max - min);
	return (1.0 - t) * Cmin + t * Cmax;
}

/*!
*	\brief �������� ���ε� ���� ���� ����Ѵ�.
*
*	\param val ������ ��
*	\param min ������ ���۰�
*	\param max ������ ����
*	\param Cmin ���۰��� ����
*	\param Cmin �߰����� ����
*	\param Cman ������ ����
*
*	\return ���ε� ������ ��ȯ�Ѵ�.
*/
EgVec3 get_color_map(double val, double min, double max, EgVec3 Cmin, EgVec3 Cmid, EgVec3 Cmax)
{
	if (EQ(min, max, 1.0e-7))
		return Cmin;

	if (val <= min)
		return Cmin;
	if (val >= max)
		return Cmax;

	double t = (val - min) / (max - min);
	if (t <= 0.5)
		return Cmin + 2 * t * (Cmid - Cmin);

	return Cmid + 2 * (t - 0.5) * (Cmax - Cmid);
}

/*!
*	\brief ������ ���󿡼� ������ ���� ��´�.
*
*	\param t ���� �Ķ����: 0(����) <= t <= 1.0(����)
*
*	\return ������ �߰� ������ ��ȯ�Ѵ�.
*/
EgVec3 get_color_map_from_rainbow(double t)
{
	int X = (int)(6 * t);

	// 0.0 <= t <= 1.0
	t = 6.0 * t - X;

	EgVec3 C0, C1, C;
	switch (X)
	{
	case 0:	// ���� ~ ��Ȳ
		C0.Set(255.0, 0.0, 0.0);
		C1.Set(255.0, 127.0, 0.0);
		C = (1.0 - t) * C0 + t * C1;
		break;

	case 1:	// ��Ȳ ~ ���
		C0.Set(255.0, 127.0, 0.0);
		C1.Set(255.0, 255.0, 0.0);
		C = (1.0 - t) * C0 + t * C1;
		break;

	case 2:	// ��� ~ �ʷ�
		C0.Set(255.0, 255.0, 0.0);
		C1.Set(0.0, 255.0, 0.0);
		C = (1.0 - t) * C0 + t * C1;
		break;

	case 3:	// �ʷ� ~ �Ķ�
		C0.Set(0.0, 255.0, 0.0);
		C1.Set(0.0, 0.0, 255.0);
		C = (1.0 - t) * C0 + t * C1;
		break;

	case 4:	// �Ķ� ~ ����
		C0.Set(0.0, 0.0, 255.0);
		C1.Set(75.0, 0.0, 130.0);
		C = (1.0 - t) * C0 + t * C1;
		break;

	case 5:	// ���� ~ ����
		C0.Set(75.0, 0.0, 130.0);
		C1.Set(148.0, 0.0, 211.0);
		C = (1.0 - t) * C0 + t * C1;
		break;

	case 6:	// ����
		C.Set(148.0, 0.0, 211.0);
		break;
	}

	// ���� ���� �������� �Ҵ��Ѵ�.
	if (t < 0.0 || t > 1.0)
		C.Set(0.0, 0.0, 0.0);
	C /= 255.0;

	return C;
}

/*!
*	\brief ������ ������ �����Ѵ�.
*
*	\return ������ ������ ��ȯ�Ѵ�.
*/
EgVec3 get_color_random()
{
	double r = rand() / (double)RAND_MAX;
	double g = rand() / (double)RAND_MAX;
	double b = rand() / (double)RAND_MAX;

	return EgVec3(r, g, b);
}

/*!
*	\brief �ε����� ���� ������ ������ ��ȯ�Ѵ�.
*
*	\idx ������ �ε���
*
*	\return �ε����� ���� ������ ������ ��ȯ�Ѵ�.
*/
EgVec3 get_color_sequence(int idx)
{
	idx = idx % 7;
	switch (idx)
	{
	case 0:
		return EgVec3(0.0, 0.0, 1.0);

	case 1:
		return EgVec3(0.0, 1.0, 0.0);

	case 2:
		return EgVec3(0.0, 1.0, 1.0);

	case 3:
		return EgVec3(1.0, 0.0, 0.0);

	case 4:
		return EgVec3(1.0, 0.0, 1.0);

	case 5:
		return EgVec3(1.0, 1.0, 0.0);

	case 6:
		return EgVec3(1.0, 1.0, 1.0);
	}

	return EgVec3(0.5, 0.5, 0.5);
}

/*!
*	\brief	���(Ȧ)�� ���̸� ����Ѵ�.
*
*	\param	BndryVerts ���(Ȧ)�� �����ϴ� ������ ����Ʈ
*	\param	bClosed ��� ������ �� ���� ����
*
*	\return ���� ������ ���̸� ��ȯ�Ѵ�.
*/
double get_hole_perimeter(std::vector<EgVertex *> &BndryVerts, bool bClosed)
{
	double len = 0.0;
	if (bClosed) // ���̶��
	{
		int NumVerts = (int)BndryVerts.size();
		for (int i = 0; i < NumVerts; ++i)
		{
			EgVertex *v0 = BndryVerts[i];
			EgVertex *v1 = (i == NumVerts - 1) ? BndryVerts[0] : BndryVerts[i + 1];
			len += ::dist_sq(v0->m_Pos, v1->m_Pos);
		}
	}
	else // ���� �ƴ϶��
	{
		int NumVerts = (int)BndryVerts.size();
		for (int i = 0; i < NumVerts - 1; ++i)
			len += ::dist_sq(BndryVerts[i]->m_Pos, BndryVerts[i + 1]->m_Pos);
	}

	// �ѷ� ���̸� ��ȯ�Ѵ�.
	return len;
}

/*!	\brief	�޽� ������ �Լ����� �̿��Ͽ� iso-line ������ �� ������ ���ϴ� �Լ�
*
*	\param	pMesh[in] ��� �޽��� ���� ������
*	\param	Values[in] �޽� i��° ������ �Լ� �� Values[i]
*	\param	MinValue[in] ������ iso value�� �ּҰ�
*	\param	MaxValue[in] ������ iso value�� �ִ밪
*	\param	NumIsoValues[in] ������ iso value�� ����
*	\param	IsoLines[out] iso-line ������ �� ������ �����
*/
void get_iso_line_pts(EgMesh *pMesh, std::vector<double> &Values, double MinValue, double MaxValue, int NumValue, std::vector<std::vector<EgPos>> &IsoLines)
{
	// ����� �ʱ�ȭ �Ѵ�.
	IsoLines.clear();
	IsoLines.assign(NumValue, std::vector<EgPos>());

	// ������ iso-value�� ���Ͽ�
	for (int i = 0; i < NumValue; ++i)
	{
		// Iso-value�� ���� ����Ͽ�
		double IsoValue = MinValue + (double)i / (double)(NumValue - 1) * (MaxValue - MinValue);

		// ������ �ﰢ���� ���Ͽ�
		for (EgFace *f : pMesh->m_pFaces)
		{
			// �ﰢ�� �� ���� iso-value�� ���Ͽ�
			int *vidx = f->GetVertexIdxs();
			double d0 = Values[vidx[0]];
			double d1 = Values[vidx[1]];
			double d2 = Values[vidx[2]];

			// Iso-value�� �ﰢ���� �������� ������ ��ŵ�Ѵ�.
			if ((d0 < IsoValue && d1 < IsoValue && d2 < IsoValue) ||
				(d0 > IsoValue && d1 > IsoValue && d2 > IsoValue))
				continue;

			// �ﰢ�� �� ������ ��ǥ�� ���ϰ�
			EgPos v0 = pMesh->m_pVerts[vidx[0]]->m_Pos;
			EgPos v1 = pMesh->m_pVerts[vidx[1]]->m_Pos;
			EgPos v2 = pMesh->m_pVerts[vidx[2]]->m_Pos;

			// iso value�� v0v1, v0v2 ������ �����ϴ� ���
			if (d0 < IsoValue && d1 > IsoValue && d2 > IsoValue)
			{
				double s = (IsoValue - d0) / (d1 - d0);
				double t = (IsoValue - d0) / (d2 - d0);
				IsoLines[i].push_back(v0 + s * (v1 - v0));
				IsoLines[i].push_back(v0 + t * (v2 - v0));
				continue;
			}
			if (d1 < IsoValue && d2 < IsoValue && d0 > IsoValue)
			{
				double s = (IsoValue - d2) / (d0 - d2);
				double t = (IsoValue - d1) / (d0 - d1);
				IsoLines[i].push_back(v2 + s * (v0 - v2));
				IsoLines[i].push_back(v1 + t * (v0 - v1));
				continue;
			}

			// iso value�� v1v0, v1v2 ������ �����ϴ� ���
			if (d1 < IsoValue && d2 > IsoValue && d0 > IsoValue)
			{
				double s = (IsoValue - d1) / (d2 - d1);
				double t = (IsoValue - d1) / (d0 - d1);
				IsoLines[i].push_back(v1 + s * (v2 - v1));
				IsoLines[i].push_back(v1 + t * (v0 - v1));
				continue;
			}
			if (d2 < IsoValue && d0 < IsoValue && d1 > IsoValue)
			{
				double s = (IsoValue - d0) / (d1 - d0);
				double t = (IsoValue - d2) / (d1 - d2);
				IsoLines[i].push_back(v0 + s * (v1 - v0));
				IsoLines[i].push_back(v2 + t * (v1 - v2));
				continue;
			}

			// iso value�� v2v0, v2v1 ������ �����ϴ� ���
			if (d2 < IsoValue && d0 > IsoValue && d1 > IsoValue)
			{
				double s = (IsoValue - d2) / (d0 - d2);
				double t = (IsoValue - d2) / (d1 - d2);
				IsoLines[i].push_back(v2 + s * (v0 - v2));
				IsoLines[i].push_back(v2 + t * (v1 - v2));
				continue;
			}
			if (d0 < IsoValue && d1 < IsoValue && d2 > IsoValue)
			{
				double s = (IsoValue - d1) / (d2 - d1);
				double t = (IsoValue - d0) / (d2 - d0);
				IsoLines[i].push_back(v1 + s * (v2 - v1));
				IsoLines[i].push_back(v0 + t * (v2 - v0));
				continue;
			}

			if (EQ(IsoValue, d0, 1.0e-7))
			{
				IsoLines[i].push_back(v0);
				IsoLines[i].push_back(v0);
				continue;
			}
			if (EQ(IsoValue, d1, 1.0e-7))
			{
				IsoLines[i].push_back(v1);
				IsoLines[i].push_back(v1);
				continue;
			}
			if (EQ(IsoValue, d2, 1.0e-7))
			{
				IsoLines[i].push_back(v2);
				IsoLines[i].push_back(v2);
				continue;
			}

			printf("Exceptional cases !!!\n");
			printf("IsoValue = %lf, d0 = %lf, d1 = %lf, d2 = %lf\n\n", IsoValue, d0, d1, d2);
		}
	}
}

/*!
*	\brief �׸��忡�� �ߺ��� ������ �����Ѵ�.
*
*	\param pGrid[IN/OUT] ������ �����(��) �׸���(10 x 10 x 10������ 1�������� ǥ��)
*	\param v[in] ������ ������ ������
*	\param Box[in] ������ ���Ե� �޽��� ��� ����
*	\param bDuplicate[out] �ߺ��� �����̸� true����, ���ο� �����̸� false���� ����ȴ�.
*
*	\return �ߺ��� �����̸� ���� ������ �����͸� ��ȯ�ϰ�, �ƴϸ� ���� ������ �����Ͽ� �����͸� ��ȯ�Ѵ�.
*/
static EgVertex *find_duplicate_vertex(std::vector<EgVertex *> *pGrid, EgVertex *v, EgBox &Box, bool &bDuplicate)
{
	// ������ ��ġ, �������� �ڳ���, �������� ũ�⸦ ���Ѵ�.
	double x = v->m_Pos[0];
	double y = v->m_Pos[1];
	double z = v->m_Pos[2];
	double ox = Box.m_MinPt[0];
	double oy = Box.m_MinPt[1];
	double oz = Box.m_MinPt[2];
	double sx = Box.GetWidth();
	double sy = Box.GetHeight();
	double sz = Box.GetDepth();

	// ������ ���Ե�(��) �׸����� �ε����� ���Ѵ�.
	int idx[3] = { 9, 9, 9 };
	for (int i = 0; i < 9; ++i)
	{
		double st = ox + i * sx / 9.0;
		double ed = ox + (i + 1) * sx / 9.0;
		if (x >= st && x < ed)
		{
			idx[0] = i;
			break;
		}
	}
	for (int i = 0; i < 9; ++i)
	{
		double st = oy + i * sy / 9.0;
		double ed = oy + (i + 1) * sy / 9.0;
		if (y >= st && y < ed)
		{
			idx[1] = i;
			break;
		}
	}
	for (int i = 0; i < 9; ++i)
	{
		double st = oz + i * sz / 9.0;
		double ed = oz + (i + 1) * sz / 9.0;
		if (z >= st && z < ed)
		{
			idx[2] = i;
			break;
		}
	}

	// 3���� �׸��� �ε����� 1�������� �����Ͽ�,
	int gidx = idx[0] * 100 + idx[1] * 10 + idx[2];

	// �ش� �׸��忡 ���Ե� ������ ������ ������ ���Ͽ�
	for (EgVertex *v0 : pGrid[gidx])
	{
		// ������ �ߺ��Ǿ��ٸ� ���� ������ �����͸� ��ȯ�Ѵ�.
		//if (v0->m_Pos == v->m_Pos)
		if (dist_sq(v0->m_Pos, v->m_Pos) < 0.00001)
		{
			bDuplicate = true;
			return v0;
		}
	}

	// ���ο� �����̶�� �����Ͽ� ���ο� ������ �����Ͽ� �׸��忡 �߰��Ѵ�.
	EgVertex *nv = new EgVertex(v->m_Pos[0], v->m_Pos[1], v->m_Pos[2]);
	pGrid[gidx].push_back(nv);

	// ���� ������ ������ �����͸� ��ȯ�Ѵ�.
	bDuplicate = false;
	return nv;
}

/*!
*	\brief �޽����� �ߺ��� ������ �ϳ��� ��ģ��.
*
*	\param pMesh[in] ��� �޽��� ���� ������
*
*	\return �޽��� ���� ������ ���� ��ȯ�Ѵ�.
*/
int delete_duplicate_verts(EgMesh *pMesh)
{
	// ���յ� ������ �ﰢ���� ������ ������ �����Ѵ�.
	std::vector<EgFace *> NewFaces;
	std::vector<EgVertex *> Grid[1000];

	// ������ �ﰢ���� ���Ͽ�
	for (EgFace *f : pMesh->m_pFaces)
	{
		// �� ������ ���Ͽ�
		EgVertex *v0 = f->GetVertex(0);
		EgVertex *v1 = f->GetVertex(1);
		EgVertex *v2 = f->GetVertex(2);

		// �ߺ��� �����̶�� ���� ������ �����͸� ���ϰ�, �ƴϸ� ���ο� ������ �����Ѵ�.
		bool bDuplicate;
		v0 = find_duplicate_vertex(Grid, v0, pMesh->m_BndBox, bDuplicate);
		v1 = find_duplicate_vertex(Grid, v1, pMesh->m_BndBox, bDuplicate);
		v2 = find_duplicate_vertex(Grid, v2, pMesh->m_BndBox, bDuplicate);

		// ���� �ﰢ���� �ؼ��� ������ ���Ͽ� ������ �����ϰ�
		EgTexel *t0 = f->GetEdge(0)->m_pTexel;
		EgTexel *t1 = f->GetEdge(1)->m_pTexel;
		EgTexel *t2 = f->GetEdge(2)->m_pTexel;
		EgNormal *n0 = f->GetEdge(0)->m_pNormal;
		EgNormal *n1 = f->GetEdge(1)->m_pNormal;
		EgNormal *n2 = f->GetEdge(2)->m_pNormal;
		EgEdge *e0 = new EgEdge(v0, t0, n0);
		EgEdge *e1 = new EgEdge(v1, t1, n1);
		EgEdge *e2 = new EgEdge(v2, t2, n2);

		// ���ο� �ﰢ���� �����Ͽ� ����Ʈ�� �߰��Ѵ�.
		EgFace *nf = new EgFace(e0, e1, e2, f->m_pMaterial, f->m_GroupName);
		NewFaces.push_back(nf);
	}

	// ���� ������ ����Ʈ�� �ﰢ�� ����Ʈ�� �����Ѵ�.
	for (EgVertex *v : pMesh->m_pVerts)
		delete v;
	for (EgFace *f : pMesh->m_pFaces)
		delete f;
	pMesh->m_pVerts.clear();
	pMesh->m_pFaces.clear();

	// ���ο� ������ ����Ʈ�� �ﰢ�� ����Ʈ�� �����Ѵ�.
	for (int i = 0; i < 1000; ++i)
		for (EgVertex *v : Grid[i])
			pMesh->AddVertex(v);
	for (EgFace *f : NewFaces)
		pMesh->AddFace(f);

	// ������ �������, ����, �����ڸ� �����Ѵ�.
	pMesh->InitEdgeMate();
	pMesh->UpdateNormal(SMOOTH);
	pMesh->UpdateBndBox();

	// ������ ���۸� �ʱ�ȭ �Ѵ�.
	pMesh->m_VertexBuf.clear();
	pMesh->m_NormalBuf.clear();
	pMesh->m_TexelBuf.clear();

	// ������ ������ ��ȯ�Ѵ�.
	return pMesh->GetNumVert();
}


