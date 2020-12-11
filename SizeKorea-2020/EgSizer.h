#pragma once

#include "SizeKorea.h"
#include "SizeKoreaUtil.h"

#include "SizeKorea_Pose1.h"

// mj::�ڼ�1 �ȱ��� convex ������ ���
#include "EgConvexhull.h"

/*!
*	\class	EgSizer
*	\brief	��ĵ �޽��� ġ�� ������ �����ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	15 July 2020
*/
class EgSizer
{
public:
	/*!	\biref	��ĵ �޽��� �ڼ�(1, 2, 3, 4) */
	int		m_PoseId;

	// mj::����Ʈ ������ MALE(false)
	bool m_bWoman;

	/*!	\biref	��ĵ �޽��� ���� ������ */
	EgMesh *m_pMesh;

	/*!	\biref	ȭ��ǥ �޽��� ���� ������ */
	EgMesh *m_pArrowMesh;

	/*!	\biref	������ ����Ʈ */
	std::vector<EgVertex *>	m_Landmarks;

	/*!	\biref	����� ���� ���� ���ο� ���� ���� */
	std::vector<bool>		m_bUserPick;  // �������帶ũ

	/*!	\biref	���õ� ������ �ε��� */
	int m_SltLandmarkIdx;

	/*!	\biref	���� �׸� ��� ����Ʈ */
	std::vector<double>	m_Results;

	/*!	\biref	���� �׸��� ��� ����Ʈ */
	std::vector<std::vector<EgPos>> m_Paths;

	/*! \brief	���� ��� ǥ�� ���� */
	std::vector<bool>	m_bShowPaths;

	/*! \brief	���帶ũ ǥ�� ���� ���� */
	bool m_bShowLandMark;

public:
	// ������ �� �Ҹ���
	EgSizer();
	~EgSizer();

	// ��� �Լ�
	void Init(int PoseId, EgMesh *pMesh);
	void FindLandmarks();
	void UpdateLandmark(EgPos pos);
	EgVertex *GetLandmark(std::string Name);
	void Measure();

	void FindLandmarkPose1();
	void MeasurePose1();

	void FindLandmarkPose2();
	void MeasurePose2();

	void FindLandmarkPose3();
	void MeasurePose3();

	void FindLandmarkPose4();
	void MeasurePose4();

	void FindLandmarkPose5();
	void MeasurePose5();

	// mj::Set gender
	void SetGender();


	void Render();
	void RenderArrow(EgPos p, EgPos q);
	int	GetLandmarkIdx(std::string Name);
	int GetResultIdx(std::string Name);
	EgPos GetSltLandmarkPos();
	void Reset();
};

