#pragma once

#ifndef _GSOLID_H
#define _GSOLID_H

class GObject;
class GFace;
class GHalfEdge;
class GVertex;

class GSolid : public GObject
{
public:
	enum FileType { GFF = 0, OFF, NOFF, PLY, IV };
	GSolid();
	virtual ~GSolid();

public:
	// internal useage.
	bool AddFace(GFace* f);
	bool AddVertex(GVertex* v);
	bool DeleteAllFaces();
	bool DeleteAllVertics();
	bool DeleteFace(GFace *pFace);
	bool DeleteVert(GVertex *pVert);
	bool CalcNormal();

public:
	// general interfaces.
	void	 Create(FileType ftype, char *filename);
	void	 Create(GNurbsSrf *srf, int segnum = 50);
	void	 Export(FileType ftype, char *filename);
	void	 Transform(MATRIX mat);
	GVertex* GetVertex(int idx);

	// Set Interfaces.
	void	SetActive();
	void	SetInActive();
	void	SetId(int id);
	void	SetVertNum(int num);
	void	SetFaceNum(int num);
	void	SetStartVert(GVertex *pvert);
	void	SetStartFace(GFace *pface);

	// Get Interfaces.
	int			GetId();
	int			GetVertNum();
	int			GetFaceNum();
	bool		IsActive();
	GFace*		GetStartFace();
	GVertex*	GetStartVert();

private: // Attributes.
	int			id; // id of gsolid.
	int			vNum;
	int			fNum;
	bool		Active;
	GFace		*pFaces;
	GVertex		*pVerts;
};


#else

#endif // _GSOLID_H
