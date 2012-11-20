/*****************************************************************************************
 * Desc: FontTests_Pretty class
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
THIS FILE IS AN ADDITIONAL FILE ADDED BY iLibguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / iLibkeskywalker007@gmail.com), BUT HAS THE
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
#include "FontTests_Pretty.h"
#include "IND_Font.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"

void FontTests_Pretty::prepareTests() {
	// ----- Surface loading -----
	CIndieLib* iLib = CIndieLib::instance();

	// Loading Background
	iLib->_surfaceManager->add(_surfaceBack, const_cast<char *>("../../resources/blue_background.jpg"), IND_OPAQUE, IND_32);

	// Loading tip page
	iLib->_surfaceManager->add(_surfaceTip, const_cast<char *>("../../resources/tip.png"), IND_ALPHA, IND_32);

	// Font 1
	iLib->_fontManager->add(_fontSmall, const_cast<char *>("../../resources/font_small.png"), const_cast<char *>("../../resources/font_small.xml"), IND_ALPHA, IND_32);

	// Font 2
	iLib->_fontManager->add(_fontBig, const_cast<char *>("../../resources/font_big.png"), const_cast<char *>("../../resources/font_big.xml"), IND_ALPHA, IND_32);
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
	_surfaceBack = new IND_Surface();
	_surfaceTip = new IND_Surface();
	_fontSmall = new IND_Font();
	_fontBig = new IND_Font();

	_back = new IND_Entity2d();
	_tip = new IND_Entity2d();
	_textTitle = new IND_Entity2d();
	_textSmallBlack = new IND_Entity2d();					
	_textSmallWhite = new IND_Entity2d();					
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
