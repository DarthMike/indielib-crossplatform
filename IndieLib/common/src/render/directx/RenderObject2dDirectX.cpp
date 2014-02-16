/*****************************************************************************************
 * File: RenderObject2dDirectX.cpp
 * Desc: Blitting of 2d objects using D3D
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


#include "Defines.h"

#ifdef INDIERENDER_DIRECTX
// ----- Includes -----

/** @cond DOCUMENT_PRIVATEAPI */

#include "Global.h"
#include "IND_SurfaceManager.h"
#include "IND_Surface.h"
#include "TextureDefinitions.h"
#include "IND_Animation.h"
#include "DirectXRender.h"


// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

void DirectXRender::blitSurface(IND_Surface *pSu) {
	// ----- Blitting -----
	int mCont = 0;
	for (int i = 0; i < pSu->getNumBlocks(); i++) {
		// ----- Transform 4 vertices of the quad into world space coordinates -----

		D3DXVECTOR4 mP1, mP2, mP3, mP4;
        Transform4Vertices(static_cast<float>(pSu->_surface->_vertexArray[mCont]._pos._x), static_cast<float>(pSu->_surface->_vertexArray[mCont]._pos._y),
		                   static_cast<float>(pSu->_surface->_vertexArray[mCont + 1]._pos._x), static_cast<float>(pSu->_surface->_vertexArray[mCont + 1]._pos._y),
		                   static_cast<float>(pSu->_surface->_vertexArray[mCont + 2]._pos._x), static_cast<float>(pSu->_surface->_vertexArray[mCont + 2]._pos._y),
		                   static_cast<float>(pSu->_surface->_vertexArray[mCont + 3]._pos._x), static_cast<float>(pSu->_surface->_vertexArray[mCont + 3]._pos._y),
		                   &mP1, &mP2, &mP3, &mP4);

		IND_Vector3 mP1_f3(mP1.x,mP1.y,mP1.z);
		IND_Vector3 mP2_f3(mP2.x,mP2.y,mP2.z);
		IND_Vector3 mP3_f3(mP3.x,mP3.y,mP3.z);
		IND_Vector3 mP4_f3(mP4.x,mP4.y,mP4.z);

		// Calculate the bounding rectangle that we are going to try to discard
		_math->calculateBoundingRectangle(&mP1_f3, &mP2_f3, &mP3_f3, &mP4_f3);

		// ---- Discard bounding rectangle using frustum culling if possible ----

		if (_math->cullFrustumBox(mP1_f3, mP2_f3,_frustrumPlanes)) {
			_info._device->SetFVF(D3DFVF_CUSTOMVERTEX2D);

			if (!pSu->isHaveGrid()) {
				//Texture ID - If it doesn't have a grid, every other block must be blit by 
				//a different texture in texture array ID. 
                _info._device->SetTexture(0, pSu->_surface->_texturesArray[i]._texture);
			} else {
				//In a case of rendering a grid. Same texture (but different vertex position)
				//is rendered all the time. In other words, different pieces of same texture are rendered
				_info._device->SetTexture(0, pSu->_surface->_texturesArray[0]._texture);
			}

			_info._device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pSu->_surface->_vertexArray + mCont, sizeof(CUSTOMVERTEX2D));

			_numrenderedObjects++;
		} else
			_numDiscardedObjects++;


		mCont += 4;
	}
}

void DirectXRender::blitGrid(IND_Surface *pSu, unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA) {
	D3DXMATRIX mMatWorld;
	_info._device->GetTransform(D3DTS_WORLD, &mMatWorld);

	for (int i = 0; i < pSu->getNumBlocks() * 4; i += 4) {
		// ----- Transform 4 vertices of the quad into world space coordinates -----

		D3DXVECTOR4 mP1, mP2, mP3, mP4;
		Transform4Vertices(static_cast<float>( pSu->_surface->_vertexArray[i]._pos._x), static_cast<float>( pSu->_surface->_vertexArray[i]._pos._y),
		                   static_cast<float>( pSu->_surface->_vertexArray[i + 1]._pos._x), static_cast<float>( pSu->_surface->_vertexArray[i + 1]._pos._y),
		                   static_cast<float>( pSu->_surface->_vertexArray[i + 2]._pos._x), static_cast<float>( pSu->_surface->_vertexArray[i + 2]._pos._y),
		                   static_cast<float>( pSu->_surface->_vertexArray[i + 3]._pos._x), static_cast<float>( pSu->_surface->_vertexArray[i + 3]._pos._y),
		                   &mP1, &mP2, &mP3, &mP4);

		IND_Vector3 mP1_f3(mP1.x,mP1.y,mP1.z);
		IND_Vector3 mP2_f3(mP2.x,mP2.y,mP2.z);
		IND_Vector3 mP3_f3(mP3.x,mP3.y,mP3.z);
		IND_Vector3 mP4_f3(mP4.x,mP4.y,mP4.z);

		// Calculate the bounding rectangle that we are going to try to discard
		_math->calculateBoundingRectangle(&mP1_f3, &mP2_f3, &mP3_f3, &mP4_f3);

		// ---- Discard bounding rectangle using frustum culling if possible ----
		//FIXME: This discards some grids when they are visible. Run test INDImageTests_nonPOTLoad to see effect on planet image grid.
		if (_math->cullFrustumBox(mP1_f3, mP2_f3,_frustrumPlanes)) {
			BlitGridQuad(static_cast<int>(pSu->_surface->_vertexArray[i]._pos._x), static_cast<int>(pSu->_surface->_vertexArray[i]._pos._y),
			             static_cast<int>(pSu->_surface->_vertexArray[i + 1]._pos._x), static_cast<int>(pSu->_surface->_vertexArray[i + 1]._pos._y),
			             static_cast<int>(pSu->_surface->_vertexArray[i + 2]._pos._x), static_cast<int>(pSu->_surface->_vertexArray[i + 2]._pos._y),
			             static_cast<int>(pSu->_surface->_vertexArray[i + 3]._pos._x), static_cast<int>(pSu->_surface->_vertexArray[i + 3]._pos._y),
			             pR, pG, pB, pA,
			             mMatWorld);
		}
	}
}

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

			IND_Vector3 mP1_f3(mP1.x,mP1.y,mP1.z);
			IND_Vector3 mP2_f3(mP2.x,mP2.y,mP2.z);
			IND_Vector3 mP3_f3(mP3.x,mP3.y,mP3.z);
			IND_Vector3 mP4_f3(mP4.x,mP4.y,mP4.z);

			// Calculate the bounding rectangle that we are going to try to discard
			_math->calculateBoundingRectangle(&mP1_f3, &mP2_f3, &mP3_f3, &mP4_f3);

			// ---- Discard bounding rectangle using frustum culling if possible ----

			if (!_math->cullFrustumBox(mP1_f3, mP2_f3,_frustrumPlanes)) {
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
			_info._device->SetFVF(D3DFVF_CUSTOMVERTEX2D);
			_info._device->SetTexture(0, pSu->_surface->_texturesArray [0]._texture);
			_info._device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &_vertices2d, sizeof(CUSTOMVERTEX2D));
		}
	}
}

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

		IND_Vector3 mP1_f3(mP1.x,mP1.y,mP1.z);
		IND_Vector3 mP2_f3(mP2.x,mP2.y,mP2.z);
		IND_Vector3 mP3_f3(mP3.x,mP3.y,mP3.z);
		IND_Vector3 mP4_f3(mP4.x,mP4.y,mP4.z);

		// Calculate the bounding rectangle that we are going to try to discard
		_math->calculateBoundingRectangle(&mP1_f3, &mP2_f3, &mP3_f3, &mP4_f3);

		// ---- Discard bounding rectangle using frustum culling if possible ----

		if (!_math->cullFrustumBox(mP1_f3, mP2_f3,_frustrumPlanes)) {
			_numDiscardedObjects++;
			return 1;
		}

		_numrenderedObjects++;

		// Prepare the quad that is going to be blitted
		// Calculates the position and mapping coords for that block
		float _u = static_cast<float>(pWidth)  / static_cast<float>(pSu->getWidthBlock());
		float _v = static_cast<float>(pHeight) / static_cast<float>(pSu->getHeightBlock());

		//Upper-right
		fillVertex2d(&_vertices2d [0], static_cast<float>(pWidth), 0, _u - pUDisplace, pVDisplace);
		//Lower-right
		fillVertex2d(&_vertices2d [1], static_cast<float>(pWidth), static_cast<float>(pHeight), _u - pUDisplace,-_v + pVDisplace);
		//Upper-left
		fillVertex2d(&_vertices2d [2], 0.0f,0.0f,-pUDisplace,pVDisplace);
		//Lower-left
		fillVertex2d(&_vertices2d [3], 0.0f,static_cast<float>(pHeight),-pUDisplace,-_v + pVDisplace);

		// Quad blitting
		_info._device->SetFVF(D3DFVF_CUSTOMVERTEX2D);
		_info._device->SetTexture(0, pSu->_surface->_texturesArray [0]._texture);


		_info._device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		_info._device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		_info._device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &_vertices2d, sizeof(CUSTOMVERTEX2D));

		_info._device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		_info._device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	}
	return correctParams;
}

int DirectXRender::blitAnimation(IND_Animation *pAn, unsigned int pSequence,
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
		_info._device->GetTransform(D3DTS_WORLD, &mMatWorld);

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
		_info._device->SetTransform(D3DTS_WORLD, &mMatWorld);

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
	pVertex2d->_pos._x       = pX;
	pVertex2d->_pos._y= pY;
	pVertex2d->_pos._z       = 0.0f;
	pVertex2d->_texCoord._u = pU;
	pVertex2d->_texCoord._v       = pV;
}

/** @endcond */

#endif //INDIERENDER_DIRECTX




