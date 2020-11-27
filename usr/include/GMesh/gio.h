#pragma once

#ifndef _GIO_H
#define _GIO_H

class GSolid;
class GVertex;
class GHalfEdge;

extern GVertex**	vArray;

void read_gff_file(GSolid *pSolid, char *filename);

void read_off_file(GSolid *pSolid, char *filename);

void read_noff_file(GSolid *pSolid, char *filename);

void read_ply_file(GSolid *pSolid, char *filename);

void read_iv_file(GSolid *pSolid, char *filename);

void write_gff_file(GSolid *pSolid, char *filename);


#else

#endif // _GIO_H
