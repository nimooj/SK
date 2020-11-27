#pragma once

typedef struct TRIANGLE {
	short p1, p2, p3;
} TRIANGLE;

typedef struct EDGE {
	short p1, p2;
} EDGE;

class GVector3f 
{
public:
	REAL X, Y, Z;

	GVector3f(); // default constructor
	GVector3f(const GVector3f& v); // copy constructor
	GVector3f(REAL x, REAL y, REAL z); 
	GVector3f(REAL v[]);
	~GVector3f();


	int operator == (GVector3f& v);
	int operator != (GVector3f& v);

	GVector3f& operator += (const GVector3f& v);
	GVector3f& operator -= (const GVector3f& v);
	GVector3f& operator *= (REAL scale);
	GVector3f& operator ^= (const GVector3f& v);

	// cross product
	GVector3f operator ^ (const GVector3f &v) const;
	// dot product
	REAL operator * (GVector3f &v) const;

	GVector3f& operator = (const GVector3f &v);
	GVector3f operator + (const GVector3f &v);
	GVector3f operator - (const GVector3f &v);
	GVector3f operator * (REAL scale);
	GVector3f operator / (REAL scale);


	// index operartor overload
	REAL& operator[] (int idx);


	void normalize();
	REAL getNorm();
	REAL getAngle(GVector3f v);
	int makeEmpty(void);
	void setVector(REAL x, REAL y, REAL z);
	void setVector(REAL v[]);
};
