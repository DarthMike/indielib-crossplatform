/*****************************************************************************************
 * File: ImageCutter.cpp
 * Desc: This class is used for cutting bitmap areas
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (javierlopezpro@gmail.com)

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
#include "ImageCutter.h"
#include "IND_ImageManager.h"
#include "IND_Image.h"

#ifdef PLATFORM_LINUX
#include <string.h>
#endif


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
 * Init.
 * @param pImageManager		TODO: describtion
 * @param pMaxTextureSize	TODO: describtion
 */
bool ImageCutter::init(IND_ImageManager *pImageManager, int pMaxTextureSize) {
	end();
	initVars();

	_imageManager = pImageManager;
	_maxTextureSize = pMaxTextureSize;

	_ok = true;
	return _ok;
}


/**
 * End.
 */
void ImageCutter::end() {
	if (_ok) {
		freeVars();
		_ok = false;
	}
}


// --------------------------------------------------------------------------------
//							        Public methods
// --------------------------------------------------------------------------------

/**
 * Get the information necessary in order to cut the image.
 * @param pImage		TODO: describtion
 * @param pI			TODO: describtion
 * @param pBlockSizeX		TODO: describtion
 * @param pBlockSizeY		TODO: describtion
 */
void ImageCutter::fillInfoSurface(IND_Image *pImage,
                                  INFO_SURFACE *pI,
                                  int pBlockSizeX,
                                  int pBlockSizeY) {
	//Quality and type of surface, based on image data
    	fillQualityAndType(pImage,pI);

	// Width and height of the image
	int _width  = pImage->getWidth();
	int _height = pImage->getHeight();

	// Block size is equal to the maximun allowed by the card
	int mBlockSize = _maxTextureSize;

	// Width and height of the are to be rendered in
	pI->_widthBlock   = powerOfTwo(_width);
	pI->_heightBlock  = powerOfTwo(_height);

	// If width and height are higher than allowed, the block will be smaller
	if (pI->_widthBlock  > mBlockSize)  pI->_widthBlock  = mBlockSize;
	if (pI->_heightBlock > mBlockSize)  pI->_heightBlock = mBlockSize;

	// If the user has choosen a size block, we change the values
	if (pBlockSizeX != 0) {
		pI->_widthBlock     = pBlockSizeX;
		pI->_heightBlock    = pBlockSizeY;
	}

	// Number of blocks (width x height) in which we will divide the image into
	pI->_blocksX = (int) _width  / pI->_widthBlock;
	pI->_blocksY = (int) _height / pI->_heightBlock;

	// If the remainder is not equal to 0, width and height will not be power of two
	// and so there will be a spare. We add a block more because that.
	if (_width  % pI->_widthBlock)(pI->_blocksX)++;
	if (_height % pI->_heightBlock)(pI->_blocksY)++;

	// Percentage not used
	// a and b are two rectangular areas of not used surface
	// _spareX, _spareY => right spare, up spare
	pI->_spareX = ((pI->_blocksX) * (pI->_widthBlock)) - _width;
	int a = (pI->_spareX) * ((pI->_blocksY) * (pI->_heightBlock));

	pI->_spareY = ((pI->_blocksY) * (pI->_heightBlock)) - _height;
	int b =  _width * (pI->_spareY);

	float mNotUsed = (float) a + b;

	// Surface of the image
	float mTotal = (float)((pI->_blocksX) * (pI->_widthBlock)) * ((pI->_blocksY) * (pI->_heightBlock));

	// Percentage not used
	pI->_notUsedProportion = ((mNotUsed * 100) / mTotal);

	// Number of vertex
	pI->_numVertices = pI->_blocksX * pI->_blocksY * 4;

	// Number of blocks
	pI->_numBlocks = pI->_blocksX * pI->_blocksY;

	// Width / height
	pI->_widthImage  = pImage->getWidth();
	pI->_heightImage = pImage->getHeight();

	// Useful area of the texture
	pI->_widthSpareImage  = pI->_widthBlock  - pI->_spareX;
	pI->_heightSpareImage = pI->_heightBlock - pI->_spareY;
}


/**
 * Cuts a block of an image in memory, returns a pointer to the cut block.
 * @param pPtrBlock		TODO: describtion
 * @param pWidthImage		TODO: describtion
 * @param pWidthBlock		TODO: describtion
 * @param pHeightBlock		TODO: describtion
 * @param pSpareX		TODO: describtion
 * @param pSpareY		TODO: describtion
 * @param pBpp			TODO: describtion
 * @param pNewBlock		TODO: describtion
 */
void ImageCutter::cutBlock(BYTE *pPtrBlock,
                           int pWidthImage,
                           int pWidthBlock,
                           int pHeightBlock,
                           int pSpareX,
                           int pSpareY,
                           int pBpp,
                           BYTE **pNewBlock) {
	// ----- Allocate memory for the block  -----

	int mSizeBlock = pWidthBlock * pHeightBlock * pBpp;
	*pNewBlock = new BYTE [mSizeBlock];
	memset(*pNewBlock, 0, mSizeBlock);

	// ----- Copy the block -----

	// Initial position
	BYTE *mInitPosition = *pNewBlock;

	// Cut
	for (int i = 0; i < pHeightBlock - pSpareY; i++) {
		// We cut one line
		memcpy(*pNewBlock, pPtrBlock, (pWidthBlock * pBpp) - (pSpareX * pBpp));

		// Following line
		pPtrBlock += pWidthImage * pBpp;
		*pNewBlock += pWidthBlock * pBpp;
	}

	// Return the initial position
	*pNewBlock = mInitPosition;
}


// --------------------------------------------------------------------------------
//							        Private methods
// --------------------------------------------------------------------------------

/**
 * Power of two imediatly higher or equal to the received parameter.
 * @param pNum			the number that we whant the power of two of.
 */
int ImageCutter::powerOfTwo(int pNum) {
	int mPower;
	for (mPower = 1; mPower < pNum; mPower *= 2);
	return mPower;
}

/**
 * Analyze image to get the quality and type attributes of target surface.
 * @param pImage		the IND_Image that is going to be examined.
 * @param pI			the INFO_SURFACE which attributes is going to be set.
 */
void ImageCutter::fillQualityAndType(IND_Image* pImage, INFO_SURFACE *pI) {
	int colorFormat = pImage->getFormatInt();
	int bpp = pImage->getBpp();
	
	//Default values
	pI->_type = IND_OPAQUE;
	pI->_quality = IND_32;

	if (IND_RGBA == colorFormat) {
		pI->_type = IND_ALPHA;
		pI->_quality = bpp;
	} else {
		if (IND_RGB == colorFormat) {
			pI->_type = IND_OPAQUE;
			pI->_quality = bpp;
		}

		if (IND_LUMINANCE == colorFormat) {
			pI->_type = IND_OPAQUE;
			if (16 == bpp) {
				pI->_quality = IND_GREY_16;
			} else {
				pI->_quality = bpp;
			}
		}
	}
}

/**
 * Init variables.
 */
void ImageCutter::initVars() {

}


/**
 * Free variables.
 */
void ImageCutter::freeVars() {

}
