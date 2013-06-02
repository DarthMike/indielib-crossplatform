/*****************************************************************************************
 * File: Animation.h
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

#ifndef _ANIMATION_
#define _ANIMATION_

// ----- Includes -----

#include "Mainline.h"
#include "Timeline.h"


// --------------------------------------------------------------------------------
//									 ANIMATION
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
 	return _timelines;
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
 std::vector <Timeline *>    *_timelines;

};

#endif // _ANIMATION_