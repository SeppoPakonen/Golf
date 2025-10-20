/*
 *  RudeRegistry.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeRegistry.h"

#ifdef RUDE_WIN
#include "RudeRegistryWin.h"
#include "RudeRegistryText.h"
#endif

#ifdef RUDE_SYMBIAN
#include "RudeRegistrySymbian.h"
#include "RudeRegistryText.h"
#endif

#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
#include "RudeRegistryCF.h"
#endif

// Include text registry for Linux and other platforms
#include "RudeRegistryText.h"

RudeRegistry::RudeRegistry(void)
{
}

RudeRegistry::~RudeRegistry(void)
{
}

/**
 * Get a pointer to the RudeRegistry singleton for the currently running platform.
 * 
 * iPhone/iPod/MacOS: RudeRegistryCF\n
 * Windows/PocketPC: RudeRegistryWin\n
 * Symbian: RudeRegistrySymbian
 */
RudeRegistry * RudeRegistry::GetSingleton()
{
	
#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
	
	static RudeRegistryCF *reg = 0;
	
	if(reg == 0)
		reg = new RudeRegistryCF();
	
	return (RudeRegistry *) reg;

#elif defined(RUDE_WIN)

	static RudeRegistryWin *reg = 0;

	if(reg == 0)
		reg = new RudeRegistryWin();

	return (RudeRegistry *) reg;
	
#else
	// For Linux and other platforms, use the text file-based registry
	#ifdef RUDE_TEXT_REGISTRY
		static RudeRegistryText *reg = 0;
		
		if(reg == 0)
			reg = new RudeRegistryText();
		
		return (RudeRegistry *) reg;
	#else
		static RudeRegistry *reg = 0;  // Fallback static instance
		if(reg == 0) {
			// For Linux platforms, we use RudeRegistryText implementation
			// This is a text file-based registry that works cross-platform
			reg = new RudeRegistryText();
		}
		return reg;
	#endif
#endif
}