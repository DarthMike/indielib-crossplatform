/*****************************************************************************************
 * File: RenderPrimitive2dOpenGL.cpp
 * Desc: Primitive drawing using OpenGL
 * TODO: Polygons blitted from a loaded xml, suffering transformations, etc...
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
#include "IND_SurfaceManager.h"
#include "OpenGLRender.h"


// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

/*
==================
TODO: CHECK THE USE OF THIS METHOD IN THE FUTURE, CURRENTLY IS DEPRECATED. ¿LET IT BE UNTIL HAVING PIXEL SHADER SUPPORT?

\b Parameters:

\b pSwitch       Activates or deactivates the antialiasing. (true = antialiasing on, false = antialiasing of)

Operation:

This method activates or deativates the antialiasing when drawing primitives. It doesn't affect
to other graphical objects, only to primitives.

This function will return 1 if the antialiasing is activated or deactivated correctly and 0 if the
graphic card of the user doesn't support this feature.
==================
*/
bool OpenGLRender::setAntialiasing(bool pSwitch) {
//TODO

	return 1;
}

/*!
\defgroup Primitives Bliting Primitives
\ingroup Advances
With these methods you can directly blit to the screen primitives using OpenGLRender class. Remember that you can also use IND_Entity2d with primitives joined to this object, in order to
draw primitives.
*/
/*@{*/

/*!
\b Parameters:

\arg \b pX, \b pY               Position in the screen
\arg \b pR, \b pG, \b pB        R, G, B components of the color
\arg \b pA                      Level of transparency. (255 = completly opaque)

\b Operation:

This function draws a pixel into the screen. This is a really slow method when the number of pixels is
big.

This method is equivalent to use a combination of these methods:
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setLine()
- IND_Entity2d::setTint()
- IND_Entity2d::setTransparency()
*/
void OpenGLRender::blitPixel(int pX,
                             int pY,
                             BYTE pR,
                             BYTE pG,
                             BYTE pB,
                             BYTE pA) {
	
	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
	// Fill the PIXEL structure
	fillPixel (&_pixels [0], static_cast<float>(pX), static_cast<float>(pY), r, g, b, a);
	
	//Render primitive - No textures
	glDisable(GL_TEXTURE_2D);

	// Color and transformation
	setForPrimitive (pA, true);

	// Pixel drawing
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_POINTS, 0,1);	

	
#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in pixel blitting ", 2);
	}
#endif	

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	mvTransformResetState();  //needed not to modify next rendering call transforms! (mvTransformPresetState()) is called inside the transform setting methods
}


/*!
\b Parameters:

\arg \b pX1, \b pY1             Origin point
\arg \b pX2, pY2                Destiny point
\arg \b pR, \b pG, \b pB        R, G, B components of the color
\arg \b pA                      Level of transparency. (255 = completly opaque)

\b Operation:

This function draws a line into the screen

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setLine()
- IND_Entity2d::setTint()
- IND_Entity2d::setTransparency()
*/
void OpenGLRender::blitLine(int pX1,
                            int pY1,
                            int pX2,
                            int pY2,
                            BYTE pR,
                            BYTE pG,
                            BYTE pB,
                            BYTE pA) {
	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
	//Fill the PIXEL structure
	fillPixel(&_pixels[0], static_cast<float>(pX1), static_cast<float>(pY1), r, g, b, a);
	fillPixel(&_pixels[1], static_cast<float>(pX2), static_cast<float>(pY2), r, g, b, a);

	//Render primitive - No textures
	glDisable(GL_TEXTURE_2D);

	// Color and transformation
	setForPrimitive(pA,true);

	//Line blitting
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_LINES, 0, 2);

	
#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in Line blitting ", 2);
	}
#endif

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	mvTransformResetState();  //needed not to modify next rendering call transforms! (mvTransformPresetState()) is called inside the transform setting methods
}


/*!
\b Parameters:

\arg \b pX1, \b pY1             Upper left corner of the rectangle
\arg \b pX2, \b pY2             Lower right corner of the rectangle
\arg \b pR, \b pG, \b pB        R, G, B components of the color
\arg \b pA                      Level of transparency. (255 = completly opaque)

\b Operation:

This function draws a rectangle into the screen

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setRectangle()
- IND_Entity2d::setTint()
- IND_Entity2d::setTransparency()
*/
void OpenGLRender::blitRectangle(int pX1,
                                 int pY1,
                                 int pX2,
                                 int pY2,
                                 BYTE pR,
                                 BYTE pG,
                                 BYTE pB,
                                 BYTE pA) {
	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
 	
	// Fill PIXEL structures
	fillPixel (&_pixels [0], static_cast<float>(pX1),static_cast<float>(pY1),      r, g, b,a);
	fillPixel (&_pixels [1], static_cast<float>(pX2), static_cast<float>(pY1),      r, g, b,a);
	fillPixel (&_pixels [2], static_cast<float>(pX2), static_cast<float>(pY2),      r, g, b,a);
	fillPixel (&_pixels [3], static_cast<float>(pX1), static_cast<float>(pY2),      r, g, b,a);
	fillPixel (&_pixels [4], static_cast<float>(pX1), static_cast<float>(pY1),      r, g, b,a);
	
	//Render primitive - No textures
	glDisable(GL_TEXTURE_2D);

	// Color and transformation
	setForPrimitive (pA,true);

	//rectangle blitting
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_LINE_STRIP, 0, 5);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in rectangle blitting ", 2);
	}
#endif

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	mvTransformResetState();  //needed not to modify next rendering call transforms! (mvTransformPresetState()) is called inside the transform setting methods
}


/*!
\b Parameters:

\arg \b pX1, \b pY1                 Upper left corner of the rectangle
\arg \b pX2, \b pY2                 Lower right corner of the rectangle
\arg \b pR, \b pG, \b pB            R, G, B components of the color
\arg \b pA                          Level of transparency. (255 = completly opaque)

\b Operation:

This function draws a rectangle filled with a color into the screen. The A component is the
transparency level (255 = complety opaque).

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setRectangle()
- IND_Entity2d::setTint()
- IND_Entity2d::setTransparency()
*/
void OpenGLRender::blitFillRectangle(int pX1,
                                     int pY1,
                                     int pX2,
                                     int pY2,
                                     BYTE pR,
                                     BYTE pG,
                                     BYTE pB,
                                     BYTE pA) {
	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
 	
	// Fill PIXEL structures
	fillPixel (&_pixels [0], static_cast<float>(pX1),static_cast<float>(pY1),      r, g, b,a);
	fillPixel (&_pixels [1], static_cast<float>(pX2), static_cast<float>(pY1),      r, g, b,a);
	fillPixel (&_pixels [2], static_cast<float>(pX1), static_cast<float>(pY2),      r, g, b,a);
	fillPixel (&_pixels [3], static_cast<float>(pX2), static_cast<float>(pY2),      r, g, b,a);

	//Render primitive - No textures
	glDisable(GL_TEXTURE_2D);

	// Color and transformation
	setForPrimitive (pA,true);

	//rectangle blitting
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in rectangle blitting ", 2);
	}
#endif

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	mvTransformResetState();  //needed not to modify next rendering call transforms! (mvTransformPresetState()) is called inside the transform setting methods
}

/*!
\b Parameters:

\arg \b pTrianglePoints             Triangle Points allocated array
\arg \b pNumPoints                  Number of points passed (numtriangles =  pNumPoints - 2)
\arg \b pR, \b pG, \b pB            R, G, B components of the color in outer vertexs
\arg \b pA                          Level of transparency. (255 = completly opaque)

\b Operation:
This function draws a triangle list in the form of a Triangle strip (most efficient)
Triangle strip are a set of triangles which share two vertexes each, thus reducing the 
number of vertexs you send to the graphics card per call (common vertexes are sent one time only)
The A parameter is transparency (255 = complety opaque).
*/
void OpenGLRender::blitTriangleList(IND_Point *pTrianglePoints,
                                    int pNumPoints,
                                    BYTE pR,
                                    BYTE pG,
                                    BYTE pB,
                                    BYTE pA) {

	//TODO: CHECK MAX POLYGONS PER CALL...
	if (pNumPoints < 3)
		return;

    float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);

	//LOOP - Fill pixels structure
	for (int i = 0; i < pNumPoints; i++) {
		fillPixel(&_pixels[i], static_cast<float>(pTrianglePoints[i].x), static_cast<float>(pTrianglePoints[i].y), r, g, b, a);
	}//LOOP END

	//Render primitive - No textures
	glDisable(GL_TEXTURE_2D);

	// Color and transformation
	setForPrimitive(pA,true);

	//Triangle strip blitting
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, pNumPoints);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in triangle list blitting ", 2);
	}
#endif
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	mvTransformResetState();  //needed not to modify next rendering call transforms! (mvTransformPresetState()) is called inside the transform setting methods
}

/********************************************************************************/

/*!
\b Parameters:

\arg \b pX1, \b pY1                     Triangle corner #1
\arg \b pX2, \b pY2                     Triangle corner #2
\arg \b pX3, \b pY3                     Triangle corner #3
\arg \b pR1, \b pG1, \b pB1             R, G, B components of the color for corner #1
\arg \b pR2, \b pG2, \b pB2             R, G, B components of the color for corner #2
\arg \b pR3, \b pG3, \b pB3             R, G, B components of the color for corner #3
\arg \b pZ                              Z depth of coords in all triangle
\arg \b pA                              Level of transparency. (255 = completly opaque)

\b Operation:

This function draws a triangle filled with a color given in three corners. The A component is the
transparency level (255 = complety opaque).
*/

void OpenGLRender::blitColoredTriangle(int pX1,
                                       int pY1,
                                       int pX2,
                                       int pY2,
                                       int pX3,
                                       int pY3,
                                       BYTE pR1, BYTE pG1, BYTE pB1,
                                       BYTE pR2, BYTE pG2, BYTE pB2,
                                       BYTE pR3, BYTE pG3, BYTE pB3,
                                       BYTE pA) {
	
    float r1(static_cast<float>(pR1) / 255.0f), g1(static_cast<float>(pG1) / 255.0f), b1(static_cast<float>(pB1) / 255.0f);
    float r2(static_cast<float>(pR2) / 255.0f), g2(static_cast<float>(pG2) / 255.0f), b2(static_cast<float>(pB2) / 255.0f);
    float r3(static_cast<float>(pR3) / 255.0f), g3(static_cast<float>(pG3) / 255.0f), b3(static_cast<float>(pB3) / 255.0f);
    float a(static_cast<float>(pA) / 255.0f);
	
    // Fill PIXEL structures
	fillPixel (&_pixels[0], static_cast<float>(pX1), static_cast<float>(pY1), r1, g1, b1, a);
	fillPixel (&_pixels[1], static_cast<float>(pX2), static_cast<float>(pY2), r2, g2, b2, a);
	fillPixel (&_pixels[2], static_cast<float>(pX3), static_cast<float>(pY3), r3, g3, b3, a);

	//Render primitive - No textures
	glDisable(GL_TEXTURE_2D);

	// Color and transformation
	setForPrimitive(pA,true);

	//Single Colored Triangle blitting
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_TRIANGLES, 0, 3);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in colored triangle blitting ", 2);
	}
#endif
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	mvTransformResetState();  //needed not to modify next rendering call transforms! (mvTransformPresetState()) is called inside the transform setting methods
}


/*!
\b Parameters:

\arg \b pPixel                      Pointer to a points array ::IND_Point. Example: ::IND_Point mPoly3 [ ] = { {60, 10},  {20, 15},  {50, 90},  {170, 190} } => Sets 3 points (each one with x and y coordinates).
\arg \b pNumLines                   Number of edges to draw
\arg \b pR, \b pG, \b pB            R, G, B components of the color
\arg \b pA                          Level of transparency. (255 = completly opaque)

\b Operation:

This function draws a 2d poly

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setPolyPoints()
- IND_Entity2d::setNumSides()
- IND_Entity2d::setTint()
- IND_Entity2d::setTransparency()
*/
bool OpenGLRender::blitPoly2d(IND_Point *pPolyPoints,
                              int pNumLines,
                              BYTE pR,
                              BYTE pG,
                              BYTE pB,
                              BYTE pA) {

	if (!pPolyPoints)   return 0;
	if (pNumLines < 1)  return 0;

    float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);

	// Fill PIXEL structures
    for (int i = 0; i < pNumLines + 1; i++){
	    fillPixel (&_pixels [i], static_cast<float>(pPolyPoints [i].x), static_cast<float>(pPolyPoints [i].y), r, g, b,a);
    }

	//Render primitive - No textures
	glDisable(GL_TEXTURE_2D);

	// Color and transformation
	setForPrimitive(pA,true);

	// Polygon blitting
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_LINE_STRIP, 0, pNumLines+1);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in triangle poly2d blitting ", 2);
	}
#endif
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	mvTransformResetState();  //needed not to modify next rendering call transforms! (mvTransformPresetState()) is called inside the transform setting methods

	return 1;
}


/*!
\b Parameters:

\arg \b pX, \b pY                   Position in the screen
\arg \b pRadius                     Radius
\arg \b pN                          Number of sides
\arg \b pAngle                      Angle in degrees (if you change this parameter the polygon
                                    will rotate)
\arg \b pR, \b pG, \b pB            R, G, B components of the color
\arg \b pA                          Level of transparency. (255 = completly opaque)

\b Operation:

This function draws 2d regunr poly of n sides. If you need to draw circles you can use this method
using 30 or more sides.

This method is equivalent to use a combination of these methods:
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setRadius()
- IND_Entity2d::setPosition()
- IND_Entity2d::setTint()
- IND_Entity2d::setTransparency()
*/
bool OpenGLRender::blitRegularPoly(int pX,
                                   int pY,
                                   int pRadius,
                                   int pN,
                                   float pAngle,
                                   BYTE pR,
                                   BYTE pG,
                                   BYTE pB,
                                   BYTE pA) {
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
	glDisable(GL_TEXTURE_2D);

	// Color and transformation
	setForPrimitive(pA,true);

	//Blitting
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//Polygon blitting
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_LINE_STRIP, 0, pN+1);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in triangle list blitting ", 2);
	}
#endif
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	mvTransformResetState();  //needed not to modify next rendering call transforms! (mvTransformPresetState()) is called inside the transform setting methods

	return 1;
}
/*@}*/

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


void OpenGLRender::setForPrimitive(BYTE pA, bool pResetTransform) {
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
 void OpenGLRender::BlitGridLine (int pPosX1, int pPosY1, int pPosX2, int pPosY2,  BYTE pR, BYTE pG, BYTE pB, BYTE pA)
{	
	float r(static_cast<float>(pR) / 255.0f), g(static_cast<float>(pG) / 255.0f), b(static_cast<float>(pB) / 255.0f), a(static_cast<float>(pA) / 255.0f);
	// Filling pixels
    fillPixel (&_pixels[0], pPosX1, pPosY1, r, g, b, a);
    fillPixel (&_pixels[1], pPosX2, pPosY2, r, g, b, a);

	//Render primitive - No textures
	glDisable(GL_TEXTURE_2D);
	// Color settings
    setRainbow2d (IND_OPAQUE, 1, 0, 0, IND_FILTER_POINT, pR, pG, pB, pA, 0, 0, 0, 255, 0, 0);

	//Blitting
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//Polygon blitting
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_LINE_STRIP, 0, 2);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in grid line blitting ", 2);
	}
#endif
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);	

	//NOTE: Not called mvTransformResetState(); as it is done in BlitGrid public API method
}

/*
==================
Blits quad of the grid of an IND_Surface
==================
*/
void OpenGLRender::BlitGridQuad    (int pAx, int pAy,
                                   int pBx, int pBy,
                                   int pCx, int pCy,
                                   int pDx, int pDy,
                                   BYTE pR, BYTE pG, BYTE pB, BYTE pA)
{
	BlitGridLine (pAx, pAy, pBx, pBy, pR, pG, pB, pA);
	BlitGridLine (pBx, pBy, pDx, pDy, pR, pG, pB, pA);
	BlitGridLine (pDx, pDy, pCx, pCy, pR, pG, pB, pA);
	BlitGridLine (pCx, pCy, pAx, pAy, pR, pG, pB, pA);

	//NOTE: Not called mvTransformResetState(); as it is done in BlitGrid public API method
}

/*
==================
Blits a bounding circle area
==================
*/
void OpenGLRender::blitCollisionCircle(int pPosX, int pPosY, int pRadius, float pScale,  BYTE pR, BYTE pG, BYTE pB, BYTE pA, IND_Matrix pIndWorldMatrix) {
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
	glDisable(GL_TEXTURE_2D);
	//Transform
	setTransform2d(pIndWorldMatrix);

	// Color settings
    setRainbow2d (IND_OPAQUE, 1, 0, 0, IND_FILTER_POINT, pR, pG, pB, pA, 0, 0, 0, 255, 0, 0);

	//Blitting
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//Polygon blitting
	glVertexPointer(3, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._x);
	glColorPointer(4, GL_FLOAT, sizeof(PIXEL), &_pixels[0]._colorR);
	glDrawArrays(GL_LINE_STRIP, 0, points);

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
		g_debug->header("OpenGL error in circle blitting ", 2);
	}
#endif
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);	

	//Reset matrix stack state
	mvTransformResetState();  //needed not to modify next rendering call transforms! (mvTransformPresetState()) is called inside the transform setting methods
}


/*
==================
Blits a bounding line
==================
*/
void OpenGLRender::blitCollisionLine(int pPosX1, int pPosY1, int pPosX2, int pPosY2,  BYTE pR, BYTE pG, BYTE pB, BYTE pA, IND_Matrix pIndWorldMatrix) {

	//Transform with supplied matrix
	setTransform2d(pIndWorldMatrix);
	//Blit the line 
	BlitGridLine (pPosX1, pPosY1, pPosX2, pPosY2,  pR, pG, pB, pA);
	//Leave matrix state the same
	mvTransformResetState();  //needed not to modify next rendering call transforms! (mvTransformPresetState()) is called inside the transform setting methods
}

#endif //INDIERENDER_OPENGL
