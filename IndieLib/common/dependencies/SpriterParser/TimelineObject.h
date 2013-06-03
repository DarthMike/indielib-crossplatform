/*****************************************************************************************
 * File: TimelineObject.h
 * Desc: Spriter entity's timeline object structure
 *****************************************************************************************/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <michael@visualdesign.dk> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Michael Fogh Kristensen
 * ----------------------------------------------------------------------------
 */

#ifndef _TIMELINEOBJECT_
#define _TIMELINEOBJECT_

struct TimelineObject {
	
	int		folder;
	int		file;
	float	x;
	float	y;
	float	pivot_x;
    float	pivot_y;
	float	angle;
	float	scale_x;
	float	scale_y;
	float	a;
	
};

#endif // _TIMELINEOBJECT_