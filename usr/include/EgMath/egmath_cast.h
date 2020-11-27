#pragma once

class EgPos;
class EgVec;
class EgVec2;
class EgVec3;
class EgVec4;
class EgMat;
class EgQuat;
class EgTrans;
class EgLine;
class EgPlane;

// EgVec3 형변환 함수
EgVec3 cast_vec3(const EgVec2 &v);
EgVec3 cast_vec3(const EgVec4 &v);
EgVec3 cast_vec3(const EgVec &v);
EgVec3 cast_vec3(const EgPos &p);

// EgVec4 형변환 함수
EgVec4 cast_vec4(const EgVec3 &v, double w = 0.0);
EgVec4 cast_vec4(const EgPos &p, double w = 0.0);
EgVec4 cast_vec4(const EgQuat &q);

// EgVec 형변환 함수
EgVec cast_vec(const EgVec2 &v);
EgVec cast_vec(const EgVec3 &v);
EgVec cast_vec(const EgVec4 &v);
EgVec cast_vec(const EgPos &p);
EgVec cast_vec(const EgQuat &q);

// EgPos 형변환 함수
EgPos cast_pos(const EgVec3 &v);
EgPos cast_pos(const EgVec4 &v);
EgPos cast_pos(const EgVec &v);

// EgMat 형변환 함수
double *cast_arr(EgMat &m);
EgMat cast_mat(const EgQuat &q, bool isGL = true);
EgMat cast_mat(const EgTrans &X, bool isGL = true);

// EgQuat 형변환 함수
EgQuat cast_quat(const EgVec &v);
EgQuat cast_quat(const EgVec4 &v);











