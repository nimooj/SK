// ConvexHull.cpp: implementation of the CConvexHull class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "SizeKorea-2020.h"
#include "ConvexHull.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConvexHull::CConvexHull()
{

}
CConvexHull::CConvexHull( GObList<GVector3f> list )
{
	m_PtList = list;
}
CConvexHull::~CConvexHull()
{

}
void CConvexHull::MakeConvexHullXY()
{
	// algorithm 
	// 1. first, find two points 
	// 2. collect the upper hull points
	// 3. collect the below hull points
	GVector3f MinPt, MaxPt;
	MinPt.X = 10000.0; MaxPt.X = -10000.0;
	GObNode<GVector3f>* pGObNode = m_PtList.m_pLeftEnd;
	while( pGObNode )
	{
		if( pGObNode->data.X < MinPt.X )
			MinPt = pGObNode->data;
		if( pGObNode->data.X > MaxPt.X )
			MaxPt = pGObNode->data;
		pGObNode = pGObNode->m_pRight;
	}
	m_HullPtList.Insert( MinPt );
	m_HullPtList.Insert( MaxPt );
	AddUpperHullPtXY( MinPt, MaxPt );
	AddBelowHullPtXY( MinPt, MaxPt );

}

void CConvexHull::MakeConvexHullYZ()
{
	// algorithm 
	// 1. first, find two points 
	// 2. collect the upper hull points
	// 3. collect the below hull points
	GVector3f MinPt, MaxPt;
	MinPt.Y = 100000.0; MaxPt.Y = -100000.0;
	GObNode<GVector3f>* pGObNode = m_PtList.m_pLeftEnd;
	while( pGObNode )
	{
		if( pGObNode->data.Y < MinPt.Y )
			MinPt = pGObNode->data;
		if( pGObNode->data.Y > MaxPt.Y )
			MaxPt = pGObNode->data;
		pGObNode = pGObNode->m_pRight;
	}
	m_HullPtList.Insert( MinPt );
	m_HullPtList.Insert( MaxPt );
	AddUpperHullPtYZ( MinPt, MaxPt );
	AddBelowHullPtYZ( MinPt, MaxPt );
}


void CConvexHull::MakeConvexHullXZ()
{
	// algorithm 
	// 1. first, find two points 
	// 2. collect the upper hull points
	// 3. collect the below hull points
	GVector3f MinPt, MaxPt;
	MinPt.X = 10000.0; MaxPt.X = -10000.0;
	GObNode<GVector3f>* pGObNode = m_PtList.m_pLeftEnd;
	while( pGObNode )
	{
		if( pGObNode->data.X < MinPt.X )
			MinPt = pGObNode->data;
		if( pGObNode->data.X > MaxPt.X )
			MaxPt = pGObNode->data;
		pGObNode = pGObNode->m_pRight;
	}
	m_HullPtList.Insert( MinPt );
	m_HullPtList.Insert( MaxPt );
	AddUpperHullPtXZ( MinPt, MaxPt );
	AddBelowHullPtXZ( MinPt, MaxPt );
}

void CConvexHull::AddUpperHullPtXY(GVector3f a, GVector3f b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddUpperHullPt( a, c )를 호출한다.
	// 3. AddUpperHullPt( c, b )를 호출한다.
	GVector3f c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;
	double slope = ( a.Y - b.Y ) / ( a.X - b.X );
	GObNode<GVector3f>* pGObNode = m_PtList.m_pLeftEnd;
	while( pGObNode )
	{
		GVector3f data = pGObNode->data;
		bool k = false;
		GObNode<GVector3f> *pGObNode1 = m_HullPtList.m_pLeftEnd;
		while( pGObNode1 )
		{
			if  ( pGObNode1->data == data )
				k = true;
			pGObNode1 = pGObNode1->m_pRight;
		}
		if ( k )
		{
			pGObNode = pGObNode->m_pRight;
			continue;
		}
		if ( data != a || data != b)
		{
			double sign = slope * ( data.X - a.X ) + a.Y - data.Y;
			if ( sign > 0 )
			{
				IsFounded = true;
				tmp_dis = fabs( slope * data.X - data.Y - slope * a.X + a.Y )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = data;
				}
			}
		}
		pGObNode = pGObNode->m_pRight;
	}
	if ( !IsFounded )
		return;
	if ( c == GVector3f( 0.0, 0.0, 0.0 ) )
		return;
	m_HullPtList.Insert( c );
	AddUpperHullPtXY( a, c );
	AddUpperHullPtXY( c, b );
}
void CConvexHull::AddBelowHullPtXY(GVector3f a, GVector3f b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddBelowHullPt( a, c )를 호출한다.
	// 3. AddBelowHullPt( c, b )를 호출한다.
	GVector3f c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;
	GObNode<GVector3f>* pGObNode = m_PtList.m_pLeftEnd;
	while( pGObNode )
	{
		GVector3f data = pGObNode->data;
		bool k = false;
		GObNode<GVector3f> *pGObNode1 = m_HullPtList.m_pLeftEnd;
		while( pGObNode1 )
		{
			if  ( pGObNode1->data == data )
				k = true;
			pGObNode1 = pGObNode1->m_pRight;
		}
		if ( k )
		{
			pGObNode = pGObNode->m_pRight;
			continue;
		}
		if ( data != a || data != b)
		{
			double slope = ( a.Y - b.Y ) / ( a.X - b.X );
			double sign = slope * ( data.X - a.X ) + a.Y - data.Y;
			if ( sign < 0 )
			{
				IsFounded = true;
				tmp_dis = fabs( slope * data.X - data.Y - slope * a.X + a.Y )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = data;
				}
			}
		}
		pGObNode = pGObNode->m_pRight;
	}
	if ( !IsFounded )
		return;
	if ( c == GVector3f( 0.0, 0.0, 0.0 ) )
		return;
	m_HullPtList.Insert( c );
	AddBelowHullPtXY( a, c );
	AddBelowHullPtXY( c, b );
}
void CConvexHull::AddUpperHullPtYZ(GVector3f a, GVector3f b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddUpperHullPt( a, c )를 호출한다.
	// 3. AddUpperHullPt( c, b )를 호출한다.
	GVector3f c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;
	double slope = ( a.Z - b.Z ) / ( a.Y - b.Y );
	GObNode<GVector3f>* pGObNode = m_PtList.m_pLeftEnd;
	while( pGObNode )
	{
		GVector3f data = pGObNode->data;
		bool k = false;
		GObNode<GVector3f> *pGObNode1 = m_HullPtList.m_pLeftEnd;
		while( pGObNode1 )
		{
			if  ( pGObNode1->data == data )
				k = true;
			pGObNode1 = pGObNode1->m_pRight;
		}
		if ( k )
		{
			pGObNode = pGObNode->m_pRight;
			continue;
		}
		if ( data != a || data != b)
		{
			double sign = slope * ( data.Y - a.Y ) + a.Z - data.Z;
			if ( sign > 0 )
			{
				IsFounded = true;
				tmp_dis = fabs( slope * data.Y - data.Z - slope * a.Y + a.Z )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = data;
				}
			}
		}
		pGObNode = pGObNode->m_pRight;
	}
	if ( !IsFounded )
		return;
	if ( c == GVector3f( 0.0, 0.0, 0.0 ) )
		return;
	m_HullPtList.Insert( c );
	AddUpperHullPtYZ( a, c );
	AddUpperHullPtYZ( c, b );
}
void CConvexHull::AddBelowHullPtYZ(GVector3f a, GVector3f b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddBelowHullPt( a, c )를 호출한다.
	// 3. AddBelowHullPt( c, b )를 호출한다.
	GVector3f c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;
	GObNode<GVector3f>* pGObNode = m_PtList.m_pLeftEnd;
	while( pGObNode )
	{
		GVector3f data = pGObNode->data;
		bool k = false;
		GObNode<GVector3f> *pGObNode1 = m_HullPtList.m_pLeftEnd;
		while( pGObNode1 )
		{
			if  ( pGObNode1->data == data )
				k = true;
			pGObNode1 = pGObNode1->m_pRight;
		}
		if ( k )
		{
			pGObNode = pGObNode->m_pRight;
			continue;
		}
		if ( data != a || data != b)
		{
			double slope = ( a.Z - b.Z ) / ( a.Y - b.Y );
			double sign = slope * ( data.Y - a.Y ) + a.Z - data.Z;
			if ( sign < 0 )
			{
				IsFounded = true;
				tmp_dis = fabs( slope * data.Y - data.Z - slope * a.Y + a.Z )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = data;
				}
			}
		}
		pGObNode = pGObNode->m_pRight;
	}
	if ( !IsFounded )
		return;
	if ( c == GVector3f( 0.0, 0.0, 0.0 ) )
		return;
	m_HullPtList.Insert( c );
	AddBelowHullPtYZ( a, c );
	AddBelowHullPtYZ( c, b );
}

void CConvexHull::AddUpperHullPtZY(GVector3f a, GVector3f b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddUpperHullPt( a, c )를 호출한다.
	// 3. AddUpperHullPt( c, b )를 호출한다.
	GVector3f c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;
	double slope = ( a.Y - b.Y ) / ( a.Z - b.Z );
	GObNode<GVector3f>* pGObNode = m_PtList.m_pLeftEnd;
	while( pGObNode )
	{
		GVector3f data = pGObNode->data;
		bool k = false;
		GObNode<GVector3f> *pGObNode1 = m_HullPtList.m_pLeftEnd;
		while( pGObNode1 )
		{
			if  ( pGObNode1->data == data )
				k = true;
			pGObNode1 = pGObNode1->m_pRight;
		}
		if ( k )
		{
			pGObNode = pGObNode->m_pRight;
			continue;
		}
		if ( data != a || data != b)
		{
			double sign = slope * ( data.Z - a.Z ) + a.Y - data.Y;
			if ( sign > 0 )
			{
				IsFounded = true;
				tmp_dis = fabs( slope * data.Z - data.Y - slope * a.Z + a.Y )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = data;
				}
			}
		}
		pGObNode = pGObNode->m_pRight;
	}
	if ( !IsFounded )
		return;
	if ( c == GVector3f( 0.0, 0.0, 0.0 ) )
		return;
	m_HullPtList.Insert( c );
	AddUpperHullPtYZ( a, c );
	AddUpperHullPtYZ( c, b );
}
void CConvexHull::AddBelowHullPtZY(GVector3f a, GVector3f b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddBelowHullPt( a, c )를 호출한다.
	// 3. AddBelowHullPt( c, b )를 호출한다.
	GVector3f c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;
	GObNode<GVector3f>* pGObNode = m_PtList.m_pLeftEnd;
	while( pGObNode )
	{
		GVector3f data = pGObNode->data;
		bool k = false;
		GObNode<GVector3f> *pGObNode1 = m_HullPtList.m_pLeftEnd;
		while( pGObNode1 )
		{
			if  ( pGObNode1->data == data )
				k = true;
			pGObNode1 = pGObNode1->m_pRight;
		}
		if ( k )
		{
			pGObNode = pGObNode->m_pRight;
			continue;
		}
		if ( data != a || data != b)
		{
			double slope = ( a.Y - b.Y ) / ( a.Z - b.Z );
			double sign = slope * ( data.Z - a.Z ) + a.Y - data.Y;
			if ( sign < 0 )
			{
				IsFounded = true;
				tmp_dis = fabs( slope * data.Z - data.Y - slope * a.Z + a.Y )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = data;
				}
			}
		}
		pGObNode = pGObNode->m_pRight;
	}
	if ( !IsFounded )
		return;
	if ( c == GVector3f( 0.0, 0.0, 0.0 ) )
		return;
	m_HullPtList.Insert( c );
	AddBelowHullPtYZ( a, c );
	AddBelowHullPtYZ( c, b );
}

void CConvexHull::AddUpperHullPtXZ(GVector3f a, GVector3f b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddUpperHullPt( a, c )를 호출한다.
	// 3. AddUpperHullPt( c, b )를 호출한다.
	GVector3f c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;
	double slope = ( a.Z - b.Z ) / ( a.X - b.X );
	GObNode<GVector3f>* pGObNode = m_PtList.m_pLeftEnd;
	while( pGObNode )
	{
		GVector3f data = pGObNode->data;
		bool k = false;
		GObNode<GVector3f> *pGObNode1 = m_HullPtList.m_pLeftEnd;
		while( pGObNode1 )
		{
			if  ( pGObNode1->data == data )
				k = true;
			pGObNode1 = pGObNode1->m_pRight;
		}
		if ( k )
		{
			pGObNode = pGObNode->m_pRight;
			continue;
		}
		if ( data != a || data != b)
		{
			double sign = slope * ( data.X - a.X ) + a.Z - data.Z;
			if ( sign > 0 )
			{
				IsFounded = true;
				tmp_dis = fabs( slope * data.X - data.Z - slope * a.X + a.Z )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = data;
				}
			}
		}
		pGObNode = pGObNode->m_pRight;
	}
	if ( !IsFounded )
		return;
	if ( c == GVector3f( 0.0, 0.0, 0.0 ) )
		return;
	m_HullPtList.Insert( c );
	AddUpperHullPtXZ( a, c );
	AddUpperHullPtXZ( c, b );
}
void CConvexHull::AddBelowHullPtXZ(GVector3f a, GVector3f b)
{
	// algorithm
	// 1. 선분 ab의 위쪽에 있는 점들중 선분 ab로부터의 거리가 최대인 점(c)을 찾는다.
	// 2. AddBelowHullPt( a, c )를 호출한다.
	// 3. AddBelowHullPt( c, b )를 호출한다.
	GVector3f c;
	double dis = -100000.0;
	double tmp_dis;
	bool IsFounded = false;
	GObNode<GVector3f>* pGObNode = m_PtList.m_pLeftEnd;
	while( pGObNode )
	{
		GVector3f data = pGObNode->data;
		bool k = false;
		GObNode<GVector3f> *pGObNode1 = m_HullPtList.m_pLeftEnd;
		while( pGObNode1 )
		{
			if  ( pGObNode1->data == data )
				k = true;
			pGObNode1 = pGObNode1->m_pRight;
		}
		if ( k )
		{
			pGObNode = pGObNode->m_pRight;
			continue;
		}
		if ( data != a || data != b)
		{
			double slope = ( a.Z - b.Z ) / ( a.X - b.X );
			double sign = slope * ( data.X - a.X ) + a.Z - data.Z;
			if ( sign < 0 )
			{
				IsFounded = true;
				tmp_dis = fabs( slope * data.X - data.Z - slope * a.X + a.Z )
								/ sqrt( slope * slope + 1 );
				if ( tmp_dis > dis )
				{
					dis = tmp_dis;
					c = data;
				}
			}
		}
		pGObNode = pGObNode->m_pRight;
	}
	if ( !IsFounded )
		return;
	if ( c == GVector3f( 0.0, 0.0, 0.0 ) )
		return;
	m_HullPtList.Insert( c );
	AddBelowHullPtXZ( a, c );
	AddBelowHullPtXZ( c, b );
}


double CConvexHull::GetHullLength()
{
	double length = 0.0;
	GObNode<GVector3f>* pGObNode = m_HullPtList.m_pLeftEnd;
	GVector3f pt0, pt1;
	while( pGObNode )
	{
		pt0 = pGObNode->data;
		pt1 = pGObNode->m_pRight->data;
		double tmplength = sqrt( (pt0.X - pt1.X) * (pt0.X - pt1.X)
								+(pt0.Y - pt1.Y) * (pt0.Y - pt1.Y)
								+(pt0.Z - pt1.Z) * (pt0.Z - pt1.Z) );
		length += tmplength;
		pGObNode = pGObNode -> m_pRight;
		if ( !pGObNode->m_pRight )
			break;
	}
	pt1 = pGObNode->data;
	pGObNode = m_HullPtList.m_pLeftEnd;
	pt0 = pGObNode->data;
	double tmplength = sqrt( (pt0.X - pt1.X) * (pt0.X - pt1.X)
							+(pt0.Y - pt1.Y) * (pt0.Y - pt1.Y)
							+(pt0.Z - pt1.Z) * (pt0.Z - pt1.Z) );
	length += tmplength;
	return length;
}
double CConvexHull::GetHullLength2()
{
	double length = 0.0;
	GObNode<GVector3f>* pGObNode = m_HullPtList.m_pLeftEnd;
	GVector3f pt0, pt1;
	while( pGObNode )
	{
		pt0 = pGObNode->data;
		pt1 = pGObNode->m_pRight->data;
		double tmplength = sqrt( (pt0.X - pt1.X) * (pt0.X - pt1.X)
								+(pt0.Y - pt1.Y) * (pt0.Y - pt1.Y)
								+(pt0.Z - pt1.Z) * (pt0.Z - pt1.Z) );
		length += tmplength;
		pGObNode = pGObNode -> m_pRight;
		if ( !pGObNode->m_pRight )
			break;
	}
	return length;
}

void CConvexHull::ArrangeHullPtXY()
{
	// Algorithm
	// 1. translate the center point of convex hull point to the origin.
	// 2. rearrange the convex hull point by angle.
	// 3. do inverse action of (1).
	int NumPt = m_HullPtList.m_nCount;
	GVector3f* pTmpPt = new GVector3f[ NumPt ];
	double* pTheta = new double[ NumPt ];

	GObNode<GVector3f>* pGObNode = m_HullPtList.m_pLeftEnd;
	int index = 0;
	while( pGObNode )
	{
		pTmpPt[index] = pGObNode->data;
		pTheta[index] = 0.0;	
		pGObNode = pGObNode -> m_pRight;
		index++;
	}

	float max_x = -100000.0;	float min_x = 100000.0;
	float max_y = -100000.0;	float min_y = 100000.0;
	float center_x, center_y;
	int j, k;
	for ( j = 0; j < NumPt; j++ )
	{
		min_x = MIN( min_x, pTmpPt[j].X );
		max_x = MAX( max_x, pTmpPt[j].X );
		min_y = MIN( min_y, pTmpPt[j].Y );
		max_y = MAX( max_y, pTmpPt[j].Y );
	}
	center_x = ( max_x + min_x ) / 2.0f;
	center_y = ( min_y + max_y ) / 2.0f;
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].X -= center_x;
		pTmpPt[j].Y -= center_y;
		double x = pTmpPt[j].X;
		double y = pTmpPt[j].Y;
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
				GVector3f imsi1 = pTmpPt[k];
				pTmpPt[k] = pTmpPt[j];
				pTmpPt[j] = imsi1;
			}
		}
	}
	m_HullPtList.DeleteAll();
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].X += center_x;
		pTmpPt[j].Y += center_y;
		m_HullPtList.Insert( pTmpPt[j] );
	}
	delete [] pTmpPt;
	delete [] pTheta;
}

void CConvexHull::ArrangeHullPtYX()
{
	// Algorithm
	// 1. translate the center point of convex hull point to the origin.
	// 2. rearrange the convex hull point by angle.
	// 3. do inverse action of (1).
	int NumPt = m_HullPtList.m_nCount;
	GVector3f* pTmpPt = new GVector3f[ NumPt ];
	double* pTheta = new double[ NumPt ];

	GObNode<GVector3f>* pGObNode = m_HullPtList.m_pLeftEnd;
	int index = 0;
	while( pGObNode )
	{
		pTmpPt[index] = pGObNode->data;
		pTheta[index] = 0.0;	
		pGObNode = pGObNode -> m_pRight;
		index++;
	}

	double max_x = -100000.0;	double min_x = 100000.0;
	double max_y = -100000.0;	double min_y = 100000.0;
	double center_x, center_y;
	int j, k;
	for ( j = 0; j < NumPt; j++ )
	{
		min_x = MIN( min_x, pTmpPt[j].X );
		max_x = MAX( max_x, pTmpPt[j].X );
		min_y = MIN( min_y, pTmpPt[j].Y );
		max_y = MAX( max_y, pTmpPt[j].Y );
	}
	center_x = ( max_x + min_x ) / 2.0;
	center_y = ( min_y + max_y ) / 2.0;
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].X -= center_x;
		pTmpPt[j].Y -= center_y;
		double x = pTmpPt[j].X;
		double y = pTmpPt[j].Y;
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
				GVector3f imsi1 = pTmpPt[k];
				pTmpPt[k] = pTmpPt[j];
				pTmpPt[j] = imsi1;
			}
		}
	}
	m_HullPtList.DeleteAll();
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].X += center_x;
		pTmpPt[j].Y += center_y;
		m_HullPtList.Insert( pTmpPt[j] );
	}
	delete [] pTmpPt;
	delete [] pTheta;
}

void CConvexHull::ArrangeHullPtYZ()
{
	// Algorithm
	// 1. translate the center point of convex hull point to the origin.
	// 2. rearrange the convex hull point by angle.
	// 3. do inverse action of (1).
	int NumPt = m_HullPtList.m_nCount;
	GVector3f* pTmpPt = new GVector3f[ NumPt ];
	double* pTheta = new double[ NumPt ];

	GObNode<GVector3f>* pGObNode = m_HullPtList.m_pLeftEnd;
	int index = 0;
	while( pGObNode )
	{
		pTmpPt[index] = pGObNode->data;
		pTheta[index] = 0.0;	
		pGObNode = pGObNode -> m_pRight;
		index++;
	}

	float max_y = -10000.0;	float min_y = 10000.0;
	float max_z = -10000.0;	float min_z = 10000.0;
	float center_y, center_z;
	int j, k;
	for ( j = 0; j < NumPt; j++ )
	{
		min_y = MIN( min_y, pTmpPt[j].Y );
		max_y = MAX( max_y, pTmpPt[j].Y );
		min_z = MIN( min_z, pTmpPt[j].Z );
		max_z = MAX( max_z, pTmpPt[j].Z );
	}
	center_y = ( max_y + min_y ) / 2.0f;
	center_z = ( min_z + max_z ) / 2.0f;
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].Y -= center_y;
		pTmpPt[j].Z -= center_z;
		float y = pTmpPt[j].Y;
		float z = pTmpPt[j].Z;
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
				GVector3f imsi1 = pTmpPt[k];
				pTmpPt[k] = pTmpPt[j];
				pTmpPt[j] = imsi1;
			}
		}
	}
	m_HullPtList.DeleteAll();
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].Y += center_y;
		pTmpPt[j].Z += center_z;
		m_HullPtList.Insert( pTmpPt[j] );
	}
	delete [] pTmpPt;
	delete [] pTheta;
	
}

void CConvexHull::ArrangeHullPtZY()
{
	// Algorithm
	// 1. translate the center point of convex hull point to the origin.
	// 2. rearrange the convex hull point by angle.
	// 3. do inverse action of (1).
	int NumPt = m_HullPtList.m_nCount;
	GVector3f* pTmpPt = new GVector3f[ NumPt ];
	double* pTheta = new double[ NumPt ];

	GObNode<GVector3f>* pGObNode = m_HullPtList.m_pLeftEnd;
	int index = 0;
	while( pGObNode )
	{
		pTmpPt[index] = pGObNode->data;
		pTheta[index] = 0.0;	
		pGObNode = pGObNode -> m_pRight;
		index++;
	}

	float max_y = -10000.0;	float min_y = 10000.0;
	float max_z = -10000.0;	float min_z = 10000.0;
	float center_y, center_z;
	int j, k;
	for ( j = 0; j < NumPt; j++ )
	{
		min_y = MIN( min_y, pTmpPt[j].Y );
		max_y = MAX( max_y, pTmpPt[j].Y );
		min_z = MIN( min_z, pTmpPt[j].Z );
		max_z = MAX( max_z, pTmpPt[j].Z );
	}
	center_y = ( max_y + min_y ) / 2.0;
	center_z = ( min_z + max_z ) / 2.0;

	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].Y -= center_y;
		pTmpPt[j].Z -= center_z;
		double y = pTmpPt[j].Y;
		double z = pTmpPt[j].Z;
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
				GVector3f imsi1 = pTmpPt[k];
				pTmpPt[k] = pTmpPt[j];
				pTmpPt[j] = imsi1;
			}
		}
	}
	m_HullPtList.DeleteAll();
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].Y += center_y;
		pTmpPt[j].Z += center_z;
		m_HullPtList.Insert( pTmpPt[j] );
	}
	delete [] pTmpPt;
	delete [] pTheta;
	
}


void CConvexHull::ArrangeHullPtXZ()
{
	// Algorithm
	// 1. translate the center point of convex hull point to the origin.
	// 2. rearrange the convex hull point by angle.
	// 3. do inverse action of (1).
	int NumPt = m_HullPtList.m_nCount;
	GVector3f* pTmpPt = new GVector3f[ NumPt ];
	double* pTheta = new double[ NumPt ];

	GObNode<GVector3f>* pGObNode = m_HullPtList.m_pLeftEnd;
	int index = 0;
	while( pGObNode )
	{
		pTmpPt[index] = pGObNode->data;
		pTheta[index] = 0.0;	
		pGObNode = pGObNode -> m_pRight;
		index++;
	}

	double max_x = -100000.0;	double min_x = 100000.0;
	double max_z = -100000.0;	double min_z = 100000.0;
	double center_x, center_z;
	int j, k;
	for ( j = 0; j < NumPt; j++ )
	{
		min_x = MIN( min_x, pTmpPt[j].X );
		max_x = MAX( max_x, pTmpPt[j].X );
		min_z = MIN( min_z, pTmpPt[j].Z );
		max_z = MAX( max_z, pTmpPt[j].Z );
	}
	center_x = ( max_x + min_x ) / 2.0;
	center_z = ( min_z + max_z ) / 2.0;
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].X -= center_x;
		pTmpPt[j].Z -= center_z;
		double x = pTmpPt[j].X;
		double z = pTmpPt[j].Z;
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
				GVector3f imsi1 = pTmpPt[k];
				pTmpPt[k] = pTmpPt[j];
				pTmpPt[j] = imsi1;
			}
		}
	}
	m_HullPtList.DeleteAll();
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].X += center_x;
		pTmpPt[j].Z += center_z;
		m_HullPtList.Insert( pTmpPt[j] );
	}
	delete [] pTmpPt;
	delete [] pTheta;
	
}

void CConvexHull::ArrangeHullPtZX()
{
	// Algorithm
	// 1. translate the center point of convex hull point to the origin.
	// 2. rearrange the convex hull point by angle.
	// 3. do inverse action of (1).
	int NumPt = m_HullPtList.m_nCount;
	GVector3f* pTmpPt = new GVector3f[ NumPt ];
	double* pTheta = new double[ NumPt ];

	GObNode<GVector3f>* pGObNode = m_HullPtList.m_pLeftEnd;
	int index = 0;
	while( pGObNode )
	{
		pTmpPt[index] = pGObNode->data;
		pTheta[index] = 0.0;	
		pGObNode = pGObNode -> m_pRight;
		index++;
	}

	double max_x = -100000.0;	double min_x = 100000.0;
	double max_z = -100000.0;	double min_z = 100000.0;
	double center_x, center_z;
	int j, k;
	for ( j = 0; j < NumPt; j++ )
	{
		min_x = MIN( min_x, pTmpPt[j].X );
		max_x = MAX( max_x, pTmpPt[j].X );
		min_z = MIN( min_z, pTmpPt[j].Z );
		max_z = MAX( max_z, pTmpPt[j].Z );
	}
	center_x = ( max_x + min_x ) / 2.0;
	center_z = ( min_z + max_z ) / 2.0;
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].X -= center_x;
		pTmpPt[j].Z -= center_z;
		double x = pTmpPt[j].X;
		double z = pTmpPt[j].Z;
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
				GVector3f imsi1 = pTmpPt[k];
				pTmpPt[k] = pTmpPt[j];
				pTmpPt[j] = imsi1;
			}
		}
	}
	m_HullPtList.DeleteAll();
	for ( j = 0; j < NumPt; j++ )
	{
		pTmpPt[j].X += center_x;
		pTmpPt[j].Z += center_z;
		m_HullPtList.Insert( pTmpPt[j] );
	}
	delete [] pTmpPt;
	delete [] pTheta;
	
}


void CConvexHull::ArrangeHullPtX()
{
	int j, k;
	int NumPt = m_HullPtList.m_nCount;
	GVector3f* pTmpPt = new GVector3f[ NumPt ];
	GObNode<GVector3f>* pGObNode = m_HullPtList.m_pLeftEnd;
	int index = 0;
	while( pGObNode )
	{
		pTmpPt[index] = pGObNode->data;
		pGObNode = pGObNode -> m_pRight;
		index++;
	}
	for ( j = 0; j < NumPt-1; j++ )
	{
		for ( k = j; k < NumPt; k++ )
		{
			if ( pTmpPt[j].X > pTmpPt[k].X )
			{
				GVector3f pt = pTmpPt[j];
				pTmpPt[j] = pTmpPt[k];
				pTmpPt[k] = pt;
			}
		}
	}
	m_HullPtList.DeleteAll();
	for ( j = 0; j < NumPt; j++ ){
	/*	if( j>0 && j<NumPt-1){
			if( ABS(pTmpPt[j].Z-pTmpPt[j-1].Z) > 10.0f ) // ||  ABS(pTmpPt[j].Z-pTmpPt[j+1].Z)>10.0f ) 
				continue;
		}*/
		m_HullPtList.Insert( pTmpPt[j] );
	}
	delete [] pTmpPt;
}
void CConvexHull::ArrangeHullPtY()
{
	int j, k;
	int NumPt = m_HullPtList.m_nCount;
	GVector3f* pTmpPt = new GVector3f[ NumPt ];
	GObNode<GVector3f>* pGObNode = m_HullPtList.m_pLeftEnd;
	int index = 0;
	while( pGObNode )
	{
		pTmpPt[index] = pGObNode->data;
		pGObNode = pGObNode -> m_pRight;
		index++;
	}
	for ( j = 0; j < NumPt-1; j++ )
	{
		for ( k = j; k < NumPt; k++ )
		{
			if ( pTmpPt[j].Y > pTmpPt[k].Y )
			{
				GVector3f pt = pTmpPt[j];
				pTmpPt[j] = pTmpPt[k];
				pTmpPt[k] = pt;
			}
		}
	}
	m_HullPtList.DeleteAll();
	for ( j = 0; j < NumPt; j++ ) {
		if( j>0 && j<NumPt-1){
			if( ABS(pTmpPt[j].Z-pTmpPt[j-1].Z) > 10.0f ) // ||  ABS(pTmpPt[j].Z-pTmpPt[j+1].Z)>10.0f ) 
				continue;
		}
		m_HullPtList.Insert( pTmpPt[j] );
	}
	delete [] pTmpPt;

}
void CConvexHull::ArrangeHullPtZ()
{
	int j, k;
	int NumPt = m_HullPtList.m_nCount;
	GVector3f* pTmpPt = new GVector3f[ NumPt ];
	GObNode<GVector3f>* pGObNode = m_HullPtList.m_pLeftEnd;
	int index = 0;
	while( pGObNode )
	{
		pTmpPt[index] = pGObNode->data;
		pGObNode = pGObNode -> m_pRight;
		index++;
	}
	for ( j = 0; j < NumPt-1; j++ )
	{
		for ( k = j; k < NumPt; k++ )
		{
			if ( pTmpPt[j].Z > pTmpPt[k].Z )
			{
				GVector3f pt = pTmpPt[j];
				pTmpPt[j] = pTmpPt[k];
				pTmpPt[k] = pt;
			}
		}
	}
	m_HullPtList.DeleteAll();
	for ( j = 0; j < NumPt; j++ )
		m_HullPtList.Insert( pTmpPt[j] );
	delete [] pTmpPt;

}
void CConvexHull::Initialize()
{
	m_HullPtList.DeleteAll();
	m_PtList.DeleteAll();
}
