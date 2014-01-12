/*****************************************************************************************
 * Desc: FontTests_Pretty class
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
#include "FontTests_Pretty.h"
#include "IND_Font.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"

void FontTests_Pretty::prepareTests() {
	// ----- Surface loading -----
	CIndieLib* iLib = CIndieLib::instance();

	// Loading Background
	iLib->_surfaceManager->add(_surfaceBack, const_cast<char *>("blue_background.jpg"), IND_OPAQUE, IND_32);

	// Loading tip page
	iLib->_surfaceManager->add(_surfaceTip, const_cast<char *>("tip.png"), IND_ALPHA, IND_32);

	// Font 1
	iLib->_fontManager->addMudFont(_fontSmall, const_cast<char *>("font_small.png"), const_cast<char *>("font_small.xml"), IND_ALPHA, IND_32);

	// Font 2
	iLib->_fontManager->addMudFont(_fontBig, const_cast<char *>("font_big.png"), const_cast<char *>("font_big.xml"), IND_ALPHA, IND_32);
}

void FontTests_Pretty::performTests(float dt) {
   
    //IF - Check if test is active
    if(!_active)
        return;

	CIndieLib *iLib = CIndieLib::instance();
	
	//Toggling of entity border lines in entities
	if(iLib->_input->onKeyPress(IND_F1) && _active) {
		_back->showGridAreas(!_back->isShowGridAreas());
		_tip->showGridAreas(!_tip->isShowGridAreas());
		_textTitle->showGridAreas(!_textTitle->isShowGridAreas());
		_textSmallBlack->showGridAreas(!_textSmallBlack->isShowGridAreas());
		_textSmallWhite->showGridAreas(!_textSmallWhite->isShowGridAreas());
	}
}

bool FontTests_Pretty::isActive(){
    return (ManualTests::isActive());
}
    
void FontTests_Pretty::setActive(bool active){
    ManualTests::setActive(active);

    CIndieLib *iLib = CIndieLib::instance();
    //IF - Active
    if(active){
	    // ----- Set the surfaces into 2d entities -----
	    // Creating 2d entity for the background
						
		iLib->_entity2dManager->add(_back);				// Entity adding
		_back->setSurface(_surfaceBack);				// Set the surface into the entity

		// Creating 2d entity for the tips page
						
		iLib->_entity2dManager->add(_tip);				// Entity adding
		_tip->setSurface(_surfaceTip);					// Set the surface into the entity

		// Title text				
		iLib->_entity2dManager->add(_textTitle);			// Entity adding
		_textTitle->setFont(_fontBig);					// Set the font into the entity

		// Text small black			
		iLib->_entity2dManager->add(_textSmallBlack);			// Entity adding
		_textSmallBlack->setFont(_fontSmall);				// Set the font into the entity

		// Text small white				
		iLib->_entity2dManager->add(_textSmallWhite);			// Entity adding
		_textSmallWhite->setFont(_fontSmall);				// Set the font into the entity

		// ----- Changing the attributes of the 2d entities -----
		_tip->setHotSpot(0.5f, 0.5f);
		_tip->setPosition(250, 300, 0);

		char titleText[] = "IndieLib Fonts";
		_textTitle->setText(titleText);	
		_textTitle->setLineSpacing(18);
		_textTitle->setCharSpacing(-8);
		_textTitle->setPosition(400, 40, 1);
		_textTitle->setAlign(IND_CENTER);

		char smallBlackText[] = "This paragraph is\ncentered using.\nIND_CENTER\n\nThis font has\nblack color using\nSetTint() method.\n\nRocks!";
		_textSmallBlack->setText(smallBlackText);	
		_textSmallBlack->setLineSpacing	(18);
		_textSmallBlack->setCharSpacing	(-8);
		_textSmallBlack->setPosition(242, 230, 1);
		_textSmallBlack->setAlign(IND_CENTER);
		_textSmallBlack->setTint(0, 0, 0);

		char smallWhiteText[] = "This is a another paragraph. This time\nusing the font without changing the color\nand with a IND_LEFT alignment.\n\nThis is a really easy way of writing\ntexts, isn't it?";
		_textSmallWhite->setText(smallWhiteText);	
		_textSmallWhite->setLineSpacing(18);
		_textSmallWhite->setCharSpacing(-7);
		_textSmallWhite->setPosition(400, 230, 1);
		_textSmallWhite->setAlign(IND_LEFT);



    } else { //Inactive
		//Release all variables from indieLib before exiting
		iLib->_entity2dManager->remove(_back);
		iLib->_entity2dManager->remove(_tip);
		iLib->_entity2dManager->remove(_textTitle);
		iLib->_entity2dManager->remove(_textSmallBlack);
		iLib->_entity2dManager->remove(_textSmallWhite);
    }
}

//-----------------------------------PRIVATE METHODS----------------------------

void FontTests_Pretty::init() {
	_surfaceBack = IND_Surface::newSurface();
	_surfaceTip = IND_Surface::newSurface();
	_fontSmall = IND_Font::newFont();
	_fontBig = IND_Font::newFont();

	_back = IND_Entity2d::newEntity2d();
	_tip = IND_Entity2d::newEntity2d();
	_textTitle = IND_Entity2d::newEntity2d();
	_textSmallBlack = IND_Entity2d::newEntity2d();					
	_textSmallWhite = IND_Entity2d::newEntity2d();					
}

void FontTests_Pretty::release() {
	CIndieLib* iLib = CIndieLib::instance();
	
	//Release all variables from indieLib before exiting
	iLib->_surfaceManager->remove(_surfaceBack);
	iLib->_surfaceManager->remove(_surfaceTip);
	iLib->_fontManager->remove(_fontSmall);
	iLib->_fontManager->remove(_fontBig);
	iLib->_entity2dManager->remove(_back);
	iLib->_entity2dManager->remove(_tip);
	iLib->_entity2dManager->remove(_textTitle);
	iLib->_entity2dManager->remove(_textSmallBlack);
	iLib->_entity2dManager->remove(_textSmallWhite);
}
