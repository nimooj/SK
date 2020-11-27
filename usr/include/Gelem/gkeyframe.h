#pragma once

class GTrackBall;
class GQuaternion;
class GVector3f;

class GKeyFrame
{

	// constructor and destructor
public:
	GKeyFrame();
	GKeyFrame(GKeyFrame& keyframe);
	GKeyFrame(REAL param, GTrackBall ball);
	~GKeyFrame();

	// set memthod.
public:
	void SetQuat(GQuaternion quat);
	void SetParam(float param);
	void SetTrackBallSize(int w, int h);
	void SetCenter(GVector3f cnt);
	void SetCenter(float x, float y, float z);
	void SetSclFactor(float x, float y, float z);

	// get method.
public:
	REAL		GetParam();
	GVector3f	GetCenter();
	GVector3f	GetSclFactor();
	GQuaternion GetQuat();

	// operator overloading.
public:
	void Rotate(int sx, int sy, int ex, int ey, GQuaternion *pQuat = NULL);
	GKeyFrame& operator=(GKeyFrame &);

	// attributes
private:
	REAL		m_Param;
	GVector3f	m_Center; // origin position.
	GVector3f	m_SclFactor;
	GTrackBall	m_TrackBall; // rotation.
};

