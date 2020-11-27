#pragma once

/*********************************************************************
 * header file for nurbs.lib                                         *
 *********************************************************************/

#ifndef _NURBS_H
#define	_NURBS_H

#include <assert.h>
#include "..\Gelem\Gelem.h"
#include "gobject.h"
#include "gnurbsutil.h"
#include "gnurbscrv.h"
#include "gnurbssrf.h"
#include "gnurbsmot.h"
#include "gnurbsgc.h"
// #include "gmlbssrf.h"
#include "gruledsrf.h"
#include "grevolsrf.h"
#include "linearsystem.h"

 /**********************************************************************
  *	For Nurbs Library.
  **********************************************************************/
#define FBSP_EPS 0.000001
#define EQ(A, B) ( MTP_EQ_EPS(A, B, FBSP_EPS) )
#define EQ_ZERO(A) EQ(A, 0.0)
#define U_DIR 0
#define V_DIR 1

#else
#endif // _NURBS_H

