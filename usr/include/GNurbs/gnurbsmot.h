#pragma once

#ifndef _GNURBSMOT_H
#define _GNURBSMOT_H
/**********************************************************************
 * header file of nurbs motion class.
 * implemented by Seung Hyun Yun.
 * last update : 2002. 5. 10
 **********************************************************************/
class GNurbsMot : public GObject
{
protected:
	bool _alloc();
	bool _free();
public: // Constructors , Destructor and Operator Overloading.
	GNurbsMot();
	GNurbsMot(GNurbsMot& mot);
	GNurbsMot(GNurbsMot* pMot);
	GNurbsMot(GNurbsCrv* t, GNurbsCrv* r = NULL, GNurbsCrv* s = NULL);
	virtual ~GNurbsMot();
	GNurbsMot& operator=(GNurbsMot& mot);
public: // Get Methods
	int		GetId() { return id; };
	short   GetDegree() { return p; };
	short	GetCtlMatIdx() { return n; };
	short	GetKnotIdx() { return n + p + 1; };
	MATRIX* GetCtlMat() { return P; };
	REAL*   GetKnot() { return U; };
	REAL    GetMinKnotVal() { return U[0]; };
	REAL    GetMaxKnotVal() { return U[n + p + 1]; };
	GNurbsCrv* GetTraCrv() { return T; };
	GNurbsCrv* GetRotCrv() { return R; };
	GNurbsCrv* GetSclCrv() { return S; };
	void	GetCtlMat(short idx, MATRIX m) { MATRIX_COPY(m, P[idx]); };
public: // Set Methods
	void	SetActive() { Active = true; };
	void	SetInActive() { Active = false; };
	void	SetId(int id) { this->id = id; };
public: // general interfaces
	bool	IsActive() { return (Active); };
public: // Evaluation Methods.
	void	Eval(REAL u, MATRIX mat, bool ByMat = true);
public: // Fundamental Geometric Functions.
	short	RemoveMultiKnot(REAL u, short r = 1);
protected: /* for internal usage */
	bool	CalcCtlMatrix();
	short	FindKnotSpan(REAL u) { return ::findKnotSpan(U, p, n, u); };
	short	FindKnotMult(REAL u) { return ::findKnotMult(U, p, n, u); };
	void	GetBasis(REAL u, short idx, REAL* basis) { ::getNurbsBasis(U, p, idx, u, basis); };
private:
	// attributes.
	int			id;		// id of motion.
	short		p;		// degree of motion.
	short		n;		// n+1 is # of control matrix.
	MATRIX*		P;		// control matrix.
	REAL*		U;		// knot vector.
	GNurbsCrv*	T;		// translational curve.
	GNurbsCrv*	R;		// rotational curve.( quaternion curve !!!! )
	GNurbsCrv*	S;		// scaling curve.
	bool		Active; // true if motion is in active state.	
};


#endif //_GNURBSMOT_H
