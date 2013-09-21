/*****************************************************************************************
 * File: MainlineKey.cpp
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
