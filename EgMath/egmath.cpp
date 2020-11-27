#include "..\usr\include\EgMath\egmath.h"

/********************/
/* EgVec2 클래스 구현 */
/********************/
/*!
*	\brief	생성자
*	\note	마지막 수정일: 2020-07-03
*
*	\param	x[in]	벡터의 x 좌표
*	\param	y[in]	벡터의 y 좌표
*/
EgVec2::EgVec2(double x, double y)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
}

/*!
*	\brief	복사 생성자
*	\note	마지막 수정일: 2020-07-03
*
*	\param	cpy[in]	복사될 객체
*/
EgVec2::EgVec2(const EgVec2 &cpy)
{
	m_Coords[0] = cpy.m_Coords[0];
	m_Coords[1] = cpy.m_Coords[1];
}

/*!
*	\brief	소멸자
*	\note	마지막 수정일: 2020-07-03
*/
EgVec2::~EgVec2()
{
}

/*!
*	\brief	벡터의 좌표를 설정한다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	x[in]	벡터의 x 좌표
*	\param	y[in]	벡터의 y 좌표
*
*	\return 설정된 자신을 반환한다.
*/
EgVec2 &EgVec2::Set(double x, double y)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
	return *this;
}

/*!
*	\brief	벡터를 단위벡터로 정규화한다.
*	\note	마지막 수정일: 2020-07-03
*
*	\return 정규화된 자신을 반환한다.
*/
EgVec2 &EgVec2::Normalize()
{
	double len = norm(*this);
	if (EQ_ZERO(len, MTYPE_EPS))
	{
		printf("[Error]: EgVec2::Normalize()...\n");
		m_Coords[0] = m_Coords[1] = 0.0;
	}
	else
	{
		m_Coords[0] /= len;
		m_Coords[1] /= len;
	}
	return *this;
}

/*!
*	\brief	대입 연산자
*	\note	마지막 수정일: 2020-07-03
*
*	\param	rhs[in]	대입될 벡터
*
*	\return 대입된 자신을 반환한다.
*/
EgVec2 &EgVec2::operator =(const EgVec2 &rhs)
{
	m_Coords[0] = rhs.m_Coords[0];
	m_Coords[1] = rhs.m_Coords[1];
	return *this;
}

/*!
*	\brief	벡터를 더한다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	rhs[in]	더할 벡터
*
*	\return 변경된 자신을 반환한다.
*/
EgVec2 &EgVec2::operator +=(const EgVec2 &rhs)
{
	m_Coords[0] += rhs.m_Coords[0];
	m_Coords[1] += rhs.m_Coords[1];
	return *this;
}

/*!
*	\brief	벡터를 뺀다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	rhs[in]	뺄 벡터
*
*	\return 변경된 자신을 반환한다.
*/
EgVec2 &EgVec2::operator -=(const EgVec2 &rhs)
{
	m_Coords[0] -= rhs.m_Coords[0];
	m_Coords[1] -= rhs.m_Coords[1];
	return *this;
}

/*!
*	\brief	벡터를 상수배 한다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	s[in]	상수
*
*	\return 변경된 자신을 반환한다.
*/
EgVec2 &EgVec2::operator *=(const double &s)
{
	m_Coords[0] *= s;
	m_Coords[1] *= s;
	return *this;
}

/*!
*	\brief	벡터를 상수로 나눈다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	s[in]	상수
*
*	\return 변경된 자신을 반환한다.
*/
EgVec2 &EgVec2::operator /=(const double &s)
{
	m_Coords[0] /= s;
	m_Coords[1] /= s;
	return *this;
}

/*!
*	\brief	단항 연산자
*	\note	마지막 수정일: 2020-07-03
*
*	\return 동일 부호를 갖는 객체를 반환한다.
*/
EgVec2 EgVec2::operator +() const
{
	return *this;
}

/*!
*	\brief	단항 연산자
*	\note	마지막 수정일: 2020-07-03
*
*	\return 반대 부호를 갖는 객체를 반환한다.
*/
EgVec2 EgVec2::operator -() const
{
	return *this * -1;
}

/*!
*	\brief	인덱스 연산자([])
*	\note	마지막 수정일: 2020-07-03
*
*	\param	idx[in]	좌표의 인덱스
*
*	\return 벡터의 \a idx번째 원소의 레퍼런스를 반환한다.
*/
double &EgVec2::operator [](const int &idx)
{
	assert(idx >= 0 && idx < 2);
	return m_Coords[idx];
}

/*!
*	\brief	상수 객체에 대한 인덱스 연산자([])
*	\note	마지막 수정일: 2020-07-03
*
*	\param	idx[in]	원소의 인덱스
*
*	\return 벡터의 \a idx번째 원소의 레퍼런스를 반환한다.
*/
const double &EgVec2::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < 2);
	return m_Coords[idx];
}

/*!
*	\brief	두 벡터의 합을 구한다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터의 합(v + w)을 반환한다.
*/
EgVec2 operator +(const EgVec2 &v, const EgVec2 &w)
{
	EgVec2 ret(v);
	ret += w;
	return ret;
}

/*!
*	\brief	두 벡터의 차를 구한다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터의 차(v - w)를 반환한다.
*/
EgVec2 operator -(const EgVec2 &v, const EgVec2 &w)
{
	EgVec2 ret(v);
	ret -= w;
	return ret;
}

/*!
*	\brief	두 벡터의 내적을 구한다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터의 내적을 반환한다.
*/
double operator *(const EgVec2 &v, const EgVec2 &w)
{
	return (v.m_Coords[0] * w.m_Coords[0] + v.m_Coords[1] * w.m_Coords[1]);
}

/*!
*	\brief	벡터를 상수배 한다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	v[in]	벡터
*	\param	s[in]	상수
*
*	\return 벡터의 상수배를 반환한다.
*/
EgVec2 operator *(const EgVec2 &v, double s)
{
	EgVec2 ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	벡터를 상수배 한다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	s[in]	상수
*	\param	v[in]	벡터
*
*	\return 벡터의 상수배를 반환한다.
*/
EgVec2 operator *(double s, const EgVec2 &v)
{
	EgVec2 ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	벡터를 상수로 나눈다
*	\note	마지막 수정일: 2020-07-03
*
*	\param	v[in]	벡터
*	\param	s[in]	상수
*
*	\return 상수로 나누어진 벡터를 반환한다.
*/
EgVec2 operator /(const EgVec2 &v, double s)
{
	EgVec2 ret(v);
	ret /= s;
	return ret;
}

/*!
*	\brief	두 벡터가 같은지 검사한다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터가 같으면 true, 다르면 false를 반한한다.
*/
bool operator ==(const EgVec2 &v, const EgVec2 &w)
{
	return (EQ(v.m_Coords[0], w.m_Coords[0], MTYPE_EPS) &&
			EQ(v.m_Coords[1], w.m_Coords[1], MTYPE_EPS));
}

/*!
*	\brief	두 벡터가 다른지 검사한다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터가 다르면 true, 같으면 false를 반환한다.
*/
bool operator !=(const EgVec2 &v, const EgVec2 &w)
{
	return (v == w) ? false : true;
}

/*!
*	\brief	벡터를 출력한다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	os[in]	출력 스트림
*	\param	v[in]	출력 벡터
*
*	\return \a v가 출력된 스트림을 반환한다.
*/
std::ostream &operator <<(std::ostream &os, const EgVec2 &v)
{
	os << "(" << std::setw(7) << v.m_Coords[0] << ", " << std::setw(7) << v.m_Coords[1] << ")";
	return os;
}

/*!
*	\brief	벡터의 좌표를 입력 받는다.
*	\note	마지막 수정일: 2020-07-03
*
*	\param	is[in]	입력 스트림
*	\param	v[out]	대상 벡터
*
*	\return 입력 스트림을 반환한다.
*/
std::istream &operator >>(std::istream &is, EgVec2 &v)
{
	is >> v.m_Coords[0] >> v.m_Coords[1];
	return is;
}

/********************/
/* EgVec3 클래스 구현 */
/********************/
/*!
*	\brief	생성자
*
*	\param	x[in]	벡터의 x 좌표
*	\param	y[in]	벡터의 y 좌표
*	\param	z[in]	벡터의 z 좌표
*/
EgVec3::EgVec3(double x, double y, double z)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
	m_Coords[2] = z;
}

/*!
*	\brief	복사 생성자
*
*	\param	cpy[in]	복사될 객체
*/
EgVec3::EgVec3(const EgVec3 &cpy)
{
	m_Coords[0] = cpy.m_Coords[0];
	m_Coords[1] = cpy.m_Coords[1];
	m_Coords[2] = cpy.m_Coords[2];
}

/*!
*	\brief	소멸자
*/
EgVec3::~EgVec3()
{
}

/*!
*	\brief	벡터의 좌표를 설정한다.
*
*	\param	x[in]	설정할 x 좌표
*	\param	y[in]	설정할 y 좌표
*	\param	z[in]	설정할 z 좌표
*/
EgVec3 &EgVec3::Set(double x, double y, double z)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
	m_Coords[2] = z;
	return *this;
}

/*!
*	\brief	벡터를 정규화한다.
*
*	\return 정규화된 자신을 반환한다.
*/
EgVec3 &EgVec3::Normalize()
{
	double len = norm(*this);
	if (EQ_ZERO(len, MTYPE_EPS))
	{
		printf("[Error]: EgVec3::Normalize()...\n");
		m_Coords[0] = m_Coords[1] = m_Coords[2] = 0.0;
	}
	else
	{
		m_Coords[0] /= len;
		m_Coords[1] /= len;
		m_Coords[2] /= len;
	}
	return *this;
}

/*!
*	\brief	대입 연산자
*
*	\param	rhs[in]	대입될 객체
*
*	\return 대입된 자신을 반환한다.
*/
EgVec3 &EgVec3::operator =(const EgVec3 &rhs)
{
	m_Coords[0] = rhs.m_Coords[0];
	m_Coords[1] = rhs.m_Coords[1];
	m_Coords[2] = rhs.m_Coords[2];
	return *this;
}

/*!	
*	\brief	벡터를 더한다.
*
*	\param	rhs[in]	더할 벡터
*
*	\return 변경된 자신을 반환한다.
*/
EgVec3 &EgVec3::operator +=(const EgVec3 &rhs)
{
	m_Coords[0] += rhs.m_Coords[0];
	m_Coords[1] += rhs.m_Coords[1];
	m_Coords[2] += rhs.m_Coords[2];
	return *this;
}

/*!
*	\brief	벡터를 뺀다.
*
*	\param	rhs[in]	뺄 벡터
*
*	\return 변경된 자신을 반환한다.
*/
EgVec3 &EgVec3::operator -=(const EgVec3 &rhs)
{
	m_Coords[0] -= rhs.m_Coords[0];
	m_Coords[1] -= rhs.m_Coords[1];
	m_Coords[2] -= rhs.m_Coords[2];
	return *this;
}

/*!
*	\brief	벡터를 상수배 한다.
*
*	\param	s[in]	상수
*
*	\return 변경된 자신을 반환한다.
*/
EgVec3 &EgVec3::operator *=(const double &s)
{
	m_Coords[0] *= s;
	m_Coords[1] *= s;
	m_Coords[2] *= s;
	return *this;
}

/*!
*	\brief	벡터를 상수로 나눈다.
*
*	\param	s[in]	나눌 상수
*
*	\return 변경된 자신을 반환한다.
*/
EgVec3 &EgVec3::operator /=(const double &s)
{
	m_Coords[0] /= s;
	m_Coords[1] /= s;
	m_Coords[2] /= s;
	return *this;
}

/*!
*	\brief	벡터를 외적한다.
*
*	\param	rhs[in]	외적할 벡터
*
*	\return 변경된 자신을 반환한다.
*/
EgVec3 &EgVec3::operator ^=(const EgVec3 &rhs)
{
	double x = m_Coords[0], y = m_Coords[1], z = m_Coords[2];
	m_Coords[0] = y * rhs.m_Coords[2] - z * rhs.m_Coords[1];
	m_Coords[1] = z * rhs.m_Coords[0] - x * rhs.m_Coords[2];
	m_Coords[2] = x * rhs.m_Coords[1] - y * rhs.m_Coords[0];
	return *this;
}

/*!
*	\brief	단항 연산자
*
*	\return 동일부호를 갖는 객체를 반환한다.
*/
EgVec3 EgVec3::operator +() const
{
	return *this;
}

/*!
*	\brief	단항 연산자
*
*	\return 반대부호를 갖는 객체를 반환한다.
*/
EgVec3 EgVec3::operator -() const
{
	return *this * -1;
}

/*!
*	\brief	두 벡터를 더한다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터의 합을 반환한다.
*/
EgVec3 operator +(const EgVec3 &v, const EgVec3 &w)
{
	return EgVec3(
		v.m_Coords[0] + w.m_Coords[0], 
		v.m_Coords[1] + w.m_Coords[1], 
		v.m_Coords[2] + w.m_Coords[2]);
}

/*!
*	\brief	두 벡터를 뺀다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터의 차를 반환한다.
*/
EgVec3 operator -(const EgVec3 &v, const EgVec3 &w)
{
	return EgVec3(
		v.m_Coords[0] - w.m_Coords[0], 
		v.m_Coords[1] - w.m_Coords[1], 
		v.m_Coords[2] - w.m_Coords[2]);
}

/*!
*	\brief	두 벡터의 내적을 구한다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터의 내적 결과를 반환한다.
*/
double operator *(const EgVec3 &v, const EgVec3 &w)
{
	return (v.m_Coords[0] * w.m_Coords[0] + 
			v.m_Coords[1] * w.m_Coords[1] + 
			v.m_Coords[2] * w.m_Coords[2]);
}

/*!
*	\brief	벡터를 상수배 한다.
*
*	\param	v[in]	벡터
*	\param	s[in]	상수
*
*	\return 벡터의 상수배를 반환한다.
*/
EgVec3 operator *(const EgVec3 &v, const double &s)
{
	EgVec3 ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	벡터를 상수배 한다.
*
*	\param	s[in]	상수
*	\param	v[in]	벡터
*
*	\return 벡터의 상수배를 반환한다.
*/
EgVec3 operator *(const double &s, const EgVec3 &v)
{
	EgVec3 ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	벡터를 상수로 나눈다.
*
*	\param	s[in]	상수
*
*	\return 상수로 나누어진 벡터를 반환한다.
*/
EgVec3 operator /(const EgVec3 &v, const double &s)
{
	EgVec3 ret(v);
	ret /= s;
	return ret;
}

/*!
*	\brief	두 벡터의 외적 벡터를 구한다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 외적 벡터 (v X w)를 반환한다.
*/
EgVec3 operator ^(const EgVec3 &v, const EgVec3 &w)
{
	return EgVec3(
		v.m_Coords[1] * w.m_Coords[2] -	v.m_Coords[2] * w.m_Coords[1], 
		v.m_Coords[2] * w.m_Coords[0] - v.m_Coords[0] * w.m_Coords[2], 
		v.m_Coords[0] * w.m_Coords[1] - v.m_Coords[1] * w.m_Coords[0]);
}

/*!
*	\brief	두 벡터가 같은지 조사한다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터가 같으면 true, 다르면 false를 반환한다.
*/
bool operator ==(const EgVec3 &v, const EgVec3 &w)
{
	return (EQ(v.m_Coords[0], w.m_Coords[0], MTYPE_EPS) && 
			EQ(v.m_Coords[1], w.m_Coords[1], MTYPE_EPS) && 
			EQ(v.m_Coords[2], w.m_Coords[2], MTYPE_EPS));
}

/*!
*	\brief	두 벡터가 다른지 조사한다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터가 다르면 ture, 같으면 false를 반환한다.
*/
bool operator !=(const EgVec3 &v, const EgVec3 &w)
{
	return (v == w) ? false : true;
}

/*!
*	\brief	인덱스 연산자([])
*
*	\param	idx[in]	인덱스
*
*	\return 벡터의 \a idx번째 원소를 반환한다.
*/
double &EgVec3::operator [](const int &idx)
{
	assert(idx >= 0 && idx < 3);
	return m_Coords[idx];
}

/*!
*	\brief	상수객체에 대한 인덱스 연산자([])
*
*	\param	idx[in]	인덱스
*
*	\return 벡터의 \a idx번째 원소를 반환한다.
*/
const double &EgVec3::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < 3);
	return m_Coords[idx];
}

/*!
*	\brief	출력 연산자(<<)
*
*	\param	os[out]		출력 스트림
*	\param	v[in]		출력할 벡터
*
*	\return \a v가 출력된 스트림을 반환한다.
*/
std::ostream &operator <<(std::ostream &os, const EgVec3 &v)
{
	os << "(" << std::setw(7) << v.m_Coords[0] << ", " << std::setw(7) << v.m_Coords[1] << ", " << std::setw(7) << v.m_Coords[2] << ")";
	return os;
}

/*!
*	\brief	입력연 산자(>>)
*
*	\param	is[in]	입력스트림
*	\param	v[out]	입력값이 저장될 벡터
*
*	\return 입력값이 제거된 입력스트림을 반환한다.
*/
std::istream &operator >>(std::istream &is, EgVec3 &v)
{
	is >> v.m_Coords[0] >> v.m_Coords[1] >> v.m_Coords[2];
	return is;
}

/********************/
/* EgVec4 클래스 구현 */
/********************/
/*!
*	\brief	생성자
*
*	\param	x[in]	벡터의 x 좌표
*	\param	y[in]	벡터의 y 좌표
*	\param	z[in]	벡터의 z 좌표
*	\param	w[in]	벡터의 w 좌표
*/
EgVec4::EgVec4(double x, double y, double z, double w)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
	m_Coords[2] = z;
	m_Coords[3] = w;
}

/*!
*	\brief	복사 생성자
*
*	\param	cpy[in]	복사될 객체
*/
EgVec4::EgVec4(const EgVec4 &cpy)
{
	m_Coords[0] = cpy.m_Coords[0];
	m_Coords[1] = cpy.m_Coords[1];
	m_Coords[2] = cpy.m_Coords[2];
	m_Coords[3] = cpy.m_Coords[3];
}

/*!
*	\brief	소멸자
*/
EgVec4::~EgVec4()
{
}

/*!
*	\brief	벡터의 좌표를 설정한다.
*
*	\param	x[in]	설정할 x 좌표
*	\param	y[in]	설정할 y 좌표
*	\param	z[in]	설정할 z 좌표
*	\param	w[in]	설정할 w 좌표
*
*	\return 설정된 자신을 반환한다.
*/
EgVec4 &EgVec4::Set(double x, double y, double z, double w)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
	m_Coords[2] = z;
	m_Coords[3] = w;
	return *this;
}

/*!
*	\brief	벡터를 단위벡터로 정규화한다.
*
*	\return 정규화된 자신을 반환한다.
*/
EgVec4 &EgVec4::Normalize()
{
	double len = norm(*this);
	if (EQ_ZERO(len, MTYPE_EPS))
	{
		printf("[Error]: EgVec4::Normalize()...\n");
		m_Coords[0] = m_Coords[1] = m_Coords[2] = m_Coords[3] = 0.0;
	}
	else
	{
		m_Coords[0] /= len;
		m_Coords[1] /= len;
		m_Coords[2] /= len;
		m_Coords[3] /= len;
	}
	return *this;
}

/*!
*	\brief	대입 연산자
*
*	\param	rhs[in]		오른쪽 피연산자
*
*	\return 대입된 자신을 반환한다.
*/
EgVec4 &EgVec4::operator =(const EgVec4 &rhs)
{
	m_Coords[0] = rhs.m_Coords[0];
	m_Coords[1] = rhs.m_Coords[1];
	m_Coords[2] = rhs.m_Coords[2];
	m_Coords[3] = rhs.m_Coords[3];
	return *this;
}

/*!
*	\brief	벡터를 더한다.
*
*	\param	rhs[in]	더할 벡터
*
*	\return 변경된 자신을 반환한다.
*/
EgVec4 &EgVec4::operator +=(const EgVec4 &rhs)
{
	m_Coords[0] += rhs.m_Coords[0];
	m_Coords[1] += rhs.m_Coords[1];
	m_Coords[2] += rhs.m_Coords[2];
	m_Coords[3] += rhs.m_Coords[3];
	return *this;
}

/*!
*	\brief	벡터를 뺀다.
*
*	\param	rhs[in]	뺄 벡터
*
*	\return 변경된 자신을 반환한다.
*/
EgVec4 &EgVec4::operator -=(const EgVec4 &rhs)
{
	m_Coords[0] -= rhs.m_Coords[0];
	m_Coords[1] -= rhs.m_Coords[1];
	m_Coords[2] -= rhs.m_Coords[2];
	m_Coords[3] -= rhs.m_Coords[3];
	return *this;
}

/*!
*	\brief	벡터를 상수해 한다.
*
*	\param	s[in]	상수
*
*	\return 변경된 자신을 반환한다.
*/
EgVec4 &EgVec4::operator *=(const double &s)
{
	m_Coords[0] *= s;
	m_Coords[1] *= s;
	m_Coords[2] *= s;
	m_Coords[3] *= s;
	return *this;
}

/*!
*	\brief	벡터를 상수로 나눈다.
*
*	\param	s[in]	상수
*
*	\return 변경된 자신을 반환한다.
*/
EgVec4 &EgVec4::operator /=(const double &s)
{
	m_Coords[0] /= s;
	m_Coords[1] /= s;
	m_Coords[2] /= s;
	m_Coords[3] /= s;
	return *this;
}

/*!
*	\brief	단항 연산자
*
*	\return 동일부호를 갖는 객체를 반환한다.
*/
EgVec4 EgVec4::operator +() const
{
	return *this;
}

/*!
*	\brief	단항 연산자
*
*	\return 반대부호를 갖는 객체를 반환한다.
*/
EgVec4 EgVec4::operator -() const
{
	return *this * -1;
}

/*!
*	\brief	두 벡터를 더한다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터의 합을 반환한다.
*/
EgVec4 operator +(const EgVec4 &v, const EgVec4 &w)
{
	return EgVec4(
		v.m_Coords[0] + w.m_Coords[0], 
		v.m_Coords[1] + w.m_Coords[1], 
		v.m_Coords[2] + w.m_Coords[2], 
		v.m_Coords[3] + w.m_Coords[3]);
}

/*!
*	\brief	두 벡터를 뺀다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터의 차를 반환한다.
*/
EgVec4 operator -(const EgVec4 &v, const EgVec4 &w)
{
	return EgVec4(
		v.m_Coords[0] - w.m_Coords[0], 
		v.m_Coords[1] - w.m_Coords[1], 
		v.m_Coords[2] - w.m_Coords[2], 
		v.m_Coords[3] - w.m_Coords[3]);
}

/*!
*	\brief	두 벡터의 내적을 구한다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터의 내적의 결과를 반환한다.
*/
double operator *(const EgVec4 &v, const EgVec4 &w)
{
	return (
		v.m_Coords[0] * w.m_Coords[0] + 
		v.m_Coords[1] * w.m_Coords[1] + 
		v.m_Coords[2] * w.m_Coords[2] + 
		v.m_Coords[3] * w.m_Coords[3]);
}

/*!
*	\brief	벡터를 상수해 한다.
*
*	\param	v[in]	벡터
*	\param	s[in]	상수
*
*	\return 연산의 결과를 반환한다.
*/
EgVec4 operator *(const EgVec4 &v, const double &s)
{
	EgVec4 ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	벡터를 상수해 한다.
*
*	\param	s[in]	상수
*	\param	v[in]	벡터
*
*	\return 연산의 결과를 반환한다.
*/
EgVec4 operator *(const double &s, const EgVec4 &v)
{
	EgVec4 ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	벡터를 상수로 나눈다.
*
*	\param	v[in]	벡터
*	\param	s[in]	상수
*
*	\return 연산의 결과를 반환한다.
*/
EgVec4 operator /(const EgVec4 &v, const double &s)
{
	EgVec4 ret(v);
	ret /= s;
	return ret;
}

/*!
*	\brief	두 벡터가 같은지 검사한다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터가 같다면 true, 다른면 false를 반환한다.
*/
bool operator ==(const EgVec4 &v, const EgVec4 &w)
{
	return (EQ(v.m_Coords[0], w.m_Coords[0], MTYPE_EPS) && 
			EQ(v.m_Coords[1], w.m_Coords[1], MTYPE_EPS) &&
			EQ(v.m_Coords[2], w.m_Coords[2], MTYPE_EPS) && 
			EQ(v.m_Coords[3], w.m_Coords[3], MTYPE_EPS));
}

/*!
*	\brief	두 벡터가 다른지 검사한다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터가 다르면 true, 같으면 false를 반환한다.
*/
bool operator !=(const EgVec4 &v, const EgVec4 &w)
{
	return (v == w) ? false : true;
}

/*!
*	\brief	인덱스 연산자([])
*
*	\param	idx[in]	인덱스
*
*	\return	벡터의 \a idx번째 원소를 반환한다.
*/
double &EgVec4::operator [](const int &idx)
{
	assert(idx >= 0 && idx < 4);
	return m_Coords[idx];
}

/*!
*	\brief	상수객체에 대한 인덱스 연산자([])
*
*	\param	idx[in]	인덱스
*
*	\return	벡터의 \a idx번째 원소를 반환한다.
*/
const double &EgVec4::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < 4);
	return m_Coords[idx];
}

/*!
*	\brief	출력 연산자(<<)
*
*	\param	os[out]		출력 스트림
*	\param	v[in]		출력 벡터
*
*	\return \a v가 출력된 스트림을 반환한다.
*/
std::ostream &operator <<(std::ostream &os, const EgVec4 &v)
{
	os << "(" << std::setw(7) << v.m_Coords[0] << ", " << std::setw(7) << v.m_Coords[1] << ", "
			  << std::setw(7) << v.m_Coords[2] << "," << std::setw(7) << v.m_Coords[3] << ")";
	return os;
}

/*!
*	\brief	입력 연산자(>>)
*
*	\param	is[in]	입력 스트림
*	\param	v[out]	입력값이 저장될 벡터
*
*	\return 입력값이 제거된 입력스트림을 반환한다.
*/
std::istream &operator >>(std::istream &is, EgVec4 &v)
{
	is >> v.m_Coords[0] >> v.m_Coords[1] >> v.m_Coords[2] >> v.m_Coords[3];
	return is;
}

/*********************/
/* EgVec 클래스 구현 */
/*********************/
/*!
*	\brief	생성자
*
*	\param	dim[in]	벡터의 차원
*/
EgVec::EgVec(int dim)
{
	m_Coords.assign(dim, 0.0);
}

/*!
*	\brief	생성자
*	\note	원소는 모두 실수형으로 전달해야 함
*
*	\param	dim[in]	벡터의 차원
*	\param	x[in]	벡터의 첫 번째 좌표
*/
EgVec::EgVec(int dim, double x, ...)
{
	m_Coords.assign(dim, 0.0);
	m_Coords[0] = x;
	va_list ap;
	va_start(ap, dim);
	va_arg(ap, double);
	for (int i = 1; i < dim; i++)
		m_Coords[i] = va_arg(ap, double);
	va_end(ap);
}

/*!
*	\brief	생성자
*	\note	원소는 모두 정수형으로 전달해야 함
*
*	\param	dim[in]		벡터의 차원
*	\param	x[in]		벡터의 첫 번째 좌표
*/
EgVec::EgVec(int dim, int x, ...)
{
	m_Coords.assign(dim, 0.0);
	m_Coords[0] = (double)x;
	va_list ap;
	va_start(ap, dim);
	va_arg(ap, int);
	for (int i = 1; i < dim; i++)
		m_Coords[i] = (double)va_arg(ap, int);
	va_end(ap);
}

/*!
*	\brief	복사 생성자
*
*	\param	copy[in]	복사될 객체
*/
EgVec::EgVec(const EgVec &cpy)
{
	m_Coords = cpy.m_Coords;
}

/*!
*	\brief	소멸자
*/
EgVec::~EgVec()
{
}

/*!
*	\brief	벡터의 원소를 설정한다.
*	\note	매개변수는 실수형으로 주어야 한다. 예) 1: 불가능, 1.0: 가능.
*
*	\param	x[in]	첫 번째 좌표
*
*	\return 설정된 자신을 반환한다.
*/
EgVec &EgVec::Set(double x, ...)
{
	m_Coords[0] = x;
	va_list ap;
	va_start(ap, x);
	for (int i = 1; i < (int)m_Coords.size(); ++i)
		m_Coords[i] = va_arg(ap, double);
	va_end(ap);
	return *this;
}

/*!
*	\brief	벡터의 원소를 설정한다.
*	\note	매개변수는 정수형으로 주어야 한다. 예) 1: 가능, 1.0: 불가능.
*
*	\param	x[in]	첫 번째 좌표
*
*	\return 설정된 자신을 반환한다.
*/
EgVec &EgVec::Set(int x, ...)
{
	m_Coords[0] = (double)x;
	va_list ap;
	va_start(ap, x);
	for (int i = 1; i < (int)m_Coords.size(); ++i)
		m_Coords[i] = (double)va_arg(ap, int);
	va_end(ap);
	return *this;
}

/*!
*	\brief	벡터의 원소를 설정한다.
*
*	\param	Coords[in]	설정될 좌표 배열
*
*	\return 설정된 자신을 반환한다.
*/
EgVec &EgVec::Set(double *Coords)
{
	for (int i = 0; i < (int)m_Coords.size(); ++i)
		m_Coords[i] = Coords[i];
	return *this;
}

/*!
*	\brief	벡터를 정규화한다.
*
*	\return 정규화된 자신을 반환한다.
*/
EgVec &EgVec::Normalize()
{
	double len = norm(*this);
	if (EQ_ZERO(len, MTYPE_EPS))
	{
		printf("[Error]: EgVec::Normalize()...\n");
		SetZeros();
	}
	else
	{
		for (int i = 0; i < (int)m_Coords.size(); ++i)
			m_Coords[i] /= len;
	}	
	return *this;
}

/*!
*	\brief	벡터를 영벡터로 설정한다.
*
*	\return 설정된 자신을 반환한다.
*/
EgVec &EgVec::SetZeros()
{
	for (int i = 0; i < (int)m_Coords.size(); ++i)
		m_Coords[i] = 0.0;
	return *this;
}

/*!
*	\brief	벡터의 차원을 반환한다.
*
*	\return 벡터의 차원을 반환한다.
*/
int EgVec::GetDim() const
{
	return (int)m_Coords.size();
}

/*!
*	\brief	대입 연산자
*
*	\param	rhs[in]	오른쪽 피연산자
*
*	\return 대입된 자신을 반환한다.
*/
EgVec &EgVec::operator =(const EgVec &rhs)
{
	m_Coords = rhs.m_Coords;
	return *this;
}

/*!
*	\brief	벡터를 더한다.
*
*	\param	rhs[in]	더할 벡터
*
*	\return 변경된 자신을 반환한다.
*/
EgVec &EgVec::operator +=(const EgVec &rhs)
{
	assert(m_Coords.size() == rhs.m_Coords.size());
	for (int i = 0; i < (int)m_Coords.size(); ++i)
		m_Coords[i] += rhs.m_Coords[i];
	return *this;
}

/*!
*	\brief	벡터를 뺀다.
*
*	\param	rhs[in]	뺄 벡터
*
*	\return 변경된 자신을 반환한다.
*/
EgVec &EgVec::operator -=(const EgVec &rhs)
{
	assert(m_Coords.size() == rhs.m_Coords.size());
	for (int i = 0; i < (int)m_Coords.size(); ++i)
		m_Coords[i] -= rhs.m_Coords[i];
	return *this;
}

/*!
*	\brief	벡터를 상수배 한다.
*
*	\param	s[in]	상수
*
*	\return 변경된 자신을 반환한다.
*/
EgVec &EgVec::operator *=(const double &s)
{
	for (int i = 0; i < (int)m_Coords.size(); ++i)
		m_Coords[i] *= s;
	return *this;
}

/*!
*	\brief	벡터를 상수로 나눈다.
*
*	\param	s[in]	상수
*
*	\return 변경된 자신을 반환한다.
*/
EgVec &EgVec::operator /=(const double &s)
{
	assert(s != 0);
	for (int i = 0; i < (int)m_Coords.size(); ++i)
		m_Coords[i] /= s;
	return *this;
}

/*!
*	\brief	단항 연산자
*
*	\return 동일부호 객체를 반환한다.
*/
EgVec EgVec::operator +() const
{
	return *this;
}

/*!
*	\brief	단항 연산자중복
*
*	\return 반대부호 객체를 반환한다.
*/
EgVec EgVec::operator -() const
{
	return *this * -1;
}

/*!
*	\brief	두 벡터를 더한다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터의 합을 반환한다.
*/
EgVec operator +(const EgVec &v, const EgVec &w)
{
	EgVec ret(v);
	ret += w;
	return ret;
}

/*!
*	\brief	두 벡터를 뺀다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터의 차를 반환한다.
*/
EgVec operator -(const EgVec &v, const EgVec &w)
{
	EgVec ret(v);
	ret -= w;
	return ret;
}

/*!
*	\brief	두 벡터의 내적을 구한다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터의 내적값을 반환한다.
*/
double operator *(const EgVec &v, const EgVec &w)
{
	assert(v.m_Coords.size() == w.m_Coords.size());
	double ret = 0;
	for (int i = 0; i < (int)v.m_Coords.size(); ++i)
		ret += v.m_Coords[i] * w.m_Coords[i];
	return ret;
}

/*!
*	\brief	벡터를 상수배 한다.
*
*	\param	v[in]	벡터
*	\param	s[in]	상수
*
*	\return 벡터의 상수배를 반환한다.
*/
EgVec operator *(const EgVec &v, const double &s)
{
	EgVec ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	벡터를 상수배 한다.
*
*	\param	s[in]	상수
*	\param	v[in]	벡터
*
*	\return 벡터의 상수배를 반환한다.
*/
EgVec operator *(const double &s, const EgVec &v)
{
	EgVec ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	벡터를 상수로 나눈다.
*
*	\param	v[in]	벡터
*	\param	s[in]	상수
*
*	\return 연산의 결과를 반환한다.
*/
EgVec operator /(const EgVec &v, const double &s)
{
	EgVec ret(v);
	ret /= s;
	return ret;
}

/*!
*	\brief	두 벡터의 크기를 비교한다.
*
*	\param	v[in]	왼쪽 벡터
*	\param	w[in]	오른쪽 벡터
*
*	\return 왼쪽 벡터의 크기가 오른쪽 벡터보다 크면 true, 아니면 false를 반환한다.
*/
bool operator >(const EgVec &v, const EgVec &w)
{
	return (norm_sq(v) > norm_sq(w)) ? true : false;
}

/*!
*	\brief	두 벡터의 크기를 비교한다.
*
*	\param	v[in]	왼쪽 벡터
*	\param	w[in]	오른쪽 벡터
*
*	\return 왼쪽 벡터의 크기가 오른쪽 벡터보다 크거나 같으면 true, 아니면 false를 반환한다.
*/
bool operator >=(const EgVec &v, const EgVec &w)
{
	return (v > w || v == w) ? true : false;
}

/*!
*	\brief	두 벡터의 크기를 비교한다.
*
*	\param	v[in]	왼쪽 벡터
*	\param	w[in]	오른쪽 벡터
*
*	\return 왼쪽 벡터의 크기가 오른쪽 벡터보다 작으면 true, 아니면 false를 반환한다.
*/
bool operator <(const EgVec &v, const EgVec &w)
{
	return (norm_sq(v) < norm_sq(w)) ? true : false;
}

/*!
*	\brief	두 벡터의 크기를 비교한다.
*
*	\param	v[in]	왼쪽 벡터
*	\param	w[in]	오른쪽 벡터
*
*	\return 왼쪽 벡터의 크기가 오른쪽 벡터보다 작거나 같으면 true, 아니면 false를 반환한다.
*/
bool operator <=(const EgVec &v, const EgVec &w)
{
	return (v < w || v == w) ? true : false;
}

/*!
*	\brief	두 벡터가 같은지 조사한다.
*
*	\param	v[in]	왼쪽 벡터
*	\param	w[in]	오른쪽 벡터
*
*	\return 두 벡터가 같으면 true, 다르면 false를 반환한다.
*/
bool operator ==(const EgVec &v, const EgVec &w)
{
	assert(v.m_Coords.size() == w.m_Coords.size());
	for (int i = 0; i < (int)v.m_Coords.size(); ++i)
		if (!EQ(v.m_Coords[i], w.m_Coords[i], MTYPE_EPS))
			return false;	// 한 원소라도 다르면 false 반환
	return true;			// 모두 같으면 true 반환
}

/*!
*	\brief	두 벡터가 다른지 조사한다.
*
*	\param	v[in]	왼쪽 벡터
*	\param	w[in]	오른쪽 벡터
*
*	\return 두 벡터가 다르면 true, 같으면 false를 반환한다.
*/
bool operator !=(const EgVec &v, const EgVec &w)
{
	return (v == w) ? false : true;
}

/*!
*	\brief	인덱스 연산자([])
*
*	\param	idx[in]	인덱스
*
*	\return 벡터의 \a idx 번째 원소를 반환한다.
*/
double &EgVec::operator [](const int &idx)
{
	assert(idx >= 0 && idx < (int)m_Coords.size());
	return m_Coords[idx];
}

/*!
*	\brief	상수객체의 인덱스 연산자([])
*
*	\param	idx[in]	인덱스
*
*	\return 벡터의 \a idx 번째 원소를 반환한다.
*/
const double &EgVec::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < (int)m_Coords.size());
	return m_Coords[idx];
}

/*!
*	\brief	출력 연산자(<<)
*
*	\param	os[out]		출력 스트림
*	\param	v[in]		출력할 벡터
*
*	\return \a v가 출력된 스트림을 반환한다.
*/
std::ostream &operator <<(std::ostream &os, const EgVec &v)
{
	os << "(";
	for (int i = 0; i < (int)v.m_Coords.size(); ++i)
	{
		os << v.m_Coords[i] << ",     ";
		if (i == (int)v.m_Coords.size() - 1)
			os << ")" << std::endl;
	}
	return os;
}

/*!
*	\brief	입력 연산자(>>)
*
*	\param	is[in]	입력 스트림
*	\param	v[out]	입력값이 저장될 벡터
*
*	\return 입력값이 제거된 입력스트림을 반환한다.
*/
std::istream &operator >>(std::istream &is, EgVec &v)
{
	for (int i = 0; i < (int)v.m_Coords.size(); ++i)
		is >> v.m_Coords[i];
	return is;
}

/********************/
/* EgPos 클래스 구현 */
/********************/
/*!
*	\brief	생성자
*
*	\param	x[in]	x 좌표
*	\param	y[in]	y 좌표
*	\param	z[in]	z 좌표
*/
EgPos::EgPos(double x, double y, double z)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
	m_Coords[2] = z;
}

/*!
*	\brief	복사 생성자
*
*	\param	cpy[in] 복사될 객체
*
*	\return 복사된 자신을 반환한다.
*/
EgPos::EgPos(const EgPos &cpy)
{
	m_Coords[0] = cpy.m_Coords[0];
	m_Coords[1] = cpy.m_Coords[1];
	m_Coords[2] = cpy.m_Coords[2];
}

/*!
*	\brief  소멸자
*/
EgPos::~EgPos()
{
}

/*!
*	\brief	포인트의 좌표를 설정한다.
*
*	\param	x[in] x 좌표
*	\param	y[in] y 좌표
*	\param	z[in] z 좌표
*
*	\return 설정된 자신을 반환한다.
*/
EgPos &EgPos::Set(double x, double y, double z)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
	m_Coords[2] = z;
	return *this;
}

/*!
*	\brief	대입 연산자
*
*	\param	rhs[in]		오른쪽 피연산자
*
*	\return 대입된 자신을 반환한다.
*/
EgPos &EgPos::operator =(const EgPos &rhs)
{
	m_Coords[0] = rhs.m_Coords[0];
	m_Coords[1] = rhs.m_Coords[1];
	m_Coords[2] = rhs.m_Coords[2];
	return *this;
}

/*!
*	\brief	벡터를 더한다.
*
*	\param	v[in]	더할 벡터
*
*	\return 변경된 자신을 반환한다.
*/
EgPos &EgPos::operator +=(const EgVec3 &v)
{
	m_Coords[0] += v.m_Coords[0];
	m_Coords[1] += v.m_Coords[1];
	m_Coords[2] += v.m_Coords[2];
	return *this;
}

/*!
*	\brief	벡터를 뺀다.
*
*	\param	v[in]	뺄 벡터
*
*	\return 변경된 자신을 반환한다.
*/
EgPos &EgPos::operator-=(const EgVec3 &v)
{
	m_Coords[0] -= v.m_Coords[0];
	m_Coords[1] -= v.m_Coords[1];
	m_Coords[2] -= v.m_Coords[2];
	return *this;
}

/*!
*	\brief	인덱스 연산자([])
*
*	\param	idx[in] 인덱스
*
*	\return 포인트의 \a idx 번째 원소를 반환한다.
*/
double &EgPos::operator [](const int &idx)
{
	assert(idx >= 0 && idx < 3);
	return m_Coords[idx];
}

/*!
*	\brief	상수객체의 인덱스 연산자([])
*
*	\param	idx[in] 인덱스
*
*	\return 포인트의 \a idx 번째 원소를 반환한다.
*/
const double &EgPos::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < 3);
	return m_Coords[idx];
}

/*!
*	\brief	두 위치의 차이 벡터를 구한다.
*
*	\param	p[in]	첫 번째 위치
*	\param	q[in]	두 번째 위치
*
*	\return q에서 p로 향하는 벡터를 반환한다.
*/
EgVec3 operator -(const EgPos &p, const EgPos &q)
{
	return EgVec3(
		p.m_Coords[0] - q.m_Coords[0], 
		p.m_Coords[1] - q.m_Coords[1], 
		p.m_Coords[2] - q.m_Coords[2]);
}

/*!
*	\brief	위치에서 벡터를 뺀다.
*
*	\param	p[in]	위치
*	\param	v[in]	벡터
*
*	\return 새로운 위치를 반환한다.
*/
EgPos operator -(const EgPos &p, const EgVec3 &v)
{
	return EgPos(
		p.m_Coords[0] - v.m_Coords[0], 
		p.m_Coords[1] - v.m_Coords[1], 
		p.m_Coords[2] - v.m_Coords[2]);
}

/*!
*	\brief	위치에서 벡터를 더한다.
*
*	\param	p[in]	위치
*	\param	v[in]	벡터
*
*	\return 새로운 위치를 반환한다.
*/
EgPos operator +(const EgPos &p, const EgVec3 &v)
{
	return EgPos(
		p.m_Coords[0] + v.m_Coords[0], 
		p.m_Coords[1] + v.m_Coords[1], 
		p.m_Coords[2] + v.m_Coords[2]);
}

/*!
*	\brief	위치에서 벡터를 더한다.
*
*	\param	v[in]	벡터
*	\param	p[in]	위치
*
*	\return 새로운 위치를 반환한다.
*/
EgPos operator +(const EgVec3 &v, const EgPos &p)
{
	return EgPos(
		p.m_Coords[0] + v.m_Coords[0], 
		p.m_Coords[1] + v.m_Coords[1], 
		p.m_Coords[2] + v.m_Coords[2]);
}

/*!
*	\brief	두 위치가 같은지 조사한다.
*
*	\param	p[in]	첫 번째 위치
*	\param	q[in]	두 번재 위치
*
*	\return 두 위치가 같으면 true, 다르면 false를 반한한다.
*/
bool operator ==(const EgPos &p, const EgPos &q)
{
	return (EQ(p.m_Coords[0], q.m_Coords[0], MTYPE_EPS) && 
			EQ(p.m_Coords[1], q.m_Coords[1], MTYPE_EPS) && 
			EQ(p.m_Coords[2], q.m_Coords[2], MTYPE_EPS));
}

/*!
*	\brief	두 위치가 같은지 조사한다.
*
*	\param	p[in]	첫 번째 위치
*	\param	q[in]	두 번재 위치
*
*	\return 두 위치가 다르면 true, 같으면 false를 반한한다.
*/
bool operator !=(const EgPos &p, const EgPos &q)
{
	return (p == q) ? false : true;
}

/*!
*	\brief	출력 연산자(<<)
*
*	\param	os[out]		출력 스트림
*	\param	p[in]		출력할 객체
*
*	\return 출력된 스트림 객체를 반환한다.
*/
std::ostream &operator <<(std::ostream &os, const EgPos &p)
{
	os << "(" << std::setw(5) << p.m_Coords[0] << ", " << std::setw(5) << p.m_Coords[1] << ", " << std::setw(5) << p.m_Coords[2] << ")";
	return os;
}

/*!
*	\brief	입력 연산자(>>)
*
*	\param	is[in]	입력스트림
*	\param	v[out]	입력값이 저장될 벡터
*
*	\return 입력값이 제거된 입력스트림을 반환한다.
*/
std::istream &operator >>(std::istream &is, EgPos &p)
{
	is >> p.m_Coords[0] >> p.m_Coords[1] >> p.m_Coords[2];
	return is;
}

/*******************/
/* EgMat 클래스 구현 */
/*******************/
/*!
*	\brief	생성자
*
*	\param	r[in]		행의 수
*	\param	c[in]		열의 수
*	\param	elem[in]	행렬의 원소 배열
*/
EgMat::EgMat(int r, int c, double *elem)
{
	m_nRow = r;
	m_nCol = c;
	m_Data.assign(r * c, 0.0);
	if (elem != NULL)
		memcpy(&m_Data[0], elem, sizeof(double) * r * c);
}

/*!
*	\brief	복사 생성자
*
*	\param	cpy[in]	복사될 객체
*/
EgMat::EgMat(const EgMat &cpy)
{
	m_nRow = cpy.m_nRow;
	m_nCol = cpy.m_nCol;
	m_Data = cpy.m_Data;
}

/*!
*	\brief 소멸자
*/
EgMat::~EgMat()
{
}

/*!
*	\brief	행렬을 전치행렬로 변환한다.
*
*	\return 변환된 자신을 반환한다.
*/
EgMat &EgMat::SetTranspose()
{
	// 정방행렬 이라면
	if (m_nRow == m_nCol)
	{
		for (int i = 0; i < m_nRow; ++i)
			for (int j = i + 1; j < m_nCol; ++j)
				SWAP(double, m_Data[i * m_nCol + j], m_Data[j * m_nCol + i]);
	}
	else // 사각행렬 이라면
	{
		std::vector<double> Temp = m_Data;
		SWAP(int, m_nRow, m_nCol);
		for (int i = 0; i < m_nRow; ++i)
			for (int j = 0; j < m_nCol; ++j)
				m_Data[i * m_nCol + j] = Temp[j * m_nRow + i];
	}
	return *this;
}

/*!
*	\brief	단위행렬로 변환한다.
*	\note	정방행렬이 아니면 최대한 단위행렬로 만든다.
*
*	\return 변환된 자신을 리턴한다.
*/
EgMat &EgMat::SetIdentity()
{
	m_Data.assign(m_nRow * m_nCol, 0.0);
	int r = MIN(m_nRow, m_nCol);
	for (int i = 0; i < r; ++i)
		m_Data[i * m_nCol + i] = 1.0;
	return *this;
}

/*!
*	\brief	영행렬로 변환한다.
*
*	\return 변환된 자신을 리턴한다.
*/
EgMat &EgMat::SetZeros()
{
	m_Data.assign(m_nRow * m_nCol, 0.0);
	return *this;
}

/*!
*	\brief	행벡터를 설정한다.
*
*	\param	idx[in]		설정할 행의 인덱스
*	\param	v[in]		설정할 벡터
*
*	\return 변환된 자신을 리턴한다.
*/
EgMat &EgMat::SetRowVec(const int idx, const EgVec &v)
{
	assert(idx >= 0 && idx < m_nRow);
	assert(v.GetDim() == m_nCol);
	for (int i = 0; i < m_nCol; i++)
		m_Data[idx * m_nCol + i] = v.m_Coords[i];
	return *this;
}

/*!
*	\brief	열벡터를 설정한다.
*
*	\param	idx[in]		설정할 열의 인덱스
*	\param	v[in]		설정할 벡터
*
*	\return 변환된 자신을 리턴한다.
*/
EgMat &EgMat::SetColVec(const int idx, const EgVec &v)
{
	assert(idx >= 0 && idx < m_nCol);
	assert(v.GetDim() == m_nRow);
	for (int i = 0; i < m_nRow; i++)
		m_Data[i * m_nCol + idx] = v.m_Coords[i];
	return *this;
}

/*!
*	\brief	두 행을 교환한다.
*
*	\param	idx0[in]	교환할 행의 인덱스
*	\param	idx1[in]	교환할 행의 인덱스
*
*	\return 변환된 자신을 리턴한다.
*/
EgMat &EgMat::ExchangeRows(const int idx0, const int idx1)
{
	EgVec tmp(m_nCol);
	tmp = GetRowVec(idx0);
	SetRowVec(idx0, GetRowVec(idx1));
	SetRowVec(idx1, tmp);
	return *this;
}
/*!

*	\brief	두 열을 교환한다.
*
*	\param	idx0[in]	교환할 열의 인덱스
*	\param	idx1[in]	교환할 열의 인덱스
*
*	\return 변환된 자신을 리턴한다.
*/
EgMat &EgMat::ExchangeCols(const int idx0, const int idx1)
{
	EgVec tmp(m_nRow);
	tmp = GetColVec(idx0);
	SetColVec(idx0, GetColVec(idx1));
	SetColVec(idx1, tmp);
	return *this;
}

/*!
*	\brief	행벡터를 반환한다.
*
*	\param	idx[in]		행의 인덱스
*
*	\return idx 번째 행벡터를 반환한다.
*/
EgVec EgMat::GetRowVec(const int idx) const
{
	assert(idx >= 0 && idx < m_nRow);
	EgVec ret(m_nCol);
	for (int i = 0; i < m_nCol; i++)
		ret.m_Coords[i] = m_Data[idx * m_nCol + i];
	return ret;
}

/*!
*	\brief	열벡터를 반환한다.
*
*	\param	idx[in]		열의 인덱스
*
*	\return idx 번째 열벡터를 반환한다.
*/
EgVec EgMat::GetColVec(const int idx) const
{
	assert(idx >= 0 && idx < m_nCol);
	EgVec ret(m_nRow);
	for (int i = 0; i < m_nRow; i++)
		ret.m_Coords[i] = m_Data[i * m_nCol + idx];
	return ret;
}

/*!
*	\brief	행의 수를 반환한다.
*
*	\return 행의 수를 반환한다.
*/
int EgMat::GetRowNum() const
{
	return m_nRow;
}

/*!
*	\brief	열의 수를 반환한다.
*
*	\return 열의 수를 반환한다.
*/
int EgMat::GetColNum() const
{
	return m_nCol;
}

/*!
*	\brief	정방행렬 여부를 조사한다.
*
*	\return 정방행렬의 경우 true, 직사각행렬의 경우 false를 반환한다.
*/
bool EgMat::IsSquare() const
{
	return (m_nRow == m_nCol) ? true : false;
}

/*!
*	\brief	대입 연산자
*
*	\param	rhs[in] 대입될 행렬 객체
*
*	\return 대입된 자신을 반환한다.
*/
EgMat &EgMat::operator =(const EgMat &rhs)
{
	m_nRow = rhs.m_nRow;
	m_nCol = rhs.m_nCol;
	m_Data = rhs.m_Data;
	return *this;
}

/*!
*	\brief	행렬을 더한다.
*
*	\param	rhs[in]	더할 행렬
*
*	\return 더해진 자신을 반환한다.
*/
EgMat &EgMat::operator +=(const EgMat &rhs)
{
	assert(m_nRow == rhs.m_nRow && m_nCol == rhs.m_nCol);
	for (int i = 0; i < (int)m_Data.size(); ++i)
		m_Data[i] += rhs.m_Data[i];
	return *this;
}

/*!
*	\brief	행렬을 뺀다.
*
*	\param	rhs[in]	뺄 행렬
*
*	\return 연산후 대입된 자신을 반환한다.
*/
EgMat &EgMat::operator -=(const EgMat &rhs)
{
	assert(m_nRow == rhs.m_nRow && m_nCol == rhs.m_nCol);
	for (int i = 0; i < (int)m_Data.size(); ++i)
		m_Data[i] -= rhs.m_Data[i];
	return *this;
}

/*!
*	\brief	행렬을 곱한다.
*
*	\param	rhs[in]	곱할 행렬
*
*	\return 곱해진 자신을 반환한다.
*/
EgMat &EgMat::operator *=(const EgMat &rhs)
{
	assert(m_nCol == rhs.m_nRow);

	m_nCol = rhs.m_nCol;
	std::vector<double> Data(m_nRow * m_nCol, 0.0);

	for (int i = 0; i < m_nRow; ++i)
		for (int j = 0; j < m_nCol; ++j)
			for (int k = 0; k < rhs.m_nRow; ++k)
				Data[i * m_nCol + j] += m_Data[i * rhs.m_nRow + k] * rhs.m_Data[k * rhs.m_nCol + j];

	m_Data = Data;
	return *this;
}

/*!
*	\brief	행렬을 상수배 한다.
*
*	\param	s[in]	상수
*
*	\return 변경된 자신을 반환한다.
*/
EgMat &EgMat::operator *=(const double &s)
{
	for (int i = 0; i < (int)m_Data.size(); i++)
		m_Data[i] *= s;
	return *this;
}

/*!
*	\brief	행렬을 상수로 나눈다.
*
*	\param	s[in]	상수
*
*	\return 변경된 자신을 반환한다.
*/
EgMat &EgMat::operator /=(const double &s)
{
	for (int i = 0; i < (int)m_Data.size(); i++)
		m_Data[i] /= s;
	return *this;
}

/*!
*	\brief	단항 연산자
*
*	\return 동일부호를 갖는 객체를 반환한다.
*/
EgMat EgMat::operator +() const
{
	return *this;
}

/*!
*	\brief	단항 연산자
*
*	\return 반대부호의 갖는 객체를 반환한다.
*/
EgMat EgMat::operator -() const
{
	return *this * -1;
}

/*!
*	\brief	인덱스 연산자([])
*
*	\param	idx[in]		행의 인덱스
*
*	\return \a idx 행의 배열의 시작 주소를 반환한다.
*/
double *EgMat::operator [](const int idx)
{
	assert(idx >= 0 && idx < m_nRow);
	return &m_Data[idx * m_nCol];
}

/*!
*	\brief	상수객체의 인덱스 연산자([])
*
*	\param	idx[in]		행의 인덱스
*
*	\return \a idx 행의 배열의 시작 주소를 반환한다.
*/
const double *EgMat::operator [](const int idx) const
{
	assert(idx >= 0 && idx < m_nRow);
	return &m_Data[idx * m_nCol];
}

/*!
*	\brief	두 행렬을 더한다.
*
*	\param	A[in]	첫 번째 행렬
*	\param	B[in]	두 번째 행렬
*
*	\return 두 행렬의 합을 반환한다.
*/
EgMat operator +(const EgMat &A, const EgMat &B)
{
	EgMat ret(A);
	ret += B;
	return ret;
}

/*!
*	\brief	두 행렬을 뺀다.
*
*	\param	A[in]	첫 번째 행렬
*	\param	B[in]	두 번째 행렬
*
*	\return 두 행렬의 차를 반환한다.
*/
EgMat operator -(const EgMat &A, const EgMat &B)
{
	EgMat ret(A);
	ret -= B;
	return ret;
}

/*!
*	\brief	두 행렬을 곱한다.
*
*	\param	A[in]	첫 번째 행렬
*	\param	B[in]	두 번째 행렬
*
*	\return 두 행렬의 곱을 반환한다.
*/
EgMat operator *(const EgMat &A, const EgMat &B)
{
	EgMat ret(A);
	ret *= B;
	return ret;
}

/*!
*	\brief	행렬을 상수배 한다.
*
*	\param	M[in]	행렬
*	\param	s[in]	상수
*
*	\return 계산의 결과를 반환한다.
*/
EgMat operator *(const EgMat &M, const double &s)
{
	EgMat ret(M);
	ret *= s;
	return ret;
}

/*!
*	\brief	행렬을 상수배 한다.
*
*	\param	s[in]	상수
*	\param	M[in]	행렬
*
*	\return 계산의 결과를 반환한다.
*/
EgMat operator *(const double &s, const EgMat &M)
{
	EgMat ret(M);
	ret *= s;
	return ret;
}

/*!
*	\brief	행렬과 벡터의 곱을 구한다.
*
*	\param	M[in]	행렬
*	\param	v[in]	벡터
*
*	\return 행렬과 벡터의 곱(벡터)를 반환한다.
*/
EgVec operator *(const EgMat &M, const EgVec &v)
{
	assert(M.m_nCol == v.GetDim());
	EgVec ret(M.m_nRow);
	for (int i = 0; i < M.m_nRow; ++i)
		for (int j = 0; j < M.m_nCol; ++j)
			ret.m_Coords[i] += M.m_Data[i * M.m_nCol + j] * v.m_Coords[j];
	return ret;
}

/*!
*	\brief	벡터와 행렬을 곱한다.
*
*	\param	v[in]	n by 1 벡터
*	\param	M[in]	1 by m 행렬
*
*	\return 벡터와 행렬의 곱(행렬)을 반환한다.
*/
EgMat operator *(const EgVec &v, const EgMat &M)
{
	assert(M.m_nRow == 1);
	EgMat ret(v.GetDim(), M.m_nCol);
	for (int i = 0; i < v.GetDim(); ++i)
		for (int j = 0; j < M.m_nCol; ++j)
			ret.m_Data[i * M.m_nCol + j] = v.m_Coords[i] * M.m_Data[j];
	return ret;
}

/*!
*	\brief	행렬을 상수로 나눈다.
*
*	\param	M[in]	행렬
*	\param	s[in]	상수
*
*	\return 계산의 결과를 반환한다.
*/
EgMat operator /(const EgMat &M, const double &s)
{
	EgMat ret(M);
	ret /= s;
	return ret;
}

/*!
*	\brief	두 행렬을 같은지 검사한다.
*
*	\param	A[in]	왼쪽 행렬
*	\param	B[in]	오른쪽 행렬
*
*	\return 두 행렬이 같으면 true, 다르면 false를 반환한다.
*/
bool operator ==(const EgMat &A, const EgMat &B)
{
	assert(A.m_nRow == B.m_nRow && A.m_nCol == B.m_nCol);
	for (int i = 0; i < A.m_nRow * A.m_nCol; ++i)
		if (!EQ(A.m_Data[i], B.m_Data[i], MTYPE_EPS))
			return false;	// 하나의 원소라도 다르면 flase를 반환한다.
	return true;			// 모든 원소가 같으므로 true를 반환한다.
}

/*!
*	\brief	두 행렬이 다른지 검사한다.
*
*	\param	A[in]	왼쪽 피연산자
*	\param	B[in]	오른쪽 피연산자
*
*	\return 두 행렬이 다르면 true, 같으면 false를 반환한다.
*/
bool operator !=(const EgMat &A, const EgMat &B)
{
	return (A == B) ? false : true;
}

/*!
*	\brief	출력 연산자(<<)
*
*	\param	os[out]	출력 스트림
*	\param	M[in]	출력할 행렬
*
*	\return 출력된 스트림 객체를 반환한다.
*/
std::ostream &operator <<(std::ostream &os, const EgMat &M)
{
	for (int i = 0; i < M.m_nRow; i++)
	{
		os << "|";
		for (int j = 0; j < M.m_nCol; j++)
		{
			double val = M.m_Data[i * M.m_nCol + j];
			os << std::setw(5) << val << " ";
		}
		os << "|" << std::endl;
	}
	return os;
}

/********************/
/* EgPos 클래스 구현 */
/********************/
/*!
*	\brief	쿼터니온 q = w + xi + yj + zk 을 생성하는 생성자
*
*	\param	w[in]	실수부
*	\param	x[in]	i 허수부
*	\param	y[in]	j 허수부
*	\param	z[in]	k 허수부
*/
EgQuat::EgQuat(double w, double x, double y, double z)
{
	m_W = w;
	m_X = x;
	m_Y = y;
	m_Z = z;
}

/*!
*	\brief	복사 생성자
*
*	\param	cpy[in]	복사될 객체
*/
EgQuat::EgQuat(const EgQuat &cpy)
{
	m_W = cpy.m_W;
	m_X = cpy.m_X;
	m_Y = cpy.m_Y;
	m_Z = cpy.m_Z;
}

/*!
*	\brief	생성자
*
*	\param	q[in]	생성될 쿼터니온의 원소를 저장한 배열
*/
EgQuat::EgQuat(const double *q)
{
	m_W = q[0];
	m_X = q[1];
	m_Y = q[2];
	m_Z = q[3];	
}

/*!
*	\brief	회전축를 중심으로 회전하는 단위쿼터니온을 생성한다.
*
*	\param	axis[in]	회전의 중심축
*	\param	theta[in]	회전각도
*	\param	radian[in]	회적각도가 라디안이면 true, 아니면 false
*/
EgQuat::EgQuat(EgVec3 axis, double theta, bool radian)
{
	double rad, sn, cs;
	axis.Normalize();

	// 라디안으로 표현된 각도를 구한다.
	if (!radian)
		rad = theta * M_PI / 360.0;
	else
		rad = theta * 0.5;

	sn = sin(rad);
	cs = cos(rad);
	sn = (abs(sn) < MTYPE_EPS) ? 0.0 : sn;
	cs = (abs(cs) < MTYPE_EPS) ? 0.0 : cs;

	m_W = cs;
	m_X = sn * axis[0];
	m_Y = sn * axis[1];
	m_Z = sn * axis[2];
}

/*!
*	\brief	오일러각(degree) 이용하여 쿼터니온을 생성한다.
*
*	\param	theta1[in]		첫 번째축에 대한 회전각도(degree)
*	\param	theta2[in]		두 번째축에 대한 회전각도(degree)
*	\param	theta3[in]		세 번째축에 대한 회전각도(degree)
*	\param	eulerType[in]	오일러축(XYZ = RxRyRz, ZYX = RzRyRx, ...)
*/
EgQuat::EgQuat(double theta1, double theta2, double theta3, TypeEuler eulerType)
{
	double c1, c2, c3;
	double s1, s2, s3;
	theta1 = DEG2RAD(theta1);
	theta2 = DEG2RAD(theta2);
	theta3 = DEG2RAD(theta3);
	c1 = cos(theta1);
	c2 = cos(theta2);
	c3 = cos(theta3);
	s1 = sin(theta1);
	s2 = sin(theta2);
	s3 = sin(theta3);

	double m[16];
	switch (eulerType)
	{
	case EULER_XYZ:
		m[0] = c2 * c3;
		m[1] = -c2 * s3;
		m[2] = s2;
		m[3] = 0.0;

		m[4] = s1 * s2 * c3 + c1 * s3;
		m[5] = -s1 * s2 * s3 + c1 * c3;
		m[6] = -s1 * c2;
		m[7] = 0.0;

		m[8] = -c1 * s2 * c3 + s1 * s3;
		m[9] = c1 * s2 * s3 + s1 * c3;
		m[10] = c1 * c2;
		m[11] = 0.0;

		m[12] = 0.0;
		m[13] = 0.0;
		m[14] = 0.0;
		m[15] = 1.0;
		break;

	case EULER_ZYX:
		m[0] = c3 * c2;
		m[1] = -s3 * c1 + c3 * s2 * s1;
		m[2] = s3 * s1 + c3 * s2 * c1;
		m[3] = 0.0;

		m[4] = s3 * c2;
		m[5] = c3 * c1 + s3 * s2 * s1;
		m[6] = -c3 * s1 + s3 * s2 * c1;
		m[7] = 0.0;

		m[8] = -s2;
		m[9] = c2 * s1;
		m[10] = c2 * c1;
		m[11] = 0.0;

		m[12] = 0.0;
		m[13] = 0.0;
		m[14] = 0.0;
		m[15] = 1.0;
		break;
	}
	SetFromMatrix(m, false);
}

/*!
*	\brief	생성자
*
*	\param	x_axis[in]	좌표계의 x축 벡터
*	\param	y_axis[in]	좌표계의 y축 벡터
*/
EgQuat::EgQuat(EgVec3 x_axis, EgVec3 y_axis)
{
	SetFromFrameXY(x_axis, y_axis);
}

/*!
*	\brief	소멸자
*/
EgQuat::~EgQuat()
{
}

/*!
*	\brief	실수부와 허수부의 값을 설정한다.
*
*	\param	w[in]	쿼터니온의 실수부
*	\param	x[in]	쿼터니온의 i 허수부
*	\param	y[in]	쿼터니온의 j 허수부
*	\param	z[in]	쿼터니온의 k 허수부
*
*	\return	설정된 자신을 반환한다.
*/
EgQuat &EgQuat::Set(double w, double x, double y, double z)
{
	m_W = w;
	m_X = x;
	m_Y = y;
	m_Z = z;
	return *this;
}

/*!
*	\brief	실수부와 허수부의 값을 설정한다.
*
*	\param	q[in]			설정될 값의 배열
*	\param	invOrder[in]	true: (w, x, y, z) = (q[3], q[0], q[1], q[2]), false: (w, x, y, z) = (q[0], q[1], q[2], q[3])
*
*	\return	설정된 자신을 반환한다.
*/
EgQuat &EgQuat::Set(double *q, bool invOrder)
{
	if (invOrder)
	{
		m_W = q[3];
		m_X = q[0];
		m_Y = q[1];
		m_Z = q[2];
	}
	else
	{
		m_W = q[0];
		m_X = q[1];
		m_Y = q[2];
		m_Z = q[3];
	}
	return *this;
}

/*!
*	\brief	회전축을 중심으로 입력각도 만큼 회전하는 쿼터니온을 설정한다.
*
*	\param	theta[in]	입력각도
*	\param	axis[in]	회전축
*	\param	radian[in]	입력각도가 라디안이면 true, 아니면 false
*
*	\return 설정된 자신을 리턴한다.
*/
EgQuat &EgQuat::SetFromAngleAxis(const double theta, EgVec3 axis, bool radian)
{
	double rad, sn, cs;
	axis.Normalize();
	if (!radian)
		rad = theta * M_PI / 360.0;
	else
		rad = theta / 2.0;

	sn = sin(rad);
	cs = cos(rad);
	sn = (abs(sn) < MTYPE_EPS) ? 0.0 : sn;
	cs = (abs(cs) < MTYPE_EPS) ? 0.0 : cs;

	m_W = cs;
	m_X = sn * axis[0];
	m_Y = sn * axis[1];
	m_Z = sn * axis[2];

	return *this;
}

/*!
*	\brief	아크볼 위의 점 p를 점 q로 회전하는 쿼터니온을 설정한다.
*
*	\param	p[in]		아크볼 위의 점
*	\param	q[in]		아크볼 위의 점
*	\param	arcType[in]	아크볼의 타입
*
*	\return 설정된 자신을 리턴한다.
*/
EgQuat &EgQuat::SetFromArcBall(EgVec3 p, EgVec3 q, TypeArcBall arcType)
{
	double ang, nm;
	EgVec3 axis;

	p.Normalize();
	q.Normalize();

	double dot = p * q;
	if (EQ(dot, -1.0, MTYPE_EPS))	// p = -q 인경우
	{
		int idx = 0;
		if (p[idx] >= p[1])
			idx = 1;

		if (p[idx] >= p[2])
			idx = 2;

		switch (idx)
		{
		case 0:
			m_W = 0.0;
			m_X = 0.0;
			m_Y = p[2];
			m_Z = -p[1];
			break;
		case 1:
			m_W = 0.0;
			m_X = p[2];
			m_Y = 0.0;
			m_Z = -p[0];
			break;
		case 2:
			m_W = 0.0;
			m_X = p[1];
			m_Y = -p[0];
			m_Z = 0.0;
			break;
		}

		return *this;
	}

	axis = p ^ q;
	nm = norm(axis);

	if (EQ_ZERO(nm, MTYPE_EPS))	// p == q 인경우
	{
		SetIdentity();
		return *this;
	}

	switch (arcType)
	{
	case ARCBALL_X:
		if (axis[0] < 0.0)
			axis.Set(-1.0, 0.0, 0.0);
		else
			axis.Set(1.0, 0.0, 0.0);
		break;
	case ARCBALL_Y:
		if (axis[1] < 0.0)
			axis.Set(0.0, -1.0, 0.0);
		else
			axis.Set(0.0, 1.0, 0.0);
		break;
	case ARCBALL_Z:
		if (axis[2] < 0.0)
			axis.Set(0.0, 0.0, -1.0);
		else
			axis.Set(0.0, 0.0, 1.0);
		break;
	}

	ang = angle(p, q, false);
	SetFromAngleAxis(ang, axis);
	return *this;
}

/*!
*	\brief	오일러각(degree) 이용하여 쿼터니온을 설정한다.
*
*	\param	theta1[in]		첫 번째축에 대한 회전각도(degree)
*	\param	theta2[in]		두 번째축에 대한 회전각도(degree)
*	\param	theta3[in]		세 번째축에 대한 회전각도(degree)
*	\param	eulerType[in]	오일러축(XYZ = RxRyRz, ZYX = RzRyRx)
*
*	\return	설정된 자신을 반환한다.
*/
EgQuat &EgQuat::SetFromEulerAngle(double theta1, double theta2, double theta3, TypeEuler eulerType)
{
	theta1 = DEG2RAD(theta1);
	theta2 = DEG2RAD(theta2);
	theta3 = DEG2RAD(theta3);
	double c1 = cos(theta1);
	double c2 = cos(theta2);
	double c3 = cos(theta3);
	double s1 = sin(theta1);
	double s2 = sin(theta2);
	double s3 = sin(theta3);

	double m[16];
	switch (eulerType)
	{
	case EULER_XYZ:
		m[0] = c2 * c3;
		m[1] = -c2 * s3;
		m[2] = s2;
		m[3] = 0.0;

		m[4] = s1 * s2 * c3 + c1 * s3;
		m[5] = -s1 * s2 * s3 + c1 * c3;
		m[6] = -s1 * c2;
		m[7] = 0.0;

		m[8] = -c1 * s2 * c3 + s1 * s3;
		m[9] = c1 * s2 * s3 + s1 * c3;
		m[10] = c1 * c2;
		m[11] = 0.0;

		m[12] = 0.0;
		m[13] = 0.0;
		m[14] = 0.0;
		m[15] = 1.0;
		break;

	case EULER_ZYX:
		m[0] = c3 * c2;
		m[1] = -s3 * c1 + c3 * s2 * s1;
		m[2] = s3 * s1 + c3 * s2 * c1;
		m[3] = 0.0;

		m[4] = s3 * c2;
		m[5] = c3 * c1 + s3 * s2 * s1;
		m[6] = -c3 * s1 + s3 * s2 * c1;
		m[7] = 0.0;

		m[8] = -s2;
		m[9] = c2 * s1;
		m[10] = c2 * c1;
		m[11] = 0.0;

		m[12] = 0.0;
		m[13] = 0.0;
		m[14] = 0.0;
		m[15] = 1.0;
		break;
	}
	return SetFromMatrix(m, false);
}

/*!
*	\brief	좌표계의 두 축으로 쿼터니온을 설정한다.
*
*	\param	x_axis[in]	좌표계의 x 축
*	\param	y_axis[in]	좌표계의 y 축
*
*	\return 설정된 자신을 반환한다.
*/
EgQuat &EgQuat::SetFromFrameXY(const EgVec3 &x_axis, const EgVec3 &y_axis)
{
	EgVec3 X(x_axis), Y(y_axis), Z;
	Z = X ^ Y;
	X.Normalize();
	Y.Normalize();
	Z.Normalize();

	double m[16];
	m[0] = X[0];
	m[4] = X[1];
	m[8] = X[2];
	m[12] = 0.0;

	m[1] = Y[0];
	m[5] = Y[1];
	m[9] = Y[2];
	m[13] = 0.0;

	m[2] = Z[0];
	m[6] = Z[1];
	m[10] = Z[2];
	m[14] = 0.0;

	m[3] = 0.0;
	m[7] = 0.0;
	m[11] = 0.0;
	m[15] = 1.0;

	return SetFromMatrix(m, false);
}

/*!
*	\brief	좌표계의 두 축으로 쿼터니온을 설정한다.
*
*	\param	y_axis[in]	좌표계의 y 축
*	\param	z_axis[in]	좌표계의 z 축
*
*	\return 설정된 자신을 반환한다.
*/
EgQuat &EgQuat::SetFromFrameYZ(const EgVec3 &y_axis, const EgVec3 &z_axis)
{
	EgVec3 X, Y(y_axis), Z(z_axis);
	X = Y ^ Z;
	X.Normalize();
	Y.Normalize();
	Z.Normalize();

	double m[16];
	m[0] = X[0];
	m[4] = X[1];
	m[8] = X[2];
	m[12] = 0.0;

	m[1] = Y[0];
	m[5] = Y[1];
	m[9] = Y[2];
	m[13] = 0.0;

	m[2] = Z[0];
	m[6] = Z[1];
	m[10] = Z[2];
	m[14] = 0.0;

	m[3] = 0.0;
	m[7] = 0.0;
	m[11] = 0.0;
	m[15] = 1.0;

	return SetFromMatrix(m, false);
}

/*!
*	\brief	좌표계의 두 축으로 쿼터니온을 설정한다.
*
*	\param	z_axis[in]	좌표계의 z 축
*	\param	x_axis[in]	좌표계의 x 축
*
*	\return 설정된 자신을 반환한다.
*/
EgQuat &EgQuat::SetFromFrameZX(const EgVec3 &z_axis, const EgVec3 &x_axis)
{
	EgVec3 X(x_axis), Y, Z(z_axis);
	Y = Z ^ X;
	X.Normalize();
	Y.Normalize();
	Z.Normalize();

	double m[16];
	m[0] = X[0];
	m[4] = X[1];
	m[8] = X[2];
	m[12] = 0.0;

	m[1] = Y[0];
	m[5] = Y[1];
	m[9] = Y[2];
	m[13] = 0.0;

	m[2] = Z[0];
	m[6] = Z[1];
	m[10] = Z[2];
	m[14] = 0.0;

	m[3] = 0.0;
	m[7] = 0.0;
	m[11] = 0.0;
	m[15] = 1.0;

	return SetFromMatrix(m, false);
}

/*!
*	\brief	회전행렬로 쿼터니온을 설정한다.
*
*	\param	mat[in]		4 by 4 변환행렬을 표현하는 배열
*	\parma	isGL[in]	OpenGL의 변환행렬이면 true, 아니면 false
*
*	\return 설정된 자신을 반환한다.
*/
EgQuat &EgQuat::SetFromMatrix(double *mat, bool isGL)
{
	double q[4];
	double tr, s;
	int i, j, k;
	int nxt[3] = { 1, 2, 0 };

	if (isGL)
	{
		tr = mat[0] + mat[5] + mat[10];

		if (tr > 0.0)
		{
			s = SQRT(tr + 1.0);
			m_W = s * 0.5;
			s = 0.5 / s;
			m_X = (mat[6] - mat[9]) * s;
			m_Y = (mat[8] - mat[2]) * s;
			m_Z = (mat[1] - mat[4]) * s;
		}
		else
		{
			i = 0;
			if (mat[5] > mat[0])
				i = 1;
			if (mat[10] > mat[i * 4 + i])
				i = 2;
			j = nxt[i];
			k = nxt[j];
			s = SQRT((mat[i + 4 * i] - (mat[j + 4 * j] + mat[k + 4 * k])) + 1.0);
			q[i] = s * 0.5;
			s = 0.5 / s;
			m_W = (mat[k + 4 * j] - mat[j + 4 * k]) * s;
			q[j] = (mat[j + 4 * i] + mat[i + 4 * j]) * s;
			q[k] = (mat[k + 4 * i] + mat[i + 4 * k]) * s;
			m_X = q[0];
			m_Y = q[1];
			m_Z = q[2];
		}
	}
	else
	{
		tr = mat[0] + mat[5] + mat[10];

		if (tr > 0.0)
		{
			s = SQRT(tr + 1.0);
			m_W = s * 0.5;
			s = 0.5 / s;
			m_X = (mat[9] - mat[6]) * s;
			m_Y = (mat[2] - mat[8]) * s;
			m_Z = (mat[4] - mat[1]) * s;
		}
		else
		{
			i = 0;
			if (mat[5] > mat[0])
				i = 1;
			if (mat[10] > mat[i * 4 + i])
				i = 2;
			j = nxt[i];
			k = nxt[j];
			s = SQRT((mat[i * 4 + i] - (mat[j * 4 + j] + mat[k * 4 + k])) + 1.0);
			q[i] = s * 0.5;
			s = 0.5 / s;
			m_W = (mat[k * 4 + j] - mat[j * 4 + k]) * s;
			q[j] = (mat[j * 4 + i] + mat[i * 4 + j]) * s;
			q[k] = (mat[k * 4 + i] + mat[i * 4 + k]) * s;
			m_X = q[0];
			m_Y = q[1];
			m_Z = q[2];
		}
	}

	Normalize();
	return *this;
}

/*!
*	\brief	쿼터니온을 항등원으로 설정한다.
*
*	\return 설정된 자신을 반환한다.
*/
EgQuat &EgQuat::SetIdentity()
{
	m_W = 1.0;
	m_X = 0.0;
	m_Y = 0.0;
	m_Z = 0.0;
	return *this;
}

/*!
*	\brief	쿼터니온을 역원으로 설정한다.
*
*	\return 설정된 자신을 반환한다.
*/
EgQuat &EgQuat::SetInverse()
{
	if (!IsUnitQuater())
	{
		double norm_sqr = SQR(m_W) + SQR(m_X) + SQR(m_Y) + SQR(m_Z);
		*this /= norm_sqr;
	}
	SetConjugate();
	return *this;
}

/*!
*	\brief	쿼터니온을 conjugate 한다.
*
*	\return 설정된 자신을 반환한다.
*/
EgQuat &EgQuat::SetConjugate()
{
	m_X = -m_X;
	m_Y = -m_Y;
	m_Z = -m_Z;
	return *this;
}

/*!
*	\brief	쿼터니온을 negate 한다.
*
*	\return 설정된 자신을 반환한다.
*/
EgQuat &EgQuat::SetNegate()
{
	m_W = -m_W;
	m_X = -m_X;
	m_Y = -m_Y;
	m_Z = -m_Z;
	return *this;
}

/*!
*	\brief	쿼터니온을 단위쿼터니온으로 설정한다.
*
*	\return 설정된 자신을 반환한다.
*/
EgQuat &EgQuat::Normalize()
{
	double len = norm(*this);
	if (EQ_ZERO(len, MTYPE_EPS))
	{
		printf("[Error]: EgQuat::Normalize()...\n");
		SetIdentity();
	}
	else
	{
		m_W /= len;
		m_X /= len;
		m_Y /= len;
		m_Z /= len;
	}	
	return *this;
}

/*!
*	\brief	단위회전축과 회전각을 구한다.
*
*	\param	axis[out]	단위회전축이 저장됨
*	\param	angle[out]	회전각이 저장됨
*	\param	radian[in]	회전각을 라디안으로 구하려면 true, 아니면 false
*/
void EgQuat::GetAngleAxis(EgVec3 &axis, double &angle, bool radian) const
{
	if (IsIdentity() || !IsUnitQuater())
	{
		angle = 0.0;
		axis.Set(1.0, 0.0, 0.0);
		return;
	}

	double cs, sn;
	cs = m_W;
	sn = SQRT(SQR(m_X) + SQR(m_Y) + SQR(m_Z));
	angle = atan2(sn, cs);
	axis.Set(m_X / sn, m_Y / sn, m_Z / sn);
	if (radian)
		angle = angle * 2.0;
	else
		angle = angle * 360.0 / M_PI;
}

/*!
*	\brief	회전의 오일러 표현을 구한다.
*
*	\param	theta1[out]		첫 번째 회전축의 각도가 저장됨
*	\param	theta2[out]		두 번째 회전축의 각도가 저장됨
*	\param	theta3[out]		세 번째 회전축의 각도가 저장됨
*	\param	radian[in]		저장되는 회전각이 라디안이면 true, 아니면 false
*	\param	eulerType[in]	회전축의 순서(EULER_XYZ 또는 EULER_ZYX)
*/
void EgQuat::GetEulerAngle(double &theta1, double &theta2, double &theta3, bool radian, TypeEuler eulerType)
{
	EgMat mat = cast_mat(*this, false);
	switch (eulerType)
	{
	case EULER_XYZ:
		theta1 = atan2(-mat[1][2], mat[2][2]);
		theta2 = asin(mat[0][2]);
		theta3 = atan2(-mat[0][1], mat[0][0]);
		break;

	case EULER_ZYX:
		theta1 = atan2(mat[2][1], mat[2][2]);
		theta2 = asin(-mat[2][0]);
		theta3 = atan2(mat[1][0], mat[0][0]);
		break;
	}
	if (!radian)
	{
		theta1 = RAD2DEG(theta1);
		theta2 = RAD2DEG(theta2);
		theta3 = RAD2DEG(theta3);
	}
}

/*!
*	\brief	단위쿼터니온 여부를 판정한다.
*
*	\return 단위쿼터니온이면 true 아니면 false를 반환한다.
*/
bool EgQuat::IsUnitQuater() const
{
	double norm = SQR(m_W) + SQR(m_X) + SQR(m_Y) + SQR(m_Z);
	return EQ(norm, 1.0, MTYPE_EPS) ? true : false;
}

/*!
*	\brief	현재 쿼터니온이 항등원인지 여부를 판정한다.
*
*	\return 항등원이면 true, 아니면 false를 반환한다.
*/
bool EgQuat::IsIdentity() const
{
	return (
		EQ(m_W, 1.0, MTYPE_EPS) && 
		EQ(m_X, 0.0, MTYPE_EPS) &&
		EQ(m_Y, 0.0, MTYPE_EPS) && 
		EQ(m_Z, 0.0, MTYPE_EPS));
}

/*!
*	\brief	쿼터니온의 W 성분을 반환한다.
*
*	\return 쿼터니온의 W 성분
*/
double EgQuat::W() const
{
	return m_W;
}

/*!
*	\brief	쿼터니온의 X 성분을 반환한다.
*
*	\return 쿼터니온의 X 성분
*/
double EgQuat::X() const
{
	return m_X;
}

/*!
*	\brief	쿼터니온의 Y 성분을 반환한다.
*
*	\return 쿼터니온의 Y 성분
*/
double EgQuat::Y() const
{
	return m_Y;
}

/*!
*	\brief	쿼터니온의 Z 성분을 반환한다.
*
*	\return 쿼터니온의 Z 성분
*/
double EgQuat::Z() const
{
	return m_Z;
}

/*!
*	\brief	대입 연산자
*
*	\param	rhs[in]		대입될 객체
*
*	\return 대입된 자신을 반환한다.
*/
EgQuat &EgQuat::operator =(const EgQuat &rhs)
{
	m_W = rhs.m_W;
	m_X = rhs.m_X;
	m_Y = rhs.m_Y;
	m_Z = rhs.m_Z;
	return *this;
}

/*!
*	\brief	쿼터니온을 더한다.
*
*	\param	rhs[in]		더할 객체
*
*	\return 연산후 대입된 자신을 반환한다.
*/
EgQuat &EgQuat::operator +=(const EgQuat &rhs)
{
	m_W += rhs.m_W;
	m_X += rhs.m_X;
	m_Y += rhs.m_Y;
	m_Z += rhs.m_Z;
	return *this;
}

/*!
*	\brief	쿼터니온을 뺀다.
*
*	\param	rhs[in]		뺄 객체
*
*	\return 연산후 대입된 자신을 반환한다.
*/
EgQuat &EgQuat::operator -=(const EgQuat &rhs)
{
	m_W -= rhs.m_W;
	m_X -= rhs.m_X;
	m_Y -= rhs.m_Y;
	m_Z -= rhs.m_Z;
	return *this;
}

/*!
*	\brief	두 쿼터니온의 곱을 구한다.
*
*	\param	rhs[in]		곱할 객체
*
*	\return	곱해진 자신을 반환한다.
*/
EgQuat &EgQuat::operator *=(const EgQuat &rhs)
{
	double w = m_W, x = m_X, y = m_Y, z = m_Z;
	m_W = w * rhs.m_W - x * rhs.m_X - y * rhs.m_Y - z * rhs.m_Z;
	m_X = w * rhs.m_X + rhs.m_W * x + y * rhs.m_Z - z * rhs.m_Y;
	m_Y = w * rhs.m_Y + rhs.m_W * y + z * rhs.m_X - x * rhs.m_Z;
	m_Z = w * rhs.m_Z + rhs.m_W * z + x * rhs.m_Y - y * rhs.m_X;
	return *this;
}

/*!
*	\brief	상수배 한다.
*
*	\param	s[in]	상수
*
*	\return 상수배 된 자신을 반환한다.
*/
EgQuat &EgQuat::operator *=(const double s)
{
	m_W *= s;
	m_X *= s;
	m_Y *= s;
	m_Z *= s;
	return *this;
}

/*!
*	\brief	상수로 나눈다.
*
*	\param	s[in]	상수
*
*	\return 상수로 나뉘어진 자신을 반환한다.
*/
EgQuat &EgQuat::operator /=(const double s)
{
	m_W /= s;
	m_X /= s;
	m_Y /= s;
	m_Z /= s;
	return *this;
}

/*!
*	\brief	단항 연산자
*
*	\return 동일 부호의 객체를 반환한다.
*/
EgQuat EgQuat::operator +() const
{
	return *this;
}

/*!
*	\brief	단항 연산자
*
*	\return 반대 부호의 객체를 반환한다.
*/
EgQuat EgQuat::operator -() const
{
	return *this * (-1);
}

/*!
*	\brief	두 쿼터니온을 더한다.
*
*	\param	q1[in]	첫 번째 쿼터니온
*	\param	q2[in]	두 번째 쿼터니온
*
*	\return 두 쿼터니온의 합을 반환한다.
*/
EgQuat operator +(const EgQuat &q1, const EgQuat &q2)
{
	EgQuat ret(q1);
	ret += q2;
	return ret;
}

/*!
*	\brief	두 쿼터니온의 차를 구한다.
*
*	\param	q1[in]	첫 번째 쿼터니온
*	\param	q2[in]	두 번째 쿼터니온
*
*	\return 두 쿼터니온의 차를 반환한다.
*/
EgQuat operator -(const EgQuat &q1, const EgQuat &q2)
{
	EgQuat ret(q1);
	ret -= q2;
	return ret;
}

/*!
*	\brief	두 쿼터니온을 곱한다.
*
*	\param	q1[in]	첫 번째 쿼터니온
*	\param	q2[in]	두 번째 쿼터니온
*
*	\return 두 쿼터니온의 곱을 반환한다.
*/
EgQuat operator *(const EgQuat &q1, const EgQuat &q2)
{
	EgQuat ret(q1);
	ret *= q2;
	return ret;
}

/*!
*	\brief	회전된 벡터를 계산한다
*	\note	현재 쿼터니온은 단위쿼터니온이어야 함
*
*	\param	q[in]	쿼터니온
*	\param	v[in]	회전시킬 벡터
*
*	\return 회전된 벡터를 반환한다.
*/
EgVec3 operator *(const EgQuat &q, const EgVec3 &v)
{
	assert(q.IsUnitQuater());
	EgVec3 ret;
	EgQuat v0(0.0, v[0], v[1], v[2]);
	EgQuat v1 = q * v0 * inv(q);
	ret.Set(v1.m_X, v1.m_Y, v1.m_Z);
	return ret;
}

/*!
*	\brief	회전된 점의 위치를 계산한다
*	\note	현재 쿼터니온은 단위쿼터니온이어야 함
*
*	\param	q[in]	쿼터니온
*	\param	p[in]	회전시킬 점의 위치
*
*	\return 회전된 벡터를 반환한다.
*/
EgPos operator *(const EgQuat &q, const EgPos &p)
{
	assert(q.IsUnitQuater());
	EgPos ret;
	EgQuat v0(0.0, p[0], p[1], p[2]);
	EgQuat v1 = q * v0 * inv(q);
	ret.Set(v1.m_X, v1.m_Y, v1.m_Z);
	return ret;
}

/*!
*	\brief	쿼터니온을 상수배 한다.
*
*	\param	q[in]	쿼터니온
*	\param	s[in]	상수
*
*	\return 쿼터니온의 상수배를 반환한다.
*/
EgQuat operator *(const EgQuat &q, const double &s)
{
	EgQuat ret(q);
	ret *= s;
	return ret;
}

/*!
*	\brief	쿼터니온을 상수배 한다.
*
*	\param	s[in]	상수
*	\param	q[in]	쿼터니온
*
*	\return 쿼터니온의 상수배를 반환한다.
*/
EgQuat operator *(const double &s, const EgQuat &q)
{
	EgQuat ret(q);
	ret *= s;
	return ret;
}

/*!
*	\brief	쿼터니온을 상수로 나눈다.
*
*	\param	q[in]	쿼터니온
*	\param	s[in]	상수
*
*	\return 나눈 결과를 반환한다.
*/
EgQuat operator /(const EgQuat &q, const double s)
{
	EgQuat ret(q);
	ret /= s;
	return ret;
}

/*!
*	\brief	등호연산자(==)
*
*	\param	q1[in]	왼쪽 피연산자
*	\param	q2[in]	오른편 피연산자
*
*	\return 두 쿼터니온이 같으면 true, 다르면 false 반환한다.
*/
bool operator ==(const EgQuat &q1, const EgQuat &q2)
{
	return (EQ(q1.m_W, q2.m_W, MTYPE_EPS) && 
			EQ(q1.m_X, q2.m_X, MTYPE_EPS) && 
			EQ(q1.m_Y, q2.m_Y, MTYPE_EPS) && 
			EQ(q1.m_Z, q2.m_Z, MTYPE_EPS));
}

/*!
*	\brief	부등호연산자(!=)
*
*	\param	q1[in]	왼쪽 피연산자
*	\param	q2[in]	오른편 피연산자
*
*	\return 두 쿼터니온이 다르면 true, 같으면 false 반환한다.
*/
bool operator !=(const EgQuat &q1, const EgQuat &q2)
{
	return (q1 == q2) ? false : true;
}

/*!
*	\brief	출력 연산자
*
*	\param	os[out]		출력 스트림
*	\param	q[in]		출력할 쿼터니온
*
*	\return 출력된 스트림을 반환한다.
*/
std::ostream &operator <<(std::ostream &os, const EgQuat &q)
{
	os << q.m_W << " + " << q.m_X << "(i) + " << q.m_Y << "(j) + " << q.m_Z << "(k)" << std::endl;
	return os;
}

/*!
*	\brief	입력 연산자
*
*	\param	is[in]	입력 스트림
*	\param	q[out]	입력받을 쿼터니온
*
*	\return 입력된 스트림을 반환한다.
*/
std::istream &operator >>(std::istream &is, EgQuat &q)
{
	is >> q.m_W >> q.m_X >> q.m_Y >> q.m_Z;
	return is;
}

/*********************/
/* EgLine 클래스 구현 */
/*********************/
/*!
*	\brief	생성자
*
*	\param	Pos[in]		직선위의 한 점
*	\param	Dir[in]		직선의 방향 벡터(단위 벡터일 필요 없음)
*/
EgLine::EgLine(EgPos Pos, EgVec3 Dir)
{
	m_Pos = Pos;
	m_Dir = Dir;
}

/*!
*	\brief	두 점을 지나는 직선을 생성하는 생성자.
*
*	\param	p[in]	직선 위의 점, l(0) = p
*	\param	q[in]	직선 위의 점, l(1) = q
*/
EgLine::EgLine(EgPos p, EgPos q)
{
	m_Pos = p;
	m_Dir = q - p;
}

/*!
*	\brief	복사 생성자
*
*	\param	cpy[in]		복사될 객체
*/
EgLine::EgLine(EgLine &cpy)
{
	m_Pos = cpy.m_Pos;
	m_Dir = cpy.m_Dir;
}

/*!
*	\brief 소멸자
*/
EgLine::~EgLine()
{
}

/*!
*	\brief	직선 위의 점을 계산한다.
*
*	\param	t[in]	매개변수
*
*	\return 직선 위의 점 l(t)를 반환한다.
*/
EgPos EgLine::Eval(double t)
{
	return m_Pos + t * m_Dir;
}

/*!
*	\brief	점 q가 직선 위의 점인지 조사한다.
*
*	\param	q[in]	조사할 점
*
*	\return 점 q가 직선 위의 점이면 true, 아니면 false를 반환한다.
*/
bool EgLine::IsOnLine(EgPos &q)
{
	EgVec3 w = q - m_Pos;
	double s = norm(m_Dir ^ w);
	if (EQ_ZERO(s, MTYPE_EPS))
		return true;
	else
		return false;
}

/*!
*	\brief	대입 연산자
*
*	\param	rhs[in]		대입할 객체
*
*	\return 대입된 자신을 반환한다.
*/
EgLine &EgLine::operator =(const EgLine &rhs)
{
	m_Pos = rhs.m_Pos;
	m_Dir = rhs.m_Dir;
	return *this;
}

/*!
*	\brief	두 직선이 같은지 조사한다.
*
*	\param	lhs[in]		왼쪽 직선
*	\param	rhs[in]		오른쪽 직선
*
*	\return 동일한 직선이면 true, 아니면 false를 반환한다.
*/
bool operator ==(EgLine &lhs, EgLine &rhs)
{
	if (lhs.IsOnLine(rhs.m_Pos) && EQ_ZERO(norm(lhs.m_Dir ^ rhs.m_Dir), MTYPE_EPS))
		return true;
	else
		return false;
}

/*!
*	\brief	두 직선이 다른지 조사한다.
*
*	\param	lhs[in]		왼쪽 직선
*	\param	rhs[in]		오른쪽 직선
*
*	\return 다른 직선이면 true, 아니면 false를 반환한다.
*/
bool operator !=(EgLine &lhs, EgLine &rhs)
{
	return (lhs == rhs) ? false : true;
}

/*!
*	\brief	출력 연산자
*
*	\param	os[out]		출력 스트림
*	\param	l[in]		출력할 객체
*
*	\return 출력된 스트림을 반환한다.
*/
std::ostream &operator <<(std::ostream &os, const EgLine &l)
{
	os << "("
		<< l.m_Pos[0] << " + (" << l.m_Dir[0] << ") * t, "
		<< l.m_Pos[1] << " + (" << l.m_Dir[1] << ") * t, "
		<< l.m_Pos[2] << " + (" << l.m_Dir[2] << ") * t)";
	return os;
}

/*********************/
/* EgPlane 클래스 구현 */
/*********************/
/*!
*	\brief	생성자
*/
EgPlane::EgPlane()
{
	m_Coords.Set(0.0, 0.0, 0.0, 0.0);
}

/*!
*	\brief	생성자
*
*	\param	n[in]	평면의 법선벡터
*	\param	p[in]	평면 위의 한 점
*/
EgPlane::EgPlane(const EgVec3 &n, const EgPos &p)
{
	m_Coords = cast_vec4(n, -n * cast_vec3(p));
}

/*!
*	\brief	세 점 p1, p2, p3 를 지나는 평면의 생성자
*
*	\param	p1[in]	평면 위의 첫 번째 점
*	\param	p2[in]	평면 위의 두 번째 점
*	\param	p3[in]	평면 위의 세 번째 점
*/
EgPlane::EgPlane(const EgPos &p1, const EgPos &p2, const EgPos &p3)
{
	EgVec3 n = (p2 - p1) ^ (p3 - p1);
	m_Coords = cast_vec4(n, -n * cast_vec3(p1));
}

/*!
*	\brief	생성자
*
*	\param	n[in]	법선벡터
*	\param	d[in]	상수 d = - n * p (p: 평면 위의 임의의 점).
*/
EgPlane::EgPlane(const EgVec3 &n, const double &d)
{
	m_Coords = cast_vec4(n, d);
}

/*!
*	\brief	생성자
*
*	\param	a[in]	평면의 방정식의 계수
*	\param	b[in]	평면의 방정식의 계수
*	\param	c[in]	평면의 방정식의 계수
*	\param	d[in]	평면의 방정식의 계수
*/
EgPlane::EgPlane(double a, double b, double c, double d)
{
	m_Coords.Set(a, b, c, d);
}

/*!
*	\brief	복사 생성자
*
*	\param	cpy[in]		복사될 객체
*/
EgPlane::EgPlane(const EgPlane &cpy)
{
	m_Coords = cpy.m_Coords;
}

/*!
*	\brief 소멸자
*/
EgPlane::~EgPlane()
{
}

/*!
*	\brief	평면의 함수값을 계산한다.
*	\note	마지막 수정일: 2020-05-24
*
*	\param	p[in]	조사할 좌표(x, y, z)
*
*	\return 함수값 f(x, y, z) = ax + by + cz + d를 반환한다.
*/
double EgPlane::Eval(EgPos p)
{
	return (
		m_Coords[0] * p.m_Coords[0] + 
		m_Coords[1] * p.m_Coords[1] + 
		m_Coords[2] * p.m_Coords[2] + 
		m_Coords[3]);
}

/*!
*	\brief	평면의 법벡터를 반환한다.
*	\note	마지막 수정일: 2020-05-28
*
*	\param	bNormalize[in]	정규화 여부
*
*	\return 평면의 법벡터를 반환한다.
*/
EgVec3 EgPlane::N(bool bNormalize)
{
	EgVec3 ret(m_Coords[0], m_Coords[1], m_Coords[2]);
	if (bNormalize)
		ret.Normalize();
	return ret;
}

/*!
*	\brief	평면의 방정식(ax + by + cz + d = 0)의 상수 d를 반환한다.
*
*	\return 평면의 방정식(ax + by + cz + d = 0)의 상수 d를 반환한다.
*/
double EgPlane::d()
{
	return m_Coords[3];
}

/*!
*	\brief	점 p가 평면에 놓인 점 인지를 조사한다.
*
*	\param	p[in]	조사할 점
*
*	\return 점 p가 평면에 놓인 점이면 true, 아니면 false를 반환한다.
*/
bool EgPlane::IsOnPlane(EgPos &p)
{
	double f = Eval(p);
	if (EQ_ZERO(f, MTYPE_EPS))
		return true;
	else
		return false;
}

/*!
*	\brief	점 p가 평면의 위쪽 점인지를 조사한다.
*
*	\param	p[in]	조사할 점
*
*	\return 점 p가 평면의 위쪽 점 일경우 true, 아니면 false를 반환한다.
*/
bool EgPlane::IsAbovePlane(EgPos &p)
{
	double f = Eval(p);
	if (f > MTYPE_EPS)
		return true;
	else
		return false;
}

/*!
*	\brief	점 p가 평면의 아래쪽 점인지를 조사한다.
*
*	\param	p[in]	조사할 점
*
*	\return 점 p가 평면의 아래쪽 점 일경우 true, 아니면 false를 반환한다.
*/
bool EgPlane::IsBelowPlane(EgPos &p)
{
	double f = Eval(p);
	if (f < -MTYPE_EPS)
		return true;
	else
		return false;
}

/*!
*	\brief	법선이 반대인 평면을 반환한다.
*	\note	마지막 수정일: 2020-09-02
*
*	\return 법선이 반대인 평면을 반환한다.
*/
EgPlane EgPlane::GetReversePlane()
{
	return EgPlane(-m_Coords[0], -m_Coords[1], -m_Coords[2], -m_Coords[3]);
}

/*!
*	\brief	대입 연산자
*
*	\param	rhs[in]		대입될 객체
*
*	\return 대입된 자신을 반환한다.
*/
EgPlane &EgPlane::operator =(const EgPlane &rhs)
{
	m_Coords = rhs.m_Coords;
	return *this;
}

/*!
*	\brief	두 평면이 같은지 조사한다.
*
*	\param	lhs[in]		첫 번째 평면
*	\param	rhs[in]		두 번째 평면
*
*	\return 두 평면이 같으면 true, 다르면 false를 반환한다.
*/
bool operator ==(const EgPlane &lhs, const EgPlane &rhs)
{
	EgVec4 v = lhs.m_Coords;
	EgVec4 w = rhs.m_Coords;
	v.Normalize();
	w.Normalize();
	double cs = v * w;
	if (EQ(cs, 1.0, MTYPE_EPS) || EQ(cs, -1.0, MTYPE_EPS))
		return true;
	else
		return false;
}

/*!
*	\brief	두 평면이 다른지 조사한다.
*
*	\param	lhs[in]		첫 번째 평면
*	\param	rhs[in]		두 번째 평면
*
*	\return 두 평면이 다르면 true, 같으면 false를 반환한다.
*/
bool operator !=(const EgPlane &lhs, const EgPlane &rhs)
{
	return (lhs == rhs) ? false : true;
}

/*!
*	\brief	출력 연산자
*
*	\param	os[out]		출력 스트림
*	\param	pi[in]		출력 평면
*
*	\return 출력 스트림을 반환한다.
*/
std::ostream &operator <<(std::ostream &os, const EgPlane &pi)
{
	os << "(" 
		<< pi.m_Coords[0] << ") * x + ("
		<< pi.m_Coords[1] << ") * y + ("
		<< pi.m_Coords[2] << ") * z + ("
		<< pi.m_Coords[3] << ") = 0";
	return os;
}

/*********************/
/* EgTrans 클래스 구현 */
/*********************/
/*!
*	\brief	생성자
*
*	\param	t[in]	이동 정보
*	\param	q[in]	회전 정보
*/
EgTrans::EgTrans(const EgVec3 &t, const EgQuat &q)
{
	m_T = t;
	m_Q = q;
}

/*!
*	\brief	생성자
*
*	\param	mat[in]		변환 행렬(4 X 4)
*	\param	isGL[in]	변환 행렬의 OpenGL 형태(row major) 여부
*/
EgTrans::EgTrans(double *mat, bool isGL)
{
	if (isGL)
		m_T.Set(mat[12], mat[13], mat[14]);
	else
		m_T.Set(mat[3], mat[7], mat[11]);
	m_Q.SetFromMatrix(mat, isGL);
}

/*!
*	\brief	복사 생성자
*
*	\param	cpy[in]		복사될 객체
*/
EgTrans::EgTrans(const EgTrans &cpy)
{
	m_T = cpy.m_T;
	m_Q = cpy.m_Q;
}

/*!
*	\brief 소멸자.
*/
EgTrans::~EgTrans()
{
}

/*!
*	\brief	회전을 negate한다.
*/
void EgTrans::SetNegate()
{
	m_Q.SetNegate();
}

/*!
*	\brief	이동 변환을 설정한다.
*
*	\param	t[in] 설정될 이동 변환
*/
void EgTrans::SetTrans(const EgVec3 &t)
{
	m_T = t;
}

/*!
*	\brief	회전 변환을 설정한다.
*
*	\param	q[in] 설정될 회전 변환
*/
void EgTrans::SetRot(const EgQuat &q)
{
	m_Q = q;
}

/*!
*	\brief	대입 연산자
*
*	\param	rhs[in]		대입될 객체
*
*	\return 변경된 자신을 반환한다.
*/
EgTrans &EgTrans::operator =(const EgTrans &rhs)
{
	m_T = rhs.m_T;
	m_Q = rhs.m_Q;
	return *this;
}

/*!
*	\brief	변환을 곱한다.
*
*	\param	rhs[in]		곱해질 변환
*
*	\return 곱해진 자신을 반환한다.
*/
EgTrans &EgTrans::operator *=(const EgTrans &rhs)
{
	m_T += m_Q * rhs.m_T;
	m_Q *= rhs.m_Q;
	m_Q.Normalize();
	return *this;
}

/*!
*	\brief	변환을 상수배 한다.
*
*	\param	s[in]	상수
*
*	\return 변경된 자신을 반환한다.
*/
EgTrans &EgTrans::operator *=(const double &s)
{
	m_T *= s;
	m_Q *= s;
	return *this;
}

/*!
*	\brief	두 변환을 곱한다.
*
*	\param	X1[in]	첫 번째 변환
*	\param	X2[in]	두 번째 변환
*
*	\return 곱한 결과를 반환한다.
*/
EgTrans operator *(const EgTrans &X1, const EgTrans &X2)
{
	EgTrans ret(X1);
	ret *= X2;
	return ret;
}

/*!
*	\brief	변환을 상수배한다.
*
*	\param	X[in]	변환
*	\param	s[in]	상수
*
*	\return 변환의 상수배를 반환한다.
*/
EgTrans operator *(const EgTrans &X, const double &s)
{
	EgTrans ret(X);
	ret *= s;
	return ret;
}

/*!
*	\brief	변환을 상수배를 구한다.
*
*	\param	s[in]	상수
*	\param	X[in]	왼쪽 피연산자
*
*	\return 변환의 상수배를 반환한다.
*/
EgTrans operator *(const double &s, const EgTrans &X)
{
	EgTrans ret(X);
	ret *= s;
	return ret;
}

/*!
*	\brief	변환된 점을 구한다.
*
*	\param	X[in]	변환 객체
*	\param	p[in]	대상 점
*
*	\return 이전 좌표계에서 표현된 점 p의 좌표를 반환한다.
*/
EgPos operator *(const EgTrans &X, const EgPos &p)
{
	return X.m_Q * p + X.m_T;
}

/*!
*	\brief	변환된 벡터를 구한다.
*
*	\param	X[in]	변환 객체
*	\param	v[in]	대상 벡터
*
*	\return 이전 좌표계에서 표현된 벡터 v의 좌표를 반환한다.
*/
EgVec3 operator *(const EgTrans &X, const EgVec3 &v)
{
	return X.m_Q * v;
}

/*!
*	\brief	변환된 직선을 구한다.
*
*	\param	X[in]	변환 객체
*	\param	l[in]	대상 직선
*
*	\return 이전 좌표계에서 표현된 직선 l의 표현을 반환한다.
*/
EgLine operator *(EgTrans &X, EgLine &l)
{
	EgPos p = X * l.m_Pos;
	EgVec3 v = X * l.m_Dir;
	return EgLine(p, v);
}

/*!
*	\brief	변환된 평면을 구한다.
*
*	\param	X[in]	변환 객체
*	\param	pi[in]	대상 평면
*
*	\return 이전 좌표계에서 표현된 평면 pi을 반환한다.
*/
EgPlane operator *(EgTrans &X, EgPlane &pi)
{
	EgVec3 n = X * pi.N();
	double d = pi.d() - pi.N() * (inv(X.m_Q) * X.m_T);
	return EgPlane(n, d);
}

/*!
*	\brief	두 변환이 같은지 조사한다.
*
*	\param	X1[in]	첫 번째 변환
*	\param	X2[in]	두 번째 변환
*
*	\return 두 변환이 같으면 true, 다르면 false를 반환한다.
*/
bool operator ==(const EgTrans &X1, const EgTrans &X2)
{
	if (X1.m_T == X2.m_T && X1.m_Q == X1.m_Q)
		return true;
	else
		return false;
}

/*!
*	\brief	두 변환이 다른지 조사한다.
*
*	\param	X1[in]	첫 번째 변환
*	\param	X2[in]	두 번째 변환
*
*	\return 두 변환이 다르면 true, 같으면 false를 반환한다.
*/
bool operator !=(const EgTrans &X1, const EgTrans &X2)
{
	return (X1 == X2) ? false : true;
}

/*!
*	\brief	출력 연산자
*
*	\param	os[out]		출력 스트림
*	\param	X[in]		출력 변환
*
*	\return 출력된 스트림을 반환한다.
*/
std::ostream &operator <<(std::ostream &os, const EgTrans &X)
{
	os << cast_mat(X, false);
	return os;
}

