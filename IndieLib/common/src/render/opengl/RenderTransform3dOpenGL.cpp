/*****************************************************************************************
 * File: RenderTransform3dOpenGL.cpp
 * Desc: Transformations applied before blitting a 3d object usind OpenGL
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
// ----- Includes -----

#include "Global.h"
#include "Defines.h"
#include "OpenGLRender.h"
#include "IND_Camera3d.h"

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

bool OpenGLRender::setViewPort3d(int pX,
                                 int pY,
                                 int pWidth,
                                 int pHeight) {
	//TODO

	return 1;
}


void OpenGLRender::setCamera3d(IND_Camera3d *pCamera3d) {
	//TODO
}



void OpenGLRender::setTransform3d(float pX,
                                  float  pY,
                                  float  pZ,
                                  float pAngleX,
                                  float pAngleY,
                                  float pAngleZ,
                                  float pScaleX,
                                  float pScaleY,
                                  float pScaleZ,
                                  IND_Matrix *pMatrix)


{
	//TODO
}


void OpenGLRender::setRainbow3d(bool pCull,
                                bool pFlipNormals,
                                IND_Filter pFilter,
                                BYTE pR,
                                BYTE pG,
                                BYTE pB,
                                BYTE pA,
                                BYTE pFadeR,
                                BYTE pFadeG,
                                BYTE pFadeB,
                                BYTE pFadeA,
                                IND_BlendingType pSo,
                                IND_BlendingType pDs) {
	//TODO
}

/** @endcond */

#endif //INDIERENDER_OPENGL
