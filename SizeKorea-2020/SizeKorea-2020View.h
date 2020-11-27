
// SizeKorea-2020View.h : interface of the CSizeKorea2020View class
//

#pragma once
#include "SizeKorea.h"

//////////////////////////////////
//			GSizerView			//
//////////////////////////////////
#include "grefplane.h" 
#include "gfont.h"
#define BUFSIZE 512
#define	PI	3.141592

extern float RefAngle[100];
extern bool UpdateAngle;// = true;

#ifndef SOLID
#define SOLID	0x0001
#endif
#ifndef WIRE
#define WIRE	0x0002
#endif
#ifndef POINT
#define POINT	0x0004
#endif
#ifndef DNONE
#define DNONE	0x0006
#endif
#ifndef AXIS
#define AXIS	0x0008
#endif
#ifndef BBOX
#define BBOX	0x0010
#endif
#ifndef COLOR
#define COLOR	0x0020
#endif
#ifndef FLOOR
#define FLOOR	0x0040
#endif

class CSizeKorea2020Doc;

class CSizeKorea2020View : public CView
{
protected: // create from serialization only
	CSizeKorea2020View() noexcept;
	DECLARE_DYNCREATE(CSizeKorea2020View)

public:
	// 정리된 변수들
	/*!	\brief  OpenGL 렌더링 변수들 */
	CDC *m_pDC;
	HDC m_hDC = 0;
	HGLRC m_hRC;

	/*!	\brief  카메라 투영 모드(직교, 원근) */
	enum { ORTHO = 0, PERS } m_CameraMode;

	/*!	\brief  관측 공간 지정 변수들 */
	float	m_Fovy;
	float	m_Near;
	float	m_Far;
	float	m_ViewSize;
	float	m_ViewRatio;

	/*!	\brief  장면 조작(회전, 이동, 스케일)을 위한 변수들 */
	GTrackBall	m_TrackBall;
	float	m_TrackBallCenter[3];
	float	m_RotMat[16];
	float	m_OffSet[3];
	CPoint	m_StartPos;
	CPoint	m_CurPos;
	CPoint	m_EndPos;
	CPoint	m_StartPt;
	CPoint  m_CurPt;
	CPoint  m_EndPt;
	bool	m_ManipulateReady;

	/*!	\brief  모델의 렌더링 모드(SOLID, WIRE, POINT, AXIS, COLOR, DNONE, FLOOR 등의 조합) */
	int	m_RenderMode;

// Attributes
public:
	//////////////////////////////////////////////////////////
	//						Pose 1							//
	//////////////////////////////////////////////////////////
	void OnPose1MarkTopHead();
	void OnPose1MarkEye();
	void OnPose1MarkJaw();
	void OnPose1MarkNeckBack();
	void OnPose1MarkNeckSide();
	void OnPose1MarkNeckFront();
	void OnPose1MarkShoulderR();
	void OnPose1MarkShoulderL();
	void OnPose1MarkShoulderSideR();
	void OnPose1MarkShoulderSideL();
	void OnPose1MarkArmpit();
	void OnPose1MarkBpR();
	void OnPose1MarkBpL();
	void OnPose1MarkUnderBust();
	void OnPose1MarkWaist();
	void OnPose1MarkNavel();
	void OnPose1MarkNavelBack();
	void OnPose1MarkStomachExtrude();
	void OnPose1MarkHipExtrude();
	void OnPose1MarkCrotch();
	void OnPose1MarkUnderHip();
	void OnPose1MarkMidKnee();
	void OnPose1MarkKnee();
	void OnPose1MarkCalfExtrude();
	void OnPose1MarkAnkle();
	void OnPose1MarkHand();
	void OnPose1MarkArmpitWallBackR();
	void OnPose1MarkArmpitWallBackL();
	void OnPose1MarkArmpitWallFrontR();
	void OnPose1MarkArmpitWallFrontL();

	void OnPose1MarkArmpitFoldFrontR();
	void OnPose1MarkArmpitFoldFrontL();
	void OnPose1MarkArmpitFoldBackR();
	void OnPose1MarkArmpitFoldBackL();

	void OnPose1MarkWaistFront();
	void OnPose1MarkWaistBack();

	void OnPose1MarkPopliteus();
	void OnPose1MarkElbow();
	void OnPose1MarkWrist();


	std::vector<EgPos> points;
	void SetPoint(EgPos);
	void SetPoint(std::vector<EgPos>);
	void DrawEgPoint();

	std::vector<EgPos> line;
	void SetLine(std::vector<EgPos>);
	void DrawLine();

	void DrawSilhouette();

	//////////////////////////////////////////////////////////
	//						  End							//
	//////////////////////////////////////////////////////////

	// manipulate mode variable
	enum { MNONE = -1, ROT_FREE = 0, ROT_X, ROT_Y, ROT_Z, TRANS, SCALE, ZOOM }
	m_ManipulateMode;

	// tool mode varialbe
	enum { TNONE = -1, PICK = 0, REF_LINE, REF_VEC, MEASURE_ANGLE } m_ToolMode;

	//for proper view change
	int		m_nPartOrder;
	enum {
		FRONT = 0, BACK, LEFT, RIGHT, TOP,
		HAND, FRONT_LEFT, FRONT_RIGHT,
		BACK_LEFT, BACK_RIGHT,
		RIGHT_TOP,
		LEFT_TOP,
		FRONT_TOP,
		BACK_TOP, }	m_nPartType;

	//for material color
	float	m_fMatR, m_fMatG, m_fMatB;

	//for gradient background
	float	m_fBgR, m_fBgG, m_fBgB;

	// true if slice dialog is opened
	bool	m_bIsSlice;
	bool	m_bIsSlices;

	BOOL	m_bSliceObject;
	BOOL	m_bSliceArm;
	BOOL	m_bSliceLeg;
	BOOL	m_bSliceTorso;

	//for angle measuring
	GVector3f	m_RefAngPt[2];
	int			m_RefAngPtIdx;

	//for reference frame.
	GRefPlane	m_RefPlane;
	bool		m_bShowRefPlane;
	bool		m_bShowCutPlane;
	bool		m_bShowMarkDlg;

	bool	m_RenderOnOff;
	bool	m_bShowLandmarks;
	bool	m_bShowLandmarksHelp;

	bool	m_bShowFSilhouette;
	bool	m_bShowRSilhouette = false;

	int m_SliceXYZ;
	int m_SlicePos;

	void DrawScene();
	void DrawModel();
	void DrawWireMesh();
	void DrawSolidMesh(GLenum mode = GL_RENDER);
	void DrawPoints();
	void DrawPoints_Arranged();
	void DrawAxis();
	void DrawStockScene();

	void DrawPtList(GObNode<GVector3f>* pNode, GLenum DrawType = GL_POINTS);
	// mj::Draw Measure Path
	void DrawPtList(std::vector<EgPos>* pData, GLenum DrawType = GL_POINTS);

	void DrawToolPath(CDC *pDC);
	void DrawHelpText(CDC *pDC);
	void DrawSlicePoint();
	void DrawGradientBackGround();
	void DrawBBox();
	void DrawLandMarks();
	void DrawCalcPath();
	void DrawAutoMarkHelper();
	void DrawAngRefPt();
	void DrawRefPlane();
	void DrawCutPlane();
	void DrawSlices();
	void DrawSliceAngleLines();
	///////////////////////////////////////////////////////////////////////////
	void SetProperView();
	void SetProperViewPose3(int PartType, int PartOrder);
	void MakeIdentityMatrix(float Mat[]);
	void SetupViewport(int w, int h);
	void SetupViewingTransform();
	void SetupViewingFrustum();
	///////////////////////////////////////////////////////////////////////////
	void UnProjection(double x, double y, double z, double *wx, double *wy, double *wz);
	void InitGL(void);
	void SetMaterial();
public:
	void SetActiveLandMark(int ScanPose, int MarkIdx);
	void ShowCoordInfo();
	void ReCalcViewParameter();
	void RegionZooming();
	void InitVariable();
	void GetSlicePts_RefPlane();
	void GetSlicePts_XYZslide();
	void TransByRefLine();
	void TransByRefVec();
	bool FindPickingPt(CPoint point, GVector3f *SltPt);
	void UpdatePickPt(CPoint point);
	void InsertLandMark(GVector3f Pt);
	void CalcAngle();
	//////////////////////////////////
	//				END				//
	//////////////////////////////////

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CSizeKorea2020View();
	CSizeKorea2020Doc* GetDocument() const;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// mj::OpenGL context setup
	virtual BOOL GetOldStyleRenderingContext(void);
	//virtual BOOL SetupPixelFormat(void);	commented by shyun 2020-08-20

	// mj::Render function
	//EgMesh *m_ScanMesh = NULL;
	
	// mj::OpenGL context setup
//	CStatic displayField;
	HGLRC CompHRC;
	float m_nWidth, m_nHeight;


// Generated message map functions
protected:
public:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnManualPoint();
	afx_msg void OnPickPoint();
	afx_msg void OnRegion();
	afx_msg void OnRotate();
	afx_msg void OnRotateX();
	afx_msg void OnRotateY();
	afx_msg void OnRotateZ();
	afx_msg void OnScale();
	afx_msg void OnTrans();
	afx_msg void OnDirFront();
	afx_msg void OnDirBack();
	afx_msg void OnDirLeft();
	afx_msg void OnDirRight();
	afx_msg void OnDirTop();
	afx_msg void OnDirBottom();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnViewShowsilhouette();
	afx_msg void OnViewShowcoronalsilhouette();
	afx_msg void OnExportCamera();
};

#ifndef _DEBUG  // debug version in SizeKorea-2020View.cpp
inline CSizeKorea2020Doc* CSizeKorea2020View::GetDocument() const
   { return reinterpret_cast<CSizeKorea2020Doc*>(m_pDocument); }
#endif
