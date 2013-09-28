/*****************************************************************************************
 * Desc: TmxmapTests class
 *****************************************************************************************/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <michael@visualdesign.dk> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Michael Fogh Kristensen
 * ----------------------------------------------------------------------------
 */

#include "CIndieLib.h"
#include "TmxmapTests.h"
#include "IND_TmxMap.h"
#include "IND_Surface.h"
#include "IND_Entity2D.h"
#include "IND_Font.h"

#include "../WorkingPath.h"

#include <math.h>
#include <cstring>

void TmxmapTests::prepareTests() {
     CIndieLib* iLib = CIndieLib::instance();
    
    
    // ----- Font -----
	
     _fontSmall = IND_Font::newFont();
	iLib->_fontManager->add(_fontSmall, "../../resources/font_small.png", "../../resources/font_small.xml", IND_ALPHA, IND_32);
    
    
	// ----- Text creation -----
    
	_textSmallWhite = IND_Entity2d::newEntity2d();
	
    
    // ----- Map -----
    
    _orthogonalMap = IND_TmxMap::newTmxMap();
    iLib->_tmxMapManager->add(_orthogonalMap, "../../resources/tmx/example.tmx");                  // Orthogonal, multiple Layers, flipped tiles.
    
    _isometricMap = IND_TmxMap::newTmxMap();
	iLib->_tmxMapManager->add(_isometricMap, "../../resources/tmx/isometric_grass_and_water.tmx"); // Isometric, one layer, no flipped tiles.
    
    
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
	if(iLib->_input->onKeyPress(IND_KEYUP) && _active) {
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
        
        strcpy(_text, "Press up to change between the two test maps ( Isometric / Orthogonal ) \n ..... TODO: we still need to handle flipped tiles + not to redraw on every gameloop.");
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
