#pragma once

#ifndef _GVERTEX_H
#define _GVERTEX_H

class GSolid;
class GFace;
class GHalfEdge;

class GVertex
{
public:
	// constructors.
	GVertex();
	GVertex(REAL x, REAL y, REAL z);
	GVertex(REAL x, REAL y, REAL z, REAL nx, REAL ny, REAL nz);
	// destructor.
	~GVertex();

	// General Interfaces.
	bool	PushHalfEdge(GHalfEdge *hedge);
	bool	PopHalfEdge(GHalfEdge *hedge);

	// Get Interfaces.
	int			GetId();
	int			GetDeg();
	REAL*		GetCoord();
	REAL*		GetNormal();
	GVertex*	GetNextVert();
	GVertex*	GetPrevVert();
	GHalfEdge*	GetHalfEdge(int idx = 0);


	// Set Interfaces.
	void	SetId(int id);
	void	SetDeg(int deg);
	void	SetPrevVert(GVertex *vert);
	void	SetNextVert(GVertex *vert);
	void	SetCoord(REAL x, REAL y, REAL z);
	void	SetNormal(REAL x, REAL y, REAL z);


private:
	// Attributes.
	int			id;
	int			deg;
	REAL		V[3], N[3];
	GHalfEdge	**pHedges;
	GVertex		*pPrev, *pNext;
};

#else

#endif // _GVERTEX_H
