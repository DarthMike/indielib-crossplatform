/*****************************************************************************************
 * File: MainlineKey.h
 * Desc: Spriter entity's mainline's Key object
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
    
    std::vector <MainlineObjectref *>* getObjectrefs(){
        return _objectrefList;
    }
    
    std::vector <MainlineObject *>* getObjects(){
        return _objectList;
    }

    
    
private:

    // ----- Entity attributes -----

    int                                 _id;
    int                                 _time;
    std::vector <MainlineObjectref *>   *_objectrefList;
    std::vector <MainlineObject *>      *_objectList;

};

#endif // _MAINLINEKEY_