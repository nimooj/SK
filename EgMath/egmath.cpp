#include "..\usr\include\EgMath\egmath.h"

/********************/
/* EgVec2 Ŭ���� ���� */
/********************/
/*!
*	\brief	������
*	\note	������ ������: 2020-07-03
*
*	\param	x[in]	������ x ��ǥ
*	\param	y[in]	������ y ��ǥ
*/
EgVec2::EgVec2(double x, double y)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
}

/*!
*	\brief	���� ������
*	\note	������ ������: 2020-07-03
*
*	\param	cpy[in]	����� ��ü
*/
EgVec2::EgVec2(const EgVec2 &cpy)
{
	m_Coords[0] = cpy.m_Coords[0];
	m_Coords[1] = cpy.m_Coords[1];
}

/*!
*	\brief	�Ҹ���
*	\note	������ ������: 2020-07-03
*/
EgVec2::~EgVec2()
{
}

/*!
*	\brief	������ ��ǥ�� �����Ѵ�.
*	\note	������ ������: 2020-07-03
*
*	\param	x[in]	������ x ��ǥ
*	\param	y[in]	������ y ��ǥ
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
*/
EgVec2 &EgVec2::Set(double x, double y)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
	return *this;
}

/*!
*	\brief	���͸� �������ͷ� ����ȭ�Ѵ�.
*	\note	������ ������: 2020-07-03
*
*	\return ����ȭ�� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���� ������
*	\note	������ ������: 2020-07-03
*
*	\param	rhs[in]	���Ե� ����
*
*	\return ���Ե� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec2 &EgVec2::operator =(const EgVec2 &rhs)
{
	m_Coords[0] = rhs.m_Coords[0];
	m_Coords[1] = rhs.m_Coords[1];
	return *this;
}

/*!
*	\brief	���͸� ���Ѵ�.
*	\note	������ ������: 2020-07-03
*
*	\param	rhs[in]	���� ����
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec2 &EgVec2::operator +=(const EgVec2 &rhs)
{
	m_Coords[0] += rhs.m_Coords[0];
	m_Coords[1] += rhs.m_Coords[1];
	return *this;
}

/*!
*	\brief	���͸� ����.
*	\note	������ ������: 2020-07-03
*
*	\param	rhs[in]	�� ����
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec2 &EgVec2::operator -=(const EgVec2 &rhs)
{
	m_Coords[0] -= rhs.m_Coords[0];
	m_Coords[1] -= rhs.m_Coords[1];
	return *this;
}

/*!
*	\brief	���͸� ����� �Ѵ�.
*	\note	������ ������: 2020-07-03
*
*	\param	s[in]	���
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec2 &EgVec2::operator *=(const double &s)
{
	m_Coords[0] *= s;
	m_Coords[1] *= s;
	return *this;
}

/*!
*	\brief	���͸� ����� ������.
*	\note	������ ������: 2020-07-03
*
*	\param	s[in]	���
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec2 &EgVec2::operator /=(const double &s)
{
	m_Coords[0] /= s;
	m_Coords[1] /= s;
	return *this;
}

/*!
*	\brief	���� ������
*	\note	������ ������: 2020-07-03
*
*	\return ���� ��ȣ�� ���� ��ü�� ��ȯ�Ѵ�.
*/
EgVec2 EgVec2::operator +() const
{
	return *this;
}

/*!
*	\brief	���� ������
*	\note	������ ������: 2020-07-03
*
*	\return �ݴ� ��ȣ�� ���� ��ü�� ��ȯ�Ѵ�.
*/
EgVec2 EgVec2::operator -() const
{
	return *this * -1;
}

/*!
*	\brief	�ε��� ������([])
*	\note	������ ������: 2020-07-03
*
*	\param	idx[in]	��ǥ�� �ε���
*
*	\return ������ \a idx��° ������ ���۷����� ��ȯ�Ѵ�.
*/
double &EgVec2::operator [](const int &idx)
{
	assert(idx >= 0 && idx < 2);
	return m_Coords[idx];
}

/*!
*	\brief	��� ��ü�� ���� �ε��� ������([])
*	\note	������ ������: 2020-07-03
*
*	\param	idx[in]	������ �ε���
*
*	\return ������ \a idx��° ������ ���۷����� ��ȯ�Ѵ�.
*/
const double &EgVec2::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < 2);
	return m_Coords[idx];
}

/*!
*	\brief	�� ������ ���� ���Ѵ�.
*	\note	������ ������: 2020-07-03
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ������ ��(v + w)�� ��ȯ�Ѵ�.
*/
EgVec2 operator +(const EgVec2 &v, const EgVec2 &w)
{
	EgVec2 ret(v);
	ret += w;
	return ret;
}

/*!
*	\brief	�� ������ ���� ���Ѵ�.
*	\note	������ ������: 2020-07-03
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ������ ��(v - w)�� ��ȯ�Ѵ�.
*/
EgVec2 operator -(const EgVec2 &v, const EgVec2 &w)
{
	EgVec2 ret(v);
	ret -= w;
	return ret;
}

/*!
*	\brief	�� ������ ������ ���Ѵ�.
*	\note	������ ������: 2020-07-03
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ������ ������ ��ȯ�Ѵ�.
*/
double operator *(const EgVec2 &v, const EgVec2 &w)
{
	return (v.m_Coords[0] * w.m_Coords[0] + v.m_Coords[1] * w.m_Coords[1]);
}

/*!
*	\brief	���͸� ����� �Ѵ�.
*	\note	������ ������: 2020-07-03
*
*	\param	v[in]	����
*	\param	s[in]	���
*
*	\return ������ ����踦 ��ȯ�Ѵ�.
*/
EgVec2 operator *(const EgVec2 &v, double s)
{
	EgVec2 ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	���͸� ����� �Ѵ�.
*	\note	������ ������: 2020-07-03
*
*	\param	s[in]	���
*	\param	v[in]	����
*
*	\return ������ ����踦 ��ȯ�Ѵ�.
*/
EgVec2 operator *(double s, const EgVec2 &v)
{
	EgVec2 ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	���͸� ����� ������
*	\note	������ ������: 2020-07-03
*
*	\param	v[in]	����
*	\param	s[in]	���
*
*	\return ����� �������� ���͸� ��ȯ�Ѵ�.
*/
EgVec2 operator /(const EgVec2 &v, double s)
{
	EgVec2 ret(v);
	ret /= s;
	return ret;
}

/*!
*	\brief	�� ���Ͱ� ������ �˻��Ѵ�.
*	\note	������ ������: 2020-07-03
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ���Ͱ� ������ true, �ٸ��� false�� �����Ѵ�.
*/
bool operator ==(const EgVec2 &v, const EgVec2 &w)
{
	return (EQ(v.m_Coords[0], w.m_Coords[0], MTYPE_EPS) &&
			EQ(v.m_Coords[1], w.m_Coords[1], MTYPE_EPS));
}

/*!
*	\brief	�� ���Ͱ� �ٸ��� �˻��Ѵ�.
*	\note	������ ������: 2020-07-03
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ���Ͱ� �ٸ��� true, ������ false�� ��ȯ�Ѵ�.
*/
bool operator !=(const EgVec2 &v, const EgVec2 &w)
{
	return (v == w) ? false : true;
}

/*!
*	\brief	���͸� ����Ѵ�.
*	\note	������ ������: 2020-07-03
*
*	\param	os[in]	��� ��Ʈ��
*	\param	v[in]	��� ����
*
*	\return \a v�� ��µ� ��Ʈ���� ��ȯ�Ѵ�.
*/
std::ostream &operator <<(std::ostream &os, const EgVec2 &v)
{
	os << "(" << std::setw(7) << v.m_Coords[0] << ", " << std::setw(7) << v.m_Coords[1] << ")";
	return os;
}

/*!
*	\brief	������ ��ǥ�� �Է� �޴´�.
*	\note	������ ������: 2020-07-03
*
*	\param	is[in]	�Է� ��Ʈ��
*	\param	v[out]	��� ����
*
*	\return �Է� ��Ʈ���� ��ȯ�Ѵ�.
*/
std::istream &operator >>(std::istream &is, EgVec2 &v)
{
	is >> v.m_Coords[0] >> v.m_Coords[1];
	return is;
}

/********************/
/* EgVec3 Ŭ���� ���� */
/********************/
/*!
*	\brief	������
*
*	\param	x[in]	������ x ��ǥ
*	\param	y[in]	������ y ��ǥ
*	\param	z[in]	������ z ��ǥ
*/
EgVec3::EgVec3(double x, double y, double z)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
	m_Coords[2] = z;
}

/*!
*	\brief	���� ������
*
*	\param	cpy[in]	����� ��ü
*/
EgVec3::EgVec3(const EgVec3 &cpy)
{
	m_Coords[0] = cpy.m_Coords[0];
	m_Coords[1] = cpy.m_Coords[1];
	m_Coords[2] = cpy.m_Coords[2];
}

/*!
*	\brief	�Ҹ���
*/
EgVec3::~EgVec3()
{
}

/*!
*	\brief	������ ��ǥ�� �����Ѵ�.
*
*	\param	x[in]	������ x ��ǥ
*	\param	y[in]	������ y ��ǥ
*	\param	z[in]	������ z ��ǥ
*/
EgVec3 &EgVec3::Set(double x, double y, double z)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
	m_Coords[2] = z;
	return *this;
}

/*!
*	\brief	���͸� ����ȭ�Ѵ�.
*
*	\return ����ȭ�� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���� ������
*
*	\param	rhs[in]	���Ե� ��ü
*
*	\return ���Ե� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec3 &EgVec3::operator =(const EgVec3 &rhs)
{
	m_Coords[0] = rhs.m_Coords[0];
	m_Coords[1] = rhs.m_Coords[1];
	m_Coords[2] = rhs.m_Coords[2];
	return *this;
}

/*!	
*	\brief	���͸� ���Ѵ�.
*
*	\param	rhs[in]	���� ����
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec3 &EgVec3::operator +=(const EgVec3 &rhs)
{
	m_Coords[0] += rhs.m_Coords[0];
	m_Coords[1] += rhs.m_Coords[1];
	m_Coords[2] += rhs.m_Coords[2];
	return *this;
}

/*!
*	\brief	���͸� ����.
*
*	\param	rhs[in]	�� ����
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec3 &EgVec3::operator -=(const EgVec3 &rhs)
{
	m_Coords[0] -= rhs.m_Coords[0];
	m_Coords[1] -= rhs.m_Coords[1];
	m_Coords[2] -= rhs.m_Coords[2];
	return *this;
}

/*!
*	\brief	���͸� ����� �Ѵ�.
*
*	\param	s[in]	���
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec3 &EgVec3::operator *=(const double &s)
{
	m_Coords[0] *= s;
	m_Coords[1] *= s;
	m_Coords[2] *= s;
	return *this;
}

/*!
*	\brief	���͸� ����� ������.
*
*	\param	s[in]	���� ���
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec3 &EgVec3::operator /=(const double &s)
{
	m_Coords[0] /= s;
	m_Coords[1] /= s;
	m_Coords[2] /= s;
	return *this;
}

/*!
*	\brief	���͸� �����Ѵ�.
*
*	\param	rhs[in]	������ ����
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���� ������
*
*	\return ���Ϻ�ȣ�� ���� ��ü�� ��ȯ�Ѵ�.
*/
EgVec3 EgVec3::operator +() const
{
	return *this;
}

/*!
*	\brief	���� ������
*
*	\return �ݴ��ȣ�� ���� ��ü�� ��ȯ�Ѵ�.
*/
EgVec3 EgVec3::operator -() const
{
	return *this * -1;
}

/*!
*	\brief	�� ���͸� ���Ѵ�.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ������ ���� ��ȯ�Ѵ�.
*/
EgVec3 operator +(const EgVec3 &v, const EgVec3 &w)
{
	return EgVec3(
		v.m_Coords[0] + w.m_Coords[0], 
		v.m_Coords[1] + w.m_Coords[1], 
		v.m_Coords[2] + w.m_Coords[2]);
}

/*!
*	\brief	�� ���͸� ����.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ������ ���� ��ȯ�Ѵ�.
*/
EgVec3 operator -(const EgVec3 &v, const EgVec3 &w)
{
	return EgVec3(
		v.m_Coords[0] - w.m_Coords[0], 
		v.m_Coords[1] - w.m_Coords[1], 
		v.m_Coords[2] - w.m_Coords[2]);
}

/*!
*	\brief	�� ������ ������ ���Ѵ�.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ������ ���� ����� ��ȯ�Ѵ�.
*/
double operator *(const EgVec3 &v, const EgVec3 &w)
{
	return (v.m_Coords[0] * w.m_Coords[0] + 
			v.m_Coords[1] * w.m_Coords[1] + 
			v.m_Coords[2] * w.m_Coords[2]);
}

/*!
*	\brief	���͸� ����� �Ѵ�.
*
*	\param	v[in]	����
*	\param	s[in]	���
*
*	\return ������ ����踦 ��ȯ�Ѵ�.
*/
EgVec3 operator *(const EgVec3 &v, const double &s)
{
	EgVec3 ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	���͸� ����� �Ѵ�.
*
*	\param	s[in]	���
*	\param	v[in]	����
*
*	\return ������ ����踦 ��ȯ�Ѵ�.
*/
EgVec3 operator *(const double &s, const EgVec3 &v)
{
	EgVec3 ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	���͸� ����� ������.
*
*	\param	s[in]	���
*
*	\return ����� �������� ���͸� ��ȯ�Ѵ�.
*/
EgVec3 operator /(const EgVec3 &v, const double &s)
{
	EgVec3 ret(v);
	ret /= s;
	return ret;
}

/*!
*	\brief	�� ������ ���� ���͸� ���Ѵ�.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return ���� ���� (v X w)�� ��ȯ�Ѵ�.
*/
EgVec3 operator ^(const EgVec3 &v, const EgVec3 &w)
{
	return EgVec3(
		v.m_Coords[1] * w.m_Coords[2] -	v.m_Coords[2] * w.m_Coords[1], 
		v.m_Coords[2] * w.m_Coords[0] - v.m_Coords[0] * w.m_Coords[2], 
		v.m_Coords[0] * w.m_Coords[1] - v.m_Coords[1] * w.m_Coords[0]);
}

/*!
*	\brief	�� ���Ͱ� ������ �����Ѵ�.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ���Ͱ� ������ true, �ٸ��� false�� ��ȯ�Ѵ�.
*/
bool operator ==(const EgVec3 &v, const EgVec3 &w)
{
	return (EQ(v.m_Coords[0], w.m_Coords[0], MTYPE_EPS) && 
			EQ(v.m_Coords[1], w.m_Coords[1], MTYPE_EPS) && 
			EQ(v.m_Coords[2], w.m_Coords[2], MTYPE_EPS));
}

/*!
*	\brief	�� ���Ͱ� �ٸ��� �����Ѵ�.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ���Ͱ� �ٸ��� ture, ������ false�� ��ȯ�Ѵ�.
*/
bool operator !=(const EgVec3 &v, const EgVec3 &w)
{
	return (v == w) ? false : true;
}

/*!
*	\brief	�ε��� ������([])
*
*	\param	idx[in]	�ε���
*
*	\return ������ \a idx��° ���Ҹ� ��ȯ�Ѵ�.
*/
double &EgVec3::operator [](const int &idx)
{
	assert(idx >= 0 && idx < 3);
	return m_Coords[idx];
}

/*!
*	\brief	�����ü�� ���� �ε��� ������([])
*
*	\param	idx[in]	�ε���
*
*	\return ������ \a idx��° ���Ҹ� ��ȯ�Ѵ�.
*/
const double &EgVec3::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < 3);
	return m_Coords[idx];
}

/*!
*	\brief	��� ������(<<)
*
*	\param	os[out]		��� ��Ʈ��
*	\param	v[in]		����� ����
*
*	\return \a v�� ��µ� ��Ʈ���� ��ȯ�Ѵ�.
*/
std::ostream &operator <<(std::ostream &os, const EgVec3 &v)
{
	os << "(" << std::setw(7) << v.m_Coords[0] << ", " << std::setw(7) << v.m_Coords[1] << ", " << std::setw(7) << v.m_Coords[2] << ")";
	return os;
}

/*!
*	\brief	�Է¿� ����(>>)
*
*	\param	is[in]	�Է½�Ʈ��
*	\param	v[out]	�Է°��� ����� ����
*
*	\return �Է°��� ���ŵ� �Է½�Ʈ���� ��ȯ�Ѵ�.
*/
std::istream &operator >>(std::istream &is, EgVec3 &v)
{
	is >> v.m_Coords[0] >> v.m_Coords[1] >> v.m_Coords[2];
	return is;
}

/********************/
/* EgVec4 Ŭ���� ���� */
/********************/
/*!
*	\brief	������
*
*	\param	x[in]	������ x ��ǥ
*	\param	y[in]	������ y ��ǥ
*	\param	z[in]	������ z ��ǥ
*	\param	w[in]	������ w ��ǥ
*/
EgVec4::EgVec4(double x, double y, double z, double w)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
	m_Coords[2] = z;
	m_Coords[3] = w;
}

/*!
*	\brief	���� ������
*
*	\param	cpy[in]	����� ��ü
*/
EgVec4::EgVec4(const EgVec4 &cpy)
{
	m_Coords[0] = cpy.m_Coords[0];
	m_Coords[1] = cpy.m_Coords[1];
	m_Coords[2] = cpy.m_Coords[2];
	m_Coords[3] = cpy.m_Coords[3];
}

/*!
*	\brief	�Ҹ���
*/
EgVec4::~EgVec4()
{
}

/*!
*	\brief	������ ��ǥ�� �����Ѵ�.
*
*	\param	x[in]	������ x ��ǥ
*	\param	y[in]	������ y ��ǥ
*	\param	z[in]	������ z ��ǥ
*	\param	w[in]	������ w ��ǥ
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���͸� �������ͷ� ����ȭ�Ѵ�.
*
*	\return ����ȭ�� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���� ������
*
*	\param	rhs[in]		������ �ǿ�����
*
*	\return ���Ե� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���͸� ���Ѵ�.
*
*	\param	rhs[in]	���� ����
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���͸� ����.
*
*	\param	rhs[in]	�� ����
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���͸� ����� �Ѵ�.
*
*	\param	s[in]	���
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���͸� ����� ������.
*
*	\param	s[in]	���
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���� ������
*
*	\return ���Ϻ�ȣ�� ���� ��ü�� ��ȯ�Ѵ�.
*/
EgVec4 EgVec4::operator +() const
{
	return *this;
}

/*!
*	\brief	���� ������
*
*	\return �ݴ��ȣ�� ���� ��ü�� ��ȯ�Ѵ�.
*/
EgVec4 EgVec4::operator -() const
{
	return *this * -1;
}

/*!
*	\brief	�� ���͸� ���Ѵ�.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ������ ���� ��ȯ�Ѵ�.
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
*	\brief	�� ���͸� ����.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ������ ���� ��ȯ�Ѵ�.
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
*	\brief	�� ������ ������ ���Ѵ�.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ������ ������ ����� ��ȯ�Ѵ�.
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
*	\brief	���͸� ����� �Ѵ�.
*
*	\param	v[in]	����
*	\param	s[in]	���
*
*	\return ������ ����� ��ȯ�Ѵ�.
*/
EgVec4 operator *(const EgVec4 &v, const double &s)
{
	EgVec4 ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	���͸� ����� �Ѵ�.
*
*	\param	s[in]	���
*	\param	v[in]	����
*
*	\return ������ ����� ��ȯ�Ѵ�.
*/
EgVec4 operator *(const double &s, const EgVec4 &v)
{
	EgVec4 ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	���͸� ����� ������.
*
*	\param	v[in]	����
*	\param	s[in]	���
*
*	\return ������ ����� ��ȯ�Ѵ�.
*/
EgVec4 operator /(const EgVec4 &v, const double &s)
{
	EgVec4 ret(v);
	ret /= s;
	return ret;
}

/*!
*	\brief	�� ���Ͱ� ������ �˻��Ѵ�.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ���Ͱ� ���ٸ� true, �ٸ��� false�� ��ȯ�Ѵ�.
*/
bool operator ==(const EgVec4 &v, const EgVec4 &w)
{
	return (EQ(v.m_Coords[0], w.m_Coords[0], MTYPE_EPS) && 
			EQ(v.m_Coords[1], w.m_Coords[1], MTYPE_EPS) &&
			EQ(v.m_Coords[2], w.m_Coords[2], MTYPE_EPS) && 
			EQ(v.m_Coords[3], w.m_Coords[3], MTYPE_EPS));
}

/*!
*	\brief	�� ���Ͱ� �ٸ��� �˻��Ѵ�.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ���Ͱ� �ٸ��� true, ������ false�� ��ȯ�Ѵ�.
*/
bool operator !=(const EgVec4 &v, const EgVec4 &w)
{
	return (v == w) ? false : true;
}

/*!
*	\brief	�ε��� ������([])
*
*	\param	idx[in]	�ε���
*
*	\return	������ \a idx��° ���Ҹ� ��ȯ�Ѵ�.
*/
double &EgVec4::operator [](const int &idx)
{
	assert(idx >= 0 && idx < 4);
	return m_Coords[idx];
}

/*!
*	\brief	�����ü�� ���� �ε��� ������([])
*
*	\param	idx[in]	�ε���
*
*	\return	������ \a idx��° ���Ҹ� ��ȯ�Ѵ�.
*/
const double &EgVec4::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < 4);
	return m_Coords[idx];
}

/*!
*	\brief	��� ������(<<)
*
*	\param	os[out]		��� ��Ʈ��
*	\param	v[in]		��� ����
*
*	\return \a v�� ��µ� ��Ʈ���� ��ȯ�Ѵ�.
*/
std::ostream &operator <<(std::ostream &os, const EgVec4 &v)
{
	os << "(" << std::setw(7) << v.m_Coords[0] << ", " << std::setw(7) << v.m_Coords[1] << ", "
			  << std::setw(7) << v.m_Coords[2] << "," << std::setw(7) << v.m_Coords[3] << ")";
	return os;
}

/*!
*	\brief	�Է� ������(>>)
*
*	\param	is[in]	�Է� ��Ʈ��
*	\param	v[out]	�Է°��� ����� ����
*
*	\return �Է°��� ���ŵ� �Է½�Ʈ���� ��ȯ�Ѵ�.
*/
std::istream &operator >>(std::istream &is, EgVec4 &v)
{
	is >> v.m_Coords[0] >> v.m_Coords[1] >> v.m_Coords[2] >> v.m_Coords[3];
	return is;
}

/*********************/
/* EgVec Ŭ���� ���� */
/*********************/
/*!
*	\brief	������
*
*	\param	dim[in]	������ ����
*/
EgVec::EgVec(int dim)
{
	m_Coords.assign(dim, 0.0);
}

/*!
*	\brief	������
*	\note	���Ҵ� ��� �Ǽ������� �����ؾ� ��
*
*	\param	dim[in]	������ ����
*	\param	x[in]	������ ù ��° ��ǥ
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
*	\brief	������
*	\note	���Ҵ� ��� ���������� �����ؾ� ��
*
*	\param	dim[in]		������ ����
*	\param	x[in]		������ ù ��° ��ǥ
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
*	\brief	���� ������
*
*	\param	copy[in]	����� ��ü
*/
EgVec::EgVec(const EgVec &cpy)
{
	m_Coords = cpy.m_Coords;
}

/*!
*	\brief	�Ҹ���
*/
EgVec::~EgVec()
{
}

/*!
*	\brief	������ ���Ҹ� �����Ѵ�.
*	\note	�Ű������� �Ǽ������� �־�� �Ѵ�. ��) 1: �Ұ���, 1.0: ����.
*
*	\param	x[in]	ù ��° ��ǥ
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	������ ���Ҹ� �����Ѵ�.
*	\note	�Ű������� ���������� �־�� �Ѵ�. ��) 1: ����, 1.0: �Ұ���.
*
*	\param	x[in]	ù ��° ��ǥ
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	������ ���Ҹ� �����Ѵ�.
*
*	\param	Coords[in]	������ ��ǥ �迭
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
*/
EgVec &EgVec::Set(double *Coords)
{
	for (int i = 0; i < (int)m_Coords.size(); ++i)
		m_Coords[i] = Coords[i];
	return *this;
}

/*!
*	\brief	���͸� ����ȭ�Ѵ�.
*
*	\return ����ȭ�� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���͸� �����ͷ� �����Ѵ�.
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
*/
EgVec &EgVec::SetZeros()
{
	for (int i = 0; i < (int)m_Coords.size(); ++i)
		m_Coords[i] = 0.0;
	return *this;
}

/*!
*	\brief	������ ������ ��ȯ�Ѵ�.
*
*	\return ������ ������ ��ȯ�Ѵ�.
*/
int EgVec::GetDim() const
{
	return (int)m_Coords.size();
}

/*!
*	\brief	���� ������
*
*	\param	rhs[in]	������ �ǿ�����
*
*	\return ���Ե� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec &EgVec::operator =(const EgVec &rhs)
{
	m_Coords = rhs.m_Coords;
	return *this;
}

/*!
*	\brief	���͸� ���Ѵ�.
*
*	\param	rhs[in]	���� ����
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec &EgVec::operator +=(const EgVec &rhs)
{
	assert(m_Coords.size() == rhs.m_Coords.size());
	for (int i = 0; i < (int)m_Coords.size(); ++i)
		m_Coords[i] += rhs.m_Coords[i];
	return *this;
}

/*!
*	\brief	���͸� ����.
*
*	\param	rhs[in]	�� ����
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec &EgVec::operator -=(const EgVec &rhs)
{
	assert(m_Coords.size() == rhs.m_Coords.size());
	for (int i = 0; i < (int)m_Coords.size(); ++i)
		m_Coords[i] -= rhs.m_Coords[i];
	return *this;
}

/*!
*	\brief	���͸� ����� �Ѵ�.
*
*	\param	s[in]	���
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec &EgVec::operator *=(const double &s)
{
	for (int i = 0; i < (int)m_Coords.size(); ++i)
		m_Coords[i] *= s;
	return *this;
}

/*!
*	\brief	���͸� ����� ������.
*
*	\param	s[in]	���
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgVec &EgVec::operator /=(const double &s)
{
	assert(s != 0);
	for (int i = 0; i < (int)m_Coords.size(); ++i)
		m_Coords[i] /= s;
	return *this;
}

/*!
*	\brief	���� ������
*
*	\return ���Ϻ�ȣ ��ü�� ��ȯ�Ѵ�.
*/
EgVec EgVec::operator +() const
{
	return *this;
}

/*!
*	\brief	���� �������ߺ�
*
*	\return �ݴ��ȣ ��ü�� ��ȯ�Ѵ�.
*/
EgVec EgVec::operator -() const
{
	return *this * -1;
}

/*!
*	\brief	�� ���͸� ���Ѵ�.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ������ ���� ��ȯ�Ѵ�.
*/
EgVec operator +(const EgVec &v, const EgVec &w)
{
	EgVec ret(v);
	ret += w;
	return ret;
}

/*!
*	\brief	�� ���͸� ����.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ������ ���� ��ȯ�Ѵ�.
*/
EgVec operator -(const EgVec &v, const EgVec &w)
{
	EgVec ret(v);
	ret -= w;
	return ret;
}

/*!
*	\brief	�� ������ ������ ���Ѵ�.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ������ �������� ��ȯ�Ѵ�.
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
*	\brief	���͸� ����� �Ѵ�.
*
*	\param	v[in]	����
*	\param	s[in]	���
*
*	\return ������ ����踦 ��ȯ�Ѵ�.
*/
EgVec operator *(const EgVec &v, const double &s)
{
	EgVec ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	���͸� ����� �Ѵ�.
*
*	\param	s[in]	���
*	\param	v[in]	����
*
*	\return ������ ����踦 ��ȯ�Ѵ�.
*/
EgVec operator *(const double &s, const EgVec &v)
{
	EgVec ret(v);
	ret *= s;
	return ret;
}

/*!
*	\brief	���͸� ����� ������.
*
*	\param	v[in]	����
*	\param	s[in]	���
*
*	\return ������ ����� ��ȯ�Ѵ�.
*/
EgVec operator /(const EgVec &v, const double &s)
{
	EgVec ret(v);
	ret /= s;
	return ret;
}

/*!
*	\brief	�� ������ ũ�⸦ ���Ѵ�.
*
*	\param	v[in]	���� ����
*	\param	w[in]	������ ����
*
*	\return ���� ������ ũ�Ⱑ ������ ���ͺ��� ũ�� true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool operator >(const EgVec &v, const EgVec &w)
{
	return (norm_sq(v) > norm_sq(w)) ? true : false;
}

/*!
*	\brief	�� ������ ũ�⸦ ���Ѵ�.
*
*	\param	v[in]	���� ����
*	\param	w[in]	������ ����
*
*	\return ���� ������ ũ�Ⱑ ������ ���ͺ��� ũ�ų� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool operator >=(const EgVec &v, const EgVec &w)
{
	return (v > w || v == w) ? true : false;
}

/*!
*	\brief	�� ������ ũ�⸦ ���Ѵ�.
*
*	\param	v[in]	���� ����
*	\param	w[in]	������ ����
*
*	\return ���� ������ ũ�Ⱑ ������ ���ͺ��� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool operator <(const EgVec &v, const EgVec &w)
{
	return (norm_sq(v) < norm_sq(w)) ? true : false;
}

/*!
*	\brief	�� ������ ũ�⸦ ���Ѵ�.
*
*	\param	v[in]	���� ����
*	\param	w[in]	������ ����
*
*	\return ���� ������ ũ�Ⱑ ������ ���ͺ��� �۰ų� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool operator <=(const EgVec &v, const EgVec &w)
{
	return (v < w || v == w) ? true : false;
}

/*!
*	\brief	�� ���Ͱ� ������ �����Ѵ�.
*
*	\param	v[in]	���� ����
*	\param	w[in]	������ ����
*
*	\return �� ���Ͱ� ������ true, �ٸ��� false�� ��ȯ�Ѵ�.
*/
bool operator ==(const EgVec &v, const EgVec &w)
{
	assert(v.m_Coords.size() == w.m_Coords.size());
	for (int i = 0; i < (int)v.m_Coords.size(); ++i)
		if (!EQ(v.m_Coords[i], w.m_Coords[i], MTYPE_EPS))
			return false;	// �� ���Ҷ� �ٸ��� false ��ȯ
	return true;			// ��� ������ true ��ȯ
}

/*!
*	\brief	�� ���Ͱ� �ٸ��� �����Ѵ�.
*
*	\param	v[in]	���� ����
*	\param	w[in]	������ ����
*
*	\return �� ���Ͱ� �ٸ��� true, ������ false�� ��ȯ�Ѵ�.
*/
bool operator !=(const EgVec &v, const EgVec &w)
{
	return (v == w) ? false : true;
}

/*!
*	\brief	�ε��� ������([])
*
*	\param	idx[in]	�ε���
*
*	\return ������ \a idx ��° ���Ҹ� ��ȯ�Ѵ�.
*/
double &EgVec::operator [](const int &idx)
{
	assert(idx >= 0 && idx < (int)m_Coords.size());
	return m_Coords[idx];
}

/*!
*	\brief	�����ü�� �ε��� ������([])
*
*	\param	idx[in]	�ε���
*
*	\return ������ \a idx ��° ���Ҹ� ��ȯ�Ѵ�.
*/
const double &EgVec::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < (int)m_Coords.size());
	return m_Coords[idx];
}

/*!
*	\brief	��� ������(<<)
*
*	\param	os[out]		��� ��Ʈ��
*	\param	v[in]		����� ����
*
*	\return \a v�� ��µ� ��Ʈ���� ��ȯ�Ѵ�.
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
*	\brief	�Է� ������(>>)
*
*	\param	is[in]	�Է� ��Ʈ��
*	\param	v[out]	�Է°��� ����� ����
*
*	\return �Է°��� ���ŵ� �Է½�Ʈ���� ��ȯ�Ѵ�.
*/
std::istream &operator >>(std::istream &is, EgVec &v)
{
	for (int i = 0; i < (int)v.m_Coords.size(); ++i)
		is >> v.m_Coords[i];
	return is;
}

/********************/
/* EgPos Ŭ���� ���� */
/********************/
/*!
*	\brief	������
*
*	\param	x[in]	x ��ǥ
*	\param	y[in]	y ��ǥ
*	\param	z[in]	z ��ǥ
*/
EgPos::EgPos(double x, double y, double z)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
	m_Coords[2] = z;
}

/*!
*	\brief	���� ������
*
*	\param	cpy[in] ����� ��ü
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgPos::EgPos(const EgPos &cpy)
{
	m_Coords[0] = cpy.m_Coords[0];
	m_Coords[1] = cpy.m_Coords[1];
	m_Coords[2] = cpy.m_Coords[2];
}

/*!
*	\brief  �Ҹ���
*/
EgPos::~EgPos()
{
}

/*!
*	\brief	����Ʈ�� ��ǥ�� �����Ѵ�.
*
*	\param	x[in] x ��ǥ
*	\param	y[in] y ��ǥ
*	\param	z[in] z ��ǥ
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
*/
EgPos &EgPos::Set(double x, double y, double z)
{
	m_Coords[0] = x;
	m_Coords[1] = y;
	m_Coords[2] = z;
	return *this;
}

/*!
*	\brief	���� ������
*
*	\param	rhs[in]		������ �ǿ�����
*
*	\return ���Ե� �ڽ��� ��ȯ�Ѵ�.
*/
EgPos &EgPos::operator =(const EgPos &rhs)
{
	m_Coords[0] = rhs.m_Coords[0];
	m_Coords[1] = rhs.m_Coords[1];
	m_Coords[2] = rhs.m_Coords[2];
	return *this;
}

/*!
*	\brief	���͸� ���Ѵ�.
*
*	\param	v[in]	���� ����
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgPos &EgPos::operator +=(const EgVec3 &v)
{
	m_Coords[0] += v.m_Coords[0];
	m_Coords[1] += v.m_Coords[1];
	m_Coords[2] += v.m_Coords[2];
	return *this;
}

/*!
*	\brief	���͸� ����.
*
*	\param	v[in]	�� ����
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgPos &EgPos::operator-=(const EgVec3 &v)
{
	m_Coords[0] -= v.m_Coords[0];
	m_Coords[1] -= v.m_Coords[1];
	m_Coords[2] -= v.m_Coords[2];
	return *this;
}

/*!
*	\brief	�ε��� ������([])
*
*	\param	idx[in] �ε���
*
*	\return ����Ʈ�� \a idx ��° ���Ҹ� ��ȯ�Ѵ�.
*/
double &EgPos::operator [](const int &idx)
{
	assert(idx >= 0 && idx < 3);
	return m_Coords[idx];
}

/*!
*	\brief	�����ü�� �ε��� ������([])
*
*	\param	idx[in] �ε���
*
*	\return ����Ʈ�� \a idx ��° ���Ҹ� ��ȯ�Ѵ�.
*/
const double &EgPos::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < 3);
	return m_Coords[idx];
}

/*!
*	\brief	�� ��ġ�� ���� ���͸� ���Ѵ�.
*
*	\param	p[in]	ù ��° ��ġ
*	\param	q[in]	�� ��° ��ġ
*
*	\return q���� p�� ���ϴ� ���͸� ��ȯ�Ѵ�.
*/
EgVec3 operator -(const EgPos &p, const EgPos &q)
{
	return EgVec3(
		p.m_Coords[0] - q.m_Coords[0], 
		p.m_Coords[1] - q.m_Coords[1], 
		p.m_Coords[2] - q.m_Coords[2]);
}

/*!
*	\brief	��ġ���� ���͸� ����.
*
*	\param	p[in]	��ġ
*	\param	v[in]	����
*
*	\return ���ο� ��ġ�� ��ȯ�Ѵ�.
*/
EgPos operator -(const EgPos &p, const EgVec3 &v)
{
	return EgPos(
		p.m_Coords[0] - v.m_Coords[0], 
		p.m_Coords[1] - v.m_Coords[1], 
		p.m_Coords[2] - v.m_Coords[2]);
}

/*!
*	\brief	��ġ���� ���͸� ���Ѵ�.
*
*	\param	p[in]	��ġ
*	\param	v[in]	����
*
*	\return ���ο� ��ġ�� ��ȯ�Ѵ�.
*/
EgPos operator +(const EgPos &p, const EgVec3 &v)
{
	return EgPos(
		p.m_Coords[0] + v.m_Coords[0], 
		p.m_Coords[1] + v.m_Coords[1], 
		p.m_Coords[2] + v.m_Coords[2]);
}

/*!
*	\brief	��ġ���� ���͸� ���Ѵ�.
*
*	\param	v[in]	����
*	\param	p[in]	��ġ
*
*	\return ���ο� ��ġ�� ��ȯ�Ѵ�.
*/
EgPos operator +(const EgVec3 &v, const EgPos &p)
{
	return EgPos(
		p.m_Coords[0] + v.m_Coords[0], 
		p.m_Coords[1] + v.m_Coords[1], 
		p.m_Coords[2] + v.m_Coords[2]);
}

/*!
*	\brief	�� ��ġ�� ������ �����Ѵ�.
*
*	\param	p[in]	ù ��° ��ġ
*	\param	q[in]	�� ���� ��ġ
*
*	\return �� ��ġ�� ������ true, �ٸ��� false�� �����Ѵ�.
*/
bool operator ==(const EgPos &p, const EgPos &q)
{
	return (EQ(p.m_Coords[0], q.m_Coords[0], MTYPE_EPS) && 
			EQ(p.m_Coords[1], q.m_Coords[1], MTYPE_EPS) && 
			EQ(p.m_Coords[2], q.m_Coords[2], MTYPE_EPS));
}

/*!
*	\brief	�� ��ġ�� ������ �����Ѵ�.
*
*	\param	p[in]	ù ��° ��ġ
*	\param	q[in]	�� ���� ��ġ
*
*	\return �� ��ġ�� �ٸ��� true, ������ false�� �����Ѵ�.
*/
bool operator !=(const EgPos &p, const EgPos &q)
{
	return (p == q) ? false : true;
}

/*!
*	\brief	��� ������(<<)
*
*	\param	os[out]		��� ��Ʈ��
*	\param	p[in]		����� ��ü
*
*	\return ��µ� ��Ʈ�� ��ü�� ��ȯ�Ѵ�.
*/
std::ostream &operator <<(std::ostream &os, const EgPos &p)
{
	os << "(" << std::setw(5) << p.m_Coords[0] << ", " << std::setw(5) << p.m_Coords[1] << ", " << std::setw(5) << p.m_Coords[2] << ")";
	return os;
}

/*!
*	\brief	�Է� ������(>>)
*
*	\param	is[in]	�Է½�Ʈ��
*	\param	v[out]	�Է°��� ����� ����
*
*	\return �Է°��� ���ŵ� �Է½�Ʈ���� ��ȯ�Ѵ�.
*/
std::istream &operator >>(std::istream &is, EgPos &p)
{
	is >> p.m_Coords[0] >> p.m_Coords[1] >> p.m_Coords[2];
	return is;
}

/*******************/
/* EgMat Ŭ���� ���� */
/*******************/
/*!
*	\brief	������
*
*	\param	r[in]		���� ��
*	\param	c[in]		���� ��
*	\param	elem[in]	����� ���� �迭
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
*	\brief	���� ������
*
*	\param	cpy[in]	����� ��ü
*/
EgMat::EgMat(const EgMat &cpy)
{
	m_nRow = cpy.m_nRow;
	m_nCol = cpy.m_nCol;
	m_Data = cpy.m_Data;
}

/*!
*	\brief �Ҹ���
*/
EgMat::~EgMat()
{
}

/*!
*	\brief	����� ��ġ��ķ� ��ȯ�Ѵ�.
*
*	\return ��ȯ�� �ڽ��� ��ȯ�Ѵ�.
*/
EgMat &EgMat::SetTranspose()
{
	// ������� �̶��
	if (m_nRow == m_nCol)
	{
		for (int i = 0; i < m_nRow; ++i)
			for (int j = i + 1; j < m_nCol; ++j)
				SWAP(double, m_Data[i * m_nCol + j], m_Data[j * m_nCol + i]);
	}
	else // �簢��� �̶��
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
*	\brief	������ķ� ��ȯ�Ѵ�.
*	\note	��������� �ƴϸ� �ִ��� ������ķ� �����.
*
*	\return ��ȯ�� �ڽ��� �����Ѵ�.
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
*	\brief	����ķ� ��ȯ�Ѵ�.
*
*	\return ��ȯ�� �ڽ��� �����Ѵ�.
*/
EgMat &EgMat::SetZeros()
{
	m_Data.assign(m_nRow * m_nCol, 0.0);
	return *this;
}

/*!
*	\brief	�຤�͸� �����Ѵ�.
*
*	\param	idx[in]		������ ���� �ε���
*	\param	v[in]		������ ����
*
*	\return ��ȯ�� �ڽ��� �����Ѵ�.
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
*	\brief	�����͸� �����Ѵ�.
*
*	\param	idx[in]		������ ���� �ε���
*	\param	v[in]		������ ����
*
*	\return ��ȯ�� �ڽ��� �����Ѵ�.
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
*	\brief	�� ���� ��ȯ�Ѵ�.
*
*	\param	idx0[in]	��ȯ�� ���� �ε���
*	\param	idx1[in]	��ȯ�� ���� �ε���
*
*	\return ��ȯ�� �ڽ��� �����Ѵ�.
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

*	\brief	�� ���� ��ȯ�Ѵ�.
*
*	\param	idx0[in]	��ȯ�� ���� �ε���
*	\param	idx1[in]	��ȯ�� ���� �ε���
*
*	\return ��ȯ�� �ڽ��� �����Ѵ�.
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
*	\brief	�຤�͸� ��ȯ�Ѵ�.
*
*	\param	idx[in]		���� �ε���
*
*	\return idx ��° �຤�͸� ��ȯ�Ѵ�.
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
*	\brief	�����͸� ��ȯ�Ѵ�.
*
*	\param	idx[in]		���� �ε���
*
*	\return idx ��° �����͸� ��ȯ�Ѵ�.
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
*	\brief	���� ���� ��ȯ�Ѵ�.
*
*	\return ���� ���� ��ȯ�Ѵ�.
*/
int EgMat::GetRowNum() const
{
	return m_nRow;
}

/*!
*	\brief	���� ���� ��ȯ�Ѵ�.
*
*	\return ���� ���� ��ȯ�Ѵ�.
*/
int EgMat::GetColNum() const
{
	return m_nCol;
}

/*!
*	\brief	������� ���θ� �����Ѵ�.
*
*	\return ��������� ��� true, ���簢����� ��� false�� ��ȯ�Ѵ�.
*/
bool EgMat::IsSquare() const
{
	return (m_nRow == m_nCol) ? true : false;
}

/*!
*	\brief	���� ������
*
*	\param	rhs[in] ���Ե� ��� ��ü
*
*	\return ���Ե� �ڽ��� ��ȯ�Ѵ�.
*/
EgMat &EgMat::operator =(const EgMat &rhs)
{
	m_nRow = rhs.m_nRow;
	m_nCol = rhs.m_nCol;
	m_Data = rhs.m_Data;
	return *this;
}

/*!
*	\brief	����� ���Ѵ�.
*
*	\param	rhs[in]	���� ���
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
*/
EgMat &EgMat::operator +=(const EgMat &rhs)
{
	assert(m_nRow == rhs.m_nRow && m_nCol == rhs.m_nCol);
	for (int i = 0; i < (int)m_Data.size(); ++i)
		m_Data[i] += rhs.m_Data[i];
	return *this;
}

/*!
*	\brief	����� ����.
*
*	\param	rhs[in]	�� ���
*
*	\return ������ ���Ե� �ڽ��� ��ȯ�Ѵ�.
*/
EgMat &EgMat::operator -=(const EgMat &rhs)
{
	assert(m_nRow == rhs.m_nRow && m_nCol == rhs.m_nCol);
	for (int i = 0; i < (int)m_Data.size(); ++i)
		m_Data[i] -= rhs.m_Data[i];
	return *this;
}

/*!
*	\brief	����� ���Ѵ�.
*
*	\param	rhs[in]	���� ���
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	����� ����� �Ѵ�.
*
*	\param	s[in]	���
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgMat &EgMat::operator *=(const double &s)
{
	for (int i = 0; i < (int)m_Data.size(); i++)
		m_Data[i] *= s;
	return *this;
}

/*!
*	\brief	����� ����� ������.
*
*	\param	s[in]	���
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgMat &EgMat::operator /=(const double &s)
{
	for (int i = 0; i < (int)m_Data.size(); i++)
		m_Data[i] /= s;
	return *this;
}

/*!
*	\brief	���� ������
*
*	\return ���Ϻ�ȣ�� ���� ��ü�� ��ȯ�Ѵ�.
*/
EgMat EgMat::operator +() const
{
	return *this;
}

/*!
*	\brief	���� ������
*
*	\return �ݴ��ȣ�� ���� ��ü�� ��ȯ�Ѵ�.
*/
EgMat EgMat::operator -() const
{
	return *this * -1;
}

/*!
*	\brief	�ε��� ������([])
*
*	\param	idx[in]		���� �ε���
*
*	\return \a idx ���� �迭�� ���� �ּҸ� ��ȯ�Ѵ�.
*/
double *EgMat::operator [](const int idx)
{
	assert(idx >= 0 && idx < m_nRow);
	return &m_Data[idx * m_nCol];
}

/*!
*	\brief	�����ü�� �ε��� ������([])
*
*	\param	idx[in]		���� �ε���
*
*	\return \a idx ���� �迭�� ���� �ּҸ� ��ȯ�Ѵ�.
*/
const double *EgMat::operator [](const int idx) const
{
	assert(idx >= 0 && idx < m_nRow);
	return &m_Data[idx * m_nCol];
}

/*!
*	\brief	�� ����� ���Ѵ�.
*
*	\param	A[in]	ù ��° ���
*	\param	B[in]	�� ��° ���
*
*	\return �� ����� ���� ��ȯ�Ѵ�.
*/
EgMat operator +(const EgMat &A, const EgMat &B)
{
	EgMat ret(A);
	ret += B;
	return ret;
}

/*!
*	\brief	�� ����� ����.
*
*	\param	A[in]	ù ��° ���
*	\param	B[in]	�� ��° ���
*
*	\return �� ����� ���� ��ȯ�Ѵ�.
*/
EgMat operator -(const EgMat &A, const EgMat &B)
{
	EgMat ret(A);
	ret -= B;
	return ret;
}

/*!
*	\brief	�� ����� ���Ѵ�.
*
*	\param	A[in]	ù ��° ���
*	\param	B[in]	�� ��° ���
*
*	\return �� ����� ���� ��ȯ�Ѵ�.
*/
EgMat operator *(const EgMat &A, const EgMat &B)
{
	EgMat ret(A);
	ret *= B;
	return ret;
}

/*!
*	\brief	����� ����� �Ѵ�.
*
*	\param	M[in]	���
*	\param	s[in]	���
*
*	\return ����� ����� ��ȯ�Ѵ�.
*/
EgMat operator *(const EgMat &M, const double &s)
{
	EgMat ret(M);
	ret *= s;
	return ret;
}

/*!
*	\brief	����� ����� �Ѵ�.
*
*	\param	s[in]	���
*	\param	M[in]	���
*
*	\return ����� ����� ��ȯ�Ѵ�.
*/
EgMat operator *(const double &s, const EgMat &M)
{
	EgMat ret(M);
	ret *= s;
	return ret;
}

/*!
*	\brief	��İ� ������ ���� ���Ѵ�.
*
*	\param	M[in]	���
*	\param	v[in]	����
*
*	\return ��İ� ������ ��(����)�� ��ȯ�Ѵ�.
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
*	\brief	���Ϳ� ����� ���Ѵ�.
*
*	\param	v[in]	n by 1 ����
*	\param	M[in]	1 by m ���
*
*	\return ���Ϳ� ����� ��(���)�� ��ȯ�Ѵ�.
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
*	\brief	����� ����� ������.
*
*	\param	M[in]	���
*	\param	s[in]	���
*
*	\return ����� ����� ��ȯ�Ѵ�.
*/
EgMat operator /(const EgMat &M, const double &s)
{
	EgMat ret(M);
	ret /= s;
	return ret;
}

/*!
*	\brief	�� ����� ������ �˻��Ѵ�.
*
*	\param	A[in]	���� ���
*	\param	B[in]	������ ���
*
*	\return �� ����� ������ true, �ٸ��� false�� ��ȯ�Ѵ�.
*/
bool operator ==(const EgMat &A, const EgMat &B)
{
	assert(A.m_nRow == B.m_nRow && A.m_nCol == B.m_nCol);
	for (int i = 0; i < A.m_nRow * A.m_nCol; ++i)
		if (!EQ(A.m_Data[i], B.m_Data[i], MTYPE_EPS))
			return false;	// �ϳ��� ���Ҷ� �ٸ��� flase�� ��ȯ�Ѵ�.
	return true;			// ��� ���Ұ� �����Ƿ� true�� ��ȯ�Ѵ�.
}

/*!
*	\brief	�� ����� �ٸ��� �˻��Ѵ�.
*
*	\param	A[in]	���� �ǿ�����
*	\param	B[in]	������ �ǿ�����
*
*	\return �� ����� �ٸ��� true, ������ false�� ��ȯ�Ѵ�.
*/
bool operator !=(const EgMat &A, const EgMat &B)
{
	return (A == B) ? false : true;
}

/*!
*	\brief	��� ������(<<)
*
*	\param	os[out]	��� ��Ʈ��
*	\param	M[in]	����� ���
*
*	\return ��µ� ��Ʈ�� ��ü�� ��ȯ�Ѵ�.
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
/* EgPos Ŭ���� ���� */
/********************/
/*!
*	\brief	���ʹϿ� q = w + xi + yj + zk �� �����ϴ� ������
*
*	\param	w[in]	�Ǽ���
*	\param	x[in]	i �����
*	\param	y[in]	j �����
*	\param	z[in]	k �����
*/
EgQuat::EgQuat(double w, double x, double y, double z)
{
	m_W = w;
	m_X = x;
	m_Y = y;
	m_Z = z;
}

/*!
*	\brief	���� ������
*
*	\param	cpy[in]	����� ��ü
*/
EgQuat::EgQuat(const EgQuat &cpy)
{
	m_W = cpy.m_W;
	m_X = cpy.m_X;
	m_Y = cpy.m_Y;
	m_Z = cpy.m_Z;
}

/*!
*	\brief	������
*
*	\param	q[in]	������ ���ʹϿ��� ���Ҹ� ������ �迭
*/
EgQuat::EgQuat(const double *q)
{
	m_W = q[0];
	m_X = q[1];
	m_Y = q[2];
	m_Z = q[3];	
}

/*!
*	\brief	ȸ���ฦ �߽����� ȸ���ϴ� �������ʹϿ��� �����Ѵ�.
*
*	\param	axis[in]	ȸ���� �߽���
*	\param	theta[in]	ȸ������
*	\param	radian[in]	ȸ�������� �����̸� true, �ƴϸ� false
*/
EgQuat::EgQuat(EgVec3 axis, double theta, bool radian)
{
	double rad, sn, cs;
	axis.Normalize();

	// �������� ǥ���� ������ ���Ѵ�.
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
*	\brief	���Ϸ���(degree) �̿��Ͽ� ���ʹϿ��� �����Ѵ�.
*
*	\param	theta1[in]		ù ��°�࿡ ���� ȸ������(degree)
*	\param	theta2[in]		�� ��°�࿡ ���� ȸ������(degree)
*	\param	theta3[in]		�� ��°�࿡ ���� ȸ������(degree)
*	\param	eulerType[in]	���Ϸ���(XYZ = RxRyRz, ZYX = RzRyRx, ...)
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
*	\brief	������
*
*	\param	x_axis[in]	��ǥ���� x�� ����
*	\param	y_axis[in]	��ǥ���� y�� ����
*/
EgQuat::EgQuat(EgVec3 x_axis, EgVec3 y_axis)
{
	SetFromFrameXY(x_axis, y_axis);
}

/*!
*	\brief	�Ҹ���
*/
EgQuat::~EgQuat()
{
}

/*!
*	\brief	�Ǽ��ο� ������� ���� �����Ѵ�.
*
*	\param	w[in]	���ʹϿ��� �Ǽ���
*	\param	x[in]	���ʹϿ��� i �����
*	\param	y[in]	���ʹϿ��� j �����
*	\param	z[in]	���ʹϿ��� k �����
*
*	\return	������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	�Ǽ��ο� ������� ���� �����Ѵ�.
*
*	\param	q[in]			������ ���� �迭
*	\param	invOrder[in]	true: (w, x, y, z) = (q[3], q[0], q[1], q[2]), false: (w, x, y, z) = (q[0], q[1], q[2], q[3])
*
*	\return	������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	ȸ������ �߽����� �Է°��� ��ŭ ȸ���ϴ� ���ʹϿ��� �����Ѵ�.
*
*	\param	theta[in]	�Է°���
*	\param	axis[in]	ȸ����
*	\param	radian[in]	�Է°����� �����̸� true, �ƴϸ� false
*
*	\return ������ �ڽ��� �����Ѵ�.
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
*	\brief	��ũ�� ���� �� p�� �� q�� ȸ���ϴ� ���ʹϿ��� �����Ѵ�.
*
*	\param	p[in]		��ũ�� ���� ��
*	\param	q[in]		��ũ�� ���� ��
*	\param	arcType[in]	��ũ���� Ÿ��
*
*	\return ������ �ڽ��� �����Ѵ�.
*/
EgQuat &EgQuat::SetFromArcBall(EgVec3 p, EgVec3 q, TypeArcBall arcType)
{
	double ang, nm;
	EgVec3 axis;

	p.Normalize();
	q.Normalize();

	double dot = p * q;
	if (EQ(dot, -1.0, MTYPE_EPS))	// p = -q �ΰ��
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

	if (EQ_ZERO(nm, MTYPE_EPS))	// p == q �ΰ��
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
*	\brief	���Ϸ���(degree) �̿��Ͽ� ���ʹϿ��� �����Ѵ�.
*
*	\param	theta1[in]		ù ��°�࿡ ���� ȸ������(degree)
*	\param	theta2[in]		�� ��°�࿡ ���� ȸ������(degree)
*	\param	theta3[in]		�� ��°�࿡ ���� ȸ������(degree)
*	\param	eulerType[in]	���Ϸ���(XYZ = RxRyRz, ZYX = RzRyRx)
*
*	\return	������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	��ǥ���� �� ������ ���ʹϿ��� �����Ѵ�.
*
*	\param	x_axis[in]	��ǥ���� x ��
*	\param	y_axis[in]	��ǥ���� y ��
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	��ǥ���� �� ������ ���ʹϿ��� �����Ѵ�.
*
*	\param	y_axis[in]	��ǥ���� y ��
*	\param	z_axis[in]	��ǥ���� z ��
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	��ǥ���� �� ������ ���ʹϿ��� �����Ѵ�.
*
*	\param	z_axis[in]	��ǥ���� z ��
*	\param	x_axis[in]	��ǥ���� x ��
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	ȸ����ķ� ���ʹϿ��� �����Ѵ�.
*
*	\param	mat[in]		4 by 4 ��ȯ����� ǥ���ϴ� �迭
*	\parma	isGL[in]	OpenGL�� ��ȯ����̸� true, �ƴϸ� false
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���ʹϿ��� �׵������ �����Ѵ�.
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���ʹϿ��� �������� �����Ѵ�.
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���ʹϿ��� conjugate �Ѵ�.
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
*/
EgQuat &EgQuat::SetConjugate()
{
	m_X = -m_X;
	m_Y = -m_Y;
	m_Z = -m_Z;
	return *this;
}

/*!
*	\brief	���ʹϿ��� negate �Ѵ�.
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���ʹϿ��� �������ʹϿ����� �����Ѵ�.
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	����ȸ����� ȸ������ ���Ѵ�.
*
*	\param	axis[out]	����ȸ������ �����
*	\param	angle[out]	ȸ������ �����
*	\param	radian[in]	ȸ������ �������� ���Ϸ��� true, �ƴϸ� false
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
*	\brief	ȸ���� ���Ϸ� ǥ���� ���Ѵ�.
*
*	\param	theta1[out]		ù ��° ȸ������ ������ �����
*	\param	theta2[out]		�� ��° ȸ������ ������ �����
*	\param	theta3[out]		�� ��° ȸ������ ������ �����
*	\param	radian[in]		����Ǵ� ȸ������ �����̸� true, �ƴϸ� false
*	\param	eulerType[in]	ȸ������ ����(EULER_XYZ �Ǵ� EULER_ZYX)
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
*	\brief	�������ʹϿ� ���θ� �����Ѵ�.
*
*	\return �������ʹϿ��̸� true �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool EgQuat::IsUnitQuater() const
{
	double norm = SQR(m_W) + SQR(m_X) + SQR(m_Y) + SQR(m_Z);
	return EQ(norm, 1.0, MTYPE_EPS) ? true : false;
}

/*!
*	\brief	���� ���ʹϿ��� �׵������ ���θ� �����Ѵ�.
*
*	\return �׵���̸� true, �ƴϸ� false�� ��ȯ�Ѵ�.
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
*	\brief	���ʹϿ��� W ������ ��ȯ�Ѵ�.
*
*	\return ���ʹϿ��� W ����
*/
double EgQuat::W() const
{
	return m_W;
}

/*!
*	\brief	���ʹϿ��� X ������ ��ȯ�Ѵ�.
*
*	\return ���ʹϿ��� X ����
*/
double EgQuat::X() const
{
	return m_X;
}

/*!
*	\brief	���ʹϿ��� Y ������ ��ȯ�Ѵ�.
*
*	\return ���ʹϿ��� Y ����
*/
double EgQuat::Y() const
{
	return m_Y;
}

/*!
*	\brief	���ʹϿ��� Z ������ ��ȯ�Ѵ�.
*
*	\return ���ʹϿ��� Z ����
*/
double EgQuat::Z() const
{
	return m_Z;
}

/*!
*	\brief	���� ������
*
*	\param	rhs[in]		���Ե� ��ü
*
*	\return ���Ե� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���ʹϿ��� ���Ѵ�.
*
*	\param	rhs[in]		���� ��ü
*
*	\return ������ ���Ե� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���ʹϿ��� ����.
*
*	\param	rhs[in]		�� ��ü
*
*	\return ������ ���Ե� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	�� ���ʹϿ��� ���� ���Ѵ�.
*
*	\param	rhs[in]		���� ��ü
*
*	\return	������ �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	����� �Ѵ�.
*
*	\param	s[in]	���
*
*	\return ����� �� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	����� ������.
*
*	\param	s[in]	���
*
*	\return ����� �������� �ڽ��� ��ȯ�Ѵ�.
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
*	\brief	���� ������
*
*	\return ���� ��ȣ�� ��ü�� ��ȯ�Ѵ�.
*/
EgQuat EgQuat::operator +() const
{
	return *this;
}

/*!
*	\brief	���� ������
*
*	\return �ݴ� ��ȣ�� ��ü�� ��ȯ�Ѵ�.
*/
EgQuat EgQuat::operator -() const
{
	return *this * (-1);
}

/*!
*	\brief	�� ���ʹϿ��� ���Ѵ�.
*
*	\param	q1[in]	ù ��° ���ʹϿ�
*	\param	q2[in]	�� ��° ���ʹϿ�
*
*	\return �� ���ʹϿ��� ���� ��ȯ�Ѵ�.
*/
EgQuat operator +(const EgQuat &q1, const EgQuat &q2)
{
	EgQuat ret(q1);
	ret += q2;
	return ret;
}

/*!
*	\brief	�� ���ʹϿ��� ���� ���Ѵ�.
*
*	\param	q1[in]	ù ��° ���ʹϿ�
*	\param	q2[in]	�� ��° ���ʹϿ�
*
*	\return �� ���ʹϿ��� ���� ��ȯ�Ѵ�.
*/
EgQuat operator -(const EgQuat &q1, const EgQuat &q2)
{
	EgQuat ret(q1);
	ret -= q2;
	return ret;
}

/*!
*	\brief	�� ���ʹϿ��� ���Ѵ�.
*
*	\param	q1[in]	ù ��° ���ʹϿ�
*	\param	q2[in]	�� ��° ���ʹϿ�
*
*	\return �� ���ʹϿ��� ���� ��ȯ�Ѵ�.
*/
EgQuat operator *(const EgQuat &q1, const EgQuat &q2)
{
	EgQuat ret(q1);
	ret *= q2;
	return ret;
}

/*!
*	\brief	ȸ���� ���͸� ����Ѵ�
*	\note	���� ���ʹϿ��� �������ʹϿ��̾�� ��
*
*	\param	q[in]	���ʹϿ�
*	\param	v[in]	ȸ����ų ����
*
*	\return ȸ���� ���͸� ��ȯ�Ѵ�.
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
*	\brief	ȸ���� ���� ��ġ�� ����Ѵ�
*	\note	���� ���ʹϿ��� �������ʹϿ��̾�� ��
*
*	\param	q[in]	���ʹϿ�
*	\param	p[in]	ȸ����ų ���� ��ġ
*
*	\return ȸ���� ���͸� ��ȯ�Ѵ�.
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
*	\brief	���ʹϿ��� ����� �Ѵ�.
*
*	\param	q[in]	���ʹϿ�
*	\param	s[in]	���
*
*	\return ���ʹϿ��� ����踦 ��ȯ�Ѵ�.
*/
EgQuat operator *(const EgQuat &q, const double &s)
{
	EgQuat ret(q);
	ret *= s;
	return ret;
}

/*!
*	\brief	���ʹϿ��� ����� �Ѵ�.
*
*	\param	s[in]	���
*	\param	q[in]	���ʹϿ�
*
*	\return ���ʹϿ��� ����踦 ��ȯ�Ѵ�.
*/
EgQuat operator *(const double &s, const EgQuat &q)
{
	EgQuat ret(q);
	ret *= s;
	return ret;
}

/*!
*	\brief	���ʹϿ��� ����� ������.
*
*	\param	q[in]	���ʹϿ�
*	\param	s[in]	���
*
*	\return ���� ����� ��ȯ�Ѵ�.
*/
EgQuat operator /(const EgQuat &q, const double s)
{
	EgQuat ret(q);
	ret /= s;
	return ret;
}

/*!
*	\brief	��ȣ������(==)
*
*	\param	q1[in]	���� �ǿ�����
*	\param	q2[in]	������ �ǿ�����
*
*	\return �� ���ʹϿ��� ������ true, �ٸ��� false ��ȯ�Ѵ�.
*/
bool operator ==(const EgQuat &q1, const EgQuat &q2)
{
	return (EQ(q1.m_W, q2.m_W, MTYPE_EPS) && 
			EQ(q1.m_X, q2.m_X, MTYPE_EPS) && 
			EQ(q1.m_Y, q2.m_Y, MTYPE_EPS) && 
			EQ(q1.m_Z, q2.m_Z, MTYPE_EPS));
}

/*!
*	\brief	�ε�ȣ������(!=)
*
*	\param	q1[in]	���� �ǿ�����
*	\param	q2[in]	������ �ǿ�����
*
*	\return �� ���ʹϿ��� �ٸ��� true, ������ false ��ȯ�Ѵ�.
*/
bool operator !=(const EgQuat &q1, const EgQuat &q2)
{
	return (q1 == q2) ? false : true;
}

/*!
*	\brief	��� ������
*
*	\param	os[out]		��� ��Ʈ��
*	\param	q[in]		����� ���ʹϿ�
*
*	\return ��µ� ��Ʈ���� ��ȯ�Ѵ�.
*/
std::ostream &operator <<(std::ostream &os, const EgQuat &q)
{
	os << q.m_W << " + " << q.m_X << "(i) + " << q.m_Y << "(j) + " << q.m_Z << "(k)" << std::endl;
	return os;
}

/*!
*	\brief	�Է� ������
*
*	\param	is[in]	�Է� ��Ʈ��
*	\param	q[out]	�Է¹��� ���ʹϿ�
*
*	\return �Էµ� ��Ʈ���� ��ȯ�Ѵ�.
*/
std::istream &operator >>(std::istream &is, EgQuat &q)
{
	is >> q.m_W >> q.m_X >> q.m_Y >> q.m_Z;
	return is;
}

/*********************/
/* EgLine Ŭ���� ���� */
/*********************/
/*!
*	\brief	������
*
*	\param	Pos[in]		�������� �� ��
*	\param	Dir[in]		������ ���� ����(���� ������ �ʿ� ����)
*/
EgLine::EgLine(EgPos Pos, EgVec3 Dir)
{
	m_Pos = Pos;
	m_Dir = Dir;
}

/*!
*	\brief	�� ���� ������ ������ �����ϴ� ������.
*
*	\param	p[in]	���� ���� ��, l(0) = p
*	\param	q[in]	���� ���� ��, l(1) = q
*/
EgLine::EgLine(EgPos p, EgPos q)
{
	m_Pos = p;
	m_Dir = q - p;
}

/*!
*	\brief	���� ������
*
*	\param	cpy[in]		����� ��ü
*/
EgLine::EgLine(EgLine &cpy)
{
	m_Pos = cpy.m_Pos;
	m_Dir = cpy.m_Dir;
}

/*!
*	\brief �Ҹ���
*/
EgLine::~EgLine()
{
}

/*!
*	\brief	���� ���� ���� ����Ѵ�.
*
*	\param	t[in]	�Ű�����
*
*	\return ���� ���� �� l(t)�� ��ȯ�Ѵ�.
*/
EgPos EgLine::Eval(double t)
{
	return m_Pos + t * m_Dir;
}

/*!
*	\brief	�� q�� ���� ���� ������ �����Ѵ�.
*
*	\param	q[in]	������ ��
*
*	\return �� q�� ���� ���� ���̸� true, �ƴϸ� false�� ��ȯ�Ѵ�.
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
*	\brief	���� ������
*
*	\param	rhs[in]		������ ��ü
*
*	\return ���Ե� �ڽ��� ��ȯ�Ѵ�.
*/
EgLine &EgLine::operator =(const EgLine &rhs)
{
	m_Pos = rhs.m_Pos;
	m_Dir = rhs.m_Dir;
	return *this;
}

/*!
*	\brief	�� ������ ������ �����Ѵ�.
*
*	\param	lhs[in]		���� ����
*	\param	rhs[in]		������ ����
*
*	\return ������ �����̸� true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool operator ==(EgLine &lhs, EgLine &rhs)
{
	if (lhs.IsOnLine(rhs.m_Pos) && EQ_ZERO(norm(lhs.m_Dir ^ rhs.m_Dir), MTYPE_EPS))
		return true;
	else
		return false;
}

/*!
*	\brief	�� ������ �ٸ��� �����Ѵ�.
*
*	\param	lhs[in]		���� ����
*	\param	rhs[in]		������ ����
*
*	\return �ٸ� �����̸� true, �ƴϸ� false�� ��ȯ�Ѵ�.
*/
bool operator !=(EgLine &lhs, EgLine &rhs)
{
	return (lhs == rhs) ? false : true;
}

/*!
*	\brief	��� ������
*
*	\param	os[out]		��� ��Ʈ��
*	\param	l[in]		����� ��ü
*
*	\return ��µ� ��Ʈ���� ��ȯ�Ѵ�.
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
/* EgPlane Ŭ���� ���� */
/*********************/
/*!
*	\brief	������
*/
EgPlane::EgPlane()
{
	m_Coords.Set(0.0, 0.0, 0.0, 0.0);
}

/*!
*	\brief	������
*
*	\param	n[in]	����� ��������
*	\param	p[in]	��� ���� �� ��
*/
EgPlane::EgPlane(const EgVec3 &n, const EgPos &p)
{
	m_Coords = cast_vec4(n, -n * cast_vec3(p));
}

/*!
*	\brief	�� �� p1, p2, p3 �� ������ ����� ������
*
*	\param	p1[in]	��� ���� ù ��° ��
*	\param	p2[in]	��� ���� �� ��° ��
*	\param	p3[in]	��� ���� �� ��° ��
*/
EgPlane::EgPlane(const EgPos &p1, const EgPos &p2, const EgPos &p3)
{
	EgVec3 n = (p2 - p1) ^ (p3 - p1);
	m_Coords = cast_vec4(n, -n * cast_vec3(p1));
}

/*!
*	\brief	������
*
*	\param	n[in]	��������
*	\param	d[in]	��� d = - n * p (p: ��� ���� ������ ��).
*/
EgPlane::EgPlane(const EgVec3 &n, const double &d)
{
	m_Coords = cast_vec4(n, d);
}

/*!
*	\brief	������
*
*	\param	a[in]	����� �������� ���
*	\param	b[in]	����� �������� ���
*	\param	c[in]	����� �������� ���
*	\param	d[in]	����� �������� ���
*/
EgPlane::EgPlane(double a, double b, double c, double d)
{
	m_Coords.Set(a, b, c, d);
}

/*!
*	\brief	���� ������
*
*	\param	cpy[in]		����� ��ü
*/
EgPlane::EgPlane(const EgPlane &cpy)
{
	m_Coords = cpy.m_Coords;
}

/*!
*	\brief �Ҹ���
*/
EgPlane::~EgPlane()
{
}

/*!
*	\brief	����� �Լ����� ����Ѵ�.
*	\note	������ ������: 2020-05-24
*
*	\param	p[in]	������ ��ǥ(x, y, z)
*
*	\return �Լ��� f(x, y, z) = ax + by + cz + d�� ��ȯ�Ѵ�.
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
*	\brief	����� �����͸� ��ȯ�Ѵ�.
*	\note	������ ������: 2020-05-28
*
*	\param	bNormalize[in]	����ȭ ����
*
*	\return ����� �����͸� ��ȯ�Ѵ�.
*/
EgVec3 EgPlane::N(bool bNormalize)
{
	EgVec3 ret(m_Coords[0], m_Coords[1], m_Coords[2]);
	if (bNormalize)
		ret.Normalize();
	return ret;
}

/*!
*	\brief	����� ������(ax + by + cz + d = 0)�� ��� d�� ��ȯ�Ѵ�.
*
*	\return ����� ������(ax + by + cz + d = 0)�� ��� d�� ��ȯ�Ѵ�.
*/
double EgPlane::d()
{
	return m_Coords[3];
}

/*!
*	\brief	�� p�� ��鿡 ���� �� ������ �����Ѵ�.
*
*	\param	p[in]	������ ��
*
*	\return �� p�� ��鿡 ���� ���̸� true, �ƴϸ� false�� ��ȯ�Ѵ�.
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
*	\brief	�� p�� ����� ���� �������� �����Ѵ�.
*
*	\param	p[in]	������ ��
*
*	\return �� p�� ����� ���� �� �ϰ�� true, �ƴϸ� false�� ��ȯ�Ѵ�.
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
*	\brief	�� p�� ����� �Ʒ��� �������� �����Ѵ�.
*
*	\param	p[in]	������ ��
*
*	\return �� p�� ����� �Ʒ��� �� �ϰ�� true, �ƴϸ� false�� ��ȯ�Ѵ�.
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
*	\brief	������ �ݴ��� ����� ��ȯ�Ѵ�.
*	\note	������ ������: 2020-09-02
*
*	\return ������ �ݴ��� ����� ��ȯ�Ѵ�.
*/
EgPlane EgPlane::GetReversePlane()
{
	return EgPlane(-m_Coords[0], -m_Coords[1], -m_Coords[2], -m_Coords[3]);
}

/*!
*	\brief	���� ������
*
*	\param	rhs[in]		���Ե� ��ü
*
*	\return ���Ե� �ڽ��� ��ȯ�Ѵ�.
*/
EgPlane &EgPlane::operator =(const EgPlane &rhs)
{
	m_Coords = rhs.m_Coords;
	return *this;
}

/*!
*	\brief	�� ����� ������ �����Ѵ�.
*
*	\param	lhs[in]		ù ��° ���
*	\param	rhs[in]		�� ��° ���
*
*	\return �� ����� ������ true, �ٸ��� false�� ��ȯ�Ѵ�.
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
*	\brief	�� ����� �ٸ��� �����Ѵ�.
*
*	\param	lhs[in]		ù ��° ���
*	\param	rhs[in]		�� ��° ���
*
*	\return �� ����� �ٸ��� true, ������ false�� ��ȯ�Ѵ�.
*/
bool operator !=(const EgPlane &lhs, const EgPlane &rhs)
{
	return (lhs == rhs) ? false : true;
}

/*!
*	\brief	��� ������
*
*	\param	os[out]		��� ��Ʈ��
*	\param	pi[in]		��� ���
*
*	\return ��� ��Ʈ���� ��ȯ�Ѵ�.
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
/* EgTrans Ŭ���� ���� */
/*********************/
/*!
*	\brief	������
*
*	\param	t[in]	�̵� ����
*	\param	q[in]	ȸ�� ����
*/
EgTrans::EgTrans(const EgVec3 &t, const EgQuat &q)
{
	m_T = t;
	m_Q = q;
}

/*!
*	\brief	������
*
*	\param	mat[in]		��ȯ ���(4 X 4)
*	\param	isGL[in]	��ȯ ����� OpenGL ����(row major) ����
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
*	\brief	���� ������
*
*	\param	cpy[in]		����� ��ü
*/
EgTrans::EgTrans(const EgTrans &cpy)
{
	m_T = cpy.m_T;
	m_Q = cpy.m_Q;
}

/*!
*	\brief �Ҹ���.
*/
EgTrans::~EgTrans()
{
}

/*!
*	\brief	ȸ���� negate�Ѵ�.
*/
void EgTrans::SetNegate()
{
	m_Q.SetNegate();
}

/*!
*	\brief	�̵� ��ȯ�� �����Ѵ�.
*
*	\param	t[in] ������ �̵� ��ȯ
*/
void EgTrans::SetTrans(const EgVec3 &t)
{
	m_T = t;
}

/*!
*	\brief	ȸ�� ��ȯ�� �����Ѵ�.
*
*	\param	q[in] ������ ȸ�� ��ȯ
*/
void EgTrans::SetRot(const EgQuat &q)
{
	m_Q = q;
}

/*!
*	\brief	���� ������
*
*	\param	rhs[in]		���Ե� ��ü
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgTrans &EgTrans::operator =(const EgTrans &rhs)
{
	m_T = rhs.m_T;
	m_Q = rhs.m_Q;
	return *this;
}

/*!
*	\brief	��ȯ�� ���Ѵ�.
*
*	\param	rhs[in]		������ ��ȯ
*
*	\return ������ �ڽ��� ��ȯ�Ѵ�.
*/
EgTrans &EgTrans::operator *=(const EgTrans &rhs)
{
	m_T += m_Q * rhs.m_T;
	m_Q *= rhs.m_Q;
	m_Q.Normalize();
	return *this;
}

/*!
*	\brief	��ȯ�� ����� �Ѵ�.
*
*	\param	s[in]	���
*
*	\return ����� �ڽ��� ��ȯ�Ѵ�.
*/
EgTrans &EgTrans::operator *=(const double &s)
{
	m_T *= s;
	m_Q *= s;
	return *this;
}

/*!
*	\brief	�� ��ȯ�� ���Ѵ�.
*
*	\param	X1[in]	ù ��° ��ȯ
*	\param	X2[in]	�� ��° ��ȯ
*
*	\return ���� ����� ��ȯ�Ѵ�.
*/
EgTrans operator *(const EgTrans &X1, const EgTrans &X2)
{
	EgTrans ret(X1);
	ret *= X2;
	return ret;
}

/*!
*	\brief	��ȯ�� ������Ѵ�.
*
*	\param	X[in]	��ȯ
*	\param	s[in]	���
*
*	\return ��ȯ�� ����踦 ��ȯ�Ѵ�.
*/
EgTrans operator *(const EgTrans &X, const double &s)
{
	EgTrans ret(X);
	ret *= s;
	return ret;
}

/*!
*	\brief	��ȯ�� ����踦 ���Ѵ�.
*
*	\param	s[in]	���
*	\param	X[in]	���� �ǿ�����
*
*	\return ��ȯ�� ����踦 ��ȯ�Ѵ�.
*/
EgTrans operator *(const double &s, const EgTrans &X)
{
	EgTrans ret(X);
	ret *= s;
	return ret;
}

/*!
*	\brief	��ȯ�� ���� ���Ѵ�.
*
*	\param	X[in]	��ȯ ��ü
*	\param	p[in]	��� ��
*
*	\return ���� ��ǥ�迡�� ǥ���� �� p�� ��ǥ�� ��ȯ�Ѵ�.
*/
EgPos operator *(const EgTrans &X, const EgPos &p)
{
	return X.m_Q * p + X.m_T;
}

/*!
*	\brief	��ȯ�� ���͸� ���Ѵ�.
*
*	\param	X[in]	��ȯ ��ü
*	\param	v[in]	��� ����
*
*	\return ���� ��ǥ�迡�� ǥ���� ���� v�� ��ǥ�� ��ȯ�Ѵ�.
*/
EgVec3 operator *(const EgTrans &X, const EgVec3 &v)
{
	return X.m_Q * v;
}

/*!
*	\brief	��ȯ�� ������ ���Ѵ�.
*
*	\param	X[in]	��ȯ ��ü
*	\param	l[in]	��� ����
*
*	\return ���� ��ǥ�迡�� ǥ���� ���� l�� ǥ���� ��ȯ�Ѵ�.
*/
EgLine operator *(EgTrans &X, EgLine &l)
{
	EgPos p = X * l.m_Pos;
	EgVec3 v = X * l.m_Dir;
	return EgLine(p, v);
}

/*!
*	\brief	��ȯ�� ����� ���Ѵ�.
*
*	\param	X[in]	��ȯ ��ü
*	\param	pi[in]	��� ���
*
*	\return ���� ��ǥ�迡�� ǥ���� ��� pi�� ��ȯ�Ѵ�.
*/
EgPlane operator *(EgTrans &X, EgPlane &pi)
{
	EgVec3 n = X * pi.N();
	double d = pi.d() - pi.N() * (inv(X.m_Q) * X.m_T);
	return EgPlane(n, d);
}

/*!
*	\brief	�� ��ȯ�� ������ �����Ѵ�.
*
*	\param	X1[in]	ù ��° ��ȯ
*	\param	X2[in]	�� ��° ��ȯ
*
*	\return �� ��ȯ�� ������ true, �ٸ��� false�� ��ȯ�Ѵ�.
*/
bool operator ==(const EgTrans &X1, const EgTrans &X2)
{
	if (X1.m_T == X2.m_T && X1.m_Q == X1.m_Q)
		return true;
	else
		return false;
}

/*!
*	\brief	�� ��ȯ�� �ٸ��� �����Ѵ�.
*
*	\param	X1[in]	ù ��° ��ȯ
*	\param	X2[in]	�� ��° ��ȯ
*
*	\return �� ��ȯ�� �ٸ��� true, ������ false�� ��ȯ�Ѵ�.
*/
bool operator !=(const EgTrans &X1, const EgTrans &X2)
{
	return (X1 == X2) ? false : true;
}

/*!
*	\brief	��� ������
*
*	\param	os[out]		��� ��Ʈ��
*	\param	X[in]		��� ��ȯ
*
*	\return ��µ� ��Ʈ���� ��ȯ�Ѵ�.
*/
std::ostream &operator <<(std::ostream &os, const EgTrans &X)
{
	os << cast_mat(X, false);
	return os;
}

