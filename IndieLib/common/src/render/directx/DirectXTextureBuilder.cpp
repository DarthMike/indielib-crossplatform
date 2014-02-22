/*****************************************************************************************
 * File: DirectXTextureBuilder.cpp
 * Desc: Builder-specific class for DirectX textures
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
THIS FILE IS AN ADDITIONAL FILE ADDED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com), BUT HAS THE
SAME LICENSE AS THE WHOLE LIBRARY TO RESPECT ORIGINAL AUTHOR OF LIBRARY

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
#include "Defines.h"

#ifdef INDIERENDER_DIRECTX

#include "dependencies/FreeImage/Dist/FreeImage.h"

#include "Global.h"
#include "DirectXTextureBuilder.h"
#include "TextureDefinitions.h"
#include "IND_Surface.h"
#include "IND_Image.h"
#include "ImageCutter.h"
#include "DirectXRender.h"
#include "IND_Render.h"

/** @cond DOCUMENT_PRIVATEAPI */

DirectXTextureBuilder::DirectXTextureBuilder(IND_ImageManager *imagemgr, IND_Render *render):
	_render(render) {
	// Image cutter
	_cutter = new ImageCutter;
	_cutter->init(imagemgr, _render->getMaxTextureSize());
}

DirectXTextureBuilder::~DirectXTextureBuilder() {
	// Free cutter object
	DISPOSE(_cutter);
}

/*
==================
Texture (IND_Surface) creation
==================
*/


bool DirectXTextureBuilder::createNewTexture(IND_Surface *pNewSurface,
        IND_Image       *pImage,
        int             pBlockSizeX,
        int             pBlockSizeY) {

	//pType and pQuality are the requested texture parameters, not the actual image type.

	bool success = false;

	// ----- Check IND_Type and IND_Quality of the image and choose a D3D source and destination format -----
	// ----- Source and destination DirectX format ------
	D3DFORMAT mSrcFormat, mDstFormat;
		
	getSourceAndDestinationFormat(pImage,&mSrcFormat,&mDstFormat);  
	
	// ----- Cutting blocks -----

	// ----- Obtaining info in order to store the image -----

	INFO_SURFACE mI;
	_cutter->fillInfoSurface(pImage, &mI, pBlockSizeX, pBlockSizeY);

	// Fill attributes
	pNewSurface->freeTextureData(); //Guard against using same texture data all over again in same surface
    pNewSurface->_surface = new SURFACE(mI._numBlocks,mI._numVertices);
	pNewSurface->_surface->_attributes._type			 = mI._type;
	pNewSurface->_surface->_attributes._quality			 = mI._quality;
	pNewSurface->_surface->_attributes._blocksX          = mI._blocksX;
	pNewSurface->_surface->_attributes._blocksY          = mI._blocksY;
	pNewSurface->_surface->_attributes._spareX           = mI._spareX;
	pNewSurface->_surface->_attributes._spareY           = mI._spareY;
	pNewSurface->_surface->_attributes._numBlocks        = mI._blocksX * mI._blocksY;
	pNewSurface->_surface->_attributes._numTextures      = mI._blocksX * mI._blocksY;
	pNewSurface->_surface->_attributes._isHaveGrid       = 0;
	pNewSurface->_surface->_attributes._widthBlock       = mI._widthBlock;
	pNewSurface->_surface->_attributes._heightBlock      = mI._heightBlock;
	pNewSurface->_surface->_attributes._width            = mI._widthImage;
	pNewSurface->_surface->_attributes._height           = mI._heightImage;
	pNewSurface->_surface->_attributes._isHaveSurface    = 1;

	// Current position of the vertex
	int mPosX = 0;
	int mPosY = mI._heightImage;
	int mPosZ = 0;

	// Position in wich we are storing a vertex
	int mPosVer = 0;

	// Position in wich we are storing a texture
	int mCont = 0;

	// Image pointer
	unsigned char *mPtrBlock = pImage->getPointer();

	// Vars
	int mActualWidthBlockX (0);
	int mActualHeightBlockY (0);
	float mActualU (0.0f);
	float mActualV (0.0f);
	int mActualSpareX (0);
	int mActualSpareY (0);
	int mSrcBytespp = pImage->getBytespp();

	// ----- Cutting blocks -----

	// We iterate the blocks starting from the lower row
	// We MUST draw the blocks in this order, because the image starts drawing from the lower-left corner
	//LOOP - All blocks (Y coords)
	for (int i = mI._blocksY; i > 0; i--) {
		//LOOP - All blocks (X coords)
		for (int j = 1; j < mI._blocksX + 1; j++) {
			// ----- Vertices position of the block -----

			// There are 4 types of blocks: the ones of the right column, the ones of the upper row,
			// the one of the upper-right corner and the rest of blocks.
			// Depending on the block, we store the vertices one way or another.

			// Normal block
			if (i != 1 && j !=  mI._blocksX) {
				mActualWidthBlockX  = mI._widthBlock;
				mActualHeightBlockY = mI._heightBlock;
				mActualU            = 1.0f;
				mActualV            = 1.0f;
				mActualSpareX       = 0;
				mActualSpareY       = 0;
			}

			// The ones of the right column
			if (i != 1 && j ==  mI._blocksX) {
				mActualWidthBlockX  = mI._widthSpareImage;
				mActualHeightBlockY = mI._heightBlock;
				mActualU            = (float) mI._widthSpareImage / mI._widthBlock;
				mActualV            = 1.0f;
				mActualSpareX       = mI._spareX;
				mActualSpareY       = 0;
			}

			// The ones of the upper row
			if (i == 1 && j !=  mI._blocksX) {
				mActualWidthBlockX  = mI._widthBlock;
				mActualHeightBlockY = mI._heightSpareImage;
				mActualU            = 1.0f;
				mActualV            = (float) mI._heightSpareImage / mI._heightBlock;
				mActualSpareX       = 0;
				mActualSpareY       = mI._spareY;
			}

			// The one of the upper-right corner
			if (i == 1 && j ==  mI._blocksX) {
				mActualWidthBlockX  = mI._widthSpareImage;
				mActualHeightBlockY = mI._heightSpareImage;
				mActualU            = (float) mI._widthSpareImage / mI._widthBlock;
				mActualV            = (float) mI._heightSpareImage / mI._heightBlock;
				mActualSpareX       = mI._spareX;
				mActualSpareY       = mI._spareY;
			}

			// ----- Block creation (using the position, uv coordiantes and texture) -----

			// We push into the buffer the 4 vertices of the block
			push4Vertices(pNewSurface->_surface->_vertexArray,           // Pointer to the buffer
			              mPosVer,                                    // Position in wich we are storing a vertex
			              mPosX,                                      // x
			              mPosY,                                      // y
			              mPosZ,                                      // z
			              mActualWidthBlockX,                         // Block width
			              mActualHeightBlockY,                        // Block height
			              mActualU,                                   // U mapping coordinate
			              mActualV);                                  // V mapping coordinate

			// Cuts a block from the image (bitmap)
			unsigned char *mTempBlock = 0;
			_cutter->cutBlock(mPtrBlock,
			                  mI._widthImage,
			                  mI._widthBlock,
			                  mI._heightBlock,
			                  mActualSpareX,
			                  mActualSpareY,
			                  mSrcBytespp,
			                  &mTempBlock);

			// We create a texture using the cut bitmap block
			pNewSurface->_surface->_texturesArray [mCont]._texture = createTexture(mTempBlock,
			        mI._widthBlock,
			        mI._heightBlock,
			        mSrcBytespp,
			        mSrcFormat,
			        mDstFormat);

			// Free the bitmap cutted block
			DISPOSEARRAY(mTempBlock);

			// ----- Advance -----

			// Increase in 4 vertices the position (we have already stored a quad)
			mPosVer += 4;

			// Increase the texture counter (we have alread stored one texture)
			mCont++;

			// ----- Column change -----

			// We point to the next block (memory and screen)
			mPosX += mI._widthBlock;
			mPtrBlock += mI._widthBlock * mSrcBytespp;
		}//LOOP END - All blocks (Y coords)

		// ----- Row change -----

		// We point to the next block  (memory and screen)
		mPosX = 0;
		mPtrBlock -= mI._spareX * mSrcBytespp;

		// If this block is in the last row, we take in count the spare areas.
		if (i == 1) {
			mPosY -= mI._spareY;
			mPtrBlock += (mI._widthImage * mSrcBytespp) * (mI._spareY - 1);
		} else {
			mPosY -= mI._heightBlock;
			mPtrBlock += (mI._widthImage * mSrcBytespp) * (mI._heightBlock - 1);
		}
	}//LOOP END - All blocks (Y coords)

	success = true;

	return success;
}

/*
==================
Creates a texture
==================
*/
IDirect3DTexture9 *DirectXTextureBuilder::createTexture(unsigned char *pImage,
        int pBlockWidth,
        int pBlockHeight,
        int pSrcBpp,
        D3DFORMAT pSrcFormat,
        D3DFORMAT pDstFormat) {
	HRESULT mHr;

	IDirect3DTexture9 *mNewTexture;

	// ----- Texture creation -----

	LPDIRECT3DDEVICE9 d3ddeviceptr = _render->_wrappedRenderer->GetDevice();  //Here we break encapsulation with frienship to access internal ptr!!!
	mHr = D3DXCreateTexture(d3ddeviceptr,
	                        pBlockWidth,
	                        pBlockHeight,
	                        1,
	                        0,
	                        pDstFormat,
	                        D3DPOOL_MANAGED,
	                        &mNewTexture);

	if (FAILED(mHr)) {
		g_debug->header("Error creating the texture", DebugApi::LogHeaderError);
		exit(0);  // TODO: Free objects?
	}

	// ----- D3D Surface creation -----

	IDirect3DSurface9 *_surface;

	// The surfaces points to the texture
	mHr = mNewTexture->GetSurfaceLevel(0, &_surface);
	if (FAILED(mHr)) {
		g_debug->header("Error creating the surface", DebugApi::LogHeaderError);
		exit(0);  // TODO: free objects
	}

	// ----- Add the source image into the surface -----

	// Source image
	RECT mSrcRect;
	mSrcRect.left       = 0;
	mSrcRect.top        = 0;
	mSrcRect.right      = pBlockWidth;
	mSrcRect.bottom     = pBlockHeight;

	mHr = D3DXLoadSurfaceFromMemory(_surface,
	                                0,
	                                0,
	                                pImage,
	                                pSrcFormat,
	                                pBlockWidth * pSrcBpp,
	                                0,
	                                &mSrcRect,
	                                D3DX_FILTER_NONE,
	                                0);

	if (FAILED(mHr)) {
		g_debug->header("Error loading the block to the surface", DebugApi::LogHeaderError);
		exit(0);  // TODO: free objects
	}

	// Free memory
	_surface->Release();

	return mNewTexture;
}


/*
==================
Receiving a IND_Image returns a D3D format for the source and destination. No conversion allowed in-between
==================
*/
void DirectXTextureBuilder::getSourceAndDestinationFormat(IND_Image *pImage, D3DFORMAT* pSrcFormat, D3DFORMAT* pDstFormat) {
	int bpp (pImage->getBpp());
	switch (pImage->getFormatInt()) {
	case IND_RGB: {
		if (16 == bpp) {
			FIBITMAP* fib = pImage->getFreeImageHandle();
			//Check if 565 RGB
			if ((FreeImage_GetRedMask(fib) == FI16_565_RED_MASK) 
				&& (FreeImage_GetGreenMask(fib) == FI16_565_GREEN_MASK) 
				&& (FreeImage_GetBlueMask(fib) == FI16_565_BLUE_MASK)) {
					*pSrcFormat = D3DFMT_R5G6B5;
			} else {
				*pSrcFormat = D3DFMT_UNKNOWN;
				g_debug->header("Not supported format of image! texture not loaded" , DebugApi::LogHeaderWarning);
	}
		} else if (32 == bpp) {
			*pSrcFormat = D3DFMT_A8B8G8R8;
		} else {
			*pSrcFormat = D3DFMT_UNKNOWN;
			g_debug->header("The pixel format for the image could not be resolved correctly!" , DebugApi::LogHeaderWarning);
		}
	}
	break;
	case IND_RGBA: {
		if (16 == bpp) {
			*pSrcFormat =D3DFMT_A4R4G4B4;
		} else if (32 == bpp){
			*pSrcFormat = D3DFMT_A8R8G8B8;
		} else {
			*pSrcFormat = D3DFMT_UNKNOWN;
			g_debug->header("The pixel format for the image could not be resolved correctly!" , DebugApi::LogHeaderWarning);
	}
		
	}
	break;
    //FIXME: IND_LUMINANCE ignored?
	default: {
		*pSrcFormat = D3DFMT_UNKNOWN;
		g_debug->header("The pixel format for the image could not be resolved correctly!" , DebugApi::LogHeaderWarning);
	}
	}

	//No conversions by DirectX
	*pDstFormat = *pSrcFormat;
}

/*
==================
Push a vertex into the buffer
==================
*/
void DirectXTextureBuilder::pushVertex(CUSTOMVERTEX2D *pVertices,
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
void DirectXTextureBuilder::push4Vertices(CUSTOMVERTEX2D *pVertices,
        int pPosVert,
        int pPosX,
        int pPosY,
        int pPosZ,
        int pWidthBlock,
        int pHeightBlock,
        float pU,
        float pV) {
	// Push the 4 vertex of the quad
	// The pushing order is important

	// Upper-right
	pushVertex(pVertices,
	           pPosVert,
	           pPosX + pWidthBlock,
	           pPosY - pHeightBlock,
	           pPosZ,
	           pU,
	           pV);

	// Lower-right
	pushVertex(pVertices,
	           pPosVert + 1,
	           pPosX + pWidthBlock,
	           pPosY,
	           pPosZ,
	           pU,
	           0.0f);

	// Upper-left
	pushVertex(pVertices,
	           pPosVert + 2,
	           pPosX,
	           pPosY - pHeightBlock,
	           pPosZ,
	           0.0f,
	           pV);

	// Lower-left
	pushVertex(pVertices,
	           pPosVert + 3,
	           pPosX,
	           pPosY,
	           pPosZ,
	           0.0f,
	           0.0f);
}

/** @endcond */

#endif //INDIERENDER_DIRECTX
