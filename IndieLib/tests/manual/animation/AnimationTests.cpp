/*****************************************************************************************
 * Desc: AnimationTests class
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
#include "AnimationTests.h"
#include "IND_Animation.h"
#include "IND_Entity2d.h"

void AnimationTests::prepareTests() {
	// ----- Surface loading -----
    CIndieLib* iLib = CIndieLib::instance();

	//Normal animation, with color key to surface. Contains some offsets (not noticeable)
	iLib->_animationManager->addToSurface(_animations[0], const_cast<char *>("../../resources/animations/character1.xml"), IND_ALPHA, IND_32, 0, 48, 152);
	//Animation with offsets animation, with color key to surface.
	iLib->_animationManager->addToSurface(_animations[1], const_cast<char *>("../../resources/animations/character1_offsetTests.xml"), IND_ALPHA, IND_32, 0, 48, 152);
	//Animation with no offsets, with color key to surface, conversion to IND_16 image. Will repeat animation only 5 times
	iLib->_animationManager->addToSurface(_animations[2], const_cast<char *>("../../resources/animations/dust.xml"), IND_ALPHA, IND_16, 255, 0, 255);
	//Animation with offsets animation, with color key to surface. Will test replays+offsets behavior
	iLib->_animationManager->addToSurface(_animations[3], const_cast<char *>("../../resources/animations/character1_offsetTests.xml"), IND_ALPHA, IND_32, 0, 48, 152);
}


void AnimationTests::performTests(float dt) {
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

bool AnimationTests::isActive(){
    return (ManualTests::isActive());
}
    
void AnimationTests::setActive(bool active){
    ManualTests::setActive(active);

    CIndieLib *iLib = CIndieLib::instance();
    //IF - Active
    if(active){
		for (int i = 0; i < _testedEntities; ++i) {
			_entities[i]->setAnimation(_animations[i]);
			iLib->_entity2dManager->add(_entities[i]);
		}
		_entities[0]->setSequence(0);						// Choose sequence
		_entities[0]->setPosition(140, 0, 0);
		
		_entities[1]->setSequence(0);						// Choose sequence
		_entities[1]->setPosition(300, 0, 0);

		_entities[2]->setSequence(0);						// Choose sequence
		_entities[2]->setPosition(500, 0, 0);
		_entities[2]->setNumReplays(5);

		_entities[3]->setSequence(0);						// Choose sequence
		_entities[3]->setPosition(300, 200, 0);
		_entities[3]->setNumReplays(5);

    } else { //Inactive
        //Release all variables from indieLib before exiting
		for (int i = 0; i < _testedEntities; ++i) {
			iLib->_entity2dManager->remove(_entities[i]);
		}
    }
}

//-----------------------------------PRIVATE METHODS----------------------------

void AnimationTests::init() {
	_testedEntities = 4;
	_animations = new IND_Animation*[_testedEntities];
	_entities = new IND_Entity2d*[_testedEntities];

	//Create underlying entities
	for (int i = 0; i < _testedEntities; ++i) {
		_animations[i] = IND_Animation::newAnimation();
		_entities[i] = IND_Entity2d::newEntity2d();
	}
}

void AnimationTests::release() {
    CIndieLib* iLib = CIndieLib::instance();
    //Release all variables from indieLib before exiting
	for (int i = 0; i < _testedEntities; ++i) {
		iLib->_animationManager->remove(_animations[i]);
		iLib->_entity2dManager->remove(_entities[i]);
	}

	DISPOSEARRAY(_animations);
	DISPOSEARRAY(_entities);
}
