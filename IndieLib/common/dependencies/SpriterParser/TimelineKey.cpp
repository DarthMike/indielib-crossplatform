/*****************************************************************************************
 * File: TimelineKey.cpp
 * Desc: Spriter entity's timeline's Key object
 *****************************************************************************************/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <michael@visualdesign.dk> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Michael Fogh Kristensen
 * ----------------------------------------------------------------------------
 */


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
