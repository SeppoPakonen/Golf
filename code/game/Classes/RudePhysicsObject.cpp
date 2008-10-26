/*
 *  RudePhysicsObject.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/8/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RudePhysicsObject.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>



#include <btBulletDynamicsCommon.h>

RudePhysicsObject::RudePhysicsObject(RudeObject *owner)
: m_motionState(0)
, m_rigidBody(0)
, m_notifyOnContact(false)
, m_contactCallback(0)
, m_owner(owner)
{
}

RudePhysicsObject::~RudePhysicsObject()
{
	if(m_rigidBody)
		delete m_rigidBody;
	if(m_motionState)
		delete m_motionState;
}


void RudePhysicsObject::Render()
{
	btTransform trans;
	m_rigidBody->getMotionState()->getWorldTransform(trans);
	btScalar m[16];
	trans.getOpenGLMatrix(m);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m);
}

void RudePhysicsObject::Contact(const btVector3 &contactNormal, RudePhysicsObject *other, int mypartId, int otherpartId, float *friction, float *restitution)
{
	if(!m_notifyOnContact)
		return;
	
	if(!m_contactCallback)
		return;
	
	m_contactCallback(contactNormal, this, other, mypartId, otherpartId, friction, restitution);
}

