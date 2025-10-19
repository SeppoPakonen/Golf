/*
 *  RudeCollision.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeCollision
#define __H_RudeCollision

#ifdef USE_BULLET_PHYSICS
#include <btBulletDynamicsCommon.h>
#else
// Include RudeGL.h which has the fallback btVector3 definition
#include "RudeGL.h"
#endif

class RudeCollision
{
public:
	
	static int LineSegmentAndSphere(const btVector3 &line_p1, const btVector3 &line_p2, const btVector3 &circle_p, float radius, btVector3 &int_p1, btVector3 &int_p2);
	
	static void UnitTest();
	
};

#endif // __H_RudeCollision