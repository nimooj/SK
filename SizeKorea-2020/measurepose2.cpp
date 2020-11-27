#include "pch.h"
#include "measurepose2.h"
#include "convexhull.h"
#include "util.h"


#define CHECK(A)	if ( A == GLandMark( 0.0f, 0.0f, 0.0f ) )	\
						return	\

float RefAngle[100] = 
{ 
20.0f, 
1.4f,
1.4f,
1.4f,
1.4f
};

bool UpdateAngle = true;

// 측정항목 1~15 : 높이 항목들...
// 1 키					2 목뒤높이(6)		 3 목옆높이(오른4)      4 목앞높이(3),		5 어깨끝점높이(오른7), 
// 6 겨드랑 높이		7 (젖)가슴높이(16),  8 젖가슴아래높이(18)   9 허리높이(19),		10 배높이, 
// 11 배꼽수준허리높이(35) 12 엉덩이높이(24),    13 다리벌린샅높이(25)  14 볼기고랑높이(26),15 무릎높이(27),  
// 16 장딴지높이(28)	   17가쪽복사높이(30) 18 엉덩이수직길이 19 몸통수직길이",   20 손끝높이
// 73 손끝점 r 높이(34)
void GetResultPose2_1_20( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark)
{
// 높이항목은 CHECK 할 필요없다.. 0 이면 점이 없는 것으로 간주할 수 있으므로 
 //   (*CvxPt).DeleteAll();

	Result[1] = LandMark[1].Y+15.0f;
	Result[2] = LandMark[6].Y+15.0f;
	Result[3] = LandMark[4].Y+15.0f;
	Result[4] = LandMark[3].Y+15.0f;
	Result[5] = LandMark[7].Y+15.0f;
	Result[6] = LandMark[9].Y+15.0f;
	Result[7] = LandMark[16].Y+15.0f;
	Result[8] = LandMark[18].Y+15.0f;
	Result[9] = LandMark[19].Y+15.0f;
	Result[10] = LandMark[23].Y+15.0f;

	Result[11] = LandMark[35].Y+15.0f;
	Result[12] = LandMark[24].Y+15.0f;
	Result[13] = LandMark[25].Y+15.0f;
	Result[14] = LandMark[26].Y+15.0f;
	Result[15] = LandMark[27].Y+15.0f;
	Result[16] = LandMark[28].Y+15.0f;
	Result[17] = LandMark[30].Y+15.0f;

	Result[18] = LandMark[19].Y-LandMark[24].Y;
	Result[19] = LandMark[6].Y-LandMark[24].Y;
	Result[20] = LandMark[34].Y;

}

// 측정항목 21 : 목둘레.  
// 필요한 측정점(인덱스) : 방패연골아래점(2)  목뒤점(6)
// 설명 : 방패연골아래점을 지나면서 목앞선에 수직한 평면과 만나는 점들의 둘레 길이.
void GetResultPose2_21( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GObList<GVector3f> CrossPtList;
	GLandMark a = LandMark[2];
	GLandMark b = LandMark[6];
	_cprintf("\n Fneck : %f %f %f\n", LandMark[3].X, LandMark[3].Y, LandMark[3].Z);
	_cprintf("\n a: %f %f %f\n", a.X, a.Y, a.Z);
	_cprintf("\n b : %f %f %f\n", b.X, b.Y, b.Z);
	CHECK(a);
	CrossPtList.Insert(a);
	b.Z -= 5.0f;

//	float angle = (90.0 - RefAngle[0]) * 3.141592f / 180.0f;

	// make plane.
	RefAngle[0] = 0.34906f;   //목둘레 20
	GPlane pln;
	pln.MakePlane( GVector3f( 0.0f,  1.0f, tan(RefAngle[0]) ), a );

	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		if ( v0.Y < a.Y && v1.Y < a.Y && v2.Y < a.Y )
			continue;
		if ( v0.Z < b.Z && v1.Z < b.Z && v2.Z < b.Z )
			continue;
		
		
		if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
			continue;
		if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
			continue;

		if ( pln.GetSignedDistance( v0 ) * pln.GetSignedDistance( v1 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v1 ) * pln.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v2 ) * pln.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;
	Result[21] = val;
}

// 측정항목 22: 목밑둘레.   목뒤점에서 시작
//			23: 목뒤밑둘레
//			24: 목앞밑둘레
//         68: 목(밑)너비.
//         89 목밑둘레 curve
// 필요한 측정점(인덱스) : 목앞점(3), 오른쪽목옆점(4), 왼쪽목옆점(5), 목뒤점(6) /보조점: 목앞점좌우점(46, 47)
// 설명 :  
void GetResultPose2_22_68( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt, GObList<GVector3f> *CvxPt2, GObList<GVector3f> *CvxPt3 )
{
	(*CvxPt).DeleteAll();  //22
	(*CvxPt2).DeleteAll(); //68
	(*CvxPt3).DeleteAll();  //89

	GObList<GVector3f> CrossPtList; // 목밑둘레.
	GObList<GVector3f> CrossPtList2; // 목뒤둘레.
	GObList<GVector3f> CrossPtList3; // 목밑둘레 curve.
	GLandMark fn = LandMark[3];   //목앞점  a
	GLandMark snr = LandMark[4];  //오른쪽목옆점 b
	GLandMark snl = LandMark[5];  //왼쪽목옆점 c
	GLandMark bn = LandMark[6];   //목뒤점 d
	GLandMark fr = LandMarkHelp[46];
	GLandMark fl = LandMarkHelp[47];
	
	fn.Z = (fr.Z*0.5) + (fl.Z*0.5);

	CHECK(fn);
	CHECK(snr);
	CHECK(snl);
	CHECK(bn);
	CrossPtList.Insert(fn);
	CrossPtList.Insert(snr);
	CrossPtList.Insert(snl);
	CrossPtList.Insert(bn);
	GVector3f o = (fn+snr+snl+bn)/4.0f; 
	o.Y = snr.Y*0.4 +fn.Y*0.6 ; //+ 30.0f; // added.

	//목뒤점 좌우에 보조점 1개씩 추가
	GVector3f bnR = bn;
	GVector3f bnL = bn;
	bnR.X -= 10.0f;    bnL.X += 10.0f; 
	// make GNurbsCrv for path.
	//p[0][], p[4][]는 목앞점을 좌우로 조금씩 움직인 것- 목앞점 그대로 하면 너무 뾰족해지므로
	//REAL4 P[5];
	REAL4 P[9];

	//curve interpol 점들 순서는 반시계 방향으로 해야한다.
	//목뒤점에서 시작  : 목뒤점 좌우에 보조점 1개씩 추가
	//         x            y              z
	P[0][0] = bn[0];       P[0][1] = bn[1];   P[0][2] = bn[2];   P[0][3] = 1.0f;
	P[1][0] = bnR[0];      P[1][1] = bnR[1];  P[1][2] = bnR[2];  P[1][3] = 1.0f;
	P[2][0] = snr[0];       P[2][1] = snr[1];   P[2][2] = snr[2];   P[2][3] = 1.0f; //a[0]+7.0f;  P[0][1] = a[1];  P[0][2] = a[2];  P[0][3] = 1.0f;
	
	P[3][0] = fr[0];       P[3][1] = fr[1];   P[3][2] = fr[2];   P[3][3] = 1.0f;
	P[4][0] = fn[0];	   P[4][1] = fn[1];   P[4][2] = fn[2];   P[4][3] = 1.0f;
	P[5][0] = fl[0];       P[5][1] = fl[1];   P[5][2] = fl[2];   P[5][3] = 1.0f;
	
	P[6][0] = snl[0];	   P[6][1] = snl[1];   P[6][2] = snl[2];   P[6][3] = 1.0f;
	P[7][0] = bnL[0];      P[7][1] = bnL[1];  P[7][2] = bnL[2];  P[7][3] = 1.0f;
	P[8][0] = bn[0];       P[8][1] = bn[1];   P[8][2] = bn[2];   P[8][3] = 1.0f;

	//목뒤점에서 시작  : 목뒤점 좌우에 보조점 없이
	/*/         x            y              z
	P[0][0] = bn[0];       P[0][1] = bn[1];   P[0][2] = bn[2];   P[0][3] = 1.0f;
	P[1][0] = snr[0];       P[1][1] = snr[1];   P[1][2] = snr[2];   P[1][3] = 1.0f; //a[0]+7.0f;  P[0][1] = a[1];  P[0][2] = a[2];  P[0][3] = 1.0f;
	P[2][0] = fn[0];	   P[2][1] = fn[1];   P[2][2] = fn[2];   P[2][3] = 1.0f;
	P[3][0] = snl[0];	   P[3][1] = snl[1];   P[3][2] = snl[2];   P[3][3] = 1.0f;
	P[4][0] = bn[0];	   P[4][1] = bn[1];   P[4][2] = bn[2];   P[4][3] = 1.0f;*/
	
	/*/목앞점에서 시작
	P[0][0] = fn[0];       P[0][1] = fn[1];   P[0][2] = fn[2];   P[0][3] = 1.0f;
	P[1][0] = snl[0];       P[1][1] = snl[1];   P[1][2] = snl[2];   P[1][3] = 1.0f; //a[0]+7.0f;  P[0][1] = a[1];  P[0][2] = a[2];  P[0][3] = 1.0f;
	P[2][0] = bn[0];	   P[2][1] = bn[1];   P[2][2] = bn[2];   P[2][3] = 1.0f;
	P[3][0] = snr[0];	   P[3][1] = snr[1];   P[3][2] = snr[2];   P[3][3] = 1.0f;
	P[4][0] = fn[0];	   P[4][1] = fn[1];   P[4][2] = fn[2];   P[4][3] = 1.0f;*/
	
	
	GNurbsCrv PathCrv;
	PathCrv.InterCrv( 3, P, 8 ); //InterCrv( 몇차 곡선, P, (점 갯수-1) )


	// make index list.
	GObList<int> SubIdxList;
	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f b0, b1, b2, low, upper;
		b0 = Points[ IdxList[i] ];
		b1 = Points[ IdxList[i+1] ];
		b2 = Points[ IdxList[i+2] ];
		low = upper = b0;

		if ( b1.Y < low.Y )
		{
			if( b1.Y < b2.Y )
				low = b1;  // b1 < b2, b0
			else 
				low = b2;  // b2 < b1 < b0
		}
		else
		{
			if ( b2.Y < low.Y )
				low = b2;
		}
		if ( b1.Y > upper.Y )
		{
			if ( b2.Y > b1.Y )
				upper = b2;
			else
				upper = b1;
		}
		else
		{
			if ( b2.Y > upper.Y )
				upper = b2;
		}

		if ( low.Y > fn.Y-50.0f && upper.Y < bn.Y+10.0f ) // 목앞점, 목뒤점 높이 사이에 있는 삼각형들 모이는 중
		{
			SubIdxList.Insert( IdxList[i] );
			SubIdxList.Insert( IdxList[i+1] );
			SubIdxList.Insert( IdxList[i+2] );
		}
	}

	// 일단 목앞, 뒤, 옆점으로 곡선 만든 점들로 eval하면서 
	// eval한 두점 사이에 있는 목부위 데이타점을 포함시킨다
	int snum = 40;
	REAL4 v0, v1;
	REAL min = PathCrv.GetMinKnotVal();
	REAL max = PathCrv.GetMaxKnotVal();
	float sidefrontmh3 = (fn.Y*0.7)+(snr.Y*0.3);
	float sidefrontmh6 = (fn.Y*0.3)+(snr.Y*0.7);
	for (int i = 0; i < snum; i++ )
	{
		REAL u0 = min + ( (max-min)*i ) / (REAL)(snum);
		REAL u1 = min + ( (max-min)*(i+1) ) / (REAL)(snum);
		PathCrv.Eval( u0, v0 );
		PathCrv.Eval( u1, v1 );
		
		// make plane.
		GVector3f pt0(v0);
		GVector3f pt1(v1);
		
		CrossPtList3.Insert( pt0 );
		CrossPtList3.Insert( pt1 );

		if (pt0.Y < sidefrontmh3) o.Y = fn.Y-30.0f;
		if (pt0.Y < sidefrontmh6) o.Y = fn.Y;
		else if (pt0.Y < snr.Y) o.Y = sidefrontmh3;
		else   o.Y = snr.Y;

		GPlane pln;
		pln.MakePlane( o, pt0, pt1 );

		// make plane1 -> eval된 점과 중심점을 지나는 수직평면
		GVector3f tmp = (o+pt0)/2.0f; tmp.Y += 10.0f; 
		GPlane pln1;
		pln1.MakePlane( o, pt0, tmp ); 

		// make plane2.
		tmp = (o+pt1)/2.0f; tmp.Y += 10.0f;
		GPlane pln2;
		pln2.MakePlane( o, pt1, tmp );

		// make plane3. 목 옆점을 기준으로 앞뒤 나누는 평면
		tmp = (snr+snl)/2.0f; tmp.Y += 10.0f; 
		GPlane pln3;
		pln3.MakePlane( snr, snl, tmp );

		//pln과 교차하는 삼각형 찾고, pln1,2,3 안에 있는 목주위 점
		GObNode<int>* pNode = SubIdxList.m_pLeftEnd;
		float limitRX=snr.X-10.0f;
		float limitLX=snl.X+10.0f;
		while( pNode )
		{
			long idx0 = pNode->data;
			long idx1 = pNode->m_pRight->data;
			long idx2 = pNode->m_pRight->m_pRight->data;
			GVector3f b0, b1, b2;
			b0 = Points[ idx0 ];
			b1 = Points[ idx1 ];
			b2 = Points[ idx2 ];

			if ( pln.IsAbovePlane(b0) && pln.IsAbovePlane(b1) && pln.IsAbovePlane(b2) )
			{
				if ( pNode->m_pRight->m_pRight->m_pRight == NULL )
					break;
				pNode = pNode->m_pRight->m_pRight->m_pRight;
				continue;
			}
			if ( pln.IsBelowPlane(b0) && pln.IsBelowPlane(b1) && pln.IsBelowPlane(b2) )
			{
				if ( pNode->m_pRight->m_pRight->m_pRight == NULL )
					break;
				pNode = pNode->m_pRight->m_pRight->m_pRight;
				continue;
			}
			if ( pln.GetSignedDistance( b0 ) * pln.GetSignedDistance( b1 ) < 0 )
			{
				GVector3f CrossPt = pln.GetProjectionPt( b0, b1-b0 );
				if ( pln1.IsBelowPlane( CrossPt ) && pln2.IsAbovePlane( CrossPt ) )
				{
					if(CrossPt.X>limitRX && CrossPt.X<limitLX && CrossPt.Y<(pt0.Y+30.0f) ){
						CrossPtList.Insert( CrossPt );
					//	if ( pln3.IsBelowPlane( CrossPt ) )
					//		CrossPtList2.Insert( CrossPt ); //목뒤둘레 점들
					}
				}
			}
			
			if ( pln.GetSignedDistance( b1 ) * pln.GetSignedDistance( b2 ) < 0 )
			{
				GVector3f CrossPt = pln.GetProjectionPt( b1, b2-b1 );
				if ( pln1.IsBelowPlane( CrossPt ) && pln2.IsAbovePlane( CrossPt ) )
				{
					if(CrossPt.X>limitRX && CrossPt.X<limitLX && CrossPt.Y<(pt0.Y+30.0f) ){
						CrossPtList.Insert( CrossPt );
					//	if ( pln3.IsBelowPlane( CrossPt ) )
					//		CrossPtList2.Insert( CrossPt );//목뒤둘레 점들
					}
				}
			}
			if ( pln.GetSignedDistance( b2 ) * pln.GetSignedDistance( b0 ) < 0 )
			{
				GVector3f CrossPt = pln.GetProjectionPt( b0, b2-b0 );
				if ( pln1.IsBelowPlane( CrossPt ) && pln2.IsAbovePlane( CrossPt ) )
				{
					if(CrossPt.X>limitRX && CrossPt.X<limitLX && CrossPt.Y<(pt0.Y+30.0f)){
						CrossPtList.Insert( CrossPt );
						if ( pln3.IsBelowPlane( CrossPt ) )
							CrossPtList2.Insert( CrossPt );//목뒤둘레 점들
					}
				}
			}
			if ( pNode->m_pRight->m_pRight->m_pRight == NULL )
				break;
			pNode = pNode->m_pRight->m_pRight->m_pRight;
		}
	}

	// CrossPtList2 : 목뒤둘레.
	// CrossPtList : 목밑둘레.

//	목뒤둘레
    CConvexHull cvx2( CrossPtList2 );
	cvx2.MakeConvexHullXZ();
	cvx2.ArrangeHullPtXZ();
	float val2 = cvx2.GetHullLength();
	(*CvxPt) = cvx2.m_HullPtList;
	GVector3f dis = snr-snl;
	val2 -= dis.getNorm();
	Result[23] = val2;


	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
//	cvx.m_HullPtList.Insert( fr );
//	cvx.m_HullPtList.Insert( fl );
	cvx.ArrangeHullPtXZ();
	float val = cvx.GetHullLength(); 
	(*CvxPt) = cvx.m_HullPtList;	 //22 목밑둘레

	(*CvxPt2).Insert( snl );		//68  목밑너비
	(*CvxPt2).Insert( snr );

	(*CvxPt3) = CrossPtList3;      // curve

	Result[22] = val;
	Result[24] = val-val2;
	Result[68] = snl.X-snr.X;
}


// 목앞점에서 시작하고 좌우에 보조점이 있음
// 측정항목    22: 목밑둘레.  
// 측정항목    68 : 목(밑)너비.   
//             89 : 목밑둘레 curve
// 필요한 측정점(인덱스) : 목앞점(3), 오른쪽목옆점(4), 왼쪽목옆점(5), 목뒤점(6)
// 설명 :  
void GetResultPose2_22_68_a( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt, GObList<GVector3f> *CvxPt2, GObList<GVector3f> *CvxPt3)
{
	(*CvxPt).DeleteAll();  //22
	(*CvxPt2).DeleteAll(); //53
	(*CvxPt3).DeleteAll();  //89


	GObList<GVector3f> CrossPtList;  // 목밑둘레.
	GObList<GVector3f> CrossPtList2; // 목뒤둘레.
	GObList<GVector3f> CrossPtList3; // 목밑둘레 curve.
	GLandMark fn =  LandMark[3];     //목앞점
	GLandMark snr =   LandMark[4];	//오른쪽목옆점
	GLandMark snl = LandMark[5];	//왼쪽목옆점
	GLandMark bn = LandMark[6];     //목뒤점
	GLandMark fr = LandMarkHelp[46]; //목앞오른쪽
	GLandMark fl = LandMarkHelp[47]; //목앞왼쪽
	
	fn.Z = (fr.Z*0.5) + (fl.Z*0.5);

	CHECK(fn);
	CHECK(snr);
	CHECK(snl);
	CHECK(bn);
	CHECK(fr);
	CHECK(fl);
	CrossPtList.Insert(fn);
	CrossPtList.Insert(snr);
	CrossPtList.Insert(snl);
	CrossPtList.Insert(bn);
	CrossPtList.Insert(fl);
	GVector3f o = (fn+bn+snl+snr)/4.0f; 
	o.Y = fn.Y ; // +30.0f added.
	//o.Y = snr.Y*0.4 +fn.Y*0.6 ;

	GVector3f bnR = bn;
	GVector3f bnL = bn;
	bnR.X -= 10.0f;    bnL.X += 10.0f; 

	// make GNurbsCrv for path.
	//p[0][], p[4][]는 목앞점을 좌우로 조금씩 움직인 것- 목앞점 그대로 하면 너무 뾰족해지므로
	//REAL4 P[5];
	//REAL4 P[7];
	REAL4 P[9];

	//         x            y              z
	P[0][0] = fn[0];       P[0][1] = fn[1];   P[0][2] = fn[2];   P[0][3] = 1.0f;
	P[1][0] = fl[0];       P[1][1] = fl[1];   P[1][2] = fl[2];   P[1][3] = 1.0f; //a[0]+7.0f;  P[0][1] = a[1];  P[0][2] = a[2];  P[0][3] = 1.0f;
	P[2][0] = snl[0];	   P[2][1] = snl[1];  P[2][2] = snl[2];  P[2][3] = 1.0f;
	
	P[3][0] = bnL[0];	   P[3][1] = bnL[1];  P[3][2] = bnL[2];  P[3][3] = 1.0f;
	P[4][0] = bn[0];	   P[4][1] = bn[1];   P[4][2] = bn[2];   P[4][3] = 1.0f;
	P[5][0] = bnR[0];	   P[5][1] = bnR[1];  P[5][2] = bnR[2];  P[5][3] = 1.0f;

	P[6][0] = snr[0];	   P[6][1] = snr[1];  P[6][2] = snr[2];  P[6][3] = 1.0f;
	P[7][0] = fr[0];       P[7][1] = fr[1];   P[7][2] = fr[2];   P[7][3] = 1.0f;
	P[8][0] = fn[0];       P[8][1] = fn[1];   P[8][2] = fn[2];   P[8][3] = 1.0f;

	GNurbsCrv PathCrv;
	PathCrv.InterCrv( 3, P, 8 ); //InterCrv( 몇차 곡선, P, (점 갯수-1) )

	// make index list.
	GObList<int> SubIdxList;
	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f b0, b1, b2, low, upper;
		b0 = Points[ IdxList[i] ];
		b1 = Points[ IdxList[i+1] ];
		b2 = Points[ IdxList[i+2] ];
		low = upper = b0;

		if ( b1.Y < low.Y )
		{
			if( b1.Y < b2.Y )
				low = b1;  // b1 < b2, b0
			else 
				low = b2;  // b2 < b1 < b0
		}
		else
		{
			if ( b2.Y < low.Y )
				low = b2;
		}
		if ( b1.Y > upper.Y )
		{
			if ( b2.Y > b1.Y )
				upper = b2;
			else
				upper = b1;
		}
		else
		{
			if ( b2.Y > upper.Y )
				upper = b2;
		}

		if ( low.Y > fn.Y-50.0f && upper.Y < bn.Y+10.0f ) // 목앞점, 목뒤점 높이 사이에 있는 삼각형들 모이는 중
		{
			SubIdxList.Insert( IdxList[i] );
			SubIdxList.Insert( IdxList[i+1] );
			SubIdxList.Insert( IdxList[i+2] );
		}
	}

	// 일단 목앞, 뒤, 옆점으로 곡선 만든 점들로 eval하면서 
	// eval한 두점 사이에 있는 목부위 데이타점을 포함시킨다
	int snum = 40;
	REAL4 v0, v1;
	REAL min = PathCrv.GetMinKnotVal();
	REAL max = PathCrv.GetMaxKnotVal();
	for (int i = 0; i < snum; i++ )
	{
		REAL u0 = min + ( (max-min)*i ) / (REAL)(snum);
		REAL u1 = min + ( (max-min)*(i+1) ) / (REAL)(snum);
		PathCrv.Eval( u0, v0 );
		PathCrv.Eval( u1, v1 );
		
		// make plane.
		GVector3f pt0(v0);
		GVector3f pt1(v1);
		
		CrossPtList3.Insert( pt0 );
		CrossPtList3.Insert( pt1 );

		if (pt0.Y < snr.Y) o.Y = fn.Y-20.0f;
		else   o.Y = snr.Y;

		GPlane pln;
		pln.MakePlane( o, pt0, pt1 );

		// make plane1 -> eval된 점과 중심점을 지나는 수직평면
		GVector3f tmp = (o+pt0)*0.5; tmp.Y += snr.Y; 
		GPlane pln1;
		pln1.MakePlane( o, pt0, tmp ); 

		// make plane2.
		tmp = (o+pt1)/2.0f; tmp.Y += snr.Y;
		GPlane pln2;
		pln2.MakePlane( o, pt1, tmp );

		// make plane3. 목 옆점을 기준으로 앞뒤 나누는 평면
		tmp = (snr+snl)/2.0f; tmp.Y += snr.Y; 
		GPlane pln3;
		pln3.MakePlane( snr, snl, tmp );

		//pln과 교차하는 삼각형 찾고, pln1,2,3 안에 있는 목주위 점
		GObNode<int>* pNode = SubIdxList.m_pLeftEnd;
		while( pNode )
		{
			long idx0 = pNode->data;
			long idx1 = pNode->m_pRight->data;
			long idx2 = pNode->m_pRight->m_pRight->data;
			GVector3f b0, b1, b2;
			b0 = Points[ idx0 ];
			b1 = Points[ idx1 ];
			b2 = Points[ idx2 ];
			if ( pln.IsAbovePlane(b0) && pln.IsAbovePlane(b1) && pln.IsAbovePlane(b2) )
			{
				if ( pNode->m_pRight->m_pRight->m_pRight == NULL )
					break;
				pNode = pNode->m_pRight->m_pRight->m_pRight;
				continue;
			}
			if ( pln.IsBelowPlane(b0) && pln.IsBelowPlane(b1) && pln.IsBelowPlane(b2) )
			{
				if ( pNode->m_pRight->m_pRight->m_pRight == NULL )
					break;
				pNode = pNode->m_pRight->m_pRight->m_pRight;
				continue;
			}
			if ( pln.GetSignedDistance( b0 ) * pln.GetSignedDistance( b1 ) < 0 )
			{
				GVector3f CrossPt = pln.GetProjectionPt( b0, b1-b0 );
				if ( pln1.IsBelowPlane( CrossPt ) && pln2.IsAbovePlane( CrossPt ) )
				{
					CrossPtList.Insert( CrossPt );
					if ( pln3.IsBelowPlane( CrossPt ) )
						CrossPtList2.Insert( CrossPt ); //목뒤둘레 점들
				}
			}
			
			if ( pln.GetSignedDistance( b1 ) * pln.GetSignedDistance( b2 ) < 0 )
			{
				GVector3f CrossPt = pln.GetProjectionPt( b1, b2-b1 );
				if ( pln1.IsBelowPlane( CrossPt ) && pln2.IsAbovePlane( CrossPt ) )
				{
					CrossPtList.Insert( CrossPt );
					if ( pln3.IsBelowPlane( CrossPt ) )
						CrossPtList2.Insert( CrossPt );
				}
			}
			if ( pln.GetSignedDistance( b2 ) * pln.GetSignedDistance( b0 ) < 0 )
			{
				GVector3f CrossPt = pln.GetProjectionPt( b0, b2-b0 );
				if ( pln1.IsBelowPlane( CrossPt ) && pln2.IsAbovePlane( CrossPt ) )
				{
					CrossPtList.Insert( CrossPt );
					if ( pln3.IsBelowPlane( CrossPt ) )
						CrossPtList2.Insert( CrossPt );
				}
			}
			if ( pNode->m_pRight->m_pRight->m_pRight == NULL )
				break;
			pNode = pNode->m_pRight->m_pRight->m_pRight;
		}
	}

	// CrossPtList2 : 목뒤둘레.
	// CrossPtList : 목밑둘레.

/*	목뒤둘레
    CConvexHull cvx2( CrossPtList2 );
	cvx2.MakeConvexHullXZ();
	cvx2.ArrangeHullPtXZ();
	float val2 = cvx2.GetHullLength();
	(*CvxPt) = cvx2.m_HullPtList;
	GVector3f dis = b-c;
	val2 -= dis.getNorm();
	Result[1] = val2;*/


	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.m_HullPtList.Insert( fr );
	cvx.m_HullPtList.Insert( fl );
	cvx.ArrangeHullPtXZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;      //22 목밑둘레

	(*CvxPt2).Insert( snl );          //68  목밑너비 
	(*CvxPt2).Insert( snr );     

	(*CvxPt3) = CrossPtList3;      //88 curve

	Result[22] = val;
	Result[68] = snl.X-snr.X;
}
				

// 측정항목    25: 겨드랑둘레 길이 - 공식
// 필요한 측정점(인덱스) : 오른쪽 어깨가쪽점(7),오른쪽 겨드랑점(9)  /보조점: 오른쪽 겨드랑앞접힘점(42),   오른쪽 겨드랑뒤접힘점(43).
// 설명: 위쪽은 뒤겨드랑접힘점, 어깨끝점, 앞겨드랑접힘점 포함되는 길이.
// 	     아래쪽은 곡률을 고려한 가상의 선.

void GetResultPose2_25( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	int i;
	GObList<GVector3f> CrossPtList; 
	GLandMark a = LandMark[7];  //오른 어깨가쪽
	GLandMark c = LandMark[9];  //오른 겨드랑
	GLandMark f = LandMark[44];  //오른쪽겨드랑앞접힘점
	GLandMark b = LandMark[45]; //오른쪽겨드랑뒤점힘점

	CHECK(a);
	CHECK(c);
	CHECK(f);
	CHECK(b);
	//새로운 겨드랑점 = 앞점힘점과 뒤접힘점의 중간, 그냥 겨드랑점으로 하면 겨드랑점을 앞에 새로찍었을때 z가 중간이 되지 못한다
	GLandMark c1 =  ( f + b ) * 0.5f; 
	c1.X = (f.X*0.55f) + (b.X*0.45f); //좌우위치는 앞쪽점 가깝게 약간 이동
	c1.Y = c.Y-2.0f;                       //높이는 원래 겨드랑점 높이 -5mm
	if(f.Y<c1.Y) f.Y=c1.Y+6.0f;       //겨드랑앞뒤접힘점중 겨드랑점보다 더 낮은 점은 겨드랑점보다 3mm 올려줌
	if(b.Y<c1.Y) b.Y=c1.Y+6.0f;

	if( f.X >b.X) {
		f.X -=2.0f;  //앞접힘의 x좌표를 바깥으로 이동해서 몸통이 포함되지 않도록 함
		b.X -=1.0f;  //뒤접힘도
	}
	else {
		f.X -=1.0f;  //
		b.X -=2.0f;  //
	}

	GLandMark ff, bb; 
	ff= ( c1+f ) * 0.5f;  //d*0.25f + e*0.75f; 
	bb= ( c1+b ) * 0.5f;  //b*0.5f + e*0.25f; 
	float yd = (ff.Y-c1.Y)*0.4f;
	float zd = (ff.Z-c1.Z)*0.3f;
	//float xd = (ff.X-c1.X)*0.5f;
	ff.Y = ff.Y-yd;
	bb.Y = bb.Y-yd;
	ff.Z = ff.Z+zd;
	bb.Z = bb.Z-zd;
	//ff.X = f.X;
	bb.X = (c1.X*0.4f) + (b.X*0.6f);

	CrossPtList.Insert( a );
	//CrossPtList.Insert( c );
	CrossPtList.Insert( f );
	CrossPtList.Insert( b );

	// make plane
	GVector3f tmp;
	tmp = b; tmp.X -= 100.0f;
	GPlane refPln( b, tmp, f );   //겨드랑 앞,뒤 접힘점을 지나는 수평에 가까운 평면
//	GVector3f norm = GVector3f(0.0f, (f.Z-b.Z), -(f.Y-b.Y) );
  //  GPlane refPln( norm, b );   //겨드랑 앞,뒤 접힘점을 지나는 수평에 가까운 평면
	GPlane cutPln( a, f, b );   //어깨점과 겨드랑 앞,뒤 접힘점을 지나는 수직에 가까운 평면
	
	//b = cutPln.GetProjectionPt( b, GVector3f( 1.0f, 0.0f, 0.0f ) );
	
	//어깨점과 겨드랑 앞,뒤 접힘점을 지나는 평면위에 있고, 겨드랑 앞,뒤 접힘점  위에 있는 점만 모으기
	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		// culling.
		if ( v0.Y > a.Y+2.0f && v1.Y > a.Y+2.0f && v2.Y > a.Y+2.0f )  //어깨점 아래만
			continue;
		if ( v0.Y < c.Y-2.0f && v1.Y < c.Y-2.0f && v2.Y < c.Y-2.0f )  // 겨드랑점 위만
			continue;
		if ( v0.Z > f.Z+20.0f && v1.Z > f.Z+20.0f && v2.Z > f.Z+20.0f )  //겨드랑앞점힘점 뒤만
			continue;
		if ( v0.Z < b.Z-20.0f && v1.Z < b.Z-20.0f && v2.Z < b.Z-20.0f )  //겨드랑뒤점힘점 앞만
			continue;

		if ( refPln.IsBelowPlane( v0 ) && refPln.IsBelowPlane( v1 ) && refPln.IsBelowPlane( v2 ) )
			continue;

		if ( cutPln.IsBelowPlane( v0 ) && cutPln.IsBelowPlane( v1 ) && cutPln.IsBelowPlane( v2 ) )
			continue;

		if ( cutPln.IsAbovePlane( v0 ) && cutPln.IsAbovePlane( v1 ) && cutPln.IsAbovePlane( v2 ) )
			continue;

		if ( cutPln.GetSignedDistance( v0 ) * cutPln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v1 ) * cutPln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v2 ) * cutPln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}

	// make GNurbsCrv for path.
	/*REAL4 P[3];
	P[0][0] = d[0]; P[0][1] = d[1]; P[0][2] = d[2]; P[0][3] = 1.0f;
	P[1][0] = b[0]; P[1][1] = b[1]; P[1][2] = b[2]; P[1][3] = 1.0f;
	P[2][0] = e[0]; P[2][1] = e[1]; P[2][2] = e[2]; P[2][3] = 1.0f;
	GNurbsCrv PathCrv;
	PathCrv.InterCrv( 2, P, 2 );*/
	// 겨드랑 앞,뒤 접힘점 아래는 곡선으로 --> 앞에서 뒤의 순서로 
	REAL4 P[5];
	P[0][0] = f[0];   P[0][1] =  f[1];  P[0][2] =  f[2];  P[0][3] = 1.0f;
	P[1][0] = ff[0];  P[1][1] = ff[1];  P[1][2] = ff[2];  P[1][3] = 1.0f;
	P[2][0] = c1[0];  P[2][1] = c1[1];  P[2][2] = c1[2];  P[2][3] = 1.0f;
	P[3][0] = bb[0];  P[3][1] = bb[1];  P[3][2] = bb[2];  P[3][3] = 1.0f;
	P[4][0] = b[0];   P[4][1] =  b[1];  P[4][2] =  b[2];  P[4][3] = 1.0f;
	GNurbsCrv PathCrv;
	PathCrv.InterCrv( 3, P, 4 );

	int snum = 20;
	REAL4 v0;
	REAL min = PathCrv.GetMinKnotVal();
	REAL max = PathCrv.GetMaxKnotVal();
	for ( i = 0; i < snum; i++ )
	{
		REAL u0 = min + ( (max-min)*i ) / (REAL)(snum-1);
		PathCrv.Eval( u0, v0 );
		GVector3f pt( v0 );
		CrossPtList.Insert( pt );
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullYZ();
	//cvx.m_HullPtList = cvx.m_PtList;

	cvx.ArrangeHullPtYZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;
	Result[25] = val;
}

//겨드랑둘레
void GetResultPose2_25_a( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	int i;
	GObList<GVector3f> CrossPtList; 
	GLandMark shp = LandMark[7];  //오른 어깨가쪽
	GLandMark armpit = LandMark[9];  //오른 겨드랑
	GLandMark f =  LandMarkHelp[54]; //
	GLandMark b =  LandMarkHelp[55]; //
	GLandMark foldf = LandMark[44]; //오른쪽겨드랑앞접힘점8
	GLandMark foldb = LandMark[45]; //오른쪽겨드랑뒤점힘점8

	CHECK(shp);
	CHECK(armpit);
	CHECK(f);
	CHECK(b);
	CHECK(foldf);
	CHECK(foldb);

	//겨드랑점 z좌표 조정  F:B = 0.531: 0.469
	armpit.Z = f.Z*0.469f + b.Z*0.531f;

	//겨드랑아래 보조점 계산하기
	GLandMark ff,bb;
	GVector3f fcornerRdn = GVector3f( armpit.X, armpit.Y,	f.Z);
	GVector3f bcornerLdn = GVector3f( armpit.X, armpit.Y,	b.Z);
	GVector3f fcornerLup = GVector3f( armpit.X, f.Y,	armpit.Z);
	GVector3f bcornerRup = GVector3f( armpit.X, b.Y,	armpit.Z);

	 GVector3f intersectPtF, intersectPtB;

	::GetIntersectPointZY( f, armpit, fcornerLup,  fcornerRdn, &intersectPtF);
	::GetIntersectPointZY( b, armpit, bcornerRup,  bcornerLdn, &intersectPtB);	

	ff = GVector3f( (foldf.X*0.3f + armpit.X*0.7f),
					(intersectPtF.Y*0.5f + fcornerRdn.Y*0.5f), //0.4, 0.6
					(intersectPtF.Z*0.5f + fcornerRdn.Z*0.5f) );

	bb = GVector3f( (foldb.X*0.3f+ armpit.X*0.7f),
					(intersectPtB.Y*0.4f + bcornerLdn.Y*0.6f), //0.33 0.67
					(intersectPtB.Z*0.4f + bcornerLdn.Z*0.6f) );
  	

	//새로운 겨드랑점 = 앞점힘점과 뒤접힘점의 중간, 그냥 겨드랑점으로 하면 겨드랑점을 앞에 새로찍었을때 z가 중간이 되지 못한다

	//armpit.Z=( f.Z + b.Z ) * 0.5f;

	CrossPtList.Insert( shp );
	//CrossPtList.Insert( armpit );
	CrossPtList.Insert( f );
	CrossPtList.Insert( b );

	// make plane
	GVector3f tmp;
	tmp = b; tmp.X -= 100.0f;
	GPlane refPln( foldb, tmp, foldf );   //겨드랑 앞,뒤 접힘점을 지나는 수평에 가까운 평면
	GPlane cutPln( shp, foldf, foldb );   //어깨점과 겨드랑 앞,뒤 접힘점을 지나는 수직에 가까운 평면
	

	//어깨점과 겨드랑 앞,뒤 접힘점을 지나는 평면위에 있고, 겨드랑 앞,뒤 접힘점  위에 있는 점만 모으기
	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		// culling.
		if ( v0.Y > shp.Y+2.0f && v1.Y > shp.Y+2.0f && v2.Y > shp.Y+2.0f )  //어깨점 아래만
			continue;
		if ( v0.Y < (armpit.Y-2.0f) && v1.Y < (armpit.Y-2.0f) && v2.Y < (armpit.Y-2.0f) )  // 겨드랑점 위만
			continue;
		if ( v0.Z > f.Z+10.0f && v1.Z > f.Z+10.0f && v2.Z > f.Z+10.0f )  //겨드랑앞점힘점 뒤만
			continue;
		if ( v0.Z < b.Z-20.0f && v1.Z < b.Z-20.0f && v2.Z < b.Z-20.0f )  //겨드랑뒤점힘점 앞만
			continue;

		if ( refPln.IsBelowPlane( v0 ) && refPln.IsBelowPlane( v1 ) && refPln.IsBelowPlane( v2 ) )
			continue;

		if ( cutPln.IsBelowPlane( v0 ) && cutPln.IsBelowPlane( v1 ) && cutPln.IsBelowPlane( v2 ) )
			continue;

		if ( cutPln.IsAbovePlane( v0 ) && cutPln.IsAbovePlane( v1 ) && cutPln.IsAbovePlane( v2 ) )
			continue;

		if ( cutPln.GetSignedDistance( v0 ) * cutPln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v1 ) * cutPln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v2 ) * cutPln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}

	// make GNurbsCrv for path.
	/*REAL4 P[3];
	P[0][0] = d[0]; P[0][1] = d[1]; P[0][2] = d[2]; P[0][3] = 1.0f;
	P[1][0] = b[0]; P[1][1] = b[1]; P[1][2] = b[2]; P[1][3] = 1.0f;
	P[2][0] = e[0]; P[2][1] = e[1]; P[2][2] = e[2]; P[2][3] = 1.0f;
	GNurbsCrv PathCrv;
	PathCrv.InterCrv( 2, P, 2 );*/
	// 겨드랑 앞,뒤 접힘점 아래는 곡선으로 --> 앞에서 뒤의 순서로 
	REAL4 P[5];
	P[0][0] = foldf[0];		P[0][1] = foldf[1];		P[0][2] = foldf[2];		P[0][3] = 1.0f;
	P[1][0] = ff[0];		P[1][1] = ff[1];		P[1][2] = ff[2];		P[1][3] = 1.0f;
	P[2][0] = armpit[0];	P[2][1] = armpit[1];	P[2][2] = armpit[2];    P[2][3] = 1.0f;
	P[3][0] = bb[0];		P[3][1] = bb[1];		P[3][2] = bb[2];		P[3][3] = 1.0f;
	P[4][0] = foldb[0];		P[4][1] = foldb[1];		P[4][2] = foldb[2];		P[4][3] = 1.0f;
	GNurbsCrv PathCrv;
	PathCrv.InterCrv( 3, P, 4 );

	int snum = 20;
	REAL4 v0;
	REAL min = PathCrv.GetMinKnotVal();
	REAL max = PathCrv.GetMaxKnotVal();
	for ( i = 0; i < snum; i++ )
	{
		REAL u0 = min + ( (max-min)*i ) / (REAL)(snum-1);
		PathCrv.Eval( u0, v0 );
		GVector3f pt( v0 );
		CrossPtList.Insert( pt );
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullYZ();
	//cvx.m_HullPtList = cvx.m_PtList;

	cvx.ArrangeHullPtYZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;
	Result[25] = val;
}



// 측정항목 88: 겨드랑둘레 길이 -Left
// 필요한 측정점(인덱스) : 왼쪽 어깨가쪽점(8), 왼쪽 겨드랑점(10), 
//                         왼쪽겨드랑앞접힘점(44),  왼쪽겨드랑뒤접힘점(45).
// 설명: 위쪽은 뒤겨드랑접힘점, 어깨끝점, 앞겨드랑접힘점 포함되는 길이.
// 	     아래쪽은 곡률을 고려한 가상의 선.

void GetResultPose2_88 ( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	int i;
	GObList<GVector3f> CrossPtList; 
	GLandMark a = LandMark[8];   //왼 어깨가쪽
	GLandMark c = LandMark[10];  //왼 겨드랑
	GLandMark f = LandMark[46];  //왼쪽 겨드랑앞접힘점
	GLandMark b = LandMark[47];  //왼쪽 겨드랑뒤점힘점
	CHECK(a);
	CHECK(c);
	CHECK(f);
	CHECK(b);
	GLandMark c1 = ( f + b ) * 0.5f; //새로운 겨드랑점 = 앞점힘점과 뒤접힘점의 중간
	c1.X = (f.X*0.55f) + (b.X*0.45f); //좌우위치는 앞쪽점 가깝게 약간 이동
	c1.Y = c.Y-6.0f;                       //높이는 원래 겨드랑점 높이 -5mm
	if(f.Y<c1.Y) f.Y=c1.Y+3.0f;
	if(b.Y<c1.Y) b.Y=c1.Y+3.0f;

	f.X +=2.0f;
	b.X +=1.0f;

	GLandMark ff, bb; 
	ff= ( c1+f ) * 0.5f;  //d*0.25f + e*0.75f; 
	bb= ( c1+b ) * 0.5f;  //b*0.5f + e*0.25f; 
	float yd = (ff.Y-c1.Y)*0.4f;
	float zd = (ff.Z-c1.Z)*0.3f;
	//float xd = (ff.X-c1.X)*0.5f;
	ff.Y = ff.Y-yd;
	bb.Y = bb.Y-yd;
	ff.Z = ff.Z+zd;
	bb.Z = bb.Z-zd;
	//ff.X = f.X;
	bb.X = (c1.X*0.4f) + (b.X*0.6f);

	CrossPtList.Insert( a );
	//CrossPtList.Insert( c );
	CrossPtList.Insert( f );
	CrossPtList.Insert( b );

	// make plane
	GVector3f tmp;
	tmp = b; tmp.X -= 100.0f;
	GPlane refPln( b, tmp, f );   //겨드랑 앞,뒤 접힘점을 지나는 수평에 가까운 평면
	GPlane cutPln( a, f, b );   //어깨점과 겨드랑 앞,뒤 접힘점을 지나는 수직에 가까운 평면
	
	//b = cutPln.GetProjectionPt( b, GVector3f( 1.0f, 0.0f, 0.0f ) );
	
	//어깨점과 겨드랑 앞,뒤 접힘점을 지나는 평면위에 있고, 겨드랑 앞,뒤 접힘점  위에 있는 점만 모으기
	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		// culling.
		if ( v0.Y > a.Y+20.0f && v1.Y > a.Y+20.0f && v2.Y > a.Y+20.0f )  //어깨점 아래만
			continue;
		if ( v0.Y < c.Y-20.0f && v1.Y < c.Y-20.0f && v2.Y < c.Y-20.0f )  // 겨드랑점 위만
			continue;
		if ( v0.Z > f.Z+20.0f && v1.Z > f.Z+20.0f && v2.Z > f.Z+20.0f )  //겨드랑앞점힘점 뒤만
			continue;
		if ( v0.Z < b.Z-20.0f && v1.Z < b.Z-20.0f && v2.Z < b.Z-20.0f )  //겨드랑뒤점힘점 앞만
			continue;

		if ( refPln.IsBelowPlane( v0 ) && refPln.IsBelowPlane( v1 ) && refPln.IsBelowPlane( v2 ) )
			continue;

		if ( cutPln.IsBelowPlane( v0 ) && cutPln.IsBelowPlane( v1 ) && cutPln.IsBelowPlane( v2 ) )
			continue;

		if ( cutPln.IsAbovePlane( v0 ) && cutPln.IsAbovePlane( v1 ) && cutPln.IsAbovePlane( v2 ) )
			continue;

		if ( cutPln.GetSignedDistance( v0 ) * cutPln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v1 ) * cutPln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v2 ) * cutPln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}

	// make GNurbsCrv for path.
	/*REAL4 P[3];
	P[0][0] = d[0]; P[0][1] = d[1]; P[0][2] = d[2]; P[0][3] = 1.0f;
	P[1][0] = b[0]; P[1][1] = b[1]; P[1][2] = b[2]; P[1][3] = 1.0f;
	P[2][0] = e[0]; P[2][1] = e[1]; P[2][2] = e[2]; P[2][3] = 1.0f;
	GNurbsCrv PathCrv;
	PathCrv.InterCrv( 2, P, 2 );*/
	/* 겨드랑 앞,뒤 접힘점 아래는 곡선으로 --> 앞에서 뒤의 순서로 
	REAL4 P[5];
	P[0][0] = f[0];   P[0][1] =  f[1];  P[0][2] =  f[2];  P[0][3] = 1.0f;
	P[1][0] = ff[0];  P[1][1] = ff[1];  P[1][2] = ff[2];  P[1][3] = 1.0f;
	P[2][0] = c1[0];  P[2][1] = c1[1];  P[2][2] = c1[2];  P[2][3] = 1.0f;
	P[3][0] = bb[0];  P[3][1] = bb[1];  P[3][2] = bb[2];  P[3][3] = 1.0f;
	P[4][0] = b[0];   P[4][1] =  b[1];  P[4][2] =  b[2];  P[4][3] = 1.0f;
	GNurbsCrv PathCrv;
	PathCrv.InterCrv( 3, P, 4 );

	int snum = 20;
	REAL4 v0;
	REAL min = PathCrv.GetMinKnotVal();
	REAL max = PathCrv.GetMaxKnotVal();
	for ( i = 0; i < snum; i++ )
	{
		REAL u0 = min + ( (max-min)*i ) / (REAL)(snum-1);
		PathCrv.Eval( u0, v0 );
		GVector3f pt( v0 );
		CrossPtList.Insert( pt );
	}
	*/
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullYZ();
	cvx.ArrangeHullPtYZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;
	Result[88] = val;
}



// 측정항목 26_70_79 : 가슴 둘레, 가슴 너비, 가슴 두께.
// 필요한 측정점(인덱스) :  오른쪽 겨드랑점(9), 
//							오른쪽 겨드랑앞접힘점(12),	오른쪽 겨드랑뒤접힘점(14),
//							왼쪽 겨드랑앞접힘점(13),    왼쪽 겨드랑뒤접힘점(15),
//							오른쪽 어깨가쪽점(7),		왼쪽 어깨가쪽점(8).
// 설명 : 오른쪽 겨드랑점 높이에서의 수평면과 만나는 점들의 둘레, 너비, 두께.     
void GetResultPose2_26_70_79( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 )
{
	(*CvxPt0).DeleteAll();	// 가슴둘레.
	(*CvxPt1).DeleteAll();	// 가슴너비.
	(*CvxPt2).DeleteAll();	// 가슴두께.

	GObList<GVector3f> CrossPtList;
	GLandMark armpitR = LandMark[9]; 
	GLandMark armpitL = LandMark[10]; 
	GLandMark aFR = LandMark[44];
	GLandMark aBR = LandMark[45];
	GLandMark aFL = LandMark[46];
	GLandMark aBL = LandMark[47];

	CHECK(armpitR);
	CHECK(aFR);
	CHECK(aBR);
	CHECK(aFL);
	CHECK(aBL);

	GPlane cutPln( GVector3f( 0.0f, 1.0f, 0.0f), armpitR );

	GVector3f normR= aFR-aBR; //GVector3f( -1.0, 0.0, 0.05 ); z +
	GVector3f normL= aFL-aBL; //GVector3f( 1.0, 0.0, 0.05 );

	normR.Y = normL.Y =0.0f;

	float tmp;
	tmp=normR.X;
	normR.X = -normR.Z;
	normR.Z=tmp;

	tmp=normL.X;
	normL.X = normL.Z;
	normL.Z = -tmp;

	GPlane refPln0( normR, armpitR ); 
	GPlane refPln1( normL, armpitL ); 
	
	
	int i;
	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		if ( cutPln.IsAbovePlane(v0) && cutPln.IsAbovePlane(v1) && cutPln.IsAbovePlane(v2) )
			continue;
		if ( cutPln.IsBelowPlane(v0) && cutPln.IsBelowPlane(v1) && cutPln.IsBelowPlane(v2) )
			continue;
		if ( refPln0.IsAbovePlane(v0) || refPln0.IsAbovePlane(v1) || refPln0.IsAbovePlane(v2) )
			continue;
		if ( refPln1.IsAbovePlane(v0) || refPln1.IsAbovePlane(v1) || refPln1.IsAbovePlane(v2) )
			continue;

		if ( cutPln.GetSignedDistance( v0 ) * cutPln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v1 ) * cutPln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v2 ) * cutPln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	(*CvxPt0) = cvx.m_HullPtList;

	float minx = 100000.0f;
	float maxx = -100000.0f;
	float minz = 100000.0f;
	float maxz = -100000.0f;
	GObNode<GVector3f> *pNode = CrossPtList.m_pLeftEnd;
	while( pNode )
	{
		GVector3f pt = pNode->data;
		minx = MIN( minx, pt[0] );
		maxx = MAX( maxx, pt[0] );
		if(pt.X<LandMark[25].X) {
			minz = MIN( minz, pt[2] );
			maxz = MAX( maxz, pt[2] );
		}
		pNode = pNode->m_pRight;
	}

	GVector3f pt0( minx, armpitR.Y, armpitR.Z );
	GVector3f pt1( maxx, armpitR.Y, armpitR.Z );
	(*CvxPt1).Insert( pt0 );
	(*CvxPt1).Insert( pt1 );

	pt0.setVector( 400.0f, armpitR.Y, minz );
	pt1.setVector( 400.0f, armpitR.Y, maxz );
	(*CvxPt2).Insert( pt0 );
	(*CvxPt2).Insert( pt1 );

	Result[26] = cvx.GetHullLength();
	Result[70] = fabs( maxx - minx );
	Result[79] = fabs( maxz - minz );

}


//	팔이 포함되지 않게하기 위해서 좌우 범위를  좁혀준 것
// 측정항목    26_70_79 : 가슴 둘레, 가슴 너비, 가슴 두께.
// 필요한 측정점:  오른쪽 겨드랑점(9), 
//		 		   오른쪽 어깨가쪽점(7),	   왼쪽 어깨가쪽점(8).
//		 보조점:   오른쪽 겨드랑앞접힘점(42), 오른쪽 겨드랑뒤접힘점(43),
//				   왼쪽 겨드랑앞접힘점(44),	  왼쪽 겨드랑뒤접힘점(45),
//							
//							
// 설명 : 오른쪽 겨드랑점 높이에서의 수평면과 만나는 점들의 둘레, 너비, 두께.

void GetResultPose2_26_70_79_a( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 )
{
	(*CvxPt0).DeleteAll();	// 가슴둘레.
	(*CvxPt1).DeleteAll();	// 가슴너비.
	(*CvxPt2).DeleteAll();	// 가슴두께.

	GObList<GVector3f> CrossPtList;
	GLandMark armpit = LandMark[9];   
	GLandMark aFR = LandMark[44]; 
	GLandMark aBR = LandMark[45]; 
	GLandMark aFL = LandMark[46]; 
	GLandMark aBL = LandMark[47]; 

	CHECK(armpit);
	CHECK(aFR);
	CHECK(aBR);
	CHECK(aFL);
	CHECK(aBL);

	GVector3f ArmpitBelow = armpit;
	ArmpitBelow.Y -=4.0f;
	aFR.X +=8.0f;      //팔이 포함되지 않게하기 위해서 좌우 범위를  좁혀준 것
	aBR.X +=5.0f;
	aFL.X -=10.0f;
	aBL.X -=5.0f;

	GPlane cutPln( GVector3f( 0.0f, 1.0f, 0.0f ), ArmpitBelow );

	GVector3f tmpR= GVector3f( aFR.X, aFR.Y-10.0, aFR.Z );
	GVector3f tmpL= GVector3f( aFL.X, aFL.Y-10.0, aFL.Z );

	GPlane refPln0( aFR, aBR, tmpR ); //오른쯕 겨드랑앞뒤접힘점, 수직 평면
	GPlane refPln1( aBL, aFL, tmpL ); //왼쪽 "                   수직 평면
	
	int i;
	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		if ( cutPln.IsAbovePlane(v0) && cutPln.IsAbovePlane(v1) && cutPln.IsAbovePlane(v2) )
			continue;
		if ( cutPln.IsBelowPlane(v0) && cutPln.IsBelowPlane(v1) && cutPln.IsBelowPlane(v2) )
			continue;
		if ( refPln0.IsAbovePlane(v0) || refPln0.IsAbovePlane(v1) || refPln0.IsAbovePlane(v2) )
			continue;
		if ( refPln1.IsAbovePlane(v0) || refPln1.IsAbovePlane(v1) || refPln1.IsAbovePlane(v2) )
			continue;

		if ( cutPln.GetSignedDistance( v0 ) * cutPln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v1 ) * cutPln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v2 ) * cutPln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	(*CvxPt0) = cvx.m_HullPtList;

	float minx = 100000.0f;
	float maxx = -100000.0f;
	float minz = 100000.0f;
	float maxz = -100000.0f;
	GObNode<GVector3f> *pNode = CrossPtList.m_pLeftEnd;
	while( pNode )
	{
		GVector3f pt = pNode->data;
		minx = MIN( minx, pt[0] );
		maxx = MAX( maxx, pt[0] );
		if(pt.X<LandMark[25].X) {
			minz = MIN( minz, pt[2] );
			maxz = MAX( maxz, pt[2] );
		}
		pNode = pNode->m_pRight;
	}

	GVector3f pt0( minx, ArmpitBelow.Y, 0.0f );
	GVector3f pt1( maxx, ArmpitBelow.Y, 0.0f );
	(*CvxPt1).Insert( pt0 );
	(*CvxPt1).Insert( pt1 );

	pt0.setVector( 400.0f, ArmpitBelow.Y, minz );
	pt1.setVector( 400.0f, ArmpitBelow.Y, maxz );
	(*CvxPt2).Insert( pt0 );
	(*CvxPt2).Insert( pt1 );

	Result[26] = cvx.GetHullLength();
	Result[71] = fabs( maxx - minx );
	Result[79] = fabs( maxz - minz );

}


/**********************************************************************
 * 측정항목 27_77_71_80  젖가슴둘레, 젖꼭지사이길이, 젖가슴 너비, 젖가슴두께, 
 * 필요한 측정점(인덱스) :	오른쪽 젖꼭지점(16), 
 *							왼쪽 젖꼭지점(17), 
 *							오른쪽 겨드랑점(9), 
 *							왼쪽 겨드랑점(10)
 * 설명 : 오른쪽, 왼쪽 젖꼭지점 평균 높이에서의 수평 둘레
 * 검증 : 2004-6-26
 **********************************************************************/
void GetResultPose2_27_77_71_80( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark,  GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2, GObList<GVector3f> *CvxPt3 )
{
	(*CvxPt0).DeleteAll();	// 젖가슴 둘레.
	(*CvxPt1).DeleteAll();	// 젖꼭지사이길이.
	(*CvxPt2).DeleteAll();  // 젖가슴 너비.
	(*CvxPt3).DeleteAll();  // 젖가슴 두께.

	GObList<GVector3f> CrossPtList, CrossPtList1;
	GLandMark BPr = LandMark[16];
	GLandMark BPl = LandMark[17];
	GLandMark c =  LandMark[9];  //LandMarkHelp[1];
	GLandMark d =  LandMark[10]; //LandMarkHelp[2];
	
	CHECK(BPr);	
	CHECK(BPl);
	CHECK(c);	
	CHECK(d);

	// plane setting.
	//GPlane pln[3];
	GPlane pln0, pln1, cutPln;
	double tan1 = tan( M_PI / 2.0 - RefAngle[5]);
	double tan2 = tan( M_PI / 2.0 + RefAngle[6]);

	pln0.MakePlane( GVector3f( -1.0f, tan1, 0.0f ), c );
	pln1.MakePlane( GVector3f( 1.0f, tan2, 0.0f ), d );
	cutPln.MakePlane( GVector3f( 0.0f, 1.0f, 0.0f ), GVector3f( 0.0, BPr.Y, 0.0 ) );

	int i;
	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		if ( cutPln.IsAbovePlane(v0) && cutPln.IsAbovePlane(v1) && cutPln.IsAbovePlane(v2) )
			continue;
		if ( cutPln.IsBelowPlane(v0) && cutPln.IsBelowPlane(v1) && cutPln.IsBelowPlane(v2) )
			continue;
		if ( pln0.IsAbovePlane(v0) || pln0.IsAbovePlane(v1) || pln0.IsAbovePlane(v2) )
			continue;
		if ( pln1.IsAbovePlane(v0) || pln1.IsAbovePlane(v1) || pln1.IsAbovePlane(v2) )
			continue;
	
		if ( cutPln.GetSignedDistance( v0 ) * cutPln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}			
		if ( cutPln.GetSignedDistance( v1 ) * cutPln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );	
		}
		if ( cutPln.GetSignedDistance( v2 ) * cutPln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );		
		}

	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	(*CvxPt0) = cvx.m_HullPtList;  //젖가슴둘레

	float minx = 100000.0f;
	float maxx = -100000.0f;
	float minz = 100000.0f;
	float maxz = -100000.0f;
	GObNode<GVector3f> *pNode = cvx.m_HullPtList.m_pLeftEnd;
	while( pNode )
	{
		GVector3f pt = pNode->data;
		minx = MIN( minx, pt[0] );
		maxx = MAX( maxx, pt[0] );
		if(pt.X<LandMark[25].X) {
			minz = MIN( minz, pt[2] );
			maxz = MAX( maxz, pt[2] );
		}
		
		if(pt.X >BPr.X && pt.X <BPl.X && pt.Z >BPr.Z)
				CrossPtList1.Insert(pt); //젖꼭지사이길이

		pNode = pNode->m_pRight;
	}
	CrossPtList1.Insert(BPr);
	CrossPtList1.Insert(BPl);
	::getSortedPoint( CrossPtList1, 0, 0 );
	(*CvxPt1) = CrossPtList1;	
	Result[77] = ::getSortedPtLen(CrossPtList1); //젖꼭지사이길이


	GVector3f pt0( minx, BPr.Y, 0.0f );
	GVector3f pt1( maxx, BPr.Y, 0.0f );
	(*CvxPt2).Insert( pt0 );
	(*CvxPt2).Insert( pt1 );		//젖가슴너비

	pt0.setVector( 0.0f, BPr.Y, minz );
	pt1.setVector( 0.0f, BPr.Y, maxz );
	(*CvxPt3).Insert( pt0 );
	(*CvxPt3).Insert( pt1 );		//젖가슴두게

	Result[27] = cvx.GetHullLength();	
	Result[71] = fabs( maxx - minx );
	Result[80] = fabs( maxz - minz );
}

/**********************************************************************
 * 측정항목 28_72_81 : 젖가슴아래둘레, 젖가슴 아래 너비, 젖가슴 아래 두께.
 * 필요한 측정점(인덱스) :	오른쪽 젖가슴 아래점(18), 
 *							오른쪽 겨드랑점(9), 
 *							왼쪽 겨드랑점(10)
 * 설명 : 오른쪽 젖가슴 아래점 높이에서의 수평 둘레
 * 검증 : 2004-6-26
 **********************************************************************/
void GetResultPose2_28_72_81( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 )
{
	(*CvxPt0).DeleteAll();
	(*CvxPt1).DeleteAll();
	(*CvxPt2).DeleteAll();

	GObList<GVector3f> CrossPtList;
	GLandMark b = LandMark[18];
	GLandMark c = LandMark[9];
	GLandMark d = LandMark[10];
	CHECK(b);
	CHECK(c);
	CHECK(d);

	// plane setting.
	double tan1 = tan( M_PI / 2.0 - RefAngle[5]);
	double tan2 = tan( M_PI / 2.0 + RefAngle[6]);
	GPlane pln[3];
	pln[0].MakePlane( GVector3f( -1.0f, tan1, 0.0f ), c );
	pln[1].MakePlane( GVector3f( 1.0f, tan2, 0.0f ), d );
	pln[2].MakePlane( GVector3f( 0.0f, 1.0f, 0.0f ), b );

	int i;
	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln[0].IsAbovePlane(v0) || pln[0].IsAbovePlane(v1) || pln[0].IsAbovePlane(v2) )
			continue;
		if ( pln[1].IsAbovePlane(v0) || pln[1].IsAbovePlane(v1) || pln[1].IsAbovePlane(v2) )
			continue;
		if ( pln[2].IsAbovePlane(v0) && pln[2].IsAbovePlane(v1) && pln[2].IsAbovePlane(v2) )
			continue;
		if ( pln[2].IsBelowPlane(v0) && pln[2].IsBelowPlane(v1) && pln[2].IsBelowPlane(v2) )
			continue;

		if ( pln[2].GetSignedDistance( v0 ) * pln[2].GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln[2].GetSignedDistance( v1 ) * pln[2].GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln[2].GetSignedDistance( v2 ) * pln[2].GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	(*CvxPt0) = cvx.m_HullPtList;

	float minx = 100000.0f;
	float maxx = -100000.0f;
	float minz = 100000.0f;
	float maxz = -100000.0f;
	GObNode<GVector3f> *pNode = CrossPtList.m_pLeftEnd;
	while( pNode )
	{
		GVector3f pt = pNode->data;
		minx = MIN( minx, pt[0] );
		maxx = MAX( maxx, pt[0] );
		if(pt.X<LandMark[25].X) {
			minz = MIN( minz, pt[2] );
			maxz = MAX( maxz, pt[2] );
		}
		pNode = pNode->m_pRight;
	}

	GVector3f pt0( minx, b.Y, 0.0f );
	GVector3f pt1( maxx, b.Y, 0.0f );
	(*CvxPt1).Insert( pt0 );
	(*CvxPt1).Insert( pt1 );

	pt0.setVector( 0.0f, b.Y, minz );
	pt1.setVector( 0.0f, b.Y, maxz );
	(*CvxPt2).Insert( pt0 );
	(*CvxPt2).Insert( pt1 );

	Result[28] = cvx.GetHullLength();
	Result[72] = fabs( maxx - minx );
	Result[81] = fabs( maxz - minz );
}
/**********************************************************
 * 측정항목 29_73_82 : 허리둘레, 허리 너비, 허리 두께.
 * 필요한 측정점(인덱스) :  오른쪽 허리 옆점(19), 
 *							오른쪽 겨드랑점(9), 
 *							왼쪽 겨드랑점(10)
 * 설명 : 오른쪽 허리 옆점 높이에서의 수평 둘레
 **********************************************************/
void GetResultPose2_29_73_82( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 )
{
	(*CvxPt0).DeleteAll();
	(*CvxPt1).DeleteAll();
	(*CvxPt2).DeleteAll();

	int i;
	GLandMark b, c, d;
	GLandMark n( 0.0f, 1.0f, 0.0 );
	GObList<GVector3f> CrossPtList;
	b = LandMark[19];
	c = LandMarkHelp[42];
	d = LandMarkHelp[43];
	CHECK(b);
	CHECK(c);
	CHECK(d);
	// plane setting.
	double tan1 = tan( M_PI / 2.0 - RefAngle[3]);
	double tan2 = tan( M_PI / 2.0 + RefAngle[4]);
	
	GPlane pln[2], cutPln;
	pln[0].MakePlane( GVector3f( -1.0f, tan1, 0.0f ), c );
	pln[1].MakePlane( GVector3f( 1.0f, tan2, 0.0f ), d );
	cutPln.MakePlane( GVector3f( 0.0f, 1.0f, 0.0f ), b );

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln[0].IsAbovePlane(v0) || pln[0].IsAbovePlane(v1) || pln[0].IsAbovePlane(v2) )
			continue;
		if ( pln[1].IsAbovePlane(v0) || pln[1].IsAbovePlane(v1) || pln[1].IsAbovePlane(v2) )
			continue;
		if ( cutPln.IsAbovePlane(v0) && cutPln.IsAbovePlane(v1) && cutPln.IsAbovePlane(v2) )
			continue;
		if ( cutPln.IsBelowPlane(v0) && cutPln.IsBelowPlane(v1) && cutPln.IsBelowPlane(v2) )
			continue;
		if ( cutPln.GetSignedDistance( v0 ) * cutPln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v1 ) * cutPln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v2 ) * cutPln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	(*CvxPt0) = cvx.m_HullPtList;
	
	float minx = 100000.0f;
	float maxx = -100000.0f;
	float minz = 100000.0f;
	float maxz = -100000.0f;
	GObNode<GVector3f> *pNode = CrossPtList.m_pLeftEnd;
	while( pNode )
	{
		GVector3f pt = pNode->data;
		minx = MIN( minx, pt[0] );
		maxx = MAX( maxx, pt[0] );
		if(pt.X<LandMark[25].X) {
			minz = MIN( minz, pt[2] );
			maxz = MAX( maxz, pt[2] );
		}
		pNode = pNode->m_pRight;
	}

	GVector3f pt0( minx, b.Y, 0.0f );
	GVector3f pt1( maxx, b.Y, 0.0f );
	(*CvxPt1).Insert( pt0 );
	(*CvxPt1).Insert( pt1 );

	pt0.setVector( 0.0f, b.Y, minz );
	pt1.setVector( 0.0f, b.Y, maxz );
	(*CvxPt2).Insert( pt0 );
	(*CvxPt2).Insert( pt1 );

	Result[29] = cvx.GetHullLength();
	Result[73] = fabs( maxx - minx );
	Result[82] = fabs( maxz - minz );
}


/*************************************************************
 * 측정항목    30_74_83: 배둘레, 배너비, 배두께
 * 필요한 측정점(인덱스) :  배돌출점(23), 
 *							오른쪽 겨드랑점(9), 
 *							왼쪽 겨드랑점(10)
 * 설명 : 배돌출점 높이에서의 수평 둘레
 *************************************************************/
void GetResultPose2_30_74_83( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 )
{
	(*CvxPt0).DeleteAll();
	(*CvxPt1).DeleteAll();
	(*CvxPt2).DeleteAll();

	int i;
	GLandMark b, c, d;
	GVector3f n( 0.0f, 1.0f, 0.0 );
	GObList<GVector3f> CrossPtList;
	b = LandMark[23];
	c = LandMarkHelp[42]; //9
	d = LandMarkHelp[43];
	CHECK(b);
	CHECK(c);
	CHECK(d);
	// plane setting.
	double tan1 = tan( M_PI / 2.0 - RefAngle[3]);
	double tan2 = tan( M_PI / 2.0 + RefAngle[4]);

	GPlane pln[3];
	pln[0].MakePlane( GVector3f( -1.0f, tan1, 0.0f ), c );
	pln[1].MakePlane( GVector3f( 1.0f, tan2, 0.0f ), d );
	pln[2].MakePlane( GVector3f( 0.0f, 1.0f, 0.0f ), b );

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln[0].IsAbovePlane(v0) || pln[0].IsAbovePlane(v1) || pln[0].IsAbovePlane(v2) )
			continue;
		if ( pln[1].IsAbovePlane(v0) || pln[1].IsAbovePlane(v1) || pln[1].IsAbovePlane(v2) )
			continue;
		if ( pln[2].IsAbovePlane(v0) && pln[2].IsAbovePlane(v1) && pln[2].IsAbovePlane(v2) )
			continue;
		if ( pln[2].IsBelowPlane(v0) && pln[2].IsBelowPlane(v1) && pln[2].IsBelowPlane(v2) )
			continue;
		if ( pln[2].GetSignedDistance( v0 ) * pln[2].GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln[2].GetSignedDistance( v1 ) * pln[2].GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln[2].GetSignedDistance( v2 ) * pln[2].GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	(*CvxPt0) = cvx.m_HullPtList;
	
	float minx = 100000.0f;
	float maxx = -100000.0f;
	float minz = 100000.0f;
	float maxz = -100000.0f;
	GObNode<GVector3f> *pNode = CrossPtList.m_pLeftEnd;
	while( pNode )
	{
		GVector3f pt = pNode->data;
		minx = MIN( minx, pt[0] );
		maxx = MAX( maxx, pt[0] );
		if(pt.X<LandMark[25].X) {
			minz = MIN( minz, pt[2] );
			maxz = MAX( maxz, pt[2] );
		}
		pNode = pNode->m_pRight;
	}

	GVector3f pt0( minx, b.Y, 0.0f );
	GVector3f pt1( maxx, b.Y, 0.0f );
	(*CvxPt1).Insert( pt0 );
	(*CvxPt1).Insert( pt1 );

	pt0.setVector( 0.0f, b.Y, minz );
	pt1.setVector( 0.0f, b.Y, maxz );
	(*CvxPt2).Insert( pt0 );
	(*CvxPt2).Insert( pt1 );


	Result[30] = cvx.GetHullLength();
	Result[74] = fabs( maxx - minx );
	Result[83] = fabs( maxz - minz );

}

/*************************************************************
 * 측정항목    31: 배꼽수준허리둘레,  75,  84
 * 필요한 측정점(인덱스) :  배꼽(35), 
 *							오른쪽 겨드랑점(9), 
 *							왼쪽 겨드랑점(10)
 * 설명 : 배꼽점 높이에서의 수평 둘레
 *************************************************************/
void GetResultPose2_31_75_84( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2)
{
	(*CvxPt0).DeleteAll();

	int i;
	GLandMark b, c, d;
	GVector3f n( 0.0f, 1.0f, 0.0 );
	GObList<GVector3f> CrossPtList;
	b = LandMark[35];
	c = LandMarkHelp[42]; //9
	d = LandMarkHelp[43];
	CHECK(b);
	CHECK(c);
	CHECK(d);
	// plane setting.
	double tan1 = tan( M_PI / 2.0 - RefAngle[3]);
	double tan2 = tan( M_PI / 2.0 + RefAngle[4]);

	GPlane pln[3];
	pln[0].MakePlane( GVector3f( -1.0f, tan1, 0.0f ), c );
	pln[1].MakePlane( GVector3f( 1.0f, tan2, 0.0f ), d );
	pln[2].MakePlane( GVector3f( 0.0f, 1.0f, 0.0f ), b );

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln[0].IsAbovePlane(v0) || pln[0].IsAbovePlane(v1) || pln[0].IsAbovePlane(v2) )
			continue;
		if ( pln[1].IsAbovePlane(v0) || pln[1].IsAbovePlane(v1) || pln[1].IsAbovePlane(v2) )
			continue;
		if ( pln[2].IsAbovePlane(v0) && pln[2].IsAbovePlane(v1) && pln[2].IsAbovePlane(v2) )
			continue;
		if ( pln[2].IsBelowPlane(v0) && pln[2].IsBelowPlane(v1) && pln[2].IsBelowPlane(v2) )
			continue;
		if ( pln[2].GetSignedDistance( v0 ) * pln[2].GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln[2].GetSignedDistance( v1 ) * pln[2].GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln[2].GetSignedDistance( v2 ) * pln[2].GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	(*CvxPt0) = cvx.m_HullPtList;	
	Result[31] = cvx.GetHullLength();


	float minx = 100000.0f;
	float maxx = -100000.0f;
	float minz = 100000.0f;
	float maxz = -100000.0f;
	GObNode<GVector3f> *pNode = CrossPtList.m_pLeftEnd;
	while( pNode )
	{
		GVector3f pt = pNode->data;
		minx = MIN( minx, pt[0] );
		maxx = MAX( maxx, pt[0] );
		if(pt.X<LandMark[25].X) {
			minz = MIN( minz, pt[2] );
			maxz = MAX( maxz, pt[2] );
		}
		pNode = pNode->m_pRight;
	}

	GVector3f pt0( minx, b.Y, 0.0f );
	GVector3f pt1( maxx, b.Y, 0.0f );
	(*CvxPt1).Insert( pt0 );
	(*CvxPt1).Insert( pt1 );

	pt0.setVector( 0.0f, b.Y, minz );
	pt1.setVector( 0.0f, b.Y, maxz );
	(*CvxPt2).Insert( pt0 );
	(*CvxPt2).Insert( pt1 );

	Result[75] = fabs( maxx - minx );
	Result[84] = fabs( maxz - minz );
}

/*************************************************************
 * 측정항목 32_76_85: 엉덩이둘레, 엉덩이너비, 엉덩이두께
  * 필요한 측정점(인덱스) :  엉덩이돌출점(23), 
 *							오른쪽 겨드랑점(9), 
 *							왼쪽 겨드랑점(10)
 * 설명 : 배돌출점 높이에서의 수평 둘레
 *************************************************************/
void GetResultPose2_32_76_85( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 )
{
	(*CvxPt0).DeleteAll();
	(*CvxPt1).DeleteAll();
	(*CvxPt2).DeleteAll();

	int i;
	GLandMark b, c, d;
	GVector3f n( 0.0f, 1.0f, 0.0 );
	GObList<GVector3f> CrossPtList;
	b = LandMark[24];
	c = LandMarkHelp[42]; //9
	d = LandMarkHelp[43];
	CHECK(b);
	CHECK(c);
	CHECK(d);
	// plane setting.
	double tan1 = tan( M_PI / 2.0 - RefAngle[3]);
	double tan2 = tan( M_PI / 2.0 + RefAngle[4]);

	GPlane pln[3];
	pln[0].MakePlane( GVector3f( -1.0f, tan1, 0.0f ), c );
	pln[1].MakePlane( GVector3f( 1.0f, tan2, 0.0f ), d );
	pln[2].MakePlane( GVector3f( 0.0f, 1.0f, 0.0f ), b );	

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln[0].IsAbovePlane(v0) || pln[0].IsAbovePlane(v1) || pln[0].IsAbovePlane(v2) )
			continue;
		if ( pln[1].IsAbovePlane(v0) || pln[1].IsAbovePlane(v1) || pln[1].IsAbovePlane(v2) )
			continue;
		if ( pln[2].IsAbovePlane(v0) && pln[2].IsAbovePlane(v1) && pln[2].IsAbovePlane(v2) )
			continue;
		if ( pln[2].IsBelowPlane(v0) && pln[2].IsBelowPlane(v1) && pln[2].IsBelowPlane(v2) )
			continue;
		if ( pln[2].GetSignedDistance( v0 ) * pln[2].GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln[2].GetSignedDistance( v1 ) * pln[2].GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln[2].GetSignedDistance( v2 ) * pln[2].GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	(*CvxPt0) = cvx.m_HullPtList;
	
	float minx = 100000.0f;
	float maxx = -100000.0f;
	float minz = 100000.0f;
	float maxz = -100000.0f;
	GObNode<GVector3f> *pNode = CrossPtList.m_pLeftEnd;
	while( pNode )
	{
		GVector3f pt = pNode->data;
		minx = MIN( minx, pt[0] );
		maxx = MAX( maxx, pt[0] );
		if(pt.X<LandMark[25].X) {
			minz = MIN( minz, pt[2] );
			maxz = MAX( maxz, pt[2] );
		}
		pNode = pNode->m_pRight;
	}

	GVector3f pt0( minx, b.Y, 0.0f );
	GVector3f pt1( maxx, b.Y, 0.0f );
	(*CvxPt1).Insert( pt0 );
	(*CvxPt1).Insert( pt1 );

	pt0.setVector( 0.0f, b.Y, minz );
	pt1.setVector( 0.0f, b.Y, maxz );
	(*CvxPt2).Insert( pt0 );
	(*CvxPt2).Insert( pt1 );

	Result[32] = cvx.GetHullLength();
	Result[76] = fabs( maxx - minx );
	Result[85] = fabs( maxz - minz );

}

/*********************************************************
 * 측정항목 33: 넙다리둘레
 * 필요한 측정점(인덱스) :  볼기 고랑점(26),  
 *							오른쪽 겨드랑점(9), 
 *							왼쪽 겨드랑점(10), 
 *							샅점(25)
 * 설명 : 볼기 고랑점 높이의 수평 둘레
 *********************************************************/
void GetResultPose2_33( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	int i;
	GLandMark gluteal, c, d, e;
	GVector3f n( 0.0f, 1.0f, 0.0 );
	GObList<GVector3f> CrossPtList;
	gluteal = LandMark[26];
	c = LandMark[9];
	d = LandMark[10];
	e = LandMark[25];
	CHECK(gluteal);
	CHECK(c);
	CHECK(d);
	CHECK(e);
	// plane setting.
	double tan1 = tan( M_PI / 2.0 - RefAngle[3]);
	double tan2 = tan( M_PI / 2.0 + RefAngle[4]);

	GPlane pln[2], cutPln;
	pln[0].MakePlane( GVector3f( -1.0f, tan1, 0.0f ), c );
	pln[1].MakePlane( GVector3f( 1.0f, tan2, 0.0f ), d );
	cutPln.MakePlane( GVector3f( 0.0f, 1.0f, 0.0f ), gluteal );

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln[0].IsAbovePlane(v0) || pln[0].IsAbovePlane(v1) || pln[0].IsAbovePlane(v2) )
			continue;
		if ( pln[1].IsAbovePlane(v0) || pln[1].IsAbovePlane(v1) || pln[1].IsAbovePlane(v2) )
			continue;
		if ( cutPln.IsAbovePlane(v0) && cutPln.IsAbovePlane(v1) && cutPln.IsAbovePlane(v2) )
			continue;
		if ( cutPln.IsBelowPlane(v0) && cutPln.IsBelowPlane(v1) && cutPln.IsBelowPlane(v2) )
			continue;
		if ( v0.X > e.X || v1.X > e.X || v2.X > e.X )
			continue;
		if ( cutPln.GetSignedDistance( v0 ) * cutPln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v1 ) * cutPln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v2 ) * cutPln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	(*CvxPt) = cvx.m_HullPtList;

	float val = cvx.GetHullLength();
	Result[33] = val;

}


/*************************************************************
 * 측정항목 34: 무릎둘레
 * 필요한 측정점(인덱스) : 오른쪽 무릎뼈 가운데점(27), 샅점(25)
 * 설명 : 오른쪽 무릎뼈 가운데점에서의 수평 둘레
 *************************************************************/
void GetResultPose2_34( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	int i;
	GLandMark b, c;
	GVector3f n( 0.0f, 1.0f, 0.0 );
	GObList<GVector3f> CrossPtList;
	b = LandMark[27];
	c = LandMark[25];
	CHECK(b);
	
	
	GPlane pln;
	pln.MakePlane( GVector3f( 0.0f, 1.0f, 0.0f ), b );

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
			continue;
		if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
			continue;
		if ( v0.X > c.X || v1.X > c.X || v2.X > c.X )
			continue;
		if ( pln.GetSignedDistance( v0 ) * pln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v1 ) * pln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v2 ) * pln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;
	Result[34] = val;
}


// 측정항목 35: 장딴지둘레
// 필요한 측정점(인덱스) : 오른쪽 장딴지 돌출점(28), 샅점(25)
// 설명 : 오른쪽 장딴지 돌출점을 지나는 수평 둘레
void GetResultPose2_35( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	int i;
	GLandMark b, c;
	GVector3f n( 0.0f, 1.0f, 0.0 );
	GObList<GVector3f> CrossPtList;
	b = LandMark[28];
	c = LandMark[25];
	CHECK(b);
	CHECK(c);
	
	
	GPlane pln;
	pln.MakePlane( GVector3f( 0.0f, 1.0f, 0.0f ), b );

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
			continue;
		if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
			continue;
		if ( v0.X > c.X || v1.X > c.X || v2.X > c.X )
			continue;
		if ( pln.GetSignedDistance( v0 ) * pln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v1 ) * pln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v2 ) * pln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;
	Result[35] = val;
}

// 측정항목 36 : 종아리최소둘레
// 필요한 측정점(인덱스) : 오른쪽 장딴지 앞점(29), 샅점(25)
// 설명 : 오른쪽 장딴지 앞점을 지나는 수평 둘레
void GetResultPose2_36( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	int i;
	GLandMark b, c;
	GVector3f n( 0.0f, 1.0f, 0.0 );
	GObList<GVector3f> CrossPtList;
	b = LandMark[29];
	c = LandMark[25];
	CHECK(b);
	CHECK(c);
	
	GPlane pln;
	pln.MakePlane( GVector3f( 0.0f, 1.0f, 0.0f ), b );

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
			continue;
		if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
			continue;
		if ( v0.X > c.X || v1.X > c.X || v2.X > c.X )
			continue;
		if ( pln.GetSignedDistance( v0 ) * pln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v1 ) * pln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v2 ) * pln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;
	Result[36] = val;
}

// 측정항목 37 : 발목최대둘레
// 필요한 측정점(인덱스) : 가쪽복사점(안쪽), 가쪽복사점(43), 샅점(18)
// 설명 : 안쪽복사점과 가쪽복사점을 지나는 둘레

void GetResultPose2_37( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	int i;
	GLandMark ankle, ankleIn;
	GVector3f n( 0.0f, 1.0f, 0.0 );
	GObList<GVector3f> CrossPtList;
	ankle = LandMark[30];
	ankleIn = LandMark[43];

	CHECK(ankle);
	CHECK(ankleIn);	
	
	GVector3f tmp = ankle;
	tmp.Z += 100.0f;
	GPlane pln;
	pln.MakePlane( ankle, tmp, ankleIn );

	float limitX = ankleIn.X;

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
			continue;
		if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
			continue;
		if ( v0.X >ankleIn.X && v1.X >ankleIn.X && v2.X >ankleIn.X )
			continue;
		if ( pln.GetSignedDistance( v0 ) * pln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v1 ) * pln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v2 ) * pln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;
	Result[37] = val;
}


/****************************************************************
 * 측정항목 38 : 편위팔둘레
 * 필요한 측정점(인덱스) :  오른쪽 겨드랑점(9), 
 *							오른쪽어깨끝점(10), 
 *							오른쪽손목안쪽점(33)
 * 설명 : 오른쪽 겨드랑점을 지나고 팔축에 수직인 수평 둘레
 ****************************************************************/
void GetResultPose2_38( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();

	int i;
	GLandMark armpit, b, c;
	GObList<GVector3f> CrossPtList;
	armpit = LandMark[9];
	b = LandMarkHelp[26]; //12
	c = LandMarkHelp[27];  //32
	GLandMark aFR = LandMark[44];
	GLandMark aBR = LandMark[45];
	CHECK(armpit);
	CHECK(b);
	CHECK(c);
	c.X+=40.0f;
	GVector3f n = armpit - c; n.Z = 0.0f;
	n.normalize();

	GPlane pln;
	pln.MakePlane( n, armpit );

	GVector3f normR= aFR-aBR;
	normR.Y = 0.0f;
	
	float tmp;
	tmp=normR.X;
	normR.X = -normR.Z;
	normR.Z=tmp;

    armpit.X +=1.0f;  	
	GPlane refPln0( normR, armpit ); 


	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
			continue;
		if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
			continue;

		if ( refPln0.IsBelowPlane(v0) || refPln0.IsBelowPlane(v1) || refPln0.IsBelowPlane(v2) )
			continue;
		if ( pln.GetSignedDistance( v0 ) * pln.GetSignedDistance( v1 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v1 ) * pln.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v2 ) * pln.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;
	Result[38] = val;
}

/*****************************************************************************
 * 측정항목    39: 팔꿈치 둘레
 * 측정항목    64: 팔안쪽길이.
 * 필요한 측정점(인덱스) :  오른쪽팔꿈치가운데점(42),  a
 *							오른쪽어깨가쪽점(7),    b
 *						    오른쪽손목가쪽점(33),   c
 *							오른쪽겨드랑점(9)       d
 *							오른쪽겨드랑뒤접힘점(45),  e 
 *							오른쪽겨드랑앞점힘점(44),  f
 *							오른쪽손목안쪽점(32),
 * 팔꿈치둘레 : 오른쪽 팔꿈치 가운데점을 지나고 팔축에 수직인 수평 둘레
 * 팔안쪽둘레 : 오른쪽겨드랑점과 오른쪽손목안쪽점을 지나는 수평면의 체표길이.
 ****************************************************************************/
void GetResultPose2_39_64( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1 )
{
	(*CvxPt0).DeleteAll();
	(*CvxPt1).DeleteAll();
	int i;
	GLandMark elbow, shp, wristIn, armpit, foldB, foldF, g;
	GObList<GVector3f> CrossPtList0;
	GObList<GVector3f> CrossPtList1;
	elbow = LandMark[42];
	shp = LandMark[7];
	wristIn = LandMark[32];  
	armpit = LandMark[9];
	foldF = LandMark[44];
	foldB = LandMark[45];	
	g = LandMark[32];
	CHECK(elbow);
	CHECK(shp);
	CHECK(wristIn);
	CHECK(armpit);
	CHECK(foldB);
	CHECK(foldF);
	CHECK(g);

	GPlane elbowPln, pln1,pln2, cutPln, refPlnElbow;

	armpit.Z = (foldF.Z+foldB.Z)*0.5f;
	GVector3f n = armpit - wristIn;     // 겨드랑점에서 손목 안쪽점까지를 normal로 잡는다.
	n.Z = 0.0f; n.normalize();

	
		
	elbowPln.MakePlane( n, elbow );                                // 팔꿈치 둘레 자를 평면
	GVector3f tmp = wristIn; 
	tmp.Z +=500.0f;
	pln2.MakePlane( wristIn, armpit,tmp); 

	double tan1 = tan( M_PI / 2.0 - RefAngle[5]);
	pln1.MakePlane( GVector3f ( -1.0f, tan1, 0.0f ), armpit );    // 팔을 몸통과 분리할 평면  LandMarkHelp[42]
	
	tmp = wristIn;
	tmp.X -= 500.0f;
	tmp.Z += 300.0f;
	cutPln.MakePlane( wristIn, tmp, armpit );					       

 // 팔안쪽길이 자를 평면 

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if( v0.Y>armpit.Y || v1.Y>armpit.Y || v2.Y>armpit.Y )
			continue;
		if( v0.Y<wristIn.Y || v1.Y<wristIn.Y || v2.Y<wristIn.Y )
			continue;

		if ( cutPln.IsAbovePlane(v0) && cutPln.IsAbovePlane(v1) && cutPln.IsAbovePlane(v2) )
			continue;
		if ( cutPln.IsBelowPlane(v0) && cutPln.IsBelowPlane(v1) && cutPln.IsBelowPlane(v2) )
			continue;
		if ( pln1.IsBelowPlane(v0) && pln1.IsBelowPlane(v1) && pln1.IsBelowPlane(v2) )
			continue;
		if ( pln2.IsBelowPlane(v0) && pln2.IsBelowPlane(v1) && pln2.IsBelowPlane(v2) )
			continue;
	
		if ( cutPln.GetSignedDistance( v0 ) * cutPln.GetSignedDistance( v1 ) < 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList0.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v1 ) * cutPln.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList0.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v2 ) * cutPln.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList0.Insert( CrossPt );
		}
	}


	//팔꿈치둘레
	tan1 = tan( M_PI / 2.0 - RefAngle[3]);
	refPlnElbow.MakePlane( GVector3f ( -1.0f, tan1, 0.0f ), LandMarkHelp[42] );    // 팔을 몸통과 분리할 평면  

	for ( i = 0; i < NumIdx; i+=3 ) 
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( elbowPln.IsAbovePlane(v0) && elbowPln.IsAbovePlane(v1) && elbowPln.IsAbovePlane(v2) )
			continue;
		if ( elbowPln.IsBelowPlane(v0) && elbowPln.IsBelowPlane(v1) && elbowPln.IsBelowPlane(v2) )
			continue;
		if ( refPlnElbow.IsBelowPlane(v0) || refPlnElbow.IsBelowPlane(v1) || refPlnElbow.IsBelowPlane(v2) )
			continue;
	
		if ( elbowPln.GetSignedDistance( v0 ) * elbowPln.GetSignedDistance( v1 ) < 0 )
		{
			GVector3f CrossPt = elbowPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList1.Insert( CrossPt );
		}
		if ( elbowPln.GetSignedDistance( v1 ) * elbowPln.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = elbowPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList1.Insert( CrossPt );
		}
		if ( elbowPln.GetSignedDistance( v2 ) * elbowPln.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = elbowPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList1.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList1 );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	float val = cvx.GetHullLength();
	(*CvxPt0) = cvx.m_HullPtList;
	Result[39] = val;


	//팔안쪽길이
	CConvexHull cvx1( CrossPtList0 );
	cvx1.m_PtList.Insert(wristIn);
	cvx1.m_PtList.Insert(tmp);
	cvx1.MakeConvexHullXY();
	cvx1.m_HullPtList.Delete(tmp);

	cvx1.m_HullPtList.Insert(armpit);
	cvx1.ArrangeHullPtY();
	val = cvx1.GetHullLength2();
	(*CvxPt1) = cvx1.m_HullPtList;
	Result[64] = val;


	/*GVector3f midPt1;  //팔꿈치쪽 맨 아래점
	midPt1.X = -10000.0f;
	pNode = CrossPtList1.m_pLeftEnd;
	while( pNode )
	{
		GVector3f pt;
		pt = pNode->data;
		if ( pt.X > midPt1.X )
			midPt1 = pt;
		pNode = pNode->m_pRight;
	}
	
	//팔안쪽길이
	GVector3f pt0, pt1, pt2, pt3;
	pt0 = armpit;								//앞뒤겨드랑접힘점의 중간 위치 점
	pt0.Z = ( flodB.Z + foldF.Z ) * 0.5f;       //높이는 겨드랑점과 같게 해주고...

	//GPlane tmpPln;
	//tmpPln.MakePlane( shp, flodB, foldF );
	//pt0 = tmpPln.GetProjectionPt( pt0, GVector3f( 1.0f, 0.0f, 0.0f ) );
	pt2 = midPt1;
	pt1 = midPt0;
	pt1.Z = pt0.Z * 0.5f + pt2.Z * 0.5f;
	pt3 = wristIn;
	(*CvxPt1).Insert( pt0 );
	(*CvxPt1).Insert( pt1 );
	(*CvxPt1).Insert( pt2 );
	(*CvxPt1).Insert( pt3 );
	Result[64] = (pt0-pt1).getNorm() + (pt1-pt2).getNorm() + (pt2-pt3).getNorm();*/
	
}

// 측정항목     40: 손목둘레
// 필요한 측정점(인덱스) : 오른쪽 손목안쪽점(32), 손목가쪽점(33), 오른쪽 겨드랑이점(9), 오른쪽 어깨끝점(7)
void GetResultPose2_40( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	int i;
	GLandMark a, b, c, d, e, underCP, upCP;
	GObList<GVector3f> CrossPtList;
	a = LandMark[32]; 
	b = LandMark[9]; 
	c = LandMark[7]; 
	d = LandMark[33]; 
	e = LandMark[48]; 

	underCP = LandMarkHelp[27] ;
	upCP = LandMarkHelp[26];
	//upCP.Z -= 30.0f;
	float Armslop;
	if ( upCP.X == underCP.X )
            Armslop= 10000.0f;
    else
            Armslop=(upCP.Y-underCP.Y)/(upCP.X-underCP.X);

	CHECK(a);
	CHECK(b);
	CHECK(c);
	CHECK(d);

	double tan1 = tan( M_PI / 2.0 - RefAngle[3]);

	//GVector3f norm = GVector3f(1.0f, Armslop+0.2f, 0.0f );

	GVector3f norm = LandMark[42]-LandMark[32];
					
	//a.Y+=5.0f;
	GVector3f midInOutPt=  (a+d)*0.5f;
	GPlane cutPln, pln1;
	cutPln.MakePlane( norm, midInOutPt );
	//cutPln.MakePlane( a, d, e );
	pln1.MakePlane( GVector3f ( -1.0f, tan1, 0.0f ), b );

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( cutPln.IsAbovePlane(v0) && cutPln.IsAbovePlane(v1) && cutPln.IsAbovePlane(v2) )
			continue;
		if ( cutPln.IsBelowPlane(v0) && cutPln.IsBelowPlane(v1) && cutPln.IsBelowPlane(v2) )
			continue;
		if ( pln1.IsBelowPlane(v0) && pln1.IsBelowPlane(v1) && pln1.IsBelowPlane(v2) )
			continue;
	
		if ( cutPln.GetSignedDistance( v0 ) * cutPln.GetSignedDistance( v1 ) < 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v1 ) * cutPln.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v2 ) * cutPln.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;
	Result[40] = val;
}


// 측정항목    41: 몸통세로둘레.
// 필요한 측정점(인덱스) : 어깨중앙점(36), 샅점(25)
// 설명: 어깨중앙점과 샅점을 지나는 수직면에 포함된 점들의 컨백스한 둘레.
void GetResultPose2_41( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GObList<GVector3f> CrossPtList;
	GLandMark a = LandMark[36];
	GLandMark b = LandMark[25];
	CHECK( a );
	CHECK( b );
	CrossPtList.Insert( a );
	CrossPtList.Insert( b );
	
	// make plane.
	GVector3f tmp = (a+b)/2.0f; tmp.Z += 100.0f;
	GPlane pln0;
	pln0.MakePlane( a, b, tmp );

	// collect intersection points.
	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln0.IsAbovePlane(v0) && pln0.IsAbovePlane(v1) && pln0.IsAbovePlane(v2) )
			continue;
		if ( pln0.IsBelowPlane(v0) && pln0.IsBelowPlane(v1) && pln0.IsBelowPlane(v2) )
			continue;

		// extra constraints.
		if ( v0.Y > a.Y || v1.Y > a.Y || v2.Y > a.Y )
			continue;
		if ( v0.Y < b.Y || v1.Y < b.Y || v2.Y < b.Y )
			continue;
				
		if ( pln0.GetSignedDistance( v0 ) * pln0.GetSignedDistance( v1 ) < 0 )
		{
			GVector3f CrossPt = pln0.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln0.GetSignedDistance( v1 ) * pln0.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = pln0.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln0.GetSignedDistance( v2 ) * pln0.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = pln0.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullYZ();
	cvx.ArrangeHullPtYZ();
	(*CvxPt) = cvx.m_HullPtList;
	Result[41] = cvx.GetHullLength();
}

/**********************************************************************
 * 측정항목    42: 앞중심길이.
 * 필요한 측정점(인덱스) : 목앞점(3), 허리앞점(21), 오른쪽젖꼭지점(16)
 * 설명: 목앞점과 허리앞점을 잇는 켄백스한 체표길이.
 **********************************************************************/
void GetResultPose2_42( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GObList<GVector3f> CrossPtList;
	GLandMark a = LandMark[3]; 
	GLandMark b = LandMark[21];
	GLandMark c = LandMark[16];
	CHECK(a);
	CHECK(b);
	CHECK(c);
	
	GVector3f tmp;
	GPlane CutPln;	// 목앞점과 허리앞점을 잇는 평면.
	GPlane RefPln;	// 참조평면.
	
	tmp = (a+b)/2.0f; tmp.Z += 100.0f;
	CutPln.MakePlane( a, b, tmp );

	tmp = (a+b)/2.0f; tmp.X += 100.0f;
	RefPln.MakePlane( a, b, tmp );
	
	float limitZ = a.Z-30.0f;

	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( CutPln.IsAbovePlane(v0) && CutPln.IsAbovePlane(v1) && CutPln.IsAbovePlane(v2) )
			continue;
		if ( CutPln.IsBelowPlane(v0) && CutPln.IsBelowPlane(v1) && CutPln.IsBelowPlane(v2) )
			continue;

		// extra constraints.
		if ( v0.Y > a.Y || v1.Y > a.Y || v2.Y > a.Y )
			continue;
		if ( v0.Y < b.Y || v1.Y < b.Y || v2.Y < b.Y )
			continue;
		if ( v0.Z < limitZ || v1.Z < limitZ || v2.Z < limitZ )
			continue;
		if ( CutPln.GetSignedDistance( v0 ) * CutPln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = CutPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( CutPln.GetSignedDistance( v1 ) * CutPln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = CutPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( CutPln.GetSignedDistance( v2 ) * CutPln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = CutPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}

	//CrossPtList.Insert( a );
	//CrossPtList.Insert( b );

	CConvexHull cvx ( CrossPtList );
	
	cvx.MakeConvexHullYZ();
	CrossPtList.DeleteAll();
	CrossPtList = cvx.m_HullPtList;
	cvx.m_HullPtList.DeleteAll();
	
	GObNode<GVector3f> *pNode = CrossPtList.m_pLeftEnd;
	while( pNode )
	{
		GVector3f pt = pNode->data;
		if ( RefPln.IsAbovePlane( pt ) )
			cvx.m_HullPtList.Insert( pt );
		pNode = pNode->m_pRight;
	}
	cvx.m_HullPtList.Insert( a );
	cvx.m_HullPtList.Insert( b );
	cvx.ArrangeHullPtY();
	(*CvxPt) = cvx.m_HullPtList;
	Result[42] = cvx.GetHullLength2();
}

// 측정항목  49: 목옆젖꼭지 길이.
// 필요한 측정점(인덱스) : 오른쪽목옆점(4), 오른쪽젖꼭지점(16)  /보조점: 목둘레 최우점(51)
// 설명: 오른쪽목옆점과 오른쪽젖꼭지점을 지나는 수직면과 만나는 점들의 컨백스한 길이.
void GetResultPose2_49( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GObList<GVector3f> CrossPtList;
	GLandMark sneckR = LandMark[4];
	GLandMark BPR = LandMark[16];
	GLandMark c = LandMarkHelp[51];
	CHECK(sneckR);
	CHECK(BPR);
	CHECK(c);

	CrossPtList.Insert( sneckR );
	CrossPtList.Insert( BPR );


	// make plane.
	GVector3f tmp = (sneckR+BPR)/2.0f;  tmp.Z -= 300.0f;  //tmp.X -= 20.0f;
	GPlane cutPln;
	float diffX = c.X-sneckR.X;
	float diffY = c.Y-sneckR.Y;
	float ratioXY= diffX/diffY;
    if(ratioXY<0) ratioXY =-ratioXY;

	if ( diffX >= -0.1f || diffY>-0.1f || ratioXY>1.0f) 
		cutPln.MakePlane( sneckR, BPR, tmp ); 
    else 
		cutPln.MakePlane( sneckR, BPR, c );


	GVector3f tmp1 = (sneckR+BPR)/2.0f; tmp1.X += 300.0f;
	GPlane pln1( sneckR, BPR, tmp1 );

	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		// extra constraints.
		if ( v0.Y > sneckR.Y || v1.Y > sneckR.Y || v2.Y > sneckR.Y )
			continue;
		if ( v0.X > sneckR.X || v1.X > sneckR.X || v2.X > sneckR.X )
			continue;
		if ( v0.Z < sneckR.Z || v1.Z < sneckR.Z || v2.Z < sneckR.Z )
			continue;
		if ( v0.Y < BPR.Y	 || v1.Y < BPR.Y	|| v2.Y < BPR.Y )
			continue;

	//	if ( pln1.IsBelowPlane(v0) && pln1.IsBelowPlane(v1) && pln1.IsBelowPlane(v2) )
	//		continue;

		if ( cutPln.IsAbovePlane(v0) && cutPln.IsAbovePlane(v1) && cutPln.IsAbovePlane(v2) )
			continue;
		if ( cutPln.IsBelowPlane(v0) && cutPln.IsBelowPlane(v1) && cutPln.IsBelowPlane(v2) )
			continue;

		if ( cutPln.GetSignedDistance( v0 ) * cutPln.GetSignedDistance( v1 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v1 ) * cutPln.GetSignedDistance( v2 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( cutPln.GetSignedDistance( v2 ) * cutPln.GetSignedDistance( v0 ) <= 0 )
		{
			GVector3f CrossPt = cutPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	tmp.Z = BPR.Z-300.0f;
	CrossPtList.Insert(tmp);//convexHull을 잘만들기 위해 추가했다가 뺄 것

	CConvexHull cvx( CrossPtList );
	
	cvx.MakeConvexHullYZ();

	//cvx.m_HullPtList = cvx.m_PtList;
	cvx.m_HullPtList.Delete(tmp);

	cvx.ArrangeHullPtY();

	(*CvxPt) = cvx.m_HullPtList;
	Result[49] = cvx.GetHullLength2();
}

// 측정항목    50: 목옆-젖꼭지 허리둘레선길이(=앞길이).
// 필요한 측정점(인덱스) : 오른쪽목옆점(4), 오른쪽젖꼭지점(16), 오른쪽허리옆점(19)
// 설명: 
void GetResultPose2_50( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0 )
{
	(*CvxPt0).DeleteAll();


	GObList<GVector3f> CrossPtList;
	GLandMark a = LandMark[4];
	GLandMark b = LandMark[16];
	GLandMark c = LandMark[19];
	CHECK(a);
	CHECK(b);
	CHECK(c);
	CrossPtList.Insert(b);
	
	// make plane
	GPlane pln;
	pln.MakePlane( GVector3f( 1.0f, 0.0f, 0.1f ), b );

	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		// extra constraints.
		if ( v0.Y > b.Y && v1.Y > b.Y && v2.Y > b.Y )
			continue;
		if ( v0.Y < c.Y && v1.Y < c.Y && v2.Y < c.Y )
			continue;
		if ( v0.Z < c.Z && v1.Z < c.Z && v2.Z < c.Z )
			continue;
		
		if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
			continue;
		if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
			continue;

		if ( pln.GetSignedDistance( v0 ) * pln.GetSignedDistance( v1 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v1 ) * pln.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v2 ) * pln.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	::getSortedPoint( CrossPtList, 1, 0 );
	GVector3f d = CrossPtList.m_pLeftEnd->data;
	(*CvxPt0).Insert( b );
	(*CvxPt0).Insert( d );
	GVector3f dis = b-d;
	Result[50] = Result[49] + dis.getNorm();
	Result[51] = dis.getNorm();
}


// 측정항목   55: 목옆뒤허리둘레선길이(뒤길이)
// 필요한 측정점(인덱스) : 목옆점(4), 허리뒤점(22)
// 설명: 목옆점과 허리뒤점을 이은 수평면과 만나는 점들의 컨백스한 길이.
void GetResultPose2_55( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GObList<GVector3f> CrossPtList;
	GLandMark a = LandMark[4];  // 목옆점
	GLandMark b = LandMark[22]; // 허리뒤점.
	CHECK(a); 
	CHECK(b); 
	CrossPtList.Insert( a );
	GVector3f norm = GVector3f(1.0f, 0.0f, 0.0f);

	GPlane plnCut;
	plnCut.MakePlane( norm, a );

//	GVector3f tmp1 = (a+b)/2.0f; tmp1.X += 100.0f;  //앞부분은 제외하기 위한 평면
//	GPlane pln1;
//	pln1.MakePlane( a, b, tmp1 );
	
	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( plnCut.IsAbovePlane(v0) && plnCut.IsAbovePlane(v1) && plnCut.IsAbovePlane(v2) )
			continue;
		if ( plnCut.IsBelowPlane(v0) && plnCut.IsBelowPlane(v1) && plnCut.IsBelowPlane(v2) )
			continue;

		// extra constraints.
		if ( v0.Y > a.Y || v1.Y > a.Y || v2.Y > a.Y )
			continue;
		if ( v0.Y < b.Y || v1.Y < b.Y || v2.Y < b.Y )
			continue;
		if ( v0.Z > a.Z || v1.Z > a.Z || v2.Z > a.Z )
		//if ( pln1.IsAbovePlane(v0) && pln1.IsAbovePlane(v1) && pln1.IsAbovePlane(v2) )
			continue;
			
		if ( plnCut.GetSignedDistance( v0 ) * plnCut.GetSignedDistance( v1 ) < 0.0f )
		{
			GVector3f CrossPt = plnCut.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( plnCut.GetSignedDistance( v1 ) * plnCut.GetSignedDistance( v2 ) < 0.0f )
		{
			GVector3f CrossPt = plnCut.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( plnCut.GetSignedDistance( v2 ) * plnCut.GetSignedDistance( v0 ) < 0.0f )
		{
			GVector3f CrossPt = plnCut.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );

	GVector3f tmp = a + b;
	tmp.Z +=100.0f;

	cvx.m_HullPtList.Insert(tmp);
	cvx.MakeConvexHullYZ();

	cvx.m_HullPtList.Delete(tmp);

	cvx.ArrangeHullPtY();

	(*CvxPt) = cvx.m_HullPtList;
	Result[55] = cvx.GetHullLength2();
}

// 측정항목   56: 등길이.
// 필요한 측정점(인덱스) : 목뒤점(6), 허리뒤점(21)
// 설명: 목뒤점과 허리뒤점을 이은 수평면과 만나는 점들의 컨백스한 길이.
void GetResultPose2_56( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GObList<GVector3f> CrossPtList;
	GLandMark a = LandMark[6]; // 목뒤점
	GLandMark b = LandMark[22]; // 허리뒤점.
	CHECK(a); 
	CHECK(b); 
	CrossPtList.Insert( a );
	CrossPtList.Insert( b );
	GVector3f tmp = (a+b)/2.0f; tmp.Z += 100.0f;

	GPlane pln;
	pln.MakePlane( a, b, tmp );

	GVector3f tmp1 = (a+b)/2.0f; tmp1.X += 100.0f;
	GPlane pln1;
	pln1.MakePlane( a, b, tmp1 );
	
	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
			continue;
		if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
			continue;

		// extra constraints.
		if ( v0.Y > a.Y || v1.Y > a.Y || v2.Y > a.Y )
			continue;
		if ( v0.Y < b.Y || v1.Y < b.Y || v2.Y < b.Y )
			continue;
		if ( pln1.IsAbovePlane(v0) && pln1.IsAbovePlane(v1) && pln1.IsAbovePlane(v2) )
			continue;
			
		if ( pln.GetSignedDistance( v0 ) * pln.GetSignedDistance( v1 ) < 0.0f )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v1 ) * pln.GetSignedDistance( v2 ) < 0.0f )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v2 ) * pln.GetSignedDistance( v0 ) < 0.0f )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );

	cvx.m_HullPtList.Insert(tmp);
	cvx.MakeConvexHullYZ();

	cvx.m_HullPtList.Delete(tmp);

	cvx.ArrangeHullPtY();


	(*CvxPt) = cvx.m_HullPtList;
	Result[56] = cvx.GetHullLength2();
}


// 측정항목    54: 진동깊이(목뒤등뼈겨드랑수준길이)
// 필요한 측정점(인덱스) : 목뒤점(6), 등뼈위겨드랑수준점(11)
// 설명: 
void GetResultPose2_54( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GObList<GVector3f> CrossPtList;
	GLandMark a = LandMark[6];
	GLandMark b = LandMark[11];
	CHECK(a);
	CHECK(b);
	CrossPtList.Insert(a);

	// make plane.
	GPlane pln;
	GVector3f tmp = (a+b)/2.0f; tmp.Z += 100.0f;
	pln.MakePlane( a, tmp, b );

	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		// extra constraints.
		if ( v0.Y > a.Y || v1.Y > a.Y || v2.Y > a.Y )
			continue;
		if ( v0.Z > a.Z || v1.Z > a.Z || v2.Z > a.Z )
			continue;
		if ( v0.Y < b.Y || v1.Y < b.Y || v2.Y < b.Y )
			continue;
			
		if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
			continue;
		if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
			continue;
		
		if ( pln.GetSignedDistance( v0 ) * pln.GetSignedDistance( v1 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v1 ) * pln.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v2 ) * pln.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullYZ();

	cvx.m_HullPtList.Insert(b);
	cvx.ArrangeHullPtY();
	(*CvxPt) = cvx.m_HullPtList;
	Result[54] = cvx.GetHullLength2();
}


// 측정항목 44: 어깨길이
// 필요한 측정점(인덱스) : 목옆점-오른쪽(4), 어깨끝점-오른쪽 (7)
// 설명 : 
void GetResultPose2_44( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	int i;
	GLandMark a, b;
	GVector3f tmp, tmp1;
	GObList<GVector3f> CrossPtList;
	a = LandMark[4];
	b = LandMark[7];
	CHECK(a);
	CHECK(b);
	tmp = GVector3f(a.X, a.Y-100.0f, a.Z);
	tmp1 = (a+b)/2.0f;
	tmp1.Z += 100.0f;
	GPlane pln( a, b, tmp );
	GPlane pln1( a, b, tmp1 );   // 아래점 제거하는 평면
    
	CrossPtList.Insert(a);
	CrossPtList.Insert(b);

	//1. 목옆점과 어깨점, 어깨점에서 아래로 100mm 내려간점을 포함하는 평면에 포함되는 점들을 모은다.
	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
			continue;
		if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
			continue;
		if ( pln1.IsBelowPlane(v0) || pln1.IsBelowPlane(v1) || pln1.IsBelowPlane(v2) )
			continue;
		if ( v0.Y > a.Y && v1.Y > a.Y && v2.Y > a.Y )
			continue;
		if ( v0.Y < b.Y && v1.Y < b.Y && v2.Y < b.Y )
			continue;
		if ( v0.X > a.X && v1.X > a.X && v2.X > a.X )
			continue;
		if ( v0.X < b.X && v1.X < b.X && v2.X < b.X )
			continue;
		
		//두점이 평면을 가운데 두고 양쪽에 있을때 - 두점을 이은선과 평면이 만나는 점을 포함시킴
		if ( pln.GetSignedDistance( v0 ) * pln.GetSignedDistance( v1 ) < 0 ) 
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v1 ) * pln.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v2 ) * pln.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}
	
	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXY();
	//cvx.ArrangeHullPtX();

	::getSortedPoint( cvx.m_HullPtList, 0, 0 );

	float val = ::getSortedPtLen( cvx.m_HullPtList );
	(*CvxPt) = cvx.m_HullPtList;
	Result[44] = val;

	//3. 길이를 구한다.
	//float val = cvx.GetHullLength2();
	//(*CvxPt) = cvx.m_HullPtList;
	//Result[7] = val;
}

// 측정항목 45 : 어깨가쪽사이길이 -45도 x ---> 목뒤어깨가쪽길이 로 바뀜
// 측정항목 46 : 어깨가쪽사이길이 -목뒤점 
// 측정항목 69 : 어깨 너비
// 필요한 측정점(인덱스) : 어깨끝점-오른쪽(7), 어깨끝점-왼쪽(8), 목뒤점(6)
// 설명 : 

void GetResultPose2_45_46_69( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0,  GObList<GVector3f> *CvxPt1,  GObList<GVector3f> *CvxPt2)
{
	(*CvxPt0).DeleteAll();
	(*CvxPt1).DeleteAll();
	(*CvxPt2).DeleteAll();

	int i;
	
	GLandMark shR, shL, BNeck;
	GVector3f tmp, tmpC, tmpFB, tmpH;
	GObList<GVector3f> CrossPtListH, CrossPtListB, CrossPtListC;
	shR = LandMark[7];
	shL = LandMark[8];
	BNeck = LandMark[6];
	CHECK(shR);
	CHECK(shL);
	CHECK(BNeck);
    
	// 목뒤점과 어깨끝점의 중간 높이의 체표점을 구한다.
	float centery = shR.Y * 0.5f + BNeck.Y * 0.5f;
	GPlane plnx; 
	plnx.MakePlane( GVector3f( 1.0f, 0.0f, 0.0f ), BNeck );

	float lx, rx, upy, downy;
	lx = BNeck.X+6.0;
	rx = BNeck.X-6.0;
	upy = centery+4.0;
	downy = centery;

	for ( i = 0; i < NumIdx; i++)
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];		
		if ( v0.X > lx )
			continue;
		if ( v0.X < rx )
			continue;
		if ( v0.Y > upy )
			continue;
		if ( v0.Y < downy )
			continue;
		if ( v0.Z > (BNeck.Z) )
			continue;
		
		tmp=v0;
	}

	GPlane  plnC( shR, shL, tmp );				// 3. 중간 측정경로 평면

	GPlane  plnB( shR, shL, BNeck );		    // 2. 목뒤점 지나는 평면
	
//	tmpH  = shR;  tmpH.Z += 200.0f;
//	GPlane  plnH ( shR, shL, tmpH );		    // 1. 수평 측정경로구하는 평면
   
	tmpFB = shR;  tmpFB.Y += 100.0f;	
	GPlane plnFB( shR, shL, tmpFB );			// 앞쪽점 제거를 위한 앞뒤 나누는 평면	

//	CrossPtListH.Insert(shR);
//	CrossPtListH.Insert(shL);

	CrossPtListB.Insert(shR);
	CrossPtListB.Insert(shL);

	CrossPtListC.Insert(shR);
	CrossPtListC.Insert(shL);

	float downH;
	if (shR.Y <shL.Y) downH= shR.Y;
	else  downH= shL.Y;

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		//if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
		//	continue;
		//if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
		//	continue;
		if ( plnFB.IsAbovePlane(v0) && plnFB.IsAbovePlane(v1) || plnFB.IsAbovePlane(v2) ) //  어깨끝점 앞쪽점 제외
			continue;
		if ( v0.Y > BNeck.Y && v1.Y > BNeck.Y && v2.Y > BNeck.Y ) //목뒤점 위에 제외 
			continue;
		if ( v0.Y < downH  &&  v1.Y < downH  &&  v2.Y < downH )
			continue;
		if ( v0.X > shL.X  &&  v1.X > shL.X  &&  v2.X > shL.X ) //왼 어깨끝점보다 작으면 제외
			continue;
		if ( v0.X < shR.X  &&  v1.X < shR.X  &&  v2.X < shR.X ) //오른어깨끝점보다 작으면 제외
			continue;

		
		// 목뒤점 평면 길이
		if ( plnB.GetSignedDistance( v0 ) * plnB.GetSignedDistance( v1 ) < 0 ) 
		{
			GVector3f CrossPt = plnB.GetProjectionPt( v0, v1-v0 );
			if ( CrossPt.X > shR.X  &&  CrossPt.X < shL.X ) 
			CrossPtListB.Insert( CrossPt );
		}
		if ( plnB.GetSignedDistance( v1 ) * plnB.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = plnB.GetProjectionPt( v1, v2-v1 );
			if ( CrossPt.X > shR.X  &&  CrossPt.X < shL.X ) 
			CrossPtListB.Insert( CrossPt );
		}
		if ( plnB.GetSignedDistance( v2 ) * plnB.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = plnB.GetProjectionPt( v0, v2-v0 );
			if ( CrossPt.X > shR.X  &&  CrossPt.X < shL.X ) 
			CrossPtListB.Insert( CrossPt );
		}

		// 목뒤점 중간 길이
		if ( plnC.GetSignedDistance( v0 ) * plnC.GetSignedDistance( v1 ) < 0 ) 
		{
			GVector3f CrossPt = plnC.GetProjectionPt( v0, v1-v0 );
				if ( CrossPt.X > shR.X  &&  CrossPt.X < shL.X ) 
			CrossPtListC.Insert( CrossPt );
		}
		if ( plnC.GetSignedDistance( v1 ) * plnC.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = plnC.GetProjectionPt( v1, v2-v1 );
				if ( CrossPt.X > shR.X  &&  CrossPt.X < shL.X ) 
			CrossPtListC.Insert( CrossPt );
		}
		if ( plnC.GetSignedDistance( v2 ) * plnC.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = plnC.GetProjectionPt( v0, v2-v0 );
				if ( CrossPt.X > shR.X  &&  CrossPt.X < shL.X ) 
			CrossPtListC.Insert( CrossPt );
		}

	}


	CConvexHull cvxB( CrossPtListB );
	cvxB.MakeConvexHullXZ();	
	::getSortedPoint( cvxB.m_HullPtList, 0, 0 );

	CConvexHull cvxC( CrossPtListC );
	cvxC.MakeConvexHullXZ();	
	::getSortedPoint( cvxC.m_HullPtList, 0, 0 );

	
	float val0 = ::getSortedPtLen( cvxB.m_HullPtList );
	float val1 = ::getSortedPtLen( cvxC.m_HullPtList );


	(*CvxPt1) = cvxB.m_HullPtList;
//	(*CvxPt0) = cvxC.m_HullPtList;

	
	(*CvxPt2).Insert(shR);
	(*CvxPt2).Insert(shL);

	Result[45] = val0*0.5;
	Result[46] = val0;
	Result[69] = shL.X-shR.X;
}



// 측정항목 47:  겨드랑앞접힘사이길이
// 필요한 측정점(인덱스) :  겨드랑앞접힘점-오른쪽(44), 겨드랑앞접힘점-왼쪽(46) //겨드랑앞벽점-오른쪽(12), 겨드랑앞벽점-왼쪽(13)
// 설명 : 
void GetResultPose2_47( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	int i;
	GLandMark a, b;
	GVector3f tmp, tmp1;
	GObList<GVector3f> CrossPtList;
	a = LandMark[44];
	b = LandMark[46];	
	CHECK(a);
	CHECK(b);

	tmp = (a + b)/2.0f; tmp.Z += 100.0f;
	tmp.Y =a.Y;
	tmp1 = a;  tmp1.Y = a.Y-100.0f;
	GPlane pln( a, b, tmp );	
	GPlane pln1( a, b, tmp1 ); // 양겨드랑앞벽점을 지나는 수직평면.
    //pln.d = 0.5f;

	CrossPtList.Insert(a);
	CrossPtList.Insert(b);

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
			continue;
		if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
			continue;
		if ( pln1.IsAbovePlane(v0) && pln1.IsAbovePlane(v1) && pln1.IsAbovePlane(v2) ) // ab를 잇는선 뒤에것을 건너뛰어야 한다.
			continue;
		if ( v0.X > b.X && v1.X > b.X && v2.X > b.X )
			continue;
		if ( v0.X < a.X && v1.X < a.X && v2.X < a.X )
			continue;
		
		//두점이 평면을 가운데 두고 양쪽에 있을때 - 두점을 이은선과 평면이 만나는 점을 포함시킴
		if ( pln.GetSignedDistance( v0 ) * pln.GetSignedDistance( v1 ) < 0 ) 
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v1 ) * pln.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v2 ) * pln.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}


	CConvexHull cvx( CrossPtList );
	
	cvx.MakeConvexHullXZ();

	cvx.m_HullPtList.Insert(a);
	cvx.m_HullPtList.Insert(b);

	::getSortedPoint( cvx.m_HullPtList, 0, 0 );

	float val = ::getSortedPtLen( cvx.m_HullPtList );
	(*CvxPt) = cvx.m_HullPtList;
	Result[47] = val;
}

// 측정항목 48 : 뒤품(겨드랑뒤접힘사이길이)
// 필요한 측정점(인덱스) : 겨드랑뒤접힘점-오른쪽(45), 겨드랑뒤접힘점-왼쪽(47)
// 설명 : 
void GetResultPose2_48( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	int i;
	GLandMark a, b, c;
	GVector3f tmp, tmp1;
	GObList<GVector3f> CrossPtList;
	a = LandMark[45];
	b = LandMark[47];	
	CHECK(a);
	CHECK(b);
	
	tmp = (a + b)/2.0f; tmp.Z += 100.0f;
	tmp.Y =a.Y;
	tmp1 = a;  tmp1.Y = a.Y+100.0f;

	GPlane pln( a, b, tmp );   // 수평면
	GPlane pln1( a, b, tmp1 ); // 양 겨드랑뒤벽점을 지나는 수직평면.
	
	CrossPtList.Insert(a);
	CrossPtList.Insert(b);

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
			continue;
		if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
			continue;
		if ( pln1.IsAbovePlane(v0) && pln1.IsAbovePlane(v1) && pln1.IsAbovePlane(v2) ) // ab를 잇는선 뒤에것을 건너뛰어야 한다.
			continue;
		if ( v0.X >b.X  &&  v1.X >b.X  &&  v2.X >b.X )
			continue;
		if ( v0.X <a.X  &&  v1.X <a.X  &&  v2.X <a.X )
			continue;

		//두점이 평면을 가운데 두고 양쪽에 있을때 - 두점을 이은선과 평면이 만나는 점을 포함시킴
		if ( pln.GetSignedDistance( v0 ) * pln.GetSignedDistance( v1 ) < 0 ) 
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v1 ) * pln.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v2 ) * pln.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}


	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
		::getSortedPoint( cvx.m_HullPtList, 0, 0 );

	float val = ::getSortedPtLen( cvx.m_HullPtList );
	(*CvxPt) = cvx.m_HullPtList;
	Result[48] = val;
}


// 측정항목    59: 밑위앞뒤 길이.
// 필요한 측정점(인덱스) : 허리앞점(20), 샅점(24), 허리뒤점(21)  /보조점: 샅앞점(16)
// 설명: 
void GetResultPose2_59( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GObList<GVector3f> CrossPtList;
	
	GLandMark a = LandMark[21];  // 허리앞점.
	GLandMark b = LandMark[25];  // 샅점
	GLandMark c = LandMark[22];  // 허리뒤점

	CHECK(a);
	CHECK(b);
	CHECK(c);
	CrossPtList.Insert( a );
	CrossPtList.Insert( b );
	CrossPtList.Insert( c );
	
	// make plane.- 앞허리 샅 평면	
	GPlane pln, pln2;
	pln.MakePlane( a, b, c );	

	// make plane. - 허리앞뒤점 지나는 수평평면
	GVector3f tmp = (a+c)/2.0f; tmp.X += 100.0f;
	pln2.MakePlane( a, c, tmp );

	// 앞쪽 샅길이 
	float ylimit = b.Y-2.0f;
	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		
		// extra constraints.
		if ( v0.Y < ylimit || v1.Y < ylimit || v2.Y < ylimit )
			continue;
		if ( pln2.IsBelowPlane(v0) && pln2.IsBelowPlane(v1) && pln2.IsBelowPlane(v2) )
			continue;
		if ( pln.IsAbovePlane(v0) && pln.IsAbovePlane(v1) && pln.IsAbovePlane(v2) )
			continue;
		if ( pln.IsBelowPlane(v0) && pln.IsBelowPlane(v1) && pln.IsBelowPlane(v2) )
			continue;
		
		if ( pln.GetSignedDistance( v0 ) * pln.GetSignedDistance( v1 ) < 0.0f )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v1 ) * pln.GetSignedDistance( v2 ) < 0.0f )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln.GetSignedDistance( v2 ) * pln.GetSignedDistance( v0 ) < 0.0f )
		{
			GVector3f CrossPt = pln.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}


	//GObList<GVector3f> CvxPt1;

	CConvexHull cvx0( CrossPtList );

	cvx0.MakeConvexHullYZ();
	cvx0.ArrangeHullPtYZ();

	Result[59] = ::getSortedPtLen( cvx0.m_HullPtList );

	(*CvxPt) = cvx0.m_HullPtList;
}

// 측정항목  61 : 기능적다리길이
// 설명: 허리옆점에서 바깥복사점까지의 컨벡스한 체표길이.

// 측정항목 _62 : 다리길이

// 필요한 측정점(인덱스) : 허리옆점(19), 바깥복사점(29)
// 설명: 허리옆점에서 바닥면까지의 컨백스한 체표길이.
void GetResultPose2_61_62( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1 )
{
	(*CvxPt0).DeleteAll();
	(*CvxPt1).DeleteAll();

	GLandMark a = LandMark[19]; // 허리옆점-r.
	GLandMark b = LandMark[30]; // 바깥복사점.
	GLandMark c = LandMarkHelp[42]; //LandMark[9];
	GVector3f tmp = (a+b)/2.0f; tmp.X += 100.0f;
	GVector3f tmp1 = (a+b)/2.0f; tmp1.Z += 100.0f;
	CHECK(a); 
	CHECK(b);
	GObList<GVector3f> CrossPtList; // 교차점을 저장하는 리스트.
	CrossPtList.Insert( a );
	CrossPtList.Insert( b );
	GPlane pln0, pln1, pln2; // 허리옆점과, 바깥복사점을 지나는 평면.
	pln0.MakePlane( a, b, tmp );
	pln1.MakePlane( a, b, tmp1 );	
	double tan1 = tan( M_PI / 2.0 - RefAngle[3]);
	pln2.MakePlane( GVector3f( -1.0f, tan1, 0.0f ), c );
	
	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		if ( pln0.IsAbovePlane(v0) && pln0.IsAbovePlane(v1) && pln0.IsAbovePlane(v2) )
			continue;
		if ( pln0.IsBelowPlane(v0) && pln0.IsBelowPlane(v1) && pln0.IsBelowPlane(v2) )
			continue;
		if ( pln1.IsBelowPlane(v0) && pln1.IsBelowPlane(v1) && pln1.IsBelowPlane(v2) )
			continue;
		if ( pln2.IsAbovePlane(v0) || pln2.IsAbovePlane(v1) || pln2.IsAbovePlane(v2) )
			continue;
		if ( v0.Y > a.Y || v1.Y > a.Y || v2.Y > a.Y )
			continue;
		if ( v0.Y < b.Y || v1.Y < b.Y || v2.Y < b.Y )
			continue;
		//  허리옆점 (2자세에선 바깥복사점)보다 1cm 들어간점들은 제외한다.
		//if ( v0.X > a.X+10.0F || v1.X > a.X+10.0F || v2.X > a.X+10.0F )
		//	continue;
		if ( pln0.GetSignedDistance( v0 ) * pln0.GetSignedDistance( v1 ) < 0 )
		{
			GVector3f CrossPt = pln0.GetProjectionPt( v0, v1-v0 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln0.GetSignedDistance( v1 ) * pln0.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = pln0.GetProjectionPt( v1, v2-v1 );
			CrossPtList.Insert( CrossPt );
		}
		if ( pln0.GetSignedDistance( v2 ) * pln0.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = pln0.GetProjectionPt( v0, v2-v0 );
			CrossPtList.Insert( CrossPt );
		}
	}

	CrossPtList.Insert(tmp);
	CConvexHull cvx( CrossPtList );
	CConvexHull cvx1( CrossPtList );
	
	cvx.MakeConvexHullXY();	
	cvx.m_HullPtList.Delete(tmp);

	cvx1.m_HullPtList = cvx.m_HullPtList;
	GVector3f bb=b;
	bb.Y=0.0f; bb.X -=10.0f;
	cvx1.m_HullPtList.Insert(bb);
	
	cvx.ArrangeHullPtY();

	cvx1.ArrangeHullPtY();

	CrossPtList.DeleteAll();
/*	GObNode<GVector3f> *pNode = cvx.m_HullPtList.m_pLeftEnd;
	while ( pNode )
	{
		GVector3f pt = pNode->data;
		if ( pt.X >= b.X )
			CrossPtList.Insert( pt );
		pNode = pNode->m_pRight;
	}*
	Result[61] = ::getSortedPtLen( CrossPtList );
	Result[62] = ::getSortedPtLen( CrossPtList ) + b.Y;
	(*CvxPt0) = CrossPtList;
	(*CvxPt1) = CrossPtList;*/
	Result[61] = cvx.GetHullLength2();
	Result[62] = cvx.GetHullLength2() + b.Y;
	(*CvxPt0) = cvx.m_HullPtList;
	(*CvxPt1) = cvx1.m_HullPtList;
}


/**********************************************************************
 * 측정항목 58 : 엉덩이옆길이
 * 필요한 측정점(인덱스) : 허리옆점(19), 엉덩이돌출점수준높이(23)
 *						   오른쪽 겨드랑점(9), 왼쪽 겨드랑점(10), 샅높이점(25)
 * 설명: 허리옆점부터 엉덩이돌출점수준높이까지의 체표길이.
 **********************************************************************/
 void GetResultPose2_58( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GLandMark w, hip, c, d, crotch;
	w =   LandMark[19]; // 허리옆점-오른쪽.
	hip = LandMark[24]; // 엉덩이돌출점.
	c = LandMarkHelp[42]; //9
	d = LandMarkHelp[43];  //10
	crotch = LandMark[25];
	GVector3f e;	// 엉덩이 돌출점 둘레에서 허리옆점과 수직한점-오른쪽

	CHECK(w); 
	CHECK(hip);
	GObList<GVector3f> CrossPtList; //  엉덩이옆길이 교차점을 저장하는 리스트.
	GObList<GVector3f> CrossPtListC; //  엉덩이옆길이 교차점을 저장하는 리스트.


	double tan1 = tan( M_PI / 2.0 - RefAngle[3]);  
	double tan2 = tan( M_PI / 2.0 + RefAngle[4]);
	
	GPlane pln[3];
	pln[0].MakePlane( GVector3f( -1.0f, tan1, 0.0f ), c );  // 팔,몸통 나눔면
	pln[1].MakePlane( GVector3f( 1.0f, tan2, 0.0f ), d );
	pln[2].MakePlane( GVector3f( 0.0f, 0.0f, 1.0f ), w ); 	// 허리옆점을 지나는 수직한 관상 평면.
	
	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		if ( pln[0].IsAbovePlane(v0) || pln[0].IsAbovePlane(v1) || pln[0].IsAbovePlane(v2) )
			continue;
		if ( pln[1].IsAbovePlane(v0) || pln[1].IsAbovePlane(v1) || pln[1].IsAbovePlane(v2) )
			continue;
		if ( pln[2].IsAbovePlane(v0) && pln[2].IsAbovePlane(v1) && pln[2].IsAbovePlane(v2) )
			continue;
		if ( pln[2].IsBelowPlane(v0) && pln[2].IsBelowPlane(v1) && pln[2].IsBelowPlane(v2) )
			continue;
		if ( v0.Y > w.Y       && v1.Y > w.Y       && v2.Y > w.Y )
			continue;
		if ( v0.Y < hip.Y && v1.Y < hip.Y && v2.Y < hip.Y )
			continue;
		if ( v0.X > w.X+100.0 || v1.X > w.X+100.0 || v2.X > w.X+100.0 )
			continue;
		if ( pln[2].GetSignedDistance( v0 ) * pln[2].GetSignedDistance( v1 ) < 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v0, v1-v0 );
			CrossPtListC.Insert( CrossPt );
			if (CrossPt.Y > hip.Y) 	CrossPtList.Insert( CrossPt );
		}
		if ( pln[2].GetSignedDistance( v1 ) * pln[2].GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v1, v2-v1 );
			CrossPtListC.Insert( CrossPt );
			if (CrossPt.Y > hip.Y) 	CrossPtList.Insert( CrossPt );
		}
		if ( pln[2].GetSignedDistance( v2 ) * pln[2].GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v0, v2-v0 );
			CrossPtListC.Insert( CrossPt );
			if (CrossPt.Y > hip.Y) 	CrossPtList.Insert( CrossPt );
		}
	}

	CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXY();
	cvx.ArrangeHullPtY();

	// refPlane 아래에 있는 점들만 엉덩이옆선점들로 함 - 다리안쪽점같은것이 포함될 수 있기 때문에..
	e.Y = 10000.0f;
	GObNode<GVector3f> *pNode = CrossPtListC.m_pLeftEnd;
	while( pNode )  //crosspt중에 제일 낮은점 찾기
	{
		GVector3f pt = pNode->data;
		if ( pt.Y < e.Y )
			e = pt;
		pNode = pNode->m_pRight;
	}
	GVector3f tmp = (w+e) * 0.5f;
	tmp.Z += 100.0f;
	GPlane refPln;
	refPln.MakePlane( w, tmp, e ); //허리와 엉덩이높이 옆점을 지나는 종단면

	CrossPtList.DeleteAll();
	CrossPtList = cvx.m_HullPtList;
	cvx.m_HullPtList.DeleteAll();
	pNode = CrossPtList.m_pLeftEnd;
	while( pNode )
	{
		GVector3f pt = pNode->data;
		if( refPln.IsBelowPlane( pt ) )
			cvx.m_HullPtList.Insert( pt );  //refPlane 아래에 있는 점들만 엉덩이옆선점들로 함
		pNode = pNode->m_pRight;
	}
	cvx.m_HullPtList.Insert( w );
	cvx.m_HullPtList.Insert( e );
	cvx.ArrangeHullPtY();
	(*CvxPt) = cvx.m_HullPtList;
	Result[58] = cvx.GetHullLength2();

}

 

/***************************************************************************
 * 측정항목    63_66_65: 팔길이, 위팔길이, 목뒤손목안쪽길이
 * 필요한 측정점(인덱스) :  목뒤점(6), 
 *							오른쪽어깨가쪽점(7), 
 *							노뼈위점(31), 
 *							손목안쪽점(32).
 * 설명 : 
 ***************************************************************************/
void GetResultPose2_63_66_65( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 )
{
	// 변수 초기화.
	(*CvxPt0).DeleteAll();
	(*CvxPt1).DeleteAll();
	(*CvxPt2).DeleteAll();
	GLandMark bn = LandMark[6];
	GLandMark shp = LandMark[7];
	GLandMark elbowPt = LandMark[31]; //노뼈위점 
	GLandMark wristPt = LandMark[32];
	CHECK(bn);
	CHECK(shp);
	CHECK(elbowPt);
	CHECK(wristPt);

	// 변수 선언.
	GObList<GVector3f> CrossPtList0;
	GObList<GVector3f> CrossPtList1;
	GObList<GVector3f> CrossPtList2;

	GPlane refPln0, refPln1, refPln2;
	GPlane cutPln0, cutPln1, cutPln2;

	GVector3f pt0, pt1, pt2;

	// 절단평면설정.
	//목뒤 ~ 어깨
	pt0 = (bn+shp) * 0.5f;   pt0.Y += 10.0f;    pt0.Z -= 5.0f; 
	cutPln0.MakePlane( bn, pt0, shp );       

	//위팔
	pt0 = (shp+elbowPt) * 0.5f;     pt0.X += 10.0f;  pt0.Z += 10.0f; 
	cutPln1.MakePlane( shp, pt0, elbowPt );

	//아래팔
	pt0 = (elbowPt+wristPt) * 0.5f;   pt0.Z += 10.0f;
	cutPln2.MakePlane( elbowPt, pt0, wristPt );
	
	// 참조평면설정.
	pt0 = (bn+shp) * 0.5f;   pt0.Z += 10.0f;
	refPln0.MakePlane( bn, shp, pt0 );

	pt0 = (shp+elbowPt) * 0.5f;    pt0.Z += 10.0f;
	refPln1.MakePlane( shp, elbowPt, pt0 );

	pt0 = (elbowPt+wristPt) * 0.5f;     pt0.X += 100.0f;
	refPln2.MakePlane( elbowPt, pt0, wristPt );

	// 교차점 모으기0 - 목뒤 ~ 어깨
	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		// culling.
		if ( v0.Y > bn.Y || v1.Y > bn.Y || v2.Y > bn.Y )
			continue;
		if ( v0.Y < shp.Y || v1.Y < shp.Y || v2.Y < shp.Y )
			continue;
		
		if ( refPln0.IsAbovePlane( v0 ) && refPln0.IsAbovePlane( v1 ) && refPln0.IsAbovePlane( v2 ) )
		{
			if ( cutPln0.IsAbovePlane(v0) && cutPln0.IsAbovePlane(v1) && cutPln0.IsAbovePlane(v2) )
				continue;
			if ( cutPln0.IsBelowPlane(v0) && cutPln0.IsBelowPlane(v1) && cutPln0.IsBelowPlane(v2) )
				continue;
			if ( cutPln0.GetSignedDistance( v0 ) * cutPln0.GetSignedDistance( v1 ) < 0 )
			{
				GVector3f CrossPt = cutPln0.GetProjectionPt( v0, v1-v0 );
				CrossPtList0.Insert( CrossPt );
			}
			if ( cutPln0.GetSignedDistance( v1 ) * cutPln0.GetSignedDistance( v2 ) < 0 )
			{
				GVector3f CrossPt = cutPln0.GetProjectionPt( v1, v2-v1 );
				CrossPtList0.Insert( CrossPt );
			}
			if ( cutPln0.GetSignedDistance( v2 ) * cutPln0.GetSignedDistance( v0 ) < 0 )
			{
				GVector3f CrossPt = cutPln0.GetProjectionPt( v0, v2-v0 );
				CrossPtList0.Insert( CrossPt );
			}
		}
	}
	CrossPtList0.Insert( bn );
	CrossPtList0.Insert( shp );

	//cutPln1.d =1.0f;
	// 교차점 모으기1  : 위팔
	for (int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		// culling.
		if ( v0.Y > shp.Y || v1.Y > shp.Y || v2.Y > shp.Y )
			continue;
		if ( v0.Y < elbowPt.Y   || v1.Y < elbowPt.Y   || v2.Y < elbowPt.Y )
			continue;
		
		if ( refPln1.IsAbovePlane( v0 ) && refPln1.IsAbovePlane( v1 ) && refPln1.IsAbovePlane( v2 ) )
		{
			if ( cutPln1.IsAbovePlane(v0) && cutPln1.IsAbovePlane(v1) && cutPln1.IsAbovePlane(v2) )
				continue;
			if ( cutPln1.IsBelowPlane(v0) && cutPln1.IsBelowPlane(v1) && cutPln1.IsBelowPlane(v2) )
				continue;

			if ( cutPln1.GetSignedDistance( v0 ) * cutPln1.GetSignedDistance( v1 ) < 0 )
			{
				GVector3f CrossPt = cutPln1.GetProjectionPt( v0, v1-v0 );
				CrossPtList1.Insert( CrossPt );
			}
			if ( cutPln1.GetSignedDistance( v1 ) * cutPln1.GetSignedDistance( v2 ) < 0 )
			{
				GVector3f CrossPt = cutPln1.GetProjectionPt( v1, v2-v1 );
				CrossPtList1.Insert( CrossPt );
			}
			if ( cutPln1.GetSignedDistance( v2 ) * cutPln1.GetSignedDistance( v0 ) < 0 )
			{
				GVector3f CrossPt = cutPln1.GetProjectionPt( v0, v2-v0 );
				CrossPtList1.Insert( CrossPt );
			}
		}
	}
	CrossPtList1.Insert( shp );
	CrossPtList1.Insert( elbowPt );

	// 교차점 모으기2 : 아래팔
	for (int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		// culling.
		if ( v0.Y > elbowPt.Y || v1.Y > elbowPt.Y || v2.Y > elbowPt.Y )
			continue;
		if ( v0.Y < wristPt.Y || v1.Y < wristPt.Y || v2.Y < wristPt.Y )
			continue;
		
		if ( refPln2.IsAbovePlane( v0 ) && refPln2.IsAbovePlane( v1 ) && refPln2.IsAbovePlane( v2 ) )
		{
			if ( cutPln2.IsAbovePlane(v0) && cutPln2.IsAbovePlane(v1) && cutPln2.IsAbovePlane(v2) )
				continue;
			if ( cutPln2.IsBelowPlane(v0) && cutPln2.IsBelowPlane(v1) && cutPln2.IsBelowPlane(v2) )
				continue;
			if ( cutPln2.GetSignedDistance( v0 ) * cutPln2.GetSignedDistance( v1 ) <= 0 )
			{
				GVector3f CrossPt = cutPln2.GetProjectionPt( v0, v1-v0 );
				CrossPtList2.Insert( CrossPt );
			}
			if ( cutPln2.GetSignedDistance( v1 ) * cutPln2.GetSignedDistance( v2 ) <= 0 )
			{
				GVector3f CrossPt = cutPln2.GetProjectionPt( v1, v2-v1 );
				CrossPtList2.Insert( CrossPt );
			}
			if ( cutPln2.GetSignedDistance( v2 ) * cutPln2.GetSignedDistance( v0 ) <= 0 )
			{
				GVector3f CrossPt = cutPln2.GetProjectionPt( v0, v2-v0 );
				CrossPtList2.Insert( CrossPt );
			}
		}
	}
	CrossPtList2.Insert( elbowPt );
	CrossPtList2.Insert( wristPt );

	// 측정경로를 위한 컨벡스헐 변수.
	CConvexHull cvx0( CrossPtList0 );
	CConvexHull cvx1( CrossPtList1 );
	CConvexHull cvx2( CrossPtList2 );

	cvx0.MakeConvexHullXY();  //목뒤 ~ 어깨
	cvx0.m_HullPtList.Insert(bn);
	cvx0.m_HullPtList.Insert(shp);
	cvx0.ArrangeHullPtY();
	
	cvx1.MakeConvexHullYZ();  //위팔
    //cvx1.m_HullPtList = cvx1.m_PtList;
	cvx1.m_HullPtList.Insert(shp);
	cvx1.m_HullPtList.Insert(elbowPt);
	cvx1.ArrangeHullPtY();

	GVector3f tmp=(elbowPt+wristPt)*0.5f;
	tmp.Z+=100.0f;
	cvx2.m_PtList.Insert(tmp);
	cvx2.MakeConvexHullYZ();  //아래팔
	cvx2.m_HullPtList.Insert(elbowPt);
	cvx2.m_HullPtList.Insert(wristPt);
	cvx2.m_HullPtList.Delete(tmp);
	cvx2.ArrangeHullPtY();

	GObNode<GVector3f> *pNode;
	//아래팔
	pNode = cvx2.m_HullPtList.m_pLeftEnd; 
	while( pNode )
	{
		GVector3f pt = pNode->data;
		(*CvxPt0).Insert( pt );  //0 팔길이
		(*CvxPt2).Insert( pt );  //2 목뒤손목안쪽길이
		pNode = pNode->m_pRight;
	}
	//위팔
	pNode = cvx1.m_HullPtList.m_pLeftEnd;
	while( pNode )
	{
		GVector3f pt = pNode->data;
		(*CvxPt0).Insert( pt ); //0 팔길이
		(*CvxPt1).Insert( pt ); //1 위팔길이,
		(*CvxPt2).Insert( pt ); //2 목뒤손목안쪽길이
		pNode = pNode->m_pRight;
	}
	pNode = cvx0.m_HullPtList.m_pLeftEnd;
	while( pNode )
	{
		GVector3f pt = pNode->data;
		(*CvxPt2).Insert( pt );
		pNode = pNode->m_pRight; //2 목뒤손목안쪽길이
	}

	Result[63] =  cvx1.GetHullLength2() + cvx2.GetHullLength2();  //팔길이
	Result[66] =  cvx1.GetHullLength2();                          //위팔길이
	Result[65] =  cvx0.GetHullLength2() + cvx1.GetHullLength2() + cvx2.GetHullLength2(); //목뒤손목안쪽길이
}


/*/ 측정항목 77  : 젖꼭지 수평 길이 = 유두간격
// 필요한 측정점(인덱스) : 오른쪽 젖꼭지점(16), 왼쪽 젖꼭지점(17)
// 설명 : 오른쪽 젖꼭지점과 왼쪽 젖꼭지점 사이의 X값의 차이
void GetResultPose2_77( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GObList<GVector3f> CrossPtList;
	GLandMark a, b;
	a = LandMark[16];
	b = LandMark[17];
	CHECK(a);
	CHECK(b);
	CrossPtList.Insert( a );
	CrossPtList.Insert( b );
	(*CvxPt) = CrossPtList;
	Result[77] = fabs( b.X - a.X );
}
*/

/* 측정항목 68: 목(밑)너비.
// 필요한 측정점(인덱스) : 오른쪽목옆점(36), 외쪽목옆점(37)
// 필요한 측정점(인덱스) : 오른쪽목옆점(4), 외쪽목옆점(5)
// 설명: 오른쪽목옆점과 왼쪽목옆점사의의 너비.
void GetResultPose2_68( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GObList<GVector3f> CrossPtList;
	GLandMark a = LandMark[4]; // 오른쪽목옆점
	GLandMark b = LandMark[5]; // 왼쪽목옆점
	CHECK(a);
	CHECK(b);
	CrossPtList.Insert( a );
	CrossPtList.Insert( b );
	(*CvxPt) = CrossPtList;
	Result[42] = (float)fabs( a.X - b.X );
}*/

// 54~60  앞에서 다 측정했음



// 72 : 바지허리둘레 -비스듬
void GetResultPose2_72( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GLandMark armpitTR, armpitTL, slacksWF, slacksWB;
	armpitTR = LandMarkHelp[1];
	armpitTL = LandMarkHelp[2];
	slacksWF = LandMarkHelp[73];
	slacksWB = LandMarkHelp[74];

    CHECK(slacksWF);
	CHECK(slacksWB);

    if( slacksWF.Y < 100.0f || slacksWB.Y < 100.0f ) return;

    GPlane lplane, rplane;
	double tan1 = tan( 3.14159265 / 2.0 - RefAngle[5]);
	double tan2 = tan( 3.14159265 / 2.0 + RefAngle[6]);
	rplane.MakePlane( GVector3f( -1.0f, tan1, 0.0f ), armpitTR  );
	lplane.MakePlane( GVector3f( 1.0f, tan2, 0.0f ), armpitTL  );

        GVector3f tmpP = GVector3f(slacksWF.X+20.0f, slacksWF.Y,  slacksWF.Z);
        GPlane CutPln;
        CutPln.MakePlane(slacksWF, slacksWB, tmpP);

        float upH, downH;
        if (slacksWF.Y > slacksWB.Y ) {
               upH = slacksWF.Y;        downH = slacksWB.Y;
        }
        else if ( slacksWF.Y < slacksWB.Y ) {
               upH = slacksWB.Y;        downH = slacksWF.Y;
        }
        else {
               upH = slacksWF.Y+5.0f;
               downH = slacksWF.Y-5.0f;
        }

        GObList<GVector3f> CrossPtList;

  	for (int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

            if(v0.Y>upH &&   v1.Y>upH   && v2.Y>upH )   continue;
            if(v0.Y<downH && v1.Y<downH && v2.Y<downH ) continue;

            if ( lplane.IsBelowPlane( v0 ) && rplane.IsBelowPlane( v0 ) )
            {
                if ( CutPln.IsAbovePlane(v0) && CutPln.IsAbovePlane(v1) && CutPln.IsAbovePlane(v2) )
                        continue;
                if ( CutPln.IsBelowPlane(v0) && CutPln.IsBelowPlane(v1) && CutPln.IsBelowPlane(v2) )
                        continue;


                if ( CutPln.GetSignedDistance( v0 ) * CutPln.GetSignedDistance( v1 ) < 0 )  //만약 v0, v1의 x좌표가 같으면?
                {
                        GVector3f CrossPt = CutPln.GetProjectionPt( v0, v1-v0 );
                        CrossPtList.Insert(CrossPt);
                }
                if ( CutPln.GetSignedDistance( v1 ) * CutPln.GetSignedDistance( v2 ) < 0 )
                {
                        GVector3f CrossPt = CutPln.GetProjectionPt( v1, v2-v1 );
                        CrossPtList.Insert(CrossPt);
                }
                if ( CutPln.GetSignedDistance( v2 ) * CutPln.GetSignedDistance( v0 ) < 0 )
                {
                        GVector3f CrossPt = CutPln.GetProjectionPt( v0, v2-v0 );
                        CrossPtList.Insert(CrossPt);
                }
            } //if


	} //for


    CConvexHull cvx( CrossPtList );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	Result[72] = (float)cvx.GetHullLength();        //바지허리둘레
	(*CvxPt) = cvx.m_HullPtList;                    //바지허리둘레 경로

}



// 측정항목    78: 겨드랑두께.
// 필요한 측정점(인덱스) : 오른쪽겨드랑앞벽점(12), 오른쪽겨드랑이뒤벽점(14)
//				  보조점 : 오른쪽겨드랑앞돌출(Help 54), 오른쪽겨드랑이뒤돌출(Help 55)

// 설명: 오른쪽겨드랑이앞벽점과 오른쪽겨드랑이뒤벽점의 두께.
void GetResultPose2_78( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GObList<GVector3f> CrossPtList;
	GLandMark a = LandMark[44];
	GLandMark b = LandMark[45];
	CHECK(a);
	CHECK(b);
	a.Y=b.Y;
	a.X=b.X;
	CrossPtList.Insert( a );
	CrossPtList.Insert( b );
	(*CvxPt) = CrossPtList;
	Result[78] = fabs( b.Z - a.Z );
}

// 63~85 앞에서 다 했음

// 측정항목 86 : 오른어깨기울기(어깨경사각도)
// 필요한 측정점(인덱스) : 목옆점-R(4), 어깨점-R(7)
// 설명 : 
void GetResultPose2_86( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt, GObList<GVector3f> *CvxPt1)
{
	(*CvxPt).DeleteAll();
	(*CvxPt1).DeleteAll();
	GLandMark shR, shL, snR, snL, c;
	shR = LandMark[7];
	snR = LandMark[4];
	shL = LandMark[8];
	snL = LandMark[5];
	CHECK(shR);
	CHECK(snR);
	c = shR-snR;
	GVector3f d = GVector3f(-1,0,0);
	
	float angle = d.getAngle( c);
    float val= 180*(angle/3.14);

	(*CvxPt).Insert( shR );
	(*CvxPt).Insert( snR );

	Result[86] = val;


	c = shL-snL;
	d = GVector3f(1,0,0);
	
	angle = c.getAngle( d);
    val= 180*(angle/3.14);

	(*CvxPt1).Insert( shL );
	(*CvxPt1).Insert( snL );

	Result[87] = val;
}

/***************************************************************************
 * 측정항목    75: 젖가슴 아래 둘래(비만용, 공식에 의한 계산).
 * 필요한 측정점(인덱스) :  오른쪽 젖가슴 아래점(18), - 앞쪽으로 옮겨짐
 *							오른쪽 겨드랑점(9), 
 *							왼쪽 겨드랑점(10),
 *							오른쪽 젖꼭지점(16),
 *							왼쪽 젖꼭지점(17).
 * 설명 : 오른쪽 젖가슴 아래점 높이에서의 수평 둘레
 ***************************************************************************
void GetResultPose2_75( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GObList<GVector3f> CrossPtList0;
	GObList<GVector3f> CrossPtList1;
	GLandMark b = LandMark[18];
	GLandMark c = LandMark[9];
	GLandMark d = LandMark[10];
	GLandMark e = LandMark[16];
	GLandMark f = LandMark[17];
	CHECK(b);
	CHECK(c);
	CHECK(d);
	CHECK(e);
	CHECK(f);

	// plane setting.
//	double ang1 = 90.0f - RefAngle[3];
//	double ang2 = 90.0f - RefAngle[4];
	double tan1 = tan( M_PI / 2.0 - RefAngle[3]);
	double tan2 = tan( M_PI / 2.0 - RefAngle[4]);
	GVector3f tmp = ( c + d ) * 0.5f;	tmp.Y -= 100.0f; 
	GPlane pln[5];
	pln[0].MakePlane( GVector3f( -1.0f, tan1, 0.0f ), c );
	pln[1].MakePlane( GVector3f( 1.0f, tan2, 0.0f ), d );
	pln[2].MakePlane( GVector3f( 0.0f, 1.0f, 0.0f ), b );
	pln[3].MakePlane( c, tmp, d );
	tmp = (e+f) * 0.5f;
	pln[4].MakePlane( GVector3f( 1.0f, 0.0f, 0.0f ), tmp );

	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln[0].IsAbovePlane(v0) || pln[0].IsAbovePlane(v1) || pln[0].IsAbovePlane(v2) )
			continue;
		if ( pln[1].IsAbovePlane(v0) || pln[1].IsAbovePlane(v1) || pln[1].IsAbovePlane(v2) )
			continue;
		if ( pln[2].IsAbovePlane(v0) && pln[2].IsAbovePlane(v1) && pln[2].IsAbovePlane(v2) )
			continue;
		if ( pln[2].IsBelowPlane(v0) && pln[2].IsBelowPlane(v1) && pln[2].IsBelowPlane(v2) )
			continue;
		if ( pln[3].IsAbovePlane(v0) || pln[3].IsAbovePlane(v1) || pln[3].IsAbovePlane(v2) )
			continue;

		if ( pln[2].GetSignedDistance( v0 ) * pln[2].GetSignedDistance( v1 ) < 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v0, v1-v0 );
			CrossPtList0.Insert( CrossPt );
		}
		if ( pln[2].GetSignedDistance( v1 ) * pln[2].GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v1, v2-v1 );
			CrossPtList0.Insert( CrossPt );
		}
		if ( pln[2].GetSignedDistance( v2 ) * pln[2].GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = pln[2].GetProjectionPt( v0, v2-v0 );
			CrossPtList0.Insert( CrossPt );
		}
	}

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( pln[0].IsAbovePlane(v0) || pln[0].IsAbovePlane(v1) || pln[0].IsAbovePlane(v2) )
			continue;
		if ( pln[1].IsAbovePlane(v0) || pln[1].IsAbovePlane(v1) || pln[1].IsAbovePlane(v2) )
			continue;
		if ( pln[2].IsBelowPlane(v0) || pln[2].IsBelowPlane(v1) || pln[2].IsBelowPlane(v2) )
			continue;
		if ( pln[3].IsBelowPlane(v0) || pln[3].IsBelowPlane(v1) || pln[3].IsBelowPlane(v2) )
			continue;

		if ( pln[4].IsAbovePlane(v0) && pln[4].IsAbovePlane(v1) && pln[4].IsAbovePlane(v2) )
			continue;
		if ( pln[4].IsBelowPlane(v0) && pln[4].IsBelowPlane(v1) && pln[4].IsBelowPlane(v2) )
			continue;
		
		if ( pln[4].GetSignedDistance( v0 ) * pln[4].GetSignedDistance( v1 ) < 0 )
		{
			GVector3f CrossPt = pln[4].GetProjectionPt( v0, v1-v0 );
			CrossPtList1.Insert( CrossPt );
		}
		if ( pln[4].GetSignedDistance( v1 ) * pln[4].GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = pln[4].GetProjectionPt( v1, v2-v1 );
			CrossPtList1.Insert( CrossPt );
		}
		if ( pln[4].GetSignedDistance( v2 ) * pln[4].GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = pln[4].GetProjectionPt( v0, v2-v0 );
			CrossPtList1.Insert( CrossPt );
		}
	}

	// 보간해야할 점들.
	GVector3f p0, p1, p2, p3, p4;
	p1 = e;
	p3 = f;

	// p0, p4 구하기.
	p0.Z = -1000.0f;
	p4.Z = -1000.0f;
	GObNode<GVector3f> *pNode = CrossPtList0.m_pLeftEnd;
	while( pNode )
	{
		GVector3f v = pNode->data;
		if ( v.X < p1.X )
		{
			if ( v.Z > p0.Z )
				p0 = v;
		}
		if ( v.X > p3.X )
		{
			if ( v.Z > p4.Z )
				p4 = v;
		}
		pNode = pNode->m_pRight;
	}

	// p2 구하기.
	p2.Y = 10000.0f;
	pNode = CrossPtList1.m_pLeftEnd;
	while( pNode )
	{
		GVector3f v = pNode->data;
		if ( v.Y < p2.Y )
			p2 = v;
		pNode = pNode->m_pRight;
	}

	// p1, p3 구하기.
	p1 = e * 0.6f + f * 0.4f;
	p3 = e * 0.4f + f * 0.6f;
	p1.Y = p2.Y; p1.Z = p2.Z;
	p3.Y = p2.Y; p3.Z = p2.Z;
	

	REAL4 P[5];
	P[0][0] = p0.X; P[0][1] = p0.Y; P[0][2] = p0.Z; P[0][3] = 1.0f;
	P[1][0] = p1.X; P[1][1] = p1.Y; P[1][2] = p1.Z; P[1][3] = 1.0f;
	P[2][0] = p2.X; P[2][1] = p2.Y; P[2][2] = p2.Z; P[2][3] = 1.0f;
	P[3][0] = p3.X; P[3][1] = p3.Y; P[3][2] = p3.Z; P[3][3] = 1.0f;
	P[4][0] = p4.X; P[4][1] = p4.Y; P[4][2] = p4.Z; P[4][3] = 1.0f;

	GNurbsCrv PathCrv;
	PathCrv.InterCrv( 3, P, 4 );
	REAL4 *PP = PathCrv.GetCtlPt();
	PP[0][3] = 1.0f;
	PP[1][3] = 0.8f;
	PP[2][3] = 1.0f;
	PP[3][3] = 0.80f;
	PP[4][3] = 1.0f;
		
	int snum = 20;
	REAL4 v0;
	REAL min = PathCrv.GetMinKnotVal();
	REAL max = PathCrv.GetMaxKnotVal();
	for ( i = 0; i < snum; i++ )
	{
		REAL u0 = min + ( (max-min)*i ) / (REAL)(snum-1);
		PathCrv.Eval( u0, v0 );
		GVector3f pt( v0 );
		CrossPtList0.Insert( pt );
	}
	CConvexHull cvx( CrossPtList0 );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;
	//(*CvxPt) = CrossPtList0;
	Result[70] = val;
}*/


/***************************************************************************
 * 측정항목    75: 젖가슴아래둘레(처짐용, 공식에 의한 계산).
 * 필요한 측정점(인덱스) :  오른쪽 젖가슴 아래점(18), - 앞쪽으로 옮겨짐
 *							오른쪽 겨드랑점(9), 
 *							왼쪽 겨드랑점(10),
 *							오른쪽 젖꼭지점(16),
 *							왼쪽 젖꼭지점(17).
							실제-젖가슴아래점(42)
 * 설명 : 오른쪽 젖가슴 아래점 높이에서의 수평 둘레
 ***************************************************************************/
//실제젖가슴아래높이(h1) 단면을 구한다
//데이터상 젖가슴아래높이(h2) 단면을 구한다
//정중시상면을 구한다- 어기에서 h2높이의 점(a)과 가슴이 시작되는 높이(주로겨드랑점높이)의 점(b)을 구한다
//ab선상에 있으면서 h1높이인 점(c)을 구한다.
//a점 c점의 깊이차(dZ)를 구한다.
//h2단면에서 겨드랑점보다 앞쪽 데이터를 dZ만큼 뒤로 이동한다
//h1단면에서 겨드랑점보다 뒤쪽 데이터와 h2단면에서 겨드랑점보다 앞쪽 데이터를 합하고 convexHull을 구한다.
void GetResultPose2_75( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	GObList<GVector3f> CrossPtList0;
	GObList<GVector3f> CrossPtList1;
	GLandMark ub3D = LandMark[18];
	GLandMark ubActual = LandMark[42];
	GLandMark c = LandMark[9];
	GLandMark d = LandMark[10];
	GLandMark CArmpitHPt = LandMarkHelp[62];
	GLandMark CpitPt = LandMarkHelp[52]; //젖가슴아래점-정중시상면상

	CHECK(ub3D);
	CHECK(ubActual);
	CHECK(d);
	CHECK(CArmpitHPt);

	// plane setting.
	GPlane lplane, rplane, FBPln, ub3DPln, ubActualPln;	
	double tan1 = tan( M_PI / 2.0 - RefAngle[5]);
	double tan2 = tan( M_PI / 2.0 + RefAngle[6]);
	GVector3f tmp = ( c + d ) * 0.5f;	tmp.Y -= 100.0f; 
	rplane.MakePlane( GVector3f( -1.0f, tan1, 0.0f ), c );
	lplane.MakePlane( GVector3f( 1.0f, tan2, 0.0f ), d );
	ub3DPln.MakePlane( GVector3f( 0.0f, 1.0f, 0.0f ), ub3D );
	FBPln.MakePlane( c, tmp, d ); //앞쪽이 위쪽	
	ubActualPln.MakePlane( GVector3f( 0.0f, 1.0f, 0.0f ), ubActual );

	float centerX = (c.X +d.X)*0.5f;
	float minDis3DHX =1000.0f;
	float minDisArmpitHX =1000.0f;
	float minDisArmpitHY =1000.0f;

	float Dis3DHX;
	GVector3f C3DHPt;

	//h2 = 3D젖가슴아래높이 단면 구하기 - 겨드랑점보다 앞쪽 데이터만..
	for ( int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( rplane.IsAbovePlane(v0) || rplane.IsAbovePlane(v1) || rplane.IsAbovePlane(v2) )
			continue;
		if ( lplane.IsAbovePlane(v0) || lplane.IsAbovePlane(v1) || lplane.IsAbovePlane(v2) )
			continue;
		if ( FBPln.IsBelowPlane(v0) || FBPln.IsBelowPlane(v1) || FBPln.IsBelowPlane(v2) )
			continue;
		if ( ub3DPln.IsBelowPlane(v0) && ub3DPln.IsBelowPlane(v1) && ub3DPln.IsBelowPlane(v2) )
			continue;		
		if ( ub3DPln.IsAbovePlane(v0) && ub3DPln.IsAbovePlane(v1) && ub3DPln.IsAbovePlane(v2) )
			continue;

		float dis0, dis1, dis2;
		GVector3f CrossPt;
		dis0 = ub3DPln.GetSignedDistance( v0 );
		dis1 = ub3DPln.GetSignedDistance( v1 );
		dis2 = ub3DPln.GetSignedDistance( v2 );
		if ( dis0 * dis1 <= 0 )
		{
			if     (dis0 == 0.0f && dis1 ==0.0f)
					CrossPt = v0 ;			
			else if(dis0 == 0.0f && dis1 !=0.0f)
					CrossPt = v0 ;
			else if(dis0 != 0.0f && dis1 ==0.0f)
					CrossPt = v1 ;
			else{
					CrossPt = ub3DPln.GetProjectionPt( v0, v1-v0 );
			}
				CrossPtList0.Insert( CrossPt );

			Dis3DHX = abs(CrossPt.X -centerX);
			if( Dis3DHX < minDis3DHX ) {
				C3DHPt =CrossPt;
				minDis3DHX =Dis3DHX;
			}
		}

		if ( dis1 * dis2 <= 0 )
		{
			if     (dis2 == 0.0f && dis1 ==0.0f)
					CrossPt = v2 ;			
			else if(dis2 == 0.0f && dis1 !=0.0f)
					CrossPt = v2 ;
			else if(dis2 != 0.0f && dis1 ==0.0f)
					CrossPt = v1 ;
			else{
					CrossPt = ub3DPln.GetProjectionPt( v2, v1-v2 );
			}
				CrossPtList0.Insert( CrossPt );

			Dis3DHX = abs(CrossPt.X -centerX);
			if( Dis3DHX < minDis3DHX ) {
				C3DHPt =CrossPt;
				minDis3DHX =Dis3DHX;
			}
		}
		if ( ub3DPln.GetSignedDistance( v2 ) * ub3DPln.GetSignedDistance( v0 ) <= 0 )
		{
			if     (dis2 == 0.0f && dis0 ==0.0f)
					CrossPt = v2 ;			
			else if(dis2 == 0.0f && dis0 !=0.0f)
					CrossPt = v2 ;
			else if(dis2 != 0.0f && dis0 ==0.0f)
					CrossPt = v0 ;
			else{
					CrossPt = ub3DPln.GetProjectionPt( v2, v0-v2 );
			}
				CrossPtList0.Insert( CrossPt );

			Dis3DHX = abs(CrossPt.X -centerX);
			if( Dis3DHX < minDis3DHX ) {
				C3DHPt =CrossPt;
				minDis3DHX =Dis3DHX;
			}
		}

	}
	float ubActualZ = ( ( (CArmpitHPt.Y-ubActual.Y)*(C3DHPt.Z-CArmpitHPt.Z) )
					/(CArmpitHPt.Y-C3DHPt.Y) ) + CArmpitHPt.Z;
	
	C3DHPt =LandMarkHelp[53];
	float dZ;
	if( ubActualZ< CpitPt.Z )
		 dZ = C3DHPt.Z - ubActualZ;
	else 
		dZ = C3DHPt.Z - CpitPt.Z;


	//h2-실제젖가슴아래 단면 구하기 - 겨드랑점보다 뒤쪽 데이터만...
	for (int i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];
		if ( rplane.IsAbovePlane(v0) || rplane.IsAbovePlane(v1) || rplane.IsAbovePlane(v2) )
			continue;
		if ( lplane.IsAbovePlane(v0) || lplane.IsAbovePlane(v1) || lplane.IsAbovePlane(v2) )
			continue;	
		if ( FBPln.IsAbovePlane(v0) || FBPln.IsAbovePlane(v1) || FBPln.IsAbovePlane(v2) )
			continue;
		if ( ubActualPln.IsAbovePlane(v0) && ubActualPln.IsAbovePlane(v1) && ubActualPln.IsAbovePlane(v2) )
			continue;
		if ( ubActualPln.IsBelowPlane(v0) && ubActualPln.IsBelowPlane(v1) && ubActualPln.IsBelowPlane(v2) )
			continue;
		
		if ( ubActualPln.GetSignedDistance( v0 ) * ubActualPln.GetSignedDistance( v1 ) < 0 )
		{
			GVector3f CrossPt = ubActualPln.GetProjectionPt( v0, v1-v0 );
			CrossPtList1.Insert( CrossPt );
		}
		if ( ubActualPln.GetSignedDistance( v1 ) * ubActualPln.GetSignedDistance( v2 ) < 0 )
		{
			GVector3f CrossPt = ubActualPln.GetProjectionPt( v1, v2-v1 );
			CrossPtList1.Insert( CrossPt );
		}
		if ( ubActualPln.GetSignedDistance( v2 ) * ubActualPln.GetSignedDistance( v0 ) < 0 )
		{
			GVector3f CrossPt = ubActualPln.GetProjectionPt( v0, v2-v0 );
			CrossPtList1.Insert( CrossPt );
		}
	}


	GObNode<GVector3f> *pNode = CrossPtList0.m_pLeftEnd; //3DH curve
	while( pNode )
	{
		pNode->data.Z -= dZ;
		pNode->data.Y = ubActual.Y;

		pNode = pNode->m_pRight;
	}

	pNode = CrossPtList0.m_pLeftEnd;
	while( pNode )
	{
		GVector3f v = pNode->data;
	
		CrossPtList1.Insert(v);

		pNode = pNode->m_pRight;
	}


	CConvexHull cvx( CrossPtList1 );
	cvx.MakeConvexHullXZ();
	cvx.ArrangeHullPtXZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;
	//(*CvxPt) = CrossPtList0;
	Result[75] = val;
}




/***************************************************************************
 * 측정항목    94: 겨드랑둘레 길이( 구버젼 )
   * 필요한 측정점(인덱스) : 오른쪽 어깨가쪽점(7),     오른쪽 겨드랑점(9), 
 *                         오른쪽겨드랑뒤접힘점(45), 오른쪽겨드랑앞점힘점(44).
 * 설명: 앞쪽은 어깨끝점, 앞겨드랑접힘점, 겨드랑점이 포함되는 길이.
 * 	     뒤쪽은 어깨끝점, 뒤겨드랑접힘점, 겨드랑점이 포함되는 길이.
 **************************************************************************/
void GetResultPose2_94( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt )
{
	(*CvxPt).DeleteAll();
	int i;
	GObList<GVector3f> CrossPtList0; // 앞쪽은 어깨끝점, 앞겨드랑접힘점, 겨드랑점이 포함되는 길이.
	GObList<GVector3f> CrossPtList1; // 뒤쪽은 어깨끝점, 뒤겨드랑접힘점, 겨드랑점이 포함되는 길이.
	GLandMark a = LandMark[7];
	GLandMark c = LandMark[9];
	GLandMark d = LandMark[45];
	GLandMark e = LandMark[44];
	CHECK(a);
	CHECK(c);
	CHECK(d);
	CHECK(e);
	GLandMark b = ( d + e ) * 0.5f;
	b.Y = c.Y;

	//CrossPtList.Insert( a );
	//CrossPtList.Insert( c );
	//CrossPtList.Insert( d );
	//CrossPtList.Insert( e );

	// make plane
	GVector3f tmp;
	tmp = (a+c)/2.0f; tmp.X += 100.0f;
	GPlane pln0( a, e, c ); // 앞쪽 길이를 재기 위한 평면.
	GPlane pln1( a, c, d ); // 뒤쪽 길이를 재기 위한 평면.
	GPlane pln2( a, c, tmp ); // 앞쪽과 뒤쪽을 나누기 위한 평면.

	for ( i = 0; i < NumIdx; i+=3 )
	{
		GVector3f v0, v1, v2;
		v0 = Points[ IdxList[i] ];
		v1 = Points[ IdxList[i+1] ];
		v2 = Points[ IdxList[i+2] ];

		// culling.
		if ( v0.Y > a.Y+20.0f && v1.Y > a.Y+20.0f && v2.Y > a.Y+20.0f )
			continue;
		if ( v0.Y < c.Y-20.0f && v1.Y < c.Y-20.0f && v2.Y < c.Y-20.0f )
			continue;
		
		// seperation
		// 앞쪽 길이에 해당되는 점들.
		if ( pln2.IsAbovePlane( v0 ) && pln2.IsAbovePlane( v1 ) && pln2.IsAbovePlane( v2 ) )
		{
			if ( pln0.IsAbovePlane(v0) && pln0.IsAbovePlane(v1) && pln0.IsAbovePlane(v2) )
				continue;
			if ( pln0.IsBelowPlane(v0) && pln0.IsBelowPlane(v1) && pln0.IsBelowPlane(v2) )
				continue;
			if ( pln0.GetSignedDistance( v0 ) * pln0.GetSignedDistance( v1 ) < 0 )
			{
				GVector3f CrossPt = pln0.GetProjectionPt( v0, v1-v0 );
				CrossPtList0.Insert( CrossPt );
			}
			if ( pln0.GetSignedDistance( v1 ) * pln0.GetSignedDistance( v2 ) < 0 )
			{
				GVector3f CrossPt = pln0.GetProjectionPt( v1, v2-v1 );
				CrossPtList0.Insert( CrossPt );
			}
			if ( pln0.GetSignedDistance( v2 ) * pln0.GetSignedDistance( v0 ) < 0 )
			{
				GVector3f CrossPt = pln0.GetProjectionPt( v0, v2-v0 );
				CrossPtList0.Insert( CrossPt );
			}
		}
		// 뒤쪽 길이에 해당되는 점들.
		else
		{
			if ( pln1.IsAbovePlane(v0) && pln1.IsAbovePlane(v1) && pln1.IsAbovePlane(v2) )
				continue;
			if ( pln1.IsBelowPlane(v0) && pln1.IsBelowPlane(v1) && pln1.IsBelowPlane(v2) )
				continue;
			if ( pln1.GetSignedDistance( v0 ) * pln1.GetSignedDistance( v1 ) < 0 )
			{
				GVector3f CrossPt = pln1.GetProjectionPt( v0, v1-v0 );
				CrossPtList0.Insert( CrossPt );
			}
			if ( pln1.GetSignedDistance( v1 ) * pln1.GetSignedDistance( v2 ) < 0 )
			{
				GVector3f CrossPt = pln1.GetProjectionPt( v1, v2-v1 );
				CrossPtList0.Insert( CrossPt );
			}
			if ( pln1.GetSignedDistance( v2 ) * pln1.GetSignedDistance( v0 ) < 0 )
			{
				GVector3f CrossPt = pln1.GetProjectionPt( v0, v2-v0 );
				CrossPtList0.Insert( CrossPt );
			}
		}
	}
	GObNode<GVector3f> *pNode = CrossPtList1.m_pLeftEnd;
	while( pNode )
	{
		GVector3f data = pNode->data;
		CrossPtList0.Insert( data );
		pNode = pNode->m_pRight;
	}
	CConvexHull cvx( CrossPtList0 );
	cvx.MakeConvexHullYZ();
	cvx.ArrangeHullPtYZ();
	float val = cvx.GetHullLength();
	(*CvxPt) = cvx.m_HullPtList;
	Result[71] = val;
}

