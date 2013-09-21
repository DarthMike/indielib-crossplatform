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

void renderIsometricMap(CIndieLib *mI, IND_TmxMap *map,IND_Surface *mSurfaceTiles, int kMapCenterOffset);
void renderOrthogonalMap(CIndieLib *mI, IND_TmxMap *orthogonalMap,IND_Surface *mSurfaceOrthogonalTiles, int kMapCenterOffset);
void renderStaggeredMap(CIndieLib *mI, IND_TmxMap *staggeredMap,IND_Surface *mSurfaceStaggeredTiles, int kMapCenterOffset);



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
	if(iLib->_input->onKeyPress(IND_SPACE) && _active) {
        if (_showIsometric){
            _showIsometric = false;
        }else{
            _showIsometric = true;
        }
    }
    
    if(_showIsometric){
        renderIsometricMap(iLib, _isometricMap, _surfaceIsometricTiles, _mapCenterOffset);
    }else{
        renderOrthogonalMap(iLib, _orthogonalMap, _surfaceOrthogonalTiles, _mapCenterOffset);
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
        
        strcpy(_text, "Press space to change between the two test maps ( Isometric / Orthogonal ) \n ..... TODO: we still need to handle flipped tiles + not to redraw on every gameloop.");
        _textSmallWhite->setText(_text);
       
        

    } else { //Inactive
        iLib->_entity2dManager->remove(_textSmallWhite);
		
    }

}


void renderOrthogonalMap(CIndieLib *mI, IND_TmxMap *orthogonalMap,IND_Surface *mSurfaceOrthogonalTiles, int kMapCenterOffset) {
    
    IND_Matrix *mMatrix = new IND_Matrix();
    
    // Iterate through the layers.
    for (int i = 0; i < orthogonalMap->getTmxMapHandle()->GetNumLayers(); ++i) {
        
        const Tmx::Layer *layer = orthogonalMap->getTmxMapHandle()->GetLayer(i);
        
        int layerColumns = layer->GetWidth();
        int layerRows = layer->GetHeight();
        for (int x = 0; x < layerColumns; ++x)
        {
            for (int y = 0; y < layerRows; ++y)
            {
                // Get the tile's id.
                int CurTile = layer->GetTileGid(x, y);
                
                // If gid is 0, means empty tile
                if(CurTile == 0)
                {
                    continue;
                }
                
                const Tmx::Tileset *tileset = orthogonalMap->getTmxMapHandle()->FindTileset(CurTile);
                int tilesetColumns = (mSurfaceOrthogonalTiles->getWidth() - 2*tileset->GetMargin()) / tileset->GetTileWidth();
                //int tilesetRows = (mSurfaceOrthogonalTiles->getHeight() - 2*tileset->GetMargin()) / tileset->GetTileHeight();
                
                // 0-based index (as valid gid starts from 1.)
                CurTile--;
                
                int tileset_col = (CurTile % tilesetColumns);
                int tileset_row = (CurTile / tilesetColumns);
                
                int sourceX = (tileset->GetMargin() + (tileset->GetTileWidth() + tileset->GetSpacing()) * tileset_col);
                int sourceY = (tileset->GetMargin() + (tileset->GetTileHeight() + tileset->GetSpacing()) * tileset_row);
                
                int sourceWidth = tileset->GetTileWidth();
                int sourceHeight = tileset->GetTileHeight();
                
                // int mirrorY = layer->IsTileFlippedVertically(x, y) ? 1 : 0;  // TODO : Add this parameter to one of the call below
                // int mirrorX = layer->IsTileFlippedHorizontally(x, y) ? 1 : 0 // TODO : Add this parameter to one of the call below
                // layer->IsTileFlippedDiagonally(x, y) ? 1 : 0;                // TODO : implement this in engine ?
                
                /*
                 TMX orthogonal coordinates are specified starting from top (upper) corner, as 0,0 and continuing (x,y)
                 Column, row . So the the next tile to the right from the statring tile will be 1,0 and the one just below the
                 starting tile will be 0,1.
                 */
                
                int destX = x * orthogonalMap->getTmxMapHandle()->GetTileWidth();
                int destY = y * orthogonalMap->getTmxMapHandle()->GetTileHeight();
                
                
                mI->_render->setTransform2d(destX + kMapCenterOffset,   // x pos - Added center because we start in 0,0 (corner of screen)
                                            destY,                      // y pos
                                            0,                          // Angle x
                                            0,                          // Angle y
                                            0,                          // Angle z
                                            1,                          // Scale x
                                            1,                          // Scale y
                                            0,                          // Axis cal x
                                            0,                          // Axis cal y
                                            0,                          // Mirror x
                                            0,                          // Mirror y
                                            0,                          // Width
                                            0,                          // Height
                                            mMatrix);                   // Matrix in wich the transformation will be applied (optional)
                
                // We apply the color, blending and culling transformations.
                mI->_render->setRainbow2d(IND_ALPHA,                    // IND_Type
                                          1,                            // Back face culling 0/1 => off / on
                                          0,                            // Mirror x
                                          0,                            // Mirror y
                                          IND_FILTER_LINEAR,            // IND_Filter
                                          255,                          // R Component	for tinting
                                          255,                          // G Component	for tinting
                                          255,                          // B Component	for tinting
                                          255,                          // A Component	for tinting
                                          0,                            // R Component	for fading to a color
                                          0,                            // G Component	for fading to a color
                                          0,                            // B Component	for fading to a color
                                          255,                          // Amount of fading
                                          IND_SRCALPHA,                 // IND_BlendingType (source)
                                          IND_INVSRCALPHA);             // IND_BlendingType (destination)
                
                
                
                // Blit the IND_Surface
                mI->_render->blitRegionSurface(mSurfaceOrthogonalTiles, sourceX, sourceY, sourceWidth, sourceHeight);
            }
        }
    }
    
}



void renderIsometricMap(CIndieLib *mI, IND_TmxMap *isometricMap,IND_Surface *mSurfaceIsometricTiles, int kMapCenterOffset) {
    
    IND_Matrix *mMatrix = new IND_Matrix();
    
    // Iterate through the layers.
    for (int i = 0; i < isometricMap->getTmxMapHandle()->GetNumLayers(); ++i) {
        
        const Tmx::Layer *layer = isometricMap->getTmxMapHandle()->GetLayer(i);
        
        int layerColumns = layer->GetWidth();
        int layerRows = layer->GetHeight();
        for (int x = 0; x < layerColumns; ++x)
        {
            for (int y = 0; y < layerRows; ++y)
            {
                // Get the tile's id.
                int CurTile = layer->GetTileGid(x, y);
                
                // If gid is 0, means empty tile
                if(CurTile == 0)
                {
                    continue;
                }
                
                const Tmx::Tileset *tileset = isometricMap->getTmxMapHandle()->FindTileset(CurTile);
                int tilesetColumns = (mSurfaceIsometricTiles->getWidth() - 2*tileset->GetMargin()) / tileset->GetTileWidth();
                //int tilesetRows = (mSurfaceIsometricTiles->getHeight() - 2*tileset->GetMargin()) / tileset->GetTileHeight();
                
                // 0-based index (as valid gid starts from 1.)
                CurTile--;
                
                int tileset_col = (CurTile % tilesetColumns);
                int tileset_row = (CurTile / tilesetColumns);
                
                int sourceX = (tileset->GetMargin() + (tileset->GetTileWidth() + tileset->GetSpacing()) * tileset_col);
                int sourceY = (tileset->GetMargin() + (tileset->GetTileHeight() + tileset->GetSpacing()) * tileset_row);
                
                int sourceWidth = tileset->GetTileWidth();
                int sourceHeight = tileset->GetTileHeight();
                
                // int mirrorY = layer->IsTileFlippedVertically(x, y) ? 1 : 0;  // TODO : Add this parameter to one of the call below
                // int mirrorX = layer->IsTileFlippedHorizontally(x, y) ? 1 : 0 // TODO : Add this parameter to one of the call below
                // layer->IsTileFlippedDiagonally(x, y) ? 1 : 0;                // TODO : implement this in engine ?
                
                /*
                 TMX isometric coordinates are specified starting from top (upper) corner, as 0,0
                 From there, 0,1 will be half tile width to the 'right', 1,0 will be half width to the 'left'
                 */
                
                int destX = ( x * isometricMap->getTmxMapHandle()->GetTileWidth() / 2  ) - ( y * isometricMap->getTmxMapHandle()->GetTileWidth() / 2  );
                int destY = ( y * isometricMap->getTmxMapHandle()->GetTileHeight() / 2 ) + ( x * isometricMap->getTmxMapHandle()->GetTileHeight() / 2 );
                
                
                mI->_render->setTransform2d(destX + kMapCenterOffset,   // x pos - Added center because we start in 0,0 (corner of screen)
                                            destY,                      // y pos
                                            0,                          // Angle x
                                            0,                          // Angle y
                                            0,                          // Angle z
                                            1,                          // Scale x
                                            1,                          // Scale y
                                            0,                          // Axis cal x
                                            0,                          // Axis cal y
                                            0,                          // Mirror x
                                            0,                          // Mirror y
                                            0,                          // Width
                                            0,                          // Height
                                            mMatrix);                   // Matrix in wich the transformation will be applied (optional)
                
                // We apply the color, blending and culling transformations.
                mI->_render->setRainbow2d(IND_ALPHA,                    // IND_Type
                                          1,                            // Back face culling 0/1 => off / on
                                          0,                            // Mirror x
                                          0,                            // Mirror y
                                          IND_FILTER_LINEAR,            // IND_Filter
                                          255,                          // R Component	for tinting
                                          255,                          // G Component	for tinting
                                          255,                          // B Component	for tinting
                                          255,                          // A Component	for tinting
                                          0,                            // R Component	for fading to a color
                                          0,                            // G Component	for fading to a color
                                          0,                            // B Component	for fading to a color
                                          255,                          // Amount of fading
                                          IND_SRCALPHA,                 // IND_BlendingType (source)
                                          IND_INVSRCALPHA);             // IND_BlendingType (destination)
                
                
                
                // Blit the IND_Surface
                mI->_render->blitRegionSurface(mSurfaceIsometricTiles, sourceX, sourceY, sourceWidth, sourceHeight);
            }
        }
    }
}


void renderStaggeredMap(CIndieLib *mI, IND_TmxMap *staggeredMap,IND_Surface *mSurfaceStaggeredTiles, int kMapCenterOffset){
    //TODO: MFK implement this
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
