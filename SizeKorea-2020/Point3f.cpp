// PickedPt.cpp: implementation of the GLandMark class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "SizeKorea-2020.h"
#include "point3f.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLandMark::GLandMark() : GVector3f()
{
	m_bIsPicked = false;
}
GLandMark::GLandMark( REAL x, REAL y, REAL z, bool active ) : GVector3f( x, y, z )
{
	m_bIsPicked = active;
}
GLandMark::GLandMark( GVector3f pt, bool active ) : GVector3f( pt )
{
	m_bIsPicked = active;
}
GLandMark::~GLandMark()
{

}
GLandMark& GLandMark::operator=( const GLandMark& Pt )
{
	this->X = Pt.X;
	this->Y = Pt.Y;
	this->Z	= Pt.Z;
	this->m_bIsPicked = Pt.m_bIsPicked;
	return *this;
}
bool GLandMark::operator==( const GLandMark& Pt )
{
	if ( X == Pt.X && Y == Pt.Y && Z == Pt.Z )
		return true;
	else
		return false;
}
bool GLandMark::operator!=( const GLandMark& Pt )
{
	if ( X == Pt.X && Y == Pt.Y && Z == Pt.Z )
		return false;
	else
		return true;
}
