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
#include "IND_ShaderManager.h"

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
	
	fillPoint (_points, static_cast<float>(pX), static_cast<float>(pY));
	
	setTransformAndGLStateForPrimitive (pA, true);
    IND_ShaderProgram* primitiveRenderProgram = prepareUniformColorProgram(pR, pG, pB, pA);
    
    glBindBuffer(GL_ARRAY_BUFFER, _pointBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VERTEX_POS), _points);
    
    GLint attribLoc = primitiveRenderProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
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
	fillPoint(&_points[0], static_cast<float>(pX1), static_cast<float>(pY1));
	fillPoint(&_points[1], static_cast<float>(pX2), static_cast<float>(pY2));
    
	setTransformAndGLStateForPrimitive(pA,true);
    IND_ShaderProgram* primitiveRenderProgram = prepareUniformColorProgram(pR, pG, pB, pA);
    
    glBindBuffer(GL_ARRAY_BUFFER, _pointBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 2*sizeof(VERTEX_POS), _points);
    
    GLint attribLoc = primitiveRenderProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
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
	fillPoint (&_points[0], static_cast<float>(pX1),static_cast<float>(pY1));
	fillPoint (&_points[1], static_cast<float>(pX2), static_cast<float>(pY1));
	fillPoint (&_points[2], static_cast<float>(pX2), static_cast<float>(pY2));
	fillPoint (&_points[3], static_cast<float>(pX1), static_cast<float>(pY2));
	fillPoint (&_points[4], static_cast<float>(pX1), static_cast<float>(pY1));

	setTransformAndGLStateForPrimitive (pA,true);
    IND_ShaderProgram* primitiveRenderProgram = prepareUniformColorProgram(pR, pG, pB, pA);
    
    glBindBuffer(GL_ARRAY_BUFFER, _pointBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 5*sizeof(VERTEX_POS), _points);
    
    GLint attribLoc = primitiveRenderProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
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
 	
	fillPoint (&_points [0], static_cast<float>(pX1),static_cast<float>(pY1));
	fillPoint (&_points [1], static_cast<float>(pX2), static_cast<float>(pY1));
	fillPoint (&_points [2], static_cast<float>(pX1), static_cast<float>(pY2));
	fillPoint (&_points [3], static_cast<float>(pX2), static_cast<float>(pY2));

	setTransformAndGLStateForPrimitive (pA,true);
    IND_ShaderProgram* primitiveRenderProgram = prepareUniformColorProgram(pR, pG, pB, pA);

    glBindBuffer(GL_ARRAY_BUFFER, _pointBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4*sizeof(VERTEX_POS), _points);
    
    GLint attribLoc = primitiveRenderProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
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
    
	for (int i = 0; i < pNumPoints; i++) {
		fillPoint(&_points[i], static_cast<float>(pTrianglePoints[i].x), static_cast<float>(pTrianglePoints[i].y));
	}

	setTransformAndGLStateForPrimitive(pA,true);
    IND_ShaderProgram* primitiveRenderProgram = prepareUniformColorProgram(pR, pG, pB, pA);
    
    glBindBuffer(GL_ARRAY_BUFFER, _pointBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, pNumPoints*sizeof(VERTEX_POS), _points);
    
    GLint attribLoc = primitiveRenderProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
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
	fillPointWithColor (&_pointsWithColor[0], static_cast<float>(pX1), static_cast<float>(pY1), pR1,pG1,pB1,pA);
	fillPointWithColor (&_pointsWithColor[1], static_cast<float>(pX2), static_cast<float>(pY2), pR2,pG2,pB2,pA);
	fillPointWithColor (&_pointsWithColor[2], static_cast<float>(pX3), static_cast<float>(pY3), pR3,pG3,pB3,pA);
    
	setTransformAndGLStateForPrimitive(pA,true);
    IND_ShaderProgram* primitiveRenderProgram = preparePervertexColorProgram();
    
    glBindBuffer(GL_ARRAY_BUFFER, _pointWithColorBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3*sizeof(VERTEX_POSANDCOLOR), _pointsWithColor);
    
    GLint posLoc = primitiveRenderProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
    glEnableVertexAttribArray(posLoc);
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX_POSANDCOLOR), 0);
    
    GLint colorLoc = primitiveRenderProgram->getPositionForVertexAttribute(IND_VertexAttribute_RGBAColor);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VERTEX_POSANDCOLOR), reinterpret_cast<void*>(sizeof(VERTEX_POS)));
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glDisableVertexAttribArray(posLoc);
    glDisableVertexAttribArray(colorLoc);
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

    for (int i = 0; i < pNumLines + 1; i++){
	    fillPoint (&_points[i], static_cast<float>(pPolyPoints [i].x), static_cast<float>(pPolyPoints [i].y));
    }

	setTransformAndGLStateForPrimitive(pA,true);
    IND_ShaderProgram* primitiveRenderProgram = prepareUniformColorProgram(pR, pG, pB, pA);
    
    glBindBuffer(GL_ARRAY_BUFFER, _pointBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (pNumLines+1)*sizeof(VERTEX_POS), _points);
    
    GLint attribLoc = primitiveRenderProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
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

		fillPoint(&_points[i], static_cast<float>(x), static_cast<float>(y));
	}

	fillPoint(&_points[i], pX + (pRadius * cos(_math.angleToRadians(pAngle))), pY + (pRadius * sin(_math.angleToRadians(pAngle))));
	
	setTransformAndGLStateForPrimitive(pA,true);
    IND_ShaderProgram* primitiveRenderProgram = prepareUniformColorProgram(pR, pG, pB, pA);
    
    glBindBuffer(GL_ARRAY_BUFFER, _pointBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (pN+1)*sizeof(VERTEX_POS), _points);
    
    GLint attribLoc = primitiveRenderProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
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
void OpenGLES2Render::fillPoint(VERTEX_POS *pVertex,
                             float pX,
                             float pY) {
	pVertex->_x = pX;
	pVertex->_y = pY;
	pVertex->_z = 0.0f;
}

void OpenGLES2Render::fillPointWithColor(VERTEX_POSANDCOLOR *pVertex, float pX, float pY, unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA) {
	pVertex->_pos._x = pX;
	pVertex->_pos._y = pY;
	pVertex->_pos._z = 0.0f;
    pVertex->_color._colorR = pR;
	pVertex->_color._colorG = pG;
	pVertex->_color._colorB = pB;
	pVertex->_color._colorA = pA;
}


void OpenGLES2Render::setTransformAndGLStateForPrimitive(unsigned char pA, bool pResetTransform) {
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
	fillPoint(&_points[0], static_cast<float>(pPosX1), static_cast<float>(pPosY1));
	fillPoint(&_points[1], static_cast<float>(pPosX2), static_cast<float>(pPosY2));
    // TODO: This repeats code on blitLine, but not resetting transform for primitive.
    
	setTransformAndGLStateForPrimitive(pA,false);
    IND_ShaderProgram* primitiveRenderProgram = prepareUniformColorProgram(pR, pG, pB, pA);
    
    glBindBuffer(GL_ARRAY_BUFFER, _pointBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 2*sizeof(VERTEX_POS), _points);
    
    GLint attribLoc = primitiveRenderProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
    glEnableVertexAttribArray(attribLoc);
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
	glDrawArrays(GL_LINES, 0, 2);
    
    glDisableVertexAttribArray(attribLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECKGLERRORS();
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
    
	float x (0.0f);
	float y (0.0f);
	int points (SIDES_PER_CIRCLE + 1);
	assert(0 != points);
	float angle (2*PI / SIDES_PER_CIRCLE);
	for (int i = 0; i <= points ; i++) {
		x = pPosX + (pRadius * cosf(angle*i));
		y = pPosY + (pRadius * sinf(angle*i));
		fillPoint (&_points[i], x, y);
	}
    
	setTransform2d(pIndWorldMatrix);
    setRainbow2d (IND_OPAQUE, 1, 0, 0, IND_FILTER_POINT, pR, pG, pB, pA, 0, 0, 0, 255, 0, 0);
    IND_ShaderProgram* primitiveRenderProgram = prepareUniformColorProgram(pR, pG, pB, pA);
    
    glBindBuffer(GL_ARRAY_BUFFER, _pointBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, points*sizeof(VERTEX_POS), _points);
    
    GLint attribLoc = primitiveRenderProgram->getPositionForVertexAttribute(IND_VertexAttribute_Position);
    glEnableVertexAttribArray(attribLoc);
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
	glDrawArrays(GL_LINE_STRIP, 0, points);
    
    glDisableVertexAttribArray(attribLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECKGLERRORS();
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
