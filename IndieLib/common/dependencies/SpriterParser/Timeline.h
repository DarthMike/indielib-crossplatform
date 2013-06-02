/*****************************************************************************************
 * File: Timeline.h
 * Desc: Spriter entity's timeline object
 *****************************************************************************************/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <michael@visualdesign.dk> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Michael Fogh Kristensen
 * ----------------------------------------------------------------------------
 */

#ifndef _TIMELINE_
#define _TIMELINE_

// ----- Includes -----

#include "TimelineKey.h"
#include <vector>


// --------------------------------------------------------------------------------
//									 Timeline
// --------------------------------------------------------------------------------


class Timeline {

public:

 Timeline(int id, const char* name, const char* object_type, const char* variable_type, const char* usage);
 ~Timeline();

 TimelineKey* addKey(int id, int time, int spin);


private:

// ----- Entity attributes -----

int                			_id;
char*                		_name;
char*						_object_type;
char*						_variable_type;
char*						_usage;
std::vector <TimelineKey *>	*_keyList;

};

#endif // _TIMELINE_