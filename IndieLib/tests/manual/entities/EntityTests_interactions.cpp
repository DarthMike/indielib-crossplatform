/*****************************************************************************************
 * Desc: EntityTests_interactions class
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
#include "EntityTests_interactions.h"
#include "IND_Animation.h"
#include "IND_Entity2d.h"
#include "IND_Font.h"
#include "IND_Surface.h"
#include "IndiePlatforms.h"

#if defined(PLATFORM_LINUX)
#include <cstring>  // INFO : since this test uses strcat and strcopy we need this on linux ( basic tutorial 08, uses another solution). DartMike, if this ok then remove this comment. 
#endif


void EntityTests_interactions::prepareTests() {
    CIndieLib* iLib = CIndieLib::instance();

    //Surfaces
    iLib->_surfaceManager->add(_surfaces[0], const_cast<char *>("../../resources/tiled_terrain.png"), IND_ALPHA, IND_32);
    
    //Animations
	//Animation with no offsets, with color key to surface, conversion to IND_16 image. Will repeat animation only 5 times
	iLib->_animationManager->addToSurface(_animations[0], const_cast<char *>("../../resources/animations/dust.xml"), IND_ALPHA, IND_16, 255, 0, 255);

	//Fonts
	iLib->_fontManager->add(_fonts[0], const_cast<char *>("../../resources/font_big.png"), const_cast<char *>("../../resources/font_big.xml"), IND_ALPHA, IND_32);
}

void EntityTests_interactions::performTests(float dt) {
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
}

bool EntityTests_interactions::isActive(){
    return (ManualTests::isActive());
}
    
void EntityTests_interactions::setActive(bool active){
    ManualTests::setActive(active);

    CIndieLib *iLib = CIndieLib::instance();
    //IF - Active
    if(active){
		for (int i = 0; i < _testedEntities; ++i) {
			iLib->_entity2dManager->add(0,_entities[i]);
		}

        _entities[0]->setSurface(_surfaces[0]);
        _entities[0]->toggleWrap(1);
        _entities[0]->setRegion(0, 0, 4096, 3072);
        
        _entities[1]->setAnimation(_animations[0]);
        _entities[1]->setPosition(12, 12, 0);
        
        _entities[2]->setFont(_fonts[0]);
        _entities[2]->setLineSpacing(18);
        _entities[2]->setCharSpacing(-8);
        _entities[2]->setPosition(300, 15, 0);
        _entities[2]->setAlign(IND_LEFT);
        _entities[2]->setText("TEST TEXT");
        
        
    } else { //Inactive
        //Release all variables from indieLib before exiting
		for (int i = 0; i < _testedEntities; ++i) {
			iLib->_entity2dManager->remove(_entities[i]);
		}
    }
}

//-----------------------------------PRIVATE METHODS----------------------------

void EntityTests_interactions::init() {
	_testedEntities = 3;
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

void EntityTests_interactions::release() {
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
