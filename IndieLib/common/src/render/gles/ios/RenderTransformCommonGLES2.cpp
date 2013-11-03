/*****************************************************************************************
 * File: RenderTransformCommonOpenGL.cpp
 * Desc: Transformations applied before blitting objects usind OpenGL
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
#include <iostream>

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------
void   OpenGLES2Render::clearViewPort(unsigned char pR,
                                   unsigned char pG,
                                   unsigned char pB) {
	//Clear color buffer
	glClearColor(static_cast<GLclampf>(pR / 255.0f),
	             static_cast<GLclampf>(pG / 255.0f),
	             static_cast<GLclampf>(pB / 255.0f),
	             0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLES2Render::perspectiveFov(float pFov, float pAspect, float pNearClippingPlane, float pFarClippingPlane) {
	//TODO
}

void OpenGLES2Render::perspectiveOrtho(float pWidth, float pHeight, float pNearClippingPlane, float pFarClippingPlane) {
	_math.matrix4DOrthographicProjectionLH(-pWidth/2,pWidth/2,-pHeight/2,pHeight/2,pNearClippingPlane,pFarClippingPlane,_shaderProjectionMatrix);
}

/** @endcond */

#endif //INDIERENDER_GLES_IOS
