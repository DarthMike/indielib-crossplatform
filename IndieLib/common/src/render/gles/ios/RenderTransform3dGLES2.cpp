/*****************************************************************************************
 * File: RenderTransform3dOpenGL.cpp
 * Desc: Transformations applied before blitting a 3d object usind OpenGL
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

#ifdef INDIERENDER_GLES_IOS

// ----- Includes -----

#include "Global.h"
#include "Defines.h"
#include "OpenGLES2Render.h"
#include "IND_Camera3d.h"

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

bool OpenGLES2Render::setViewPort3d(int pX,
                                 int pY,
                                 int pWidth,
                                 int pHeight) {
	//TODO

	return 1;
}


void OpenGLES2Render::setCamera3d(IND_Camera3d *pCamera3d) {
	//TODO
}



void OpenGLES2Render::setTransform3d(float pX,
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


void OpenGLES2Render::setRainbow3d(bool pCull,
                                bool pFlipNormals,
                                IND_Filter pFilter,
                                unsigned char pR,
                                unsigned char pG,
                                unsigned char pB,
                                unsigned char pA,
                                unsigned char pFadeR,
                                unsigned char pFadeG,
                                unsigned char pFadeB,
                                unsigned char pFadeA,
                                IND_BlendingType pSo,
                                IND_BlendingType pDs) {
	//TODO
}

/** @endcond */

#endif //INDIERENDER_GLES_IOS
