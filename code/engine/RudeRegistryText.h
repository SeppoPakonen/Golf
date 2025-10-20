/*
 *  RudeRegistryText.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef _H_RudeRegistryText
#define _H_RudeRegistryText

#include "RudeRegistry.h"

/**
 * Registry implementation that uses text files.  This class is mostly only useful for debugging.
 * Look at one of the platform-specific implementation for routine use.
 */
class RudeRegistryText : public RudeRegistry
{
public:
	RudeRegistryText(void);
	~RudeRegistryText(void);

	int QueryByte(const TCHAR *app, const TCHAR *name, void *buffer, int *buffersize) override;
	int SetByte(const TCHAR *app, const TCHAR *name, void *buffer, int buffersize) override;

private:


};

#endif