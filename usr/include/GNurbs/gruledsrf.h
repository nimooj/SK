#pragma once
#ifndef _GRULEDSRF_H
#define _GRULEDSRF_H

/**********************************************************************
 * header file of ruled surface class.
 * implemented by Seung-Hyun Yun.
 * last update : 2003. 2. 7
 **********************************************************************/

class GRuledSrf : public GNurbsSrf
{
public:
	// consturctors and desturctor
	GRuledSrf();
	GRuledSrf(GNurbsCrv *C0, GNurbsCrv *C1);
	GRuledSrf(GRuledSrf &Srf);
	GRuledSrf(GRuledSrf *pSrf);
	virtual ~GRuledSrf() { _free(); };

protected:
	// internal usage.
	bool	_alloc();
	bool	_free();

public:
	// set and get methods
	GNurbsCrv *GetStartCrv() { return C0; };
	GNurbsCrv *GetEndCrv() { return C1; };

	// public interfaces.

	// operator overloading.
	GRuledSrf & operator=(GRuledSrf &Srf);

private:
	// member attributes.
	GNurbsCrv *C0;
	GNurbsCrv *C1;
};

#else
#endif // _GRULEDSRF_H

