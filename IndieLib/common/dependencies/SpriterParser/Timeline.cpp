/*****************************************************************************************
 * File: Line.cpp
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

// ----- Includes -----

#include "Timeline.h"


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------
Timeline::Timeline(int id, const char* name, const char* object_type, const char* variable_type, const char* usage) {
    _id				= id;
    _name 			= const_cast<char *>(name);
    _object_type 	= const_cast<char *>(object_type);
    _variable_type	= const_cast<char *>(variable_type);
    _usage			= const_cast<char *>(usage);
    _keyList 		= new std::vector <TimelineKey *>;
}

Timeline::~Timeline() {
    delete [] _keyList;
}

// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

TimelineKey* Timeline::addKey(int id, int time, int spin) {
	TimelineKey *keyPtr = new TimelineKey(id, time, (spin == 0 ? 1 : spin));
    _keyList->insert(_keyList->begin() + id, keyPtr);
    
    return keyPtr;
}