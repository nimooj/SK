#include "pch.h"

#include "SizeKorea.h"

#include "loadfile.h"
#include <conio.h>
#include <process.h>
#include "optimize.h"
#include "OBJdll.h"

#include <errno.h>

bool read_iv_asc( CString strFileName, 
				  GVector3f **m_Vertex, 
			      GVector3f **m_Color, 
			      GVector3f **m_Normal,
			      int **m_FaceIdx, 
			      int *m_vNum, 
			      int *m_iNum,
				  float *m_MaxY, 
				  float *m_MinY,
				  float *m_MaxX,
				  float *m_MinX)
{
	/* open file */
	FILE *stream;
	fopen_s(&stream, CT2A(strFileName), "r");

	/* declararion of local variables */
	int vnum = 0;
	int inum = 0;
	char line_buffer[1024];
	
	/* move file pointer to starting position of vertex list */
	while( true )
	{
		fgets( line_buffer, 1024, stream );
		CString str( line_buffer );
		str.TrimLeft();
		if ( str == "[\n" )
			break;
	}
	
	/* count the vertex number */
	while( true )
	{
		fgets( line_buffer, 1024, stream );
		CString str( line_buffer );
		str.TrimLeft();
		if ( str == "]\n" )
			break;
		vnum++;
	}

	/* move file pointer to the starting position of index list */
	while( true )
	{
		fgets( line_buffer, 1024, stream );
		CString str( line_buffer );
		str.TrimLeft();
		if ( str == "[\n" )
			break;
	}

	/* count the index number */
	while( true )
	{
		fgets( line_buffer, 1024, stream );
		CString str( line_buffer );
		str.TrimLeft();
		if ( str == "]\n" )
			break;
		inum++;
	}
	inum *=3;

	/* alloc memory for class member variables */
	*m_Vertex = new GVector3f[vnum];
	*m_FaceIdx = new int[inum];

	/* move file pointer to beginning of file */
	fseek( stream, 0L, SEEK_SET );

	/* move file pointer to starting position of vertex list */
	while( true )
	{
		fgets( line_buffer, 1024, stream );
		CString str( line_buffer );
		str.TrimLeft();
		if ( str == "[\n" )
			break;
	}
		
	/* to calculate the position of progress control. */
	// mj::Get min-max vals
	// mj::flag for scaling and rotating
	bool needScaling = false;
	float minX = 100000, maxX = -100000;
	float minY = 100000, maxY = -100000;
	float minZ = 100000, maxZ = -100000;

	for ( int i = 0; i < vnum; i++ )
	{
		float x, y, z;
		fgets( line_buffer, 1024, stream );
		sscanf_s(line_buffer, "%f %f %f", &x, &y, &z );

		// mj::Check scale
		if ( y > 10000) {
			needScaling = true;
		}

		// mj::Set min-max
		if (x < minX) {
			minX = x;
		}
		if (x > maxX) {
			maxX = x;
		}
		if (y < minY) {
			minY = y;
		}
		if (y > maxY) {
			maxY = y;
		}
		if (z < minZ) {
			minZ = z;
		}
		if (z > maxZ) {
			maxZ = z;
		}

		(*m_Vertex)[i].setVector( x, y, z );
	}

	// mj::Perform scaling
	if (needScaling) {
		for (int i = 0; i < vnum; i++) {
			(*m_Vertex)[i].X /= 10;
			(*m_Vertex)[i].Y /= 10;
			(*m_Vertex)[i].Z /= 10;
		}
	}

	// mj::Perform rotation
	// mj::X폭보다 Z폭이 클 경우...? 배가 너어어어무 많이 나와서 어깨폭보다 큰 사람은 제외되게 됨
	// mj::Y축 기준으로 회전하는데...오른쪽회전?왼쪽회전? -> 일단 오른쪽 회전
	if (abs(maxZ - minZ) > abs(maxX - maxX)) {
		for (int i = 0; i < vnum; i++) {
			float rad = 90 * M_PI / 180;
			double x = (*m_Vertex)[i].X;
			double z = (*m_Vertex)[i].Z;

			(*m_Vertex)[i].X = cos(rad) * x + sin(rad) * z;
			(*m_Vertex)[i].Z = -sin(rad) * x + cos(rad) * x;
		}
	}


	/* move file pointer to the starting position of vertex list */
	while( true )
	{
		fgets( line_buffer, 1024, stream );
		CString str( line_buffer );
		str.TrimLeft();
		if ( str == "[\n" )
			break;
	}
	
	/* read index list and store at m_FaceIdx */
	for  ( int i = 0; i < inum; i+=3 )
	{
		int idx0, idx1, idx2;
		fgets( line_buffer, 1024, stream );
		sscanf_s( line_buffer, "%d,%d,%d", &idx0, &idx1, &idx2 );
		(*m_FaceIdx)[i] = idx0;
		(*m_FaceIdx)[i+1] = idx1;
		(*m_FaceIdx)[i+2] = idx2;
	}
	/* close file pointer */
	fclose( stream );

	// Computation of Normal Value
	GVector3f *tmp = new GVector3f[vnum];

	// Vertex Normal Data Array
	*m_Normal = new GVector3f[vnum];

	// Normal Value Summing up : by Face Traversing
	for ( int i = 0; i < inum; i+=3 )
	{
		GVector3f b0 = (*m_Vertex)[(*m_FaceIdx)[i]];
		GVector3f b1 = (*m_Vertex)[(*m_FaceIdx)[i+1]];
		GVector3f b2 = (*m_Vertex)[(*m_FaceIdx)[i+2]];

		// Triangle Plane Normals : Crossproduct of 2 Edges
		GVector3f v1, v2, c;
		v1 = b1 - b0;	// Vector function		
		v2 = b2 - b0;	// Cross function
		c  = v1^v2;

		// Contributing the computed face normals to 
		// final vertex normals
		tmp[(*m_FaceIdx)[i]]	 += c;
		tmp[(*m_FaceIdx)[i+1]] += c;
		tmp[(*m_FaceIdx)[i+2]] += c;
	}	
	// Normalization of Added Value Averaging Routine : Face Traversing
	for ( int i = 0; i < vnum; i++ )
	{
		tmp[i].normalize();
		(*m_Normal)[i] = tmp[i];
	}
	delete[] tmp;

	*m_vNum = vnum;
	*m_iNum = inum;
	return true;
}

bool read_obj_asc(	CString strFileName, 
					GVector3f **m_Vertex, 
					GVector3f **m_Color, 
					GVector3f **m_Normal,
					int **m_FaceIdx, 
					int *m_vNum, 
					int *m_iNum,
					float *m_MaxY,
					float *m_MinY,
					float *m_MaxX,
					float *m_MinX
					)
{	
	HINSTANCE b = LoadLibrary( _T("OBJDll.dll") );
	if( !b )
	{
		AfxMessageBox(_T("해당 dll 로딩실패"));
		return false;
	}
	
	 //PTEST            (*pFunc)(); 
	 typedef int ( __stdcall *ImportOBJ_FN_Type )( char	*FileName,
                        int *nNode,
                        float **ppNodes,
                        float **ppColor,
                        float **ppNormal,
                        int *nElement,
                        int **ppElements ); 
	ImportOBJ_FN_Type ImportOBJ;

	ImportOBJ =(ImportOBJ_FN_Type)GetProcAddress(b,"ImportOBJ"); 
	
	 char *FN =(char*) CT2A(strFileName);

	 float* ppNodes;
     float* ppColor;
     float* ppNormal;
	 int* ppElems;
	 int vNum, iNum;
	 ppNodes = ppColor =ppNormal =NULL;
	 ppElems =NULL;

	ImportOBJ(FN, &vNum, &ppNodes, &ppColor, &ppNormal, &iNum, &ppElems);
	*m_vNum=vNum;
	*m_iNum=iNum*3;
	float min_y = 10000.0, max_y = -10000.0;
	float min_x = 10000.0, max_x = -10000.0;
	float min_z = 10000.0, max_z = -10000.0;

	*m_Vertex = new GVector3f[vNum];
	*m_Color = new GVector3f[vNum];
	*m_FaceIdx = new int[iNum*3];	


	for (int k = 0; k< vNum; k++ )
	{
	  (*m_Vertex)[k][0] = ppNodes[k*3];  
	  (*m_Vertex)[k][1] = ppNodes[k*3+1];
	  (*m_Vertex)[k][2] = ppNodes[k*3+2];


	  min_x = MIN( min_x, (*m_Vertex)[k][0] );
      max_x = MAX( max_x, (*m_Vertex)[k][0] );

	  min_y = MIN( min_y, (*m_Vertex)[k][1] );
	  max_y = MAX( max_y, (*m_Vertex)[k][1] );

	  min_z = MIN( min_z, (*m_Vertex)[k][2] );
	  max_z = MAX( max_z, (*m_Vertex)[k][2] );
	}

	if(ppColor !=NULL)
	{	
		for (int k = 0; k< vNum; k++ )
		{
			(*m_Color)[k][0] = ppColor[k*3];  
			(*m_Color)[k][1] = ppColor[k*3+1];
			(*m_Color)[k][2] = ppColor[k*3+2];		
		}
	}

		// Vertex Normal Data Array
	*m_Normal = new GVector3f[vNum];
		
	if(ppNormal !=NULL)
	{	
		for (int k = 0; k< vNum; k++ )
		{
			(*m_Normal)[k][0] = ppNormal[k*3];  
			(*m_Normal)[k][1] = ppNormal[k*3+1];
			(*m_Normal)[k][2] = ppNormal[k*3+2];		
		}
	}

    int iNum3 = iNum*3;
	for (int j = 0; j < iNum3; j++ )
	{
	  (*m_FaceIdx)[j] = *ppElems++;
	}


	// Computation of Normal Value
	GVector3f *tmp = new GVector3f[vNum];

	// Normal Value Summing up : by Face Traversing
	for (int i = 0; i < iNum3; i+=3 )
	{
		GVector3f b0 = (*m_Vertex)[(*m_FaceIdx)[i]];
		GVector3f b1 = (*m_Vertex)[(*m_FaceIdx)[i+1]];
		GVector3f b2 = (*m_Vertex)[(*m_FaceIdx)[i+2]];

		// Triangle Plane Normals : Crossproduct of 2 Edges
		GVector3f v1, v2, c;
		v1 = b1 - b0;	// Vector function		
		v2 = b2 - b0;	// Cross function
		c  = v1^v2;

		// Contributing the computed face normals to 
		// final vertex normals
		tmp[(*m_FaceIdx)[i]]   += c;
		tmp[(*m_FaceIdx)[i+1]] += c;
		tmp[(*m_FaceIdx)[i+2]] += c;
	}	
	// Normalization of Added Value Averaging Routine : Face Traversing
	for ( int i = 0; i < vNum; i++ )
	{
		tmp[i].normalize();
		(*m_Normal)[i] = tmp[i];
	}
	delete[] tmp;

	*m_MaxY = max_y;
	*m_MinY = min_y;
	*m_MaxX = max_x;
	*m_MinX = min_x;

	return 1;

}


bool read_ply_asc(CString strFileName,
	GVector3f **m_Vertex,
	GVector3f **m_Color,
	GVector3f **m_Normal,
	int **m_FaceIdx,
	int *m_vNum,
	int *m_iNum,
	float *m_MaxY,
	float *m_MinY,
	float *m_MaxX,
	float *m_MinX
)
{
	// 파일을 오픈한다.
	FILE *fp;
	fopen_s(&fp, strFileName, "r");
	if (!fp)
	{
		_cprintf("Ply file open failed...\n");
		return false;
	}

	// 파일 헤더를 읽는다.
	int NumVert = 0, NumFace = 0;
	char buffer[1024];
	while (fscanf_s(fp, "%s", buffer, 1024) != EOF)
	{
		// element 태그라면,
		if (!strcmp(buffer, "element"))
		{
			// 다음 태그를 읽는다.
			fscanf_s(fp, "%s", buffer, 1024);

			// vertex 태그라면,
			if (!strcmp(buffer, "vertex"))
				fscanf_s(fp, "%d", &NumVert);

			// face 태그라면,
			if (!strcmp(buffer, "face"))
				fscanf_s(fp, "%d", &NumFace);
		}
		// end_header 태그라면
		if (!strcmp(buffer, "end_header"))
			break;
	}

	// 메쉬 데이터를 동적할당한다.
	*m_Vertex = new GVector3f[NumVert];
	*m_Color = new GVector3f[NumVert];
	*m_Normal = new GVector3f[NumVert];
	*m_FaceIdx = new int[NumFace * 3];

	// 정점의 좌표를 읽는다.
	float minX = INFINITY, minY = INFINITY, minZ = INFINITY;
	float maxX = -INFINITY, maxY = -INFINITY, maxZ = -INFINITY;
	for (int i = 0; i < NumVert; ++i)
	{
		float x, y, z;
		fscanf_s(fp, "%f%f%f", &x, &y, &z);
		(*m_Vertex)[i].setVector(x, y, z);
		(*m_Color)[i].setVector(0.5f, 0.5f, 0.5f);
		minX = MIN(minX, x);
		minY = MIN(minY, y);
		maxX = MAX(maxX, x);
		maxY = MAX(maxY, y);
	}

	// 삼각형의 인덱스를 읽는다.
	for (int i = 0; i < NumFace; ++i)
	{
		int tmp, idx0, idx1, idx2;
		fscanf_s(fp, "%d%d%d%d", &tmp, &idx0, &idx1, &idx2);
		(*m_FaceIdx)[3 * i] = idx0;
		(*m_FaceIdx)[3 * i + 1] = idx1;
		(*m_FaceIdx)[3 * i + 2] = idx2;
	}

	// 법선을 계산한다.
	for (int i = 0; i < NumFace * 3; i += 3)
	{
		GVector3f b0 = (*m_Vertex)[(*m_FaceIdx)[i]];
		GVector3f b1 = (*m_Vertex)[(*m_FaceIdx)[i + 1]];
		GVector3f b2 = (*m_Vertex)[(*m_FaceIdx)[i + 2]];
		GVector3f v1, v2, c;
		v1 = b1 - b0;   // Vector function      
		v2 = b2 - b0;   // Cross function
		c = v1 ^ v2;
		(*m_Normal)[(*m_FaceIdx)[i]] += c;
		(*m_Normal)[(*m_FaceIdx)[i + 1]] += c;
		(*m_Normal)[(*m_FaceIdx)[i + 2]] += c;
	}

	// 법선을 정규화 한다.
	for (int i = 0; i < NumVert; i++)
		(*m_Normal)[i].normalize();

	*m_vNum = NumVert;
	*m_iNum = NumFace * 3;
	*m_MinX = minX;
	*m_MaxX = maxX;
	*m_MinY = minY;
	*m_MaxY = maxY;
	return true;
}

bool read_txt(	CString strFileName, 
					GVector3f **m_Vertex, 
					GVector3f **m_Color, 
					GVector3f **m_Normal,
					int **m_FaceIdx, 
					int *m_vNum, 
					int *m_iNum,
					float *m_MaxY,
					float *m_MinY,
					float *m_MaxX,
					float *m_MinX
					)
{
	/* open file */
	FILE *stream;
	fopen_s(&stream, CT2A(strFileName), "r");

	/* declararion of local variables */
	int vnum = 0;
	int inum = 0;
	char line_buffer[1024];

	float min_y = 10000.0, max_y = -10000.0;
	float min_x = 10000.0, max_x = -10000.0;

	fgets( line_buffer, 1024, stream );
	sscanf_s( line_buffer, "%d", &vnum );

	*m_Vertex = new GVector3f[vnum];
	*m_Color = new GVector3f[vnum];

	/* to calculate the position of progress control. */
	for ( int i = 0; i < vnum; i++ )
	{
		float x, y, z;
		fgets( line_buffer, 1024, stream );
	//	sscanf( line_buffer, "%lf, %s, %lf, %s %lf", &x, &y, &z );
		//sscanf( line_buffer, "%lf,%lf,%lf", &x, &y, &z );
		sscanf_s( line_buffer, "%f,%f,%f", &x, &y, &z );
		(*m_Vertex)[i].setVector( x, y, z );
	}

	int fnum;
	fgets(line_buffer, 1024, stream );
	sscanf_s(line_buffer, "%i", &fnum );

    inum=fnum*3;
	*m_FaceIdx = new int[inum];
	for  ( int i = 0; i < inum; i+=3 )
	{
		int idx0, idx1, idx2;
		fgets( line_buffer, 1024, stream );
		sscanf_s( line_buffer, "%d,%d,%d", &idx0, &idx1, &idx2 );
		(*m_FaceIdx)[i] = idx0;
		(*m_FaceIdx)[i+1] = idx1;
		(*m_FaceIdx)[i+2] = idx2;
	}

	//----------------------------------

	// Computation of Normal Value
	GVector3f *tmp = new GVector3f[vnum];

	// Vertex Normal Data Array
	*m_Normal = new GVector3f[vnum];

	// Normal Value Summing up : by Face Traversing

	for ( int i = 0; i < vnum; i++ )
	{
		tmp[i] = GVector3f(0.0f, 0.0f, 0.0f);		
	}

	for ( int i = 0; i < fnum; i+=3 )
	{
		GVector3f b0 = (*m_Vertex)[(*m_FaceIdx)[i]];
		GVector3f b1 = (*m_Vertex)[(*m_FaceIdx)[i+1]];
		GVector3f b2 = (*m_Vertex)[(*m_FaceIdx)[i+2]];

		// Triangle Plane Normals : Crossproduct of 2 Edges
		GVector3f v1, v2, c;
		v1 = b1 - b0;	// Vector function		
		v2 = b2 - b0;	// Cross function
		c  = v1^v2;

		// Contributing the computed face normals to 
		// final vertex normals
		tmp[(*m_FaceIdx)[i]]   += c;
		tmp[(*m_FaceIdx)[i+1]] += c;
		tmp[(*m_FaceIdx)[i+2]] += c;
	}	
	// Normalization of Added Value Averaging Routine : Face Traversing
	for ( int i = 0; i < vnum; i++ )
	{
		tmp[i].normalize();
		(*m_Normal)[i] = tmp[i];
	}
	delete[] tmp;

	*m_iNum = inum;
	*m_vNum = vnum;
//	*m_iNum = fnumNew;
	*m_MaxY = max_y;
	*m_MinY = min_y;
	*m_MaxX = max_x;
	*m_MinX = min_x;

	return true;
}

bool write_ply_asc( CString strFileName, 
				    GVector3f *m_Vertex,
					GVector3f *m_Color,
					int *m_FaceIdx, 
					int m_vNum,
					int m_iNum )
{
	FILE *fp;
	fopen_s(&fp, CT2A(strFileName), "w" );
	fprintf( fp, "ply\n" );
	fprintf( fp, "format ascii 1.0\n" );
	fprintf( fp, "comment Exported by 3DM\n" );
	fprintf( fp, "element vertex %d\n", m_vNum );
	fprintf( fp, "property float x\n" );
	fprintf( fp, "property float y\n" );
	fprintf( fp, "property float z\n" );
	fprintf( fp, "property uchar red\n" );
	fprintf( fp, "property uchar green\n" );
	fprintf( fp, "property uchar blue\n" );
	fprintf( fp, "element face %d\n", m_iNum/3 );
	fprintf( fp, "property list uchar int vertex_index\n" );
	fprintf( fp, "end_header\n" );
	for ( int i = 0; i < m_vNum; i++ )
	{
		if ( m_Color != NULL )
		{

			fprintf( fp, "%f %f %f %d %d %d\n", m_Vertex[i][0], m_Vertex[i][1], m_Vertex[i][2], 
											  (int)(m_Color[i][0]*255), (int)(m_Color[i][1]*255), (int)(m_Color[i][2]*255) );
		}
		else
		{
			fprintf( fp, "%f %f %f %d %d %d\n", m_Vertex[i][0], m_Vertex[i][1], m_Vertex[i][2], 100, 100, 100 );
		}
	}
	for ( int i = 0; i < m_iNum; i+=3 )
	{
		fprintf( fp, "%d %d %d %d\n", 3, m_FaceIdx[i], m_FaceIdx[i+1], m_FaceIdx[i+2] );
	}

	fclose( fp );


	
	return true;

}

//add_ysj
bool read_msh(CString strFileName, 
					GVector3f **m_Vertex, 
					GVector3f **m_Color, 
					GVector3f **m_Normal,
					int **m_FaceIdx, 
					int *m_vNum, 
					int *m_iNum )
{

	//float x,y,z;
	//int vnum =0;
	//int inum =0;

	//FILE *fp;	
	//		
	///* open file */
	//if( (fp=fopen(CT2A(strFileName),"rt"))==NULL)
	//{
	//	printf("Error - cannot open msh file\n");
	//	return false;
	//}

	//fscanf(fp,"%d %d",&vnum,&inum);
	//inum *= 3;

	//*m_Vertex = new GVector3f[vnum];
	//*m_FaceIdx = new int[inum];


	//for(int i=0;i< vnum;i++)
	//{
	//	fscanf(fp,"%f %f %f",&x,&y,&z);
	//	(*m_Vertex)[i].X=x;
	//	(*m_Vertex)[i].Y=y;
	//	(*m_Vertex)[i].Z=z;	
	//}
	//
	///* read index list and store at m_pIndexList */
	//for(int i=0;i < inum; i+=3)
	//{
	//   	fscanf(fp,"%d %d %d",&(*m_FaceIdx)[i+0],&(*m_FaceIdx)[i+1],&(*m_FaceIdx)[i+2]);	
	//}
	//fclose(fp);

	////Computation of Normal Value
	//GVector3f * tmp  = new GVector3f[vnum];
	//
	////Vertex Normal Data Array
	//*m_Normal = new GVector3f[vnum];

	//// Normal Value Summing up : by Face Traversing
	//for ( int i = 0; i < inum; i+=3 )
	//{
	//	GVector3f b0 = (*m_Vertex)[(*m_FaceIdx)[i]];
	//	GVector3f b1 = (*m_Vertex)[(*m_FaceIdx)[i+1]];
	//	GVector3f b2 = (*m_Vertex)[(*m_FaceIdx)[i+2]];

	//	// Triangle Plane Normals : Crossproduct of 2 Edges
	//	GVector3f v1, v2, c;
	//	v1 = b1 - b0;	// Vector function		
	//	v2 = b2 - b0;	// Cross function
	//	c  = v1^v2;

	//	// Contributing the computed face normals to 
	//	// final vertex normals
	//	tmp[(*m_FaceIdx)[i]]	 += c;
	//	tmp[(*m_FaceIdx)[i+1]] += c;
	//	tmp[(*m_FaceIdx)[i+2]] += c;
	//}	
	//// Normalization of Added Value Averaging Routine : Face Traversing
	//for ( int i = 0; i < vnum; i++ )
	//{
	//	tmp[i].normalize();
	//	(*m_Normal)[i] = tmp[i];
	//}
	//delete[] tmp;

	//*m_vNum = vnum;
	//*m_iNum = inum;
	//fclose(fp);
	return true;
}

bool read_vrml(CString strFileName, 
					GVector3f **m_Vertex, 
					GVector3f **m_Color, 
					GVector3f **m_Normal,
					int **m_FaceIdx, 
					int *m_vNum, 
					int *m_iNum,
					float *m_MaxY, 
				   float *m_MinY,
				   float *m_MaxX,
				   float *m_MinX)
{
////////////////////////////////////////////////////////////
	char	pStart = '[';
	char	pEnd = ']';
	char	st[80];
	// 1. open the file with read option.
	char	*file_buffer;
	// file size.
	long		nFileSize; 
	int m_nNumPt, m_nNumIndex, inum, vnum;
	CFile	BodyDataFile;
	if( !BodyDataFile.Open( strFileName, CFile::modeRead ))
		return false;
				
	// get file size.
	nFileSize	=	BodyDataFile.GetLength();
	long		read_size;
	long		readable_size;
	readable_size		=	nFileSize;
	file_buffer			=	new char[ readable_size ];
	long buffer_index	=	0;
	// read_size			=	BodyDataFile.ReadHuge( file_buffer, readable_size ); // mj::
	read_size			=	BodyDataFile.Read( file_buffer, readable_size );

	long nNumIndex=0;
	long m_nVertexCount=0;
	long m_nTriangleCount =0;
	// 2. move the first start polong Coordinate3  "["
	while( file_buffer[buffer_index] != pStart )
		buffer_index++;

	long First_Start_Index	=	buffer_index;
	// 3. count the number of polongs.

	while( file_buffer[buffer_index] != pEnd )
	{
		buffer_index++;
		if( file_buffer[buffer_index] == '\n' )
			m_nVertexCount ++;
	}
	m_nVertexCount--;
	//cerr << "vertex Count=" << m_nVertexCount<<endl;
	// 4. move the second start polong CoordIndex "["

	while( file_buffer[buffer_index] != pStart )
		buffer_index++;
	long Second_Start_Index = buffer_index;
	
	// 5. count the number of indexs.

	while( file_buffer[buffer_index] != pEnd )
	{
		buffer_index++;
		if( file_buffer[buffer_index] == '-' )
			nNumIndex ++;
	}
	m_nTriangleCount=nNumIndex;

	
	// 6. specify the size of array.
	m_nNumPt = m_nVertexCount;
	m_nNumIndex = nNumIndex *3;
	
	*m_vNum = m_nNumPt;
	*m_iNum = m_nNumIndex;

	vnum = *m_vNum;
	inum = *m_iNum;
	
	
	*m_Vertex = new GVector3f[m_nNumPt];
	*m_FaceIdx = new int[m_nNumIndex];

	nNumIndex *= 4;
	
	
	// 7. move the file polonger to the head of file.
		/* read vertex position and store at m_pPoints */
	/* to calculate the position of progress control. */
	

	// 8. move the first start polong.

	buffer_index	=	First_Start_Index;
	buffer_index++;

	long		buffer_preindex, st_index;
	float x_value,y_value,z_value;


	for ( long l = 0; l < m_nNumPt ; l ++ )
	{
		
		for (long j = 0; j < 3 ; j++ )
		{

			buffer_index++;
			buffer_preindex = buffer_index;

			if( file_buffer[buffer_index] == pEnd ) break;

			//buffer_index++;
			if( file_buffer[buffer_index] == pEnd ) break;
			while( file_buffer[buffer_index] == '\n'||file_buffer[buffer_index] == '\t'||file_buffer[buffer_index] == '\r' || file_buffer[buffer_index] == ' '||file_buffer[buffer_index] == ',' )
				buffer_index++;				

			st_index = 0;
			while( file_buffer[buffer_index] != '\n' && file_buffer[buffer_index] != ' '&&file_buffer[buffer_index] != ',' ){
				st[st_index] = file_buffer[buffer_index];
				st_index++;
				buffer_index++;
			}
			st[st_index] = '\0';

			if(j == 0)
				x_value = (float)atof( st );
			if(j == 1)
				y_value = (float)atof( st );
			if(j == 2)
				z_value = (float)atof( st );

		}
		if( file_buffer[buffer_index] == pEnd ) break;
		(*m_Vertex)[ l ].X=x_value;
		(*m_Vertex)[ l ].Y=y_value;
		(*m_Vertex)[ l ].Z=z_value;
		
	}

	

	// 9. move the second start polong. i.e CoordIndex "["
//	buffer_index = Second_Start_Index;
//	buffer_index++;

	/* read index list and store at m_pIndexList */
		
	long index=0;
	while( file_buffer[buffer_index] != '[' )
			buffer_index++;
	buffer_index++;	

	for ( int l = 0; l < nNumIndex; l ++ )
	{

					
		//buffer_index++;
		if( file_buffer[buffer_index] == pEnd ) break;
		while( file_buffer[buffer_index] == '\n'||file_buffer[buffer_index] == '\t'||file_buffer[buffer_index] == '\r' || file_buffer[buffer_index] == ' '||file_buffer[buffer_index] == ',' )
			buffer_index++;				

		st_index = 0;
		while( file_buffer[buffer_index] != '\n' && file_buffer[buffer_index] != ' '&&file_buffer[buffer_index] != ',' ){
			st[st_index] = file_buffer[buffer_index];
			st_index++;
			buffer_index++;
		}
		st[st_index] = '\0';		
	
		if( (l%4) !=3) {(*m_FaceIdx)[index] = atoi(st);index++;	}
			
	} 
	
	delete [] file_buffer;	

	//Computation of Normal Value
	GVector3f * tmp  = new GVector3f[vnum];
	
	//Vertex Normal Data Array
	*m_Normal = new GVector3f[vnum];

	// Normal Value Summing up : by Face Traversing
	for (int  i = 0; i < inum; i+=3 )
	{
		GVector3f b0 = (*m_Vertex)[(*m_FaceIdx)[i]];
		GVector3f b1 = (*m_Vertex)[(*m_FaceIdx)[i+1]];
		GVector3f b2 = (*m_Vertex)[(*m_FaceIdx)[i+2]];

		// Triangle Plane Normals : Crossproduct of 2 Edges
		GVector3f v1, v2, c;
		v1 = b1 - b0;	// Vector function		
		v2 = b2 - b0;	// Cross function
		c  = v1^v2;

		// Contributing the computed face normals to 
		// final vertex normals
		tmp[(*m_FaceIdx)[i]]	 += c;
		tmp[(*m_FaceIdx)[i+1]] += c;
		tmp[(*m_FaceIdx)[i+2]] += c;
	}	
	// Normalization of Added Value Averaging Routine : Face Traversing
	for ( int i = 0; i < vnum; i++ )
	{
		tmp[i].normalize();
		(*m_Normal)[i] = tmp[i];
	}
	delete[] tmp;

	*m_vNum = vnum;
	*m_iNum = inum;

	BodyDataFile.Close();
	return true;	
}

bool read_wrb(CString strFileName, 
					GVector3f **m_Vertex, 
					GVector3f **m_Color, 
					GVector3f **m_Normal,
					int **m_FaceIdx, 
					int *m_vNum, 
					int *m_iNum,
					float *m_MaxY,
					float *m_MinY,
					float *m_MaxX,
					float *m_MinX
					)
{	
	int pId, termstat, len;
	CString fileName1 = strFileName; //*.wrb	
	
	CString fileName2 = strFileName; //*.wrl			
	len = fileName2.GetLength();
	fileName2.SetAt(len-1,'l');		
	
	//1. decompress wrb file	
	CString tmps(_OPTIMIZER_PATH_);
	tmps+="vcbf_bin.exe";

	const char* tmpscc = CT2A(tmps);
	const char* fileName1cc = CT2A(fileName1);
	pId = _spawnl( _P_NOWAIT, tmpscc, tmpscc, fileName1cc, NULL );	 		
	_cwait( &termstat, pId, _WAIT_CHILD );	
	
	//2. read
	bool tf = read_vrml(fileName2, m_Vertex, m_Color, m_Normal, m_FaceIdx, m_vNum, m_iNum, m_MaxY, m_MinY, m_MaxX, m_MinX );
	if(!tf) return false;

	//3. del_tmprary file
		TRY
		{			
			CFile::Remove( fileName2 );			//delete temprary file
		}
		CATCH( CFileException, e )
		{
			#ifdef _DEBUG
				afxDump << "File " << (LPCSTR)fileName2<< " cannot be removed\n";
			#endif
		}
		END_CATCH
	return true;
}



//end_ysj

