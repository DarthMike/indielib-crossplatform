/*****************************************************************************************
 * File: TimelineKey.h
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


#ifndef _TIMELINEKEY_
#define _TIMELINEKEY_

// ----- Includes -----

#include "TimelineObject.h"
#include <vector>


// --------------------------------------------------------------------------------
//									 TimelineKey
// --------------------------------------------------------------------------------


class TimelineKey {

public:
    
    TimelineKey(int id, int time, int spin);
    ~TimelineKey();
    
    
    // ----- Public Sets ------
    
    void addTimelineObject(int folder, int file, float x, float y, float pivot_x, float pivot_y, float angle, float scale_x, float scale_y, float a);
    
    // ----- Public Gets ------
    
    int getId(){
        return _id;
    }
    
    int getTime(){
        return _time;
    }
    
    int getSpin(){
        return _spin;
    }
    
    std::vector <TimelineObject *>* getObjects(){
        return _objectList;
    }
    
private:

    // ----- Entity attributes -----

    int                             _id;
    int                             _time;
    int                             _spin;
    std::vector <TimelineObject *>	*_objectList;

};

#endif // _TIMELINEKEY_