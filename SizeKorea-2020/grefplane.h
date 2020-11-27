#ifndef _GREFPLANE_H
#define _GREFPLANE_H

#include "SizeKorea.h"

class GRefPlane : public GTrackBall
{
public:
	// constructors and destructor.
	GRefPlane();
	~GRefPlane();

	// set interfaces.
	void SetActive()	{ m_IsSelected = true; };
	void SetInActive()	{ m_IsSelected = false; };

	// get interfaces.
	GPlane GetPlaneEq();
	REAL GetSize()			{ return m_Size; };
	GVector3f GetOrigin()	{ return m_Origin; };
	GVector3f GetLocalCoord( GVector3f pt );
	bool IsActive()			{ return m_IsSelected; };
	bool PtInRect( GVector3f pt );

	// general interface
	void Translate( REAL dx, REAL dy, REAL dz );
	void Scale( int delta );

	//private:
	// attributes.
	GVector3f	m_Origin;
	REAL		m_Size;
	bool		m_IsSelected;
};

#else
#endif //_GREFPLANE_H

