// PickedPt.h: interface for the CPoint3f class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICKEDPT_H__5CC41E41_CD1D_11D5_9583_005004C068B3__INCLUDED_)
#define AFX_PICKEDPT_H__5CC41E41_CD1D_11D5_9583_005004C068B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SizeKorea.h"

class GLandMark  : public GVector3f 
{
public:
	GLandMark();
	GLandMark( REAL x, REAL y, REAL z, bool active = false );
	GLandMark( GVector3f pt, bool active = false );
	~GLandMark();
public:
	GLandMark&	operator=( const GLandMark& Pt );
	bool		operator==( const GLandMark& Pt );
	bool		operator!=( const GLandMark& Pt );
public:
	bool m_bIsPicked;
};

#endif // !defined(AFX_PICKEDPT_H__5CC41E41_CD1D_11D5_9583_005004C068B3__INCLUDED_)
