/*
 *  RudeRegistryCF.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeRegistryCF.h"

#include <CoreFoundation/CFPreferences.h>

RudeRegistryCF::RudeRegistryCF(void)
{
}

RudeRegistryCF::~RudeRegistryCF(void)
{
}


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

int RudeRegistryCF::SetByte(const TCHAR *app, const TCHAR *name, void *buffer, int buffersize)
{
	CFPropertyListRef value = CFDataCreate(NULL, (UInt8 *) buffer, buffersize);
	CFStringRef key = CFStringCreateWithCString(NULL, name, kCFStringEncodingASCII);
	CFPreferencesSetAppValue(key, value, kCFPreferencesCurrentApplication);

	return 0;
}
