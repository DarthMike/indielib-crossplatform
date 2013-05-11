/*****************************************************************************************
 * Desc: EntityTests_animateScale class
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
THIS FILE IS AN ADDITIONAL FILE ADDED BY iLibguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / iLibkeskywalker007@gmail.com), BUT HAS THE
SAME LICENSE AS THE WHOLE LIBRARY TO RESPECT ORIGINAL AUTHOR OF LIBRARY

This library is free software; you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
Suite 330, Boston, MA 02111-1307 USA
*/

#include "CIndieLib.h"
#include "EntityTests_animateScale.h"
#include "IND_Animation.h"
#include "IND_Entity2d.h"
#include "IND_Font.h"
#include "IND_Surface.h"
#include "IndiePlatforms.h"

#if defined(PLATFORM_LINUX)
#include <cstring>  // INFO : since this test uses strcat and strcopy we need this on linux ( basic tutorial 08, uses another solution). DartMike, if this ok then remove this comment. 
#endif


void EntityTests_animateScale::prepareTests() {
	// ----- Surface loading -----
    CIndieLib* iLib = CIndieLib::instance();

	// Loading Rocket
	if (!iLib->_surfaceManager->add(_surfaces[0], const_cast<char *>("../../resources/rocket.png"), IND_ALPHA, IND_32)) return;

	// Loading Beetleship
	if (!iLib->_surfaceManager->add(_surfaces[1], const_cast<char *>("../../resources/beetleship.png"), IND_ALPHA, IND_32)) return;
	
	// Loading star
	if (!iLib->_surfaceManager->add(_surfaces[2], const_cast<char *>("../../resources/star.png"), IND_ALPHA, IND_32)) return;

	// Sword Master animation, we apply a color key of (0, 255, 0)
	if (!iLib->_animationManager->addToSurface(_animations[0], const_cast<char *>("../../resources/animations/sword_master.xml"), IND_ALPHA, IND_16, 0, 255, 0)) return;

	// ----- Font loading -----

	// Font
	if (!iLib->_fontManager->add(_fonts[0], const_cast<char *>("../../resources/font_small.png"), const_cast<char *>("../../resources/font_small.xml"), IND_ALPHA, IND_32)) return;

	// Rocket
	_entities[0]->setSurface(_surfaces[0]);
	_entities[0]->setHotSpot(0.5f, 0.5f);
	_entities[0]->setPosition(200, 450, 1);

	// Beetle
	_entities[1]->setSurface(_surfaces[1]);
	_entities[1]->setHotSpot(0.5f, 0.5f);
	_entities[1]->setMirrorX(1);
	_entities[1]->setPosition(300,250,1);

	// Sword Master Animation
	_entities[2]->setAnimation(_animations[0]);
	_entities[2]->setHotSpot(0.5f, 0.5f);
	_entities[2]->setPosition(500, 220, 0);

	//Star
	_entities[3]->setSurface(_surfaces[2]);
	_entities[3]->setHotSpot(0.5f, 0.5f);
	_entities[3]->setPosition(350, 50, 1);
}


void EntityTests_animateScale::performTests(float dt) {
	//IF - Check if test is active
    if(!_active)
        return;

	CIndieLib *iLib = CIndieLib::instance();
	
	//Toggling of entity border lines in entities
	if(iLib->_input->onKeyPress(IND_F1) && _active) {
		for (int i = 0; i < _testedEntities; ++i) {
			_entities[i]->showGridAreas(!_entities[i]->isShowGridAreas());
			_entities[i]->showCollisionAreas(!_entities[i]->isShowCollisionAreas());
		}
	}

	// ----- Input ----

	float mDelta = iLib->_render->getFrameTime() / 1000.0f;


	
	_animationValue += _animationSpeed * mDelta;
	
	if (_animationValue < 0){
		_animationValue = 0;
	}

	// ----- Updating entities attributes  -----
	float sinValue = 1.0f + sinf(_animationValue);
	_entities[0]->setScale(2*sinValue,2*sinValue);
	_entities[1]->setScale(sinValue,sinValue);
	_entities[2]->setScale(10*sinValue,10*sinValue);
}

bool EntityTests_animateScale::isActive(){
    return (ManualTests::isActive());
}
    
void EntityTests_animateScale::setActive(bool active){
    ManualTests::setActive(active);

    CIndieLib *iLib = CIndieLib::instance();
    //IF - Active
    if(active){
		for (int i = 0; i < _testedEntities; ++i) {
			iLib->_entity2dManager->add(_entities[i]);
		}

    } else { //Inactive
        //Release all variables from indieLib before exiting
		for (int i = 0; i < _testedEntities; ++i) {
			iLib->_entity2dManager->remove(_entities[i]);
		}
    }
}

//-----------------------------------PRIVATE METHODS----------------------------

void EntityTests_animateScale::init() {
	_testedEntities = 4;
	_animations = new IND_Animation*[_testedEntities];
	_entities = new IND_Entity2d*[_testedEntities];
	_fonts = new IND_Font*[_testedEntities];
	_surfaces = new IND_Surface*[_testedEntities];

	//Create underlying entities
	for (int i = 0; i < _testedEntities; ++i) {
		_animations[i] = IND_Animation::newAnimation();
		_entities[i] = IND_Entity2d::newEntity2d();
		_fonts[i] = IND_Font::newFont();
		_surfaces[i] = IND_Surface::newSurface();
	}
}

void EntityTests_animateScale::release() {
    CIndieLib* iLib = CIndieLib::instance();
    //Release all variables from indieLib before exiting
	for (int i = 0; i < _testedEntities; ++i) {
		iLib->_animationManager->remove(_animations[i]);
		iLib->_entity2dManager->remove(_entities[i]);
		iLib->_fontManager->remove(_fonts[i]);
		iLib->_surfaceManager->remove(_surfaces[i]);
	}

	DISPOSEARRAY(_animations);
	DISPOSEARRAY(_entities);
	DISPOSEARRAY(_fonts);
	DISPOSEARRAY(_surfaces);
}
