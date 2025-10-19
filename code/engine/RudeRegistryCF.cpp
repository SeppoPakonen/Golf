/*
 *  RudeRegistryCF.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeRegistryCF.h"

#ifdef RUDE_MACOS
#include <CoreFoundation/CFPreferences.h>

RudeRegistryCF::RudeRegistryCF(void)
{
}

RudeRegistryCF::~RudeRegistryCF(void)
{
}


/**
 * Retrieve the stored data specified by the given 'app' and data 'name'.  Requires
 * 'buffer' be pre-allocated and contains enough space to hold the data.  Pass in
 * the size of 'buffer' with 'buffersize'.
 *
 * @returns 0 on success, -1 on error
 */
int RudeRegistryCF::QueryByte(const TCHAR *app, const TCHAR *name, void *buffer, int *buffersize)
{
	CFStringRef key = CFStringCreateWithCString(NULL, name, kCFStringEncodingASCII);
	CFPropertyListRef value = CFPreferencesCopyAppValue(key, kCFPreferencesCurrentApplication);
	
	if(value == 0)
		return -1;
	
	if(CFDataGetLength((CFDataRef) value) != *buffersize)
		return -1;
	
	CFRange range = CFRangeMake(0, *buffersize);
	CFDataGetBytes((CFDataRef) value, range, (UInt8 *) buffer);

	return 0;
}

/**
 * Store the data given in 'buffer' under the given 'app' and 'name'.  Pass in the size
 * of the 'buffer' with 'buffersize'.
 *
 * @returns 0 on success
 */
int RudeRegistryCF::SetByte(const TCHAR *app, const TCHAR *name, void *buffer, int buffersize)
{
	CFPropertyListRef value = CFDataCreate(NULL, (UInt8 *) buffer, buffersize);
	CFStringRef key = CFStringCreateWithCString(NULL, name, kCFStringEncodingASCII);
	CFPreferencesSetAppValue(key, value, kCFPreferencesCurrentApplication);

	return 0;
}

#else  // RUDE_MACOS

// Stub implementation for non-macOS platforms
RudeRegistryCF::RudeRegistryCF(void)
{
}

RudeRegistryCF::~RudeRegistryCF(void)
{
}


/**
 * Retrieve the stored data specified by the given 'app' and data 'name'.  Requires
 * 'buffer' be pre-allocated and contains enough space to hold the data.  Pass in
 * the size of 'buffer' with 'buffersize'.
 *
 * @returns -1 (failure) on non-macOS platforms which don't have CoreFoundation
 */
int RudeRegistryCF::QueryByte(const TCHAR *app, const TCHAR *name, void *buffer, int *buffersize)
{
	return -1; // Not supported on this platform
}

/**
 * Store the data given in 'buffer' under the given 'app' and 'name'.  Pass in the size
 * of the 'buffer' with 'buffersize'.
 *
 * @returns -1 (failure) on non-macOS platforms which don't have CoreFoundation
 */
int RudeRegistryCF::SetByte(const TCHAR *app, const TCHAR *name, void *buffer, int buffersize)
{
	return -1; // Not supported on this platform
}

#endif // RUDE_MACOS