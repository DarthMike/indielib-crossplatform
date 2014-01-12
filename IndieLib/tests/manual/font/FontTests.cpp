/*****************************************************************************************
 * Desc: FontTests class
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
#include "FontTests.h"
#include "IND_Font.h"
#include "IND_Entity2d.h"

void FontTests::prepareTests() {
	// ----- Surface loading -----
    CIndieLib* iLib = CIndieLib::instance();

	// Font 1
	iLib->_fontManager->addMudFont(_fonts[0], const_cast<char *>("font_big.png"), const_cast<char *>("font_big.xml"), IND_ALPHA, IND_32);
	iLib->_fontManager->addMudFont(_fonts[1], const_cast<char *>("font_big.png"), const_cast<char *>("font_big.xml"), IND_ALPHA, IND_32);
	iLib->_fontManager->addMudFont(_fonts[2], const_cast<char *>("font_big.png"), const_cast<char *>("font_big.xml"), IND_ALPHA, IND_32);
}


void FontTests::performTests(float dt) {
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

bool FontTests::isActive(){
    return (ManualTests::isActive());
}
    
void FontTests::setActive(bool active){
    ManualTests::setActive(active);

    CIndieLib *iLib = CIndieLib::instance();
    //IF - Active
    if(active){
	    // ----- Set the surfaces into 2d entities -----
	    for (int i = 0; i < _testedEntities; ++i) {
			_entities[i]->setFont(_fonts[i]);
			iLib->_entity2dManager->add(_entities[i]);
		}

	    // ----- Changing the attributes of the 2d entities -----
		char fontTextLeft[] = "Linespacing 35, \ncharSpacing 0. \nIND_LEFT";	
		_entities[0]->setText(fontTextLeft);	
		_entities[0]->setLineSpacing	(35);
		_entities[0]->setCharSpacing	(0);
		_entities[0]->setPosition(350, 40, 1);
		_entities[0]->setAlign(IND_LEFT);

		char fontTextCenter[] = "Linespacing 25, \ncharSpacing 15. \nIND_CENTER";
		_entities[1]->setText(fontTextCenter);	
		_entities[1]->setLineSpacing	(25);
		_entities[1]->setCharSpacing	(15);
		_entities[1]->setPosition(350, 200, 1);
		_entities[1]->setAlign(IND_CENTER);

		char fontTextRight[] = "Linespacing 20, \ncharSpacing -15. \nIND_RIGHT";
		_entities[2]->setText(fontTextRight);	
		_entities[2]->setLineSpacing	(20);
		_entities[2]->setCharSpacing	(-15);
		_entities[2]->setPosition(350, 400, 1);
		_entities[2]->setAlign(IND_RIGHT);



    } else { //Inactive
		//Release all variables from indieLib before exiting
		for (int i = 0; i < _testedEntities; ++i) {
			iLib->_entity2dManager->remove(_entities[i]);
		}
    }
}

//-----------------------------------PRIVATE METHODS----------------------------

void FontTests::init() {
	_testedEntities = 3;
	_fonts = new IND_Font*[_testedEntities];
	_entities = new IND_Entity2d*[_testedEntities];

	//Create underlying entities
	for (int i = 0; i < _testedEntities; ++i) {
		_fonts[i] = IND_Font::newFont();
		_entities[i] = IND_Entity2d::newEntity2d();
	}
}

void FontTests::release() {
	CIndieLib* iLib = CIndieLib::instance();
	//Release all variables from indieLib before exiting
	for (int i = 0; i < _testedEntities; ++i) {
		iLib->_fontManager->remove(_fonts[i]);
		iLib->_entity2dManager->remove(_entities[i]);
	}

	DISPOSEARRAY(_fonts);
	DISPOSEARRAY(_entities);
}
