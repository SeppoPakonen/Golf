/*
 *  RudeObject.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeObject
#define __H_RudeObject

#include "Rude.h"

#ifdef USE_BULLET_PHYSICS
#include <btBulletDynamicsCommon.h>
#else
// Fallback math structures if Bullet Physics is not available
#include "RudeGL.h" // This already defines btVector3
#endif

class RudeMesh;
class RudeSkinnedMesh;
class RudePhysicsObject;

class RudeObject {
	
public:
	
	RudeObject();
	~RudeObject();
	
	virtual void LoadMesh(const char *name);
	
	virtual void LoadSkinnedMesh(const char *name);
	
	RudeMesh * GetMesh() { return m_mesh; }
	
	
	void LoadPhysicsMesh(float mass);
	void LoadPhysicsSphere(float radius, float mass);
	
	virtual void NextFrame(float delta);
	virtual void Render();
	
	RudePhysicsObject * GetPhysicsObject() { return m_physics; }
	
	virtual void SetPosition(const btVector3 &p)
	{
		m_position = p;
	}
	
protected:
	
	btVector3 m_position;
	
private:
	
	RudeMesh *m_mesh;
	RudePhysicsObject *m_physics;
};

#endif


