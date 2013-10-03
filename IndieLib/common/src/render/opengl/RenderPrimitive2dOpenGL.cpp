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

#ifdef INDIERENDER_OPENGL

// ----- Includes -----

#include "Global.h"
#include "IND_SurfaceManager.h"
#include "OpenGLRender.h"

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

bool OpenGLRender::setAntialiasing(bool pSwitch) {
//TODO

	return 1;
}

void OpenGLRender::blitPixel(int pX,
                             int pY,
                             unsigned char pR,
                             unsigned char pG,
                             unsigned char pB,
                             unsigned char pA) {
	
	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
	// Fill the PIXEL structure
	fillPixel (&_pixels [0], static_cast<float>(pX), static_cast<float>(pY), r, g, b, a);
	
	// Pixel drawing
	setGLClientStateToPrimitive();

	// Color and transformation
	setForPrimitive (pA, true);

	
    
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_POINTS, 0,1);	

	
#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in pixel blitting ", DebugApi::LogHeaderError);
	}
#endif	

    //Reenable texturing
    setGLClientStateToTexturing();
	
}

void OpenGLRender::blitLine(int pX1,
                            int pY1,
                            int pX2,
                            int pY2,
                            unsigned char pR,
                            unsigned char pG,
                            unsigned char pB,
                            unsigned char pA) {
	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
	//Fill the PIXEL structure
	fillPixel(&_pixels[0], static_cast<float>(pX1), static_cast<float>(pY1), r, g, b, a);
	fillPixel(&_pixels[1], static_cast<float>(pX2), static_cast<float>(pY2), r, g, b, a);

	//Render primitive - No textures
	setGLClientStateToPrimitive();

	// Color and transformation
	setForPrimitive(pA,true);

	//Line blitting
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_LINES, 0, 2);

	
#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in Line blitting ", DebugApi::LogHeaderError);
	}
#endif

    //Reenable texturing
	setGLClientStateToTexturing();
}

void OpenGLRender::blitRectangle(int pX1,
                                 int pY1,
                                 int pX2,
                                 int pY2,
                                 unsigned char pR,
                                 unsigned char pG,
                                 unsigned char pB,
                                 unsigned char pA) {
	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
 	
	// Fill PIXEL structures
	fillPixel (&_pixels [0], static_cast<float>(pX1),static_cast<float>(pY1),      r, g, b,a);
	fillPixel (&_pixels [1], static_cast<float>(pX2), static_cast<float>(pY1),      r, g, b,a);
	fillPixel (&_pixels [2], static_cast<float>(pX2), static_cast<float>(pY2),      r, g, b,a);
	fillPixel (&_pixels [3], static_cast<float>(pX1), static_cast<float>(pY2),      r, g, b,a);
	fillPixel (&_pixels [4], static_cast<float>(pX1), static_cast<float>(pY1),      r, g, b,a);
	
	//Render primitive - No textures
	setGLClientStateToPrimitive();

	// Color and transformation
	setForPrimitive (pA,true);

	//rectangle blitting
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_LINE_STRIP, 0, 5);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in rectangle blitting ", DebugApi::LogHeaderError);
	}
#endif

    //Reenable texturing
	setGLClientStateToTexturing();
}

void OpenGLRender::blitFillRectangle(int pX1,
                                     int pY1,
                                     int pX2,
                                     int pY2,
                                     unsigned char pR,
                                     unsigned char pG,
                                     unsigned char pB,
                                     unsigned char pA) {
	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
 	
	// Fill PIXEL structures
	fillPixel (&_pixels [0], static_cast<float>(pX1),static_cast<float>(pY1),      r, g, b,a);
	fillPixel (&_pixels [1], static_cast<float>(pX2), static_cast<float>(pY1),      r, g, b,a);
	fillPixel (&_pixels [2], static_cast<float>(pX1), static_cast<float>(pY2),      r, g, b,a);
	fillPixel (&_pixels [3], static_cast<float>(pX2), static_cast<float>(pY2),      r, g, b,a);

	//Render primitive - No textures
	setGLClientStateToPrimitive();

	// Color and transformation
	setForPrimitive (pA,true);

	//rectangle blitting
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in rectangle blitting ", DebugApi::LogHeaderError);
	}
#endif

    //Reenable texturing
	setGLClientStateToTexturing();
}

void OpenGLRender::blitTriangleList(IND_Point *pTrianglePoints,
                                    int pNumPoints,
                                    unsigned char pR,
                                    unsigned char pG,
                                    unsigned char pB,
                                    unsigned char pA) {

	//TODO: CHECK MAX POLYGONS PER CALL...
	if (pNumPoints < 3)
		return;

    float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);

	//LOOP - Fill pixels structure
	for (int i = 0; i < pNumPoints; i++) {
		fillPixel(&_pixels[i], static_cast<float>(pTrianglePoints[i].x), static_cast<float>(pTrianglePoints[i].y), r, g, b, a);
	}//LOOP END

	//Render primitive - No textures
	setGLClientStateToPrimitive();

	// Color and transformation
	setForPrimitive(pA,true);

	//Triangle strip blitting
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, pNumPoints);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in triangle list blitting ", DebugApi::LogHeaderError);
	}
#endif

    //Reenable texturing
	setGLClientStateToTexturing();
}


void OpenGLRender::blitColoredTriangle(int pX1,
                                       int pY1,
                                       int pX2,
                                       int pY2,
                                       int pX3,
                                       int pY3,
                                       unsigned char pR1, unsigned char pG1, unsigned char pB1,
                                       unsigned char pR2, unsigned char pG2, unsigned char pB2,
                                       unsigned char pR3, unsigned char pG3, unsigned char pB3,
                                       unsigned char pA) {
	
    float r1(static_cast<float>(pR1) / 255.0f), g1(static_cast<float>(pG1) / 255.0f), b1(static_cast<float>(pB1) / 255.0f);
    float r2(static_cast<float>(pR2) / 255.0f), g2(static_cast<float>(pG2) / 255.0f), b2(static_cast<float>(pB2) / 255.0f);
    float r3(static_cast<float>(pR3) / 255.0f), g3(static_cast<float>(pG3) / 255.0f), b3(static_cast<float>(pB3) / 255.0f);
    float a(static_cast<float>(pA) / 255.0f);
	
    // Fill PIXEL structures
	fillPixel (&_pixels[0], static_cast<float>(pX1), static_cast<float>(pY1), r1, g1, b1, a);
	fillPixel (&_pixels[1], static_cast<float>(pX2), static_cast<float>(pY2), r2, g2, b2, a);
	fillPixel (&_pixels[2], static_cast<float>(pX3), static_cast<float>(pY3), r3, g3, b3, a);

	//Render primitive - No textures
	setGLClientStateToPrimitive();

	// Color and transformation
	setForPrimitive(pA,true);

	//Single Colored Triangle blitting
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_TRIANGLES, 0, 3);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in colored triangle blitting ", DebugApi::LogHeaderError);
	}
#endif

    //Reenable texturing
	setGLClientStateToTexturing();
}


bool OpenGLRender::blitPoly2d(IND_Point *pPolyPoints,
                              int pNumLines,
                              unsigned char pR,
                              unsigned char pG,
                              unsigned char pB,
                              unsigned char pA) {

	if (!pPolyPoints)   return 0;
	if (pNumLines < 1)  return 0;

    float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);

	// Fill PIXEL structures
    for (int i = 0; i < pNumLines + 1; i++){
	    fillPixel (&_pixels [i], static_cast<float>(pPolyPoints [i].x), static_cast<float>(pPolyPoints [i].y), r, g, b,a);
    }

	//Render primitive - No textures
	setGLClientStateToPrimitive();

	// Color and transformation
	setForPrimitive(pA,true);

	// Polygon blitting
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_LINE_STRIP, 0, pNumLines+1);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in triangle poly2d blitting ", DebugApi::LogHeaderError);
	}
#endif

    //Reenable texturing
	setGLClientStateToTexturing();
	return 1;
}


bool OpenGLRender::blitRegularPoly(int pX,
                                   int pY,
                                   int pRadius,
                                   int pN,
                                   float pAngle,
                                   unsigned char pR,
                                   unsigned char pG,
                                   unsigned char pB,
                                   unsigned char pA) {
	int x, y, i;
	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);

	//LOOP - Fill pixels structure
	for (i = 0; i < pN; i++) {
		float c = i * 2 * static_cast<float>(PI / pN);
		x = static_cast<int>(pX + (pRadius * cos(c + _math.angleToRadians(pAngle))));
		y = static_cast<int>(pY + (pRadius * sin(c + _math.angleToRadians(pAngle))));

		fillPixel(&_pixels [i], static_cast<float>(x), static_cast<float>(y), r, g, b, a);
	}//LOOP END

	fillPixel(&_pixels [i], pX + (pRadius * cos(_math.angleToRadians(pAngle))), pY + (pRadius * sin(_math.angleToRadians(pAngle))), r, g, b, a);
	
	//Render primitive - No textures
	setGLClientStateToPrimitive();

	// Color and transformation
	setForPrimitive(pA,true);

	//Polygon blitting
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_LINE_STRIP, 0, pN+1);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in triangle list blitting ", DebugApi::LogHeaderError);
	}
#endif

    //Reenable texturing
	setGLClientStateToTexturing();
	return 1;
}

// --------------------------------------------------------------------------------
//							         Private methods
// --------------------------------------------------------------------------------
void OpenGLRender::fillPixel(PIXEL *pPixel,
                             float pX,
                             float pY,
                             float pR,
                             float pG,
                             float pB,
                             float pA) {
	// Pixel
	pPixel->_x      =  pX;
	pPixel->_y      =  pY;
	pPixel->_z      = 0.0f;
	pPixel->_colorR = pR;
	pPixel->_colorG = pG;
	pPixel->_colorB = pB;
	pPixel->_colorA = pA;
}


void OpenGLRender::setForPrimitive(unsigned char pA, bool pResetTransform) {
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
 void OpenGLRender::blitGridLine (int pPosX1, int pPosY1, int pPosX2, int pPosY2,  unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA)
{	
	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
	// Filling pixels
    fillPixel (&_pixels[0], static_cast<float>(pPosX1), static_cast<float>(pPosY1), r, g, b, a);
    fillPixel (&_pixels[1], static_cast<float>(pPosX2), static_cast<float>(pPosY2), r, g, b, a);

	//Render primitive - No textures
	setGLClientStateToPrimitive();
    
	// Color settings
    setRainbow2d (IND_OPAQUE, 1, 0, 0, IND_FILTER_POINT, pR, pG, pB, pA, 0, 0, 0, 255, 0, 0);

	//Polygon blitting
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_LINE_STRIP, 0, 2);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in grid line blitting ", DebugApi::LogHeaderError);
	}
#endif

    //Reenable texturing
	setGLClientStateToTexturing();
}

/*
==================
Blits quad of the grid of an IND_Surface
==================
*/
void OpenGLRender::blitGridQuad    (int pAx, int pAy,
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
void OpenGLRender::blitCollisionCircle(int pPosX, int pPosY, int pRadius, float pScale,  unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA, IND_Matrix pIndWorldMatrix) {
	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);

	// Filling pixels
	float x (0.0f);
	float y (0.0f);
	int points (SIDES_PER_CIRCLE + 1);
	assert(0 != points);
	float angle (2*PI / SIDES_PER_CIRCLE);
	for (int i = 0; i <= points ; i++) {
		x = pPosX + (pRadius * cosf(angle*i));
		y = pPosY + (pRadius * sinf(angle*i));
		fillPixel (&_pixels[i], x, y, r, g, b, a);
	}
    
	//Render primitive - No textures
	setGLClientStateToPrimitive();
    
	//Transform
	setTransform2d(pIndWorldMatrix);

	// Color settings
    setRainbow2d (IND_OPAQUE, 1, 0, 0, IND_FILTER_POINT, pR, pG, pB, pA, 0, 0, 0, 255, 0, 0);

	//Polygon blitting
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_LINE_STRIP, 0, points);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in circle blitting ", DebugApi::LogHeaderError);
	}
#endif

    //Reenable texturing
	setGLClientStateToTexturing();
}


/*
==================
Blits a bounding line
==================
*/
void OpenGLRender::blitCollisionLine(int pPosX1, int pPosY1, int pPosX2, int pPosY2,  unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA, IND_Matrix pIndWorldMatrix) {

	//Transform with supplied matrix
	setTransform2d(pIndWorldMatrix);
	//Blit the line 
	blitGridLine (pPosX1, pPosY1, pPosX2, pPosY2,  pR, pG, pB, pA);
}

/** @endcond */

#endif //INDIERENDER_OPENGL
