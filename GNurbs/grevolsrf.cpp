#include "pch.h"
#include "..\usr\include\GNurbs\GNurbs.h"

GRevolSrf::GRevolSrf()
{
}
GRevolSrf::GRevolSrf(GRevolSrf *pSrf) : GNurbsSrf(pSrf)
{
}
GRevolSrf::GRevolSrf(GRevolSrf &Srf) : GNurbsSrf(Srf)
{
}
GRevolSrf::~GRevolSrf()
{
	_free();
}

GRevolSrf& GRevolSrf::operator =(GRevolSrf &Srf)
{
	GNurbsSrf::operator =(Srf);
	return (*this);
}
bool GRevolSrf::_alloc()
{
	return true;
}
bool GRevolSrf::_free()
{
	return true;
}

