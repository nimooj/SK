#pragma once

#ifndef _GREVOLSRF_H
#define _GREVOLSRF_H

/**********************************************************************
 * Header file of revolved surface class.
 * Implemented by Seung-Hyun Yun.
 * Last update : 2003. 2. 7
 **********************************************************************/

class GRevolSrf : public GNurbsSrf
{
public:
	// constructors and destructor
	GRevolSrf();
	GRevolSrf(GRevolSrf *pSrf);
	GRevolSrf(GRevolSrf &Srf);
	virtual ~GRevolSrf();

	// get and set methods.


	// public interfaces.


	// operator overloading.
	GRevolSrf & operator=(GRevolSrf &Srf);


protected:
	// internal usage.
	bool _alloc();
	bool _free();

private:
	// member attributes.


};

#else 
#endif // _GREVOLSRF_H

