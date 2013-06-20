/*****************************************************************************************
 * File: IND_SurfaceManager.cpp
 * Desc: Surface manager
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
MODIFIED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)

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

// ----- Includes -----

#include "Global.h"
#include "IND_SurfaceManager.h"
#include "IND_ImageManager.h"
#include "IND_Render.h"
#include "IND_Surface.h"
#include "TextureDefinitions.h"
#include "IND_Image.h"
#include <assert.h>

#ifdef INDIERENDER_DIRECTX
#include "render/directX/DirectXTextureBuilder.h"
#endif

#ifdef INDIERENDER_GLES_IOS
#include "render/opengl/OpenGLTextureBuilder.h"
#endif

#ifdef INDIERENDER_OPENGL
#include "render/opengl/OpenGLTextureBuilder.h"
#endif

// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
@b parameters:

@arg @b pImageManager           Pointer to IND_ImageManager correctly initialized
@arg @b pRender                 Pointer to IND_Render correctly initialized

@b Operation:

This function returns 1 (true) when the manager is correctly initialized.
It should be called before using any method.
*/
bool IND_SurfaceManager::init(IND_ImageManager *pImageManager, IND_Render *pRender) {
	end();
	initVars();

	g_debug->header("Initializing SurfaceManager", DebugApi::LogHeaderBegin);

	// Checking IND_Render
	if (pRender->isOK()) {
		g_debug->header("Checking IND_Render", DebugApi::LogHeaderOk);
		_render = pRender;
	} else {
		g_debug->header("IND_Render is not correctly initialized", DebugApi::LogHeaderError);
		_ok = false;
		return _ok;
	}

	// Checking IND_ImageManager
	if (pImageManager->isOK()) {
		g_debug->header("Checking IND_ImageManager", DebugApi::LogHeaderOk);
		_imageManager = pImageManager;

		// Texture Builder
#ifdef INDIERENDER_DIRECTX
		_textureBuilder = new DirectXTextureBuilder(pImageManager, _render);
#endif

#ifdef INDIERENDER_GLES_IOS
		_textureBuilder = new OpenGLTextureBuilder(pImageManager, _render);
#endif

#ifdef INDIERENDER_OPENGL
		_textureBuilder = new OpenGLTextureBuilder(pImageManager, _render);
#endif
		_ok = true;

		g_debug->header("SurfaceManager OK", DebugApi::LogHeaderEnd);
	} else {
		g_debug->header("ImageManager is not correctly initialized", DebugApi::LogHeaderError);
		_ok = false;
	}

	return _ok;
}


/**
@b Operation:

This function frees the manager and all the objects that it contains.
*/
void IND_SurfaceManager::end() {
	if (_ok) {
		g_debug->header("Finalizing SurfaceManager", DebugApi::LogHeaderBegin);
		g_debug->header("Freeing surfaces" , DebugApi::LogHeaderBegin);
		freeVars();
		g_debug->header("Surfaces freed", DebugApi::LogHeaderEnd);
		g_debug->header("IND_SurfaceManager finalized", DebugApi::LogHeaderEnd);

		_ok = false;
	}
}

/**
 Returns state of initialization.
 @return  Will give true if object initialized correctly, false otherwise
 */
bool IND_SurfaceManager::isOK() {
    return _ok;
}


// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

/**
@b parameters:

@arg @b pNewSurface             Pointer to a new surface object
@arg @b pName                   Name of the file that contains the image
@arg @b pType                   Type of surface (see ::IND_Type)
@arg @b pQuality                Quality of the surface (see ::IND_Quality)

@b Operation:

This function returns 1 (true) if the parameter surface object exists and it is added
by loading the image directly from the file.

Graphic formats supported (Thanks to http://freeimage.sourceforge.net ):
bmp, png, tga, jpg and pcx.
*/
bool IND_SurfaceManager::add(IND_Surface    *pNewSurface,
                             const char    *pName,
                             IND_Type        pType,
                             IND_Quality     pQuality) {
	// Loads the image
	IND_Image *mNewImage = IND_Image::newImage();

    bool noError(true);
    noError = _imageManager->add(mNewImage, pName);

	// Surface creation
	if (noError) {
        addMain(pNewSurface, mNewImage, 0, 0, pType, pQuality);
	}

	// Free image
    if (!noError) {
        DISPOSEMANAGED(mNewImage);
    }
    
	_imageManager->remove(mNewImage);

	return noError;
}


/**
@b parameters:

@arg @b pNewSurface             Pointer to a new surface object
@arg @b pImage                  Pointer to a ::IND_Image object
@arg @b pType                   Type of surface (see ::IND_Type)
@arg @b pQuality                Quality of surface (see ::IND_Quality)

@b Operation:

This function returns 1 (true) if the parameter surface object exists and it is added
to the manager, loading the image directly from a ::IND_Image object.
*/
bool IND_SurfaceManager::add(IND_Surface    *pNewSurface,
                             IND_Image       *pImage,
                             IND_Type        pType,
                             IND_Quality     pQuality) {
	// Surface creation
	if (!addMain(pNewSurface, pImage, 0, 0, pType, pQuality))
		return 0;

	return 1;
}


/**
@b parameters:

@arg @b pNewSurface             Pointer to a new surface object
@arg @b pName                   Name of the file that contains the image
@arg @b pType                   Type of surface (see ::IND_Type)
@arg @b pQuality                Quality of the surface (see ::IND_Quality)
@arg <b>pR, pG, pB</b>          Color from which the colorkey will be applied, this areas will become transparent.


@b Operation:

This function returns 1 (true) if the parameter surface object exists and it is added
satisfactory loading the image directly from the file and all the colors equal to RGB=(pR, pG, pB), will
become transparent.

Graphic formats supported (Thanks to http://freeimage.sourceforge.net/ ):
bmp, png, tga, jpg and pcx.
*/
bool IND_SurfaceManager::add(IND_Surface    *pNewSurface,
                             const char    *pName,
                             IND_Type        pType,
                             IND_Quality     pQuality,
                             BYTE            pR,
                             BYTE            pG,
                             BYTE            pB) {
    // Loads the image
	IND_Image *mNewImage = IND_Image::newImage();
    
    bool noError(true);
    noError = _imageManager->add(mNewImage, pName);

    // Color key
    if (noError) {
        mNewImage->setAlpha(pR, pG, pB);
    }
    
	// Surface creation
	if (noError) {
        addMain(pNewSurface, mNewImage, 0, 0, pType, pQuality);
	}
    
	// Free image
    if (!noError) {
        DISPOSEMANAGED(mNewImage);
    }
    
	_imageManager->remove(mNewImage);
    
	return noError;
}


/**
@b parameters:

@arg @b pNewSurface             Pointer to a new surface object
@arg @b pName                   Name of the file that contains the image
@arg @b pBlockSize              Width of the blocks
@arg @b pType                   Type of surface (see ::IND_Type)
@arg @b pQuality                Quality of surface (see ::IND_Quality)

@b Operation:

This function returns 1 (true) if the parameter surface object exists and it is added satisfactory
to the manager loading the image directly from a file. This method is useful for creating scrolls because we
can specify the width of the block. Blocks outside the viewport will be automatically discarded.

Graphic formats supported:
bmp, png, tga, jpg and pcx.
*/
bool IND_SurfaceManager::add(IND_Surface    *pNewSurface,
                             const char    *pName,
                             int             pBlockSize,
                             IND_Type        pType,
                             IND_Quality     pQuality) {
    // Loads the image
	IND_Image *mNewImage = IND_Image::newImage();
    
    bool noError(true);
    noError = _imageManager->add(mNewImage, pName);
    
	// Surface creation
	if (noError) {
        addMain(pNewSurface, mNewImage, pBlockSize, pBlockSize, pType, pQuality);
	}
    
	// Free image
    if (!noError) {
        DISPOSEMANAGED(mNewImage);
    }
    
	_imageManager->remove(mNewImage);
    
	return noError;
}


/**
@b parameters:

@arg @b pNewSurface             Pointer to a new surface object
@arg @b pImage                  Pointer to a  ::IND_Image object
@arg @b pBlockSize              Width of the blocks.
@arg @b pType                   Type of surface (see ::IND_Type)
@arg @b pQuality                Quality of surface (see ::IND_Quality)

@b Operation:

This function returns 1 (true) if the parameter surface object exists and it is added
to the manager loading the image directly from a file. This method is useful for creating scrolls because we
can specify the width of the block. Blocks outside the viewport will be automatically discarded.
*/
bool IND_SurfaceManager::add(IND_Surface    *pNewSurface,
                             IND_Image       *pImage,
                             int             pBlockSize,
                             IND_Type        pType,
                             IND_Quality     pQuality) {
	if (!addMain(pNewSurface, pImage, pBlockSize, pBlockSize, pType, pQuality))
		return 0;

	return 1;
}


/**
@b parameters:

@arg @b pNewSurface             Pointer to a new surface object
@arg @b pName                   Name of the file that contains the image
@arg @b pBlockSize              Width of the blocks
@arg @b pType                   Type of surface (see ::IND_Type)
@arg @b pQuality                Quality of surface (see ::IND_Quality)
@arg <b>pR, pG, pB</b>          Color from which the colorkey will be applied, this areas will become transparent.

@b Operation:

This function returns 1 (true) if the parameter surface object exists and it is added satisfactory
to the manager loading the image directly from a file. This method is useful for creating scrolls because we
can specify the width of the block. Blocks outside the viewport will be automatically discarded.

Graphic formats supported (Thanks to http://freeimage.sourceforge.net ):
bmp, png, tga, jpg and pcx.
*/
bool IND_SurfaceManager::add(IND_Surface    *pNewSurface,
                             const char    *pName,
                             int             pBlockSize,
                             IND_Type        pType,
                             IND_Quality     pQuality,
                             BYTE pR,
                             BYTE pG,
                             BYTE pB) {
    // Loads the image
	IND_Image *mNewImage = IND_Image::newImage();
    
    bool noError(true);
    noError = _imageManager->add(mNewImage, pName);
    
    // Color key
    if (noError) {
        mNewImage->setAlpha(pR, pG, pB);
    }
    
	// Surface creation
	if (noError) {
        addMain(pNewSurface, mNewImage, pBlockSize, pBlockSize, pType, pQuality);
	}
    
	// Free image
    if (!noError) {
        DISPOSEMANAGED(mNewImage);
    }
    
	_imageManager->remove(mNewImage);
    
	return noError;
}

/**
@b parameters:

@arg @b pNewSurface             Pointer to a new surface object
@arg @b pSurfaceToClone         Surface we want to clone

@b Operation:

This function returns 1 (true) if the parameter surface object exists and a new surface is created
cloned from a previous existing one. The new surface will share the texture data from the "father"
but will have it's own grid data. Show, if you want to have several ::IND_Surface objects
with different grid assigned (see IND_Surface::setGrid()) this is the way to go.
*/
bool IND_SurfaceManager::clone(IND_Surface *pNewSurface, IND_Surface *pSurfaceToClone) {
	assert (pNewSurface);
    assert (pSurfaceToClone);

    if(!pNewSurface || !pSurfaceToClone)
        return false;
    
    // Copy attributes
	pNewSurface->_surface->_attributes._type = pSurfaceToClone->_surface->_attributes._type;
	pNewSurface->_surface->_attributes._quality = pSurfaceToClone->_surface->_attributes._quality;
	pNewSurface->_surface->_attributes._width = pSurfaceToClone->_surface->_attributes._width;
	pNewSurface->_surface->_attributes._height = pSurfaceToClone->_surface->_attributes._height;
	pNewSurface->_surface->_attributes._blocksX = pSurfaceToClone->_surface->_attributes._blocksX;
	pNewSurface->_surface->_attributes._blocksY = pSurfaceToClone->_surface->_attributes._blocksY;
	pNewSurface->_surface->_attributes._spareX = pSurfaceToClone->_surface->_attributes._spareX;
	pNewSurface->_surface->_attributes._spareY = pSurfaceToClone->_surface->_attributes._spareY;
	pNewSurface->_surface->_attributes._numBlocks = pSurfaceToClone->_surface->_attributes._numBlocks;
	pNewSurface->_surface->_attributes._widthBlock = pSurfaceToClone->_surface->_attributes._widthBlock;
	pNewSurface->_surface->_attributes._heightBlock = pSurfaceToClone->_surface->_attributes._heightBlock;
	pNewSurface->_surface->_attributes._isHaveSurface = pSurfaceToClone->_surface->_attributes._isHaveSurface;
	pNewSurface->_surface->_attributes._isHaveGrid = pSurfaceToClone->_surface->_attributes._isHaveGrid;

	// Reference to texture
	pNewSurface->_surface->_texturesArray =  pSurfaceToClone->_surface->_texturesArray;

	// Copy vertex data
	int _numVertices = pSurfaceToClone->getBlocksX() * pSurfaceToClone->getBlocksY() * 4;
	pNewSurface->_surface->_vertexArray = new CUSTOMVERTEX2D [_numVertices];

	for (int i = 0; i < _numVertices; i++)
		pNewSurface->_surface->_vertexArray [i] = pSurfaceToClone->_surface->_vertexArray[i];

	return 1;
}


/**
@b parameters:

@arg @b pSu             Pointer to a new surface object

@b Operation:

This function returns 1 (true) if the parameter surface object exists and it is satisfactory deleted from the manager.
*/
bool IND_SurfaceManager::remove(IND_Surface *pSu) {
	g_debug->header("Freeing surface", DebugApi::LogHeaderBegin);

	if (!_ok || !pSu) {
		writeMessage();
		return false;
	}

	// Search object
	bool mIs = false;
	list <IND_Surface *>::iterator mSurfaceListIter;
	for (mSurfaceListIter  = _listSurfaces->begin();
	        mSurfaceListIter != _listSurfaces->end();
	        mSurfaceListIter++) {
		if ((*mSurfaceListIter) == pSu) {
			mIs = true;
			break;
		}
	}

	if (!mIs) {
		writeMessage();
		return false;
	}

	// ----- Free object -----

	// Quit from list
	delFromlist(pSu);

	g_debug->header("Ok", DebugApi::LogHeaderEnd);

	return true;
}


// --------------------------------------------------------------------------------
//										Private methods
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
==================
Add main (All public Add use this)
==================
*/
bool IND_SurfaceManager::addMain(IND_Surface    *pNewSurface,
                                 IND_Image       *pImage,
                                 int             pBlockSizeX,
                                 int             pBlockSizeY,
                                 IND_Type        pType,
                                 IND_Quality     pQuality) {
	g_debug->header("Creating surface", DebugApi::LogHeaderBegin);

	if (!_ok || !pNewSurface || !pImage) {
		writeMessage();
		return 0;
	}

	g_debug->header("From image:", DebugApi::LogHeaderInfo);
	g_debug->dataChar(pImage->getName(), 1);

	//Convert image if needed
	convertImage(pImage,pType,pQuality);
	
	if (_textureBuilder->createNewTexture(pNewSurface, pImage, pBlockSizeX, pBlockSizeY)) {
		//TODO: ERROR DEBUG FILE
	}
	assert(pNewSurface);

	// ----- Puts the object into the manager  -----

	addToList(pNewSurface);

	// ----- g_debug -----


	g_debug->header("Type:", DebugApi::LogHeaderInfo);
	g_debug->dataChar(pNewSurface->getTypeString(), 1);

	g_debug->header("Quality:", DebugApi::LogHeaderInfo);
	g_debug->dataChar(pNewSurface->getQualityString(), 1);

	g_debug->header("Image size:", DebugApi::LogHeaderInfo);
	g_debug->dataInt(pNewSurface->_surface->_attributes._width, 0);
	g_debug->dataChar("x", 0);
	g_debug->dataInt(pNewSurface->_surface->_attributes._height, 1);

	g_debug->header("Block size:", DebugApi::LogHeaderInfo);
	g_debug->dataInt(pNewSurface->_surface->_attributes._widthBlock, 0);
	g_debug->dataChar("x", 0);
	g_debug->dataInt(pNewSurface->_surface->_attributes._heightBlock, 1);

	g_debug->header("Number of blocks:", DebugApi::LogHeaderInfo);
	g_debug->dataInt(pNewSurface->_surface->_attributes._blocksX, 0);
	g_debug->dataChar("x", 0);
	g_debug->dataInt(pNewSurface->_surface->_attributes._blocksY, 1);

	g_debug->header("Spare (Right | Down):", DebugApi::LogHeaderInfo);
	g_debug->dataInt(pNewSurface->_surface->_attributes._spareX, 0);
	g_debug->dataChar("x", 0);
	g_debug->dataInt(pNewSurface->_surface->_attributes._spareY, 1);

	//TODO: LOG % NOT USED
	//g_debug->Header ("Not used percentage:", DebugApi::LogHeaderInfo);
	//g_debug->DataFloat (pNewSurface->_surface->_attributes., 0);
	//g_debug->DataChar ("%", 1);

	g_debug->header("Surface created", DebugApi::LogHeaderEnd);

	return 1;
}


/*
==================
This function returns 1 (true) if the parameter surface object exists and it returns in
 @b pAxisCalX and @b pAxisCalY the values necessary to specify the hotspot when we
 want to call the function IND_Render::SetTransform() (before drawing the surface).

This function is used to change the point of reference of the image. The point of reference
 or hotspot is an imaginary point from which the translation, rotation and range transformations
 are applied. This way we can originate the rotation of asurface over its center, over
 one of its vertexes or over any intermediate location.

To seat the hotspot we @b usepAxisX and @b pAxisY that have a range from 0 to 1, and refer
respectively to the horizontal position and vertical position of the surface. So, (0.5f, 0.5f)
is the center of the image and  (1.0f, 0.0f) is the top right corner, etc.

The results values (pAxisCalX and pAxisCalY) should be stored and used later as parameters
to the function IND_Render::SetTransform(), in such way, when the image is drawn
the hotspot specified os used.

When we use two null values for the function IND_Render::SetTransform() the default values
would be used, the default hotspot is the top left corner of the object.

This function can be very difficult to use with each surface separately. It is optimal
to apply it over a list of surfaces that the user can manage. An other option of using this
function is using  ::IND_Entity2dManager to manage all the objects of  IndieLib and its
attributes efficiently.
==================
*/
bool IND_SurfaceManager::calculateAxis(IND_Surface *pSu,
                                     float pAxisX,
                                     float pAxisY,
                                     int *pAxisCalX,
                                     int *pAxisCalY) {
	if (!_ok || !pSu) {
		return 0;
	}

	if (pAxisX < 0.0f || pAxisX > 1.0f)
		pAxisX = 0.0f;

	if (pAxisY < 0.0f || pAxisY > 1.0f)
		pAxisY = 0.0f;


	*pAxisCalX = (int)(pAxisX * pSu->_surface->_attributes._width   * -1);
	*pAxisCalY = (int)(pAxisY * pSu->_surface->_attributes._height  * -1);

	return 1;
}

/*
==================
Inserts object into the manager
==================
*/
void IND_SurfaceManager::addToList(IND_Surface *pNewImage) {
	_listSurfaces->push_back(pNewImage);
}


/*
==================
Deletes object from the manager
==================
*/
void IND_SurfaceManager::delFromlist(IND_Surface *pSu) {
	_listSurfaces->remove(pSu);
	DISPOSEMANAGED(pSu);
}


/*
==================
Writes a message in the log that the object was not initialized
==================
*/
void IND_SurfaceManager::writeMessage() {
	g_debug->header("This operation can not be done", DebugApi::LogHeaderInfo);
	g_debug->dataChar("", 1);
	g_debug->header("Invalid Id or SurfaceManager not correctly initialized", DebugApi::LogHeaderError);
}

/*
==================
Analizes existing image object and converts it to the needed type to render as requested and as supported
==================
*/
void IND_SurfaceManager::convertImage(IND_Image* pImage ,IND_Type pType, IND_Quality pQuality) {
	//Convert implicitly the color format if in colour index to RGB
	//This is to reduce complexities in renderer, and because GL ES doesn't support colour index
	//formats.
	if (IND_COLOUR_INDEX == pImage->getFormatInt()) {
		//Ignore quality specifier (we assume is IND_16)
		if (IND_ALPHA == pType) {
			pImage->convert(IND_RGBA, 32);
			g_debug->header("Image type converted to IND_RGBA implicitly" , DebugApi::LogHeaderWarning);
		} else {
			pImage->convert(IND_RGB, 16);
			g_debug->header("Image type converted to IND_RGB implicitly" , DebugApi::LogHeaderWarning);
		}
	}
	
	//Convert 24-bit RGBs. That's because 24-bit RGB format is non standard for many cards, and it's not performant.
	if (IND_RGB == pImage->getFormatInt()) {
		switch (pQuality) {
			case IND_GREY_8:
				//TODO
				break;
			case IND_GREY_16:
				//TODO
				break;
			case IND_16:
				if (IND_ALPHA == pType) {
					g_debug->header("Image type converted to 16-bit RGBA implicitly." , DebugApi::LogHeaderOk);
					pImage->convert(IND_RGBA, 16);
				} else {
					g_debug->header("Image type converted to 16-bit RGB implicitly." , DebugApi::LogHeaderOk);
					pImage->convert(pImage->getFormatInt(), 16);
				}
				break;
			case IND_32:
				if (IND_ALPHA == pType) {
					g_debug->header("Image type converted to 32-bit RGBA implicitly." , DebugApi::LogHeaderOk);
					pImage->convert(IND_RGBA, 32);
				} else {
					g_debug->header("Image type converted to 32-bit RGB implicitly." , DebugApi::LogHeaderOk);
					pImage->convert(pImage->getFormatInt(), 32);
				}
				break;
			default:
				g_debug->header("No good Quality specified when creating surface" , DebugApi::LogHeaderWarning);
				g_debug->header("Image type converted to 32-bit RGB implicitly." , DebugApi::LogHeaderWarning);
				pImage->convert(pImage->getFormatInt(), 32);
				break;
		}
	}
}
/*
==================
Init manager vars
==================
*/
void IND_SurfaceManager::initVars() {
	_listSurfaces = new list <IND_Surface *>;
}


/*
==================
Free manager memory
==================
*/
void IND_SurfaceManager::freeVars() {
	// Deletes all the manager entities
	list <IND_Surface *>::iterator mSurfaceListIter;
	for (mSurfaceListIter  = _listSurfaces->begin();
	        mSurfaceListIter != _listSurfaces->end();
	        mSurfaceListIter++) {
		g_debug->header("Freeing surface:", DebugApi::LogHeaderInfo);
        //FIXME: This breaks on LLVM compiler (OSX, XCode)
		//g_debug->dataInt((int) & (*mSurfaceListIter)->_surface, 1);
        DISPOSEMANAGED((*mSurfaceListIter));
	}

	// Clear list
	_listSurfaces->clear();

	// Free list
	DISPOSE(_listSurfaces);

    //Free Texture builder
    DISPOSE(_textureBuilder);
}

/** @endcond */
