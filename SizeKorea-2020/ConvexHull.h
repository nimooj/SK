// ConvexHull.h: interface for the CConvexHull class.
//
//////////////////////////////////////////////////////////////////////

#include "SizeKorea.h"

#if !defined(AFX_CONVEXHULL_H__0B5B64C1_DA77_11D5_9583_005004C068B3__INCLUDED_)
#define AFX_CONVEXHULL_H__0B5B64C1_DA77_11D5_9583_005004C068B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CConvexHull  
{
public:
	CConvexHull();
	CConvexHull( GObList<GVector3f> list );
	virtual ~CConvexHull();
public:
	GObList<GVector3f> m_PtList;
	GObList<GVector3f> m_HullPtList;
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
	void AddUpperHullPtXY( GVector3f a, GVector3f b );
	void AddBelowHullPtXY( GVector3f a, GVector3f b );
	
	void AddUpperHullPtYZ( GVector3f a, GVector3f b );
	void AddBelowHullPtYZ( GVector3f a, GVector3f b );
	void AddUpperHullPtZY( GVector3f a, GVector3f b );
	void AddBelowHullPtZY( GVector3f a, GVector3f b );

	void AddUpperHullPtXZ( GVector3f a, GVector3f b );
	void AddBelowHullPtXZ( GVector3f a, GVector3f b );
};

#endif // !defined(AFX_CONVEXHULL_H__0B5B64C1_DA77_11D5_9583_005004C068B3__INCLUDED_)
