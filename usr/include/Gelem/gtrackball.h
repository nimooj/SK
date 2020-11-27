#pragma once

/******************************************************
 * GTrackBall class.
 * Made By Seung-Hyun Yun. 2002. 9.13.
 * This class uses the GQuaternion class.
 * This class supports the followings
 * 1. Fixed-Axis rotation ( RotateMode = ROT_FREE, ROT_X, ROT_Y, ROT_Z )
 * 2. World ( local ) rotation ( RotateType = WROLD, LOCAL )
 * 3. Referenced Axis rotation ( RefQuat )
 ******************************************************/
class GQuaternion;

class GTrackBall
{
	// enumerator variables.
public:
	enum RotateMode { ROT_FREE = 0, ROT_X, ROT_Y, ROT_Z };
	enum RotateType { LOCAL = 0, WORLD };

	// constructors 
public:
	GTrackBall();
	GTrackBall(GTrackBall& ball);
	GTrackBall(int wx, int wy, int cx, int cy);
	GTrackBall(GQuaternion quat, int wx, int wy, int cx, int cy);

	// destructor
public:
	~GTrackBall();

	// operator overloading
public:
	GTrackBall& operator=(GTrackBall& ball);

	// internal usage
protected:
	void GetSphereCoord(GVector3f& p);

	// public interfaces
public:
	void InitBall(int wx, int wy, int cx, int cy);
	void Rotate(int sx, int sy, int ex, int ey, GQuaternion *RefQuat = NULL,
		RotateMode mode = ROT_FREE, RotateType = WORLD);

	void Rotate(GVector3f p1, GVector3f p2, GQuaternion *RefQuat = NULL,
		RotateMode mode = ROT_FREE, RotateType = WORLD);

	void GetRotMatrix(float *mat);
	void GetRotMatrix(double *mat);

	// attributes
public:
	GQuaternion quat;
	REAL size;
	int left;
	int top;
	int width;
	int height;
};


