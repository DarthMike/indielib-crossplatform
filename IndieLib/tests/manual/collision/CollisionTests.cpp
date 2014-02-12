/*****************************************************************************************
 * Desc: CollisionTests class
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
#include "CollisionTests.h"
#include "IND_Animation.h"
#include "IND_Entity2d.h"
#include "IND_Font.h"
#include "IND_Surface.h"
#include "IndiePlatforms.h"

#if defined(PLATFORM_LINUX)
#include <cstring>  // INFO : since this test uses strcat and strcopy we need this on linux ( basic tutorial 08, uses another solution). DartMike, if this ok then remove this comment. 
#endif


void CollisionTests::prepareTests() {
	// ----- Surface loading -----
    CIndieLib* iLib = CIndieLib::instance();

	// Loading Rocket
	if (!iLib->_surfaceManager->add(_surfaces[0], const_cast<char *>("rocket.png"), IND_ALPHA, IND_32)) return;

	// Loading Beetleship
	if (!iLib->_surfaceManager->add(_surfaces[1], const_cast<char *>("beetleship.png"), IND_ALPHA, IND_32)) return;
	
	// Sword Master animation, we apply a color key of (0, 255, 0)
	if (!iLib->_animationManager->addToSurface(_animations[0], const_cast<char *>("animations/sword_master.xml"), IND_ALPHA, IND_16, 0, 255, 0)) return;

	// ----- Font loading -----

	// Font
	if (!iLib->_fontManager->addMudFont(_fonts[0], const_cast<char *>("font_small.png"), const_cast<char *>("font_small.xml"), IND_ALPHA, IND_32)) return;

	// Rocket
	_entities[0]->setSurface(_surfaces[0]);
	_entities[0]->setHotSpot(0.5f, 0.5f);
	_entities[0]->setPosition(200, 450, 1);
	_entities[0]->setBoundingAreas("rocket_collisions.xml");

	// Beetle
	_entities[1]->setSurface(_surfaces[1]);
	_entities[1]->setHotSpot(0.5f, 0.5f);
	_entities[1]->setMirrorX(1);
	_entities[1]->setPosition(0,0,1);
	_entities[1]->setBoundingTriangle("beetle_head", 160, 105, 160, 170, 190, 135);
	_entities[1]->setBoundingCircle("beetle_boy_head", 85, 52, 55);

	// Sword Master Animation
	_entities[2]->setAnimation(_animations[0]);
	_entities[2]->setHotSpot(0.5f, 0.5f);
	_entities[2]->setPosition(500, 220, 3);

	// Text
	_entities[3]->setFont(_fonts[0]);
	_entities[3]->setLineSpacing	(18);
	_entities[3]->setCharSpacing	(-7);
	_entities[3]->setPosition(150, 30, 10);
	_entities[3]->setAlign(IND_LEFT);
}


void CollisionTests::performTests(float dt) {
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

	// ----- Input ----

	float mDelta = iLib->_render->getFrameTime() / 1000.0f;

	if (iLib->_input->isKeyPressed(IND_KEYUP)){
		_scale += _speedScaling * mDelta;
	}
	if (iLib->_input->isKeyPressed(IND_KEYDOWN)){
		_scale -= _speedScaling * mDelta;
	}

	_angle += _speedRotation * mDelta;
	
	if (_scale < 0){
		_scale = 0;
	}

	// ----- Updating entities attributes  -----

	_entities[0]->setAngleXYZ (0, 0, _angle);
	_entities[1]->setScale (_scale, _scale);	
	_entities[1]->setPosition((float) iLib->_input->getMouseX(), (float) iLib->_input->getMouseY(), 5);

	// ----- Check collisions -----

	char collisionText [1500];
	char noCollisionText [500];
	strcpy(noCollisionText,"No collision between the groups we are checking");
	strcpy(collisionText,"");
	bool collision = false;

	if (iLib->_entity2dManager->isCollision(_entities[0], "rocket_head", _entities[1], "beetle_boy_head")){
		collision = true;
		strcat(collisionText,"\nCIRCLE TO TRIANGLE: rocket head and beetle boy head");
	}	

	if (iLib->_entity2dManager->isCollision(_entities[0], "rocket_head", _entities[1], "beetle_head")){ 
		collision = true;
		strcat(collisionText,"\nTRIANGLE TO TRIANGLE: rocket head / beetle head");
	}

	if (iLib->_entity2dManager->isCollision(_entities[0], "rocket_boy_head", _entities[1], "beetle_boy_head")){ 
		collision = true;
		strcat(collisionText,"\nCIRCLE TO CIRCLE: rocket boy head / beetle boy head");	
	}

	if (iLib->_entity2dManager->isCollision(_entities[1], "beetle_boy_head", _entities[2], "sword")){ 
		collision = true;
		strcat(collisionText,"\nCIRCLE TO ANIMATED: boy head / sword");
	}

	if (iLib->_entity2dManager->isCollision(_entities[1], "beetle_head", _entities[2], "sword")){ 
		collision = true;
		strcat(collisionText,"\nANIMATED TO TRIANGLE: beetle head / sword");
	}

	if (collision) {
		_entities[3]->setText(collisionText);
	} else {
		_entities[3]->setText(noCollisionText);
	}
}

bool CollisionTests::isActive(){
    return (ManualTests::isActive());
}
    
void CollisionTests::setActive(bool active){
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

void CollisionTests::init() {
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

void CollisionTests::release() {
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
