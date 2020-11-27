#pragma once
#ifndef _GPLY_H
#define _GPLY_H
/**********************************************************************
 * Header file of polygon file format class.
 * Implemented by Seo-Yeon Moon and revised by Seung-Hyun Yun.
 * Last modified : 2003. 2. 7
 **********************************************************************/
//#include <afx.h>
#include <atlstr.h>

class GPly
{
public:
	// constructor and destructor
	GPly();
	~GPly();

public:
	// Get Methods
	int			GetVertNum();
	int			GetFaceNum();
	GVector3f*	GetVertList();
	int*		GetIdxList();
	bool		ReadPlyFile(CString fileName);

private:
	// class memeber attributes.
	CString		m_fileName;			// load«— ply file name
	CString		m_fileFormat;		// ascii
	CString		m_fileVersion;		// 1.0
	int			m_nVertexNum;
	int			m_nFaceNum;
	int			m_nFaceFormat;		// trinagle=3
	// vertex, color and face index array pointer
	GVector3f	*m_pVertex;
	GVector3f	*m_pColor;
	int			*m_pIndex;
};

#else

#endif // _GPLY_H
