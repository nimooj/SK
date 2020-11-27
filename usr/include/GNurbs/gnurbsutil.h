#pragma once

bool checkInDomain(REAL* knot, short p, short n, REAL& u);

short findKnotSpan(REAL* knot, short p, short n, REAL u);

short findKnotMult(REAL* knot, short p, short n, REAL u);

void poly2rational(REAL4 pt);

void getNurbsBasis(REAL* knot, short p, short idx, REAL u, REAL* basis);

void getNurbsDerivBasis(REAL* knot, short p, short idx, REAL u, REAL** derivbasis);

bool interpolateCurve(REAL *knot, REAL4 *cv, short degree, short length, short ptlen, REAL *params, REAL4 *pts);

void getNurbsDerivCtlPt(short p, short n, REAL4 *ctlpt, REAL* knot, REAL4** derctlpt, bool G3);

void getBinomialCoeff(int n, short** B);

void translateKnotWithScale(REAL* kont, short kidx, REAL off, REAL scale);

REAL * mergeKnotDistinct(REAL* U1, short idx1, REAL* U2, short idx2, short* kidx);

short removeCrvKnot(short p, short n, REAL4* P, REAL* U, REAL u, short r, short s, short num);

short removeMotKnot(short p, short n, MATRIX* P, REAL* U, REAL u, short r, short s, short num);

void setMotionMatrix(REAL4 v, REAL4 r1, REAL4 r2, REAL4 s, MATRIX M, bool rat);

void getUniformParam(short n, REAL4 *Q, REAL *UK);

void getChordLengthParam(short n, REAL4 *Q, REAL*UK);

void getUniformKnotVector(short p, short n, REAL *knot);

void getAveragingKnotVector(short p, short n, REAL* U, REAL* UK);

void getApproxKnotVector(short p, short n, REAL* U, short m, REAL* UK);

void interpolatePoint(short p, short n, REAL4* P, REAL* U, REAL* UK, REAL4* Q);

void approximatePoint(short p, short n, REAL4* P, REAL* U, REAL* UK, short Qm, REAL4* Q);


