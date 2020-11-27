#pragma once

#ifndef _GFACE_H
#define _GFACE_H

class GSolid;
class GHalfEdge;
class GVertex;

class GFace
{
public:
	// constructors.
	GFace();
	GFace(GSolid* solid, GVertex* v0, GVertex* v1, GVertex* v2);

	// destructors.
	~GFace();

	// Get Interfaces.
	int			GetId();
	GFace*		GetNextFace();
	GFace*		GetPrevFace();
	GSolid*		GetSolid();
	GHalfEdge*	GetStartHalfEdge();
	GVertex*	GetFirstVert();
	GVertex*	GetSecondVert();
	GVertex*	GetThirdVert();

	// Set Interfaces.
	void SetId(int id);
	void SetSolid(GSolid* psolid);
	void SetPrevFace(GFace* pface);
	void SetNextFace(GFace* pface);
	void SetStartHalfEdge(GHalfEdge* phedge);

private:
	// Attributes.
	int			id;
	GFace		*pPrev, *pNext;
	GHalfEdge	*pHedges;
	GSolid		*pSolid;
};

#else

#endif // _GFACE_H

