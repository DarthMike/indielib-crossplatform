/*****************************************************************************************
 * File: IND_Surface.h
 * Desc: Surface object
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

#ifndef _IND_SURFACE_
#define _IND_SURFACE_

// ----- Includes -----

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
    /** @endcond */
};
/**@}*/

#endif // _IND_SURFACE_
