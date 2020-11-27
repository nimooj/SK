#include "pch.h"
#include "..\usr\include\GNurbs\GNurbs.h"

/**************************************************************************
 * implementation of constructors and descructor of GNurbsSgc class
 **************************************************************************/
bool GNurbsSgc::_alloc()
{
	register int i, j;
	REAL* knot;
	short kidx;

	/**********************************************************************
	 * make translational, rotation, scaling (if exist) compatible.
	 **********************************************************************/
	if (R && (R->GetDegree() > 0))
	{
		T->MakeKnotCompatible(R);
		if (S && (S->GetDegree() > 0))
		{
			S->MakeKnotCompatible(R);
			T->MakeKnotCompatible(R);
		}
		knot = mergeKnotDistinct(T->GetKnot(), T->GetKnotIdx(),
			R->GetKnot(), R->GetKnotIdx(), &kidx);
	}
	else if (S && (S->GetDegree() > 0))
	{
		T->MakeKnotCompatible(S);
		knot = mergeKnotDistinct(T->GetKnot(), T->GetKnotIdx(),
			S->GetKnot(), S->GetKnotIdx(), &kidx);
	}
	else
	{
		knot = mergeKnotDistinct(T->GetKnot(), T->GetKnotIdx(),
			T->GetKnot(), T->GetKnotIdx(), &kidx);
	}

	/***********************************************************************
	 * degree setting.
	 * p : U-direction degree.
	 * q : v-direction degree.
	 ***********************************************************************/
	this->p = T->GetDegree();
	if (R)
		this->p += 2 * R->GetDegree();
	if (S)
		this->p += S->GetDegree();
	this->q = X->GetDegree();

	/**********************************************************************
	 * get control point net size for each direction.
	 **********************************************************************/
	short numknot = p * (kidx + 1) + 1; // numknot+1 is # of motion knot vector;
	this->m = numknot - p - 1;
	this->n = X->GetCtlPtIdx();
	this->P = new REAL4 *[m + 1];
	for (i = 0; i < m + 1; P[i] = new REAL4[n + 1], REAL4_ARR_ZERO(P[i], n + 1), i++);

	/**********************************************************************
	 * get knot vectors for each direction.
	 **********************************************************************/
	this->U = new REAL[numknot + 1];
	this->V = new REAL[X->GetKnotIdx() + 1];
	for (i = 0; i < kidx + 1; i++)
		for (j = 1; j < p + 1; j++)
			U[i*p + j] = knot[i];
	U[0] = knot[0]; U[numknot] = knot[kidx];
	REAL_ARR_COPY(V, (X->GetKnot()), (X->GetKnotIdx() + 1));
	/**********************************************************************
	 * memory free.
	 **********************************************************************/
	delete[] knot;
	return true;
}
bool GNurbsSgc::CalcCtlPtNet()
{
	GNurbsMot mot(T, R, S);
	MATRIX *ctlmat = mot.GetCtlMat();
	for (int i = 0; i < m + 1; i++)
	{
		for (int j = 0; j < n + 1; j++)
		{
			REAL4 x;
			X->GetCtlPt(j, x);
			x[0] *= x[3]; x[1] *= x[3]; x[2] *= x[3];
			P[i][j][0] = ctlmat[i][0][0] * x[0] + ctlmat[i][0][1] * x[1] + ctlmat[i][0][2] * x[2] +
				ctlmat[i][0][3] * x[3];
			P[i][j][1] = ctlmat[i][1][0] * x[0] + ctlmat[i][1][1] * x[1] + ctlmat[i][1][2] * x[2] +
				ctlmat[i][1][3] * x[3];
			P[i][j][2] = ctlmat[i][2][0] * x[0] + ctlmat[i][2][1] * x[1] + ctlmat[i][2][2] * x[2] +
				ctlmat[i][2][3] * x[3];
			P[i][j][3] = ctlmat[i][3][0] * x[0] + ctlmat[i][3][1] * x[1] + ctlmat[i][3][2] * x[2] +
				ctlmat[i][3][3] * x[3];
			P[i][j][0] /= P[i][j][3];
			P[i][j][1] /= P[i][j][3];
			P[i][j][2] /= P[i][j][3];
		}
	}
	return true;
}
bool GNurbsSgc::_free()
{
	if (X)
	{
		delete X; X = NULL;
	}
	if (T)
	{
		delete T; T = NULL;
	}
	if (R)
	{
		delete R;
		R = NULL;
	}
	if (S)
	{
		delete S;
		S = NULL;
	}
	return true;
}
GNurbsSgc::GNurbsSgc() : GNurbsSrf()
{
	X = NULL;	R = NULL;
	S = NULL;	T = NULL;
}
GNurbsSgc::GNurbsSgc(const GNurbsSgc *pGC) : GNurbsSrf(pGC)
{
	X = new GNurbsCrv(pGC->X);
	T = new GNurbsCrv(pGC->T);
	R = new GNurbsCrv(pGC->R);
	S = new GNurbsCrv(pGC->S);
}

GNurbsSgc::GNurbsSgc(const GNurbsSgc &GC) : GNurbsSrf(GC)
{
	X = new GNurbsCrv(GC.X);
	T = new GNurbsCrv(GC.T);
	R = new GNurbsCrv(GC.R);
	S = new GNurbsCrv(GC.S);
}
GNurbsSgc::GNurbsSgc(GNurbsCrv *X, GNurbsCrv *T, GNurbsCrv *R, GNurbsCrv *S)
{
	this->X = new GNurbsCrv(X);
	this->T = new GNurbsCrv(T);
	if (R)
		this->R = new GNurbsCrv(R);
	else
		this->R = NULL;
	if (S)
		this->S = new GNurbsCrv(S);
	else
		this->S = NULL;
	_alloc();
	CalcCtlPtNet();
}
GNurbsSgc::~GNurbsSgc()
{
	_free();
}
/**************************************************************************
 * operator overloading.
 **************************************************************************/
GNurbsSgc& GNurbsSgc::operator =(GNurbsSgc& gc)
{
	GNurbsSrf::operator =(gc);
	this->X = new GNurbsCrv(gc.X);
	this->T = new GNurbsCrv(gc.T);
	this->R = new GNurbsCrv(gc.R);
	this->S = new GNurbsCrv(gc.S);
	return (*this);
}
/**************************************************************************
 * evaluate surface point.
 **************************************************************************/
void GNurbsSgc::Eval(REAL u, REAL v, REAL4 pt, bool byctlpt)
{
	if (byctlpt)
	{
		GNurbsSrf::Eval(u, v, pt, true);
		return;
	}
	REAL_ARR_ZERO(pt, 4);
	REAL4 t, r, s, tmp;
	X->Eval(v, tmp);
	T->Eval(u, t);
	R->Eval(u, r);
	S->Eval(u, s);
	MATRIX mat;
	::setMotionMatrix(t, r, r, s, mat, true);
	for (int rr = 0; rr < 4; rr++)
		for (int cc = 0; cc < 4; cc++)
			pt[rr] += mat[rr][cc] * tmp[cc];
}


/***************************************************************************
 * implementation of GNurbsVGC class
 ***************************************************************************/


 /**************************************************************************
  * implementation of constructors and descructor of GNurbsVGC class
  **************************************************************************/
bool GNurbsCgc::_alloc()
{
	register int i, j;
	REAL *mknot, *Tknot, *Cknot;
	short mkidx, Tkidx, Ckidx;

	/**************************************************************************
	 * make translation, rotation and x surface (u-direction) to be compatible.
	 *************************************************************************/
	T->MakeKnotCompatible(R);

	Tknot = T->GetKnot();
	Tkidx = T->GetKnotIdx();
	Cknot = C->GetKnotU();
	Ckidx = C->GetKnotIdxU();

	REAL ratio = (Cknot[Ckidx] - Cknot[0]) / (Tknot[Tkidx] - Tknot[0]);
	::translateKnotWithScale(Tknot, Tkidx, Cknot[0] - Tknot[0], ratio);
	mknot = mergeKnotDistinct(Tknot, Tkidx, Cknot, Ckidx, &mkidx);
	for (i = 1; i < mkidx; i++)
	{
		int mult = T->FindKnotMult(mknot[i]);
		if (mult == 0)
			T->InsertMultiKnot(mknot[i], 1);
		/********************************************************
		 * make contour surface to be picecwise bezier surface
		 * in the direction of u via knot insertion.
		 ********************************************************/
		mult = C->FindKnotMultU(mknot[i]);
		int r = C->GetDegU() - mult;
		if (r > 0)
			C->InsertMultiKnot(U_DIR, mknot[i], r);
	}
	delete[] mknot;

	T->MakeKnotCompatible(R);

	/**********************************************************************
	 * get merged knot vector, which has distinct knot value.
	 **********************************************************************/
	mknot = mergeKnotDistinct(T->GetKnot(), T->GetKnotIdx(),
		R->GetKnot(), R->GetKnotIdx(), &mkidx);

	/***********************************************************************
	 * degree setting.
	 * p : U-direction degree.
	 * q : v-direction degree.
	 ***********************************************************************/
	this->p = T->GetDegree() + 2 * R->GetDegree() + C->GetDegU();
	this->q = C->GetDegV();

	/**********************************************************************
	 * get control point net size for each direction.
	 **********************************************************************/
	short numknot = p * (mkidx + 1) + 1; // numknot+1 is # of motion knot vector;
	this->m = numknot - p - 1;
	this->n = C->GetCtlPtIdxV();
	this->P = new REAL4 *[m + 1];
	for (i = 0; i < m + 1; P[i] = new REAL4[n + 1], REAL4_ARR_ZERO(P[i], n + 1), i++);

	/**********************************************************************
	 * get knot vectors for each direction.
	 **********************************************************************/
	this->U = new REAL[numknot + 1];
	this->V = new REAL[C->GetKnotIdxV() + 1];
	for (i = 0; i < mkidx + 1; i++)
		for (j = 1; j < p + 1; j++)
			U[i*p + j] = mknot[i];
	U[0] = mknot[0]; U[numknot] = mknot[mkidx];
	REAL_ARR_COPY(V, (C->GetKnotV()), (C->GetKnotIdxV() + 1));
	/**********************************************************************
	 * memory free.
	 **********************************************************************/
	delete[] mknot;
	return true;
}
bool GNurbsCgc::CalcCtlPtNet()
{
	int s, i, j, k;
	GNurbsMot mot(T, R);
	MATRIX *ctlmat = mot.GetCtlMat();
	int mdeg = mot.GetDegree();
	int udeg = C->GetDegU();
	int vdeg = C->GetDegV();
	/********************************************************************
	 * seg is number of motion segment
	 * seg is equal to the number of segment of contour in u direction
	 ********************************************************************/
	int seg = mot.GetCtlMatIdx() / mdeg;

	/* memory allocation. */
	short** BIN = new short*[p + 1];
	for (i = 0; i < p + 1; BIN[i] = new short[p + 1], i++);
	::getBinomialCoeff(p, BIN);

	for (s = 0; s < seg; s++)
	{
		for (k = 0; k <= n; k++)
			REAL_ARR_ZERO(P[s*p][k], 4);

		/* for each motion segment and each contour segment ( u-direction ) */
		for (i = 0; i <= mdeg; i++)
			for (j = 0; j <= udeg; j++)
			{
				REAL coeff = ((REAL)BIN[mdeg][i] * (REAL)BIN[udeg][j])
					/ (REAL)BIN[mdeg + udeg][i + j];
				for (k = 0; k <= n; k++)
				{
					REAL4 x;
					C->GetCtlPt((s*udeg) + j, k, x);
					x[0] *= x[3]; x[1] *= x[3]; x[2] *= x[3];

					P[s*p + i + j][k][0] += coeff * (
						ctlmat[s*mdeg + i][0][0] * x[0] + ctlmat[s*mdeg + i][0][1] * x[1] +
						ctlmat[s*mdeg + i][0][2] * x[2] + ctlmat[s*mdeg + i][0][3] * x[3]);

					P[s*p + i + j][k][1] += coeff * (
						ctlmat[s*mdeg + i][1][0] * x[0] + ctlmat[s*mdeg + i][1][1] * x[1] +
						ctlmat[s*mdeg + i][1][2] * x[2] + ctlmat[s*mdeg + i][1][3] * x[3]);

					P[s*p + i + j][k][2] += coeff * (
						ctlmat[s*mdeg + i][2][0] * x[0] + ctlmat[s*mdeg + i][2][1] * x[1] +
						ctlmat[s*mdeg + i][2][2] * x[2] + ctlmat[s*mdeg + i][2][3] * x[3]);

					P[s*p + i + j][k][3] += coeff * (
						ctlmat[s*mdeg + i][3][0] * x[0] + ctlmat[s*mdeg + i][3][1] * x[1] +
						ctlmat[s*mdeg + i][3][2] * x[2] + ctlmat[s*mdeg + i][3][3] * x[3]);
				}
			}
	}
	for (i = 0; i <= m; i++)
		for (j = 0; j <= n; j++)
		{
			P[i][j][0] /= P[i][j][3];
			P[i][j][1] /= P[i][j][3];
			P[i][j][2] /= P[i][j][3];
		}
	/* memory free. */
	for (i = 0; i < p + 1; delete[] BIN[i], i++);
	delete BIN;
	return true;
}
bool GNurbsCgc::_free()
{
	if (C)
	{
		delete C; C = NULL;
	}
	if (T)
	{
		delete T; T = NULL;

	}
	if (R)
	{
		delete R; R = NULL;
	}
	return true;
}
GNurbsCgc::GNurbsCgc() : GNurbsSrf()
{
	C = NULL;
	R = NULL;
	T = NULL;
}
GNurbsCgc::GNurbsCgc(GNurbsCgc *pGC) : GNurbsSrf(pGC)
{
	C = new GNurbsSrf(pGC->C);
	T = new GNurbsCrv(pGC->T);
	R = new GNurbsCrv(pGC->R);
}
GNurbsCgc::GNurbsCgc(GNurbsCgc &GC) : GNurbsSrf(GC)
{
	C = new GNurbsSrf(GC.C);
	T = new GNurbsCrv(GC.T);
	R = new GNurbsCrv(GC.R);
}
GNurbsCgc::GNurbsCgc(GNurbsSrf *C, GNurbsCrv *T, GNurbsCrv *R)
{
	this->C = new GNurbsSrf(C);
	this->T = new GNurbsCrv(T);
	this->R = new GNurbsCrv(R);
	_alloc();
	CalcCtlPtNet();
}
GNurbsCgc::~GNurbsCgc()
{
	_free();
}
/**************************************************************************
 * operator overloading.
 **************************************************************************/
GNurbsCgc& GNurbsCgc::operator =(GNurbsCgc& gc)
{
	GNurbsSrf::operator =(gc);
	this->C = new GNurbsSrf(gc.C);
	this->T = new GNurbsCrv(gc.T);
	this->R = new GNurbsCrv(gc.R);
	return (*this);
}
/**************************************************************************
 * evaluate surface point.
 **************************************************************************/
void GNurbsCgc::Eval(REAL u, REAL v, REAL4 pt, bool byctlpt)
{
	if (byctlpt)
	{
		GNurbsSrf::Eval(u, v, pt, true);
		return;
	}
	REAL_ARR_ZERO(pt, 4);
	GNurbsMot mot(T, R);
	MATRIX mat;
	mot.Eval(u, mat, true);
	REAL4 vtx;
	C->Eval(u, v, vtx, true);
	for (int rr = 0; rr < 4; rr++)
		for (int cc = 0; cc < 4; cc++)
			pt[rr] += mat[rr][cc] * vtx[cc];
}