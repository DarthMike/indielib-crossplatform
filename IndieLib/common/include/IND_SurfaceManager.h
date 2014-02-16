/*****************************************************************************************
 * File: IND_SurfaceManager.h
 * Desc: Surface manager
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


#ifndef _IND_SURFACEMANAGER_
#define _IND_SURFACEMANAGER_

// ----- Includes -----

#include <list>
#include "Defines.h"

// ----- Forward declarations -----

class IND_ImageManager;
class TextureBuilder;
class IND_Render;
class IND_Surface;
class IND_Image;

// --------------------------------------------------------------------------------
//							     IND_SurfaceManager
// --------------------------------------------------------------------------------

/**
@defgroup IND_SurfaceManager IND_SurfaceManager
@ingroup Managers
Manager of IND_Surface objects. Click in ::IND_SurfaceManager to see all the methods of this class.
*/
/**@{*/

/**
This class stores 2d surfaces (IND_Surface) that can be inserted into a IND_Entity2d and rendered to
the screen using IND_Entity2dManager::renderEntities2d().

IND_Surface is the most used object
in the library due to its versatility. Suppose that we use @b IndieLib to develop a game.
The IND_Surface object will be the sprite of the main character or enemies, the background scroll,
the tiles of the map, the markups of life, etc.

All graphic entities in @b IndieLib that are drawn in the screen as a surface, including
text and animations, are internally managed by IND_Surface class.

One of the main characteristics of a IND_Surface is that it can have any desired size
(it doesn't matter the maximum texture of the graphic card and they don't need to be power of two).
And you can use a big IND_Surface as s a  @b scroll, without worrying about the areas that are out of
the screen (because blocks outside the viewport will be automatically discarded). Furthermore,
to make scroll creation easier, <b>the size of blocks</b>, in which the surface is
divided when it is created from a ::IND_Image object or directly from a graphic file <b>can be
specified</b>.

There are several types of surfaces (see ::IND_Type), each type is used for a different purpose:
- IND_ALPHA: Per pixel transparency using alpha channel
- IND_OPAQUE: Opaque

To <b>save memory</b> or to get <b>black and white images</b>, it is also possible to specify
differents surfaces qualities (see ::IND_Quality).

<BR>

@image html surfa2.jpg All the graphics entities in IndieLib are internally represented in surfaces
*/

class LIB_EXP IND_SurfaceManager {
public:

	// ----- Init/End -----

	IND_SurfaceManager(): _ok(false)  { }
	~IND_SurfaceManager()              {
		end();
	}

	bool    init(IND_ImageManager *pImageManager, IND_Render *pRender);
	void    end();
	bool    isOK();

	// ----- Public methods -----

	// ----- Not specifying block size -----

	bool    add(IND_Surface    *pNewSurface,
	            const char    *pName,
	            IND_Type        pType,
	            IND_Quality     pQuality);

	bool     add(IND_Surface    *pNewSurface,
	             IND_Image       *pImage,
	             IND_Type        pType,
	             IND_Quality     pQuality);

	bool     add(IND_Surface    *pNewSurface,
	             const char    *pName,
	             IND_Type        pType,
	             IND_Quality     pQuality,
	             unsigned char pR,
	             unsigned char pG,
	             unsigned char pB);

	// ----- Specifying block size -----

	bool add(IND_Surface    *pNewSurface,
	         const char    *pName,
	         int             pBlockSize,
	         IND_Type        pType,
	         IND_Quality     pQuality);

	bool add(IND_Surface    *pNewSurface,
	         IND_Image       *pImage,
	         int             pBlockSize,
	         IND_Type        pType,
	         IND_Quality     pQuality);

	bool add(IND_Surface    *pNewSurface,
	         const char    *pName,
	         int             pBlockSize,
	         IND_Type        pType,
	         IND_Quality     pQuality,
	         unsigned char pR,
	         unsigned char pG,
	         unsigned char pB);

	bool clone(IND_Surface *pNewSurface, IND_Surface *pSurfaceToClone);

	bool remove(IND_Surface *pSu);

private:

	/** @cond DOCUMENT_PRIVATEAPI */

	// ----- Private -----

	bool _ok;

	// ----- Objects -----

	IND_ImageManager *_imageManager;
	IND_Render *_render;
    TextureBuilder *_textureBuilder;

	// ----- Containers -----

    std::list <IND_Surface *> *_listSurfaces;

	// ----- Private methods -----

	bool    addMain(IND_Surface    *pNewSurface,
	                IND_Image       *pImage,
	                int             pBlockSizeX,
	                int             pBlockSizeY,
	                IND_Type        pType,
	                IND_Quality     pQuality);

	bool    calculateAxis(IND_Surface *pSu,
	                    float pAxisX,
	                    float pAxisY,
	                    int *pAxisCalX,
	                    int *pAxisCalY);

	void                addToList(IND_Surface *pNewImage);
	void                delFromlist(IND_Surface *pSu);
	void                writeMessage();
	void				convertImage(IND_Image* pImage ,IND_Type pType, IND_Quality pQuality);
	void                initVars();
	void                freeVars();

    /** @endcond */
};
/**@}*/

#endif // _IND_SURFACEMANAGER_
