#include "..\usr\include\EgMath\egmath.h"

/***********************/
/* EgVec2 ��ƿ �Լ� ���� */
/***********************/
/*!
*	\brief	���� v�� ���� w�� �翵��Ų ���͸� ���Ѵ�.
*
*	\param	v[in]	�翵 ����
*	\param	w[in]	���� ����
*
*	\return ���� v�� ���� w�� �翵��Ų ���͸� ��ȯ�Ѵ�.
*/
EgVec2 proj(const EgVec2 &v, const EgVec2 &w)
{
	return (v * w / norm_sq(w)) * w;
}

/*!
*	\brief	���� v�� 90�� ȸ���� ���� ���͸� ���Ѵ�.
*	\note	������ ������: 2020-07-05
*
*	\param	v[in]	ȸ���� ����
*	\param	CCW[in]	ȸ�� ����(�ݽð� ����: true, �ð� ����: false)
*
*	\return ���� v�� 90�� ȸ���� ���� ���͸� ��ȯ�Ѵ�.
*/
EgVec2 ortho(const EgVec2 &v, bool CCW)
{
	return (CCW) ? EgVec2(-v[1], v[0]) : EgVec2(v[1], -v[0]);
}

/*!
*	\brief	�� ������ ���� ������ ���̸� ���Ѵ�.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ���� ����
*
*	\return ���� ���� (v - w)�� ���̸� ��ȯ�Ѵ�.
*/
double dist(const EgVec2 &v, const EgVec2 &w)
{
	return norm(v - w);
}

/*!
*	\brief	������ ũ�⸦ ���Ѵ�.
*
*	\param	v[in]	��� ����
*
*	\return ���� v�� ũ�⸦ ��ȯ�Ѵ�.
*/
double norm(const EgVec2 &v)
{
	return SQRT(SQR(v.m_Coords[0]) + SQR(v.m_Coords[1]));
}

/*!
*	\brief	������ ũ�� ������ ���Ѵ�.
*
*	\param	v[in]	��� ����
*
*	\return ���� v�� ũ�� ������ ��ȯ�Ѵ�.
*/
double norm_sq(const EgVec2 &v)
{
	return SQR(v.m_Coords[0]) + SQR(v.m_Coords[1]);
}

/*!
*	\brief	�� ������ ���̰�(0 ~ 180)�� ���Ѵ�.
*
*	\param	v[in]		���� ����
*	\param	w[in]		�� ����
*	\param	radian[in]	������ ����(true: radian, false: degree)
*
*	\return ���� \a v���� ���� \a w������ ���̰��� ��ȯ�Ѵ�.
*/
double angle(const EgVec2 &v, const EgVec2 &w, bool radian)
{
	EgVec3 p = (cast_vec3(v)).Normalize();
	EgVec3 q = (cast_vec3(w)).Normalize();
	double cs = p * q;
	double sn = norm(p ^ q);
	return (radian) ? atan2(sn, cs) : RAD2DEG(atan2(sn, cs));
}

/***********************/
/* EgVec3 ��ƿ �Լ� ���� */
/***********************/
/*!
*	\brief	���� v�� ���� w�� �翵��Ų ���͸� ���Ѵ�.
*
*	\param	v[in]	�翵 ����
*	\param	w[in]	���� ����
*
*	\return ���� v�� ���� w�� �翵��Ų ���͸� ��ȯ�Ѵ�.
*/
EgVec3 proj(const EgVec3 &v, const EgVec3 &w)
{
	return (v * w / norm_sq(w)) * w;
}

/*!
*	\brief	���� v�� ������ ���� ���͸� ���Ѵ�.
*
*	\param	v[in]	��� ����
*
*	\return ���� v�� ������ ���� ���͸� ���Ͽ� ��ȯ�Ѵ�.
*/
EgVec3 ortho(const EgVec3 &v)
{
	EgVec3 ret;

	double min_val = MIN(MIN(v[0], v[1]), v[2]);
	if (min_val == v[0])
		ret.Set(0.0, -v[2], v[1]);

	if (min_val == v[1])
		ret.Set(v[2], 0.0, -v[0]);

	if (min_val == v[2])
		ret.Set(-v[1], v[0], 0.0);

	return ret.Normalize();
}

/*!
*	\brief	���� v�� w�� ���̺����� ���̸� ���Ѵ�.
*
*	\param	v[in]	����
*	\param	w[in]	����
*
*	\return	���� \a v, \a w�� ���̺����� ���̸� ��ȯ�Ѵ�.
*/
double dist(const EgVec3 &v, const EgVec3 &w)
{
	return norm(v - w);
}

/*!
*	\brief	���� u, v, w�� ��Ľ��� ����Ѵ�.
*
*	\param	u[in]	����
*	\param	v[in]	����
*	\param	w[in]	����
*
*	\return ���� u, v, w�� ��Ľ��� ���� ��ȯ�Ѵ�.
*/
double det(const EgVec3 &u, const EgVec3 &v, const EgVec3 &w)
{
	return (u[0] * v[1] * w[2] + u[1] * v[2] * w[0] + u[2] * v[0] * w[1]) - (w[0] * v[1] * u[2] + v[0] * u[1] * w[2] + u[0] * w[1] * v[2]);
}

/*!
*	\brief	���� v�� ũ�⸦ ���Ѵ�.
*
*	\param	v[in]	��� ����
*
*	\return ���� \a v�� ũ�⸦ ��ȯ�Ѵ�.
*/
double norm(const EgVec3 &v)
{
	return SQRT(norm_sq(v));
}

/*!
*	\brief	���� v�� ũ���� ������ ����Ѵ�.
*
*	\param	v[in]	��� ����
*
*	\return ���� \a v�� ũ���� ������ ��ȯ�Ѵ�.
*/
double norm_sq(const EgVec3 &v)
{
	return SQR(v.m_Coords[0]) + SQR(v.m_Coords[1]) + SQR(v.m_Coords[2]);
}

/*!
*	\brief	�� ���� ������ ���̰�(0 ~ 180)�� ���Ѵ�.
*
*	\param	v[in]		���� ����
*	\param	w[in]		�� ����
*	\param	radian[in]	������ ����(true: radian, false: degree)
*
*	\return ���� \a v���� ���� \a w������ ���̰�(����, ���� �������)
*/
double angle(const EgVec3 &v, const EgVec3 &w, bool radian)
{
	EgVec3 p(v);
	EgVec3 q(w);
	p.Normalize();
	q.Normalize();
	double cs = p * q;
	double sn = norm(p ^ q);
	return (radian) ? atan2(sn, cs) : RAD2DEG(atan2(sn, cs));
}

/*!
*	\brief	�� ���� ������ ���̰�(0, ~ 360)�� ���Ѵ�.
*
*	\param	v[in]		���� ����
*	\param	w[in]		�� ����
*	\param	up[in]		���� ����
*	\param	radian[in]	������ ����(true: radian, false: degree)
*
*	\return ���� \a v���� ���� \a w������ ���̰�(���� ���͸� ���� �ݽð� ����)
*/
double angle(const EgVec3 &v, const EgVec3 &w, const EgVec3 &up, bool radian)
{
	EgVec3 p(v);
	EgVec3 q(w);
	p.Normalize();
	q.Normalize();
	EgVec3 r = p ^ q;

	double cs = p * q;
	double sn = norm(r);
	double theta = atan2(sn, cs);
	if (r * up < 0.0)
		theta = 2 * M_PI - theta;

	theta = radian ? theta : RAD2DEG(theta);
	return theta;
}

/***********************/
/* EgVec24��ƿ �Լ� ���� */
/***********************/
/*!
*	\brief	���� v�� ũ�⸦ ���Ѵ�.
*
*	\param	v[in]	����
*
*	\return ���� \a v�� ũ�⸦ ��ȯ�Ѵ�.
*/
double norm(const EgVec4 &v)
{
	return SQRT(norm_sq(v));
}

/*!
*	\brief	���� v�� ũ�� ������ ���Ѵ�.
*
*	\param	v[in]	����
*
*	\return ���� \a v�� ũ�� ������ ��ȯ�Ѵ�.
*/
double norm_sq(const EgVec4 &v)
{
	return
		SQR(v.m_Coords[0]) +
		SQR(v.m_Coords[1]) +
		SQR(v.m_Coords[2]) +
		SQR(v.m_Coords[3]);
}

/*!
*	\brief	�� ������ ���̺����� ũ�⸦ ���Ѵ�.
*
*	\param	v[in]	����
*	\param	w[in]	����
*
*	\return �� ������ ���̺����� ũ�⸦ ��ȯ�Ѵ�.
*/
double dist(const EgVec4 &v, const EgVec4 &w)
{
	return norm(v - w);
}

/*!
*	\brief	����ġ�� �����Ͽ� ���� ��ǥ�� ��ȯ�Ѵ�: v=(x, y, z, w) -> v'=(wx, wy, wz, w)
*
*	\param	v[in]	��� ����
*
*	\return ���� ��ǥ v'=(wx, wy, wz, w)�� ��ȯ�Ѵ�.
*/
EgVec4 homogenize(const EgVec4 &v)
{
	return EgVec4(v[0] * v[3], v[1] * v[3], v[2] * v[3], v[3]);
}

/*!
*	\brief	���� ��ǥ v=(wx, wy, wz, w) -> v'=(x, y, z, w)�� ��ȯ�Ѵ�.
*
*	\param	v[in]	4���� ������ ����
*
*	\return ��ȯ�� ���� v'=(x, y, z, w)�� ��ȯ�Ѵ�.
*/
EgVec4 rationalize(const EgVec4 &v)
{
	return EgVec4(v[0] / v[3], v[1] / v[3], v[2] / v[3], v[3]);
}

/*!
*	\brief	v=(wx, wy, wz, w) -> v'=(x, y, z, 1)
*
*	\param	v[in]	4���� ������ ����
*
*	\return v'=(x, y, z, 1)�� ��ȯ�Ѵ�.
*/
EgVec4 cartesianize(const EgVec4 &v)
{
	return EgVec4(v[0] / v[3], v[1] / v[3], v[2] / v[3], 1.0);
}

/***********************/
/* EgVec ��ƿ �Լ� ���� */
/***********************/
/*!
*	\brief	������ ũ�⸦ ���Ѵ�.
*
*	\param	v[in]	��� ����
*
*	\return ���� ũ�⸦ ��ȯ�Ѵ�.
*/
double norm(const EgVec &v)
{
	return SQRT(norm_sq(v));
}

/*!
*	\brief	������ ũ���� ������ ���Ѵ�.
*
*	\param	v[in]	��� ����
*
*	\return ���� ũ���� ������ ��ȯ�Ѵ�.
*/
double norm_sq(const EgVec &v)
{
	double len = 0;
	for (int i = 0; i < (int)v.m_Coords.size(); ++i)
		len += SQR(v.m_Coords[i]);
	return len;
}

/*!
*	\brief	�� ������ ���̺����� ũ�⸦ ���Ѵ�.
*
*	\param	v[in]	ù ��° ����
*	\param	w[in]	�� ��° ����
*
*	\return �� ������ ���̺����� ũ�⸦ ��ȯ�Ѵ�.
*/
double dist(const EgVec &v, const EgVec &w)
{
	return norm(v - w);
}

/**********************/
/* EgPos ��ƿ �Լ� ���� */
/**********************/
/*!
*	\brief	����Ʈ�� \a p�� ���̸� ���Ѵ�.
*
*	\param	p[in]	����Ʈ
*
*	\return ����Ʈ \a p�� ���̸� ��ȯ�Ѵ�.
*/
double norm(const EgPos &p)
{
	return SQRT(norm_sq(p));
}

/*!
*	\brief	����Ʈ�� \a p�� ���� ������ ���Ѵ�.
*
*	\param	p[in]	����Ʈ
*
*	\return ����Ʈ \a p�� ���� ������ ��ȯ�Ѵ�.
*/
double norm_sq(const EgPos & p)
{
	return SQR(p.m_Coords[0]) + SQR(p.m_Coords[1]) + SQR(p.m_Coords[2]);
}

/*!
*	\brief	�� \a p���� �� \a q������ �Ÿ��� ���Ѵ�.
*
*	\param	p[in]	ù ��° ��
*	\param	q[in]	�� ��° ��
*
*	\return �� \a p���� �� \a q������ �Ÿ��� ��ȯ�Ѵ�.
*/
double dist(const EgPos &p, const EgPos &q)
{
	return SQRT(dist_sq(p, q));
}

/*!
*	\brief	�� \a p���� �� \a q������ �Ÿ� ������ ���Ѵ�.
*
*	\param	p[in]	ù ��° ��
*	\param	q[in]	�� ��° ��
*
*	\return �� \a p���� �� \a q������ �Ÿ� ������ ��ȯ�Ѵ�.
*/
double dist_sq(const EgPos &p, const EgPos &q)
{
	return (SQR(p.m_Coords[0] - q.m_Coords[0]) + SQR(p.m_Coords[1] - q.m_Coords[1]) + SQR(p.m_Coords[2] - q.m_Coords[2]));
}

/*!
*	\brief	�� p�� q�� t : (1 - t)�� �����Ѵ�.
*
*	\param	p[in]	3���� ������ ��
*	\param	q[in]	3���� ������ ��
*
*	\return �� p�� q�� �������� ��ȯ�Ѵ�.
*/
EgPos lerp(const EgPos &p, const EgPos &q, double t)
{
	double x = (1.0 - t) * p[0] + t * q[0];
	double y = (1.0 - t) * p[1] + t * q[1];
	double z = (1.0 - t) * p[2] + t * q[2];
	return EgPos(x, y, z);
}

/*!
*	\brief	������ affine combination�� ���Ѵ�
*
*	\param	N[in]		���� ����
*	\param	alpha0[in]	���
*	\param	p0[in]		ù ��° ��
*	\param	alpha1[in]	���
*	\param	p1[in]		�� ��° ��
*
*	\return ������ affine combination�� ��� (alpha0 * p0 + alpha1 * p1 + ... + alpha_n-1 * p_n-1)
*/
EgPos affine_sum(int N, ...)
{
	EgPos ret;

	va_list ap;
	va_start(ap, N);
	for (int i = 0; i < N; ++i)
	{
		double alpha = va_arg(ap, double);
		EgPos p = va_arg(ap, EgPos);

		ret[0] = ret[0] + alpha * p[0];
		ret[1] = ret[1] + alpha * p[1];
		ret[2] = ret[2] + alpha * p[2];
	}
	va_end(ap);

	return ret;
}

/*!
*	\brief	������ affine combination�� ���Ѵ�
*
*	\param	Points[in]		���� �迭�� ���� ������
*	\param	Weights[in]		combination�� �����
*	\param	Size[in]		���� ���� (Points, Weights �迭�� ũ��)
*
*	\return ������ affine combination�� ����� ��ȯ�Ѵ�(Point[0] * Weight[0] + Points[1] * Weight[1] + ... + Points[Size-1] * Weight[Size-1])
*/
EgPos affine_sum(EgPos *Points, double *Weights, const int Size)
{
	EgPos ret;
	for (int i = 0; i < Size; i++)
	{
		ret.m_Coords[0] += Points[i][0] * Weights[i];
		ret.m_Coords[1] += Points[i][1] * Weights[i];
		ret.m_Coords[2] += Points[i][2] * Weights[i];
	}
	return ret;
}

/*!
*	\brief	�ﰢ���� �а��ﰢ������ �˻��Ѵ�.
*	\note	����: ����ȣ(2019-02-03), Ȯ��: ������(2019-03-03)
*
*	\param	p[in]	�ﰢ���� ù ��° ����(�ݽð� ����)
*	\param	q[in]	�ﰢ���� �� ��° ����(�ݽð� ����)
*	\param	r[in]	�ﰢ���� �� ��° ����(�ݽð� ����)
*
*	\return ��(rpq)�� �а��� ��� 1, ��(pqr)�� �а��� ��� 2, ��(qrp)�� �а��ΰ�� 3, �а��� �ƴѰ�� 0�� ��ȯ�Ѵ�.
*/
int is_obtuse_tri(const EgPos &p, const EgPos &q, const EgPos &r)
{
	// �ﰢ���� ���� ���Ϳ� �����͸� ���Ѵ�.
	EgVec3 a = q - p;
	EgVec3 b = r - q;
	EgVec3 c = p - r;
	EgVec3 N = (a ^ b).Normalize();

	// ù ��° ���� �а��̸� 1�� ��ȯ�Ѵ�.
	double alpha = angle(a, -c, N);
	if (alpha > 90.0)
		return 1;

	// �� ��° ���� �а��̸� 2�� ��ȯ�Ѵ�.
	double beta = angle(b, -a, N);
	if (beta > 90.0)
		return 2;

	// �� ��° ���� �а��̸� 3�� ��ȯ�Ѵ�.
	if (alpha + beta < 90.0)
		return 3;

	// ��� �����̸� 0�� ��ȯ�Ѵ�.
	return 0;
}

/*!
*	\brief	�ﰢ���� �ܽ�(circumcenter)�� ���Ѵ�.
*	\note	����: ����ȣ(2019-02-03), Ȯ��: ������(2019-03-03)
*
*	\param	p[in]	�ﰢ���� ù ��° ����(�ݽð� ����)
*	\param	q[in]	�ﰢ���� �� ��° ����(�ݽð� ����)
*	\param	r[in]	�ﰢ���� �� ��° ����(�ݽð� ����)
*
*	\return �ﰢ���� �ܽ��� ��ġ�� ��ȯ�Ѵ�.
*/
EgPos circumcenter(const EgPos &p, const EgPos &q, const EgPos &r)
{
	// p�� �������� �ϴ� ���� ��ǥ�踦 �����Ͽ� 
	EgVec3 x_axis = (q - p).Normalize();
	EgVec3 z_axis = ((q - p) ^ (r - p)).Normalize();
	EgVec3 y_axis = (z_axis ^ x_axis).Normalize();
	EgTrans X(cast_vec3(p), EgQuat(x_axis, y_axis));

	// ���� ��ǥ�迡�� ǥ���� ���� q�� r�� ��ǥ q0, r0�� ���Ѵ�.
	EgPos q0 = inv(X) * q;
	EgPos r0 = inv(X) * r;

	// ���� ��ǥ�迡�� �ﰢ���� �ܽ��� ���ϰ�,
	EgPos O(q0[0] * 0.5, (SQR(r0[0]) + SQR(r0[1]) - q0[0] * r0[0]) / (2.0 * r0[1]), 0.0);

	// ���� ��ǥ��� ��ȯ�Ͽ� ��ȯ�Ѵ�.
	return X * O;
}

/*!
*	\brief	�ﰢ���� ���̸� ���Ѵ�
*
*	\param	p[in]	�ﰢ���� ù ��° ����(�ݽð� ����)
*	\param	q[in]	�ﰢ���� �� ��° ����(�ݽð� ����)
*	\param	r[in]	�ﰢ���� �� ��° ����(�ݽð� ����)
*
*	\return �ﰢ���� ���̸� ���Ͽ� ��ȯ�Ѵ�.
*/
double area_tri(const EgPos &p, const EgPos &q, const EgPos &r)
{
	double area = norm((q - p) ^ (r - p)) * 0.5;
	return area;
}

/*!
*	\brief	�� p�� ��ǥ�� �־��� �ڸ�������(�ݿø��Ͽ�) ��Ÿ��
*
*	\param	p[in] 3���� ������ ��
*
*	\return �ݿø��� ���� ǥ���� ��ȯ�Ѵ�.
*/
EgPos round_off(const EgPos &p, int place)
{
	double a = pow(10.0, -place);
	double x = ((int)(p[0] * a + 0.5)) / a;
	double y = ((int)(p[1] * a + 0.5)) / a;
	double z = ((int)(p[2] * a + 0.5)) / a;
	return EgPos(x, y, z);
}


/**********************/
/* EgMat ��ƿ �Լ� ���� */
/**********************/
/*!
*	\brief	��ġ����� ��ȯ�Ѵ�.
*
*	\param	m[in]	���
*
*	\return ��� \a m�� ��ġ����� ��ȯ�Ѵ�.
*/
EgMat tr(const EgMat &m)
{
	EgMat ret(m);
	ret.SetTranspose();
	return ret;
}

/*!
*	\brief	����� row-echelon ���� ��ȯ�Ѵ�.
*
*	\param	m[in]	���
*
*	\return row-echelon ���� ��ȯ�Ѵ�.
*/
EgMat ref(const EgMat &m)
{
	int i, j, k; // �ݺ����� ����
	int r = m.GetRowNum();
	int c = m.GetColNum();
	int n = MIN(r, c);
	EgMat T(m);

	int shift = 0;
	for (i = 0; i < n; i++)
	{
		// pivoting.
		double maxi = ABS(T[i][i + shift]);
		int pivot_idx = i;
		for (j = i + 1; j < r; j++)
		{
			if (maxi < ABS(T[j][i + shift]))
			{
				maxi = ABS(T[j][i + shift]);
				pivot_idx = j;
			}
		}

		if (EQ_ZERO(maxi, MTYPE_EPS))
		{
			shift++;
			i--;
			continue;
		}

		if (i != pivot_idx)
			T.ExchangeRows(i, pivot_idx);

		double s = T[i][i + shift];
		for (j = i + shift; j < c; j++)
			T[i][j] = T[i][j] / s;

		for (j = i + 1; j < r; j++)
		{
			s = T[j][i + shift];
			for (k = i + shift; k < c; k++)
			{
				T[j][k] = T[j][k] - s * T[i][k];
			}
		}
	}

	return T;
}

/*!
*	\brief	����� reduced row-echelon ���� ��ȯ�Ѵ�.
*
*	\param	m[in]	���
*
*	\return reduced row-echelon ���� ��ȯ�Ѵ�.
*/
EgMat rref(const EgMat &m)
{
	int i, j, k; // �ݺ����� ����
	int r = m.GetRowNum();
	int c = m.GetColNum();
	int n = MIN(r, c);
	EgMat T(m);

	int shift = 0;
	for (i = 0; i < n; i++)
	{
		// pivoting.
		double maxi = ABS(T[i][i + shift]);
		int pivot_idx = i;
		for (j = i + 1; j < r; j++)
		{
			if (maxi < ABS(T[j][i + shift]))
			{
				maxi = ABS(T[j][i + shift]);
				pivot_idx = j;
			}
		}

		if (EQ_ZERO(maxi, MTYPE_EPS))
		{
			shift++;
			i--;
			continue;
		}

		if (i != pivot_idx)
			T.ExchangeRows(i, pivot_idx);

		double s = T[i][i + shift];
		for (j = i + shift; j < c; j++)
			T[i][j] = T[i][j] / s;

		for (j = 0; j < r; j++)
		{
			if (i == j)
				continue;

			s = T[j][i + shift];
			for (k = i + shift; k < c; k++)
			{
				T[j][k] = T[j][k] - s * T[i][k];
			}
		}
	}

	return T;
}

/*!
*	\brief	��� \a m�� Null ������ ������ �� ���ͷ� ���� ����� ��ȯ�Ѵ�.
*
*	\param	m[in]	���
*
*	\return ��� \a m�� Null ������ ������ �� ���ͷ� ���� ����� ��ȯ�Ѵ�.
*/
EgMat basis_null(const EgMat &m)
{
	// ��� m�� reduced row-echelon form�� ���Ѵ�.
	EgMat R = rref(m);

	// ��� R�� ���� ���� ���� ���� ���Ѵ�.
	int r = R.GetRowNum();
	int c = R.GetColNum();

	// ��� R�� free ������ pivot ������ �ε����� ���Ѵ�.
	std::vector<int> FreeIdx;
	std::vector<int> PivotIdx;
	int tmp = 0;
	for (int i = 0; i < c; ++i)
	{
		EgVec x = R.GetColVec(i);
		if (EQ(x[tmp], 1.0, 1.0e-6))
		{
			tmp++;
			PivotIdx.push_back(i);
		}
		else
		{
			FreeIdx.push_back(i);
		}
	}

	// Free �� ���͸� ������ ��� B�� �����Ѵ�.
	int NumFree = (int)FreeIdx.size();
	EgMat B(r, NumFree);
	for (int i = 0; i < NumFree; ++i)
	{
		EgVec x = R.GetColVec(FreeIdx[i]);
		B.SetColVec(i, -x);
	}

	// Null ������ ������ ������ ��� N�� �����Ѵ�.
	EgMat N(c, NumFree);
	EgMat I(NumFree, NumFree);
	I.SetIdentity();
	for (int i = 0; i < NumFree; ++i)
	{
		EgVec x = I.GetRowVec(i);
		N.SetRowVec(FreeIdx[i], x);
	}

	// Free ������ �� ������ ���� ������ ��� N�� Pivot ���� ä���.
	int NumPivot = (int)PivotIdx.size();
	for (int i = 0; i < NumPivot; ++i)
	{
		EgVec x = B.GetRowVec(i);
		N.SetRowVec(PivotIdx[i], x);
	}

	// ������ ������ ��ȯ�Ѵ�.
	return N;
}

/*!
*	\brief	����� rank (������ ������ Ȥ�� �຤���� ��)�� ���
*
*	\param	m[in]	���
*
*	\return ������ ����� rank�� ��ȯ�Ѵ�.
*/
int rank(const EgMat &m)
{
	int rank = 0;
	EgMat T = ref(m);
	EgVec Zero(m.GetColNum());

	for (int i = 0; i < m.GetRowNum(); i++)
		if (T.GetRowVec(i) != Zero)
			rank++;

	return rank;
}

/*!
*	\brief	����� nullity �� ���
*
*	\param	m[in]	���
*
*	\return ������ nullity�� ��ȯ�Ѵ�.
*/
int nullity(const EgMat &m)
{
	int rnk = rank(m);
	int c = m.GetColNum();
	return (c - rnk);
}

/*!
*	\brief	��� A�� L(�ϻﰢ���)�� U(��ﰢ���)�� ������ ����
*
*	\param	A[in]	��� �������
*	\param	L[out]	�ϻﰢ����� �����
*	\param	U[out]	��ﰢ����� �����
*
*	\return ����: true, ����: false
*/
int dcmp_lu(const EgMat &A, EgMat &L, EgMat &U)
{
	assert(A.m_nRow == A.m_nCol);
	assert(L.m_nRow == A.m_nRow);
	assert(L.m_nCol == A.m_nCol);
	assert(U.m_nRow == A.m_nRow);
	assert(U.m_nCol == A.m_nCol);
	int i, j, k, n;
	double big, dum, sum;
	n = A.m_nRow;

	EgVec vv(n);  // vv stores the implicit scaling of each row
	EgMat AA(A);
	L.SetIdentity();
	U.SetZeros();

	for (i = 0; i < n; i++)
	{
		big = 0.0;
		for (j = 0; j < n; j++)
			if (ABS(AA[i][j]) > big)
				big = ABS(AA[i][j]);
		if (big == 0.0)
		{
			printf("Singular matrix in dcmp_lu()...\n");
			return false;
		}
	}

	for (j = 0; j < n; j++)			// loop over columns of Crout's method
	{
		for (i = 0; i < j; i++)		// equation (2.3.12) except i=j
		{
			sum = AA[i][j];
			for (k = 0; k < i; k++)
				sum -= AA[i][k] * AA[k][j];
			AA[i][j] = sum;
		}
		for (i = j; i < n; i++) 	// i=j of equation (2.3.12) and i= j+1 .. N
		{							// of equation (2.3.13)
			sum = AA[i][j];
			for (k = 0; k < j; k++)
				sum -= AA[i][k] * AA[k][j];
			AA[i][j] = sum;
		}
		if (AA[j][j] == 0.0)
			AA[j][j] = 1.0e-15;
		/*
		if the pivot element is zero the matrix is singular (at least the
		precision of the algorithm). For some applications on singular
		matrices, it is desirable to substitute TINY for zero
		*/
		if (j != n - 1)				// now finally, divide by the pivot element
		{
			dum = 1.0 / AA[j][j];
			for (i = j + 1; i < n; i++)
				AA[i][j] *= dum;
		}
	}		// go back for the next column in the reduction

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (j >= i)
				U[i][j] = AA[i][j];
			else
				L[i][j] = AA[i][j];
		}
	}
	return true;
}

/*!
*	\brief	��� A�� Householder ����� ����Ͽ� Q * R�� ����
*
*	\param	A[in]	m x n ������
*	\param	Q[out]	m x m �������
*	\param	R[out]	m x n ��ﰢ���
*
*	\return �����ϸ� -1, �����ϸ� householder ����� ������ ȸ���� ��ȯ�Ѵ�.
*/
int dcmp_qr(const EgMat &A, EgMat &Q, EgMat &R)
{
	if (Q.m_nRow != A.m_nRow || Q.m_nCol != A.m_nRow)
		return -1;
	if (R.m_nRow != A.m_nRow || R.m_nCol != A.m_nCol)
		return -1;

	int i, j, k;
	double alpha, beta;
	Q.SetIdentity();
	R = A;
	EgVec u(A.m_nRow);				// Target column std::vector 	
	EgMat H(A.m_nRow, A.m_nRow);	// HouseHolder Matrix.

	for (k = 0; k < A.m_nCol; k++) // for each column
	{
		alpha = 0.0;
		for (i = k; i < A.m_nRow; i++)
		{
			alpha = alpha + SQR(R[i][k]);
			u[i] = R[i][k];
		}
		alpha = SQRT(alpha);
		if (EQ_ZERO(alpha, MTYPE_EPS))
			break;
		beta = 1.0 / (alpha * alpha + SIGN1(alpha, u[k]) * u[k]);
		u[k] = u[k] + SIGN1(alpha, u[k]);

		H.SetIdentity();
		for (i = k; i < A.m_nRow; i++)
		{
			for (j = k; j < A.m_nRow; j++)
			{
				H[i][j] -= beta * u[i] * u[j]; // Construct HouseHolder Matrix. -> Beautiful Computation.
			}
		}

		Q = Q * H;
		R = H * R;
	}
	return k;
}

/*!
*	\brief	LU ���ظ� ����, ��� A�� ��Ľ��� ���Ѵ�.
*
*	\param	A[in]	n x n ��� ���
*
*	\return ��Ľ��� ���� ��ȯ�Ѵ�.
*/
double det(const EgMat &A)
{
	assert(A.IsSquare());

	double d;
	if (A.m_nRow == 3)	// 3 by 3 matrix
	{
		d = (A[0][0] * A[1][1] * A[2][2] + A[1][0] * A[2][1] * A[0][2] + A[0][1] * A[1][2] * A[2][0])
			- (A[0][2] * A[1][1] * A[2][0] + A[0][1] * A[1][0] * A[2][2] + A[1][2] * A[2][1] * A[0][0]);
	}
	else  // n by n matrix
	{
		int n = A.m_nRow;
		EgMat LU(A);
		EgVec Idx(n);
		dcmp_lu(LU, Idx, d);

		for (int i = 0; i < n; i++)
			d *= LU[i][i];
	}

	return d;
}

/*!
*	\brief	LU ���ظ� ����, ��� A�� ������� ���Ѵ�.
*
*	\param	A[in]	n x n ������.
*
*	\return ��� A�� ������� ��ȯ�Ѵ�.
*/
EgMat inv(const EgMat &A)
{
	assert(A.IsSquare());
	int n = A.m_nRow;
	EgMat ret(n, n);

	if (n == 3)	// 3 x 3 matrix
	{
		double det = (A[0][0] * A[1][1] * A[2][2] + A[1][0] * A[2][1] * A[0][2] + A[0][1] * A[1][2] * A[2][0])
			- (A[0][2] * A[1][1] * A[2][0] + A[0][1] * A[1][0] * A[2][2] + A[1][2] * A[2][1] * A[0][0]);

		if (EQ(det, 0.0, 1.0e-6))
		{
			printf("[Error]: EgMat inv(...)\n");
			return ret;
		}

		ret[0][0] = (A[1][1] * A[2][2] - A[1][2] * A[2][1]) / det;
		ret[0][1] = -(A[0][1] * A[2][2] - A[0][2] * A[2][1]) / det;
		ret[0][2] = (A[0][1] * A[1][2] - A[0][2] * A[1][1]) / det;
		ret[1][0] = -(A[1][0] * A[2][2] - A[1][2] * A[2][0]) / det;
		ret[1][1] = (A[0][0] * A[2][2] - A[0][2] * A[2][0]) / det;
		ret[1][2] = -(A[0][0] * A[1][2] - A[0][2] * A[1][0]) / det;
		ret[2][0] = (A[1][0] * A[2][1] - A[1][1] * A[2][0]) / det;
		ret[2][1] = -(A[0][0] * A[2][1] - A[0][1] * A[2][0]) / det;
		ret[2][2] = (A[0][0] * A[1][1] - A[0][1] * A[1][0]) / det;
	}
	else		// general matrix (n x n, n = 4, 5, 6, ....)
	{
		double d;
		EgVec Idx(n);
		EgMat LU(A);

		// for fast inverse computation remove determinant check.
		d = det(LU);
		assert(!EQ_ZERO(d, MTYPE_EPS));

		dcmp_lu(LU, Idx, d);
		for (int j = 0; j < n; j++)
		{
			EgVec x(n);
			EgVec b(n);
			b[j] = 1.0;
			lin_sys_lu(LU, x, b, Idx);
			ret.SetColVec(j, x);
		}
	}

	return ret;
}

/*!
*	\brief	��� A�� �κ������ ��ȯ�Ѵ�.
*
*	\param	A[in]	���
*	\param	r0[in]	���� ���� �ε���
*	\param	c0[in]	���� ���� �ε���
*	\param	r1[in]	�� ���� �ε���
*	\param	c1[in]	�� ���� �ε���
*
*	\return ��� A�� �κ������ ��ȯ�Ѵ�.
*/
EgMat sub(const EgMat &A, int r0, int c0, int r1, int c1)
{
	int r = r1 - r0 + 1;
	int c = c1 - c0 + 1;
	EgMat ret(r, c);
	for (int i = 0; i < r; ++i)
		for (int j = 0; j < c; ++j)
			ret[i][j] = A[r0 + i][c0 + j];

	return ret;
}

/*!
*	\brief	��� A�� L(�ϻﰢ���)�� U(��ﰢ���)�� ��ȯ(�κ��Ǻ���(�౳ȯ)�� ����)
*			���� ��, lin_sys_lu(A, x, b, Idx)�� ȣ���Ͽ� ���� �ý��� Ax = b �� �����ϴ� ���� x�� ����
*
*	\param	A[in\out]	n by n ������� LU ���·� ��ȯ�ȴ�.
*	\param	Idx[out]	�� ��ȯ ������ ����ȴ�.
*	\param	d[out]		�� ��ȯ���� ¦���̸� 1, Ȧ���̸� -1 �� ����ȴ�.
*
*	\return �����ϸ� true, �����ϸ� false�� ��ȯ�Ѵ�.
*/
int dcmp_lu(EgMat &A, EgVec &Idx, double &d)
{
	assert(A.m_nRow == A.m_nCol);
	assert(Idx.GetDim() == A.m_nRow);
	int i, imax, j, k, n;
	double big, dum, sum, temp;
	n = A.m_nRow;
	EgVec vv(n);  // vv stores the implicit scaling of each row

	d = 1.0;
	for (i = 0; i < n; i++)
	{
		big = 0.0;
		for (j = 0; j < n; j++)
			if ((temp = ABS(A[i][j])) > big)
				big = temp;
		if (big == 0.0)
		{
			printf("[Error]: Singular matrix in dcmp_lu()...\n");
			return false;
		}
		// No nonzero largest element
		vv[i] = 1.0 / big;		// save the scaling
	}

	for (j = 0; j < n; j++)			// loop over columns of Crout's method
	{
		for (i = 0; i < j; i++)		// equation (2.3.12) except i=j
		{
			sum = A[i][j];
			for (k = 0; k < i; k++)
				sum -= A[i][k] * A[k][j];
			A[i][j] = sum;
		}
		big = 0.0;					// initialize for the search for largest pivot element
		for (i = j; i < n; i++) 	// i=j of equation (2.3.12) and i= j+1 .. N
		{							// of equation (2.3.13)
			sum = A[i][j];
			for (k = 0; k < j; k++)
				sum -= A[i][k] * A[k][j];
			A[i][j] = sum;
			if ((dum = vv[i] * ABS(sum)) >= big)
			{
				// is the figure of merit for the pivot better than the best so far
				big = dum;
				imax = i;
			}
		}
		if (j != imax) 			// Do we need to interchange rows?
		{
			A.ExchangeRows(j, imax);
			d = -d;			// and change the parity of d
			vv[imax] = vv[j];	// also interchange the scale factor
		}

		Idx[j] = imax;
		if (A[j][j] == 0.0)
			A[j][j] = 1e-15;
		/*
		if the pivot element is zero the matrix is singular (at least the
		precision of the algorithm). For some applications on singular
		matrices , it is desirable to substitute TINY for zero
		*/
		if (j != n - 1)				// now finally, divide by the pivot element
		{
			dum = 1.0 / (A[j][j]);
			for (i = j + 1; i < n; i++)
				A[i][j] *= dum;
		}
	}		// go back for the next column in the reduction
	return true;
}

/*!
*	\brief	SVD�� �̿��Ͽ� ��� A�� A = U * W * tr(V)�� ����
*
*	\param	A[in]	m x n ��� (m >= n)
*	\param	U[out]	m x n ��ķμ� �����ʹ� A�� �������� ���������� ����ȴ�.
*	\param	S[out]	Ư�̰��� ����� n ��������. W�� S�� ���ҷ� �����Ǵ� �밢�� ���
*	\param	V[out]	n x n ��ķμ� �����ʹ� A�� ����� �� �ΰ����� ���������� ����ȴ�.
*/
void dcmp_sv(const EgMat &A, EgMat &U, EgVec &S, EgMat &V)
{
	int flag, i, its, j, jj, k, l, nm;
	double c, f, h, s, x, y, z;
	double anorm = 0.0, g = 0.0, scale = 0.0;
	int m = A.m_nRow;
	int n = A.m_nCol;
	assert(m >= n);
	assert(m == U.m_nRow && n == U.m_nCol);
	EgVec rv1(n);
	U = A;
	for (i = 0; i < n; i++)
	{
		l = i + 1;
		rv1[i] = scale * g;
		g = s = scale = 0.0;
		if (i < m)
		{
			for (k = i; k < m; k++)
				scale += fabs(U[k][i]);
			if (scale)
			{
				for (k = i; k < m; k++)
				{
					U[k][i] /= scale;
					s += U[k][i] * U[k][i];
				}
				f = U[i][i];
				g = -SIGN2(SQRT(s), f);
				h = f * g - s;
				U[i][i] = f - g;
				if (i != n - 1)
				{
					for (j = l; j < n; j++)
					{
						for (s = 0.0, k = i; k < m; k++)
							s += U[k][i] * U[k][j];
						f = s / h;
						for (k = i; k < m; k++)
							U[k][j] += f * U[k][i];
					}
				}
				for (k = i; k < m; k++)
					U[k][i] *= scale;
			}
		}
		S[i] = scale * g;
		g = s = scale = 0.0;
		if (i <= m - 1 && i != n - 1)
		{
			for (k = l; k < n; k++)
				scale += fabs(U[i][k]);
			if (scale)
			{
				for (k = l; k < n; k++)
				{
					U[i][k] /= scale;
					s += U[i][k] * U[i][k];
				}
				f = U[i][l];
				g = -SIGN2(SQRT(s), f);
				h = f * g - s;
				U[i][l] = f - g;
				for (k = l; k < n; k++)
					rv1[k] = U[i][k] / h;
				if (i != m - 1)
				{
					for (j = l; j < m; j++)
					{
						for (s = 0.0, k = l; k < n; k++)
							s += U[j][k] * U[i][k];
						for (k = l; k < n; k++)
							U[j][k] += s * rv1[k];
					}
				}
				for (k = l; k < n; k++)
					U[i][k] *= scale;
			}
		}
		s = fabs(S[i]) + fabs(rv1[i]);
		anorm = MAX(anorm, s);
	}
	for (i = n - 1; i >= 0; i--)
	{
		if (i < n - 1)
		{
			if (g)
			{
				for (j = l; j < n; j++)
					V[j][i] = (U[i][j] / U[i][l]) / g;
				for (j = l; j < n; j++)
				{
					for (s = 0.0, k = l; k < n; k++)
						s += U[i][k] * V[k][j];
					for (k = l; k < n; k++)
						V[k][j] += s * V[k][i];
				}
			}
			for (j = l; j < n; j++)
				V[i][j] = V[j][i] = 0.0;
		}
		V[i][i] = 1.0;
		g = rv1[i];
		l = i;
	}
	for (i = n - 1; i >= 0; i--)
	{
		l = i + 1;
		g = S[i];
		if (i < n - 1)
			for (j = l; j < n; j++)
				U[i][j] = 0.0;
		if (g)
		{
			g = 1.0 / g;
			if (i != n - 1)
			{
				for (j = l; j < n; j++)
				{
					for (s = 0.0, k = l; k < m; k++)
						s += U[k][i] * U[k][j];
					f = (s / U[i][i]) * g;
					for (k = i; k < m; k++)
						U[k][j] += f * U[k][i];
				}
			}
			for (j = i; j < m; j++)
				U[j][i] *= g;
		}
		else
		{
			for (j = i; j < m; j++)
				U[j][i] = 0.0;
		}
		++U[i][i];
	}
	for (k = n - 1; k >= 0; k--)
	{
		for (its = 1; its <= 30; its++)
		{
			flag = 1;
			for (l = k; l >= 0; l--)
			{
				nm = l - 1;
				if (fabs(rv1[l]) + anorm == anorm)
				{
					flag = 0;
					break;
				}
				if (fabs(S[nm]) + anorm == anorm)
					break;
			}
			if (flag)
			{
				c = 0.0;
				s = 1.0;
				for (i = l; i <= k; i++)
				{
					f = s * rv1[i];
					if (fabs(f) + anorm != anorm)
					{
						g = S[i];
						h = PYTHAG(f, g);
						S[i] = h;
						h = 1.0 / h;
						c = g * h;
						s = (-f * h);
						for (j = 0; j < m; j++)
						{
							y = U[j][nm];
							z = U[j][i];
							U[j][nm] = y * c + z * s;
							U[j][i] = z * c - y * s;
						}
					}
				}
			}
			z = S[k];
			if (l == k)
			{
				if (z < 0.0)
				{
					S[k] = -z;
					for (j = 0; j < n; j++)
						V[j][k] = (-V[j][k]);
				}
				break;
			}
			//if (its == 30)
			//	cerr << "No convergence in 30 SVDCMP iterations";
			x = S[l];
			nm = k - 1;
			y = S[nm];
			g = rv1[nm];
			h = rv1[k];
			f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
			g = PYTHAG(f, 1.0);
			f = ((x - z) * (x + z) + h * ((y / (f + SIGN2(g, f))) - h)) / x;
			c = s = 1.0;
			for (j = l; j <= nm; j++)
			{
				i = j + 1;
				g = rv1[i];
				y = S[i];
				h = s * g;
				g = c * g;
				z = PYTHAG(f, h);
				rv1[j] = z;
				c = f / z;
				s = h / z;
				f = x * c + g * s;
				g = g * c - x * s;
				h = y * s;
				y = y * c;
				for (jj = 0; jj < n; jj++)
				{
					x = V[jj][j];
					z = V[jj][i];
					V[jj][j] = x * c + z * s;
					V[jj][i] = z * c - x * s;
				}
				z = PYTHAG(f, h);
				S[j] = z;
				if (z)
				{
					z = 1.0 / z;
					c = f * z;
					s = h * z;
				}
				f = (c * g) + (s * y);
				x = (c * y) - (s * g);
				for (jj = 0; jj < m; jj++)
				{
					y = U[jj][j];
					z = U[jj][i];
					U[jj][j] = y * c + z * s;
					U[jj][i] = z * c - y * s;
				}
			}
			rv1[l] = 0.0;
			rv1[k] = f;
			S[k] = x;
		}
	}
}

/*!
*	\brief	���� ������ Ax = b�� �ظ� ���Ѵ�.
*
*	\param	A[in]		LU�� ���ص� ���(dcmp_lu(A, Idx, d)�� ���)
*	\param	x[out]		�ذ� ����� ����
*	\param	b[in]		��� ����
*	\param	Idx[in]		���� ��ȯ������ ������ ����(dcmp_lu(A, Idx, d)�� ���)
*/
void lin_sys_lu(const EgMat &A, EgVec &x, const EgVec &b, const EgVec &Idx)
{
	assert(A.m_nRow == A.m_nCol);
	assert(A.m_nRow == x.GetDim());
	assert(x.GetDim() == b.GetDim());
	assert(Idx.GetDim() == x.GetDim());
	x = b;

	int i, ii = -1, ip, j, n;
	double sum;
	n = A.m_nRow;

	for (i = 0; i < n; i++)			// when ii is set to a positive value, it
	{								// will become the index of the first non-
		ip = (int)Idx[i];			// vanishing element of b. We now do the
		sum = x[ip];				// forward substitution, equation (2.3.6).
		x[ip] = x[i];				// The only new wrinkle is to unscalable 
		if (ii >= 0)				// the permutation as we go
			for (j = ii; j < i; j++)
				sum -= A[i][j] * x[j];
		else if (sum)				// A nonzero element was encountered, so 
			ii = i;					// from now on we will have to do the sums
		x[i] = sum;					// in the loop above
	}

	for (i = n - 1; i >= 0; i--) // Now we do the back-substitution equation
	{							 // (2.3.7)
		sum = x[i];
		for (j = i + 1; j < n; j++)
			sum -= A[i][j] * x[j];
		x[i] = sum / A[i][i];		// Store a component of the solution std::vector X
	}
}

/*!
*	\brief	���������� Ax = b �� �����ϴ� minimum length least square �ظ� ����
*
*	\param	A[in]	m x n ��� (m >= n)
*	\param	x[out]	�ذ� ����ȴ�.
*	\param	b[in]	�������
*
*	\return ���Ǽ�(condition number)�� ��ȯ�Ѵ�.
*/
double lin_sys_sv(const EgMat &A, EgVec &x, const EgVec &b)
{
	int i, j;
	int m = A.m_nRow;
	int n = A.m_nCol;
	double min, max, cnum;

	EgMat U(m, n);
	EgVec S(n);
	EgMat V(n, n);
	dcmp_sv(A, U, S, V);
	min = max = S[0];

	for (i = 0; i < n; i++)
	{
		min = MIN(min, S[i]);
		max = MAX(max, S[i]);
		cnum = min / max;
	}

	EgVec tmp(n);
	for (j = 0; j < n; j++)
	{
		double s = 0.0;
		if (!EQ_ZERO(S[j], MTYPE_EPS))
		{
			for (i = 0; i < m; i++)
				s += U[i][j] * b[i];
			s /= S[j];
		}
		tmp[j] = s;
	}
	for (j = 0; j < n; j++)
	{
		double s = 0.0;
		for (i = 0; i < n; i++)
			s += V[j][i] * tmp[i];
		x[j] = s;
	}
	return cnum;
}

/*!
*	\brief	���������� Ax = b �� �����ϴ� minimum length least square �� x�� ���Ѵ�.\n
*			- �� �Լ��� ȣ���ϱ� ����, dcmp_sv(A, U, S, V)�� ȣ���Ͽ� ��� A�� ���ص� ����� ����Ѵ�.
*			- �ѹ� ���ص� ��� A�� ������� b�� �ٲٸ鼭 �ظ� ���ϰ� �ȴ�.
*
*	\param	U[in]	m x n ��ķ� dcmp_sv(A, U, S, V)�� ���
*	\param	S[in]	n ���� ���ͷ� dcmp_sv(A, U, S, V)�� ���
*	\param	V[in]	n x n ��ķ� dcmp_sv(A, U, S, V)�� ���
*	\param	x[out]	�ذ� ����� n ���� ����
*	\param	b[in]	m ���� �������
*
*	\return ���Ǽ�(condition number)�� ��ȯ�Ѵ�.
*/
double lin_sys_sv(const EgMat &U, const EgVec &S, const EgMat &V, EgVec &x, const EgVec &b)
{
	int i, j;
	int m = U.m_nRow;
	int n = U.m_nCol;
	double min, max, cnum;
	min = max = S[0];

	for (i = 0; i < n; i++)
	{
		min = MIN(min, S[i]);
		max = MAX(max, S[i]);
		cnum = min / max;
	}
	EgVec tmp(n);
	for (j = 0; j < n; j++)
	{
		double s = 0.0;
		if (!EQ_ZERO(S[j], MTYPE_EPS))
		{
			for (i = 0; i < m; i++)
				s += U[i][j] * b[i];
			s /= S[j];
		}
		tmp[j] = s;
	}
	for (j = 0; j < n; j++)
	{
		double s = 0.0;
		for (i = 0; i < n; i++)
			s += V[j][i] * tmp[i];
		x[j] = s;
	}
	return cnum;
}

/*!
*	\brief	Conjugate gradient ����� ����Ͽ� ���������� Ax = b �� Ǭ��.\n
*			- ��� A�� ��Ī����̾�� �Ѵ�.
*			- sparse �����ý����� Ǯ������ ȿ������ ����̴�.
*
*	\param	A[in]		n x n ��Ī���.
*	\param	x[in/out]	n ���� �ʱ� �� ����.
*	\param	b[in]		n ���� ��� ����.
*	\param	iter[in]	�ݺ�ȸ��, ���� �ݺ�ȸ���� n�̸� ��Ȯ�� �ظ� ���Ѵ�.
*
*	\return ���� �ݺ�ȸ���� ��ȯ�Ѵ�.
*/
int lin_sys_cg(const EgMat &A, EgVec &x, const EgVec &b, int iter)
{
	int n = A.m_nRow;
	assert(A.IsSquare());
	assert(n == x.GetDim());
	assert(n == b.GetDim());
	if (iter < 0 || iter > n)
		iter = n;

	EgVec tmp0(n);
	double alpha, beta, tmp1;
	EgVec r(b);	// initial residual.
	EgVec d(b);	// initial conjugate direction.

	// Conjugate Gradient Iteration
	// Such a simple algorithm... 
	// but great theorem....(Expanding Subspace Theorem)
	int i;
	for (i = 0; i < iter; ++i)
	{
		tmp0 = A * d;
		tmp1 = r * r;

		alpha = tmp1 / (d * tmp0);
		x = x + alpha * d;
		r = r - alpha * tmp0;
		beta = (r * r) / tmp1;
		d = r + beta * d;
	}

	return i;
}

/*!
*	\brief	QR ���ع��� �̿��Ͽ� ���������� Ax = b�� least square �ظ� ã�´�.
*
*	\param	A[in]	m x n ���.
*	\param	x[out]	n ���� �� ����.
*	\param	b[in]	n ���� ��� ����.
*
*	\return 1: ����, 0: ����.
*/
int lin_sys_qr(const EgMat &A, EgVec &x, const EgVec &b)
{
	int m, n, i, j, scs;
	m = A.m_nRow;
	n = A.m_nCol;

	EgMat Q(m, m);
	EgMat R(m, n);
	EgVec c(m);

	scs = dcmp_qr(A, Q, R);

	if (scs != A.m_nCol)
		return false;

	Q.SetTranspose();
	c = Q * b;

	for (i = n - 1; i >= 0; i--)
	{
		double sum = 0.0;
		for (j = i + 1; j < n; j++)
			sum += (R[i][j] * x[j]);
		x[i] = (c[i] - sum) / R[i][i];
	}
	return true;
}

/*!
*	\brief	��纯ȯ(similarity transformation)�� ����Ͽ� �Ǽ���Ī����� ��� eigenvector�� eigenvalue�� ���Ѵ�.
*
*	\param	A[in]	n x n ��Ī���.
*	\param	D[out]	n���� eigenvalue�� ����ȴ�.
*	\param	V[out]	n���� eigenvector (������)�� ����ȴ�.
*
*	\return ��纯ȯ�� ȸ���� ��ȯ�Ѵ�.
*/
#define ROTATE(a, i, j, k, l)\
{ \
	g = a[i][j]; \
	h = a[k][l]; \
	a[i][j] = g - s * (h + g * tau); \
	a[k][l] = h + s * (g - h * tau); \
}
int eigen_sys(const EgMat &A, EgVec &D, EgMat &V)
{
	int n = A.m_nRow;
	assert(A.m_nCol == n);
	assert(D.GetDim() == n);
	assert(V.m_nRow == n);
	assert(V.m_nCol == n);
	int j, iq, ip, i, nrot;
	double tresh, theta, tau, t, sm, s, h, g, c, *b, *z;

	b = new double[n];
	z = new double[n];

	EgMat B(A);
	V.SetIdentity();

	for (ip = 0; ip < n; ip++)
	{
		b[ip] = D[ip] = B[ip][ip];
		z[ip] = 0.0;
	}

	nrot = 0;

	for (i = 1; i <= 50; i++)
	{
		sm = 0.0;
		for (ip = 0; ip < n - 1; ip++)
		{
			for (iq = ip + 1; iq < n; iq++)
				sm += ABS(B[ip][iq]);
		}
		if (sm == 0.0)
		{
			delete[] b;
			delete[] z;
			return nrot;
		}
		if (i < 4)
			tresh = 0.2 * sm / (n * n);
		else
			tresh = 0.0;
		for (ip = 0; ip < n - 1; ip++)
			for (iq = ip + 1; iq < n; iq++)
			{
				g = 100.0 * ABS(B[ip][iq]);
				if (i > 4 && ABS((D[ip]) + g) == ABS(D[ip]) && (ABS(D[iq]) + g) == ABS(D[iq]))
					B[ip][iq] = 0.0;
				else if (ABS(B[ip][iq]) > tresh)
				{
					h = D[iq] - D[ip];
					if ((ABS(h) + g) == ABS(h))
						t = (B[ip][iq]) / h;
					else
					{
						theta = 0.5 * h / B[ip][iq];
						t = 1.0 / (ABS(theta) + SQRT((1.0 + SQR(theta))));
						if (theta < 0.0)
							t = -t;
					}
					c = 1.0 / SQRT(1 + SQR(t));
					s = t * c;
					tau = s / (1.0 + c);
					h = t * B[ip][iq];
					z[ip] -= h;
					z[iq] += h;
					D[ip] -= h;
					D[iq] += h;
					B[ip][iq] = 0.0;
					for (j = 0; j < ip; j++)
						ROTATE(B, j, ip, j, iq)
						for (j = ip + 1; j < iq; j++)
							ROTATE(B, ip, j, j, iq)
							for (j = iq + 1; j < n; j++)
								ROTATE(B, ip, j, iq, j)
								for (j = 0; j < n; j++)
									ROTATE(V, j, ip, j, iq)
									++nrot;
				}
			}
		for (ip = 0; ip < n; ip++)
		{
			b[ip] += z[ip];
			D[ip] = b[ip];
			z[ip] = 0.0;
		}
	}
	return nrot;
}

/*!
*	\brief	dcmp_sv �� ����Ͽ� �ּ��� (principle component)�� ã�´�.
*	\note	�Լ�ȣ�� ��, Data ������ ����� ������ ��ġ�ϵ��� �̵��ȴ�.
*
*	\param	Data[in]		m x n ������ ��� (m: �������� ����, n: �������� ����)
*	\param	PC[out]			�����ͷ� ǥ���� �ּ���.
*	\param	SigVal[out]		�ּ��к����� Ư�̰��� ������ n ���� ����.
*	\param	Mean[out]		��պ���
*/
void pca(EgMat &Data, EgMat &PC, EgVec &SigVal, EgVec &Mean)
{
	int i;
	int num = Data.m_nRow;
	int dim = Data.m_nCol;
	assert(PC.m_nRow == dim);
	assert(PC.m_nCol == dim);
	assert(SigVal.GetDim() == dim);

	// 1. Get mean std::vector
	EgVec data(dim);
	for (i = 0; i < num; i++)
	{
		data = Data.GetRowVec(i);
		Mean += data;
	}
	Mean = Mean / (double)num;
	// for debug.
	// cerr << "mean std::vector:" << std::endl << Mean;

	// 2. Translate to mean std::vector.
	for (i = 0; i < num; i++)
	{
		data = Data.GetRowVec(i);
		data -= Mean;
		Data.SetRowVec(i, data);
	}
	Data /= SQRT((double)num);
	// for debug.
	//printf( "translate matrix:\n" );
	//Data.Print();

	// 3. Do singular Value Decomposition.
	EgMat U(num, dim), V(dim, dim);
	EgVec W(dim);
	dcmp_sv(Data, U, W, V);

	// 4. Sorting by its singular value size.
	for (i = 0; i < dim - 1; i++)
	{
		if (W[i] < W[i + 1])
		{
			// swap eigenvalue.
			SWAP(double, W[i], W[i + 1]);
			// swap eigenvector.
			V.ExchangeCols(i, i + 1);
			U.ExchangeCols(i, i + 1);
		}
	}

	PC = V;
	SigVal = W;
}

/***********************/
/* EgQuat ��ƿ �Լ� ���� */
/***********************/
/*!
*	\brief	���ʹϿ��� ũ�⸦ ���Ѵ�.
*
*	\param	q[in] ��� ���ʹϿ�
*
*	\return ���ʹϿ��� ũ�⸦ ��ȯ�Ѵ�.
*/
double norm(const EgQuat &q)
{
	return SQRT(SQR(q.m_W) + SQR(q.m_X) + SQR(q.m_Y) + SQR(q.m_Z));
}

/*!
*	\brief	���ʹϿ��� ������ ���ϴ� �Լ�.
*
*	\param	q[in] ��� ���ʹϿ�
*
*	\return ���ʹϿ��� ������ ��ȯ�Ѵ�.
*/
EgQuat inv(const EgQuat &q)
{
	EgQuat ret(q);
	if (!ret.IsUnitQuater())
	{
		double norm_sqr = SQR(ret.m_W) + SQR(ret.m_X) + SQR(ret.m_Y) + SQR(ret.m_Z);
		ret /= norm_sqr;
	}
	ret.SetConjugate();
	return ret;
}

/*!
*	\brief	����(pure imaginary ���ʹϿ�)�� exponential mapping�� ���Ѵ�.
*
*	\param	v[in]	���ʹϿ� q = (0, v)
*
*	\return ����ȸ���� v/||v|| �� �߽����� 2 * ||v|| �� ȸ������ ���� ���ʹϿ��� ��ȯ�Ѵ�.
*/
EgQuat exp(const EgVec3 &v)
{
	double nm, s;
	EgQuat rq;

	nm = norm(v);
	if (EQ(nm, 0.0, MTYPE_EPS))
	{
		rq.SetIdentity();
		return rq;
	}

	rq.m_W = cos(nm);
	s = sin(nm) / nm;
	rq.m_X = s * v[0];
	rq.m_Y = s * v[1];
	rq.m_Z = s * v[2];

	return rq;
}

/*!
*	\brief	���ʹϿ� q�� �αװ��� ���Ѵ�.
*
*	\param	q[in]	��� ���ʹϿ�
*
*	\return ����� ���ʹϿ��� ����κ� Log(q) = (0, x, y, z)�� ��ȯ�Ѵ�.
*/
EgVec3 log(const EgQuat &q)
{
	EgVec3 rv;
	if (q.IsIdentity())
	{
		rv.Set(0.0, 0.0, 0.0);
		return rv;
	}
	double theta, s;
	theta = acos(q.m_W);
	s = theta / sin(theta);
	rv.Set(s * q.m_X, s * q.m_Y, s * q.m_Z);
	return rv;
}

/*!
*	\brief  �� ���ʹϿ��� �����Ѵ�.
*
*	\param	q1[in]	���ʹϿ�
*	\param	q2[in]	���ʹϿ�
*
*	\return �������� ��ȯ�Ѵ�.
*/
double quat_dot(const EgQuat &q1, const EgQuat &q2)
{
	return q1.m_W * q2.m_W + q1.m_X * q2.m_X + q1.m_Y * q2.m_Y + q1.m_Z * q2.m_Z;
}

/*!
*	\brief	���ʹϿ� q1, q2�� Spherical linear interpolation �Ѵ�.
*
*	\param	q1[in]	���ʹϿ�
*	\param	q2[in]	���ʹϿ�
*	\param	t[in]	������ �ð�
*
*	\return ������ ���ʹϿ��� ��ȯ�Ѵ�.
*/
EgQuat slerp(EgQuat q1, EgQuat q2, double t)
{
	if (quat_dot(q1, q2) < 0.0)
		q2.SetNegate();

	EgQuat q;
	double cs = q1.m_W * q2.m_W + q1.m_X * q2.m_X + q1.m_Y * q2.m_Y + q1.m_Z * q2.m_Z;

	// �� ���ʹϿ� ������ ������ 180���� �ſ� ����� ���
	if (EQ(cs, -1.0, 1.0e-6))
	{
		q = q1 * sin((0.5 - t) * M_PI) + q2 * sin(t * M_PI);
	}
	else if (EQ(cs, 1.0, 1.0e-6)) // �� ���ʹϿ� ������ ������ 0���� �ſ� ����� ���
	{
		q = q1 * (1.0 - t) + q2 * t;
	}
	else // �Ϲ����� ���
	{
		double theta = acos(cs);
		q = (q1 * sin((1.0 - t) * theta) + q2 * sin(t * theta)) / sin(theta);
	}
	//if (1.0 + cs > 1.0e-6)
	//{
	//	if (1.0 - cs > 1.0e-6)
	//	{
	//		// �Ϲ����� ���
	//		double theta = acos(cs);
	//		q = (q1 * sin((1.0 - t) * theta) + q2 * sin(t * theta)) / sin(theta);
	//	}
	//	else // �� ���ʹϿ� ������ ������ 0���� �ſ� ����� ���
	//		q = q1 * (1.0 - t) + q2 * t;
	//}
	//else // �� ���ʹϿ� ������ ������ 180���� �ſ� ����� ���
	//	q = q1 * sin((0.5 - t) * M_PI) + q2 * sin(t * M_PI);

	q.Normalize();
	return q;
}

/***********************/
/* EgLine ��ƿ �Լ� ���� */
/***********************/
/*!
*	\brief	�� ������ ���������� �Ÿ��� ���Ѵ�.
*
*	\param	l[in]	����
*	\param	q[in]	�� ��
*
*	\return �� \a q���� ���� \a l������ �����Ÿ�
*/
double dist(const EgLine &l, const EgPos &q)
{
	return norm(proj(q - l.m_Pos, l.m_Dir) - (q - l.m_Pos));
}

/*!
*	\brief	�� ���� l1(t)�� l2(t)������ �ִ� �Ÿ��� ���Ѵ�.
*
*	\param	l1[in]	ù ��° ����
*	\param	l2[in]	�� ��° ����
*	\param	t1[in]	�ִ� �Ÿ��� ����� l1�� �ĸ����� ���� �����
*	\param	t2[in]	�ִ� �Ÿ��� ����� l2�� �ĸ����� ���� �����
*
*	\return �� ���� ������ �ִܰŸ��� ����Ͽ� ��ȯ�Ѵ� (�� ������ ������ ��� -1.0�� ��ȯ�Ѵ�).
*/
double dist(EgLine &l1, EgLine &l2, double &t1, double &t2)
{
	EgPos p1 = l1.m_Pos;
	EgPos p2 = l2.m_Pos;
	EgVec3 v1 = l1.m_Dir;
	EgVec3 v2 = l2.m_Dir;

	double a11 = v1 * v1;
	double a12 = -v2 * v1;
	double a21 = v1 * v2;
	double a22 = -v2 * v2;
	double b1 = (p1 - p2) * v1;
	double b2 = (p1 - p2) * v2;
	double D = a11 * a22 - a12 * a21;

	double ret = 0.0;
	if (EQ_ZERO(D, 1.0e-6))
	{
		t1 = 0.0;
		t2 = 0.0;
		ret = -1.0;
	}
	else
	{
		t1 = (a22 * b1 - a12 * b2) / D;
		t2 = (-a21 * b1 + a11 * b2) / D;

		EgPos q1 = l1.Eval(t1);
		EgPos q2 = l2.Eval(t2);

		ret = dist(q1, q2);
	}

	return ret;
}

/*!
*	\brief	������ ������ �������� ����Ѵ�.
*
*	\param	p[in]	�������� ����� ����
*	\param	l0[in]	ù ��° ������ ������ (ruling line)
*	\param	l1[in]	�� ��° ������ ������ (boundary line)
*	\param	t0[in]	ù ��° ������ �������� �Ķ���� ���� �����
*	\param	t1[in]	�� ��° ������ �������� �Ķ���� ���� �����
*
*	\return true: ����, false: ������ ���� ��ġ�� �ְų�, (0 ~ 1)���̸� ��� ���
*/
bool intersect_line_line(EgPos &p, EgLine &l0, EgLine &l1, double &t0, double &t1)
{
	double a = l0.m_Dir[0];
	double b = -l1.m_Dir[0];
	double c = l0.m_Dir[1];
	double d = -l1.m_Dir[1];
	double e = l1.m_Pos[0] - l0.m_Pos[0];
	double f = l1.m_Pos[1] - l0.m_Pos[1];

	double det = a * d - b * c;
	if (EQ(det, 0.0, 1.0e-6))
		return false;

	t0 = (d * e - b * f) / det;
	t1 = (a * f - c * e) / det;

	if (EQ(t0, 0.0, 1.0e-6))
		t0 = 0.0;

	if (EQ(t0, 1.0, 1.0e-6))
		t0 = 1.0;

	if (EQ(t1, 0.0, 1.0e-6))
		t1 = 0.0;

	if (EQ(t1, 1.0, 1.0e-6))
		t1 = 1.0;

	if (t0 < 0.0 || t0 > 1.0)
		return false;

	if (t1 < 0.0 || t1 > 1.0)
		return false;

	// p = l0.Eval(t0);
	p = l1.Eval(t1);

	return true;
}

/*!
*	\brief	������ ������ �������� ����Ѵ�.
*
*	\param	p[in]	�������� ����� ����
*	\param	l0[in]	ù ��° ������ ������
*	\param	l1[in]	�� ��° ������ ������
*
*	\return true: ����, false: ������ ���� ��ġ�� �ִ� ���
*/
bool intersect_line_line2(EgPos &p, EgLine &l0, EgLine &l1)
{
	double a = l0.m_Dir[0];
	double b = -l1.m_Dir[0];
	double c = l0.m_Dir[1];
	double d = -l1.m_Dir[1];
	double e = l1.m_Pos[0] - l0.m_Pos[0];
	double f = l1.m_Pos[1] - l0.m_Pos[1];

	double det = a * d - b * c;
	if (EQ(det, 0.0, 1.0e-6))
		return false;

	double t0, t1;
	t0 = (d * e - b * f) / det;
	t1 = (a * f - c * e) / det;

	// p = l0.Eval(t0);
	p = l1.Eval(t1);

	return true;
}

/*!
*	\brief	������ �ﰢ�� (p1p2p3)�� �������� ����Ѵ�.
*
*	\param	p1[in]				�ﰢ���� ù ��° ����(�ݽð� ����)
*	\param	p2[in]				�ﰢ���� �� ��° ����(�ݽð� ����)
*	\param	p3[in]				�ﰢ���� �� ��° ����(�ݽð� ����)
*	\param	l[in]				������ ������
*	\param	t[out]				�������� ���� �Ķ���Ͱ� �����
*	\param	q[out]				�������� �����
*	\param	alpha[out]			barycentric ��ǥ�� �����
*	\param	beta[out]			barycentric ��ǥ�� �����
*	\param	gamma[out]			barycentric ��ǥ�� �����
*	\param	bCullBackFace[in]	������ �ﰢ���� �޸�� ������ ��츦 �����Ϸ���. (����Ʈ: true).
*	\note	q = alpha * p1 + beta * p2 + gamma * p3, (alpha + beta + gamma = 1)
*
*	\return true: ����, false: ������ �ﰢ���� �����ϰų� �������� �ﰢ���� �ܺο� �����ϴ� ���.
*/
bool intersect_line_triangle(EgPos &p1, EgPos &p2, EgPos &p3, EgLine &l, double &t, EgPos &q, double &alpha, double &beta, double &gamma, bool bCullBackFace)
{
	EgVec3 e1, e2, u, v, w;
	double det;
	e1 = p1 - p3;
	e2 = p2 - p3;
	u = l.m_Dir ^ e2;
	det = e1 * u;

	if (bCullBackFace)	// ������ �ﰢ���� �޸�� ������ ���� ����.
	{
		if (det < MTYPE_EPS)
			return false;

		w = l.m_Pos - p3;
		alpha = w * u;
		if (alpha < 0.0 || alpha > det)
			return false;

		v = w ^ e1;
		beta = l.m_Dir * v;
		if (beta < 0.0 || alpha + beta > det)
			return false;

		t = e2 * v;

		alpha /= det;
		beta /= det;
		t /= det;
	}
	else
	{
		if (det > -MTYPE_EPS && det < MTYPE_EPS)
			return false;

		w = l.m_Pos - p3;
		alpha = w * u / det;
		if (alpha < 0.0 || alpha > 1.0)
			return false;

		v = w ^ e1;
		beta = l.m_Dir * v / det;
		if (beta < 0.0 || alpha + beta > 1.0)
			return false;

		t = e2 * v / det;
	}

	gamma = 1.0 - alpha - beta;
	q = l.Eval(t);
	return true;
}

/*!
*	\brief	������ �ﰢ�� (p1p2p3)�� �������� ����Ѵ�.
*
*	\param	p1[in]				�ﰢ���� ù ��° ����(�ݽð� ����)
*	\param	p2[in]				�ﰢ���� �� ��° ����(�ݽð� ����)
*	\param	p3[in]				�ﰢ���� �� ��° ����(�ݽð� ����)
*	\param	l[in]				������ ������
*	\param	ispt[out]			�������� �����
*	\param	bCullBackFace[in]	������ �ﰢ���� �޸�� ������ ��츦 �����Ϸ���. (����Ʈ: true).
*
*	\return true: ����, false: ������ �ﰢ���� �����ϰų� �������� �ﰢ���� �ܺο� �����ϴ� ���.
*/
bool intersect_line_triangle(EgPos &p1, EgPos &p2, EgPos &p3, EgLine &l, EgPos &ispt, bool bCullBackFace)
{
	EgVec3 e1, e2, u, v, w;
	double det;
	e1 = p1 - p3;
	e2 = p2 - p3;
	u = l.m_Dir ^ e2;
	det = e1 * u;

	double t;
	if (bCullBackFace)	// ������ �ﰢ���� �޸�� ������ ���� ����.
	{
		if (det < MTYPE_EPS)
			return false;

		w = l.m_Pos - p3;
		double alpha = w * u;
		if (alpha < 0.0 || alpha > det)
			return false;

		v = w ^ e1;
		double beta = l.m_Dir * v;
		if (beta < 0.0 || alpha + beta > det)
			return false;

		t = e2 * v;
		t /= det;
	}
	else
	{
		if (det > -MTYPE_EPS && det < MTYPE_EPS)
			return false;

		w = l.m_Pos - p3;
		double alpha = w * u / det;
		if (alpha < 0.0 || alpha > 1.0)
			return false;

		v = w ^ e1;
		double beta = l.m_Dir * v / det;
		if (beta < 0.0 || alpha + beta > 1.0)
			return false;

		t = e2 * v / det;
	}

	ispt = l.Eval(t);
	return true;
}

bool intersect_line_triangle2(EgPos &p1, EgPos &p2, EgPos &p3, EgLine &l, double &param, bool bCullBackFace)
{
	EgVec3 e1, e2, u, v, w;
	double det;
	e1 = p1 - p3;
	e2 = p2 - p3;
	u = l.m_Dir ^ e2;
	det = e1 * u;

	double t;
	if (bCullBackFace)	// ������ �ﰢ���� �޸�� ������ ���� ����.
	{
		if (det < MTYPE_EPS)
			return false;

		w = l.m_Pos - p3;
		double alpha = w * u;
		if (alpha < 0.0 || alpha > det)
			return false;

		v = w ^ e1;
		double beta = l.m_Dir * v;
		if (beta < 0.0 || alpha + beta > det)
			return false;

		t = e2 * v;
		t /= det;
	}
	else
	{
		if (det > -MTYPE_EPS && det < MTYPE_EPS)
			return false;

		w = l.m_Pos - p3;
		double alpha = w * u / det;
		if (alpha < 0.0 || alpha > 1.0)
			return false;

		v = w ^ e1;
		double beta = l.m_Dir * v / det;
		if (beta < 0.0 || alpha + beta > 1.0)
			return false;

		t = e2 * v / det;
	}

	param = t;
	return true;
}

/************************/
/* EgPlane ��ƿ �Լ� ���� */
/************************/
/*!
*	\brief	��� pi���� �� p������ ��ȣ�ִ� �Ÿ��� ����Ѵ�.
*
*	\param	pi[in]	����� ��ü
*	\param	p[in]	��� ���� ����.
*
*	\return ��� \a pi���� �� \a p������ ��ȣ�ִ� �Ÿ�
*/
double dist(EgPlane &pi, EgPos &p)
{
	return pi.Eval(p) / norm(pi.N());
}

/*!
*	\brief	�� p�� ��� pi�� �翵��Ų ���� ���Ѵ�.
*
*	\param	p[in]	��� ���� ��
*	\param	pi[in]	���
*
*	\return	�� p�� ��� pi�� �翵��Ų ���� ��ȯ�Ѵ�.
*/
EgPos proj(EgPos &p, EgPlane &pi)
{
	double s = -dist(pi, p) / norm(pi.N());
	return p + s * pi.N();
}

/*!
*	\brief	�� ����� ���� ������ ����Ѵ�.
*
*	\param	l[out]		���������� ����� ������ü
*	\param	pi1[in]		ù ��° ���
*	\param	pi2[in]		�� ��° ���
*
*	\return true: ����, false: �� ����� ������ ���.
*/
bool intersect_plane_plane(EgLine &l, EgPlane &pi1, EgPlane &pi2)
{
	double s = norm(pi1.N() ^ pi2.N());
	if (EQ_ZERO(s, MTYPE_EPS))
	{
		std::cout << "[Error]: two planes are parallel..." << std::endl;
		return false;
	}

	l.m_Dir = pi1.N() ^ pi2.N();

	EgPos p(0, 0, 0);
	EgPlane pi3(l.m_Dir, p);
	intersect_planes(p, pi1, pi2, pi3);

	l.m_Pos = p;
	return true;
}

/*!
*	\brief	������ ����� �������� ����Ѵ�.
*
*	\param	p[out]	�������� ����� ����
*	\param	l[in]	���� ��ü
*	\param	pi[in]	��� ��ü
*
*	\return true: ����, false: ������ ���� �����ϰų�, ����� ������ �����ϴ� ���.
*/
bool intersect_line_plane(EgPos &p, EgLine &l, EgPlane &pi)
{
	if (EQ_ZERO(l.m_Dir * pi.N(), MTYPE_EPS))
	{
		std::cout << "[Error]: line is parallel to plane..." << std::endl;
		return false;
	}

	double t = -pi.Eval(l.m_Pos) / (l.m_Dir * pi.N());
	p = l.Eval(t);
	return true;
}

/*!
*	\brief	�� ����� �������� ����Ѵ�.
*
*	\param	p[out] �������� ����� �� ��ü.
*	\param	pi1[in]		ù ��° ���
*	\param	pi2[in]		�� ��° ���
*	\param	pi3[in]		�� ��° ���
*
*	\return true: ����, false: �� ��� �� ��� �� ����� ��ġ�ϰų� �����ϴ� ���.
*/
bool intersect_planes(EgPos &p, EgPlane &pi1, EgPlane &pi2, EgPlane &pi3)
{
	EgMat A(3, 3);
	EgVec x(3), b(3), Idx(3);
	double tmp;

	A.SetRowVec(0, cast_vec(pi1.N()));
	A.SetRowVec(1, cast_vec(pi2.N()));
	A.SetRowVec(2, cast_vec(pi3.N()));

	if (rank(A) != 3)
	{
		std::cout << "[Error]: at least two planes are parallel !" << std::endl;
		return false;
	}

	b.Set(-pi1.m_Coords[3], -pi2.m_Coords[3], -pi3.m_Coords[3]);
	dcmp_lu(A, Idx, tmp);

	lin_sys_lu(A, x, b, Idx);
	p[0] = x[0];
	p[1] = x[1];
	p[2] = x[2];
	return true;
}

/************************/
/* EgTrans ��ƿ �Լ� ���� */
/************************/
/*!
*	\brief	��ü��ȯ�� ����ȯ�� ���Ѵ�.
*
*	\param	X[in]	��� ��ü
*
*	\return X�� ����ȯ�� ��ȯ�Ѵ�.
*/
EgTrans inv(const EgTrans &X)
{
	EgQuat q = inv(X.m_Q);
	EgVec3 t = -(q * X.m_T);
	return EgTrans(t, q);
}

/*!
*	\brief	��ü��ȯ�� �̵������� ���Ѵ�.
*
*	\param	X[in]	��ü��ȯ ��ü
*
*	\return X�� �̵������� ��ȯ�Ѵ�.
*/
EgVec3 part_trans(const EgTrans &X)
{
	return X.m_T;
}

/*!
*	\brief	��ü��ȯ�� ȸ�������� ���Ѵ�.
*
*	\param	X[in]	��ü��ȯ ��ü
*
*	\return X�� ȸ�������� ��ȯ�Ѵ�.
*/
EgQuat part_rot(const EgTrans &X)
{
	return X.m_Q;
}

/*!
*	\brief	��ȯ X�� ũ�⸦ ���Ѵ�.
*
*	\param	X[in]	��ü��ȯ
*
*	\return ��ȯ X�� ũ�⸦ ��ȯ�Ѵ�.
*/
double norm(const EgTrans &X)
{
	return norm(X.m_T) + norm(log(X.m_Q));
}

/*!
*	\brief	�� ��ȯ ������ �Ÿ��� ���Ѵ�
*
*	\param	X1[in]		ù ��° ��ȯ
*	\param	X2[in]		�� ��° ��ȯ
*
*	\return X1���� X2��ȯ������ �Ÿ��� ��ȯ�Ѵ�.
*/
double dist(const EgTrans &X1, const EgTrans &X2)
{
	return norm(inv(X1) * X2);
}

/*!
*	\brief	�� ��ȯ�� �����Ѵ�.
*
*	\param	X1[in]		ù ��° ��ȯ
*	\param	X2[in]		�� ��° ��ȯ
*	\param	t[in]		���� �Ķ����(0 ~ 1)
*
*	\return ������ ��ȯ�� ��ȯ�ȴ�.
*/
EgTrans lerp(EgTrans X1, EgTrans X2, const double t)
{
	if (quat_dot(X1.m_Q, X2.m_Q) < 0.0)
		X2.m_Q.SetNegate();

	EgTrans ret;
	ret.m_T = (1.0 - t) * X1.m_T + t * X2.m_T;
	ret.m_Q = slerp(X1.m_Q, X2.m_Q, t);

	return ret;
}

/*!
*	\brief	X ��ǥ�迡�� ǥ���� ��ȯ dX�� Y ��ǥ�迡�� ǥ���� ��ȯ���� ����Ѵ�.
*
*	\param	X[in]		������ǥ��
*	\param	dX[in]		������ǥ�� X���� ǥ���� ��ȯ
*	\param	Y[in]		�����ǥ��
*
*	\return X���� ǥ���� ��ȯ dX�� Y���� ǥ������ ��ȯ�Ͽ� ��ȯ�Ѵ�.
*/
EgTrans ad(const EgTrans &X, const EgTrans &dX, const EgTrans &Y)
{
	EgTrans Y2X = inv(Y) * X;
	EgVec3 p = part_trans(Y2X * dX) - part_trans(Y2X);

	EgVec3 axis;
	double angle;
	part_rot(dX).GetAngleAxis(axis, angle, true);

	EgQuat q;
	q.SetFromAngleAxis(angle, Y2X * axis, true);

	return EgTrans(p, q);
}

/********************/
/* ��Ÿ ��ƿ �Լ� ���� */
/********************/
/*!
*	\brief	���� ����Ʈ���� �ִ� ������� ���Ѵ�.
*
*	\param	IntegerList[in]		�Է� ���� ����Ʈ
*
*	\return	�ִ� ������� ��ȯ�Ѵ�.
*/
int get_gcd(std::vector<int> &IntegerList)
{
	if (IntegerList.empty())
		return 0;

	int min = IntegerList[0];
	int Num = (int)IntegerList.size();
	for (int i = 1; i < Num; ++i)
		min = MIN(min, IntegerList[i]);

	int gcd = 1;
	for (int i = 1; i < min; ++i)
	{
		bool common = true;
		for (int j = 0; j < Num; ++j)
		{
			if (IntegerList[j] % i)
				common = false;
		}

		if (common)
			gcd = i;
	}

	return gcd;
}


/*!
*	\brief	���� r�� ���� pq�� ���� �ִ����� �˻��Ѵ�.
*
*	\param	p[in]	������ ������
*	\param	q[in]	������ ����
*	\param	r[in]	�Է� ����
*
*	\return ���� r�� ���� pq�� ���� ������ ���(true), �׷��� ������(false)
*/
bool left_turn(const EgPos &p, const EgPos &q, const EgPos &r)
{
	double det;
	det = (q[0] - p[0]) * (r[1] - p[1]) - (q[1] - p[1]) * (r[0] - p[0]);

	if (det > 0)
		return true;
	else if (det < 0)
		return false;
	else // co-linear case.
	{
		double qr = dist(q, r);
		double qp = dist(q, p);
		if (qr >= qp)
			return true;
		else // pr < pq
			return false;
	}
}

/*!
*	\brief	Convex hull�� �����ϴ� ���� ������ ã�´�.
*
*	\param	InPts[in]	�Է� ����
*	\param	PrevPt[in]	���� ����
*	\param	CurrPt[in]	���� ����
*
*	\return Gift Wrap �˰��򿡼� convex hull�� �����ϴ� ���� ������ ��ȯ�Ѵ�.
*/
EgPos wrap(const std::vector<EgPos> &InPts, const EgPos &PrevPt, const EgPos &CurrPt)
{
	EgPos p, q, r;
	p = PrevPt;
	q = CurrPt;

	int NumPt = (int)InPts.size();
	for (int i = 0; i < NumPt; ++i)
	{
		r = InPts[i];
		if (r != PrevPt && r != CurrPt)
		{
			if (left_turn(p, q, r))
				p = r;
		}
	}
	return p;
}

/*!
*	\brief Gift Wrap�˰����� �̿��Ͽ� 2���� convex hull�� ���Ѵ�.
*
*	\param InPts[in]	�Է� ����
*	\param OutPts[out]	convex hull�� �������� �ݽð� �������� �����
*/
void find_convexhull_2d(const std::vector<EgPos> &InPts, std::vector<EgPos> &OutPts)
{
	if (InPts.empty())
		return;

	// �������� ���Ѵ�
	EgPos StartPt = InPts[0];
	int NumPt = (int)InPts.size();
	for (int i = 1; i < NumPt; ++i)
	{
		if (InPts[i][1] < StartPt[1])
			StartPt = InPts[i];
	}
	OutPts.push_back(StartPt);

	EgPos PrevPt, CurrPt, NextPt;
	PrevPt = CurrPt = StartPt;
	while (true)
	{
		NextPt = wrap(InPts, PrevPt, CurrPt);
		if (NextPt == StartPt)
			break;

		OutPts.push_back(NextPt);
		PrevPt = CurrPt;
		CurrPt = NextPt;
	}
}

/*!
*	\brief	�־��� ������ �ٰ��� ���ο� ���ԵǴ��� ���θ� �Ǵ�
*	\note	�׽�Ʈ �ʿ�
*
*	\param	Pts[in]		�ٰ����� �̷�� ������ ����Ʈ(�ݽð� ����)
*	\param	p[in]		���� ���θ� ������ ����
*
*	\return �ٰ��� ���ο� ���Ե� ��� true, �׷��� ������ false�� ��ȯ
*/
bool pt_in_polygon(const std::vector<EgPos> &Pts, EgPos const &p)
{
	int pnum = (int)Pts.size();
	
	bool ret = false;
	int j = pnum - 1;
	for (int i = 0; i < pnum; ++i) 
	{
		if (Pts[i][1] < p[1] && Pts[j][1] >= p[1] || Pts[j][1] < p[1] && Pts[i][1] >= p[1]) 
		{
			if (Pts[i][0] + (p[1] - Pts[i][1]) / (Pts[j][1] - Pts[i][1]) * (Pts[j][0] - Pts[i][0]) < p[0]) 
			{
				ret = !ret;
			}
		}
		j = i; 
	}

	return ret;
}

/*!
*	\brief	�־��� ������ �ﰢ�� ���ο� ���ԵǴ��� ���θ� �Ǵ�
*
*	\param	p[in]	��� ����
*	\param	p1[in]	�ﰢ���� ù ��° ����(�ݽð� ����)
*	\param	p2[in]	�ﰢ���� �� ��° ����(�ݽð� ����)
*	\param	p3[in]	�ﰢ���� �� ��° ����(�ݽð� ����)
*
*	\return �ﰢ�� ���ο� ���Ե� ��� true, �׷��� ������ false�� ��ȯ
*/
bool pt_in_triangle(EgVec2 p, EgVec2 p1, EgVec2 p2, EgVec2 p3)
{
	double a, b, c, d, e, f;
	EgVec2 v1 = p2 - p1;
	EgVec2 v2 = p3 - p1;
	EgVec2 v3 = p - p1;
	
	a = v1[0];
	c = v1[1];

	b = v2[0];
	d = v2[1];

	e = v3[0];
	f = v3[1];

	double alpha, beta;
	alpha = (d * e - b * f) / (a * d - b * c);
	beta = (a * f - c * e) / (a * d - b * c);

	if (alpha >= 0 && beta >= 0 && (alpha + beta) <= 1.0)
		return true;
	
	return false;
}

/*!
*	\brief	�ﰢ���� �� ���� p, q, r,�� �Լ���(�µ�, ����, �з� ��)f, g, h�� �־��� ��� �׷����Ʈ ������ ����Ѵ�.
*
*	\param	p[in]	�ﰢ���� ù ��° ����(�ݽð� ����)
*	\param	q[in]	�ﰢ���� �� ��° ����(�ݽð� ����)
*	\param	r[in]	�ﰢ���� �� ��° ����(�ݽð� ����)
*	\param	f[in]	���� p�� �־��� �Լ���
*	\param	g[in]	���� q�� �־��� �Լ���
*	\param	h[in]	���� r�� �־��� �Լ���
*
*	\return �Լ����� �ִ�� �����ϴ� ������ ��ȯ�Ѵ�.
*/
EgVec3 get_grad_on_tri(const EgPos &p, const EgPos &q, const EgPos &r, double f, double g, double h)
{
	// �� ��(����)�� �����Ѵ�.
	EgVec3 a = q - p;
	EgVec3 b = r - q;
	EgVec3 c = p - r;

	// �ﰢ���� ������ ����Ѵ�.
	double At = area_tri(p, q, r);

	// �� ���� ȸ����ų ��(���ʹϿ�)�� ���Ѵ�.
	EgVec3 N = (a ^ b).Normalize();
	EgQuat qt(N, 90.0);

	// �� ���� �ݽð� �������� 90�� ȸ����Ų��.
	EgVec3 at = qt * a;
	EgVec3 ct = qt * c;

	// �ﰢ���� �׷����Ʈ�� ����Ͽ� ��ȯ�Ѵ�.
	return ((g - f) * ct + (h - f) * at) / (2.0 * At);
}

/*!
*	\brief	���� p, q, r�� ���ǵǴ� �ﰢ���� �������� pt�� barycentric ��ǥ�� ����Ѵ�.
*	\note	������ ������: 2020-06-18
*
*	\param	pt[in]	������ ���� ��ǥ
*	\param	p[in]	�ﰢ���� ù ��° ����(�ݽð� ����)
*	\param	q[in]	�ﰢ���� �� ��° ����(�ݽð� ����)
*	\param	r[in]	�ﰢ���� �� ��° ����(�ݽð� ����)
*
*	\return pt�� barycentric ��ǥ (alpha, beta, gamma)�� ��ȯ�Ѵ� (pt = alpha * p + beta * q + gamma * r).
*/
EgVec3 get_tri_barycentric_coords(EgPos pt, EgPos p, EgPos q, EgPos r)
{
	// ���� ��ǥ�踦 ���Ѵ�.
	EgVec3 x_axis = (p - r).Normalize();
	EgVec3 z_axis = ((p - r) ^ (q - r)).Normalize();
	EgQuat qt;
	qt.SetFromFrameZX(z_axis, x_axis);
	qt = inv(qt);

	// ���� ��ǥ�迡�� ǥ���� ���͸� ���ϰ�,
	EgVec3 a = qt * (p - r);
	EgVec3 b = qt * (q - r);
	EgVec3 c = qt * (pt - r);

	// Barycentric ��ǥ�� ���Ͽ� ��ȯ�Ѵ�.
	double denominator = a[0] * b[1] - b[0] * a[1];
	double alpha = (b[1] * c[0] - b[0] * c[1]) / denominator;
	double beta = (a[1] * c[0] + a[0] * c[1]) / denominator;
	return EgVec3(alpha, beta, 1.0 - alpha - beta);
}

