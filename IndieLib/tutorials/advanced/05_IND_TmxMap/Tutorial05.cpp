/*****************************************************************************************
 * Desc: Tutorials b) 05 IND_TmxMap
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
#include "IND_TmxMap.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Font.h"
#include "../../WorkingPath.h"

#include <math.h>
#include <cstring>


void renderIsometricMap(CIndieLib *mI, IND_TmxMap *map,IND_Surface *mSurfaceTiles, int kMapCenterOffset);
void renderOrthogonalMap(CIndieLib *mI, IND_TmxMap *orthogonalMap,IND_Surface *mSurfaceOrthogonalTiles, int kMapCenterOffset);
void renderStaggeredMap(CIndieLib *mI, IND_TmxMap *staggeredMap,IND_Surface *mSurfaceStaggeredTiles, int kMapCenterOffset);

/*
==================
Main
==================
*/
Indielib_Main			
{
    //Sets the working path at the resources directory. Resources paths are relative to that directory
	if (!WorkingPathSetup::setWorkingPath(WorkingPathSetup::resourcesDirectory())) {
		std::cout<<"\nUnable to Set the working path !";
	}
	
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
    if (!mI->init()) return 0;
    
    
    // ----- Font -----
	
    IND_Font *mFontSmall = IND_Font::newFont();
	if (!mI->_fontManager->addMudFont(mFontSmall, "font/bitmap/MudFont/font_small.png", "font/bitmap/MudFont/font_small.xml", IND_ALPHA, IND_32)) return 0;
    
	// ----- Font creation -----
    
	IND_Entity2d *mTextSmallWhite = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mTextSmallWhite);				// Entity adding
	mTextSmallWhite->setFont(mFontSmall);					// Set the font into the entity
	mTextSmallWhite->setLineSpacing(18);
	mTextSmallWhite->setCharSpacing(-8);
	mTextSmallWhite->setPosition(5, 5, 1);
	mTextSmallWhite->setAlign(IND_LEFT);
    
    
    // ----- Map -----
    
    IND_TmxMap *orthogonalMap = IND_TmxMap::newTmxMap();
    if (!mI->_tmxMapManager->add(orthogonalMap, "tmx/example.tmx")) return 0;                  // Orthogonal, multiple Layers, flipped tiles.
    
    IND_TmxMap *isometricMap = IND_TmxMap::newTmxMap();
	if (!mI->_tmxMapManager->add(isometricMap, "tmx/isometric_grass_and_water.tmx")) return 0; // Isometric, one layer, no flipped tiles.
    
    
    IND_Surface *mSurfaceIsometricTiles = IND_Surface::newSurface();
    if (!mI->_surfaceManager->add(mSurfaceIsometricTiles, isometricMap->getImagePath(), IND_ALPHA, IND_32)) return 0;

    IND_Surface *mSurfaceOrthogonalTiles = IND_Surface::newSurface();
    if (!mI->_surfaceManager->add(mSurfaceOrthogonalTiles, orthogonalMap->getImagePath(), IND_ALPHA, IND_32)) return 0;

    
    
    char mText [2048];
	mText [0] = 0;
    int kMapCenterOffset = mI->_window->getWidth() / 2;
    bool mShowIsometric = true;
	
    while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
        // ----- Input update ----
        
		mI->_input->update();
        
        if (mI->_input->onKeyPress(IND_SPACE))
		{                                       // TODO: MFK implement 3 map switching ...
			if (mShowIsometric){
				mShowIsometric = false;
			}else{
				mShowIsometric = true;
			}
		}
        
        // ----- Text -----
        
		strcpy(mText, "Press space to change between the two test maps ( Isometric / Orthogonal ) \n ..... TODO: we still need to handle flipped tiles + not to redraw on every gameloop.");
		mTextSmallWhite->setText(mText);
        
		// ----- Render  -----
        
		mI->_render->beginScene();
		mI->_render->clearViewPort(60, 60, 60);
        
        if(mShowIsometric){
            renderIsometricMap(mI, isometricMap, mSurfaceIsometricTiles, kMapCenterOffset);
        }else{
            renderOrthogonalMap(mI, orthogonalMap, mSurfaceOrthogonalTiles, kMapCenterOffset);
        }
        mI->_entity2dManager->renderEntities2d();
        
        mI->_render->endScene();
        
	}

	// ----- Free -----

	mI->end();

	return 0;
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

