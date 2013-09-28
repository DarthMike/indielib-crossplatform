/*****************************************************************************************
 * File: TimelineKey.cpp
 * Desc: Spriter entity's timeline's Key object
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

#include "TimelineKey.h"


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

TimelineKey::TimelineKey(int id, int time, int spin) {
    _id			= id;
    _time		= time;
    _spin		= spin;
    _objectList	= new std::vector <TimelineObject *>;
}

TimelineKey::~TimelineKey() {
    delete [] _objectList; // TODO is this really only one instance instead?
}

// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

void TimelineKey::addTimelineObject(int folder, int file, float x, float y, float pivot_x, float pivot_y, float angle, float scale_x, float scale_y, float a) {
	TimelineObject *timelineObjectPtr = new TimelineObject();
    timelineObjectPtr->folder = folder;
    timelineObjectPtr->file = file;
    timelineObjectPtr->x = x;
    timelineObjectPtr->y = y;
    timelineObjectPtr->pivot_x = pivot_x;
    timelineObjectPtr->pivot_y = pivot_y;
    timelineObjectPtr->angle = angle;
    timelineObjectPtr->scale_x = scale_x;
    timelineObjectPtr->scale_y = scale_y;
    timelineObjectPtr->a = a;
    
    _objectList->insert(_objectList->begin() + 0, timelineObjectPtr); //FIXME: no fixed index!!! - checkdesign documents, has this object possible sibling?
}
