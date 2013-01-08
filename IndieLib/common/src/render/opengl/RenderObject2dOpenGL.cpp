/*****************************************************************************************
 * File: RenderObject2dOpenGL.cpp
 * Desc: Blitting of 2d objects using OpenGL
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
#include <stdio.h>

#ifdef INDIERENDER_OPENGL
// ----- Includes -----

#include "Global.h"
#include "IND_SurfaceManager.h"
#include "OpenGLRender.h"
#include "IND_Surface.h"
#include "IND_Animation.h"
#include "TextureDefinitions.h"

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

void OpenGLRender::blitSurface(IND_Surface *pSu) {
    // ----- Blitting -----
	int mCont = 0;
    //LOOP - Blit textures in surface
	for (int i = 0; i < pSu->getNumBlocks(); i++) {
        //Get vertex world coords, to perform frustrum culling test in world coords
		IND_Vector3 mP1, mP2, mP3, mP4;
        transformVerticesToWorld(static_cast<float>(pSu->_surface->_vertexArray[mCont]._x), static_cast<float>(pSu->_surface->_vertexArray[mCont]._y),
		                   static_cast<float>(pSu->_surface->_vertexArray[mCont + 1]._x), static_cast<float>(pSu->_surface->_vertexArray[mCont + 1]._y),
		                   static_cast<float>(pSu->_surface->_vertexArray[mCont + 2]._x), static_cast<float>(pSu->_surface->_vertexArray[mCont + 2]._y),
		                   static_cast<float>(pSu->_surface->_vertexArray[mCont + 3]._x), static_cast<float>(pSu->_surface->_vertexArray[mCont + 3]._y),
		                   &mP1, &mP2, &mP3, &mP4);

		//Calculate the bounding rectangle that we are going to try to discard
		_math.calculateBoundingRectangle(&mP1, &mP2, &mP3, &mP4);

		//Discard bounding rectangle using frustum culling if possible

		if (!_math.cullFrustumBox(mP1, mP2,_frustrumPlanes)) {
			_numDiscardedObjects++;
		} else {

            //Enable texturing
            glEnable(GL_TEXTURE_2D);
            
			//Surface drawing
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			
			if (!pSu->isHaveGrid()) {
				//Texture ID - If it doesn't have a grid, every other block must be blit by 
				//a different texture in texture array ID. 
				glBindTexture(GL_TEXTURE_2D,pSu->_surface->_texturesArray[i]);
			} else {
				//In a case of rendering a grid. Same texture (but different vertex position)
				//is rendered all the time. In other words, different pieces of same texture are rendered
				glBindTexture(GL_TEXTURE_2D,pSu->_surface->_texturesArray[0]);
			}
			//Set CLAMP for texture
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	        
			glVertexPointer(3, GL_FLOAT, sizeof(CUSTOMVERTEX2D), &pSu->_surface->_vertexArray[mCont]._x);
			glTexCoordPointer(2, GL_FLOAT, sizeof(CUSTOMVERTEX2D), &pSu->_surface->_vertexArray[mCont]._u);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,4);	
	    	
		#ifdef _DEBUG
			GLenum glerror = glGetError();
			if (glerror) {
				g_debug->header("OpenGL error in surface blitting ", 2);
			}
		#endif	

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			_numrenderedObjects++;
		}
  		
		mCont += 4;
	}//LOOP END
}


void OpenGLRender::blitGrid(IND_Surface *pSu, BYTE pR, BYTE pG, BYTE pB, BYTE pA) {

	//LOOP - All texture blocks of the surface
	for (int i = 0; i < pSu->getNumBlocks() * 4; i += 4) {
        //Get vertex world coords, to perform frustrum culling test in world coords
		IND_Vector3 mP1, mP2, mP3, mP4;
        transformVerticesToWorld(static_cast<float>(pSu->_surface->_vertexArray[i]._x), static_cast<float>(pSu->_surface->_vertexArray[i]._y),
                                 static_cast<float>(pSu->_surface->_vertexArray[i + 1]._x), static_cast<float>(pSu->_surface->_vertexArray[i + 1]._y),
                                 static_cast<float>(pSu->_surface->_vertexArray[i + 2]._x), static_cast<float>(pSu->_surface->_vertexArray[i + 2]._y),
                                 static_cast<float>(pSu->_surface->_vertexArray[i + 3]._x), static_cast<float>(pSu->_surface->_vertexArray[i + 3]._y),
                                 &mP1, &mP2, &mP3, &mP4);
        
        //Calculate the bounding rectangle that we are going to try to discard
		_math.calculateBoundingRectangle(&mP1, &mP2, &mP3, &mP4);
        
        //Discard bounding rectangle using frustum culling if possible
        if (_math.cullFrustumBox(mP1, mP2,_frustrumPlanes)) {
			blitGridQuad((int) pSu->_surface->_vertexArray[i]._x, (int) pSu->_surface->_vertexArray[i]._y,
			             (int) pSu->_surface->_vertexArray[i + 1]._x, (int) pSu->_surface->_vertexArray[i + 1]._y,
			             (int) pSu->_surface->_vertexArray[i + 2]._x, (int) pSu->_surface->_vertexArray[i + 2]._y,
			             (int) pSu->_surface->_vertexArray[i + 3]._x, (int) pSu->_surface->_vertexArray[i + 3]._y,
			             pR, pG, pB, pA);
        }
		
	}//LOOP END

}


void OpenGLRender::blitRegionSurface(IND_Surface *pSu,
                                     int pX,
                                     int pY,
                                     int pWidth,
                                     int pHeight) {

	//If the region is the same as the image area, we blit normally
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
			//Only draws first texture block in texture
			// Prepare the quad that is going to be blitted
			// Calculates the position and mapping coords for that block
			float x (static_cast<float>(pX));
			float y (static_cast<float>(pY));
			float height (static_cast<float>(pHeight));
			float width (static_cast<float>(pWidth));
			float bWidth (static_cast<float>(pSu->getWidthBlock()));
			float bHeight (static_cast<float>(pSu->getHeightBlock()));
			float spareY (static_cast<float>(pSu->getSpareY()));
			fillVertex2d(&_vertices2d [0], width, 0.0f, ((x + width) / bWidth), (1.0f - ((y + spareY) / bHeight)));
			fillVertex2d(&_vertices2d [1], width, height, (x + width) / bWidth, (1.0f - ((y + height + spareY) / bHeight)));
			fillVertex2d(&_vertices2d [2], 0.0f, 0.0f , (x/bWidth), (1.0f - ((y+ spareY) / bHeight)));
			fillVertex2d(&_vertices2d [3], 0.0f, height, (x/bWidth), (1.0f - (y + height + spareY) / bHeight));
		        
        	//Get vertex world coords, to perform frustrum culling test in world coords
            IND_Vector3 mP1, mP2, mP3, mP4;
            transformVerticesToWorld(_vertices2d[0]._x, _vertices2d[0]._y,
                                     _vertices2d[1]._x, _vertices2d[1]._y,
                                     _vertices2d[2]._x, _vertices2d[2]._y,
                                     _vertices2d[3]._x, _vertices2d[3]._y,
                                     &mP1, &mP2, &mP3, &mP4);
            
            //Calculate the bounding rectangle that we are going to try to discard
            _math.calculateBoundingRectangle(&mP1, &mP2, &mP3, &mP4);
            
            //Discard bounding rectangle using frustum culling if possible
            if (!_math.cullFrustumBox(mP1, mP2, _frustrumPlanes)) {
                _numDiscardedObjects++;
            } else {
                //Enable texturing
                glEnable(GL_TEXTURE_2D);
                //Surface drawing
                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glBindTexture(GL_TEXTURE_2D,pSu->_surface->_texturesArray[0]);
                //Set CLAMP for texture
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                
                glVertexPointer(3, GL_FLOAT, sizeof(CUSTOMVERTEX2D), &_vertices2d[0]._x);
                glTexCoordPointer(2, GL_FLOAT, sizeof(CUSTOMVERTEX2D), &_vertices2d[0]._u);
                glDrawArrays(GL_TRIANGLE_STRIP, 0,4);
		    	
#ifdef _DEBUG
				GLenum glerror = glGetError();
				if (glerror) {
					g_debug->header("OpenGL error in surface blitting ", 2);
				}
#endif
                
                glDisableClientState(GL_VERTEX_ARRAY);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                _numrenderedObjects++;
            }
		}
		
	}
	
}


bool OpenGLRender::blitWrapSurface(IND_Surface *pSu,
                                   int pWidth,
                                   int pHeight,
                                   float pUDisplace,
                                   float pVDisplace) {
   bool correctParams = true;
   if (pSu->getNumTextures() != 1) {
		correctParams = false; 
   }

   if (correctParams) {
		_numrenderedObjects++;

		// Prepare the quad that is going to be blitted
		// Calculates the position and mapping coords for that block
		float u (static_cast<float>(pWidth)  / static_cast<float>(pSu->getWidthBlock()));
		float v (static_cast<float>(pHeight) / static_cast<float>(pSu->getHeightBlock()));
		float witdh (static_cast<float>(pWidth));
		float height (static_cast<float>(pHeight));
		//Upper-right
		fillVertex2d(&_vertices2d [0], witdh, 0, u - pUDisplace, pVDisplace);
		//Lower-right
		fillVertex2d(&_vertices2d [1], witdh, height, u - pUDisplace, -v + pVDisplace);
		//Upper-left
		fillVertex2d(&_vertices2d [2], 0.0f, 0.0f,-pUDisplace,pVDisplace);
		//Lower-left
		fillVertex2d(&_vertices2d [3], 0.0f, height,-pUDisplace, -v + pVDisplace);

       //Get vertex world coords, to perform frustrum culling test in world coords
       IND_Vector3 mP1, mP2, mP3, mP4;
       transformVerticesToWorld(_vertices2d[0]._x, _vertices2d[0]._y,
                                _vertices2d[1]._x, _vertices2d[1]._y,
                                _vertices2d[2]._x, _vertices2d[2]._y,
                                _vertices2d[3]._x, _vertices2d[3]._y,
                                &mP1, &mP2, &mP3, &mP4);
       
       //Calculate the bounding rectangle that we are going to try to discard
       _math.calculateBoundingRectangle(&mP1, &mP2, &mP3, &mP4);
       
       //Discard bounding rectangle using frustum culling if possible
       if (!_math.cullFrustumBox(mP1, mP2, _frustrumPlanes)) {
           _numDiscardedObjects++;
       } else {
           //Enable texturing
           glEnable(GL_TEXTURE_2D);
           //Surface drawing
           glEnableClientState(GL_VERTEX_ARRAY);
           glEnableClientState(GL_TEXTURE_COORD_ARRAY);
           glBindTexture(GL_TEXTURE_2D,pSu->_surface->_texturesArray[0]);
           //Set wrap for this texture
           glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
           glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
           
           glVertexPointer(3, GL_FLOAT, sizeof(CUSTOMVERTEX2D), &_vertices2d[0]._x);
           glTexCoordPointer(2, GL_FLOAT, sizeof(CUSTOMVERTEX2D), &_vertices2d[0]._u);
           glDrawArrays(GL_TRIANGLE_STRIP, 0,4);
           _numrenderedObjects++;
       }
   }

	return correctParams;
}


int OpenGLRender::blitAnimation(IND_Animation *pAn, int pSequence,
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
		
		glTranslatef(static_cast<float>(pAn->getActualOffsetX(pSequence)),
					 static_cast<float>(pAn->getActualOffsetY(pSequence)),
					 0.0f);

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
			blitWrapSurface(pAn->getActualSurface(pSequence), pWidth, pHeight, pUDisplace, pVDisplace);
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
void OpenGLRender::fillVertex2d(CUSTOMVERTEX2D *pVertex2d, 
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
/** @endcond */
#endif //INDIERENDER_OPENGL




