#pragma once

#include "..\usr\include\EgMesh\egmesh.h"
#include <cstring>
#include <vector>
#include <map>
#include <direct.h>

static bool import_obj_mtl(EgMesh *pMesh, const char *fname);
static bool export_obj_mtl(EgMesh *pMesh, const char *fname);


/*!
*	\brief ��θ��� ������ ���ϸ�(Ȯ��� ����)�� ��ȯ�Ѵ�.
*
*	\param full_path ��θ��� ������ ������ �̸�
*
*	\return \a full_path���� ��θ��� ������ ���ϸ�(Ȯ���)�� ��ȯ�Ѵ�.
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
*	\brief ��θ��� ������ ���ϸ�(Ȯ��� ������)�� ��ȯ�Ѵ�.
*
*	\param full_path ��θ��� ������ ������ �̸�
*
*	\return \a full_path���� ��θ��� ������ ���ϸ�(Ȯ��� ������)�� ��ȯ�Ѵ�.
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
*	\brief ������ Ȯ���ڸ� ��ȯ�Ѵ�.
*
*	\param full_path ��θ��� ������ ������ �̸�
*
*	\return \a full_path���� ���� Ȯ���ڸ� ��ȯ�Ѵ�.
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
*	\brief ������ ���丮 ��θ��� ��ȯ�Ѵ�.
*
*	\param full_path ��θ��� ������ ������ �̸�
*
*	\return \a full_path���� ���丮 ��θ��� ��ȯ�Ѵ�.
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
*	\brief �׷����� ���е� �ﰢ���� ������ �迭�� ���Ѵ�.
*
*	\param pMesh ��� �޽��� ���� ������
*	\param GroupedFaces �׷������ ���е� �ﰢ�� �����Ͱ� ����ȴ�.
*
*	\return �׷��� ������ ��ȯ�Ѵ�.
*/
int get_grouped_faces(EgMesh *pMesh, std::map<std::string, std::vector<EgFace *>> &GroupedFaces)
{
	// ���� �ʱ�ȭ �ϰ�
	GroupedFaces.clear();

	// ������ �ﰢ���� ���Ͽ�
	for (EgFace *f : pMesh->m_pFaces)
	{
		// �ﰢ���� ���� �׷��� �迭�� �ﰢ���� �����͸� �߰��Ѵ�.
		if (f->m_GroupName != "")
			GroupedFaces[f->m_GroupName].push_back(f);
		else
		{
			std::string key("default");
			GroupedFaces[key].push_back(f);
		}
	}

	// �׷��� ������ ��ȯ�Ѵ�.
	return (int)GroupedFaces.size();
}

/*!
*	\brief �ؽ�ó ��ǥ�� 0�� 1���̷� ����ȭ �Ѵ�.
*
*	\param u �ؽ�ó ��ǥ u
*	\param v �ؽ�ó ��ǥ v
*/
void normalize_uv(double &u, double &v)
{
	u = (u > 1.0) ? (u - (int)(u)) : u;
	u = (u < 0.0) ? (u - (int)(u - 1.0)) : u;

	v = (v > 1.0) ? (v - (int)(v)) : v;
	v = (v < 0.0) ? (v - (int)(v - 1.0)) : v;
}

/*!
*	\brief �Ǽ��� ���ڿ��� ��ȯ�Ͽ� ��ȯ�Ѵ�.
*
*	\param v ���ڿ��� ��ȯ�� �Ǽ�
*
*	\return ���ڿ��� ��ȯ�� �Ǽ�
*/
std::string cast_str(double v)
{
	char buffer[128];
	sprintf_s(buffer, "%lf", v);

	return std::string(buffer);
}

/*!
*	\brief OBJ ���Ͽ��� ����ϴ� MTL(����)������ �о� �޽��� �߰��Ѵ�.
*
*	\param pMesh ������ �߰��� �޽��� ������
*	\param fname ���� ������ �̸�(���� �۾� ��ο��� ������� ��η� ǥ����)
*
*	\return ����: true, ����: flase
*/
bool import_obj_mtl(EgMesh *pMesh, const char *fname)
{
	// ������ ���� ������ �����Ѵ�.
	EgMaterial *pMtl = NULL;

	// ������ ����, 
	FILE *fp;
	fopen_s(&fp, fname, "r");
	if (!fp) // �����ϸ� false�� ��ȯ�Ѵ�.
	{
		printf("\tImporting %s failed...\n", fname);
		return false;
	}
	else
		printf("\tImporting %s...\n", fname);

	// ������ ������ �� �ܾ �о�, tag �迭�� �����Ѵ�.
	char tag[256];
	while (fscanf_s(fp, "%s", tag, 256) != EOF)
	{
		// newmtl (new material) �±׶��,
		if (!strcmp(tag, "newmtl"))
		{
			// ������ �̸��� �а�,
			char tmp[256];
			fscanf_s(fp, "%s", tmp, 256);

			// ������ �����Ͽ�, �޽��� ���� ����Ʈ�� �߰��Ѵ�.
			pMtl = new EgMaterial(tmp);
			pMesh->AddMaterial(pMtl);
		}

		// Ka (ambient coefficients) �±׶��,
		if (!strcmp(tag, "Ka"))
		{
			// ambient ������ �о ������ �����Ѵ�.
			GLfloat c[3];
			fscanf_s(fp, "%f%f%f", &c[0], &c[1], &c[2]);
			pMtl->SetAmbient(c[0], c[1], c[2]);
		}

		// Kd (diffuse coefficients) �±׶��,
		if (!strcmp(tag, "Kd"))
		{
			// diffuse ������ �о ������ �����Ѵ�.
			GLfloat c[3];
			fscanf_s(fp, "%f%f%f", &c[0], &c[1], &c[2]);
			pMtl->SetDiffuse(c[0], c[1], c[2]);
		}

		// Ks (specular coefficients) �±׶��,
		if (!strcmp(tag, "Ks"))
		{
			// specular ������ �о ������ �����Ѵ�.
			GLfloat c[3];
			fscanf_s(fp, "%f%f%f", &c[0], &c[1], &c[2]);
			pMtl->SetSpecular(c[0], c[1], c[2]);
		}

		// map_Kd (diffuse texture file) �±׶��,
		if (!strcmp(tag, "map_Kd"))
		{
			// �ؽ�ó ������ �д´�.
			char tex_name[512];
			fscanf_s(fp, "%s", tex_name, 512);

			// �̹� �ε�� �ؽ�ó��� ������ �ؽ�ó�� �����Ѵ�.
			pMtl->m_pTexture = pMesh->GetTexture(tex_name);

			// ���ο� �ؽ�ó���
			if (pMtl->m_pTexture == NULL)
			{
				// ���ο� �ؽ�ó�� �����Ͽ� �޽��� �߰��ϰ�, ������ �����Ѵ�.
				EgTexture *pTexture = ::create_texture(tex_name);
				if (pTexture != NULL)
				{
					pMesh->AddTexture(pTexture);
					pMtl->SetTexture(pTexture);
				}
			}
		}
	}

	// ������ �ݴ´�.
	fclose(fp);
	return true;
}

/*!
*	\brief OBJ ������ ����Ʈ�Ѵ�.
*
*	\param fname �޽� ������ �̸�[in]
*
*	\return ����Ʈ�� �޽��� �����͸� ��ȯ�Ѵ�.
*/
EgMesh *import_obj(const char *fname)
{
	// ������ ���� �����ϸ� false�� ��ȯ�Ѵ�.
	FILE *fp;
	fopen_s(&fp, fname, "r");
	if (!fp)
	{
		printf("Importing %s failed...\n", fname);
		return NULL;
	}
	else
		printf("Importing %s...\n", fname);

	// ���� �۾� ���͸��� ���� ��η� ����Ѵ�.
	char cwd[1024] = { 0 };
	_getcwd(cwd, 1024);
	printf("\tCurrent working directory = %s\n", cwd);

	// �� �˻� ���͸��� �̵��ϰ�, ���� ��η� ����Ѵ�.
	std::string model_dir = get_file_path(fname);
	_chdir(model_dir.c_str());
	char msd[1024] = { 0 };
	_getcwd(msd, 1024);
	printf("\tModel directory = %s\n\n", msd);

	// ������ �̸�(��� �� Ȯ���� ����)�� ���Ͽ� �޽��� �����Ѵ�.
	std::string MeshName = get_file_name(fname);
	EgMesh *pMesh = ::create_mesh(MeshName);
	printf("\tMesh name = %s\n", MeshName.c_str());

	// �ε��� �ʿ��� �ӽ� �������� �����Ѵ�.
	char tag[1024];
	bool bTexture = false;
	bool bNormal = false;
	std::string GroupName;
	EgMaterial *pCurrMtl = NULL;

	// ������ ������ �� �ܾ �����鼭 tag �迭�� �����Ѵ�.
	while (fscanf_s(fp, "%s", tag, 1024) != EOF)
	{
		// mtllib (material library) �±׶��,
		if (!strcmp(tag, "mtllib"))
		{
			// ���� ���ϸ�(�ַ� ��� ���)�� ���Ͽ�
			char mtl_fname[1024];
			fscanf_s(fp, "%s", mtl_fname, 1024);

			// ���� ������ �д´�. 
			import_obj_mtl(pMesh, mtl_fname);
		}

		// v (vertex) �±׶��,
		if (!strcmp(tag, "v"))
		{
			// ��ǥ(x, y, z)�� �о� ������ �����ϰ� �޽��� �߰��Ѵ�.
			double x, y, z;
			fscanf_s(fp, "%lf%lf%lf", &x, &y, &z);
			EgVertex *pVert = ::create_vertex(x, y, z);
			pMesh->AddVertex(pVert);
		}

		// vt (vertex texture) �±׶��,
		if (!strcmp(tag, "vt"))
		{
			// �ؽ�ó ��ǥ(u, v)�� �о� �ؼ��� �����ϰ� �޽��� �߰��Ѵ�.
			bTexture = true;
			double u, v;
			fscanf_s(fp, "%lf%lf", &u, &v);
			EgTexel *pTexel = ::create_texel(u, v);
			pMesh->AddTexel(pTexel);
		}

		// vn (vertex normal) �±׶��,
		if (!strcmp(tag, "vn"))
		{
			// ������ ����(nx, ny, nz)�� �о� ������ �����ϰ� �޽��� �߰��Ѵ�.
			bNormal = true;
			double nx, ny, nz;
			fscanf_s(fp, "%lf%lf%lf", &nx, &ny, &nz);
			EgNormal *pNormal = ::create_normal(nx, ny, nz);
			pMesh->AddNormal(pNormal);
		}

		// g (group) �±׶��,
		if (!strcmp(tag, "g"))
		{
			// �׷��� �̸��� �д´�.
			char grp_name[256];
			fscanf_s(fp, "%s", grp_name, 256);
			GroupName = std::string(grp_name);
		}

		// usemtl (use material) �±׶��,
		if (!strcmp(tag, "usemtl"))
		{
			// �޽��� ���� ����Ʈ���� �ش� ������ ã�´�.
			char mtl_name[256];
			fscanf_s(fp, "%s", mtl_name, 256);
			pCurrMtl = pMesh->GetMaterial(mtl_name);
		}

		// f (face) �±׶��,
		if (!strcmp(tag, "f"))
		{
			// �������� �����ϴ� ����, �ؼ�, �븻�� �ε����� �����ϴ� ������ �����ϰ�,
			std::vector<int> vIdxs, tIdxs, nIdxs;

			// �� ���� �о,
			char line[256];
			fgets(line, 256, fp);
			char *pData = line;

			// �� ���� ��� ���� ������
			while (true)
			{
				// ���ʿ��� ����� �ٺ����� �ǳʶ��,
				while (*pData == ' ' || *pData == '\n')
					pData++;

				// �� ���� ��� �о��ٸ� �ݺ����� ���� ���´�.
				if (!strcmp(pData, ""))
					break;

				// �ε��� ������ buffer�� ���� ��,
				char buffer[256];	// buffer[] = "vidx/tidx/nidx" 
				sscanf_s(pData, "%s", buffer, 256);
				pData += strlen(buffer);

				// �ε��� ������ ������ ������ �����ϰ�
				char sep;
				int vidx, tidx, nidx;

				// ����, �ؼ�, �븻 �ε����� ��� �ִ� ��� (f 1/1/1/ 2/2/2 3/3/3 ...)
				if (bTexture && bNormal)
				{
					// ��� �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d%c%d%c%d", &vidx, &sep, 1, &tidx, &sep, 1, &nidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					tidx = (tidx < 0) ? -tidx : tidx;
					nidx = (nidx < 0) ? -nidx : nidx;
					vIdxs.push_back(vidx - 1);
					tIdxs.push_back(tidx - 1);
					nIdxs.push_back(nidx - 1);
					continue;
				}

				// ������ �ؼ� �ε����� �ִ� ��� (f 1/1 2/2 3/3 ...)
				if (bTexture && !bNormal)
				{
					// ������ �ؼ� �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d%c%d", &vidx, &sep, 1, &tidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					tidx = (tidx < 0) ? -tidx : tidx;
					vIdxs.push_back(vidx - 1);
					tIdxs.push_back(tidx - 1);
					continue;
				}

				// ������ �븻 �ε����� �ִ� ��� (f 1//1 2//2 3//3 ...)
				if (!bTexture && bNormal)
				{
					// ������ �븻 �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d%c%c%d", &vidx, &sep, 1, &sep, 1, &nidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					nidx = (nidx < 0) ? -nidx : nidx;
					vIdxs.push_back(vidx - 1);
					nIdxs.push_back(nidx - 1);
					continue;
				}

				// ������ �ε����� �ִ� ��� (f 1 2 3 ...)
				if (!bTexture && !bNormal)
				{
					// ������ �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d", &vidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					vIdxs.push_back(vidx - 1);
					continue;
				}
			}

			// �������� �ﰢ������ ������ �޽��� �߰��Ѵ�.
			int PolygonSize = (int)vIdxs.size(); // �ﰢ��(PolygonSize = 3), �簢��(PolygonSize = 4), ...
			for (int i = 0; i < PolygonSize - 2; ++i)
			{
				// �ﰢ���� ������ ���ϰ�,
				EgVertex *V[3] = { 0 };
				V[0] = pMesh->GetVert(vIdxs[0]);
				V[1] = pMesh->GetVert(vIdxs[i + 1]);
				V[2] = pMesh->GetVert(vIdxs[i + 2]);

				// �� ������ �ؼ� ������ ���ϰ�,
				EgTexel *T[3] = { 0 };
				if (bTexture)
				{
					T[0] = pMesh->GetTexel(tIdxs[0]);
					T[1] = pMesh->GetTexel(tIdxs[i + 1]);
					T[2] = pMesh->GetTexel(tIdxs[i + 2]);
				}

				// �� ������ ���� ������ ���ϰ�,
				EgNormal *N[3] = { 0 };
				if (bNormal)
				{
					N[0] = pMesh->GetNormal(nIdxs[0]);
					N[1] = pMesh->GetNormal(nIdxs[i + 1]);
					N[2] = pMesh->GetNormal(nIdxs[i + 2]);
				}
				else // ���� ������ ���ٸ� �ﰢ�� �������� ��ü�Ѵ�.
				{
					EgVec3 n = ((V[1]->m_Pos - V[0]->m_Pos) ^ (V[2]->m_Pos - V[0]->m_Pos)).Normalize();
					N[0] = N[1] = N[2] = ::create_normal(n[0], n[1], n[2]);
					pMesh->AddNormal(N[0]);
				}

				// �ﰢ���� �����Ͽ� �޽��� �߰��Ѵ�.
				EgFace *f = new EgFace(V[0], V[1], V[2], T[0], T[1], T[2], N[0], N[1], N[2], pCurrMtl, GroupName);
				pMesh->AddFace(f);
			}
		}
	}

	// ������ �ݴ´�.
	fclose(fp);

	// ���������� �ݴ��� ������ ������ �����Ѵ�.
	pMesh->InitEdgeMate();

	// �޽��� ��� ���ڸ� ����Ѵ�.
	pMesh->UpdateBndBox();

	// �ؽ�ó ������ ���� ���̵� Ÿ���� �����Ѵ�.
	if (bTexture)
		pMesh->m_ShadeType = SHADE_BY_TEXTURE;

	printf("Importing OBJ mesh complete...\n\n");

	// �� �˻� ���͸����� ���� �۾� ���͸��� �����Ѵ�.
	_chdir(cwd);

	// ����Ʈ�� �޽��� �����͸� ��ȯ�Ѵ�.
	return pMesh;
}

/*!
*	\brief �޽��� ����(*.obj)�� �����Ѵ�.
*
*	\param pMesh ������ �޽��� ������
*	\param fname ������ �̸�
*
*	\return ����: true, ����: flase
*/
bool export_obj(EgMesh *pMesh, const char *fname)
{
	// ������ ���� �����ϸ� false�� ��ȯ�Ѵ�.
	FILE *fp;
	fopen_s(&fp, fname, "w");
	if (!fp)
	{
		printf("Exporting OBJ mesh failed...\n");
		return false;
	}
	else
		printf("Exporting OBJ mesh...\n");

	// ���� �۾� ���͸��� ���� ��η� ����Ѵ�.
	char cwd[1024] = { 0 };
	_getcwd(cwd, 1024);
	printf("\tCurrent working directory = %s\n", cwd);

	// �� ���� ���͸��� �̵��ϰ�, ���� ��η� ����Ѵ�.
	std::string model_dir = get_file_path(fname);
	_chdir(model_dir.c_str());
	char msd[1024] = { 0 };
	_getcwd(msd, 1024);
	printf("\tModel directory = %s\n\n", msd);

	// ������ ���� ���� ����ϰ�,
	fprintf_s(fp, "%s", "# Wavefront OBJ exported by EgTools\n");
	fprintf_s(fp, "%s", "# http://gaia.dongguk.edu \n\n");

	// ���� ���̺귯�� ������ ����ϰ�,
	int NumMtrl = pMesh->GetNumMtrl();
	printf("\tNumber of materials = %d\n", NumMtrl);
	if (NumMtrl != 0)
	{
		// ���� ����(*.mtl)�� �̸��� ����ϰ�,
		std::string file_name(fname);
		file_name = get_file_name(fname) + ".mtl";
		fprintf(fp, "mtllib %s\n\n", file_name.c_str());

		// ���� ���� ����(*.mtl)�� �����Ͽ� ����Ѵ�.
		export_obj_mtl(pMesh, file_name.c_str());
	}

	// ����, �ؼ�, ������ �ε����� �����Ѵ�.
	int NumVert = pMesh->UpdateVertIdx();
	int NumTexel = pMesh->UpdateTexIdx();
	int NumNormal = pMesh->UpdateNormalIdx();
	int NumFace = pMesh->UpdateFaceIdx();

	// ������ ������ ����Ѵ�.
	printf("\tNumber of vertices = %d\n", NumVert);
	for (EgVertex *v : pMesh->m_pVerts)
		fprintf(fp, "%s %lf %lf %lf\n", "v", v->m_Pos[0], v->m_Pos[1], v->m_Pos[2]);
	fprintf(fp, "\n");

	// �ؼ��� ������ ����Ѵ�.
	printf("\tNumber of texels = %d\n", NumTexel);
	for (EgTexel *t : pMesh->m_pTexels)
		fprintf(fp, "%s %lf %lf\n", "vt", t->m_UV[0], t->m_UV[1]);
	fprintf(fp, "\n");

	// ������ ������ ����Ѵ�.
	printf("\tNumber of normals = %d\n", NumNormal);
	for (EgNormal *n : pMesh->m_pNormals)
		fprintf(fp, "%s %lf %lf %lf\n", "vn", n->m_Dir[0], n->m_Dir[1], n->m_Dir[2]);
	fprintf(fp, "\n");

	// �ﰢ�� ������ ����Ѵ�.
	printf("\tNumber of faces = %d\n", NumFace);

	// �׷������ ���е� �ﰢ���� ������ �迭�� ��´�.
	std::map<std::string, std::vector<EgFace *>> GroupedFaces;
	int NumGroup = ::get_grouped_faces(pMesh, GroupedFaces);

	// �ﰢ���� �׷� ������ ���ٸ�
	if (NumGroup == 0)
	{
		// ����ô���� ��� �����￡ ���Ͽ�,
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
		// ������ �׷쿡 ���Ͽ�,
		for (it = GroupedFaces.begin(); it != GroupedFaces.end(); ++it)
		{
			// �׷��� �̸��� ����ϰ�,
			std::string GroupName = it->first;
			fprintf(fp, "g %s\n", GroupName.c_str());

			// ���� ������ �ε����� ���� ������ �����ϰ�,
			EgMaterial *pPrevMtrl = NULL;

			// �׷쿡 ���� ������ �����￡ ���Ͽ�,
			int NumFace = (int)it->second.size();
			for (int i = 0; i < NumFace; ++i)
			{
				// �ﰢ�� ������ ���,
				EgFace *f = it->second[i];

				// �ﰢ���� ���� ������ �ִٸ� ���� ���������� ���Ͽ� �ٸ� ��� ����ϰ�,
				if (pPrevMtrl == NULL || f->m_pMaterial != pPrevMtrl)
				{
					if (f->m_pMaterial != NULL)
						fprintf(fp, "usemtl %s\n", f->m_pMaterial->m_Name.c_str());
					pPrevMtrl = f->m_pMaterial;
				}

				// ������ �ε����� ���ϰ�,
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

	// ������ �ݴ´�.
	fclose(fp);
	printf("Exporting OBJ mesh complete...\n\n");

	// ���� �۾� ���͸��� �����Ѵ�.
	_chdir(cwd);

	return true;
}

/*!
*	\brief ���� ������ ����(*.mtl)�� �����Ѵ�.
*
*	\param pMesh ������ �޽��� ������
*	\param fname ������ �̸�
*
*	\return ����: true, ����: flase
*/
bool export_obj_mtl(EgMesh *pMesh, const char *fname)
{
	// ������ ���� �����ϸ� false�� ��ȯ�Ѵ�.
	FILE *fp;
	fopen_s(&fp, fname, "w");
	if (!fp)
		return false;

	// ������ ���� ���� ����ϰ�,
	fprintf(fp, "%s", "# Wavefront MTL exported by EgForm\n");
	fprintf(fp, "%s", "# http://gaia.dongguk.edu \n\n");

	// ������ ������ ���Ͽ�,
	for (EgMaterial *pMtrl : pMesh->m_pMaterials)
	{
		// ������ ������ ����ϰ�,
		fprintf(fp, "newmtl %s\n", pMtrl->m_Name.c_str());
		fprintf(fp, "Ka %f %f %f\n", pMtrl->m_Ambient[0], pMtrl->m_Ambient[1], pMtrl->m_Ambient[2]);
		fprintf(fp, "Kd %f %f %f\n", pMtrl->m_Diffuse[0], pMtrl->m_Diffuse[1], pMtrl->m_Diffuse[2]);
		fprintf(fp, "Ks %f %f %f\n", pMtrl->m_Specular[0], pMtrl->m_Specular[1], pMtrl->m_Specular[2]);

		// �ؽ�ó�� �ִٸ� ���ϸ��� ����Ѵ�.
		if (pMtrl->m_pTexture != NULL)
			fprintf(fp, "map_Kd %s\n", pMtrl->m_pTexture->m_FileName.c_str());
		fprintf(fp, "\n");
	}

	// ������ �ݴ´�.
	fclose(fp);

	return true;
}
