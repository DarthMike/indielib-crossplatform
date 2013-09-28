/*****************************************************************************************
 * File: IND_Timer.h
 * Desc: Class for measuring the pass of time
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


#ifndef _IND_TIMER_
#define _IND_TIMER_

#include "Defines.h"

// ---- Forward declarations ----
class PrecissionTimer;
// --------------------------------------------------------------------------------
//									   IND_Timer
// --------------------------------------------------------------------------------

/**
@defgroup IND_Timer IND_Timer
@ingroup Timer
IND_Timer class for measuring the time. Click in IND_Timer to see all the methods of this class.
*/
/**@{*/

/**
@b IND_Timer is an object that will help you to measure the passing of time. Click in IND_Timer to see all the methods of this class.
*/
class LIB_EXP IND_Timer {
public:

	// ----- Init/End -----

	IND_Timer();
	~IND_Timer();
	// ----- Public methods -----

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	double getTicks();

	// ----- Public gets -----

	// This function returns true if the timer has been started
	bool isStarted();
	// This function returns true if the timer is paused
	bool isPaused();

private:
	/** @cond DOCUMENT_PRIVATEAPI */
	// ----- Private -----
	//Internal precission timer
	PrecissionTimer *_precissionTimer;
    /** @endcond */
};
/**@}*/

#endif // _IND_TIMER_
