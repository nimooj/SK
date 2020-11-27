#include "pch.h"
#include "../usr/include/Gelem/gelem.h"
#include <assert.h>

#define QUAT_EPSILON (REAL)0.00000001
#define CHECK_QUAT(Q) \
		if ( Q.X < QUAT_EPSILON ) \
			Q.X = 0.0f; \
		if ( Q.Y < QUAT_EPSILON ) \
			Q.Y = 0.0f; \
		if ( Q.Z < QUAT_EPSILON ) \
			Q.Z = 0.0f; \
		if ( Q.W < QUAT_EPSILON ) \
			Q.W = 0.0f; 


/********************************************************************
 * STATIC MEMBER FUNCTION FOR QUATERNION ADDITION.
 ********************************************************************/
void GQuaternion::Add(GQuaternion &tgt, GQuaternion q1, GQuaternion q2)
{
	tgt.X = q1.X + q2.X;
	tgt.Y = q1.Y + q2.Y;
	tgt.Z = q1.Z + q2.Z;
	tgt.W = q1.W + q2.W;
}
/********************************************************************
 * STATIC MEMBER FUNCTION FOR QUATERNION SCALE.
 ********************************************************************/
void GQuaternion::Scale(GQuaternion& tgt, REAL s)
{
	tgt.X *= s;
	tgt.Y *= s;
	tgt.Z *= s;
	tgt.W *= s;
}
/********************************************************************
 * STATIC MEMBER FUNCTION FOR QUATERNION BLENDING.
 ********************************************************************/
void GQuaternion::Blend(GQuaternion& tgt, GQuaternion q1, GQuaternion q2, REAL s1, REAL s2)
{
	tgt.X = q1.X*s1 + q2.X*s2;
	tgt.Y = q1.Y*s1 + q2.Y*s2;
	tgt.Z = q1.Z*s1 + q2.Z*s2;
	tgt.W = q1.W*s1 + q2.W*s2;
}
/********************************************************************
 * DEFAULT CONSTRUCTOR.
 ********************************************************************/
GQuaternion::GQuaternion()
{
	X = (REAL)0.0;
	Y = (REAL)0.0;
	Z = (REAL)0.0;
	W = (REAL)1.0;
}
/********************************************************************
 * COPY CONSTRUCTOR.
 ********************************************************************/
GQuaternion::GQuaternion(const GQuaternion &q)
{
	X = q.X;
	Y = q.Y;
	Z = q.Z;
	W = q.W;
}
/********************************************************************
 * GENERAL CONSTRUCTOR(1).
 ********************************************************************/
GQuaternion::GQuaternion(REAL x, REAL y, REAL z, REAL w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}
/********************************************************************
 * GENERAL CONSTRUCTOR(2).
 ********************************************************************/
GQuaternion::GQuaternion(REAL *q)
{
	SetQuat(q);
}
/********************************************************************
 * DEFAULT DESTRUCTOR.
 ********************************************************************/
GQuaternion::~GQuaternion(void)
{
}
/********************************************************************
 * SET QUATERNION FROM MATRIX.
 ********************************************************************/
void GQuaternion::SetFromMatrix(MATRIX mat)
{
	REAL  q[4];
	REAL  tr, s;
	int   i, j, k;

	int nxt[3] = { 1, 2, 0 };

	tr = mat[0][0] + mat[1][1] + mat[2][2];

	if (tr > (REAL)0.0)
	{
		s = SQRT(tr + (REAL)1.0);
		W = s * (REAL)0.5;
		s = (REAL)0.5 / s;
		X = (mat[2][1] - mat[1][2]) * s;
		Y = (mat[0][2] - mat[2][0]) * s;
		Z = (mat[1][0] - mat[0][1]) * s;
	}
	else
	{
		i = 0;
		if (mat[1][1] > mat[0][0])
			i = 1;
		if (mat[2][2] > mat[i][i])
			i = 2;
		j = nxt[i];
		k = nxt[j];
		s = SQRT((mat[i][i] - (mat[j][j] + mat[k][k])) + (REAL)1.0);
		q[i] = s * (REAL)0.5;
		s = (REAL)0.5 / s;
		W = (mat[k][j] - mat[j][k]) * s;
		q[j] = (mat[j][i] + mat[i][j]) * s;
		q[k] = (mat[k][i] + mat[i][k]) * s;
		X = q[0];
		Y = q[1];
		Z = q[2];
	}
}
/********************************************************************
 * SETT QUATERNION FROM FRAME.
 ********************************************************************/
void GQuaternion::SetFromFrame(GVector3f &X, GVector3f &Y, GVector3f &Z)
{
	/*	matrix form
		|X0	Y0	Z0	0| |X|
		|X1	Y1	Z1	0| |Y|
		|X2	Y2	Z2	0| |Z|
		|0	0	0	1| |W|
	*/
	MATRIX mat;
	MATRIX_ZERO(mat);

	mat[0][0] = X.X;
	mat[1][0] = X.Y;
	mat[2][0] = X.Z;

	mat[0][1] = Y.X;
	mat[1][1] = Y.Y;
	mat[2][1] = Y.Z;

	mat[0][2] = Z.X;
	mat[1][2] = Z.Y;
	mat[2][2] = Z.Z;

	SetFromMatrix(mat);
}
/*********************************************************
 * SET UP QUATERNION FROM TWO VECTORS.
 * ARGUMENTS(FROM ,TO ) MUST BE ON THE UNIT SPHERE.
 *********************************************************/
void GQuaternion::Rotate(REAL *from, REAL *to, RotateMode mode)
{
	REAL axis[3], angle, cs, cs2, sn, sn2;

	PT3_CROSS(axis, from, to);
	if (PT3_EQ_ZERO_EPS(axis, MTYPE_EPS))
	{
		X = 0.0; Y = 0.0; Z = 0.0; W = 1.0;
		return;
	}

	sn = PT3_LENGTH(axis);
	cs = PT3_DOT(from, to);
	angle = (REAL)atan2((double)sn, (double)cs);
	sn2 = SIN(angle / (REAL)2.0);
	cs2 = COS(angle / (REAL)2.0);

	switch (mode)
	{
	case ROT_FREE:
		X = sn2 * axis[0] / sn;
		Y = sn2 * axis[1] / sn;
		Z = sn2 * axis[2] / sn;
		W = cs2;
		break;
	case ROT_X:
		X = (axis[0] < 0.0f) ? -sn2 : sn2;
		Y = 0.0f;
		Z = 0.0f;
		W = cs2;
		break;
	case ROT_Y:
		X = 0.0f;
		Y = (axis[1] < 0.0f) ? -sn2 : sn2;
		Z = 0.0f;
		W = cs2;
		break;
	case ROT_Z:
		X = 0.0f;
		Y = 0.0f;
		Z = (axis[2] < 0.0f) ? -sn2 : sn2;
		W = cs2;
		break;
	}

}
void GQuaternion::Rotate(GVector3f p1, GVector3f p2, RotateMode mode)
{
	REAL3 from, to;
	from[0] = p1[0];
	from[1] = p1[1];
	from[2] = p1[2];
	to[0] = p2[0];
	to[1] = p2[1];
	to[2] = p2[2];
	Rotate(from, to, mode);
}
/********************************************************************
 * ASSIGNMENT OPERATOR OVERLOADING(1).
 ********************************************************************/
GQuaternion& GQuaternion::operator =(GQuaternion q)
{
	X = q.X;
	Y = q.Y;
	Z = q.Z;
	W = q.W;
	return (*this);
}
/********************************************************************
 * ASSIGNMENT OPERATOR OVERLOADING(2).
 ********************************************************************/
GQuaternion& GQuaternion::operator = (REAL *q)
{
	SetQuat(q);
	return *this;
}
/********************************************************************
 * MULTIPLICATION OPERATOR OVERLOADING(1).
 ********************************************************************/
GQuaternion GQuaternion::operator *(GQuaternion qR)
{
	GQuaternion qq;
	qq.W = W * qR.W - X * qR.X - Y * qR.Y - Z * qR.Z;
	qq.X = W * qR.X + X * qR.W + Y * qR.Z - Z * qR.Y;
	qq.Y = W * qR.Y + Y * qR.W + Z * qR.X - X * qR.Z;
	qq.Z = W * qR.Z + Z * qR.W + X * qR.Y - Y * qR.X;
	return qq;
}
/********************************************************************
 * MULTIPLICATION OPERATOR OVERLOADING(2).
 ********************************************************************/
GVector3f GQuaternion::operator *(GVector3f p)
{
	GQuaternion qp;
	GVector3f	ret;

	qp.X = p.X;
	qp.Y = p.Y;
	qp.Z = p.Z;

	ret.X = ((*this) * qp / (*this)).X;
	ret.Y = ((*this) * qp / (*this)).Y;
	ret.Z = ((*this) * qp / (*this)).Z;

	return ret;
}
/********************************************************************
 * MULTIPLICATION OPERATOR OVERLOADING(3)
 ********************************************************************/
GQuaternion GQuaternion::operator *(REAL s)
{
	GQuaternion ret;
	ret.X = s * X;
	ret.Y = s * Y;
	ret.Z = s * Z;
	ret.W = s * W;
	return ret;
}
/********************************************************************
 * MULTIPLICATION OPERATOR OVERLOADING(4)
 ********************************************************************/
GQuaternion& GQuaternion::operator *=(GQuaternion qR)
{
	GQuaternion qq;
	qq.W = W * qR.W - X * qR.X - Y * qR.Y - Z * qR.Z;
	qq.X = W * qR.X + X * qR.W + Y * qR.Z - Z * qR.Y;
	qq.Y = W * qR.Y + Y * qR.W + Z * qR.X - X * qR.Z;
	qq.Z = W * qR.Z + Z * qR.W + X * qR.Y - Y * qR.X;
	*this = qq;
	return *this;
}
/********************************************************************
 * MULTIPLICATION OPERATOR OVERLOADING(5)
 ********************************************************************/
GQuaternion& GQuaternion::operator *=(REAL s)
{
	X *= s;
	Y *= s;
	Z *= s;
	W *= s;
	return *this;
}
/********************************************************************
 * ADDITION OPERATOR OVERLOADING(1)
 ********************************************************************/
GQuaternion GQuaternion::operator +(GQuaternion qR)
{
	GQuaternion ret;
	ret.X = X + qR.X;
	ret.Y = Y + qR.Y;
	ret.Z = Z + qR.Z;
	ret.W = W + qR.W;
	return ret;
}
/********************************************************************
 * ADDITION OPERATOR OVERLOADING(2)
 ********************************************************************/
GQuaternion& GQuaternion::operator +=(GQuaternion qR)
{
	X += qR.X;
	Y += qR.Y;
	Z += qR.Z;
	W += qR.W;
	return (*this);
}
/********************************************************************
 * SUBTRACTION OPERATOR OVERLOADING(1)
 ********************************************************************/
GQuaternion GQuaternion::operator -(GQuaternion qR)
{
	GQuaternion ret;
	ret.X = X - qR.X;
	ret.Y = Y - qR.Y;
	ret.Z = Z - qR.Z;
	ret.W = W - qR.W;
	return ret;
}
/********************************************************************
 * SUBTRACTION OPERATOR OVERLOADING(2)
 ********************************************************************/
GQuaternion& GQuaternion::operator -=(GQuaternion qR)
{
	X -= qR.X;
	Y -= qR.Y;
	Z -= qR.Z;
	W -= qR.W;
	return (*this);
}
/********************************************************************
 * DIVISION OPERATOR OVERLOADING(1)
 ********************************************************************/
GQuaternion GQuaternion::operator /(GQuaternion qR)
{
	GQuaternion qR_(qR);
	qR_.Conjugate();
	return *(&((*this) * qR_));
}
/********************************************************************
 * DIVISION OPERATOR OVERLOADING(2)
 ********************************************************************/
GQuaternion& GQuaternion::operator /=(GQuaternion qR)
{
	GQuaternion qR_ = qR;
	qR_.Conjugate();
	// *this = (*this) * qR_;
	(*this) * qR_;
	return *this;
}
/********************************************************************
 * []OPERATOR OVERLOADING
 ********************************************************************/
REAL& GQuaternion::operator [](int i)
{
	assert(i < 4 && i >= 0);
	switch (i)
	{
	case 0:
		return X;
	case 1:
		return Y;
	case 2:
		return Z;
	case 3:
		return W;
	}
	return X;
}
/********************************************************************
 * SCALE FUNCTION
 ********************************************************************/
GQuaternion GQuaternion::Scale(REAL s)
{
	X = s * X;
	Y = s * Y;
	Z = s * Z;
	W = s * W;
	return *this;
}
/*********************************************************
 * ASSIGN QUATERNION VALUE(1)
 *********************************************************/
void GQuaternion::SetQuat(REAL x, REAL y, REAL z, REAL w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}
/*********************************************************
 * ASSIGN QUATERNION VALUE(2)
 *********************************************************/
void GQuaternion::SetQuat(REAL *q)
{
	X = q[0];
	Y = q[1];
	Z = q[2];
	W = q[3];
}
/*********************************************************
 * MAKE QUATERNION VALUE TO BE ZERO
 *********************************************************/
void GQuaternion::Empty()
{
	X = (REAL)0.0;
	Y = (REAL)0.0;
	Z = (REAL)0.0;
	W = (REAL)0.0;
}
/*********************************************************
 * GET QUATERNION NORM
 *********************************************************/
REAL GQuaternion::Norm()
{
	return SQRT(X*X + Y * Y + Z * Z + W * W);
}
/*********************************************************
 * GET DISTANCE TWO QUATERNIONS
 *********************************************************/
REAL GQuaternion::Distance(GQuaternion& q)
{
	GQuaternion qq;
	return ((*this) - q).Norm();
	// return (qq.Norm());
}
/*********************************************************
 * MAKE QUATERNION CONJUGATE
 *********************************************************/
void GQuaternion::Conjugate()
{
	X *= (REAL)(-(REAL)1.0);
	Y *= (REAL)(-(REAL)1.0);
	Z *= (REAL)(-(REAL)1.0);
}
/*********************************************************
 * NORMALIZE QUATERNION
 *********************************************************/
void GQuaternion::Normalize()
{
	REAL norm = Norm();
	X /= norm;
	Y /= norm;
	Z /= norm;
	W /= norm;
}
/*********************************************************
 * CHECK WHETHER QUATERNION IS UNIT QUATERNION OR NOT
 *********************************************************/
bool GQuaternion::IsUnitQuaternion()
{
	REAL norm = Norm();
	if (ABS(norm - (REAL)1.0) < QUAT_EPSILON)
		return true;
	else
		return false;
}
/*********************************************************
 * ???
 *********************************************************/
GQuaternion GQuaternion::GetExp(void)
{
	REAL		norm, s;
	GQuaternion	qe;

	norm = SQRT(X*X + Y * Y + Z * Z);
	if (norm >= QUAT_EPSILON)
	{
		qe.W = COS(norm);
		s = SIN(norm) / norm;
		qe.X = s * X;
		qe.Y = s * Y;
		qe.Z = s * Z;
	}
	else
		qe.W = 1, qe.X = qe.Y = qe.Z = 0;
	return qe;
}
/*********************************************************
 * ???
 *********************************************************/
GQuaternion GQuaternion::GetLog(void)
{
	GQuaternion	ql;
	REAL	theta, s;

	if (ABS(W) >= 0.999999999)
	{
		ql.W = ql.X = ql.Y = ql.Z = 0;
		return ql;
	}
	theta = ACOS(W);
	s = theta / SIN(theta);
	ql.W = 0;
	ql.X = s * X;
	ql.Y = s * Y;
	ql.Z = s * Z;
	return ql;
}
/*********************************************************
 * GET ROTATION ANGLE FOR UNIT QUATERNION.
 *********************************************************/
REAL GQuaternion::GetAngle()
{
	assert(IsUnitQuaternion() == true);
	REAL cs, sn;
	if (ABS(W) >= 0.999999999)
		return 0.0;

	cs = W;
	sn = SQRT(X*X + Y * Y + Z * Z);

	return (REAL)atan2(sn, cs);
}
/*********************************************************
 * GET ROTATION AXIS FOR UNIT QUATERNION(1).
 *********************************************************/
void GQuaternion::GetAxis(REAL *axis)
{
	assert(IsUnitQuaternion() == true);
	REAL norm = SQRT(X*X + Y * Y + Z * Z);
	if (ABS(W) >= 0.999999999)
		REAL_ARR_ZERO(axis, 3);

	axis[0] = X / norm;
	axis[1] = Y / norm;
	axis[2] = Z / norm;
}
/*********************************************************
 * GET ROTATION AXIS FOR UNIT QUATERNION(2).
 *********************************************************/
GVector3f GQuaternion::GetAxis()
{
	assert(IsUnitQuaternion() == true);
	GVector3f axis;
	REAL norm = SQRT(X*X + Y * Y + Z * Z);
	if (ABS(W) >= 0.999999999)
		axis.makeEmpty();
	if (!(ABS(norm) < 0.00001))
	{
		axis.X = X / norm;
		axis.Y = Y / norm;
		axis.Z = Z / norm;
	}
	else
	{
		axis.X = 1.0;
		axis.Y = 0.0;
		axis.Z = 0.0;
	}
	return axis;
}
/*********************************************************
 * GET GENERAL ROTATION MATRIX.
 *********************************************************/
void GQuaternion::GetMatrix(MATRIX mat)
{
	MATRIX_ZERO(mat);
	if (IsUnitQuaternion())
	{
		REAL xx = SQR(X), yy = SQR(Y), zz = SQR(Z), ww = SQR(W);
		REAL Nq = xx + yy + zz + ww;
		REAL wx2 = (REAL)2.0 * W * X, wy2 = (REAL)2.0 * W * Y,
			wz2 = (REAL)2.0 * W * Z, xy2 = (REAL)2.0 * X * Y,
			xz2 = (REAL)2.0 * X * Z, yz2 = (REAL)2.0 * Y * Z;

		mat[0][0] = ww + xx - yy - zz;
		mat[1][0] = xy2 + wz2;
		mat[2][0] = xz2 - wy2;

		mat[0][1] = xy2 - wz2;
		mat[1][1] = ww - xx + yy - zz;
		mat[2][1] = yz2 + wx2;

		mat[0][2] = xz2 + wy2;
		mat[1][2] = yz2 - wx2;
		mat[2][2] = ww - xx - yy + zz;

		mat[0][3] =
			mat[1][3] =
			mat[2][3] =
			mat[3][0] =
			mat[3][1] =
			mat[3][2] = (REAL)0.0;

		mat[3][3] = Nq;
	}
	else
	{
		REAL Nq = X * X + Y * Y + Z * Z + W * W;
		REAL s = (Nq > (REAL)0.0) ? ((REAL)2.0 / Nq) : (REAL)0.0;
		REAL xs = X * s, ys = Y * s, zs = Z * s;
		REAL wx = W * xs, wy = W * ys, wz = W * zs;
		REAL xx = X * xs, xy = X * ys, xz = X * zs;
		REAL yy = Y * ys, yz = Y * zs, zz = Z * zs;


		mat[0][0] = (REAL)1.0 - (yy + zz);
		mat[1][0] = xy + wz;
		mat[2][0] = xz - wy;

		mat[0][1] = xy - wz;
		mat[1][1] = (REAL)1.0 - (xx + zz);
		mat[2][1] = yz + wx;

		mat[0][2] = xz + wy;
		mat[1][2] = yz - wx;
		mat[2][2] = (REAL)1.0 - (xx + yy);

		mat[0][3] =
			mat[1][3] =
			mat[2][3] =
			mat[3][0] =
			mat[3][1] =
			mat[3][2] = (REAL)0.0;

		mat[3][3] = (REAL)1.0;
	}

}
/*********************************************************
 * GET ROTATION MATRIX FOR OPENGL(1)
 *********************************************************/
void GQuaternion::GetGLMatrixd(double *mat)
{
	double(*out)[4] = (double(*)[4])mat;
	if (IsUnitQuaternion())
	{
		double xx = SQR(X), yy = SQR(Y), zz = SQR(Z), ww = SQR(W);
		double Nq = xx + yy + zz + ww;
		double wx2 = 2.0 * W * X, wy2 = 2.0 * W * Y,
			wz2 = 2.0 * W * Z, xy2 = 2.0 * X * Y,
			xz2 = 2.0 * X * Z, yz2 = 2.0 * Y * Z;

		out[0][0] = ww + xx - yy - zz;
		out[0][1] = xy2 + wz2;
		out[0][2] = xz2 - wy2;

		out[1][0] = xy2 - wz2;
		out[1][1] = ww - xx + yy - zz;
		out[1][2] = yz2 + wx2;

		out[2][0] = xz2 + wy2;
		out[2][1] = yz2 - wx2;
		out[2][2] = ww - xx - yy + zz;


		out[0][3] =
			out[1][3] =
			out[2][3] =
			out[3][0] =
			out[3][1] =
			out[3][2] = 0.0;
		out[3][3] = 1.0;
	}
	else
	{
		double Nq = X * X + Y * Y + Z * Z + W * W;
		double s = (Nq > 0.0) ? (2.0 / Nq) : 0.0;
		double xs = X * s, ys = Y * s, zs = Z * s;
		double wx = W * xs, wy = W * ys, wz = W * zs;
		double xx = X * xs, xy = X * ys, xz = X * zs;
		double yy = Y * ys, yz = Y * zs, zz = Z * zs;

		out[0][0] = 1.0 - (yy + zz);
		out[0][1] = xy + wz;
		out[0][2] = xz - wy;

		out[1][0] = xy - wz;
		out[1][1] = 1.0 - (xx + zz);
		out[1][2] = yz + wx;

		out[2][0] = xz + wy;
		out[2][1] = yz - wx;
		out[2][2] = 1.0 - (xx + yy);

		out[0][3] =
			out[1][3] =
			out[2][3] =
			out[3][0] =
			out[3][1] =
			out[3][2] = 0.0;
		out[3][3] = 1.0;
	}

}
/*********************************************************
 * GET ROTATION MATRIX FOR OPENGL(2)
 *********************************************************/
void GQuaternion::GetGLMatrixf(float *mat)
{
	float(*out)[4] = (float(*)[4])mat;
	if (IsUnitQuaternion())
	{
		float xx = SQR(X), yy = SQR(Y), zz = SQR(Z), ww = SQR(W);
		float Nq = xx + yy + zz + ww;
		float wx2 = (REAL)2.0 * W * X, wy2 = (REAL)2.0 * W * Y,
			wz2 = (REAL)2.0 * W * Z, xy2 = (REAL)2.0 * X * Y,
			xz2 = (REAL)2.0 * X * Z, yz2 = (REAL)2.0 * Y * Z;

		out[0][0] = ww + xx - yy - zz;
		out[0][1] = xy2 + wz2;
		out[0][2] = xz2 - wy2;

		out[1][0] = xy2 - wz2;
		out[1][1] = ww - xx + yy - zz;
		out[1][2] = yz2 + wx2;

		out[2][0] = xz2 + wy2;
		out[2][1] = yz2 - wx2;
		out[2][2] = ww - xx - yy + zz;

		out[0][3] =
			out[1][3] =
			out[2][3] =
			out[3][0] =
			out[3][1] =
			out[3][2] = (REAL)0.0;
		out[3][3] = (REAL)1.0;
	}
	else
	{
		float Nq = X * X + Y * Y + Z * Z + W * W;
		float s = (Nq > 0.0) ? ((REAL)2.0 / Nq) : (REAL)0.0;
		float xs = X * s, ys = Y * s, zs = Z * s;
		float wx = W * xs, wy = W * ys, wz = W * zs;
		float xx = X * xs, xy = X * ys, xz = X * zs;
		float yy = Y * ys, yz = Y * zs, zz = Z * zs;

		out[0][0] = (REAL)1.0 - (yy + zz);
		out[0][1] = xy + wz;
		out[0][2] = xz - wy;

		out[1][0] = xy - wz;
		out[1][1] = (REAL)1.0 - (xx + zz);
		out[1][2] = yz + wx;

		out[2][0] = xz + wy;
		out[2][1] = yz - wx;
		out[2][2] = (REAL)1.0 - (xx + yy);

		out[0][3] =
			out[1][3] =
			out[2][3] =
			out[3][0] =
			out[3][1] =
			out[3][2] = (REAL)0.0;
		out[3][3] = (REAL)1.0;
	}
}

void GQuaternion::SetFromAngleAxis(REAL Angle, GVector3f Axis)
{
	Axis.normalize();
	Angle = Angle * (REAL)M_PI / 180.0f;
	REAL theta = Angle * 0.5f;
	REAL sn = (REAL)sin(theta);
	REAL cs = (REAL)cos(theta);

	X = sn * Axis[0];
	Y = sn * Axis[1];
	Z = sn * Axis[2];
	W = cs;
}

/*********************************************************
 * GET DOT-PRODUCT
 *********************************************************/
REAL GQuaternion::Dot(GQuaternion& q)
{
	return (W*q.W + X * q.X + Y * q.Y + Z * q.Z);
}
/*********************************************************
 * PRINT OUT QUATERNION VALUE
 *********************************************************/
void GQuaternion::PrintOut()
{
	printf("GQuaternion(x,y,z,w): %4.2lf %4.2lf %4.2lf %4.2lf\n",
		X, Y, Z, W);
}
