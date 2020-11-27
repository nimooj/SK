// NurbsSrf.cpp: implementation of the NurbsSrf class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "..\usr\include\GNurbs\GNurbs.h"
//////////////////////////////////////////////////////////////////////
// for memory allocation and free.
bool GNurbsSrf::_alloc(short p, short m, short q, short n)
{
	this->p = p;
	this->m = m;
	this->q = q;
	this->n = n;
	this->P = new REAL4*[m + 1];
	if (!(this->P)) return false;
	for (int i = 0; i < m + 1; i++)
		P[i] = new REAL4[n + 1];

	this->U = new REAL[p + m + 2];
	this->V = new REAL[q + n + 2];

	if (!(this->U) || !(this->V))
	{
		for (register int i = 0; i < m + 1; i++)
			delete[] P[i];
		delete P;
		return false;
	}

	return true;
}
bool GNurbsSrf::_free()
{
	if (U && V && P)
	{
		delete[] U;
		delete[] V;
		for (register int i = 0; i < m + 1; i++)
			delete[] P[i];
		delete P;
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GNurbsSrf::GNurbsSrf()
{
	id = -1;
	p = q = m = n = 0;
	U = V = NULL;
	P = NULL;
	G3 = true;
	Active = true;
}
GNurbsSrf::GNurbsSrf(short p, short m, REAL* U, short q, short n, REAL* V, REAL4* P, bool G3)
{
	this->id = -1;
	this->G3 = G3;
	this->Active = true;
	_alloc(p, m, q, n);
	REAL_ARR_COPY(this->U, U, p + m + 2);
	REAL_ARR_COPY(this->V, V, q + n + 2);
	for (register int i = 0; i < m + 1; i++)
		REAL4_ARR_COPY(*(this->P[i]), P[i*(n + 1)], n + 1);
}
GNurbsSrf::GNurbsSrf(short p, short m, REAL* U, short q, short n, REAL* V, REAL4** P, bool G3)
{
	this->id = -1;
	this->G3 = G3;
	this->Active = true;
	_alloc(p, m, q, n);
	REAL_ARR_COPY(this->U, U, p + m + 2);
	REAL_ARR_COPY(this->V, V, q + n + 2);
	for (register int i = 0; i < m + 1; i++)
		REAL4_ARR_COPY(*(this->P[i]), &P[i][0], n + 1);
}
GNurbsSrf::GNurbsSrf(short p, short m, short q, short n, REAL4 **P, bool G3)
{
	this->id = -1;
	this->G3 = G3;
	this->Active = true;
	_alloc(p, m, q, n);
	::getUniformKnotVector(p, m, U);
	::getUniformKnotVector(q, n, V);
	for (register int i = 0; i < m + 1; i++)
		REAL4_ARR_COPY(*(this->P[i]), &P[i][0], n + 1);
}
GNurbsSrf::GNurbsSrf(short p, short m, short q, short n, REAL4 *P, bool G3)
{
	this->id = -1;
	this->G3 = G3;
	this->Active = true;
	_alloc(p, m, q, n);
	::getUniformKnotVector(p, m, U);
	::getUniformKnotVector(q, n, V);
	for (register int i = 0; i < m + 1; i++)
		REAL4_ARR_COPY(*(this->P[i]), P[i*(n + 1)], n + 1);
}
GNurbsSrf::GNurbsSrf(const GNurbsSrf& srf)
{
	this->id = -1;
	this->G3 = srf.G3;
	this->Active = srf.Active;

	short p = srf.GetDegU();		
	short q = srf.GetDegV();
	short m = srf.GetCtlPtIdxU();	
	short n = srf.GetCtlPtIdxV();

	REAL* U = srf.GetKnotU();		
	REAL* V = srf.GetKnotV();
	REAL4** P = srf.GetCtlPtNet();

	_alloc(p, m, q, n);
	REAL_ARR_COPY(this->U, U, p + m + 2);
	REAL_ARR_COPY(this->V, V, q + n + 2);
	for (register int i = 0; i < m + 1; i++)
		REAL4_ARR_COPY(*(this->P[i]), *(P[i]), n + 1);
}
GNurbsSrf::GNurbsSrf(const GNurbsSrf* ptrSrf)
{
	this->id = -1;
	this->G3 = ptrSrf->G3;
	this->Active = ptrSrf->Active;
	short p = ptrSrf->GetDegU();		short q = ptrSrf->GetDegV();
	short m = ptrSrf->GetCtlPtIdxU();	short n = ptrSrf->GetCtlPtIdxV();
	REAL* U = ptrSrf->GetKnotU();		REAL* V = ptrSrf->GetKnotV();
	REAL4** P = ptrSrf->GetCtlPtNet();
	_alloc(p, m, q, n);
	REAL_ARR_COPY(this->U, U, p + m + 2);
	REAL_ARR_COPY(this->V, V, q + n + 2);
	for (register int i = 0; i < m + 1; i++)
		REAL4_ARR_COPY(*(this->P[i]), *(P[i]), n + 1);
}

GNurbsSrf::~GNurbsSrf() { _free(); }
//////////////////////////////////////////////////////////////////////
// operator overloading.
GNurbsSrf& GNurbsSrf::operator=(GNurbsSrf& srf)
{
	_free();
	this->id = -1;
	this->G3 = srf.G3;
	this->Active = srf.Active;
	short p = srf.GetDegU();		short q = srf.GetDegV();
	short m = srf.GetCtlPtIdxU();	short n = srf.GetCtlPtIdxV();
	REAL* U = srf.GetKnotU();		REAL* V = srf.GetKnotV();
	REAL4** P = srf.GetCtlPtNet();
	_alloc(p, m, q, n);
	REAL_ARR_COPY(this->U, U, p + m + 2);
	REAL_ARR_COPY(this->V, V, q + n + 2);
	for (register int i = 0; i < m + 1; i++)
		REAL4_ARR_COPY(*(this->P[i]), *(P[i]), n + 1);
	return (*this);
}
//////////////////////////////////////////////////////////////////////
// for internal usages
void GNurbsSrf::GetDerivCtlPt(short kth, short lth, REAL4** ctlpt)
{
	// Caution : Can be used only in the case of all weights are 1.0 !!!
	register int i, j;
	REAL4*** derctlpt_v = new REAL4**[m + 1];
	for (i = 0; i < m + 1; i++)
	{
		derctlpt_v[i] = new REAL4*[q + 1];
		for (j = 0; j < q + 1; j++)
			derctlpt_v[i][j] = new REAL4[n + 1];
	}
	REAL4*** derctlpt_u = new REAL4**[n - lth + 1];
	for (i = 0; i < n - lth + 1; i++)
	{
		derctlpt_u[i] = new REAL4*[p + 1];
		for (j = 0; j < p + 1; j++)
			derctlpt_u[i][j] = new REAL4[m + 1];
	}
	REAL4** tmp = new REAL4*[n - lth + 1];
	for (i = 0; i < n - lth + 1; i++)
		tmp[i] = new REAL4[m + 1];

	for (i = 0; i < m + 1; i++)
		::getNurbsDerivCtlPt(q, n, &P[i][0], V, derctlpt_v[i], G3);

	for (i = 0; i < n - lth + 1; i++)
	{
		for (j = 0; j < m + 1; j++)
		{
			tmp[i][j][0] = derctlpt_v[j][lth][i][0];
			tmp[i][j][1] = derctlpt_v[j][lth][i][1];
			tmp[i][j][2] = derctlpt_v[j][lth][i][2];
			tmp[i][j][3] = derctlpt_v[j][lth][i][3];
		}
	}
	for (i = 0; i < n - lth + 1; i++)
		::getNurbsDerivCtlPt(p, m, tmp[i], U, derctlpt_u[i], G3);

	for (i = 0; i < n - lth + 1; i++)
	{
		for (j = 0; j < m - kth + 1; j++)
		{
			ctlpt[j][i][0] = derctlpt_u[i][kth][j][0];
			ctlpt[j][i][1] = derctlpt_u[i][kth][j][1];
			ctlpt[j][i][2] = derctlpt_u[i][kth][j][2];
			ctlpt[j][i][3] = derctlpt_u[i][kth][j][3];
		}
	}
	for (i = 0; i < m + 1; i++)
	{
		for (j = 0; j < q + 1; j++)
		{
			delete[] derctlpt_v[i][j];
		}
		delete[] derctlpt_v[i];
	}
	delete derctlpt_v;
	for (i = 0; i < n - lth + 1; i++)
	{
		for (j = 0; j < p + 1; j++)
		{
			delete[] derctlpt_u[i][j];
		}
		delete[] derctlpt_u[i];
	}
	delete derctlpt_u;
	for (i = 0; i < n - lth + 1; i++)
		delete[] tmp[i];
	delete tmp;
}
void GNurbsSrf::ApplyWeight()
{
	register int i, j;
	for (i = 0; i < m + 1; i++)
		for (j = 0; j < n + 1; j++)
		{
			P[i][j][0] *= P[i][j][3];
			P[i][j][1] *= P[i][j][3];
			P[i][j][2] *= P[i][j][3];
		}
}
void GNurbsSrf::ApplyUnWeight()
{
	register int i, j;
	for (i = 0; i < m + 1; i++)
		for (j = 0; j < n + 1; j++)
		{
			P[i][j][0] /= P[i][j][3];
			P[i][j][1] /= P[i][j][3];
			P[i][j][2] /= P[i][j][3];
		}
}
/****************************************************************
 * Evaluation function for NurbsSrf
 ****************************************************************/
void GNurbsSrf::Eval(REAL u, REAL v, REAL4 pt, bool byctlpt)
{

	/* variable declaration. */
	register int i, j;
	short uIdx, vIdx;
	REAL* uBasis, *vBasis;

	REAL_ARR_ZERO(pt, 4);
	uBasis = new REAL[p + 1];
	vBasis = new REAL[q + 1];
	FindKnotSpan(u, v, uIdx, vIdx);
	GetBasis(u, v, uIdx, vIdx, uBasis, vBasis);

	if (G3)
		ApplyWeight(); // case of considering weights.
	for (i = 0; i <= q; i++)
	{
		REAL4 tmp;
		REAL_ARR_ZERO(tmp, 4);
		for (j = 0; j <= p; j++)
		{
			tmp[0] += uBasis[j] * P[uIdx - p + j][vIdx - q + i][0]; // wx
			tmp[1] += uBasis[j] * P[uIdx - p + j][vIdx - q + i][1]; // wy
			tmp[2] += uBasis[j] * P[uIdx - p + j][vIdx - q + i][2]; // wz
			tmp[3] += uBasis[j] * P[uIdx - p + j][vIdx - q + i][3]; // w
		}
		pt[0] += vBasis[i] * tmp[0]; //wx
		pt[1] += vBasis[i] * tmp[1]; //wy
		pt[2] += vBasis[i] * tmp[2]; //wz
		pt[3] += vBasis[i] * tmp[3]; //w
	}
	if (G3)
	{
		::poly2rational(pt);
		ApplyUnWeight();
	}
	delete[] uBasis;
	delete[] vBasis;
}
void GNurbsSrf::EvalDeriv(REAL u, REAL v, short kth, short lth, REAL4 pt)
{
	register int i, j, k, l;
	short uIdx, vIdx, d = kth + lth;

	REAL_ARR_ZERO(pt, 4);
	assert((kth <= p) && (lth <= q));

	/* memory allocation */
	REAL** uDerBasis = new REAL*[p + 1];
	REAL** vDerBasis = new REAL*[q + 1];
	REAL4** Aders = new REAL4*[d + 1];
	REAL4** SKL = new REAL4*[d + 1];
	short** Bin = new short*[d + 1];
	for (j = 0; j <= p; uDerBasis[j] = new REAL[p + 1], j++);
	for (j = 0; j <= q; vDerBasis[j] = new REAL[q + 1], j++);
	for (j = 0; j <= d; Aders[j] = new REAL4[d + 1], j++);
	for (j = 0; j <= d; SKL[j] = new REAL4[d + 1], j++);
	for (i = 0; i < d + 1; Bin[i] = new short[d + 1], i++);

	FindKnotSpan(u, v, uIdx, vIdx);
	GetDerivBasis(u, v, uIdx, vIdx, uDerBasis, vDerBasis);

	if (G3)
	{
		ApplyWeight();// the case of considering weights.
		::getBinomialCoeff(d, Bin);
		for (i = 0; i <= d; i++)
		{
			for (j = 0; j <= d; j++)
			{
				REAL_ARR_ZERO(Aders[i][j], 4);
				if (i > p || j > q)
					continue;
				for (k = 0; k <= q; k++)
				{
					REAL4 tmp;
					REAL_ARR_ZERO(tmp, 4);
					for (l = 0; l <= p; l++)
					{
						tmp[0] += uDerBasis[i][l] * P[uIdx - p + l][vIdx - q + k][0];
						tmp[1] += uDerBasis[i][l] * P[uIdx - p + l][vIdx - q + k][1];
						tmp[2] += uDerBasis[i][l] * P[uIdx - p + l][vIdx - q + k][2];
						tmp[3] += uDerBasis[i][l] * P[uIdx - p + l][vIdx - q + k][3];
					}
					Aders[i][j][0] += vDerBasis[j][k] * tmp[0];
					Aders[i][j][1] += vDerBasis[j][k] * tmp[1];
					Aders[i][j][2] += vDerBasis[j][k] * tmp[2];
					Aders[i][j][3] += vDerBasis[j][k] * tmp[3];
				}
			}
		}
		for (k = 0; k <= d; k++)
		{
			REAL3 v;
			for (l = 0; l <= d - k; l++)
			{
				REAL_ARR_COPY(v, Aders[k][l], 3);
				for (j = 1; j <= l; j++)
				{
					v[0] = v[0] - (REAL)Bin[l][j] * Aders[0][j][3] * SKL[k][l - j][0];
					v[1] = v[1] - (REAL)Bin[l][j] * Aders[0][j][3] * SKL[k][l - j][1];
					v[2] = v[2] - (REAL)Bin[l][j] * Aders[0][j][3] * SKL[k][l - j][2];
				}
				for (i = 1; i <= k; i++)
				{
					v[0] = v[0] - (REAL)Bin[k][i] * Aders[i][0][3] * SKL[k - i][l][0];
					v[1] = v[1] - (REAL)Bin[k][i] * Aders[i][0][3] * SKL[k - i][l][1];
					v[2] = v[2] - (REAL)Bin[k][i] * Aders[i][0][3] * SKL[k - i][l][2];
					REAL3 v2;
					REAL_ARR_ZERO(v2, 3);
					for (j = 1; j <= l; j++)
					{
						v2[0] = v2[0] + (REAL)Bin[l][j] * Aders[i][j][3] * SKL[k - i][l - j][0];
						v2[1] = v2[1] + (REAL)Bin[l][j] * Aders[i][j][3] * SKL[k - i][l - j][1];
						v2[2] = v2[2] + (REAL)Bin[l][j] * Aders[i][j][3] * SKL[k - i][l - j][2];
					}
					v[0] = v[0] - (REAL)Bin[k][i] * v2[0];
					v[1] = v[1] - (REAL)Bin[k][i] * v2[1];
					v[2] = v[2] - (REAL)Bin[k][i] * v2[2];
				}
				if (EQ_ZERO(v[0]))	v[0] = 0.0;
				if (EQ_ZERO(v[1]))	v[1] = 0.0;
				if (EQ_ZERO(v[2]))	v[2] = 0.0;
				SKL[k][l][0] = v[0] / Aders[0][0][3];
				SKL[k][l][1] = v[1] / Aders[0][0][3];
				SKL[k][l][2] = v[2] / Aders[0][0][3];
				SKL[k][l][3] = 1.0f;
			}
		}
		REAL_ARR_COPY(pt, SKL[kth][lth], 4);
		ApplyUnWeight();
	}
	else
	{
		for (i = 0; i <= q; i++)
		{
			REAL4 tmp;
			REAL_ARR_ZERO(tmp, 4);
			for (j = 0; j <= p; j++)
			{
				tmp[0] += uDerBasis[kth][j] * P[uIdx - p + j][vIdx - q + i][0];
				tmp[1] += uDerBasis[kth][j] * P[uIdx - p + j][vIdx - q + i][1];
				tmp[2] += uDerBasis[kth][j] * P[uIdx - p + j][vIdx - q + i][2];
				tmp[3] += uDerBasis[kth][j] * P[uIdx - p + j][vIdx - q + i][3];
			}
			pt[0] += vDerBasis[lth][i] * tmp[0];
			pt[1] += vDerBasis[lth][i] * tmp[1];
			pt[2] += vDerBasis[lth][i] * tmp[2];
			pt[3] += vDerBasis[lth][i] * tmp[3];
		}
	}

	/* memory free */
	for (i = 0; i <= p; delete[] uDerBasis[i], i++);
	for (j = 0; j <= q; delete[] vDerBasis[j], j++);
	for (j = 0; j <= d; delete[] Aders[j], j++);
	for (j = 0; j <= d; delete[] SKL[j], j++);
	for (j = 0; j <= d; delete[] Bin[j], j++);
	delete uDerBasis;
	delete vDerBasis;
	delete Aders;
	delete SKL;
	delete Bin;
}
/*********************************************************************/
/* Utilities for NurbsSrf                                            */
/*********************************************************************/
GNurbsSrf GNurbsSrf::GetDerivSrf(short kth, short lth)
{
	// Caution : Can be used only in the case of all weights are 1.0 !!!
	assert(kth < p + 1);
	assert(lth < q + 1);

	register int i;
	REAL4** ctlpt = new REAL4*[m - kth + 1];
	for (i = 0; i < m - kth + 1; i++)
		ctlpt[i] = new REAL4[n - lth + 1];
	REAL* uknot = new REAL[m + p + 2 * (1 - kth)];
	REAL* vknot = new REAL[n + q + 2 * (1 - lth)];

	GetDerivCtlPt(kth, lth, ctlpt);

	for (i = 0; i < m + p + 2 * (1 - kth); i++)
		uknot[i] = this->U[i + kth];

	for (i = 0; i < n + q + 2 * (1 - lth); i++)
		vknot[i] = this->U[i + lth];

	GNurbsSrf retSrf(p - kth, m - kth, uknot, q - lth, n - lth, vknot, ctlpt);

	delete[] uknot;
	delete[] vknot;
	for (i = 0; i < m - kth + 1; i++)
		delete[] ctlpt[i];
	delete ctlpt;
	return retSrf;

}
/*********************************************************************/
/* Fundamental Geometric Algorithms For NurbsCrv                     */
/*********************************************************************/
void GNurbsSrf::InsertMultiKnot(short dir, REAL uv, short r)
{
	// NurbsSrf is defined as follows.
	// p  : degree of u direction.
	// U  : knot vector of u direction.
	// m  : m+1 is # of control points of u direction.
	// ur : ur+1 is # of knots of u direction.

	// q  : degree of v direction.
	// V  : knot vector of v direction.
	// n  : n+1 is # of control points of v direction.
	// vs : vs+1 is # of knot of v dirction.

	// P : control point net. (m+1) x (n+1)
	register int i, j, k;

	if (dir == U_DIR)
	{
		short idx, s;
		short ur, vs, r1, m1, L;
		idx = ::findKnotSpan(U, p, m, uv); // knot span idex.
		s = ::findKnotMult(U, p, m, uv); // knot multiplicity.
		assert(r + s <= p); // pre - condition.

		ur = m + p + 1; // ur+1 is # of old u direction knot.
		vs = n + q + 1; // vs+1 is # of old v direction knot.
		m1 = m + r;  // m1+1 is # of new control points.
		r1 = ur + r; // r1+1 is # of new knot.

		// calculate the inserted knot vector and control point which is cutted corner.
		REAL4** P1 = new REAL4*[m1 + 1]; // new control points.
		for (i = 0; i < m1 + 1; P1[i] = new REAL4[n + 1], i++);

		REAL* U1 = new REAL[r1 + 1]; // new knot vectors of u direction.
		REAL* V1 = new REAL[vs + 1]; // new kont vectors of v direction.

		REAL4* R = new REAL4[p + 1]; // temporary control points.

		REAL** alpha = new REAL*[p];
		for (i = 0; i < p; alpha[i] = new REAL[r + 1], i++);

		// load new knot vectors.
		for (i = 0; i <= idx; U1[i] = U[i], i++);
		for (i = 1; i <= r; U1[idx + i] = uv, i++);
		for (i = idx + 1; i <= ur; U1[i + r] = U[i], i++);
		REAL_ARR_COPY(V1, V, vs + 1);

		// save the alphas
		for (j = 1; j <= r; j++)
		{
			L = idx - p + j;
			for (i = 0; i <= p - j - s; i++)
				alpha[i][j] = (uv - U[L + i]) / (U[i + idx + 1] - U[L + i]);
		}

		if (G3) ApplyWeight();  // the case of considering weight 
									  // i.e rational form
		for (k = 0; k <= n; k++)
		{
			// save unaltered control points
			for (i = 0; i <= idx - p; REAL_ARR_COPY(P1[i][k], P[i][k], 4), i++);
			for (i = idx - s; i <= m; REAL_ARR_COPY(P1[i + r][k], P[i][k], 4), i++);


			for (i = 0; i <= p - s; i++)
				REAL_ARR_COPY(R[i], P[idx - p + i][k], 4);

			// knot insert process.
			for (j = 1; j <= r; j++)
			{
				L = idx - p + j;
				for (i = 0; i <= p - j - s; i++)
				{
					R[i][0] = alpha[i][j] * R[i + 1][0] + (1.0f - alpha[i][j]) * R[i][0];
					R[i][1] = alpha[i][j] * R[i + 1][1] + (1.0f - alpha[i][j]) * R[i][1];
					R[i][2] = alpha[i][j] * R[i + 1][2] + (1.0f - alpha[i][j]) * R[i][2];
					R[i][3] = alpha[i][j] * R[i + 1][3] + (1.0f - alpha[i][j]) * R[i][3];
				}
				P1[L][k][0] = R[0][0];
				P1[L][k][1] = R[0][1];
				P1[L][k][2] = R[0][2];
				P1[L][k][3] = R[0][3];

				P1[idx + r - j - s][k][0] = R[p - j - s][0];
				P1[idx + r - j - s][k][1] = R[p - j - s][1];
				P1[idx + r - j - s][k][2] = R[p - j - s][2];
				P1[idx + r - j - s][k][3] = R[p - j - s][3];
			}
			for (i = L + 1; i < idx - s; i++)
			{
				P1[i][k][0] = R[i - L][0];
				P1[i][k][1] = R[i - L][1];
				P1[i][k][2] = R[i - L][2];
				P1[i][k][3] = R[i - L][3];
			}
		}

		_free();
		_alloc(p, m1, q, n);
		REAL_ARR_COPY(this->U, U1, r1 + 1);
		REAL_ARR_COPY(this->V, V1, vs + 1);
		for (i = 0; i < m1 + 1; REAL4_ARR_COPY(this->P[i], P1[i], n + 1), i++);
		if (G3) ApplyUnWeight();

		// memory free.
		for (i = 0; i < m1 + 1; delete[] P1[i], i++);
		for (i = 0; i < p; delete[] alpha[i], i++);
		delete P1;
		delete alpha;
		delete[] R;
		delete[] U1;
		delete[] V1;
	}
	else if (dir == V_DIR)
	{
		short idx, s;
		short ur, vs, s1, n1, L;
		idx = ::findKnotSpan(V, q, n, uv); // knot span idex.
		s = ::findKnotMult(V, q, n, uv); // knot multiplicity.
		assert(r + s <= q); // pre - condition.

		ur = m + p + 1; // ur+1 is # of old u direction knot.
		vs = n + q + 1; // vs+1 is # of old v direction knot.
		n1 = n + r;  // n1+1 is # of new control points.
		s1 = vs + r; // s1+1 is # of new knot.

		// calculate the inserted knot vector and control point which is cutted corner.
		REAL4** P1 = new REAL4*[m + 1]; // new control points.
		for (i = 0; i < m + 1; P1[i] = new REAL4[n1 + 1], i++);

		REAL* U1 = new REAL[ur + 1]; // new knot vectors of u direction.
		REAL* V1 = new REAL[s1 + 1]; // new kont vectors of v direction.

		REAL4* R = new REAL4[q + 1]; // temporary control points.

		REAL** alpha = new REAL*[q];
		for (i = 0; i < q; alpha[i] = new REAL[r + 1], i++);

		// load new knot vectors.
		for (i = 0; i <= idx; V1[i] = V[i], i++);
		for (i = 1; i <= r; V1[idx + i] = uv, i++);
		for (i = idx + 1; i <= vs; V1[i + r] = V[i], i++);
		REAL_ARR_COPY(U1, U, ur + 1);

		// save the alphas
		for (j = 1; j <= r; j++)
		{
			L = idx - q + j;
			for (i = 0; i <= q - j - s; i++)
				alpha[i][j] = (uv - V[L + i]) / (V[i + idx + 1] - V[L + i]);
		}

		if (G3) ApplyWeight(); // the case of considering weight 
									 // i.e rational form
		for (k = 0; k <= m; k++)
		{
			// save unaltered control points
			for (i = 0; i <= idx - q; REAL_ARR_COPY(P1[k][i], P[k][i], 4), i++);
			for (i = idx - s; i <= n; REAL_ARR_COPY(P1[k][i + r], P[k][i], 4), i++);


			for (i = 0; i <= q - s; i++)
				REAL_ARR_COPY(R[i], P[k][idx - q + i], 4);

			// knot insert process.
			for (j = 1; j <= r; j++)
			{
				L = idx - q + j;
				for (i = 0; i <= q - j - s; i++)
				{
					R[i][0] = alpha[i][j] * R[i + 1][0] + (1.0f - alpha[i][j]) * R[i][0];
					R[i][1] = alpha[i][j] * R[i + 1][1] + (1.0f - alpha[i][j]) * R[i][1];
					R[i][2] = alpha[i][j] * R[i + 1][2] + (1.0f - alpha[i][j]) * R[i][2];
					R[i][3] = alpha[i][j] * R[i + 1][3] + (1.0f - alpha[i][j]) * R[i][3];
				}
				P1[k][L][0] = R[0][0];
				P1[k][L][1] = R[0][1];
				P1[k][L][2] = R[0][2];
				P1[k][L][3] = R[0][3];

				P1[k][idx + r - j - s][0] = R[q - j - s][0];
				P1[k][idx + r - j - s][1] = R[q - j - s][1];
				P1[k][idx + r - j - s][2] = R[q - j - s][2];
				P1[k][idx + r - j - s][3] = R[q - j - s][3];
			}
			for (i = L + 1; i < idx - s; i++)
			{
				P1[k][i][0] = R[i - L][0];
				P1[k][i][1] = R[i - L][1];
				P1[k][i][2] = R[i - L][2];
				P1[k][i][3] = R[i - L][3];
			}
		}

		_free();
		_alloc(p, m, q, n1);
		REAL_ARR_COPY(this->U, U1, ur + 1);
		REAL_ARR_COPY(this->V, V1, s1 + 1);
		for (i = 0; i < m + 1; REAL4_ARR_COPY(this->P[i], P1[i], n1 + 1), i++);
		if (G3) ApplyUnWeight();

		// memory free.
		for (i = 0; i < m + 1; delete[] P1[i], i++);
		for (i = 0; i < q; delete[] alpha[i], i++);
		delete P1;
		delete alpha;
		delete[] R;
		delete[] U1;
		delete[] V1;
	}

}
void GNurbsSrf::Edit(short uIdx, short vIdx, REAL dx, REAL dy, REAL dz, REAL alpha)
{
	assert(uIdx >= 0);
	assert(uIdx <= m);
	assert(vIdx >= 0);
	assert(vIdx <= n);
	P[uIdx][vIdx][0] += (alpha*dx);
	P[uIdx][vIdx][1] += (alpha*dy);
	P[uIdx][vIdx][2] += (alpha*dz);
}
void GNurbsSrf::Edit(short uIdx, short vIdx, REAL3 Offset, REAL alpha)
{
	Edit(uIdx, vIdx, Offset[0], Offset[1], Offset[2], alpha);
}
void GNurbsSrf::Edit(REAL u, REAL v, REAL dx, REAL dy, REAL dz, REAL alpha)
{
	// loop idx.
	int i, j;

	// get nodes.
	REAL *node_u = new REAL[m + 1];
	REAL *node_v = new REAL[n + 1];
	REAL_ARR_ZERO(node_u, m + 1);
	REAL_ARR_ZERO(node_v, n + 1);
	for (i = 0; i < m + 1; i++)
	{
		REAL sum = 0.0f;
		for (j = 1; j < p + 1; j++)
			sum += U[i + j];
		node_u[i] = sum / p;
	}
	for (i = 0; i < n + 1; i++)
	{
		REAL sum = 0.0f;
		for (j = 1; j < q + 1; j++)
			sum += V[i + j];
		node_v[i] = sum / q;
	}

	// get index k,l, P[k][l] is translated to produce the desired movement.
	int k = 0, l = 0;
	REAL min = 10000000.0f;
	for (i = 0; i < m + 1; i++)
	{
		REAL tmp = (float)fabs(u - node_u[i]);
		if (tmp < min)
		{
			k = i;
			min = tmp;
		}
	}
	min = 1000000.0f;
	for (i = 0; i < n + 1; i++)
	{
		REAL tmp = (float)fabs(v - node_v[i]);
		if (tmp < min)
		{
			l = i;
			min = tmp;
		}
	}

	short uIdx, vIdx;
	FindKnotSpan(u, v, uIdx, vIdx);
	REAL* uBasis = new REAL[p + 1];
	REAL* vBasis = new REAL[q + 1];
	GetBasis(u, v, uIdx, vIdx, uBasis, vBasis);

	REAL Rmax_U = -10000.0f;
	REAL Rmax_V = -10000.0f;
	REAL w = 0.0f;

	for (i = 0; i < p + 1; i++)
		Rmax_U = MAX(uBasis[i], Rmax_U);
	for (i = 0; i < q + 1; i++)
		Rmax_V = MAX(vBasis[i], Rmax_V);
	for (i = 0; i < q + 1; i++)
	{
		REAL tmp = 0.0f;
		for (j = 0; j < p + 1; j++)
			tmp += uBasis[j] * P[uIdx - p + j][vIdx - q + i][3]; // w
		w += vBasis[i] * tmp; //w
	}
	REAL alp = Rmax_U * Rmax_V / w;
	alp = 1.0f / alp;

	P[k][l][0] = P[k][l][0] + alp * dx;
	P[k][l][1] = P[k][l][1] + alp * dy;
	P[k][l][2] = P[k][l][2] + alp * dz;


	// memory free
	delete[] node_u;
	delete[] node_v;
	delete[] uBasis;
	delete[] vBasis;
}
void GNurbsSrf::Edit(REAL u, REAL v, REAL3 Offset, REAL alpha)
{
	Edit(u, v, Offset[0], Offset[1], Offset[2], alpha);
}
void GNurbsSrf::MakeBzrSegment(REAL u, REAL v)
{
	short ur, vr;
	ur = p - FindKnotMultU(u);
	vr = q - FindKnotMultV(v);
	if (ur != 0)
		InsertMultiKnot(U_DIR, u, ur);
	if (vr != 0)
		InsertMultiKnot(V_DIR, v, vr);
}
void GNurbsSrf::MakeBzrSegment()
{
	int i;
	short r = p + m + 1;
	short s = q + n + 1;
	REAL *uKnot = new REAL[r + 1];
	REAL *vKnot = new REAL[s + 1];
	REAL_ARR_COPY(uKnot, U, r + 1);
	REAL_ARR_COPY(vKnot, V, s + 1);
	for (i = p + 1; i < r - p; i++)
	{
		REAL u = uKnot[i];
		short rtimes = p - FindKnotMultU(u);
		if (rtimes != 0)
			InsertMultiKnot(U_DIR, u, rtimes);
	}
	for (i = q + 1; i < s - q; i++)
	{
		REAL v = vKnot[i];
		short rtimes = q - FindKnotMultV(v);
		if (rtimes != 0)
			InsertMultiKnot(V_DIR, v, rtimes);
	}
	delete[] uKnot;
	delete[] vKnot;
}
// 2002. 11. 25.
bool GNurbsSrf::IsBzrSegment()
{
	register int i;
	short m = GetKnotIdxU();
	short n = GetKnotIdxV();
	for (i = 0; i < m + 1; i++)
	{
		if (FindKnotMultU(U[i]) < p)
			return false;
	}
	for (i = 0; i < n + 1; i++)
	{
		if (FindKnotMultV(V[i]) < q)
			return false;
	}
	return true;
}
GNurbsSrf GNurbsSrf::GetBzrSegment(int row, int col, bool scale)
{
	register int i, j;
	if (!IsBzrSegment())
		MakeBzrSegment();

	int r = p + m + 1;
	int s = q + n + 1;

	assert(row < (r / p));
	assert(col < (s / q));

	REAL *nU = new REAL[2 * p + 2];
	REAL *nV = new REAL[2 * q + 2];
	REAL4 **CtlPt = new REAL4*[p + 1];
	for (i = 0; i < p + 1; CtlPt[i] = new REAL4[q + 1], i++);


	// 2002. 11. 25.
	// set nU kont vector.
	for (i = 0; i < p + 1; i++)
	{
		if (scale)
			nU[i] = 0.0f;
		else
			nU[i] = U[row*p + 1];
	}
	for (i = p + 1; i < 2 * p + 2; i++)
	{
		if (scale)
			nU[i] = 1.0f;
		else
			nU[i] = U[(row + 1)*p + 1];
	}
	// set nV kont vector.
	for (i = 0; i < q + 1; i++)
	{
		if (scale)
			nV[i] = 0.0f;
		else
			nV[i] = V[col*q + 1];
	}
	for (i = q + 1; i < 2 * q + 2; i++)
	{
		if (scale)
			nV[i] = 1.0f;
		else
			nV[i] = V[(row + 1)*q + 1];
	}
	for (i = 0; i < p + 1; i++)
		for (j = 0; j < q + 1; j++)
		{
			CtlPt[i][j][0] = P[row*p + i][col*q + j][0];
			CtlPt[i][j][1] = P[row*p + i][col*q + j][1];
			CtlPt[i][j][2] = P[row*p + i][col*q + j][2];
			CtlPt[i][j][3] = P[row*p + i][col*q + j][3];
		}

	// create surface.
	GNurbsSrf srf(p, p, nU, q, q, nV, CtlPt, G3);
	delete[] nU;
	delete[] nV;
	for (i = 0; i < p + 1; delete[] CtlPt[i], i++);
	delete CtlPt;
	return srf;

}
void GNurbsSrf::RefineKnotVector(short dir, REAL* X, short r)
{
	// input parameter 
	// X : new knot vector that is inserted into U or V
	// r : r+1 is # of X's knot
	/*
	short ur;
	short a, b;
	register i, row;
	if ( dir == U_DIR )
	{
		ur = p+m+1; // ur+1 is # of u direction knot vectors
		a = ::findKnotSpan( U, p, m, X[0] );
		b = ::findKnotSpan( U, p, m, X[r] );
		b++;

		// nU is new knot vector
		REAL* nU = new REAL[ur+r+2];
		// initialize nU knot vector
		for ( i = 0; i <= a; nU[i]=U[i], i++ );
		for ( i = b+p; i <= ur; nU[i+r+1]=U[i], i++ );

		// nP is new control point net.
		REAL4** nP = new REAL4* [m+r+2];
		for ( i = 0; i < m+r+2; nP[i] = new REAL4[n+1], i++ );

		// save unaltered control points
		for ( row = 0; row <= n; row++ )
		{
			for ( k = 0; k <= a-p; k++ )
				REAL4_ARR_COPY( , P[k][row] );
			for ( k = b-1; k <= n; k++ )
				REAL4_ARR_COPY( , P[k][row] );
		}
		for ( i = r; i >= 0; i-- )
		{
			while( X[i] <= U[i

	}
	else
	{
	}
	*/

}


short	GNurbsSrf::GetDegU() const { return p; };
short	GNurbsSrf::GetDegV() const { return q; };
short	GNurbsSrf::GetCtlPtIdxU() const { return m; };
short	GNurbsSrf::GetCtlPtIdxV() const { return n; };
short	GNurbsSrf::GetKnotIdxU() const { return (m + p + 1); };
short	GNurbsSrf::GetKnotIdxV() const { return (n + q + 1); };
REAL*	GNurbsSrf::GetKnotU() const { return U; };
REAL*	GNurbsSrf::GetKnotV() const { return V; };
REAL	GNurbsSrf::GetMinKnotU() const { return U[0]; };
REAL	GNurbsSrf::GetMaxKnotU() const { return U[m + p + 1]; };
REAL	GNurbsSrf::GetMinKnotV() const { return V[0]; };
REAL	GNurbsSrf::GetMaxKnotV() const { return V[n + q + 1]; };
REAL4** GNurbsSrf::GetCtlPtNet() const { return P; };
void	GNurbsSrf::GetCtlPt(short i, short j, REAL4 pt) const { REAL_ARR_COPY(pt, P[i][j], 4); };
short	GNurbsSrf::FindKnotMultU(REAL u) { return ::findKnotMult(U, p, m, u); };
short	GNurbsSrf::FindKnotMultV(REAL v) { return ::findKnotMult(V, q, n, v); };

GNurbsCrv * GNurbsSrf::GetProCrv() { return NULL; };
GNurbsSrf * GNurbsSrf::GetContSrf() { return NULL; };
GNurbsCrv * GNurbsSrf::GetTraCrv() { return NULL; };
GNurbsCrv * GNurbsSrf::GetRotCrv() { return NULL; };
GNurbsCrv * GNurbsSrf::GetSclCrv() { return NULL; };

void	GNurbsSrf::FindKnotSpan(REAL u, REAL v, short& uIdx, short& vIdx)
{
	uIdx = ::findKnotSpan(U, p, m, u); 	vIdx = ::findKnotSpan(V, q, n, v);
};
void	GNurbsSrf::GetBasis(REAL u, REAL v, short uIdx, short vIdx, REAL* uBasis, REAL* vBasis)
{
	::getNurbsBasis(U, p, uIdx, u, uBasis);
	::getNurbsBasis(V, q, vIdx, v, vBasis);
};
void	GNurbsSrf::GetDerivBasis(REAL u, REAL v, short uIdx, short vIdx, REAL** uDerivBasis, REAL** vDerivBasis)
{
	::getNurbsDerivBasis(U, p, uIdx, u, uDerivBasis);
	::getNurbsDerivBasis(V, q, vIdx, v, vDerivBasis);
};
