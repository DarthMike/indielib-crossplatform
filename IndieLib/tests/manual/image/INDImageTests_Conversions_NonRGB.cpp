/*****************************************************************************************
 * Desc: INDImageTests_Conversions_NonRGB class
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


#include "../../CIndieLib.h"
#include "INDImageTests_Conversions_NonRGB.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Image.h"

void INDImageTests_Conversions_NonRGB::prepareTests() {
	CIndieLib *iLib = CIndieLib::instance();
	// ----- Images loading -----
	//Colour index format, 16-bit
	iLib->_imageManager->add(_images[0], const_cast<char *>("animations/advance/Rock_Avanza_01.png"));

	//Colour index format, 16-bit
	iLib->_imageManager->add(_images[1], const_cast<char *>("animations/advance/Rock_Avanza_02.png"));

	//Colour index format, 16-bit
	//NO image preload needed. Added here to keep image-surface match perfect in index
	
	// ----- Surfaces loading -----
	//No conversion - Show it 
	iLib->_surfaceManager->add(_surfaces[0], _images[0], IND_OPAQUE, IND_32);

	//Conversion by adding alpha channel, no colorkey specified
	iLib->_surfaceManager->add(_surfaces[1], _images[1], IND_ALPHA, IND_32);

	//Conversion by adding alpha channel, colorkey specified
	iLib->_surfaceManager->add(_surfaces[2], const_cast<char *>("animations/advance/Rock_Avanza_03.png"), IND_ALPHA, IND_32,0, 48, 152);
	
}


void INDImageTests_Conversions_NonRGB::performTests(float dt) {
   
    //IF - Check if test is active
    if(!_active)
        return;

	CIndieLib *iLib = CIndieLib::instance();
	
	//Toggling of entity border lines in entities
	if(iLib->_input->onKeyPress(IND_G) && _active) {
		for (int i = 0; i < _testedEntities; ++i) {
			_entities[i]->showGridAreas(!_entities[i]->isShowGridAreas());
		}
	}

}

bool INDImageTests_Conversions_NonRGB::isActive(){
    return (ManualTests::isActive());
}
    
void INDImageTests_Conversions_NonRGB::setActive(bool active){
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
		_entities[1]->setPosition(static_cast<float>(_entities[0]->getSurface()->getWidth()),
								  _entities[0]->getPosY(),
								  0);
		_entities[2]->setPosition(static_cast<float>(_entities[1]->getPosX() + _entities[1]->getSurface()->getWidth()),
								  _entities[0]->getPosY(),
								  0);
		/*_entities[3]->setPosition(_entities[2]->getPosX() + _entities[2]->getSurface()->getWidth() ,_entities[2]->getPosY(),0);

		_entities[4]->setPosition(0.0f,_entities[2]->getPosY() + _entities[2]->getSurface()->getHeight() , 0.0f);
		_entities[5]->setPosition(_entities[4]->getPosX() + _entities[4]->getSurface()->getWidth(),_entities[4]->getPosY() , 0.0f);

		_entities[6]->setPosition(0.0f,_entities[4]->getPosY() + _entities[4]->getSurface()->getHeight() , 0.0f);
		_entities[7]->setPosition(_entities[6]->getPosX() + _entities[6]->getSurface()->getWidth(),_entities[6]->getPosY() , 0.0f);
		_entities[8]->setPosition(_entities[7]->getPosX() + _entities[7]->getSurface()->getWidth(),_entities[7]->getPosY() , 0.0f);
		_entities[9]->setPosition(_entities[8]->getPosX() + _entities[8]->getSurface()->getWidth(),_entities[8]->getPosY() , 0.0f);

		_entities[10]->setPosition(0.0f,_entities[6]->getPosY() + _entities[6]->getSurface()->getHeight()*1.5 , 0.0f);
		_entities[11]->setPosition(_entities[10]->getPosX() + _entities[10]->getSurface()->getWidth(),_entities[10]->getPosY() , 0.0f);
		_entities[12]->setPosition(_entities[11]->getPosX() + _entities[11]->getSurface()->getWidth(),_entities[11]->getPosY() , 0.0f);*/
    } else { //Inactive
		//Release all variables from indieLib before exiting
		for (int i = 0; i < _testedEntities; ++i) {
			iLib->_entity2dManager->remove(_entities[i]);
		}
	}
}

//-----------------------------------PRIVATE METHODS----------------------------

void INDImageTests_Conversions_NonRGB::init() {

	_testedEntities = 14;

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

void INDImageTests_Conversions_NonRGB::release() {
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
