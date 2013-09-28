/*****************************************************************************************
 * File: Line.cpp
 * Desc: Spriter entity's timeline object
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