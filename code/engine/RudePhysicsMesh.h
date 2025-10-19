/*
 *  RudePhysicsMesh.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudePhysicsMesh
#define __H_RudePhysicsMesh

#include "Rude.h"
#include "RudeObject.h"
#include "RudePhysicsObject.h"

#include <map>
#include <vector>

#ifdef USE_BULLET_PHYSICS
#include <btBulletDynamicsCommon.h>
#else
#include "RudeGL.h"  // This has our fallback btVector3
// Define fallback types for Bullet Physics when not available
class btTriangleIndexVertexArray {};
class btBvhTriangleMeshShape {};
#endif

class RudeMesh;


class RudePhysicsMesh : public RudePhysicsObject {
	
public:
	RudePhysicsMesh(RudeObject *owner);
	virtual ~RudePhysicsMesh();
	
	virtual void Load(RudeMesh *mesh, float mass);
	
	int GetSubPartMapping(int part) { return m_subPartMappings[part]; }
	
protected:
	
	std::map<int, int> m_subPartMappings;
	
#ifdef USE_BULLET_PHYSICS
	btTriangleIndexVertexArray * m_data;
	btBvhTriangleMeshShape *m_shape;
#else
	void * m_data;                    // Placeholder when Bullet Physics not available
	void *m_shape;                    // Placeholder when Bullet Physics not available
#endif
	
};


#endif
