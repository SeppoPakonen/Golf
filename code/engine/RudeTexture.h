/*
 *  RudeTexture.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeTexture
#define __H_RudeTexture

#include <cstring>

const int kNameLen = 32;

class RudeTexture {

public:
	RudeTexture();
	~RudeTexture();
	
	int LoadFromPVRTFile(const char *name);
	int LoadFromPVRTPointer(const char *name, const void *data);
	int LoadFromPNG(const char *name);

	void SetActive();
	
	const char * GetName() { return m_name; }
	void SetName(const char *name) { strncpy(m_name, name, kNameLen-1); m_name[kNameLen-1] = '\0'; }
	
	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
	void SetWidth(int width) { m_width = width; }
	void SetHeight(int height) { m_height = height; }
	void SetTexture(unsigned int texture) { m_texture = texture; }
	
private:
	
	char m_name[kNameLen];
	unsigned int m_texture;
	int m_height;
	int m_width;
	
};

#endif


