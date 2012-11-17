/*****************************************************************************************
 * Desc: SurfaceTests class
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
#include "SurfaceTests.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"

void SurfaceTests::prepareTests() {
	// ----- Surface loading -----

    CIndieLib* iLib = CIndieLib::instance();
	// Loading Background. No conversions
	iLib->_surfaceManager->add (_surfaces[0], const_cast<char *>("../../resources/blue_background.jpg"), IND_OPAQUE, IND_32);
	// Loading sprite of a warrior. Colorkey conversion
	iLib->_surfaceManager->add (_surfaces[1], const_cast<char *>("../../resources/derekyu_sprite.png"), IND_ALPHA, IND_32, 255, 0, 255);
	// Loading sprite of a star. No conversions
	iLib->_surfaceManager->add (_surfaces[2], const_cast<char *>("../../resources/star.png"), IND_ALPHA, IND_32);
	// Loading sprite of a star. No conversions
	iLib->_surfaceManager->add (_surfaces[3], const_cast<char *>("../../resources/star.png"), IND_ALPHA, IND_32);
	// Loading sprite of a star. No conversions
	iLib->_surfaceManager->add (_surfaces[4], const_cast<char *>("../../resources/star.png"), IND_ALPHA, IND_32);
	// Loading sprite of a rabbit. No conversions
	iLib->_surfaceManager->add (_surfaces[5], const_cast<char *>("../../resources/rabbit.png"), IND_ALPHA, IND_32);
	// Loading sprite of a rabbit. No conversions
	iLib->_surfaceManager->add (_surfaces[6], const_cast<char *>("../../resources/rabbit.png"), IND_ALPHA, IND_32);
	// Loading sprite of a rabbit. No conversions
	iLib->_surfaceManager->add (_surfaces[7], const_cast<char *>("../../resources/rabbit.png"), IND_ALPHA, IND_32);
	// Loading sprite of a rabbit. No conversions
	iLib->_surfaceManager->add (_surfaces[8], const_cast<char *>("../../resources/rabbit.png"), IND_ALPHA, IND_32);
	// Loading sprite of a rabbit. No conversions
	iLib->_surfaceManager->add (_surfaces[9], const_cast<char *>("../../resources/rabbit.png"), IND_ALPHA, IND_32);
	// Loading sprite of a rabbit. No conversions
	iLib->_surfaceManager->add (_surfaces[10], const_cast<char *>("../../resources/rabbit.png"), IND_ALPHA, IND_32);
}


void SurfaceTests::performTests(float dt) {
   
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

bool SurfaceTests::isActive(){
    return (ManualTests::isActive());
}
    
void SurfaceTests::setActive(bool active){
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

	    // Warrior
	    _entities[1]->setPosition (460, 0, 0);				// Set the position of the entity

	    // Original Star without chaning it's attributes
	    _entities[2]->setPosition (0, 0, 0);					// Set the position of the entity
    	//
	    // We change the attributes of this entity in order 
	    // to create a big rotated semitransparent star with 
	    // an orange tint
	    _entities[3]->setPosition (330, 0, 0);					// Set the position of the entity
	    _entities[3]->setScale	(2, 2);							// Set the scale of the entity
	    _entities[3]->setTint	(255, 0, 0);					// Set tint to color R
	    _entities[3]->setAngleXYZ	(0, 0, 45);						// Rotation in Z angle = 45º

	    // A bigger star than the original, faded to green. We only draw a region of 50x50 pixels
	    _entities[4]->setHotSpot (0.5f, 0.5f);					// We change the reference point of the entity
	    _entities[4]->setPosition (170, 60, 0);					// Set the position of the entity
	    _entities[4]->setScale	(1.5f, 1.5f);					// Set the scale of the entity
	    _entities[4]->setFade	(0, 255, 0, 128);					// Set fade to green color, the amout of fade is 128 (255 will be complety gree)
	    _entities[4]->setRegion (20, 20, 50, 50);					// Region we want to draw

		//Original rabbit image with transparency
		_entities[5]->setPosition(0,230,0);
        _entities[5]->setTransparency(200);
		//Rabbit mirrored in X
		_entities[6]->setPosition(70,230,0);
		_entities[6]->setMirrorX(true);
		//Rabbit mirrored in Y
		_entities[7]->setPosition(140,230,0);
		_entities[7]->setMirrorY(true);
		//Rabbit mirrored in X and Y
		_entities[8]->setPosition(210,230,0);
		_entities[8]->setMirrorX(true);
		_entities[8]->setMirrorY(true);
		//Rabbit mirrored in X, hotspot set to 0.5f
		_entities[9]->setPosition(280,230,0);
		_entities[9]->setMirrorX(true);
		_entities[9]->setHotSpot(0.5f,0.5f);
		//Rabbit mirrored in Y, hotspot set to 0.5f
		_entities[10]->setPosition(350,230,0);
		_entities[10]->setMirrorY(true);
		_entities[10]->setHotSpot(0.5f,0.5f);
    } else { //Inactive
        //Release all variables from indieLib before exiting
		for (int i = 0; i < _testedEntities; ++i) {
			iLib->_entity2dManager->remove(_entities[i]);
		}
    }
}

//-----------------------------------PRIVATE METHODS----------------------------

void SurfaceTests::init() {
    _testedEntities = 11;
	_surfaces = new IND_Surface*[_testedEntities];
	_entities = new IND_Entity2d*[_testedEntities];

	//Create underlying entities
	for (int i = 0; i < _testedEntities; ++i) {
		_surfaces[i] = new IND_Surface();
		_entities[i] = new IND_Entity2d();
	}
}

void SurfaceTests::release() {
    CIndieLib* iLib = CIndieLib::instance();
    //Release all variables from indieLib before exiting
	for (int i = 0; i < _testedEntities; ++i) {
		iLib->_surfaceManager->remove(_surfaces[i]);
		iLib->_entity2dManager->remove(_entities[i]);
	}

	DISPOSEARRAY(_surfaces);
	DISPOSEARRAY(_entities);
}
