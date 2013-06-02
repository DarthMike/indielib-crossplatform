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
//TODO: delete the two arrays ...
}

// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

void MainlineKey::addObjectref(int id, int timeline, int key, int z_index) {
//TODO: create Objectref and append it to the list
}

void MainlineKey::addObject(int id, char* object_type, int folder, int file, float x, float y, float pivot_x, float pivot_y, float angle, float scale_x, float scale_y, float a) {
//TODO: create Object and append it to the list
}
