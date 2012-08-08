/*****************************************************************************************
 * File: IND_Vector3.h
 * Dess: 3D Vector implementation
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


#ifndef _IND_VECTOR3_
#define _IND_VECTOR3_

//Libraries dependencies
#include <math.h>
#include <assert.h>

//Classes dependencies

class IND_Vector3 {
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	//Default constructor
	IND_Vector3(): _x(0), _y(0), _z(0) {
	}
	//Must use this constructor to create a vector with data
	explicit IND_Vector3(float newx, float newy, float newz): _x(newx), _y(newy), _z(newz)
	{}
	~IND_Vector3()
	{}
	//****Operators overloading****
	//Equaling to other vector
	IND_Vector3 &operator = (const IND_Vector3 &newvector) {
		_x = newvector._x;
		_y = newvector._y;
		_z = newvector._z;

		return *this;
	}
	//Comparison between vectors
	inline bool operator == (const IND_Vector3 &newvector) const {
		return (_x == newvector._x && _y == newvector._y && _z == newvector._z);
	}
	inline bool operator != (const IND_Vector3 &newvector) const {
		return (_x != newvector._x || _y != newvector._y || _z != newvector._z);
	}
	//Arithmetic
	//Sum of coordinates
	inline IND_Vector3 operator + (const IND_Vector3 &newvector) const {
		return IND_Vector3(
		          _x + newvector._x,
		           _y + newvector._y,
		           _z + newvector._z);
	}
	//Self sum....
	inline const IND_Vector3 &operator + () const {
		return *this;
	}
	//Sum updating - Point to point
	inline IND_Vector3 &operator += (const IND_Vector3 &tosum) {
		_x += tosum._x;
		_y += tosum._y;
		_z += tosum._z;

		return *this;
	}
	//Sum updating - Offset to every coordinate
	inline IND_Vector3 &operator += (const float offset) {
		_x += offset;
		_y += offset;
		_z += offset;

		return *this;
	}
	//Rest of coordinates
	inline IND_Vector3 operator - (const IND_Vector3 &newvector) const {
		return IND_Vector3(
		           _x - newvector._x,
		           _y - newvector._y,
		           _z - newvector._z);
	}
	//Rest Update
	inline IND_Vector3 &operator -= (const IND_Vector3 &torest) {
		_x -= torest._x;
		_y -= torest._y;
		_z -= torest._z;

		return *this;
	}
	//Rest updating - Offset to every coordinate
	inline IND_Vector3 &operator -= (const float offset) {
		_x -= offset;
		_y -= offset;
		_z -= offset;

		return *this;
	}
	//Rest or negative vector
	inline IND_Vector3 operator - () const {
		return IND_Vector3(-_x, -_y, -_z);
	}
	//Scalar multiply
	inline IND_Vector3 operator * (const float scalar) const {
		return IND_Vector3(
		           _x * scalar,
		           _y * scalar,
		           _z * scalar);
	}
	//Point to point multiply
	inline IND_Vector3 operator * (const IND_Vector3 &rhs) const {
		return IND_Vector3(
		           _x * rhs._x,
		           _y * rhs._y,
		           _z * rhs._z);
	}
	//Multiply update - Multiply to scalar
	inline IND_Vector3 &operator *= (const float scalar) {
		_x *= scalar;
		_y *= scalar;
		_z *= scalar;
		return *this;
	}
	//Multiply update - Point to point multiply
	inline  IND_Vector3 &operator *= (const IND_Vector3 &tomultiply) {
		_x *= tomultiply._x;
		_y *= tomultiply._y;
		_z *= tomultiply._z;

		return *this;
	}
	//Scalar divide
	inline IND_Vector3 operator / (const float scalar) const {
		assert(scalar != 0.0f);
		float inv = 1.0f / scalar;
		return IND_Vector3(
		           _x * inv,
		           _y * inv,
		           _z * inv);
	}
	//Point to point divide
	inline IND_Vector3 operator / (const IND_Vector3 &divideto) const {
		assert(divideto._x != 0 && divideto._y != 0 && divideto._z != 0);
		return IND_Vector3(
		           _x / divideto._x,
		           _y / divideto._y,
		           _z / divideto._z);
	}
	//Divide update - Divide to scalar
	inline IND_Vector3 &operator /= (const float scalar) {
		assert(scalar != 0.0f);

		float inv = 1.0f / scalar;
		_x *= inv;
		_y *= inv;
		_z *= inv;
		return *this;
	}
	//Divide update - Point to point divide
	inline IND_Vector3 &operator /= (const IND_Vector3 &todivide) {
       assert(todivide._x != 0 && todivide._y != 0 && todivide._z != 0);
		_x /= todivide._x;
		_y /= todivide._y;
		_z /= todivide._z;
		return *this;
	}

	//******************

	//----- GET/SET FUNCTIONS -----
	//No Get/Set - PUBLIC VARIABLES
	//----- OTHER FUNCTIONS -----
	//Length of vector
	inline float length() {
		return(sqrt((_x * _x) + (_y * _y) + (_z * _z)));
	}

	inline float dotProduct(const IND_Vector3 &vec) const {
		return _x * vec._x + _y * vec._y + _z * vec._z;
	}

	inline IND_Vector3 crossProduct(const IND_Vector3 &other) const {
		return IND_Vector3(
		           _y * other._z - _z * other._y,
		           _z * other._x - _x * other._z,
		           _x * other._y - _y * other._x);
	}

	//Normalize of vector
	inline float normalise() {
		float l = length();

		// Protection on floating point calcs
		if (l > 1e-08) {
			float invlength = 1.0f / l;
			_x *= invlength;
			_y *= invlength;
			_z *= invlength;
		}

		return l;
	}
	//Return distance to another vector
	inline float distance(const IND_Vector3 &to) const {
		return (*this - to).length();
	}

	//Return angle between this and another vector
	inline float angleBetweenUnitVecs(const IND_Vector3 &dest) {
		//Angle between vectors:
		//alpha = arcos((vec1*vec2)/|vec1|*|vec2|) in a general way
		//alpha = arcos(vec1*vec2) in unit vectors

		float dotp = dotProduct(dest);

		return (acos(dotp));
	}

	//----- PUBLIC VARIABLES -----
	float _x, _y, _z;          //Values of vector
};

#endif   //_IND_VECTOR3_
