/*****************************************************************************************
 * File: RenderPrimitive2dOpenGL.cpp
 * Desc: Primitive drawing using OpenGL
 * TODO: Polygons blitted from a loaded xml, suffering transformations, etc...
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
#include "IND_SurfaceManager.h"
#include "OpenGLES2Render.h"
#include "RenderMacros.h"
#include "IND_ShaderProgram.h"
#include "IND_Shaders.h"

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

bool OpenGLES2Render::setAntialiasing(bool pSwitch) {
//TODO

	return 1;
}

void OpenGLES2Render::blitPixel(int pX,
                             int pY,
                             unsigned char pR,
                             unsigned char pG,
                             unsigned char pB,
                             unsigned char pA) {
	
	fillPixel (&_points[0], static_cast<float>(pX), static_cast<float>(pY));
	
	setForPrimitive (pA, true);
    
    _defaultProgram->use();
    glBindBuffer(GL_ARRAY_BUFFER, _blitbuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(PIXEL), &_points[0]._x);
    
    float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
    _defaultProgram->setValueForUniform4f(IND_Uniform_Color, r, g, b, a);
    _defaultProgram->setValueForUniformMat(IND_Uniform_MVMatrix, _shaderModelViewMatrix);
    _defaultProgram->setValueForUniformMat(IND_Uniform_PMatrix, _shaderProjectionMatrix);
    
    GLint attribLoc = _defaultProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
    glEnableVertexAttribArray(attribLoc);
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
	glDrawArrays(GL_POINTS, 0,1);	

    glDisableVertexAttribArray(attribLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECKGLERRORS();
}

void OpenGLES2Render::blitLine(int pX1,
                            int pY1,
                            int pX2,
                            int pY2,
                            unsigned char pR,
                            unsigned char pG,
                            unsigned char pB,
                            unsigned char pA) {
	fillPixel(&_points[0], static_cast<float>(pX1), static_cast<float>(pY1));
	fillPixel(&_points[1], static_cast<float>(pX2), static_cast<float>(pY2));
    
	setForPrimitive(pA,true);

    _defaultProgram->use();
    glBindBuffer(GL_ARRAY_BUFFER, _blitbuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 2*sizeof(PIXEL), &_points[0]._x);
    
    float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
    _defaultProgram->setValueForUniform4f(IND_Uniform_Color, r, g, b, a);
    _defaultProgram->setValueForUniformMat(IND_Uniform_MVMatrix, _shaderModelViewMatrix);
    _defaultProgram->setValueForUniformMat(IND_Uniform_PMatrix, _shaderProjectionMatrix);
    
    GLint attribLoc = _defaultProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
    glEnableVertexAttribArray(attribLoc);
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
	glDrawArrays(GL_LINES, 0, 2);
    
    glDisableVertexAttribArray(attribLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECKGLERRORS();
}

void OpenGLES2Render::blitRectangle(int pX1,
                                 int pY1,
                                 int pX2,
                                 int pY2,
                                 unsigned char pR,
                                 unsigned char pG,
                                 unsigned char pB,
                                 unsigned char pA) {
	fillPixel (&_points[0], static_cast<float>(pX1),static_cast<float>(pY1));
	fillPixel (&_points[1], static_cast<float>(pX2), static_cast<float>(pY1));
	fillPixel (&_points[2], static_cast<float>(pX2), static_cast<float>(pY2));
	fillPixel (&_points[3], static_cast<float>(pX1), static_cast<float>(pY2));
	fillPixel (&_points[4], static_cast<float>(pX1), static_cast<float>(pY1));

	setForPrimitive (pA,true);

    _defaultProgram->use();
    glBindBuffer(GL_ARRAY_BUFFER, _blitbuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 5*sizeof(PIXEL), &_points[0]._x);
    
    float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
    _defaultProgram->setValueForUniform4f(IND_Uniform_Color, r, g, b, a);
    _defaultProgram->setValueForUniformMat(IND_Uniform_MVMatrix, _shaderModelViewMatrix);
    _defaultProgram->setValueForUniformMat(IND_Uniform_PMatrix, _shaderProjectionMatrix);
    
    GLint attribLoc = _defaultProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
    glEnableVertexAttribArray(attribLoc);
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
	glDrawArrays(GL_LINE_STRIP, 0, 5);

    glDisableVertexAttribArray(attribLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECKGLERRORS();
}

void OpenGLES2Render::blitFillRectangle(int pX1,
                                     int pY1,
                                     int pX2,
                                     int pY2,
                                     unsigned char pR,
                                     unsigned char pG,
                                     unsigned char pB,
                                     unsigned char pA) {
 	
	fillPixel (&_points [0], static_cast<float>(pX1),static_cast<float>(pY1));
	fillPixel (&_points [1], static_cast<float>(pX2), static_cast<float>(pY1));
	fillPixel (&_points [2], static_cast<float>(pX1), static_cast<float>(pY2));
	fillPixel (&_points [3], static_cast<float>(pX2), static_cast<float>(pY2));

	setForPrimitive (pA,true);
    
    float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
    _defaultProgram->setValueForUniform4f(IND_Uniform_Color, r, g, b, a);
    _defaultProgram->setValueForUniformMat(IND_Uniform_MVMatrix, _shaderModelViewMatrix);
    _defaultProgram->setValueForUniformMat(IND_Uniform_PMatrix, _shaderProjectionMatrix);

    glBindBuffer(GL_ARRAY_BUFFER, _blitbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4*sizeof(PIXEL), &_points[0]._x);
    GLint attribLoc = _defaultProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
    glEnableVertexAttribArray(attribLoc);
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(attribLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECKGLERRORS();
}

void OpenGLES2Render::blitTriangleList(IND_Point *pTrianglePoints,
                                    int pNumPoints,
                                    unsigned char pR,
                                    unsigned char pG,
                                    unsigned char pB,
                                    unsigned char pA) {
	if (pNumPoints < 3)
		return;
    
	//LOOP - Fill pixels structure
	for (int i = 0; i < pNumPoints; i++) {
		fillPixel(&_points[i], static_cast<float>(pTrianglePoints[i].x), static_cast<float>(pTrianglePoints[i].y));
	}//LOOP END

	// Color and transformation
	setForPrimitive(pA,true);

    _defaultProgram->use();
    
    float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
    _defaultProgram->setValueForUniform4f(IND_Uniform_Color, r, g, b, a);
    _defaultProgram->setValueForUniformMat(IND_Uniform_MVMatrix, _shaderModelViewMatrix);
    _defaultProgram->setValueForUniformMat(IND_Uniform_PMatrix, _shaderProjectionMatrix);
    
    glBindBuffer(GL_ARRAY_BUFFER, _blitbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, pNumPoints*sizeof(PIXEL), &_points[0]._x);
    GLint attribLoc = _defaultProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
    glEnableVertexAttribArray(attribLoc);
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, pNumPoints);
    
    glDisableVertexAttribArray(attribLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECKGLERRORS();
}


void OpenGLES2Render::blitColoredTriangle(int pX1,
                                       int pY1,
                                       int pX2,
                                       int pY2,
                                       int pX3,
                                       int pY3,
                                       unsigned char pR1, unsigned char pG1, unsigned char pB1,
                                       unsigned char pR2, unsigned char pG2, unsigned char pB2,
                                       unsigned char pR3, unsigned char pG3, unsigned char pB3,
                                       unsigned char pA) {
	//TODO: per-vertex color
//    float r1(static_cast<float>(pR1) / 255.0f), g1(static_cast<float>(pG1) / 255.0f), b1(static_cast<float>(pB1) / 255.0f);
//    float r2(static_cast<float>(pR2) / 255.0f), g2(static_cast<float>(pG2) / 255.0f), b2(static_cast<float>(pB2) / 255.0f);
//    float r3(static_cast<float>(pR3) / 255.0f), g3(static_cast<float>(pG3) / 255.0f), b3(static_cast<float>(pB3) / 255.0f);
//    float a(static_cast<float>(pA) / 255.0f);

	fillPixel (&_points[0], static_cast<float>(pX1), static_cast<float>(pY1));
	fillPixel (&_points[1], static_cast<float>(pX2), static_cast<float>(pY2));
	fillPixel (&_points[2], static_cast<float>(pX3), static_cast<float>(pY3));
    
	// Color and transformation
	setForPrimitive(pA,true);
    
    _defaultProgram->use();
    
    glBindBuffer(GL_ARRAY_BUFFER, _blitbuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3*sizeof(PIXEL), &_points[0]._x);
    
    float r(static_cast<float>(pR1) / 255.0f), g(static_cast<float>(pG1) / 255.0f), b(static_cast<float>(pB1) / 255.0f), a(static_cast<float>(pA) / 255.0f);
    _defaultProgram->setValueForUniform4f(IND_Uniform_Color, r, g, b, a);
    _defaultProgram->setValueForUniformMat(IND_Uniform_MVMatrix, _shaderModelViewMatrix);
    _defaultProgram->setValueForUniformMat(IND_Uniform_PMatrix, _shaderProjectionMatrix);
    
    GLint attribLoc = _defaultProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
    glEnableVertexAttribArray(attribLoc);
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glDisableVertexAttribArray(attribLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECKGLERRORS();

}


bool OpenGLES2Render::blitPoly2d(IND_Point *pPolyPoints,
                              int pNumLines,
                              unsigned char pR,
                              unsigned char pG,
                              unsigned char pB,
                              unsigned char pA) {

	if (!pPolyPoints)   return 0;
	if (pNumLines < 1)  return 0;

	// Fill PIXEL structures
    for (int i = 0; i < pNumLines + 1; i++){
	    fillPixel (&_points[i], static_cast<float>(pPolyPoints [i].x), static_cast<float>(pPolyPoints [i].y));
    }


	// Color and transformation
	setForPrimitive(pA,true);

    _defaultProgram->use();
    
    glBindBuffer(GL_ARRAY_BUFFER, _blitbuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (pNumLines+1)*sizeof(PIXEL), &_points[0]._x);
    
    float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
    _defaultProgram->setValueForUniform4f(IND_Uniform_Color, r, g, b, a);
    _defaultProgram->setValueForUniformMat(IND_Uniform_MVMatrix, _shaderModelViewMatrix);
    _defaultProgram->setValueForUniformMat(IND_Uniform_PMatrix, _shaderProjectionMatrix);
    
    GLint attribLoc = _defaultProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
    glEnableVertexAttribArray(attribLoc);
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
	glDrawArrays(GL_LINE_STRIP, 0, pNumLines+1);

    glDisableVertexAttribArray(attribLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECKGLERRORS();
	return 1;
}


bool OpenGLES2Render::blitRegularPoly(int pX,
                                   int pY,
                                   int pRadius,
                                   int pN,
                                   float pAngle,
                                   unsigned char pR,
                                   unsigned char pG,
                                   unsigned char pB,
                                   unsigned char pA) {
	int x, y, i;
	for (i = 0; i < pN; i++) {
		float c = i * 2 * static_cast<float>(PI / pN);
		x = static_cast<int>(pX + (pRadius * cos(c + _math.angleToRadians(pAngle))));
		y = static_cast<int>(pY + (pRadius * sin(c + _math.angleToRadians(pAngle))));

		fillPixel(&_points[i], static_cast<float>(x), static_cast<float>(y));
	}

	fillPixel(&_points[i], pX + (pRadius * cos(_math.angleToRadians(pAngle))), pY + (pRadius * sin(_math.angleToRadians(pAngle))));
	
	setForPrimitive(pA,true);

    _defaultProgram->use();
    
    glBindBuffer(GL_ARRAY_BUFFER, _blitbuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (pN+1)*sizeof(PIXEL), &_points[0]._x);
    
    float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
    _defaultProgram->setValueForUniform4f(IND_Uniform_Color, r, g, b, a);
    _defaultProgram->setValueForUniformMat(IND_Uniform_MVMatrix, _shaderModelViewMatrix);
    _defaultProgram->setValueForUniformMat(IND_Uniform_PMatrix, _shaderProjectionMatrix);
    
    GLint attribLoc = _defaultProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
    glEnableVertexAttribArray(attribLoc);
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
	glDrawArrays(GL_LINE_STRIP, 0, pN+1);

    glDisableVertexAttribArray(attribLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECKGLERRORS();
	return 1;
}

// --------------------------------------------------------------------------------
//							         Private methods
// --------------------------------------------------------------------------------
void OpenGLES2Render::fillPixel(PIXEL *pPixel,
                             float pX,
                             float pY) {
	// Pixel
	pPixel->_x      =  pX;
	pPixel->_y      =  pY;
	pPixel->_z      = 0.0f;
}


void OpenGLES2Render::setForPrimitive(unsigned char pA, bool pResetTransform) {
	// Transformation reset
	if (pResetTransform) {
		setTransform2d(0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0);
	}

	//IF - Totally opaque
	if (pA == 255) {
		setRainbow2d(IND_OPAQUE, 1, 0, 0, IND_FILTER_POINT, 255, 255, 255, pA, 0, 0, 0, 255, 0, 0);
	} else { // ELSE - Some transparency
		setRainbow2d(IND_ALPHA, 1, 0, 0, IND_FILTER_POINT, 255, 255, 255, pA, 0, 0, 0, 255, 0, 0);
	}
}

/*
==================
Blits a bounding line
==================
*/
 void OpenGLES2Render::blitGridLine (int pPosX1, int pPosY1, int pPosX2, int pPosY2,  unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA)
{	
//	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
//	// Filling pixels
//    fillPixel (&_pixels[0], static_cast<float>(pPosX1), static_cast<float>(pPosY1), r, g, b, a);
//    fillPixel (&_pixels[1], static_cast<float>(pPosX2), static_cast<float>(pPosY2), r, g, b, a);
//
//	//Render primitive - No textures
//	setGLClientStateToPrimitive();
//    
//	// Color settings
//    setRainbow2d (IND_OPAQUE, 1, 0, 0, IND_FILTER_POINT, pR, pG, pB, pA, 0, 0, 0, 255, 0, 0);
//
//	//Polygon blitting
//	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
//	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
//	glDrawArrays(GL_LINE_STRIP, 0, 2);
//
//#ifdef _DEBUG
//    GLenum glerror = glGetError();
//	if (glerror) {
//		g_debug->header("OpenGL error in grid line blitting ", DebugApi::LogHeaderError);
//	}
//#endif
}

/*
==================
Blits quad of the grid of an IND_Surface
==================
*/
void OpenGLES2Render::blitGridQuad    (int pAx, int pAy,
                                   int pBx, int pBy,
                                   int pCx, int pCy,
                                   int pDx, int pDy,
                                   unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA)
{
	blitGridLine (pAx, pAy, pBx, pBy, pR, pG, pB, pA);
	blitGridLine (pBx, pBy, pDx, pDy, pR, pG, pB, pA);
	blitGridLine (pDx, pDy, pCx, pCy, pR, pG, pB, pA);
	blitGridLine (pCx, pCy, pAx, pAy, pR, pG, pB, pA);
}

/*
==================
Blits a bounding circle area
==================
*/
void OpenGLES2Render::blitCollisionCircle(int pPosX, int pPosY, int pRadius, float pScale,  unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA, IND_Matrix pIndWorldMatrix) {
//	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
//
//	// Filling pixels
//	float x (0.0f);
//	float y (0.0f);
//	int points (SIDES_PER_CIRCLE + 1);
//	assert(0 != points);
//	float angle (2*PI / SIDES_PER_CIRCLE);
//	for (int i = 0; i <= points ; i++) {
//		x = pPosX + (pRadius * cosf(angle*i));
//		y = pPosY + (pRadius * sinf(angle*i));
//		fillPixel (&_pixels[i], x, y, r, g, b, a);
//	}
//    
//	//Render primitive - No textures
//	setGLClientStateToPrimitive();
//    
//	//Transform
//	setTransform2d(pIndWorldMatrix);
//
//	// Color settings
//    setRainbow2d (IND_OPAQUE, 1, 0, 0, IND_FILTER_POINT, pR, pG, pB, pA, 0, 0, 0, 255, 0, 0);
//
//	//Polygon blitting
//	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
//	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
//	glDrawArrays(GL_LINE_STRIP, 0, points);
//
//#ifdef _DEBUG
//    GLenum glerror = glGetError();
//	if (glerror) {
//		g_debug->header("OpenGL error in circle blitting ", DebugApi::LogHeaderError);
//	}
//#endif
}


/*
==================
Blits a bounding line
==================
*/
void OpenGLES2Render::blitCollisionLine(int pPosX1, int pPosY1, int pPosX2, int pPosY2,  unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA, IND_Matrix pIndWorldMatrix) {

	//Transform with supplied matrix
	setTransform2d(pIndWorldMatrix);
	//Blit the line 
	blitGridLine (pPosX1, pPosY1, pPosX2, pPosY2,  pR, pG, pB, pA);
}

/** @endcond */

#endif //INDIERENDER_GLES_IOS
