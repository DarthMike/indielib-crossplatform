/*****************************************************************************************
 * File: OpenGLTextureBuilder.cpp
 * Desc: Builder-specific class for OpenGL textures
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

#ifdef INDIERENDER_OPENGL

#include "dependencies/FreeImage/Dist/FreeImage.h"

#include "Global.h"
#include "OpenGLTextureBuilder.h"
#include "IND_Surface.h"
#include "TextureDefinitions.h"
#include "IND_Image.h"
#include "ImageCutter.h"
/** @cond DOCUMENT_INTERNALRENDERCODE */

OpenGLTextureBuilder::OpenGLTextureBuilder(IND_ImageManager *imagemgr, IND_Render *render):
	_render(render) {
	// Image cutter
	_cutter = new ImageCutter();
	_cutter->init(imagemgr, _render->getMaxTextureSize());
}

OpenGLTextureBuilder::~OpenGLTextureBuilder() {
	// Free cutter object
	DISPOSE(_cutter);
}

/*
==================
Texture (IND_Surface) creation
==================
*/


bool OpenGLTextureBuilder::createNewTexture(IND_Surface  *pNewSurface,
        IND_Image       *pImage,
        int             pBlockSizeX,
        int             pBlockSizeY) {
    
#ifdef _DEBUG
    GLboolean enabled;
    glGetBooleanv(GL_TEXTURE_2D,&enabled);
    if (GL_FALSE == enabled){
        g_debug->header("GL Textures not enabled!!", 2);
        return false;
    }
#endif	
    
    // ----- Cutting blocks -----
    INFO_SURFACE mI;
	_cutter->fillInfoSurface(pImage, &mI, pBlockSizeX, pBlockSizeY);
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

	// Allocate space for the vertex buffer
	// This buffer will be used for drawing the IND_Surface using DrawPrimitiveUp
	pNewSurface->_surface->_vertexArray = new CUSTOMVERTEX2D [mI._numVertices];

	// Each block, needs a texture. We use an array of textures in order to store them.
	pNewSurface->_surface->_texturesArray = new TEXTURE [mI._numBlocks];
    glGenTextures(mI._numBlocks,pNewSurface->_surface->_texturesArray);
    
    GLenum glerror = glGetError();
    if (glerror) {
		g_debug->header("OpenGL error while creating textures ", 2);
        //TODO: Test error and mem. leaks 
        return false;
    }
	
	GLint mInternalFormat, mFormat, mType;
	//Get format and type of surface (image) in GL types (and check for compatibilities)
    getGLFormat(pNewSurface,pImage,&mInternalFormat,&mFormat,&mType);
	  
    // ----- Vertex creation -----
    
	// Current position of the vertex
	int mPosX = 0;
	int mPosY = mI._heightImage;
	int mPosZ = 0;

	// Position in wich we are storing a vertex
	int mPosVer = 0;

	// Position in wich we are storing a texture
	int mCont = 0;

	// Image pointer
	BYTE *mPtrBlock = pImage->getPointer();
	
    // Vars
	int mActualWidthBlockX (0);
	int mActualHeightBlockY (0);
	float mActualU (0);
	float mActualV (0);
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
			BYTE *mTempBlock = 0;
			_cutter->cutBlock(mPtrBlock,
			                  mI._widthImage,
			                  mI._widthBlock,
			                  mI._heightBlock,
			                  mActualSpareX,
			                  mActualSpareY,
			                  mSrcBytespp,
			                  &mTempBlock);

			// We create a texture using the cut bitmap block
			glBindTexture(GL_TEXTURE_2D,pNewSurface->_surface->_texturesArray[mCont]);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D,
						0,
						mInternalFormat,
			        mI._widthBlock,
			        mI._heightBlock,
						0,
						mFormat,
						mType,
						mTempBlock);

			// Free the bitmap cutted block
			DISPOSEARRAY(mTempBlock);

			GLenum glerror = glGetError();
			if (glerror) {
				g_debug->header("OpenGL error while assigning texture to buffer", 2);
				//TODO: Test error and mem. leaks 
				return false;
			}

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
	} //LOOP END - All blocks (Y coords)

	return true;
}

// --------------------------------------------------------------------------------
//									Private methods
// --------------------------------------------------------------------------------

/*
==================
Return OpenGL format and type depending on IndieLib defined quality and type
This methot converts source image as needed
==================
*/
void OpenGLTextureBuilder::getGLFormat(IND_Surface *pNewSurface, IND_Image* pImage,GLint *pGLInternalFormat, GLint *pGLFormat, GLint *pGLType) {
    
    assert(pNewSurface);
    assert(pImage);
    assert(pGLFormat);
    assert(pGLType);

    if (!pGLFormat || !pGLType || !pNewSurface || !pImage)
        return;

    //TODO: Review image loading to support luminance+alpha. Now only supports Luminance without alpha
	int colorFormat = pImage->getFormatInt();
	int bpp = pImage->getBpp();
	//Check image color format
	switch (colorFormat) {
		case IND_LUMINANCE:
			//TODO: Other bpp
			*pGLInternalFormat = GL_LUMINANCE;
			*pGLFormat = GL_LUMINANCE;
			if (8 == bpp) {
				//No alpha channel by default
            pNewSurface->_surface->_attributes._type = IND_OPAQUE;
				//GL type
				*pGLType = GL_UNSIGNED_BYTE;
			} else if (16 == bpp) {
				//No alpha channel by default
				pNewSurface->_surface->_attributes._type = IND_OPAQUE;
            //GL types
				*pGLType = GL_UNSIGNED_SHORT_5_6_5;
			} else {
				*pGLInternalFormat = GL_NONE;
				*pGLFormat = GL_NONE;
				*pGLType = GL_NONE;
				g_debug->header("Not supported format of image! texture not loaded" , 4);
                }

			break;
		case (IND_RGB):
				*pGLInternalFormat = GL_RGB;

			if (16 == bpp) {
				FIBITMAP* fib = pImage->getFreeImageHandle();
                *pGLFormat = GL_RGB;
				//Check if 565 RGB
				if ((FreeImage_GetRedMask(fib) == FI16_565_RED_MASK) 
					&& (FreeImage_GetGreenMask(fib) == FI16_565_GREEN_MASK) 
					&& (FreeImage_GetBlueMask(fib) == FI16_565_BLUE_MASK)) {
                *pGLType = GL_UNSIGNED_SHORT_5_6_5;
				} else {
					*pGLInternalFormat = GL_NONE;
					*pGLFormat = GL_NONE;
					*pGLType = GL_NONE;
					g_debug->header("Not supported format of image! texture not loaded" , 4);
            }
			} else if (32 == bpp) {
				//FreeImage loads channel order differently depending on processor endianess (24 and 32 bits).
#if FI_RGBA_BLUE > FI_RGBA_RED
                *pGLFormat = GL_RGB;  //Generally for MacOSX and Big Endian Linux / Unix
#else
				*pGLFormat = GL_BGR;  //Generally for Windows and Linux (little Endian)
#endif
                *pGLType = GL_UNSIGNED_BYTE;
			} else {
				*pGLInternalFormat = GL_NONE;
				*pGLFormat = GL_NONE;
				*pGLType = GL_NONE;
				g_debug->header("Not supported format of image! texture not loaded" , 4);
                }
			break;
		case (IND_RGBA): 
			if (16 == bpp) {
				*pGLInternalFormat = GL_RGBA;
                *pGLFormat = GL_RGBA; 
                *pGLType = GL_UNSIGNED_SHORT_4_4_4_4;
			} else if (32 == bpp) {
				*pGLInternalFormat = GL_RGBA;
				//FreeImage loads channel order differently depending on processor endianess (24 and 32 bits).
#if FI_RGBA_BLUE > FI_RGBA_RED
                *pGLFormat = GL_RGBA;  //Generally for MacOSX and Big Endian Linux / Unix
#else
				*pGLFormat = GL_BGRA;  //Generally for Windows and Linux (little Endian)
#endif
                *pGLType = GL_UNSIGNED_BYTE;
			} else {
				*pGLInternalFormat = GL_NONE;
				*pGLFormat = GL_NONE;
				*pGLType = GL_NONE;
				g_debug->header("Not supported format of image! texture not loaded" , 4);
            }

            break;
		case (IND_COLOUR_INDEX):
		default:
			*pGLInternalFormat = GL_NONE;
            *pGLFormat = GL_NONE;
			*pGLType = GL_NONE;
            g_debug->header("Not supported format of image! texture not loaded" , 4);
            break;
    }        
}




void OpenGLTextureBuilder::pushVertex(CUSTOMVERTEX2D *pVertices,
                               int pPosVert,
                               int pVx,
                               int pVy,
                               int pVz,
                               float pU,
                               float pV){
	pVertices [pPosVert]._x = static_cast<float>(pVx);
	pVertices [pPosVert]._y = static_cast<float>(pVy);
	pVertices [pPosVert]._z = static_cast<float>(pVz);
	pVertices [pPosVert]._u = pU;
	pVertices [pPosVert]._v = pV;
}

void OpenGLTextureBuilder::push4Vertices(CUSTOMVERTEX2D *pVertices,
                                  int pPosVert,
                                  int pPosX,
                                  int pPosY,
                                  int pPosZ,
                                  int pWidthBlock,
                                  int pHeightBlock,
                                  float pU,
                                  float pV){
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
#endif //INDIERENDER_OPENGL
