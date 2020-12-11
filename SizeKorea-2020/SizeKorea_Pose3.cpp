#include "pch.h"
#include "SizeKoreaUtil.h"

extern EgSlicer theSlicer;
extern EgSizer theSizer;

std::vector<std::string> LandmarkPose3 = {
	"�����̵�����",				// 0
	"�赹����",					// 1
	"�������ִ�ѷ�������",			// 2
	"�����ָ԰����",			// 3
	"�������ָ԰����",			// 4
	"�����ʵ����������",			// 5
	"�����Ȳ�ġ�Ʒ���",			// 6
	"�����Ȳ�ġ����",				// 7
	"�����ʾ����"				// 8
};

std::vector<std::string> ResultPose3 = {
	"�������ִ�ѷ�����",			// 0
	"�����Ȳ�ġ����",				// 1
	"�����̵ѷ�",					// 2
	"�������ִ�ѷ�",				// 3
	"�赹�������ؾ����̵ѷ�",		// 4
	"�Ȳ�ġ�ָԼ������",			// 5
	"��������λ����ָԼ������",	// 6
	"�������������"				// 7
};

/*!
*	\brief	�����̵�����(����)�� ã�´�.
*	\note	������ ������: 2020-06-23
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	�����̵������� �����͸� ��ȯ�Ѵ�.
*/
EgVertex *SK_Pose3_Buttock_Protrusion(EgMesh *pMesh)
{
	// X�� �߰� ��ǥ�� ���Ѵ�.
	double cntX = pMesh->m_BndBox.m_MinPt[0] * 0.65 + pMesh->m_BndBox.m_MaxPt[0] * 0.35;

	// Y�� �ּ�/�ִ� ��ǥ�� ���Ѵ�.
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];

	// Ž�� ���̸� �����Ѵ�. 
	double low = minY + (maxY - minY) * 0.4;
	double high = minY + (maxY - minY) * 0.55;
	double minZ = INFINITY;

	// �ּ� Z��ǥ�� ���� ������ ã�� ��ȯ�Ѵ�.
	EgVertex *pResult = NULL;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] > low && v->m_Pos[1] < high)
		{
			if (v->m_Pos[2] < minZ && v->m_Pos[0] < cntX)
			{
				minZ = v->m_Pos[2];
				pResult = v;
			}
		}
	}
	return pResult;
}

/*!
*	\brief	�赹������ ã�´�.
*	\note	������ ������: 2020-07-08
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*
*	\return	�赹������ �����͸� ��ȯ�Ѵ�.
*/
EgVertex *SK_Pose3_Abdominal_Protrusion(EgMesh *pMesh)
{
	// X, Y�� �ּ�/�ִ� ��ǥ�� ���Ѵ�.
	double minX = pMesh->m_BndBox.m_MinPt[0];
	double maxX = pMesh->m_BndBox.m_MaxPt[0];
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];

	// Ž�� ���̸� �����Ѵ�. 
	double low = minY + (maxY - minY) * 0.5;
	double high = minY + (maxY - minY) * 0.6;

	// Ž�� ���� �����Ѵ�. (cntX - 10cm ~ cntX + 10cm)
	double cntX = (minX + maxX) * 0.5;

	// �ִ� z ��ǥ�� ���� ������ ã�� ��ȯ�Ѵ�.
	double maxZ = -INFINITY;
	EgVertex *pResult = NULL;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		if (v->m_Pos[1] > low && v->m_Pos[1] < high &&
			ABS(v->m_Pos[0] - cntX) < 100.0)
		{
			if (v->m_Pos[2] > maxZ)
			{
				maxZ = v->m_Pos[2];
				pResult = v;
			}
		}
	}
	return pResult;
}

/*!
*	\brief	�������ִ�ѷ�����(��)�� ã�´�.
*	\note	������ ������: 2020-06-26
*
*	\param	pMesh[in]	��ĵ �޽��� ���� ������
*	\param	Path[out]	���� ��ΰ� ����ȴ�.
*
*	\return	�������ִ�ѷ�����(��)�� �����͸� ��ȯ�Ѵ�.
*/
EgVertex *SK_Pose3_Buttock_Max_Perimeter(EgMesh *pMesh, std::vector<EgPos> &Path)
{
	// Y�� �ּ�/�ִ� ��ǥ�� ���Ѵ�.
	double minY = pMesh->m_BndBox.m_MinPt[1];
	double maxY = pMesh->m_BndBox.m_MaxPt[1];

	// �������� ~ �����̵����� ������ Ž�� ���̸� �����Ѵ�. 
	EgVertex *BP = SK_Pose3_Buttock_Protrusion(pMesh);
	double low = minY + (maxY - minY) * 0.4;
	double high = BP->m_Pos[1];

	EgVertex *pResult = NULL;
	double maxPerimeter = -INFINITY;
	for (EgVertex *v : pMesh->m_pVerts)
	{
		// Ž�� ������ ������ ���Ͽ�
		if (v->m_Pos[1] > low && v->m_Pos[1] < high)
		{
			if (ABS(BP->m_Pos[0] - v->m_Pos[0]) < 1 &&
				ABS(BP->m_Pos[2] - v->m_Pos[2]) < 10)
			{
				// �ִ� �ѷ��� ������ ���Ѵ�.
				std::vector<EgPos> tmp;
				EgPlane Pln(EgVec3(0.0, 1.0, 0.0), v->m_Pos);
				double perimeter = ::SK_Planar_Perimeter(v, &Pln, tmp, true);
				if (perimeter >= maxPerimeter)
				{
					maxPerimeter = perimeter;
					pResult = v;
					Path = tmp;
				}
			}
		}
	}

	// �������ִ�ѷ��������� ��ȯ�Ѵ�.
	return pResult;
}

/*
*	\brief	������ �ָ԰������ ã�� �Լ�
*
*	\param	pMesh[in]	��� �޽��� ���� ������
*
*	\return	������ �ָ԰������ ��ġ�� ��ȯ�Ѵ�.
*/
EgVertex *SK_Pose3_Right_Fist_Center(EgMesh *pMesh)
{
	double minZ = 0.7, maxZ = 1.0, z_star;
	double prevLen = INFINITY;
	EgPos CntPos;
	for (int i = 0; i < 10; ++i)
	{
		double z = (double)i / (9.0) * (maxZ - minZ) + minZ;
		double len = theSlicer.CvxSlice(pMesh, 2, z, -1.0, 0.0, 0.0, 1.0, -1.0, 1.0);
		if (len == -1.0)
			continue;

		//printf("Len = %lf\n", len);
		if (len > prevLen)
		{
			z_star = z;
			CntPos = theSlicer.GetCenterPos(0);
			break;
		}
		else
			prevLen = len;
	}

	// �ָ� �߽��� ������ ���ܸ��� ������ ���Ѵ�.
	double len = theSlicer.CvxSlice(pMesh, 2, (z_star + 1.0) * 0.5, -1.0, 0.0, 0.0, 1.0, -1.0, 1.0);
	if (len == -1.0)
	{
		printf("[Error]: SK_Pose5_Right_Fist_Center()...\n");
		return NULL;
	}

	// ���ܸ��� ��ǥ �� �ּ� X��ǥ�� ���� ���� �����Ѵ�.
	EgPos FistCenter(INFINITY, 0.0, 0.0);
	for (EgPos p : theSlicer.m_CutLines[0])
	{
		if (p[0] < FistCenter[0])
			FistCenter = p;
	}

	// ������ �ָ԰������ ��ȯ�Ѵ�.
	return FindClosestVert(pMesh, FistCenter);
}

/*
*	\brief	���� �ָ԰������ ã�� �Լ�
*
*	\param	pMesh[in]	��� �޽��� ���� ������
*
*	\return	���� �ָ԰������ ��ġ�� ��ȯ�Ѵ�.
*/
EgVertex *SK_Pose3_Left_Fist_Center(EgMesh *pMesh)
{
	double minZ = 0.7, maxZ = 1.0, z_star;
	double prevLen = INFINITY;
	EgPos CntPos;
	for (int i = 0; i < 10; ++i)
	{
		double z = (double)i / (9.0) * (maxZ - minZ) + minZ;
		double len = theSlicer.CvxSlice(pMesh, 2, z, 0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
		if (len == -1.0)
			continue;

		//printf("Len = %lf\n", len);
		if (len > prevLen)
		{
			z_star = z;
			CntPos = theSlicer.GetCenterPos(0);
			break;
		}
		else
			prevLen = len;
	}

	// �ָ� �߽��� ������ ���ܸ��� ������ ���Ѵ�.
	double len = theSlicer.CvxSlice(pMesh, 2, (z_star + 1.0) * 0.5, 0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	if (len == -1.0)
	{
		printf("[Error]: SK_Pose3_Left_Fist_Center()...\n");
		return NULL;
	}

	// ���ܸ��� ��ǥ �� �ִ� X��ǥ�� ���� ���� �����Ѵ�.
	EgPos FistCenter(-INFINITY, 0.0, 0.0);
	for (EgPos p : theSlicer.m_CutLines[0])
	{
		if (p[0] > FistCenter[0])
			FistCenter = p;
	}

	// ���� �ָ԰������ ��ȯ�Ѵ�.
	return FindClosestVert(pMesh, FistCenter);
}

/*
*	\brief	���� �Ȳ�ġ�Ʒ����� ã�� �Լ�
*	\note	������ ������: 2020-11-29
*
*	\param	pMesh[in]	��� �޽��� ���� ������
*
*	\return	���� �Ȳ�ġ�Ʒ����� ��ġ�� ��ȯ�Ѵ�.
*/
EgVertex *SK_Pose3_Bottom_Olecranon(EgMesh *pMesh)
{
	EgVertex *pSeedVert = theSizer.GetLandmark("�����Ȳ�ġ����");
	if (pSeedVert == NULL)
		return NULL;

	pSeedVert = SK_Extreme_Vertex(pSeedVert, 30.0, 3, 1);
	return pSeedVert;
	// ���� ����
	//for (int i = 0; i < 10; ++i)
	//{
	//	double z = 0.7 + 0.3 * i / 9.0;
	//	double Len = theSlicer.CvxSlice(pMesh, 2, z, 0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	//	if (Len != -1.0)
	//		break;
	//}

	//// ������ �����Ѵ�.
	//EgPos CntPos = theSlicer.GetCenterPos(0);
	//EgLine ray(CntPos, EgVec3(0.0, 0.0, -1.0));
	//EgPos SeedPt(0.0, 0.0, INFINITY);
	//for (EgFace *f : pMesh->m_pFaces)
	//{
	//	EgPos p0 = f->GetVertexPos(0);
	//	EgPos p1 = f->GetVertexPos(1);
	//	EgPos p2 = f->GetVertexPos(2);
	//	EgPos tmp;
	//	if (::intersect_line_triangle(p0, p1, p2, ray, tmp, false))
	//	{
	//		if (tmp[2] < SeedPt[2])
	//			SeedPt = tmp;
	//	}
	//}

	//EgVertex *pSeedVert = FindClosestVert(pMesh, SeedPt);
	//pSeedVert = SK_Extreme_Vertex(pSeedVert, 30.0, 3, 2);
	//return pSeedVert;
}

/*
*	\brief	���� �Ȳ�ġ������ ã�� �Լ�
*	\note	������ ������: 2020-11-29
*
*	\param	pMesh[in]	��� �޽��� ���� ������
*
*	\return	���� �Ȳ�ġ������ ��ġ�� ��ȯ�Ѵ�.
*/
EgVertex *SK_Pose3_Rear_Olecranon(EgMesh *pMesh)
{
	double minZ = 0.7, maxZ = 1.0, z_star;
	double prevLen = INFINITY;
	EgPos prevCntPos, CntPos;
	for (int i = 0; i < 10; ++i)
	{
		double z = minZ + (maxZ - minZ) * (double)i / (9.0);
		double len = theSlicer.CvxSlice(pMesh, 2, z, 0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
		if (len == -1.0)
			continue;

		if (len > prevLen)
		{
			z_star = z;
			CntPos = theSlicer.GetCenterPos(0);
			break;
		}
		else
		{
			prevLen = len;
			prevCntPos = theSlicer.GetCenterPos(0);
		}
	}

	// ������ �����Ѵ�.
	EgLine ray(CntPos, EgVec3(0.0, 0.0, -1.0));
	EgPos SeedPt(0.0, 0.0, INFINITY);
	for (EgFace *f : pMesh->m_pFaces)
	{
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);
		EgPos tmp;
		if (::intersect_line_triangle(p0, p1, p2, ray, tmp, false))
		{
			if (tmp[2] < SeedPt[2])
				SeedPt = tmp;
		}
	}

	EgVertex *pSeedVert = FindClosestVert(pMesh, SeedPt);
	pSeedVert = SK_Elliptic_Vertex(pSeedVert, 50.0);
	pSeedVert = SK_Extreme_Vertex(pSeedVert, 20.0, 5);
	return pSeedVert;
}

/*!
*	\brief	������ ����������� ã�� �Լ�
*
*	\param	pMesh[in]	��� �޽��� ���� ������
*
*	\return	������ ������������� ��ǥ�� ��ȯ�Ѵ�.
*/
EgVertex *SK_Pose3_Right_Back_Protrusion(EgMesh *pMesh)
{
	// �޽��� �����Ѵ�.
	double Len = theSlicer.CvxSlice(pMesh, 2, 0.7, -1.0, 0.0, 0.0, 1.0, 0.0, 1.0);
	if (Len == -1.0)
	{
		printf("[Error]: SK_Pose3_Right_Back_Protrusion()...\n");
		return NULL;
	}

	// ������ �����Ѵ�.
	EgLine ray(theSlicer.GetCenterPos(0), EgVec3(0.0, 0.0, -1.0));

	// ������ �޽��� ������(�ּ� Z���� ����)�� ���Ѵ�.
	EgPos SeedPt(0.0, 0.0, INFINITY);
	for (EgFace *f : pMesh->m_pFaces)
	{
		EgPos p0 = f->GetVertexPos(0);
		EgPos p1 = f->GetVertexPos(1);
		EgPos p2 = f->GetVertexPos(2);
		EgPos tmp; // ������
		if (::intersect_line_triangle(p0, p1, p2, ray, tmp, false))
		{
			if (tmp[2] < SeedPt[2])
				SeedPt = tmp;
		}
	}

	// ������ �αٿ��� �ּ� Z���� ���� ������ ã�´�.
	EgVertex *pResult = FindClosestVert(pMesh, SeedPt);
	return SK_Extreme_Vertex(pResult, 100.0, 5);
}

/*!
*	\brief	������ ������� ã�� �Լ�
*
*	\param	pMesh[in]		��� �޽��� ���� ������
*
*	\return	������ ������� ��ǥ�� ��ȯ�Ѵ�.
*/
EgVertex *SK_Pose3_Right_Acromion(EgMesh *pMesh)
{
	// �������� ���ܸ��� �߽� 3���� ���Ѵ�.
	std::vector<EgPos> CntPos;
	for (int i = 0; i < 10; ++i)
	{
		// z = 0.4 ~ 1.0 �������� ������ ���ܸ��� ���Ѵ�.
		double z = 0.4 + 0.6 * i / 9.0;
		double Len = theSlicer.CvxSlice(pMesh, 2, z, -1.0, -0.5, 0.0, 1.0, -1.0, 1.0);
		if (Len != -1.0)
		{
			CntPos.push_back(theSlicer.GetCenterPos(0));
			if (NUM(CntPos) == 2)
				break;
		}
	}

	// ���ܸ��� �߽ɵ��� ������ ���� ����� �����Ѵ�.
	EgPos MidPos = lerp(CntPos[0], CntPos[1], 0.5);
	MidPos[1] -= 50.0;
	theSlicer.SetCutPlane(EgPlane(CntPos[0], MidPos, CntPos[1]));
	theSlicer.CvxSlice(pMesh);

	// ���ܸ��� y ��ǥ�� �ִ��� ���� ������ ��������� �����Ѵ�.
	EgPos Acromion(0.0, -INFINITY, 0.0);
	for (EgPos &pt : theSlicer.m_CutLines[0])
		if (pt[1] > Acromion[1] && pt[2] < CntPos[0][2])
			Acromion = pt;

	// ������ ������� ��ȯ�Ѵ�.
	EgVertex *Ret = FindClosestVert(pMesh, Acromion);
	return SK_Extreme_Vertex(Ret, 30.0, 5);
	//return FindClosestVert(pMesh, Acromion);
}
// ���� ����
//EgVertex *SK_Pose3_Right_Acromion(EgMesh *pMesh)
//{
//	for (int i = 0; i < 10; ++i)
//	{
//		// z = 0.7 ~ 1.0 �������� ������ ���ܸ��� ���Ѵ�.
//		double z = 0.7 + 0.3 * i / 9.0;
//		double Len = theSlicer.CvxSlice(pMesh, 2, z, -1.0, 0.0, 0.0, 1.0, -1.0, 1.0);
//		if (Len != -1.0)
//			break;
//	}
//
//	// ���� �߽��� ������ x ���� �������� �ϴ� ���� ����� �����Ѵ�.
//	EgPos p = theSlicer.GetCenterPos(0);
//	theSlicer.SetCutPlane(EgPlane(EgVec3(1.0, 0.0, 0.0), p));
//	theSlicer.CvxSlice(pMesh);
//
//	// ���ܸ��� y ��ǥ�� �ִ��� ���� ������ ��������� �����Ѵ�.
//	EgPos Acromion(0.0, -INFINITY, 0.0);
//	for (EgPos &pt : theSlicer.m_CutLines[0])
//		if (pt[1] > Acromion[1] && pt[2] < p[2])
//			Acromion = pt;
//
//	// ������ ������� ��ȯ�Ѵ�.
//	return FindClosestVert(pMesh, Acromion);
//}

