/*****************************************************************************************
 * File: INDh
 * Dess: Math methods
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (javierlopezpro@gmail.com)
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

#ifndef _IND_MATH_
#define _IND_MATH_


#include <stdlib.h>
#include <math.h>

#include "Defines.h"
#include "IND_Vector2.h"

// --------------------------------------------------------------------------------
//									   IND_Math
// --------------------------------------------------------------------------------

/**
 @defgroup IND_Math IND_Math
 @ingroup Math
 Encapsulates common math operations used through library
 */

/**@{*/

/**
Max macro
*/
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

/**
Min macro
*/
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

/**
 @brief Encapsulates common math operations used through the library
 
 This class is comprised by a set of static methods, and a set of object methds which need 
 and IND_Math instance created.
 
 @b Note:
 Math ttransform operations assume:
 - right-handed coordinate system
 - matrix representation by column order (changed in matrix definition)
 - vectors as columns, and post-multiplying
 
 Great site with sense on this math stuff: http://www.mindcontrol.org/~hplus/graphics/matrix-layout.html
 */
class LIB_EXP IND_Math {
public:
	// ----- Init/End -----

	IND_Math(): _ok(false)  { }
	~IND_Math()              {
		end();
	}

	bool init();
	void end();
	bool isOK();

	// ----- Public methods -----

	void randSeed(long pNum);

    /** 
     @name Miscellaneus utilities
     */
	/**@{*/
    bool isPowerOfTwo(long pN);
	/**
     Generates a random number, given two boundary values
     @param pIni Lowest possible value to generate
     @param pEnd Higher possible value to generate
	 @return A random number between the two boundaries
	*/
	inline long randnum(long pIni, long pEnd){
        if (pIni > pEnd) {
            long pTemp = pEnd;
            pEnd = pIni;
            pIni = pTemp;
        }
        
        return rand() % (pEnd - pIni + 1) + pIni;
	}

	/**
     Performs conversion from radians to degree units
     @param rads A value to convert to degrees
     @param invert Will multiply the result degrees values by -1 if this is true
     @return Value converted to degrees
     */
	template <typename type>
	inline type radiansToAngle(type rads, bool invert = false)const {
		type angle; //Angle to return

		//Inversion of angle results
		if (invert) {
			angle = (-1) * rads * static_cast<type>(180.0f / PI);
		} else {
			angle = rads * static_cast<type>(180.0f / PI);
		}
		return angle;
	}

	/**
     Performs conversion from degrees to radian units
     @param angle A value to convert to radian
     @param invert Will multiply the result radian values by -1 if this is true
     @return Value converted to radian
     */
	template <typename type>
	inline type angleToRadians(type angle, bool invert = false)const {
		type radians; //Radians to return

		//Inversion of radians result
		if (invert) {
			radians = (-1) * angle * static_cast<type>(PI / 180.0f);
		} else {
			radians = angle * static_cast<type>(PI / 180.0f);
		}

		return radians;
	}

	/**
	 * Cross-Platform version of itoa. support only 10 radix for easy use and better performance
	 * taken from http://code.google.com/p/my-itoa/  (LGPL)
	 *
	 * @param val             The int that needs to be converted
	 * @param buf             The char buffer that is going to held the result
	 * @return                The length of the result String.
	 */
	static inline int itoa(int val, char* buf) {
		const unsigned int radix = 10;

		char* p;
		unsigned int a;        //every digit
		int len;
		char* b;               //start of the digit char
		char temp;
		unsigned int u;

		p = buf;

		if (val < 0) {
		    *p++ = '-';
		    val = 0 - val;
		}
		
		u = (unsigned int)val;
		b = p;

		do {
		    a = u % radix;
		    u /= radix;
		    *p++ = (char)a + '0';
		} while (u > 0);

		len = (int)(p - buf);
		*p-- = 0;

		//swap
		do {
		    temp = *p;
		    *p = *b;
		    *b = temp;
		    --p;
		    ++b;
		} while (b < p);

		return len;
	}

	/**
	@brief Calculates a minimum and maximum value of 4 values.

	Given 4 values, it calculates and returns the maximum and minimum values

	@param p1 Value to compare
	@param p2 Value to compare
	@param p3 Value to compare
	@param p4 Value to compare
	@param [in,out] pMax Max value result
	@param [in,out] pMin Min value result
	*/
	inline void minAndMax4(int p1, int p2, int p3, int p4, int *pMax,int *pMin) {
		*pMax = MAX(p1, p2);
		*pMax = MAX(*pMax, p3);
		*pMax = MAX(*pMax, p4);
		*pMin = MIN(p1, p2);
		*pMin = MIN(*pMin, p3);
		*pMin = MIN(*pMin, p4);
	}
    /**@}*/

	/**
     @name Culling utilities
     */
	/**@{*/
    /**
	@brief Creates a bounding rectangle surrounding a block.

	p1* -- -- *p2
	  |       |
	  |		  |
	  |		  |
	p3* -- -- *p4

	All points can be arbitrary, as the bounding rectangle gets max/min values to form a rectangle
	@param mP1 A point
	@param mP2 A point
	@param mP3 A point
	@param mP4 A point
	*/
	inline void calculateBoundingRectangle(IND_Vector3 *mP1, IND_Vector3 *mP2, IND_Vector3 *mP3, IND_Vector3 *mP4) {
		int mMinX, mMaxX, mMinY, mMaxY;

		minAndMax4((int) mP1->_x, (int) mP2->_x, (int) mP3->_x, (int) mP4->_x, &mMaxX, &mMinX);
		minAndMax4((int) mP1->_y, (int) mP2->_y, (int) mP3->_y, (int) mP4->_y, &mMaxY, &mMinY);

		mP1->_x = (float) mMinX;
		mP1->_y = (float) mMinY;
		mP2->_x = (float) mMaxX;
		mP2->_y = (float) mMaxY;
	}

	/** @brief Determines if a box determined by 2 points, is inside the supplied frustrum

		Taking an AABB min and max in world space, work out its interaction with the view frustum
		0 is outside
		1 is partially in
		2 is completely within
		Note: the viewing frustum must be calculated first
		//Here we are testing boxes against the frustrum, using the technique called "Geometric Approach - Testing Boxes II" here:
		http://zach.in.tu-clausthal.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html

		@param pAABBMin First vertex of the box
		@param pAABBMax Second vertex of the box
		@param pFrustrum Frustrum structure as per camera position, specified by 6 planes
	*/
	unsigned int cullFrustumBox(const IND_Vector3 &pAABBMin, const IND_Vector3 &pAABBMax, FRUSTRUMPLANES& pFrustrum) {
		bool mIntersect = 0;
		unsigned int mResult = 0;
		IND_Vector3 mMinExtreme, mMaxExtreme;

		for (unsigned int i = 0; i < 6; i++) {
			if (pFrustrum._planes[i]._normal._x >= 0.0f) {
				mMinExtreme._x = pAABBMin._x;
				mMaxExtreme._x = pAABBMax._x;
			} else {
				mMinExtreme._x = pAABBMax._x;
				mMaxExtreme._x = pAABBMin._x;
			}

			if (pFrustrum._planes[i]._normal._y >= 0.0f) {
				mMinExtreme._y = pAABBMin._y;
				mMaxExtreme._y = pAABBMax._y;
			} else {
				mMinExtreme._y = pAABBMax._y;
				mMaxExtreme._y = pAABBMin._y;
			}

			if (pFrustrum._planes[i]._normal._z >= 0.0f) {
				mMinExtreme._z = pAABBMin._z;
				mMaxExtreme._z = pAABBMax._z;
			} else {
				mMinExtreme._z = pAABBMax._z;
				mMaxExtreme._z = pAABBMin._z;
			}

			if (pFrustrum._planes[i].distanceToPoint(mMaxExtreme) < 0.0f) {
				mIntersect = 0;
				mResult = 0;
				return mResult;
			}

			if (pFrustrum._planes[i].distanceToPoint(mMinExtreme) < 0.0f) {
				mIntersect = 1;
			}
		}

		if (mIntersect)
			mResult = 1;
		else
			mResult = 2;

		return mResult;
	}
	/**@}*/

    /**
     @name Collision calculation
     */
	/**@{*/
    
	/**
     Check collision between two circles in different coordinate systems.
     
     Transforms the circle coordinates using supplied transform matrices, and computes collision
     
     @param pB1 Collision data for first circle-type collison
     @param pMat1 Matrix used to transform from coordinates where circle is, to world coordinates
     @param pScale1 Scale to apply to circle radius
     @param pB2 Collision data for second circle-type collison
     @param pMat2 Matrix used to transform from coordinates where circle is, to world coordinates
     @param pScale2 Scale to apply to circle radius
     
     @return true if collision, false otherwise
	*/
	bool isCircleToCircleCollision(BOUNDING_COLLISION *pB1,
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
		transformVector2DbyMatrix4D(mCenter1,pMat1);
		transformVector2DbyMatrix4D(mCenter2,pMat2);
        
		if (isCircleToCircleCollision(mCenter1, mRadius1, mCenter2, mRadius2))
			return 1;
        
		return 0;
	}


	/**
 	 Check collision between two triangles in different coordinate systems.
     
     Transforms the triangle coordinates using supplied transform matrices, and computes collision
     
     @param pB1 Collision data for first triangle-type collision
     @param pMat1 Matrix used to transform from coordinates where triangle is, to world coordinates
     @param pB2 Collision data for second triangle-type collision
     @param pMat2 Matrix used to transform from coordinates where triangle is, to world coordinates
     
     @return true if collision, false otherwise
	*/
	bool isTriangleToTriangleCollision(BOUNDING_COLLISION *pB1,
                                       IND_Matrix pMat1,
                                       BOUNDING_COLLISION *pB2,
                                       IND_Matrix pMat2) {
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
		transformVector2DbyMatrix4D(mA1,pMat1);
		transformVector2DbyMatrix4D(mB1,pMat1);
		transformVector2DbyMatrix4D(mC1,pMat1);

		transformVector2DbyMatrix4D(mA2,pMat2);
		transformVector2DbyMatrix4D(mB2,pMat2);
		transformVector2DbyMatrix4D(mC2,pMat2);


		if (isTriangleToTriangleCollision(mA1, mB1, mC1, mA2, mB2, mC2))
			return 1;

		return 0;
	}


    /**
 	 Check collision between a circle an a triangle in different coordinate systems.
     
     Transforms the triangle and circle coordinates using supplied transform matrices, and computes collision
     
     @param pB1 Collision data for circle-type collison
     @param pMat1 Matrix used to transform from coordinates where circle is, to world coordinates
     @param pScale Scale to apply to circle radius
     @param pB2 Collision data for triangle-type collision
     @param pMat2 Matrix used to transform from coordinates where triangle is, to world coordinates
     
     @return true if collision, false otherwise
     */
	bool isCircleToTriangleCollision(BOUNDING_COLLISION *pB1,
                                     IND_Matrix pMat1,
                                     float pScale,
                                     BOUNDING_COLLISION *pB2,
                                     IND_Matrix pMat2) {

		// ----- Circle -----

		// Circle 1
		IND_Vector2 mCenter((float) pB1->_posX, (float) pB1->_posY);
		int _radius = (int)(pB1->_radius * pScale);
		
		transformVector2DbyMatrix4D(mCenter,pMat1);

		// ----- Triangle -----

		// Triangle
		IND_Vector2 mA((float) pB2->_ax, (float) pB2->_ay);
		IND_Vector2 mB((float) pB2->_bx, (float) pB2->_by);
		IND_Vector2 mC((float) pB2->_cx, (float) pB2->_cy);

		// Transformations
		transformVector2DbyMatrix4D(mA,pMat2);
		transformVector2DbyMatrix4D(mB,pMat2);
		transformVector2DbyMatrix4D(mC,pMat2);

		if (isCircleToTriangleCollision(mCenter, _radius, mA, mB, mC))
			return 1;

		return 0;
	}


	/**
	 Check collision between two triangles in same coordinate system.
     Triangles are specified as (a1, b1, c1) and (a2, b2, c2).
     Triangles must be in same coordinate system for this method to work!
     
	 - First checks if each of the vertices of either triange is within inside the other triangle
	 - After that it checks for intersections between the triangle segments
     @param a1,b1,c1 First triangle coordinates
     @param a2,b2,c2 Second triangle coordinates
     
     @return true if collision, false otherwise
	*/
	bool isTriangleToTriangleCollision(IND_Vector2 &a1,
                                       IND_Vector2 &b1,
                                       IND_Vector2 &c1,
                                       IND_Vector2 &a2,
                                       IND_Vector2 &b2,
                                       IND_Vector2 &c2) {
		// Check if any vertex of triange 1 is inside triange 2
		if (isPointInsideTriangle(a1, a2, b2, c2)) return 1;
		if (isPointInsideTriangle(b1, a2, b2, c2)) return 1;
		if (isPointInsideTriangle(c1, a2, b2, c2)) return 1;

		// Check if any vertex of triange 2 is inside triange 1
		if (isPointInsideTriangle(a2, a1, b1, c1)) return 1;
		if (isPointInsideTriangle(c2, a1, b1, c1)) return 1;
		if (isPointInsideTriangle(c2, a1, b1, c1)) return 1;

		// Checks if any segment of triange 1 intersects with any segment of triange 2
		// Segment (a1 - b1)
		if (isSegmentIntersection(a1, b1, a2, b2)) return 1;
		if (isSegmentIntersection(a1, b1, b2, c2)) return 1;
		if (isSegmentIntersection(a1, b1, c2, a2)) return 1;

		// Segment (b1 - c1)
		if (isSegmentIntersection(b1, c1, a2, b2)) return 1;
		if (isSegmentIntersection(b1, c1, b2, c2)) return 1;
		if (isSegmentIntersection(b1, c1, c2, a2)) return 1;

		// Segment (c1 - a1)
		if (isSegmentIntersection(c1, a1, a2, b2)) return 1;
		if (isSegmentIntersection(c1, a1, b2, c2)) return 1;
		if (isSegmentIntersection(c1, a1, c2, a2)) return 1;

		return 0;
	}


	/**
	 Check collision between two circles in same coordinate system.
     Both circles must be in same coordinate system to work!
     
     @param pP1 Center of first circle
     @param pRadius1 Radius of first circle
     @param pP2 Center of second circle
     @param pRadius2 Radius of second circle
     
     @return true if collision, false otherwise
	*/
	bool isCircleToCircleCollision(IND_Vector2 &pP1,
                                   int pRadius1,
                                   IND_Vector2 &pP2,
                                   int pRadius2) {
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


	/**
     Check collision between a circle and a triangle in same coordinate system.
     Both elements must be in same coordinate system to work!
     
     @param pPCenter Center of first circle
     @param pRadius Radius of first circle
     @param pA2, pB2, pC2 Point of triangle
     
     @return true if collision, false otherwise
	*/
	bool isCircleToTriangleCollision(IND_Vector2 &pPCenter,
                                     int pRadius,
                                     IND_Vector2 &pA2,
                                     IND_Vector2 &pB2,
                                     IND_Vector2 &pC2) {
		// Check if circle center inside the triangle
		if (isPointInsideTriangle(pPCenter, pA2, pB2, pC2)) return 1;

		// Check the distance of the circle center to the 3 triangle segments
		if (pointToLineDistance(pA2, pB2, pPCenter, 1) < pRadius) return 1;
		if (pointToLineDistance(pB2, pC2, pPCenter, 1) < pRadius) return 1;
		if (pointToLineDistance(pC2, pA2, pPCenter, 1) < pRadius) return 1;

		return 0;
	}
    
	/**
	 Check if point p is inside the triangle with vertex a, b, c
	 Technique from: http://www.blackpawn.com/texts/pointinpoly/default.html
     
     @param p A point
     @param a,b,c Triangle point
     
     @return true if inside, false otherwise
	*/
	static bool isPointInsideTriangle(IND_Vector2 &p,
                                      IND_Vector2 &a,
                                      IND_Vector2 &b,
                                      IND_Vector2 &c) {
		// Compute vectors
		IND_Vector2 v0 = c - a;
		IND_Vector2 v1 = b - a;
		IND_Vector2 v2 = p - a;

		// Compute dot products
		float dot00 = v0.dotProduct(v0);
		float dot01 = v0.dotProduct(v1);
		float dot02 = v0.dotProduct(v2);
		float dot11 = v1.dotProduct(v1);
		float dot12 = v1.dotProduct(v2);

		// Compute barycentric coordinates
		float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is in triangle
		return (u > 0) && (v > 0) && (u + v < 1);
	}

	/**
	 Compute the distance from AB to C.
	 
     @param pA,pB Sement or line point
     @param pC Point to compute distance from
     @param pIsSegment If true, AB is a segment, not a line
     
     @return Value of distance
	*/
	double pointToLineDistance(IND_Vector2 &pA,
                               IND_Vector2 &pB,
                               IND_Vector2 &pC,
                               bool pIsSegment) {
		IND_Vector2 ab (pB._x - pA._x, pB._y - pA._y);
		IND_Vector2 ac (pC._x - pA._x, pC._y - pA._y);
		
		double mDist (ab.crossProduct(ac) / pA.distance(pB));

		if (pIsSegment) {
			IND_Vector2 bc (pC._x - pB._x, pC._y - pB._y);
			IND_Vector2 ba (pA._x - pB._x, pA._y - pB._y);

			float mDot1 (ab.dotProduct(bc));
			if (mDot1 > 0.0f) { 
				return pB.distance(pC);
			}
			float mDot2 (ba.dotProduct(ac));
			if (mDot2 > 0.0f) {
				return pA.distance(pC);
			}
		}

		return fabs(mDist);
	}

	/**
	 Check if there is an intersection between two segments
	 Segment 1 => (pAx, pAy) - (pBx, pBy)
	 Segment 2 => (pCx, pCy) - (pDx, pDy)
     
     @param a,b First segment
     @param c,d Second segment
     
     @return true if segments intersect, false otherwise
	*/
	bool isSegmentIntersection(IND_Vector2 &a,
                               IND_Vector2 &b,
                               IND_Vector2 &c,
                               IND_Vector2 &d) {
		float mGradAB, mGradCD, mYcptAB, mYcptCD, mInterceptX, mInterceptY;

		// In order to avoid divisions by zero
		if (a._y == b._y)
			b._y += 0.0001f;

		if (a._x == b._x)
			b._x += 0.0001f;

		if (c._y == d._y)
			d._y += 0.0001f;

		if (c._x == d._x)
			d._x += 0.0001f;

		// Calculates the intersection between the two lines
		mGradAB = (a._y - b._y) / (a._x - b._x);
		mGradCD = (c._y - d._y) / (c._x - d._x);

		mYcptAB = a._y - a._x * mGradAB;
		mYcptCD = c._y - c._x * mGradCD;
		mInterceptX = (mYcptAB - mYcptCD) / (mGradCD - mGradAB);
		mInterceptY = (mYcptAB - (mGradAB * mYcptCD) / mGradCD) / (1 - mGradAB / mGradCD);

		// Checking in the intersection is inside the segment
		if (!((mInterceptX >= a._x && mInterceptX <= b._x) || (mInterceptX >= b._x && mInterceptX <= a._x)))
			return 0;

		if (!((mInterceptY >= a._y && mInterceptY <= b._y) || (mInterceptY >= b._y && mInterceptY <= a._y)))
			return 0;

		if (!((mInterceptX >= c._x && mInterceptX <= d._x) || (mInterceptX >= d._x && mInterceptX <= c._x)))
			return 0;

		if (!((mInterceptY >= c._y && mInterceptY <= d._y) || (mInterceptY >= d._y && mInterceptY <= c._y)))
			return 0;

		return 1;
	}
    
    /**@}*/
    
	/**
     @name Matrix utilities
     */
	/**@{*/
    /**
	 Multiplies 2 matrices.
     
	 In the form of R = M1*M2 (Post multiplication on the right) that means we take the rows from
	 the left, and the columns from the right as:
     
      [        ][       ]
      [   M1   ][   M2  ]
      [        ][       ]  = R
      [        ][       ]
     
     @param m1 First matrix (left side)
     @param m2 Second matrix (right side)
     @param result Result matrix
	*/
	inline void matrix4DMultiply(const IND_Matrix &m1, const IND_Matrix &m2, IND_Matrix &result)const {
	  
	  result._11 = m1._11 * m2._11 + m1._12 * m2._21 + m1._13 * m2._31 + m1._14 * m2._41;
	  result._12 = m1._11 * m2._12 + m1._12 * m2._22 + m1._13 * m2._32 + m1._14 * m2._42;
	  result._13 = m1._11 * m2._13 + m1._12 * m2._23 + m1._13 * m2._33 + m1._14 * m2._43;
	  result._14 = m1._11 * m2._14 + m1._12 * m2._24 + m1._13 * m2._34 + m1._14 * m2._44;
 
 	  result._21 = m1._21 * m2._11 + m1._22 * m2._21 + m1._23 * m2._31 + m1._24 * m2._41;
	  result._22 = m1._21 * m2._12 + m1._22 * m2._22 + m1._23 * m2._32 + m1._24 * m2._42;
	  result._23 = m1._21 * m2._13 + m1._22 * m2._23 + m1._23 * m2._33 + m1._24 * m2._43;
	  result._24 = m1._21 * m2._14 + m1._22 * m2._24 + m1._23 * m2._34 + m1._24 * m2._44;

	  result._31 = m1._31 * m2._11 + m1._32 * m2._21 + m1._33 * m2._31 + m1._34 * m2._41;
	  result._32 = m1._31 * m2._12 + m1._32 * m2._22 + m1._33 * m2._32 + m1._34 * m2._42;
	  result._33 = m1._31 * m2._13 + m1._32 * m2._23 + m1._33 * m2._33 + m1._34 * m2._43;
	  result._34 = m1._31 * m2._14 + m1._32 * m2._24 + m1._33 * m2._34 + m1._34 * m2._44;

	  result._41 = m1._41 * m2._11 + m1._42 * m2._21 + m1._43 * m2._31 + m1._44 * m2._41;
	  result._42 = m1._41 * m2._12 + m1._42 * m2._22 + m1._43 * m2._32 + m1._44 * m2._42;
	  result._43 = m1._41 * m2._13 + m1._42 * m2._23 + m1._43 * m2._33 + m1._44 * m2._43;
	  result._44 = m1._41 * m2._14 + m1._42 * m2._24 + m1._43 * m2._34 + m1._44 * m2._44;
	}

	/**
	 Initializes matrix as Identity.
     
     @param m Matrix to initialize
	*/
	inline void matrix4DSetIdentity(IND_Matrix &m)const  {
	  m._11 = 1.0f;
	  m._12 = 0.0f;
	  m._13 = 0.0f;
	  m._14 = 0.0f;
	  m._21 = 0.0f;
	  m._22 = 1.0f;
	  m._23 = 0.0f;
	  m._24 = 0.0f;
	  m._31 = 0.0f;
	  m._32 = 0.0f;
	  m._33 = 1.0f;
	  m._34 = 0.0f;
	  m._41 = 0.0f;
	  m._42 = 0.0f;
	  m._43 = 0.0f;
	  m._44 = 1.0f;
	}
	
	/**
	 Initializes matrix as Translation Matrix
     
     @param m Matrix to initialize
     @param transx,transy,transz Translation values
	*/
	inline void matrix4DSetTranslation(IND_Matrix &m, float transx, float transy, float transz) const {
	  m._11 = 1.0f;
	  m._12 = 0.0f;
	  m._13 = 0.0f;
	  m._14 = transx;
	  m._21 = 0.0f;
	  m._22 = 1.0f;
	  m._23 = 0.0f;
	  m._24 = transy;
	  m._31 = 0.0f;
	  m._32 = 0.0f;
	  m._33 = 1.0f;
	  m._34 = transz;
	  m._41 = 0.0f;
	  m._42 = 0.0f;
	  m._43 = 0.0f;
	  m._44 = 1.0f;
	}
	
	/**
	 Initializes matrix as Rotation Matrix.
     
	 This one is tough (or representation of glRotatef(), some kind of quaternion represented as a matrix. 
     Detailed formula can be found in the net.
     
     @param m Matrix to initialize
	 @param angledegrees Angle IN DEGREES
     @param axis Axis to rotate around to, typically, for 2d, is (0,0,1)
	*/
	inline void matrix4DSetRotationAroundAxis(IND_Matrix &m, float angledegrees, const IND_Vector3 &axis) const  {
		  float angle = angleToRadians(angledegrees);
		  float x(axis._x);
		  float y(axis._y);
		  float z(axis._z);
		  float mag = sqrtf((x * x) + (y * y) + (z * z));
		  
		  //Normalize input vector (Don't call normalize and perform here)
		  if (mag < 1e-08) {
			x = 1.0;
			y = 0.0;
			z = 0.0;
		  } else if (mag != 1.0f) {
			x /= mag;
			y /= mag;
			z /= mag;
		  }
		  
		  float c = cosf(angle);
		  float s = sinf(angle);
		  float t = 1 - c;
		  //Left-handed coord system
		  /*m._11 = (x * x) * t + c;
			m._21 = t * x * y - s * z;
			m._31 = t * x * y + s * y;
		    
			m._12 = t * x * y + s * z;
			m._22 = t * y * y + c;
			m._32 = t * y * z - s * x;
		    
			m._13 = t * x * z - s * y;
			m._23 = t * y * z + s * x;
			m._33 = t * z * z + c;*/
		  
		  //Right-handed coord system
		  m._11 = 1 + t * (x * x - 1);
		  m._21 = z * s + t * x * y;
		  m._31 = -y * s + c * x *z;
		  
		  m._12 = -z * s + t * x * y;
		  m._22 = 1 + t * (y * y - 1);
		  m._32 = x * s + t * y * z;
		  
		  m._13 = y * s + t * x* z;
		  m._23 = -x * s + t * y * z;
		  m._33 = 1 + t * (z * z - 1);

		  m._14 = m._24 = m._34 = 0.0f;
		  m._41 = m._42 = m._43 = 0.0f;
		  m._44 = 1.0f;
	}
        
	/**
	 Initializes matrix as a Scale Matrix.
     
     @param m Matrix to initialize
     @param scalex, scaley, scalez Scale values
	*/
	inline void matrix4DSetScale(IND_Matrix &m, float scalex, float scaley, float scalez)const {
		  m._11 = scalex;
		  m._12 = 0.0f;
		  m._13 = 0.0f;
		  m._14 = 0.0f;
		  m._21 = 0.0f;
		  m._22 = scaley;
		  m._23 = 0.0f;
		  m._24 = 0.0f;
		  m._31 = 0.0f;
		  m._32 = 0.0f;
		  m._33 = scalez;
		  m._34 = 0.0f;
		  m._41 = 0.0f;
		  m._42 = 0.0f;
		  m._43 = 0.0f;
		  m._44 = 1.0f;
	}
	

	/**
	 Initializes a matrix to represent a rotation matrix from 3 axes: r (right), u(up), l(look) AND p (pos).
     
     It uses left handed coordinate system conventions.
	 In some documentation you will see pos referred as eye (eye position)
	 Form:
	        AXES      TRANSLATION
	    [l1  l2  l3  0][ 1         -px]
	    [u1  u2  u3  0][    1      -py]
	    [r1  r2  r3  0][        1  -pz]
	    [0   0   0   1][            1 ]
	
	 Last column are dot products
	 It is quite similar as the gluLookAt() function from OpenGL utility library, but using 3 axes vectors from camera
     
     @param r Right camera vector
     @param u Up camera vector
     @param l Look camera vector
     @param p Where the camera is positioned
     @param result Matrix to initialize
	*/
	inline void matrix4DLookAtMatrixLH(const IND_Vector3 &r, const IND_Vector3 &u, const IND_Vector3 &l, const IND_Vector3 &p, IND_Matrix &result)const {
		IND_Matrix rot;
		IND_Matrix trans;

		//Compute axes matrix manually
		rot._11 = r._x;
		rot._12 = u._x;
		rot._13 = l._x;
		rot._14 = 0.0f;
		rot._21 = r._y;
		rot._22 = u._y;
		rot._23 = l._y;
		rot._24 = 0.0f;
		rot._31 = r._z;
		rot._32 = u._z;
		rot._33 = l._z;
		rot._34 = 0.0f;
		rot._41 = 0.0f;
		rot._42 = 0.0f;
		rot._43 = 0.0f;
		rot._44 = 1.0f;


		//Compute tranlation matrix
		matrix4DSetTranslation(trans, -p._x, -p._y, -p._z);

		//Compute result matrix
		matrix4DMultiply(rot,trans, result);
	}

	/**
	 Initializes a matrix to represent a rotation matrix from 3 axes: r (right), u(up), l(look) AND p (pos).
     
     It uses right handed coordinate system conventions.
	 In some documentation you will see pos referred as eye (eye position)
	 Form:
     AXES      TRANSLATION
     [l1  l2  l3  0][ 1         -px]
     [u1  u2  u3  0][    1      -py]
     [r1  r2  r3  0][        1  -pz]
     [0   0   0   1][            1 ]
     
	 Last column are dot products
	 It is quite similar as the gluLookAt() function from OpenGL utility library, but using 3 axes vectors from camera
     
     @param r Right camera vector
     @param u Up camera vector
     @param l Look camera vector
     @param p Where the camera is positioned
     @param result Matrix to initialize
     */
	inline void matrix4DLookAtMatrixRH(const IND_Vector3 &r, const IND_Vector3 &u, const IND_Vector3 &l, const IND_Vector3 &p, IND_Matrix &result)const {
		IND_Matrix rot;
		IND_Matrix trans;

		//Compute axes matrix manually
		rot._11 = -r._x;
		rot._12 = u._x;
		rot._13 = -l._x;
		rot._14 = 0.0f;
		rot._21 = -r._y;
		rot._22 = u._y;
		rot._23 = -l._y;
		rot._24 = 0.0f;
		rot._31 = -r._z;
		rot._32 = u._z;
		rot._33 = -l._z;
		rot._34 = 0.0f;
		rot._41 = 0.0f;
		rot._42 = 0.0f;
		rot._43 = 0.0f;
		rot._44 = 1.0f;


		//Compute tranlation matrix
		matrix4DSetTranslation(trans, p._x, -p._y, -p._z);

		//Compute result matrix
		matrix4DMultiply(rot,trans, result);
	}


	/**
	 Initializes a matrix to represent a rotation matrix from 3 axes: eye, lookat and up.
     
     It uses right handed coordinate system conventions
	 It replaces gluLookAt directly (without multiplying the matrix to GL of course)
 	 
     @param eye Where camera is positioned
     @param lookAt Where camera looks to
     @param up Up vector from camera
     @param result Matrix to initialize
	*/        
	inline void matrix4DLookAtMatrixEyeLookUpRH(const IND_Vector3 &eye, const IND_Vector3 &lookAt, const IND_Vector3 &up, IND_Matrix &result)const {
		IND_Vector3 z (lookAt - eye);
    	z.normalise();
    	IND_Vector3 x (up.crossProduct(z));
    	x.normalise();
		IND_Vector3 y (z.crossProduct(x));
    	y.normalise();

    	//Use other class method to finally construct matrix
    	matrix4DLookAtMatrixRH(x,y,z,eye,result);
	}

	/**
	 Initializes a matrix to represent a rotation matrix from 3 axes: eye, lookat and up.
     
     It uses left handed coordinate system conventions
	 It replaces gluLookAt directly (without multiplying the matrix to GL of course)
 	 
     @param eye Where camera is positioned
     @param lookAt Where camera looks to
     @param up Up vector from camera
     @param result Matrix to initialize
     */
	inline void matrix4DLookAtMatrixEyeLookUpLH(const IND_Vector3 &eye, const IND_Vector3 &lookAt, const IND_Vector3 &up, IND_Matrix &result)const {
		IND_Vector3 z (lookAt - eye);
    	z.normalise();
    	IND_Vector3 x (up.crossProduct(z));
    	x.normalise();
		IND_Vector3 y (z.crossProduct(x));
    	y.normalise();

    	//Use other class method to finally construct matrix
    	matrix4DLookAtMatrixLH(x,y,z,eye,result);
	}

	/**
	 Initializes a matrix representing an orthonormal transformation.
	 It uses right handed coordinate system conventions
	 It replaces glOrtho directly (without multiplying the matrix to GL of course)
     
     @param left Left margin of view
     @param right Right margin of view
     @param bottom Bottom margin of view
     @param top Top margin of view
     @param znear Nearest z to show in view
     @param zfar Farthest z to show in view
     @param result Matrix to initialize
     */
	inline void matrix4DOrthographicProjectionRH(float left , float right, float bottom, float top, float znear, float zfar, IND_Matrix &result)  {
		float a = 2.0f / (right - left);
		float b = 2.0f / (top - bottom);
		float c =  -2.0f / (znear - zfar);
		float tx = (right + left) / (right - left);
		float ty = (top + bottom) / (top - bottom);
		float tz = -(zfar + znear) / (zfar - znear);
		result._11 = a;
		result._12 = 0.0f;
		result._13 = 0.0f;
		result._14 = tx;
		result._21 = 0.0f;
		result._22 = b;
		result._23 = 0.0f;
		result._24 = ty;
		result._31 = 0.0f;
		result._32 = 0.0f;
		result._33 = c;
		result._34 = tz;
		result._41 = 0.0f;
		result._42 = 0.0f;
		result._43 = 0.0f;
		result._44 = 1.0f;
	}

	/**
	 Initializes a matrix representing an orthonormal transformation.
	 It uses left handed coordinate system conventions
	 It replaces glOrtho directly (without multiplying the matrix to GL of course)
     
     @param left Left margin of view
     @param right Right margin of view
     @param bottom Bottom margin of view
     @param top Top margin of view
     @param znear Nearest z to show in view
     @param zfar Farthest z to show in view
     @param result Matrix to initialize
     */
	inline void matrix4DOrthographicProjectionLH(float left , float right, float bottom, float top, float znear, float zfar, IND_Matrix &result)  {
		float a = 2.0f / (right - left);
		float b = 2.0f / (top - bottom);
		float c =  -2.0f / (zfar - znear);
		float tx = (right + left) / (right - left);
		float ty = (top + bottom) / (top - bottom);
		float tz = (zfar + znear) / (zfar - znear);
		result._11 = a;
		result._12 = 0.0f;
		result._13 = 0.0f;
		result._14 = tx;
		result._21 = 0.0f;
		result._22 = b;
		result._23 = 0.0f;
		result._24 = ty;
		result._31 = 0.0f;
		result._32 = 0.0f;
		result._33 = c;
		result._34 = tz;
		result._41 = 0.0f;
		result._42 = 0.0f;
		result._43 = 0.0f;
		result._44 = 1.0f;
	}
    /**@}*/

	/**
     @name Matrix with vector utilities
     */
	/**@{*/

	/**
	 Multiplies the column vector to the matrix (from the right side).
     
     @param vector The vector to multiply (right side), will be modified directly with transform.
     @param mat The matrix, left side.
	*/
	inline void transformVector3DbyMatrix4D(IND_Vector3 &vector, const IND_Matrix &mat)const {
		float x = vector._x;
		float y = vector._y;
		float z = vector._z;

		vector._x = mat._11 * x + mat._12 * y + mat._13 * z + mat._14;
		vector._y = mat._21 * x + mat._22 * y + mat._23 * z + mat._24;
		vector._z = mat._31 * x + mat._32 * y + mat._33 * z + mat._34;
	}

    /**
	 Multiplies the column vector to the matrix (from the right side).
     
     @param vector The vector to multiply (right side), will be modified directly with transform.
     @param mat The matrix, left side.
     */
	inline void transformVector2DbyMatrix4D(IND_Vector2 &vector, const IND_Matrix &mat)const {
		IND_Vector3 fake (vector._x,vector._y,0.0f);

		transformVector3DbyMatrix4D(fake,mat);

		vector._x = fake._x;
		vector._y = fake._y;
	}

    /**@}*/

private:

    /** @cond DOCUMENT_PRIVATEAPI */
	// ----- Private -----

	bool _ok;


	// ----- Private methods -----

	void                initVars();
	void                freeVars();
    
    /** @endcond */
};

/**@}*/

#endif // _IND_MATH_
