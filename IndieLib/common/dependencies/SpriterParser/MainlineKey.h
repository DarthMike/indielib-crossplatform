/*****************************************************************************************
 * File: MainlineKey.h
 * Desc: Spriter entity's mainline's Key object
 *****************************************************************************************/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <michael@visualdesign.dk> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Michael Fogh Kristensen
 * ----------------------------------------------------------------------------
 */

#ifndef _MAINLINEKEY_
#define _MAINLINEKEY_

// ----- Includes -----

#include "MainlineObjectref.h"
#include "MainlineObject.h"
#include <vector>


// --------------------------------------------------------------------------------
//									 MainlineKey
// --------------------------------------------------------------------------------


class MainlineKey {

public:
    
    MainlineKey(int id, int time);
    ~MainlineKey();
    
    
    // ----- Public Sets ------
    
    void addObjectref(int id, int timeline, int key, int z_index);
    void addObject(int id, const char* object_type, int folder, int file, float x, float y, float pivot_x, float pivot_y, float angle, float scale_x, float scale_y, float a);
    
    // ----- Public Gets ------
    
    int getId(){
        return _id;
    }
    
    int getTime(){
        return _time;
    }
    
private:

    // ----- Entity attributes -----

    int                                 _id;
    int                                 _time;
    std::vector <MainlineObjectref *>   *_objectrefList;
    std::vector <MainlineObject *>      *_objectList;

};

#endif // _MAINLINEKEY_