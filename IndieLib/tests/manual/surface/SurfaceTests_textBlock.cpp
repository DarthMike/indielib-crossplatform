/*****************************************************************************************
 * Desc: SurfaceTests_textBlock class
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
#include "SurfaceTests_textBlock.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"

void SurfaceTests_textBlock::prepareTests() {
	// ----- Surface loading -----

    CIndieLib* iLib = CIndieLib::instance();
	//Draco. Will be partitioned in several texture blocks
	iLib->_surfaceManager->add(_surfaces[0], const_cast<char *>("draco.png"), IND_ALPHA, IND_32);
	//Make an arbitrary grid
	_surfaces[0]->setGrid(8, 8);
}


void SurfaceTests_textBlock::performTests(float dt) {
   
    //IF - Check if test is active
    if(!_active)
        return;

	CIndieLib *iLib = CIndieLib::instance();
	int mNumBlocksX = _surfaces[0]->getBlocksX();
	int mNumBlocksY = _surfaces[0]->getBlocksY();
	int mWidthBlock = _surfaces[0]->getWidthBlock();
	int mHeightBlock = _surfaces[0]->getHeightBlock();

	float mT = dt / 1000.f;
	for (int i = 1; i < mNumBlocksX; i++) {
		for (int j = 1; j < mNumBlocksY; j++) {
            int newPosX = static_cast<int>((j * mHeightBlock + cosf (mT * 10 + (i + j) / 2) * 5));
            int newPosY = static_cast<int>((i * mWidthBlock	+ sinf (mT * 10 + (i + j) / 2) * 5));
            _surfaces[0]->setVertexPos (j, i, newPosX, newPosY);
        }
    }

	//Toggling of entity border lines in entities
	if(iLib->_input->onKeyPress(IND_G) && _active) {
		for (int i = 0; i < _testedEntities; ++i) {
			_entities[i]->showGridAreas(!_entities[i]->isShowGridAreas());
		}
	}
}

bool SurfaceTests_textBlock::isActive(){
    return (ManualTests::isActive());
}
    
void SurfaceTests_textBlock::setActive(bool active){
    ManualTests::setActive(active);

    CIndieLib *iLib = CIndieLib::instance();
    //IF - Active
    if(active){
	    // ----- Set the surfaces into 2d entities -----
	    for (int i = 0; i < _testedEntities; ++i) {
			_entities[i]->setSurface(_surfaces[i]);
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

void SurfaceTests_textBlock::init() {
    _testedEntities = 1;
	_surfaces = new IND_Surface*[_testedEntities];
	_entities = new IND_Entity2d*[_testedEntities];

	//Create underlying entities
	for (int i = 0; i < _testedEntities; ++i) {
		_surfaces[i] = IND_Surface::newSurface();
		_entities[i] = IND_Entity2d::newEntity2d();
	}
}

void SurfaceTests_textBlock::release() {
    CIndieLib* iLib = CIndieLib::instance();
    //Release all variables from indieLib before exiting
	for (int i = 0; i < _testedEntities; ++i) {
		iLib->_surfaceManager->remove(_surfaces[i]);
		iLib->_entity2dManager->remove(_entities[i]);
	}

	DISPOSEARRAY(_surfaces);
	DISPOSEARRAY(_entities);
}
