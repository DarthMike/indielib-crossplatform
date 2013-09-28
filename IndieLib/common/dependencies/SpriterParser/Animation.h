/*****************************************************************************************
 * File: Animation.h
 * Desc: Spriter entity's animation object
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


#ifndef _ANIMATION_
#define _ANIMATION_

// ----- Includes -----

#include "Mainline.h"
#include "Timeline.h"


// --------------------------------------------------------------------------------
//									 Animation
// --------------------------------------------------------------------------------


class Animation {

public:

Animation(int id, const char* name, int length, const char* looping, int loop_to);

~Animation();


 // ----- Public Gets ------
 
 Mainline* getMainline(){
 	return _mainline;
 }
 
std::vector <Timeline *>* getTimeLines(){
 	return _timelineList;
 }


 // ----- Public Sets ------

 Timeline* addTimeline(int id, const char* name, const char* object_type, const char* variable_type, const char* usage);


private:

 // ----- Entity attributes -----
 int                         _id;
 char*                       _name;
 int                         _length;
 char*                       _looping;
 int                         _loop_to;
 Mainline                    *_mainline;
 std::vector <Timeline *>    *_timelineList;

};

#endif // _ANIMATION_