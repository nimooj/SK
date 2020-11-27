#pragma once

#ifndef _GNURBSCRV_H
#define _GNURBSCRV_H
/**********************************************************************
 * Header file of nurbs curvs class.
 * Implemented by Seung Hyun Yun.
 * Last update : 2002. 5. 10
 **********************************************************************/
class GNurbsCrv : public GObject
{
public:
	enum		CREATE_MODE { CTLPT_CRV = 0, INTER_CRV = 1 };
	enum		PARAM_TYPE { UNIFORM = 0, CHORD_LEN = 1 };
private: //attributes.
	int			id; // id of curve.
	short		p;  // degree of Curve.
	short		n;  // n + 1 is the number of CtlPt.
	REAL4*		P;  // array  of CtlPt.
	REAL*		U;  // array  of Knot.
	bool		G3; // if 3D or 4D.
	bool		Active; // true if curve is active.
protected:
	bool		_alloc(const short, const short);
	bool		_free();
public: // constructors and destructor.
	// default constructor.
	GNurbsCrv();
	// general constructor.
	GNurbsCrv(short p, short n, REAL4 *P, REAL *U, bool G3 = true);
	// constructor without knot vector
	GNurbsCrv(short p, short n, REAL4 *P, CREATE_MODE cmode = CTLPT_CRV,
		PARAM_TYPE ptype = UNIFORM, bool G3 = true);
	// constructor for approximation curve.
	GNurbsCrv(short p, short n, REAL4 *Q, short Qm,
		PARAM_TYPE ptype = CHORD_LEN, bool G3 = true);
	// copy constructor.
	GNurbsCrv(const GNurbsCrv&);
	GNurbsCrv(const GNurbsCrv*);
	// default destroctor.
	virtual ~GNurbsCrv();
public: // Get Methods
	int			GetId() const { return id; };
	short		GetDegree() const { return p; };
	short		GetCtlPtIdx() const { return n; };
	short		GetKnotIdx() const { return n + p + 1; };
	REAL4*		GetCtlPt() const { return P; };
	REAL*		GetKnot() const { return U; };
	REAL		GetMinKnotVal() const { return U[0]; };
	REAL		GetMaxKnotVal() const { return U[n + p + 1]; };
	void		GetCtlPt(short idx, REAL4 pt) const { REAL_ARR_COPY(pt, P[idx], 4); }
	short		FindKnotMult(REAL u) { return ::findKnotMult(U, p, n, u); };
public: // Set Methods
	void		SetInActive() { Active = false; };
	void		SetActive() { Active = true; };
	void		SetId(int id) { this->id = id; };

public: // Evalution interfaces for GNurbsCrv.
	void		Eval(REAL u, REAL4 pt);
	void		EvalDeriv(REAL u, short nth, REAL4 pt);
public: // Fundamental Geometric Algorithms.
	void		InsertMultiKnot(REAL u, short r = 1);
	void		RefineKnotVector(REAL* X, short r);
	short		RemoveMultiKnot(REAL u, short r = 1);
	void		ElevateDegree(short t);
public: // Utility for GNurbsCrv
	bool		IsBezier() { return (n == p); };
	bool		IsQuatCrv() { return (!G3); };
	bool		IsBzrSegment();
	bool		IsActive() { return (Active); };
	void		Edit(short EidtCtlPtIdx, REAL dx, REAL dy, REAL dz, REAL alpha = 1.0f);
	void		Edit(short EditCtlPtIdx, REAL3 Offset, REAL alpha = 1.0f);
	void		Edit(REAL u, REAL3 Offset, REAL alpha = 1.0f);
	void		Edit(REAL u, REAL dx, REAL dy, REAL dz, REAL alpha = 1.0f);
	void		MakeBzrSegment();
	void		TranslateKnot(REAL offset, REAL scale);
	void		MakeKnotCompatible(GNurbsCrv* pCrv);
	void		MakeDegCompatible(GNurbsCrv *pCrv);
	// 2002. 11. 25
	GNurbsCrv	GetBzrSegment(short ith, bool scale = false);
	GNurbsCrv	GetPolyDerivCrv(short nth);
	GNurbsCrv	GetRatDerivCrv(short nth);
	GNurbsCrv*	GetFrenetFrame(short deg);
public: // curve fitting.
	void		InterCrv(short p, REAL4* Q, short n, PARAM_TYPE ptype = CHORD_LEN, bool isG3 = true);
	void		ApproxCrv(short p, short n, REAL4* Q, short Qm, PARAM_TYPE ptype = CHORD_LEN, bool isG3 = true);
public: // Operators overloading.
	GNurbsCrv&	operator=(const GNurbsCrv& crv);
	GNurbsCrv	operator*(GNurbsCrv& crv);
	GNurbsCrv	operator^(GNurbsCrv& crv);
	void		PrintKnotVector();
protected: // for internal use.
	short		FindKnotSpan(REAL u) { return ::findKnotSpan(U, p, n, u); };
	void		GetBasis(REAL u, short idx, REAL* basis)
	{
		::getNurbsBasis(U, p, idx, u, basis);
	};
	void		GetDerivBasis(REAL u, short idx, REAL** derivbasis)
	{
		::getNurbsDerivBasis(U, p, idx, u, derivbasis);
	};
	void		GetDerivCtlPt(short nth, REAL4* ctlpt);
	void		ApplyWeight();
	void		ApplyUnWeight();
};

#endif // _GNURBSCRV_H

