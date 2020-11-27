#pragma once

#ifndef _GHALFEDGE_H
#define _GHALFEDGE_H

class GSolid;
class GFace;
class GVertex;

class GHalfEdge
{
public:
	// Costructors.
	GHalfEdge();
	GHalfEdge(GFace *face, GVertex *v);

	// Destructor.
	~GHalfEdge();

	// Get Interfaces.
	GFace*		GetFace();
	GVertex*	GetStartVert();
	GHalfEdge*	GetPrevHalfEdge();
	GHalfEdge*	GetNextHalfEdge();
	GHalfEdge*  GetHalfEdgeMate();

	// Set Interfaces.
	void SetFace(GFace *pface);
	void SetStartVert(GVertex *pvert);
	void SetPrevHalfEdge(GHalfEdge *e);
	void SetNextHalfEdge(GHalfEdge *e);

private:
	// Attributes.
	GFace		*pFace;
	GVertex		*pVert;
	GHalfEdge	*pPrev, *pNext;
};

#else

#endif // _GHALFEDGE_H

