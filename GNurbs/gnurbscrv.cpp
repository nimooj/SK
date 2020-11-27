// NurbsCrv.cpp: implementation of the NurbsCrv class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "..\usr\include\GNurbs\GNurbs.h"
// #include <iostream.h>
#include <iostream>

using namespace std;

/***************************************************************************************************
	for memory allocation
****************************************************************************************************/
bool GNurbsCrv::_alloc(const short p, const short n)
{
	this->p = p;
	this->n = n;
	this->P = new REAL4[n + 1];
	if (!(this->P)) return false;
	this->U = new REAL[p + n + 2];
	if (!(this->U))
	{
		delete[] P;
		return false;
	}
	return true;
}
bool GNurbsCrv::_free()
{
	if (P && U)
	{
		delete[] P;
		delete[] U;
		return true;
	}
	else
		return false;
}
/*********************************************************************/
/* Construction/Destruction                                          */
/*********************************************************************/
GNurbsCrv::GNurbsCrv()
{
	id = -1;
	p = 0;
	n = 0;
	P = NULL;
	U = NULL;
	G3 = true;
	Active = true;
}
GNurbsCrv::GNurbsCrv(short p, short n, REAL4* P, REAL* U, bool G3)
{
	this->id = -1;
	this->G3 = G3;
	this->Active = true;
	_alloc(p, n);
	REAL_ARR_COPY(this->U, U, p + n + 2);
	REAL4_ARR_COPY(this->P, P, n + 1);
}
GNurbsCrv::GNurbsCrv(short p, short n, REAL4 *P, CREATE_MODE cmode, PARAM_TYPE ptype, bool G3)
{
	this->id = -1;
	this->G3 = G3;
	this->Active = true;
	switch (cmode)
	{
	case CTLPT_CRV:
		_alloc(p, n);
		::getUniformKnotVector(p, n, U);
		REAL4_ARR_COPY(this->P, P, n + 1);
		break;
	case INTER_CRV:
		this->P = NULL;
		this->U = NULL;
		this->InterCrv(p, P, n, ptype, G3);
		break;
	}
}
GNurbsCrv::GNurbsCrv(short p, short n, REAL4 *Q, short Qm, PARAM_TYPE ptype, bool G3)
{
	this->id = -1;
	this->G3 = G3;
	this->Active = true;
	this->P = NULL;
	this->U = NULL;
	ApproxCrv(p, n, Q, Qm, ptype, G3);
}
GNurbsCrv::GNurbsCrv(const GNurbsCrv& Crv)
{
	this->id = -1;
	this->G3 = Crv.G3;
	this->Active = Crv.Active;
	_alloc(Crv.GetDegree(), Crv.GetCtlPtIdx());
	REAL_ARR_COPY(U, Crv.GetKnot(), p + n + 2);
	REAL4_ARR_COPY(P, Crv.GetCtlPt(), n + 1);
}
GNurbsCrv::GNurbsCrv(const GNurbsCrv* pCrv)
{
	this->id = -1;
	this->G3 = pCrv->G3;
	this->Active = pCrv->Active;
	_alloc(pCrv->GetDegree(), pCrv->GetCtlPtIdx());
	REAL_ARR_COPY(U, pCrv->GetKnot(), p + n + 2);
	REAL4_ARR_COPY(P, pCrv->GetCtlPt(), n + 1);

}
GNurbsCrv::~GNurbsCrv()
{
	_free();
}
/*********************************************************************/
/* operator overloading ( =, * )                                     */
/*********************************************************************/
GNurbsCrv& GNurbsCrv::operator=(const GNurbsCrv& crv)
{
	this->id = -1;
	this->G3 = crv.G3;
	this->Active = crv.Active;
	_free();
	_alloc(crv.GetDegree(), crv.GetCtlPtIdx());
	REAL_ARR_COPY(U, crv.GetKnot(), p + n + 2);
	REAL4_ARR_COPY(P, crv.GetCtlPt(), n + 1);
	return (*this);
}
GNurbsCrv GNurbsCrv::operator*(GNurbsCrv& crv)
{
	// ret = src1 * src2 
	GNurbsCrv src1(this), src2(crv);

	// check G3 type.
	assert(src1.G3 == src2.G3);

	// merge two knot vectors of src1 and src2
	// and get distinct knot vector of merged vector.
	REAL *knot;
	short kidx;
	knot = mergeKnotDistinct
	(src1.GetKnot(), src1.GetKnotIdx(), src2.GetKnot(), src2.GetKnotIdx(), &kidx);

	// make compatible and make bezier segments.
	src1.MakeKnotCompatible(&src2);
	src1.MakeBzrSegment();
	src2.MakeBzrSegment();

	// get degrees of each crv.
	int p1 = src1.GetDegree();
	int p2 = src2.GetDegree();

	// other local variables.
	int i, j, k;
	REAL coeff;
	REAL4 P1, P2, tmpP;


	// result curve initialization.
	int	p = p1 + p2; // degree.
	int	n = kidx * p, m = p + n + 1; // n+1 is the # of ctlpt, m+1 is the # of knots.
	REAL4	*P = new REAL4[n + 1]; // alloc control point's array
	REAL	*U = new REAL[m + 1]; // alloc knot array.
	REAL4_ARR_ZERO(P, n + 1); // initialize cotrol points array.


	// initialize knot vectors
	for (i = 0; i < kidx + 1; i++)
		for (j = 1; j < p + 1; j++)
			U[i*p + j] = knot[i];
	U[0] = knot[0]; U[m] = knot[kidx];

	// memory allocation for binomial coefficients.
	short** BIN = new short*[p + 1];
	for (i = 0; i < p + 1; BIN[i] = new short[p + 1], i++);
	::getBinomialCoeff(p, BIN);

	for (i = 0; i < kidx; i++)
	{
		GNurbsCrv seg1 = src1.GetBzrSegment(i);
		GNurbsCrv seg2 = src2.GetBzrSegment(i);
		if (seg1.G3 && seg2.G3)
		{
			seg1.ApplyWeight();
			seg2.ApplyWeight();
		}
		REAL4_ARR_ZERO(P[i*p], 1);
		for (j = 0; j <= p1; j++)
			for (k = 0; k <= p2; k++)
			{
				coeff = (REAL)BIN[p1][j] * (REAL)BIN[p2][k] / (REAL)BIN[p][j + k];
				seg1.GetCtlPt(j, P1);
				seg2.GetCtlPt(k, P2);
				tmpP[0] = P1[0] * P2[0] * coeff;
				tmpP[1] = P1[1] * P2[1] * coeff;
				tmpP[2] = P1[2] * P2[2] * coeff;
				tmpP[3] = P1[3] * P2[3] * coeff;

				P[(i*p) + j + k][0] += tmpP[0];
				P[(i*p) + j + k][1] += tmpP[1];
				P[(i*p) + j + k][2] += tmpP[2];
				P[(i*p) + j + k][3] += tmpP[3];
			}
	}
	// get unapplied weight form! very important.
	if (src1.G3) // rational curve i.e not quaternion curve.
	{
		for (i = 0; i < n + 1; i++)
		{
			P[i][0] /= P[i][3];
			P[i][1] /= P[i][3];
			P[i][2] /= P[i][3];
		}
	}

	// create result curve.
	GNurbsCrv ret(p, n, P, U, src1.G3);

	// memory free.
	for (i = 0; i < p + 1; delete[] BIN[i], i++);
	delete BIN;
	delete[] knot;
	delete[] P;
	delete[] U;

	// return result
	return ret;
}
GNurbsCrv GNurbsCrv::operator^(GNurbsCrv& crv)
{
	GNurbsCrv ret;
	return ret;
}

/*********************************************************************/
/* for internal useage                                               */
/*********************************************************************/
void GNurbsCrv::ApplyWeight()
{
	register int i;
	for (i = 0; i < n + 1; i++)
	{
		P[i][0] = P[i][0] * P[i][3];
		P[i][1] = P[i][1] * P[i][3];
		P[i][2] = P[i][2] * P[i][3];
	}
}
void GNurbsCrv::ApplyUnWeight()
{
	register int i;
	for (i = 0; i < n + 1; i++)
	{
		P[i][0] /= P[i][3];
		P[i][1] /= P[i][3];
		P[i][2] /= P[i][3];
	}
}

/*********************************************************************/
/* Evalution Interfaces For GNurbsCrv                                */
/*********************************************************************/
void GNurbsCrv::Eval(REAL u, REAL4 pt)
{
	register int i;
	REAL_ARR_ZERO(pt, 4);
	short idx = FindKnotSpan(u);
	REAL* basis = new REAL[p + 1];
	GetBasis(u, idx, basis);

	if (G3) 	// the case of considering weight.
		ApplyWeight();

	for (i = 0; i <= p; i++)
	{
		pt[0] += basis[i] * P[idx - p + i][0];
		pt[1] += basis[i] * P[idx - p + i][1];
		pt[2] += basis[i] * P[idx - p + i][2];
		pt[3] += basis[i] * P[idx - p + i][3];
	}

	if (G3)
	{
		ApplyUnWeight();
		::poly2rational(pt);
	}

	delete[] basis;
}
void GNurbsCrv::EvalDeriv(REAL u, short nth, REAL4 pt)
{
	register int i, j;
	REAL_ARR_ZERO(pt, 4);
	assert(nth <= p);
	short idx = FindKnotSpan(u);
	REAL** nders = new REAL*[p + 1];
	for (j = 0; j <= p; j++)
		nders[j] = new REAL[p + 1];
	GetDerivBasis(u, idx, nders);
	if (G3) // the case of considering weight.
	{
		ApplyWeight();
		// 1. P=( x, y, z, w ) --> Pw=( w*x, w*y, w*z, w ) 
		// 2. calculate derivative of Pw from 0 to nth 
		// 3. Aders[i][0-2] contains i-th derivatives of Pw'=( w*x, w*y, w*z )
		// 4. Aders[i][3] contains i-th derivative of w
		REAL4* Aders = new REAL4[nth + 1];
		REAL4* CK = new REAL4[nth + 1];
		short** Bin = new short*[nth + 1];
		for (i = 0; i < nth + 1; i++)
			Bin[i] = new short[nth + 1];
		::getBinomialCoeff(nth, Bin);
		for (i = 0; i < nth + 1; i++)
		{
			REAL_ARR_ZERO(Aders[i], 4);
			for (j = 0; j <= p; j++)
			{
				Aders[i][0] += nders[i][j] * P[idx - p + j][0];
				Aders[i][1] += nders[i][j] * P[idx - p + j][1];
				Aders[i][2] += nders[i][j] * P[idx - p + j][2];
				Aders[i][3] += nders[i][j] * P[idx - p + j][3];
			}
		}
		REAL3 v;
		for (i = 0; i < nth + 1; i++)
		{
			REAL_ARR_COPY(v, Aders[i], 3);
			for (j = 1; j <= i; j++)
			{
				v[0] = v[0] - Bin[i][j] * Aders[j][3] * CK[i - j][0];
				v[1] = v[1] - Bin[i][j] * Aders[j][3] * CK[i - j][1];
				v[2] = v[2] - Bin[i][j] * Aders[j][3] * CK[i - j][2];
				if (EQ_ZERO(v[0]))	v[0] = 0.0;
				if (EQ_ZERO(v[1]))	v[1] = 0.0;
				if (EQ_ZERO(v[2]))	v[2] = 0.0;
			}
			CK[i][0] = v[0] / Aders[0][3];
			CK[i][1] = v[1] / Aders[0][3];
			CK[i][2] = v[2] / Aders[0][3];
			CK[i][3] = 1.0f;
		}
		REAL_ARR_COPY(pt, CK[nth], 4);
		ApplyUnWeight();
		delete[] Aders;
		delete[] CK;
		for (i = 0; i < nth + 1; delete[] Bin[i], i++);
		delete[] Bin;
	}
	else // the case of not consigering weight.
		 // i.e the case of 4-dimensional polynomial.
	{
		for (j = 0; j <= p; j++)
		{
			pt[0] = pt[0] + nders[nth][j] * P[idx - p + j][0];
			pt[1] = pt[1] + nders[nth][j] * P[idx - p + j][1];
			pt[2] = pt[2] + nders[nth][j] * P[idx - p + j][2];
			pt[3] = pt[3] + nders[nth][j] * P[idx - p + j][3];
		}
	}
	for (j = 0; j <= p; delete[] nders[j], j++);
	delete nders;
}
/*********************************************************************/
/* Utilities  For NurbsCrv                                           */
/*********************************************************************/
GNurbsCrv GNurbsCrv::GetRatDerivCrv(short nth)
{
	/*
	assert( nth <= p );
	GNurbsCrv crv(this);
	if ( !crv.IsBezier() )
		crv.MakeBzrSegment();
	int cnum = crv.n / p;
	int i, j, k;
	for ( i = 0; i < cnum; i++ )
	{
		GNurbsCrv cseg = crv.GetBzrSegment( i );
		cseg.ApplyWeight();

		short n = cseg.GetCtlPtIdx();

		REAL4 *dA = new REAL4[n];
		REAL4_ARR_ZERO( dA, n );

		REAL4* CtlPt = cseg.GetCtlPt();
		for ( j = 0; j < n; j++ )
		{
			dA[j][0] = n*(CtlPt[j+1][0] - CtlPt[j][0]);
			dA[j][1] = n*(CtlPt[j+1][1] - CtlPt[j][1]);
			dA[j][2] = n*(CtlPt[j+1][2] - CtlPt[j][2]);
			dA[j][3] = n*(CtlPt[j+1][3] - CtlPt[j][3]);
		}
		// memory allocation for binomial coefficients.

		short p1 = p-1;
		short p2 = p;
		short p = p1 + p2;

		REAL4 *P = new REAL4[p+1];
		REAL4_ARR_ZERO( P, p+1 );

		REAL *W = new REAL[p+2];
		REAL_ARR_ZERO( W, p+2 );

		short** BIN = new short* [p+2];
		for ( j = 0; j < p+2; BIN[j] = new short[p+2], j++ );
		::getBinomialCoeff( p+1, BIN );

		REAL coeff;
		REAL4 P1, P2, tmpP;
		for ( j = 0; j < n; j++ )
		for ( k = 0; k < n+1; k++ )
		{
			coeff = (REAL)BIN[p1][j] * (REAL)BIN[p2][k] / (REAL)BIN[p][j+k];

			REAL4_ARR_COPY( P1, dA[j], 1 );
			cseg.GetCtlPt( k, P2 );

			tmpP[0] = P1[0]*P2[3]*coeff;
			tmpP[1] = P1[1]*P2[3]*coeff;
			tmpP[2] = P1[2]*P2[3]*coeff;

			P[j+k][0] += tmpP[0];
			P[j+k][1] += tmpP[1];
			P[j+k][2] += tmpP[2];
		}
		for ( j = 0; j < n+1; j++ )
		for ( k = 0; k < n; k++ )
		{
			coeff = (REAL)BIN[p1][j] * (REAL)BIN[p2][k] / (REAL)BIN[p][j+k];

			cseg.GetCtlPt( j, P1 );
			REAL4_ARR_COPY( P2, dA[k], 1 );

			tmpP[0] = P1[0]*P2[3]*coeff;
			tmpP[1] = P1[1]*P2[3]*coeff;
			tmpP[2] = P1[2]*P2[3]*coeff;

			P[j+k][0] -= tmpP[0];
			P[j+k][1] -= tmpP[1];
			P[j+k][2] -= tmpP[2];
		}
		for ( j = 0; j < n+1; j++ )
		for ( k = 0; k < n+1; k++ )
		{
			coeff = (REAL)BIN[p1][j] * (REAL)BIN[p2][k] / (REAL)BIN[p][j+k];
			cseg.GetCtlPt( j, P1 );
			cseg.GetCtlPt( k, P2 );

			tmpP[3] = P1[3]*P2[3]*coeff;

			W[j+k] += tmpP[3];
		}
	}
	*/
	GNurbsCrv crv;
	return crv;
}
GNurbsCrv GNurbsCrv::GetPolyDerivCrv(short nth)
{
	// precondition.
	assert(nth <= p);

	// local variables.
	int i;

	// alloc control point.
	REAL4* ctlpt = new REAL4[n - nth + 1];

	// alloc knot vector.
	REAL* knot = new REAL[n + p + 2 * (1 - nth)];
	for (i = 0; i < n + p + 2 * (1 - nth); i++)
		knot[i] = this->U[i + nth];

	// get derivative control points.
	GetDerivCtlPt(nth, ctlpt);

	// create nth - derivative curve.
	GNurbsCrv retCrv(p - nth, n - nth, ctlpt, knot, G3);

	// free control points and knot vectors.
	delete[] ctlpt;
	delete[] knot;

	// return nth-derivative curve.
	return retCrv;
}
void GNurbsCrv::GetDerivCtlPt(short nth, REAL4* ctlpt)
{
	// allocation.
	REAL4** derctlpt = new REAL4*[p + 1];
	for (int i = 0; i < p + 1; i++)
		derctlpt[i] = new REAL4[n + 1];

	// get nurbs derivative (0-n)th contorl points
	// if G3 is true, rational curve's control points are obtained, but not implemented yet.
	// if G3 is false, polynomial curve's control points are obtained.
	::getNurbsDerivCtlPt(p, n, P, U, derctlpt, G3);
	for (int i = 0; i < n - nth + 1; i++)
	{
		ctlpt[i][0] = derctlpt[nth][i][0];
		ctlpt[i][1] = derctlpt[nth][i][1];
		ctlpt[i][2] = derctlpt[nth][i][2];
		ctlpt[i][3] = derctlpt[nth][i][3];
	}

	// free.
	for (int i = 0; i < p + 1; i++)
		delete[] derctlpt[i];
	delete derctlpt;
}
/*****************************************************************************
 * 1. MakeBzrSegment
 *    convert nurbs curve into piecewise bezier segment.
/*****************************************************************************/
void GNurbsCrv::MakeBzrSegment()
{
	register int i, j;
	short r = 0, idx = 0;
	REAL* X = new REAL[1000];
	for (i = p + 1; i <= n; i++)
	{
		r = (p - FindKnotMult(U[i]));
		for (j = 0; j < r; j++)
		{
			X[idx] = U[i];
			idx++;
		}
	}
	if (idx > 0)
		RefineKnotVector(X, idx - 1);
	delete[] X;
}
/*****************************************************************************
 * 2. GetBzrSegment.
 *    convert nurbs curve into piecewise bezier segment.
 *    return ith bezier curve.
/*****************************************************************************/
GNurbsCrv GNurbsCrv::GetBzrSegment(short ith, bool scale)
{
	register int i;
	if (!IsBzrSegment())
		MakeBzrSegment();
	assert(ith < (n / p));
	REAL* nU = new REAL[2 * p + 2];
	// 2002. 11. 25.
	for (i = 0; i < p + 1; i++)
	{
		if (p == 0)
		{
			if (scale)
				nU[i] = 0.0f;
			else
				nU[i] = U[ith];
		}
		else
		{
			if (scale)
				nU[i] = 0.0f;
			else
				nU[i] = U[ith*p + 1];
		}
	}
	for (i = p + 1; i < 2 * p + 2; i++)
	{
		if (p == 0)
		{
			if (scale)
				nU[i] = 1.0f;
			else
				nU[i] = U[ith + 1];
		}
		else
		{
			if (scale)
				nU[i] = 1.0f;
			else
				nU[i] = U[(ith + 1)*p + 1];
		}
	}
	if (p == 0)
	{
		GNurbsCrv crv(p, p, &P[ith], nU, G3);
		delete[] nU;
		return crv;
	}
	else
	{
		GNurbsCrv crv(p, p, &P[ith*p], nU, G3);
		delete[] nU;
		return crv;
	}
}
/*****************************************************************************
 * 3. TranslateKnot.
 *    translate knot vector with scaling.
/*****************************************************************************/
void GNurbsCrv::TranslateKnot(REAL offset, REAL scale)
{
	::translateKnotWithScale(U, n + p + 1, offset, scale);
}
/*****************************************************************************
 * 4. MakeKnotCompatible.
 *    make curve to be compatible to pCrv.
 *    union knot vector.
 *    insert new knot value once.
/*****************************************************************************/
void GNurbsCrv::MakeKnotCompatible(GNurbsCrv* pCrv)
{
	register int i;
	REAL* knot = pCrv->GetKnot();
	short kidx = pCrv->GetKnotIdx();
	short m = GetKnotIdx();
	::translateKnotWithScale(U, m, knot[0] - U[0], (knot[kidx] - knot[0]) / (U[m] - U[0]));

	REAL* mknot;
	short mkidx;
	mknot = mergeKnotDistinct(U, m, knot, kidx, &mkidx);
	for (i = 1; i < mkidx; i++)
	{
		if (this->FindKnotMult(mknot[i]) == 0)
			this->InsertMultiKnot(mknot[i], 1);
		if (pCrv->FindKnotMult(mknot[i]) == 0)
			pCrv->InsertMultiKnot(mknot[i], 1);
	}
	delete[] mknot;
}
/*****************************************************************************
 * 5. MakeDegCompatible.
 *    make curve to be compatible to pCrv.
/*****************************************************************************/
void GNurbsCrv::MakeDegCompatible(GNurbsCrv *pCrv)
{
	short p0 = this->p;
	short p1 = pCrv->GetDegree();
	short p = MAX(p0, p1);
	ElevateDegree(p - p0);
	pCrv->ElevateDegree(p - p1);
}
/*****************************************************************************
 *  check curve is piecewise bezier form.
/*****************************************************************************/
bool GNurbsCrv::IsBzrSegment()
{
	register int i;
	short m = GetKnotIdx();
	for (i = 0; i < m + 1; i++)
	{
		if (FindKnotMult(U[i]) < p)
			return false;
	}
	return true;
}
/*********************************************************************/
/* Fundamental Geometric Algorithms For NurbsCrv                     */
/*********************************************************************/
// 1. KNOT INSERTION.
void GNurbsCrv::InsertMultiKnot(REAL u, short r)
{
	register int i, j;
	short idx, s;
	short m, n1, m1, L;
	idx = FindKnotSpan(u); // knot span idex.
	s = FindKnotMult(u);   // knot multiplicity.

	assert(r + s <= p); // pre - codition.

	m = n + p + 1; // m+1 is # of old knot.
	n1 = n + r; // n1+1 is # of new control points.
	m1 = m + r;  // m1+1 is # of new knot.

	REAL4* P1 = new REAL4[n1 + 1]; // new control points.
	REAL* U1 = new REAL[m1 + 1]; // new knot vectors.
	REAL4* R = new REAL4[p + 1]; // temporary control points.

	// load new knot vectors.
	for (i = 0; i <= idx; U1[i] = U[i], i++);
	for (i = 1; i <= r; U1[idx + i] = u, i++);
	for (i = idx + 1; i <= m; U1[i + r] = U[i], i++);

	// save unaltered control points
	for (i = 0; i <= idx - p; REAL_ARR_COPY(P1[i], P[i], 4), i++);
	for (i = idx - s; i <= n; REAL_ARR_COPY(P1[i + r], P[i], 4), i++);

	if (G3) // the case of considering weight 
			   // i.e rational form
	{
		for (i = 0; i <= p - s; i++)
		{
			REAL wx, wy, wz, w;
			wx = P[idx - p + i][0];
			wy = P[idx - p + i][1];
			wz = P[idx - p + i][2];
			w = P[idx - p + i][3];
			R[i][0] = wx * w;
			R[i][1] = wy * w;
			R[i][2] = wz * w;
			R[i][3] = w;
		}
	}
	else // the case of not considering weight
		 // i.e 4 dimensional polynomial
	{
		for (i = 0; i <= p - s; i++)
		{
			R[i][0] = P[idx - p + i][0];
			R[i][1] = P[idx - p + i][1];
			R[i][2] = P[idx - p + i][2];
			R[i][3] = P[idx - p + i][3];
		}
	}
	// knot insert process.
	for (j = 1; j <= r; j++)
	{
		L = idx - p + j;
		for (i = 0; i <= p - j - s; i++)
		{
			REAL alpha = (u - U[L + i]) / (U[i + idx + 1] - U[L + i]);
			R[i][0] = alpha * R[i + 1][0] + (1.0f - alpha) * R[i][0];
			R[i][1] = alpha * R[i + 1][1] + (1.0f - alpha) * R[i][1];
			R[i][2] = alpha * R[i + 1][2] + (1.0f - alpha) * R[i][2];
			R[i][3] = alpha * R[i + 1][3] + (1.0f - alpha) * R[i][3];
		}
		P1[L][0] = R[0][0];
		P1[L][1] = R[0][1];
		P1[L][2] = R[0][2];
		P1[L][3] = R[0][3];

		P1[idx + r - j - s][0] = R[p - j - s][0];
		P1[idx + r - j - s][1] = R[p - j - s][1];
		P1[idx + r - j - s][2] = R[p - j - s][2];
		P1[idx + r - j - s][3] = R[p - j - s][3];
	}
	for (i = L + 1; i < idx - s; i++)
	{
		P1[i][0] = R[i - L][0];
		P1[i][1] = R[i - L][1];
		P1[i][2] = R[i - L][2];
		P1[i][3] = R[i - L][3];
	}
	L = idx - p + 1;

	if (G3)
	{
		for (i = L; i <= idx + r - s - 1; i++)
		{
			P1[i][0] /= P1[i][3];
			P1[i][1] /= P1[i][3];
			P1[i][2] /= P1[i][3];
		}
	}

	_free();
	_alloc(p, n1);

	REAL_ARR_COPY(this->U, U1, p + n1 + 2);
	REAL4_ARR_COPY(this->P, P1, n1 + 1);

	delete[] R;
	delete[] U1;
	delete[] P1;
}

// 2. KNOT REFINEMENT
void GNurbsCrv::RefineKnotVector(REAL* X, short r)
{
	// input parameter 
	// X : new knot vector that is inserted into U
	// r : r+1 is # of X's knot
	register int j, l;
	short m = n + p + 1;
	short a = FindKnotSpan(X[0]);
	short b = FindKnotSpan(X[r]);
	short i, k, idx;
	REAL* nU = new REAL[m + r + 2];
	REAL4* nP = new REAL4[n + r + 2];
	REAL alpha = 0.0f;
	b = b + 1;

	if (G3)
		ApplyWeight();

	for (j = 0; j <= a - p; j++)
		REAL_ARR_COPY(nP[j], P[j], 4);
	for (j = b - 1; j <= n; j++)
		REAL_ARR_COPY(nP[j + r + 1], P[j], 4);
	for (j = 0; j <= a; j++)
		nU[j] = U[j];
	for (j = b + p; j <= m; j++)
		nU[j + r + 1] = U[j];
	i = b + p - 1;
	k = b + p + r;
	for (j = r; j >= 0; j--)
	{
		while (X[j] <= U[i] && i > a)
		{
			REAL_ARR_COPY(nP[k - p - 1], P[i - p - 1], 4);
			nU[k] = U[i];
			k = k - 1;
			i = i - 1;
		}
		REAL_ARR_COPY(nP[k - p - 1], nP[k - p], 4);
		for (l = 1; l <= p; l++)
		{
			idx = k - p + l;
			alpha = nU[k + l] - X[j];
			if (EQ_ZERO(ABS(alpha)))
				REAL_ARR_COPY(nP[idx - 1], nP[idx], 4);
			else
			{
				alpha = alpha / (nU[k + l] - U[i - p + l]);
				nP[idx - 1][0] = alpha * nP[idx - 1][0] + (1.0f - alpha) * nP[idx][0];
				nP[idx - 1][1] = alpha * nP[idx - 1][1] + (1.0f - alpha) * nP[idx][1];
				nP[idx - 1][2] = alpha * nP[idx - 1][2] + (1.0f - alpha) * nP[idx][2];
				nP[idx - 1][3] = alpha * nP[idx - 1][3] + (1.0f - alpha) * nP[idx][3];
			}
		}
		nU[k] = X[j];
		k--;
	}
	_free();
	_alloc(p, n + r + 1);
	REAL_ARR_COPY(U, nU, m + r + 2);
	REAL4_ARR_COPY(P, nP, n + 1);
	if (G3)
		ApplyUnWeight();
	delete[] nU;
	delete[] nP;
}
//3. KNOT REMOVE
short GNurbsCrv::RemoveMultiKnot(REAL u, short r)
{
	short idx = FindKnotSpan(u);
	short s = FindKnotMult(u);
	assert((0 <= r) && (r <= s));

	if (G3) ApplyWeight();

	short scs = ::removeCrvKnot(p, n, P, U, u, idx, s, r);
	short new_n = n - scs;
	REAL4* nP = new REAL4[new_n + 1];
	REAL* nU = new REAL[p + new_n + 2];
	REAL4_ARR_COPY(nP, P, new_n + 1);
	REAL_ARR_COPY(nU, U, p + new_n + 2);
	_free();
	_alloc(p, new_n);
	REAL4_ARR_COPY(P, nP, n + 1);
	REAL_ARR_COPY(U, nU, n + p + 2);

	if (G3) ApplyUnWeight();

	return scs;
}
//4. Degree Elevation
void GNurbsCrv::ElevateDegree(short t)
{
	// Degree Elevate a curve t-times.
	if (t == 0)
		return;
}

/****************************************************************************
 *	Curve Fitting.
*****************************************************************************/
void GNurbsCrv::InterCrv(short p, REAL4* Q, short n, PARAM_TYPE ptype, bool isG3)
{
	_free();
	this->G3 = isG3;
	this->p = p;
	this->n = n;
	this->P = new REAL4[n + 1];
	this->U = new REAL[n + p + 2];
	REAL* UK = new REAL[n + 1];
	switch (ptype)
	{
	case UNIFORM:
		::getUniformParam(n, Q, UK);
		::getUniformKnotVector(p, n, U);
		break;
	case CHORD_LEN:
		::getChordLengthParam(n, Q, UK);
		::getAveragingKnotVector(p, n, U, UK);
		break;
	}
	::interpolatePoint(p, n, P, U, UK, Q);

	delete[] UK;
}
void GNurbsCrv::ApproxCrv(short p, short n, REAL4* Q, short Qm, PARAM_TYPE ptype, bool isG3)
{
	_free();
	this->G3 = isG3;
	this->p = p;
	this->n = n;
	this->P = new REAL4[n + 1];
	this->U = new REAL[n + p + 2];
	REAL* UK = new REAL[Qm + 1];

	switch (ptype)
	{
	case UNIFORM:
		::getUniformParam(Qm, Q, UK);
		::getUniformKnotVector(p, n, U);
		break;
	case CHORD_LEN:
		::getChordLengthParam(Qm, Q, UK);
		::getApproxKnotVector(p, n, U, Qm, UK);
		break;
	}
	::approximatePoint(p, n, P, U, UK, Qm, Q);

	delete[] UK;
}
/****************************************************************************
 *	Curve Editing.
*****************************************************************************/
void GNurbsCrv::Edit(short EditCtlPtIdx, REAL dx, REAL dy, REAL dz, REAL alpha)
{
	assert(EditCtlPtIdx >= 0);
	assert(EditCtlPtIdx <= n);
	P[EditCtlPtIdx][0] += (alpha*dx);
	P[EditCtlPtIdx][1] += (alpha*dy);
	P[EditCtlPtIdx][2] += (alpha*dz);
}
void GNurbsCrv::Edit(short EditCtlPtIdx, REAL3 Offset, REAL alpha)
{
	assert(EditCtlPtIdx >= 0);
	assert(EditCtlPtIdx <= n);
	P[EditCtlPtIdx][0] += (alpha*Offset[0]);
	P[EditCtlPtIdx][1] += (alpha*Offset[1]);
	P[EditCtlPtIdx][2] += (alpha*Offset[2]);
}
void GNurbsCrv::Edit(REAL u, REAL3 Offset, REAL alpha)
{
	// loop idx.
	int i, j;

	// get nodes.
	REAL *node = new REAL[n + 1];
	REAL_ARR_ZERO(node, n + 1);
	for (i = 0; i < n + 1; i++)
	{
		REAL sum = 0.0f;
		for (j = 1; j < p + 1; j++)
			sum += U[i + j];
		node[i] = sum / p;
	}

	// get index k, P[k] is translated to produce the desired movement.
	int k = 0;
	REAL min = 10000000.0f;
	for (i = 0; i < n + 1; i++)
	{
		REAL tmp = (float)fabs(u - node[i]);
		if (tmp < min)
		{
			k = i;
			min = tmp;
		}
	}


	REAL4 pt;
	Eval(u, pt);
	REAL d = PT3_DISTANCE(pt, Offset);
	GVector3f V(Offset);

	short idx = FindKnotSpan(u);
	REAL* basis = new REAL[p + 1];
	GetBasis(u, idx, basis);
	REAL Rmax = -10000.0f;
	REAL w = 0.0f;
	for (i = 0; i < p + 1; i++)
	{
		Rmax = MAX(basis[i], Rmax);
		w += basis[i] * P[idx - p + i][3];
	}
	Rmax /= w;
	Rmax = 1 / Rmax;

	P[k][0] = P[k][0] + Rmax * Offset[0];
	P[k][1] = P[k][1] + Rmax * Offset[1];
	P[k][2] = P[k][2] + Rmax * Offset[2];


	// memory free
	delete[] node;
	delete[] basis;
}
void GNurbsCrv::Edit(REAL u, REAL dx, REAL dy, REAL dz, REAL alpha)
{
	REAL3 offset;
	offset[0] = dx;
	offset[1] = dy;
	offset[2] = dz;
	Edit(u, offset, alpha);
}
/****************************************************************************
 * For Frenet Frame.
 * Return rotation curve with degree deg, which approximates The Frenet Frame
 * Knot vector is the same with trajectory expect only end knots.
*****************************************************************************/
GNurbsCrv * GNurbsCrv::GetFrenetFrame(short deg)
{
	short kidx;
	REAL *knot = mergeKnotDistinct(U, n + p + 1, U, n + p + 1, &kidx);
	int i;
	int p = deg;
	int n = kidx + p - 1;
	int m = kidx + p * 2;

	REAL4 *Q = new REAL4[n + 1];
	REAL4 *P = new REAL4[n + 1];
	REAL *UK = new REAL[n + 1];
	REAL *U = new REAL[m + 1];

	// 1. set up uniform parameter.
	::getUniformParam(n, Q, UK);

	// 2. set up knot vector.
	for (i = 0; i < p; U[i] = knot[0], i++);
	for (i = p; i < m - p + 1; U[i] = knot[i - p], i++);
	for (i = m - p + 1; i < m + 1; U[i] = knot[kidx], i++);

	// 3. set up key frame to be interpolated.
	for (i = 0; i < n + 1; i++)
	{
		REAL u = UK[i];
		//REAL4 v0, v1, v2, v3, v4;
		REAL4 v0, v1, v3, v4;
		Eval(u, v0);
		EvalDeriv(u, 1, v1);
		GVector3f y_axis(v1[0], v1[1], v1[2]);
		GVector3f tmp;
		GVector3f y = GVector3f(0.0f, 1.0f, 0.0f);
		if (y_axis == y)
			tmp.setVector(0.0f, 0.0f, 1.0f);
		else
		{
			REAL ang = (REAL)(30.0f * M_PI / 180.0f);
			tmp[0] = (REAL)(v1[0] * cos(ang) + v1[2] * sin(ang));
			tmp[1] = (REAL)(v1[1]);
			tmp[2] = (REAL)(-v1[0] * sin(ang) + v1[2] * cos(ang));
		}
		GVector3f x_axis = y_axis ^ tmp;
		GVector3f z_axis = x_axis ^ y_axis;
		v3[0] = x_axis[0]; v3[1] = x_axis[1]; v3[2] = x_axis[2];
		v4[0] = z_axis[0]; v4[1] = z_axis[1]; v4[2] = z_axis[2];
		PT3_NORMALIZE(v3); // B Vector  // x_axis
		PT3_NORMALIZE(v1); // T Vector  // y_axis
		PT3_NORMALIZE(v4); // N Vector  // z_axis
		/*
		else
		{
			EvalDeriv( u, 2, v2 );
			PT3_CROSS( v3, v1, v2 );
			PT3_NORMALIZE( v1 ); // T Vector  : x-axis
			PT3_NORMALIZE( v3 ); // B Vector  : y-axis
			PT3_CROSS( v4, v3, v1 );
			PT3_NORMALIZE( v4 ); // N Vector  : z-axis
		}
		*/
		GQuaternion quat;
		GVector3f* frameX = new GVector3f(v3[0], v3[1], v3[2]);
		GVector3f* frameY = new GVector3f(v1[0], v1[1], v1[2]);
		GVector3f* frameZ = new GVector3f(v4[0], v4[1], v4[2]);
		/*
		quat.SetFromFrame(GVector3f(v3[0], v3[1], v3[2]),
			GVector3f(v1[0], v1[1], v1[2]),
			GVector3f(v4[0], v4[1], v4[2]));
		*/
		quat.SetFromFrame(*frameX, *frameY, *frameZ);
		Q[i][0] = quat.X;
		Q[i][1] = quat.Y;
		Q[i][2] = quat.Z;
		Q[i][3] = quat.W;
	}

	// 3. set up rotation curve.
	::interpolatePoint(p, n, P, U, UK, Q);

	GNurbsCrv *R = new GNurbsCrv(p, n, P, U, false);
	delete[] P;
	delete[] Q;
	delete[] U;
	delete[] UK;
	delete[] knot;
	return R;
}
/****************************************************************************
 *	For debug...
*****************************************************************************/
void GNurbsCrv::PrintKnotVector()
{
	for (int i = 0; i < GetKnotIdx() + 1; i++)
	{
		cout << U[i] << " : ";
	}
	cout << endl;
}
