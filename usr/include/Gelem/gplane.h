#pragma once

class GPlane 
{
public:
	GPlane();
	GPlane(GVector3f n, REAL d);
	GPlane(GVector3f n, GVector3f o);
	GPlane(GVector3f a, GVector3f b, GVector3f c);
	GPlane(GPlane* pPln);
	GPlane(GPlane &Pln);
	~GPlane();

	
	GPlane& operator = (GPlane& Pln);


	void SetNormal(GVector3f n);
	void SetConstant(REAL d);

	void MakePlane(GVector3f n, GVector3f pt);
	void MakePlane(GVector3f a, GVector3f b, GVector3f c);

	bool IsOnPlane(GVector3f pt);
	bool IsOnPlane5(GVector3f pt);

	bool IsAbovePlane(GVector3f pt);
	bool IsBelowPlane(GVector3f pt);

	GVector3f GetNormal();
	REAL GetConstant();
	REAL GetSignedDistance(GVector3f pt);
	GVector3f GetProjectionPt(GVector3f r, GVector3f ray);
	GVector3f GetOrigin();


	GVector3f n;
	REAL d;
};
