/*****************************************************************************************
 * File: RenderCollision2dOpenGL.cpp
 * Desc: Collision between 2d objects
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
#include "IND_math.h"
#include "IND_Vector2.h"

// --------------------------------------------------------------------------------
//							       Private methods
// --------------------------------------------------------------------------------

/*
==================
Check collision between two circles that are not transformed
==================
*/
bool   OpenGLRender::isCircleToCircleCollision(BOUNDING_COLLISION *pB1, 
											   IND_Matrix pMat1, 
											   float pScale1,
											   BOUNDING_COLLISION *pB2, 
											   IND_Matrix pMat2, 
											   float pScale2) {
	// Untransformed points

	// Circle 1
	IND_Vector2 mCenter1((float) pB1->_posX, (float) pB1->_posY);
	int mRadius1 = (int)(pB1->_radius * pScale1);

	// Circle 1
	IND_Vector2 mCenter2((float) pB2->_posX, (float) pB2->_posY);
	int mRadius2 = (int)(pB2->_radius * pScale2);

	// Transform the points to it's position in world coordinates by using supplied transform
	_math.transformVector2DbyMatrix4D(mCenter1,pMat1);
	_math.transformVector2DbyMatrix4D(mCenter2,pMat2);

	if (isCircleToCircleCollision(mCenter1, mRadius1, mCenter2, mRadius2))
		return 1;

	return 0;
}


/*
==================
Check collision between two triangles that are not transformed
==================
*/
bool   OpenGLRender::isTriangleToTriangleCollision(BOUNDING_COLLISION *pB1, IND_Matrix pMat1,
												   BOUNDING_COLLISION *pB2, IND_Matrix pMat2) {
	// Untransformed points

	// Triangle 1
	IND_Vector2 mA1((float) pB1->_ax, (float) pB1->_ay);
	IND_Vector2 mB1((float) pB1->_bx, (float) pB1->_by);
	IND_Vector2 mC1((float) pB1->_cx, (float) pB1->_cy);

	// Triangle 2
	IND_Vector2 mA2((float) pB2->_ax, (float) pB2->_ay);
	IND_Vector2 mB2((float) pB2->_bx, (float) pB2->_by);
	IND_Vector2 mC2((float) pB2->_cx, (float) pB2->_cy);

	// Transform the points to it's position in world coordinates by using supplied transform
	_math.transformVector2DbyMatrix4D(mA1,pMat1);
	_math.transformVector2DbyMatrix4D(mB1,pMat1);
	_math.transformVector2DbyMatrix4D(mC1,pMat1);

	_math.transformVector2DbyMatrix4D(mA2,pMat2);
	_math.transformVector2DbyMatrix4D(mB2,pMat2);
	_math.transformVector2DbyMatrix4D(mC2,pMat2);


	if (isTriangleToTriangleCollision(mA1, mB1, mC1, mA2, mB2, mC2))
		return 1;

	return 0;
}


/*
==================
Check collision between a circle an a triangle that are not transformed
==================
*/
bool OpenGLRender::isCircleToTriangleCollision(BOUNDING_COLLISION *pB1, IND_Matrix pMat1, float pScale,
        BOUNDING_COLLISION *pB2, IND_Matrix pMat2) {

	//// ----- Circle -----

	//// Circle 1
	//IND_Vector2 mCenter((float) pB1->_posX, (float) pB1->_posY);
	//int _radius = (int)(pB1->_radius * pScale);

	//// Transformations
	//D3DXVECTOR4 mCenterT;

	//D3DXVec2Transform(&mCenterT, &mCenter, &pMat1);

	//// Center 1 vector final
	//IND_Vector2 mCenterf(mCenterT);

	//// ----- Triangle -----

	//// Triangle
	//IND_Vector2 _a((float) pB2->_ax, (float) pB2->_ay);
	//IND_Vector2 _b((float) pB2->_bx, (float) pB2->_by);
	//IND_Vector2 mC((float) pB2->_cx, (float) pB2->_cy);

	//// Transformations
	//D3DXVECTOR4 mAt, mBt, mCt;

	//D3DXVec2Transform(&mAt, &_a, &pMat2);
	//D3DXVec2Transform(&mBt, &_b, &pMat2);
	//D3DXVec2Transform(&mCt, &mC, &pMat2);

	//// Triangle in VECTOR2
	//IND_Vector2 mAf(mAt);
	//IND_Vector2 mBf(mBt);
	//IND_Vector2 mCf(mCt);

	//if (isCircleToTriangleCollision(mCenterf, _radius, mAf, mBf, mCf))
	//	return 1;

	return 0;
}


/*
==================
Check collision between two triangles (a1, b1, c1) and (a2, b2, c2)
- First checks if each of the vertices of either triange is within inside the other triangle->
- After that it checks for intersections between the triangle segments->
==================
*/
bool OpenGLRender::isTriangleToTriangleCollision(IND_Vector2 &a1,
        IND_Vector2 &b1,
        IND_Vector2 &c1,
        IND_Vector2 &a2,
        IND_Vector2 &b2,
        IND_Vector2 &c2) {
	// Check if any vertex of triange 1 is inside triange 2
	if (_math.isPointInsideTriangle(a1, a2, b2, c2)) return 1;
	if (_math.isPointInsideTriangle(b1, a2, b2, c2)) return 1;
	if (_math.isPointInsideTriangle(c1, a2, b2, c2)) return 1;

	// Check if any vertex of triange 2 is inside triange 1
	if (_math.isPointInsideTriangle(a2, a1, b1, c1)) return 1;
	if (_math.isPointInsideTriangle(c2, a1, b1, c1)) return 1;
	if (_math.isPointInsideTriangle(c2, a1, b1, c1)) return 1;

	// Checks if any segment of triange 1 intersects with any segment of triange 2
	// Segment (a1 - b1)
	if (_math.isSegmentIntersection(a1, b1, a2, b2)) return 1;
	if (_math.isSegmentIntersection(a1, b1, b2, c2)) return 1;
	if (_math.isSegmentIntersection(a1, b1, c2, a2)) return 1;

	// Segment (b1 - c1)
	if (_math.isSegmentIntersection(b1, c1, a2, b2)) return 1;
	if (_math.isSegmentIntersection(b1, c1, b2, c2)) return 1;
	if (_math.isSegmentIntersection(b1, c1, c2, a2)) return 1;

	// Segment (c1 - a1)
	if (_math.isSegmentIntersection(c1, a1, a2, b2)) return 1;
	if (_math.isSegmentIntersection(c1, a1, b2, c2)) return 1;
	if (_math.isSegmentIntersection(c1, a1, c2, a2)) return 1;

	return 0;
}


/*
==================
Check collision between two circles
==================
*/
bool OpenGLRender::isCircleToCircleCollision(IND_Vector2 &pP1, int pRadius1,
        IND_Vector2 &pP2, int pRadius2) {
	// h^2 = x^2 + y^2 Pythagoras :D

	// x^2
	double mDeltaXSquared = pP1._x - pP2._x;
	mDeltaXSquared *= mDeltaXSquared;

	// y^2
	double mDeltaYSquared = pP1._y - pP2._y;
	mDeltaYSquared *= mDeltaYSquared;

	// Adding radius^2
	double mSumRadiiSquared = pRadius1 + pRadius2;
	mSumRadiiSquared *= mSumRadiiSquared;

	// If that result is greater than  h^2, so less than x^2 + y^2
	// then there is a collision
	if (mDeltaXSquared + mDeltaYSquared <= mSumRadiiSquared)
		return 1;
	else
		return 0;
}


/*
==================
Check collision between a circle and a triangle
==================
*/
bool OpenGLRender::isCircleToTriangleCollision(IND_Vector2 &pPCenter, int pRadius,
        IND_Vector2 &pA2, IND_Vector2 &pB2, IND_Vector2 &pC2) {
	// Check if circle center inside the triangle
	if (_math.isPointInsideTriangle(pPCenter, pA2, pB2, pC2)) return 1;

	// Check the distance of the circle center to the 3 triangle segments
	if (_math.pointToLineDistance(pA2, pB2, pPCenter, 1) < pRadius) return 1;
	if (_math.pointToLineDistance(pB2, pC2, pPCenter, 1) < pRadius) return 1;
	if (_math.pointToLineDistance(pC2, pA2, pPCenter, 1) < pRadius) return 1;

	return 0;
}

#endif //INDIERENDER_OPENGL
