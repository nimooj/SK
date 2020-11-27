#pragma once

#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <cassert>
#include <stdarg.h>
#include "egmath_type.h"
#include "egmath_opt.h"
#include "egmath_util.h"
#include "egmath_cast.h"

// Ŭ���� ����
class EgVec2;
class EgVec3;
class EgVec4;
class EgVec;
class EgPos;
class EgMat;
class EgQuat;
class EgTrans;
class EgLine;
class EgPlane;

/*!
*	\class	EgVec2
*	\brief	2���� ������ ���͸� ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	01 Jan 2001
*/
class EgVec2
{
public:
	/*! \brief 2���� ������ ��ǥ �迭 */
	double m_Coords[2];

public:
	// ������ �� �Ҹ���
	EgVec2(double x = 0.0, double y = 0.0);
	EgVec2(const EgVec2 &cpy);
	~EgVec2();	

	// ��� �Լ�
	EgVec2 &Set(double x, double y);
	EgVec2 &Normalize();

	// �ɹ� ������
	EgVec2 &operator =(const EgVec2 &rhs);
	EgVec2 &operator +=(const EgVec2 &rhs);
	EgVec2 &operator -=(const EgVec2 &rhs);
	EgVec2 &operator *=(const double &s);
	EgVec2 &operator /=(const double &s);
	EgVec2 operator +() const;
	EgVec2 operator -() const;
	double &operator [](const int &idx);
	const double &operator [](const int &idx) const;

	// ���� ������
	friend EgVec2 operator +(const EgVec2 &v, const EgVec2 &w);
	friend EgVec2 operator -(const EgVec2 &v, const EgVec2 &w);
	friend EgVec2 operator *(const EgVec2 &v, double s);
	friend EgVec2 operator *(double s, const EgVec2 &v);
	friend double operator *(const EgVec2 &v, const EgVec2 &w);
	friend EgVec2 operator /(const EgVec2 &v, double s);
	friend bool operator ==(const EgVec2 &v, const EgVec2 &w);
	friend bool operator !=(const EgVec2 &v, const EgVec2 &w);
	friend std::ostream &operator <<(std::ostream &os, const EgVec2 &v);
	friend std::istream &operator >>(std::istream &is, EgVec2 &v);
};

/*!
*	\class EgVec3
*	\brief 3���� ���͸� ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	01 Jan 2001
*/
class EgVec3
{
public:
	/*! \brief 3���� ������ ��ǥ �迭 */
	double m_Coords[3];

public:
	// ������ �� �Ҹ���
	EgVec3(double x = 0.0, double y = 0.0, double z = 0.0);
	EgVec3(const EgVec3 &cpy);
	~EgVec3();	

	// ��� �Լ�
	EgVec3 &Set(double x, double y, double z);
	EgVec3 &Normalize();

	// ��� ������
	EgVec3 &operator =(const EgVec3 &rhs);
	EgVec3 &operator +=(const EgVec3 &rhs);
	EgVec3 &operator -=(const EgVec3 &rhs);
	EgVec3 &operator *=(const double &s);
	EgVec3 &operator /=(const double &s);
	EgVec3 &operator ^=(const EgVec3 &rhs);
	EgVec3 operator +() const;
	EgVec3 operator -() const;
	double &operator [](const int &idx);
	const double &operator [](const int &idx) const;

	// �̸�� ������
	friend EgVec3 operator +(const EgVec3 &v, const EgVec3 &w);
	friend EgPos operator +(const EgVec3 &v, const EgPos &p);
	friend EgVec3 operator -(const EgVec3 &v, const EgVec3 &w);
	friend EgVec3 operator *(const EgVec3 &v, const double &s);
	friend EgVec3 operator *(const double &s, const EgVec3 &v);
	friend double operator *(const EgVec3 &v, const EgVec3 &w);
	friend EgVec3 operator /(const EgVec3 &v, const double &s);
	friend EgVec3 operator ^(const EgVec3 &v, const EgVec3 &w);
	friend bool operator ==(const EgVec3 &v, const EgVec3 &w);
	friend bool operator !=(const EgVec3 &v, const EgVec3 &w);
	friend std::ostream &operator <<(std::ostream &os, const EgVec3 &v);
	friend std::istream &operator >>(std::istream &is, EgVec3 &v);
};

/*!
*	\class	EgVec4
*	\brief	4���� ���͸� ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	01 Jan 2001
*/
class EgVec4
{
public:
	/*! \brief 4���� ������ ��ǥ �迭 */
	double m_Coords[4];

public:
	// ������ �� �Ҹ���
	EgVec4(double x = 0.0, double y = 0.0, double z = 0.0, double w = 0.0);
	EgVec4(const EgVec4 &cpy);
	~EgVec4();	

	// ��� �Լ�
	EgVec4 &Set(double x, double y, double z, double w);
	EgVec4 &Normalize();

	// ��� ������
	EgVec4 &operator =(const EgVec4 &rhs);
	EgVec4 &operator +=(const EgVec4 &rhs);
	EgVec4 &operator -=(const EgVec4 &rhs);
	EgVec4 &operator *=(const double &s);
	EgVec4 &operator /=(const double &s);
	EgVec4 operator +() const;
	EgVec4 operator -() const;
	double &operator [](const int &idx);
	const double &operator [](const int &idx) const;

	// ���� ������
	friend EgVec4 operator +(const EgVec4 &v, const EgVec4 &w);
	friend EgVec4 operator -(const EgVec4 &v, const EgVec4 &w);
	friend EgVec4 operator *(const EgVec4 &v, const double &s);
	friend EgVec4 operator *(const double &s, const EgVec4 &v);
	friend double operator *(const EgVec4 &v, const EgVec4 &w);
	friend EgVec4 operator /(const EgVec4 &v, const double &s);
	friend bool operator ==(const EgVec4 &v, const EgVec4 &w);
	friend bool operator !=(const EgVec4 &v, const EgVec4 &w);
	friend std::ostream &operator <<(std::ostream &os, const EgVec4 &v);
	friend std::istream &operator >>(std::istream &is, EgVec4 &v);
};

/*!
*	\class	EgVec
*	\brief	n ���� ���͸� ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	01 Jan 2001
*/
class EgVec
{
public:
	/*! \brief ������ ��ǥ �迭 */
	std::vector<double> m_Coords;

public:
	// ������ �� �Ҹ���
	EgVec(int dim = 3);
	EgVec(int dim, double x, ...);
	EgVec(int dim, int x, ...);
	EgVec(const EgVec &cpy);
	~EgVec();
	
	// ��� �Լ�
	EgVec &Set(double x, ...);
	EgVec &Set(int x, ...);
	EgVec &Set(double *Coords);
	EgVec &Normalize();
	EgVec &SetZeros();
	int GetDim() const;

	// ��� ������
	EgVec &operator =(const EgVec &rhs);
	EgVec &operator +=(const EgVec &rhs);
	EgVec &operator -=(const EgVec &rhs);
	EgVec &operator *=(const double &s);
	EgVec &operator /=(const double &s);
	EgVec operator +() const;
	EgVec operator -() const;
	double &operator [](const int &idx);
	const double &operator [](const int &idx) const;

	// ���� ������
	friend EgVec operator +(const EgVec &v, const EgVec &w);
	friend EgVec operator -(const EgVec &v, const EgVec &w);
	friend double operator *(const EgVec &v, const EgVec &w);
	friend EgVec operator *(const double &s, const EgVec &v);
	friend EgVec operator *(const EgVec &v, const double &s);
	friend EgVec operator /(const EgVec &v, const double &s);
	friend bool operator >(const EgVec &v, const EgVec &w);
	friend bool operator >=(const EgVec &v, const EgVec &w);
	friend bool operator <(const EgVec &v, const EgVec &w);
	friend bool operator <=(const EgVec &v, const EgVec &w);
	friend bool operator ==(const EgVec &v, const EgVec &w);
	friend bool operator !=(const EgVec &v, const EgVec &w);
	friend std::ostream &operator <<(std::ostream &os, const EgVec &v);
	friend std::istream &operator >>(std::istream &is, EgVec &v);
};

/*!
*	\class	EgPos
*	\brief	3���� ��ġ(position)�� ��Ÿ���� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	01 Jan 2001
*/
class EgPos
{
public:
	/*! \brief 3���� ��ǥ �迭 */
	double m_Coords[3];

public:
	// ������ �� �Ҹ���
	EgPos(double x = 0.0, double y = 0.0, double z = 0.0);
	EgPos(const EgPos &cpy);
	~EgPos();

	// ��� �Լ�
	EgPos &Set(double x, double y, double z);
	
	// ��� ������
	EgPos &operator =(const EgPos &rhs);
	EgPos &operator +=(const EgVec3 &v);
	EgPos &operator -=(const EgVec3 &v);
	double &operator [](const int &idx);
	const double &operator [](const int &idx) const;

	// ���� ������
	friend EgVec3 operator -(const EgPos &p, const EgPos &q);
	friend EgPos operator -(const EgPos &p, const EgVec3 &v);
	friend EgPos operator +(const EgPos &p, const EgVec3 &v);
	friend EgPos operator +(const EgVec3 &v, const EgPos &p);
	friend bool operator ==(const EgPos &p, const EgPos &q);
	friend bool operator !=(const EgPos &p, const EgPos &q);
	friend std::ostream &operator <<(std::ostream &os, const EgPos &p);
	friend std::istream &operator >>(std::istream &is, EgPos &p);
};

/*!
*	\class	EgMat
*	\brief	\a m �� \a n ���� ũ�⸦ ���� ����� ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	01 Jan 2001
*/
class EgMat
{
public:
	/*! \brief ����� ���� ���� */
	int m_nRow;

	/*! \brief ����� ���� ���� */
	int m_nCol;

	/*! \brief ����� ���Ҹ� �����ϴ� �迭 */
	std::vector<double> m_Data;	
	
public:
	// ������ �� �Ҹ���
	EgMat(int r = 4, int c = 4, double *elem = NULL);
	EgMat(const EgMat &cpy);
	~EgMat();	

	// ��� �Լ�
	EgMat &SetTranspose();
	EgMat &SetIdentity();
	EgMat &SetZeros();
	EgMat &SetRowVec(const int idx, const EgVec &v);
	EgMat &SetColVec(const int idx, const EgVec &v);
	EgMat &ExchangeRows(const int idx0, const int idx1);
	EgMat &ExchangeCols(const int idx0, const int idx1);
	EgVec GetRowVec(const int idx) const;
	EgVec GetColVec(const int idx) const;
	int	GetRowNum() const;
	int GetColNum() const;
	bool IsSquare() const;

	// ��� ������
	EgMat &operator =(const EgMat &rhs);
	EgMat &operator +=(const EgMat &rhs);
	EgMat &operator -=(const EgMat &rhs);
	EgMat &operator *=(const EgMat &rhs);
	EgMat &operator *=(const double &s);
	EgMat &operator /=(const double &s);
	EgMat operator +() const;
	EgMat operator -() const;
	double *operator [](const int idx);
	const double *operator [](const int idx) const;

	// ���� ������
	friend EgMat operator +(const EgMat &A, const EgMat &B);
	friend EgMat operator -(const EgMat &A, const EgMat &B);
	friend EgMat operator *(const EgMat &A, const EgMat &B);
	friend EgMat operator *(const EgMat &M, const double &s);
	friend EgMat operator *(const double &s, const EgMat &M);
	friend EgVec operator *(const EgMat &M, const EgVec &v);
	friend EgMat operator *(const EgVec &v, const EgMat &M);
	friend EgMat operator /(const EgMat &A, const double &s);
	friend bool operator ==(const EgMat &A, const EgMat &B);
	friend bool operator !=(const EgMat &A, const EgMat &B);	
	friend std::ostream &operator <<(std::ostream &os, const EgMat &m);
};

/*!
*	\class	EgQuat
*	\brief	���ʹϿ� (q = W + iX + jY + kZ)�� ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	01 Jan 2001
*/
enum TypeEuler
{
	EULER_XYZ = 0,
	EULER_ZYX = 1,
};
enum TypeArcBall
{
	ARCBALL_FREE = 0,
	ARCBALL_X,
	ARCBALL_Y,
	ARCBALL_Z
};
class EgQuat
{
public:
	/*! \brief ���ʹϿ��� �Ǽ��� */
	double m_W;

	/*! \brief ���ʹϿ��� i ����� */
	double m_X;

	/*! \brief ���ʹϿ��� j ����� */
	double m_Y;

	/*! \brief ���ʹϿ��� k ����� */
	double m_Z;

public:
	// ������ �� �Ҹ���
	EgQuat(double w = 1.0, double x = 0.0, double y = 0.0, double z = 0.0);
	EgQuat(const EgQuat &cpy);
	EgQuat(const double *q);
	EgQuat(EgVec3 axis, double theta, bool radian = false);
	EgQuat(double theta1, double theta2, double theta3, TypeEuler eulerType = EULER_XYZ);
	EgQuat(EgVec3 x_axis, EgVec3 y_axis);
	~EgQuat();	

	// ��� �Լ�
	EgQuat &Set(double w, double x, double y, double z);
	EgQuat &Set(double *q, bool invOrder = false);
	EgQuat &SetFromAngleAxis(const double theta, EgVec3 axis, bool radian = false);
	EgQuat &SetFromArcBall(EgVec3 p, EgVec3 q, TypeArcBall arcType = ARCBALL_FREE);
	EgQuat &SetFromEulerAngle(double theta1, double theta2, double theta3, TypeEuler eulerType = EULER_XYZ);
	EgQuat &SetFromFrameXY(const EgVec3 &x_axis, const EgVec3 &y_axis);
	EgQuat &SetFromFrameYZ(const EgVec3 &y_axis, const EgVec3 &z_axis);
	EgQuat &SetFromFrameZX(const EgVec3 &z_axis, const EgVec3 &x_axis);
	EgQuat &SetFromMatrix(double *mat, bool isGL);
	EgQuat &SetIdentity();
	EgQuat &SetInverse();
	EgQuat &SetConjugate();
	EgQuat &SetNegate();
	EgQuat &Normalize();
	void	GetAngleAxis(EgVec3 &axis, double &angle, bool radian = false) const;
	void	GetEulerAngle(double &theta1, double &theta2, double &theta3, bool radian = false, TypeEuler eulerType = EULER_XYZ);
	bool	IsUnitQuater() const;
	bool	IsIdentity() const;
	double	W() const;
	double	X() const;
	double	Y() const;
	double	Z() const;	

	// ��� ������
	EgQuat &operator =(const EgQuat &rhs);
	EgQuat &operator +=(const EgQuat &rhs);
	EgQuat &operator -=(const EgQuat &rhs);
	EgQuat &operator *=(const EgQuat &rhs);
	EgQuat &operator *=(const double s);
	EgQuat &operator /=(const double s);
	EgQuat operator +() const;
	EgQuat operator -() const;

	// ���� ������
	friend EgQuat operator +(const EgQuat &q1, const EgQuat &q2);
	friend EgQuat operator -(const EgQuat &q1, const EgQuat &q2);
	friend EgQuat operator *(const EgQuat &q1, const EgQuat &q2);
	friend EgPos operator *(const EgQuat &q, const EgPos &p);
	friend EgVec3 operator *(const EgQuat &q, const EgVec3 &v);
	friend EgQuat operator *(const EgQuat &q, const double &s);
	friend EgQuat operator *(const double &s, const EgQuat &q);
	friend EgQuat operator /(const EgQuat &q1, const double s);
	friend bool operator ==(const EgQuat &q1, const EgQuat &q2);
	friend bool operator !=(const EgQuat &q1, const EgQuat &q2);
	friend std::ostream &operator <<(std::ostream &os, const EgQuat &q);
	friend std::istream &operator >>(std::istream &is, EgQuat &q);
};

/*!
*	\class	EgLine
*	\brief	3���� �������� ������ ��Ÿ���� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	01 Jan 2001
*/
class EgLine
{
public:
	/*! \brief ���� ���� �� �� */
	EgPos m_Pos;

	/*! \brief ������ ���� ���� l(t) = m_Pos + m_Dir * t */
	EgVec3 m_Dir;

public:
	// ������ �� �Ҹ���
	EgLine(EgPos p = EgPos(0, 0, 0), EgVec3 v = EgVec3(0, 0, 0));
	EgLine(EgPos p, EgPos q);
	EgLine(EgLine &cpy);
	~EgLine();	

	// ��� �Լ�
	EgPos Eval(double t);
	bool IsOnLine(EgPos &q);	

	// ��� ������
	EgLine &operator =(const EgLine &rhs);

	// ���� ������
	friend bool operator ==(EgLine &lhs, EgLine &rhs);
	friend bool operator !=(EgLine &lhs, EgLine &rhs);
	friend std::ostream &operator <<(std::ostream &os, const EgLine &l);
	friend EgLine operator *(EgTrans &X, EgLine &l);
};

/*!
*	\class	EgPlane
*	\brief	����� ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	01 Jan 2001
*/
class EgPlane
{
public:
	/*! \brief ����� ������(ax + by + cz + d = 0)�� 4���� ������ǥ�� ǥ�� (a, b, c, d) */
	EgVec4 m_Coords;

public:
	// ������ �� �Ҹ���
	EgPlane();
	EgPlane(const EgVec3 &_n, const EgPos &_p);
	EgPlane(const EgPos &p1, const EgPos &p2, const EgPos &p3);
	EgPlane(const EgVec3 &_n, const double &_d);
	EgPlane(double a, double b, double c, double d);
	EgPlane(const EgPlane &cpy);
	~EgPlane();

	// ��� �Լ�
	double Eval(EgPos p);
	EgVec3 N(bool bNormalize = false);
	double d();
	bool IsOnPlane(EgPos &p);
	bool IsAbovePlane(EgPos &p);
	bool IsBelowPlane(EgPos &p);
	EgPlane GetReversePlane();

	// ��� ������
	EgPlane &operator =(const EgPlane &rhs);
	
	// ���� ������
	friend bool operator ==(const EgPlane &lhs, const EgPlane &rhs);
	friend bool operator !=(const EgPlane &lhs, const EgPlane &rhs);
	friend std::ostream &operator <<(std::ostream &os, const EgPlane &pi);
};

/*!
*	\class EgTrans
*	\brief ��ü��ȯ(rigid-body transformation)�� ��Ÿ���� Ŭ����.
*
*	\author ������(shyun@dongguk.edu)
*	\date 2001�� 1�� 1��.
*/
class EgTrans
{
public:
	/*! \brief 3���� �̵���ȯ */
	EgVec3 m_T;

	/*! \brief 3���� ȸ����ȯ */
	EgQuat m_Q;

public:
	// ������ �� �Ҹ���
	EgTrans(const EgVec3 &t = EgVec3(0.0, 0.0, 0.0), const EgQuat &q = EgQuat(1.0, 0.0, 0.0, 0.0));
	EgTrans(double *mat, bool isGL);
	EgTrans(const EgTrans &cpy);
	~EgTrans();

	// ��� �Լ�
	void SetNegate();
	void SetTrans(const EgVec3 &t);
	void SetRot(const EgQuat &q);

	// ��� ������
	EgTrans &operator =(const EgTrans &rhs);
	EgTrans &operator *=(const EgTrans &rhs);
	EgTrans &operator *=(const double &s);

	// ���� ������
	friend EgTrans operator *(const EgTrans &X1, const EgTrans &X2);
	friend bool operator ==(const EgTrans &X1, const EgTrans &X2);
	friend bool operator !=(const EgTrans &X1, const EgTrans &X2);
	friend std::ostream &operator <<(std::ostream &os, const EgTrans &X);
	friend EgTrans operator *(const EgTrans &X, const double &s);
	friend EgTrans operator *(const double &s, const EgTrans &X);
	friend EgPos operator *(const EgTrans &X, const EgPos &p);
	friend EgLine operator *(const EgTrans &X, const EgLine &l);
	friend EgPlane operator *(EgTrans &X, EgPlane &pi);
	friend EgVec3 operator *(const EgTrans &X, const EgVec3 &v);
};


