/*****************************************************************************************
 * File: IND_Timer.h
 * Desc: Class for measuring the pass of time
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
