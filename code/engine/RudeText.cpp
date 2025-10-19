/*
 *  RudeText.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeText.h"

#include "RudeGL.h"

#include <stdarg.h>

#if USE_PVR_TEXT
#include "PVRTFixedPoint.h"
#include "PVRTPrint3D.h"

CPVRTPrint3D gTextPrint;

void RudeText::Init()
{
	gTextPrint.SetTextures(0, (unsigned int) RGL.GetDeviceHeight(), (unsigned int) RGL.GetDeviceWidth());
}

void RudeText::Print(float x, float y, float scale, unsigned int color, const char * const pszFormat, ...)
{
	va_list args;
	char text[512];
	
	va_start(args, pszFormat);
	vsprintf(text, pszFormat, args);
	va_end(args);
	
	gTextPrint.Print3D(x, y, scale, color, text);
}

void RudeText::Flush()
{
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, true);
	gTextPrint.Flush();
}

#else  // USE_PVR_TEXT

// Stub implementations when PowerVR text is disabled
void RudeText::Init()
{
	// No-op when PowerVR text is disabled
}

void RudeText::Print(float x, float y, float scale, unsigned int color, const char * const pszFormat, ...)
{
	// No-op when PowerVR text is disabled
}

void RudeText::Flush()
{
	// No-op when PowerVR text is disabled
}

#endif // USE_PVR_TEXT