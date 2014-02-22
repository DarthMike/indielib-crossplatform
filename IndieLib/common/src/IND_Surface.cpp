/*****************************************************************************************
 * File: IND_Surface.cpp
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


// ----- Includes -----

#include "Global.h"
#include "IND_Surface.h"
#include "IND_Math.h"
#include "TextureDefinitions.h"

#ifdef PLATFORM_LINUX
#include <stdlib.h>
#endif

// --------------------------------------------------------------------------------
//					 IND_Surface object implementation concept
// --------------------------------------------------------------------------------

// To be able to render image of any size (Direct3d /Opengl only supports textures power
// of two) IndieLib divides the image in blocks (textures). The size of that blocks depends
// on the size of the image and the maximum texture size allowed by the 3d card.
// So, from now, we will call the images "surfaces", that are in fact a
// group of blocks. (Note: that these "surfaces" are not the same thing as Direct3d surfaces)
//
// IndieLib implements this surfaces in the IND_Surface class. An example of an IND_Surface object will be:
// --------------------------------------------------------------------------------------------------------
// The numbers indicate the order in which the blocks will be rendered
// To avoiding showing the spare areas, that quads of the border are adjusted with proper uv coordinates
//       ___________
//      |___|___|_ 9|
//      |_7_|_8_|_|_|
//      | 4 | 5 |6| |
//      |___|___|_|_|
//      | 1 | 2 |3| |
//      |___|___|_|_|
//
// On the screen, after adjusting the quad and uv coordiantes the surface will be rendered as follows,
// it will be rendered only the image, not the spare areas
//
//       _________
//      |_7_|_8_|_|9
//      | 4 | 5 |6|
//      |___|___|_|
//      | 1 | 2 |3|
//      |___|___|_|
//

// --------------------------------------------------------------------------------
//										Public Methods
// --------------------------------------------------------------------------------

/**
 *  Factory method to build instances of this class.
 *
 *  You should never allocate instances by calling new or allocating objects in the stack.
 *  All memory related to this instance is managed by Indielib internally, and you must add it or remove it from
 *  a manager.
 *
 *  @return A correctly initialized instance
 */
IND_Surface* IND_Surface::newSurface() {
    return new IND_Surface();
}

void IND_Surface::destroy() {
    delete this;
}

/**
 * Returns the type of the surface. See ::IND_Type.
 */
IND_Type IND_Surface::getTypeInt() {
	return _surface->_attributes._type;
}

/**
 * Returns the quality of the surface. See ::IND_Quality.
 */
IND_Quality IND_Surface::getQualityInt() {
	return _surface->_attributes._quality;
}

/**
 * Returns the number of textures the surface is using.
 */
int IND_Surface::getNumTextures() {
	return _surface->_attributes._numTextures;
}

/**
 * Returns the width of the surface.
 */
int IND_Surface::getWidth() {
	return _surface->_attributes._width;
}

/**
 * Returns the height of the surface.
 */
int IND_Surface::getHeight() {
	return _surface->_attributes._height;
}

/**
 * Returns the number of blocks (width) in which the surface is divided.
 */
int IND_Surface::getBlocksX() {
	return _surface->_attributes._blocksX;
}

/**
 * Returns the number of blocks (height) in which the surface is divided.
 */
int IND_Surface::getBlocksY() {
	return _surface->_attributes._blocksY;
}

/**
 * Returns the number of blocks in which the surface is divided.
 */
int IND_Surface::getNumBlocks() {
	return _surface->_attributes._numBlocks;
}

/**
 * Returns the width of each block in which the surface is divided.
 */
int IND_Surface::getWidthBlock() {
	return _surface->_attributes._widthBlock;
}

/**
 * Returns the height of each block in which the surface is divided.
 */
int IND_Surface::getHeightBlock() {
	return _surface->_attributes._heightBlock;
}

/**
 * Returns 1 if the surface is loaded.
 */
bool IND_Surface::isHaveSurface() {
	return _surface->_attributes._isHaveSurface;
}

/**
 * Returns 1 if the surface has a grid assigned.
 */
bool IND_Surface::isHaveGrid() {
	return _surface->_attributes._isHaveGrid;
}

/**
 * Returns the type of surface in a string.
 */
string IND_Surface::getTypeString() {
	return TypeToString(_surface->_attributes._type);
}

/**
 * Returns the quality of the surface in a string. See ::IND_Quality.
 */
string IND_Surface::getQualityString() {
	return QualityToString(_surface->_attributes._quality);
}

/**
 * Returns the internal vertexes of a surface
 */
CUSTOMVERTEX2D* IND_Surface::getVertexArray() {
    return _surface->_vertexArray;
}

/**
 * TODO: Description.
 */
int IND_Surface::getSpareX() {
	return _surface->_attributes._spareX;
}

/**
 * TODO: Description.
 */
int IND_Surface::getSpareY() {
	return _surface->_attributes._spareY;
}

/**
 * Sets a grid to the ::IND_Surface object. A grid is just a mesh which vertices
 * can be moved in order to deform the graphical object. You can set grids of different levels
 * of tesselation.
 *
 * Using grids you can apply lot of different morphing effects to your sprites or animations
 * (waves, bubble animation, etc). It is also possible to change the position of all the vertices
 * so it would be possible to create for example a "snake" sprite that could simulate the crawling
 * when moving.
 * 
 * There is a restriction: the amount of horizontal and vertical blocks should be power of two.
 *
 * Example:
 * - SetGrid ( 4, 4) => Correct!
 * - SetGrid (16, 2) => Correct!
 * - SetGrid ( 1, 8) => Correct!
 * - SetGrid ( 1, 3) => Incorrect!
 *
 * @param pNumBlocksX					Number of horizontal blocks.
 * @param pNumBlocksY					Number of vertical blocks.
 */
bool IND_Surface::setGrid(int pNumBlocksX, int pNumBlocksY) {
	// At least one block
	if (pNumBlocksX < 1 || pNumBlocksY < 1) return 0;

	// Only power of two values allowed
	IND_Math mMath;
	if (!mMath.isPowerOfTwo(pNumBlocksX) || !mMath.isPowerOfTwo(pNumBlocksY)) return 0;

	// Only 1-texture-IND_Surfaces allowed
	if (getNumTextures() != 1) return 0;

	// Reset attributes
	_surface->_attributes._isHaveGrid        = 1;
	_surface->_attributes._blocksX           = pNumBlocksX;
	_surface->_attributes._blocksY           = pNumBlocksY;
	_surface->_attributes._widthBlock        = (_surface->_attributes._width / _surface->_attributes._blocksX);
	_surface->_attributes._heightBlock       = (_surface->_attributes._height / _surface->_attributes._blocksY);
	_surface->_attributes._numBlocks         = _surface->_attributes._blocksX * _surface->_attributes._blocksY;
	
	// Reset the vertex array
	DISPOSEARRAY(_surface->_vertexArray);
	_surface->_vertexArray = new CUSTOMVERTEX2D [_surface->_attributes._blocksX * _surface->_attributes._blocksY * 4];

	// Current position of the vertex
	int _posX = 0;
	int _posY = _surface->_attributes._height;
	int mPosZ = 0;

	// Position in which we are storing a vertex
	int mPosVer = 0;

	// Create the new vertex array
	// We iterate the blocks starting from the lower row
	// We MUST draw the blocks in this order, because the image starts drawing from the lower-left corner
	for (int i = getBlocksY(); i > 0; i--) {
		for (int j = 1; j < getBlocksX() + 1; j++) {
			// ----- Block creation (using the position, uv coordiantes and texture) -----

			// We push into the buffer the 4 vertices of the block

			// Normal block
			if (i != 1 && j !=  getBlocksX()) {
				push4Vertices(_surface->_vertexArray,                     // Pointer to the buffer
				              mPosVer,                                    // Position in wich we are storing a vertex
				              _posX,                                      // x
				              _posY,                                      // y
				              mPosZ,                                      // z
				              _surface->_attributes._widthBlock,          // Block width
				              _surface->_attributes._heightBlock,         // Block height
				              _surface->_attributes._width,               // U mapping coordinate
				              _surface->_attributes._height);             // V mapping coordinate
			}

			// The ones of the right column
			if (i != 1 && j ==  getBlocksX()) {
				push4Vertices(_surface->_vertexArray,                     // Pointer to the buffer
				              mPosVer,                                    // Position in wich we are storing a vertex
				              _posX,                                      // x
				              _posY,                                      // y
				              mPosZ,                                      // z
				              _surface->_attributes._widthBlock,          // Block width
				              _surface->_attributes._heightBlock,         // Block height
				              _surface->_attributes._width,               // U mapping coordinate
				              _surface->_attributes._height);             // V mapping coordinate
			}

			// The ones of the upper row
			if (i == 1 && j != getBlocksX()) {
				push4Vertices(_surface->_vertexArray,                     // Pointer to the buffer
				              mPosVer,                                    // Position in wich we are storing a vertex
				              _posX,                                      // x
				              _posY,                                      // y
				              mPosZ,                                      // z
				              _surface->_attributes._widthBlock,          // Block width
				              _surface->_attributes._heightBlock,         // Block height
				              _surface->_attributes._width,               // U mapping coordinate
				              _surface->_attributes._height);             // V mapping coordinate
			}

			// The one of the upper-right corner
			if (i == 1 && j ==  getBlocksX()) {
				push4Vertices(_surface->_vertexArray,                     // Pointer to the buffer
				              mPosVer,                                    // Position in wich we are storing a vertex
				              _posX,                                      // x
				              _posY,                                      // y
				              mPosZ,                                      // z
				              _surface->_attributes._widthBlock,          // Block width
				              _surface->_attributes._heightBlock,         // Block height
				              _surface->_attributes._width,               // U mapping coordinate
				              _surface->_attributes._height);             // V mapping coordinate
			}

			// ----- Advance -----

			// Increase the vertex's position by 4
			mPosVer += 4;

			// ----- Column change -----

			// We point to the next block
			_posX += _surface->_attributes._widthBlock;
		}

		// ----- Row change -----

		// We point to the next block
		_posX = 0;
		_posY -= _surface->_attributes._heightBlock;
	}

	return 1;
}

/**
 * Changes the position of one of the vertices of the grid. The vertices starts
 * from 0 to n, from left to right and from up to down. So: (0,0) is the upper-left vertex
 * of the grid and (n, n) is the lower-right vertex of the grid.
 *
 * Remember that there is always one vertex more (horizontal or vertical) that number of blocks.
 * For example, a grid of 2x2 blocks would have 3x3 vertices.
 * @param pVertexX, pVertexY				The vertex we want to move.
 * @param pX, pY					The new position for the vertex.
 */
bool IND_Surface::setVertexPos(int pVertexX, int pVertexY, int pX, int pY) {
	if (!isHaveGrid()) return 0;

	// ----- Corners (we must move 1 vertex) -----

	if (pVertexX == 0 && pVertexY == 0) {
		_surface->_vertexArray [((getNumBlocks() - getBlocksX()) * 4) + 2]._pos._x = (float) pX;
		_surface->_vertexArray [((getNumBlocks() - getBlocksX()) * 4) + 2]._pos._y = (float) pY;
		return 1;
	}

	if (pVertexX == getBlocksX() && pVertexY == getBlocksY()) {
		_surface->_vertexArray [((getBlocksX() - 1) * 4) + 1]._pos._x = (float) pX;
		_surface->_vertexArray [((getBlocksX() - 1) * 4) + 1]._pos._y = (float) pY;
		return 1;
	}

	if (pVertexX == getBlocksX() && pVertexY == 0) {
		_surface->_vertexArray [(getNumBlocks() - 1) * 4]._pos._x = (float) pX;
		_surface->_vertexArray [(getNumBlocks() - 1) * 4]._pos._y = (float) pY;
		return 1;
	}

	if (pVertexX == 0 && pVertexY == getBlocksY()) {
		_surface->_vertexArray [3]._pos._x = (float) pX;
		_surface->_vertexArray [3]._pos._y = (float) pY;
		return 1;
	}

	// ----- Borders (we must move 2 vertices) -----

	if (pVertexX == 0) {
		_surface->_vertexArray [(abs(getBlocksY() - pVertexY - 1) * getBlocksX() * 4) + 2]._pos._x = (float) pX;
		_surface->_vertexArray [(abs(getBlocksY() - pVertexY - 1) * getBlocksX() * 4) + 2]._pos._y = (float) pY;
		_surface->_vertexArray [(abs(getBlocksY() - pVertexY) * getBlocksX() * 4) + 3]._pos._x = (float) pX;
		_surface->_vertexArray [(abs(getBlocksY() - pVertexY) * getBlocksX() * 4) + 3]._pos._y = (float) pY;
		return 1;
	}

	if (pVertexX == getBlocksX()) {
		_surface->_vertexArray [(((abs(getBlocksY() - pVertexY - 1) * getBlocksX()) + getBlocksX() - 1) * 4)]._pos._x = (float) pX;
		_surface->_vertexArray [(((abs(getBlocksY() - pVertexY - 1) * getBlocksX()) + getBlocksX() - 1) * 4)]._pos._y = (float) pY;
		_surface->_vertexArray [(((abs(getBlocksY() - pVertexY) * getBlocksX()) + getBlocksX() - 1) * 4) + 1]._pos._x = (float) pX;
		_surface->_vertexArray [(((abs(getBlocksY() - pVertexY) * getBlocksX()) + getBlocksX() - 1) * 4) + 1]._pos._y = (float) pY;
		return 1;
	}

	if (pVertexY == getBlocksY()) {
		_surface->_vertexArray [((pVertexX - 1) * 4) + 1]._pos._x = (float) pX;
		_surface->_vertexArray [((pVertexX - 1) * 4) + 1]._pos._y = (float) pY;
		_surface->_vertexArray [((pVertexX) * 4) + 3]._pos._x = (float) pX;
		_surface->_vertexArray [((pVertexX) * 4) + 3]._pos._y = (float) pY;
		return 1;
	}

	if (pVertexY == 0) {
		_surface->_vertexArray [((getNumBlocks() - getBlocksX() + pVertexX - 1) * 4)]._pos._x = (float) pX;
		_surface->_vertexArray [((getNumBlocks() - getBlocksX() + pVertexX - 1) * 4)]._pos._y = (float) pY;
		_surface->_vertexArray [((getNumBlocks() - getBlocksX() + pVertexX) * 4) + 2]._pos._x = (float) pX;
		_surface->_vertexArray [((getNumBlocks() - getBlocksX() + pVertexX) * 4) + 2]._pos._y = (float) pY;
		return 1;
	}

	// ----- Inside (we must move 4 vertices) -----

	if (pVertexX > 0 && pVertexX < getBlocksX() && pVertexY > 0 && pVertexY < getBlocksY()) {
		_surface->_vertexArray [(((abs(getBlocksY() - pVertexY - 1) * getBlocksX()) + pVertexX - 1) * 4)]._pos._x = (float) pX;
		_surface->_vertexArray [(((abs(getBlocksY() - pVertexY - 1) * getBlocksX()) + pVertexX - 1) * 4)]._pos._y = (float) pY;
		_surface->_vertexArray [(((abs(getBlocksY() - pVertexY - 1) * getBlocksX()) + pVertexX) * 4) + 2]._pos._x = (float) pX;
		_surface->_vertexArray [(((abs(getBlocksY() - pVertexY - 1) * getBlocksX()) + pVertexX) * 4) + 2]._pos._y = (float) pY;
		_surface->_vertexArray [(((abs(getBlocksY() - pVertexY) * getBlocksX()) + pVertexX - 1) * 4) + 1]._pos._x = (float) pX;
		_surface->_vertexArray [(((abs(getBlocksY() - pVertexY) * getBlocksX()) + pVertexX - 1) * 4) + 1]._pos._y = (float) pY;
		_surface->_vertexArray [(((abs(getBlocksY() - pVertexY) * getBlocksX()) + pVertexX) * 4) + 3]._pos._x = (float) pX;
		_surface->_vertexArray [(((abs(getBlocksY() - pVertexY) * getBlocksX()) + pVertexX) * 4) + 3]._pos._y = (float) pY;
		return 1;
	}

	return 0;
}

/**
 * Returns the horizontal position of the vertex passed as parameter.
 *
 * Remember that there is always one vertex more (horizontal or vertical) than number of blocks.
 * For example, a grid of 2x2 blocks would have 3x3 vertices.
 * @param pVertexX, pVertexY				One vertex of the grid.
 */
int IND_Surface::getVertexPosX(int pVertexX, int pVertexY) {
	if (!isHaveGrid()) return 0;

	// ----- Corners -----

	if (pVertexX == 0 && pVertexY == 0)
		return (int) _surface->_vertexArray [((getNumBlocks() - getBlocksX()) * 4) + 2]._pos._x;

	if (pVertexX == getBlocksX() && pVertexY == getBlocksY())
		return (int) _surface->_vertexArray [((getBlocksX() - 1) * 4) + 1]._pos._x;

	if (pVertexX == getBlocksX() && pVertexY == 0)
		return (int) _surface->_vertexArray [(getNumBlocks() - 1) * 4]._pos._x;

	if (pVertexX == 0 && pVertexY == getBlocksY())
		return (int) _surface->_vertexArray [3]._pos._x;

	// ----- Borders -----

	if (pVertexX == 0)
		return (int) _surface->_vertexArray [(abs(getBlocksY() - pVertexY - 1) * getBlocksX() * 4) + 2]._pos._x;

	if (pVertexX == getBlocksX())
		return (int) _surface->_vertexArray [(((abs(getBlocksY() - pVertexY) * getBlocksX()) + getBlocksX() - 1) * 4) + 1]._pos._x;

	if (pVertexY == getBlocksY())
		return (int) _surface->_vertexArray [((pVertexX) * 4) + 3]._pos._x;

	if (pVertexY == 0)
		return (int) _surface->_vertexArray [((getNumBlocks() - getBlocksX() + pVertexX - 1) * 4)]._pos._x;

	// ----- Inside (we must move 4 vertices) -----

	if (pVertexX > 0 && pVertexX < getBlocksX() && pVertexY > 0 && pVertexY < getBlocksY())
		return (int) _surface->_vertexArray [(((abs(getBlocksY() - pVertexY - 1) * getBlocksX()) + pVertexX - 1) * 4)]._pos._x;

	return 0;
}

/**
 * Returns the vertical position of the vertex passed as parameter.
 *
 * Remember that there is always one vertex more (horizontal or vertical) than number of blocks.
 * For example, a grid of 2x2 blocks would have 3x3 vertices.
 * @param pVertexX, pVertexY				One vertex of the grid.
 */
int IND_Surface::getVertexPosY(int pVertexX, int pVertexY) {
	if (!isHaveGrid()) return 0;

	// ----- Corners -----

	if (pVertexX == 0 && pVertexY == 0)
		return (int) _surface->_vertexArray [((getNumBlocks() - getBlocksX()) * 4) + 2]._pos._y;

	if (pVertexX == getBlocksX() && pVertexY == getBlocksY())
		return (int) _surface->_vertexArray [((getBlocksX() - 1) * 4) + 1]._pos._y;

	if (pVertexX == getBlocksX() && pVertexY == 0)
		return (int) _surface->_vertexArray [(getNumBlocks() - 1) * 4]._pos._y;

	if (pVertexX == 0 && pVertexY == getBlocksY())
		return (int) _surface->_vertexArray [3]._pos._y;

	// ----- Borders -----

	if (pVertexX == 0)
		return (int) _surface->_vertexArray [(abs(getBlocksY() - pVertexY - 1) * getBlocksX() * 4) + 2]._pos._y;

	if (pVertexX == getBlocksX())
		return (int) _surface->_vertexArray [(((abs(getBlocksY() - pVertexY) * getBlocksX()) + getBlocksX() - 1) * 4) + 1]._pos._y;

	if (pVertexY == getBlocksY())
		return (int) _surface->_vertexArray [((pVertexX) * 4) + 3]._pos._y;

	if (pVertexY == 0)
		return (int) _surface->_vertexArray [((getNumBlocks() - getBlocksX() + pVertexX - 1) * 4)]._pos._y;

	// ----- Inside (we must move 4 vertices) -----

	if (pVertexX > 0 && pVertexX < getBlocksX() && pVertexY > 0 && pVertexY < getBlocksY())
		return (int) _surface->_vertexArray [(((abs(getBlocksY() - pVertexY - 1) * getBlocksX()) + pVertexX - 1) * 4)]._pos._y;

	return 0;
}

// --------------------------------------------------------------------------------
//										Private methods
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
==================
Init at construction time
==================
*/
void IND_Surface::init() {
    if (!_surface) {
    	_surface = new SURFACE(1,1);
    }
}

/*
==================
Release at destruction time
==================
*/
void IND_Surface::release() {
    freeTextureData();
}

/*
==================
Private sets
==================
*/
void IND_Surface::setTypeInt(int pType) {
	_surface->_attributes._type = pType;
}
void IND_Surface::setQualityInt(int pQuality) {
	_surface->_attributes._quality = pQuality;
}
void IND_Surface::setWidth(int pWidth){
	_surface->_attributes._width = pWidth;
}
void IND_Surface::setHeight(int pHeight) {
	_surface->_attributes._height = pHeight;
}
void IND_Surface::setBlocksX(int pBlocksX) {
	_surface->_attributes._blocksX = pBlocksX;
}
void IND_Surface::setBlocksY(int pBlocksY) {
	_surface->_attributes._blocksY = pBlocksY;
}
void IND_Surface::setNumBlocks(int pNumBlocks) {
	_surface->_attributes._numBlocks = pNumBlocks;
}
void IND_Surface::setWidthBlock(int pWidthBlock) {
	_surface->_attributes._widthBlock = pWidthBlock;
}
void IND_Surface::setHeightBlock(int pHeightBlock) {
	_surface->_attributes._heightBlock = pHeightBlock;
}

/*
==================
Release texture data in render side
==================
*/
void IND_Surface::freeTextureData() {
   if (!_surface) return;

    // Free textures
    int numTextures (getNumTextures());
    
    if(0 != numTextures) {
        //Free all texture handles from renderer
#ifdef INDIERENDER_DIRECTX
        //LOOP - All textures
        for (int i = 0; i < numTextures; i++) {
            _surface->_texturesArray [i]._texture->Release();
        }//LOOP END
#endif
#ifdef INDIERENDER_OPENGL
		glDeleteTextures(numTextures,_surface->_texturesArray);
#endif
	}

    _surface->_attributes._isHaveSurface = 0;
    _surface->_attributes._blocksX = 0;
    _surface->_attributes._blocksY = 0;
    _surface->_attributes._numTextures = 0;
    
    DISPOSE(_surface);
}

/*
==================
Push a vertex into the buffer
==================
*/
void IND_Surface::pushVertex(CUSTOMVERTEX2D *pVertices,
                             int pPosVert,
                             int pVx,
                             int pVy,
                             int pVz,
                             float pU,
                             float pV) {

	pVertices [pPosVert]._pos._x = static_cast<float>(pVx);
	pVertices [pPosVert]._pos._y = static_cast<float>(pVy);
	pVertices [pPosVert]._pos._z = static_cast<float>(pVz);
	pVertices [pPosVert]._texCoord._u = pU;
	pVertices [pPosVert]._texCoord._v = pV;
}

/*
==================
Push 4 vertices into the buffer creating a quad
==================
*/
void IND_Surface::push4Vertices(CUSTOMVERTEX2D *pVertices,
                                int pPosVert,
                                int pX,
                                int pY,
                                int pZ,
                                int pWidthBlock,
                                int pHeightBlock,
                                int pWidth,
                                int pHeight) {
	// Push the 4 vertex of the quad
	// The pushing order is important

	// Upper-right
	pushVertex(pVertices,
	           pPosVert,
	           pX + pWidthBlock,
	           pY - pHeightBlock,
	           pZ,
	           (float)(pX + pWidthBlock) / pWidth,
	           1.0f - ((float)(pY - pHeightBlock) / pHeight));

	// Lower-right
	pushVertex(pVertices,
	           pPosVert + 1,
	           pX + pWidthBlock,
	           pY,
	           pZ,
	           (float)(pX + pWidthBlock) / pWidth,
	           1.0f - ((float) pY / pHeight));

	// Upper-left
	pushVertex(pVertices,
	           pPosVert + 2,
	           pX,
	           pY - pHeightBlock,
	           pZ,
	           (float) pX / pWidth,
	           1.0f - ((float)(pY - pHeightBlock) / pHeight));

	// Lower-left
	pushVertex(pVertices,
	           pPosVert + 3,
	           pX,
	           pY,
	           pZ,
	           (float) pX / pWidth,
	           1.0f - ((float) pY / pHeight));
}

/*
==================
Format to string
==================
*/
string IND_Surface::TypeToString(IND_Type pType) {
	switch (pType) {
	case IND_OPAQUE:
		return "IND_OPAQUE";
	case IND_ALPHA:
		return "IND_ALPHA";

	default:
		return "TYPE_NOT_IDENTIFIED";
	}
}

/*
==================
Quality to string
==================
*/
string IND_Surface::QualityToString(IND_Quality pQuality) {
	switch (pQuality) {
	case IND_GREY_8:
		return "IND_GREY_8";
	case IND_GREY_16:
		return "IND_GREY_16";
	case IND_16:
		return "IND_16";

	case IND_32:
		return "IND_32";

	default:
		return "QUALITY_NOT_IDENTIFIED";
	}
}

/** @endcond */
