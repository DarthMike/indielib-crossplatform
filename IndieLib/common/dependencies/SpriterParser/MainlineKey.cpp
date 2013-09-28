/*****************************************************************************************
 * File: MainlineKey.cpp
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


// ----- Includes -----

#include "MainlineKey.h"


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

MainlineKey::MainlineKey(int id, int time) {
    _id				= id;
    _time			= time;
    _objectrefList	= new std::vector <MainlineObjectref *>;
    _objectList		= new std::vector <MainlineObject *>;
}

MainlineKey::~MainlineKey() {
    delete [] _objectrefList;
    delete [] _objectList;
}

// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

void MainlineKey::addObjectref(int id, int timeline, int key, int z_index) {
	MainlineObjectref *mainlineObjectrefPtr = new MainlineObjectref();
    mainlineObjectrefPtr->id = id;
    mainlineObjectrefPtr->timeline = timeline;
    mainlineObjectrefPtr->key = key;
    mainlineObjectrefPtr->z_index = z_index;
    
    _objectrefList->insert(_objectrefList->begin() + id, mainlineObjectrefPtr);
}

void MainlineKey::addObject(int id, const char* object_type, int folder, int file, float x, float y, float pivot_x, float pivot_y, float angle, float scale_x, float scale_y, float a) {
    MainlineObject *mainlineObjectPtr = new MainlineObject();
    mainlineObjectPtr->id = id;
    mainlineObjectPtr->object_type = const_cast<char *>(object_type);
    mainlineObjectPtr->folder = folder;
    mainlineObjectPtr->file = file;
    mainlineObjectPtr->x = x;
    mainlineObjectPtr->y = y;
    mainlineObjectPtr->pivot_x = pivot_x;
    mainlineObjectPtr->pivot_y = pivot_y;
    mainlineObjectPtr->angle = angle;
    mainlineObjectPtr->scale_x = scale_x;
    mainlineObjectPtr->scale_y = scale_y;
    mainlineObjectPtr->a = a;
    
    _objectList->insert(_objectList->begin() + id, mainlineObjectPtr);
}
