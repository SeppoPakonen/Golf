/*
 *  RudeGL.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */


#ifndef __H_RudeGL_OGLES
#define __H_RudeGL_OGLES

#include "Rude.h"
#include "RudeRect.h"


#define kNumberPi 3.14159267f
#define kNumberInversePi .318309886184f
#define kNumberInverse180 0.00555555555f
#define kNumberPiTimesInverse180 0.01745329252f
#define kNumberSquareroot2 1.41421356237f
#define kNumber180OverPi 57.2957792137960f

#ifdef USE_BULLET_PHYSICS
#include <btBulletDynamicsCommon.h>
#else
// Define fallback implementations for Bullet Physics types when not available
#include <cmath>

// Simple 3D vector class as fallback when Bullet Physics is not available
class btVector3 {
public:
    float m_floats[3];
    
    btVector3() { m_floats[0] = m_floats[1] = m_floats[2] = 0.0f; }
    btVector3(float x, float y, float z) {
        m_floats[0] = x;
        m_floats[1] = y;
        m_floats[2] = z;
    }
    
    float& operator[](int i) { return m_floats[i]; }
    const float& operator[](int i) const { return m_floats[i]; }
    
    btVector3 operator+(const btVector3& other) const {
        return btVector3(m_floats[0] + other.m_floats[0], 
                         m_floats[1] + other.m_floats[1], 
                         m_floats[2] + other.m_floats[2]);
    }
    
    btVector3 operator-(const btVector3& other) const {
        return btVector3(m_floats[0] - other.m_floats[0], 
                         m_floats[1] - other.m_floats[1], 
                         m_floats[2] - other.m_floats[2]);
    }
    
    btVector3& operator+=(const btVector3& other) {
        m_floats[0] += other.m_floats[0];
        m_floats[1] += other.m_floats[1];
        m_floats[2] += other.m_floats[2];
        return *this;
    }
    
    btVector3& operator-=(const btVector3& other) {
        m_floats[0] -= other.m_floats[0];
        m_floats[1] -= other.m_floats[1];
        m_floats[2] -= other.m_floats[2];
        return *this;
    }
    
    btVector3 operator*(float scalar) const {
        return btVector3(m_floats[0] * scalar, 
                         m_floats[1] * scalar, 
                         m_floats[2] * scalar);
    }
    
    // Friend function to handle scalar * vector (needed for float * btVector3)
    friend btVector3 operator*(float scalar, const btVector3& vec) {
        return btVector3(vec.m_floats[0] * scalar, 
                         vec.m_floats[1] * scalar, 
                         vec.m_floats[2] * scalar);
    }
    
    btVector3& operator*=(float scalar) {
        m_floats[0] *= scalar;
        m_floats[1] *= scalar;
        m_floats[2] *= scalar;
        return *this;
    }
    
    float length() const {
        return sqrt(m_floats[0]*m_floats[0] + m_floats[1]*m_floats[1] + m_floats[2]*m_floats[2]);
    }
    
    float dot(const btVector3& other) const {
        return m_floats[0]*other.m_floats[0] + m_floats[1]*other.m_floats[1] + m_floats[2]*other.m_floats[2];
    }
    
    // Bullet Physics compatible methods
    float x() const { return m_floats[0]; }
    float y() const { return m_floats[1]; }
    float z() const { return m_floats[2]; }
    
    float& x() { return m_floats[0]; }
    float& y() { return m_floats[1]; }
    float& z() { return m_floats[2]; }
    
    // Additional methods required by the code
    void normalize() {
        float len = length();
        if(len > 0) {
            m_floats[0] /= len;
            m_floats[1] /= len;
            m_floats[2] /= len;
        }
    }
    
    btVector3 cross(const btVector3& v) const {
        return btVector3(
            m_floats[1] * v.m_floats[2] - m_floats[2] * v.m_floats[1],
            m_floats[2] * v.m_floats[0] - m_floats[0] * v.m_floats[2],
            m_floats[0] * v.m_floats[1] - m_floats[1] * v.m_floats[0]
        );
    }
    
    void setX(float x) { m_floats[0] = x; }
    void setY(float y) { m_floats[1] = y; }
    void setZ(float z) { m_floats[2] = z; }
};

// Define btQuaternion fallback
class btQuaternion {
public:
    float m_floats[4]; // x, y, z, w
    
    btQuaternion() { m_floats[0] = m_floats[1] = m_floats[2] = 0.0f; m_floats[3] = 1.0f; }
    btQuaternion(float x, float y, float z, float w) {
        m_floats[0] = x; m_floats[1] = y; m_floats[2] = z; m_floats[3] = w;
    }
};

// Define btTransform fallback
class btTransform {
public:
    btQuaternion m_rotation;
    btVector3 m_origin;
    
    btTransform() {}
    btTransform(const btQuaternion& q, const btVector3& v) : m_rotation(q), m_origin(v) {}
};

// Define btScalar as float
typedef float btScalar;

// Define other needed Bullet Physics types
class btDefaultMotionState {
public:
    btTransform m_graphicsWorldTrans;
    btTransform m_centerOfMassOffset;
    btTransform m_startWorldTrans;
    
    btDefaultMotionState(const btTransform& xform) : m_graphicsWorldTrans(xform) {}
};
#endif // USE_BULLET_PHYSICS


typedef enum {
	kDepthTest = 0,
	kBackfaceCull,
	
	kNumRudeGLEnableOptions
} eRudeGLEnableOption;

typedef enum {
	kVertexArray,
	kColorArray,
	kTextureCoordArray,
	
	kNumRudeGLEnableClientOptions
} eRudeGLEnableClientOption;

class RudeGL
{
public:

	RudeGL();
	~RudeGL(void);

	void SetViewport(int top, int left, int bottom, int right);
	void Ortho(float ox, float oy, float oz, float w, float h, float d);
	void Frustum(float ox, float oy, float w, float h, float near, float far);
	void LookAt(float eyex, float eyey, float eyez, float vx, float vy, float vz,
			float upx, float upy, float upz);
	void LoadIdentity();
	void Translate(float x, float y, float z);
	void Scale(float sx, float sy, float sz);
	void Rotate(float degrees, float ax, float ay, float az);
	void RotateX(int degrees);
	void RotateY(int degrees);
	void RotateZ(int degrees);
	void RotateView(float degrees, float ax, float ay, float az);
	void TranslateView(float x, float y, float z);
	
	btVector3 Project(const btVector3 &point);
	btVector3 InverseProject(const btVector3 &point);
	
	void Enable(eRudeGLEnableOption option, bool enable);
	void EnableClient(eRudeGLEnableClientOption option, bool enable);
	void FlushEnables();

	btVector3 GetEye() { return m_eye; }
	btVector3 GetLookAt() { return m_lookAt; }
	btVector3 GetForward() { return m_forward; }
	btVector3 GetUp() { return m_up; }
	
	float GetHalfWidth() { return m_hw; }
	float GetHalfHeight() { return m_hh; }
	
	float * GetViewMat() { return m_viewmat; }
	
	void SetLandscape(bool landscape) { m_landscape = landscape; }
	bool GetLandscape() { return m_landscape; }
	
	void SetUpsideDown(bool upsideDown) { m_upsideDown = upsideDown; }
	bool GetUpsideDown() { return m_upsideDown; }

	void SetDeviceHeight(float h) { m_deviceHeight = h; }
	float GetDeviceHeight() { return m_deviceHeight; }

	void SetDeviceWidth(float w) { m_deviceWidth = w; }
	float GetDeviceWidth() { return m_deviceWidth; }

private:
	
	float m_viewmat[16];

	bool m_enables[kNumRudeGLEnableOptions];
	bool m_enableClients[kNumRudeGLEnableClientOptions];
	
	RudeRect m_viewport;				// screen viewport rect
	btVector3 m_eye;
	btVector3 m_lookAt;
	btVector3 m_forward;
	btVector3 m_up;
	float m_hw;
	float m_hh;
	float m_near;
	
	bool m_landscape;
	bool m_upsideDown;
	float m_deviceHeight;
	float m_deviceWidth;
	
};

extern RudeGL RGL;

#endif
