/*****************************************************************************************
 * File: ImageCutter.h
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

#ifndef _IND_IMAGECUTTER_H_
#define _IND_IMAGECUTTER_H_

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

	void cutBlock(BYTE *pPtrBlock,
	              int pWidthImage,
	              int pWidthBlock,
	              int pHeightBlock,
	              int pSpareX,
	              int pSpareY,
	              int pBpp,
	              BYTE **pNewBlock);


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
