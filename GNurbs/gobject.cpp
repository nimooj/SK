#include "pch.h"
#include "..\usr\include\GNurbs\GNurbs.h"

GObject::GObject()
{
	Origin = GVector3f(0.0f, 0.0f, 0.0f);
	Quat = GQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
}
GObject::~GObject()
{
}

GVector3f GObject::GetOrigin() {
	return Origin;
}

GQuaternion GObject::GetRotation() {
	return Quat;
}

void GObject::SetOrigin(GVector3f orig) {
	Origin = orig;
}

void GObject::SetRotation(GQuaternion q) {
	Quat = q;
}
