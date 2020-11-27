#include "..\usr\include\EgMath\egmath.h"

/*************************/
/* EgVec3 형변환 함수 구현 */
/*************************/
/*!
*	\brief	2차원 벡터를 3차원 벡터로 변환한다.
*
*	\param	v[in]	2차원 벡터
*
*	\return 3차원 벡터를 반환한다.
*/
EgVec3 cast_vec3(const EgVec2 &v)
{
	return EgVec3(v[0], v[1], 0.0);
}

/*!
*	\brief	4차원 벡터를 3차원 벡터 형태로 반환한다.
*
*	\param	v[in] 4차원 벡터
*
*	\return 3차원 벡터를 반환한다.
*/
EgVec3 cast_vec3(const EgVec4 &v)
{
	return EgVec3(v[0], v[1], v[2]);
}

/*!
*	\brief	N 차원 벡터를 3차원 벡터로 변환한다.
*
*	\param	v[in]	N 차원 벡터
*
*	\return 3차원 벡터를 반환한다.
*/
EgVec3 cast_vec3(const EgVec &v)
{
	return EgVec3(v[0], v[1], v[2]);
}

/*!
*	\brief	점을 벡터로 변환한다.
*
*	\param	pt[in]	3차원 공간의 점
*
*	\return 변환된 벡터를 반환한다.
*/
EgVec3 cast_vec3(const EgPos &pt)
{
	return EgVec3(pt[0], pt[1], pt[2]);
}

/*************************/
/* EgVec4 형변환 함수 구현 */
/*************************/
/*!
*	\brief	3차원 벡터를 4차원 벡터형태로 반환한다.
*
*	\param	v[in]	3차원 벡터
*	\param	w[in]	4번째 좌표값
*
*	\return v의 확장된 4차원 벡터 표현을 반환한다.
*/
EgVec4 cast_vec4(const EgVec3 &v, double w)
{
	return EgVec4(v[0], v[1], v[2], w);
}

/*!
*	\brief	점 pt의 좌표를 4차원 벡터로 반환한다.
*
*	\param	pt[in]	3차원 공간의 점
*	\param	w[in]	4번째 좌표값
*
*	\return pt의 4차원 벡터 표현을 반환한다.
*/
EgVec4 cast_vec4(const EgPos &pt, double w)
{
	return EgVec4(pt[0], pt[1], pt[2], w);
}

/*!
*	\brief	쿼터니온을 4차원 벡터로 변환한다.
*
*	\param	q[in]	대상 쿼터니온
*
*	\return 4차원 벡터를 반환한다.
*/
EgVec4 cast_vec4(const EgQuat &q)
{
	return EgVec4(q.m_W, q.m_X, q.m_Y, q.m_Z);
}

/*************************/
/* EgVec 형변환 함수 구현 */
/*************************/
/*!
*	\brief	2차원 벡터 v의 좌표를 일반적인 벡터 형태로 반환한다.
*
*	\param	v[in]	2차원 벡터
*
*	\return 3차원 벡터 v의 좌표를 일반적인 벡터 형태로 반환한다.
*/
EgVec cast_vec(const EgVec2 &v)
{
	return EgVec(2, v[0], v[1]);
}

/*!
*	\brief	3차원 벡터 v의 좌표를 일반적인 벡터 형태로 반환한다.
*
*	\param	v[in]	3차원 공간의 벡터
*
*	\return 3차원 벡터 v의 좌표를 일반적인 벡터 형태로 반환한다.
*/
EgVec cast_vec(const EgVec3 &v)
{
	return EgVec(3, v[0], v[1], v[2]);
}

/*!
*	\brief	4차원 벡터 v의 좌표를 일반적인 벡터 형태로 반환
*
*	\param	v[in]	4차원 공간의 벡터
*
*	\return 4차원 벡터 v의 좌표의 일반 벡터 표현을 반환한다.
*/
EgVec cast_vec(const EgVec4 &v)
{
	return EgVec(4, v[0], v[1], v[2], v[3]);
}

/*!
*	\brief	점 pt의 좌표를 벡터형태로 반환한다.
*
*	\param	pt[in] 3차원 공간의 점
*
*	\return pt의 벡터 표현을 반환한다.
*/
EgVec cast_vec(const EgPos &pt)
{
	return EgVec(3, pt[0], pt[1], pt[2]);
}

/*!
*	\brief	쿼터니온을 4차원 벡터로 변환한다.
*
*	\param	q[in]	대상 쿼터니온
*
*	\return 벡터를 반환한다.
*/
EgVec cast_vec(const EgQuat &q)
{
	return EgVec(4, q.m_W, q.m_X, q.m_Y, q.m_Z);
}

/************************/
/* EgPos 형변환 함수 구현 */
/************************/
/*!
*	\brief	벡터 v의 좌표를 포인트 형태로 반환한다.
*
*	\param	v[in] 3차원 공간의 벡터
*
*	\return v의 좌표를 포인트 형태로 반환한다.
*/
EgPos cast_pos(const EgVec3 &v)
{
	return EgPos(v[0], v[1], v[2]);
}

/*!
*	\brief	4차원 벡터 v의 좌표를 3차원 포인트 형태로 반환한다.
*
*	\param	v[in]	4차원 공간의 벡터
*
*	\return 4차원 벡터 v의 좌표를 3차원 포인트 형태로 반환한다.
*/
EgPos cast_pos(const EgVec4 &v)
{
	return EgPos(v[0], v[1], v[2]);
}

/*!
*	\brief	n 차원 벡터를 3차원 포인트의 형태로 변환한다.
*
*	\param	v[in]	벡터
*
*	\return v의 3차원 포인트 표현을 반환한다.
*/
EgPos cast_pos(const EgVec &v)
{
	return EgPos(v[0], v[1], v[2]);
}

/************************/
/* EgMat 형변환 함수 구현 */
/************************/
/*!
*	\brief	행렬의 원소를 담은 실수배열을 반환한다.
*
*	\param	m[in]	행렬
*
*	\return 행렬의 원소를 저장한 실수 배열을 반환한다.
*/
double *cast_arr(EgMat &m)
{
	if (m.m_Data.empty())
		return NULL;
	else
		return &(m.m_Data[0]);
}

/*!
*	\brief	쿼터니온으로 표현되는 회전을 4 by 4 행렬로 변환한다.
*
*	\param	q[in]		대상쿼터니온
*	\param	isGL[in]	만약 true 이면 행렬에 OpenGL 형태로 계산된다.
*
*	\return 회전을 표현하는 행렬을 반환한다.
*/
EgMat cast_mat(const EgQuat &q, bool isGL)
{
	EgMat ret(4, 4);

	double s = 2.0 / (q.m_W * q.m_W + q.m_X * q.m_X + q.m_Y * q.m_Y + q.m_Z * q.m_Z);
	double xs = q.m_X * s, ys = q.m_Y * s, zs = q.m_Z * s;
	double wx = q.m_W * xs, wy = q.m_W * ys, wz = q.m_W * zs;
	double xx = q.m_X * xs, xy = q.m_X * ys, xz = q.m_X * zs;
	double yy = q.m_Y * ys, yz = q.m_Y * zs, zz = q.m_Z * zs;

	ret[0][0] = 1.0 - (yy + zz);
	ret[1][0] = xy + wz;
	ret[2][0] = xz - wy;

	ret[0][1] = xy - wz;
	ret[1][1] = 1.0 - (xx + zz);
	ret[2][1] = yz + wx;

	ret[0][2] = xz + wy;
	ret[1][2] = yz - wx;
	ret[2][2] = 1.0 - (xx + yy);

	ret[0][3] = ret[1][3] = ret[2][3] = 0.0;
	ret[3][0] = ret[3][1] = ret[3][2] = 0.0;
	ret[3][3] = 1.0;

	if (isGL)
		ret.SetTranspose();

	return ret;
}

/*!
*	\brief 강체변환 X에 해당하는 4행 4열의 동차좌표변환행렬로 변환한다.
*	\param X 강체변환
*	\param isGL 만약 true 이면 행렬에 OpenGL 형태로 저장한다.
*
*	\return 강체변환을 표현하는 행렬
*/
EgMat cast_mat(const EgTrans &X, bool isGL)
{
	EgMat R;
	R = cast_mat(X.m_Q, isGL);
	if (isGL)
	{
		R[3][0] = X.m_T[0];
		R[3][1] = X.m_T[1];
		R[3][2] = X.m_T[2];
	}
	else
	{
		R[0][3] = X.m_T[0];
		R[1][3] = X.m_T[1];
		R[2][3] = X.m_T[2];
	}
	return R;
}

/*************************/
/* EgQuat 형변환 함수 구현 */
/*************************/
/*!
*	\brief	벡터를 쿼터니온의 형태로 변환한다.
*
*	\param	v[in]	벡터
*
*	\return 변환된 쿼터니온 (w, x, y, z) = (v[0], v[1], v[2], v[3])을 반환한다.
*/
EgQuat cast_quat(const EgVec &v)
{
	return EgQuat(v[0], v[1], v[2], v[3]);
}

/*!
*	\brief	4차원 벡터를 쿼터니온으로 변환한다.
*
*	\param	v[in]	4차원 벡터
*
*	\return 변환된 쿼터니온 (w, x, y, z) = (v[0], v[1], v[2], v[3])을 반환한다.
*/
EgQuat cast_quat(const EgVec4 &v)
{
	return EgQuat(v[0], v[1], v[2], v[3]);
}

