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
and color attributes of the surface you have to use the OpenGLRender::setTransform2d() and OpenGLRender::setRainbow2d() methods before
calling this function. Remember that you can use IND_Entity2d object for drawing surfaces to the screen without having to use these
advanced methods directly. This method is only useful for advanced users with really concrete purposes.

Using this method is equivalent to using:
- IND_Entity2d::setSurface()
*/
void OpenGLRender::blitSurface(IND_Surface *pSu) {
	//FIXME: DISCARD BLITTING ELEMENTS OUTSIDE BOUNDING RECTANGLE AS IN DIRECTX
	//Enable texturing
	glEnable(GL_TEXTURE_2D);

    // ----- Blitting -----
	int mCont = 0;
    //LOOP - Blit textures in surface
	for (int i = 0; i < pSu->getNumBlocks(); i++) {

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
		
  		mCont += 4;
	}//LOOP END
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
void OpenGLRender::blitGrid(IND_Surface *pSu, BYTE pR, BYTE pG, BYTE pB, BYTE pA) {

	//LOOP - All texture blocks of the surface
	for (int i = 0; i < pSu->getNumBlocks() * 4; i += 4) {

		// FIXME: ---- Discard bounding rectangle using frustum culling if possible ----

			BlitGridQuad((int) pSu->_surface->_vertexArray[i]._x, (int) pSu->_surface->_vertexArray[i]._y,
			             (int) pSu->_surface->_vertexArray[i + 1]._x, (int) pSu->_surface->_vertexArray[i + 1]._y,
			             (int) pSu->_surface->_vertexArray[i + 2]._x, (int) pSu->_surface->_vertexArray[i + 2]._y,
			             (int) pSu->_surface->_vertexArray[i + 3]._x, (int) pSu->_surface->_vertexArray[i + 3]._y,
			             pR, pG, pB, pA);
		
	}//LOOP END
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
(you can check this parameter using ::OpenGLRender::getMaxTextureSize()). The method will return 0
otherwise.

In order to change the transformations and color attributes of the surface you have to use the OpenGLRender::setTransform2d() and OpenGLRender::setRainbow2d() methods before
calling to this function. Remember that you can use IND_Entity2d object for drawing surfaces to the screen without having to use this
advanced methods directly. This method is only useful for advanced users with really concrete purposes.

Using this method is equivalent to using both of these methods:
- IND_Entity2d::setSurface()
- IND_Entity2d::setRegion()
*/
void OpenGLRender::blitRegionSurface(IND_Surface *pSu,
                                     int pX,
                                     int pY,
                                     int pWidth,
                                     int pHeight) {

	//FIXME: DISCARD BLITTING ELEMENTS OUTSIDE BOUNDING RECTANGLE AS IN DIRECTX
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
			//Enable texturing
			glEnable(GL_TEXTURE_2D);

			// ----- Blitting -----
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
		        
			//Surface drawing
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindTexture(GL_TEXTURE_2D,pSu->_surface->_texturesArray[0]);
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
(you can check this parameter using ::OpenGLRender::getMaxTextureSize()). The method will return 0
otherwise.

In order to change the transformations and color attributes of the surface you have to use the OpenGLRender::setTransform2d() and OpenGLRender::setRainbow2d() methods before
calling to this function. Remember that you can use IND_Entity2d object for drawing surfaces to the screen without having to use this
advanced methods directly. This method is only useful for advanced users with really concrete purposes.

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setSurface()
- IND_Entity2d::setRegion()
- IND_Entity2d::toggleWrap()
- IND_Entity2d::setWrapDisplacement()
*/
bool OpenGLRender::blitWrapSurface(IND_Surface *pSu,
                                   int pWidth,
                                   int pHeight,
                                   float pUDisplace,
                                   float pVDisplace) {
	//FIXME: DISCARD BLITTING ELEMENTS OUTSIDE BOUNDING RECTANGLE AS IN DIRECTX
   bool correctParams = true;
   if (pSu->getNumTextures() != 1) {
		correctParams = false; 
   }

   if (correctParams) {
		//Enable texturing
		glEnable(GL_TEXTURE_2D);

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

		//Set wrap for this texture
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

		//Surface drawing
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D,pSu->_surface->_texturesArray[0]);
		glVertexPointer(3, GL_FLOAT, sizeof(CUSTOMVERTEX2D), &_vertices2d[0]._x);
		glTexCoordPointer(2, GL_FLOAT, sizeof(CUSTOMVERTEX2D), &_vertices2d[0]._u);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,4);
		
		//Set back clamp ON
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);	
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
and color attributes of the animation you have to use the OpenGLRender::setTransform2d() and OpenGLRender::setRainbow2d() methods before
calling to this function. Remember that you can use IND_Entity2d object for drawing animations to the screen without having to use this
advanced methods directly. This method is only useful for advanced users with really concrete purposes.

Special remark: if you specify a region this function only works with ::IND_Surface objects that only have ONE texture
assigned (you can check this using::IND_Surface::getNumTextures() method). So, it will work only
with images that are power of two and lower than the maximum texture size allowed by your card
(you can check this parameter using ::OpenGLRender::getMaxTextureSize()). The method will return 0
otherwise.

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setAnimation()
- IND_Entity2d::setRegion()
- IND_Entity2d::toggleWrap()
- IND_Entity2d::setWrapDisplacement()
*/
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
/*@}*/

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
#endif //INDIERENDER_OPENGL




