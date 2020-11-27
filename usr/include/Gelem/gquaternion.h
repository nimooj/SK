#pragma once

/***********************************************************
 * QUATERNION CLASS
 * QUATERNION Q IS AS FOLLOW
 * Q = ( X, Y, Z, W )
 * MADE BY SEUNG-HYUN YUN
 * LAST MODIFIED : 2002. 7. 12.
 ***********************************************************/
class GQuaternion
{

public:
	static void Add(GQuaternion&, GQuaternion, GQuaternion);
	static void Scale(GQuaternion&, REAL s);
	static void Blend(GQuaternion&, GQuaternion, GQuaternion, REAL, REAL);

	// ATTRIBUTES.
public:
	enum RotateMode { ROT_FREE = 0, ROT_X, ROT_Y, ROT_Z };
	REAL X, Y, Z, W;

public:
	GQuaternion();
	GQuaternion(const GQuaternion& q);
	GQuaternion(REAL x, REAL y, REAL z, REAL w);
	GQuaternion(REAL *q);
	~GQuaternion();

	// OVERLOADED OPERATORS.
public:
	REAL&			operator [](int);
	GQuaternion&	operator =(GQuaternion);
	GQuaternion&	operator =(REAL*);

	GQuaternion		operator +(GQuaternion);
	GQuaternion		operator -(GQuaternion);
	GQuaternion		operator *(GQuaternion);
	GQuaternion		operator *(REAL);
	GVector3f		operator *(GVector3f);
	GQuaternion		operator /(GQuaternion);
	GQuaternion&	operator +=(GQuaternion);
	GQuaternion&	operator -=(GQuaternion);
	GQuaternion&	operator *=(GQuaternion);
	GQuaternion&	operator *=(REAL);
	GQuaternion&	operator /=(GQuaternion);

	GQuaternion		Scale(REAL);
	REAL			Norm();
	REAL			Distance(GQuaternion&);

	// GENERAL MEMBER FUNCTIONS.
public:
	void		Empty();
	void		Conjugate();
	void		Normalize();
	bool		IsUnitQuaternion();
	void		Rotate(REAL *, REAL *, RotateMode mode = ROT_FREE);
	void		Rotate(GVector3f p1, GVector3f p2, RotateMode mode = ROT_FREE);
	void		PrintOut();
	REAL		Dot(GQuaternion &);

	// SET(GET) MEMBER FUNCTIONS.
public:
	void	SetQuat(REAL *);
	void	SetQuat(REAL, REAL, REAL, REAL);
	void 	SetFromMatrix(MATRIX);
	void    SetFromAngleAxis(REAL rfAngle, GVector3f rkAxis);
	void 	SetFromFrame(GVector3f &X, GVector3f &Y, GVector3f &Z);

	GQuaternion		GetExp();
	GQuaternion		GetLog();
	REAL			GetAngle();
	GVector3f		GetAxis();
	void			GetAxis(REAL *);
	void			GetMatrix(MATRIX);
	void			GetGLMatrixd(double *);
	void			GetGLMatrixf(float *);
};


