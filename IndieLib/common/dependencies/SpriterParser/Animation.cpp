/*****************************************************************************************
 * File: Animation.cpp
 * Desc: Spriter entity's animation object
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

#include "Animation.h"


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

Animation::Animation(int id, const char* name, int length, const char* looping, int loop_to) {
    _id			= id;
    _name		= const_cast<char *>(name);
    _length		= length;
    _looping	= const_cast<char *>(looping);
    _loop_to	= loop_to;
    _mainline	= new Mainline();
    _timelines	= new std::vector <Timeline *>;
}

Animation::~Animation() {
//TODO: delete the one (two) arrays ...
}

// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

Timeline* Animation::addTimeline(int id, const char* name, const char* object_type, const char* variable_type, const char* usage) {
	Timeline *line = new Timeline(id, name, object_type, variable_type, usage);
	_timelines->push_back(line);  // TODO: use index instead
	
	return line;
}