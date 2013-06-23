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

void IND_SpriterEntity::playAnimation(int animation, IND_Render *render) { // TODO: MFK maybe animation name instead?
    _currentAnimation   = animation;
    _currentKey         = 0;
    _currentTime        = 0;
    _render             = render;
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
    _render             = NULL;

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
    
    _render                 = NULL;
}


// ----- Rendering -----


void  IND_SpriterEntity::drawTransientObject(float x, float y, float angle, float scale_x, float scale_y, MainlineObjectref *mObjectref) {
    TimelineObject *tObject = getTimelineObject(mObjectref->timeline, mObjectref->key);
    
    IND_Surface *surface = getSurface(tObject->folder, tObject->file);
    
    g_debug->header(surface->getTypeString(), DebugApi::LogHeaderError); //TODO: MFK continue from here....
    
    IND_Matrix *mMatrix = new IND_Matrix();
    
    
    _render->setTransform2d(tObject->x,                     // x pos  note: we start in 0,0 (corner of screen)
                                tObject->y,                      // y pos
                                0,                          // Angle x
                                0,                          // Angle y
                                0,                          // Angle z
                                1,                          // Scale x
                                1,                          // Scale y
                                0,                          // Axis cal x
                                0,                          // Axis cal y
                                0,                          // Mirror x
                                0,                          // Mirror y
                                0,                          // Width
                                0,                          // Height
                                mMatrix);                   // Matrix in wich the transformation will be applied (optional)
    
    // We apply the color, blending and culling transformations.
    _render->setRainbow2d(IND_ALPHA,                    // IND_Type
                              1,                            // Back face culling 0/1 => off / on
                              0,                            // Mirror x
                              0,                            // Mirror y
                              IND_FILTER_LINEAR,            // IND_Filter
                              255,                          // R Component	for tinting
                              255,                          // G Component	for tinting
                              255,                          // B Component	for tinting
                              255,                          // A Component	for tinting
                              0,                            // R Component	for fading to a color
                              0,                            // G Component	for fading to a color
                              0,                            // B Component	for fading to a color
                              255,                          // Amount of fading
                              IND_SRCALPHA,                 // IND_BlendingType (source)
                              IND_INVSRCALPHA);             // IND_BlendingType (destination)
    
    
    
    // Blit the IND_Surface
    _render->blitRegionSurface(surface, 0, 0, surface->getWidth(), surface->getHeight());
    
    
    delete mMatrix; // TODO : optimize this...
    
    
    
}

void  IND_SpriterEntity::drawPersistentObject(float x, float y, float angle, float scale_x, float scale_y) {
    // TODO: MFK, implement this
}

void IND_SpriterEntity::drawBone(float x, float y, float angle, float scale_x, float scale_y) {
    // TODO: support this in a later version
}


TimelineObject* IND_SpriterEntity::getTimelineObject(int timelineId, int keyId) {
    return getAnimations()->at(_currentAnimation)->getTimeLines()->at(timelineId)->getKeys()->at(keyId)->getObjects()->at(0); // TODO : is there allways one object here? ( we're using an array )
}

IND_Surface* IND_SpriterEntity::getSurface(int folderId, int fileId) {
    Fileref ref = Fileref(static_cast<unsigned int>(folderId), static_cast<unsigned int>(fileId));
    
    // Find the first matching key. (we should only have one)
    SurfaceToFileMap::iterator res = _surfaces->find(ref);
    
    if(res != _surfaces->end()) {
        return res->second;
    }
    else {
        return NULL;
    }
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
