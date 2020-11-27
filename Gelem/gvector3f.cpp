#include "pch.h"
//#include "gelem.h"
#include "../usr/include/Gelem/gelem.h"
#include <assert.h>

// Default constructor
GVector3f::GVector3f() {
	X = Y = Z = 0.0;
}

// Copy constructor
GVector3f::GVector3f(const GVector3f& v) {
	X = v.X;
	Y = v.Y;
	Z = v.Z;
}


// General constructor
GVector3f::GVector3f(REAL x, REAL y, REAL z) {
	X = x;
	Y = y;
	Z = z;
}

GVector3f::GVector3f(REAL v[]) {
	X = v[0];
	Y = v[1];
	Z = v[2];
}

// Destructor
GVector3f::~GVector3f() {
}


int GVector3f::operator == (GVector3f& v) {
	if (X == v.X && Y == v.Y && Z == v.Z)
		return 1;
	else
		return 0;
}

int GVector3f::operator != (GVector3f& v) {
	if (X != v.X || Y != v.Y || Z != v.Z)
		return 1;
	else
		return 0;
}

GVector3f& GVector3f::operator += (const GVector3f& v) {
	X += v.X;
	Y += v.Y;
	Z += v.Z;
	return *this;
}

GVector3f& GVector3f::operator -= (const GVector3f& v) {
	X -= v.X;
	Y -= v.Y;
	Z -= v.Z;
	return *this;
}

GVector3f& GVector3f::operator *= (REAL scale) {
	X *= scale;
	Y *= scale;
	Z *= scale;
	return *this;
}

GVector3f& GVector3f::operator ^= (const GVector3f& v) { 
	*this = v ^ *this;
	return *this;
}

// cross product
GVector3f GVector3f::operator ^ (const GVector3f &v) const {
	return GVector3f(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X);
}
// dot product
REAL GVector3f::operator * (GVector3f &v) const {
	REAL retVal = X * v.X + Y * v.Y + Z * v.Z;
	return retVal;
}

GVector3f& GVector3f::operator = (const GVector3f &v) {
	X = v.X;
	Y = v.Y;
	Z = v.Z;
	return *this;
}
GVector3f GVector3f::operator + (const GVector3f &v) {
	GVector3f* retVal = new GVector3f(X + v.X, Y + v.Y, Z + v.Z);
	return *retVal;
}

GVector3f GVector3f::operator - (const GVector3f &v) {
	GVector3f* retVal = new GVector3f(X - v.X, Y - v.Y, Z - v.Z);
	return *retVal;
}

GVector3f GVector3f::operator * (REAL scale) {
	return GVector3f(X * scale, Y * scale, Z * scale);
}

GVector3f GVector3f::operator / (REAL scale) {
	return GVector3f(X / scale, Y / scale, Z / scale);
}


// index operartor overload
REAL& GVector3f::operator[] (int idx) {
	assert(idx >= 0);
	assert(idx <= 2);
	switch (idx) {
		default:
		case 0:
			return X;
			break;
		case 1:
			return Y;
			break;
		case 2:
			return Z;
			break;
	}
}


void GVector3f::normalize() {
	REAL length = this->getNorm();
	X /= length;
	Y /= length;
	Z /= length;
}

REAL GVector3f::getAngle(GVector3f v) {
	GVector3f x = *this;
	REAL valCos, valSin;

	x.normalize();
	v.normalize();

	valCos = x * v;
	valSin = (x ^ v).getNorm();

	return (REAL)atan2(valSin, valCos);
}

int GVector3f::makeEmpty(void) {
	X = Y = Z = 0.0;
	return 1;
}

void GVector3f::setVector(REAL x, REAL y, REAL z) {
	X = x;
	Y = y;
	Z = z;
}

void GVector3f::setVector(REAL v[]) {
	this->setVector(v[0], v[1], v[2]);
}

REAL GVector3f::getNorm() {
	REAL retVal = SQRT(SQR(X) + SQR(Y) + SQR(Z));
	return retVal;
}
