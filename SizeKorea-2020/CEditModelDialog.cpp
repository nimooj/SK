// CEditModelDialog.cpp: 구현 파일
//

#include "pch.h"
#include "SizeKorea-2020.h"
#include "CEditModelDialog.h"
#include "MainFrm.h"

extern EgSizer theSizer;

// CEditModelDialog

IMPLEMENT_DYNAMIC(CEditModelDialog, CPaneDialog)

CEditModelDialog::CEditModelDialog()
{
	m_FileName = "";
	m_ScanPose = "없음";
	m_NumVert = 0;
	m_NumFace = 0;
	m_Width = "0.0";
	m_Height = "0.0";
	m_Depth = "0.0";
	m_TransX = 0.0;
	m_TransY = 0.0;
	m_TransZ = 0.0;
	m_RotX = 0.0;
	m_RotY = 0.0;
	m_RotZ = 0.0;
	m_Scale = 1.0;
}

CEditModelDialog::~CEditModelDialog()
{
}

void CEditModelDialog::DoDataExchange(CDataExchange* pDX)
{
	CPaneDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILE_NAME, m_FileName);
	DDX_Text(pDX, IDC_SCAN_POSE, m_ScanPose);
	DDX_Text(pDX, IDC_NUM_VERT, m_NumVert);
	DDX_Text(pDX, IDC_NUM_FACE, m_NumFace);
	DDX_Text(pDX, IDC_WIDTH, m_Width);
	DDX_Text(pDX, IDC_HEIGHT, m_Height);
	DDX_Text(pDX, IDC_DEPTH, m_Depth);
	DDX_Text(pDX, IDC_TRANS_X, m_TransX);
	DDX_Text(pDX, IDC_TRANS_Y, m_TransY);
	DDX_Text(pDX, IDC_TRANS_Z, m_TransZ);
	DDX_Text(pDX, IDC_ROT_X, m_RotX);
	DDX_Text(pDX, IDC_ROT_Y, m_RotY);
	DDX_Text(pDX, IDC_ROT_Z, m_RotZ);
	DDX_Text(pDX, IDC_SCALE, m_Scale);
}

BEGIN_MESSAGE_MAP(CEditModelDialog, CPaneDialog)
	ON_BN_CLICKED(IDC_APPLY_EDIT, &CEditModelDialog::OnBnClickedApplyEdit)
	ON_BN_CLICKED(IDC_RENDER_POLYGON, &CEditModelDialog::OnBnClickedRenderPolygon)
	ON_BN_CLICKED(IDC_RENDER_WIRE, &CEditModelDialog::OnBnClickedRenderWire)
	ON_BN_CLICKED(IDC_RENDER_POINT, &CEditModelDialog::OnBnClickedRenderPoint)
	ON_BN_CLICKED(IDC_RENDER_TEXTURE, &CEditModelDialog::OnBnClickedRenderTexture)
	ON_BN_CLICKED(IDC_INIT_ORIGIN, &CEditModelDialog::OnBnClickedInitOrigin)
	ON_BN_CLICKED(IDC_EDIT_AUTO, &CEditModelDialog::OnBnClickedEditAuto)
END_MESSAGE_MAP()


CSizeKorea2020View *CEditModelDialog::GetView()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020View *)pFrame->GetActiveView();
}

CSizeKorea2020Doc *CEditModelDialog::GetDocument()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020Doc *)pFrame->GetActiveDocument();
}

void CEditModelDialog::UpdateDlgData()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	m_FileName = pDoc->m_FileName;
	switch (pDoc->m_ScanPose)
	{
	case 1:
		m_ScanPose = "자세 1";
		break;
	case 2:
		m_ScanPose = "자세 2";
		break;
	case 3:
		m_ScanPose = "자세 3";
		break;
	case 4:
		m_ScanPose = "자세 4";
		break;
	case 5:
		m_ScanPose = "자세 3(좌우반전)";
		break;
	}
	m_NumVert = pDoc->m_vNum;
	m_NumFace = pDoc->m_iNum / 3;

	CString str;
	str.Format("%.1f", pDoc->m_Width);
	m_Width = str;
	str.Format("%.1f", pDoc->m_Height);
	m_Height = str;
	str.Format("%.1f", pDoc->m_Depth);
	m_Depth = str;

	m_TransX = 0.0;
	m_TransY = 0.0;
	m_TransZ = 0.0;
	m_RotX = 0.0;
	m_RotY = 0.0;
	m_RotZ = 0.0;
	m_Scale = 1.0;

	EgMesh *pMesh = pDoc->m_ScanMesh;
	if (pMesh->m_RenderType & RENDER_POLYGON)
		((CButton *)GetDlgItem(IDC_RENDER_POLYGON))->SetCheck(true);
	else
		((CButton *)GetDlgItem(IDC_RENDER_POLYGON))->SetCheck(false);

	if (pMesh->m_RenderType & RENDER_WIRE)
		((CButton *)GetDlgItem(IDC_RENDER_WIRE))->SetCheck(true);
	else
		((CButton *)GetDlgItem(IDC_RENDER_WIRE))->SetCheck(false);

	if (pMesh->m_RenderType & RENDER_POINT)
		((CButton *)GetDlgItem(IDC_RENDER_POINT))->SetCheck(true);
	else
		((CButton *)GetDlgItem(IDC_RENDER_POINT))->SetCheck(false);

	if (pMesh->m_ShadeType == SHADE_BY_TEXTURE)
		((CButton *)GetDlgItem(IDC_RENDER_TEXTURE))->SetCheck(true);
	else
		((CButton *)GetDlgItem(IDC_RENDER_TEXTURE))->SetCheck(false);

	UpdateData(FALSE);
}

// CEditModelDialog 메시지 처리기
void CEditModelDialog::OnBnClickedApplyEdit()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	UpdateData(TRUE);

	EgMesh *pMesh = pDoc->m_ScanMesh;

	// 1. 이동 변환 적용
	EgVec3 offset(m_TransX, m_TransY, m_TransZ);
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_Pos += offset;

	// 2. 회전 변환 적용
	EgQuat qt;
	qt.SetFromEulerAngle(m_RotX, m_RotY, m_RotZ);
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_Pos = qt * v->m_Pos;

	// 3. 크기 변환 적용
	for (EgVertex *v : pMesh->m_pVerts)
	{
		v->m_Pos[0] *= m_Scale;
		v->m_Pos[1] *= m_Scale;
		v->m_Pos[2] *= m_Scale;
	}

	// 메쉬를 업데이트 한다.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal(SMOOTH);
	pMesh->UpdateVertexBuf();
	pMesh->UpdateNormalBuf();
	pMesh->UpdateTexelBuf();

	// 변환된 메쉬를 Document 클래스의 원시데이터로 복사한다.
	pDoc->EgMesh2RawData();

	// 대화상자의 데이터를 초기화한다.
	m_NumVert = pDoc->m_vNum;
	m_NumFace = pDoc->m_iNum / 3;

	CString str;
	str.Format("%.1f", pDoc->m_Width);
	m_Width = str;
	str.Format("%.1f", pDoc->m_Height);
	m_Height = str;
	str.Format("%.1f", pDoc->m_Depth);
	m_Depth = str;

	m_TransX = 0.0;
	m_TransY = 0.0;
	m_TransZ = 0.0;
	m_RotX = 0.0;
	m_RotY = 0.0;
	m_RotZ = 0.0;
	m_Scale = 1.0;
	UpdateData(FALSE);

	// 뷰를 갱신한다.
	GetView()->Invalidate();
}


void CEditModelDialog::OnBnClickedRenderPolygon()
{
	EgMesh *pMesh = GetDocument()->m_ScanMesh;
	if (pMesh->m_RenderType & RENDER_POLYGON)
		pMesh->m_RenderType &= (~RENDER_POLYGON);
	else
		pMesh->m_RenderType |= RENDER_POLYGON;
	GetView()->Invalidate(false);
}


void CEditModelDialog::OnBnClickedRenderWire()
{
	EgMesh *pMesh = GetDocument()->m_ScanMesh;
	if (pMesh->m_RenderType & RENDER_WIRE)
		pMesh->m_RenderType &= (~RENDER_WIRE);
	else
		pMesh->m_RenderType |= RENDER_WIRE;
	GetView()->Invalidate(false);
}


void CEditModelDialog::OnBnClickedRenderPoint()
{
	EgMesh *pMesh = GetDocument()->m_ScanMesh;
	if (pMesh->m_RenderType & RENDER_POINT)
		pMesh->m_RenderType &= (~RENDER_POINT);
	else
		pMesh->m_RenderType |= RENDER_POINT;
	GetView()->Invalidate(false);
}

void CEditModelDialog::OnBnClickedRenderTexture()
{
	EgMesh *pMesh = GetDocument()->m_ScanMesh;
	if (pMesh->m_pTexels.empty())
		return;
	if (pMesh->m_ShadeType == SHADE_BY_TEXTURE)
		pMesh->m_ShadeType = SHADE_BY_LIGHT;
	else
	{
		pMesh->m_ShadeType = SHADE_BY_TEXTURE;
	}
	GetView()->Invalidate(false);
}

void CEditModelDialog::OnBnClickedInitOrigin()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	EgMesh *pMesh = pDoc->m_ScanMesh;

	// 모델 정점의 최소 좌표를 (0, 0, 0)으로 변환한다.
	pDoc->Arrange3DDataXYZ_to0();

	// 메쉬를 업데이트 한다.
	pMesh->UpdateBndBox();
	pMesh->UpdateNormal(SMOOTH);
	pMesh->UpdateVertexBuf();
	pMesh->UpdateNormalBuf();
	pMesh->UpdateTexelBuf();

	// 변환된 메쉬를 Document 클래스의 원시데이터로 복사한다.
	pDoc->EgMesh2RawData();

	// 대화상자의 데이터를 초기화한다.
	m_NumVert = pDoc->m_vNum;
	m_NumFace = pDoc->m_iNum / 3;

	CString str;
	str.Format("%.1f", pDoc->m_Width);
	m_Width = str;
	str.Format("%.1f", pDoc->m_Height);
	m_Height = str;
	str.Format("%.1f", pDoc->m_Depth);
	m_Depth = str;

	m_TransX = 0.0;
	m_TransY = 0.0;
	m_TransZ = 0.0;
	m_RotX = 0.0;
	m_RotY = 0.0;
	m_RotZ = 0.0;
	m_Scale = 1.0;
	UpdateData(FALSE);

	// 뷰를 갱신한다.
	GetView()->Invalidate(false);
}

void CEditModelDialog::OnBnClickedEditAuto()
{
	CSizeKorea2020Doc *pDoc = GetDocument();
	EgMesh *pMesh = pDoc->m_ScanMesh;

	// 배치 편집
	m_TransX = 0.0;
	m_TransY = 0.0;
	m_TransZ = 0.0;
	m_RotX = 0.0;
	m_RotY = 180.0;
	m_RotZ = 90.0;
	m_Scale = 1000.0;

	// 1. 이동 변환 적용
	EgVec3 offset(m_TransX, m_TransY, m_TransZ);
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_Pos += offset;

	// 2. 회전 변환 적용
	EgQuat qt;
	qt.SetFromEulerAngle(m_RotX, m_RotY, m_RotZ);
	for (EgVertex *v : pMesh->m_pVerts)
		v->m_Pos = qt * v->m_Pos;

	// 3. 크기 변환 적용
	for (EgVertex *v : pMesh->m_pVerts)
	{
		v->m_Pos[0] *= m_Scale;
		v->m_Pos[1] *= m_Scale;
		v->m_Pos[2] *= m_Scale;
	}

	// 메쉬를 업데이트 한다.
	pMesh->UpdateBndBox();

	// 모델 정점의 최소 좌표를 (0, 0, 0)으로 변환한다.
	pDoc->Arrange3DDataXYZ_to0();
	pMesh->UpdateNormal(SMOOTH);
	pMesh->UpdateVertexBuf();
	pMesh->UpdateNormalBuf();
	pMesh->UpdateTexelBuf();

	// 변환된 메쉬를 Document 클래스의 원시데이터로 복사한다.
	pDoc->EgMesh2RawData();

	// 대화상자의 데이터를 초기화한다.
	m_NumVert = pDoc->m_vNum;
	m_NumFace = pDoc->m_iNum / 3;

	CString str;
	str.Format("%.1f", pDoc->m_Width);
	m_Width = str;
	str.Format("%.1f", pDoc->m_Height);
	m_Height = str;
	str.Format("%.1f", pDoc->m_Depth);
	m_Depth = str;

	m_TransX = 0.0;
	m_TransY = 0.0;
	m_TransZ = 0.0;
	m_RotX = 0.0;
	m_RotY = 0.0;
	m_RotZ = 0.0;
	m_Scale = 1.0;
	UpdateData(FALSE);

	// 뷰를 갱신한다.
	GetView()->Invalidate();
	pDoc->InitPoseInfoDialog();
}
