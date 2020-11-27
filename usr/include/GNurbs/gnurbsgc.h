#pragma once
#ifndef _GNURBGC_H
#define _GNURBGC_H
/**********************************************************************
 * header file of scaling generalized cylinder class.
 * implemented by Seung Hyun Yun.
 * last update : 2002. 5. 10
 **********************************************************************/
class GNurbsSgc : public GNurbsSrf
{
protected:
	bool _alloc();
	bool _free();
public:
	// constructors and descructor.
	GNurbsSgc();
	GNurbsSgc(const GNurbsSgc &GC);
	GNurbsSgc(const GNurbsSgc *pGC);
	GNurbsSgc(GNurbsCrv *X, GNurbsCrv *T, GNurbsCrv *R = NULL, GNurbsCrv *S = NULL);
	GNurbsSgc& operator =(GNurbsSgc& gc);
	virtual ~GNurbsSgc();
public:
	// get methods.
	GNurbsSrf * GetContSrf() { return NULL; };
	GNurbsCrv * GetProCrv() { return X; };
	GNurbsCrv * GetTraCrv() { return T; };
	GNurbsCrv * GetRotCrv() { return R; };
	GNurbsCrv * GetSclCrv() { return S; };
	// set methods.
protected:
	bool CalcCtlPtNet();
public:
	// evaluation methods.
	void Eval(REAL u, REAL v, REAL4 pt, bool byctlpt = true);
private:
	/* cross section curve */
	GNurbsCrv *X;

	/* translational curve */
	GNurbsCrv *T;

	/* ratational curve */
	GNurbsCrv *R;

	/* scaling curve */
	GNurbsCrv *S;
};

/**********************************************************************
 * generalized cylinder with contour line surface.
 * implemented by Seung Hyun Yun.
 * last update : 2002. 6. 27
 **********************************************************************/
class GNurbsCgc : public GNurbsSrf
{
protected:
	bool _alloc();
	bool _free();
public:
	// constructors and descructor.
	GNurbsCgc();
	GNurbsCgc(GNurbsCgc &GC);
	GNurbsCgc(GNurbsCgc *pGC);
	GNurbsCgc(GNurbsSrf *C, GNurbsCrv *T, GNurbsCrv *R);
	GNurbsCgc& operator =(GNurbsCgc& gc);
	virtual ~GNurbsCgc();
public:
	// get methods.
	GNurbsSrf * GetContSrf() { return C; };
	GNurbsCrv * GetProCrv() { return NULL; };
	GNurbsCrv * GetTraCrv() { return T; };
	GNurbsCrv * GetRotCrv() { return R; };
	GNurbsCrv * GetSclCrv() { return NULL; };
	// see parent class GNurbsSrf.
	// set methods.
protected:
	bool CalcCtlPtNet();
public:
	// evaluation methods.
	void Eval(REAL u, REAL v, REAL4 pt, bool byctlpt = true);
private:
	/* contour line surface */
	GNurbsSrf *C;

	/* translational curve */
	GNurbsCrv *T;

	/* rotational curve */
	GNurbsCrv *R;
};


#endif // _GNURBSGC_H

