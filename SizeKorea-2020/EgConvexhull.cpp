#include "pch.h"
#include "EgConvexhull.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EgConvexhull::EgConvexhull()
{

}
EgConvexhull::EgConvexhull( std::vector<EgPos> list )
{
	m_PtList = list;
}

EgConvexhull::~EgConvexhull()
{

}
void EgConvexhull::MakeConvexHullXY()
{
	// algorithm 
	// 1. first, find two points 
	// 2. collect the upper hull points
	// 3. collect the below hull points
	EgPos MinPt, MaxPt;
	MinPt.m_Coords[0] = 10000.0; 
	MaxPt.m_Coords[0] = -10000.0;

	for (EgPos pos : m_PtList) {
		if (pos.m_Coords[0] < MinPt.m_Coords[0])
			MinPt = pos;
		if (pos.m_Coords[0] > MaxPt.m_Coords[0])
			MaxPt = pos;
	}

	m_HullPtList.push_back( MinPt );
	m_HullPtList.push_back( MaxPt );
	AddUpperHullPtXY( MinPt, MaxPt );
	AddBelowHullPtXY( MinPt, MaxPt );

}

void EgConvexhull::MakeConvexHullYZ()
{
	// algorithm 
	// 1. first, find two points 
	// 2. collect the upper hull points
	// 3. collect the below hull points
	EgPos MinPt, MaxPt;
	MinPt.m_Coords[1] = 100000.0; 
	MaxPt.m_Coords[1] = -100000.0;
	for (EgPos pos : m_PtList) {
		if (pos.m_Coords[1] < MinPt.m_Coords[1])
			MinPt = pos;
		if (pos.m_Coords[1] > MaxPt.m_Coords[1])
			MaxPt = pos;
	}

	m_HullPtList.push_back( MinPt );
	m_HullPtList.push_back( MaxPt );
	AddUpperHullPtYZ( MinPt, MaxPt );
	AddBelowHullPtYZ( MinPt, MaxPt );
}


void EgConvexhull::MakeConvexHullXZ()
{
	// algorithm 
	// 1. first, find two points 
	// 2. collect the upper hull points
	// 3. collect the below hull points
	EgPos MinPt, MaxPt;
	MinPt.m_Coords[0] = 10000.0; 
	MaxPt.m_Coords[0] = -10000.0;

	for (EgPos pos : m_PtList) {
		if (pos.m_Coords[0] < MinPt.m_Coords[0])
			MinPt = pos;
		if (pos.m_Coords[0] > MaxPt.m_Coords[0])
			MaxPt = pos;
	}

	m_HullPtList.push_back( MinPt );
	m_HullPtList.push_back( MaxPt );
	AddUpperHullPtXZ( MinPt, MaxPt );
	AddBelowHullPtXZ( MinPt, MaxPt );
}

void EgConvexhull::AddUpperHullPtXY(EgPos a, EgPos b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddUpperHullPt( a, c )를 호출한다.
	// 3. AddUpperHullPt( c, b )를 호출한다.
	EgPos c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;
	double slope = ( a.m_Coords[1] - b.m_Coords[1] ) / ( a.m_Coords[0] - b.m_Coords[0] );

	for (EgPos pos : m_PtList) {
		bool k = false;
		
		for (EgPos pos1 : m_HullPtList) {
			if (pos == pos1)
				k = true;
		}

		if (k) {
			continue;
		}

		if (pos != a || pos != b) {
			double sign = slope * ( pos.m_Coords[0] - a.m_Coords[0] ) + a.m_Coords[1] - pos.m_Coords[1];
			if (sign > 0) {
				IsFounded = true;
				tmp_dis = fabs( slope * pos.m_Coords[0] - pos.m_Coords[1] - slope * a.m_Coords[0] + a.m_Coords[1])
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = pos;
				}
			}
		}
	}

	if ( !IsFounded )
		return;
	if ( c == EgPos() )
		return;

	m_HullPtList.push_back( c );
	AddUpperHullPtXY( a, c );
	AddUpperHullPtXY( c, b );
}

void EgConvexhull::AddBelowHullPtXY(EgPos a, EgPos b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddBelowHullPt( a, c )를 호출한다.
	// 3. AddBelowHullPt( c, b )를 호출한다.
	EgPos c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;

	for (EgPos pos : m_PtList) {
		bool k = false;

		for (EgPos pos1 : m_HullPtList) {
			if (pos1 == pos)
				k = true;
		}

		if (k) {
			continue;
		}

		if (pos != a || pos != b) {
			double slope = ( a.m_Coords[1] - b.m_Coords[1] ) / ( a.m_Coords[0] - b.m_Coords[0] );
			double sign = slope * ( pos.m_Coords[0] - a.m_Coords[0] ) + a.m_Coords[1] - pos.m_Coords[1];

			if ( sign < 0 )
			{
				IsFounded = true;
				tmp_dis = fabs( slope * pos.m_Coords[0] - pos.m_Coords[1] - slope * a.m_Coords[0] + a.m_Coords[1] )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = pos;
				}
			}
		}
	}

	if ( !IsFounded )
		return;
	if ( c == EgPos() )
		return;

	m_HullPtList.push_back( c );
	AddBelowHullPtXY( a, c );
	AddBelowHullPtXY( c, b );
}

void EgConvexhull::AddUpperHullPtYZ(EgPos a, EgPos b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddUpperHullPt( a, c )를 호출한다.
	// 3. AddUpperHullPt( c, b )를 호출한다.
	EgPos c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;
	double slope = ( a.m_Coords[2] - b.m_Coords[2] ) / ( a.m_Coords[1] - b.m_Coords[1] );

	for (EgPos pos : m_PtList) {
		bool k = false;
		
		for (EgPos pos1 : m_HullPtList) {
			if (pos == pos1)
				k = true;
		}

		if (k) {
			continue;
		}

		if (pos != a || pos != b) {
			double sign = slope * ( pos.m_Coords[1] - a.m_Coords[1] ) + a.m_Coords[2] - pos.m_Coords[2];

			if (sign > 0) {
				IsFounded = true;
				tmp_dis = fabs( slope * pos.m_Coords[1] - pos.m_Coords[2] - slope * a.m_Coords[1] + a.m_Coords[2])
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = pos;
				}
			}
		}
	}

	if ( !IsFounded )
		return;
	if ( c == EgPos() )
		return;
	
	m_HullPtList.push_back( c );
	AddUpperHullPtYZ( a, c );
	AddUpperHullPtYZ( c, b );
}

void EgConvexhull::AddBelowHullPtYZ(EgPos a, EgPos b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddBelowHullPt( a, c )를 호출한다.
	// 3. AddBelowHullPt( c, b )를 호출한다.
	EgPos c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;

	for (EgPos pos : m_PtList) {
		bool k = false;

		for (EgPos pos1 : m_HullPtList) {
			if (pos1 == pos)
				k = true;
		}

		if (k) {
			continue;
		}

		if (pos != a || pos != b) {
			double slope = ( a.m_Coords[2] - b.m_Coords[2] ) / ( a.m_Coords[1] - b.m_Coords[1] );
			double sign = slope * ( pos.m_Coords[1] - a.m_Coords[1] ) + a.m_Coords[2] - pos.m_Coords[2];

			if ( sign < 0 )
			{
				IsFounded = true;
				tmp_dis = fabs( slope * pos.m_Coords[1] - pos.m_Coords[2] - slope * a.m_Coords[1] + a.m_Coords[2] )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = pos;
				}
			}
		}
	}


	if ( !IsFounded )
		return;
	if ( c == EgPos() )
		return;
	m_HullPtList.push_back( c );
	AddBelowHullPtYZ( a, c );
	AddBelowHullPtYZ( c, b );
}

void EgConvexhull::AddUpperHullPtZY(EgPos a, EgPos b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddUpperHullPt( a, c )를 호출한다.
	// 3. AddUpperHullPt( c, b )를 호출한다.
	EgPos c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;
	double slope = ( a.m_Coords[1] - b.m_Coords[1]) / ( a.m_Coords[2] - b.m_Coords[2] );

	for (EgPos pos : m_PtList) {
		bool k = false;
		
		for (EgPos pos1 : m_HullPtList) {
			if (pos == pos1)
				k = true;
		}

		if (k) {
			continue;
		}

		if (pos != a || pos != b) {
			double sign = slope * ( pos.m_Coords[2] - a.m_Coords[2] ) + a.m_Coords[1] - pos.m_Coords[1];

			if (sign > 0) {
				IsFounded = true;
				tmp_dis = fabs( slope * pos.m_Coords[2] - pos.m_Coords[1] - slope * a.m_Coords[2] + a.m_Coords[1] )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = pos;
				}
			}
		}
	}


	if ( !IsFounded )
		return;
	if ( c == EgPos() )
		return;
	m_HullPtList.push_back( c );
	AddUpperHullPtYZ( a, c );
	AddUpperHullPtYZ( c, b );
}

void EgConvexhull::AddBelowHullPtZY(EgPos a, EgPos b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddBelowHullPt( a, c )를 호출한다.
	// 3. AddBelowHullPt( c, b )를 호출한다.
	EgPos c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;

	for (EgPos pos : m_PtList) {
		bool k = false;
		for (EgPos pos1 : m_HullPtList) {
			if  ( pos1 == pos )
				k = true;
		}
		if ( k )
		{
			continue;
		}

		if ( pos != a || pos != b)
		{
			double slope = ( a.m_Coords[1] - b.m_Coords[1] ) / ( a.m_Coords[2] - b.m_Coords[2] );
			double sign = slope * ( pos.m_Coords[2] - a.m_Coords[2] ) + a.m_Coords[1] - pos.m_Coords[1];
			if ( sign < 0 )
			{
				IsFounded = true;
				tmp_dis = fabs( slope * pos.m_Coords[2] - pos.m_Coords[1] - slope * a.m_Coords[2] + a.m_Coords[1] )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = pos;
				}
			}
		}
	}

	if ( !IsFounded )
		return;
	if ( c == EgPos() )
		return;
	m_HullPtList.push_back( c );
	AddBelowHullPtYZ( a, c );
	AddBelowHullPtYZ( c, b );
}

void EgConvexhull::AddUpperHullPtXZ(EgPos a, EgPos b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddUpperHullPt( a, c )를 호출한다.
	// 3. AddUpperHullPt( c, b )를 호출한다.
	EgPos c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;
	double slope = ( a.m_Coords[2] - b.m_Coords[2] ) / ( a.m_Coords[0] - b.m_Coords[0] );

	for (EgPos pos : m_PtList) {
		bool k = false;

		for (EgPos pos1 : m_HullPtList) {
			if  ( pos1 == pos )
				k = true;
		}
		if ( k )
		{
			continue;
		}
		if ( pos != a || pos != b)
		{
			double sign = slope * ( pos.m_Coords[0] - a.m_Coords[0] ) + a.m_Coords[2] - pos.m_Coords[2];
			if ( sign > 0 )
			{
				IsFounded = true;
				tmp_dis = fabs( slope * pos.m_Coords[0] - pos.m_Coords[2] - slope * a.m_Coords[0] + a.m_Coords[2] )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = pos;
				}
			}
		}
	}

	if ( !IsFounded )
		return;
	if ( c == EgPos() )
		return;
	m_HullPtList.push_back( c );
	AddUpperHullPtXZ( a, c );
	AddUpperHullPtXZ( c, b );
}
void EgConvexhull::AddBelowHullPtXZ(EgPos a, EgPos b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddBelowHullPt( a, c )를 호출한다.
	// 3. AddBelowHullPt( c, b )를 호출한다.
	EgPos c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;

	for (EgPos pos : m_PtList) {
		bool k = false;

		for (EgPos pos1 : m_HullPtList) {
			if  ( pos1 == pos )
				k = true;
		}
		if ( k )
		{
			continue;
		}

		if ( pos != a || pos != b)
		{
			double slope = ( a.m_Coords[2] - b.m_Coords[2] ) / ( a.m_Coords[0] - b.m_Coords[0] );
			double sign = slope * ( pos.m_Coords[0] - a.m_Coords[0] ) + a.m_Coords[2] - pos.m_Coords[2];
			if ( sign < 0 )
			{
				IsFounded = true;
				tmp_dis = fabs( slope * pos.m_Coords[0] - pos.m_Coords[2] - slope * a.m_Coords[0] + a.m_Coords[2] )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = pos;
				}
			}
		}
	}
	if ( !IsFounded )
		return;
	if ( c == EgPos( 0.0, 0.0, 0.0 ) )
		return;
	m_HullPtList.push_back( c );
	AddBelowHullPtXZ( a, c );
	AddBelowHullPtXZ( c, b );
}


double EgConvexhull::GetHullLength()
{
	double length = 0.0;
	EgPos pt0, pt1;

	for (int i = 0; i < m_HullPtList.size() - 1; i++) {
		pt0 = m_HullPtList[i];
		pt1 = m_HullPtList[i + 1];

		double tmplength = sqrt( (pt0.m_Coords[0] - pt1.m_Coords[0]) * (pt0.m_Coords[0] - pt1.m_Coords[0])
								+(pt0.m_Coords[1] - pt1.m_Coords[1]) * (pt0.m_Coords[1] - pt1.m_Coords[1])
								+(pt0.m_Coords[2] - pt1.m_Coords[2]) * (pt0.m_Coords[2] - pt1.m_Coords[2]) );
		length += tmplength;
	}

	pt1 = m_HullPtList[m_HullPtList.size() - 1];
	pt0 = m_HullPtList[0];

	double tmplength = sqrt( (pt0.m_Coords[0] - pt1.m_Coords[0]) * (pt0.m_Coords[0] - pt1.m_Coords[0])
							+(pt0.m_Coords[1] - pt1.m_Coords[1]) * (pt0.m_Coords[1] - pt1.m_Coords[1])
							+(pt0.m_Coords[2] - pt1.m_Coords[2]) * (pt0.m_Coords[2] - pt1.m_Coords[2]) );
	length += tmplength;
	return length;
}
double EgConvexhull::GetHullLength2()
{
	double length = 0.0;
	EgPos pt0, pt1;

	for (int i = 0; i < m_HullPtList.size() - 1; i++) {
		pt0 = m_HullPtList[i];
		pt1 = m_HullPtList[i + 1];
		double tmplength = sqrt( (pt0.m_Coords[0] - pt1.m_Coords[0]) * (pt0.m_Coords[0] - pt1.m_Coords[0])
								+(pt0.m_Coords[1] - pt1.m_Coords[1]) * (pt0.m_Coords[1] - pt1.m_Coords[1])
								+(pt0.m_Coords[2] - pt1.m_Coords[2]) * (pt0.m_Coords[2] - pt1.m_Coords[2]) );
		length += tmplength;
	}
	return length;
}

void EgConvexhull::ArrangeHullPtXY()
{
	// Algorithm
	// 1. translate the center point of convex hull point to the origin.
	// 2. rearrange the convex hull point by angle.
	// 3. do inverse action of (1).
	int NumPt = m_HullPtList.size();
	EgPos* pTmpPt = new EgPos[ NumPt ];
	double* pTheta = new double[ NumPt ];

	for (int index = 0; index < NumPt; index++) {
		pTmpPt[index] = m_HullPtList[index];
		pTheta[index] = 0.0;	
	}

	float max_x = -100000.0;	float min_x = 100000.0;
	float max_y = -100000.0;	float min_y = 100000.0;
	float center_x, center_y;
	int j, k;
	for ( j = 0; j < NumPt; j++ )
	{
		min_x = MIN( min_x, pTmpPt[j].m_Coords[0] );
		max_x = MAX( max_x, pTmpPt[j].m_Coords[0] );
		min_y = MIN( min_y, pTmpPt[j].m_Coords[1] );
		max_y = MAX( max_y, pTmpPt[j].m_Coords[1] );
	}
	center_x = ( max_x + min_x ) / 2.0f;
	center_y = ( min_y + max_y ) / 2.0f;
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].m_Coords[0] -= center_x;
		pTmpPt[j].m_Coords[1] -= center_y;
		double x = pTmpPt[j].m_Coords[0];
		double y = pTmpPt[j].m_Coords[1];
		double angle = acos( x / sqrt( x*x + y*y ) );
		if ( x > 0 && y > 0 )
			pTheta[j] = angle;
		if ( x < 0 && y > 0 )
			pTheta[j] = angle;
		if ( x > 0 && y < 0 )
			pTheta[j] =  2 * M_PI - angle;
		if ( x < 0 && y < 0 )
			pTheta[j] = 2 * M_PI - angle ;
	}
	for ( j = 0; j < NumPt - 1; j++ )
	{
		for ( k = j + 1; k < NumPt; k ++)
		{
			if ( pTheta[j] > pTheta[k] )
			{
				double imsi = pTheta[k];
				pTheta[k] = pTheta[j];
				pTheta[j] = imsi;
				EgPos imsi1 = pTmpPt[k];
				pTmpPt[k] = pTmpPt[j];
				pTmpPt[j] = imsi1;
			}
		}
	}
	m_HullPtList.clear();
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].m_Coords[0] += center_x;
		pTmpPt[j].m_Coords[1] += center_y;
		m_HullPtList.push_back( pTmpPt[j] );
	}
	delete [] pTmpPt;
	delete [] pTheta;
}

void EgConvexhull::ArrangeHullPtYX()
{
	// Algorithm
	// 1. translate the center point of convex hull point to the origin.
	// 2. rearrange the convex hull point by angle.
	// 3. do inverse action of (1).
	int NumPt = m_HullPtList.size();
	EgPos* pTmpPt = new EgPos[ NumPt ];
	double* pTheta = new double[ NumPt ];

	for (int index = 0; index < NumPt; index++) {
		pTmpPt[index] = m_HullPtList[index];
		pTheta[index] = 0.0;	
	}

	double max_x = -100000.0;	double min_x = 100000.0;
	double max_y = -100000.0;	double min_y = 100000.0;
	double center_x, center_y;
	int j, k;
	for ( j = 0; j < NumPt; j++ )
	{
		min_x = MIN( min_x, pTmpPt[j].m_Coords[0] );
		max_x = MAX( max_x, pTmpPt[j].m_Coords[0] );
		min_y = MIN( min_y, pTmpPt[j].m_Coords[1] );
		max_y = MAX( max_y, pTmpPt[j].m_Coords[1] );
	}
	center_x = ( max_x + min_x ) / 2.0;
	center_y = ( min_y + max_y ) / 2.0;
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].m_Coords[0] -= center_x;
		pTmpPt[j].m_Coords[1] -= center_y;
		double x = pTmpPt[j].m_Coords[0];
		double y = pTmpPt[j].m_Coords[1];
		double angle = acos( y / sqrt( x*x + y*y ) );
		if ( x > 0 && y > 0 )
			pTheta[j] = angle;
		if ( x > 0 && y < 0 )
			pTheta[j] = angle;
		if ( x < 0 && y < 0 )
			pTheta[j] =  2 * M_PI - angle;
		if ( x < 0 && y > 0 )
			pTheta[j] = 2 * M_PI - angle ;
	}
	for ( j = 0; j < NumPt - 1; j++ )
	{
		for ( k = j + 1; k < NumPt; k ++)
		{
			if ( pTheta[j] > pTheta[k] )
			{
				double imsi = pTheta[k];
				pTheta[k] = pTheta[j];
				pTheta[j] = imsi;
				EgPos imsi1 = pTmpPt[k];
				pTmpPt[k] = pTmpPt[j];
				pTmpPt[j] = imsi1;
			}
		}
	}
	m_HullPtList.clear();
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].m_Coords[0] += center_x;
		pTmpPt[j].m_Coords[1] += center_y;
		m_HullPtList.push_back( pTmpPt[j] );
	}
	delete [] pTmpPt;
	delete [] pTheta;
}

void EgConvexhull::ArrangeHullPtYZ()
{
	// Algorithm
	// 1. translate the center point of convex hull point to the origin.
	// 2. rearrange the convex hull point by angle.
	// 3. do inverse action of (1).
	int NumPt = m_HullPtList.size();
	EgPos* pTmpPt = new EgPos[ NumPt ];
	double* pTheta = new double[ NumPt ];

	for (int index = 0; index < NumPt; index++) {
		pTmpPt[index] = m_HullPtList[index];
		pTheta[index] = 0.0;	
	}

	float max_y = -10000.0;	float min_y = 10000.0;
	float max_z = -10000.0;	float min_z = 10000.0;
	float center_y, center_z;
	int j, k;
	for ( j = 0; j < NumPt; j++ )
	{
		min_y = MIN( min_y, pTmpPt[j].m_Coords[1] );
		max_y = MAX( max_y, pTmpPt[j].m_Coords[1] );
		min_z = MIN( min_z, pTmpPt[j].m_Coords[2] );
		max_z = MAX( max_z, pTmpPt[j].m_Coords[2] );
	}
	center_y = ( max_y + min_y ) / 2.0f;
	center_z = ( min_z + max_z ) / 2.0f;
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].m_Coords[1] -= center_y;
		pTmpPt[j].m_Coords[2] -= center_z;
		float y = pTmpPt[j].m_Coords[1];
		float z = pTmpPt[j].m_Coords[2];
		float angle = acos( y / sqrt( y*y + z*z ) );
		if ( y > 0 && z > 0 )
			pTheta[j] = angle;
		if ( y < 0 && z > 0 )
			pTheta[j] = angle;
		if ( y > 0 && z < 0 )
			pTheta[j] =  2 * M_PI - angle;
		if ( y < 0 && z < 0 )
			pTheta[j] = 2 * M_PI - angle ;
	}
	for ( j = 0; j < NumPt - 1; j++ )
	{
		for ( k = j + 1; k < NumPt; k ++)
		{
			if ( pTheta[j] > pTheta[k] )
			{
				float imsi = pTheta[k];
				pTheta[k] = pTheta[j];
				pTheta[j] = imsi;
				EgPos imsi1 = pTmpPt[k];
				pTmpPt[k] = pTmpPt[j];
				pTmpPt[j] = imsi1;
			}
		}
	}
	m_HullPtList.clear();
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].m_Coords[1] += center_y;
		pTmpPt[j].m_Coords[2] += center_z;
		m_HullPtList.push_back( pTmpPt[j] );
	}
	delete [] pTmpPt;
	delete [] pTheta;
	
}

void EgConvexhull::ArrangeHullPtZY()
{
	// Algorithm
	// 1. translate the center point of convex hull point to the origin.
	// 2. rearrange the convex hull point by angle.
	// 3. do inverse action of (1).
	int NumPt = m_HullPtList.size();
	EgPos* pTmpPt = new EgPos[ NumPt ];
	double* pTheta = new double[ NumPt ];

	for (int index = 0; index < NumPt; index++) {
		pTmpPt[index] = m_HullPtList[index];
		pTheta[index] = 0.0;	
	}

	float max_y = -10000.0;	float min_y = 10000.0;
	float max_z = -10000.0;	float min_z = 10000.0;
	float center_y, center_z;
	int j, k;
	for ( j = 0; j < NumPt; j++ )
	{
		min_y = MIN( min_y, pTmpPt[j].m_Coords[1] );
		max_y = MAX( max_y, pTmpPt[j].m_Coords[1] );
		min_z = MIN( min_z, pTmpPt[j].m_Coords[2] );
		max_z = MAX( max_z, pTmpPt[j].m_Coords[2] );
	}
	center_y = ( max_y + min_y ) / 2.0;
	center_z = ( min_z + max_z ) / 2.0;

	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].m_Coords[1] -= center_y;
		pTmpPt[j].m_Coords[2] -= center_z;
		double y = pTmpPt[j].m_Coords[1];
		double z = pTmpPt[j].m_Coords[2];
		double angle = acos( z / sqrt( y*y + z*z ) );
		if ( y > 0 && z > 0 )
			pTheta[j] = angle;
		if ( y > 0 && z < 0 )
			pTheta[j] = angle;
		if ( y < 0 && z < 0 )
			pTheta[j] =  2 * M_PI - angle;
		if ( y < 0 && z > 0 )
			pTheta[j] = 2 * M_PI - angle ;
	}

	for ( j = 0; j < NumPt - 1; j++ )
	{
		for ( k = j + 1; k < NumPt; k ++)
		{
			if ( pTheta[j] > pTheta[k] )
			{
				double imsi = pTheta[k];
				pTheta[k] = pTheta[j];
				pTheta[j] = imsi;
				EgPos imsi1 = pTmpPt[k];
				pTmpPt[k] = pTmpPt[j];
				pTmpPt[j] = imsi1;
			}
		}
	}
	m_HullPtList.clear();
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].m_Coords[1] += center_y;
		pTmpPt[j].m_Coords[2] += center_z;
		m_HullPtList.push_back( pTmpPt[j] );
	}
	delete [] pTmpPt;
	delete [] pTheta;
	
}


void EgConvexhull::ArrangeHullPtXZ()
{
	// Algorithm
	// 1. translate the center point of convex hull point to the origin.
	// 2. rearrange the convex hull point by angle.
	// 3. do inverse action of (1).
	int NumPt = m_HullPtList.size();
	EgPos* pTmpPt = new EgPos[ NumPt ];
	double* pTheta = new double[ NumPt ];

	for (int index = 0; index < NumPt; index++) {
		pTmpPt[index] = m_HullPtList[index];
		pTheta[index] = 0.0;	
	}

	double max_x = -100000.0;	double min_x = 100000.0;
	double max_z = -100000.0;	double min_z = 100000.0;
	double center_x, center_z;
	int j, k;
	for ( j = 0; j < NumPt; j++ )
	{
		min_x = MIN( min_x, pTmpPt[j].m_Coords[0] );
		max_x = MAX( max_x, pTmpPt[j].m_Coords[0] );
		min_z = MIN( min_z, pTmpPt[j].m_Coords[2] );
		max_z = MAX( max_z, pTmpPt[j].m_Coords[2] );
	}
	center_x = ( max_x + min_x ) / 2.0;
	center_z = ( min_z + max_z ) / 2.0;
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].m_Coords[0] -= center_x;
		pTmpPt[j].m_Coords[2] -= center_z;
		double x = pTmpPt[j].m_Coords[0];
		double z = pTmpPt[j].m_Coords[2];
		double angle = acos( x / sqrt( x*x + z*z ) );
		if ( x > 0 && z > 0 )
			pTheta[j] = angle;
		if ( x < 0 && z > 0 )
			pTheta[j] = angle;
		if ( x > 0 && z < 0 )
			pTheta[j] =  2 * M_PI - angle;
		if ( x < 0 && z < 0 )
			pTheta[j] = 2 * M_PI - angle ;
	}
	for ( j = 0; j < NumPt - 1; j++ )
	{
		for ( k = j + 1; k < NumPt; k ++)
		{
			if ( pTheta[j] > pTheta[k] )
			{
				double imsi = pTheta[k];
				pTheta[k] = pTheta[j];
				pTheta[j] = imsi;
				EgPos imsi1 = pTmpPt[k];
				pTmpPt[k] = pTmpPt[j];
				pTmpPt[j] = imsi1;
			}
		}
	}
	m_HullPtList.clear();
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].m_Coords[0] += center_x;
		pTmpPt[j].m_Coords[2] += center_z;
		m_HullPtList.push_back( pTmpPt[j] );
	}
	delete [] pTmpPt;
	delete [] pTheta;
	
}

void EgConvexhull::ArrangeHullPtZX()
{
	// Algorithm
	// 1. translate the center point of convex hull point to the origin.
	// 2. rearrange the convex hull point by angle.
	// 3. do inverse action of (1).
	int NumPt = m_HullPtList.size();
	EgPos* pTmpPt = new EgPos[ NumPt ];
	double* pTheta = new double[ NumPt ];

	for (int index = 0; index < m_HullPtList.size(); index++) {
		pTmpPt[index] = m_HullPtList[index];
		pTheta[index] = 0.0;	
	}

	double max_x = -100000.0;	double min_x = 100000.0;
	double max_z = -100000.0;	double min_z = 100000.0;
	double center_x, center_z;
	int j, k;
	for ( j = 0; j < NumPt; j++ )
	{
		min_x = MIN( min_x, pTmpPt[j].m_Coords[0] );
		max_x = MAX( max_x, pTmpPt[j].m_Coords[0] );
		min_z = MIN( min_z, pTmpPt[j].m_Coords[2] );
		max_z = MAX( max_z, pTmpPt[j].m_Coords[2] );
	}
	center_x = ( max_x + min_x ) / 2.0;
	center_z = ( min_z + max_z ) / 2.0;
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].m_Coords[0] -= center_x;
		pTmpPt[j].m_Coords[2] -= center_z;
		double x = pTmpPt[j].m_Coords[0];
		double z = pTmpPt[j].m_Coords[2];
		double angle = acos( z / sqrt( x*x + z*z ) );
		if ( x > 0 && z > 0 )
			pTheta[j] = angle;
		if ( x > 0 && z < 0 )
			pTheta[j] = angle;
		if ( x < 0 && z < 0 )
			pTheta[j] =  2 * M_PI - angle;
		if ( x < 0 && z > 0 )
			pTheta[j] = 2 * M_PI - angle ;
	}
	for ( j = 0; j < NumPt - 1; j++ )
	{
		for ( k = j + 1; k < NumPt; k ++)
		{
			if ( pTheta[j] > pTheta[k] )
			{
				double imsi = pTheta[k];
				pTheta[k] = pTheta[j];
				pTheta[j] = imsi;
				EgPos imsi1 = pTmpPt[k];
				pTmpPt[k] = pTmpPt[j];
				pTmpPt[j] = imsi1;
			}
		}
	}
	m_HullPtList.clear();
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].m_Coords[0] += center_x;
		pTmpPt[j].m_Coords[2] += center_z;
		m_HullPtList.push_back( pTmpPt[j] );
	}
	delete [] pTmpPt;
	delete [] pTheta;
	
}


void EgConvexhull::ArrangeHullPtX()
{
	int j, k;
	int NumPt = m_HullPtList.size();
	EgPos* pTmpPt = new EgPos[ NumPt ];
	for (int index = 0; index < NumPt; index++) {
		pTmpPt[index] = m_HullPtList[index];
	}

	for ( j = 0; j < NumPt-1; j++ )
	{
		for ( k = j; k < NumPt; k++ )
		{
			if ( pTmpPt[j].m_Coords[0] > pTmpPt[k].m_Coords[0] )
			{
				EgPos pt = pTmpPt[j];
				pTmpPt[j] = pTmpPt[k];
				pTmpPt[k] = pt;
			}
		}
	}
	m_HullPtList.clear();
	for ( j = 0; j < NumPt; j++ ){
	/*	if( j>0 && j<NumPt-1){
			if( ABS(pTmpPt[j].m_Coords[2]-pTmpPt[j-1].m_Coords[2]) > 10.0f ) // ||  ABS(pTmpPt[j].m_Coords[2]-pTmpPt[j+1].m_Coords[2])>10.0f ) 
				continue;
		}*/
		m_HullPtList.push_back( pTmpPt[j] );
	}
	delete [] pTmpPt;
}
void EgConvexhull::ArrangeHullPtY()
{
	int j, k;
	int NumPt = m_HullPtList.size();
	EgPos* pTmpPt = new EgPos[ NumPt ];

	for (int index = 0; index < NumPt; index++) {
		pTmpPt[index] = m_HullPtList[index];
	}

	for ( j = 0; j < NumPt-1; j++ )
	{
		for ( k = j; k < NumPt; k++ )
		{
			if ( pTmpPt[j].m_Coords[1] > pTmpPt[k].m_Coords[1] )
			{
				EgPos pt = pTmpPt[j];
				pTmpPt[j] = pTmpPt[k];
				pTmpPt[k] = pt;
			}
		}
	}
	m_HullPtList.clear();
	for ( j = 0; j < NumPt; j++ ) {
		if( j>0 && j<NumPt-1){
			if( ABS(pTmpPt[j].m_Coords[2]-pTmpPt[j-1].m_Coords[2]) > 10.0f ) // ||  ABS(pTmpPt[j].m_Coords[2]-pTmpPt[j+1].m_Coords[2])>10.0f ) 
				continue;
		}
		m_HullPtList.push_back( pTmpPt[j] );
	}
	delete [] pTmpPt;

}
void EgConvexhull::ArrangeHullPtZ()
{
	int j, k;
	int NumPt = m_HullPtList.size();
	EgPos* pTmpPt = new EgPos[ NumPt ];

	for (int index = 0; index < NumPt; index++) {
		pTmpPt[index] = m_HullPtList[index];
	}
	for ( j = 0; j < NumPt-1; j++ )
	{
		for ( k = j; k < NumPt; k++ )
		{
			if ( pTmpPt[j].m_Coords[2] > pTmpPt[k].m_Coords[2] )
			{
				EgPos pt = pTmpPt[j];
				pTmpPt[j] = pTmpPt[k];
				pTmpPt[k] = pt;
			}
		}
	}
	m_HullPtList.clear();
	for ( j = 0; j < NumPt; j++ )
		m_HullPtList.push_back( pTmpPt[j] );
	delete [] pTmpPt;

}
void EgConvexhull::Initialize()
{
	m_HullPtList.clear();
	m_PtList.clear();
}