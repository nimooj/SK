#pragma once
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Definition of macros and etc.

#ifndef _MTYPE_H
#define _MTYPE_H


/***************************************************************************************
	1. Header files include
/***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <string.h>

/*****************************************************************************************
	1. Definition of REAL Type
/*****************************************************************************************/
#define REAL float
typedef REAL REAL4[4];
typedef REAL REAL3[3];
typedef REAL MATRIX[4][4];
/****************************************************************************************
	Definition of  EPISILON
/****************************************************************************************/
#define VECTOR_EPSILON 0.001
#define MTYPE_EPS			0.000001
#define M_PI 3.14159265
/*****************************************************************************************/
/*	Definition of macros of basic memory management.                                               */
/*****************************************************************************************/
#define REAL_ARR_ZERO( A, N ) \
		memset( A, 0, sizeof(REAL) * (N) )
#define REAL_ARR_COPY( D, S, N ) \
		memmove( (D), (S), (N) * sizeof(REAL) )
/*****************************************************************************************/
#define REAL3_ARR_ZERO( A ) \
		memset( A, 0, sizeof(REAL) * 3 )
#define REAL3_ARR_COPY( D, S, N ) \
		memmove( (D), (S), (N) * 3 * sizeof(REAL) )
/*****************************************************************************************/
#define REAL4_ARR_ZERO( A, N ) \
		memset( A, 0, sizeof(REAL) * 4 * (N) )
#define REAL4_ARR_COPY( D, S, N ) \
		memmove( (D), (S), (N) * 4 * sizeof(REAL) )
/*****************************************************************************************/
#define MATRIX_ZERO( A ) \
		REAL_ARR_ZERO( (A), 16 )
#define MATRIX_ARR_ZERO( M, N ) \
		REAL_ARR_ZERO( (M), (N)*16 )
#define MATRIX_IDENTITY( A ) \
		REAL_ARR_ZERO( (A), 16 ); \
		A[0][0] = A[1][1] = A[2][2] = A[3][3] = 1.0;
#define MATRIX_COPY( A, B ) \
		REAL_ARR_COPY( (A), (B), 16 )
#define MATRIX_ARR_COPY( T, S, N ) \
		REAL_ARR_COPY( (T), (S), (N) * 16 )

/*****************************************************************************
	Definition of macros of mathe operations.
/****************************************************************************/
#define MIN(x, y)	((x) > (y) ? (y) : (x))
#define MAX(x, y)	((x) > (y) ? (x) : (y))
#define SQR(X)		((X)*(X))
#define SQRT(X)		sqrtf(X)
#define	ACOS(x)		acosf(x)
#define SIN(x)		sinf(x)
#define COS(x)		cosf(x)

/******************************************************************************
	Definition of macros of comparision
/*****************************************************************************/
#define ABS(X) ((X) > 0 ? (X) : (-(X)))
#define MTP_EQ_EPS( X, Y, EPS) (ABS((X) - (Y)) < EPS)
#define PT3_EQ_ZERO_EPS(Pt, EPS) \
				(MTP_EQ_EPS(Pt[0], 0.0, EPS) && \
				 MTP_EQ_EPS(Pt[1], 0.0, EPS) && \
				 MTP_EQ_EPS(Pt[2], 0.0, EPS))


/*****************************************************************************/
/*	Definition of REAL3( PT3 ) macros                                                              */
/*****************************************************************************/
#define PT3_ADD(dest,v1,v2) \
		dest[0]=v1[0]+v2[0]; \
		dest[1]=v1[1]+v2[1]; \
		dest[2]=v1[2]+v2[2]; 

#define PT3_SUB(dest,v1,v2) \
		dest[0]=v1[0]-v2[0]; \
		dest[1]=v1[1]-v2[1]; \
		dest[2]=v1[2]-v2[2]; 

#define PT3_DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define PT3_NORM(P) ( SQR(P[0]) + SQR(P[1]) + SQR(P[2]) )

#define PT3_LENGTH(P) SQRT( PT3_NORM(P) )

#define PT3_NORMALIZE(P) \
		{ \
			REAL Size = PT3_LENGTH(P); \
			if (Size < MTYPE_EPS) \
			{ \
				printf("Attempt to normalize a zero length vector\n"); \
			} \
			else \
			{ \
				P[0] /= Size; \
				P[1] /= Size; \
				P[2] /= Size; \
			} \
		}

#define PT3_CROSS(dest,v1,v2) \
		dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
		dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
		dest[2]=v1[0]*v2[1]-v1[1]*v2[0];

#define PT3_DISTANCE(A, B) \
		SQRT( \
				SQR((A[0]) - (B[0]))+ \
				SQR((A[1]) - (B[1]))+ \
				SQR((A[2]) - (B[2])) \
			 )

/**************************************************************************/
/*	Definition of REAL4( PT4 ) macros                                                              */
/***************************************************************************/
#define PT4_ADD(dest,v1,v2) \
		dest[0]=v1[0]+v2[0]; \
		dest[1]=v1[1]+v2[1]; \
		dest[2]=v1[2]+v2[2]; \
		dest[3]=v1[3]+v2[3]; 

#define PT4_SUB(dest,v1,v2) \
		dest[0]=v1[0]-v2[0]; \
		dest[1]=v1[1]-v2[1]; \
		dest[2]=v1[2]-v2[2]; \
		dest[3]=v1[3]-v2[3]; 

#define PT4_DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2]+v1[3]*v2[3])

#define PT4_NORM(P) (SQR(P[0]) + SQR(P[1]) + SQR(P[2]) + SQR(P[3]))

#define PT4_LENGTH(P) SQRT(PT4_NORM(P))

#define PT4_NORMALIZE(P) \
		{ \
			REAL Size = PT4_LENGTH(P); \
			if (Size < MTYPE_EPS) \
			{ \
				printf("Attempt to normalize a zero length vector\n"); \
			} \
			else \
			{ \
				P[0] /= Size; \
				P[1] /= Size; \
				P[2] /= Size; \
				P[3] /= Size; \
			} \
		}

#define PT4_CARTESIANIZE(P) \
		{ \
			if ( P[3] < MTYPE_EPS ) \
			{ \
				fprintf( stderr, "Attempt to cartesianize a zero weight vector\n" ); \
			} \
			else \
			{ \
				P[0] /= P[3]; \
				P[1] /= P[3]; \
				P[2] /= P[3]; \
			} \
		}
#define PT4_BLEND(P, P0, P1, T) \
		{ \
			(P[0]) = (T)*(P1[0]) + ((1.0f)-(T))*(P0[0]); \
			(P[1]) = (T)*(P1[1]) + ((1.0f)-(T))*(P0[1]); \
			(P[2]) = (T)*(P1[2]) + ((1.0f)-(T))*(P0[2]); \
			(P[3]) = (T)*(P1[3]) + ((1.0f)-(T))*(P0[3]); \
		} 
#define PT4_DISTANCE(A, B) \
		SQRT( \
				SQR((A[0]) - (B[0]))+ \
				SQR((A[1]) - (B[1]))+ \
				SQR((A[2]) - (B[2]))+ \
				SQR((A[3]) - (B[3])) \
			)
/**************************************************************************/
/*	Definition of MATRIX macros                                           */
/***************************************************************************/
#define MATRIX_BLEND( D, S1, S2, T ) \
		{ \
			(D[0][0]) = (T)*(S2[0][0]) + ((1.0f) - (T))*(S1[0][0]); \
			(D[0][1]) = (T)*(S2[0][1]) + ((1.0f) - (T))*(S1[0][1]); \
			(D[0][2]) = (T)*(S2[0][2]) + ((1.0f) - (T))*(S1[0][2]); \
			(D[0][3]) = (T)*(S2[0][3]) + ((1.0f) - (T))*(S1[0][3]); \
			(D[1][0]) = (T)*(S2[1][0]) + ((1.0f) - (T))*(S1[1][0]); \
			(D[1][1]) = (T)*(S2[1][1]) + ((1.0f) - (T))*(S1[1][1]); \
			(D[1][2]) = (T)*(S2[1][2]) + ((1.0f) - (T))*(S1[1][2]); \
			(D[1][3]) = (T)*(S2[1][3]) + ((1.0f) - (T))*(S1[1][3]); \
			(D[2][0]) = (T)*(S2[2][0]) + ((1.0f) - (T))*(S1[2][0]); \
			(D[2][1]) = (T)*(S2[2][1]) + ((1.0f) - (T))*(S1[2][1]); \
			(D[2][2]) = (T)*(S2[2][2]) + ((1.0f) - (T))*(S1[2][2]); \
			(D[2][3]) = (T)*(S2[2][3]) + ((1.0f) - (T))*(S1[2][3]); \
			(D[3][0]) = (T)*(S2[3][0]) + ((1.0f) - (T))*(S1[3][0]); \
			(D[3][1]) = (T)*(S2[3][1]) + ((1.0f) - (T))*(S1[3][1]); \
			(D[3][2]) = (T)*(S2[3][2]) + ((1.0f) - (T))*(S1[3][2]); \
			(D[3][3]) = (T)*(S2[3][3]) + ((1.0f) - (T))*(S1[3][3]); \
		}

#define MATRIX_DISTANCE(S0,S1) \
	SQRT( \
				SQR( ((S0[0][0])-(S1[0][0])) ) + SQR( ((S0[0][1])-(S1[0][1])) ) +  \
				SQR( ((S0[0][2])-(S1[0][2])) ) + SQR( ((S0[0][3])-(S1[0][3])) ) +  \
				SQR( ((S0[1][0])-(S1[1][0])) ) + SQR( ((S0[1][1])-(S1[1][1])) ) +  \
				SQR( ((S0[1][2])-(S1[1][2])) ) + SQR( ((S0[1][3])-(S1[1][3])) ) +  \
				SQR( ((S0[2][0])-(S1[2][0])) ) + SQR( ((S0[2][1])-(S1[2][1])) ) +  \
				SQR( ((S0[2][2])-(S1[2][2])) ) + SQR( ((S0[2][3])-(S1[2][3])) ) +  \
				SQR( ((S0[3][0])-(S1[3][0])) ) + SQR( ((S0[3][1])-(S1[3][1])) ) +  \
				SQR( ((S0[3][2])-(S1[3][2])) ) + SQR( ((S0[3][3])-(S1[3][3])) ) \
		) \

/*********************************************************************/
#endif // _MTYPE_H
