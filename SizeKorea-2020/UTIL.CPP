#include "pch.h"
#include "util.h"

int trimax = 10000;

bool getBaricentricCoord( GVector3f a, GVector3f b, GVector3f c, GVector3f p, double *u, double *v, double *w )
{
	double abc = ( (a.X*b.Y*c.Z + a.Y*b.Z*c.X + b.X*c.Y*a.Z) 
				 - (c.X*b.Y*a.Z + b.X*a.Y*c.Z + c.Y*b.Z*a.X) );
	double pbc = ( (p.X*b.Y*c.Z + p.Y*b.Z*c.X + b.X*c.Y*p.Z) 
				 - (c.X*b.Y*p.Z + b.X*p.Y*c.Z + c.Y*b.Z*p.X) );
	double apc = ( (a.X*p.Y*c.Z + a.Y*p.Z*c.X + p.X*c.Y*a.Z) 
				 - (c.X*p.Y*a.Z + p.X*a.Y*c.Z + c.Y*p.Z*a.X) );
	double abp = ( (a.X*b.Y*p.Z + a.Y*b.Z*p.X + b.X*p.Y*a.Z) 
				 - (p.X*b.Y*a.Z + b.X*a.Y*p.Z + p.Y*b.Z*a.X) );
	*u = pbc / abc; *v = apc / abc; *w = abp / abc;
	if ( *u > 0.0 && *v > 0.0 && *w > 0.0 )
		return true;
	return false;
}

void getSortedPoint( GObList<GVector3f> &PtList, int axis, int stype )
{
	// axis
	// 0 : sort about x-axis.
	// 1 : sort about y-axis.
	// 2 : sort about z-axis.
	// stype 
	// 0 : less->greater
	// 1 : greater->less
	int j, k;
	int NumPt = PtList.m_nCount;
	GVector3f* pTmpPt = new GVector3f[ NumPt ];
	GObNode<GVector3f>* pNode = PtList.m_pLeftEnd;
	int index = 0;
	while( pNode )
	{
		pTmpPt[index] = pNode->data;
		pNode = pNode -> m_pRight;
		index++;
	}
	for ( j = 0; j < NumPt-1; j++ )
	{
		for ( k = j; k < NumPt; k++ )
		{
			GVector3f tmp;
			switch( axis )
			{
			case 0: // x-axis
				switch( stype )
				{
				case 0:
					if ( pTmpPt[j].X > pTmpPt[k].X )
					{	
						tmp = pTmpPt[j];
						pTmpPt[j] = pTmpPt[k];
						pTmpPt[k] = tmp;
					}
					break;
				case 1:
					if ( pTmpPt[j].X < pTmpPt[k].X )
					{	
						tmp = pTmpPt[j];
						pTmpPt[j] = pTmpPt[k];
						pTmpPt[k] = tmp;
					}
					break;
				}
				break;
			case 1: // y-axis
				switch( stype )
				{
				case 0:
					if ( pTmpPt[j].Y > pTmpPt[k].Y )
					{	
						tmp = pTmpPt[j];
						pTmpPt[j] = pTmpPt[k];
						pTmpPt[k] = tmp;
					}
					break;
				case 1:
					if ( pTmpPt[j].Y < pTmpPt[k].Y )
					{	
						tmp = pTmpPt[j];
						pTmpPt[j] = pTmpPt[k];
						pTmpPt[k] = tmp;
					}
					break;
				}
				break;
			case 2: // z-axis
				switch( stype )
				{
				case 0:
					if ( pTmpPt[j].Z > pTmpPt[k].Z )
					{	
						tmp = pTmpPt[j];
						pTmpPt[j] = pTmpPt[k];
						pTmpPt[k] = tmp;
					}
					break;
				case 1:
					if ( pTmpPt[j].Z < pTmpPt[k].Z )
					{	
						tmp = pTmpPt[j];
						pTmpPt[j] = pTmpPt[k];
						pTmpPt[k] = tmp;
					}
					break;
				}
				break;
			}
			
		}
	}
	PtList.DeleteAll();
	for ( j = 0; j < NumPt; j++ )
		PtList.Insert( pTmpPt[j] );
	delete [] pTmpPt;
}

float getSortedPtLen( GObList<GVector3f> &PtList )
{
	float length = 0.0;
	GObNode<GVector3f>* pNode = PtList.m_pLeftEnd;
	GVector3f pt0, pt1;
	while( pNode )
	{
		pt0 = pNode->data;
		pt1 = pNode->m_pRight->data;
		float tmplength = sqrt( (pt0.X - pt1.X) * (pt0.X - pt1.X)
								+(pt0.Y - pt1.Y) * (pt0.Y - pt1.Y)
								+(pt0.Z - pt1.Z) * (pt0.Z - pt1.Z) );
		length += tmplength;
		pNode = pNode -> m_pRight;
		if ( !pNode->m_pRight )
			break;
	}
	return length;
}

//
//void getSortedAnglePoint(GObList<GVector3f> &PtList, int plane, double *CX, double *CY, double *CZ)
//{
//	// Algorithm
//	// 1. translate the center point of convex hull point to the origin.
//	// 2. rearrange the convex hull point by angle.
//	// 3. do inverse action of (1).
//	// plane   XZ:1,  YZ:2,  XY:3 
//	int NumPt = PtList.m_nCount;
//	GVector3f* pTmpPt = new GVector3f[ NumPt ];
//	double* pTheta = new double[ NumPt ];
//
//	GObNode<GVector3f>* pGObNode = PtList.m_pLeftEnd;
//	int index = 0;
//	while( pGObNode )
//	{
//		pTmpPt[index] = pGObNode->data;
//		pTheta[index] = 0.0;	
//		pGObNode = pGObNode -> m_pRight;
//		index++;
//	}
//
//	double max_x = -100000.0;	double min_x = 100000.0;
//	double max_z = -100000.0;	double min_z = 100000.0;
//	double center_x, center_z;
//	int j, k;
//	for ( j = 0; j < NumPt; j++ )
//	{
//		min_x = MIN( min_x, pTmpPt[j].X );
//		max_x = MAX( max_x, pTmpPt[j].X );
//		min_z = MIN( min_z, pTmpPt[j].Z );
//		max_z = MAX( max_z, pTmpPt[j].Z );
//	}
//	center_x = ( max_x + min_x ) / 2.0;
//	center_z = ( min_z + max_z ) / 2.0;
//	*CX = center_x;
//	*CZ = center_z;
//	for ( j = 0; j < NumPt; j++ )
//	{
//		pTmpPt[j].X -= center_x;
//		pTmpPt[j].Z -= center_z;
//		double x = pTmpPt[j].X;
//		double z = pTmpPt[j].Z;
//		double angle = acos( z / sqrt( x*x + z*z ) ) *(180.0/M_PI);
//		if ( x > 0 && z > 0 )
//			pTheta[j] = angle;
//		if ( x > 0 && z < 0 )
//			pTheta[j] = angle;
//		if ( x < 0 && z < 0 )
//			pTheta[j] =  360.0-angle; //2 * M_PI - angle;
//		if ( x < 0 && z > 0 )
//			pTheta[j] = 360.0-angle; //2 * M_PI - angle ;
//	}
//	for ( j = 0; j < NumPt - 1; j++ )
//	{
//		for ( k = j + 1; k < NumPt; k ++)
//		{
//			if ( pTheta[j] > pTheta[k] )
//			{
//				double imsi = pTheta[k];
//				pTheta[k] = pTheta[j];
//				pTheta[j] = imsi;
//				GVector3f imsi1 = pTmpPt[k];
//				pTmpPt[k] = pTmpPt[j];
//				pTmpPt[j] = imsi1;
//			}
//		}
//	}
//	PtList.DeleteAll();
//	int intTheta = 0, preTheta = 0;
//	for ( j = 0; j < NumPt; j++ )
//	{
//		pTmpPt[j].X += center_x;
//		pTmpPt[j].Z += center_z;
//		intTheta = ROUND(pTheta[j]);
//
//		if(intTheta != preTheta) {
//			//if( intTheta%2 )
//				PtList.Insert( pTmpPt[j] );		
//		}
//
//		preTheta = intTheta;
//
//	}
//	delete [] pTmpPt;
//	delete [] pTheta;	
//}

bool GetIntersectPointXY(const GVector3f& AP1, const GVector3f& AP2, const GVector3f& BP1, const GVector3f& BP2, GVector3f* IP) 
{ 
	float t; 
	float s;

	float under = (BP2.Y-BP1.Y)*(AP2.X-AP1.X)-(BP2.X-BP1.X)*(AP2.Y-AP1.Y);
	if(under==0) 
		return false; 
	
	float _t = (BP2.X-BP1.X)*(AP1.Y-BP1.Y) - (BP2.Y-BP1.Y)*(AP1.X-BP1.X);
	float _s = (AP2.X-AP1.X)*(AP1.Y-BP1.Y) - (AP2.Y-AP1.Y)*(AP1.X-BP1.X);
	
	t = _t/under; 
	s = _s/under; 
	if(t<0.0f || t>1.0f || s<0.0f || s>1.0f) 
        return false; 
	if( _t==0 && _s==0 ) 
		return false; 
	
	IP->X = AP1.X + t * (AP2.X-AP1.X); 
	IP->Y = AP1.Y + t * (AP2.Y-AP1.Y); 
	return true;
	
}

bool GetIntersectPointZY(const GVector3f& AP1, const GVector3f& AP2, const GVector3f& BP1, const GVector3f& BP2, GVector3f* IP) 
{ 
	double t; 
	double s; 

	double under = (BP2.Y-BP1.Y)*(AP2.Z-AP1.Z)-(BP2.Z-BP1.Z)*(AP2.Y-AP1.Y); 
	if(under==0) 
		return false; 
	
	double _t = (BP2.Z-BP1.Z)*(AP1.Y-BP1.Y) - (BP2.Y-BP1.Y)*(AP1.Z-BP1.Z); 
	double _s = (AP2.Z-AP1.Z)*(AP1.Y-BP1.Y) - (AP2.Y-AP1.Y)*(AP1.Z-BP1.Z); 
	
	t = _t/under; 
	s = _s/under; 
	if(t<0.0 || t>1.0 || s<0.0 || s>1.0) 
        return false; 
	if( _t==0 && _s==0 ) 
		return false; 
	
	IP->Z = AP1.Z + t * (double)(AP2.Z-AP1.Z); 
	IP->Y = AP1.Y + t * (double)(AP2.Y-AP1.Y); 
	return true;
	
}



bool GetIntersectPointZX(const GVector3f& AP1, const GVector3f& AP2, const GVector3f& BP1, const GVector3f& BP2, GVector3f* IP) 
{ 
	double t; 
	double s; 

	double under = (BP2.X-BP1.X)*(AP2.Z-AP1.Z)-(BP2.Z-BP1.Z)*(AP2.X-AP1.X); 
	if(under==0) 
		return false; 
	
	double _t = (BP2.Z-BP1.Z)*(AP1.X-BP1.X) - (BP2.X-BP1.X)*(AP1.Z-BP1.Z); 
	double _s = (AP2.Z-AP1.Z)*(AP1.X-BP1.X) - (AP2.X-AP1.X)*(AP1.Z-BP1.Z); 
	
	t = _t/under; 
	s = _s/under; 
	if(t<0.0 || t>1.0 || s<0.0 || s>1.0) 
        return false; 
	if( _t==0 && _s==0 ) 
		return false; 
	
	IP->Z = AP1.Z + t * (double)(AP2.Z-AP1.Z); 
	IP->X = AP1.X + t * (double)(AP2.X-AP1.X); 
	return true;
	
}

int InCircle(GVector3f p, GVector3f p1, GVector3f p2, GVector3f p3, GVector3f *cp, double *r)
{

	//Private Function InCircle(Gxp As Long, yp As Long, x1 As Long, y1 As Long, x2 As Long, y2 As Long, x3 As Long, y3 As Long, ByRef xc, ByRef yc, ByRef r) As Boolean
	//'Return TRUE if the point (xp,yp) lies inside the circumcircle
	//'made up by points (x1,y1) (x2,y2) (x3,y3)
	//'The circumcircle centre is returned in (xc,yc) and the radius r
	//'NOTE: A point on the edge is inside the circumcircle
     
	float eps, m1, m2, dx, dy, rsqr, drsqr;
	GVector3f mp1, mp2, tmp;

	eps = 0.000001f;
      
	if( ABS(p1.Y - p2.Y) < eps && ABS(p2.Y - p3.Y) < eps){
	   // MsgBox "INCIRCUM - F - Points are coincident !!"
		return(FALSE);
	  }
 
	if( ABS(p2.Y -p1.Y) < eps){
		m2 = -(p3.X - p2.X) / (p3.Y - p2.Y);
		mp2.X = (p2.X + p3.X) / 2.0f;
		mp2.Y = (p2.Y + p3.Y) / 2.0f;
		tmp.X = (p2.X + p1.X) / 2.0f;
		tmp.Y = m2 * (tmp.X - mp2.X) + mp2.Y;
		*cp = tmp;
		}
	else if( ABS(p3.Y - p2.Y) < eps ){
		m1 = -(p2.X - p1.X) / (p2.Y - p1.Y);
		mp1.X = (p1.X + p2.X) / 2.0f;
		mp1.Y = (p1.Y + p2.Y) / 2.0f;
		tmp.X = (p3.X + p2.X) / 2.0f;
		tmp.Y = m1 * (tmp.X - mp1.X) + mp1.Y;
		*cp = tmp;
		}
	else {
		m1 = -(p2.X - p1.X) / (p2.Y - p1.Y);
		m2 = -(p3.X - p2.X) / (p3.Y - p2.Y);
		mp1.X = (p1.X + p2.X) / 2.0f;
		mp2.X = (p2.X + p3.X) / 2.0f;
		mp1.Y = (p1.Y + p2.Y) / 2.0f;
		mp2.Y = (p2.Y + p3.Y) / 2.0f;
		tmp.X = (m1 * mp1.X - m2 * mp2.X + mp2.Y - mp1.Y) / (m1 - m2);
		tmp.Y = m1 * (tmp.X - mp1.X) + mp1.Y;
		*cp = tmp;
	}
      
	dx = p2.X - tmp.X;
	dy = p2.Y - tmp.Y;
	rsqr = dx * dx + dy * dy;
	*r = SQRT(rsqr);
	dx = p.X - tmp.X;
	dy = p.Y - tmp.Y;
	drsqr = dx * dx + dy * dy;

	return ( ( drsqr <= rsqr )? TRUE : FALSE );
        
}


//'Determines which side of a line the point (xp,yp) lies.
//'The line goes from (x1,y1) to (x2,y2)
//'Returns -1 for a point to the left
//'         0 for a point on the line
//'        +1 for a point to the right
 
int WhichSide(GVector3f p, GVector3f p1, GVector3f p2) {

double equation = ((p.Y - p1.Y) * (p2.X - p1.X)) - ((p2.Y - p1.Y) * (p.X - p1.X));

if( equation > 0) 
    return(-1);	
else if (equation = 0 )
    return(0);
else
    return(1);

}



//Takes as input nv vertices in arrays Vertex()
//Returned is a list of NTRI triangular faces in the array
//Triangle(). These triangles are arranged in clockwise order.
//void Triangulate(int nv, GVector3f *Vertex, TRIANGLE *Triangle, int ntri) {

void Triangulate(int nv, GVector3f *Vertex, TRIANGLE *Triangle, int ntri) {

int *Complete = 0;
EDGE *Edges = NULL;
int Nedge = 0;

//General Variables
int inside;
int i, j, k;
double r;
GVector3f p, v1, v2, v3, cp;

//For Super Triangle
long xmin, xmax, ymin, ymax, xmid, ymid;
double dx, dy, dmax;


//Find the maximum and minimum vertex bounds.
//This is to allow calculation of the bounding triangle
xmin = Vertex[1].X;
ymin = Vertex[1].Y;
xmax = xmin;
ymax = ymin;
 for (i=2;i<=nv;i++) {
    if (Vertex[i].X < xmin) xmin = Vertex[i].X;
    if (Vertex[i].X > xmax) xmax = Vertex[i].X;
    if (Vertex[i].Y < ymin) ymin = Vertex[i].Y;
    if (Vertex[i].Y > ymax) ymax = Vertex[i].Y;
 }
dx = xmax - xmin;
dy = ymax - ymin;
dmax = (dx > dy) ? dx : dy;
xmid = (xmax + xmin) / 2.0;
ymid = (ymax + ymin) / 2.0;

//'Set up the supertriangle
//'This is a triangle which encompasses all the sample points.
//'The supertriangle coordinates are added to the end of the
//'vertex list. The supertriangle is the first triangle in
//'the triangle list.

Vertex[nv + 1].X = xmid - 2 * dmax;
Vertex[nv + 1].Y = ymid - dmax;
Vertex[nv + 2].X = xmid;
Vertex[nv + 2].Y = ymid + 2 * dmax;
Vertex[nv + 3].X = xmid + 2 * dmax;
Vertex[nv + 3].Y = ymid - dmax;
Triangle[1].p1 = nv + 1;
Triangle[1].p2 = nv + 2;
Triangle[1].p3 = nv + 3;
Complete[1] = FALSE;
ntri = 1;

//Include each point one at a time into the existing mesh
 for (i=1; i<=nv; i++) {
  
    p.X = Vertex[i].X;
    p.Y = Vertex[i].Y;
    Nedge = 0;

    //'Set up the edge buffer.
    //'If the point (Vertex(i).x,Vertex(i).y) lies inside the circumcircle then the
    //'three edges of that triangle are added to the edge buffer.
    j = 0;
    do {
        j++;
        if ( !Complete[j] ){ 
			 v1 = Vertex[Triangle[j].p1];		     
			 v2 = Vertex[Triangle[j].p2];			
			 v3 = Vertex[Triangle[j].p3];			 

            inside = InCircle(Vertex[i], v1, v2, v3, &cp, &r);
            //Include this if points are sorted by X
            if ( (cp.X + r) < Vertex[i].X) {
                Complete[j] = TRUE;
              if(inside)
                Edges[Nedge + 1].p1 = Triangle[j].p1;
                Edges[Nedge + 1].p2 = Triangle[j].p2;
                Edges[Nedge + 2].p1 = Triangle[j].p2;
                Edges[Nedge + 2].p2 = Triangle[j].p3;
                Edges[Nedge + 3].p1 = Triangle[j].p3;
                Edges[Nedge + 3].p2 = Triangle[j].p1;
                Nedge = Nedge + 3;
                Triangle[j] = Triangle[ntri];                
                Complete[j] = Complete[ntri];
                j--;
                ntri--;
            }//if inside
          }//if
		  }
    while ( j < ntri );

    //Tag multiple edges
    //Note: if all triangles are specified anticlockwise then all
    //interior edges are opposite pointing in direction.
	for (j=1;j<Nedge;j++) {
		for (k=j+1;k<=Nedge;k++) {
			if ((Edges[j].p1 == Edges[k].p2) && (Edges[j].p2 == Edges[k].p1)) {
				Edges[j].p1 = 0; 
				Edges[j].p2 = 0;
				Edges[k].p1 = 0; 
				Edges[k].p2 = 0;
			}
			// Shouldn't need the following, see note above
			if ((Edges[j].p1 == Edges[k].p1) && (Edges[j].p2 == Edges[k].p2)) {
				Edges[j].p1 = 0; 
				Edges[j].p2 = 0;
				Edges[k].p1 = 0; 
				Edges[k].p2 = 0;
			}
			
		}  
	}

    
    //Form new triangles for the current point
    //Skipping over any tagged edges.
    //All edges are arranged in clockwise order.
   for (j=1;j<=Nedge;j++) {
        if (Edges[j].p1 != 0 && Edges[j].p2 != 0) {
           if (ntri > trimax) {
             //fprintf(stderr,"TRIANGULATE - %d triangles exceeds maximum\n",*ntri);
              exit(0);
			}
			(ntri)++;
			Triangle[ntri].p1 = Edges[j].p1;
			Triangle[ntri].p2 = Edges[j].p2;
			Triangle[ntri].p3 = i;
			Complete[ntri] = FALSE;
			}
		}
    }


//Remove triangles with supertriangle vertices
//These are triangles which have a vertex number greater than NVERT
    i = 0;
    do {
	   i++;
	   if ((Triangle[i].p1 > nv) || (Triangle[i].p2 > nv) || (Triangle[i].p3 > nv)) {
	   Triangle[i] = Triangle[ntri];
	   i--;
	   ntri--;
	   }

	 } while (i < ntri);
	   
		 free(Edges);
		 free(Complete);

}

