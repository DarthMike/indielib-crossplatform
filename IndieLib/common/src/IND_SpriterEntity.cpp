/*****************************************************************************************
 * File: IND_SpriterEntity.cpp
 * Desc: Spriter entity object
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

void IND_SpriterEntity::playAnimation(int animation) { // TODO: MFK maybe animation name instead?
    _currentAnimation   = animation;
    _currentKey         = 0;
    _currentTime        = 0;
    
    update(0); // we need to handle timing later...
}


void IND_SpriterEntity::update(int deltaTime) {
    draw(1.0f, 1.0f, 1.0f, 1.0f, 1.0f); // TODO: parameters not used yet
}

void  IND_SpriterEntity::draw(float x, float y, float angle, float scale_x, float scale_y) {
    
    if (_currentAnimation < 0 || _currentKey < 0 || _currentTime < 0 ){
        return;
    }
    
    std::vector <MainlineObject *> *mainlineObjects = getAnimations()->at(_currentAnimation)->getMainline()->getKeys()->at(_currentKey)->getObjects();
    
    std::vector <MainlineObjectref *> *mainlineObjectrefs = getAnimations()->at(_currentAnimation)->getMainline()->getKeys()->at(_currentKey)->getObjectrefs();

    
    for (unsigned i=0; i < mainlineObjects->size(); i++) {
        drawPersistentObject(x, y, angle, scale_x, scale_y);
    }
    
    
    for (unsigned i=0; i < mainlineObjectrefs->size(); i++) {
        drawTransientObject(x, y, angle, scale_x, scale_y, mainlineObjectrefs->at(i));
    }
    

}

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
    _images     = new map<Fileref*, IND_Image *>();
    _animations = new std::vector<Animation *>();
    
    _currentAnimation       = -1;       // TODO: ??
    _currentKey             = -1;       // TODO: ??
    _currentTime            = -1;       // TODO: ??
    
    _drawBones              = false;    // TODO: support this in a later version
    _drawObjectpositions    = false;    // TODO: support this in a later version
}


// ----- Rendering -----


void  IND_SpriterEntity::drawTransientObject(float x, float y, float angle, float scale_x, float scale_y, MainlineObjectref *mObjectref) {
    TimelineObject *tObject = getTimelineObject(mObjectref->timeline, mObjectref->key);
    
    IND_Image *image = getImage(tObject->folder, tObject->file);
    
    g_debug->header(image->getName(), DebugApi::LogHeaderError); //TODO: MFK continue from here.... currently we are missing the images in the list, and whoops we get an acces vialation ... need to look at the image parsing
    
}

void  IND_SpriterEntity::drawPersistentObject(float x, float y, float angle, float scale_x, float scale_y) {
    // TODO: MFK, implement this
}

void IND_SpriterEntity::drawBone(float x, float y, float angle, float scale_x, float scale_y) {
    // TODO: support this in a later version
}


TimelineObject* IND_SpriterEntity::getTimelineObject(int timelineId, int keyId) {
    return getAnimations()->at(_currentAnimation)->getTimeLines()->at(timelineId)->getKeys()->at(keyId)->getObjects()->at(0); // TODO : is there allways one obejct here? ( we're using an array )
}

IND_Image* IND_SpriterEntity::getImage(int folderId, int fileId) {
    Fileref* ref = new Fileref();
    ref->first = static_cast<unsigned int>(folderId);  // TODO: this is probably wrong ... compare with insert
    ref->second = static_cast<unsigned int>(fileId);   // TODO: this is probably wrong ... compare with insert
    
    auto res = _images->find(ref);
    
    // Find the first matching key.
    //itr = mmp.find(n);
    
    if(res != _images->end()) {
        //cout << "the numbers for " << n << ": " << endl;
        //do {
        //    cout << " " << itr->second << endl;
        //    ++itr;
        //} while (itr != mmp.upper_bound(n));
        return res->second;
    }
    else {
        //cout << "No entry for " << n << " found." << endl;
        return 0;
    }
}


    
// ----- Parsing -----


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
