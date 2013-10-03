/*****************************************************************************************
 * File: ImageCutter.h
 * Desc: This class is used for cutting bitmap areas
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


#ifndef _IND_IMAGECUTTER_H_
#define _IND_IMAGECUTTER_H_

#include "Defines.h"

// ----- Forward declarations -----

class IND_ImageManager;
class IND_Image;

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//									 INFO_SURFACE
// --------------------------------------------------------------------------------

// Information about the surface
struct structInfoSurface {
	int _widthBlock;
	int _heightBlock;
	int _blocksX;
	int _blocksY;
	int _spareX;
	int _spareY;
	float _notUsedProportion;
	int _numVertices;
	int _numBlocks;
	int _widthImage;
	int _heightImage;
	int _widthSpareImage;
	int _heightSpareImage;
	IND_Type    _type;                      // Surface type
    IND_Quality _quality;                   // Color quality
};
typedef struct structInfoSurface INFO_SURFACE;


// --------------------------------------------------------------------------------
//								 ImageCutter
// --------------------------------------------------------------------------------

class ImageCutter {
public:

	// ----- Init/End -----

	ImageCutter(): _ok(false)  { }
	~ImageCutter()          {
		end();
	}

	bool    init(IND_ImageManager *pImageManager, int pMaxTextureSize);
	void    end();
	bool    isOK()           const {
		return _ok;
	}

	// ----- Public methods -----

	void fillInfoSurface(IND_Image *pImage, INFO_SURFACE *pI, int pBlockSizeX, int pBlockSizeY);

	void cutBlock(unsigned char *pPtrBlock,
	              int pWidthImage,
	              int pWidthBlock,
	              int pHeightBlock,
	              int pSpareX,
	              int pSpareY,
	              int pBpp,
	              unsigned char **pNewBlock);


private:

	// ----- Private -----

	bool _ok;
	int _maxTextureSize;

	// ----- Objects -----

	IND_ImageManager *_imageManager;

	// ----- Private methods -----

	int     powerOfTwo(int pNum);
	void	fillQualityAndType(IND_Image* pImage, INFO_SURFACE *pI);
	void    initVars();
	void    freeVars();
};

/** @endcond */

#endif // _IND_IMAGECUTTER_H_
