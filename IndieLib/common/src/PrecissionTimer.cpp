/****************************************************************************************************************
 *   File: PrecissionTimer.cpp
 *   Desc: Class to encapsulate a precission timer (if hardware permits it)
 ****************************************************************************************************************/

/*
     Copyright: Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)
     Comments : You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
                Nevertheless it would be nice if you tell me you are using something I made, just for curiosity
*/

#include "PrecissionTimer.h"
#if defined (PLATFORM_WIN32)
#if defined (_MSC_VER)
#pragma warning(push)  //Store warnings state
#pragma warning(disable : 4244)  //Disable warning to conversion between __int64 and double
#endif
#endif

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
	 uint64_t start_time = ((((uint64_t)linux_start.tv_sec) * 1000000000ULL) + (uint64_t) linux_start.tv_nsec);
     uint64_t end_time = ((((uint64_t) linux_end.tv_sec) * 1000000000ULL) + (uint64_t) linux_end.tv_nsec); 
     _elapsedTime = end_time - start_time; 
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

//WARNING FROM __int64 to double
#if defined (PLATFORM_WIN32)
#pragma warning(pop)  //Restore warnings state
#endif
