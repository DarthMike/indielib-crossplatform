/*****************************************************************************************
 * Desc: Defines... are evil :)
 *
 * gametuto.com - Javier López López (javilop.com)
 *
 *****************************************************************************************
 *
 * Creative Commons - Attribution 3.0 Unported
 * You are free:
 *	to Share — to copy, distribute and transmit the work
 *	to Remix — to adapt the work
 *
 * Under the following conditions:
 * Attribution. You must attribute the work in the manner specified by the author or licensor 
 * (but not in any way that suggests that they endorse you or your use of the work).
 *
 *****************************************************************************************/

#ifndef _EDITOR_DEFINES_
#define _EDITOR_DEFINES_

#include "IndiePlatforms.h"
#include "Ind_Entity2d.h"
#include "IND_Surface.h"


// --------------------------------------------------------------------------------
//										Defines
// --------------------------------------------------------------------------------

// ' ' needs to have dll-interface to be used by clients of class ' '
#pragma warning (disable : 4800)	

// Brush entity, used for each of the brushes for droping backdrops to the screen
struct structBackDropBrushes
{	
	IND_Entity2d* mEntity; 			// Entity
	int mId;						// Brush Id
};
typedef struct structBackDropBrushes BRUSH;


// Brush surface, used for having a vector of [surfaces, id] of elements loaded in "Resources" class. This surfaces
// can be converted later to brushes
struct structBackDropSurfaces
{	
	IND_Surface* mSurface; 			// Surface
	int mId;						// Surface Id
};
typedef struct structBackDropSurfaces SURFACEWITHID;


// ----- Defines -----

const int NUM_EDITOR_LAYERS		= 13;			// Number of backdrop layers
const int BRUSH_LAYER			= 13;			// Number of the layer where the backdrop brush is rendered
const int GUI_LAYER				= 14;			// Number of the layer where the gui (text) is rendered
const int NUM_INFINITE			= 99999999L;

// "Distancy" between layers (affects camera zooming)
const int DIST_CAMERA_B			= 430;
const int DIST_CAMERA_N			= 420;
const int DIST_CAMERA_M			= 410;
const int DIST_CAMERA_LAYERS	= 400;

// Layer speed (affects camera tranlation)
const int CAMERA_SPEED_B		= 300;
const int CAMERA_SPEED_N		= 330;
const int CAMERA_SPEED_M		= 360;
const int CAMERA_SPEED_LAYERS	= 400;

#endif // _EDITOR_DEFINES_
