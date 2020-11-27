#pragma once

#ifndef _LINEAR_SYSTEM_H
#define _LINEAR_SYSTEM_H

REAL svdLeastSqr(REAL *A, REAL *x, REAL *b, int NData, int Nx);
void svdcmp(REAL **a, int m, int n, REAL *w, REAL **v);
void gaussianElimination(REAL** A, REAL* X, REAL* Y, short n);

#define SVD_TOL 1.0e-5
#define SIGN2(a, b)	((b) >= 0.0 ? (REAL)fabs(a) : -(REAL)fabs(a))
#define PYTHAG(a, b)	SQRT((SQR(a) + SQR(b)))
#define SVD_VECTOR(n)	((REAL *) malloc(sizeof(REAL) * (n + 1)))
#define SVD_FREE_VEC(p)	free(p)

#else
#endif // _LINEAR_SYSTEM_H
