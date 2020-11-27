#ifndef _GMATH_UTIL_H
#define _GMATH_UTIL_H

// 클래스 선언
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

// EgVec2 유틸 함수
EgVec2 proj(const EgVec2 &v, const EgVec2 &w);
EgVec2 ortho(const EgVec2 &v, bool CCW);
double dist(const EgVec2 &v, const EgVec2 &w);
double norm(const EgVec2 &v);
double norm_sq(const EgVec2 &v);
double angle(const EgVec2 &v, const EgVec2 &w, bool radian = false);

// EgVec3 유틸 함수
EgVec3 proj(const EgVec3 &v, const EgVec3 &w);
EgVec3 ortho(const EgVec3 &v);
double dist(const EgVec3 &v, const EgVec3 &w);
double det(const EgVec3 &u, const EgVec3 &v, const EgVec3 &w);
double norm(const EgVec3 &v);
double norm_sq(const EgVec3 &v);
double angle(const EgVec3 &v, const EgVec3 &w, bool radian = false);
double angle(const EgVec3 &v, const EgVec3 &w, const EgVec3 &up, bool radian = false);

// EgVec4 유틸 함수
double norm(const EgVec4 &v);
double norm_sq(const EgVec4 &v);
double dist(const EgVec4 &v, const EgVec4 &w);
EgVec4 homogenize(const EgVec4 &v);
EgVec4 cartesianize(const EgVec4 &v);
EgVec4 rationalize(const EgVec4 &v);

// EgVec 유틸 함수
double norm(const EgVec &v);
double norm_sq(const EgVec &v);
double dist(const EgVec &v, const EgVec &w);

// EgPos 유틸 함수
double norm(const EgPos &p);
double norm_sq(const EgPos &p);
double dist(const EgPos &p, const EgPos &q);
double dist_sq(const EgPos &p, const EgPos &q);
EgPos lerp(const EgPos &p, const EgPos &q, double t);
EgPos affine_sum(int N, ...);
EgPos affine_sum(EgPos *Points, double *Weights, const int Size);
EgPos round_off(const EgPos &p, int place);
int is_obtuse_tri(const EgPos &p, const EgPos &q, const EgPos &r); // 박정호 2019 02 03 추가
EgPos circumcenter(const EgPos &p, const EgPos &q, const EgPos &r); // 박정호 2019 02 03 추가
double area_tri(const EgPos &p, const EgPos &q, const EgPos &r); // 박정호 2019 02 03 추가

// EgMat 유틸 함수
EgMat tr(const EgMat &m);
EgMat ref(const EgMat &m);
EgMat rref(const EgMat &m);
EgMat basis_null(const EgMat &m);
int rank(const EgMat &m);
int nullity(const EgMat &m);
double det(const EgMat &A);
EgMat inv(const EgMat &A);
EgMat sub(const EgMat &A, int r0, int c0, int r1, int c1);
int dcmp_lu(const EgMat &A, EgMat &L, EgMat &U);
int dcmp_lu(EgMat &A, EgVec &Idx, double &d);
int dcmp_qr(const EgMat &A, EgMat &Q, EgMat &R);
void dcmp_sv(const EgMat &A, EgMat &U, EgVec &S, EgMat &V);
void lin_sys_lu(const EgMat &A, EgVec &x, const EgVec &b, const EgVec &Idx);
double lin_sys_sv(const EgMat &A, EgVec &x, const EgVec &b);
double lin_sys_sv(const EgMat &U, const EgVec &S, const EgMat &V, EgVec &x, const EgVec &b);
int lin_sys_cg(const EgMat &A, EgVec &x, const EgVec &b, int iter);
int lin_sys_qr(const EgMat &A, EgVec &x, const EgVec &b);
int eigen_sys(const EgMat &A, EgVec &D, EgMat &V);
void pca(EgMat &Data, EgMat &PC, EgVec &SigVal, EgVec &Mean);

// EgQuat 유틸 함수
double norm(const EgQuat &q);
EgQuat inv(const EgQuat &q);
EgQuat exp(const EgVec3 &v);
EgVec3 log(const EgQuat &q);
double quat_dot(const EgQuat &q1, const EgQuat &q2);
EgQuat slerp(EgQuat q1, EgQuat q2, double t);

// EgLine 유틸 함수
double dist(const EgLine &l, const EgPos &q);
double dist(EgLine &l0, EgLine &l1, double &t1, double &t2);
bool intersect_line_line(EgPos &p, EgLine &l0, EgLine &l1, double &t0, double &t1);
bool intersect_line_line2(EgPos &p, EgLine &l0, EgLine &l1);
bool intersect_line_triangle(EgPos &p1, EgPos &p2, EgPos &p3, EgLine &l, double &t, EgPos &q, double &alpha, double &beta, double &gamma, bool bCull = true);
bool intersect_line_triangle(EgPos &p1, EgPos &p2, EgPos &p3, EgLine &l, EgPos &ispt, bool bCull = true);
bool intersect_line_triangle2(EgPos &p1, EgPos &p2, EgPos &p3, EgLine &l, double &param, bool bCull);

// EgPlane 유틸 함수
double dist(EgPlane &pi, EgPos &p);
EgPos proj(EgPos &p, EgPlane &pi);
bool intersect_line_plane(EgPos &p, EgLine &l, EgPlane &pi);
bool intersect_plane_plane(EgLine &l, EgPlane &pi1, EgPlane &pi2);     
bool intersect_planes(EgPos &p, EgPlane &pi1, EgPlane &pi2, EgPlane &pi3);

// EgTrans 유틸 함수
EgTrans inv(const EgTrans &X);
EgVec3 part_trans(const EgTrans &X);
EgQuat part_rot(const EgTrans &X);
double norm(const EgTrans &X);
double dist(const EgTrans &X1, const EgTrans &X2);
EgTrans ad(const EgTrans &X1, const EgTrans &dX, const EgTrans &X2);
EgTrans lerp(EgTrans X1, EgTrans X2, const double t);

// 기타 유틸 함수
int get_gcd(std::vector<int> &NumList);
void find_convexhull_2d(const std::vector<EgPos> &InPts, std::vector<EgPos> &OutPts);
bool pt_in_polygon(const std::vector<EgPos> &Pts, EgPos const &p);
bool pt_in_triangle(EgVec2 p, EgVec2 p1, EgVec2 p2, EgVec2 p3);
EgVec3 get_grad_on_tri(const EgPos &p, const EgPos &q, const EgPos &r, double f, double g, double h);
EgVec3 get_tri_barycentric_coords(EgPos pt, EgPos p, EgPos q, EgPos r);

#endif // _GMATH_UTIL_H