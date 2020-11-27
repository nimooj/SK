#include "pch.h"
#include "..\usr\include\GNurbs\GNurbs.h"

GRuledSrf::GRuledSrf()
{
	C0 = C1 = NULL;
}

GRuledSrf::GRuledSrf(GNurbsCrv *C0, GNurbsCrv *C1)
{
	this->C0 = new GNurbsCrv(C0);
	this->C1 = new GNurbsCrv(C1);
	_alloc();
	REAL4 *P0 = this->C0->GetCtlPt();
	REAL4 *P1 = this->C1->GetCtlPt();
	REAL4_ARR_COPY(*(this->P[0]), P0, n + 1);
	REAL4_ARR_COPY(*(this->P[1]), P1, n + 1);
}

GRuledSrf::GRuledSrf(GRuledSrf *pSrf) : GNurbsSrf(pSrf)
{
	this->C0 = new GNurbsCrv(pSrf->C0);
	this->C1 = new GNurbsCrv(pSrf->C1);
}

GRuledSrf::GRuledSrf(GRuledSrf &Srf) : GNurbsSrf(Srf)
{
	this->C0 = new GNurbsCrv(Srf.C0);
	this->C1 = new GNurbsCrv(Srf.C1);
}

GRuledSrf& GRuledSrf::operator =(GRuledSrf &Srf)
{
	GNurbsSrf::operator =(Srf);
	this->C0 = new GNurbsCrv(Srf.C0);
	this->C1 = new GNurbsCrv(Srf.C1);
	return (*this);
}

bool GRuledSrf::_free()
{
	if (C0 && C1)
	{
		delete C0;
		delete C1;
		C0 = NULL;
		C1 = NULL;
	}
	return true;
}
bool GRuledSrf::_alloc()
{
	C0->MakeDegCompatible(C1);
	C0->MakeKnotCompatible(C1);
	short p = 1;
	short q = C0->GetDegree();
	short m = 1;
	short n = C0->GetCtlPtIdx();
	GNurbsSrf::_alloc(p, m, q, n);
	return true;
}