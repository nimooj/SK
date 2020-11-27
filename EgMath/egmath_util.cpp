#include "..\usr\include\EgMath\egmath.h"

/***********************/
/* EgVec2 유틸 함수 구현 */
/***********************/
/*!
*	\brief	벡터 v를 벡터 w에 사영시킨 벡터를 구한다.
*
*	\param	v[in]	사영 벡터
*	\param	w[in]	기준 벡터
*
*	\return 벡터 v를 벡터 w에 사영시킨 벡터를 반환한다.
*/
EgVec2 proj(const EgVec2 &v, const EgVec2 &w)
{
	return (v * w / norm_sq(w)) * w;
}

/*!
*	\brief	벡터 v를 90도 회전한 수직 벡터를 구한다.
*	\note	마지막 수정일: 2020-07-05
*
*	\param	v[in]	회전할 벡터
*	\param	CCW[in]	회전 방향(반시계 방향: true, 시계 방향: false)
*
*	\return 벡터 v를 90도 회전한 수직 벡터를 반환한다.
*/
EgVec2 ortho(const EgVec2 &v, bool CCW)
{
	return (CCW) ? EgVec2(-v[1], v[0]) : EgVec2(v[1], -v[0]);
}

/*!
*	\brief	두 벡터의 차이 벡터의 길이를 구한다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번재 벡터
*
*	\return 차이 벡터 (v - w)의 길이를 반환한다.
*/
double dist(const EgVec2 &v, const EgVec2 &w)
{
	return norm(v - w);
}

/*!
*	\brief	벡터의 크기를 구한다.
*
*	\param	v[in]	대상 벡터
*
*	\return 벡터 v의 크기를 반환한다.
*/
double norm(const EgVec2 &v)
{
	return SQRT(SQR(v.m_Coords[0]) + SQR(v.m_Coords[1]));
}

/*!
*	\brief	벡터의 크기 제곱을 구한다.
*
*	\param	v[in]	대상 벡터
*
*	\return 벡터 v의 크기 제곱을 반환한다.
*/
double norm_sq(const EgVec2 &v)
{
	return SQR(v.m_Coords[0]) + SQR(v.m_Coords[1]);
}

/*!
*	\brief	두 벡터의 사이각(0 ~ 180)을 구한다.
*
*	\param	v[in]		시작 벡터
*	\param	w[in]		끝 벡터
*	\param	radian[in]	각도의 형태(true: radian, false: degree)
*
*	\return 벡터 \a v에서 벡터 \a w까지의 사이각을 반환한다.
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
/* EgVec3 유틸 함수 구현 */
/***********************/
/*!
*	\brief	벡터 v를 벡터 w에 사영시킨 벡터를 구한다.
*
*	\param	v[in]	사영 벡터
*	\param	w[in]	참조 벡터
*
*	\return 벡터 v를 벡터 w에 사영시킨 벡터를 반환한다.
*/
EgVec3 proj(const EgVec3 &v, const EgVec3 &w)
{
	return (v * w / norm_sq(w)) * w;
}

/*!
*	\brief	벡터 v에 수직한 단위 벡터를 구한다.
*
*	\param	v[in]	대상 벡터
*
*	\return 벡터 v에 수직한 단위 벡터를 구하여 반환한다.
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
*	\brief	벡터 v와 w의 차이벡터의 길이를 구한다.
*
*	\param	v[in]	벡터
*	\param	w[in]	벡터
*
*	\return	벡터 \a v, \a w의 차이벡터의 길이를 반환한다.
*/
double dist(const EgVec3 &v, const EgVec3 &w)
{
	return norm(v - w);
}

/*!
*	\brief	벡터 u, v, w의 행렬식을 계산한다.
*
*	\param	u[in]	벡터
*	\param	v[in]	벡터
*	\param	w[in]	벡터
*
*	\return 벡터 u, v, w의 행렬식의 값을 반환한다.
*/
double det(const EgVec3 &u, const EgVec3 &v, const EgVec3 &w)
{
	return (u[0] * v[1] * w[2] + u[1] * v[2] * w[0] + u[2] * v[0] * w[1]) - (w[0] * v[1] * u[2] + v[0] * u[1] * w[2] + u[0] * w[1] * v[2]);
}

/*!
*	\brief	벡터 v의 크기를 구한다.
*
*	\param	v[in]	대상 벡터
*
*	\return 벡터 \a v의 크기를 반환한다.
*/
double norm(const EgVec3 &v)
{
	return SQRT(norm_sq(v));
}

/*!
*	\brief	벡터 v의 크기의 제곱을 계산한다.
*
*	\param	v[in]	대상 벡터
*
*	\return 벡터 \a v의 크기의 제곱을 반환한다.
*/
double norm_sq(const EgVec3 &v)
{
	return SQR(v.m_Coords[0]) + SQR(v.m_Coords[1]) + SQR(v.m_Coords[2]);
}

/*!
*	\brief	두 벡터 사이의 사이각(0 ~ 180)을 구한다.
*
*	\param	v[in]		시작 벡터
*	\param	w[in]		끝 벡터
*	\param	radian[in]	각도의 형태(true: radian, false: degree)
*
*	\return 벡터 \a v에서 벡터 \a w까지의 사이각(예각, 방향 상관없음)
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
*	\brief	두 벡터 사이의 사이각(0, ~ 360)을 구한다.
*
*	\param	v[in]		시작 벡터
*	\param	w[in]		끝 벡터
*	\param	up[in]		기준 벡터
*	\param	radian[in]	각도의 형태(true: radian, false: degree)
*
*	\return 벡터 \a v에서 벡터 \a w까지의 사이각(기준 벡터를 기준 반시계 방향)
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
/* EgVec24유틸 함수 구현 */
/***********************/
/*!
*	\brief	벡터 v의 크기를 구한다.
*
*	\param	v[in]	벡터
*
*	\return 벡터 \a v의 크기를 반환한다.
*/
double norm(const EgVec4 &v)
{
	return SQRT(norm_sq(v));
}

/*!
*	\brief	벡터 v의 크기 제곱을 구한다.
*
*	\param	v[in]	벡터
*
*	\return 벡터 \a v의 크기 제곱을 반환한다.
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
*	\brief	두 벡터의 차이벡터의 크기를 구한다.
*
*	\param	v[in]	벡터
*	\param	w[in]	벡터
*
*	\return 두 벡터의 차이벡터의 크기를 반환한다.
*/
double dist(const EgVec4 &v, const EgVec4 &w)
{
	return norm(v - w);
}

/*!
*	\brief	가중치를 적용하여 동차 좌표로 변환한다: v=(x, y, z, w) -> v'=(wx, wy, wz, w)
*
*	\param	v[in]	대상 벡터
*
*	\return 동차 좌표 v'=(wx, wy, wz, w)를 반환한다.
*/
EgVec4 homogenize(const EgVec4 &v)
{
	return EgVec4(v[0] * v[3], v[1] * v[3], v[2] * v[3], v[3]);
}

/*!
*	\brief	동차 좌표 v=(wx, wy, wz, w) -> v'=(x, y, z, w)로 변환한다.
*
*	\param	v[in]	4차원 공간의 벡터
*
*	\return 변환된 벡터 v'=(x, y, z, w)를 반환한다.
*/
EgVec4 rationalize(const EgVec4 &v)
{
	return EgVec4(v[0] / v[3], v[1] / v[3], v[2] / v[3], v[3]);
}

/*!
*	\brief	v=(wx, wy, wz, w) -> v'=(x, y, z, 1)
*
*	\param	v[in]	4차원 공간의 벡터
*
*	\return v'=(x, y, z, 1)를 반환한다.
*/
EgVec4 cartesianize(const EgVec4 &v)
{
	return EgVec4(v[0] / v[3], v[1] / v[3], v[2] / v[3], 1.0);
}

/***********************/
/* EgVec 유틸 함수 구현 */
/***********************/
/*!
*	\brief	벡터의 크기를 구한다.
*
*	\param	v[in]	대상 벡터
*
*	\return 계산된 크기를 반환한다.
*/
double norm(const EgVec &v)
{
	return SQRT(norm_sq(v));
}

/*!
*	\brief	벡터의 크기의 제곱을 구한다.
*
*	\param	v[in]	대상 벡터
*
*	\return 계산된 크기의 제곱을 반환한다.
*/
double norm_sq(const EgVec &v)
{
	double len = 0;
	for (int i = 0; i < (int)v.m_Coords.size(); ++i)
		len += SQR(v.m_Coords[i]);
	return len;
}

/*!
*	\brief	두 벡터의 차이벡터의 크기를 구한다.
*
*	\param	v[in]	첫 번째 벡터
*	\param	w[in]	두 번째 벡터
*
*	\return 두 벡터의 차이벡터의 크기를 반환한다.
*/
double dist(const EgVec &v, const EgVec &w)
{
	return norm(v - w);
}

/**********************/
/* EgPos 유틸 함수 구현 */
/**********************/
/*!
*	\brief	포인트의 \a p의 길이를 구한다.
*
*	\param	p[in]	포인트
*
*	\return 포인트 \a p의 길이를 반환한다.
*/
double norm(const EgPos &p)
{
	return SQRT(norm_sq(p));
}

/*!
*	\brief	포인트의 \a p의 길이 제곱을 구한다.
*
*	\param	p[in]	포인트
*
*	\return 포인트 \a p의 길이 제곱을 반환한다.
*/
double norm_sq(const EgPos & p)
{
	return SQR(p.m_Coords[0]) + SQR(p.m_Coords[1]) + SQR(p.m_Coords[2]);
}

/*!
*	\brief	점 \a p에서 점 \a q까지의 거리를 구한다.
*
*	\param	p[in]	첫 번째 점
*	\param	q[in]	두 번째 점
*
*	\return 점 \a p에서 점 \a q까지의 거리를 반환한다.
*/
double dist(const EgPos &p, const EgPos &q)
{
	return SQRT(dist_sq(p, q));
}

/*!
*	\brief	점 \a p에서 점 \a q까지의 거리 제곱을 구한다.
*
*	\param	p[in]	첫 번째 점
*	\param	q[in]	두 번째 점
*
*	\return 점 \a p에서 점 \a q까지의 거리 제곱을 반환한다.
*/
double dist_sq(const EgPos &p, const EgPos &q)
{
	return (SQR(p.m_Coords[0] - q.m_Coords[0]) + SQR(p.m_Coords[1] - q.m_Coords[1]) + SQR(p.m_Coords[2] - q.m_Coords[2]));
}

/*!
*	\brief	점 p와 q를 t : (1 - t)로 내분한다.
*
*	\param	p[in]	3차원 공간의 점
*	\param	q[in]	3차원 공간의 점
*
*	\return 점 p와 q의 내분점을 반환한다.
*/
EgPos lerp(const EgPos &p, const EgPos &q, double t)
{
	double x = (1.0 - t) * p[0] + t * q[0];
	double y = (1.0 - t) * p[1] + t * q[1];
	double z = (1.0 - t) * p[2] + t * q[2];
	return EgPos(x, y, z);
}

/*!
*	\brief	점들의 affine combination을 구한다
*
*	\param	N[in]		점의 개수
*	\param	alpha0[in]	상수
*	\param	p0[in]		첫 번째 점
*	\param	alpha1[in]	상수
*	\param	p1[in]		두 번째 점
*
*	\return 점들의 affine combination의 결과 (alpha0 * p0 + alpha1 * p1 + ... + alpha_n-1 * p_n-1)
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
*	\brief	점들의 affine combination을 구한다
*
*	\param	Points[in]		점의 배열에 대한 포인터
*	\param	Weights[in]		combination의 상수값
*	\param	Size[in]		점의 개수 (Points, Weights 배열의 크기)
*
*	\return 점들의 affine combination의 결과를 반환한다(Point[0] * Weight[0] + Points[1] * Weight[1] + ... + Points[Size-1] * Weight[Size-1])
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
*	\brief	삼각형이 둔각삼각형인지 검사한다.
*	\note	구현: 박정호(2019-02-03), 확인: 윤승현(2019-03-03)
*
*	\param	p[in]	삼각형의 첫 번째 정점(반시계 방향)
*	\param	q[in]	삼각형의 두 번째 정점(반시계 방향)
*	\param	r[in]	삼각형의 세 번째 정점(반시계 방향)
*
*	\return 각(rpq)각 둔각인 경우 1, 각(pqr)이 둔각인 경우 2, 각(qrp)가 둔각인경우 3, 둔각이 아닌경우 0를 반환한다.
*/
int is_obtuse_tri(const EgPos &p, const EgPos &q, const EgPos &r)
{
	// 삼각형의 선분 벡터와 법벡터를 구한다.
	EgVec3 a = q - p;
	EgVec3 b = r - q;
	EgVec3 c = p - r;
	EgVec3 N = (a ^ b).Normalize();

	// 첫 번째 각이 둔각이면 1를 반환한다.
	double alpha = angle(a, -c, N);
	if (alpha > 90.0)
		return 1;

	// 두 번째 각이 둔각이면 2를 반환한다.
	double beta = angle(b, -a, N);
	if (beta > 90.0)
		return 2;

	// 세 번째 각이 둔각이면 3을 반환한다.
	if (alpha + beta < 90.0)
		return 3;

	// 모두 예각이면 0을 반환한다.
	return 0;
}

/*!
*	\brief	삼각형의 외심(circumcenter)를 구한다.
*	\note	구현: 박정호(2019-02-03), 확인: 윤승현(2019-03-03)
*
*	\param	p[in]	삼각형의 첫 번째 정점(반시계 방향)
*	\param	q[in]	삼각형의 두 번째 정점(반시계 방향)
*	\param	r[in]	삼각형의 세 번째 정점(반시계 방향)
*
*	\return 삼각형의 외심의 위치를 반환한다.
*/
EgPos circumcenter(const EgPos &p, const EgPos &q, const EgPos &r)
{
	// p를 원점으로 하는 지역 좌표계를 정의하여 
	EgVec3 x_axis = (q - p).Normalize();
	EgVec3 z_axis = ((q - p) ^ (r - p)).Normalize();
	EgVec3 y_axis = (z_axis ^ x_axis).Normalize();
	EgTrans X(cast_vec3(p), EgQuat(x_axis, y_axis));

	// 지역 좌표계에서 표현된 정점 q와 r의 좌표 q0, r0를 구한다.
	EgPos q0 = inv(X) * q;
	EgPos r0 = inv(X) * r;

	// 지역 좌표계에서 삼각형의 외심을 구하고,
	EgPos O(q0[0] * 0.5, (SQR(r0[0]) + SQR(r0[1]) - q0[0] * r0[0]) / (2.0 * r0[1]), 0.0);

	// 전역 좌표계로 변환하여 반환한다.
	return X * O;
}

/*!
*	\brief	삼각형의 넓이를 구한다
*
*	\param	p[in]	삼각형의 첫 번째 정점(반시계 방향)
*	\param	q[in]	삼각형의 두 번째 정점(반시계 방향)
*	\param	r[in]	삼각형의 세 번째 정점(반시계 방향)
*
*	\return 삼각형의 넓이를 구하여 반환한다.
*/
double area_tri(const EgPos &p, const EgPos &q, const EgPos &r)
{
	double area = norm((q - p) ^ (r - p)) * 0.5;
	return area;
}

/*!
*	\brief	점 p의 좌표를 주어진 자리수까지(반올림하여) 나타냄
*
*	\param	p[in] 3차원 공간의 점
*
*	\return 반올림된 점의 표현을 반환한다.
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
/* EgMat 유틸 함수 구현 */
/**********************/
/*!
*	\brief	전치행렬을 반환한다.
*
*	\param	m[in]	행렬
*
*	\return 행렬 \a m의 전치행렬을 반환한다.
*/
EgMat tr(const EgMat &m)
{
	EgMat ret(m);
	ret.SetTranspose();
	return ret;
}

/*!
*	\brief	행렬의 row-echelon 폼을 반환한다.
*
*	\param	m[in]	행렬
*
*	\return row-echelon 폼을 반환한다.
*/
EgMat ref(const EgMat &m)
{
	int i, j, k; // 반복제어 변수
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
*	\brief	행렬의 reduced row-echelon 폼을 반환한다.
*
*	\param	m[in]	행렬
*
*	\return reduced row-echelon 폼을 반환한다.
*/
EgMat rref(const EgMat &m)
{
	int i, j, k; // 반복제어 변수
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
*	\brief	행렬 \a m의 Null 공간의 기저를 열 벡터로 갖는 행렬을 반환한다.
*
*	\param	m[in]	행렬
*
*	\return 행렬 \a m의 Null 공간의 기저를 열 벡터로 갖는 행렬을 반환한다.
*/
EgMat basis_null(const EgMat &m)
{
	// 행렬 m의 reduced row-echelon form을 구한다.
	EgMat R = rref(m);

	// 행렬 R의 행의 수와 열의 수를 구한다.
	int r = R.GetRowNum();
	int c = R.GetColNum();

	// 행렬 R의 free 변수와 pivot 변수의 인데스를 구한다.
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

	// Free 열 벡터를 저장할 행렬 B를 정의한다.
	int NumFree = (int)FreeIdx.size();
	EgMat B(r, NumFree);
	for (int i = 0; i < NumFree; ++i)
	{
		EgVec x = R.GetColVec(FreeIdx[i]);
		B.SetColVec(i, -x);
	}

	// Null 공간의 기저를 저장할 행렬 N을 정의한다.
	EgMat N(c, NumFree);
	EgMat I(NumFree, NumFree);
	I.SetIdentity();
	for (int i = 0; i < NumFree; ++i)
	{
		EgVec x = I.GetRowVec(i);
		N.SetRowVec(FreeIdx[i], x);
	}

	// Free 변수의 열 벡터의 음수 값으로 행렬 N의 Pivot 행을 채운다.
	int NumPivot = (int)PivotIdx.size();
	for (int i = 0; i < NumPivot; ++i)
	{
		EgVec x = B.GetRowVec(i);
		N.SetRowVec(PivotIdx[i], x);
	}

	// 구해진 기저를 반환한다.
	return N;
}

/*!
*	\brief	행렬의 rank (독립한 열벡터 혹은 행벡터의 수)를 계산
*
*	\param	m[in]	행렬
*
*	\return 구해진 행렬의 rank를 반환한다.
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
*	\brief	행렬의 nullity 를 계산
*
*	\param	m[in]	행렬
*
*	\return 구해진 nullity를 반환한다.
*/
int nullity(const EgMat &m)
{
	int rnk = rank(m);
	int c = m.GetColNum();
	return (c - rnk);
}

/*!
*	\brief	행렬 A를 L(하삼각행렬)과 U(상삼각행렬)의 곱으로 분해
*
*	\param	A[in]	대상 정방행렬
*	\param	L[out]	하삼각행렬이 저장됨
*	\param	U[out]	상삼각행렬이 저장됨
*
*	\return 성공: true, 실패: false
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
*	\brief	행렬 A를 Householder 행렬을 사용하여 Q * R로 분해
*
*	\param	A[in]	m x n 대상행렬
*	\param	Q[out]	m x m 직교행렬
*	\param	R[out]	m x n 상삼각행렬
*
*	\return 실패하면 -1, 성공하면 householder 행렬을 적용한 회수를 반환한다.
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
*	\brief	LU 분해를 통해, 행렬 A의 행렬식을 구한다.
*
*	\param	A[in]	n x n 대상 행렬
*
*	\return 행렬식의 값을 반환한다.
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
*	\brief	LU 분해를 통해, 행렬 A의 역행렬을 구한다.
*
*	\param	A[in]	n x n 대상행렬.
*
*	\return 행렬 A의 역행렬을 반환한다.
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
*	\brief	행렬 A의 부분행렬을 반환한다.
*
*	\param	A[in]	행렬
*	\param	r0[in]	시작 행의 인덱스
*	\param	c0[in]	시작 열의 인덱스
*	\param	r1[in]	끝 행의 인덱스
*	\param	c1[in]	끝 열의 인덱스
*
*	\return 행렬 A의 부분행렬을 반환한다.
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
*	\brief	행렬 A를 L(하삼각행렬)과 U(상삼각행렬)로 변환(부분피봇팅(행교환)을 수행)
*			분해 후, lin_sys_lu(A, x, b, Idx)를 호출하여 선형 시스템 Ax = b 를 만족하는 벡터 x를 구함
*
*	\param	A[in\out]	n by n 행렬으로 LU 형태로 변환된다.
*	\param	Idx[out]	행 교환 정보가 저장된다.
*	\param	d[out]		행 교환수가 짝수이면 1, 홀수이면 -1 이 저장된다.
*
*	\return 성공하면 true, 실패하면 false를 반환한다.
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
*	\brief	SVD를 이용하여 행렬 A를 A = U * W * tr(V)로 분해
*
*	\param	A[in]	m x n 행렬 (m >= n)
*	\param	U[out]	m x n 행렬로서 열벡터는 A의 열공간의 단위기저가 저장된다.
*	\param	S[out]	특이값이 저장될 n 차원벡터. W는 S의 원소로 생성되는 대각선 행렬
*	\param	V[out]	n x n 행렬로서 열벡터는 A의 행공간 및 널공간의 단위기저가 저장된다.
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
*	\brief	선형 방정식 Ax = b의 해를 구한다.
*
*	\param	A[in]		LU로 분해된 행렬(dcmp_lu(A, Idx, d)의 결과)
*	\param	x[out]		해가 저장될 벡터
*	\param	b[in]		상수 벡터
*	\param	Idx[in]		행의 교환정보를 저장한 벡터(dcmp_lu(A, Idx, d)의 결과)
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
*	\brief	선형방정식 Ax = b 를 만족하는 minimum length least square 해를 구함
*
*	\param	A[in]	m x n 행렬 (m >= n)
*	\param	x[out]	해가 저장된다.
*	\param	b[in]	상수벡터
*
*	\return 조건수(condition number)를 반환한다.
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
*	\brief	선형방정식 Ax = b 를 만족하는 minimum length least square 해 x를 구한다.\n
*			- 이 함수를 호출하기 전에, dcmp_sv(A, U, S, V)를 호출하여 행렬 A의 분해된 결과를 사용한다.
*			- 한번 분해된 행렬 A는 상수벡터 b만 바꾸면서 해를 구하게 된다.
*
*	\param	U[in]	m x n 행렬로 dcmp_sv(A, U, S, V)의 결과
*	\param	S[in]	n 차원 벡터로 dcmp_sv(A, U, S, V)의 결과
*	\param	V[in]	n x n 행렬로 dcmp_sv(A, U, S, V)의 결과
*	\param	x[out]	해가 저장될 n 차원 벡터
*	\param	b[in]	m 차원 상수벡터
*
*	\return 조건수(condition number)를 반환한다.
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
*	\brief	Conjugate gradient 방법을 사용하여 선형방정식 Ax = b 를 푼다.\n
*			- 행렬 A는 대칭행렬이어야 한다.
*			- sparse 선형시스템을 풀기위한 효과적인 방법이다.
*
*	\param	A[in]		n x n 대칭행렬.
*	\param	x[in/out]	n 차원 초기 해 벡터.
*	\param	b[in]		n 차원 상수 벡터.
*	\param	iter[in]	반복회수, 만약 반복회수가 n이면 정확한 해를 구한다.
*
*	\return 실제 반복회수를 반환한다.
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
*	\brief	QR 분해법을 이용하여 선형방정식 Ax = b의 least square 해를 찾는다.
*
*	\param	A[in]	m x n 행렬.
*	\param	x[out]	n 차원 해 벡터.
*	\param	b[in]	n 차원 상수 벡터.
*
*	\return 1: 성공, 0: 실패.
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
*	\brief	상사변환(similarity transformation)을 사용하여 실수대칭행렬의 모든 eigenvector와 eigenvalue를 구한다.
*
*	\param	A[in]	n x n 대칭행렬.
*	\param	D[out]	n개의 eigenvalue를 저장된다.
*	\param	V[out]	n개의 eigenvector (열벡터)를 저장된다.
*
*	\return 상사변환의 회수를 반환한다.
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
*	\brief	dcmp_sv 를 사용하여 주성분 (principle component)를 찾는다.
*	\note	함수호출 후, Data 값들은 평균이 원점에 위치하도록 이동된다.
*
*	\param	Data[in]		m x n 데이터 행렬 (m: 데이터의 개수, n: 데이터의 차원)
*	\param	PC[out]			열벡터로 표현된 주성분.
*	\param	SigVal[out]		주성분벡터의 특이값을 저장한 n 차원 벡터.
*	\param	Mean[out]		평균벡터
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
/* EgQuat 유틸 함수 구현 */
/***********************/
/*!
*	\brief	쿼터니온의 크기를 구한다.
*
*	\param	q[in] 대상 쿼터니온
*
*	\return 쿼터니온의 크기를 반환한다.
*/
double norm(const EgQuat &q)
{
	return SQRT(SQR(q.m_W) + SQR(q.m_X) + SQR(q.m_Y) + SQR(q.m_Z));
}

/*!
*	\brief	쿼터니온의 역원을 구하는 함수.
*
*	\param	q[in] 대상 쿼터니온
*
*	\return 쿼터니온의 역원을 반환한다.
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
*	\brief	벡터(pure imaginary 쿼터니온)의 exponential mapping을 구한다.
*
*	\param	v[in]	쿼터니온 q = (0, v)
*
*	\return 단위회전축 v/||v|| 를 중심으로 2 * ||v|| 의 회전각을 갖는 쿼터니온을 반환한다.
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
*	\brief	쿼터니온 q의 로그값을 구한다.
*
*	\param	q[in]	대상 쿼터니온
*
*	\return 순허수 쿼터니온의 허수부분 Log(q) = (0, x, y, z)을 반환한다.
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
*	\brief  두 쿼터니온을 내적한다.
*
*	\param	q1[in]	쿼터니온
*	\param	q2[in]	쿼터니온
*
*	\return 내적값을 반환한다.
*/
double quat_dot(const EgQuat &q1, const EgQuat &q2)
{
	return q1.m_W * q2.m_W + q1.m_X * q2.m_X + q1.m_Y * q2.m_Y + q1.m_Z * q2.m_Z;
}

/*!
*	\brief	쿼터니온 q1, q2를 Spherical linear interpolation 한다.
*
*	\param	q1[in]	쿼터니온
*	\param	q2[in]	쿼터니온
*	\param	t[in]	보간될 시간
*
*	\return 보간된 쿼터니온을 반환한다.
*/
EgQuat slerp(EgQuat q1, EgQuat q2, double t)
{
	if (quat_dot(q1, q2) < 0.0)
		q2.SetNegate();

	EgQuat q;
	double cs = q1.m_W * q2.m_W + q1.m_X * q2.m_X + q1.m_Y * q2.m_Y + q1.m_Z * q2.m_Z;

	// 두 쿼터니온 사이의 각도가 180도에 매우 가까운 경우
	if (EQ(cs, -1.0, 1.0e-6))
	{
		q = q1 * sin((0.5 - t) * M_PI) + q2 * sin(t * M_PI);
	}
	else if (EQ(cs, 1.0, 1.0e-6)) // 두 쿼터니온 사이의 각도가 0도에 매우 가까운 경우
	{
		q = q1 * (1.0 - t) + q2 * t;
	}
	else // 일반적인 경우
	{
		double theta = acos(cs);
		q = (q1 * sin((1.0 - t) * theta) + q2 * sin(t * theta)) / sin(theta);
	}
	//if (1.0 + cs > 1.0e-6)
	//{
	//	if (1.0 - cs > 1.0e-6)
	//	{
	//		// 일반적인 경우
	//		double theta = acos(cs);
	//		q = (q1 * sin((1.0 - t) * theta) + q2 * sin(t * theta)) / sin(theta);
	//	}
	//	else // 두 쿼터니온 사이의 각도가 0도에 매우 가까운 경우
	//		q = q1 * (1.0 - t) + q2 * t;
	//}
	//else // 두 쿼터니온 사이의 각도가 180도에 매우 가까운 경우
	//	q = q1 * sin((0.5 - t) * M_PI) + q2 * sin(t * M_PI);

	q.Normalize();
	return q;
}

/***********************/
/* EgLine 유틸 함수 구현 */
/***********************/
/*!
*	\brief	한 점에서 직선까지의 거리를 구한다.
*
*	\param	l[in]	직선
*	\param	q[in]	한 점
*
*	\return 점 \a q에서 직선 \a l까지의 수직거리
*/
double dist(const EgLine &l, const EgPos &q)
{
	return norm(proj(q - l.m_Pos, l.m_Dir) - (q - l.m_Pos));
}

/*!
*	\brief	두 직선 l1(t)와 l2(t)사이의 최단 거리를 구한다.
*
*	\param	l1[in]	첫 번째 직선
*	\param	l2[in]	두 번째 직선
*	\param	t1[in]	최단 거리가 생기는 l1의 파리미터 값이 저장됨
*	\param	t2[in]	최단 거리가 생기는 l2의 파리미터 값이 저장됨
*
*	\return 두 직선 사이의 최단거리를 계산하여 반환한다 (두 직선이 만나는 경우 -1.0을 반환한다).
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
*	\brief	직선과 직선의 교차점을 계산한다.
*
*	\param	p[in]	교차점이 저장될 변수
*	\param	l0[in]	첫 번째 직선의 방정식 (ruling line)
*	\param	l1[in]	두 번째 직선의 방정식 (boundary line)
*	\param	t0[in]	첫 번째 직선위 교차점의 파라미터 값이 저장됨
*	\param	t1[in]	두 번째 직선위 교차점의 파라미터 값이 저장됨
*
*	\return true: 성공, false: 직선이 꼬인 위치에 있거나, (0 ~ 1)사이를 벗어날 경우
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
*	\brief	직선과 직선의 교차점을 계산한다.
*
*	\param	p[in]	교차점이 저장될 변수
*	\param	l0[in]	첫 번째 직선의 방정식
*	\param	l1[in]	두 번째 직선의 방정식
*
*	\return true: 성공, false: 직선이 꼬인 위치에 있는 경우
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
*	\brief	직선과 삼각형 (p1p2p3)의 교차점을 계산한다.
*
*	\param	p1[in]				삼각형의 첫 번째 정점(반시계 방향)
*	\param	p2[in]				삼각형의 두 번째 정점(반시계 방향)
*	\param	p3[in]				삼각형의 세 번째 정점(반시계 방향)
*	\param	l[in]				직선의 방정식
*	\param	t[out]				교차점의 직선 파라미터가 저장됨
*	\param	q[out]				교차점이 저장됨
*	\param	alpha[out]			barycentric 좌표가 저장됨
*	\param	beta[out]			barycentric 좌표가 저장됨
*	\param	gamma[out]			barycentric 좌표가 저장됨
*	\param	bCullBackFace[in]	직선이 삼각형의 뒷면과 만나는 경우를 제외하려면. (디폴트: true).
*	\note	q = alpha * p1 + beta * p2 + gamma * p3, (alpha + beta + gamma = 1)
*
*	\return true: 성공, false: 직선이 삼각형이 평행하거나 교차점이 삼각형의 외부에 존재하는 경우.
*/
bool intersect_line_triangle(EgPos &p1, EgPos &p2, EgPos &p3, EgLine &l, double &t, EgPos &q, double &alpha, double &beta, double &gamma, bool bCullBackFace)
{
	EgVec3 e1, e2, u, v, w;
	double det;
	e1 = p1 - p3;
	e2 = p2 - p3;
	u = l.m_Dir ^ e2;
	det = e1 * u;

	if (bCullBackFace)	// 직선이 삼각형의 뒷면과 만나는 경우는 제외.
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
*	\brief	직선과 삼각형 (p1p2p3)의 교차점을 계산한다.
*
*	\param	p1[in]				삼각형의 첫 번째 정점(반시계 방향)
*	\param	p2[in]				삼각형의 두 번째 정점(반시계 방향)
*	\param	p3[in]				삼각형의 세 번째 정점(반시계 방향)
*	\param	l[in]				직선의 방정식
*	\param	ispt[out]			교차점이 저장됨
*	\param	bCullBackFace[in]	직선이 삼각형의 뒷면과 만나는 경우를 제외하려면. (디폴트: true).
*
*	\return true: 성공, false: 직선이 삼각형이 평행하거나 교차점이 삼각형의 외부에 존재하는 경우.
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
	if (bCullBackFace)	// 직선이 삼각형의 뒷면과 만나는 경우는 제외.
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
	if (bCullBackFace)	// 직선이 삼각형의 뒷면과 만나는 경우는 제외.
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
/* EgPlane 유틸 함수 구현 */
/************************/
/*!
*	\brief	평면 pi에서 점 p까지의 부호있는 거리를 계산한다.
*
*	\param	pi[in]	평면의 객체
*	\param	p[in]	평면 밖의 한점.
*
*	\return 평면 \a pi에서 점 \a p까지의 부호있는 거리
*/
double dist(EgPlane &pi, EgPos &p)
{
	return pi.Eval(p) / norm(pi.N());
}

/*!
*	\brief	점 p를 평면 pi에 사영시킨 점을 구한다.
*
*	\param	p[in]	평면 밖의 점
*	\param	pi[in]	평면
*
*	\return	점 p를 평면 pi에 사영시킨 점을 반환한다.
*/
EgPos proj(EgPos &p, EgPlane &pi)
{
	double s = -dist(pi, p) / norm(pi.N());
	return p + s * pi.N();
}

/*!
*	\brief	두 평면의 교차 직선을 계산한다.
*
*	\param	l[out]		교차직선이 저장될 직선객체
*	\param	pi1[in]		첫 번째 평면
*	\param	pi2[in]		두 번째 평면
*
*	\return true: 성공, false: 두 평면이 평행한 경우.
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
*	\brief	직선과 평면의 교차점을 계산한다.
*
*	\param	p[out]	교차점이 저장될 변수
*	\param	l[in]	직선 객체
*	\param	pi[in]	평면 객체
*
*	\return true: 성공, false: 직선이 평면과 평행하거나, 평면이 직선을 포함하는 경우.
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
*	\brief	세 평면의 교차점을 계산한다.
*
*	\param	p[out] 교차점이 저장될 점 객체.
*	\param	pi1[in]		첫 번째 평면
*	\param	pi2[in]		두 번째 평면
*	\param	pi3[in]		세 번째 평면
*
*	\return true: 성공, false: 세 평면 중 적어도 두 평면이 일치하거나 평행하는 경우.
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
/* EgTrans 유틸 함수 구현 */
/************************/
/*!
*	\brief	강체변환의 역변환을 구한다.
*
*	\param	X[in]	대상 객체
*
*	\return X의 역변환을 반환한다.
*/
EgTrans inv(const EgTrans &X)
{
	EgQuat q = inv(X.m_Q);
	EgVec3 t = -(q * X.m_T);
	return EgTrans(t, q);
}

/*!
*	\brief	강체변환의 이동성분을 구한다.
*
*	\param	X[in]	강체변환 객체
*
*	\return X의 이동정보를 반환한다.
*/
EgVec3 part_trans(const EgTrans &X)
{
	return X.m_T;
}

/*!
*	\brief	강체변환의 회전성분을 구한다.
*
*	\param	X[in]	강체변환 객체
*
*	\return X의 회전정보를 반환한다.
*/
EgQuat part_rot(const EgTrans &X)
{
	return X.m_Q;
}

/*!
*	\brief	변환 X의 크기를 구한다.
*
*	\param	X[in]	강체변환
*
*	\return 변환 X의 크기를 반환한다.
*/
double norm(const EgTrans &X)
{
	return norm(X.m_T) + norm(log(X.m_Q));
}

/*!
*	\brief	두 변환 사이의 거리를 구한다
*
*	\param	X1[in]		첫 번째 변환
*	\param	X2[in]		두 번째 변환
*
*	\return X1에서 X2변환까지의 거리를 반환한다.
*/
double dist(const EgTrans &X1, const EgTrans &X2)
{
	return norm(inv(X1) * X2);
}

/*!
*	\brief	두 변환을 보간한다.
*
*	\param	X1[in]		첫 번째 변환
*	\param	X2[in]		두 번째 변환
*	\param	t[in]		보간 파라미터(0 ~ 1)
*
*	\return 보간된 변환이 반환된다.
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
*	\brief	X 좌표계에서 표현된 변환 dX를 Y 좌표계에서 표현된 변환으로 계산한다.
*
*	\param	X[in]		기준좌표계
*	\param	dX[in]		기준좌표계 X에서 표현된 변환
*	\param	Y[in]		대상좌표계
*
*	\return X에서 표현된 변환 dX를 Y에서 표현으로 변환하여 반환한다.
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
/* 기타 유틸 함수 구현 */
/********************/
/*!
*	\brief	정수 리스트에서 최대 공약수를 구한다.
*
*	\param	IntegerList[in]		입력 정수 리스트
*
*	\return	최대 공약수를 반환한다.
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
*	\brief	정점 r이 선분 pq의 왼편에 있는지를 검사한다.
*
*	\param	p[in]	선분의 시작점
*	\param	q[in]	선분의 끝점
*	\param	r[in]	입력 정점
*
*	\return 정점 r이 선분 pq의 왼편에 존재할 경우(true), 그렇지 않으면(false)
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
*	\brief	Convex hull을 구성하는 다음 정점을 찾는다.
*
*	\param	InPts[in]	입력 정점
*	\param	PrevPt[in]	이전 정점
*	\param	CurrPt[in]	현재 정점
*
*	\return Gift Wrap 알고리즘에서 convex hull을 구성하는 다음 정점을 반환한다.
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
*	\brief Gift Wrap알고리즘을 이용하여 2차원 convex hull을 구한다.
*
*	\param InPts[in]	입력 정점
*	\param OutPts[out]	convex hull의 정점들이 반시계 방향으로 저장됨
*/
void find_convexhull_2d(const std::vector<EgPos> &InPts, std::vector<EgPos> &OutPts)
{
	if (InPts.empty())
		return;

	// 시작점을 구한다
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
*	\brief	주어진 정점이 다각형 내부에 포함되는지 여부를 판단
*	\note	테스트 필요
*
*	\param	Pts[in]		다각형을 이루는 정점의 리스트(반시계 방향)
*	\param	p[in]		포함 여부를 조사할 정점
*
*	\return 다각형 내부에 포함될 경우 true, 그렇지 않으면 false를 반환
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
*	\brief	주어진 정점이 삼각형 내부에 포함되는지 여부를 판단
*
*	\param	p[in]	대상 정점
*	\param	p1[in]	삼각형의 첫 번째 정점(반시계 방향)
*	\param	p2[in]	삼각형의 두 번째 정점(반시계 방향)
*	\param	p3[in]	삼각형의 세 번째 정점(반시계 방향)
*
*	\return 삼각형 내부에 포함될 경우 true, 그렇지 않으면 false를 반환
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
*	\brief	삼각형의 세 정점 p, q, r,에 함수값(온도, 습도, 압력 등)f, g, h가 주어진 경우 그래디언트 방향을 계산한다.
*
*	\param	p[in]	삼각형의 첫 번째 정점(반시계 방향)
*	\param	q[in]	삼각형의 두 번째 정점(반시계 방향)
*	\param	r[in]	삼각형의 세 번째 정점(반시계 방향)
*	\param	f[in]	정점 p에 주어진 함수값
*	\param	g[in]	정점 q에 주어진 함수값
*	\param	h[in]	정점 r에 주어진 함수값
*
*	\return 함수값이 최대로 증가하는 방향을 반환한다.
*/
EgVec3 get_grad_on_tri(const EgPos &p, const EgPos &q, const EgPos &r, double f, double g, double h)
{
	// 세 변(벡터)을 정의한다.
	EgVec3 a = q - p;
	EgVec3 b = r - q;
	EgVec3 c = p - r;

	// 삼각형의 면적을 계산한다.
	double At = area_tri(p, q, r);

	// 각 변을 회전시킬 축(쿼터니온)을 구한다.
	EgVec3 N = (a ^ b).Normalize();
	EgQuat qt(N, 90.0);

	// 각 변을 반시계 방향으로 90도 회전시킨다.
	EgVec3 at = qt * a;
	EgVec3 ct = qt * c;

	// 삼각형의 그래디언트를 계산하여 반환한다.
	return ((g - f) * ct + (h - f) * at) / (2.0 * At);
}

/*!
*	\brief	세점 p, q, r로 정의되는 삼각형을 기준으로 pt의 barycentric 좌표를 계산한다.
*	\note	마지막 수정일: 2020-06-18
*
*	\param	pt[in]	조사할 점의 좌표
*	\param	p[in]	삼각형의 첫 번째 정점(반시계 방향)
*	\param	q[in]	삼각형의 두 번째 정점(반시계 방향)
*	\param	r[in]	삼각형의 세 번째 정점(반시계 방향)
*
*	\return pt의 barycentric 좌표 (alpha, beta, gamma)를 반환한다 (pt = alpha * p + beta * q + gamma * r).
*/
EgVec3 get_tri_barycentric_coords(EgPos pt, EgPos p, EgPos q, EgPos r)
{
	// 로컬 좌표계를 구한다.
	EgVec3 x_axis = (p - r).Normalize();
	EgVec3 z_axis = ((p - r) ^ (q - r)).Normalize();
	EgQuat qt;
	qt.SetFromFrameZX(z_axis, x_axis);
	qt = inv(qt);

	// 로컬 좌표계에서 표현된 벡터를 구하고,
	EgVec3 a = qt * (p - r);
	EgVec3 b = qt * (q - r);
	EgVec3 c = qt * (pt - r);

	// Barycentric 좌표를 구하여 반환한다.
	double denominator = a[0] * b[1] - b[0] * a[1];
	double alpha = (b[1] * c[0] - b[0] * c[1]) / denominator;
	double beta = (a[1] * c[0] + a[0] * c[1]) / denominator;
	return EgVec3(alpha, beta, 1.0 - alpha - beta);
}

