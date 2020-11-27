#pragma once

#include "SizeKorea.h"

// mj::EgConvexhull from ConvexHull.h (2020. 9. 4)

class EgConvexhull  
{
public:
	EgConvexhull();
	EgConvexhull( std::vector<EgPos> list );
	virtual ~EgConvexhull();
public:
	std::vector<EgPos> m_PtList;
	std::vector<EgPos> m_HullPtList;
public:
	void Initialize( void );
	void MakeConvexHullXY( void );
	void MakeConvexHullXZ();
	void MakeConvexHullYZ();
	void ArrangeHullPtXY( void );
	void ArrangeHullPtYX();
	void ArrangeHullPtXZ();
	void ArrangeHullPtZX();
	void ArrangeHullPtYZ();	
	void ArrangeHullPtZY();
	void ArrangeHullPtX();
	void ArrangeHullPtY();
	void ArrangeHullPtZ();
	double GetHullLength();
	double GetHullLength2();
protected:
	void AddUpperHullPtXY( EgPos a, EgPos b );
	void AddBelowHullPtXY( EgPos a, EgPos b );
	
	void AddUpperHullPtYZ( EgPos a, EgPos b );
	void AddBelowHullPtYZ( EgPos a, EgPos b );
	void AddUpperHullPtZY( EgPos a, EgPos b );
	void AddBelowHullPtZY( EgPos a, EgPos b );

	void AddUpperHullPtXZ( EgPos a, EgPos b );
	void AddBelowHullPtXZ( EgPos a, EgPos b );
};

