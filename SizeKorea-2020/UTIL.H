#ifndef _UTIL_H
#define _UTIL_H

#include "SizeKorea.h"

extern bool getBaricentricCoord( GVector3f a, GVector3f b, GVector3f c, GVector3f p, double *u, double *v, double *w );
extern void getSortedPoint( GObList<GVector3f> &PtList, int axis, int stype );
//extern void getSortedAnglePoint( GObList<GVector3f> &PtList, int plane, double *CX, double *CY, double *CZ);
extern float getSortedPtLen( GObList<GVector3f> &PtList );
extern bool GetIntersectPointXY(const GVector3f& AP1, const GVector3f& AP2, const GVector3f& BP1, const GVector3f& BP2, GVector3f* IP);
extern bool GetIntersectPointZY(const GVector3f& AP1, const GVector3f& AP2, const GVector3f& BP1, const GVector3f& BP2, GVector3f* IP);
extern bool GetIntersectPointZX(const GVector3f& AP1, const GVector3f& AP2, const GVector3f& BP1, const GVector3f& BP2, GVector3f* IP);

extern int InCircle(GVector3f p, GVector3f p1, GVector3f p2, GVector3f p3, GVector3f *cp, double *r);
extern int WhichSide(GVector3f p, GVector3f p1, GVector3f p2);
//extern void triangulate(int, GVector3f *,TRIANGLE *,int *);
#endif // _UTIL_H
