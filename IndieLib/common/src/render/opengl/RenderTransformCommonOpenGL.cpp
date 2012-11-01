/*****************************************************************************************
 * File: RenderTransformCommonOpenGL.cpp
 * Desc: Transformations applied before blitting objects usind OpenGL
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
#include <iostream>

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------
void   OpenGLRender::clearViewPort(BYTE pR,
                                   BYTE pG,
                                   BYTE pB) {
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
