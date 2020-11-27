#pragma once

#include "..\usr\include\EgMesh\egmesh.h"
#include <cstring>
#include <vector>
#include <map>
#include <direct.h>

static bool import_obj_mtl(EgMesh *pMesh, const char *fname);
static bool export_obj_mtl(EgMesh *pMesh, const char *fname);


/*!
*	\brief 경로명을 제외한 파일명(확장명 포함)을 반환한다.
*
*	\param full_path 경로명을 포함한 파일의 이름
*
*	\return \a full_path에서 경로명을 제외한 파일명(확장명)을 반환한다.
*/
std::string get_file_name_ext(std::string full_path)
{
	std::string ret;
	if (full_path.empty())
		return ret;

	int idx = (int)full_path.rfind("\\");
	ret = full_path.substr(idx + 1, full_path.length() - idx - 1);

	return ret;
}

/*!
*	\brief 경로명을 제외한 파일명(확장명 불포함)을 반환한다.
*
*	\param full_path 경로명을 포함한 파일의 이름
*
*	\return \a full_path에서 경로명을 제외한 파일명(확장명 불포함)을 반환한다.
*/
std::string get_file_name(std::string full_path)
{
	std::string ret;
	if (full_path.empty())
		return ret;

	int idx0 = (int)full_path.rfind("\\");
	if (idx0 == -1)
		idx0 = (int)full_path.rfind("/");
	int idx1 = (int)full_path.rfind(".");
	ret = full_path.substr(idx0 + 1, idx1 - idx0 - 1);

	return ret;
}

/*!
*	\brief 파일의 확장자를 반환한다.
*
*	\param full_path 경로명을 포함한 파일의 이름
*
*	\return \a full_path에서 파일 확장자를 반환한다.
*/
std::string get_file_ext(std::string full_path)
{
	std::string ret;
	if (full_path.empty())
		return ret;

	int idx = (int)full_path.rfind(".");
	ret = full_path.substr(idx + 1, full_path.length() - idx - 1);

	return ret;
}

/*!
*	\brief 파일의 디렉토리 경로명을 반환한다.
*
*	\param full_path 경로명을 포함한 파일의 이름
*
*	\return \a full_path에서 디렉토리 경로명을 반환한다.
*/
std::string get_file_path(std::string full_path)
{
	std::string ret;
	if (full_path.empty())
		return ret;

	int idx = (int)full_path.rfind("\\");
	if (idx == -1)
		idx = (int)full_path.rfind("/");
	ret = full_path.substr(0, idx);

	return ret;
}

/*!
*	\brief 그룹으로 구분된 삼각형의 포인터 배열을 구한다.
*
*	\param pMesh 대상 메쉬에 대한 포인터
*	\param GroupedFaces 그룹명으로 구분된 삼각형 포인터가 저장된다.
*
*	\return 그룹의 개수를 반환한다.
*/
int get_grouped_faces(EgMesh *pMesh, std::map<std::string, std::vector<EgFace *>> &GroupedFaces)
{
	// 맵을 초기화 하고
	GroupedFaces.clear();

	// 각각의 삼각형에 대하여
	for (EgFace *f : pMesh->m_pFaces)
	{
		// 삼각형이 속한 그룹의 배열에 삼각형의 포인터를 추가한다.
		if (f->m_GroupName != "")
			GroupedFaces[f->m_GroupName].push_back(f);
		else
		{
			std::string key("default");
			GroupedFaces[key].push_back(f);
		}
	}

	// 그룹의 개수를 반환한다.
	return (int)GroupedFaces.size();
}

/*!
*	\brief 텍스처 좌표를 0과 1사이로 정규화 한다.
*
*	\param u 텍스처 좌표 u
*	\param v 텍스처 좌표 v
*/
void normalize_uv(double &u, double &v)
{
	u = (u > 1.0) ? (u - (int)(u)) : u;
	u = (u < 0.0) ? (u - (int)(u - 1.0)) : u;

	v = (v > 1.0) ? (v - (int)(v)) : v;
	v = (v < 0.0) ? (v - (int)(v - 1.0)) : v;
}

/*!
*	\brief 실수를 문자열로 변환하여 반환한다.
*
*	\param v 문자열로 변환될 실수
*
*	\return 문자열로 변환된 실수
*/
std::string cast_str(double v)
{
	char buffer[128];
	sprintf_s(buffer, "%lf", v);

	return std::string(buffer);
}

/*!
*	\brief OBJ 파일에서 사용하는 MTL(재질)정보를 읽어 메쉬에 추가한다.
*
*	\param pMesh 재질을 추가할 메쉬의 포인터
*	\param fname 재질 파일의 이름(현재 작업 경로에서 상대적인 경로로 표현됨)
*
*	\return 성공: true, 실패: flase
*/
bool import_obj_mtl(EgMesh *pMesh, const char *fname)
{
	// 생성할 재질 변수를 정의한다.
	EgMaterial *pMtl = NULL;

	// 파일을 열고, 
	FILE *fp;
	fopen_s(&fp, fname, "r");
	if (!fp) // 실패하면 false를 반환한다.
	{
		printf("\tImporting %s failed...\n", fname);
		return false;
	}
	else
		printf("\tImporting %s...\n", fname);

	// 파일의 끝까지 한 단어씩 읽어, tag 배열에 저장한다.
	char tag[256];
	while (fscanf_s(fp, "%s", tag, 256) != EOF)
	{
		// newmtl (new material) 태그라면,
		if (!strcmp(tag, "newmtl"))
		{
			// 재질의 이름을 읽고,
			char tmp[256];
			fscanf_s(fp, "%s", tmp, 256);

			// 재질을 생성하여, 메쉬의 재질 리스트에 추가한다.
			pMtl = new EgMaterial(tmp);
			pMesh->AddMaterial(pMtl);
		}

		// Ka (ambient coefficients) 태그라면,
		if (!strcmp(tag, "Ka"))
		{
			// ambient 성분을 읽어서 재질을 설정한다.
			GLfloat c[3];
			fscanf_s(fp, "%f%f%f", &c[0], &c[1], &c[2]);
			pMtl->SetAmbient(c[0], c[1], c[2]);
		}

		// Kd (diffuse coefficients) 태그라면,
		if (!strcmp(tag, "Kd"))
		{
			// diffuse 성분을 읽어서 재질을 설정한다.
			GLfloat c[3];
			fscanf_s(fp, "%f%f%f", &c[0], &c[1], &c[2]);
			pMtl->SetDiffuse(c[0], c[1], c[2]);
		}

		// Ks (specular coefficients) 태그라면,
		if (!strcmp(tag, "Ks"))
		{
			// specular 성분을 읽어서 재질을 설정한다.
			GLfloat c[3];
			fscanf_s(fp, "%f%f%f", &c[0], &c[1], &c[2]);
			pMtl->SetSpecular(c[0], c[1], c[2]);
		}

		// map_Kd (diffuse texture file) 태그라면,
		if (!strcmp(tag, "map_Kd"))
		{
			// 텍스처 파일을 읽는다.
			char tex_name[512];
			fscanf_s(fp, "%s", tex_name, 512);

			// 이미 로드된 텍스처라면 기존의 텍스처를 설정한다.
			pMtl->m_pTexture = pMesh->GetTexture(tex_name);

			// 새로운 텍스처라면
			if (pMtl->m_pTexture == NULL)
			{
				// 새로운 텍스처를 생성하여 메쉬에 추가하고, 재질에 설정한다.
				EgTexture *pTexture = ::create_texture(tex_name);
				if (pTexture != NULL)
				{
					pMesh->AddTexture(pTexture);
					pMtl->SetTexture(pTexture);
				}
			}
		}
	}

	// 파일을 닫는다.
	fclose(fp);
	return true;
}

/*!
*	\brief OBJ 파일을 임포트한다.
*
*	\param fname 메쉬 파일의 이름[in]
*
*	\return 임포트된 메쉬의 포인터를 반환한다.
*/
EgMesh *import_obj(const char *fname)
{
	// 파일을 열고 실패하면 false를 반환한다.
	FILE *fp;
	fopen_s(&fp, fname, "r");
	if (!fp)
	{
		printf("Importing %s failed...\n", fname);
		return NULL;
	}
	else
		printf("Importing %s...\n", fname);

	// 현재 작업 디렉터리를 절대 경로로 출력한다.
	char cwd[1024] = { 0 };
	_getcwd(cwd, 1024);
	printf("\tCurrent working directory = %s\n", cwd);

	// 모델 검색 디렉터리로 이동하고, 절대 경로로 출력한다.
	std::string model_dir = get_file_path(fname);
	_chdir(model_dir.c_str());
	char msd[1024] = { 0 };
	_getcwd(msd, 1024);
	printf("\tModel directory = %s\n\n", msd);

	// 파일의 이름(경로 및 확장자 제외)을 구하여 메쉬를 생성한다.
	std::string MeshName = get_file_name(fname);
	EgMesh *pMesh = ::create_mesh(MeshName);
	printf("\tMesh name = %s\n", MeshName.c_str());

	// 로딩에 필요한 임시 변수들을 선언한다.
	char tag[1024];
	bool bTexture = false;
	bool bNormal = false;
	std::string GroupName;
	EgMaterial *pCurrMtl = NULL;

	// 파일의 끝까지 한 단어씩 읽으면서 tag 배열에 저장한다.
	while (fscanf_s(fp, "%s", tag, 1024) != EOF)
	{
		// mtllib (material library) 태그라면,
		if (!strcmp(tag, "mtllib"))
		{
			// 재질 파일명(주로 상대 경로)을 구하여
			char mtl_fname[1024];
			fscanf_s(fp, "%s", mtl_fname, 1024);

			// 재질 파일을 읽는다. 
			import_obj_mtl(pMesh, mtl_fname);
		}

		// v (vertex) 태그라면,
		if (!strcmp(tag, "v"))
		{
			// 좌표(x, y, z)를 읽어 정점을 생성하고 메쉬에 추가한다.
			double x, y, z;
			fscanf_s(fp, "%lf%lf%lf", &x, &y, &z);
			EgVertex *pVert = ::create_vertex(x, y, z);
			pMesh->AddVertex(pVert);
		}

		// vt (vertex texture) 태그라면,
		if (!strcmp(tag, "vt"))
		{
			// 텍스처 좌표(u, v)를 읽어 텍셀을 생성하고 메쉬에 추가한다.
			bTexture = true;
			double u, v;
			fscanf_s(fp, "%lf%lf", &u, &v);
			EgTexel *pTexel = ::create_texel(u, v);
			pMesh->AddTexel(pTexel);
		}

		// vn (vertex normal) 태그라면,
		if (!strcmp(tag, "vn"))
		{
			// 법선의 방향(nx, ny, nz)을 읽어 법선을 생성하고 메쉬에 추가한다.
			bNormal = true;
			double nx, ny, nz;
			fscanf_s(fp, "%lf%lf%lf", &nx, &ny, &nz);
			EgNormal *pNormal = ::create_normal(nx, ny, nz);
			pMesh->AddNormal(pNormal);
		}

		// g (group) 태그라면,
		if (!strcmp(tag, "g"))
		{
			// 그룹의 이름을 읽는다.
			char grp_name[256];
			fscanf_s(fp, "%s", grp_name, 256);
			GroupName = std::string(grp_name);
		}

		// usemtl (use material) 태그라면,
		if (!strcmp(tag, "usemtl"))
		{
			// 메쉬의 재질 리스트에서 해당 재질을 찾는다.
			char mtl_name[256];
			fscanf_s(fp, "%s", mtl_name, 256);
			pCurrMtl = pMesh->GetMaterial(mtl_name);
		}

		// f (face) 태그라면,
		if (!strcmp(tag, "f"))
		{
			// 폴리곤을 구성하는 정점, 텍셀, 노말의 인덱스를 저장하는 변수를 선언하고,
			std::vector<int> vIdxs, tIdxs, nIdxs;

			// 한 줄을 읽어서,
			char line[256];
			fgets(line, 256, fp);
			char *pData = line;

			// 한 줄을 모두 읽을 때까지
			while (true)
			{
				// 불필요한 공백과 줄빠꿈을 건너띄고,
				while (*pData == ' ' || *pData == '\n')
					pData++;

				// 한 줄을 모두 읽었다면 반복문을 빠져 나온다.
				if (!strcmp(pData, ""))
					break;

				// 인덱스 정보를 buffer로 읽은 후,
				char buffer[256];	// buffer[] = "vidx/tidx/nidx" 
				sscanf_s(pData, "%s", buffer, 256);
				pData += strlen(buffer);

				// 인덱스 정보를 저장할 변수를 선언하고
				char sep;
				int vidx, tidx, nidx;

				// 정점, 텍셀, 노말 인덱스가 모두 있는 경우 (f 1/1/1/ 2/2/2 3/3/3 ...)
				if (bTexture && bNormal)
				{
					// 모든 인덱스를 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d%c%d%c%d", &vidx, &sep, 1, &tidx, &sep, 1, &nidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					tidx = (tidx < 0) ? -tidx : tidx;
					nidx = (nidx < 0) ? -nidx : nidx;
					vIdxs.push_back(vidx - 1);
					tIdxs.push_back(tidx - 1);
					nIdxs.push_back(nidx - 1);
					continue;
				}

				// 정점과 텍셀 인덱스만 있는 경우 (f 1/1 2/2 3/3 ...)
				if (bTexture && !bNormal)
				{
					// 정점과 텍셀 인덱스만 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d%c%d", &vidx, &sep, 1, &tidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					tidx = (tidx < 0) ? -tidx : tidx;
					vIdxs.push_back(vidx - 1);
					tIdxs.push_back(tidx - 1);
					continue;
				}

				// 정점과 노말 인덱스만 있는 경우 (f 1//1 2//2 3//3 ...)
				if (!bTexture && bNormal)
				{
					// 정점과 노말 인덱스만 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d%c%c%d", &vidx, &sep, 1, &sep, 1, &nidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					nidx = (nidx < 0) ? -nidx : nidx;
					vIdxs.push_back(vidx - 1);
					nIdxs.push_back(nidx - 1);
					continue;
				}

				// 정점의 인덱스만 있는 경우 (f 1 2 3 ...)
				if (!bTexture && !bNormal)
				{
					// 정점의 인덱스만 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d", &vidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					vIdxs.push_back(vidx - 1);
					continue;
				}
			}

			// 폴리곤을 삼각형으로 나누어 메쉬에 추가한다.
			int PolygonSize = (int)vIdxs.size(); // 삼각형(PolygonSize = 3), 사각형(PolygonSize = 4), ...
			for (int i = 0; i < PolygonSize - 2; ++i)
			{
				// 삼각형의 정점을 구하고,
				EgVertex *V[3] = { 0 };
				V[0] = pMesh->GetVert(vIdxs[0]);
				V[1] = pMesh->GetVert(vIdxs[i + 1]);
				V[2] = pMesh->GetVert(vIdxs[i + 2]);

				// 각 정점의 텍셀 정보를 구하고,
				EgTexel *T[3] = { 0 };
				if (bTexture)
				{
					T[0] = pMesh->GetTexel(tIdxs[0]);
					T[1] = pMesh->GetTexel(tIdxs[i + 1]);
					T[2] = pMesh->GetTexel(tIdxs[i + 2]);
				}

				// 각 정점의 법선 정보를 구하고,
				EgNormal *N[3] = { 0 };
				if (bNormal)
				{
					N[0] = pMesh->GetNormal(nIdxs[0]);
					N[1] = pMesh->GetNormal(nIdxs[i + 1]);
					N[2] = pMesh->GetNormal(nIdxs[i + 2]);
				}
				else // 법선 정보가 없다면 삼각형 법선으로 대체한다.
				{
					EgVec3 n = ((V[1]->m_Pos - V[0]->m_Pos) ^ (V[2]->m_Pos - V[0]->m_Pos)).Normalize();
					N[0] = N[1] = N[2] = ::create_normal(n[0], n[1], n[2]);
					pMesh->AddNormal(N[0]);
				}

				// 삼각형을 생성하여 메쉬에 추가한다.
				EgFace *f = new EgFace(V[0], V[1], V[2], T[0], T[1], T[2], N[0], N[1], N[2], pCurrMtl, GroupName);
				pMesh->AddFace(f);
			}
		}
	}

	// 파일을 닫는다.
	fclose(fp);

	// 하프에지에 반대편 에지의 정보를 갱신한다.
	pMesh->InitEdgeMate();

	// 메쉬의 경계 상자를 계산한다.
	pMesh->UpdateBndBox();

	// 텍스처 유무에 따라 쉐이딩 타입을 설정한다.
	if (bTexture)
		pMesh->m_ShadeType = SHADE_BY_TEXTURE;

	printf("Importing OBJ mesh complete...\n\n");

	// 모델 검색 디렉터리에서 원래 작업 디렉터리로 복귀한다.
	_chdir(cwd);

	// 임포트된 메쉬의 포인터를 반환한다.
	return pMesh;
}

/*!
*	\brief 메쉬를 파일(*.obj)로 저장한다.
*
*	\param pMesh 저장할 메쉬의 포인터
*	\param fname 파일의 이름
*
*	\return 성공: true, 실패: flase
*/
bool export_obj(EgMesh *pMesh, const char *fname)
{
	// 파일을 열고 실패하면 false를 반환한다.
	FILE *fp;
	fopen_s(&fp, fname, "w");
	if (!fp)
	{
		printf("Exporting OBJ mesh failed...\n");
		return false;
	}
	else
		printf("Exporting OBJ mesh...\n");

	// 현재 작업 디렉터리를 절대 경로로 출력한다.
	char cwd[1024] = { 0 };
	_getcwd(cwd, 1024);
	printf("\tCurrent working directory = %s\n", cwd);

	// 모델 저장 디렉터리로 이동하고, 절대 경로로 출력한다.
	std::string model_dir = get_file_path(fname);
	_chdir(model_dir.c_str());
	char msd[1024] = { 0 };
	_getcwd(msd, 1024);
	printf("\tModel directory = %s\n\n", msd);

	// 파일의 생성 정보 출력하고,
	fprintf_s(fp, "%s", "# Wavefront OBJ exported by EgTools\n");
	fprintf_s(fp, "%s", "# http://gaia.dongguk.edu \n\n");

	// 재질 라이브러리 파일을 출력하고,
	int NumMtrl = pMesh->GetNumMtrl();
	printf("\tNumber of materials = %d\n", NumMtrl);
	if (NumMtrl != 0)
	{
		// 재질 파일(*.mtl)의 이름을 출력하고,
		std::string file_name(fname);
		file_name = get_file_name(fname) + ".mtl";
		fprintf(fp, "mtllib %s\n\n", file_name.c_str());

		// 재질 정보 파일(*.mtl)을 생성하여 출력한다.
		export_obj_mtl(pMesh, file_name.c_str());
	}

	// 정점, 텍셀, 법선의 인덱스를 갱신한다.
	int NumVert = pMesh->UpdateVertIdx();
	int NumTexel = pMesh->UpdateTexIdx();
	int NumNormal = pMesh->UpdateNormalIdx();
	int NumFace = pMesh->UpdateFaceIdx();

	// 정점의 정보를 출력한다.
	printf("\tNumber of vertices = %d\n", NumVert);
	for (EgVertex *v : pMesh->m_pVerts)
		fprintf(fp, "%s %lf %lf %lf\n", "v", v->m_Pos[0], v->m_Pos[1], v->m_Pos[2]);
	fprintf(fp, "\n");

	// 텍셀의 정보를 출력한다.
	printf("\tNumber of texels = %d\n", NumTexel);
	for (EgTexel *t : pMesh->m_pTexels)
		fprintf(fp, "%s %lf %lf\n", "vt", t->m_UV[0], t->m_UV[1]);
	fprintf(fp, "\n");

	// 법선의 정보를 출력한다.
	printf("\tNumber of normals = %d\n", NumNormal);
	for (EgNormal *n : pMesh->m_pNormals)
		fprintf(fp, "%s %lf %lf %lf\n", "vn", n->m_Dir[0], n->m_Dir[1], n->m_Dir[2]);
	fprintf(fp, "\n");

	// 삼각형 정보를 출력한다.
	printf("\tNumber of faces = %d\n", NumFace);

	// 그룹명으로 구분된 삼각형의 포인터 배열을 얻는다.
	std::map<std::string, std::vector<EgFace *>> GroupedFaces;
	int NumGroup = ::get_grouped_faces(pMesh, GroupedFaces);

	// 삼각형의 그룹 정보가 없다면
	if (NumGroup == 0)
	{
		// 순자척으로 모든 폴리곤에 대하여,
		EgMaterial *pPrevMtrl = NULL;
		for (EgFace *f : pMesh->m_pFaces)
		{
			if (pPrevMtrl == NULL || f->m_pMaterial != pPrevMtrl)
			{
				if (f->m_pMaterial != NULL)
					fprintf(fp, "usemtl %s\n", f->m_pMaterial->m_Name.c_str());
				pPrevMtrl = f->m_pMaterial;
			}
			int vidx0 = f->GetVertex(0)->m_Idx + 1;
			int vidx1 = f->GetVertex(1)->m_Idx + 1;
			int vidx2 = f->GetVertex(2)->m_Idx + 1;
			int nidx0 = f->GetNormal(0)->m_Idx + 1;
			int nidx1 = f->GetNormal(1)->m_Idx + 1;
			int nidx2 = f->GetNormal(2)->m_Idx + 1;

			if (f->GetTexel(0) != NULL)
			{
				int tidx0 = f->GetTexel(0)->m_Idx + 1;
				int tidx1 = f->GetTexel(1)->m_Idx + 1;
				int tidx2 = f->GetTexel(2)->m_Idx + 1;
				fprintf_s(fp, "%s %d/%d/%d %d/%d/%d %d/%d/%d\n", "f",
					vidx0, tidx0, nidx0, vidx1, tidx1, nidx1, vidx2, tidx2, nidx2);
			}
			else
				fprintf_s(fp, "%s %d//%d %d//%d %d//%d\n", "f",
					vidx0, nidx0, vidx1, nidx1, vidx2, nidx2);
		}
		fprintf_s(fp, "\n");
	}
	else
	{
		std::map<std::string, std::vector<EgFace *>>::iterator it;
		// 각각의 그룹에 대하여,
		for (it = GroupedFaces.begin(); it != GroupedFaces.end(); ++it)
		{
			// 그룹의 이름을 출력하고,
			std::string GroupName = it->first;
			fprintf(fp, "g %s\n", GroupName.c_str());

			// 이전 재질의 인덱스를 위한 변수를 선언하고,
			EgMaterial *pPrevMtrl = NULL;

			// 그룹에 속한 각각의 폴리곤에 대하여,
			int NumFace = (int)it->second.size();
			for (int i = 0; i < NumFace; ++i)
			{
				// 삼각형 정보를 얻고,
				EgFace *f = it->second[i];

				// 삼각형의 재질 정보가 있다면 이전 재질정보와 비교하여 다를 경우 출력하고,
				if (pPrevMtrl == NULL || f->m_pMaterial != pPrevMtrl)
				{
					if (f->m_pMaterial != NULL)
						fprintf(fp, "usemtl %s\n", f->m_pMaterial->m_Name.c_str());
					pPrevMtrl = f->m_pMaterial;
				}

				// 정점의 인덱스를 구하고,
				int vidx0 = f->GetVertex(0)->m_Idx + 1;
				int vidx1 = f->GetVertex(1)->m_Idx + 1;
				int vidx2 = f->GetVertex(2)->m_Idx + 1;
				int nidx0 = f->GetNormal(0)->m_Idx + 1;
				int nidx1 = f->GetNormal(1)->m_Idx + 1;
				int nidx2 = f->GetNormal(2)->m_Idx + 1;

				if (f->GetTexel(0) != NULL)
				{
					int tidx0 = f->GetTexel(0)->m_Idx + 1;
					int tidx1 = f->GetTexel(1)->m_Idx + 1;
					int tidx2 = f->GetTexel(2)->m_Idx + 1;
					fprintf_s(fp, "%s %d/%d/%d %d/%d/%d %d/%d/%d\n", "f",
						vidx0, tidx0, nidx0, vidx1, tidx1, nidx1, vidx2, tidx2, nidx2);
				}
				else
					fprintf_s(fp, "%s %d//%d %d//%d %d//%d\n", "f",
						vidx0, nidx0, vidx1, nidx1, vidx2, nidx2);
			}
			fprintf(fp, "\n");
		}
	}

	// 파일을 닫는다.
	fclose(fp);
	printf("Exporting OBJ mesh complete...\n\n");

	// 원래 작업 디렉터리로 복귀한다.
	_chdir(cwd);

	return true;
}

/*!
*	\brief 재질 정보를 파일(*.mtl)에 저장한다.
*
*	\param pMesh 저장할 메쉬의 포인터
*	\param fname 파일의 이름
*
*	\return 성공: true, 실패: flase
*/
bool export_obj_mtl(EgMesh *pMesh, const char *fname)
{
	// 파일을 열고 실패하면 false를 반환한다.
	FILE *fp;
	fopen_s(&fp, fname, "w");
	if (!fp)
		return false;

	// 파일의 생성 정보 출력하고,
	fprintf(fp, "%s", "# Wavefront MTL exported by EgForm\n");
	fprintf(fp, "%s", "# http://gaia.dongguk.edu \n\n");

	// 각각의 재질에 대하여,
	for (EgMaterial *pMtrl : pMesh->m_pMaterials)
	{
		// 재질의 정보를 출력하고,
		fprintf(fp, "newmtl %s\n", pMtrl->m_Name.c_str());
		fprintf(fp, "Ka %f %f %f\n", pMtrl->m_Ambient[0], pMtrl->m_Ambient[1], pMtrl->m_Ambient[2]);
		fprintf(fp, "Kd %f %f %f\n", pMtrl->m_Diffuse[0], pMtrl->m_Diffuse[1], pMtrl->m_Diffuse[2]);
		fprintf(fp, "Ks %f %f %f\n", pMtrl->m_Specular[0], pMtrl->m_Specular[1], pMtrl->m_Specular[2]);

		// 텍스처가 있다면 파일명을 출력한다.
		if (pMtrl->m_pTexture != NULL)
			fprintf(fp, "map_Kd %s\n", pMtrl->m_pTexture->m_FileName.c_str());
		fprintf(fp, "\n");
	}

	// 파일을 닫는다.
	fclose(fp);

	return true;
}
