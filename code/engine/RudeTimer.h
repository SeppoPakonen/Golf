/*
 *  RudeTimer.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeTimer
#define __H_RudeTimer

#include "Rude.h"

#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>


class RudeTimer
{
public:

	RudeTimer()
	{
		Restart();
	}

	void Restart()
	{
		m_starttime = mach_absolute_time();
	}
	
	u64 ElapsedRaw()
	{
		return mach_absolute_time() - m_starttime;
	}
	
	u64 ElapsedNanoseconds()
	{
		static mach_timebase_info_data_t sTimebaseInfo = { 0,0 };
		if ( sTimebaseInfo.denom == 0 ) {
			(void) mach_timebase_info(&sTimebaseInfo);
		}
		
		return ElapsedRaw() * sTimebaseInfo.numer / sTimebaseInfo.denom;
	}
	
	float ElapsedMilliseconds()
	{
		return ((float) ElapsedNanoseconds()) / 1000000.0f;
	}
	
	float ElapsedSeconds()
	{
		return ((float) ElapsedNanoseconds()) / 1000000000.0f;
	}

private:
	
	u64 m_starttime;
	
};

#endif // RUDE_IPHONE || RUDE_MACOS


#ifdef RUDE_WIN

class RudeTimer
{
public:

	RudeTimer()
	{
		Restart();
	}

	void Restart()
	{
		m_starttime = GetTickCount();
	}

	DWORD ElapsedRaw()
	{
		return GetTickCount() - m_starttime;
	}


	float ElapsedMilliseconds()
	{
		return (float) ElapsedRaw();
	}

	float ElapsedSeconds()
	{
		return ((float) ElapsedRaw()) * 0.0001f;
	}

private:

	DWORD m_starttime;

};

#endif // RUDE_WIN

#ifdef USE_SDL
#ifdef __linux__
#include <sys/time.h>

class RudeTimer
{
public:

	RudeTimer()
	{
		Restart();
	}

	void Restart()
	{
		gettimeofday(&m_starttime, NULL);
	}
	
	float ElapsedSeconds()
	{
		struct timeval now;
		gettimeofday(&now, NULL);
		
		float elapsed = (now.tv_sec - m_starttime.tv_sec) + 
						(now.tv_usec - m_starttime.tv_usec) / 1000000.0f;
		
		return elapsed;
	}

private:
	
	struct timeval m_starttime;

};

#endif // __linux__
#endif // USE_SDL

// Add generic POSIX/Linux fallback when not on specific platforms
#ifndef RUDE_IPHONE
#ifndef RUDE_MACOS
#ifndef RUDE_WIN
#ifndef USE_SDL
#ifdef __linux__
#include <sys/time.h>

class RudeTimer
{
public:

	RudeTimer()
	{
		Restart();
	}

	void Restart()
	{
		gettimeofday(&m_starttime, NULL);
	}
	
	float ElapsedSeconds()
	{
		struct timeval now;
		gettimeofday(&now, NULL);
		
		float elapsed = (now.tv_sec - m_starttime.tv_sec) + 
						(now.tv_usec - m_starttime.tv_usec) / 1000000.0f;
		
		return elapsed;
	}

private:
	
	struct timeval m_starttime;

};

#endif // __linux__
#endif // USE_SDL
#endif
#endif
#endif

#endif
