/*****************************************************************************************
 * File: Mainline.cpp
 * Desc: Spriter entity's mainline object
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

#include "Mainline.h"


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------
Mainline::Mainline() {
    _keyList = new std::vector <MainlineKey *>;
}

Mainline::~Mainline() {
//TODO: delete the array
}

// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

MainlineKey* Mainline::addKey(int id, int time) {
	MainlineKey *keyPtr = new MainlineKey(id, time);
    _keyList->push_back(keyPtr);		// TODO : use index instead
    
    return keyPtr;
}