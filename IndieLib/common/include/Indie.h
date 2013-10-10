/*****************************************************************************************
 * File: IndieLib.h
 * Desc: Includes
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


#ifndef _INDIELIB_
#define _INDIELIB_

/*
    TODO: This documentation should probably be moved somewhere else...
 
    MODIFICATIONS FROM ORIGINAL:
        - IND_RENDER METHOD: BlitTriangleList Added
        - IND_RENDER: Method SetForPrimitive was not adding IND_ALPHA to make transparencies when blitting!
        - IND_Entity2d: Method GetLayer() added, added int variable mLayer (to get layer of entity easily)
                        Method SetLayer(int newlayer) added
        - IND_Entity2d: Method Add(int layer, IND_Entity2d* entity) changed to make a setlayer of the entity
        - IND_Entity2d: Method SetSequence (int pSequence) didnt restart previous animation when setting a new one
                                                           this caused me a lot of problems having 1 replay more when
                                                           changing animations and replaying no times (pReplays 1)
                                                           Also needed to put "Active" flag right on to 1, as from there, it
                                                           must be considered that animation is active (coherence of flags)
        - IND_Entity2d: Method SetNumReplays(int pReplays  Had to change it as now, to replay, you put how many times
                                                            you want to replay. If parameter is 0, no replays are made
        - IND_RenderText2d: Method BlitText() Changed it to be able to scale the related entity and make the text bigger as it
                                              should be. Before scale didnt affect to text
*/


// Namespace
#include <fstream>
using namespace std;

//Version defs
#include "IndieVersion.h"

// Platform definitions
#include "IndiePlatforms.h"

// Math
#include "IND_Math.h"
#include "IND_Vector2.h"
#include "IND_Vector3.h"

// Defines and types
#include "Defines.h"

// Init/End
#include "IndieLib.h"

// Display
#include "IND_Window.h"

// Render
#include "IND_Render.h"

// Timer
#include "IND_Timer.h"

// Tmx Maps
#include "IND_TmxMapManager.h"

// Spriter animations
#include "IND_SpriterManager.h"

// Entities
#include "IND_Entity2dManager.h"
//TODO: SUPPORT FOR 3D ENTITIES IN OPENGL
//#include "IND_Entity3dManager.h"
class IND_Entity3dManager;

// Input
#include "IND_Input.h"

// 2d Objects
#include "IND_ImageManager.h"
#include "IND_SurfaceManager.h"
#include "IND_AnimationManager.h"
#include "IND_FontManager.h"

// 3d Objects
//TODO: SUPPORT FOR 3D ENTITIES IN OPENGL
//#include "IND_3dMeshManager.h"
class IND_3dMeshManager;

// Cameras
#include "IND_Camera2d.h"
#include "IND_Camera3d.h"

// Common
//TODO: SUPPORT FOR LIGHT IN OPENGL
//#include "IND_LightManager.h"
class IND_LightManager;

#endif // _INDIELIB_


