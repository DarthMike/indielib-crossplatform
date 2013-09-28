/*****************************************************************************************
 * File: PrecissionTimer.cpp
 * Desc: Class to encapsulate a precission timer (if hardware permits it)
 *****************************************************************************************/

/*********************************** The zlib License ************************************
 *
 * Copyright (c) 2013 Indielib-crossplatform Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 *****************************************************************************************/


#include "PrecissionTimer.h"
#if defined (PLATFORM_WIN32)
#if defined (_MSC_VER)
#pragma warning(push)  //Store warnings state
#pragma warning(disable : 4244)  //Disable warning to conversion between __int64 and double
#endif
#endif

/** @cond DOCUMENT_PRIVATEAPI */

//Get count
double PrecissionTimer::getTicks() {

	//IF- Not started or paused
	if (!_started || _paused)
		return _elapsedTime;

#ifdef PLATFORM_WIN32
	if (_highRes) {
		QueryPerformanceCounter((LARGE_INTEGER *)&mFinalTime);
		_elapsedTime = (mFinalTime - mStartTime) * 1000 / mFrequency;
	} else {
		mFinalTime = static_cast<__int64>(GetTickCount());
		_elapsedTime = mFinalTime - mStartTime;

	}
#endif

#if defined (PLATFORM_IOS) || defined (PLATFORM_OSX) 
	mFinalTime = mach_absolute_time();
	_elapsedTime = (mFinalTime - mStartTime) * (mTimeBaseInfo.numer) / (mTimeBaseInfo.denom) / 1000000.0f;
#endif

#ifdef PLATFORM_LINUX
	 clock_gettime(CLOCK_MONOTONIC, &linux_end);
     mFinalTime = ((((uint64_t) linux_end.tv_sec) * 1000000000ULL) + (uint64_t) linux_end.tv_nsec); 
     _elapsedTime = (mFinalTime - mStartTime) / 1000000.0f; 
#endif
	return(static_cast<double>(_elapsedTime));

}

//Start count
void PrecissionTimer::start() {
#ifdef PLATFORM_WIN32
	if (_highRes)
		QueryPerformanceCounter((LARGE_INTEGER *)&mStartTime);
	else
		mStartTime = static_cast<__int64>(GetTickCount());
#endif

#if defined (PLATFORM_IOS) || defined (PLATFORM_OSX)
	mStartTime = mach_absolute_time();
#endif

#ifdef PLATFORM_LINUX
    clock_gettime(CLOCK_MONOTONIC, &linux_start);
	mStartTime = (((uint64_t) linux_start.tv_sec) * 1000000000ULL) + (uint64_t) linux_start.tv_nsec;
#endif

	_started = true;
	_paused = false;
}

//Stop count
void PrecissionTimer::stop() {
	_elapsedTime = 0;
	_started = false;
	_paused = false;
}

//Pause count
void PrecissionTimer::pause() {
	_elapsedTime = getTicks();
	_paused = true;
	_started = true;
}

//Unpause count
void PrecissionTimer::unpause() {
	_paused = false;
	_started = true;
}


void PrecissionTimer::init() {
#ifdef PLATFORM_WIN32
	//Check if hardware permits precission timer
	if (QueryPerformanceFrequency((LARGE_INTEGER *)&mFrequency)) {
		_highRes = true;
	}
#endif

#if defined (PLATFORM_IOS) || defined (PLATFORM_OSX)
	mach_timebase_info(&mTimeBaseInfo);
	mStartTime = mach_absolute_time();
#endif

#ifdef PLATFORM_LINUX
    clock_gettime(CLOCK_MONOTONIC, &linux_start);
	mStartTime = (((uint64_t) linux_start.tv_sec) * 1000000000ULL) + (uint64_t) linux_start.tv_nsec;
#endif
}

/** @endcond */

//WARNING FROM __int64 to double
#if defined (PLATFORM_WIN32)
#pragma warning(pop)  //Restore warnings state
#endif
