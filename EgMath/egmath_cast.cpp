#include "..\usr\include\EgMath\egmath.h"

/*************************/
/* EgVec3 ����ȯ �Լ� ���� */
/*************************/
/*!
*	\brief	2���� ���͸� 3���� ���ͷ� ��ȯ�Ѵ�.
*
*	\param	v[in]	2���� ����
*
*	\return 3���� ���͸� ��ȯ�Ѵ�.
*/
EgVec3 cast_vec3(const EgVec2 &v)
{
	return EgVec3(v[0], v[1], 0.0);
}

/*!
*	\brief	4���� ���͸� 3���� ���� ���·� ��ȯ�Ѵ�.
*
*	\param	v[in] 4���� ����
*
*	\return 3���� ���͸� ��ȯ�Ѵ�.
*/
EgVec3 cast_vec3(const EgVec4 &v)
{
	return EgVec3(v[0], v[1], v[2]);
}

/*!
*	\brief	N ���� ���͸� 3���� ���ͷ� ��ȯ�Ѵ�.
*
*	\param	v[in]	N ���� ����
*
*	\return 3���� ���͸� ��ȯ�Ѵ�.
*/
EgVec3 cast_vec3(const EgVec &v)
{
	return EgVec3(v[0], v[1], v[2]);
}

/*!
*	\brief	���� ���ͷ� ��ȯ�Ѵ�.
*
*	\param	pt[in]	3���� ������ ��
*
*	\return ��ȯ�� ���͸� ��ȯ�Ѵ�.
*/
EgVec3 cast_vec3(const EgPos &pt)
{
	return EgVec3(pt[0], pt[1], pt[2]);
}

/*************************/
/* EgVec4 ����ȯ �Լ� ���� */
/*************************/
/*!
*	\brief	3���� ���͸� 4���� �������·� ��ȯ�Ѵ�.
*
*	\param	v[in]	3���� ����
*	\param	w[in]	4��° ��ǥ��
*
*	\return v�� Ȯ��� 4���� ���� ǥ���� ��ȯ�Ѵ�.
*/
EgVec4 cast_vec4(const EgVec3 &v, double w)
{
	return EgVec4(v[0], v[1], v[2], w);
}

/*!
*	\brief	�� pt�� ��ǥ�� 4���� ���ͷ� ��ȯ�Ѵ�.
*
*	\param	pt[in]	3���� ������ ��
*	\param	w[in]	4��° ��ǥ��
*
*	\return pt�� 4���� ���� ǥ���� ��ȯ�Ѵ�.
*/
EgVec4 cast_vec4(const EgPos &pt, double w)
{
	return EgVec4(pt[0], pt[1], pt[2], w);
}

/*!
*	\brief	���ʹϿ��� 4���� ���ͷ� ��ȯ�Ѵ�.
*
*	\param	q[in]	��� ���ʹϿ�
*
*	\return 4���� ���͸� ��ȯ�Ѵ�.
*/
EgVec4 cast_vec4(const EgQuat &q)
{
	return EgVec4(q.m_W, q.m_X, q.m_Y, q.m_Z);
}

/*************************/
/* EgVec ����ȯ �Լ� ���� */
/*************************/
/*!
*	\brief	2���� ���� v�� ��ǥ�� �Ϲ����� ���� ���·� ��ȯ�Ѵ�.
*
*	\param	v[in]	2���� ����
*
*	\return 3���� ���� v�� ��ǥ�� �Ϲ����� ���� ���·� ��ȯ�Ѵ�.
*/
EgVec cast_vec(const EgVec2 &v)
{
	return EgVec(2, v[0], v[1]);
}

/*!
*	\brief	3���� ���� v�� ��ǥ�� �Ϲ����� ���� ���·� ��ȯ�Ѵ�.
*
*	\param	v[in]	3���� ������ ����
*
*	\return 3���� ���� v�� ��ǥ�� �Ϲ����� ���� ���·� ��ȯ�Ѵ�.
*/
EgVec cast_vec(const EgVec3 &v)
{
	return EgVec(3, v[0], v[1], v[2]);
}

/*!
*	\brief	4���� ���� v�� ��ǥ�� �Ϲ����� ���� ���·� ��ȯ
*
*	\param	v[in]	4���� ������ ����
*
*	\return 4���� ���� v�� ��ǥ�� �Ϲ� ���� ǥ���� ��ȯ�Ѵ�.
*/
EgVec cast_vec(const EgVec4 &v)
{
	return EgVec(4, v[0], v[1], v[2], v[3]);
}

/*!
*	\brief	�� pt�� ��ǥ�� �������·� ��ȯ�Ѵ�.
*
*	\param	pt[in] 3���� ������ ��
*
*	\return pt�� ���� ǥ���� ��ȯ�Ѵ�.
*/
EgVec cast_vec(const EgPos &pt)
{
	return EgVec(3, pt[0], pt[1], pt[2]);
}

/*!
*	\brief	���ʹϿ��� 4���� ���ͷ� ��ȯ�Ѵ�.
*
*	\param	q[in]	��� ���ʹϿ�
*
*	\return ���͸� ��ȯ�Ѵ�.
*/
EgVec cast_vec(const EgQuat &q)
{
	return EgVec(4, q.m_W, q.m_X, q.m_Y, q.m_Z);
}

/************************/
/* EgPos ����ȯ �Լ� ���� */
/************************/
/*!
*	\brief	���� v�� ��ǥ�� ����Ʈ ���·� ��ȯ�Ѵ�.
*
*	\param	v[in] 3���� ������ ����
*
*	\return v�� ��ǥ�� ����Ʈ ���·� ��ȯ�Ѵ�.
*/
EgPos cast_pos(const EgVec3 &v)
{
	return EgPos(v[0], v[1], v[2]);
}

/*!
*	\brief	4���� ���� v�� ��ǥ�� 3���� ����Ʈ ���·� ��ȯ�Ѵ�.
*
*	\param	v[in]	4���� ������ ����
*
*	\return 4���� ���� v�� ��ǥ�� 3���� ����Ʈ ���·� ��ȯ�Ѵ�.
*/
EgPos cast_pos(const EgVec4 &v)
{
	return EgPos(v[0], v[1], v[2]);
}

/*!
*	\brief	n ���� ���͸� 3���� ����Ʈ�� ���·� ��ȯ�Ѵ�.
*
*	\param	v[in]	����
*
*	\return v�� 3���� ����Ʈ ǥ���� ��ȯ�Ѵ�.
*/
EgPos cast_pos(const EgVec &v)
{
	return EgPos(v[0], v[1], v[2]);
}

/************************/
/* EgMat ����ȯ �Լ� ���� */
/************************/
/*!
*	\brief	����� ���Ҹ� ���� �Ǽ��迭�� ��ȯ�Ѵ�.
*
*	\param	m[in]	���
*
*	\return ����� ���Ҹ� ������ �Ǽ� �迭�� ��ȯ�Ѵ�.
*/
double *cast_arr(EgMat &m)
{
	if (m.m_Data.empty())
		return NULL;
	else
		return &(m.m_Data[0]);
}

/*!
*	\brief	���ʹϿ����� ǥ���Ǵ� ȸ���� 4 by 4 ��ķ� ��ȯ�Ѵ�.
*
*	\param	q[in]		������ʹϿ�
*	\param	isGL[in]	���� true �̸� ��Ŀ� OpenGL ���·� ���ȴ�.
*
*	\return ȸ���� ǥ���ϴ� ����� ��ȯ�Ѵ�.
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
*	\brief ��ü��ȯ X�� �ش��ϴ� 4�� 4���� ������ǥ��ȯ��ķ� ��ȯ�Ѵ�.
*	\param X ��ü��ȯ
*	\param isGL ���� true �̸� ��Ŀ� OpenGL ���·� �����Ѵ�.
*
*	\return ��ü��ȯ�� ǥ���ϴ� ���
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
/* EgQuat ����ȯ �Լ� ���� */
/*************************/
/*!
*	\brief	���͸� ���ʹϿ��� ���·� ��ȯ�Ѵ�.
*
*	\param	v[in]	����
*
*	\return ��ȯ�� ���ʹϿ� (w, x, y, z) = (v[0], v[1], v[2], v[3])�� ��ȯ�Ѵ�.
*/
EgQuat cast_quat(const EgVec &v)
{
	return EgQuat(v[0], v[1], v[2], v[3]);
}

/*!
*	\brief	4���� ���͸� ���ʹϿ����� ��ȯ�Ѵ�.
*
*	\param	v[in]	4���� ����
*
*	\return ��ȯ�� ���ʹϿ� (w, x, y, z) = (v[0], v[1], v[2], v[3])�� ��ȯ�Ѵ�.
*/
EgQuat cast_quat(const EgVec4 &v)
{
	return EgQuat(v[0], v[1], v[2], v[3]);
}

