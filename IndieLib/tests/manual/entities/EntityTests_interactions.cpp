/*****************************************************************************************
 * Desc: EntityTests_interactions class
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
    iLib->_surfaceManager->add(_surfaces[0], const_cast<char *>("tiled_terrain.png"), IND_ALPHA, IND_32);
    
    //Animations
	//Animation with no offsets, with color key to surface, conversion to IND_16 image. Will repeat animation only 5 times
	iLib->_animationManager->addToSurface(_animations[0], const_cast<char *>("animations/dust.xml"), IND_ALPHA, IND_16, 255, 0, 255);

	//Fonts
	iLib->_fontManager->addMudFont(_fonts[0], const_cast<char *>("font_big.png"), const_cast<char *>("font_big.xml"), IND_ALPHA, IND_32);
}

void EntityTests_interactions::performTests(float dt) {
	//IF - Check if test is active
    if(!_active)
        return;

	CIndieLib *iLib = CIndieLib::instance();
	
	//Toggling of entity border lines in entities
	if(iLib->_input->onKeyPress(IND_G) && _active) {
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
