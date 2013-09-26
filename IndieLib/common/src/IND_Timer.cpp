/*****************************************************************************************
 * File: IND_Timer.h
 * Desc: Class for measuring time.
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
#include "IND_Timer.h"
#include "PrecissionTimer.h"

// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/*
==================
Init
==================
*/
IND_Timer::IND_Timer() {
	//Create precission timer
	_precissionTimer = new PrecissionTimer();
}

/*
==================
Destruction
==================
*/
IND_Timer::~IND_Timer() {
	DISPOSE(_precissionTimer);
}

// --------------------------------------------------------------------------------
//									   Public methods
// --------------------------------------------------------------------------------

/**
 * Initializes the timer. Just after calling this method the timer will start to measure the time.
 */
void IND_Timer::start() {
	_precissionTimer->start();
}

/**
 * Stops the timer.
 */
void IND_Timer::stop() {
	_precissionTimer->stop();
}

/**
 * Pauses the timer.
 */
void IND_Timer::pause() {
	_precissionTimer->pause();
}

/**
 * Unpauses the timer.
 */
void IND_Timer::unpause() {
	_precissionTimer->unpause();
}

/**
 * Returns the number of millisecods passed since the last call to IND_Timer::start() or
 * IND_Timer::init().
 */
double IND_Timer::getTicks() {
	return (_precissionTimer->getTicks());
}

/**
 * Returns true if the timer is started.
 */
bool IND_Timer::isStarted() {
	return (_precissionTimer->isStarted());
}

/**
 * Returns true if the timer is paused.
 */
bool IND_Timer::isPaused() {
	return (_precissionTimer->isPaused());
}
