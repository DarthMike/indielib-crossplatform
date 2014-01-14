/*****************************************************************************************
 * File: RenderObject2dOpenGL.cpp
 * Desc: Blitting of 2d objects using OpenGL
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
#include <stdio.h>

#ifdef INDIERENDER_GLES_IOS
// ----- Includes -----

#include "Global.h"
#include "IND_SurfaceManager.h"
#include "OpenGLES2Render.h"
#include "IND_Surface.h"
#include "IND_Animation.h"
#include "TextureDefinitions.h"
#include "RenderMacros.h"
#include "IND_ShaderProgram.h"
#include "IND_Shaders.h"

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

void OpenGLES2Render::blitSurface(IND_Surface *pSu) {
    _tex2dState.wrapS = GL_CLAMP_TO_EDGE;
    _tex2dState.wrapS = GL_CLAMP_TO_EDGE;
    blitSurfaceImpl(pSu,0,0,0,0);
}

void OpenGLES2Render::blitRegionSurface(IND_Surface *pSu,
                                     int pX,
                                     int pY,
                                     int pWidth,
                                     int pHeight) {

//	//If the region is the same as the image area, we blit normally
//	if (!pX && !pY && (pWidth == pSu->getWidth()) && (pHeight == pSu->getHeight())) {
//		blitSurface(pSu);
//	} else {
//		bool correctParams = true;
//		if (pSu->getNumTextures() > 1 || 
//			pX < 0 || pX + pWidth > pSu->getWidth()
//			||
//			pY < 0 || pY + pHeight > pSu->getHeight()) {
//			correctParams = false;
//		}
//		
//		if (correctParams) {
//			//Only draws first texture block in texture
//			// Prepare the quad that is going to be blitted
//			// Calculates the position and mapping coords for that block
//			float x (static_cast<float>(pX));
//			float y (static_cast<float>(pY));
//			float height (static_cast<float>(pHeight));
//			float width (static_cast<float>(pWidth));
//			float bWidth (static_cast<float>(pSu->getWidthBlock()));
//			float bHeight (static_cast<float>(pSu->getHeightBlock()));
//			float spareY (static_cast<float>(pSu->getSpareY()));
//			fillVertex2d(&_vertices2d [0], width, 0.0f, ((x + width) / bWidth), (1.0f - ((y + spareY) / bHeight)));
//			fillVertex2d(&_vertices2d [1], width, height, (x + width) / bWidth, (1.0f - ((y + height + spareY) / bHeight)));
//			fillVertex2d(&_vertices2d [2], 0.0f, 0.0f , (x/bWidth), (1.0f - ((y+ spareY) / bHeight)));
//			fillVertex2d(&_vertices2d [3], 0.0f, height, (x/bWidth), (1.0f - (y + height + spareY) / bHeight));
//		        
//        	//Get vertex world coords, to perform frustrum culling test in world coords
//            IND_Vector3 mP1, mP2, mP3, mP4;
//            transformVerticesToWorld(_vertices2d[0]._x, _vertices2d[0]._y,
//                                     _vertices2d[1]._x, _vertices2d[1]._y,
//                                     _vertices2d[2]._x, _vertices2d[2]._y,
//                                     _vertices2d[3]._x, _vertices2d[3]._y,
//                                     &mP1, &mP2, &mP3, &mP4);
//            
//            //Calculate the bounding rectangle that we are going to try to discard
//            _math.calculateBoundingRectangle(&mP1, &mP2, &mP3, &mP4);
//            
//            //Discard bounding rectangle using frustum culling if possible
//            if (!_math.cullFrustumBox(mP1, mP2, _frustrumPlanes)) {
//                _numDiscardedObjects++;
//            } else {
//#ifdef _DEBUG
//                GLboolean enabled;
//                glGetBooleanv(GL_TEXTURE_2D,&enabled);
//                assert(GL_FALSE != enabled); //Should have texturing enabled
//#endif
//                
//                glBindTexture(GL_TEXTURE_2D,pSu->_surface->_texturesArray[0]);
//                
//                //Set texture params requested before (via rainbow2d API)
//                setGLBoundTextureParams();
//                
//                //Override CLAMP for texture
//                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//                
//                glVertexPointer(3, GL_FLOAT, sizeof(CUSTOMVERTEX2D), &_vertices2d[0]._x);
//                glTexCoordPointer(2, GL_FLOAT, sizeof(CUSTOMVERTEX2D), &_vertices2d[0]._u);
//                glDrawArrays(GL_TRIANGLE_STRIP, 0,4);
//		    	
//#ifdef _DEBUG
//				GLenum glerror = glGetError();
//				if (glerror) {
//					g_debug->header("OpenGL error in surface blitting ", DebugApi::LogHeaderError);
//				}
//#endif
//                _numrenderedObjects++;
//            }
//		}
//		
//	}
	
}


bool OpenGLES2Render::blitWrapSurface(IND_Surface *pSu,
                                      int pBlitWidth,
                                      int pBlitHeight,
                                      float pUOffset,
                                      float pVOffset) {
   bool correctParams = true;
   if (pSu->getNumTextures() != 1) {
		correctParams = false; 
   }

   if (correctParams) {
       _tex2dState.wrapS = GL_REPEAT;
       _tex2dState.wrapT = GL_REPEAT;
       blitSurfaceImpl(pSu, pBlitWidth, pBlitHeight, pUOffset, pVOffset);
       
   }
	return correctParams;
}

void OpenGLES2Render::blitSurfaceImpl(IND_Surface *pSu, float pBlitWidth, float pBlitHeight, float pUOffset, float pVOffset) {
    // ----- Blitting -----
	int mCont = 0;

	for (int i = 0; i < pSu->getNumBlocks(); i++) {
        //Get vertex world coords, to perform frustrum culling test in world coords
		IND_Vector3 mP1, mP2, mP3, mP4;
        transformVerticesToWorld(static_cast<float>(pSu->_surface->_vertexArray[mCont]._pos._x), static_cast<float>(pSu->_surface->_vertexArray[mCont]._pos._y),
                                 static_cast<float>(pSu->_surface->_vertexArray[mCont + 1]._pos._x), static_cast<float>(pSu->_surface->_vertexArray[mCont + 1]._pos._y),
                                 static_cast<float>(pSu->_surface->_vertexArray[mCont + 2]._pos._x), static_cast<float>(pSu->_surface->_vertexArray[mCont + 2]._pos._y),
                                 static_cast<float>(pSu->_surface->_vertexArray[mCont + 3]._pos._x), static_cast<float>(pSu->_surface->_vertexArray[mCont + 3]._pos._y),
                                 &mP1, &mP2, &mP3, &mP4);
        
		//Discard bounding rectangle using frustum culling if possible
		_math.calculateBoundingRectangle(&mP1, &mP2, &mP3, &mP4);
		if (!_math.cullFrustumBox(mP1, mP2,_frustrumPlanes)) {
			_numDiscardedObjects++;
		} else {
            glActiveTexture(GL_TEXTURE0);
            
            // FIXME: This implies a perf. problem by swicthing textures for every block. Need better solution to send
            // all blocks to graphics card in same GL call.
			if (!pSu->isHaveGrid()) {
				//Texture ID - If it doesn't have a grid, every other block must be blit by
				//a different texture in texture array ID.
				glBindTexture(GL_TEXTURE_2D,pSu->_surface->_texturesArray[i]);
			} else {
				//In a case of rendering a grid. Same texture (but different vertex position)
				//is rendered all the time. In other words, different pieces of same texture are rendered
				glBindTexture(GL_TEXTURE_2D,pSu->_surface->_texturesArray[0]);
			}
            
            // Prepare the quad that is going to be blitted
            // Calculates the position and mapping coords for that block
            float u (pSu->_surface->_vertexArray[0]._texCoord._u);
            float v (pSu->_surface->_vertexArray[0]._texCoord._v);
            float width (static_cast<float>(pSu->getWidth()));
            float height (static_cast<float>(pSu->getHeight()));

            if (pBlitWidth > 0.f) {
                u = static_cast<float>(pBlitWidth)  / static_cast<float>(pSu->getWidthBlock());
                width = pBlitWidth;
            }
            
            if (pBlitHeight > 0.f) {
                v = static_cast<float>(pBlitHeight) / static_cast<float>(pSu->getHeightBlock());
                height = pBlitHeight;
            }
            
            //Upper-right
            fillVertex2d(&_vertices2d [mCont], width, 0.f, u + pUOffset, v + pVOffset);
            //Lower-right
            fillVertex2d(&_vertices2d [mCont+1], width, height, u + pUOffset, pVOffset);
            //Upper-left
            fillVertex2d(&_vertices2d [mCont+2], 0.f, 0.f,pUOffset,v + pVOffset);
            //Lower-left
            fillVertex2d(&_vertices2d [mCont+3], 0.f, height,pUOffset, pVOffset);
            
            //Set texture params as cached
            setGLBoundTextureParams();
	        
            IND_ShaderProgram* program = prepareSimple2DTexturingProgram();
            
            GLint posLoc = program->getPositionForVertexAttribute(IND_VertexAttribute_Position);
            glEnableVertexAttribArray(posLoc);
            glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CUSTOMVERTEX2D), &_vertices2d[mCont]._pos._x);
            
            GLint texCoordLoc = program->getPositionForVertexAttribute(IND_VertexAttribute_TexCoord);
            glEnableVertexAttribArray(texCoordLoc);
            glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(CUSTOMVERTEX2D), &_vertices2d[mCont]._texCoord._u);
            
			glDrawArrays(GL_TRIANGLE_STRIP, 0,4);
            
            glDisableVertexAttribArray(posLoc);
            glDisableVertexAttribArray(texCoordLoc);
            
            CHECKGLERRORS();
			_numrenderedObjects++;
		}
  		
		mCont += 4;
	}
}


int OpenGLES2Render::blitAnimation(IND_Animation *pAn, unsigned int pSequence,
                                int pX, int pY,
                                int pWidth, int pHeight,
                                bool pToggleWrap,
                                float pUOffset,
                                float pVOffset) {

	int mFinish = 1;

	if (pSequence < pAn->getNumSequences()) {
		if (!pAn->getIsActive(pSequence)) {
			pAn->getSequenceTimer(pSequence)->start();
			pAn->setIsActive(pSequence, 1);
		}

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
		
        IND_Matrix translation;
        _math.matrix4DSetTranslation(translation,static_cast<float>(pAn->getActualOffsetX(pSequence)),
                                      static_cast<float>(pAn->getActualOffsetY(pSequence)),
                                      0.0f);
        _math.matrix4DMultiplyInPlace(_modelToWorld, translation);
        
		// Blits all the IND_Surface (all the blocks)
		if (!pX && !pY && !pWidth && !pHeight) {
			blitSurface(pAn->getActualSurface(pSequence));
		} else
			// Blits a region of the IND_Surface
			if (!pToggleWrap) {
				if (pAn->getActualSurface(pSequence)->getNumTextures() > 1)
					return 0;
				blitRegionSurface(pAn->getActualSurface(pSequence), pX, pY, pWidth, pHeight);
		}
		// Blits a wrapping IND_Surface
		else {
			if (pAn->getActualSurface(pSequence)->getNumTextures() > 1)
				return 0;
			blitWrapSurface(pAn->getActualSurface(pSequence), pWidth, pHeight, pUOffset, pVOffset);
		}
	}

	return mFinish;
}

void OpenGLES2Render::blitGrid(IND_Surface *pSu, unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA) {
    
	for (int i = 0; i < pSu->getNumBlocks() * 4; i += 4) {
        //Get vertex world coords, to perform frustrum culling test in world coords
		IND_Vector3 mP1, mP2, mP3, mP4;
        int ax, ay, bx, by, cx, cy, dx, dy;
        ax = pSu->_surface->_vertexArray[i]._pos._x;
        ay = pSu->_surface->_vertexArray[i]._pos._y;
        bx = pSu->_surface->_vertexArray[i + 1]._pos._x;
        by = pSu->_surface->_vertexArray[i + 1]._pos._y;
        cx = pSu->_surface->_vertexArray[i + 2]._pos._x;
        cy = pSu->_surface->_vertexArray[i + 2]._pos._y;
        dx = pSu->_surface->_vertexArray[i + 3]._pos._x;
        dy = pSu->_surface->_vertexArray[i + 3]._pos._y;
        transformVerticesToWorld(static_cast<float>(ax), static_cast<float>(ay),
                                 static_cast<float>(bx), static_cast<float>(by),
                                 static_cast<float>(cx), static_cast<float>(cy),
                                 static_cast<float>(dx), static_cast<float>(dy),
                                 &mP1, &mP2, &mP3, &mP4);
        
        blitGridQuad(mP1._x, mP1._y,
                     mP2._x, mP2._y,
                     mP3._x, mP3._y,
                     mP4._x, mP4._y,
                     pR, pG, pB, pA);
		
	}
}

// --------------------------------------------------------------------------------
//							       Private methods
// --------------------------------------------------------------------------------


/*
==================
Fills a CUSTOMVERTEX2D structure
==================
*/
void OpenGLES2Render::fillVertex2d(CUSTOMVERTEX2D *pVertex2d, 
								 float pX, 
								 float pY, 
								 float pU,
								 float pV) {
	// Vertex
	pVertex2d->_pos._x       = pX;
	pVertex2d->_pos._y       = pY;
	pVertex2d->_pos._z       = 0.0f;
	pVertex2d->_texCoord._u       = pU;
	pVertex2d->_texCoord._v       = pV;
}
/** @endcond */
#endif //INDIERENDER_GLES_IOS




