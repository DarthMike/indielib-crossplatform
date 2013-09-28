/*****************************************************************************************
 * File: IND_Math.cpp
 * Desc: Math methods
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


// ----- Includes -----

#include "Global.h"
#include "Defines.h"
#include "IND_Math.h"

#ifdef PLATFORM_LINUX
#include <sys/time.h> 
#endif

#if defined (PLATFORM_IOS) || defined (PLATFORM_OSX)
#include <sys/time.h>
#include <mach/clock.h>
#include <mach/mach.h>
#endif

// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
 Initializes the object.
 
 @return true if initialized correctly, false otherwise
 */
bool IND_Math::init() {
	end();
	freeVars();
 
    //----- Random seed ------
    
#ifdef PLATFORM_LINUX
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	srand(tp.tv_nsec);
#endif
    
    // OS X does not have clock_gettime, use clock_get_time
    // Great solution from StackOverflow:http://stackoverflow.com/questions/5167269/clock-gettime-alternative-in-mac-os-x
#if defined (PLATFORM_IOS) || defined (PLATFORM_OSX) 
    struct timespec tp;
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    tp.tv_sec = mts.tv_sec;
    tp.tv_nsec = mts.tv_nsec;
    srand(static_cast<unsigned>(tp.tv_nsec));
#endif
    
#ifdef PLATFORM_WIN32	 
	srand(GetTickCount());
#endif
	_ok = true;

	return _ok;
}


/**
 * Frees the manager and all the objects that it contains.
 */
void IND_Math::end() {
	if (_ok) {

		freeVars();

		_ok = false;
	}
}

/**
 Returns state of initialization.
 @return  Will give true if object initialized correctly, false otherwise
 */
bool IND_Math::isOK() {
    return _ok;
}

// --------------------------------------------------------------------------------
//										 Public methods
// --------------------------------------------------------------------------------

/**
 * Sets the seed for random numbers
 * @param pNum a number to initiate random seed with. Should be different always, like a time() value
 */
void IND_Math::randSeed(long pNum) {
	srand(static_cast<unsigned>(pNum));
}


/**
 * Compares if a number is power of two
 * @param pN Number to check
 * @return true if the number is power of two, false otherwise
 */
bool IND_Math::isPowerOfTwo(long pN) {
	while (pN > 1L) {
		if (pN % 2L) break;
		pN >>= 1L;
	}

	return (pN == 1L);
}

// --------------------------------------------------------------------------------
//									 Private methods
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
==================
Init vars
==================
*/
void IND_Math::initVars() {

}

/*
==================
Free memory
==================
*/
void IND_Math::freeVars() {

}

/** @endcond */
