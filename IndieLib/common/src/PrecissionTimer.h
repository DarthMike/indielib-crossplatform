/*
    Filename: PrecissionTimer.h
    Copyright: Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)
    Description: Class to encapsulate a precission timer (if hardware permits it)
    Comments: Supported native platforms are Windows and iOS (MacOS)
    Attribution:
    License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
             Nevertheless it would be nice if you tell me you are using something I made, just for curiosity
*/

#ifndef _PRECISSIONTIMER
#define _PRECISSIONTIMER

//Library dependencies
#include "Defines.h"

//Timer for Windows
#if defined (PLATFORM_WIN32)
#include <windows.h>   //Queryperformance counter
#endif

//Timer for iOS
#if defined (PLATFORM_IOS)
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>
#endif

//Timer for Linux (should work for BSD also..)
#if defined (PLATFORM_LINUX)
#include <sys/time.h>
#include <stdint.h>
#endif

//Timer for OSX
#if defined (PLATFORM_OSX)
#include <sys/time.h>
#include <stdint.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

/** @cond DOCUMENT_PRIVATEAPI */

class PrecissionTimer {
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	PrecissionTimer():
		_highRes(false),
		_started(false),
		_paused(false),
		_elapsedTime(0) {
		init();
	}
	~PrecissionTimer() {
	}
	//----- GET/SET FUNCTIONS -----
	double getTicks();

	bool isStarted()        {
		return _started;
	}
	bool isPaused()         {
		return _paused;
	}
	//----- OTHER FUNCTIONS -----
	void start();
	void stop();
	void pause();
	void unpause();
	//----- PUBLIC VARIABLES ------

private:
	//----- INTERNAL VARIABLES -----
	bool _highRes;    		//Using High-res timer?
	bool _started;    		//Started status
	bool _paused;     		//Paused Status

#ifdef PLATFORM_WIN32
	__int64 mStartTime;		//Start time (Windows- specific format...)
	__int64 mFinalTime;		//End time (Windows- specific format...)
	__int64 _elapsedTime;	//Elapsed time (Windows- specific format...)
	__int64 mFrequency; 	//High-res timer frequency
#endif

#if defined (PLATFORM_IOS) || defined (PLATFORM_OSX)
	uint64_t mStartTime;    //Start time (MACOS - specific)
	uint64_t mFinalTime;    //End time (MACOS - specific)
	uint64_t _elapsedTime;  //Elapsed time (MACOS - specific)
	mach_timebase_info_data_t mTimeBaseInfo;  //TimeBase (MACOS - specific)
#endif

#ifdef PLATFORM_LINUX
	uint64_t mStartTime;	//Start time (linux - specific)
	uint64_t mFinalTime;	//End time (linux - specific)
	uint64_t _elapsedTime;	//Elapsed time (linux - specific)
	struct timespec linux_start, linux_end;
#endif

	//----- INTERNAL FUNCTIONS -----
	void init();
};

/** @endcond */

#endif
