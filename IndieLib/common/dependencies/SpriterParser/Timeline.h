/*****************************************************************************************
 * File: Timeline.h
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
    
    
    // ----- Public Sets ------


    TimelineKey* addKey(int id, int time, int spin);


    // ----- Public Gets ------
    
    
    std::vector <TimelineKey *>* getKeys() {
        return _keyList;
    }
    


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