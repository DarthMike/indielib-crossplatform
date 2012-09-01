/*****************************************************************************************
 * File: IND_Math.h
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


/*
    BIG NOTE:
    IN THE COURSE OF THIS PORT TO OPENGL I FOUND BIG USE OF DIRECTX MATH LIBRARY
    AS OPENGL DOESN'T HAVE MATH LIBRARY, I IMPLEMENTED MY OWN. SO, THE IMPORTANT PART
    IS THAT HERE SOME ASSUMPTIONS ABOUT THE UNDERLYING SYSTEM ARE WORKING:
        - RIGHT-HANDED COORDINATE SYSTEM
        - MATRIX REPRESENTATION BY COLUMN ORDER  (JUST IN THE DEFINITION OF THE MATRIX IT IS CHANGED)
        - VECTORS AS COLUMNS AND POST-MULTIPLYING

    IF YOU DON'T UNDERSTAND IT: GREAT SITE JUST TO THE POINT OF IT: http://www.mindcontrol.org/~hplus/graphics/matrix-layout.html
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

class LIB_EXP IND_Math {
public:
	// ----- Init/End -----

	IND_Math(): _ok(false)  { }
	~IND_Math()              {
		end();
	}

	bool    init();
	void    end();
	bool    isOK() {
		return _ok;
	}

	// ----- Public methods -----

	void        randSeed(long pNum);

	/*
	==================
	Returns a random number between two boundaries
	==================
	*/
	inline long randnum(long pIni, long pEnd){
	if (pIni > pEnd) {
		long pTemp = pEnd;
		pEnd = pIni;
		pIni = pTemp;
	}

	return rand() % (pEnd - pIni + 1) + pIni;

	//long mResu = pIni + (rand () % ((pEnd - pIni) + 1));
	//return mResu;
	}

	bool        isPowerOfTwo(long pN);

	//Radians to angle (templated)
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

	//Angle to radians (templated)
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
    
    // ----- Collision utilities -----
	/*
	==================
	Check if point p is inside the triangle with vertex a, b, c
	Technique from: http://www.blackpawn.com/texts/pointinpoly/default.html
	==================
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

	/*
	==================
	Compute the distance from AB to C
	if isSegment is true, AB is a segment, not a line.
	==================
	*/
	double pointToLineDistance(IND_Vector2 &pA, IND_Vector2 &pB, IND_Vector2 &pC, bool pIsSegment) {
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

	/*
	==================
	Check if there is an intersection between two segments
	Segment 1 => (pAx, pAy) - (pBx, pBy)
	Segment 2 => (pCx, pCy) - (pDx, pDy)
	==================
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
	// ----- Matrix utilities -----
#if defined (INDIERENDER_OPENGL) || defined (INDIERENDER_GLES_IOS)

	 /*
	==================
	Multiplies 2 matrices (needs allocated result)
	In the form of R = M1*M2 (Post multiplication on the right) that means we take the rows from
	the left, and the columns from the right
	==================
	*/
	inline void matrix4DMultiply(const IND_Matrix &m1, const IND_Matrix &m2, IND_Matrix &result)const {
	  /*
	          [        ][       ]
	          [   M1   ][   M2  ]
	          [        ][       ]  = R
	          [        ][       ]
	  */
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
	};

	/*
	==================
	Initializes matrix as Identity
	==================
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
	};
	
	/*
	==================
	Initializes matrix as Translation Matrix
	==================
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
	};
	
	/*
	==================
	Initializes matrix as Rotation Matrix
	This one is tough (or representation of glRotatef()
	some kind of quaternion represented as a matrix.
	Detailed formula can be found in the net
	Angle is in degrees
	==================
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
	};
        
	/*
	==================
	Initializes matrix as a Scale Matrix
	==================
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
	};
	

	/*
	==================

	Initializes a matrix to represent a rotation matrix from 3 axes: r (right), u(up), l(look) AND p (pos)
	In some documentation you will see pos referred as eye (eye position)
	Form:
	        AXES      TRANSLATION
	    [l1  l2  l3  0][ 1         -px]
	    [u1  u2  u3  0][    1      -py]
	    [r1  r2  r3  0][        1  -pz]
	    [0   0   0   1][            1 ]
	
	Last column are dot products
	It is quite similar as the gluLookAt() function from OpenGL, but from 3 axes vectors from camera
	==================
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
	};

/*
	==================

	Initializes a matrix to represent a rotation matrix from 3 axes: r (right), u(up), l(look) AND p (pos)
	In some documentation you will see pos referred as eye (eye position)
	Form:
	        AXES      TRANSLATION
	    [l1  l2  l3  0][ 1         -px]
	    [u1  u2  u3  0][    1      -py]
	    [r1  r2  r3  0][        1  -pz]
	    [0   0   0   1][            1 ]
	
	Last column are dot products
	It is quite similar as the gluLookAt() function from OpenGL, but from 3 axes vectors from camera
	==================
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
	};


	/*
	==================
	Initializes a matrix to represent a rotation matrix from 3 axes: eye, lookat and up
	It replaces gluLookAt directly (without multiplying the matrix to GL of course)
	It uses Right handed coordinate system conventions.
	==================
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
	};

	/*
	==================
	Initializes a matrix to represent a rotation matrix from 3 axes: eye, lookat and up
	It replaces gluLookAt directly (without multiplying the matrix to GL of course)
	It uses Left handed coordinate system conventions.
	==================
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
	};

	/*
	==================
	Initializes a matrix representing an orthonormal transformation
	Uses Right handed coord system
	(Just as glOrtho makes)
	==================
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

	/*
	==================
	Initializes a matrix representing an orthonormal transformation
	Uses Left handed coord system
	(Just as glOrtho makes)
	==================
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

	// ----- Matrix4/Vector3 utilities -----

	/*
	==================
	Multiplies the column vector to the matrix (from the right side)
	==================
	*/
	inline void transformVector3DbyMatrix4D(IND_Vector3 &vector, const IND_Matrix &mat)const {
		float x = vector._x;
		float y = vector._y;
		float z = vector._z;

		vector._x = mat._11 * x + mat._12 * y + mat._13 * z + mat._14;
		vector._y = mat._21 * x + mat._22 * y + mat._23 * z + mat._24;
		vector._z = mat._31 * x + mat._32 * y + mat._33 * z + mat._34;
	}

	/*
	==================
	Multiplies the column vector to the matrix (from the right side)
	As 2d vector can't be multiplied, a 3D vector is created with z = 0
	==================
	*/
	inline void transformVector2DbyMatrix4D(IND_Vector2 &vector, const IND_Matrix &mat)const {
		IND_Vector3 fake (vector._x,vector._y,0.0f);

		transformVector3DbyMatrix4D(fake,mat);

		vector._x = fake._x;
		vector._y = fake._y;
	}
#endif
private:

	// ----- Private -----

	bool _ok;


	// ----- Private methods -----

	void                initVars();
	void                freeVars();
};

#endif // _IND_MATH_
