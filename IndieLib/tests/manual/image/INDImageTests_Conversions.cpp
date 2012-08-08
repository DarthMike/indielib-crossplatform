/*****************************************************************************************
 * Desc: INDImageTests_Conversions class
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
#include "INDImageTests_Conversions.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Image.h"

void INDImageTests_Conversions::prepareTests() {
	CIndieLib *iLib = CIndieLib::instance();
	// ----- Images loading -----
	iLib->_imageManager->add(_images[0],  const_cast<char *>("../../resources/gem_squared_noalpha.jpg"));
	iLib->_imageManager->add(_images[1],  const_cast<char *>("../../resources/gem_squared_noalpha.jpg"));
	iLib->_imageManager->add(_images[2],  const_cast<char *>("../../resources/gem_squared.png"));
	iLib->_imageManager->add(_images[3],  const_cast<char *>("../../resources/gem_squared.png"));
	iLib->_imageManager->add(_images[4],  const_cast<char *>("../../resources/star.png"));
	iLib->_imageManager->add(_images[5],  const_cast<char *>("../../resources/star.png"));
	iLib->_imageManager->add(_images[6],  const_cast<char *>("../../resources/rabbit.png"));
	iLib->_imageManager->add(_images[7],  const_cast<char *>("../../resources/rabbit.png"));
	iLib->_imageManager->add(_images[8],  const_cast<char *>("../../resources/rabbit.png"));
	iLib->_imageManager->add(_images[9],  const_cast<char *>("../../resources/rabbit.png"));
	iLib->_imageManager->add(_images[10], const_cast<char *>("../../resources/rabbit.png"));
	iLib->_imageManager->add(_images[11], const_cast<char *>("../../resources/rabbit.png"));
	iLib->_imageManager->add(_images[12], const_cast<char *>("../../resources/rabbit.png"));

	// ----- Surfaces loading -----
	//No conversion
	iLib->_surfaceManager->add(_surfaces[0], _images[0], IND_OPAQUE, IND_32);
	//Convert image - Add alpha channel to opaque image
	_images[1]->setAlpha(255,0,0);
	iLib->_surfaceManager->add(_surfaces[1], _images[1], IND_ALPHA, IND_32);

	//No conversion
	iLib->_surfaceManager->add(_surfaces[2], _images[2], IND_ALPHA, IND_32);
	//Convert image - Clear to half opaque and greenish color
	_images[3]->clear(0,255,0,25);
	iLib->_surfaceManager->add(_surfaces[3], _images[3], IND_ALPHA, IND_32);

	//No conversion
	iLib->_surfaceManager->add(_surfaces[4], _images[4], IND_ALPHA, IND_32);
	//Convert image - Invert alpha channel
	_images[5]->invertAlpha();
	iLib->_surfaceManager->add(_surfaces[5], _images[5], IND_ALPHA, IND_32);

	//No conversion
	iLib->_surfaceManager->add(_surfaces[6], _images[6], IND_ALPHA, IND_32);
	//Convert image - Rotate 90 degrees
	_images[7]->rotate(90);
	iLib->_surfaceManager->add(_surfaces[7], _images[7], IND_ALPHA, IND_32);
	//Convert image - Rotate 45 degrees
	_images[8]->rotate(45);
	iLib->_surfaceManager->add(_surfaces[8], _images[8], IND_ALPHA, IND_32);
	//Convert image - Rotate 180 degrees
	_images[9]->rotate(180);
	iLib->_surfaceManager->add(_surfaces[9], _images[9], IND_ALPHA, IND_32);

	//No conversion
	iLib->_surfaceManager->add(_surfaces[10], _images[10], IND_ALPHA, IND_32);
	//Convert image - Scale (bigger)
	_images[11]->scale(_images[11]->getWidth() * 1.2, _images[11]->getHeight() * 1.2);
	iLib->_surfaceManager->add(_surfaces[11], _images[11], IND_ALPHA, IND_32);
	//Convert image - Scale (smaller)
	_images[12]->scale(_images[12]->getWidth() * 0.8, _images[12]->getHeight() * 0.8);
	iLib->_surfaceManager->add(_surfaces[12], _images[12], IND_ALPHA, IND_32);
}


void INDImageTests_Conversions::performTests(float dt) {
   
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

bool INDImageTests_Conversions::isActive(){
    return (ManualTests::isActive());
}
    
void INDImageTests_Conversions::setActive(bool active){
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
		_entities[0]->setPosition(0.0f,0.0f,0.0f);
		_entities[1]->setPosition(_entities[0]->getSurface()->getWidth(),_entities[0]->getPosY(),0.0f);

		_entities[2]->setPosition(0.0f,_entities[0]->getPosY() + _entities[0]->getSurface()->getHeight(),0.0f);
		_entities[3]->setPosition(_entities[2]->getPosX() + _entities[2]->getSurface()->getWidth() ,_entities[2]->getPosY(),0);

		_entities[4]->setPosition(0.0f,_entities[2]->getPosY() + _entities[2]->getSurface()->getHeight() , 0.0f);
		_entities[5]->setPosition(_entities[4]->getPosX() + _entities[4]->getSurface()->getWidth(),_entities[4]->getPosY() , 0.0f);

		_entities[6]->setPosition(0.0f,_entities[4]->getPosY() + _entities[4]->getSurface()->getHeight() , 0.0f);
		_entities[7]->setPosition(_entities[6]->getPosX() + _entities[6]->getSurface()->getWidth(),_entities[6]->getPosY() , 0.0f);
		_entities[8]->setPosition(_entities[7]->getPosX() + _entities[7]->getSurface()->getWidth(),_entities[7]->getPosY() , 0.0f);
		_entities[9]->setPosition(_entities[8]->getPosX() + _entities[8]->getSurface()->getWidth(),_entities[8]->getPosY() , 0.0f);

		_entities[10]->setPosition(0.0f,_entities[6]->getPosY() + _entities[6]->getSurface()->getHeight()*1.5 , 0.0f);
		_entities[11]->setPosition(_entities[10]->getPosX() + _entities[10]->getSurface()->getWidth(),_entities[10]->getPosY() , 0.0f);
		_entities[12]->setPosition(_entities[11]->getPosX() + _entities[11]->getSurface()->getWidth(),_entities[11]->getPosY() , 0.0f);
    } else { //Inactive
		//Release all variables from indieLib before exiting
		for (int i = 0; i < _testedEntities; ++i) {
			iLib->_entity2dManager->remove(_entities[i]);
		}
	}
}

//-----------------------------------PRIVATE METHODS----------------------------

void INDImageTests_Conversions::init() {

	_testedEntities = 14;

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

void INDImageTests_Conversions::release() {
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
