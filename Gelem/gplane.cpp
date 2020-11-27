#include "pch.h"
//#include "gelem.h"
#include "../usr/include/Gelem/gelem.h"


GPlane::GPlane() {
	d = 0.0f;
}

GPlane::GPlane(GVector3f n, REAL d) {
	this->n = n;
	this->d = d;
}

GPlane::GPlane(GVector3f n, GVector3f o) {
	MakePlane(n, o);
}

GPlane::GPlane(GVector3f a, GVector3f b, GVector3f c) {
	MakePlane(a, b, c);
}

GPlane::GPlane(GPlane* pPln) {
	this->n = pPln->n;
	this->d = pPln->d;
}

GPlane::GPlane(GPlane &Pln) {
	this->n = Pln.n;
	this->d = Pln.d;
}

GPlane::~GPlane() {
}


GPlane& GPlane::operator = (GPlane& Pln) {
	n = Pln.n;
	d = Pln.d;
	return *this;
}


void GPlane::SetNormal(GVector3f n) {
	this->n = n;
}
void GPlane::SetConstant(REAL d) {
	this->d = d;
}

void GPlane::MakePlane(GVector3f n, GVector3f pt) {
	this->n = n;
	this->d = n * pt;
}

void GPlane::MakePlane(GVector3f a, GVector3f b, GVector3f c) {
	GVector3f n = (b - a) ^ (c - a);
	n.normalize();
	MakePlane(n, a);
}

bool GPlane::IsOnPlane(GVector3f pt) {
	if (fabs((REAL)(this->n * pt) - d) < 1.2f)
		return true;
	else
		return false;
}

bool GPlane::IsOnPlane5(GVector3f pt) {
	if (fabs((REAL)(this->n * pt) - d) < 6.0)
		return true;
	else
		return false;
}

bool GPlane::IsAbovePlane(GVector3f pt) {
	if ((REAL)(n * pt) > d)
		return true;
	else
		return false;
}

bool GPlane::IsBelowPlane(GVector3f pt) {
	if ((REAL)(n * pt) < d)
		return true;
	else
		return false;
}

GVector3f GPlane::GetNormal() {
	return n;
}

REAL GPlane::GetConstant() {
	return d;
}

REAL GPlane::GetSignedDistance(GVector3f pt) {
	return (n * pt - d);
}

GVector3f GPlane::GetProjectionPt(GVector3f r, GVector3f ray) {
	float nray = n * ray;
	if (nray != 0.0f) {
		REAL t = (d - n * r) / (n * ray);
		return (r + ray * t);
	}
	else return r;
}

GVector3f GPlane::GetOrigin() {
	return GetProjectionPt(GVector3f(0.0f, 0.0f, 0.0f), n);
}
