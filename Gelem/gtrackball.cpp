#include "pch.h"
#include "../usr/include/Gelem/gelem.h"
#include <assert.h>

GTrackBall::GTrackBall()
{
	quat.SetQuat(0.0f, 0.0f, 0.0f, 1.0f);
	left = 0;
	top = 0;
	width = 100;
	height = 100;
	size = SQRT(2.0);

}
GTrackBall::GTrackBall(GTrackBall& ball)
{
	this->left = ball.left;
	this->top = ball.top;
	this->height = ball.height;
	this->size = ball.size;
	this->width = ball.width;
	this->quat = ball.quat;
}

GTrackBall::GTrackBall(int wx, int wy, int cx, int cy)
{
	quat.SetQuat(0.0f, 0.0f, 0.0f, 1.0f);
	left = wx;
	top = wy;
	width = cx;
	height = cy;
	size = SQRT(2.0);
}
GTrackBall::GTrackBall(GQuaternion quat, int wx, int wy, int cx, int cy)
{
	this->quat = quat;
	this->left = wx;
	this->top = wy;
	this->width = cx;
	this->height = cy;
	this->size = SQRT(2.0);
}
GTrackBall::~GTrackBall()
{
}
void GTrackBall::InitBall(int wx, int wy, int cx, int cy)
{
	left = wx;
	top = wy;
	width = cx;
	height = cy;
}
void GTrackBall::GetSphereCoord(GVector3f& p)
{
	REAL r = p.getNorm();
	if (r >= 1.0)
	{
		p.normalize();
	}
	else
	{
		p[2] = SQRT(1.0f - r);
	}
}
void GTrackBall::Rotate(int sx, int sy, int ex, int ey,
	GQuaternion *RefQuat, RotateMode mode, RotateType type)
{
	int cx, cy;
	GVector3f p1, p2, axis, d;
	GQuaternion q;

	cx = left + width / 2;
	cy = top + height / 2;
	p1.X = ((REAL)(sx - cx)) / ((REAL)width / 2.0f);
	p1.Y = ((REAL)(cy - sy)) / ((REAL)height / 2.0f);
	p2.X = ((REAL)(ex - cx)) / ((REAL)width / 2.0f);
	p2.Y = ((REAL)(cy - ey)) / ((REAL)height / 2.0f);
	GetSphereCoord(p1);
	GetSphereCoord(p2);

	if (RefQuat != NULL)
	{
		float mat[16];

		RefQuat->GetGLMatrixf(mat);
		GVector3f tmp;
		tmp.X = mat[0] * p1.X + mat[1] * p1.Y + mat[2] * p1.Z;
		tmp.Y = mat[4] * p1.X + mat[5] * p1.Y + mat[6] * p1.Z;
		tmp.Z = mat[8] * p1.X + mat[9] * p1.Y + mat[10] * p1.Z;
		p1 = tmp;
		tmp.X = mat[0] * p2.X + mat[1] * p2.Y + mat[2] * p2.Z;
		tmp.Y = mat[4] * p2.X + mat[5] * p2.Y + mat[6] * p2.Z;
		tmp.Z = mat[8] * p2.X + mat[9] * p2.Y + mat[10] * p2.Z;
		p2 = tmp;
	}

	switch (mode)
	{
	case ROT_FREE:
		q.Rotate(p1, p2, GQuaternion::RotateMode::ROT_FREE);
		break;
	case ROT_X:
		q.Rotate(p1, p2, GQuaternion::RotateMode::ROT_X);
		break;
	case ROT_Y:
		q.Rotate(p1, p2, GQuaternion::RotateMode::ROT_Y);
		break;
	case ROT_Z:
		q.Rotate(p1, p2, GQuaternion::RotateMode::ROT_Z);
		break;
	}
	if (type == WORLD) // rotation about global axis.
		quat = q * quat;
	else // rotation about local axis.
		quat = quat * q;
	quat.Normalize();
}

// hhs Ãß°¡, 2008.12.23
void GTrackBall::Rotate(GVector3f p1, GVector3f p2,
	GQuaternion *RefQuat, RotateMode mode, RotateType type)
{
	GVector3f axis, d;
	GQuaternion q;

	if (RefQuat != NULL)
	{
		float mat[16];

		RefQuat->GetGLMatrixf(mat);
		GVector3f tmp;
		tmp.X = mat[0] * p1.X + mat[1] * p1.Y + mat[2] * p1.Z;
		tmp.Y = mat[4] * p1.X + mat[5] * p1.Y + mat[6] * p1.Z;
		tmp.Z = mat[8] * p1.X + mat[9] * p1.Y + mat[10] * p1.Z;
		p1 = tmp;
		tmp.X = mat[0] * p2.X + mat[1] * p2.Y + mat[2] * p2.Z;
		tmp.Y = mat[4] * p2.X + mat[5] * p2.Y + mat[6] * p2.Z;
		tmp.Z = mat[8] * p2.X + mat[9] * p2.Y + mat[10] * p2.Z;
		p2 = tmp;
	}

	switch (mode)
	{
	case ROT_FREE:
		q.Rotate(p1, p2, GQuaternion::RotateMode::ROT_FREE);
		break;
	case ROT_X:
		q.Rotate(p1, p2, GQuaternion::RotateMode::ROT_X);
		break;
	case ROT_Y:
		q.Rotate(p1, p2, GQuaternion::RotateMode::ROT_Y);
		break;
	case ROT_Z:
		q.Rotate(p1, p2, GQuaternion::RotateMode::ROT_Z);
		break;
	}
	if (type == WORLD) // rotation about global axis.
		quat = q * quat;
	else // rotation about local axis.
		quat = quat * q;
	quat.Normalize();
}


void GTrackBall::GetRotMatrix(float *mat)
{
	quat.GetGLMatrixf(mat);
}

void GTrackBall::GetRotMatrix(double *mat)
{
	quat.GetGLMatrixd(mat);
}

GTrackBall& GTrackBall::operator =(GTrackBall &ball)
{
	this->left = ball.left;
	this->top = ball.top;
	this->height = ball.height;
	this->size = ball.size;
	this->width = ball.width;
	this->quat = ball.quat;
	return (*this);
}
