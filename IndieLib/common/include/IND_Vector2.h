/*****************************************************************************************
 * File: IND_Vector2.h
 * Dess: 2D Vector implementation
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


#ifndef _IND_VECTOR2_
#define _IND_VECTOR2_

//Libraries dependencies
#include <math.h>
#include <assert.h>

//Classes dependencies

class IND_Vector2 {
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	//Default constructor
	IND_Vector2(): _x(0), _y(0) {
	}
	//Must use this constructor to create a vector with data
	explicit IND_Vector2(float newx, float newy): _x(newx), _y(newy)
	{}
	~IND_Vector2()
	{}
	//****Operators overloading****
	//Equaling to other vector
	IND_Vector2 &operator = (const IND_Vector2 &newvector) {
		_x = newvector._x;
		_y = newvector._y;

		return *this;
	}
	//Comparison between vectors
	inline bool operator == (const IND_Vector2 &newvector) const {
		return (_x == newvector._x && _y == newvector._y);
	}
	inline bool operator != (const IND_Vector2 &newvector) const {
		return (_x != newvector._x || _y != newvector._y);
	}
	//Arithmetic
	//Sum of coordinates
	inline IND_Vector2 operator + (const IND_Vector2 &newvector) const {
		return IND_Vector2(
		          _x + newvector._x,
		           _y + newvector._y);
	}
	//Self sum....
	inline const IND_Vector2 &operator + () const {
		return *this;
	}
	//Sum updating - Point to point
	inline IND_Vector2 &operator += (const IND_Vector2 &tosum) {
		_x += tosum._x;
		_y += tosum._y;

		return *this;
	}
	//Sum updating - Offset to every coordinate
	inline IND_Vector2 &operator += (const float offset) {
		_x += offset;
		_y += offset;

		return *this;
	}
	//Rest of coordinates
	inline IND_Vector2 operator - (const IND_Vector2 &newvector) const {
		return IND_Vector2(
		           _x - newvector._x,
		           _y - newvector._y);
	}
	//Rest Update
	inline IND_Vector2 &operator -= (const IND_Vector2 &torest) {
		_x -= torest._x;
		_y -= torest._y;

		return *this;
	}
	//Rest updating - Offset to every coordinate
	inline IND_Vector2 &operator -= (const float offset) {
		_x -= offset;
		_y -= offset;

		return *this;
	}
	//Rest or negative vector
	inline IND_Vector2 operator - () const {
		return IND_Vector2(-_x, -_y);
	}
	//Scalar multiply
	inline IND_Vector2 operator * (const float scalar) const {
		return IND_Vector2(
		           _x * scalar,
		           _y * scalar);
	}
	//Point to point multiply
	inline IND_Vector2 operator * (const IND_Vector2 &rhs) const {
		return IND_Vector2(
		           _x * rhs._x,
		           _y * rhs._y);
	}
	//Multiply update - Multiply to scalar
	inline IND_Vector2 &operator *= (const float scalar) {
		_x *= scalar;
		_y *= scalar;
		return *this;
	}
	//Multiply update - Point to point multiply
	inline  IND_Vector2 &operator *= (const IND_Vector2 &tomultiply) {
		_x *= tomultiply._x;
		_y *= tomultiply._y;

		return *this;
	}
	//Scalar divide
	inline IND_Vector2 operator / (const float scalar) const {
		assert(scalar != 0.0f);
		float inv = 1.0f / scalar;
		return IND_Vector2(
		           _x * inv,
		           _y * inv);
	}
	//Point to point divide
	inline IND_Vector2 operator / (const IND_Vector2 &divideto) const {
		assert(divideto._x != 0 && divideto._y != 0);
		return IND_Vector2(
		           _x / divideto._x,
		           _y / divideto._y);
	}
	//Divide update - Divide to scalar
	inline IND_Vector2 &operator /= (const float scalar) {
		assert(scalar != 0.0f);

		float inv = 1.0f / scalar;
		_x *= inv;
		_y *= inv;
		return *this;
	}
	//Divide update - Point to point divide
	inline IND_Vector2 &operator /= (const IND_Vector2 &todivide) {
       assert(todivide._x != 0 && todivide._y != 0);
		_x /= todivide._x;
		_y /= todivide._y;
		return *this;
	}

	//******************

	//----- GET/SET FUNCTIONS -----
	//No Get/Set - PUBLIC VARIABLES
	//----- OTHER FUNCTIONS -----
	//Length of vector
	inline float length() {
		return(sqrt(_x * _x) + (_y * _y));
	}

	inline float dotProduct(const IND_Vector2 &vec) const {
		return _x * vec._x + _y * vec._y;
	}

	inline float crossProduct(const IND_Vector2 &vec) const {
		return _x * vec._y - _y * vec._x;
	}

	//Normalize of vector
	inline float normalise() {
		float l = length();

		// Protection on floating point calcs
		if (l > 1e-08) {
			float invlength = 1.0f / l;
			_x *= invlength;
			_y *= invlength;
		}

		return l;
	}
	//Return distance to another vector
	inline float distance(const IND_Vector2 &to) const {
		return (*this - to).length();
	}

	//Return angle between this and another vector
	inline float angleBetweenUnitVecs(const IND_Vector2 &dest) {
		//Angle between vectors:
		//alpha = arcos((vec1*vec2)/|vec1|*|vec2|) in a general way
		//alpha = arcos(vec1*vec2) in unit vectors

		float dotp = dotProduct(dest);

		return (acos(dotp));
	}

	//----- PUBLIC VARIABLES -----
	float _x, _y;          //Values of vector
};

#endif   //_IND_VECTOR2_
