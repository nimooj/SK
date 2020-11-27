#include "pch.h"
#include "SizeKoreaUtil.h"


extern EgSlicer theSlicer;
extern EgSizer theSizer;

std::vector<std::string> LandmarkPose4 = {
	"�Ӹ�������",						// 1
	"���ʸ���",						// 2
	"�����",						// 3
	"�����ʾ��������",				// 4
	"���ʾ��������",					// 5
	"�����ʾ����",					// 6
	"���ʾ����",						// 7
	"���������̵ڵ�����",				// 8
	"�Ȳ�ġ�����",					// 9
	"�ո������",						// 10
	"�ճ���",						// 11
	"�����������",					// 12
	"�����Ҵٸ�����",					// 13
	"����������",						// 14
	"����������",						// 15
	"�����ʾ���������",				// 16
	"���ʾ���������",				// 17
	"�������Ȳ�ġ������",				// 18
	"�����Ȳ�ġ������",				// 19
	"�����ʾ��������̻����ִ뵹�������",	// 20
	"���ʾ��������̻����ִ뵹�������",	// 21
	"������������",					// 22
	"�Ȳ�ġ����",						// 23
	"�Ȳ�ġ�Ʒ���",					// 24
	"�����赹����"					// 25
};

std::vector<std::string> ResultPose4 = {
	"����Ű",						// 1
	"����������",						// 2
	"������ڳ���",					// 3
	"�����������",					// 4
	"�����Ȳ�ġ����",					// 5
	"�����Ҵٸ�����",					// 6
	"������������",					// 7
	"�������ݳ���",					// 8
	"������̳ʺ�",					// 9
	"������ʻ��̳ʺ�",				// 10
	"���Ȼ��̳ʺ�",					// 11
	"�Ȳ�ġ���̳ʺ�",					// 12
	"���������̳ʺ�",					// 13
	"�����Ҵٸ��β�",					// 14
	"������β�",						// 15
	"���������̹�β�",				// 16
	"�����ȱ���",						// 17
	"���������̿��ݼ������",			// 18
	"���������̹���(��)�������",		// 19
	"(�ȱ���)���ȼ�������",			// 20
	"(�ȱ���)�Ʒ��ȼ������",			// 21
	"(�ȱ���)�Ȳ�ġ�ճ��������"		// 22
};

/*!
*	\brief	�Ӹ��������� ã�� �Լ�
*	\note	������ ������: 2020-07-19
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	�Ӹ��������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Head_Top(EgMesh *pMesh)
{
	EgVertex *HT = NULL;
	double minY = -INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] > minY)
		{
			HT = v;
			minY = v->m_Pos[1];
		}
	}
	return HT;
}

/*!
*	\brief	���������̵ڵ����� ã�� �Լ�
*	\note	������ ������ : 2020-07-19
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	���������̵ڵ������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Buttock_Protrusion(EgMesh *pMesh)
{
	// �������������� �߰ߵ��� �ʾҴٸ� NULL�� ��ȯ�Ѵ�.
	EgVertex *RF = theSizer.GetLandmark("�����ʾ��������̻����ִ뵹�������");
	if (RF == NULL)
		return NULL;

	// Ž�� ������ �����Ѵ�.
	double minY = RF->m_Pos[1] - 20;
	double maxY = RF->m_Pos[1] + 20;

	EgVertex *BP = NULL;
	double minZ = INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] > minY &&
			v->m_Pos[1] < maxY &&
			v->m_Pos[2] < minZ)
		{
			minZ = v->m_Pos[2];
			BP = v;
		}
	}

	return BP;
}

/*!
*	\brief	�Ȳ�ġ������� ã�� �Լ�
*	\note	������ ������ : 2020-07-19
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	�Ȳ�ġ������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Center_Olecranon(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("�ո������");
	if (pRef == NULL)
		return NULL;

	EgPos p = pRef->m_Pos;
	p[2] -= 100.0;
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 0.0, 1.0), p));
	theSlicer.Slice(pMesh, p, 200, 200, 10.0);
	EgPos c0 = theSlicer.GetCenterPos(0);

	p[2] -= 50.0;
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 0.0, 1.0), p));
	theSlicer.Slice(pMesh, p, 200, 200, 10.0);
	EgPos c1 = theSlicer.GetCenterPos(0);

	EgVertex *tmp = NULL;
	EgLine ray(c0, c1);
	for (EgFace *f : pMesh->m_pFaces)
	{
		EgPos q;
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);
		if (::intersect_line_triangle(p0, p1, p2, ray, q, false))
		{
			if (q[2] < c1[2])
			{
				tmp = f->GetVertex(0);
				break;
			}
		}
	}

	if (tmp == NULL)
		return NULL;
	return ::SK_Elliptic_Vertex(tmp, 50);;
	//return pMesh->m_pVerts[26477];
}

/*!
*	\brief	(������)�Ȳ�ġ������ ã�� �Լ�
*	\note	������ ������ : 2020-07-26
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	�Ȳ�ġ������ �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Rear_Olecranon_Sit(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("�Ȳ�ġ�����");
	if (pRef == NULL)
		return NULL;

	return SK_Extreme_Vertex(pRef, 40, 5);
	//return pMesh->m_pVerts[26690];
}

/*!
*	\brief	�Ȳ�ġ�Ʒ����� ã�� �Լ�
*	\note	������ ������ : 2020-07-19
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	�Ȳ�ġ�Ʒ����� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Bottom_Olecranon_Sit(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("�Ȳ�ġ�����");
	if (pRef == NULL)
		return NULL;

	return SK_Extreme_Vertex(pRef, 40, 3);
	//return pMesh->m_pVerts[26552];
}

/*!
*	\brief	������ �Ȳ�ġ�������� ã�� �Լ�
*	\note	������ ������ : 2020-07-19
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	������ �Ȳ�ġ�������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Lateral_Humeral_Epicondyle_Right(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("�Ȳ�ġ�����");
	if (pRef == NULL)
		return NULL;

	return SK_Extreme_Vertex(pRef, 40, 1);
	//return pMesh->m_pVerts[26205];
}

/*!
*	\brief	���� �Ȳ�ġ�������� ã�� �Լ�
*	\note	������ ������ : 2020-07-19
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	���� �Ȳ�ġ�������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Lateral_Humeral_Epicondyle_Left(EgMesh *pMesh)
{
	EgPos pt(-INFINITY, 0.0, 0.0);
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];
	double y0 = minY + (maxY - minY) * 0.45;
	double y1 = minY + (maxY - minY) * 0.6;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] < y0 || v->m_Pos[1] > y1)
			continue;

		if (v->m_Pos[0] > pt[0])
			pt = v->m_Pos;
	}
	return FindClosestVert(pMesh, pt);
}
//EgVertex *SK_Pose4_Lateral_Humeral_Epicondyle_Left(EgMesh *pMesh)
//{
//	EgVertex *pRef = theSizer.GetLandmark("�������Ȳ�ġ������");
//	if (pRef == NULL)
//		return NULL;
//
//	EgPos pt(-INFINITY, 0.0, 0.0);
//	EgLine ray(pRef->m_Pos, EgVec3(1.0, 0.0, 0.0));
//	for (EgFace *f : pMesh->m_pFaces)
//	{
//		EgPos q;
//		EgPos p0 = f->GetVertexPos(0);
//		EgPos p1 = f->GetVertexPos(1);
//		EgPos p2 = f->GetVertexPos(2);
//		if (::intersect_line_triangle(p0, p1, p2, ray, q, false))
//		{
//			if (q[0] > pt[0])
//			{
//				pt = q;
//			}
//		}
//	}
//	return FindClosestVert(pMesh, pt);
//}


/*!
*	\brief	������ ����������� ã�� �Լ�
*	\note	������ ������ : 2020-08-03
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	������ ����������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Lateral_Shoulder_Right(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("�ճ���");
	if (pRef == NULL)
		return NULL;

	EgPos p = pRef->m_Pos;
	for (int i = 0; i < 5; ++i)
	{
		p[1] += 50.0;
		theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), p));
		int NumCut = theSlicer.Slice(pMesh);
		if (NumCut == 1)
			break;
	}

	if (theSlicer.GetNumCutLine() != 1)
		return NULL;

	double cntX = (pMesh->m_BndBox.m_MinPt[0] + pMesh->m_BndBox.m_MaxPt[0]) * 0.5;
	std::vector<EgPos> Pts;
	for (EgPos &p : theSlicer.m_CutLines[0])
	{
		if (p[0] < cntX)
			Pts.push_back(p);
	}

	std::vector<EgPos> SortedPts;
	SK_Sort_By_Curvature(Pts, SortedPts, true, false, EgVec3());

	EgVertex *v = FindClosestVert(pMesh, SortedPts[0]);
	std::vector<EgPos> Path;
	SK_Planar_Perimeter(v, &EgPlane(EgVec3(1.0, 0.0, 0.0), v->m_Pos), Path, false);

	EgPos minPt(0.0, -INFINITY, 0.0);
	for (EgPos &p : Path)
	{
		if (p[1] > minPt[1]) {
			minPt = p;
			_cprintf("%f %f %f\n", p[0], p[1], p[2]);
		}
	}

	return FindClosestVert(pMesh, minPt);
	//return pMesh->m_pVerts[32983];
}

/*!
*	\brief	���� ����������� ã�� �Լ�
*	\note	������ ������: 2020-07-19
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	���� ����������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Lateral_Shoulder_Left(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("�ճ���");
	if (pRef == NULL)
		return NULL;

	EgPos p = pRef->m_Pos;
	for (int i = 0; i < 5; ++i)
	{
		p[1] += 50.0;
		theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), p));
		int NumCut = theSlicer.Slice(pMesh);
		if (NumCut == 1)
			break;
	}

	if (theSlicer.GetNumCutLine() != 1)
		return NULL;

	double cntX = (pMesh->m_BndBox.m_MinPt[0] + pMesh->m_BndBox.m_MaxPt[0]) * 0.5;
	std::vector<EgPos> Pts;
	for (EgPos &p : theSlicer.m_CutLines[0])
	{
		if (p[0] > cntX)
			Pts.push_back(p);
	}
	std::vector<EgPos> SortedPts;
	SK_Sort_By_Curvature(Pts, SortedPts, true, false, EgVec3());

	EgVertex *v = FindClosestVert(pMesh, SortedPts[0]);
	std::vector<EgPos> Path;
	SK_Planar_Perimeter(v, &EgPlane(EgVec3(1.0, 0.0, 0.0), v->m_Pos), Path, false);

	EgPos minPt(0.0, -INFINITY, 0.0);
	for (EgPos &p : Path)
	{
		if (p[1] > minPt[1])
			minPt = p;
	}

	return FindClosestVert(pMesh, minPt);
	//return pMesh->m_pVerts[45923];
}

/*!
*	\brief	�����ʾ������ ã�� �Լ�
*	\note	������ ������ : 2020-08-07
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	������ ������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Acromion_Right(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("�����ʾ��������");
	if (pRef == NULL)
		return NULL;

	EgVertex *AR = SK_Elliptic_Vertex(pRef, 50.0);
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), AR->m_Pos));
	theSlicer.Slice(pMesh);

	EgPos minXPos(INFINITY, 0.0, 0.0);
	for (EgPos &p : theSlicer.m_CutLines[0])
		minXPos = (p[0] < minXPos[0]) ? p : minXPos;

	// ������ ����� ã��
	AR = FindClosestVert(pMesh, minXPos);

	// ������ ��������� ����
	int idx = theSizer.GetLandmarkIdx("�����ʾ��������");
	theSizer.m_Landmarks[idx] = SK_Extreme_Vertex(AR, 10.0, 2, 1);
	return AR;
}

/*!
*	\brief	���� ������� ã�� �Լ�
*	\note	������ ������ : 2020-08-07
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	���� ������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Acromion_Left(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("���ʾ��������");
	if (pRef == NULL)
		return NULL;

	EgVertex *AL = SK_Elliptic_Vertex(pRef, 50.0);
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), AL->m_Pos));
	theSlicer.Slice(pMesh);

	EgPos maxXPos(-INFINITY, 0.0, 0.0);
	for (EgPos &p : theSlicer.m_CutLines[0])
		maxXPos = (p[0] > maxXPos[0]) ? p : maxXPos;

	// ���� ����� ã��
	AL = FindClosestVert(pMesh, maxXPos);

	// ���� ��������� ����
	int idx = theSizer.GetLandmarkIdx("���ʾ��������");
	theSizer.m_Landmarks[idx] = SK_Extreme_Vertex(AL, 10.0, 2, 1);
	return AL;
}

/*!
*	\brief	�����ʾ����������� ã�� �Լ�
*	\note	������ ������ : 2020-08-07
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	�����ʾ����������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Deltoid_Right(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("�����ʾ����");
	if (pRef == NULL)
		return NULL;

	//EgVertex *DR = SK_Elliptic_Vertex(pRef, 50.0);
	EgPos p = pRef->m_Pos;
	p[1] -= 60.0;
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), p));
	theSlicer.Slice(pMesh);

	EgPos minXPos(INFINITY, 0.0, 0.0);
	for (EgPos &p : theSlicer.m_CutLines[0])
		minXPos = (p[0] < minXPos[0]) ? p : minXPos;

	return FindClosestVert(pMesh, minXPos);
	//return pMesh->m_pVerts[28196];
}

/*!
*	\brief	���ʾ����������� ã�� �Լ�
*	\note	������ ������ : 2020-08-07
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	���ʾ����������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Deltoid_Left(EgMesh *pMesh)
{
	EgVertex *pRef = theSizer.GetLandmark("���ʾ����");
	if (pRef == NULL)
		return NULL;

	EgPos p = pRef->m_Pos;
	p[1] -= 60.0;
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), p));
	theSlicer.Slice(pMesh);

	EgPos maxXPos(-INFINITY, 0.0, 0.0);
	for (EgPos &p : theSlicer.m_CutLines[0])
		maxXPos = (p[0] > maxXPos[0]) ? p : maxXPos;

	return FindClosestVert(pMesh, maxXPos);
	//return pMesh->m_pVerts[41886];
}

/*!
*	\brief	���ʸ����� ã�� �Լ�
*	\note	������ ������: 2020-08-07
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	���ʸ����� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Ectocanthus(EgMesh *pMesh)
{
	EgVertex *pRef1 = theSizer.GetLandmark("�Ӹ�������");
	EgVertex *pRef2 = theSizer.GetLandmark("�����");
	if (pRef1 == NULL || pRef2 == NULL)
		return NULL;

	// Step 1: �ĺ� �ڳ����� ���Ѵ�.
	EgPos maxZPt(0.0, 0.0, -INFINITY);
	for (EgVertex *v : pMesh->m_pVerts)	
		if (v->m_Pos[1] > pRef2->m_Pos[1] + 50.0 && v->m_Pos[2] > maxZPt[2])
			maxZPt = v->m_Pos;

	// �ĺ� �ڳ����� ������ x�࿡ ������ ������� �ڸ���.
	theSlicer.SetCutPlane(EgPlane(EgVec3(1.0, 0.0, 0.0), maxZPt));
	theSlicer.Slice(pMesh);

	// ������ �߿��� 
	std::vector<EgPos> Pts;
	for (EgPos p : theSlicer.m_CutLines[0])
	{
		// ���̰� (�ڳ��� + 1.0cm ~ �Ӹ������� - 10.0cm) ���̿� �ְ�
		if (p[1] > maxZPt[1] + 10.0 && p[1] < pRef1->m_Pos[1] - 100.0)
		{
			// �Ӹ������� + 2.0cm ���ʿ� �ִ� �������� ������.
			if (p[2] > pRef1->m_Pos[2] + 20.0)
				Pts.push_back(p);
		}
	}

	// ���� ����� ���� ������ ���� �����Ѵ�.
	std::vector<EgPos> NegPts;
	SK_Find_Negative_Curvature(Pts, NegPts, true, EgVec3(1.0, 0.0, 0.0));

	// ���� ���� ���� ����� ���� ���� ������ y���� �������� ���� ������� �����Ѵ�.
	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), NegPts[0]));
	theSlicer.CvxSlice(pMesh);

	// Step 2: ������ �߿��� ���� ū z���� ���� �ڳ����� ���Ѵ�.
	EgPos NosePt(0.0, 0.0, -INFINITY);
	for (EgPos p : theSlicer.m_CutLines[0])
		if (p[2] > NosePt[2])
			NosePt = p;

	// �ڳ����� ������ x�࿡ ������ ������� �ڸ���.
	theSlicer.SetCutPlane(EgPlane(EgVec3(1.0, 0.0, 0.0), NosePt));
	theSlicer.Slice(pMesh);

	Pts.clear();
	for (EgPos p : theSlicer.m_CutLines[0])
	{
		// ���̰� (�ڳ��� + 1.0cm ~ �Ӹ������� - 10.0cm) ���̿� �ְ�
		if (p[1] > maxZPt[1] + 10.0 && p[1] < pRef1->m_Pos[1] - 100.0)
		{
			// �Ӹ������� + 2.0cm ���ʿ� �ִ� �������� ������.
			if (p[2] > pRef1->m_Pos[2] + 20.0)
				Pts.push_back(p);
		}
	}

	// ���� ����� ���� ������ �����Ѵ�.
	NegPts.clear();
	SK_Find_Negative_Curvature(Pts, NegPts, true, EgVec3(1.0, 0.0, 0.0));

	// ���� ����� ���� �����߿��� z��ǥ�� ���� ���� ���� ���Ѵ�.
	EgPos minZPt(0.0, 0.0, INFINITY);
	for (EgPos p : NegPts)
	{
		if (dist(p, NosePt) < 10.0)
		{
			if (p[2] < minZPt[2])
				minZPt = p;
		}
	}
	
	// ������ ���� ��ġ�� �̵��Ѵ�.
	minZPt[0] -= 40.0;
	EgLine ray(minZPt, EgVec3(0.0, 0.0, -1.0));

	EgPos q(0.0, 0.0, -INFINITY);
	for (EgFace *f : pMesh->m_pFaces)
	{
		EgPos tmp;
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);
		if (::intersect_line_triangle(p0, p1, p2, ray, tmp, false))
		{
			double d = dist(minZPt, tmp);
			if (d > 0.0001 && tmp[2] > q[2])
				q = tmp;
		}
	}

	return FindClosestVert(pMesh, q);
}
// ���� 
//EgVertex *SK_Pose4_Ectocanthus(EgMesh *pMesh)
//{
//	EgVertex *pRef1 = theSizer.GetLandmark("�Ӹ�������");
//	EgVertex *pRef2 = theSizer.GetLandmark("�����");
//	if (pRef1 == NULL || pRef2 == NULL)
//		return NULL;
//
//	EgVertex *NoseVert = NULL;
//	EgPos maxZPt(0.0, 0.0, -INFINITY);
//	for (EgVertex *v : pMesh->m_pVerts)
//	{
//		// ����� ���� ������ ���Ͽ�
//		if (v->m_Pos[1] > pRef2->m_Pos[1])
//		{
//			if (v->m_Pos[2] > maxZPt[2])
//			{
//				NoseVert = v;
//				maxZPt = v->m_Pos;
//			}
//		}
//	}
//
//	// �ڳ����� ������ ������� �ڸ���.
//	theSlicer.SetCutPlane(EgPlane(EgVec3(1.0, 0.0, 0.0), maxZPt));
//	theSlicer.Slice(pMesh);
//
//	std::vector<EgPos> Pts;
//	for (EgPos p : theSlicer.m_CutLines[0])
//	{
//		if (p[1] > maxZPt[1] && p[2] > pRef1->m_Pos[2])
//			Pts.push_back(p);
//	}
//
//	std::vector<EgPos> SortedPts;
//	SK_Sort_By_Curvature(Pts, SortedPts, false, true, EgVec3(1.0, 0.0, 0.0));
//
//	EgPos pt(SortedPts[0][0] - 40.0, SortedPts[0][1], SortedPts[0][2]);
//	EgLine ray(pt, EgVec3(0.0, 0.0, -1.0));
//
//	EgPos q(0.0, 0.0, -INFINITY);
//	for (EgFace *f : pMesh->m_pFaces)
//	{
//		EgPos tmp;
//		EgPos p0 = f->GetVertexPos(0);
//		EgPos p1 = f->GetVertexPos(1);
//		EgPos p2 = f->GetVertexPos(2);
//		if (::intersect_line_triangle(p0, p1, p2, ray, tmp, false))
//		{
//			double d = dist(pt, tmp);
//			if (d > 0.0001 && tmp[2] > q[2])
//				q = tmp;
//		}
//	}
//
//	return FindClosestVert(pMesh, q);
//}

/*!
*	\brief	������� ã�� �Լ�
*	\note	������ ������ : 2020-08-16
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Cervicale(EgMesh *pMesh)
{
	// ������ ã�´�.
	// �����ʾ�������� ~ �Ӹ������������� ������ �Ƿ翧�� ���Ѵ�.
	EgVertex *pRef1 = theSizer.GetLandmark("�Ӹ�������");
	EgVertex *pRef2 = theSizer.GetLandmark("�����ʾ����");
	EgVertex *pRef3 = theSizer.GetLandmark("���ʾ����");
	if (pRef1 == NULL || pRef2 == NULL || pRef3 == NULL)
		return NULL;

	theSlicer.SetCutPlane(EgPlane(pRef1->m_Pos, pRef2->m_Pos, pRef3->m_Pos));
	theSlicer.Slice(pMesh);
	std::vector<EgPos> Silhouette;

	for (EgPos p : theSlicer.m_CutLines[0])
	{
		if (p[0] < pRef1->m_Pos[0] && p[1] > pRef2->m_Pos[1])
			Silhouette.push_back(p);
	}

	double maxDist = 0.0;
	EgPos maxPt;
	EgLine line(pRef2->m_Pos, pRef1->m_Pos);
	for (EgPos p : Silhouette)
	{
		double d = dist(line, p);

		if (d > maxDist)
		{
			maxDist = d;
			maxPt = p;
		}
	}

	theSlicer.SetCutPlane(EgPlane(EgVec3(0.0, 1.0, 0.0), maxPt));
	theSlicer.Slice(pMesh);
	EgPos minPt(0.0, 0.0, INFINITY);
	for (EgPos p : theSlicer.m_CutLines[0])
	{
		if (p[2] < minPt[2])
			minPt = p;
	}

	return FindClosestVert(pMesh, minPt);
	//return pMesh->m_pVerts[34076];
}

/*!
*	\brief	�ո�������� ã�� �Լ�
*	\note	������ ������ : 2020-10-22
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	�ո�������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Ulnar_Styloid(EgMesh *pMesh)
{
	// �ճ����� �߰ߵ��� �ʾҴٸ� NULL�� ��ȯ�Ѵ�.
	EgVertex *HD = theSizer.GetLandmark("�ճ���");
	if (HD == NULL)
		return NULL;

	// Ž�� ������ �����Ѵ�.
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];
	double minZ = pMesh->m_BndBox.m_MinPt[2];
	double maxZ = pMesh->m_BndBox.m_MaxPt[2];
	double y0 = 2 * (HD->m_Pos[1] - 100.0 - minY) / (maxY - minY) - 1.0;
	double y1 = 2 * (HD->m_Pos[1] + 100.0 - minY) / (maxY - minY) - 1.0;
	double z0 = 2 * (HD->m_Pos[2] - 250.0 - minZ) / (maxZ - minZ) - 1.0;
	double z1 = 1.0;
	double prevLen = INFINITY;
	std::vector<EgPos> PrevCut;
	EgPos minPt(0.0, INFINITY, 0.0);
	int snum = 20;
	for (int i = 0; i < snum; ++i)
	{
		double z = (double)i / (snum - 1);
		int NumCut = theSlicer.Slice(pMesh, 2, z, -1.0, 0.0, y0, y1, z0, z1, true);
		if (NumCut != 1)
			continue;

		PrevCut = theSlicer.m_CutLines[0];
		double Len = theSlicer.GetLength(0);
		if (Len > prevLen)
		{
			for (EgPos p : PrevCut)
			{
				if (p[1] < minPt[1])
					minPt = p;
			}
			break;
		}
		else
			prevLen = Len;
	}

	return FindClosestVert(pMesh, minPt);
}

/*!
*	\brief	(������)�ճ����� ã�� �Լ�
*	\note	������ ������ : 2020-07-20, ��Ȯ��(��)
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	(������)�ճ����� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Dactylion(EgMesh *pMesh)
{
	// Y�� �ּ�/�ִ� ��ǥ�� ���Ѵ�.
	double minX = pMesh->m_BndBox.m_MinPt[0];
	double maxX = pMesh->m_BndBox.m_MaxPt[0];
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];

	// Ž�� ������ �����Ѵ�.
	double low = minY + (maxY - minY) * 0.45;
	double high = minY + (maxY - minY) * 0.6;
	double center = (minX + maxX) * 0.5;

	// Ž�� ���������� �ִ� Z ��ǥ�� ���� ������ ã�´�.
	EgVertex *AK = NULL;
	double maxZ = -INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[0] < center &&
			v->m_Pos[1] > low && v->m_Pos[1] < high &&
			v->m_Pos[2] > maxZ)
		{
			AK = v;
			maxZ = v->m_Pos[2];
		}
	}
	return AK;
}

/*!
*	\brief	������������� ã�� �Լ�
*	\note	������ ������ : 2020-09-24
*	\note	�Ӹ��������� ���� ã�ƾ� �Ѵ�.
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	������������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Base_Sit(EgMesh *pMesh)
{
	// ���� ��� ����: Z = 7cm
	EgPlane Pln(EgVec3(0.0, 0.0, 1.0), EgPos(0.0, 0.0, 70));
	theSlicer.SetCutPlane(Pln);
	theSlicer.CvxSlice(pMesh);

	if (theSlicer.m_CutLines.empty())
	{
		_cprintf("��������ظ� ����...\n");
		return NULL;
	}

	EgPos MinPt(0.0, INFINITY, 0.0);
	for (EgPos &p : theSlicer.m_CutLines[0])
	{
		if (p[1] < MinPt[1])
			MinPt = p;
	}

	return FindClosestVert(pMesh, MinPt);
}
// ���� ����: 2020-07-19
//EgVertex *SK_Pose4_Base_Sit(EgMesh *pMesh)
//{
//	// �Ӹ��������� �߰ߵ��� �ʾҴٸ� NULL�� ��ȯ�Ѵ�.
//	EgVertex *HT = theSizer.GetLandmark("�Ӹ�������");
//	if (HT == NULL)
//		return NULL;
//
//	// �Ӹ��������� ���� -Y �� ������ ������ �����Ѵ�.
//	EgLine ray(HT->m_Pos, EgVec3(0.0, -1.0, 0.0));
//
//	EgVertex *BS = NULL;
//	double minDist = INFINITY;
//	for (EgFace *f : pMesh->m_pFaces)
//	{
//		EgPos q;
//		EgPos p0 = f->GetVertexPos(0);
//		EgPos p1 = f->GetVertexPos(1);
//		EgPos p2 = f->GetVertexPos(2);
//		if (::intersect_line_triangle(p0, p1, p2, ray, q, false))
//		{
//			double d = dist(HT->m_Pos, q);
//			if (d > 0.0001 && d < minDist)
//			{
//				minDist = d;
//				BS = f->GetVertex(0);
//			}
//		}
//	}
//	return BS;
//}

/*!
*	\brief	�����Ҵٸ������� ã�� �Լ�
*	\note	������ ������ : 2020-07-24
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	�����Ҵٸ������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Superior_Thigh_Sit(EgMesh *pMesh)
{
	// �������������� �߰ߵ��� �ʾҴٸ� NULL�� ��ȯ�Ѵ�.
	EgVertex *AK = theSizer.GetLandmark("������������");
	if (AK == NULL)
		return NULL;

	EgLine ray(AK->m_Pos, EgVec3(0.0, 0.0, -1.0));
	EgPos q(0.0, 0.0, INFINITY);
	for (EgFace *f : pMesh->m_pFaces)
	{
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);
		EgPos tmp;
		if (::intersect_line_triangle(p0, p1, p2, ray, tmp, false))
		{
			double d = dist(AK->m_Pos, tmp);
			if (d > 0.0001 && tmp[2] < q[2])
				q = tmp;
		}
	}

	std::vector<EgPos> IsPts;
	EgPos p = lerp(AK->m_Pos, q, 0.5);
	int snum = 5;
	for (int i = 0; i < snum; ++i)
	{
		EgVec3 v(i / (double)(snum - 1) - 0.5, 1.0, 0.0);
		EgLine ray(p, v);
		double minDist = INFINITY;
		EgPos FirstPt;
		for (EgFace *f : pMesh->m_pFaces)
		{
			EgPos p0 = f->GetVertexPos(0);
			EgPos p1 = f->GetVertexPos(1);
			EgPos p2 = f->GetVertexPos(2);
			double param;
			if (::intersect_line_triangle2(p0, p1, p2, ray, param, false) && param > 0.0)
			{
				double d = dist(p, ray.Eval(param));
				if (d < minDist)
				{
					minDist = d;
					FirstPt = ray.Eval(param);
				}
			}
		}
		IsPts.push_back(FirstPt);
	}

	if (IsPts.empty())
		return NULL;

	EgPos MaxPt(0.0, -INFINITY, 0.0);
	for (EgPos p : IsPts)
	{
		if (p[1] > MaxPt[1])
			MaxPt = p;
	}

	return FindClosestVert(pMesh, MaxPt);
}

/*!
*	\brief	������������ ã�� �Լ�
*	\note	������ ������ : 2020-07-19
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	������������ �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Superior_Patella_Sit(EgMesh *pMesh)
{
	// ������������ �߰ߵ��� �ʾҴٸ� NULL�� ��ȯ�Ѵ�.
	EgVertex *pRef1 = theSizer.GetLandmark("������������");
	if (pRef1 == NULL)
		return NULL;

	EgVertex *SPS = SK_Extreme_Vertex(pRef1, 30.0, 2, 1);
	return SPS;
	//return pMesh->m_pVerts[16045];
}

/*!
*	\brief	������������ ã�� �Լ�
*	\note	������ ������ : 2020-07-20, ��Ȯ��(��)
*	\note	�������������� ���� ã�ƾ� �Ѵ�.
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	������������ �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Posterior_Juncture_Calf_Thigh(EgMesh *pMesh)
{
	// �������������� �߰ߵ��� �ʾҴٸ� NULL�� ��ȯ�Ѵ�.
	EgVertex *AK = theSizer.GetLandmark("������������");
	if (AK == NULL)
		return NULL;

	EgVertex *PJCT = NULL;
	int snum = 10;
	double minDist = INFINITY;
	for (int i = 0; i < snum; ++i)
	{
		EgVec3 v(0.0, -i / (double)(snum - 1), -1.0);
		EgLine ray(AK->m_Pos, v);
		for (EgFace *f : pMesh->m_pFaces)
		{
			EgPos q;
			EgPos p0 = f->GetVertexPos(0);
			EgPos p1 = f->GetVertexPos(1);
			EgPos p2 = f->GetVertexPos(2);
			if (::intersect_line_triangle(p0, p1, p2, ray, q, false))
			{
				double d = dist(AK->m_Pos, q);
				if (d > 0.0001 && d < minDist)
				{
					minDist = d;
					PJCT = f->GetVertex(0);
				}
			}
		}
	}

	return PJCT;
	//return pMesh->m_pVerts[14261];
}

/*!
*	\brief	(������)���������̻����ִ뵹��������� ã�� �Լ�
*	\note	������ ������ : 2020-07-22
*	\note	�������������� ���� ã�ƾ� �Ѵ�.
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	(������)���������̻����ִ뵹��������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Max_Hip_Width_Right(EgMesh *pMesh)
{
	// �������������� �Ӹ��������� �߰ߵ��� �ʾҴٸ� NULL�� ��ȯ�Ѵ�.
	EgVertex *AK = theSizer.GetLandmark("������������");
	EgVertex *HT = theSizer.GetLandmark("�Ӹ�������");
	if (AK == NULL || HT == NULL)
		return NULL;

	// Ž�� ������ �����Ѵ�.
	double minY = AK->m_Pos[1] - 150;
	double maxY = AK->m_Pos[1] + 150;

	EgVertex *MHW_R = NULL;
	double minX = INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] > minY &&
			v->m_Pos[1] < maxY &&
			v->m_Pos[2] < HT->m_Pos[2] &&
			v->m_Pos[0] < minX)
		{
			minX = v->m_Pos[0];
			MHW_R = v;
		}
	}

	return MHW_R;
}

/*!
*	\brief	(����)���������̻����ִ뵹��������� ã�� �Լ�
*	\note	������ ������ : 2020-07-22
*	\note	�������������� ���� ã�ƾ� �Ѵ�.
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	(����)���������̻����ִ뵹��������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Max_Hip_Width_Left(EgMesh *pMesh)
{
	// �������������� �Ӹ��������� �߰ߵ��� �ʾҴٸ� NULL�� ��ȯ�Ѵ�.
	EgVertex *AK = theSizer.GetLandmark("������������");
	EgVertex *HT = theSizer.GetLandmark("�Ӹ�������");
	if (AK == NULL || HT == NULL)
		return NULL;

	// Ž�� ������ �����Ѵ�.
	double minY = AK->m_Pos[1] - 150;
	double maxY = AK->m_Pos[1] + 150;

	EgVertex *MHW_L = NULL;
	double maxX = -INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] > minY &&
			v->m_Pos[1] < maxY &&
			v->m_Pos[2] < HT->m_Pos[2] &&
			v->m_Pos[0] > maxX)
		{
			maxX = v->m_Pos[0];
			MHW_L = v;
		}
	}

	return MHW_L;
	//return pMesh->m_pVerts[373];
}

/*!
*	\brief	(������)�������������� ã�� �Լ�
*	\note	������ ������ : 2020-07-20, ��Ȯ��(��)
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	(������)�������������� �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Anterior_Knee_Sit(EgMesh *pMesh)
{
	// Y�� �ּ�/�ִ� ��ǥ�� ���Ѵ�.
	double minX = pMesh->m_BndBox.m_MinPt[0];
	double maxX = pMesh->m_BndBox.m_MaxPt[0];
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];

	// Ž�� ������ �����Ѵ�.
	double low = minY + (maxY - minY) * 0.2;
	double high = minY + (maxY - minY) * 0.5;
	double center = (minX + maxX) * 0.5;

	// Ž�� ���������� �ִ� Z ��ǥ�� ���� ������ ã�´�.
	EgVertex *AK = NULL;
	double maxZ = -INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[0] < center &&
			v->m_Pos[1] > low && v->m_Pos[1] < high &&
			v->m_Pos[2] > maxZ)
		{
			AK = v;
			maxZ = v->m_Pos[2];
		}
	}
	return AK;
	//return pMesh->m_pVerts[14999];
}

/*!
*	\brief	�����赹������ ã�� �Լ�
*	\note	������ ������ : 2020-10-22
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	�����赹������ �����͸� ��ȯ�Ѵ�
*/
EgVertex *SK_Pose4_Abdominal_Protrusion_Sit(EgMesh *pMesh)
{
	// ������: P: �Ӹ�������, Q: �Ҵٸ�����, R: �Ȳ�ġ�Ʒ���
	EgVertex *P = theSizer.GetLandmark("�Ӹ�������");
	EgVertex *Q = theSizer.GetLandmark("�����Ҵٸ�����");
	EgVertex *R = theSizer.GetLandmark("�Ȳ�ġ�Ʒ���");

	// ���������� �߰ߵ��� �ʾҴٸ� NULL�� ��ȯ�Ѵ�.
	if (P == NULL || Q == NULL || R == NULL)
		return NULL;

	// �Ӹ��������� ������ x���� ���������ϴ� ������� �޽��� �ڸ���.
	theSlicer.SetCutPlane(EgPlane(EgVec3(1.0, 0.0, 0.0), P->m_Pos));
	theSlicer.Slice(pMesh);

	// (�Ҵٸ����� + 2cm) ~ (�Ȳ�ġ�Ʒ��� - 2cm) ���̿��� z���� �ִ��� ���� ã�´�.
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];
	double y0 = minY + (maxY - minY) * 0.44;
	double y1 = minY + (maxY - minY) * 0.46;
	EgPos maxPt(0.0, 0.0, -INFINITY);
	for (EgPos p : theSlicer.m_CutLines[0])
		if (p[1] > Q->m_Pos[1] + 20.0 && p[1] < R->m_Pos[1] - 20.0 && p[2] > maxPt[2])
			maxPt = p;

	// Ư¡���� ��ȯ�Ѵ�.
	return FindClosestVert(pMesh, maxPt);
}
