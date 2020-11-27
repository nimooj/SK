#include "pch.h"
#include "..\usr\include\GNurbs\GNurbs.h"

/*****************************************************************************/
/* DESCRIPTION:                                                              */
/* Singular value decomposition of matrix A into A W V.			             */
/*   Given A is m by n, m >= n.						                         */
/*   Output:								                                 */
/*	A is m by n modified in place into U,				                     */
/*	W is a vector of length m,												 */
/*	V is a square matrix of size n by n.									 */
/*   Due to the Fortran style of this code all indices are from 1 to k, so   */
/* an array from 1 to k actually have k + 1 elements in this C translation.  */
/*                                                                           */
/* PARAMETERS:                                                               */
/*   a:    A m by m matrix.                                                  */
/*   m, n: Dimensions of A, W and V											 */
/*   w:    A vector of length m												 */
/*   v:    A matrix of size n by n.											 */
/*                                                                           */
/* RETURN VALUE:                                                             */
/*   void                                                                    */
/*****************************************************************************/
void svdcmp(REAL **a, int m, int n, REAL *w, REAL **v)
{
	int flag, i, its, j, jj, k, l, nm;
	REAL c, f, h, s, x, y, z, *rv1;
	REAL anorm = 0.0, g = 0.0, scale = 0.0;
	//if (m < n)
	//	cerr << "SVDCMP: You must augment A with extra zero rows";
	rv1 = SVD_VECTOR(n);
	for (i = 1; i <= n; i++)
	{
		l = i + 1;
		rv1[i] = scale * g;
		g = s = scale = 0.0;
		if (i <= m)
		{
			for (k = i; k <= m; k++)
				scale += (REAL)fabs(a[k][i]);
			if (scale)
			{
				for (k = i; k <= m; k++)
				{
					a[k][i] /= scale;
					s += a[k][i] * a[k][i];
				}
				f = a[i][i];
				g = -SIGN2(SQRT(s), f);
				h = f * g - s;
				a[i][i] = f - g;
				if (i != n)
				{
					for (j = l; j <= n; j++)
					{
						for (s = 0.0, k = i; k <= m; k++)
							s += a[k][i] * a[k][j];
						f = s / h;
						for (k = i; k <= m; k++)
							a[k][j] += f * a[k][i];
					}
				}
				for (k = i; k <= m; k++)
					a[k][i] *= scale;
			}
		}
		w[i] = scale * g;
		g = s = scale = 0.0;
		if (i <= m && i != n)
		{
			for (k = l; k <= n; k++)
				scale += (REAL)fabs(a[i][k]);
			if (scale)
			{
				for (k = l; k <= n; k++)
				{
					a[i][k] /= scale;
					s += a[i][k] * a[i][k];
				}
				f = a[i][l];
				g = -SIGN2(SQRT(s), f);
				h = f * g - s;
				a[i][l] = f - g;
				for (k = l; k <= n; k++)
					rv1[k] = a[i][k] / h;
				if (i != m)
				{
					for (j = l; j <= m; j++)
					{
						for (s = 0.0, k = l; k <= n; k++)
							s += a[j][k] * a[i][k];
						for (k = l; k <= n; k++)
							a[j][k] += s * rv1[k];
					}
				}
				for (k = l; k <= n; k++) a[i][k] *= scale;
			}
		}
		s = (REAL)fabs(w[i]) + (REAL)fabs(rv1[i]);
		anorm = MAX(anorm, s);
	}
	for (i = n; i >= 1; i--)
	{
		if (i < n)
		{
			if (g)
			{
				for (j = l; j <= n; j++)
					v[j][i] = (a[i][j] / a[i][l]) / g;
				for (j = l; j <= n; j++)
				{
					for (s = 0.0, k = l; k <= n; k++)
						s += a[i][k] * v[k][j];
					for (k = l; k <= n; k++)
						v[k][j] += s * v[k][i];
				}
			}
			for (j = l; j <= n; j++)
				v[i][j] = v[j][i] = 0.0;
		}
		v[i][i] = 1.0;
		g = rv1[i];
		l = i;
	}
	for (i = n; i >= 1; i--)
	{
		l = i + 1;
		g = w[i];
		if (i < n)
			for (j = l; j <= n; j++)
				a[i][j] = 0.0;
		if (g)
		{
			g = (REAL)1.0 / g;
			if (i != n)
			{
				for (j = l; j <= n; j++)
				{
					for (s = 0.0, k = l; k <= m; k++)
						s += a[k][i] * a[k][j];
					f = (s / a[i][i]) * g;
					for (k = i; k <= m; k++)
						a[k][j] += f * a[k][i];
				}
			}
			for (j = i; j <= m; j++)
				a[j][i] *= g;
		}
		else
		{
			for (j = i; j <= m; j++)
				a[j][i] = 0.0;
		}
		++a[i][i];
	}
	for (k = n; k >= 1; k--)
	{
		for (its = 1; its <= 30; its++)
		{
			flag = 1;
			for (l = k; l >= 1; l--)
			{
				nm = l - 1;
				if (fabs(rv1[l]) + anorm == anorm)
				{
					flag = 0;
					break;
				}
				if (fabs(w[nm]) + anorm == anorm)
					break;
			}
			if (flag)
			{
				c = 0.0;
				s = 1.0;
				for (i = l; i <= k; i++)
				{
					f = s * rv1[i];
					if (fabs(f) + anorm != anorm)
					{
						g = w[i];
						h = (REAL)PYTHAG(f, g);
						w[i] = h;
						h = (REAL)1.0 / h;
						c = g * h;
						s = (-f * h);
						for (j = 1; j <= m; j++)
						{
							y = a[j][nm];
							z = a[j][i];
							a[j][nm] = y * c + z * s;
							a[j][i] = z * c - y * s;
						}
					}
				}
			}
			z = w[k];
			if (l == k)
			{
				if (z < 0.0)
				{
					w[k] = -z;
					for (j = 1; j <= n; j++)
						v[j][k] = (-v[j][k]);
				}
				break;
			}
			//if (its == 30)
			//	cerr << "No convergence in 30 SVDCMP iterations";
			x = w[l];
			nm = k - 1;
			y = w[nm];
			g = rv1[nm];
			h = rv1[k];
			f = ((y - z) * (y + z) + (g - h) * (g + h)) / ((REAL)2.0 * h * y);
			g = PYTHAG(f, (REAL)1.0);
			f = ((x - z) * (x + z) + h * ((y / (f + (REAL)SIGN2(g, f))) - h)) / x;
			c = s = 1.0;
			for (j = l; j <= nm; j++)
			{
				i = j + 1;
				g = rv1[i];
				y = w[i];
				h = s * g;
				g = c * g;
				z = PYTHAG(f, h);
				rv1[j] = z;
				c = f / z;
				s = h / z;
				f = x * c + g * s;
				g = g * c - x * s;
				h = y * s;
				y = y * c;
				for (jj = 1; jj <= n; jj++)
				{
					x = v[jj][j];
					z = v[jj][i];
					v[jj][j] = x * c + z * s;
					v[jj][i] = z * c - x * s;
				}
				z = PYTHAG(f, h);
				w[j] = z;
				if (z)
				{
					z = (REAL)1.0 / z;
					c = f * z;
					s = h * z;
				}
				f = (c * g) + (s * y);
				x = (c * y) - (s * g);
				for (jj = 1; jj <= m; jj++)
				{
					y = a[jj][j];
					z = a[jj][i];
					a[jj][j] = y * c + z * s;
					a[jj][i] = z * c - y * s;
				}
			}
			rv1[l] = 0.0;
			rv1[k] = f;
			w[k] = x;
		}
	}
	SVD_FREE_VEC(rv1);
}
/******************************************************************************/
/* DESCRIPTION:                                                               */
/* Least square solves A x = b.						                          */
/*   The vector X is of size Nx, vector b is of size NData and matrix A is    */
/* of size Nx by NData.							                              */
/*   Uses singular value decomposition.					                      */
/*   If A != NULL is SVD decomposition is computed, otherwise (A == NULL) a   */
/* solution is computed for the given b and is placed in x.		              */
/*                                                                            */
/* PARAMETERS:                                                                */
/*   A:         The matrix of size Nx by NData.                               */
/*   x:         The vector of sought solution of size Nx.                     */
/*   b:         The vector of coefficients of size NData.                     */
/*   NData, Nx:  Dimensions of input.                                         */
/*                                                                            */
/* RETURN VALUE:                                                              */
/*   REAL:  The reciprocal of the condition number, if A != NULL, zero        */
/*	       otherwise.						                                  */
/*                                                                            */
/* KEYWORDS:                                                                  */
/*   SvdLeastSqr,  singular value decomposition, linear systems               */
/******************************************************************************/
REAL svdLeastSqr(REAL *A, REAL *x, REAL *b, int NData, int Nx)
{
	static int AllocNData = 0, AllocNx = 0;
	static REAL	**SVD_U = NULL, **SVD_V = NULL, *SVD_W = NULL;
	int i, j;
	if (A != NULL)
	{
		REAL Min, Max;
		if (SVD_U != NULL)
		{		 /* Free old instance of aux. data. */
			for (i = 0; i <= AllocNData; i++)
				SVD_FREE_VEC(SVD_U[i]);
			free(SVD_U);
			for (i = 0; i <= AllocNx; i++)
				SVD_FREE_VEC(SVD_V[i]);
			free(SVD_V);
			SVD_FREE_VEC(SVD_W);
		}
		SVD_U = (REAL**)malloc((NData + 1) * sizeof(REAL *));
		SVD_V = (REAL **)malloc((Nx + 1) * sizeof(REAL *));
		SVD_W = SVD_VECTOR(1 + MAX(NData, Nx));
		for (i = 0; i <= NData; i++)
			SVD_U[i] = SVD_VECTOR(Nx);
		for (i = 0; i <= Nx; i++)
			SVD_V[i] = SVD_VECTOR(Nx);
		AllocNData = NData;
		AllocNx = Nx;
		for (i = 0; i < NData; i++)
		{
			for (j = 0; j < Nx; j++)
			{
				SVD_U[i + 1][j + 1] = A[i * Nx + j];
			}
		}
		svdcmp(SVD_U, NData, Nx, SVD_W, SVD_V);
		Min = Max = SVD_W[1];
		for (i = 1; i <= Nx; i++)
		{
			if (Min > SVD_W[i])
				Min = SVD_W[i];
			if (Max < SVD_W[i])
				Max = SVD_W[i];
#ifdef DEBUG_PRINT_SVD_W
			fprintf(stderr, "W[%d] = %f\n", i, SVD_W[i]);
#endif /* DEBUG_PRINT_SVD_W */
		}
		return Min / Max; /* The reciprocal of the codition number. */
	}
	else // A == NULL
	{
		REAL* TVec = SVD_VECTOR(Nx);
		for (j = 1; j <= Nx; j++)
		{
			REAL s = 0.0;
			if (SVD_W[j])
			{
				for (i = 1; i <= NData; i++)
					s += SVD_U[i][j] * b[i - 1];
				s /= SVD_W[j];
			}
			TVec[j] = s;
		}
		for (j = 1; j <= Nx; j++)
		{
			REAL s = 0.0;
			for (i = 1; i <= Nx; i++)
				s += SVD_V[j][i] * TVec[i];
			x[j - 1] = s;
		}
		SVD_FREE_VEC(TVec);
		return 0.0;
	}
}
/*********************************************************************************
* Gaussian_Ellimination
* void gaussianElimination( REAL** A, REAL* X, REAL* Y, short n )
* solve the linear equation.
* A*X = Y
* A = n x n matrix.
* X = n x 1 array.
* Y = n x 1 array.
**********************************************************************************/
void gaussianElimination(REAL** A, REAL* X, REAL* Y, short n)
{
	// Gaussian Elimination 
	register int i, j, k;
	int co_var;
	REAL result;

	REAL **M = new REAL*[n];
	for (i = 0; i < n; M[i] = new REAL[n + 1], i++);

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			M[i][j] = A[i][j];
		M[i][n] = Y[i];
	}

	for (k = 0; k < n; k++)
		for (i = k + 1; i < n; i++)
			for (j = k + 1; j < n + 1; j++)
				M[i][j] = M[i][j] - ((M[k][j] / M[k][k]) * M[i][k]);

	X[n - 1] = M[n - 1][n] / M[n - 1][n - 1];
	co_var = n - 1;

	while (co_var != 0)
	{
		result = 0;
		for (j = co_var + 1; j < n; j++)
			result += M[co_var][j] * X[j];
		X[co_var] = (M[co_var][n] - result) / M[co_var][co_var];
		co_var--;
	}

	if (co_var == 0)
	{
		result = 0;
		for (j = co_var + 1; j < n; j++)
			result += M[co_var][j] * X[j];
		X[co_var] = (M[co_var][n] - result) / M[co_var][co_var];
		co_var--;
	}

	for (i = 0; i < n; delete[] M[i], i++);
	delete M;
}