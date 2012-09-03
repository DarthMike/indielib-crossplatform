/*****************************************************************************************
 * Desc: INDImageTests_nonPOTLoad class
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
#include "INDImageTests_nonPOTLoad.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Image.h"


void INDImageTests_nonPOTLoad::prepareTests() {
	CIndieLib *iLib = CIndieLib::instance();

	// ----- Images loading -----

	iLib->_imageManager->add(_images[0], const_cast<char *>("../../resources/twist.jpg"));

	iLib->_imageManager->add(_images[1], const_cast<char *>("../../resources/octopus_small.png"));

	iLib->_imageManager->add(_images[2], const_cast<char *>("../../resources/planet.png"));

	// ----- Surfaces loading -----

	//Load NonPOT image without giving blocks for a 800x600 image
	iLib->_surfaceManager->add(_surfaces[0], _images[0], IND_OPAQUE, IND_32);

	//Load NonPOT image without giving blocks data. A small image (68x64)
	iLib->_surfaceManager->add(_surfaces[1], _images[1], IND_ALPHA, IND_32);

	//Load NonPOT image giving blocks data (size 20 blocks)
	iLib->_surfaceManager->add(_surfaces[2], _images[2], 20, IND_ALPHA, IND_32);
}

void INDImageTests_nonPOTLoad::performTests(float dt) {
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

bool INDImageTests_nonPOTLoad::isActive(){
    return (ManualTests::isActive());
}
    
void INDImageTests_nonPOTLoad::setActive(bool active){
    ManualTests::setActive(active);

	CIndieLib *iLib = CIndieLib::instance();
    //IF - Active
    if(active){
	    // ----- Set the surfaces into 2d entities -----
		for (int i = 0; i < _testedEntities; ++i) {
			_entities[i]->setSurface(_surfaces[i]);
			iLib->_entity2dManager->add(_entities[i]);
		}
	    // ----- Changing the attributes of the 2d entities -----
		_entities[0]->setPosition(0.0f,0.0f,0);
		_entities[1]->setPosition(0.0f,0.0f,0);
		_entities[2]->setPosition(0.0f,_entities[1]->getPosY() + static_cast<float>(_entities[1]->getSurface()->getHeight()),0);

    } else { //Inactive
		//Release all variables from indieLib before exiting
		for (int i = 0; i < _testedEntities; ++i) {
			iLib->_entity2dManager->remove(_entities[i]);
		}
	}
}

//-----------------------------------PRIVATE METHODS----------------------------

void INDImageTests_nonPOTLoad::init() {
	_testedEntities = 3;

	_images = new IND_Image*[_testedEntities];
	_surfaces = new IND_Surface*[_testedEntities];
	_entities = new IND_Entity2d*[_testedEntities];

	//Create underlying entities
	for (int i = 0; i < _testedEntities; ++i) {
		_images[i] = new IND_Image();
		_surfaces[i] = new IND_Surface();
		_entities[i] = new IND_Entity2d();
	}
}	

void INDImageTests_nonPOTLoad::release() {
    CIndieLib* iLib = CIndieLib::instance();
    //Release all variables from indieLib before exiting
	for (int i = 0; i < _testedEntities; ++i) {
		iLib->_imageManager->remove(_images[i]);
		DISPOSE(_images[i]);
		iLib->_surfaceManager->remove(_surfaces[i]);
		DISPOSE(_surfaces[i]);
		iLib->_entity2dManager->remove(_entities[i]);
		DISPOSE(_entities[i]);
	}

	DISPOSEARRAY(_images);
	DISPOSEARRAY(_surfaces);
	DISPOSEARRAY(_entities);
}
