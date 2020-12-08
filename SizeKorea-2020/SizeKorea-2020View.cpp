
// SizeKorea-2020View.cpp : implementation of the CSizeKorea2020View class
//
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SizeKorea-2020.h"
#endif

//#include "include/GL/glew.h"		// commented by shyun 2020-08-20
//#include "include/GL/wglew.h"		// commented by shyun 2020-08-20
//#include "include/glad/glad.h"	// Commented to solve OpenGL already included error
//#include "include/GLFW/glfw3.h"	// commented by shyun 2020-08-20
//#pragma comment(lib, "OpenGL32.lib")	// commented by shyun 2020-08-20
//#pragma comment(lib, "glu32.lib")		// commented by shyun 2020-08-20
//#pragma comment(lib, "../usr/lib/glew32.lib")	// commented by shyun 2020-08-20
//#define GLM_ENABLE_EXPERIMENTAL					// commented by shyun 2020-08-20
//#include "include/glm/glm.hpp"					// commented by shyun 2020-08-20
//#include "include/glm/gtx/transform.hpp"			// commented by shyun 2020-08-20
//#include "include/glm/gtc/matrix_transform.hpp"	// commented by shyun 2020-08-20
//#include "include/glm/gtc/type_ptr.hpp"			// commented by shyun 2020-08-20
// mj::For glutSolidSphere in CPoseDisplayDialog.cpp -- check compile order
#include "..\usr\include\GL\freeglut.h" 
//#include "..\usr\include\GL\glut.h" 
#include "SizeKorea-2020View.h"
#include "SizeKorea-2020Doc.h"
#include "Pose1Landmarks.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUFSIZE 512
#define	PI	3.141592

extern float RefAngle[100];
extern bool UpdateAngle;// = true;

#define ORIGIN GVector3f( 0.0f, 0.0f, 0.0f )
GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f };
GLfloat mat_shininess[] = { 128.0f * 0.6f };

extern EgSizer theSizer;

// CSizeKorea2020View

IMPLEMENT_DYNCREATE(CSizeKorea2020View, CView)

BEGIN_MESSAGE_MAP(CSizeKorea2020View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_MANUAL_POINT, &CSizeKorea2020View::OnManualPoint)
	ON_COMMAND(ID_PICK_POINT, &CSizeKorea2020View::OnPickPoint)
	ON_COMMAND(ID_REGION, &CSizeKorea2020View::OnRegion)
	ON_COMMAND(ID_ROTATE, &CSizeKorea2020View::OnRotate)
	ON_COMMAND(ID_ROTATE_X, &CSizeKorea2020View::OnRotateX)
	ON_COMMAND(ID_ROTATE_Y, &CSizeKorea2020View::OnRotateY)
	ON_COMMAND(ID_ROTATE_Z, &CSizeKorea2020View::OnRotateZ)
	ON_COMMAND(ID_SCALE, &CSizeKorea2020View::OnScale)
	ON_COMMAND(ID_TRANS, &CSizeKorea2020View::OnTrans)
	ON_COMMAND(ID_DIR_FRONT, &CSizeKorea2020View::OnDirFront)
	ON_COMMAND(ID_DIR_BACK, &CSizeKorea2020View::OnDirBack)
	ON_COMMAND(ID_DIR_LEFT, &CSizeKorea2020View::OnDirLeft)
	ON_COMMAND(ID_DIR_RIGHT, &CSizeKorea2020View::OnDirRight)
	ON_COMMAND(ID_DIR_TOP, &CSizeKorea2020View::OnDirTop)
	ON_COMMAND(ID_DIR_BOTTOM, &CSizeKorea2020View::OnDirBottom)
	ON_COMMAND(ID_VIEW_SHOWSILHOUETTE, &CSizeKorea2020View::OnViewShowsilhouette)
	ON_COMMAND(ID_VIEW_SHOWCORONALSILHOUETTE, &CSizeKorea2020View::OnViewShowcoronalsilhouette)
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_COMMAND(ID_EXPORT_CAMERA, &CSizeKorea2020View::OnExportCamera)
END_MESSAGE_MAP()

// CSizeKorea2020View construction/destruction

CSizeKorea2020View::CSizeKorea2020View() noexcept
{
	// TODO: add construction code here
	m_pDC = 0;
	m_hRC = 0;

	m_CameraMode = ORTHO;
	m_RenderMode = SOLID | FLOOR | AXIS | BBOX;

	m_ManipulateMode = MNONE;
	//m_ManipulateMode = ROT_FREE;
	m_ToolMode = TNONE;
	m_RenderOnOff = true;

	m_ManipulateReady = false;

	// 인터페이스를 위한 변수 선언들...
	m_nHeight = 0;
	m_nWidth = 0;

	//variables for region-zooming
	m_StartPos = CPoint(0, 0);
	m_EndPos = CPoint(0, 0);
	m_CurPos = CPoint(0, 0);

	m_StartPt = CPoint(0, 0);
	m_CurPt = CPoint(0, 0);
	m_EndPt = CPoint(0, 0);

	// for background color
	m_fBgR = 171.0f / 255.0f; //190.0f/ 255.0f; //89.0f/ 255.0f;
	m_fBgG = 225.0f / 255.0f; //255.0f/ 255.0f; /102.0f/ 255.0f;
	m_fBgB = 250.0f / 255.0f; //180.0f/ 255.0f; //186.0f/ 255.0f;

	m_fBgR = 108.0f / 255.0f;
	m_fBgG = 108.0f / 255.0f;
	m_fBgB = 108.0f / 255.0f;

	// for human materail color - skin color
	m_fMatR = 128.0f / 255.0f;
	m_fMatG = 128.0f / 255.0f;
	m_fMatB = 128.0f / 255.0f;

	// for slice view
	m_bIsSlice = false;
	m_bIsSlices = false;

	// perspective viewing frustum parameters.
	m_OffSet[0] = 0.0f;
	m_OffSet[1] = 0.0f;
	m_OffSet[2] = 0.0f;
	m_TrackBallCenter[0] = 0.0f;
	m_TrackBallCenter[1] = 0.0f;
	m_TrackBallCenter[2] = -2000.0f;
	m_Fovy = 30.0f;
	m_Near = 10.0f;
	m_Far = 500000.0f;
	m_ViewSize = -m_TrackBallCenter[2] * tan((m_Fovy*M_PI / 360.0));
	m_ViewRatio = 0.0f;

	/////////////////////////////////////////////////////////////////////////////
	m_RotMat[0] = 1.0f; m_RotMat[4] = 0.0f; m_RotMat[8] = 0.0f; m_RotMat[12] = 0.0f;
	m_RotMat[1] = 0.0f; m_RotMat[5] = 1.0f; m_RotMat[9] = 0.0f; m_RotMat[13] = 0.0f;
	m_RotMat[2] = 0.0f; m_RotMat[6] = 0.0f; m_RotMat[10] = 1.0f; m_RotMat[14] = 0.0f;
	m_RotMat[3] = 0.0f; m_RotMat[7] = 0.0f; m_RotMat[11] = 0.0f; m_RotMat[15] = 1.0f;
	//m_TrackBall.quat.SetQuat( 0.102988f, 0.0f, 0.0f, 0.994683f );

	//for angle measuring
	m_RefAngPtIdx = 0;

	// for reference plane.
	m_bShowRefPlane = false;
	m_bShowCutPlane = false;
	m_bShowMarkDlg = true;

	m_SliceXYZ = 1;


	//for show hide
	m_bShowLandmarksHelp = 0;
	m_bShowLandmarks = true;
	m_bShowFSilhouette = 0;
}

CSizeKorea2020View::~CSizeKorea2020View()
{
	if (m_pDC != NULL)
		delete m_pDC;
}

BOOL CSizeKorea2020View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSizeKorea2020View drawing
void CSizeKorea2020View::OnDraw(CDC * /*pDC*/)
{
	CSizeKorea2020Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// OpenGL 렌더링 모드로 진입한다.
	wglMakeCurrent(m_hDC, m_hRC);
	{
		// 칼라 버퍼를 초기화 한다.
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 관측 공간을 설정한다.
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		SetupViewingFrustum();

		// 그래디에이션 배경을 그린다.
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		DrawGradientBackGround();

		// 관측 변환을 수행한다.
		SetupViewingTransform();

		// 바닥 평면을 렌더링한다.
		glPushMatrix();
		DrawStockScene();
		glPopMatrix();

		// 스캔 모델이 포함된 장면을 렌더링 한다.
		glPushMatrix();
		DrawScene();
		glPopMatrix();

		glPushMatrix();
		if (m_RenderMode & AXIS)
			DrawAxis();
		glPopMatrix();

		glPushMatrix();
		// mj::Draw BBox
		// if ((m_RenderMode)& BBOX && pDoc->m_bOpened)
		if (pDoc->m_bOpened)
			DrawBBox();
		glPopMatrix();

		// mj::Draw Line
		glPushMatrix();
		if (line.size() > 0)
			DrawLine();
		glPopMatrix();

		// mj::Draw Point
		glPushMatrix();
		if (points.size() > 0)
			DrawEgPoint();
		glPopMatrix();


		// 사이즈 측정툴을 렌더링 한다.
		glPushMatrix();
		{
			theSizer.Render();
		}
		glPopMatrix();

		glFinish();

		// DrawToolPath(m_pDC);
		// DrawHelpText(m_pDC);
		glFlush();

		SwapBuffers(m_hDC);
	}
	wglMakeCurrent(NULL, NULL);
}

void CSizeKorea2020View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSizeKorea2020View diagnostics

#ifdef _DEBUG
void CSizeKorea2020View::AssertValid() const
{
	CView::AssertValid();
}

void CSizeKorea2020View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSizeKorea2020Doc* CSizeKorea2020View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSizeKorea2020Doc)));
	return (CSizeKorea2020Doc*)m_pDocument;
}
#endif //_DEBUG


// CSizeKorea2020View message handlers




///////////////////////////////////////////////////////////////
//						OPENGL SET-UP					     //
///////////////////////////////////////////////////////////////
BOOL CSizeKorea2020View::GetOldStyleRenderingContext()
{
	//A generic pixel format descriptor. This will be replaced with a more
	//specific and modern one later, so don't worry about it too much.
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,               // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		32,                             // 32-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		24,                        // 24-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};

	// Get the id number for the best match supported by the hardware device context
	// to what is described in pfd
	int pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	//If there's no match, report an error
	if (0 == pixelFormat)
	{
		AfxMessageBox(CString("ChoosePixelFormat failed"));
		return FALSE;
	}

	//If there is an acceptable match, set it as the current 
	if (FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd))
	{
		AfxMessageBox(CString("SetPixelFormat failed"));
		return FALSE;
	}

	//Create a context with this pixel format
	if (!m_hRC)
		m_hRC = wglCreateContext(m_pDC->GetSafeHdc());
	if (0 == m_hRC)
	{
		AfxMessageBox(CString("wglCreateContext failed"));
		return FALSE;
	}

	//Make it current. 
	if (FALSE == wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
	{
		AfxMessageBox(CString("wglMakeCurrent failed"));
		return FALSE;
	}
	return TRUE;
}

// Commented by shyun 2020-08-20
//BOOL CSizeKorea2020View::SetupPixelFormat() {
//	const int attribList[] =
//	{
//		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
//		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
//		WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
//		WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
//		WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
//		WGL_COLOR_BITS_ARB,     32,
//		WGL_DEPTH_BITS_ARB,     24,
//		WGL_STENCIL_BITS_ARB,   8,
//		0, 0
//	};
//
//
//	unsigned int numFormats;
//	int pixelFormat;
//	PIXELFORMATDESCRIPTOR pfd;
//
//	//Select a pixel format number
//	wglChoosePixelFormatARB(m_pDC->GetSafeHdc(), attribList, NULL, 1, &pixelFormat, &numFormats);
//
//
//	//Set it as the current 
//	if (FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd))
//	{
//		AfxMessageBox(CString("SelectPixelFormat failed"));
//		return FALSE;
//	}
//
//	return TRUE;
//}

///////////////////////////////////////////////////////////////
//							END							     //
///////////////////////////////////////////////////////////////


int CSizeKorea2020View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// 디바이스 컨텍스트를 생성하고 핸들을 구한다.
	m_pDC = new CClientDC(this);
	m_hDC = m_pDC->GetSafeHdc();

	// 픽셀포맷 디스크립터를 설정한다.
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			24,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			32,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
	};
	int pixelformat;
	pixelformat = ::ChoosePixelFormat(m_hDC, &pfd);
	::SetPixelFormat(m_hDC, pixelformat, &pfd);

	// 렌더링 컨텍스트에 대한 핸들을 구한다.
	m_hRC = ::wglCreateContext(m_hDC);

	::wglMakeCurrent(m_hDC, m_hRC);
	InitGL();
	::wglMakeCurrent(0, 0);

	return 0;
}


void CSizeKorea2020View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	if (::wglDeleteContext(m_hRC) == FALSE)
		AfxMessageBox(_T("Failed to delete wglDeleteContext."));
}


BOOL CSizeKorea2020View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	// return CView::OnEraseBkgnd(pDC);
	return TRUE;
}


//////////////////////////////////////////
//				GSizerView				//
//////////////////////////////////////////

void CSizeKorea2020View::DrawScene()
{
	CSizeKorea2020Doc* pDoc = GetDocument();

	// 파일 오픈 전이라면 리턴한다.
	if (!pDoc->m_bOpened)
		return;

	// 스캔 모델을 선택적으로 렌더링 한다.
	if (m_RenderOnOff)
		DrawModel();

	// 다음 보조적인 정보들을 렌더링 한다.
	// draw proper Y-Axis Red Line, if SliceView Dialog is openned
	DrawSlicePoint();
	// draw picked points, which are including landmarks.
	// DrawLandMarks();
	// draw angle reference points
	DrawAngRefPt();
	// draw measuring path and etc.
	// DrawCalcPath();
	DrawAutoMarkHelper(); //silhouette 선들
	// draw reference frame.
	DrawRefPlane();
	// draw cutting plane for setting environment varialbles.
	DrawCutPlane();
	DrawSlices();
	DrawSliceAngleLines();
}

void CSizeKorea2020View::DrawModel()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	pDoc->m_ScanMesh->Render();
	return;

	if (m_RenderMode & SOLID)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
		DrawSolidMesh();
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	if (m_RenderMode & WIRE)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
		DrawSolidMesh();
		glDisable(GL_POLYGON_OFFSET_FILL);
		DrawWireMesh();
	}

	if (m_RenderMode & POINT)
	{

		if (pDoc->m_bArrangedXY)
			DrawPoints_Arranged();
		else
			DrawPoints();
	}
}

void CSizeKorea2020View::DrawWireMesh() 
{
	CSizeKorea2020Doc* pDoc = GetDocument();

	glDisable(GL_LIGHTING);
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(0.00001f);
	int l = 0;
	long index0, index1, index2;
	glColor3f(0.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_TRIANGLES);
	for (l = 0; l < pDoc->m_iNum; l += 3)
	{
		index0 = pDoc->m_FaceIdx[l];
		index1 = pDoc->m_FaceIdx[l + 1];
		index2 = pDoc->m_FaceIdx[l + 2];

		glVertex3f(pDoc->m_Vertex[index0].X, pDoc->m_Vertex[index0].Y, pDoc->m_Vertex[index0].Z);
		glVertex3f(pDoc->m_Vertex[index1].X, pDoc->m_Vertex[index1].Y, pDoc->m_Vertex[index1].Z);
		glVertex3f(pDoc->m_Vertex[index2].X, pDoc->m_Vertex[index2].Y, pDoc->m_Vertex[index2].Z);
	}
	glEnd();
	glPolygonMode(GL_FRONT, GL_FILL);
	glLineWidth(0.00001f);
	glEnable(GL_LIGHTING);
}

void CSizeKorea2020View::DrawSolidMesh(GLenum mode) 
{
	CSizeKorea2020Doc* pDoc = GetDocument();

	if (m_RenderMode & COLOR)
		glDisable(GL_LIGHTING);
	long l = 0L;
	int name = 0;
	int idx0, idx1, idx2;
	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_TRIANGLES);
	for (l = 0; l < pDoc->m_iNum; l += 3)
	{
		idx0 = pDoc->m_FaceIdx[l];
		idx1 = pDoc->m_FaceIdx[l + 1];
		idx2 = pDoc->m_FaceIdx[l + 2];

		glNormal3f(pDoc->m_Normal[idx0].X, pDoc->m_Normal[idx0].Y, pDoc->m_Normal[idx0].Z);
		if (pDoc->m_Color)
			glColor3f(pDoc->m_Color[idx0].X, pDoc->m_Color[idx0].Y, pDoc->m_Color[idx0].Z);
		glVertex3f(pDoc->m_Vertex[idx0].X, pDoc->m_Vertex[idx0].Y, pDoc->m_Vertex[idx0].Z);

		glNormal3d(pDoc->m_Normal[idx1].X, pDoc->m_Normal[idx1].Y, pDoc->m_Normal[idx1].Z);
		if (pDoc->m_Color)
			glColor3f(pDoc->m_Color[idx1].X, pDoc->m_Color[idx1].Y, pDoc->m_Color[idx1].Z);
		glVertex3d(pDoc->m_Vertex[idx1].X, pDoc->m_Vertex[idx1].Y, pDoc->m_Vertex[idx1].Z);

		glNormal3d(pDoc->m_Normal[idx2].X, pDoc->m_Normal[idx2].Y, pDoc->m_Normal[idx2].Z);
		if (pDoc->m_Color)
			glColor3f(pDoc->m_Color[idx2].X, pDoc->m_Color[idx2].Y, pDoc->m_Color[idx2].Z);
		glVertex3d(pDoc->m_Vertex[idx2].X, pDoc->m_Vertex[idx2].Y, pDoc->m_Vertex[idx2].Z);
	}
	glEnd();

	if (m_RenderMode & COLOR)
		glEnable(GL_LIGHTING);
}

void CSizeKorea2020View::DrawPoints() {
	CSizeKorea2020Doc* pDoc = GetDocument();

	glPointSize(0.0001f);
	if (m_RenderMode & COLOR)
		glDisable(GL_LIGHTING);

	int i;
	glBegin(GL_POINTS);
	for (i = 0; i < pDoc->m_vNum; i += 1)
	{
		glNormal3d(pDoc->m_Normal[i].X, pDoc->m_Normal[i].Y, pDoc->m_Normal[i].Z);
		glVertex3d(pDoc->m_Vertex[i].X, pDoc->m_Vertex[i].Y, pDoc->m_Vertex[i].Z);
		if (pDoc->m_Color)
			glColor3d(pDoc->m_Color[i].X, pDoc->m_Color[i].Y, pDoc->m_Color[i].Z);

	}
	glEnd();
	if (m_RenderMode & COLOR)
		glEnable(GL_LIGHTING);

	glPointSize(0.0001f);
}

void CSizeKorea2020View::DrawPoints_Arranged() {
	CSizeKorea2020Doc* pDoc = GetDocument();
	glPointSize(0.0001f);
	if (m_RenderMode & COLOR)
		glDisable(GL_LIGHTING);

	int i;
	::glBegin(GL_POINTS);

	for (i = 0; i < (int)pDoc->m_Height; i += 2)
	{
		GObNode<GVector3f>* pNode = pDoc->m_pAlignedPointsV_Y[i].m_pLeftEnd;

		while (pNode)
		{ //시작높이에서 탐색할 좌,우 범위 찾기
			GVector3f p = pNode->data;
			glVertex3d(p.X, p.Y, p.Z);
			pNode = pNode->m_pRight;
		}
	}
	glEnd();

	glBegin(GL_POINTS);

	if (m_RenderMode & COLOR)
		glEnable(GL_LIGHTING);

	glPointSize(0.0001f);
}

void CSizeKorea2020View::DrawAxis() {
	glLoadIdentity();

	float size = -(m_TrackBallCenter[2]) / 30.0f;
	glTranslatef(0.0f, 0.0f, m_TrackBallCenter[2]);
	glMultMatrixf((float*)m_RotMat);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glColor3f(1.0, 0.0, 0.0);

	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(size, 0.0, 0.0);
	glVertex3f(size, size / 5, -size / 5);
	glVertex3f(size, -size / 5, size / 5);
	glVertex3f(size, size / 5, size / 5);
	glVertex3f(size, -size / 5, -size / 5);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);

	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, size, 0.0);
	glVertex3f(-size / 5, size, -size / 5);
	glVertex3f(0.0, size, 0.0);
	glVertex3f(size / 5, size, -size / 5);
	glVertex3f(0.0, size, 0.0);
	glVertex3f(0.0, size, 0.0);
	glVertex3f(0.0, size, size / 5);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);

	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, size);
	glVertex3f(size / 5, size / 5, size);
	glVertex3f(-size / 5, -size / 5, size);
	glVertex3f(-size / 5, size / 5, size);
	glVertex3f(size / 5, size / 5, size);
	glVertex3f(-size / 5, -size / 5, size);
	glVertex3f(size / 5, -size / 5, size);
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void CSizeKorea2020View::DrawStockScene()
{
	// 10cm 간격으로 그리드를 렌더링 한다.
	glDisable(GL_LIGHTING);
	glLineWidth(1.0f);
	if (m_RenderMode & FLOOR)
	{
		glBegin(GL_LINES);
		for (int i = -1200; i <= 1200; i += 100)
		{
			if (i == 0)
			{
				glColor3f(0.0, 0.0, 0.0);
				glVertex3f((float)i / 1.0, 0.0, 1200.0);
				glVertex3f((float)i / 1.0, 0.0, -1200.0);
				glVertex3f(-1200.0, 0.0, (float)i / 1.0);
				glVertex3f(1200.0, 0.0, (float)i / 1.0);
			}
			else
			{
				glColor3f(0.9f, 0.9f, 0.9f);
				glVertex3f((float)i / 1.0, 0.0, 1200.0);
				glVertex3f((float)i / 1.0, 0.0, -1200.0);
				glVertex3f(-1200.0, 0.0, (float)i / 1.0);
				glVertex3f(1200.0, 0.0, (float)i / 1.0);
			}
		}
		glEnd();
	}
	glEnable(GL_LIGHTING);
}

void CSizeKorea2020View::DrawPtList(GObNode<GVector3f>* pNode, GLenum DrawType) {
	glBegin(DrawType);
	while (pNode)
	{
		glVertex3d(pNode->data.X, pNode->data.Y, pNode->data.Z);
		pNode = pNode->m_pRight;
	}
	glEnd();
}

void CSizeKorea2020View::DrawPtList(std::vector<EgPos>* pData, GLenum DrawType) {
	glBegin(DrawType);
	for (EgPos pos : *pData) {
		glVertex3d(pos.m_Coords[0], pos.m_Coords[1], pos.m_Coords[2]);
	}
	glEnd();
}


void CSizeKorea2020View::DrawCalcPath() {
	CSizeKorea2020Doc* pDoc = GetDocument();

	if (!pDoc->m_bSizeResult)
		return;

	GObNode<GVector3f> *pNode;
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(2.0f);
	int i = 0;

	glDisable(GL_LIGHTING);

	switch (pDoc->m_ScanPose)
	{
	case 1 :
		/*
		for (i = 0; i < RESULT_NUM_POSE1; i++) {
			if (pDoc->m_ResultPathViewPose1[i]) {
				DrawPtList(pDoc->m_ResultPathPose1, GL_LINE_STRIP);
			}
		}
		*/
		break;
	case 2:
		for (i = 0; i < 88; i++) //RESULT_NUM_POSE2
		{
			if (pDoc->m_ResultPose2PathView[i])
			{
				pNode = pDoc->m_ResultPathPose2[i].m_pLeftEnd;
				switch (i)
				{
				case 20:
				case 21:
				case 22:
				case 23:
				case 24:
				case 25:
				case 26:
				case 27:
				case 28:
				case 29:
				case 30:
				case 31:
				case 32:
				case 33:
				case 34:
				case 35:
				case 36:
				case 37:
				case 38:
				case 39:
				case 40:
				case 41:
					DrawPtList(pNode, GL_LINE_LOOP); //둘레는 line line loop으로 
					break;
					//case 30:
					//	DrawPtList( pNode, GL_POINTS );
					//	break;

				default:
					DrawPtList(pNode, GL_LINE_STRIP); //길이는 LINE_STRIP으로
					break;

				} //switch
			} //if view
		}//for

		if (pDoc->m_ResultPose2PathView[72]) {
			pNode = pDoc->m_ResultPathPose2[72].m_pLeftEnd;
			glColor3f(0.0f, 0.0f, 1.0f);
			DrawPtList(pNode, GL_LINE_LOOP);

			pNode = pDoc->m_ResultPathPose2[72].m_pLeftEnd;
			glColor3f(0.0f, 1.0f, 1.0f);
			glPointSize(4.0);
			DrawPtList(pNode, GL_POINTS);
			glPointSize(0.0001f);
		}

		for (i = 0; i < RESULT_NUM_POSE2; i++)
		{
			if (pDoc->m_ResultPose2PathView[i])
			{
				pNode = pDoc->m_ResultPathPose2[i].m_pLeftEnd;
				switch (i)
				{
				case 27:
					DrawPtList(pNode, GL_LINE_LOOP);
					break;
				default:
					DrawPtList(pNode, GL_LINE_STRIP);
					break;
				}
			}
		}
		break;
	}
	glEnable(GL_LIGHTING);
	glLineWidth(0.0001f);
}

void CSizeKorea2020View::DrawToolPath(CDC *pDC) 
{
	if (m_ManipulateReady)
		return;
	//draw Zoom -drag rectangle

	if (m_ManipulateMode == ZOOM)
	{
		RECT rect;
		rect.left = rect.bottom = rect.right = rect.top = 0;
		rect.left = m_StartPos.x;		rect.top = m_StartPos.y;
		rect.right = m_CurPos.x;		rect.bottom = m_CurPos.y;
		pDC->DrawFocusRect(&rect);
	}
	switch (m_ToolMode)
	{
	case REF_LINE:
		pDC->MoveTo(m_StartPos.x, m_StartPos.y);
		m_CurPos.y = m_StartPos.y;
		pDC->LineTo(m_CurPos.x, m_CurPos.y);
		break;
	case REF_VEC:
		pDC->MoveTo(m_StartPos.x, m_StartPos.y);
		pDC->LineTo(m_CurPos.x, m_CurPos.y);
		break;
	}
}

void CSizeKorea2020View::DrawHelpText(CDC *pDC) {
	/*
	CSizeKorea2020Doc* pDoc = GetDocument();

	CFont newFont, *oldFont;
	newFont.CreateFont(17, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, (CString)"system");
	oldFont = pDC->SelectObject(&newFont);
	pDC->SetTextColor(RGB(0, 255, 0));
	pDC->SetBkMode(TRANSPARENT);

	pDC->TextOutW(7, 5, (CString)FileName);
	pDC->SelectObject(oldFont);
	newFont.DeleteObject();
	*/
}

void CSizeKorea2020View::DrawSlicePoint() {
	CSizeKorea2020Doc* pDoc = GetDocument();

	if (m_bIsSlice)
		return;
	GObNode<GVector3f> *pNode = pDoc->m_SlicePtList.m_pLeftEnd;

	// draw points
	glDisable(GL_LIGHTING);
	glPointSize(2.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	while (pNode)
	{
		GVector3f v = pNode->data;
		glVertex3f(v[0], v[1], v[2]);
		pNode = pNode->m_pRight;
	}
	glEnd();

	glPointSize(0.0001f);
	glEnable(GL_LIGHTING);
}

void CSizeKorea2020View::DrawGradientBackGround() {
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	float dx, dy, dz;
	if (m_nWidth > m_nHeight)
	{
		dx = m_ViewSize * m_ViewRatio;
		dy = m_ViewSize;
		dz = m_TrackBallCenter[2];
	}
	else
	{
		dx = m_ViewSize;
		dy = m_ViewSize / m_ViewRatio;
		dz = m_TrackBallCenter[2];
	}
	glBegin(GL_POLYGON);
	glColor3f(m_fBgR, m_fBgG, m_fBgB);
	glVertex3f(-dx, dy, dz);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(-dx, -dy, dz);
	glVertex3f(dx, -dy, dz);
	glColor3f(m_fBgR, m_fBgG, m_fBgB);
	glVertex3f(dx, dy, dz);
	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void CSizeKorea2020View::DrawBBox() {
	CSizeKorea2020Doc* pDoc = GetDocument();

	if (pDoc->m_ScanMesh != NULL) {
		glDisable(GL_LIGHTING);
		glColor3f(1.0f, 0.0f, 0.0f);
		glLineWidth(1.0f);
		glBegin(GL_LINES);

	/*
	glVertex3f(pDoc->m_MinX, pDoc->m_MinY, pDoc->m_MinZ);
	glVertex3f(pDoc->m_MaxX, pDoc->m_MinY, pDoc->m_MinZ);

	glVertex3f(pDoc->m_MaxX, pDoc->m_MinY, pDoc->m_MinZ);
	glVertex3f(pDoc->m_MaxX, pDoc->m_MinY, pDoc->m_MaxZ);

	glVertex3f(pDoc->m_MaxX, pDoc->m_MinY, pDoc->m_MaxZ);
	glVertex3f(pDoc->m_MinX, pDoc->m_MinY, pDoc->m_MaxZ);

	glVertex3f(pDoc->m_MinX, pDoc->m_MinY, pDoc->m_MaxZ);
	glVertex3f(pDoc->m_MinX, pDoc->m_MinY, pDoc->m_MinZ);

	glVertex3f(pDoc->m_MinX, pDoc->m_MinY, pDoc->m_MinZ);
	glVertex3f(pDoc->m_MinX, pDoc->m_MaxY, pDoc->m_MinZ);

	glVertex3f(pDoc->m_MaxX, pDoc->m_MinY, pDoc->m_MinZ);
	glVertex3f(pDoc->m_MaxX, pDoc->m_MaxY, pDoc->m_MinZ);

	glVertex3f(pDoc->m_MaxX, pDoc->m_MinY, pDoc->m_MaxZ);
	glVertex3f(pDoc->m_MaxX, pDoc->m_MaxY, pDoc->m_MaxZ);

	glVertex3f(pDoc->m_MinX, pDoc->m_MinY, pDoc->m_MaxZ);
	glVertex3f(pDoc->m_MinX, pDoc->m_MaxY, pDoc->m_MaxZ);

	glVertex3f(pDoc->m_MinX, pDoc->m_MaxY, pDoc->m_MinZ);
	glVertex3f(pDoc->m_MaxX, pDoc->m_MaxY, pDoc->m_MinZ);

	glVertex3f(pDoc->m_MaxX, pDoc->m_MaxY, pDoc->m_MinZ);
	glVertex3f(pDoc->m_MaxX, pDoc->m_MaxY, pDoc->m_MaxZ);

	glVertex3f(pDoc->m_MaxX, pDoc->m_MaxY, pDoc->m_MaxZ);
	glVertex3f(pDoc->m_MinX, pDoc->m_MaxY, pDoc->m_MaxZ);

	glVertex3f(pDoc->m_MinX, pDoc->m_MaxY, pDoc->m_MaxZ);
	glVertex3f(pDoc->m_MinX, pDoc->m_MaxY, pDoc->m_MinZ);
	*/

		EgBox bbox = pDoc->m_ScanMesh->m_BndBox;
		EgPos bv[8];
		bbox.GetCornerPts(bv, true);
		glVertex3f(bv[0].m_Coords[0], bv[0].m_Coords[1], bv[0].m_Coords[2]);
		glVertex3f(bv[1].m_Coords[0], bv[1].m_Coords[1], bv[1].m_Coords[2]);

		glVertex3f(bv[1].m_Coords[0], bv[1].m_Coords[1], bv[1].m_Coords[2]);
		glVertex3f(bv[3].m_Coords[0], bv[3].m_Coords[1], bv[3].m_Coords[2]);

		glVertex3f(bv[3].m_Coords[0], bv[3].m_Coords[1], bv[3].m_Coords[2]);
		glVertex3f(bv[2].m_Coords[0], bv[2].m_Coords[1], bv[2].m_Coords[2]);

		glVertex3f(bv[2].m_Coords[0], bv[2].m_Coords[1], bv[2].m_Coords[2]);
		glVertex3f(bv[0].m_Coords[0], bv[0].m_Coords[1], bv[0].m_Coords[2]);

		glVertex3f(bv[2].m_Coords[0], bv[2].m_Coords[1], bv[2].m_Coords[2]);
		glVertex3f(bv[6].m_Coords[0], bv[6].m_Coords[1], bv[6].m_Coords[2]);

		glVertex3f(bv[3].m_Coords[0], bv[3].m_Coords[1], bv[3].m_Coords[2]);
		glVertex3f(bv[7].m_Coords[0], bv[7].m_Coords[1], bv[7].m_Coords[2]);

		glVertex3f(bv[1].m_Coords[0], bv[1].m_Coords[1], bv[1].m_Coords[2]);
		glVertex3f(bv[5].m_Coords[0], bv[5].m_Coords[1], bv[5].m_Coords[2]);

		glVertex3f(bv[0].m_Coords[0], bv[0].m_Coords[1], bv[0].m_Coords[2]);
		glVertex3f(bv[4].m_Coords[0], bv[4].m_Coords[1], bv[4].m_Coords[2]);

		glVertex3f(bv[4].m_Coords[0], bv[4].m_Coords[1], bv[4].m_Coords[2]);
		glVertex3f(bv[5].m_Coords[0], bv[5].m_Coords[1], bv[5].m_Coords[2]);

		glVertex3f(bv[5].m_Coords[0], bv[5].m_Coords[1], bv[5].m_Coords[2]);
		glVertex3f(bv[7].m_Coords[0], bv[7].m_Coords[1], bv[7].m_Coords[2]);

		glVertex3f(bv[7].m_Coords[0], bv[7].m_Coords[1], bv[7].m_Coords[2]);
		glVertex3f(bv[6].m_Coords[0], bv[6].m_Coords[1], bv[6].m_Coords[2]);

		glVertex3f(bv[6].m_Coords[0], bv[6].m_Coords[1], bv[6].m_Coords[2]);
		glVertex3f(bv[4].m_Coords[0], bv[4].m_Coords[1], bv[4].m_Coords[2]);
		glEnd();

		glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(0.00001f);

		glEnable(GL_LIGHTING);
	}
}

void CSizeKorea2020View::SetPoint(EgPos p) {
	points.push_back(p);
}

void CSizeKorea2020View::SetPoint(std::vector<EgPos> positions) {
	if (positions.empty())
		points = positions;
	else
		points.insert(points.end(), positions.begin(), positions.end());
}

void CSizeKorea2020View::SetLine(std::vector<EgPos> positions) {
	if (line.empty())
		line = positions;
	else
		line.insert(line.end(), positions.begin(), positions.end());
}

void CSizeKorea2020View::DrawEgPoint() {
	glDisable(GL_LIGHTING);

	glBegin(GL_POINTS);

	for (int i = 0; i < points.size(); i++) {
		glColor3f(0, 0, 1);
		glPushMatrix();
		glTranslatef(points[i].m_Coords[0], points[i].m_Coords[1], points[i].m_Coords[2]);
		gluSphere(gluNewQuadric(), 2.0f, 10.0f, 10.f);
		glPopMatrix();
	}
	glEnd();
	
	glEnable(GL_LIGHTING);
	glPointSize(0.0001f);
}

void CSizeKorea2020View::DrawLine() {
	glBegin(GL_LINES);
	for (int i = 0; i < line.size(); i += 2) {
		EgPos coord0 = line[i];
		EgPos coord1 = line[i + 1];

		glVertex3f(coord0.m_Coords[0], coord0.m_Coords[1], coord0.m_Coords[2]);
		glVertex3f(coord1.m_Coords[0], coord1.m_Coords[1], coord1.m_Coords[2]);
	}
	glEnd();
}

void CSizeKorea2020View::DrawSilhouette() {
	CSizeKorea2020Doc* pDoc = GetDocument();

}

void CSizeKorea2020View::DrawLandMarks() {
	CSizeKorea2020Doc* pDoc = GetDocument();
	GLandMark *LandMark;
	int LandMarkNum;

	// mj::New Landmark draw for pose 1 (EgMesh)
	EgPos* egLandmark;

	switch (pDoc->m_ScanPose)
	{
	case 1:
		/*
		egLandmark = pDoc->m_LandmarkPose1;
		LandMarkNum = LANDMARK_NUM_POSE1;

		m_bShowLandmarksHelp = true;

		if (m_bShowFSilhouette)
			DrawSilhouette();

		if (m_bShowRSilhouette)
			DrawSilhouette();

		if (m_bShowLandmarks) {
			// White landmarks
			m_fMatR = 1.0f;
			m_fMatG = 1.0f;
			m_fMatB = 1.0f;
			SetMaterial();
			
			for (int i = 0; i < LandMarkNum; i++) {
				EgPos pt = egLandmark[i];

				if (pt == EgPos(0.0, 0.0, 0.0))
					continue;
				glPushMatrix();
				glTranslatef(pt[0], pt[1], pt[2]);

				if (pDoc->m_bIsPicked[i])
				{
					float old_R = m_fMatR;
					float old_G = m_fMatG;
					float old_B = m_fMatB;
					// Red for picked landmarks
					m_fMatR = 1.0f;
					m_fMatG = 0.0f;
					m_fMatB = 0.0f;
					SetMaterial();

					gluSphere(gluNewQuadric(), 4.0f, 10.0f, 10.f);
					m_fMatR = old_R;
					m_fMatG = old_G;
					m_fMatB = old_B;
					SetMaterial();
				}
				else
				{
					gluSphere(gluNewQuadric(), 4.0f, 10.0f, 10.f);
				}
				glPopMatrix();
			}
		}

		if (m_bShowLandmarksHelp) {
			m_fMatR = 1.0f;   //1,1,0
			m_fMatG = 1.0f;
			m_fMatB = 0.0f;
			SetMaterial();
			// egLandmark = pDoc->m_LandmarkHelpPose1;
			LandMarkNum = LANDMARK_HELP_NUM_POSE1;

			for (int i = 0; i < LANDMARK_HELP_NUM_POSE1; i++)
			{
				EgPos pt = egLandmark[i];

				if (pt == EgPos(0.0, 0.0, 0.0))
					continue;

				glPushMatrix();
				glTranslatef(pt[0], pt[1], pt[2]);
				gluSphere(gluNewQuadric(), 2.0f, 10.0f, 10.f);
				glPopMatrix();
			}
		}
		*/
		break;

	case 2:
		LandMark = pDoc->m_LandMarkPose2;
		LandMarkNum = LANDMARK_NUM_POSE2;

		if (m_bShowLandmarks) {
			m_fMatR = 1.0f;
			m_fMatG = 1.0f;
			m_fMatB = 1.0f;
			SetMaterial();

			for (int i = 0; i < 12; i++)
			{
				GLandMark pt = LandMark[i];
				if (pt == GLandMark(0.0f, 0.0f, 0.0f))
					continue;
				glPushMatrix();
				glTranslatef(pt[0], pt[1], pt[2]);
				if (pt.m_bIsPicked)
				{
					float old_R = m_fMatR;
					float old_G = m_fMatG;
					float old_B = m_fMatB;
					m_fMatR = 1.0f;
					m_fMatG = 0.0f;
					m_fMatB = 0.0f;
					SetMaterial();
					// glutSolidSphere(4.0f, 10.0f, 10.0f);
					gluSphere(gluNewQuadric(), 4.0f, 10.0f, 10.f);
					m_fMatR = old_R;
					m_fMatG = old_G;
					m_fMatB = old_B;
					SetMaterial();
				}
				else
				{
					// glutSolidSphere(4.0f, 10.0f, 10.0f);
					gluSphere(gluNewQuadric(), 4.0f, 10.0f, 10.f);
				}
				glPopMatrix();
			}
			for (int i = 16; i < LandMarkNum; i++)
			{
				GLandMark pt = LandMark[i];
				if (pt == GLandMark(0.0f, 0.0f, 0.0f))
					continue;
				glPushMatrix();
				glTranslatef(pt[0], pt[1], pt[2]);
				if (pt.m_bIsPicked)
				{
					float old_R = m_fMatR;
					float old_G = m_fMatG;
					float old_B = m_fMatB;
					m_fMatR = 1.0f;
					m_fMatG = 0.0f;
					m_fMatB = 0.0f;
					SetMaterial();
					// glutSolidSphere(4.0f, 10.0f, 10.0f);
					gluSphere(gluNewQuadric(), 4.0f, 10.0f, 10.f);
					m_fMatR = old_R;
					m_fMatG = old_G;
					m_fMatB = old_B;
					SetMaterial();
				}
				else
				{
					// glutSolidSphere(4.0f, 10.0f, 10.0f);
					gluSphere(gluNewQuadric(), 4.0f, 10.0f, 10.f);
				}
				glPopMatrix();
			}
		}

		if (m_bShowLandmarksHelp) { 	//보조점은 다른 색으로 표시...
			m_fMatR = 1.0f;   //1,1,0
			m_fMatG = 1.0f;
			m_fMatB = 0.0f;
			SetMaterial();
			LandMark = pDoc->m_LandMarkHelp;
			LandMarkNum = LANDMARK_NUM_HELP;

			for (int i = 0; i < 12; i++)
			{
				GLandMark pt = LandMark[i];
				if (pt == GLandMark(0.0f, 0.0f, 0.0f))
					continue;

				glPushMatrix();
				glTranslatef(pt[0], pt[1], pt[2]);

				// glutSolidSphere(2.0f, 10.0f, 10.0f);
				gluSphere(gluNewQuadric(), 2.0f, 10.0f, 10.f);
				glPopMatrix();
			}

			m_fMatR = 1.0f; //1,0,1
			m_fMatG = 0.0f;
			m_fMatB = 1.0f;
			SetMaterial();
			for (int i = 12; i < 34; i++)
			{
				GLandMark pt = LandMark[i];
				if (pt == GLandMark(0.0f, 0.0f, 0.0f))
					continue;

				glPushMatrix();
				glTranslatef(pt[0], pt[1], pt[2]);

				// glutSolidSphere(2.0f, 10.0f, 10.0f);
				gluSphere(gluNewQuadric(), 2.0f, 10.0f, 10.f);
				glPopMatrix();
			}

			m_fMatR = 1.0f; //1,1,0
			m_fMatG = 1.0f;
			m_fMatB = 0.0f;
			SetMaterial();
			for (int i = 34; i < 42; i++)
			{
				GLandMark pt = LandMark[i];
				if (pt == GLandMark(0.0f, 0.0f, 0.0f))
					continue;

				glPushMatrix();
				glTranslatef(pt[0], pt[1], pt[2]);

				// glutSolidSphere(2.0f, 10.0f, 10.0f);
				gluSphere(gluNewQuadric(), 2.0f, 10.0f, 10.f);
				glPopMatrix();
			}

			m_fMatR = 0.0f; //0,1,1
			m_fMatG = 1.0f;
			m_fMatB = 1.0f;
			SetMaterial();
			for (int i = 42; i < 46; i++)
			{
				GLandMark pt = LandMark[i];
				if (pt == GLandMark(0.0f, 0.0f, 0.0f))
					continue;

				glPushMatrix();
				glTranslatef(pt[0], pt[1], pt[2]);

				// glutSolidSphere(2.0f, 10.0f, 10.0f);
				gluSphere(gluNewQuadric(), 2.0f, 10.0f, 10.f);
				glPopMatrix();
			}

			m_fMatR = 0.0f; //0,1,0
			m_fMatG = 0.8f;
			m_fMatB = 0.2f;
			SetMaterial();
			for (int i = 46; i < 58; i++)
			{
				GLandMark pt = LandMark[i];
				if (pt == GLandMark(0.0f, 0.0f, 0.0f))
					continue;

				glPushMatrix();
				glTranslatef(pt[0], pt[1], pt[2]);

				// glutSolidSphere(2.0f, 10.0f, 10.0f);
				gluSphere(gluNewQuadric(), 2.0f, 10.0f, 10.f);
				glPopMatrix();
			}

			m_fMatR = 1.0f; //0,1,0
			m_fMatG = 0.0f;
			m_fMatB = 1.0f;
			SetMaterial();
			for (int i = 58; i < LandMarkNum; i++)
			{
				GLandMark pt = LandMark[i];
				if (pt == GLandMark(0.0f, 0.0f, 0.0f))
					continue;

				glPushMatrix();
				glTranslatef(pt[0], pt[1], pt[2]);

				// glutSolidSphere(2.0f, 10.0f, 10.0f);
				gluSphere(gluNewQuadric(), 2.0f, 10.0f, 10.f);
				glPopMatrix();
			}
		}
		/*
		*/
		break;
	default:
		return;
	}

}


void CSizeKorea2020View::DrawAutoMarkHelper() {
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bLandmarkFind) //m_bSizeResult
		return;

	//Draw 정면, 측면 실루엣, 시상단면
	if (pDoc->m_ScanPose == 1 && m_bShowFSilhouette)
	{
		float old_R = m_fMatR;
		float old_G = m_fMatG;
		float old_B = m_fMatB;
		m_fMatR = 0.0f;
		m_fMatG = 1.0f;
		m_fMatB = 1.0f;
		SetMaterial();

		//--------------------------------------------
		//   실루엣선들 

		glDisable(GL_LIGHTING);
		GObNode<GVector3f> *pNode;
		glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(2.0f);

		pNode = pDoc->m_SagittalPtsF.m_pLeftEnd;
		DrawPtList(pNode, GL_LINE_STRIP);


		//draw FRsilhouette -right
		pNode = pDoc->m_FRSilhouetteV.m_pLeftEnd;
		DrawPtList(pNode, GL_LINE_STRIP);

		//-------------------------------------------
		// 자동인식을 위한 보조선들 

		// FNeck을 위한 목앞선
		pNode = pDoc->m_FNeckCrv.m_pLeftEnd;
		DrawPtList(pNode, GL_LINE_STRIP);


		glEnable(GL_LIGHTING);
		glLineWidth(0.0001f);

		m_fMatR = old_R;
		m_fMatG = old_R;
		m_fMatB = old_R;
		SetMaterial();
	}
}

void CSizeKorea2020View::DrawAngRefPt() {
	if (m_RefAngPt[0] == ORIGIN && m_RefAngPt[1] == ORIGIN)
		return;
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(m_RefAngPt[0][0], m_RefAngPt[0][1], m_RefAngPt[0][2]);
	glColor3f(1.0f, 0.0f, 0.0f);
	// glutSolidSphere(m_ViewSize / 100.0f, 20.0f, 20.0f);
	gluSphere(gluNewQuadric(), m_ViewSize / 100.0f, 20.0f, 20.f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(m_RefAngPt[1][0], m_RefAngPt[1][1], m_RefAngPt[1][2]);
	glColor3f(1.0f, 0.0f, 0.0f);
	// glutSolidSphere(m_ViewSize / 100.0f, 20.0f, 20.0f);
	gluSphere(gluNewQuadric(), m_ViewSize / 100.0f, 20.0f, 20.f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();

	glBegin(GL_LINES);
	glVertex3f(m_RefAngPt[0][0], m_RefAngPt[0][1], m_RefAngPt[0][2]);
	glVertex3f(m_RefAngPt[1][0], m_RefAngPt[1][1], m_RefAngPt[1][2]);
	glEnd();

	glEnable(GL_LIGHTING);
}

void CSizeKorea2020View::DrawRefPlane() {
	if (!m_bShowRefPlane)
		return;

	CSizeKorea2020Doc* pDoc = GetDocument();

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glColor4f(0.7f, 0.7f, 0.0f, 0.6f);
	{
		GVector3f Origin = m_RefPlane.GetOrigin();
		float mat[16];
		m_RefPlane.GetRotMatrix(mat);
		glTranslatef(Origin[0], Origin[1], Origin[2]);
		glMultMatrixf((float*)mat);
		glBegin(GL_POLYGON);
		{
			REAL size = m_RefPlane.GetSize();
			glVertex3f(-size, 0.0f, -size);
			glVertex3f(-size, 0.0f, size);
			glVertex3f(size, 0.0f, size);
			glVertex3f(size, 0.0f, -size);
		}
		glEnd();
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
}

void CSizeKorea2020View::DrawCutPlane() {
	if (!m_bShowCutPlane)
		return;

	CSizeKorea2020Doc* pDoc = GetDocument();

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glLineWidth(3.0f);

	// 머리분할평면그리기.
	if (pDoc->m_LandMarkPose2[2] != ORIGIN)
	{
		::glPushMatrix();
		::glColor4f(0.7f, 0.7f, 0.7f, 0.6f);
		{
			GQuaternion q;
			q.SetFromAngleAxis(RefAngle[0], GVector3f(1.0f, 0.0f, 0.0f));
			GVector3f Origin = pDoc->m_LandMarkPose2[2];  //방패연골아래점
			float mat[16];
			q.GetGLMatrixf(mat);
			glTranslatef(Origin[0], Origin[1], Origin[2]);
			glMultMatrixf((float*)mat);
			glBegin(GL_POLYGON);
			{
				REAL size = 100.0f;
				glVertex3f(-size, 0.0f, -size);
				glVertex3f(-size, 0.0f, size);
				glVertex3f(size, 0.0f, size);
				glVertex3f(size, 0.0f, -size);
			}
			glEnd();
		}
		glColor3f(1.0f, 1.0f, 1.0f);
		glPopMatrix();
	}

	float RefAngle5 = RefAngle[5] / M_PI * 180.0f;
	// 오른팔분할평면보기
	if (pDoc->m_LandMarkPose2[9] != ORIGIN)
	{
		glPushMatrix();
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		{
			GQuaternion q;
			q.SetFromAngleAxis(RefAngle5, GVector3f(0.0f, 0.0f, 1.0f));
			GVector3f Origin = pDoc->m_LandMarkPose2[9];
			float mat[16];
			q.GetGLMatrixf(mat);
			glTranslatef(Origin[0], Origin[1], Origin[2]);
			glMultMatrixf((float*)mat);
			glBegin(GL_POLYGON);
			{
				REAL size = 200.0f;
				glVertex3f(-(size), 0.0f, -size);
				glVertex3f(-(size), 0.0f, size);
				glVertex3f(size, 0.0f, size);
				glVertex3f(size, 0.0f, -size);
			}
			glEnd();
			glBegin(GL_LINES);
			{
				REAL size = 200.0f;
				glVertex3f(-(size), 0.0f, 0.0f);
				glVertex3f(size, 0.0f, 0.0f);
			}
			glEnd();

		}
		glColor3f(1.0f, 1.0f, 1.0f);
		glPopMatrix();
	}
	// 왼팔분할평면보기
	float RefAngle6 = -RefAngle[6] / M_PI * 180.0f;
	if (pDoc->m_LandMarkPose2[10] != ORIGIN)
	{
		glPushMatrix();
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		{
			GQuaternion q;
			q.SetFromAngleAxis(RefAngle6, GVector3f(0.0f, 0.0f, -1.0f));
			GVector3f Origin = pDoc->m_LandMarkPose2[10];
			float mat[16];
			q.GetGLMatrixf(mat);
			glTranslatef(Origin[0], Origin[1], Origin[2]);
			glMultMatrixf((float*)mat);
			glBegin(GL_POLYGON);
			{
				REAL size = 200.0f;
				glVertex3f(-size, 0.0f, -size);
				glVertex3f(-size, 0.0f, size);
				glVertex3f((size), 0.0f, size);
				glVertex3f((size), 0.0f, -size);
			}
			glEnd();
			glBegin(GL_LINES);
			{
				REAL size = 200.0f;
				glVertex3f(-size, 0.0f, 0.0f);
				glVertex3f((size), 0.0f, 0.0f);
			}
			glEnd();
		}
		glColor3f(1.0f, 1.0f, 1.0f);
		glPopMatrix();
	}

	// 허리아래- 오른팔분할평면보기
	float RefAngle3 = RefAngle[3] / M_PI * 180.0f;
	if (pDoc->m_LandMarkHelp[42] != ORIGIN)
	{
		glPushMatrix();
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		{
			GQuaternion q;
			q.SetFromAngleAxis(RefAngle3, GVector3f(0.0f, 0.0f, 1.0f));
			GVector3f Origin = pDoc->m_LandMarkHelp[42];
			float mat[16];
			q.GetGLMatrixf(mat);
			glTranslatef(Origin[0], Origin[1], Origin[2]);
			glMultMatrixf((float*)mat);
			glBegin(GL_POLYGON);
			{
				REAL size = 200.0f;
				glVertex3f(-(size*2.0f), 0.0f, -size);
				glVertex3f(-(size*2.0f), 0.0f, size);
				glVertex3f(0.0f, 0.0f, size);
				glVertex3f(0.0f, 0.0f, -size);
			}
			glEnd();
			glBegin(GL_LINES);
			{
				REAL size = 200.0f;
				glVertex3f(-(size*2.0f), 0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
			}
			glEnd();

		}
		glColor3f(1.0f, 1.0f, 1.0f);
		glPopMatrix();
	}

	// 왼팔분할평면보기
	float RefAngle4 = -RefAngle[4] / M_PI * 180.0f;
	if (pDoc->m_LandMarkHelp[43] != ORIGIN)
	{
		glPushMatrix();
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		{
			GQuaternion q;
			q.SetFromAngleAxis(RefAngle4, GVector3f(0.0f, 0.0f, -1.0f));
			GVector3f Origin = pDoc->m_LandMarkHelp[43];
			float mat[16];
			q.GetGLMatrixf(mat);
			glTranslatef(Origin[0], Origin[1], Origin[2]);
			glMultMatrixf((float*)mat);
			glBegin(GL_POLYGON);
			{
				REAL size = 200.0f;
				glVertex3f(0.0f, 0.0f, -size);
				glVertex3f(0.0f, 0.0f, size);
				glVertex3f((size*2.0f), 0.0f, size);
				glVertex3f((size*2.0f), 0.0f, -size);
			}
			glEnd();
			glBegin(GL_LINES);
			{
				REAL size = 200.0f;
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f((size*2.0f), 0.0f, 0.0f);
			}
			glEnd();
		}
		glColor3f(1.0f, 1.0f, 1.0f);
		glPopMatrix();
	}

	glLineWidth(1.0f);

	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
}

void CSizeKorea2020View::DrawSlices() {
	CSizeKorea2020Doc *pDoc = GetDocument();

	if (pDoc->m_pSlicesPtList == NULL)
		return;

	glDisable(GL_LIGHTING);
	glPointSize(2.0);
	glColor3f(1.0f, 0.0f, 0.0f);

	GObNode<GVector3f> *pNode;
	for (int k = 0; k < pDoc->m_SlicesNum; k++)
	{
		pNode = pDoc->m_pSlicesPtList[k].m_pLeftEnd;

		glBegin(GL_LINE_LOOP);
		while (pNode)
		{
			GVector3f v = pNode->data;
			glVertex3f(v[0], v[1], v[2]);
			pNode = pNode->m_pRight;
		}
		glEnd();
	}

	glPointSize(0.0001f);
	glEnable(GL_LIGHTING);
}

void CSizeKorea2020View::DrawSliceAngleLines() {
	CSizeKorea2020Doc *pDoc = GetDocument();

	if (pDoc->m_pSliceAnglePtList == NULL)
		return;

	glDisable(GL_LIGHTING);
	glPointSize(2.0);
	glColor3f(1.0f, 0.0f, 0.0f);

	GObNode<GVector3f> *pNode;
	for (int k = 0; k < pDoc->m_SlicesNum; k++)
	{
		pNode = pDoc->m_pSliceAnglePtList[k].m_pLeftEnd;

		GVector3f c = pNode->data;
		pNode = pNode->m_pRight;

		glBegin(GL_LINES);
		while (pNode)
		{
			GVector3f v = pNode->data;
			glVertex3f(c[0], c[1], c[2]);
			glVertex3f(v[0], v[1], v[2]);
			pNode = pNode->m_pRight;
		}
		glEnd();
	}

	glPointSize(0.0001f);
	glEnable(GL_LIGHTING);
}

///////////////////////////////////////////////////////////////////////////
void CSizeKorea2020View::SetProperView() 
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	m_ManipulateMode = MNONE;

	// to make View Matrix initial state.
	m_TrackBallCenter[0] = 0.0f;
	m_TrackBallCenter[1] = 0.0f;
	m_TrackBallCenter[2] = -2000.0f;

	MakeIdentityMatrix(m_RotMat);

	m_OffSet[0] = -400.0;
	m_OffSet[1] = -pDoc->m_Height + 100.0; // MaxY approximate offset : the center of each part is on the origin

	switch (m_nPartType)
	{
	case HAND:
		m_OffSet[0] = -pDoc->m_MinX;
		m_TrackBall.quat.SetQuat(0.0f, 1.0f, 0.0f, 0.0f);
		break;
	case FRONT:
		m_TrackBall.quat.SetQuat(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case BACK:
		m_TrackBall.quat.SetQuat(0.0f, 1.0f, 0.0f, 0.0f);
		break;
	case LEFT:
		m_TrackBall.quat.SetQuat(0.0f, -sin(PI / 4), 0.0f, cos(PI / 4));
		break;
	case RIGHT:
		m_TrackBall.quat.SetQuat(0.0f, sin(PI / 4), 0.0f, cos(PI / 4));
		break;
	case RIGHT_TOP:
		m_TrackBall.quat.SetQuat(0.162878f, 0.688092, 0.162878, 0.688092f);
		break;
	case LEFT_TOP:
		m_TrackBall.quat.SetQuat(0.162878f, -0.688092, -0.162878, 0.688092f);
		break;
	case TOP:
		m_TrackBall.quat.SetQuat(-sin(PI / 4), 0.0f, 0.0f, cos(PI / 4));
		break;
	case FRONT_LEFT:
		m_TrackBall.quat.SetQuat(0.0f, -sin(PI / 8), 0.0f, cos(PI / 8));
		break;
	case FRONT_RIGHT:
		m_TrackBall.quat.SetQuat(0.0f, sin(PI / 8), 0.0f, cos(PI / 8));
		break;
	case BACK_LEFT:
		m_TrackBall.quat.SetQuat(0.0f, -sin(3 * PI / 8), 0.0f, cos(3 * PI / 8));
		break;
	case BACK_RIGHT:
		m_TrackBall.quat.SetQuat(0.0f, sin(3 * PI / 8), 0.0f, cos(3 * PI / 8));
		break;
	case FRONT_TOP:
		m_TrackBall.quat.SetQuat(sin(PI / 6), 0.0f, 0.0f, cos(PI / 6));
		break;
	case BACK_TOP:
		m_TrackBall.quat.SetQuat(0.0f, 1.0f, sin(PI / 6), 0.0f);
		break;
	default:
		break;
	}

	m_OffSet[1] = m_OffSet[1] * m_nPartOrder / 5;

	// view size is fitted
	m_TrackBallCenter[2] = -1000.0f;
	Invalidate(false);
}

void CSizeKorea2020View::SetProperViewPose3(int PartType, int PartOrder)
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	m_ManipulateMode = MNONE;

	EgMesh *pMesh = pDoc->m_ScanMesh;
	EgVec3 p(pDoc->m_MinX, pDoc->m_MinY, pDoc->m_MinZ);
	EgVec3 q(pDoc->m_MaxX, pDoc->m_MaxY, pDoc->m_MaxZ);
	double sz = ::dist(p, q);
	m_TrackBallCenter[0] = 0.0f;
	m_TrackBallCenter[1] = 0.0f;
	m_TrackBallCenter[2] = -sz;
		
	// 모델 좌표계에서...
	EgVec3 c = (p + q) * 0.5;
	c[0] += m_OffSet[0];
	c[1] += m_OffSet[1];
	c[2] += m_OffSet[2];
	_cprintf("%lf, %lf, %lf\n", c[0], c[1], c[2]);

	// 월드 좌표계에서...
	EgVec3 offset;
	offset[0] = (m_RotMat[0] * c[0] + m_RotMat[4] * c[1] + m_RotMat[8] * c[2]);
	offset[1] = (m_RotMat[1] * c[0] + m_RotMat[5] * c[1] + m_RotMat[9] * c[2]);
	offset[2] = (m_RotMat[2] * c[0] + m_RotMat[6] * c[1] + m_RotMat[10] * c[2]);

	_cprintf("%lf, %lf, %lf\n", offset[0], offset[1], offset[2]);

	//m_OffSet[0] += c[0];
	//m_OffSet[1] += c[1];
	//m_OffSet[2] += c[2];

	//c[1] = p[1] + (q[1] - p[1]) * (double)PartOrder / 5.0;
	//m_OffSet[0] = -(m_RotMat[0] * c[0] + m_RotMat[1] * c[1] + m_RotMat[2] * c[2]);
	//m_OffSet[1] = -(m_RotMat[4] * c[0] + m_RotMat[5] * c[1] + m_RotMat[6] * c[2]);
	//m_OffSet[2] = -(m_RotMat[8] * c[0] + m_RotMat[9] * c[1] + m_RotMat[10] * c[2]);

	// 회전 변환을 설정한다.
	//MakeIdentityMatrix(m_RotMat);
	switch (PartType)
	{
	case FRONT:
		m_TrackBall.quat.SetQuat(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case BACK:
		m_TrackBall.quat.SetQuat(0.0f, 1.0f, 0.0f, 0.0f);
		break;
	case LEFT:
		m_TrackBall.quat.SetQuat(0.0f, -sin(PI / 4), 0.0f, cos(PI / 4));
		break;
	case RIGHT:
		m_TrackBall.quat.SetQuat(0.0f, sin(PI / 4), 0.0f, cos(PI / 4));
		break;
	case RIGHT_TOP:
		m_TrackBall.quat.SetQuat(0.162878f, 0.688092, 0.162878, 0.688092f);
		break;
	case LEFT_TOP:
		m_TrackBall.quat.SetQuat(0.162878f, -0.688092, -0.162878, 0.688092f);
		break;
	case TOP:
		m_TrackBall.quat.SetQuat(-sin(PI / 4), 0.0f, 0.0f, cos(PI / 4));
		break;
	case FRONT_LEFT:
		m_TrackBall.quat.SetQuat(0.0f, -sin(PI / 8), 0.0f, cos(PI / 8));
		break;
	case FRONT_RIGHT:
		m_TrackBall.quat.SetQuat(0.0f, sin(PI / 8), 0.0f, cos(PI / 8));
		break;
	case BACK_LEFT:
		m_TrackBall.quat.SetQuat(0.0f, -sin(3 * PI / 8), 0.0f, cos(3 * PI / 8));
		break;
	case BACK_RIGHT:
		m_TrackBall.quat.SetQuat(0.0f, sin(3 * PI / 8), 0.0f, cos(3 * PI / 8));
		break;
	case FRONT_TOP:
		m_TrackBall.quat.SetQuat(sin(PI / 6), 0.0f, 0.0f, cos(PI / 6));
		break;
	case BACK_TOP:
		m_TrackBall.quat.SetQuat(0.0f, 1.0f, sin(PI / 6), 0.0f);
		break;
	}

	m_TrackBall.GetRotMatrix(m_RotMat);
	m_OffSet[0] -= (m_RotMat[0] * offset[0] + m_RotMat[1] * offset[1] + m_RotMat[2] * offset[2]);
	m_OffSet[1] -= (m_RotMat[4] * offset[0] + m_RotMat[5] * offset[1] + m_RotMat[6] * offset[2]);
	m_OffSet[2] -= (m_RotMat[8] * offset[0] + m_RotMat[9] * offset[1] + m_RotMat[10] * offset[2]);
	_cprintf("%lf, %lf, %lf\n", m_OffSet[0], m_OffSet[1], m_OffSet[2]);

	SetupViewingTransform();
	m_OffSet[0] = 0.0;
	m_OffSet[1] = 0.0;
	m_OffSet[2] = 0.0;
	Invalidate(false);
}

void CSizeKorea2020View::MakeIdentityMatrix(float Mat[]) {
	Mat[0] = 1.0;	Mat[4] = 0.0;	Mat[8] = 0.0;	Mat[12] = 0.0;
	Mat[1] = 0.0;	Mat[5] = 1.0;	Mat[9] = 0.0;	Mat[13] = 0.0;
	Mat[2] = 0.0;	Mat[6] = 0.0;	Mat[10] = 1.0;	Mat[14] = 0.0;
	Mat[3] = 0.0;	Mat[7] = 0.0;	Mat[11] = 0.0;	Mat[15] = 1.0;
}

void CSizeKorea2020View::SetupViewport(int w, int h) {
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	glViewport(0, 0, w, h);
	wglMakeCurrent(0, 0);
}

void CSizeKorea2020View::SetupViewingTransform() {
	// 1. Move origin to trackball's center.
	glTranslatef(m_TrackBallCenter[0], m_TrackBallCenter[1], m_TrackBallCenter[2]);

	// 2. Get Rotation Matrix.
	m_TrackBall.GetRotMatrix(m_RotMat);

	// 3. Apply Rotation Matrix.
	glMultMatrixf((float*)m_RotMat);

	// 4. Apply the OffSet.
	glTranslatef(m_OffSet[0], m_OffSet[1], m_OffSet[2]);
}

void CSizeKorea2020View::SetupViewingFrustum() 
{
	m_ViewSize = -m_TrackBallCenter[2] * tan((m_Fovy * M_PI / 360.0));

	switch (m_CameraMode)
	{
	case ORTHO:
		if (m_nWidth > m_nHeight)
			glOrtho(-m_ViewSize * m_ViewRatio, m_ViewSize * m_ViewRatio, -m_ViewSize, m_ViewSize, -m_Far, m_Far);
		else
			glOrtho(-m_ViewSize, m_ViewSize, -m_ViewSize / m_ViewRatio,	m_ViewSize / m_ViewRatio, -m_Far, m_Far);
		break;

	case PERS:
	default:
		gluPerspective(m_Fovy, m_ViewRatio, m_Near, m_Far);
		break;
	}
}
///////////////////////////////////////////////////////////////////////////
void CSizeKorea2020View::UnProjection(double x, double y, double z, double *wx, double *wy, double *wz) {
	GLint viewport[4];
	double mvmat[16], promat[16];
	GLint realy;

	// Opengl Support Mode.
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	{
		// get viewport
		glGetIntegerv(GL_VIEWPORT, viewport);
		if (m_ManipulateMode == ZOOM)
		{
			glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glGetDoublev(GL_MODELVIEW_MATRIX, mvmat);
			glPopMatrix();
		}
		else
			// get model view matrix.
			glGetDoublev(GL_MODELVIEW_MATRIX, mvmat);

		glGetDoublev(GL_PROJECTION_MATRIX, promat);

		realy = viewport[3] - (GLint)y - 1;

		gluUnProject(x, realy, z, mvmat, promat, viewport, wx, wy, wz);
	}
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
}

void CSizeKorea2020View::InitGL() {
	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);

	// other initialize routine can be added to following....
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(0.0001f);
	glPointSize(0.0001f);

	GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f };
	GLfloat light_specular[] = { 0.8f, 0.8f, 0.8f };
	GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f };

	GLfloat light_position1[4] = { 10000.0f, 0.0f, 0.0f, 0.0f };
	GLfloat light_position2[4] = { 0.0f, 10000.0f, 0.0f, 0.0f };
	GLfloat light_position3[4] = { 0.0f, 0.0f, 10000.0f, 0.0f };

	GLfloat color1[4], color2[4], color3[4];
	color1[0] = 0.7f; color1[1] = 0.7f; color1[2] = 0.7f; color1[3] = 1.0f;
	color2[0] = 0.7f; color2[1] = 0.7f; color2[2] = 0.7f; color2[3] = 1.0f;
	color3[0] = 0.7f; color3[1] = 0.7f; color3[2] = 0.7f; color3[3] = 1.0f;

	GLfloat ambient[4] = { 0.3f,0.3f,0.3f,1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, color1);

	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, color2);

	glLightfv(GL_LIGHT2, GL_POSITION, light_position3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, color3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, color3);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	SetMaterial();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
}

void CSizeKorea2020View::SetMaterial() {
	GLfloat mat_ambient[] = { m_fMatR, m_fMatG, m_fMatB };
	GLfloat mat_diffuse[] = { m_fMatR + 0.2f, m_fMatG + 0.2f, m_fMatB + 0.2f };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void CSizeKorea2020View::SetActiveLandMark(int ScanPose, int MarkIdx) {
	CSizeKorea2020Doc *pDoc = GetDocument();

	GLandMark *LandMark = NULL;
	int LandMarkNum = 0;

	EgPos* egLandmark = NULL;
	switch (ScanPose)
	{
	case 1:
		// egLandmark = pDoc->m_LandmarkPose1;
		LandMarkNum = LANDMARK_NUM_POSE1;


		for (int i = 0; i < LandMarkNum; pDoc->m_bIsPicked[i] = false, i++)
			pDoc->m_bIsPicked[MarkIdx] = true;
		break;
	case 2:
		LandMark = pDoc->m_LandMarkPose2;
		LandMarkNum = LANDMARK_NUM_POSE2;

		for (int i = 0; i < LandMarkNum; LandMark[i].m_bIsPicked = false, i++)
			LandMark[MarkIdx].m_bIsPicked = true;
		break;
	default:
		return;
	}
}

void CSizeKorea2020View::ShowCoordInfo() {
	/*
	GLandMark *LandMark;
	int LandMarkNum;
	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
	CDialogBar *pCtlBar;
	CStatic *pStatic;

	switch (m_ScanPose)
	{
	case 1:
		LandMark = m_LandMarkPose2;
		LandMarkNum = LANDMARK_NUM_POSE2;
		pCtlBar = &(pFrame->m_wndPose2MarkDlg);
		pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
		break;
	case 2:
		LandMark = m_LandMarkPose2;
		LandMarkNum = LANDMARK_NUM_POSE2;
		pCtlBar = &(pFrame->m_wndPose2MarkDlg);
		pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
		break;
	}
	GLandMark pt;
	for (int i = 0; i < LandMarkNum; i++)
	{
		if (LandMark[i].m_bIsPicked == true)
			pt = LandMark[i];
	}
	CString str;
	str.Format("[ %4.2f, %4.2f, %4.2f ]", pt[0], pt[1], pt[2]);
	pStatic->SetWindowText(str);
	*/
}

void CSizeKorea2020View::ReCalcViewParameter() 
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	float max = -10000.0f;
	max = MAX(max, fabs(pDoc->m_MinX));
	max = MAX(max, fabs(pDoc->m_MaxX));
	max = MAX(max, fabs(pDoc->m_MinY));
	max = MAX(max, fabs(pDoc->m_MaxY));
	max = MAX(max, fabs(pDoc->m_MinZ));
	max = MAX(max, fabs(pDoc->m_MaxZ));
	m_TrackBallCenter[0] = 0.0f;
	m_TrackBallCenter[1] = 0.0f;
	m_TrackBallCenter[2] = max * (-1.0f) / tan(m_Fovy * M_PI / 180.0f) - 500.0f;
	m_ViewSize = -m_TrackBallCenter[2] * tan((m_Fovy * M_PI / 360.0f));
	m_OffSet[1] = -max / 2.0f;
}

void CSizeKorea2020View::RegionZooming() {
	double left, right, bottom, top, useless, depth; // Z value in world coordinate is useless
	float dx, dy;
	// If user drags very small region, don't zoom it. 
	if ((m_EndPos.x - m_StartPos.x) < 30 || (m_EndPos.y - m_StartPos.y) < 30)
		return;

	if (m_CameraMode == ORTHO)
	{
		UnProjection((double)m_StartPos.x, (double)m_StartPos.y, 0.0, &left, &top, &useless);
		UnProjection((double)m_EndPos.x, (double)m_EndPos.y, 0.0, &right, &bottom, &useless);
	}
	else
	{
		//depth = -(m_Near+m_TrackBallCenter[2]) / (m_Far-m_Near);
		depth = 0.0;
		UnProjection((double)m_StartPos.x, (double)m_StartPos.y, (double)depth, &left, &top, &useless);
		UnProjection((double)m_EndPos.x, (double)m_EndPos.y, (double)depth, &right, &bottom, &useless);
		left = left * (-m_TrackBallCenter[2]) / 10.0f;
		right = right * (-m_TrackBallCenter[2]) / 10.0f;
		bottom = bottom * (-m_TrackBallCenter[2]) / 10.0f;
		top = top * (-m_TrackBallCenter[2]) / 10.0f;
	}


	dx = -(left + right) / 2;
	dy = -(bottom + top) / 2;
	m_TrackBallCenter[2] = -((fabs(right - left) / 2.0f) / tan(m_Fovy*M_PI / 360.0));

	m_OffSet[0] += (m_RotMat[0] * dx + m_RotMat[1] * dy);
	m_OffSet[1] += (m_RotMat[4] * dx + m_RotMat[5] * dy);
	m_OffSet[2] += (m_RotMat[8] * dx + m_RotMat[9] * dy);

	m_StartPos = m_EndPos = CPoint(0, 0);
}

void CSizeKorea2020View::InitVariable() 
{
	m_CameraMode = ORTHO;
	m_RenderMode = 0x0000;
	m_RenderMode |= SOLID | FLOOR | COLOR;
	m_ManipulateMode = MNONE;
	m_ToolMode = TNONE;

	////////////////////////////////////////////////////////////////////
	m_StartPos = CPoint(0, 0);
	m_EndPos = CPoint(0, 0);
	m_CurPos = CPoint(0, 0);
	m_StartPt = CPoint(0, 0);
	m_CurPt = CPoint(0, 0);
	m_EndPt = CPoint(0, 0);

	m_OffSet[0] = 0.0f;
	m_OffSet[1] = 0.0f;
	m_OffSet[2] = 0.0f;

	m_RotMat[0] = 1.0f; m_RotMat[4] = 0.0f; m_RotMat[8] = 0.0f; m_RotMat[12] = 0.0f;
	m_RotMat[1] = 0.0f; m_RotMat[5] = 1.0f; m_RotMat[9] = 0.0f; m_RotMat[13] = 0.0f;
	m_RotMat[2] = 0.0f; m_RotMat[6] = 0.0f; m_RotMat[10] = 1.0f; m_RotMat[14] = 0.0f;
	m_RotMat[3] = 0.0f; m_RotMat[7] = 0.0f; m_RotMat[11] = 0.0f; m_RotMat[15] = 1.0f;
	//m_TrackBall.quat.SetQuat( 0.102988f, 0.0f, 0.0f, 0.994683f );
	////////////////////////////////////////////////////////////////////
	//for angle measuring
	m_RefAngPtIdx = 0;
	m_RenderOnOff = true;
	m_bShowLandmarksHelp = 0;
	m_bShowLandmarks = true;
	m_bShowFSilhouette = 0;
}

void CSizeKorea2020View::GetSlicePts_RefPlane() {
	CSizeKorea2020Doc *pDoc = GetDocument();

	GPlane pln = m_RefPlane.GetPlaneEq();
	pDoc->m_SlicePtList.DeleteAll();
	pDoc->m_PtOnRefPlnList.DeleteAll();

	GPlane lplane, rplane;
	double tan1 = tan(3.14159265 / 2.0 - RefAngle[5]);
	double tan2 = tan(3.14159265 / 2.0 + RefAngle[6]);
	rplane.MakePlane(GVector3f(-1.0f, tan1, 0.0f), pDoc->m_LandMarkPose2[9]);
	lplane.MakePlane(GVector3f(1.0f, tan2, 0.0f), pDoc->m_LandMarkPose2[10]);

	for (int i = 0; i < pDoc->m_iNum; i += 3)
	{
		GVector3f b0, b1, b2;
		b0 = pDoc->m_Vertex[pDoc->m_FaceIdx[i]];
		b1 = pDoc->m_Vertex[pDoc->m_FaceIdx[i + 1]];
		b2 = pDoc->m_Vertex[pDoc->m_FaceIdx[i + 2]];

		if (rplane.IsAbovePlane(b0) || rplane.IsAbovePlane(b1) || rplane.IsAbovePlane(b2))
			continue;
		if (lplane.IsAbovePlane(b0) || lplane.IsAbovePlane(b1) || lplane.IsAbovePlane(b2))
			continue;

		if (pln.IsAbovePlane(b0) && pln.IsAbovePlane(b1) && pln.IsAbovePlane(b2))
			continue;
		if (pln.IsBelowPlane(b0) && pln.IsBelowPlane(b1) && pln.IsBelowPlane(b2))
			continue;
		if (pln.GetSignedDistance(b0) * pln.GetSignedDistance(b1) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b1 - b0);
			if (m_RefPlane.PtInRect(CrossPt))
			{
				pDoc->m_PtOnRefPlnList.Insert(m_RefPlane.GetLocalCoord(CrossPt));
				pDoc->m_SlicePtList.Insert(CrossPt);
			}
		}
		if (pln.GetSignedDistance(b1) * pln.GetSignedDistance(b2) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b1, b2 - b1);
			if (m_RefPlane.PtInRect(CrossPt))
			{
				pDoc->m_PtOnRefPlnList.Insert(m_RefPlane.GetLocalCoord(CrossPt));
				pDoc->m_SlicePtList.Insert(CrossPt);
			}
		}
		if (pln.GetSignedDistance(b2) * pln.GetSignedDistance(b0) < 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b2 - b0);
			if (m_RefPlane.PtInRect(CrossPt))
			{
				pDoc->m_PtOnRefPlnList.Insert(m_RefPlane.GetLocalCoord(CrossPt));
				pDoc->m_SlicePtList.Insert(CrossPt);
			}
		}
	}

	m_bIsSlice = false; //true;
	Invalidate(true);
}

void CSizeKorea2020View::GetSlicePts_XYZslide() {
	GPlane pln;
	GVector3f norm;
	switch (m_SliceXYZ)
	{
	case 0: //X
		norm = GVector3f(1.0f, 0.0f, 0.0f);
		break;
	case 1://Y
		norm = GVector3f(0.0f, 1.0f, 0.0f);
		break;
	case 2://Z
		norm = GVector3f(0.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
	int d = m_SlicePos;
	pln = GPlane(norm, (float)d);

	CSizeKorea2020Doc *pDoc = GetDocument();

	pDoc->m_SliceHeight = (float)m_SlicePos;
	pDoc->m_SlicePtList.DeleteAll();
	pDoc->m_PtOnRefPlnList.DeleteAll();
	for (int i = 0; i < pDoc->m_iNum; i += 3)
	{
		GVector3f b0, b1, b2;
		b0 = pDoc->m_Vertex[pDoc->m_FaceIdx[i]];
		b1 = pDoc->m_Vertex[pDoc->m_FaceIdx[i + 1]];
		b2 = pDoc->m_Vertex[pDoc->m_FaceIdx[i + 2]];
		if (pln.IsAbovePlane(b0) && pln.IsAbovePlane(b1) && pln.IsAbovePlane(b2))
			continue;
		if (pln.IsBelowPlane(b0) && pln.IsBelowPlane(b1) && pln.IsBelowPlane(b2))
			continue;
		if (pln.GetSignedDistance(b0) * pln.GetSignedDistance(b1) <= 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b1 - b0);
			pDoc->m_SlicePtList.Insert(CrossPt);
		}
		if (pln.GetSignedDistance(b1) * pln.GetSignedDistance(b2) <= 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b1, b2 - b1);
			pDoc->m_PtOnRefPlnList.Insert(m_RefPlane.GetLocalCoord(CrossPt));
			pDoc->m_SlicePtList.Insert(CrossPt);
		}
		if (pln.GetSignedDistance(b2) * pln.GetSignedDistance(b0) <= 0)
		{
			GVector3f CrossPt = pln.GetProjectionPt(b0, b2 - b0);
			pDoc->m_PtOnRefPlnList.Insert(m_RefPlane.GetLocalCoord(CrossPt));
			pDoc->m_SlicePtList.Insert(CrossPt);
		}
	}

	CConvexHull cvx;
	cvx.m_HullPtList = cvx.m_PtList = pDoc->m_SlicePtList;
	cvx.ArrangeHullPtXZ();

	pDoc->m_SlicePtList.DeleteAll();
	pDoc->m_SlicePtList = cvx.m_HullPtList;

	m_bIsSlice = false;
	Invalidate(true);
}

void CSizeKorea2020View::TransByRefLine() {
	CSizeKorea2020Doc *pDoc = GetDocument();

	GPlane pln;
	pln.MakePlane(GVector3f(0.0f, 0.0f, 1.0f), GVector3f(0.0f, 0.0f, 0.0f));
	double wx, wy, wz;
	UnProjection((double)m_StartPos.x, (double)m_StartPos.y, 0.0, &wx, &wy, &wz);
	for (int i = 0; i < pDoc->m_vNum; i++)
		pDoc->m_Vertex[i][1] -= (float)wy;

	GLandMark *LandMark = new GLandMark();
	int LandMarkNum;
	switch (pDoc->m_ScanPose)
	{
	case 1:
		LandMark = pDoc->m_LandMarkPose2;
		LandMarkNum = LANDMARK_NUM_POSE2;
		break;
	case 2:
		LandMark = pDoc->m_LandMarkPose2;
		LandMarkNum = LANDMARK_NUM_POSE2;
		break;
	}
	for (int i = 0; i < LandMarkNum; i++)
	{
		if (LandMark[i] != GLandMark(0.0f, 0.0f, 0.0f))
			LandMark[i][1] -= (float)wy;
	}

	pDoc->BoundingBox();
	ReCalcViewParameter();
	Invalidate(false);
}

void CSizeKorea2020View::TransByRefVec() {
	CSizeKorea2020Doc *pDoc = GetDocument();

	GPlane pln;
	pln.MakePlane(GVector3f(0.0f, 0.0f, 1.0f), GVector3f(0.0f, 0.0f, 0.0f));
	double wsx, wsy, wsz;
	double wex, wey, wez;
	UnProjection((double)m_StartPos.x, (double)m_StartPos.y, 0.0, &wsx, &wsy, &wsz);
	UnProjection((double)m_CurPos.x, (double)m_CurPos.y, 0.0, &wex, &wey, &wez);
	// rotate the point croud to be front direction 
	GVector3f left_right_axis(wex - wsx, 0.0f, wez - wsz);
	double sin_theta = left_right_axis.Z / sqrt(left_right_axis.X * left_right_axis.X + left_right_axis.Z * left_right_axis.Z);
	double cos_theta = left_right_axis.X / sqrt(left_right_axis.X * left_right_axis.X + left_right_axis.Z * left_right_axis.Z);
	double new_x, new_y, new_z;
	for (int i = 0; i < pDoc->m_vNum; i++)
	{
		new_x = cos_theta * pDoc->m_Vertex[i].X + sin_theta * pDoc->m_Vertex[i].Z;
		new_y = pDoc->m_Vertex[i].Y;
		new_z = -sin_theta * pDoc->m_Vertex[i].X + cos_theta * pDoc->m_Vertex[i].Z;
		pDoc->m_Vertex[i].setVector(new_x, new_y, new_z);
	}
	GLandMark *LandMark = new GLandMark();
	int LandMarkNum;
	switch (pDoc->m_ScanPose)
	{
	case 1:
		LandMark = pDoc->m_LandMarkPose2;
		LandMarkNum = LANDMARK_NUM_POSE2;
		break;
	case 2:
		LandMark = pDoc->m_LandMarkPose2;
		LandMarkNum = LANDMARK_NUM_POSE2;
		break;
	}
	for (int i = 0; i < LandMarkNum; i++)
	{
		if (LandMark[i] != GLandMark(0.0f, 0.0f, 0.0f))
		{
			new_x = cos_theta * LandMark[i].X + sin_theta * LandMark[i].Z;
			new_y = LandMark[i].Y;
			new_z = -sin_theta * LandMark[i].X + cos_theta * LandMark[i].Z;
			LandMark[i].setVector(new_x, new_y, new_z);
		}
	}
	pDoc->BoundingBox();
	Invalidate(false);
}

bool CSizeKorea2020View::FindPickingPt(CPoint point, GVector3f *SltPt) {
	CSizeKorea2020Doc *pDoc = GetDocument();

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	{
		GLuint selectBuf[BUFSIZE];
		GLint hits;
		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);

		//specify selection buffer
		glSelectBuffer(BUFSIZE, selectBuf);

		//change rendering mode
		glRenderMode(GL_SELECT);
		//initialize name stack
		glInitNames();
		glPushName(0);

		//change current matrix mode to projection mode
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		//specify selection region.
		int offset = 1;
		gluPickMatrix((GLdouble)point.x, (GLdouble)(viewport[3] - point.y), offset, offset, viewport);
		SetupViewingFrustum();

		{
			int i, name = 0;
			for (i = 0; i < pDoc->m_iNum; i += 3)
			{
				GVector3f v0 = pDoc->m_Vertex[pDoc->m_FaceIdx[i]];
				GVector3f v1 = pDoc->m_Vertex[pDoc->m_FaceIdx[i + 1]];
				GVector3f v2 = pDoc->m_Vertex[pDoc->m_FaceIdx[i + 2]];
				glLoadName(name);
				::glBegin(GL_POLYGON);
				glVertex3f(v0[0], v0[1], v0[2]);
				glVertex3f(v1[0], v1[1], v1[2]);
				glVertex3f(v2[0], v2[1], v2[2]);
				::glEnd();
				name++;
			}
		}
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		// get nembers of hit triangles
		hits = glRenderMode(GL_RENDER);

		if (hits > 0)
		{
			double mvmat[16], promat[16], wx, wy, wz;
			glGetDoublev(GL_MODELVIEW_MATRIX, mvmat);
			glGetDoublev(GL_PROJECTION_MATRIX, promat);
			int realy = viewport[3] - (GLint)point.y;
			::gluUnProject((GLdouble)point.x, (GLdouble)realy, 0.0, mvmat, promat, viewport, &wx, &wy, &wz);
			GVector3f p0(wx, wy, wz);
			::gluUnProject((GLdouble)point.x, (GLdouble)realy, 1.0, mvmat, promat, viewport, &wx, &wy, &wz);
			GVector3f p1(wx, wy, wz);

			GVector3f selectpt;
			GVector3f V(0.0f, 0.0f, -1000000.0f);

			for (int i = 0; i < hits; i++)
			{
				int idx = selectBuf[i * 4 + 3];
				int fidx0 = pDoc->m_FaceIdx[idx * 3];
				int fidx1 = pDoc->m_FaceIdx[idx * 3 + 1];
				int fidx2 = pDoc->m_FaceIdx[idx * 3 + 2];
				GVector3f a = pDoc->m_Vertex[fidx0];
				GVector3f b = pDoc->m_Vertex[fidx1];
				GVector3f c = pDoc->m_Vertex[fidx2];
				GPlane pln(a, b, c);
				GVector3f p = pln.GetProjectionPt(p0, p1 - p0);
				double u, v, w;
				if (::getBaricentricCoord(a, b, c, p, &u, &v, &w))
				{
					GVector3f pt2;
					pt2.X = mvmat[0] * p.X + mvmat[4] * p.Y + mvmat[8] * p.Z;
					pt2.Y = mvmat[1] * p.X + mvmat[5] * p.Y + mvmat[9] * p.Z;
					pt2.Z = mvmat[2] * p.X + mvmat[6] * p.Y + mvmat[10] * p.Z;
					if (pt2.Z > V.Z)
					{
						V = pt2;
						selectpt = p;
					}
				}
			}
			if (selectpt[0] == 0.0f && selectpt[1] == 0.0f && selectpt[2] == 0.0f)
			{
				AfxMessageBox(_T("측정점을 다시 선택해주세요"));
				wglMakeCurrent(NULL, NULL);
				return false;
			}
			*SltPt = selectpt;
		}
		else
		{
			AfxMessageBox(_T("선택된 점이 없습니다!"));
			return false;
		}
	}
	wglMakeCurrent(NULL, NULL);
	return true;
}

void CSizeKorea2020View::UpdatePickPt(CPoint point) {

}

void CSizeKorea2020View::InsertLandMark(GVector3f SltPt)
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	GLandMark InsPt(SltPt, true);

	if (pDoc->m_MarkIdx != -1)
	{
		int i;
		switch (pDoc->m_ScanPose)
		{
		case 1:
		case 2:
			for (i = 0; i < LANDMARK_NUM_POSE2; pDoc->m_LandMarkPose2[i].m_bIsPicked = false, i++);
			pDoc->m_LandMarkPose2[pDoc->m_MarkIdx] = InsPt;


			if (pDoc->m_MarkIdx == 3 || pDoc->m_MarkIdx == 4 && pDoc->m_bLandmarkFind) { //목앞점, 목옆점이 새로 찍히면 등쪽점, 방패연골아래점도 새로 만들어준다
				pDoc->MakeSagittalSection();

				pDoc->FindNeckBackSideFront();
				pDoc->Calc_NeckAngles();
				pDoc->FindNeckBackPoint();
				if (pDoc->m_MarkIdx == 3)
					pDoc->FindNeckFrontPoint_maxZV();
			}
			if (pDoc->m_MarkIdx == 19)
				pDoc->FindWaistF_B_LV();

			if (pDoc->m_MarkIdx == 7)
				pDoc->FindShouderMid();

			if (pDoc->m_MarkIdx == 9) {
				if (!pDoc->m_bLandmarkFind) pDoc->MakeSagittalSection();
				pDoc->FindArmPitH_Back();
			}
			break;
		}
		if (pDoc->m_MarkIdx == 9 && pDoc->m_bLandmarkFind)  pDoc->FindArmpitConcavePtsV();
	}
	else
	{
		CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
		switch (pDoc->m_ScanPose)
		{
		case 1:
			theSizer.UpdateLandmark(EgPos(SltPt[0], SltPt[1], SltPt[2]));
			pFrame->m_Pose1InfoDlg.m_Pose1MarkCoords.Format("(%.1f, %.1f, %.1f)", SltPt[0], SltPt[1], SltPt[2]);
			pFrame->m_Pose1InfoDlg.UpdateData(false);
			break;
		case 2:
			theSizer.UpdateLandmark(EgPos(SltPt[0], SltPt[1], SltPt[2]));

			// mj::Update pDoc Landmark(20. 11. 9)
			if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_TOPHEAD) {
				pDoc->m_LandMarkPose2[1] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_NECK_THYROID) {
				pDoc->m_LandMarkPose2[2] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_NECK_FRONT) {
				pDoc->m_LandMarkPose2[3] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_NECK_SIDE_R) {
				pDoc->m_LandMarkPose2[4] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_NECK_SIDE_L) {
				pDoc->m_LandMarkPose2[5] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_NECK_BACK) {
				pDoc->m_LandMarkPose2[6] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_SHOULDERSIDE_R) {
				pDoc->m_LandMarkPose2[7] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_SHOULDERSIDE_L) {
				pDoc->m_LandMarkPose2[8] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_ARMPITFOLD_FRONT_R) {
				pDoc->m_LandMarkPose2[44] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_ARMPITFOLD_BACK_R) {
				pDoc->m_LandMarkPose2[45] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_ARMPIT_R) {
				pDoc->m_LandMarkPose2[9] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_ARMPIT_L) {
				pDoc->m_LandMarkPose2[10] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_BP_R) {
				pDoc->m_LandMarkPose2[16] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_BP_L) {
				pDoc->m_LandMarkPose2[17] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_UNDERBUST) {
				pDoc->m_LandMarkPose2[18] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_WAIST) {
				pDoc->m_LandMarkPose2[19] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_WAIST_L) {
				pDoc->m_LandMarkPose2[20] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_WAIST_FRONT) {
				pDoc->m_LandMarkPose2[21] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_WAIST_BACK) {
				pDoc->m_LandMarkPose2[22] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_STOMACH_EXTRUDE) {
				pDoc->m_LandMarkPose2[23] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_NAVEL) {
				pDoc->m_LandMarkPose2[35] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_HIP_EXTRUDE) {
				pDoc->m_LandMarkPose2[24] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_CROTCH) {
				pDoc->m_LandMarkPose2[25] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_UNDERHIP) {
				pDoc->m_LandMarkPose2[26] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_KNEE_MID) {
				pDoc->m_LandMarkPose2[27] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_CALF_EXTRUDE) {
				pDoc->m_LandMarkPose2[28] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_WRIST) {
				pDoc->m_LandMarkPose2[32] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_WRIST_OUTSIDE) {
				pDoc->m_LandMarkPose2[33] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_HAND) {
				pDoc->m_LandMarkPose2[34] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_ELBOW) {
				pDoc->m_LandMarkPose2[31] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_SHOULDER_MID) {
				pDoc->m_LandMarkPose2[36] = InsPt;
			}
			else if (theSizer.m_SltLandmarkIdx == POSE2_LANDMARK_UNDERCALF) {
				pDoc->m_LandMarkPose2[29] = InsPt;
			}

			pFrame->m_Pose2InfoDlg.m_Pose2MarkCoords.Format("(%.1f, %.1f, %.1f)", SltPt[0], SltPt[1], SltPt[2]);
			pFrame->m_Pose2InfoDlg.UpdateData(false);
			break;

		case 3:
			theSizer.UpdateLandmark(EgPos(SltPt[0], SltPt[1], SltPt[2]));
			pFrame->m_Pose3InfoDlg.m_Pose3MarkCoords.Format("(%.1f, %.1f, %.1f)", SltPt[0], SltPt[1], SltPt[2]);
			pFrame->m_Pose3InfoDlg.UpdateData(false);
			break;

		case 4:
			theSizer.UpdateLandmark(EgPos(SltPt[0], SltPt[1], SltPt[2]));
			pFrame->m_Pose4InfoDlg.m_Pose4MarkCoords.Format("(%.1f, %.1f, %.1f)", SltPt[0], SltPt[1], SltPt[2]);
			pFrame->m_Pose4InfoDlg.UpdateData(false);
			break;

		case 5:
			theSizer.UpdateLandmark(EgPos(SltPt[0], SltPt[1], SltPt[2]));
			pFrame->m_Pose5InfoDlg.m_Pose5MarkCoords.Format("(%.1f, %.1f, %.1f)", SltPt[0], SltPt[1], SltPt[2]);
			pFrame->m_Pose5InfoDlg.UpdateData(false);
			break;
		}
	}
}

void CSizeKorea2020View::CalcAngle() {
	GVector3f v0, v1, l;

	//zx_angle : zx평면(수평면)에 투영시킨 선이 +z축 방향과 이루는 각 
	v0.setVector(m_RefAngPt[0][0], 0.0f, m_RefAngPt[0][2]);
	v1.setVector(m_RefAngPt[1][0], 0.0f, m_RefAngPt[1][2]);
	l = v1 - v0;
	REAL zx_angle = l.getAngle(GVector3f(0.0f, 0.0f, 1.0f)) * 180.0f / M_PI;


	//xy_angle : xy(관상면)평면에 투영시킨 선이 +X축 방향과 이루는 각 
	v0.setVector(m_RefAngPt[0][0], m_RefAngPt[0][1], 0.0f);
	v1.setVector(m_RefAngPt[1][0], m_RefAngPt[1][1], 0.0f);
	l = v1 - v0;
	REAL xy_angle = l.getAngle(GVector3f(1.0f, 0.0f, 0.0f)) * 180.0f / M_PI;

	//yz_angle : yz(시상면)평면에 투영시킨 선이 +Y축 방향과 이루는 각 
	v0.setVector(0.0f, m_RefAngPt[0][1], m_RefAngPt[0][2]);
	v1.setVector(0.0f, m_RefAngPt[1][1], m_RefAngPt[1][2]);
	l = v1 - v0;
	REAL yz_angle = l.getAngle(GVector3f(0.0f, 1.0f, 0.0f)) * 180.0f / M_PI;

	/*
	CAngleDlg dlg;
	dlg.m_AngleZX = zx_angle;
	dlg.m_AngleXY = xy_angle;
	dlg.m_AngleYZ = yz_angle;

	if (dlg.DoModal() != IDOK)
		return;
	*/
	Invalidate(false);
}

//////////////////////////////////////////
//					END					//
//////////////////////////////////////////
void CSizeKorea2020View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CSizeKorea2020Doc *pDoc = GetDocument();
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	switch (nChar)
	{
	case 'q':
	case 'Q':
		OnPickPoint();
		break;

	case 'f':
	case 'F':
		OnDirFront();
		break;

	case 'l':
	case 'L':
		OnDirLeft();
		break;

	case 'r':
	case 'R':
		OnDirRight();
		break;

	case 'b':
	case 'B':
		OnDirBack();
		break;

	case 't':
	case 'T':
		OnDirTop();
		break;

	case 'W':
	case 'w':
		ReCalcViewParameter();
		Invalidate();
		break;

	case 'm':
	case 'M':
		// pDoc->OnExportLandmark();
		break;

	case 'n':
	case 'N':
		// pDoc->OnImportLandmark();
		break;

	case 'z':
	case 'Z':
		OnRegion();
		break;

	case '1':
		pFrame->m_EditModelDlg.OnBnClickedRenderPolygon();
		pFrame->m_EditModelDlg.UpdateDlgData();
		break;

	case '2':
		pFrame->m_EditModelDlg.OnBnClickedRenderWire();
		pFrame->m_EditModelDlg.UpdateDlgData();
		break;

	case '3':
		pFrame->m_EditModelDlg.OnBnClickedRenderPoint();
		pFrame->m_EditModelDlg.UpdateDlgData();
		break;

	case '4':
		pFrame->m_EditModelDlg.OnBnClickedRenderTexture();
		pFrame->m_EditModelDlg.UpdateDlgData();
		break;

	case '5':
		//m_ManipulateMode = MNONE;
		m_RenderOnOff = !m_RenderOnOff;
		Invalidate(false);
		break;

	case '6':
		theSizer.m_bShowLandMark = !theSizer.m_bShowLandMark;
		Invalidate(false);
		break;

	case '7':
		//m_ManipulateMode = MNONE;
		m_bShowLandmarksHelp = !m_bShowLandmarksHelp;
		Invalidate(false);
		break;

	case '8':
		//m_ManipulateMode = MNONE;
		m_bShowFSilhouette = !m_bShowFSilhouette;
		Invalidate(false);
		break;

	case 27:
		//m_ManipulateMode = MNONE;
		m_ToolMode = TNONE;
		pDoc->m_MarkIdx = -1;
		Invalidate(false);
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSizeKorea2020View::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_StartPt = m_CurPt = m_StartPos = m_CurPos = point;
	if ((nFlags & MK_SHIFT) || (nFlags & MK_CONTROL))
		m_ManipulateReady = true;
	CView::OnLButtonDown(nFlags, point);
}

void CSizeKorea2020View::OnMouseMove(UINT nFlags, CPoint point)
{
	// 마우스 버튼이 눌리지 않은 상태의 움직임이라면 리턴한다.
	if (!(nFlags & MK_LBUTTON) && !(nFlags & MK_RBUTTON))
		return;

	m_CurPt = m_CurPos = point;
	float dx, dy;
	float ratio = m_ViewSize * 0.005;

	switch (m_ManipulateMode)
	{
	case ROT_FREE:
		m_TrackBall.Rotate(m_StartPt.x, m_StartPt.y, m_CurPt.x, m_CurPt.y, NULL, GTrackBall::RotateMode::ROT_FREE);
		m_StartPt = point;
		Invalidate(false);
		break;

	case ROT_X:
		m_TrackBall.Rotate(m_StartPt.x, m_StartPt.y, m_CurPt.x, m_CurPt.y, NULL, GTrackBall::RotateMode::ROT_X);
		m_StartPt = point;
		Invalidate(false);
		break;

	case ROT_Y:
		m_TrackBall.Rotate(m_StartPt.x, m_StartPt.y, m_CurPt.x, m_CurPt.y, NULL, GTrackBall::RotateMode::ROT_Y);
		m_StartPt = point;
		Invalidate(false);
		break;

	case ROT_Z:
		m_TrackBall.Rotate(m_StartPt.x, m_StartPt.y, m_CurPt.x, m_CurPt.y, NULL, GTrackBall::RotateMode::ROT_Z);
		m_StartPt = point;
		Invalidate(false);
		break;

	case TRANS:
		dx = (float)(m_CurPt.x - m_StartPt.x) * ratio;
		dy = -(float)(m_CurPt.y - m_StartPt.y) * ratio;
		m_OffSet[0] += (m_RotMat[0] * dx + m_RotMat[1] * dy);
		m_OffSet[1] += (m_RotMat[4] * dx + m_RotMat[5] * dy);
		m_OffSet[2] += (m_RotMat[8] * dx + m_RotMat[9] * dy);
		m_StartPt = point;
		Invalidate(false);
		break;

	case SCALE:
		m_TrackBallCenter[2] += (float)(point.y - m_StartPt.y) * ratio;
		if (m_TrackBallCenter[2] < -m_Far)
			m_TrackBallCenter[2] = -m_Far;
		if (m_TrackBallCenter[2] > -m_Near)
			m_TrackBallCenter[2] = -m_Near;
		m_StartPt = point;
		Invalidate(false);
		break;

	case ZOOM:
		Invalidate(false);
		break;
	}

	switch (nFlags)
	{
	case MK_SHIFT + MK_LBUTTON:
		m_ManipulateMode = MNONE;
		if (m_RefPlane.IsActive())
		{
			m_RefPlane.Scale(point.y - m_StartPt.y);
		}
		else
		{
			m_TrackBallCenter[2] += (float)(point.y - m_StartPt.y)*ratio;
			if (m_TrackBallCenter[2] < -m_Far)
				m_TrackBallCenter[2] = -m_Far;
			if (m_TrackBallCenter[2] > -m_Near)
				m_TrackBallCenter[2] = -m_Near;
		}
		m_StartPt = point;
		Invalidate(false);
		break;

	case MK_SHIFT + MK_CONTROL + MK_LBUTTON:
		m_ManipulateMode = MNONE;
		dx = (float)(point.x - m_StartPt.x)*ratio;
		dy = -(float)(point.y - m_StartPt.y)*ratio;
		if (m_RefPlane.IsActive())
		{
			m_RefPlane.Translate(m_RotMat[0] * dx + m_RotMat[1] * dy,
				m_RotMat[4] * dx + m_RotMat[5] * dy,
				m_RotMat[8] * dx + m_RotMat[9] * dy);
		}
		else
		{
			m_OffSet[0] += (m_RotMat[0] * dx + m_RotMat[1] * dy);
			m_OffSet[1] += (m_RotMat[4] * dx + m_RotMat[5] * dy);
			m_OffSet[2] += (m_RotMat[8] * dx + m_RotMat[9] * dy);
		}
		m_StartPt = point;
		Invalidate(false);
		break;

	case MK_CONTROL + MK_LBUTTON:
		m_ManipulateMode = MNONE;
		if (m_RefPlane.IsActive())
			m_RefPlane.Rotate(m_StartPt.x, m_StartPt.y, m_CurPt.x, m_CurPt.y, NULL, GTrackBall::RotateMode::ROT_FREE);
		else
			m_TrackBall.Rotate(m_StartPt.x, m_StartPt.y, m_CurPt.x, m_CurPt.y, NULL, GTrackBall::RotateMode::ROT_FREE);
		m_StartPt = point;
		Invalidate(false);
		break;
	}

	if (nFlags & MK_RBUTTON)
	{
		m_ManipulateMode = MNONE;
		dx = (float)(point.x - m_StartPt.x)*ratio;
		dy = -(float)(point.y - m_StartPt.y)*ratio;
		m_OffSet[0] += (m_RotMat[0] * dx + m_RotMat[1] * dy);
		m_OffSet[1] += (m_RotMat[4] * dx + m_RotMat[5] * dy);
		m_OffSet[2] += (m_RotMat[8] * dx + m_RotMat[9] * dy);
		m_StartPt = point;
		Invalidate(false);
	}
	if (!m_ManipulateReady && m_ToolMode != TNONE)
		Invalidate(false);

	CView::OnMouseMove(nFlags, point);
}

void CSizeKorea2020View::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_EndPos = point;
	SendMessage(WM_SETCURSOR, 0, 0);

	switch (m_ManipulateMode)
	{
	case ZOOM:
		RegionZooming();
		break;
	}

	if (!m_ManipulateReady)
	{
		GVector3f SltPt;
		switch (m_ToolMode)
		{
		case TNONE:
			UpdatePickPt(point);
			ShowCoordInfo();
			break;

		case PICK:
			if (!FindPickingPt(point, &SltPt))
				break;
			InsertLandMark(SltPt);
			ShowCoordInfo();
			m_ToolMode = TNONE;
			break;

		case REF_LINE:
			TransByRefLine();
			m_ToolMode = TNONE;
			break;

		case REF_VEC:
			TransByRefVec();
			m_ToolMode = TNONE;
			break;

		case MEASURE_ANGLE:
			if (!FindPickingPt(point, &SltPt))
				break;
			switch (m_RefAngPtIdx)
			{
			case 0:
				m_RefAngPt[0] = SltPt;
				m_RefAngPt[1] = SltPt;
				break;
			case 1:
				m_RefAngPt[1] = SltPt;
				break;
			}
			m_RefAngPtIdx++;
			if (m_RefAngPtIdx == 2)
			{
				CalcAngle();
				m_RefAngPtIdx = 0;
				m_ToolMode = TNONE;
			}
			break;
		}
	}
	m_ManipulateReady = false;
	Invalidate(false);

	CView::OnLButtonUp(nFlags, point);
}

BOOL CSizeKorea2020View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta > 0)
		m_TrackBallCenter[2] += m_ViewSize;
	else
		m_TrackBallCenter[2] -= m_ViewSize;
	Invalidate(true);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CSizeKorea2020View::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_StartPt = m_CurPt = m_StartPos = m_CurPos = point;
	CView::OnRButtonDown(nFlags, point);
}
void CSizeKorea2020View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	// ClientToScreen(&point);
	// OnContextMenu(this, point);
	m_EndPos = point;
	m_ManipulateReady = false;
	Invalidate(false);
}

BOOL CSizeKorea2020View::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	switch (m_ToolMode)
	{
	case PICK:
	case MEASURE_ANGLE:
		::SetCursor(AfxGetApp()->LoadCursor(IDC_PICK_CURSOR));
		return true;
	}

	//if (!m_bIsDrag)
	//	return CView::OnSetCursor(pWnd, nHitTest, message);

	//switch (m_ManipulateMode)
	//{
	//case ROT_FREE:
	//case ROT_X:
	//case ROT_Y:
	//case ROT_Z:
	//	::SetCursor(AfxGetApp()->LoadCursor(IDC_ROT_CURSOR));
	//	break;
	//case TRANS:
	//	::SetCursor(AfxGetApp()->LoadCursor(IDC_TRANS_CURSOR));
	//	break;
	//case SCALE:
	//	::SetCursor(AfxGetApp()->LoadCursor(IDC_SCALE_CURSOR));
	//	break;
	//case ZOOM:
	//	::SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOM_CURSOR));
	//	break;
	//}
	//return true;

	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CSizeKorea2020View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_nWidth = cx;
	m_nHeight = cy;
	m_ViewRatio = (float)m_nWidth / (float)m_nHeight;
	m_TrackBall.InitBall(0, 0, cx, cy);
	m_RefPlane.InitBall(0, 0, cx, cy);
	SetupViewport(cx, cy);
}


void CSizeKorea2020View::OnManualPoint()
{
	// TODO: Add your command handler code here
	m_bShowMarkDlg = !m_bShowMarkDlg;

	/*
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (m_bShowMarkDlg)
		pFrame->ShowControlBar(&(pFrame->m_wndPose2MarkDlg), true, true);
	else
		pFrame->ShowControlBar(&(pFrame->m_wndPose2MarkDlg), false, false);
	*/

	//Invalidate();
}


void CSizeKorea2020View::OnPickPoint()
{
	// TODO: Add your command handler code here
	m_ManipulateMode = MNONE;
	m_ToolMode = PICK;
}


void CSizeKorea2020View::OnRegion()
{
	// TODO: Add your command handler code here
	m_StartPos = CPoint(0, 0);
	m_EndPos = CPoint(0, 0);
	m_CurPos = CPoint(0, 0);
	m_ManipulateMode = ZOOM;
}


void CSizeKorea2020View::OnRotate()
{
	// TODO: Add your command handler code here
	m_ManipulateMode = ROT_FREE;
}


void CSizeKorea2020View::OnRotateX()
{
	// TODO: Add your command handler code here
	m_ManipulateMode = ROT_X;
}


void CSizeKorea2020View::OnRotateY()
{
	// TODO: Add your command handler code here
	m_ManipulateMode = ROT_Y;
}


void CSizeKorea2020View::OnRotateZ()
{
	// TODO: Add your command handler code here
	m_ManipulateMode = ROT_Z;
}


void CSizeKorea2020View::OnScale()
{
	// TODO: Add your command handler code here
	m_ManipulateMode = SCALE;
}


void CSizeKorea2020View::OnTrans()
{
	// TODO: Add your command handler code here
	m_ManipulateMode = TRANS;
}


void CSizeKorea2020View::OnDirFront()
{
	// TODO: Add your command handler code here
	m_ManipulateMode = MNONE;
	m_TrackBall.quat.SetQuat(0.0f, 0.0f, 0.0f, 1.0f);
	Invalidate(false);
}


void CSizeKorea2020View::OnDirBack()
{
	// TODO: Add your command handler code here
	m_ManipulateMode = MNONE;
	m_TrackBall.quat.SetQuat(0.0f, 1.0f, 0.0f, 0.0f);
	Invalidate(false);
}


void CSizeKorea2020View::OnDirLeft()
{
	// TODO: Add your command handler code here
	m_ManipulateMode = MNONE;
	m_TrackBall.quat.SetQuat(0.0f, -sqrt(2.0f) / 2.0f, 0.0f, sqrt(2.0f) / 2.0f);
	Invalidate(false);
}


void CSizeKorea2020View::OnDirRight()
{
	// TODO: Add your command handler code here
	m_ManipulateMode = MNONE;
	m_TrackBall.quat.SetQuat(0.0f, sqrt(2.0f) / 2.0f, 0.0f, sqrt(2.0f) / 2.0f);
	Invalidate(false);
}


void CSizeKorea2020View::OnDirTop()
{
	// TODO: Add your command handler code here
	m_ManipulateMode = MNONE;
	m_TrackBall.quat.SetQuat(sqrt(2.0f) / 2.0f, 0.0f, 0.0f, sqrt(2.0f) / 2.0f);
	Invalidate(false);
}


void CSizeKorea2020View::OnDirBottom()
{
	// TODO: Add your command handler code here
	m_ManipulateMode = MNONE;
	m_TrackBall.quat.SetQuat(-sqrt(2.0f) / 2.0f, 0.0f, 0.0f, sqrt(2.0f) / 2.0f);
	Invalidate(false);
}


void CSizeKorea2020View::OnViewShowsilhouette()
{
	// TODO: Add your command handler code here
	m_bShowFSilhouette = true;
}


//void CSizeKorea2020View::OnPose2Mark1()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 5;	m_nPartType = FRONT; // LEFT_TOP;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 1;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark2()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	// 방패연골아래점
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 2;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark3()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	// 목앞점
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 3;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark4()
//{
//	// TODO: Add your control notification handler code here
//		// TODO: Add your command handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//목옆점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = RIGHT;   //FRONT;	
//	SetProperView();
//
//	pDoc->m_MarkIdx = 4;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark5()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//목옆점(L)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = LEFT;   //FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 5;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark6()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	// 목뒤점 
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = BACK;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 6;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark7()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	// 어깨가쪽점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = RIGHT_TOP;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 7;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark8()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//어깨가쪽점(L)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = LEFT_TOP;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 8;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark9()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//겨드랑점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 9;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark10()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//겨드랑점(L)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 10;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark11()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	// 진동깊이점
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = BACK; //FRONT_RIGHT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 11;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark12()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	// 겨드랑앞벽점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = FRONT;  //_LEFT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 12;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark13()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	// 겨드랑앞벽점(L)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = FRONT;   //BACK;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 13;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark14()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//겨드랑뒤벽점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = BACK; // FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 14;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark15()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//겨드랑뒤벽점(L)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = BACK;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 15;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark16()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	// 젖꼭지점(R)    
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = FRONT; //BACK;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 16;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark17()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//젖꼭지점(L)
//	if (!pDoc->m_bOpened)
//		return;
//
//	m_nPartOrder = 4;	m_nPartType = FRONT;  //BACK;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 17;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark18()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//겨드랑앞접힘점(L)   젖가슴아래점(여)
//	if (!pDoc->m_bOpened)
//		return;
//
//	m_nPartOrder = 3;	m_nPartType = FRONT_RIGHT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 18;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark19()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//허리옆점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 3;	m_nPartType = FRONT_RIGHT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 19;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark20()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//허리옆점(L)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 3;	m_nPartType = FRONT_LEFT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 20;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark21()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//허리앞점
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 3;	m_nPartType = FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 21;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark22()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//허리뒤점 
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 3;	m_nPartType = BACK;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 22;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark23()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//배돌출점
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 3;	m_nPartType = FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 23;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark24()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//엉덩이돌출점
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 3;	m_nPartType = RIGHT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 24;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark25()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//샅점
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 3;	m_nPartType = FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 25;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark26()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//볼기고랑점
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 2;	m_nPartType = BACK;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 26;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark27()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//무릎뼈중간(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 1;	m_nPartType = FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 27;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark28()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//장딴지돌출점
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 1;	m_nPartType = RIGHT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 28;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark29()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//종아리아래점
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 1;	m_nPartType = RIGHT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 29;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark30()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	// 바깥복사점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 0;	m_nPartType = RIGHT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 30;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark31()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	// 노뼈위점
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 3;	m_nPartType = HAND;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 31;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark32()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//손목안쪽점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 3;	m_nPartType = RIGHT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 32;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark33()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//손목가쪽점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 3;	m_nPartType = RIGHT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 33;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark34()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//손끝점 R
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 3;	m_nPartType = HAND;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 34;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark35()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//배꼽점
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 3;	m_nPartType = FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 35;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark36()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//어깨가운데점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 5;	m_nPartType = FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 36;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark37()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//등돌출점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = RIGHT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 37;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark38()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//발끝점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 0;	m_nPartType = FRONT_TOP;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 38;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark39()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//발뒤꿈치점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 0;	m_nPartType = BACK_TOP;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 39;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark40()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//발안쪽점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 0;	m_nPartType = FRONT_TOP;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 40;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark41()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//발바깥점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 0;	m_nPartType = FRONT_TOP;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 41;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark42()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//팔꿈치가운데점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 3;	m_nPartType = HAND;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 42;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark43()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//안쪽복사점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 1;	m_nPartType = FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 43;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


//void CSizeKorea2020View::OnPose2Mark44()
//{
//	// TODO: Add your control notification handler code here
//	CSizeKorea2020Doc *pDoc = GetDocument();
//	//겨드랑앞접힘점(R)
//	if (!pDoc->m_bOpened)
//		return;
//	m_nPartOrder = 4;	m_nPartType = FRONT;
//	SetProperView();
//
//	pDoc->m_MarkIdx = 44;
//	/*
//	CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
//	CDialogBar *pCtlBar = &(pFrame->m_wndPose2MarkDlg);
//	CStatic *pStatic = (CStatic*)(pCtlBar->GetDlgItem(IDC_POSE2_COORD));
//	CString str;
//	GVector3f v = GetDocument()->m_LandMarkPose2[GetDocument()->m_MarkIdx];
//	str.Format("[ %4.2f, %4.2f, %4.2f ]", v[0], v[1], v[2]);
//	pStatic->SetWindowText(str);
//	*/
//	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
//	Invalidate();
//}


void CSizeKorea2020View::OnPose1MarkTopHead() {
	// TODO: Add your control notification handler code here
	CSizeKorea2020Doc *pDoc = GetDocument();
	//겨드랑앞접힘점(R)
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_TOPHEAD;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}

void CSizeKorea2020View::OnPose1MarkEye() {
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_EYE;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}

void CSizeKorea2020View::OnPose1MarkJaw() {
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_JAW;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkNeckBack()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_NECK_BACK;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkNeckSide()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_NECK_SIDE;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkNeckFront()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_NECK_FRONT;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkShoulderR()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_SHOULDER_R;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkShoulderL()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_SHOULDER_L;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkShoulderSideR()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_SHOULDERSIDE_R;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkShoulderSideL()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_SHOULDERSIDE_L;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkArmpit()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_ARMPIT;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkBpR()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_BP_R;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkBpL()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_BP_L;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkUnderBust()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_UNDERBUST;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkWaist()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_WAIST;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkWaistFront()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_WAIST_FRONT;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkWaistBack()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_WAIST_BACK;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkNavel()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_NAVEL;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkNavelBack()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_NAVEL_BACK;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkStomachExtrude()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_STOMACH_EXTRUDE;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkHipExtrude()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_HIP_EXTRUDE;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkCrotch()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_CROTCH;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkUnderHip()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_UNDERHIP;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkMidKnee()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_KNEE_MID;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkKnee()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_KNEE_SIDE;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkCalfExtrude()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_CALF_EXTRUDE;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkAnkle()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_ANKLE;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkHand()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_HAND;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkArmpitWallBackR()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_ARMPITWALL_BACK_R;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkArmpitWallBackL()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_ARMPITWALL_BACK_L;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkArmpitWallFrontR()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_ARMPITWALL_FRONT_R;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkArmpitWallFrontL()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_ARMPITWALL_FRONT_L;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkArmpitFoldFrontR()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_ARMPITFOLD_FRONT_R;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkArmpitFoldFrontL()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_ARMPITFOLD_FRONT_L;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkArmpitFoldBackR()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_ARMPITFOLD_BACK_R;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkArmpitFoldBackL()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_ARMPITFOLD_BACK_L;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnPose1MarkPopliteus()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_POPLITEUS;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}

void CSizeKorea2020View::OnPose1MarkElbow() {
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_ELBOW;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}

void CSizeKorea2020View::OnPose1MarkWrist() {
	CSizeKorea2020Doc *pDoc = GetDocument();
	if (!pDoc->m_bOpened)
		return;

	m_nPartOrder = 5;	m_nPartType = FRONT;
	SetProperView();

	pDoc->m_MarkIdx = POSE1_LANDMARK_WRIST;

	SetActiveLandMark(pDoc->m_ScanPose, pDoc->m_MarkIdx);
	Invalidate();
}


void CSizeKorea2020View::OnViewShowcoronalsilhouette()
{
	// TODO: Add your command handler code here
	m_bShowRSilhouette = true;
}



void CSizeKorea2020View::OnExportCamera()
{
	_cprintf("Rot = (%lf, %lf, %lf, %lf)\n", m_TrackBall.quat.X, m_TrackBall.quat.Y, m_TrackBall.quat.Z, m_TrackBall.quat.W);
	_cprintf("Offset = (%lf, %lf, %lf)\n", m_OffSet[0], m_OffSet[1], m_OffSet[2]);
	_cprintf("Zoom = %lf\n", m_TrackBallCenter[2]);
}
