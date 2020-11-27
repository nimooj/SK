#include "pch.h"
#include "grefplane.h"

GRefPlane::GRefPlane()
{
	m_Origin = GVector3f( 0.0f, 0.0f, 0.0f );
	m_Size = 500.0f;
	m_IsSelected = false;
}
GRefPlane::~GRefPlane()
{
}

GPlane GRefPlane::GetPlaneEq()
{
	GPlane pln;
	float mat[16];
	this->quat.GetGLMatrixf( mat );
	GVector3f n = GVector3f( mat[4], mat[5], mat[6] );
	pln.MakePlane( n, m_Origin );
	return pln;
}
void GRefPlane::Translate( REAL dx, REAL dy, REAL dz )
{
	m_Origin[0] += dx; 
	m_Origin[1] += dy; 
	m_Origin[2] += dz;
}
void GRefPlane::Scale( int delta )
{
	m_Size += delta;
}
bool GRefPlane::PtInRect( GVector3f pt )
{
	GVector3f tmp = pt - m_Origin;
	if ( tmp.getNorm() < m_Size )
		return true;
	else
		return false;
}
GVector3f GRefPlane::GetLocalCoord( GVector3f pt )
{
	float mat[16];
	quat.GetGLMatrixf( mat );
	REAL x = mat[0] * pt[0] + mat[1] * pt[1] + mat[2] * pt[2];
	REAL y = mat[4] * pt[0] + mat[5] * pt[1] + mat[6] * pt[2];
	REAL z = mat[8] * pt[0] + mat[9] * pt[1] + mat[10] * pt[2];
	return GVector3f( x, y, z );
}
