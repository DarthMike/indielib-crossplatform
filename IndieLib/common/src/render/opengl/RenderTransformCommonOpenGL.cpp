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

#ifdef INDIERENDER_OPENGL

// ----- Includes -----

#include "Global.h"
#include "Defines.h"
#include "OpenGLRender.h"
#include <iostream>

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------
void   OpenGLRender::clearViewPort(unsigned char pR,
                                   unsigned char pG,
                                   unsigned char pB) {
	//Clear color buffer
	glClearColor(static_cast<GLclampf>(pR / 255.0f),
	             static_cast<GLclampf>(pG / 255.0f),
	             static_cast<GLclampf>(pB / 255.0f),
	             0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRender::lookAt(float pEyeX, float pEyeY, float pEyeZ,
                          float pLookAtX, float pLookAtY, float pLookAtZ,
                          float pUpX, float pUpY, float pUpZ) {
    
                                 
    //Build the view matrix from given vectors
	IND_Matrix lookatmatrix; 
	_math.matrix4DLookAtMatrixEyeLookUpLH(IND_Vector3(pEyeX,pEyeY,pEyeZ),
	                           IND_Vector3(pLookAtX,pLookAtY,pLookAtZ),
	                           IND_Vector3(pUpX,pUpY,pUpZ),
	                           lookatmatrix);

#ifdef _DEBUG
	int mmode;
	glGetIntegerv(GL_MATRIX_MODE,&mmode);
	assert( mmode == GL_MODELVIEW);
#endif
    glLoadIdentity();
    glMultMatrixf(reinterpret_cast<GLfloat *>(&lookatmatrix));
}

void OpenGLRender::perspectiveFov(float pFov, float pAspect, float pNearClippingPlane, float pFarClippingPlane) {
	//TODO
}

void OpenGLRender::perspectiveOrtho(float pWidth, float pHeight, float pNearClippingPlane, float pFarClippingPlane) {
	//Projection matrix modification
	glMatrixMode(GL_PROJECTION);
	IND_Matrix orthoMatrix;
	_math.matrix4DOrthographicProjectionLH(-pWidth/2,pWidth/2,-pHeight/2,pHeight/2,pNearClippingPlane,pFarClippingPlane,orthoMatrix);
	glLoadMatrixf(reinterpret_cast<GLfloat *>(&orthoMatrix));
	
	//float m[16];
	//glGetFloatv(GL_PROJECTION_MATRIX, m);
	//for (size_t i = 0; i < 4; ++i) {    
	//	for (size_t j = 0; j < 4; ++j) {        
	//		std::cout << m[i + j * 4] << " ";    
	//	}    
	//	std::cout << std::endl;
	//}
	glMatrixMode(GL_MODELVIEW);
}

/** @endcond */

#endif //INDIERENDER_OPENGL
