/*****************************************************************************************
 * File: IND_SpriterEntity.cpp
 * Desc: Spriter entity object
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

#include "Global.h"
#include "IND_SpriterEntity.h"
#include "dependencies/SpriterParser/Animation.h"


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------
/**
 *  Factory method to build instances of this class.
 *
 *  You should never allocate instances by calling new or allocating objects in the stack.
 *  All memory related to this instance is managed by Indielib internally, and you must add it or remove it from
 *  a manager.
 *
 *  @return A correctly initialized instance
 */
IND_SpriterEntity* IND_SpriterEntity::newSpriterEntity() {
    return new IND_SpriterEntity();
}

void IND_SpriterEntity::destroy() {
    delete this;
}


IND_SpriterEntity::IND_SpriterEntity()  {
	initAttrib();
}


IND_SpriterEntity::~IND_SpriterEntity() {
}


// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

/**
 * Starts the animation
 * @param animation			The index of the animation to play.
 */
void IND_SpriterEntity::playAnimation(int animation/*, IND_Render *render*/) { // TODO: MFK maybe animation name instead?
    _currentAnimation   = animation;
    _currentKey         = 0;
    _currentTime        = 0;
}

/**
 * Stops the animation
 */
void IND_SpriterEntity::stopAnimation() {
    _currentAnimation   = -1;
    _currentKey         = -1;
    _currentTime        = -1;
}


// --------------------------------------------------------------------------------
//									 Private methods
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
==================
Attributes initialization
==================
*/
void IND_SpriterEntity::initAttrib() {
    _id         = NULL;
	_name       = NULL;
    _surfaces   = new SurfaceToFileMap;
    _animations = new std::vector<Animation *>();
    
    _currentAnimation       = -1;       // TODO: ??
    _currentKey             = -1;       // TODO: ??
    _currentTime            = -1;       // TODO: ??
    
    _drawBones              = false;    // TODO: support this in a later version
    _drawObjectpositions    = false;    // TODO: support this in a later version
}

    
// ----- Parsing -----


void IND_SpriterEntity::addSurface(int folderId, int fileId, IND_Surface *pSurface) {
    Fileref ref(static_cast<unsigned int>(folderId),static_cast<unsigned int>(fileId));
    
    _surfaces->insert(make_pair(ref, pSurface));
}

Animation* IND_SpriterEntity::addAnimation(int id, const char* name, int length, const char* looping, int loop_to) {
    Animation* aniPtr = new Animation(id, name, length, looping, loop_to);
    _animations->push_back(aniPtr);
    
    return aniPtr;
}



/** @endcond */
