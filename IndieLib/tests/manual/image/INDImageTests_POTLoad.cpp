/*****************************************************************************************
 * Desc: INDImageTests_POTLoad class
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
#include "INDImageTests_POTLoad.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Image.h"

void INDImageTests_POTLoad::prepareTests() {
	CIndieLib *iLib = CIndieLib::instance();
	// ----- Images loading -----
	iLib->_imageManager->add(_images[0],  const_cast<char *>("blue_background_square_1024.jpg"));

	iLib->_imageManager->add(_images[1],  const_cast<char *>("gem_squared.png"));

	iLib->_imageManager->add(_images[2],  const_cast<char *>("hole1.png"));

	iLib->_imageManager->add(_images[3],  const_cast<char *>("hole2.png"));

	iLib->_imageManager->add(_images[4],  const_cast<char *>("rabbit.png"));

	iLib->_imageManager->add(_images[5],  const_cast<char *>("draco.png"));

	

	// ----- Surfaces loading -----
	iLib->_surfaceManager->add(_surfaces[0], _images[0], IND_OPAQUE, IND_32);

	iLib->_surfaceManager->add(_surfaces[1], _images[1], IND_ALPHA, IND_32);

	iLib->_surfaceManager->add(_surfaces[2], _images[2], IND_ALPHA, IND_32);

	iLib->_surfaceManager->add(_surfaces[3], _images[3], IND_ALPHA, IND_32);

	iLib->_surfaceManager->add(_surfaces[4], _images[4], IND_ALPHA, IND_32);

	iLib->_surfaceManager->add(_surfaces[5], _images[5], IND_ALPHA, IND_32);
}


void INDImageTests_POTLoad::performTests(float dt) {
   
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

bool INDImageTests_POTLoad::isActive(){
    return (ManualTests::isActive());
}
    
void INDImageTests_POTLoad::setActive(bool active){
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
		_entities[3]->setPosition(400, 300, 0);
		_entities[4]->setPosition(700,0,0);
		_entities[5]->setPosition(150,50,0);
	    // ----- Changing the attributes of the 2d entities -----

    } else { //Inactive
		//Release all variables from indieLib before exiting
		for (int i = 0; i < _testedEntities; ++i) {
			iLib->_entity2dManager->remove(_entities[i]);
		}
	}
}

//-----------------------------------PRIVATE METHODS----------------------------

void INDImageTests_POTLoad::init() {
	_testedEntities = 6;
	_images = new IND_Image*[_testedEntities];
	_surfaces = new IND_Surface*[_testedEntities];
	_entities = new IND_Entity2d*[_testedEntities];

	//Create underlying entities
	for (int i = 0; i < _testedEntities; ++i) {
		_images[i] = IND_Image::newImage();
		_surfaces[i] = IND_Surface::newSurface();
		_entities[i] = IND_Entity2d::newEntity2d();
	}
}	

void INDImageTests_POTLoad::release() {
	CIndieLib* iLib = CIndieLib::instance();
	
	//Release all variables from indieLib before exiting
	for (int i = 0; i < _testedEntities; ++i) {
		iLib->_imageManager->remove(_images[i]);
		iLib->_surfaceManager->remove(_surfaces[i]);
		iLib->_entity2dManager->remove(_entities[i]);
	}

	DISPOSEARRAY(_images);
	DISPOSEARRAY(_surfaces);
	DISPOSEARRAY(_entities);
}
