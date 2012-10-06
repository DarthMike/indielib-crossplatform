/*****************************************************************************************
 * File: RenderPrimitive2dDirectX.cpp
 * Desc: Primitive drawing using D3D
 * TODO: Polygons blitted from a loaded xml, suffering transformations, etc...
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (javierlopezpro@gmail.com)

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

#ifdef INDIERENDER_DIRECTX
// ----- Includes -----

#include "Global.h"
#include "IND_SurfaceManager.h"
#include "IND_Surface.h"
#include "DirectXRender.h"
#include "IND_Vector2.h"
#include "IND_Vector3.h"
#include "IND_Math.h"


// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

bool DirectXRender::setAntialiasing(bool pSwitch) {
	if (!_info._antialiasing) return 0;

	if (pSwitch) {
		_info._device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
	} else {
		_info._device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
	}

	return 1;
}

void DirectXRender::blitPixel(int pX,
                              int pY,
                              BYTE pR,
                              BYTE pG,
                              BYTE pB,
                              BYTE pA) {

	// Fill the PIXEL structure
	fillPixel(&_pixels [0], pX, pY, pR, pG, pB);

	// Transformation
	setForPrimitive(pA);

	// Pixel drawing
	_info._device->DrawPrimitiveUP(D3DPT_POINTLIST, 1, &_pixels, sizeof(PIXEL));
}

void DirectXRender::blitLine(int pX1,
                             int pY1,
                             int pX2,
                             int pY2,
                             BYTE pR,
                             BYTE pG,
                             BYTE pB,
                             BYTE pA) {
	// Fill the PIXEL structure
	fillPixel(&_pixels [0], pX1, pY1, pR, pG, pB);
	fillPixel(&_pixels [1], pX2, pY2, pR, pG, pB);

	// Transformation
	setForPrimitive(pA);

	// Line blitting
	_info._device->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &_pixels, sizeof(PIXEL));
}

void DirectXRender::blitRectangle(int pX1,
                                  int pY1,
                                  int pX2,
                                  int pY2,
                                  BYTE pR,
                                  BYTE pG,
                                  BYTE pB,
                                  BYTE pA) {
	// Fill PIXEL structures
	fillPixel(&_pixels [0], pX1, pY1,      pR, pG, pB);
	fillPixel(&_pixels [1], pX2, pY1,      pR, pG, pB);
	fillPixel(&_pixels [2], pX2, pY2,      pR, pG, pB);
	fillPixel(&_pixels [3], pX1, pY2,      pR, pG, pB);
	fillPixel(&_pixels [4], pX1, pY1,      pR, pG, pB);

	// Transformation
	setForPrimitive(pA);

	// Rectangle blitting
	_info._device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &_pixels, sizeof(PIXEL));
}

void DirectXRender::blitFillRectangle(int pX1,
                                      int pY1,
                                      int pX2,
                                      int pY2,
                                      BYTE pR,
                                      BYTE pG,
                                      BYTE pB,
                                      BYTE pA) {
	// Fill PIXEL structures
	fillPixel(&_pixels [0], pX2, pY1,      pR, pG, pB);
	fillPixel(&_pixels [1], pX2, pY2,      pR, pG, pB);
	fillPixel(&_pixels [2], pX1, pY1,      pR, pG, pB);
	fillPixel(&_pixels [3], pX1, pY2,      pR, pG, pB);

	// Transformation
	//setTransform2d(0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0);

	// Color transformation
	setForPrimitive(pA);

	// Rectangle blitting
	_info._device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &_pixels, sizeof(PIXEL));
}

void DirectXRender::blitTriangleList(IND_Point *pTrianglePoints,
                                     int pNumPoints,
                                     BYTE pR,
                                     BYTE pG,
                                     BYTE pB,
                                     BYTE pA) {
	//TODO: CHECK MAX POLYGONS PER CALL...  mD3dcap.MaxPrimitiveCount
	if (pNumPoints < 3)
		return;

	//LOOP - Fill pixels structure
	for (int i = 0; i < pNumPoints; i++) {
		fillPixel(&_pixels[i], pTrianglePoints[i].x, pTrianglePoints[i].y, pR, pG, pB);
	}//LOOP END

	//Transformation
	setForPrimitive(pA);

	//Blitting
	_info._device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, pNumPoints - 2, &_pixels, sizeof(PIXEL));
}

void DirectXRender::blitColoredTriangle(int pX1,
                                        int pY1,
                                        int pX2,
                                        int pY2,
                                        int pX3,
                                        int pY3,
                                        BYTE pR1, BYTE pG1, BYTE pB1,
                                        BYTE pR2, BYTE pG2, BYTE pB2,
                                        BYTE pR3, BYTE pG3, BYTE pB3,
                                        BYTE pA) {
	// Fill PIXEL structures
	fillPixel(&_pixels [0], pX1, pY1, pR1, pG1, pB1);
	fillPixel(&_pixels [1], pX2, pY2, pR2, pG2, pB2);
	fillPixel(&_pixels [2], pX3, pY3, pR3, pG3, pB3);

	// Transformation
	//setTransform2d(0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0);

	// Color transformation
	setForPrimitive(pA);

	// Rectangle blitting
	_info._device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &_pixels, sizeof(PIXEL));
}

bool DirectXRender::blitPoly2d(IND_Point *pPolyPoints,
                               int pNumLines,
                               BYTE pR,
                               BYTE pG,
                               BYTE pB,
                               BYTE pA) {
	if (!pPolyPoints)   return 0;
	if (pNumLines < 1)  return 0;

	// Fill PIXEL structures
	for (int i = 0; i < pNumLines + 1; i++)
		fillPixel(&_pixels [i], pPolyPoints [i].x, pPolyPoints [i].y, pR, pG, pB);

	// Transformation
	setForPrimitive(pA);

	// Polygon blitting
	_info._device->DrawPrimitiveUP(D3DPT_LINESTRIP, pNumLines, &_pixels, sizeof(PIXEL));

	return 1;
}

bool DirectXRender::blitRegularPoly(int pX,
                                    int pY,
                                    int pRadius,
                                    int pN,
                                    float pAngle,
                                    BYTE pR,
                                    BYTE pG,
                                    BYTE pB,
                                    BYTE pA) {
	int x, y, i;

	for (i = 0; i < pN; i++) {
		float c = i * 2 * (float)(PI / pN);
		x = (int)(pX + (pRadius * cos(c + D3DXToRadian(pAngle))));
		y = (int)(pY + (pRadius * sin(c + D3DXToRadian(pAngle))));

		fillPixel(&_pixels [i], x, y, pR, pG, pB);
	}

	fillPixel(&_pixels [i], pX + (int)(pRadius * cos(D3DXToRadian(pAngle))), pY + (int)(pRadius * sin(D3DXToRadian(pAngle))), pR, pG, pB);

	// Transformation
	setForPrimitive(pA);

	// Polygon blitting
	_info._device->DrawPrimitiveUP(D3DPT_LINESTRIP, i, &_pixels, sizeof(PIXEL));

	return 1;
}

/*
==================
Blits a bounding circle area
==================
*/
void DirectXRender::blitCollisionCircle(int pPosX, int pPosY, int pRadius, float pScale,  BYTE pR, BYTE pG, BYTE pB, BYTE pA, IND_Matrix pIndWorldMatrix) {
	if (pScale != 1.0f) pRadius = (int)(pRadius * pScale);

	setTransform2d(0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0);


	IND_Vector3 mP1 (static_cast<float>(pPosX),static_cast<float>(pPosY),0.0f);
	_math->transformVector3DbyMatrix4D(mP1,pIndWorldMatrix);

	// Color
	setRainbow2d(IND_OPAQUE, 1, 0, 0, IND_FILTER_POINT, pR, pG, pB, pA, 0, 0, 0, 255, 0, 0);

	// Color
	_info._device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

	// Pixel format
	_info._device->SetFVF(D3DFVF_PIXEL);

	// Blitting
	int x, y, i;
	float c = 2 * (float)(PI / SIDES_PER_CIRCLE);
	int points (SIDES_PER_CIRCLE + 1);
	assert (0 != points);
	for (i = 0; i <= points; i++) {
		
		x = (int)(mP1._x + (pRadius * cos((i * c) + D3DXToRadian(0))));
		y = (int)(mP1._y + (pRadius * sin((i* c) + D3DXToRadian(0))));

		fillPixel(&_pixels [i], x, y, pR, pG, pB);
	}

	fillPixel(&_pixels [i], (int) mP1._x + (int)(pRadius * cos(D3DXToRadian(0))), (int) mP1._y + (int)(pRadius * sin(D3DXToRadian(0))), pR, pG, pB);

	// Blitting circle
	_info._device->DrawPrimitiveUP(D3DPT_LINESTRIP, points, &_pixels, sizeof(PIXEL));
}


/*
==================
Blits a bounding line
==================
*/
void DirectXRender::blitCollisionLine(int pPosX1, int pPosY1, int pPosX2, int pPosY2,  BYTE pR, BYTE pG, BYTE pB, BYTE pA, IND_Matrix pIndWorldMatrix) {
	setTransform2d(0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0);

	IND_Vector3 mP1 (static_cast<float>(pPosX1),static_cast<float>(pPosY1),0.0f);
	IND_Vector3 mP2 (static_cast<float>(pPosX2),static_cast<float>(pPosY2),0.0f);
	_math->transformVector3DbyMatrix4D(mP1,pIndWorldMatrix);
	_math->transformVector3DbyMatrix4D(mP2,pIndWorldMatrix);

	// Color
	setRainbow2d(IND_OPAQUE, 1, 0, 0, IND_FILTER_POINT, pR, pG, pB, pA, 0, 0, 0, 255, 0, 0);

	// Color
	_info._device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

	// Pixel format
	_info._device->SetFVF(D3DFVF_PIXEL);

	// Filling pixels
	fillPixel(&_pixels [0], (int) mP1._x, (int) mP1._y, pR, pG, pB);
	fillPixel(&_pixels [1], (int) mP2._x, (int) mP2._y, pR, pG, pB);

	// Blitting line
	_info._device->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &_pixels, sizeof(PIXEL));
}


/*
==================
Blits a bounding line
==================
*/
void DirectXRender::BlitGridLine(int pPosX1, int pPosY1, int pPosX2, int pPosY2,  BYTE pR, BYTE pG, BYTE pB, BYTE pA, D3DXMATRIX pWorldMatrix) {
	setTransform2d(0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0);

	// Untransformed Points
	D3DXVECTOR2 mP1Untransformed((float) pPosX1, (float) pPosY1);
	D3DXVECTOR2 mP2Untransformed((float) pPosX2, (float) pPosY2);

	D3DXVECTOR4 mP1, mP2;
	D3DXVec2Transform(&mP1, &mP1Untransformed, &pWorldMatrix);
	D3DXVec2Transform(&mP2, &mP2Untransformed, &pWorldMatrix);

	// Color
	setRainbow2d(IND_OPAQUE, 1, 0, 0, IND_FILTER_POINT, pR, pG, pB, pA, 0, 0, 0, 255, 0, 0);

	// Color
	_info._device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

	// Pixel format
	_info._device->SetFVF(D3DFVF_PIXEL);

	// Filling pixels
	fillPixel(&_pixels [0], (int) mP1.x, (int) mP1.y, pR, pG, pB);
	fillPixel(&_pixels [1], (int) mP2.x, (int) mP2.y, pR, pG, pB);

	// Blitting line
	_info._device->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &_pixels, sizeof(PIXEL));
}


/*
==================
Blits quad of the grid of an IND_Surface
==================
*/
void DirectXRender::BlitGridQuad(int pAx, int pAy,
                                 int pBx, int pBy,
                                 int pCx, int pCy,
                                 int pDx, int pDy,
                                 BYTE pR, BYTE pG, BYTE pB, BYTE pA,
                                 D3DXMATRIX pWorldMatrix) {
	BlitGridLine(pAx, pAy, pBx, pBy, pR, pG, pB, pA, pWorldMatrix);
	BlitGridLine(pBx, pBy, pDx, pDy, pR, pG, pB, pA, pWorldMatrix);
	BlitGridLine(pDx, pDy, pCx, pCy, pR, pG, pB, pA, pWorldMatrix);
	BlitGridLine(pCx, pCy, pAx, pAy, pR, pG, pB, pA, pWorldMatrix);
}



// --------------------------------------------------------------------------------
//							         Private methods
// --------------------------------------------------------------------------------

/*
==================
Fill PIXEL structures
==================
*/
void DirectXRender::fillPixel(PIXEL *pPixel,
                              int pX,
                              int pY,
                              BYTE pR,
                              BYTE pG,
                              BYTE pB) {
	// Pixel
	pPixel->_x      = (float) pX;
	pPixel->_y      = (float) pY;
	pPixel->_z      = (float) 0.0f;
	pPixel->_color  = D3DCOLOR_RGBA(pR, pG, pB, 255);
}

/*
==================
Transformations before drawing a primitive
==================
*/
void DirectXRender::setForPrimitive(BYTE pA) {
	// Transformation
	setTransform2d(0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0);

	// Antialiasing
	//if (_antialiasing)
	//{
	//  exit (0);
	//SetRainbow (IND_ALPHA, 255, 255, 255, pA, 0, 0, 0, 255, 0, 0);
	//}
	//else
	//{

	//IF - Totally opaque
	if (pA == 255) {
		setRainbow2d(IND_OPAQUE, 1, 0, 0, IND_FILTER_POINT, 255, 255, 255, pA, 0, 0, 0, 255, 0, 0);
	} else { // ELSE - Some transparency
		setRainbow2d(IND_ALPHA, 1, 0, 0, IND_FILTER_POINT, 255, 255, 255, pA, 0, 0, 0, 255, 0, 0);
	}
	//}

	// Disable color OP
	_info._device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);

	// Pixel format
	_info._device->SetFVF(D3DFVF_PIXEL);
}

#endif //INDIERENDER_DIRECTX
