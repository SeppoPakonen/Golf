/*
 * RudeTextureManager.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeTextureManager.h"



static RudeTextureManager *gTextureManager = 0;

RudeTextureManager * RudeTextureManager::GetInstance()
{
	if(gTextureManager == 0)
		gTextureManager = new RudeTextureManager();
	
	return gTextureManager;
}


RudeTextureManager::RudeTextureManager()
{
}

RudeTextureManager::~RudeTextureManager()
{
}

int RudeTextureManager::GetTextureID(const char *name)
{
	unsigned int s = m_textures.size();
	
	if(gVerbosityLevel >= 2) {
		printf("Searching for texture: %s in cache of %d entries\n", name, s);
	}
	
	for(unsigned int i = 0; i < s; i++)
	{
		if(m_textures[i])
		{
			const char *texname = m_textures[i]->GetName();
			
			if(gVerbosityLevel >= 2) {
				printf("  Checking texture ID %d: %s\n", i, texname);
			}
			
			if(strcmp(name, texname) == 0) {
				if(gVerbosityLevel >= 1) {
					printf("  Found texture %s at ID %d\n", name, i);
				}
				return i;
			}
		}
	}
	
	if(gVerbosityLevel >= 1) {
		printf("  Texture %s not found in cache\n", name);
	}
	
	return -1;
}


int RudeTextureManager::LoadTextureFromPVRTFile(const char *name)
{
	if(gVerbosityLevel >= 1) {
		printf("Attempting to load texture: %s\n", name);
	}
	
	// check to make sure its not already loaded
	int texid = GetTextureID(name);
	if(texid >= 0) {
		if(gVerbosityLevel >= 2) {
			printf("Texture %s already loaded with ID %d\n", name, texid);
		}
		return texid;
	}
	
	if(gVerbosityLevel >= 1) {
		printf("Texture %s not found in cache, loading from file\n", name);
	}
	
	RudeTexture *tex = new RudeTexture();
	
	int result = tex->LoadFromPVRTFile(name);
	
	if(gVerbosityLevel >= 2) {
		printf("LoadFromPVRTFile result: %d for texture %s\n", result, name);
	}
	
	// If PVR loading failed (maybe PVR support is disabled), try PNG with various name patterns
	if(result < 0) {
		if(gVerbosityLevel >= 1) {
			printf("PVR loading failed, trying PNG for texture: %s\n", name);
		}
		
		// First, try loading as PNG with the original name
		delete tex;
		tex = new RudeTexture();
		result = tex->LoadFromPNG(name);
		
		if(gVerbosityLevel >= 2) {
			printf("LoadFromPNG result with original name: %d for texture %s\n", result, name);
		}
		
		// If that failed, try common naming variations that might exist
		if(result < 0) {
			// Try changing the last 2 characters "00" to "0" (e.g. "ab18_00" -> "ab18_0") 
			// This handles the case where PVR is named ab18_00 but PNG is named ab18_0
			size_t len = strlen(name);
			if (len >= 3 && strcmp(name + len - 2, "00") == 0) {
				char modified_name[256];
				strncpy(modified_name, name, len - 2);
				strcpy(modified_name + len - 2, "0"); // Replace "00" with "0"
				
				if(gVerbosityLevel >= 1) {
					printf("PVR and original PNG failed, trying modified name: %s\n", modified_name);
				}
				
				delete tex;
				tex = new RudeTexture();
				result = tex->LoadFromPNG(modified_name);
				
				if(gVerbosityLevel >= 2) {
					printf("LoadFromPNG result with modified name %s: %d\n", modified_name, result);
				}
			}
		}
		
		// If still failed, try other common patterns
		if(result < 0) {
			// Try removing trailing number (e.g. "ab18_0" -> "ab18")
			size_t len = strlen(name);
			if (len >= 2 && name[len-1] == '0' && name[len-2] == '_') {
				char modified_name[256];
				strncpy(modified_name, name, len - 2);
				modified_name[len - 2] = '\0'; // Remove "_0"
				
				if(gVerbosityLevel >= 1) {
					printf("Trying another modified name: %s\n", modified_name);
				}
				
				delete tex;
				tex = new RudeTexture();
				result = tex->LoadFromPNG(modified_name);
				
				if(gVerbosityLevel >= 2) {
					printf("LoadFromPNG result with modified name %s: %d\n", modified_name, result);
				}
			}
		}
	}
	
	RUDE_ASSERT(result >= 0, "Unable to load texture %d (%s)", result, name);
	
	if(result < 0)
	{
		delete tex;
		return result;
	}
	
	int final_id = InsertTexture(tex);
	if(gVerbosityLevel >= 2) {
		printf("Successfully inserted texture %s with ID %d\n", name, final_id);
	}
	
	return final_id;
}


int RudeTextureManager::LoadTextureFromPVRTPointer(const char *name, const void *data)
{
	// check to make sure its not already loaded
	int texid = GetTextureID(name);
	if(texid >= 0)
		return texid;
	
	RudeTexture *tex = new RudeTexture();
	
	int result = tex->LoadFromPVRTPointer(name, data);
	
	RUDE_ASSERT(result >= 0, "Unable to load texture");
	
	if(result < 0)
	{
		delete tex;
		return result;
	}
	
	return InsertTexture(tex);
}


int RudeTextureManager::LoadTextureFromPNGFile(const char *name)
{
	RUDE_ASSERT(name, "Invalid texture name");
	
	if(gVerbosityLevel >= 1) {
		printf("Attempting to load PNG texture: %s\n", name);
	}
	
	// check to make sure its not already loaded
	int texid = GetTextureID(name);
	if(texid >= 0) {
		if(gVerbosityLevel >= 2) {
			printf("PNG texture %s already loaded with ID %d\n", name, texid);
		}
		return texid;
	}
	
	if(gVerbosityLevel >= 1) {
		printf("PNG texture %s not found in cache, loading from file\n", name);
	}
	
	RudeTexture *tex = new RudeTexture();
	
	int result = tex->LoadFromPNG(name);
	
	if(gVerbosityLevel >= 2) {
		printf("LoadFromPNG result: %d for texture %s\n", result, name);
	}
	
	RUDE_ASSERT(result >= 0, "Unable to load texture (%s)", name);
	
	if(result < 0)
	{
		delete tex;
		return result;
	}
	
	int final_id = InsertTexture(tex);
	if(gVerbosityLevel >= 2) {
		printf("Successfully inserted PNG texture %s with ID %d\n", name, final_id);
	}
	
	return final_id;
}

int RudeTextureManager::ReplaceTextureFromPNGFile(int texid, const char *name)
{
	RUDE_ASSERT(name, "Invalid texture name");
	RUDE_ASSERT(texid >= 0, "Invalid id");
	RUDE_ASSERT(((unsigned int) texid) < m_textures.size(), "Invalid id");
	RUDE_ASSERT(m_textures[texid], "Invalid texture");
	
	m_textures[texid]->LoadFromPNG(name);
	
	return texid;
	
}

void RudeTextureManager::ReleaseTexture(int texid)
{
	RUDE_ASSERT(texid >= 0, "Invalid id");
	RUDE_ASSERT(((unsigned int) texid) < m_textures.size(), "Invalid id");
	
	if(m_textures[texid])
	{
		delete m_textures[texid];
		m_textures[texid] = 0;
	}
}

int RudeTextureManager::InsertTexture(RudeTexture *texture)
{
	if(gVerbosityLevel >= 2) {
		printf("Inserting texture: %s\n", texture ? texture->GetName() : "NULL");
	}
	
	unsigned int size = m_textures.size();
	for(unsigned int i = 0; i < size; i++)
	{
		if(m_textures[i] == 0)
		{
			m_textures[i] = texture;
			if(gVerbosityLevel >= 2) {
				printf("  Reused slot at index %d\n", i);
			}
			return i;
		}
	}
	
	m_textures.push_back(texture);	
	int new_id = m_textures.size() - 1;
	if(gVerbosityLevel >= 2) {
		printf("  Created new slot at index %d\n", new_id);
	}
	return new_id;
}



