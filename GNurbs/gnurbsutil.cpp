/*******************************************************************************/
/*	nurbsbasis.cpp file                                                        */
/*******************************************************************************/
#include "pch.h"
#include "..\usr\include\GNurbs\GNurbs.h"

/********************************************************************************/
/* Functin : bool checkInDomain( REAL* knot, short p, short n, REAL& u )         */
/* Description : check if u is in domain or not                                 */
/* Parameter   : knot : knot vectors                                            */
/*				 n    : n+1 is # of control points                              */
/*				 p    : degree                                                  */
/*				 u    : parameter value                                         */
/********************************************************************************/
bool checkInDomain(REAL* knot, short p, short n, REAL& u)
{
	REAL umin = knot[p], umax = knot[n + 1];
	if (EQ(u, umin)) u = umin;
	if (EQ(u, umax)) u = umax;
	if (u < umin)
	{
		u = umin;
		return false;
	}
	if (u > umax)
	{
		u = umax;
		return false;
	}
	return true;
}
/********************************************************************************/
/* Functin : short findKnotSpan( REAL* knot, short p, short n, REAL u )         */
/* Description : return i such that knot[i] <= u < knot[i+1]                    */
/* Parameter:    knot : knot vectors                                            */
/*				 n    : n+1 is # of control points                              */
/*				 p    : degree                                                  */
/*				 u    : parameter value                                         */
/********************************************************************************/
short findKnotSpan(REAL* knot, short p, short n, REAL u)
{
	::checkInDomain(knot, p, n, u);
	short low, high, mid;
	if (u == knot[n + 1])
		return n;
	low = p;
	high = n + 1;
	mid = (low + high) / 2;
	while (u < knot[mid] || u >= knot[mid + 1])
	{
		if (u < knot[mid])
			high = mid;
		else
			low = mid;
		mid = (low + high) / 2;
	}
	return mid;
}
/********************************************************************************/
/* Functin : short findKnotMult( REAL* knot, short p, short n, REAL u )         */
/* Description : return multiplicity of u                                       */
/* Parameter:    knot : knot vectors                                            */
/*				 n    : n+1 is # of control points                              */
/*				 p    : degree                                                  */
/*				 u    : parameter value                                         */
/********************************************************************************/
short findKnotMult(REAL* knot, short p, short n, REAL u)
{
	::checkInDomain(knot, p, n, u);
	register int i;
	int mult = 0;
	for (i = 0; i < n + p + 2; i++)
	{
		if (EQ(u, knot[i]))
			mult++;
	}
	return mult;
}
/********************************************************************************/
/* Functin :  void getNurbsBasis                                                */
/*                 ( REAL *knot, short p, short idx, REAL u, REAL* basis )      */
/* Description : calculte basis function values and store them in basis         */
/* Parameter   : knot  : knot vectors                                           */
/*				 p     : degree                                                 */
/*				 idx   : knot span index                                        */
/*				 u     : parameter value                                        */
/*				 basis : ptr to basis function values                           */
/********************************************************************************/
void getNurbsBasis(REAL* knot, short p, short idx, REAL u, REAL* basis)
{
	basis[0] = (REAL)1.0;
	for (register int j = 1; j <= p; j++)
	{
		REAL saved = (REAL)0.0;
		for (register int k = 0; k < j; k++)
		{
			REAL tmp = basis[k] / (knot[idx + k + 1] - knot[idx + k + 1 - j]);
			basis[k] = saved + (knot[idx + k + 1] - u) * tmp;
			saved = (u - knot[idx + 1 - j + k]) * tmp;
		}
		basis[j] = saved;
	}
}
/********************************************************************************/
/* Functin :  void getNurbsDerivBasis                                           */
/*                 ( REAL *knot, short p, short idx, REAL u, REAL** basis )      */
/* Description : calculte derivate basis function values and store them in basis*/
/* Parameter   : knot  : knot vectors                                           */
/*				 p     : degree                                                 */
/*				 idx   : knot span index                                        */
/*				 u     : parameter value                                        */
/*				 basis : ptr                                                    */
/********************************************************************************/
void getNurbsDerivBasis(REAL* knot, short p, short idx, REAL u, REAL** derivbasis)
{
	REAL** ndu = new REAL*[p + 1];
	REAL** a = new REAL*[2]; a[0] = new REAL[p + 1]; a[1] = new REAL[p + 1];
	REAL* left = new REAL[p + 1];
	REAL* right = new REAL[p + 1];
	REAL saved, tmp, dd;
	int j, r, k, s1, s2, j1, j2, rk, pk;
	for (j = 0; j < p + 1; j++)
		ndu[j] = new REAL[p + 1];
	ndu[0][0] = 1.0;
	for (j = 1; j <= p; j++)
	{
		left[j] = u - knot[idx + 1 - j];
		right[j] = knot[idx + j] - u;
		saved = 0.0;
		for (r = 0; r < j; r++)
		{
			ndu[j][r] = right[r + 1] + left[j - r];
			tmp = ndu[r][j - 1] / ndu[j][r];
			ndu[r][j] = saved + right[r + 1] * tmp;
			saved = left[j - r] * tmp;
		}
		ndu[j][j] = saved;
	}
	for (j = 0; j <= p; j++)
		derivbasis[0][j] = ndu[j][p];
	for (r = 0; r <= p; r++)
	{
		s1 = 0; s2 = 1;
		a[0][0] = 1.0;
		for (k = 1; k <= p; k++)
		{
			dd = 0.0;
			rk = r - k, pk = p - k;
			if (r >= k)
			{
				a[s2][0] = a[s1][0] / ndu[pk + 1][rk];
				dd = a[s2][0] * ndu[rk][pk];
			}
			if (rk >= -1)
				j1 = 1;
			else
				j1 = -rk;
			if (r - 1 <= pk)
				j2 = k - 1;
			else
				j2 = p - r;
			for (j = j1; j <= j2; j++)
			{
				a[s2][j] = (a[s1][j] - a[s1][j - 1]) / ndu[pk + 1][rk + j];
				dd += a[s2][j] * ndu[rk + j][pk];
			}
			if (r <= pk)
			{
				a[s2][k] = -a[s1][k - 1] / ndu[pk + 1][r];
				dd += a[s2][k] * ndu[r][pk];
			}
			derivbasis[k][r] = dd;
			j = s1; s1 = s2; s2 = j;
		}
	}
	r = p;
	for (k = 1; k <= p; k++)
	{
		for (j = 0; j <= p; j++)
		{
			derivbasis[k][j] *= r;
		}
		r *= (p - k);
	}
	delete left;
	delete right;
	delete[] a[0];
	delete[] a[1];
	delete a;
	for (j = 0; j < p + 1; j++)
		delete[] ndu[j];
	delete ndu;
}
/********************************************************************************/
/* Functin : void getNurbsDerivCtlPt( .....       )                             */
/* Description :                                                                */
/* Parameter   :                                                                */
/********************************************************************************/
void getNurbsDerivCtlPt(short p, short n, REAL4* ctlpt, REAL* knot, REAL4** derctlpt, bool G3)
{
	int i, k;
	if (G3) // the case of considering weights
	{
		// not implemented yet.
	}
	else // the case of 4D polynomial curve.
	{
		for (i = 0; i <= n; i++)
		{
			derctlpt[0][i][0] = ctlpt[i][0];
			derctlpt[0][i][1] = ctlpt[i][1];
			derctlpt[0][i][2] = ctlpt[i][2];
			derctlpt[0][i][3] = ctlpt[i][3];
		}
		for (k = 1; k <= p; k++)
			for (i = 0; i <= n - k; i++)
			{
				REAL coeff = (p - k + 1) / (knot[i + p + 1] - knot[i + k]);
				derctlpt[k][i][0] = coeff * (derctlpt[k - 1][i + 1][0] - derctlpt[k - 1][i][0]);
				derctlpt[k][i][1] = coeff * (derctlpt[k - 1][i + 1][1] - derctlpt[k - 1][i][1]);
				derctlpt[k][i][2] = coeff * (derctlpt[k - 1][i + 1][2] - derctlpt[k - 1][i][2]);
				derctlpt[k][i][3] = coeff * (derctlpt[k - 1][i + 1][3] - derctlpt[k - 1][i][3]);
			}
	}
}

/********************************************************************************/
/* Functin : void poly2rational( REAL4* pt )                                    */
/* Description : cartesianize the point                                         */
/* Parameter   : pt   : pointer of point                                        */
/********************************************************************************/
void poly2rational(REAL4 pt)
{
	if (pt[3] < MTYPE_EPS)
	{
		fprintf(stderr, "Attempt to cartesianize a zero weight vector\n"); \
	}
	else
	{
		pt[0] /= pt[3];
		pt[1] /= pt[3];
		pt[2] /= pt[3];
		pt[3] /= pt[3];
	}
}
/********************************************************************************/
/* Functin : void curveInterpolate(        )                                    */
/* Description :                                                                */
/* Parameter   :                                                                */
/********************************************************************************/
void getBinomialCoeff(int n, short** B)
{
	register int i, j;
	for (i = 0; i < n + 1; i++)
	{
		for (j = 0; j < n + 1; j++)
		{
			if (i == 0)
			{
				if (j == 0)
					B[i][j] = 1; // B[0][0] = 1
				else // B[0][j] = 0
					B[i][j] = 0;
			}
			else if (j == 0) //B[i][0] = 1
			{
				B[i][j] = 1;
			}
			else // B[i][j]
			{
				B[i][j] = B[i - 1][j - 1] + B[i - 1][j];
			}
		}
	}
}
/********************************************************************************/
/* Functin : void translateKnotWithScale                                        */
/*                ( REAL* knot, short kidx, REAL off, REAL scale)               */
/* Description :  translate the knot vector with scale factor.                  */
/* Parameter   :																*/
/*				  - knot : knot vector				                            */
/*				  - kidx : kidx+1 is # of knots                                 */
/*				  - off  : offset                                               */
/*				  - scale: scale factor                                         */
/********************************************************************************/
void translateKnotWithScale(REAL* knot, short kidx, REAL off, REAL scale)
{
	register int i;
	REAL min = knot[0];
	for (i = 0; i < kidx + 1; i++)
		knot[i] = min + off + (knot[i] - min) * scale;
}
/********************************************************************************/
REAL* mergeKnotDistinct(REAL* U1, short idx1, REAL* U2, short idx2, short* idx)
{
	short i = 0, j = 0, k = 0, m = 0;
	short idx0 = idx1 + idx2 + 1;
	REAL* U;
	U = new REAL[idx0 + 1];
	for (i = 0; i < idx0 + 1; i++)
	{
		if (i < idx1 + 1)
			U[i] = U1[i];
		else
			U[i] = U2[i - idx1 - 1];
	}
	for (i = 0; i < idx0; i++)
	{
		for (j = i; j < idx0 + 1; j++)
		{
			if (U[i] > U[j])
			{
				REAL tmp = U[j];
				U[j] = U[i];
				U[i] = tmp;
			}
		}
	}
	short cnt = 0;
	for (i = 0; i < idx0; i++)
	{
		if (!EQ(U[i], U[i + 1]))
			cnt++;
	}
	REAL *Knot = new REAL[cnt + 1];
	cnt = 0;
	Knot[cnt] = U[cnt];
	for (i = 1; i < idx0 + 1; i++)
	{
		if (!EQ(U[i], U[i - 1]))
		{
			cnt++;
			Knot[cnt] = U[i];
		}
	}
	delete U;
	*idx = cnt;
	return Knot;
}

/*********************************************************************
* void removeKnot(
					short p, short n, REAL4* P, REAL* U,
					REAL u, short r, short s, short num, short* t );
* Description.
*	Remove knot u_r num-times.
*	P and U is updated.
* Parameter.
*   p : degree of curve.
*	n : n+1 is # of control points.
*	P : control points.
*	U : knot vectors.
*	u : knot value to be removed.
*	r : idx of u in U.
*	s : multiplicity of u in U.
*	num : try to remove u, num times.
*	t : actual # of times u is removed.
*********************************************************************/
short removeCrvKnot(short p, short n, REAL4* P, REAL* U,
	REAL u, short r, short s, short num)

{
	short m = n + p + 1;
	short ord = p + 1;
	short fout = (2 * r - s - p) / 2;
	short last = r - s;
	short first = r - p;
	short t, i, j, ii, jj, off, remflag;
	float alfi, alfj;
	register int k;
	REAL4* tmp = new REAL4[2 * p + 1];
	REAL_ARR_ZERO(tmp, (2 * p + 1) * 4);
	for (t = 0; t < num; t++)
	{
		off = first - 1;
		REAL_ARR_COPY(tmp[0], P[off], 4);
		REAL_ARR_COPY(tmp[last + 1 - off], P[last + 1], 4);
		i = first;
		j = last;
		ii = 1;
		jj = last - off;
		remflag = 0;
		while (j - i > t)
		{
			alfi = (u - U[i]) / (U[i + ord + t] - U[i]);
			alfj = (u - U[j - t]) / (U[j + ord] - U[j - t]);

			tmp[ii][0] = (P[i][0] - (1.0f - alfi)*tmp[ii - 1][0]) / alfi;
			tmp[ii][1] = (P[i][1] - (1.0f - alfi)*tmp[ii - 1][1]) / alfi;
			tmp[ii][2] = (P[i][2] - (1.0f - alfi)*tmp[ii - 1][2]) / alfi;
			tmp[ii][3] = (P[i][3] - (1.0f - alfi)*tmp[ii - 1][3]) / alfi;

			tmp[jj][0] = (P[j][0] - alfj * tmp[jj + 1][0]) / (1.0f - alfj);
			tmp[jj][1] = (P[j][1] - alfj * tmp[jj + 1][1]) / (1.0f - alfj);
			tmp[jj][2] = (P[j][2] - alfj * tmp[jj + 1][2]) / (1.0f - alfj);
			tmp[jj][3] = (P[j][3] - alfj * tmp[jj + 1][3]) / (1.0f - alfj);

			i++; ii++;
			j--; jj--;
		}
		if (j - i < t)
		{
			if (PT4_DISTANCE(tmp[ii - 1], tmp[jj + 1]) <= FBSP_EPS)
				remflag = 1;
		}
		else
		{
			alfi = (u - U[i]) / (U[i + ord + t] - U[i]);
			REAL4 X;
			PT4_BLEND(X, tmp[ii - 1], tmp[ii + t + 1], alfi);
			if (PT4_DISTANCE(P[i], X) <= FBSP_EPS)
				remflag = 1;
		}
		if (remflag == 0)
			break;
		else
		{
			i = first;
			j = last;
			while (j - i > t)
			{
				REAL_ARR_COPY(P[i], tmp[i - off], 4);
				REAL_ARR_COPY(P[j], tmp[j - off], 4);
				i++; j--;
			}
		}
		first--;
		last++;
	}
	if (t == 0)
	{
		delete[] tmp;
		return t;
	}
	for (k = r + 1; k <= m; k++)
		U[k - t] = U[k];
	j = fout; i = j;
	for (k = 1; k < t; k++)
	{
		if (k % 2 == 1)
			i++;
		else
			j--;
	}
	for (k = i + 1; k <= n; k++)
	{
		REAL_ARR_COPY(P[j], P[k], 4);
		j++;
	}
	delete[] tmp;
	return t;
}
/*********************************************************************
* void removeMotKnot(
					short p, short n, REAL4* P, REAL* U,
					REAL u, short r, short s, short num );
* Description.
*	Remove knot u_r num-times.
*	P and U is updated.
* Parameter.
*   p : degree of curve.
*	n : n+1 is # of control points.
*	P : control MATRIX.
*	U : knot vectors.
*	u : knot value to be removed.
*	r : idx of u in U.
*	s : multiplicity of u in U.
*	num : try to remove u, num times.
*	t : actual # of times u is removed.
*********************************************************************/
short removeMotKnot(short p, short n, MATRIX* P, REAL* U,
	REAL u, short r, short s, short num)

{
	short m = n + p + 1;
	short ord = p + 1;
	short fout = (2 * r - s - p) / 2;
	short last = r - s;
	short first = r - p;
	short t, i, j, ii, jj, off, remflag;
	float alfi, alfj;
	register int k;
	MATRIX* tmp = new MATRIX[2 * p + 1];
	MATRIX_ARR_ZERO(tmp, (2 * p + 1));
	for (t = 0; t < num; t++)
	{
		off = first - 1;
		MATRIX_COPY(tmp[0], P[off]);
		MATRIX_COPY(tmp[last + 1 - off], P[last + 1]);
		i = first;
		j = last;
		ii = 1;
		jj = last - off;
		remflag = 0;
		while (j - i > t)
		{
			alfi = (u - U[i]) / (U[i + ord + t] - U[i]);
			alfj = (u - U[j - t]) / (U[j + ord] - U[j - t]);

			for (int row = 0; row < 4; row++)
				for (int col = 0; col < 4; col++)
				{
					tmp[ii][row][col] = (P[i][row][col] - (1.0f - alfi)*tmp[ii - 1][row][col]) / alfi;
					tmp[jj][row][col] = (P[j][row][col] - alfi * tmp[jj + 1][row][col]) / (1.0f - alfi);
				}
			i++; ii++;
			j--; jj--;
		}
		if (j - i < t)
		{
			if (MATRIX_DISTANCE(tmp[ii - 1], tmp[jj + 1]) <= 0.0001)
				remflag = 1;
		}
		else
		{
			alfi = (u - U[i]) / (U[i + ord + t] - U[i]);
			MATRIX X;
			MATRIX_BLEND(X, tmp[ii - 1], tmp[ii + t + 1], alfi);
			REAL D = MATRIX_DISTANCE(P[i], X);
			if (MATRIX_DISTANCE(P[i], X) <= 0.0001)
				remflag = 1;
		}
		if (remflag == 0)
			break;
		else
		{
			i = first;
			j = last;
			while (j - i > t)
			{
				MATRIX_COPY(P[i], tmp[i - off]);
				MATRIX_COPY(P[j], tmp[j - off]);
				i++; j--;
			}
		}
		first--;
		last++;
	}
	if (t == 0)
	{
		delete[] tmp;
		return t;
	}
	for (k = r + 1; k <= m; k++)
		U[k - t] = U[k];
	j = fout; i = j;
	for (k = 1; k < t; k++)
	{
		if (k % 2 == 1)
			i++;
		else
			j--;
	}
	for (k = i + 1; k <= n; k++)
	{
		MATRIX_COPY(P[j], P[k]);
		j++;
	}
	delete[] tmp;
	return t;
}
/*********************************************************************
* void setMotion(PT4 t, REAL4 r1, REAL4 r2, REAL4 s, MATRIX M, bool rat )

* Description.

  Motion Matrix corresponding to t, r1, r2, s is M[4][4]
	t  = ( tx, ty, tz, tw )
	r1 = ( rx1, ry1, yz1, rw1 )
	r1 = ( rx1, ry1, yz1, rw1 )
	s  = ( sx, sy, sz, sw )

* Parameter.

*********************************************************************/
void setMotionMatrix(REAL4 t, REAL4 r1, REAL4 r2, REAL4 s, MATRIX M, bool rat)
{
	/********************************************************
	 *	Motion Matrix corresponding to t, r1, r2, s is M[4][4]
	 * t  = ( tx, ty, tz, tw )
	 * r1 = ( rx1, ry1, yz1, rw1 )
	 * r2 = ( rx2, ry2, yz2, rw2 )
	 * s  = ( sx, sy, sz, sw )
	*********************************************************/
	REAL Delta;

	Delta = r1[0] * r2[0] + r1[1] * r2[1] + r1[2] * r2[2] + r1[3] * r2[3];

	M[0][0] = s[0] * t[3] * (r1[3] * r2[3] + r1[0] * r2[0] - r1[1] * r2[1] - r1[2] * r2[2]);
	M[0][1] = s[1] * t[3] * 2.0f*(r1[0] * r2[1] - r1[3] * r2[2]);
	M[0][2] = s[2] * t[3] * 2.0f*(r1[0] * r2[2] + r1[3] * r2[1]);
	M[0][3] = s[3] * t[0] * Delta;

	M[1][0] = s[0] * t[3] * 2.0f*(r1[0] * r2[1] + r1[3] * r2[2]);
	M[1][1] = s[1] * t[3] * (r1[3] * r2[3] - r1[0] * r2[0] + r1[1] * r2[1] - r1[2] * r2[2]);
	M[1][2] = s[2] * t[3] * 2.0f*(r1[1] * r2[2] - r1[3] * r2[0]);
	M[1][3] = s[3] * t[1] * Delta;

	M[2][0] = s[0] * t[3] * 2.0f*(r1[0] * r2[2] - r1[3] * r2[1]);
	M[2][1] = s[1] * t[3] * 2.0f*(r1[1] * r2[2] + r1[3] * r2[0]);
	M[2][2] = s[2] * t[3] * (r1[3] * r2[3] - r1[0] * r2[0] - r1[1] * r2[1] + r1[2] * r2[2]);
	M[2][3] = s[3] * t[2] * Delta;

	M[3][0] =
		M[3][1] =
		M[3][2] = 0.0f;
	M[3][3] = s[3] * t[3] * Delta;

	if (rat)
	{
		int i, j;
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				M[i][j] /= M[3][3];
		M[3][3] = 1.0;
	}
}
/*************************************************
 * get nuiform parameters
 *************************************************/
void getUniformParam(short n, REAL4 *Q, REAL *UK)
{
	register int i;
	UK[0] = (REAL)0.0;
	UK[n] = (REAL)1.0;
	for (i = 1; i < n; UK[i] = (REAL)i / (REAL)n, i++);
}
/*************************************************
 * get chord length parameters
 *************************************************/
void getChordLengthParam(short n, REAL4* Q, REAL* UK)
{
	register int i;
	REAL len = 0.0;
	for (i = 1; i < n + 1; i++)
		len += PT4_DISTANCE(Q[i], Q[i - 1]);
	UK[0] = (REAL)0.0;
	UK[n] = (REAL)1.0;

	for (i = 1; i < n; i++)
		UK[i] = UK[i - 1] + PT4_DISTANCE(Q[i], Q[i - 1]) / len;
}
/*****************************************************
 * void getUniformKnotVector( short p, short n, REAL *knot )
 *
 * Description.
 *	:  get uniform kont vector.
 *
 * Input Parameter.
 *  p : degree of polynomial
 *	n : n+1 is # of control points( or given data points).
 *  knot : knot vector.
 *
 * Output Parameter.
 *  knot : uniformly distributed knot vector.
 *****************************************************/
void getUniformKnotVector(short p, short n, REAL *knot)
{
	register int i, j;
	int kidx = p + n + 1;
	int snum = kidx - 2 * p;
	for (i = 0; i < p + 1; knot[i] = (REAL)0.0, i++);
	for (i = n + 1; i < kidx + 1; knot[i] = (REAL)1.0, i++);
	for (i = p + 1, j = 1; i < n + 1; knot[i] = (REAL)j / (REAL)(snum), j++, i++);
}
/*****************************************************
 * void getAveragingKnotVector( short p, short n, REAL *knot )
 *
 * Description.
 *	:  get average kont vector.
 *
 * Input Parameter.
 *  p : degree of polynomial
 *	n : n+1 is # of control points( or given data points).
 *  knot : knot vector.
 *
 * Output Parameter.
 *  knot : uniformly distributed knot vector.
 *****************************************************/
void getAveragingKnotVector(short p, short n, REAL* U, REAL* UK)
{
	register int j, i;
	short m = n + p + 1;
	for (j = 0; j < p + 1; j++)
	{
		U[j] = (REAL)0.0;
		U[m - j] = (REAL)1.0;
	}
	for (j = 1; j < n - p + 1; j++)
	{
		REAL tmp = 0.0f;
		for (i = j; i < j + p; i++)
			tmp += UK[i];
		if (p > 0)
			U[p + j] = tmp / p;
		else
			U[p + j] = UK[j];
	}
}
/*******************************************************
 * get knot vector for curve approximation
 *******************************************************/
void getApproxKnotVector(short p, short n, REAL* U, short m, REAL* UK)
{
	// now we decide the knot vectors...
	register int i, j;
	for (i = 0; i < p + 1; i++)
	{
		U[i] = (REAL)0.0;
		U[n + p + 1 - i] = (REAL)1.0;
	}
	REAL d = (REAL)(m + 1.0) / (REAL)(n - p + 1.0);
	for (j = 1; j < n - p + 1; j++)
	{
		i = (int)(j*d);
		REAL alpha = j * d - i;
		U[p + j] = ((REAL)1.0 - alpha)*UK[i - 1] + alpha * UK[i];
	}
}
/*************************************************************************
 * void interpolatePoint( short p, short n, REAL4* P, REAL* U, REAL4* Q )
 *
 * Description.
 *	: interpolate the given data points in Q.
 *
 * Input Parameter.
 *  p : degree of polynomial
 *	n : n+1 is # of control points( or given data points).
 *  U : knot vector.
 *  Q : given data points.
 * Output Parameter.
 *  P : newly computed control points.
 *
 *
**************************************************************************/
void interpolatePoint(short p, short n, REAL4* P, REAL* U, REAL* UK, REAL4* Q)
{
	register int i, j;
	REAL** A = new REAL*[n + 1];
	for (i = 0; i < n + 1; A[i] = new REAL[n + 1], REAL_ARR_ZERO(A[i], n + 1), i++);

	for (i = 0; i < n + 1; i++)
	{
		short idx = ::findKnotSpan(U, p, n, UK[i]);
		::getNurbsBasis(U, p, idx, UK[i], &A[i][idx - p]);
	}

	REAL* Y = new REAL[n + 1];
	REAL* X = new REAL[n + 1];

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < n + 1; j++)
			Y[j] = Q[j][i];
		::gaussianElimination(A, X, Y, n + 1);
		for (j = 0; j < n + 1; j++)
			P[j][i] = X[j];
	}
	for (i = 0; i < n + 1; delete[] A[i], i++);
	delete A;
	delete[] Y;
	delete[] X;
}
void approximatePoint(short p, short n, REAL4* P, REAL* U, REAL* UK, short Qm, REAL4* Q)
{
	register int i, j, k;

	/* memory allocation */
	/*******************************************************/
	REAL** N = new REAL*[Qm - 1];
	for (i = 0; i < Qm - 1; N[i] = new REAL[n - 1], i++);
	REAL** tN = new REAL*[n - 1];
	for (i = 0; i < n - 1; tN[i] = new REAL[Qm - 1], i++);

	REAL* basis = new REAL[p + 1];
	REAL* tmp = new REAL[n + 1];

	REAL** A = new REAL*[n - 1];
	for (i = 0; i < n - 1; i++)
	{
		A[i] = new REAL[n - 1];
		REAL_ARR_ZERO(A[i], (n - 1));
	}


	REAL4* tmpR = new REAL4[Qm - 1];
	for (i = 0; i < Qm - 1; REAL_ARR_ZERO(tmpR[i], 4), i++);


	REAL4* R = new REAL4[n - 1];
	for (i = 0; i < n - 1; REAL_ARR_ZERO(R[i], 4), i++);

	REAL* Y = new REAL[n - 1];
	REAL* X = new REAL[n - 1];
	/**************************************************/

	for (i = 0; i < Qm - 1; i++)
	{
		REAL_ARR_ZERO(N[i], (n - 1));
		REAL_ARR_ZERO(basis, (p + 1));
		REAL_ARR_ZERO(tmp, (n + 1));
		int idx = ::findKnotSpan(U, p, n, UK[i + 1]);
		::getNurbsBasis(U, p, idx, UK[i + 1], basis);
		for (j = 0; j < p + 1; tmp[idx - p + j] = basis[j], j++);
		for (j = 1; j < n; N[i][j - 1] = tmp[j], j++);
	}
	for (i = 0; i < n - 1; i++)
		for (j = 0; j < Qm - 1; j++)
			tN[i][j] = N[j][i];

	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - 1; j++)
			for (k = 0; k < Qm - 1; k++)
				A[i][j] += tN[i][k] * N[k][j];

	for (i = 0; i < Qm - 1; i++)
	{
		short idx = ::findKnotSpan(U, p, n, UK[i + 1]);
		::getNurbsBasis(U, p, idx, UK[i + 1], basis);
		REAL_ARR_ZERO(tmp, (n + 1));
		for (j = 0; j < p + 1; tmp[idx - p + j] = basis[j], j++);
		tmpR[i][0] = Q[i + 1][0] - tmp[0] * Q[0][0] - tmp[n] * Q[Qm][0];
		tmpR[i][1] = Q[i + 1][1] - tmp[0] * Q[0][1] - tmp[n] * Q[Qm][1];
		tmpR[i][2] = Q[i + 1][2] - tmp[0] * Q[0][2] - tmp[n] * Q[Qm][2];
		tmpR[i][3] = Q[i + 1][3] - tmp[0] * Q[0][3] - tmp[n] * Q[Qm][3];
	}

	for (j = 0; j < Qm - 1; j++)
	{
		short idx = ::findKnotSpan(U, p, n, UK[j + 1]);
		::getNurbsBasis(U, p, idx, UK[j + 1], basis);
		REAL_ARR_ZERO(tmp, (n + 1));
		for (k = 0; k < p + 1; tmp[idx - p + k] = basis[k], k++);
		for (i = 0; i < n - 1; i++)
		{
			R[i][0] += tmp[i + 1] * tmpR[j][0];
			R[i][1] += tmp[i + 1] * tmpR[j][1];
			R[i][2] += tmp[i + 1] * tmpR[j][2];
			R[i][3] += tmp[i + 1] * tmpR[j][3];
		}
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < n - 1; j++)
			Y[j] = R[j][i];
		::gaussianElimination(A, X, Y, n - 1);
		for (j = 0; j < n - 1; j++)
			P[j + 1][i] = X[j];
	}
	REAL_ARR_COPY(P[0], Q[0], 4);
	REAL_ARR_COPY(P[n], Q[Qm], 4);
	/* memory free */
	/**********************************************/
	for (i = 0; i < Qm - 1; delete[] N[i], i++);
	delete N;
	for (i = 0; i < n - 1; delete[] tN[i], i++);
	delete tN;

	delete[] basis;
	delete[] tmp;

	for (i = 0; i < n - 1; delete[] A[i], i++);
	delete A;

	delete[] tmpR;
	delete[] R;

	delete[] X;
	delete[] Y;
	/************************************************/
}


