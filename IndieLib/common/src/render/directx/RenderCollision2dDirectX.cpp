/*****************************************************************************************
 * File: RenderCollision2dDirectX.cpp
 * Desc: Collision between 2d objects
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
MODIFIED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)

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


// --------------------------------------------------------------------------------
//							       Private methods
// --------------------------------------------------------------------------------

/*
==================
Check collision between two circles that are not transformed
==================
*/
bool   DirectXRender::isCircleToCircleCollision(BOUNDING_COLLISION *pB1, IND_Matrix pMat1, float pScale1,
        BOUNDING_COLLISION *pB2, IND_Matrix pMat2, float pScale2) {
	D3DXMATRIX mMat1, mMat2;
	d3DMatrixToIndMatrix(pMat1, &mMat1);
	d3DMatrixToIndMatrix(pMat2, &mMat2);

	// Untransformed points

	// Circle 1
	D3DXVECTOR2 mCenter1((float) pB1->_posX, (float) pB1->_posY);
	int mRadius1 = (int)(pB1->_radius * pScale1);

	// Circle 1
	D3DXVECTOR2 mCenter2((float) pB2->_posX, (float) pB2->_posY);
	int mRadius2 = (int)(pB2->_radius * pScale2);

	// Transformations
	D3DXVECTOR4 mCenter1t, mCenter2t;

	D3DXVec2Transform(&mCenter1t, &mCenter1, &mMat1);
	D3DXVec2Transform(&mCenter2t, &mCenter2, &mMat2);

	// Center 1 vector final
	D3DXVECTOR2 mCenter1f(mCenter1t);

	// Center 2 vector final
	D3DXVECTOR2 mCenter2f(mCenter2t);

	if (isCircleToCircleCollision(mCenter1f, mRadius1, mCenter2f, mRadius2))
		return 1;

	return 0;
}


/*
==================
Check collision between two triangles that are not transformed
==================
*/
bool   DirectXRender::isTriangleToTriangleCollision(BOUNDING_COLLISION *pB1, IND_Matrix pMat1,
        BOUNDING_COLLISION *pB2, IND_Matrix pMat2) {
	D3DXMATRIX mMat1, mMat2;
	d3DMatrixToIndMatrix(pMat1, &mMat1);
	d3DMatrixToIndMatrix(pMat2, &mMat2);

	// Untransformed points

	// Triangle 1
	D3DXVECTOR2 mA1((float) pB1->_ax, (float) pB1->_ay);
	D3DXVECTOR2 mB1((float) pB1->_bx, (float) pB1->_by);
	D3DXVECTOR2 mC1((float) pB1->_cx, (float) pB1->_cy);

	// Triangle 2
	D3DXVECTOR2 mA2((float) pB2->_ax, (float) pB2->_ay);
	D3DXVECTOR2 mB2((float) pB2->_bx, (float) pB2->_by);
	D3DXVECTOR2 mC2((float) pB2->_cx, (float) pB2->_cy);

	// Transformations
	D3DXVECTOR4 mA1t, mB1t, mC1t, mA2t, mB2t, mC2t;

	D3DXVec2Transform(&mA1t, &mA1, &mMat1);
	D3DXVec2Transform(&mB1t, &mB1, &mMat1);
	D3DXVec2Transform(&mC1t, &mC1, &mMat1);

	D3DXVec2Transform(&mA2t, &mA2, &mMat2);
	D3DXVec2Transform(&mB2t, &mB2, &mMat2);
	D3DXVec2Transform(&mC2t, &mC2, &mMat2);

	// Triangle 1 in VECTOR2
	D3DXVECTOR2 mA1f(mA1t);
	D3DXVECTOR2 mB1f(mB1t);
	D3DXVECTOR2 mC1f(mC1t);

	// Triangle 2 in VECTOR2
	D3DXVECTOR2 mA2f(mA2t);
	D3DXVECTOR2 mB2f(mB2t);
	D3DXVECTOR2 mC2f(mC2t);

	if (isTriangleToTriangleCollision(mA1f, mB1f, mC1f, mA2f, mB2f, mC2f))
		return 1;

	return 0;
}


/*
==================
Check collision between a circle an a triangle that are not transformed
==================
*/
bool DirectXRender::isCircleToTriangleCollision(BOUNDING_COLLISION *pB1, IND_Matrix pMat1, float pScale,
        BOUNDING_COLLISION *pB2, IND_Matrix pMat2) {
	D3DXMATRIX mMat1, mMat2;
	d3DMatrixToIndMatrix(pMat1, &mMat1);
	d3DMatrixToIndMatrix(pMat2, &mMat2);

	// ----- Circle -----

	// Circle 1
	D3DXVECTOR2 mCenter((float) pB1->_posX, (float) pB1->_posY);
	int _radius = (int)(pB1->_radius * pScale);

	// Transformations
	D3DXVECTOR4 mCenterT;

	D3DXVec2Transform(&mCenterT, &mCenter, &mMat1);

	// Center 1 vector final
	D3DXVECTOR2 mCenterf(mCenterT);

	// ----- Triangle -----

	// Triangle
	D3DXVECTOR2 _a((float) pB2->_ax, (float) pB2->_ay);
	D3DXVECTOR2 _b((float) pB2->_bx, (float) pB2->_by);
	D3DXVECTOR2 mC((float) pB2->_cx, (float) pB2->_cy);

	// Transformations
	D3DXVECTOR4 mAt, mBt, mCt;

	D3DXVec2Transform(&mAt, &_a, &mMat2);
	D3DXVec2Transform(&mBt, &_b, &mMat2);
	D3DXVec2Transform(&mCt, &mC, &mMat2);

	// Triangle in VECTOR2
	D3DXVECTOR2 mAf(mAt);
	D3DXVECTOR2 mBf(mBt);
	D3DXVECTOR2 mCf(mCt);

	if (isCircleToTriangleCollision(mCenterf, _radius, mAf, mBf, mCf))
		return 1;

	return 0;
}


/*
==================
Check collision between two triangles (a1, b1, c1) and (a2, b2, c2)
- First checks if each of the vertices of either triange is within inside the other triangle.
- After that it checks for intersections between the triangle segments.
==================
*/
bool DirectXRender::isTriangleToTriangleCollision(D3DXVECTOR2 a1,
        D3DXVECTOR2 b1,
        D3DXVECTOR2 c1,
        D3DXVECTOR2 a2,
        D3DXVECTOR2 b2,
        D3DXVECTOR2 c2) {
	// Check if any vertex of triange 1 is inside triange 2
	if (IsPointInsideTriangle(a1, a2, b2, c2)) return 1;
	if (IsPointInsideTriangle(b1, a2, b2, c2)) return 1;
	if (IsPointInsideTriangle(c1, a2, b2, c2)) return 1;

	// Check if any vertex of triange 2 is inside triange 1
	if (IsPointInsideTriangle(a2, a1, b1, c1)) return 1;
	if (IsPointInsideTriangle(c2, a1, b1, c1)) return 1;
	if (IsPointInsideTriangle(c2, a1, b1, c1)) return 1;

	// Checks if any segment of triange 1 intersects with any segment of triange 2
	// Segment (a1 - b1)
	if (IsSegmentIntersection(a1, b1, a2, b2)) return 1;
	if (IsSegmentIntersection(a1, b1, b2, c2)) return 1;
	if (IsSegmentIntersection(a1, b1, c2, a2)) return 1;

	// Segment (b1 - c1)
	if (IsSegmentIntersection(b1, c1, a2, b2)) return 1;
	if (IsSegmentIntersection(b1, c1, b2, c2)) return 1;
	if (IsSegmentIntersection(b1, c1, c2, a2)) return 1;

	// Segment (c1 - a1)
	if (IsSegmentIntersection(c1, a1, a2, b2)) return 1;
	if (IsSegmentIntersection(c1, a1, b2, c2)) return 1;
	if (IsSegmentIntersection(c1, a1, c2, a2)) return 1;

	return 0;
}


/*
==================
Check collision between two circles
==================
*/
bool DirectXRender::isCircleToCircleCollision(D3DXVECTOR2 pP1, int pRadius1,
        D3DXVECTOR2 pP2, int pRadius2) {
	// h^2 = x^2 + y^2 Pythagoras :D

	// x^2
	double mDeltaXSquared = pP1.x - pP2.x;
	mDeltaXSquared *= mDeltaXSquared;

	// y^2
	double mDeltaYSquared = pP1.y - pP2.y;
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
bool DirectXRender::isCircleToTriangleCollision(D3DXVECTOR2 pPCenter, int pRadius,
        D3DXVECTOR2 pA2, D3DXVECTOR2 pB2, D3DXVECTOR2 pC2) {
	// Check if circle center inside the triangle
	if (IsPointInsideTriangle(pPCenter, pA2, pB2, pC2)) return 1;

	// Check the distance of the circle center to the 3 triangle segments
	if (PointToLineDistance(pA2, pB2, pPCenter, 1) < pRadius) return 1;
	if (PointToLineDistance(pB2, pC2, pPCenter, 1) < pRadius) return 1;
	if (PointToLineDistance(pC2, pA2, pPCenter, 1) < pRadius) return 1;

	return 0;
}


/*
==================
Compute the dot product AB . BC
==================
*/
int DirectXRender::Dot3(D3DXVECTOR2 pA, D3DXVECTOR2 pB, D3DXVECTOR2 pC) {
	D3DXVECTOR2 mAB;
	D3DXVECTOR2 mBC;

	mAB.x = pB.x - pA.x;
	mAB.y = pB.y - pA.y;
	mBC.x = pC.x - pB.x;
	mBC.y = pC.y - pB.y;
	return (int)((mAB.x * mBC.x) + (mAB.y * mBC.y));
}


/*
==================
Compute the cross product AB x AC
==================
*/
int DirectXRender::Cross3(D3DXVECTOR2 pA, D3DXVECTOR2 pB, D3DXVECTOR2 pC) {
	D3DXVECTOR2 mAB;
	D3DXVECTOR2 mAC;
	mAB.x = pB.x - pA.x;
	mAB.y = pB.y - pA.y;
	mAC.x = pC.x - pA.x;
	mAC.y = pC.y - pA.y;

	return (int)((mAB.x * mAC.y) - (mAB.y * mAC.x));
}


/*
==================
Compute the distance from A to B
==================
*/
double DirectXRender::Distance(D3DXVECTOR2 pA, D3DXVECTOR2 pB) {
	float mD1 = (pA.x - pB.x);
	float mD2 = (pA.y - pB.y);
	return sqrt((mD1 * mD1) + (mD2 * mD2));
}


/*
==================
Compute the distance from AB to C
if isSegment is true, AB is a segment, not a line.
==================
*/
double DirectXRender::PointToLineDistance(D3DXVECTOR2 pA, D3DXVECTOR2 pB, D3DXVECTOR2 pC, bool pIsSegment) {
	double mDist = Cross3(pA, pB, pC) / Distance(pA, pB);

	if (pIsSegment) {
		int mDot1 = Dot3(pA, pB, pC);
		if (mDot1 > 0) return Distance(pB, pC);
		int mDot2 = Dot3(pB, pA, pC);
		if (mDot2 > 0) return Distance(pA, pC);
	}

	return abs(mDist);
}

/*
==================
Check if point p is inside the triangle with vertex a, b, c
Technique from: http://www.blackpawn.com/texts/pointinpoly/default.html
==================
*/
bool DirectXRender::IsPointInsideTriangle(D3DXVECTOR2 p,
        D3DXVECTOR2 a,
        D3DXVECTOR2 b,
        D3DXVECTOR2 c) {
	// Compute vectors
	D3DXVECTOR2 v0 = c - a;
	D3DXVECTOR2 v1 = b - a;
	D3DXVECTOR2 v2 = p - a;

	// Compute dot products
	float dot00 = D3DXVec2Dot(&v0, &v0);
	float dot01 = D3DXVec2Dot(&v0, &v1);
	float dot02 = D3DXVec2Dot(&v0, &v2);
	float dot11 = D3DXVec2Dot(&v1, &v1);
	float dot12 = D3DXVec2Dot(&v1, &v2);

	// Compute barycentric coordinates
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	return (u > 0) && (v > 0) && (u + v < 1);
}


/*
==================
Check if there is an intersection between two segments
Segment 1 => (pAx, pAy) - (pBx, pBy)
Segment 2 => (pCx, pCy) - (pDx, pDy)
==================
*/
bool DirectXRender::IsSegmentIntersection(D3DXVECTOR2 a,
        D3DXVECTOR2 b,
        D3DXVECTOR2 c,
        D3DXVECTOR2 d) {
	float mGradAB, mGradCD, mYcptAB, mYcptCD, mInterceptX, mInterceptY;

	// In order to avoid divisions by zero
	if (a.y == b.y)
		b.y += 0.0001f;

	if (a.x == b.x)
		b.x += 0.0001f;

	if (c.y == d.y)
		d.y += 0.0001f;

	if (c.x == d.x)
		d.x += 0.0001f;

	// Calculates the intersection between the two lines
	mGradAB = (a.y - b.y) / (a.x - b.x);
	mGradCD = (c.y - d.y) / (c.x - d.x);

	mYcptAB = a.y - a.x * mGradAB;
	mYcptCD = c.y - c.x * mGradCD;
	mInterceptX = (mYcptAB - mYcptCD) / (mGradCD - mGradAB);
	mInterceptY = (mYcptAB - (mGradAB * mYcptCD) / mGradCD) / (1 - mGradAB / mGradCD);

	// Checking in the intersection is inside the segment
	if (!((mInterceptX >= a.x && mInterceptX <= b.x) || (mInterceptX >= b.x && mInterceptX <= a.x)))
		return 0;

	if (!((mInterceptY >= a.y && mInterceptY <= b.y) || (mInterceptY >= b.y && mInterceptY <= a.y)))
		return 0;

	if (!((mInterceptX >= c.x && mInterceptX <= d.x) || (mInterceptX >= d.x && mInterceptX <= c.x)))
		return 0;

	if (!((mInterceptY >= c.y && mInterceptY <= d.y) || (mInterceptY >= d.y && mInterceptY <= c.y)))
		return 0;

	return 1;
}

#endif //INDIERENDER_DIRECTX
