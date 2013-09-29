/*****************************************************************************************
 * File: IND_Surface.h
 * Desc: Surface object
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


#ifndef _IND_SURFACE_
#define _IND_SURFACE_

// ----- Includes -----

#include "IndiePlatforms.h"
#include "Defines.h"
#include "IND_Object.h"

// ----- Forward declarations -----
struct SURFACE;

// --------------------------------------------------------------------------------
//									 IND_Surface
// --------------------------------------------------------------------------------

/**
@defgroup IND_Surface IND_Surface
@ingroup Objects
IND_Surface class managed by IND_SurfaceManager for displaying surfaces and animations into the screen. Click in IND_Surface to see all the methods of this class.
*/
/**@{*/

/**
@b IND_Surface is an surface object from the class ::IND_SurfaceManager. Read the explanation in ::IND_SurfaceManager for more details.
*/
class LIB_EXP IND_Surface : public IND_Object {
public:
    
    static IND_Surface* newSurface();
    virtual void destroy();
    
	// ----- Public Methods ------

	bool        setGrid(int pNumBlocksX, int pNumBlocksY);
	bool        setVertexPos(int pVertexX, int pVertexY, int pX, int pY);
	int         getVertexPosX(int pVertexX, int pVertexY);
	int         getVertexPosY(int pVertexX, int pVertexY);

	// ----- Public gets ------

	//! This function returns the type of the surface. See ::IND_Type.
	IND_Type    getTypeInt();
	//! This function returns the quality of the surface. See ::IND_Quality.
	IND_Quality getQualityInt();
	//! This function returns the number of textures the surface is using.
	int         getNumTextures();
	//! This function returns the width of the surface.
	int         getWidth();
	//! This function returns the height of the surface.
	int         getHeight();
	//! This function returns the number of blocks (width) in which the surface is divided.
	int         getBlocksX();
	//! This function returns the number of blocks (height) in which the surface is divided.
	int         getBlocksY();
	//! This function returns the number of blocks in which the surface is divided.
	int         getNumBlocks();
	//! This function returns the width of each block in which the surface is divided.
	int         getWidthBlock();
	//! This function returns the height of each block in which the surface is divided.
	int         getHeightBlock();
	//! This function returns 1 if the surface is loaded.
	bool        isHaveSurface();
	//! This function returns 1 if the surface has a grid assigned.
	bool        isHaveGrid();
	//! This function returns the type of surface in a string.
	string      getTypeString();
	//! This function returns the quality of the surface in a string. See ::IND_Quality.
	string      getQualityString();

	int         getSpareX();
	int         getSpareY();

private:
	/** @cond DOCUMENT_PRIVATEAPI */
    IND_Surface (): _surface(NULL) {
        init();
    }
    
    ~IND_Surface () {
        release();
    }
    
	// ----- Structures ------
	SURFACE *_surface;

	// ----- Private sets ------

	void setTypeInt(int pType);
	void setQualityInt(int pQuality);
	void setWidth(int pWidth);
	void setHeight(int pHeight);
	void setBlocksX(int pBlocksX);
	void setBlocksY(int pBlocksY);
	void setNumBlocks(int pNumBlocks);
	void setWidthBlock(int pWidthBlock);
	void setHeightBlock(int pHeightBlock);

	// ----- Private methods ------

    void init();
    void release();

    void freeTextureData();    //Used to free any render-specific data

	string                  TypeToString(IND_Type pType);
	string                  QualityToString(IND_Quality pQuality);
	void                    pushVertex(CUSTOMVERTEX2D *pVertices,
	                                   int pPosVert,
	                                   int pX,
	                                   int pY,
	                                   int pZ,
	                                   float pU,
	                                   float pV);

	void                    push4Vertices(CUSTOMVERTEX2D *pVertices,
	                                      int pPosVert,
	                                      int pPosX,
	                                      int pPosY,
	                                      int pPosZ,
	                                      int pWidthBlock,
	                                      int pHeightBlock,
	                                      int pWidth,
	                                      int pHeight);

	// ----- Friends -----

	friend class IND_SurfaceManager;
	friend class DirectXTextureBuilder;
	friend class OpenGLTextureBuilder;
	friend class DirectXRender;
	friend class OpenGLRender;
    friend class OpenGLES2Render;
    /** @endcond */
};
/**@}*/

#endif // _IND_SURFACE_
