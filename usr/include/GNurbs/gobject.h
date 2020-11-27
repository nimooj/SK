#pragma once
#ifndef _GOBJECT_H
#define _GOBJECT_H

#include "../usr/include/Gelem/Gelem.h"

/**********************************************************************
 * header file of gobject class.
 * implemented by Seung-Hyun Yun.
 * last update : 2003. 2. 7
 **********************************************************************/
class GObject
{
public:
	// constructors
	GObject();
	// destructor.

	virtual ~GObject();

public:
	// Get interfaces
	GVector3f	GetOrigin(); 
	GQuaternion GetRotation(); 
	// Set interfaces
	void SetOrigin(GVector3f orig); 
	void SetRotation(GQuaternion q); 

private:
	GVector3f Origin;
	GQuaternion Quat;
};

#else
#endif // _GOBJECT_H

