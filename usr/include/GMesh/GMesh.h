#pragma once

#ifdef _WINDOWS_
#undef _WINDOWS_
#endif

#ifndef _GMESH_H
#define _GMESH_H

#ifndef MAX_DEG
#define	MAX_DEG 30
#endif // MAX_DEG

#include <assert.h>
#include "..\GNurbs\GNurbs.h"
#include "..\Gelem\Gelem.h"
#include "gio.h"
#include "gsolid.h"
#include "gface.h"
#include "ghalfedge.h"
#include "gvertex.h"
// #include "gvolume.h"
#include "gply.h"

#endif // _GMESH_H


