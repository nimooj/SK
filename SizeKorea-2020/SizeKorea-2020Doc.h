
// SizeKorea-2020Doc.h : interface of the CSizeKorea2020Doc class
//
#pragma once

//////////////////////////////////
//			GSizerDoc			//
//////////////////////////////////
#include "SizeKorea.h"
#include "ConvexHull.h"
#include "DBRecord.h"
#include "measurepose2.h"
// #include "measurepose2.h"

#include "EgSizer.h"
#include "SizeKorea-2020View.h"
#include "loadfile.h"
#include "CSelectPoseDialog.h"

#include <algorithm>

#include "SizeKoreaUtil.h"

// mj::Pose1
#include "Pose1Landmarks.h"
#include "Pose1Measures.h"

// mj::Pose2
#include "Pose2Landmarks.h"
#include "Pose2Measures.h"
#include "EgConvexhull.h"

#define LANDMARK_NUM_AUTO 70
#define LANDMARK_NUM_POSE2 70
#define LANDMARK_NUM_HELP  80

#define RESULT_NUM_POSE2 90
#define RESULT_NUM_POSE2_EXPORT 87


#define NAME_SIZE 512

#define CHECK(A)	if ( A == GVector3f( 0.0f, 0.0f, 0.0f ) ) return


enum ASERR_ErrorMode
{
	ASERR_OK = 0,
	ASERR_WRONGDATA,
	ASERR_HEAD,
	ASERR_CROTCH,
	ASERR_ARMPIT,
	ASERR_HAND,
	ASERR_FOOT
};
//////////////////////////////////
//				END				//
//////////////////////////////////


class CSizeKorea2020Doc : public CDocument
{
protected: // create from serialization only
	CSizeKorea2020Doc() noexcept;
	DECLARE_DYNCREATE(CSizeKorea2020Doc)

	// 정리된 멤버 변수
public: 
	/* !\brief	스캔 메쉬의 파일명 */
	CString m_FileName;

	/*! \brief	스캔 메쉬에 대한 포인터 */
	EgMesh *m_ScanMesh;

	/*! \brief	모델의 스캔 포즈 */
	int	m_ScanPose;

	/*! \brief	정점의 수 */
	int	m_vNum;

	/*! \brief	삼각형의 인덱스 수 */
	int	m_iNum;

	/*! \brief	정점 위치 배열에 대한 포인터 */
	GVector3f *m_Vertex;

	/*! \brief	정점 색상 배열에 대한 포인터 */
	GVector3f *m_Color;

	/*! \brief	정점 법선 배열에 대한 포인터 */
	GVector3f *m_Normal;

	/*! \brief	삼각형 인덱스 배열에 대한 포인터 */
	int	*m_FaceIdx;

	/*! \brief	정점의 x 좌표의 최소값 */
	float m_MinX;
	float m_MinXOrigin;

	/*! \brief	정점의 x 좌표의 최대값 */
	float m_MaxX;
	float m_MaxXOrigin;

	/*! \brief	정점의 y 좌표의 최소값 */
	float m_MinY;
	float m_MinYOrigin;

	/*! \brief	정점의 y 좌표의 최대값 */
	float m_MaxY;
	float m_MaxYOrigin;

	/*! \brief	정점의 z 좌표의 최소값 */
	float m_MinZ;
	float m_MinZOrigin;

	/*! \brief	정점의 z 좌표의 최대값 */
	float m_MaxZ;
	float m_MaxZOrigin;

	/*! \brief	모델의 높이(y 방향) */
	float m_Height;
	
	/*! \brief	모델의 너비(x 방향) */
	float m_Width;
	
	/*! \brief	모델의 깊이(z 방향) */
	float m_Depth;

public: // 멤버 함수
	bool ImportObj();
	void EgMesh2RawData();
	bool Copy2EgMesh();

	CSizeKorea2020View *GetView();
	
	void OnMeasurePose1();
	void OnMeasurePose2();
	void OnMeasurePose3();
	void OnMeasurePose4();
	void OnMeasurePose5();
	
	bool m_bIsPicked[LANDMARK_NUM_POSE1];
	bool m_bHelpIsPicked[LANDMARK_HELP_NUM_POSE1];


	void FindPose2Landmarks();

	//////////////////////////////////
	//			GSizerDoc			//
	//////////////////////////////////
	// mj::Member variables from GSizer
	int	m_Error;
	CDBRecord m_DBSet;

	// Landmarks Variables
	int m_MarkIdx;

	char m_LandMarkName[LANDMARK_NUM_POSE2][NAME_SIZE + 1];
	char m_SizeName[RESULT_NUM_POSE2][NAME_SIZE + 1];
	
	// Pose 2
	GLandMark	m_LandMarkPose2[LANDMARK_NUM_POSE2];
	GLandMark	m_LandMarkHelp[LANDMARK_NUM_HELP];
	GLandMark	m_OldLandMarkPose2[LANDMARK_NUM_POSE2];
	GLandMark	m_OldLandMarkHelp[LANDMARK_NUM_HELP];
	
	// Result values.
	REAL		m_ResultPose2[RESULT_NUM_POSE2];

	// Result paths view.
	BOOL		m_ResultPose2PathView[RESULT_NUM_POSE2];
	
	// Result paths.
	GObList<GVector3f>	m_ResultPathPose2[RESULT_NUM_POSE2];

	// Result Dialog.
	// CPose2ResultDialog *m_pPose2ResultDlg;
	// CPose2ResultDlg		*m_pPose2ResultDlg;

	// CPose2ResultPathDialog *m_pPose2ResultPathDlg;
	// CPose2ResultPathDlg *m_pPose2ResultPathDlg;


	// Boolean variables
	bool m_bOpened,
		m_bSizeResult,  //치수가 측정됬는지의 지표 -> true일때만 draw를 한다
		m_bArrangedXY,
		m_bArrangedY,
		m_bArrangedX,
		m_bLandmarkFind,
		m_bWoman,
		m_bParam;

	bool m_bReplaceResult_Underbust;

	

	int m_LandmarkNumExport;
	int m_LandmarkHelpNumExport;
	int m_ResultSizeNumExport;



// Intersection point with slice plane.
	GObList<GVector3f>	m_SlicePtList;

	float m_SliceHeight;

	// Intersection point with slice plane in slice plane coordinates.
	GObList<GVector3f>	m_PtOnRefPlnList;


	GObList<GVector3f>  *m_pSlicesPtList;
	GObList<GVector3f>	*m_pSliceAnglePtList;
	int m_SlicesNum;
	int m_SlicesAngleInterval;

	float m_RefAngle[5];


	//------------------------------------------------------------------------
	//						 Auto Landmark Detection                         //
	//------------------------------------------------------------------------
	// Aligned data variable and it's size.

	GObList<GVector3f>  *m_pAlignedPointsV_X,
		*m_pAlignedPointsV_Y,
		*m_pAlignedPointsV_Z;

	int	m_nCount_X,
		m_nCount_Y,
		m_nCount_Z,
		m_HInterval,
		m_DataHStart;

	// Etc. Varialbles.
	GVector3f m_NeckFBVector;
	GVector3f m_NeckFVector;
	GVector3f m_NeckBVector;


	double  m_ShoulderAngleR, m_ShoulderAngleL;
	double  m_Obesity;
	double  m_ArmObesity;
	double	m_BustCObesity;
	double  m_UnderBustCObesity;
	double  m_Armslop, m_ArmslopDown;

	double	m_NeckBackAngle, m_NeckFrontAngle, m_NeckFSAngle;

	void	InitVariable();
	void    InitVariable_Landmark();
	void	BoundingBox();
	void	UpdateOldLandMark(int ScanPose);

	int GetNumberOfLandMarks();
	int GetNumberOfSizes();
	//void GetLandMark(int i, float* xyz, char* name);
	//void GetSize(int i, float* size, char* name);
	void InputMesh(int nNode, float* ppNodes, int nElement, int* ppElements);
	void MeasureBody();
	void SetManWoman(bool sex);
	void MoveModel(float dx, float dy, float dz);
	int  GetErrorState();

	void MakeSliceCrv();

	void    Pre_FindLandmarks();
	void    FindAllLandmarks(); //extern "C" //	__declspec(dllexport)
	void    FindCrotchTempV();
	void    FindCrotchFV();
	void	FindCrotch();
	void	FindCrotchFB();
	void	FindArmPitTempV();
	void	FindArmPitFoldV();
	void	FindArmPit_ArmpitFold();

	void    FindArmpitConcavePtsV();
	void	FindArmEndV_Temp();
	void	FindArmEndV();
	void	FindSP2_1V();
	void	FindShoulderPtConvexV();
	void	FindShoulderPoint1();
	void	FindShoulderPoint_SP2_ConvexV();

	void    FindShoulderPointZ1_V();
	void    FindShouderMid();

	void	FindNeckFrontPointV1();
	void	FindNeckFrontPointV_1();
	void    FindNeckFrontPoint_maxZV();
	void    FindNeckFrontTemp1();   //2014.10.14
	void    FindNeckThyroidPointV();
	void	FindNeckSidePointV();
	void	FindNeckSidePoint_Z();
	void	FindNeckSideTempV();
	void	FindNeckSideTempV1();  //2014.10.14
	void	FindNeckSidePoint_Old();
	void	FindNeckCircRight();
	void	FindNeckBackPoint();
	void	FindNeckBackSideFront();
	void	FindHipPointV();
	void    FindBustUnderPoint();
	void    FindBustUnderPointV();
	void    FindBustUnderPointC();
	void	FindBustPoint_SlopV();
	void	FindBustPoint_Slop_mV();
	void	FindBustPoint_Ratio_mV();
	void	FindWaistRightPt_wV();
	void	FindWaistRightPt_mV();
	void	FindWaistF_B_LV();
	void	FindWaistBack_ConcavePtV();
	void	FindAbdominalPtV();
	void	FindKneePtV();
	void	FindTopHeadPoint();
	void	FindArmscyePt_1();
	void	FindElbowPtV();
	void	FindWristPt_InV();
	void	FindWristPt_FV();
	void	FindAnklePtV();
	void	FindAnklePtV2(); // 가쪽복사점 ver2.0
	void	FindLegSmallPtV();
	void    FindNavelPtV();
	void    FindNavel_RPtV();
	void	FindFootEndPt();
	void	FindFootInPt();
	void	FindBackProjectingPt();

	void    FindWaistR_SlacksBand();

	void	FindArmPitH_Back();

	///////////////////////////////////////////////
	//			mj::New Landmarks(2020. 8)		 //
	///////////////////////////////////////////////
	EgVertex *FindMidriffPt();
	EgVertex *FindNavelBackPt();
	EgVertex *FindUpperHipPt();
	EgVertex *FindTopHipPt();
	EgVertex *FindThighMidPt();
	EgVertex *FindUnderkneePt();
	EgVertex *FindCalfExtrudeFrontPt();
	EgVertex *FindAnkleInsidePt();
	EgVertex *FindShoulderRPt();
	EgVertex *FindShoulderLPt();

	EgVertex *FindNeckFrontPt(std::vector<EgVertex *> sagittal);
	EgVertex *FindNeckThyroidPt(std::vector<EgVertex *> sagittal);

	EgVertex *FindCrotchPt();
	EgVertex *FindAnklePt();

	///////////////////////////////////////////////
	//					  END					 //
	///////////////////////////////////////////////



	void	MakeFSilhouetteV();
	void    MakeSagittalSection();

	void    Calc_ArmC_Obesity_Temp();
	void    Calc_ArmC_Obesity();
	void	Calc_TorsoC_ObesityV();
	void    Calc_RefAngleArmsV();
	void    Calc_ArmSlopV();
	void	Calc_NeckAngles();
	void	Calc_NeckAngles_BackTemp();


	void    Analyze3DStructY();
	void    Arrange3DDataY();
	void    Arrange3DDataY_V();
	void    Arrange3DDataX();
	void    Arrange3DDataX_V();
	void    Arrange3DDataXY_to0();
	void    Arrange3DDataXYZ_to0();
	void    Arrange3DDataXY_V();
	void    OnArrange3DData();

	void    Measure_Circum_Arranged();
	void	Measure_Circum_UnArranged();
	void    GetResultPose2_54_56();
	void    Measure_LengthSurface(GVector3f vStart, GVector3f vEnd, GVector3f v3, int vnum, int parallelPlane, int ForB, float *Result, int resultIndex, GObList<GVector3f> *CvxPt);


	// 자동인식을 위한 보조 선들

	int m_waistType;
	GObList<GVector3f>	 m_SagittalPtsF, m_SagittalPtsB, m_ArmRInSilhouetteV, m_ArmRFSilhouetteV;
	GObList<GVector3f>	 m_FRSilhouetteV, m_FLSilhouetteV, m_SBSilhouetteV, m_SFSilhouetteV, m_SFSilhouetteV1;
	GObList<GVector3f>   m_FNeckCrv;
	int					 m_vPBNum,  // vertex number. 
		m_iPBNum;  // face index number. 54


	void PointsToLine(GObNode<GVector3f>* pNode, GObList<GVector3f> *CvxPt, int plane);
	void TransformModel(float rx, float ry, float rz, float sx, float sy, float sz);
	void InitPoseInfoDialog();


	//////////////////////////////////
	//				END				//
	//////////////////////////////////

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CSizeKorea2020Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFindLandmark();
	afx_msg void OnMeasure();
//	afx_msg void OnFileSelectpose();
	afx_msg void OnImportLandmark();
	afx_msg void OnExportLandmark();
	afx_msg void OnSetGender();
	afx_msg void OnFileSave();
	afx_msg void OnModelLandmarkSave();
	afx_msg void OnSelectPose();
	afx_msg void OnUpdateSetGender(CCmdUI *pCmdUI);
};
