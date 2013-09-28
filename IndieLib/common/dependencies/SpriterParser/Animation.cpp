/*****************************************************************************************
 * File: Animation.cpp
 * Desc: Spriter entity's animation object
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

#include "Animation.h"


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

Animation::Animation(int id, const char* name, int length, const char* looping, int loop_to) {
    _id             = id;
    _name           = const_cast<char *>(name);
    _length         = length;
    _looping        = const_cast<char *>(looping);
    _loop_to        = loop_to;
    _mainline       = new Mainline();
    _timelineList   = new std::vector <Timeline *>;
}

Animation::~Animation() {
    delete _mainline;
    delete [] _timelineList;
}

// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

Timeline* Animation::addTimeline(int id, const char* name, const char* object_type, const char* variable_type, const char* usage) {
	Timeline *timelinePtr = new Timeline(id, name, object_type, variable_type, usage);
	_timelineList->insert(_timelineList->begin() + id, timelinePtr);
	
	return timelinePtr;
}