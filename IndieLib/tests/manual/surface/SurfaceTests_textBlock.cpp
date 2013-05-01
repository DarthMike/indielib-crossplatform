/*****************************************************************************************
 * Desc: SurfaceTests_textBlock class
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
THIS FILE IS AN ADDITIONAL FILE ADDED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com), BUT HAS THE
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

#include "CIndieLib_vc2008.h"
#include "SurfaceTests_textBlock.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"

void SurfaceTests_textBlock::prepareTests() {
	// ----- Surface loading -----

    CIndieLib* iLib = CIndieLib::instance();
	//Draco. Will be partitioned in several texture blocks
	iLib->_surfaceManager->add(_surfaces[0], const_cast<char *>("../../resources/draco.png"), IND_ALPHA, IND_32);
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
	if(iLib->_input->onKeyPress(IND_F1) && _active) {
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
