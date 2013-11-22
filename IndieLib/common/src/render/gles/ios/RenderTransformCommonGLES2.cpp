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
#include "IND_Shaders.h"
#include "IND_ShaderProgram.h"
#include "IND_ShaderManager.h"

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

IND_ShaderProgram* OpenGLES2Render::prepareUniformColorProgram (unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA) {
    IND_ShaderProgram* program =  _shaderManager->getProgram(IND_UniformColorNoTextureProgram);
    program->use();
    
    float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
    program->setValueForUniform4f(IND_Uniform_Color, r, g, b, a);
    program->setValueForUniformMat(IND_Uniform_MVMatrix, _shaderModelViewMatrix);
    program->setValueForUniformMat(IND_Uniform_PMatrix, _shaderProjectionMatrix);
    
    return program;
}

/** @endcond */

#endif //INDIERENDER_GLES_IOS
