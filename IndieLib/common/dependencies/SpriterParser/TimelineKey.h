/*****************************************************************************************
 * File: TimelineKey.h
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