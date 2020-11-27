#pragma once
#ifndef _GNURBSSRF_H
#define _GNURBSSRF_H
/**********************************************************************
 * header file of nurbs surface class.
 * implemented by Seung-Hyun Yun.
 * last update : 2003. 2. 7
 **********************************************************************/

class GNurbsSrf : public GObject
{
protected: // member attributes.
	int		id;
	short	p; // degree of u-direction.
	short	q; // degree of v-dirction.
	short	m; // (m+1)*(n+1) is the number of control points.
	short	n; // (m+1)*(n+1) is the number of control points.
	REAL*	U; // knot vectors of u-direction.
	REAL*	V; // knot vectors of v-direction.
	REAL4** P; // control point net.
	bool	G3;
	bool	Active;

protected: // for memory allocation and deallocation.
	bool _alloc(short p, short m, short q, short n);
	bool _free();
public: // constructor and destructor
	GNurbsSrf();
	GNurbsSrf(short p, short m, REAL *U, short q, short n, REAL *V, REAL4 *P, bool G3 = true);
	GNurbsSrf(short p, short m, REAL *U, short q, short n, REAL *V, REAL4 **P, bool G3 = true);
	GNurbsSrf(short p, short m, short q, short n, REAL4 *P, bool G3 = true);
	GNurbsSrf(short p, short m, short q, short n, REAL4 **P, bool G3 = true);
	GNurbsSrf(const GNurbsSrf& srf);
	GNurbsSrf(const GNurbsSrf* ptrSrf);
	virtual ~GNurbsSrf();

public: // interface to get surface properties.
	short	GetDegU() const;
	short	GetDegV() const;
	short	GetCtlPtIdxU() const;
	short	GetCtlPtIdxV() const;
	short	GetKnotIdxU() const;
	short	GetKnotIdxV() const;
	REAL*	GetKnotU() const;
	REAL*	GetKnotV() const;
	REAL	GetMinKnotU() const;
	REAL	GetMaxKnotU() const;
	REAL	GetMinKnotV() const;
	REAL	GetMaxKnotV() const;
	REAL4** GetCtlPtNet() const;
	void	GetCtlPt(short i, short j, REAL4 pt) const;
	short	FindKnotMultU(REAL u);
	short	FindKnotMultV(REAL v);
	virtual GNurbsCrv * GetProCrv();
	virtual GNurbsSrf * GetContSrf();
	virtual GNurbsCrv * GetTraCrv();
	virtual GNurbsCrv * GetRotCrv();
	virtual GNurbsCrv * GetSclCrv();


public: // operator overloading.
	GNurbsSrf& operator=(GNurbsSrf& srf);

public: // evaluation methods.
	virtual void Eval(REAL u, REAL v, REAL4 pt, bool byctlpt = true);
	void	EvalDeriv(REAL u, REAL v, short kth, short lth, REAL4 pt);

public: // fundamental geometry algorithms.
	void	InsertMultiKnot(short dir, REAL uv, short r = 1);
	void	RefineKnotVector(short dir, REAL* X, short r);

public: // utility for NurbsSrf.
	void	MakeBzrSegment(REAL u, REAL v);
	void	MakeBzrSegment();
	bool	IsBezier() { return (((p == m) && (q == n))); };
	bool	IsBzrSegment();
	bool	IsActive() { return (Active); };
	void	SetActive() { Active = true; };
	void	SetInActive() { Active = false; };
	void	SetId(int id) { this->id = id; };
	int		GetId() { return id; };
	void	Edit(short uIdx, short vIdx, REAL dx, REAL dy, REAL dz, REAL alpha = 1.0f);
	void	Edit(short uIdx, short vIdx, REAL3 Offset, REAL alpha = 1.0f);
	void	Edit(REAL u, REAL v, REAL dx, REAL dy, REAL dz, REAL alpha = 1.0f);
	void	Edit(REAL u, REAL v, REAL3 Offset, REAL alpha = 1.0f);
	GNurbsSrf	GetDerivSrf(short kth, short lth);
	// 2002. 11. 25.
	GNurbsSrf  GetBzrSegment(int r, int c, bool scale = false);
	GNurbsCrv *GetIsoCrvU(REAL u);
	GNurbsCrv *GetIsoCrvV(REAL v);

protected: // for internal use.
	void	FindKnotSpan(REAL u, REAL v, short& uIdx, short& vIdx);
	void	GetBasis(REAL u, REAL v, short uIdx, short vIdx, REAL* uBasis, REAL* vBasis);
	void	GetDerivBasis(REAL u, REAL v, short uIdx, short vIdx, REAL** uDerivBasis, REAL** vDerivBasis);
	void	GetDerivCtlPt(short kth, short lth, REAL4** ctlpt);
	void	ApplyWeight();
	void	ApplyUnWeight();
};

#endif // _GNURBSSRF_H
