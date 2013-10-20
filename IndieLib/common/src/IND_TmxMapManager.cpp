/*****************************************************************************************
 * File: IND_TmxMapManager.cpp
 * Desc: TmxMap manager
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


// ----- Includes -----

#include "Defines.h"
#include "Global.h"
#include "dependencies/TmxParser/Tmx.h"
#include "dependencies/FreeImage/Dist/FreeImage.h"
#include "IND_TmxMapManager.h"
#include "IND_TmxMap.h"
#include "IND_Surface.h"
#include "IND_Render.h" 

#ifdef PLATFORM_LINUX
#include <string.h>
#endif


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
This function returns 1 (true) if the adminstrator is successfully initialized.
Must be called before using any method.
*/
bool IND_TmxMapManager::init(IND_Render* render) {
	end();
	initVars();

	g_debug->header("Initializing TmxMapManager", DebugApi::LogHeaderBegin);
	g_debug->header("Preparing TmxMapManager", DebugApi::LogHeaderOk);
	_render = render;
	_ok = true;

	g_debug->header("TmxMapManager OK", 6);
	
	//TODO: REGISTER ERROR HANDLERS FOR FREEIMAGE

	return _ok;
}


/**
@b Operation:

This function frees the manager and all the objects that it contains.
*/
void IND_TmxMapManager::end() {
	if (_ok) {
		g_debug->header("Finalizing TmxMapManager", DebugApi::LogHeaderBegin);
		g_debug->header("Freeing TmxMaps" , DebugApi::LogHeaderBegin);
		freeVars();
		g_debug->header("TmxMaps freed", 6);
		g_debug->header("TmxMapManager finalized", 6);

		_ok = false;
	}
}


// --------------------------------------------------------------------------------
//									Public methods
// --------------------------------------------------------------------------------

/**
@b Parameters:

@arg @b pNewTmxMap            Pointer no a new IND_TmxMap object.
@arg @b pName                 TmxMap filename

@b Operation:

This function returns 1 (true) if the image object passed as a parameter
exists and is added successfully to the manager.

It supports the following file formats:
tmx.

*/
bool IND_TmxMapManager::add(IND_TmxMap *pNewTmxMap,const char *pName) {
	g_debug->header("Loading TmxMap", DebugApi::LogHeaderBegin);
	
	if(!pName) {
		g_debug->header("Invalid File name provided (null)",DebugApi::LogHeaderError);
		return 0;
	}

	g_debug->header("File name:", DebugApi::LogHeaderInfo);
	g_debug->dataChar(pName, 1);

	if (!_ok) {
		writeMessage();
		return 0;
	}

	// ----- Obtaining and checking file extension -----

	char ext [128];
	getExtensionFromName(pName,ext);
	if (!checkExtImage(ext)){
		g_debug->header("Unknown extension", DebugApi::LogHeaderError);
		return 0;
	}
	

	// ----- Load TmxMap -----

	Tmx::Map *map = new Tmx::Map();
	map->ParseFile(pName);
    
	if (map->HasError()) {
		g_debug->header("Error code:", 2);
		//g_debug->dataChar(map->GetErrorCode(), 1); //TODO
		g_debug->header("Error text:", 2);
		g_debug->dataChar(map->GetErrorText().c_str(), 1);
		
        DISPOSE(map);
		
        return 0;
	}

    
   	// ----- Load TmxMap tilesetimagesheet -----
    
    
    string tmxPath;
    string imagePath;
    string s = string(pName);
    
    size_t lastPosTemp = s.find_last_of("\\/");
    
    if(lastPosTemp == string::npos){
        tmxPath = "./";
    }
    else{
        tmxPath = s.substr(0, lastPosTemp + 1);
    }
    
    
    imagePath = tmxPath.append(map->GetTileset(0)->GetImage()->GetSource());  // FIXME : this is very wrong we need to store an array of images instead.... i.e NO '0'
    
    
    // ----- Load image -----

	FREE_IMAGE_FORMAT imgFormat =  FreeImage_GetFileType(imagePath.c_str(), 0);
	if (FIF_UNKNOWN == imgFormat) {
		g_debug->header("Image not found", 2);
        DISPOSE(map);
        return 0;
    }
	FIBITMAP* image = FreeImage_Load(imgFormat, imagePath.c_str(), 0);
	if (!image) {
		g_debug->header("Image could not be loaded", 2);
        DISPOSE(map);
		return 0;
	}

	
	 // ----- Attributes -----
	
    pNewTmxMap->setTmxMapHandle(map);
	pNewTmxMap->setName(pName);
    pNewTmxMap->setImage(image);                    // FIXME should be added to an array
    pNewTmxMap->setImagePath(imagePath.c_str());    // FIXME should be added to an array
    
	// ----- Puts the object into the manager -----
    
    addToList(pNewTmxMap);

	// ----- g_debug -----
    
    g_debug->header("TmxMap loaded", 6);

    
	return 1;
}



/**
@b Parameters:

@arg @b pMap                 Pointer to an IND_TmxMap object.

@b Operation:

This function returns 1(true) if the IND_TmxMap object passed as parameter exists and it is
deleted from the manager successfully.
*/
bool IND_TmxMapManager::remove(IND_TmxMap *pMap) {
	g_debug->header("Freeing Map", DebugApi::LogHeaderBegin);

	if (!_ok || !pMap) {
		writeMessage();
		return 0;
	}

	// Search object
	bool mIs = 0;
	list <IND_TmxMap *>::iterator mMapListIter;
	for (mMapListIter  = _listMaps->begin();
	        mMapListIter != _listMaps->end();
	        mMapListIter++) {
		if ((*mMapListIter) == pMap) {
			mIs = 1;
			break;
		}
	}

	// Not found
	if (!mIs) {
		writeMessage();
		return 0;
	}

	// ----- Free object -----

//FIXME	g_debug->header("File name:", DebugApi::LogHeaderInfo);
//FIXME	g_debug->dataChar(pIm->getName(), 1);

	// Quit from list
	delFromlist(pMap);

	// Free Map
//FIXME	FreeImage_Unload(pIm->getFreeImageHandle());	

	g_debug->header("Ok", DebugApi::LogHeaderEnd);

	return 1;
}


/**
@b Parameters:

@arg @b pNewTmxMap            Pointer to a new IND_TmxMap object
@arg @b pOldTmxMap            Pointer to the IND_TmxMAp we want to clone

@b Operation:

This function returns 1 (true) if the image object passed as a parameter
exists and is added successfully to the manager creating a new image cloning from a previous one.
*/
bool IND_TmxMapManager::clone(IND_TmxMap *pNewTmxMap, IND_TmxMap *pOldTmxMap) { // FIXME: is a clone method needed or should it be removed
//	g_debug->header("Cloning Image", DebugApi::LogHeaderBegin);

//	if (!_ok || !pNewImage || !pOldImage || !pOldImage->getFreeImageHandle()) {
//		writeMessage();

//		return 0;
//	}

//	// ----- Image creation using FreeImage -----

//	// Clone the old image

//	FIBITMAP* image = FreeImage_Clone(pOldImage->getFreeImageHandle());
//	if (!image) {
//		g_debug->header("Image could not be cloned", DebugApi::LogHeaderError);
//		return 0;
//	}
	
//	// Attributes

//	pNewImage->setWidth(pOldImage->getWidth());
//	pNewImage->setHeight(pOldImage->getHeight());
//	pNewImage->setBpp(pOldImage->getBpp());
//	pNewImage->setBytespp(pOldImage->getBytespp());
//	pNewImage->setFormatInt(pOldImage->getFormatInt());
//	char ext [128];
//	getExtensionFromName(pOldImage->getName(),ext);
//	pNewImage->setExtension(ext);
//	pNewImage->setPointer(FreeImage_GetBits(image));
//	pNewImage->setName("Image cloned");
//	pNewImage->setFreeImageHandle(image);

//	// ----- Put the object into the manager -----

//	addToList(pNewImage);

	// ----- g_debug -----

//	g_debug->header("File name:", DebugApi::LogHeaderInfo);
//	g_debug->dataChar(pOldImage->getName(), 1);
//	g_debug->header("Image cloned", DebugApi::LogHeaderEnd);

	return 1;
}

/**
@b Parameters:

@arg @b orthogonalMap           The map to render
@arg @b mSurfaceOrthogonalTiles The surface to use when rendering

@b Operation:

Call this method to render a TMX map as ortogonal tiles. 
*/
void IND_TmxMapManager::renderOrthogonalMap(IND_TmxMap *orthogonalMap,IND_Surface *mSurfaceOrthogonalTiles, int kMapCenterOffset) {
    
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
                
				// This doesn't compile when we build DLL, as we must export a dependency class.
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
                
                
                _render->setTransform2d(destX + kMapCenterOffset,   // x pos - Added center because we start in 0,0 (corner of screen)
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
                                            NULL);                   // Matrix in wich the transformation will be applied (optional)
                
                // We apply the color, blending and culling transformations.
                _render->setRainbow2d(IND_ALPHA,                    // IND_Type
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
                _render->blitRegionSurface(mSurfaceOrthogonalTiles, sourceX, sourceY, sourceWidth, sourceHeight);
            }
        }
    }
    
}


/**
@b Parameters:

@arg @b isometricMap           The map to render
@arg @b mSurfaceIsometricTiles The surface to use when rendering

@b Operation:

Call this method to render a TMX map as isometric tiles. 
*/
void IND_TmxMapManager::renderIsometricMap(IND_TmxMap *isometricMap,IND_Surface *mSurfaceIsometricTiles, int kMapCenterOffset) {
    
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
                
                
                _render->setTransform2d(destX + kMapCenterOffset,   // x pos - Added center because we start in 0,0 (corner of screen)
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
                                            NULL);                   // Matrix in wich the transformation will be applied (optional)
                
                // We apply the color, blending and culling transformations.
                _render->setRainbow2d(IND_ALPHA,                    // IND_Type
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
                _render->blitRegionSurface(mSurfaceIsometricTiles, sourceX, sourceY, sourceWidth, sourceHeight);
            }
        }
    }
}


void IND_TmxMapManager::renderStaggeredMap(IND_TmxMap *staggeredMap,IND_Surface *mSurfaceStaggeredTiles, int kMapCenterOffset){
    //TODO: MFK implement this
}



// --------------------------------------------------------------------------------
//									Private methods
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
==================
Obtain file extension
Return "" if the file hasn't got an extension
==================
*/
void IND_TmxMapManager::getExtensionFromName(const char *pName, char* pExtImage) {
	
	int i;
	// The search starts at the end of the name
	for (i = strlen(pName); i > -1; i--)
		if (pName [i] == '.') 
			break;

	// Image without extension
	if (pName [i] != '.')
		strcpy (pExtImage, "");

	// Copy the extension
	strcpy(pExtImage, &pName [i + 1]);
}


/*
==================
Check if it is a known extension
==================
*/
bool IND_TmxMapManager::checkExtImage(const char *pExtImage) {
	for (int i = 0; i < MAX_EXT_TMXMAP; i++) {
		if (!strcmp(pExtImage, _supportedExt [i]))
			return 1;
	}

	return 0;
}


/*
==================
Inserts object into the manager
==================
*/
void IND_TmxMapManager::addToList(IND_TmxMap *pNewMap) {
	_listMaps->push_back(pNewMap);
}


/*
==================
Deletes object from the manager
==================
*/
void IND_TmxMapManager::delFromlist(IND_TmxMap *pMap) {
	_listMaps->remove(pMap);
	DISPOSEMANAGED(pMap);
}


/*
==================
Initialization error message
==================
*/
void IND_TmxMapManager::writeMessage() {
	g_debug->header("This operation can not be done", DebugApi::LogHeaderInfo);
	g_debug->dataChar("", 1);
	g_debug->header("Invalid Id or IND_TmxMapManager not correctly initialized", DebugApi::LogHeaderError);
}


/*
==================
Init manager vars
==================
*/
void IND_TmxMapManager::initVars() {
	_render = NULL;
	_listMaps = new list <IND_TmxMap *>;

	// Supported extensions
	_supportedExt [0]  = (char*) "tmx";
}


/*
==================
Free manager memory
==================
*/
void IND_TmxMapManager::freeVars() {
// FIXME: free chararray for name and free the TMXMap itself fo each of the IND_TmxMap
//	// Deletes all the manager entities
//	list <IND_Image *>::iterator mImageListIter;
//	for (mImageListIter  = _listImages->begin();
//	        mImageListIter != _listImages->end();
//	        mImageListIter++) {
//			FIBITMAP* handle = (*mImageListIter)->getFreeImageHandle();
//		if (handle) {
//			g_debug->header("Freeing image:", DebugApi::LogHeaderInfo);
//			g_debug->dataChar((*mImageListIter)->getName(), 1);
//
//			// Free image
//			FreeImage_Unload(handle);
//
//			// Free the string of the extension
//			DISPOSEARRAY((*mImageListIter)->_image._ext);
//		}
//	}
//
	// Clear list
	_listMaps->clear();

	// Free list
	DISPOSE(_listMaps);
}

/** @endcond */





