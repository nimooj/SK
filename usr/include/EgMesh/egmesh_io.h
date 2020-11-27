#pragma once
// 8개 API 제공

#include <Windows.h>
#include "..\EgMath\egmath.h"
#include "..\EgMesh\egmesh.h"
#include "..\GL\freeglut.h"
//#include "..\GL\glut.h"

/*!	\brief 경로명을 제외한 파일명(확장명 포함)을 반환하는 함수 */
std::string get_file_name_ext(std::string full_path);

/*! \brief 경로명을 제외한 파일명(확장명 불포함)을 반환하는 함수 */
std::string get_file_name(std::string full_path);

/*! \brief 파일의 확장자를 반환하는 함수 */
std::string get_file_ext(std::string full_path);

/*! \brief 파일의 디렉토리 경로명을 반환하는 함수 */
std::string get_file_path(std::string full_path);

/*! \brief 그룹으로 구분된 삼각형의 포인터 배열을 구한다 */
int get_grouped_faces(EgMesh *pMesh, std::map<std::string, std::vector<EgFace *>> &GroupedFaces);

/*! \brief 텍스처 좌표를 정규화 하는 함수 */
void normalize_uv(double &u, double &v);

// OBJ 파일 함수
EgMesh *import_obj(const char *fname);
bool export_obj(EgMesh *pMesh, const char *fname);

