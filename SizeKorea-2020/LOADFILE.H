#include <fstream>

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
				  float *m_MinX);

bool read_obj_asc( CString strFileName, 
				   GVector3f **m_Vertex, 
				   GVector3f **m_Color, 
				   GVector3f **m_Normal,
				   int **m_FaceIdx, 
				   int *m_vNum, 
			       int *m_iNum,
				   float *m_MaxY, 
				   float *m_MinY,
				   float *m_MaxX,
				   float *m_MinX);

bool read_ply_asc( CString strFileName, 
				   GVector3f **m_Vertex, 
				   GVector3f **m_Color, 
				   GVector3f **m_Normal,
				   int **m_FaceIdx, 
				   int *m_vNum, 
			       int *m_iNum,
				   float *m_MaxY, 
				   float *m_MinY,
				   float *m_MaxX,
				   float *m_Min);

bool read_txt( CString strFileName, 
				   GVector3f **m_Vertex, 
				   GVector3f **m_Color, 
				   GVector3f **m_Normal,
				   int **m_FaceIdx, 
				   int *m_vNum, 
			       int *m_iNum,
				   float *m_MaxY, 
				   float *m_MinY,
				   float *m_MaxX,
				   float *m_Min
);

bool write_ply_asc( CString srfFileName, 
				    GVector3f *m_Vertex,
					GVector3f *m_Color,
					int *m_FaceIdx, 
					int m_vNum,
					int m_iNum );


bool read_msh( CString strFileName, 
				   GVector3f **m_Vertex, 
				   GVector3f **m_Color, 
				   GVector3f **m_Normal,
				   int **m_FaceIdx, 
				   int *m_vNum, 
			       int *m_iNum );

bool read_vrml( CString strFileName, 
				   GVector3f **m_Vertex, 
				   GVector3f **m_Color, 
				   GVector3f **m_Normal,
				   int **m_FaceIdx, 
				   int *m_vNum, 
			       int *m_iNum,
				   float *m_MaxY, 
				   float *m_MinY,
				   float *m_MaxX,
				   float *m_MinX);

bool read_wrb( CString strFileName, 
				   GVector3f **m_Vertex, 
				   GVector3f **m_Color, 
				   GVector3f **m_Normal,
				   int **m_FaceIdx, 
				   int *m_vNum, 
			       int *m_iNum,
				   float *m_MaxY, 
				   float *m_MinY,
				   float *m_MaxX,
				   float *m_MinX);


