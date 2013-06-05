/*****************************************************************************************
 * File: IND_SpriterEntity.cpp
 * Desc: Spriter entity object
 *****************************************************************************************/

/*
 .... something about license
*/

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
    _images     = new map<Fileref*, IND_Image *>();
    _animations = new std::vector<Animation *>();
}

void IND_SpriterEntity::addImage(const char *folderId, const char *fileId, IND_Image *pImage) {
    Fileref* ref = new Fileref();
    ref->first = static_cast<unsigned int>(*folderId);
    ref->second = static_cast<unsigned int>(*fileId);
    
    _images->insert(make_pair(ref, pImage));
}

Animation* IND_SpriterEntity::addAnimation(int id, const char* name, int length, const char* looping, int loop_to) {
    Animation* aniPtr = new Animation(id, name, length, looping, loop_to);
    _animations->push_back(aniPtr);
    
    return aniPtr;
}



/** @endcond */
