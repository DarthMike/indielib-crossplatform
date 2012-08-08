/*****************************************************************************************
 * File: IND_Timer.h
 * Desc: Class for measuring time.
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (javierlopezpro@gmail.com)
MODIFIED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)

This library is free software; you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
Suite 330, Boston, MA 02111-1307 USA
*/

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
