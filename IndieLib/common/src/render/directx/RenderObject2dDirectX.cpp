/*****************************************************************************************
 * File: RenderObject2dDirectX.cpp
 * Desc: Blitting of 2d objects using D3D
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
#include "Defines.h"

#ifdef INDIERENDER_DIRECTX
// ----- Includes -----

#include "Global.h"
#include "IND_SurfaceManager.h"
#include "IND_Surface.h"
#include "TextureDefinitions.h"
#include "IND_Animation.h"
#include "DirectXRender.h"


// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

/*!
\defgroup Graphical_Objects Bliting Surfaces, Animations and Fonts and setting the transformations directly
\ingroup Advances
*/
/*@{*/

/*!
\b Parameters:

\arg \b pSu                     Pointer to a ::IND_Surface object

\b Operation:

This function blits directly to the screen a ::IND_Surface object.

In order to change the transformations
and color attributes of the surface you have to use the DirectXRender::setTransform2d() and DirectXRender::setRainbow2d() methods before
calling this function. Remember that you can use IND_Entity2d object for drawing surfaces to the screen without having to use these
advanced methods directly. This method is only useful for advanced users with really concrete purposes.

Using this method is equivalent to using:
- IND_Entity2d::setSurface()
*/
void DirectXRender::blitSurface(IND_Surface *pSu) {
	// ----- Blitting -----
	int mCont = 0;
	for (int i = 0; i < pSu->getNumBlocks(); i++) {
		// ----- Transform 4 vertices of the quad into world space coordinates -----

		D3DXVECTOR4 mP1, mP2, mP3, mP4;
        Transform4Vertices(static_cast<float>(pSu->_surface->_vertexArray[mCont]._x), static_cast<float>(pSu->_surface->_vertexArray[mCont]._y),
		                   static_cast<float>(pSu->_surface->_vertexArray[mCont + 1]._x), static_cast<float>(pSu->_surface->_vertexArray[mCont + 1]._y),
		                   static_cast<float>(pSu->_surface->_vertexArray[mCont + 2]._x), static_cast<float>(pSu->_surface->_vertexArray[mCont + 2]._y),
		                   static_cast<float>(pSu->_surface->_vertexArray[mCont + 3]._x), static_cast<float>(pSu->_surface->_vertexArray[mCont + 3]._y),
		                   &mP1, &mP2, &mP3, &mP4);

		D3DXVECTOR3 mP1_f3(mP1);
		D3DXVECTOR3 mP2_f3(mP2);
		D3DXVECTOR3 mP3_f3(mP3);
		D3DXVECTOR3 mP4_f3(mP4);

		// Calculate the bounding rectangle that we are going to try to discard
		CalculateBoundingRectangle(&mP1_f3, &mP2_f3, &mP3_f3, &mP4_f3);

		// ---- Discard bounding rectangle using frustum culling if possible ----

		if (CullFrustumBox(mP1_f3, mP2_f3)) {
			_info.mDevice->SetFVF(D3DFVF_CUSTOMVERTEX2D);

			if (!pSu->isHaveGrid()) {
				//Texture ID - If it doesn't have a grid, every other block must be blit by 
				//a different texture in texture array ID. 
                _info.mDevice->SetTexture(0, pSu->_surface->_texturesArray[i]._texture);
			} else {
				//In a case of rendering a grid. Same texture (but different vertex position)
				//is rendered all the time. In other words, different pieces of same texture are rendered
				_info.mDevice->SetTexture(0, pSu->_surface->_texturesArray[0]._texture);
			}

			_info.mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pSu->_surface->_vertexArray + mCont, sizeof(CUSTOMVERTEX2D));

			_numrenderedObjects++;
		} else
			_numDiscardedObjects++;


		mCont += 4;
	}
}


/*!
\b Parameters:

\arg \b pSu                     Pointer to a ::IND_Surface object

\b Operation:

This function blits directly to the screen the grid of an ::IND_Surface object.

Using this method is equivalent to using both of these methods:
- IND_Entity2dManager::renderGridAreas()
- IND_Entity2d::showGridAreas()
*/
void DirectXRender::blitGrid(IND_Surface *pSu, BYTE pR, BYTE pG, BYTE pB, BYTE pA) {
	D3DXMATRIX mMatWorld;
	_info.mDevice->GetTransform(D3DTS_WORLD, &mMatWorld);

	for (int i = 0; i < pSu->getNumBlocks() * 4; i += 4) {
		// ----- Transform 4 vertices of the quad into world space coordinates -----

		D3DXVECTOR4 mP1, mP2, mP3, mP4;
		Transform4Vertices(static_cast<float>( pSu->_surface->_vertexArray[i]._x), static_cast<float>( pSu->_surface->_vertexArray[i]._y),
		                   static_cast<float>( pSu->_surface->_vertexArray[i + 1]._x), static_cast<float>( pSu->_surface->_vertexArray[i + 1]._y),
		                   static_cast<float>( pSu->_surface->_vertexArray[i + 2]._x), static_cast<float>( pSu->_surface->_vertexArray[i + 2]._y),
		                   static_cast<float>( pSu->_surface->_vertexArray[i + 3]._x), static_cast<float>( pSu->_surface->_vertexArray[i + 3]._y),
		                   &mP1, &mP2, &mP3, &mP4);

		D3DXVECTOR3 mP1_f3(mP1);
		D3DXVECTOR3 mP2_f3(mP2);
		D3DXVECTOR3 mP3_f3(mP3);
		D3DXVECTOR3 mP4_f3(mP4);

		// Calculate the bounding rectangle that we are going to try to discard
		CalculateBoundingRectangle(&mP1_f3, &mP2_f3, &mP3_f3, &mP4_f3);

		// ---- Discard bounding rectangle using frustum culling if possible ----
		//FIXME: This discards some grids when they are visible. Run test INDImageTests_nonPOTLoad to see effect on planet image grid.
		if (CullFrustumBox(mP1_f3, mP2_f3)) {
			BlitGridQuad(static_cast<int>(pSu->_surface->_vertexArray[i]._x), static_cast<int>(pSu->_surface->_vertexArray[i]._y),
			             static_cast<int>(pSu->_surface->_vertexArray[i + 1]._x), static_cast<int>(pSu->_surface->_vertexArray[i + 1]._y),
			             static_cast<int>(pSu->_surface->_vertexArray[i + 2]._x), static_cast<int>(pSu->_surface->_vertexArray[i + 2]._y),
			             static_cast<int>(pSu->_surface->_vertexArray[i + 3]._x), static_cast<int>(pSu->_surface->_vertexArray[i + 3]._y),
			             pR, pG, pB, pA,
			             mMatWorld);
		}
	}
}


/*!
\b Parameters:

\arg \b pSu                     Pointer to a ::IND_Surface object
\arg \b pX, \b pY               Upper left coordinate of the region
\arg \b pWidth, \b pHeight      Width and Height of the region

\b Operation:

This method is useful when we want to render only a certain region of a ::IND_Surface.

If the region that we chose is out of the range of the sprite, the function will return false and no
region will be rendered.

Special remark: this function only works with ::IND_Surface objects that only have ONE texture
assigned (you can check this using::IND_Surface::getNumTextures() method). So, it will work only
with images that are power of two and lower than the maximum texture size allowed by your card
(you can check this parameter using ::DirectXRender::getMaxTextureSize()). The method will return 0
otherwise.

In order to change the transformations and color attributes of the surface you have to use the DirectXRender::setTransform2d() and DirectXRender::setRainbow2d() methods before
calling to this function. Remember that you can use IND_Entity2d object for drawing surfaces to the screen without having to use this
advanced methods directly. This method is only useful for advanced users with really concrete purposes.

Using this method is equivalent to using both of these methods:
- IND_Entity2d::setSurface()
- IND_Entity2d::setRegion()
*/
void DirectXRender::blitRegionSurface(IND_Surface *pSu,
                                      int pX,
                                      int pY,
                                      int pWidth,
                                      int pHeight) {
	// If the region is the same as the image area, we blit normally
	// If the region is the same as the image area, we blit normally
	if (!pX && !pY && (pWidth == pSu->getWidth()) && (pHeight == pSu->getHeight())) {
		blitSurface(pSu);
	} else {
		bool correctParams = true;
		if (pSu->getNumTextures() > 1 || 
			pX < 0 || pX + pWidth > pSu->getWidth()
			||
			pY < 0 || pY + pHeight > pSu->getHeight()) {
			correctParams = false;
		}
		
		if (correctParams) {
			// ----- Transform 4 vertices of the quad into world space coordinates -----

			D3DXVECTOR4 mP1, mP2, mP3, mP4;
			Transform4Vertices(static_cast<float>(pWidth), 0.0f,
							   static_cast<float>(pWidth), static_cast<float>(pHeight),
							   0.0f, 0.0f,
							   0.0f, static_cast<float>(pHeight),
							   &mP1, &mP2, &mP3, &mP4);

			D3DXVECTOR3 mP1_f3(mP1);
			D3DXVECTOR3 mP2_f3(mP2);
			D3DXVECTOR3 mP3_f3(mP3);
			D3DXVECTOR3 mP4_f3(mP4);

			// Calculate the bounding rectangle that we are going to try to discard
			CalculateBoundingRectangle(&mP1_f3, &mP2_f3, &mP3_f3, &mP4_f3);

			// ---- Discard bounding rectangle using frustum culling if possible ----

			if (!CullFrustumBox(mP1_f3, mP2_f3)) {
				_numDiscardedObjects++;
				return;
			}

			_numrenderedObjects++;

			// Prepare the quad that is going to be blitted
			// Calculates the position and mapping coords for that block
			fillVertex2d(&_vertices2d [0], static_cast<float>(pWidth), 0, (static_cast<float>(pX + pWidth) / pSu->getWidthBlock()), (1.0f - (static_cast<float>(pY + pSu->getSpareY()) / pSu->getHeightBlock())));
			fillVertex2d(&_vertices2d [1], static_cast<float>(pWidth), static_cast<float>(pHeight), (static_cast<float>(pX + pWidth) / pSu->getWidthBlock()), (1.0f - (static_cast<float>(pY + pHeight + pSu->getSpareY()) / pSu->getHeightBlock())));
			fillVertex2d(&_vertices2d [2], 0.0f, 0.0f, static_cast<float>(pX) / static_cast<float>(pSu->getWidthBlock()), (1.0f - (static_cast<float>(pY + pSu->getSpareY())  / pSu->getHeightBlock())));
			fillVertex2d(&_vertices2d [3], 0.0f, static_cast<float>(pHeight), (static_cast<float>(pX)/ pSu->getWidthBlock()), (1.0f - (static_cast<float>(pY + pHeight + pSu->getSpareY()) / pSu->getHeightBlock())));

			// Quad blitting
			_info.mDevice->SetFVF(D3DFVF_CUSTOMVERTEX2D);
			_info.mDevice->SetTexture(0, pSu->_surface->_texturesArray [0]._texture);
			_info.mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &_vertices2d, sizeof(CUSTOMVERTEX2D));
		}
	}
}


/*!
\b Parameters:

\arg \b pSu                         Pointer to a ::IND_Surface object
\arg \b pX, \b pY                   Upper left coordinate of the region
\arg \b pWidth, \b pHeight          Width and Height of the region
\arg \b pUDisplace, \b pVDisplace   Horizontal and vertical displacement of the image

\b Operation:

This function returns 1 (true) if it blits directly to the screen a ::IND_Surface object tiling it both
in X and Y coordinates.

This method is useful when we want to render a tiled texture or background.

Special remark: this function only works with ::IND_Surface objects that only have ONE texture
assigned (you can check this using::IND_Surface::getNumTextures() method). So, it will work only
with images that are power of two and lower than the maximum texture size allowed by your card
(you can check this parameter using ::DirectXRender::getMaxTextureSize()). The method will return 0
otherwise.

In order to change the transformations and color attributes of the surface you have to use the DirectXRender::setTransform2d() and DirectXRender::setRainbow2d() methods before
calling to this function. Remember that you can use IND_Entity2d object for drawing surfaces to the screen without having to use this
advanced methods directly. This method is only useful for advanced users with really concrete purposes.

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setSurface()
- IND_Entity2d::setRegion()
- IND_Entity2d::toggleWrap()
- IND_Entity2d::setWrapDisplacement()
*/
bool DirectXRender::blitWrapSurface(IND_Surface *pSu,
                                    int pWidth,
                                    int pHeight,
                                    float pUDisplace,
                                    float pVDisplace) {
	bool correctParams = true;
	if (pSu->getNumTextures() != 1) {
		correctParams = false; 
	}

	if (correctParams) {
		// ----- Transform 4 vertices of the quad into world space coordinates -----

		D3DXVECTOR4 mP1, mP2, mP3, mP4;
		Transform4Vertices(static_cast<float>(pWidth), 0.0f,
						   static_cast<float>(pWidth), static_cast<float>(pHeight),
						   0.0f, 0.0f,
						   0.0f, static_cast<float>(pHeight),
						   &mP1, &mP2, &mP3, &mP4);

		D3DXVECTOR3 mP1_f3(mP1);
		D3DXVECTOR3 mP2_f3(mP2);
		D3DXVECTOR3 mP3_f3(mP3);
		D3DXVECTOR3 mP4_f3(mP4);

		// Calculate the bounding rectangle that we are going to try to discard
		CalculateBoundingRectangle(&mP1_f3, &mP2_f3, &mP3_f3, &mP4_f3);

		// ---- Discard bounding rectangle using frustum culling if possible ----

		if (!CullFrustumBox(mP1_f3, mP2_f3)) {
			_numDiscardedObjects++;
			return 1;
		}

		_numrenderedObjects++;

		// Prepare the quad that is going to be blitted
		// Calculates the position and mapping coords for that block
		float _u = static_cast<float>(pWidth  / pSu->getWidthBlock());
		float _v = static_cast<float>(pHeight / pSu->getHeightBlock());

		//Upper-right
		fillVertex2d(&_vertices2d [0], static_cast<float>(pWidth), 0,           _u - pUDisplace,   pVDisplace);
		//Lower-right
		fillVertex2d(&_vertices2d [1], static_cast<float>(pWidth), static_cast<float>(pHeight),     _u - pUDisplace,   -_v + pVDisplace);
		//Upper-left
		fillVertex2d(&_vertices2d [2], 0.0f,      0.0f,           -pUDisplace,       pVDisplace);
		//Lower-left
		fillVertex2d(&_vertices2d [3], 0.0f,      static_cast<float>(pHeight),     -pUDisplace,       -_v + pVDisplace);

		// Quad blitting
		_info.mDevice->SetFVF(D3DFVF_CUSTOMVERTEX2D);
		_info.mDevice->SetTexture(0, pSu->_surface->_texturesArray [0]._texture);


		_info.mDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		_info.mDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		_info.mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &_vertices2d, sizeof(CUSTOMVERTEX2D));

		_info.mDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		_info.mDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	}
	return correctParams;
}


/*!
\b Parameters:

\arg \b pAn                         Pointer to a ::IND_Animation object
\arg \b pSequence                   Number of the sequence to blit (the first sequence is 0)
\arg \b pX, \b pY                   Upper left coordinate of the region
\arg \b pWidth, \b pHeight          Width and Height of the region
\arg \b pToggleWrap                 Wraping on (1) / off (0)
\arg \b pUDisplace, \b pVDisplace   Horizontal and vertical displacement of the image

\b Operation:

This function blits directly to the screen a certain sequence of a ::IND_Animation object.

Each frame of the animation will be blited to the screen the number of milliseconds that are
defined in the animation script file. The sequecen starts in the frame 0 and finishes in the last frame
specefied in the animation script. The animation will be displayed only one time, after that
it will stop in the last frame (bliting it permanently).

This functions returns -1 when the animation finishes, 0 if there is any error (for example trying to
blit an invalid IND_Animation pointer) and 1 if is in the middle of the animation and there are no errors.

In order to change the transformations
and color attributes of the animation you have to use the DirectXRender::setTransform2d() and DirectXRender::setRainbow2d() methods before
calling to this function. Remember that you can use IND_Entity2d object for drawing animations to the screen without having to use this
advanced methods directly. This method is only useful for advanced users with really concrete purposes.

Special remark: if you specify a region this function only works with ::IND_Surface objects that only have ONE texture
assigned (you can check this using::IND_Surface::getNumTextures() method). So, it will work only
with images that are power of two and lower than the maximum texture size allowed by your card
(you can check this parameter using ::DirectXRender::getMaxTextureSize()). The method will return 0
otherwise.

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setAnimation()
- IND_Entity2d::setRegion()
- IND_Entity2d::toggleWrap()
- IND_Entity2d::setWrapDisplacement()
*/
int DirectXRender::blitAnimation(IND_Animation *pAn, int pSequence,
                                 int pX, int pY,
                                 int pWidth, int pHeight,
                                 bool pToggleWrap,
                                 float pUDisplace,
                                 float pVDisplace) {
	bool correctParams = true;
	int mFinish = 1;
	if (pSequence < 0 || pSequence > pAn->getNumSequences() - 1)  {
		correctParams = false;
	}

	if (correctParams) {
		if (!pAn->getIsActive(pSequence)) {
			pAn->getSequenceTimer(pSequence)->start();
			pAn->setIsActive(pSequence, 1);
		}

		// Current world matrix
		D3DXMATRIX mMatWorld, mTrans;
		_info.mDevice->GetTransform(D3DTS_WORLD, &mMatWorld);

		// If the time of a frame have passed, go to the next frame
		if (pAn->getSequenceTimer(pSequence)->getTicks() > (unsigned long) pAn->getActualFrameTime(pSequence)) {
			// Put timer to zero
			pAn->getSequenceTimer(pSequence)->start();

			// Point to the next frame increasing the counter
			int i = pAn->getActualFramePos(pSequence);
			pAn->setActualFramePos(pSequence, i + 1);

			// If the counter is higher than the number of frames of the sequence, we put it to zero
			if (pAn->getActualFramePos(pSequence) > pAn->getNumFrames(pSequence) - 1) {
				pAn->setActualFramePos(pSequence, pAn->getNumFrames(pSequence) - 1);
				pAn->setIsActive(pSequence, 0);
				mFinish = -1;
			}
		}

		// ----- OffsetX y OffsetY -----

		D3DXMatrixTranslation(&mTrans,
							  static_cast<float>(pAn->getActualOffsetX(pSequence)),
							  static_cast<float>(pAn->getActualOffsetY(pSequence)),
							  0);
		D3DXMatrixMultiply(&mMatWorld, &mMatWorld, &mTrans);
		_info.mDevice->SetTransform(D3DTS_WORLD, &mMatWorld);

		// ----- Blitting -----

		// Blits all the IND_Surface (all the blocks)
		if (!pX && !pY && !pWidth && !pHeight) {
			blitSurface(pAn->getActualSurface(pSequence));
		} else {
			if (!pToggleWrap) { // Blits a region of the IND_Surface
				if (pAn->getActualSurface(pSequence)->getNumTextures() > 1)
					return 0;
				blitRegionSurface(pAn->getActualSurface(pSequence), pX, pY, pWidth, pHeight);
			} else {// Blits a wrapping IND_Surface
				if (pAn->getActualSurface(pSequence)->getNumTextures() > 1)
					return 0;
				blitWrapSurface(pAn->getActualSurface(pSequence), pWidth, pHeight, pUDisplace, pVDisplace);
			}
		}
	}
	return mFinish;
}
/*@}*/


// --------------------------------------------------------------------------------
//							       Private methods
// --------------------------------------------------------------------------------


/*
==================
Fills a CUSTOMVERTEX2D structure
==================
*/
void DirectXRender::fillVertex2d(CUSTOMVERTEX2D *pVertex2d,
                                 float pX,
                                 float pY,
                                 float pU,
                                 float pV) {
	// Vertex
	pVertex2d->_x       = pX;
	pVertex2d->_y       = pY;
	pVertex2d->_z       = 0.0f;
	pVertex2d->_u       = pU;
	pVertex2d->_v       = pV;
}

#endif //INDIERENDER_DIRECTX




