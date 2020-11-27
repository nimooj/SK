#include "pch.h"
#include "..\usr\include\GNurbs\GNurbs.h"

bool GNurbsMot::_alloc()
{
	/*****************************************************************
	*	1. convert translational curve into piecewise bezier curve.
	*	2. convert rotational curve into piecewise bezier curve.
	*	3. set up motion knot vector.
	*****************************************************************/
	REAL* knot = 0;
	register int i, j;
	short kidx, m;

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

	this->p = T->GetDegree();
	if (R)
		this->p += 2 * R->GetDegree();
	if (S)
		this->p += S->GetDegree();

	m = p * (kidx + 1) + 1; // m+1 is # of motion knot vector;

	this->n = m - p - 1;
	this->P = new MATRIX[n + 1];

	MATRIX_ARR_ZERO(P, n + 1);
	this->U = new REAL[m + 1];
	for (i = 0; i < kidx + 1; i++)
		for (j = 1; j < p + 1; j++)
			U[i*p + j] = knot[i];
	U[0] = knot[0]; U[m] = knot[kidx];

	/* memeory free */
	delete[] knot;

	return true;
}
bool GNurbsMot::CalcCtlMatrix()
{
	T->MakeBzrSegment();
	if (R && (R->GetDegree() > 0))
		R->MakeBzrSegment();
	if (S && (S->GetDegree() > 0))
		S->MakeBzrSegment();


	int i, j, k, l, m, r, c;
	/* seg is number of piecewise motion */
	int seg = n / p;

	REAL coeff;
	MATRIX tmpMat;
	MATRIX_ZERO(tmpMat);
	REAL4 t, r0, r1, s;
	if (R)
	{
		if (R->GetDegree() == 0)
		{
			R->GetCtlPt(0, r0);
			R->GetCtlPt(0, r1);
		}
	}
	else
	{
		r0[0] = 0.0f; r0[1] = 0.0f; r0[2] = 0.0f; r0[3] = 1.0f;
		r1[0] = 0.0f; r1[1] = 0.0f; r1[2] = 0.0f; r1[3] = 1.0f;
	}
	if (S)
	{
		if (S->GetDegree() == 0)
			S->GetCtlPt(0, s);
	}
	else
	{
		s[0] = 1.0f; s[1] = 1.0f; s[2] = 1.0f; s[3] = 1.0f;
	}

	// memory allocation.
	short** BIN = new short*[p + 1];
	for (i = 0; i < p + 1; BIN[i] = new short[p + 1], i++);
	::getBinomialCoeff(p, BIN);

	short Tp = T->GetDegree();
	short Rp = 0; if (R) Rp = R->GetDegree();
	short Sp = 0; if (S) Sp = S->GetDegree();

	for (i = 0; i < seg; i++) // for each picec-wise motion segment...
	{
		GNurbsCrv bzrT = T->GetBzrSegment(i);
		GNurbsCrv bzrR;
		GNurbsCrv bzrS;
		if (R && (R->GetDegree() > 0))
			bzrR = R->GetBzrSegment(i);
		if (S && (S->GetDegree() > 0))
			bzrS = S->GetBzrSegment(i);
		MATRIX_ZERO(P[i*p]);
		for (j = 0; j <= Rp; j++)
			for (k = 0; k <= Rp; k++)
				for (l = 0; l <= Tp; l++)
					for (m = 0; m <= Sp; m++)
					{
						/* calculate coefficient */
						coeff = (REAL)BIN[Rp][j] * (REAL)BIN[Rp][k] * (REAL)BIN[Tp][l]
							* (REAL)BIN[Sp][m] / (REAL)BIN[p][j + k + l + m];

						/* get control point for each curve */
						bzrT.GetCtlPt(l, t);
						t[0] *= t[3]; t[1] *= t[3]; t[2] *= t[3];

						if (R && (R->GetDegree() > 0))
						{
							bzrR.GetCtlPt(j, r0);
							bzrR.GetCtlPt(k, r1);
						}

						/* apply weights to translation curve and scaling curve */
						if (S && (S->GetDegree() > 0))
						{
							bzrS.GetCtlPt(m, s);
							s[0] *= s[3]; s[1] *= s[3]; s[2] *= s[3];
						}

						/* set up motion matrix which is sumed up */
						::setMotionMatrix(t, r0, r1, s, tmpMat, false);

						for (r = 0; r < 4; r++)
							for (c = 0; c < 4; c++)
								P[(i*p) + j + k + l + m][r][c] += (coeff*tmpMat[r][c]);
					}
	}
	/* knot removal
	theoretically, multiplication of two B-spline curve is not continuous at ends of
	each piecewise bezier segments.
	so following routine is not useful.
	only a few knot is removed.
	for ( i = p+1; i < n; i++ )
	{
		while(1)
		{
			int n = RemoveMultiKnot( U[i] );
			if ( n == 0 )
				break;
		}
	}
	*/
	// memory free.
	for (i = 0; i < p + 1; delete[] BIN[i], i++);
	delete BIN;
	return true;
}
bool GNurbsMot::_free()
{
	if (T) { delete T; T = NULL; }
	if (R) { delete R; R = NULL; }
	if (S) { delete S; S = NULL; }
	if (P) { delete[] P; P = NULL; }
	if (U) { delete[] U; U = NULL; }
	return true;
}
///////////////////////////////////////////////////////////////////////////////
// constructors and destructor.
GNurbsMot::GNurbsMot()
{
	id = -1;
	p = 0;
	n = 0;
	P = NULL;
	U = NULL;
	T = NULL;
	R = NULL;
	S = NULL;
	Active = true;
}
GNurbsMot::GNurbsMot(GNurbsCrv* t, GNurbsCrv* r, GNurbsCrv* s)
{
	this->id = -1;
	this->T = new GNurbsCrv(t); // G3 translation curve.
	if (r)
		this->R = new GNurbsCrv(r); // quaternion curve.
	else
		R = NULL;
	if (s)
		this->S = new GNurbsCrv(s); // G3 scaling curve.
	else
		S = NULL;

	Active = true; // make state be active

	_alloc(); // set up motion knot vector.

	CalcCtlMatrix();

}
GNurbsMot::GNurbsMot(GNurbsMot& mot)
{
	this->id = -1;
	this->T = new GNurbsCrv(mot.T);
	this->R = new GNurbsCrv(mot.R);
	if (mot.S)
		this->S = new GNurbsCrv(mot.S);
	else
		S = NULL;
	this->p = mot.p;
	this->n = mot.n;
	this->U = new REAL[n + p + 2];
	this->P = new MATRIX[n + 1];
	this->Active = mot.Active;
	MATRIX_ARR_COPY(this->P, mot.P, n + 1);
	REAL_ARR_COPY(this->U, mot.U, GetKnotIdx() + 1);
}
GNurbsMot::GNurbsMot(GNurbsMot* pMot)
{
	this->id = -1;
	this->T = new GNurbsCrv(pMot->T);
	this->R = new GNurbsCrv(pMot->R);
	if (pMot->S)
		this->S = new GNurbsCrv(pMot->S);
	else
		S = NULL;
	this->p = pMot->p;
	this->n = pMot->n;
	this->U = new REAL[n + p + 2];
	this->P = new MATRIX[n + 1];
	this->Active = pMot->Active;
	MATRIX_ARR_COPY(this->P, pMot->P, n + 1);
	REAL_ARR_COPY(this->U, pMot->U, GetKnotIdx() + 1);
}

GNurbsMot::~GNurbsMot()
{
	_free();
}
/**********************************************************************
 *	Assignment Operator Overloading.
***********************************************************************/
GNurbsMot& GNurbsMot::operator=(GNurbsMot& mot)
{
	_free();
	this->id = -1;
	this->T = new GNurbsCrv(mot.T);
	this->R = new GNurbsCrv(mot.R);
	if (mot.S)
		this->S = new GNurbsCrv(mot.S);
	else
		S = NULL;
	this->p = mot.p;
	this->n = mot.n;
	this->U = new REAL[n + p + 2];
	this->P = new MATRIX[n + 1];
	this->Active = mot.Active;
	MATRIX_ARR_COPY(this->P, mot.P, n + 1);
	REAL_ARR_COPY(this->U, mot.U, GetKnotIdx() + 1);
	return(*this);
}
/**********************************************************************
 *	Evalution Function
***********************************************************************/
void GNurbsMot::Eval(REAL u, MATRIX mat, bool ByMat)
{
	short idx = FindKnotSpan(u);
	MATRIX_ZERO(mat);
	if (ByMat) // evaluate by control motion matrix.
	{
		register int i, j;
		REAL* basis = new REAL[p + 1];

		GetBasis(u, idx, basis);
		for (i = 0; i <= p; i++)
		{
			mat[0][0] += basis[i] * P[idx - p + i][0][0];
			mat[0][1] += basis[i] * P[idx - p + i][0][1];
			mat[0][2] += basis[i] * P[idx - p + i][0][2];
			mat[0][3] += basis[i] * P[idx - p + i][0][3];

			mat[1][0] += basis[i] * P[idx - p + i][1][0];
			mat[1][1] += basis[i] * P[idx - p + i][1][1];
			mat[1][2] += basis[i] * P[idx - p + i][1][2];
			mat[1][3] += basis[i] * P[idx - p + i][1][3];

			mat[2][0] += basis[i] * P[idx - p + i][2][0];
			mat[2][1] += basis[i] * P[idx - p + i][2][1];
			mat[2][2] += basis[i] * P[idx - p + i][2][2];
			mat[2][3] += basis[i] * P[idx - p + i][2][3];

			mat[3][0] += basis[i] * P[idx - p + i][3][0];
			mat[3][1] += basis[i] * P[idx - p + i][3][1];
			mat[3][2] += basis[i] * P[idx - p + i][3][2];
			mat[3][3] += basis[i] * P[idx - p + i][3][3];
		}

		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				mat[i][j] /= mat[3][3];
		delete[] basis;
	}
	else // evaluate by component structure ( 3-curves )
	{
		REAL4 tcp, scp, rcp;
		T->Eval(u, tcp);
		R->Eval(u, rcp);
		if (S)
			S->Eval(u, scp);
		else
		{
			scp[0] = 1.0; scp[1] = 1.0; scp[2] = 1.0; scp[3] = 1.0;
		}
		::setMotionMatrix(tcp, rcp, rcp, scp, mat, true);
	}

}
short GNurbsMot::RemoveMultiKnot(REAL u, short r)
{
	short idx = FindKnotSpan(u);
	short s = FindKnotMult(u);
	assert((0 <= r) && (r <= s));

	short scs = ::removeMotKnot(p, n, P, U, u, idx, s, r);
	short new_n = n - scs;
	MATRIX* nP = new MATRIX[new_n + 1];
	REAL* nU = new REAL[p + new_n + 2];

	MATRIX_ARR_COPY(nP, P, new_n + 1);
	REAL_ARR_COPY(nU, U, p + new_n + 2);

	n = new_n;
	delete[] P;
	delete[] U;
	P = new MATRIX[new_n + 1];
	U = new REAL[new_n + p + 2];
	MATRIX_ARR_COPY(P, nP, n + 1);
	REAL_ARR_COPY(U, nU, n + p + 2);
	delete[] nP;
	delete[] nU;

	return scs;
}


