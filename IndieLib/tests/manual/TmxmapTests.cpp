/*****************************************************************************************
 * Desc: TmxmapTests class
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


#include "../CIndieLib.h"
#include "TmxmapTests.h"
#include "IND_TmxMap.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Font.h"

#include "../WorkingPath.h"

#include <math.h>
#include <cstring>

void TmxmapTests::prepareTests() {
     CIndieLib* iLib = CIndieLib::instance();
    
    
    // ----- Font -----
	
     _fontSmall = IND_Font::newFont();
	iLib->_fontManager->addMudFont(_fontSmall, "font/bitmap/MudFont/font_small.png", "font/bitmap/MudFont/font_small.xml", IND_ALPHA, IND_32);
    
    
	// ----- Text creation -----
    
	_textSmallWhite = IND_Entity2d::newEntity2d();
	
    
    // ----- Map -----
    
    _orthogonalMap = IND_TmxMap::newTmxMap();
    iLib->_tmxMapManager->add(_orthogonalMap, "tmx/example.tmx");                  // Orthogonal, multiple Layers, flipped tiles.
    
    _isometricMap = IND_TmxMap::newTmxMap();
	iLib->_tmxMapManager->add(_isometricMap, "tmx/isometric_grass_and_water.tmx"); // Isometric, one layer, no flipped tiles.
    
    
    _surfaceIsometricTiles = IND_Surface::newSurface();
    iLib->_surfaceManager->add(_surfaceIsometricTiles, _isometricMap->getImagePath(), IND_ALPHA, IND_32);
    
    _surfaceOrthogonalTiles = IND_Surface::newSurface();
    iLib->_surfaceManager->add(_surfaceOrthogonalTiles, _orthogonalMap->getImagePath(), IND_ALPHA, IND_32);

}


void TmxmapTests::performTests(float dt) {
    //IF - Check if test is active
    if(!_active)
        return;
    
	CIndieLib *iLib = CIndieLib::instance();
	
	//Toggling of entity border lines in entities
	if(iLib->_input->onKeyPress(IND_Q) && _active) {
        _showIsometric = !_showIsometric;
    }
    
    if(_showIsometric){
        iLib->_tmxMapManager->renderIsometricMap(_isometricMap, _surfaceIsometricTiles, _mapCenterOffset);
    }else{
        iLib->_tmxMapManager->renderOrthogonalMap(_orthogonalMap, _surfaceOrthogonalTiles, _mapCenterOffset);
    }
    
    
}


bool TmxmapTests::isActive(){
    return (ManualTests::isActive());
}


void TmxmapTests::setActive(bool active){
    ManualTests::setActive(active);
    
    CIndieLib *iLib = CIndieLib::instance();
    //IF - Active
    if(active){
	    
        // ----- Set the surfaces into 2d entities -----
	    
        iLib->_entity2dManager->add(_textSmallWhite);				// Entity adding
	    
        
        // ----- Changing the attributes of the 2d entities -----
        
        _textSmallWhite->setFont(_fontSmall);					// Set the font into the entity
        _textSmallWhite->setLineSpacing(18);
        _textSmallWhite->setCharSpacing(-8);
        _textSmallWhite->setPosition(5, 5, 1);
        _textSmallWhite->setAlign(IND_LEFT);
        
        strcpy(_text, "Press 'q' to change between the two test maps ( Isometric / Orthogonal ) \n ..... TODO: we still need to handle flipped tiles + not to redraw on every gameloop.");
        _textSmallWhite->setText(_text);
       
        

    } else { //Inactive
        iLib->_entity2dManager->remove(_textSmallWhite);
		
    }

}

//-----------------------------------PRIVATE METHODS----------------------------

void TmxmapTests::init() {
    CIndieLib* iLib = CIndieLib::instance();
    _mapCenterOffset = iLib->_window->getWidth() / 2;
    _text [0] = 0;
    _showIsometric = true;

    
    // TODO: MFK
}

void TmxmapTests::release() {
    //TODO: MFK
}
