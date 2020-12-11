#pragma once

#include "SizeKorea.h"
#include "SizeKoreaUtil.h"

#include "SizeKorea_Pose1.h"

// mj::자세1 팔길이 convex 생성에 사용
#include "EgConvexhull.h"

/*!
*	\class	EgSizer
*	\brief	스캔 메쉬의 치수 측정을 관리하는 클래스
*
*	\author 윤승현(shyun@dongguk.edu)
*	\date	15 July 2020
*/
class EgSizer
{
public:
	/*!	\biref	스캔 메쉬의 자세(1, 2, 3, 4) */
	int		m_PoseId;

	// mj::디폴트 성별은 MALE(false)
	bool m_bWoman;

	/*!	\biref	스캔 메쉬에 대한 포인터 */
	EgMesh *m_pMesh;

	/*!	\biref	화살표 메쉬에 대한 포인터 */
	EgMesh *m_pArrowMesh;

	/*!	\biref	측정점 리스트 */
	std::vector<EgVertex *>	m_Landmarks;

	/*!	\biref	사용자 수동 지정 여부에 대한 변수 */
	std::vector<bool>		m_bUserPick;  // 수동랜드마크

	/*!	\biref	선택된 측정점 인덱스 */
	int m_SltLandmarkIdx;

	/*!	\biref	측정 항목 결과 리스트 */
	std::vector<double>	m_Results;

	/*!	\biref	측정 항목의 경로 리스트 */
	std::vector<std::vector<EgPos>> m_Paths;

	/*! \brief	측정 경로 표시 변수 */
	std::vector<bool>	m_bShowPaths;

	/*! \brief	랜드마크 표시 선택 변수 */
	bool m_bShowLandMark;

public:
	// 생성자 및 소멸자
	EgSizer();
	~EgSizer();

	// 멤버 함수
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

