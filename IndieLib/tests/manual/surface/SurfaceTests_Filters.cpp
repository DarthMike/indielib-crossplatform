/*****************************************************************************************
 * Desc: SurfaceTests_Filters.cpp
 SurfaceTests_Filters.h class
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

#include "CIndieLib.h"
#include "SurfaceTests_Filters.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Animation.h"

void SurfaceTests_Filters::prepareTests() {
	// ----- Surface loading -----

    CIndieLib* iLib = CIndieLib::instance();
	// Loading sprite of a star. No conversions
	iLib->_surfaceManager->add (_surfaces[0], "star.png", IND_ALPHA, IND_32);
    // Loading sprite of a star. No conversions
	iLib->_surfaceManager->add (_surfaces[1], "star.png", IND_ALPHA, IND_32);
    // Loading sprite of a star. No conversions
	iLib->_surfaceManager->add (_surfaces[2], "star.png", IND_ALPHA, IND_32);
    
    //Normal animation, with color key to surface. Contains some offsets (not noticeable)
	iLib->_animationManager->addToSurface(_animations[0], const_cast<char *>("animations/character1.xml"), IND_ALPHA, IND_32, 0, 48, 152);
    
    //Normal animation, with color key to surface. Contains some offsets (not noticeable)
	iLib->_animationManager->addToSurface(_animations[1], const_cast<char *>("animations/character1.xml"), IND_ALPHA, IND_32, 0, 48, 152);
    
    //Normal animation, with color key to surface. Contains some offsets (not noticeable)
	iLib->_animationManager->addToSurface(_animations[2], const_cast<char *>("animations/character1.xml"), IND_ALPHA, IND_32, 0, 48, 152);
}


void SurfaceTests_Filters::performTests(float dt) {
   
    //IF - Check if test is active
    if(!_active)
        return;

	CIndieLib *iLib = CIndieLib::instance();
	
	//Toggling of entity border lines in entities
	if(iLib->_input->onKeyPress(IND_F1) && _active) {
		for (int i = 0; i < _testedEntities; ++i) {
			_entities[i]->showGridAreas(!_entities[i]->isShowGridAreas());
		}
	}
}

bool SurfaceTests_Filters::isActive(){
    return (ManualTests::isActive());
}
    
void SurfaceTests_Filters::setActive(bool active){
    ManualTests::setActive(active);

    CIndieLib *iLib = CIndieLib::instance();
    //IF - Active
    if(active){
        
        _entities[0]->setSurface(_surfaces[0]);
        _entities[1]->setSurface(_surfaces[1]);
        _entities[2]->setSurface(_surfaces[2]);
        
        _entities[3]->setAnimation(_animations[0]);
        _entities[4]->setAnimation(_animations[1]);
        _entities[5]->setAnimation(_animations[2]);
	    // ----- Set the surfaces into 2d entities -----
	    for (int i = 0; i < _testedEntities; ++i) {
			iLib->_entity2dManager->add(_entities[i]);
		}

	    // ----- Changing the attributes of the 2d entities -----

	    //Star, no filter
	    _entities[0]->setPosition (10, 50, 0);
        _entities[0]->setScale(1.5f, 1.5f);
        _entities[0]->setFilter(IND_FILTER_NONE);

	    //Star point filter
	    _entities[1]->setPosition (150, 50, 0);
        _entities[1]->setScale(1.5f, 1.5f);
        _entities[1]->setFilter(IND_FILTER_POINT);
        
        //Star linear filter
	    _entities[2]->setPosition (300, 50, 0);
        _entities[2]->setScale(1.5f, 1.5f);
        _entities[2]->setFilter(IND_FILTER_LINEAR);
        
        //Animation, no filter
        _entities[3]->setPosition (10, 200, 0);
        _entities[3]->setScale(1.3f, 1.3f);
        _entities[3]->setFilter(IND_FILTER_NONE);
        
        //Animation, point filter
        _entities[4]->setPosition (200, 200, 0);
        _entities[4]->setScale(1.3f, 1.3f);
        _entities[4]->setFilter(IND_FILTER_POINT);
        
        //Animation, point filter
        _entities[5]->setPosition (400, 200, 0);
        _entities[5]->setScale(1.3f, 1.3f);
        _entities[5]->setFilter(IND_FILTER_LINEAR);

    } else { //Inactive
        //Release all variables from indieLib before exiting
		for (int i = 0; i < _testedEntities; ++i) {
			iLib->_entity2dManager->remove(_entities[i]);
		}
    }
}

//-----------------------------------PRIVATE METHODS----------------------------

void SurfaceTests_Filters::init() {
    _testedEntities = 6;
	_surfaces = new IND_Surface*[_testedEntities];
	_entities = new IND_Entity2d*[_testedEntities];
    _animations = new IND_Animation*[_testedEntities];
    
	//Create underlying entities
	for (int i = 0; i < _testedEntities; ++i) {
		_surfaces[i] = IND_Surface::newSurface();
		_entities[i] = IND_Entity2d::newEntity2d();
        _animations[i] = IND_Animation::newAnimation();
	}
}

void SurfaceTests_Filters::release() {
    CIndieLib* iLib = CIndieLib::instance();
    //Release all variables from indieLib before exiting
	for (int i = 0; i < _testedEntities; ++i) {
        iLib->_animationManager->remove(_animations[i]);
		iLib->_surfaceManager->remove(_surfaces[i]);
		iLib->_entity2dManager->remove(_entities[i]);
	}

	DISPOSEARRAY(_surfaces);
	DISPOSEARRAY(_entities);
    DISPOSEARRAY(_animations);
}
