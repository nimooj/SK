#pragma once
// 8�� API ����

#include <Windows.h>
#include "..\EgMath\egmath.h"
#include "..\EgMesh\egmesh.h"
#include "..\GL\freeglut.h"
//#include "..\GL\glut.h"

/*!	\brief ��θ��� ������ ���ϸ�(Ȯ��� ����)�� ��ȯ�ϴ� �Լ� */
std::string get_file_name_ext(std::string full_path);

/*! \brief ��θ��� ������ ���ϸ�(Ȯ��� ������)�� ��ȯ�ϴ� �Լ� */
std::string get_file_name(std::string full_path);

/*! \brief ������ Ȯ���ڸ� ��ȯ�ϴ� �Լ� */
std::string get_file_ext(std::string full_path);

/*! \brief ������ ���丮 ��θ��� ��ȯ�ϴ� �Լ� */
std::string get_file_path(std::string full_path);

/*! \brief �׷����� ���е� �ﰢ���� ������ �迭�� ���Ѵ� */
int get_grouped_faces(EgMesh *pMesh, std::map<std::string, std::vector<EgFace *>> &GroupedFaces);

/*! \brief �ؽ�ó ��ǥ�� ����ȭ �ϴ� �Լ� */
void normalize_uv(double &u, double &v);

// OBJ ���� �Լ�
EgMesh *import_obj(const char *fname);
bool export_obj(EgMesh *pMesh, const char *fname);

