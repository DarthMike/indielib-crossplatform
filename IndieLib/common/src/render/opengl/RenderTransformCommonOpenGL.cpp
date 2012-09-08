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
// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

/*!
\b Parameters:

\arg \b pR                  Byte R (Red)
\arg \b pG                  Byte G (Green)
\arg \b pB                  Byte B (Blue)

\b Operation:

Clean the viewport with a color expressed in RGB
*/
void   OpenGLRender::clearViewPort(BYTE pR,
                                   BYTE pG,
                                   BYTE pB) {
	//Clear color buffer
	glClearColor(static_cast<GLclampf>(pR / 255.0f),
	             static_cast<GLclampf>(pG / 255.0f),
	             static_cast<GLclampf>(pB / 255.0f),
	             0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//Clear back color buffer again if double buffering
    //TODO: See if double buffering needs to be taken into account here (clear semantics of "clearViewPort"
    //if (_doubleBuffer) {
	   // _osOpenGLMgr->presentBuffer();
	   // glClearColor(static_cast<GLclampf>(pR / 255.0f),
	   //              static_cast<GLclampf>(pG / 255.0f),
	   //              static_cast<GLclampf>(pB / 255.0f),
	   //              0.0f);
	   // glClear(GL_COLOR_BUFFER_BIT);
    //}
}


/*!
\b Parameters:

\arg \b pEyeX, pEyeY, pEyeZ                         Position of the camera
\arg \b pLookAtX, pLookAtY, pLookAtZ                Vector defining the direction of the camera
\arg \b pUpX, pUpY, pUpZ                            Vector "up" of the camera.

Operation:

This function sets a perspective matrix through the definition of a position and two vectors (lookat and up).

Using this method is equivalent to using a combination of methods of the class ::setCamera3d().
*/
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


/*!
\b Parameters:

\arg \b pFov                                        Vertical fov
\arg \b pAspect                                     Aspect ratio (usually the width of the viewport divided by the height)
\arg \b pNearClippingPlane                          Near clipping plane
\arg \b pFarClippingPlane                           Far clipping plane

Operation:

This function sets a fov projection matrix.

This method is equivalent to use a combination of methods of the class ::setCamera3d().
*/
void OpenGLRender::perspectiveFov(float pFov, float pAspect, float pNearClippingPlane, float pFarClippingPlane) {
	//TODO
}


/*!
\b Parameters:

\arg \b pWidth                                      Width
\arg \b pHeight                                     Height
\arg \b pNearClippingPlane                          Near clipping plane
\arg \b pFarClippingPlane                           Far clipping plane

Operation:

This function sets a orthographic projection matrix.

Using this method is equivalent to using a combination of the methods of the class ::setCamera3d().
*/
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

// --------------------------------------------------------------------------------
//							         Private methods
// --------------------------------------------------------------------------------

#endif //INDIERENDER_OPENGL
