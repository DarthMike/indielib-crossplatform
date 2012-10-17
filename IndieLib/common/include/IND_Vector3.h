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

/**
 @defgroup IND_Vector3 IND_Vector3
 @ingroup Math
 Encapsulates a 3d vector and it's operations
 */

/**@{*/

/**
 Encapsulates a 3d vector and it's operations. Makes easier to work with vector types and encapsultes common operations
 like sum, multiplication, dot product...
 */
class LIB_EXP IND_Vector3 {
public:
    
	/**
     Default constructor, initializes elements to 0
     */
	IND_Vector3(): _x(0), _y(0), _z(0) {
	}
    
	/**
     Designated constructor
     @param newx,newy,newz Values of components
     */
	explicit IND_Vector3(float newx, float newy, float newz): _x(newx), _y(newy), _z(newz)
	{}
	~IND_Vector3()
	{}
    
	/** @name Operators overloading */
    /**@{*/
    
	/**
     Copies element by element all components to the vector
     @param newvector Another vector
     @return Ourselves, modified
     */
	IND_Vector3 &operator = (const IND_Vector3 &newvector) {
		_x = newvector._x;
		_y = newvector._y;
		_z = newvector._z;

		return *this;
	}
	
    /**
     Performs element by element comparison
     @param newvector Another vector
     @return true if all components equal, false otherwise
     */
	inline bool operator == (const IND_Vector3 &newvector) const {
		return (_x == newvector._x && _y == newvector._y && _z == newvector._z);
	}
    
    /**
     Performs element by element comparison
     @param newvector Another vector
     @return true if all components equal, false otherwise
     */
	inline bool operator != (const IND_Vector3 &newvector) const {
		return (_x != newvector._x || _y != newvector._y || _z != newvector._z);
	}
	
    /**
     Performs sum of element by element.
     Means x+x, y+y etc
     @param newvector Another vector
     @return Ourselves, modified
     */
	inline IND_Vector3 operator + (const IND_Vector3 &newvector) const {
		return IND_Vector3(
		          _x + newvector._x,
		           _y + newvector._y,
		           _z + newvector._z);
	}
	
    /**
     Performs sum to ourselves. Doesn't do anything.
     @return Ourselves, modified
     */
	inline const IND_Vector3 &operator + () const {
		return *this;
	}
    
	/**
     Performs sum of element by element.
     Means x+x, y+y etc
     @param tosum Another vector
     @return Ourselves, modified
     */
	inline IND_Vector3 &operator += (const IND_Vector3 &tosum) {
		_x += tosum._x;
		_y += tosum._y;
		_z += tosum._z;

		return *this;
	}
    
	/**
     Sums the same ammount to every element.
     @param offset The value to add to every coordinate
     @return Ourselves, modified
     */
	inline IND_Vector3 &operator += (const float offset) {
		_x += offset;
		_y += offset;
		_z += offset;

		return *this;
	}
    
	/**
     Performs a substraction of element by element.
     Means x+x, y+y etc
     @param newvector Another vector
     @return Ourselves, modified
     */
	inline IND_Vector3 operator - (const IND_Vector3 &newvector) const {
		return IND_Vector3(
		           _x - newvector._x,
		           _y - newvector._y,
		           _z - newvector._z);
	}
    
	/**
     Performs a substraction of element by element.
     Means x+x, y+y etc
     @param torest Another vector
     @return Ourselves, modified
     */
	inline IND_Vector3 &operator -= (const IND_Vector3 &torest) {
		_x -= torest._x;
		_y -= torest._y;
		_z -= torest._z;

		return *this;
	}
    
	/**
     Subtracts the same ammount to every element.
     @param offset The value to substract to every coordinate
     @return Ourselves, modified
     */
	inline IND_Vector3 &operator -= (const float offset) {
		_x -= offset;
		_y -= offset;
		_z -= offset;

		return *this;
	}
	
    /**
     Negates the vector, element by element.
     @return Ourselves, modified
     */
	inline IND_Vector3 operator - () const {
		return IND_Vector3(-_x, -_y, -_z);
	}
    
	/**
     Multiply by a scalar every element.
     @param scalar The value to multiply to every coordinate
     @return Ourselves, modified
     */
	inline IND_Vector3 operator * (const float scalar) const {
		return IND_Vector3(
		           _x * scalar,
		           _y * scalar,
		           _z * scalar);
	}
    
	/**
     Multiplies corresponding elements of vectors.
     @param rhs Another vector
     @return Ourselves, modified
     */
	inline IND_Vector3 operator * (const IND_Vector3 &rhs) const {
		return IND_Vector3(
		           _x * rhs._x,
		           _y * rhs._y,
		           _z * rhs._z);
	}
    
	/**
     Multiply by a scalar every element.
     @param scalar The value to multiply to every coordinate
     @return Ourselves, modified
     */
	inline IND_Vector3 &operator *= (const float scalar) {
		_x *= scalar;
		_y *= scalar;
		_z *= scalar;
		return *this;
	}
    
	/**
     Multiplies corresponding elements of vectors.
     @param tomultiply Another vector
     @return Ourselves, modified
     */
	inline  IND_Vector3 &operator *= (const IND_Vector3 &tomultiply) {
		_x *= tomultiply._x;
		_y *= tomultiply._y;
		_z *= tomultiply._z;

		return *this;
	}
    
	/**
     Divices by a scalar every element.
     If value is 0, will break!
     @param scalar The value to divide to every coordinate
     @return Ourselves, modified
     */
	inline IND_Vector3 operator / (const float scalar) const {
		assert(scalar != 0.0f);
		float inv = 1.0f / scalar;
		return IND_Vector3(
		           _x * inv,
		           _y * inv,
		           _z * inv);
	}
    
	/**
     Divides corresponding elements of vectors.
     @param divideto Another vector
     @return Ourselves, modified
     */
	inline IND_Vector3 operator / (const IND_Vector3 &divideto) const {
		assert(divideto._x != 0 && divideto._y != 0 && divideto._z != 0);
		return IND_Vector3(
		           _x / divideto._x,
		           _y / divideto._y,
		           _z / divideto._z);
	}
    
	/**
     Divices by a scalar every element.
     If value is 0, will break!
     @param scalar The value to divide to every coordinate
     @return Ourselves, modified
     */
	inline IND_Vector3 &operator /= (const float scalar) {
		assert(scalar != 0.0f);

		float inv = 1.0f / scalar;
		_x *= inv;
		_y *= inv;
		_z *= inv;
		return *this;
	}
    
	/**
     Divides corresponding elements of vectors.
     @param todivide Another vector
     @return Ourselves, modified
     */
	inline IND_Vector3 &operator /= (const IND_Vector3 &todivide) {
       assert(todivide._x != 0 && todivide._y != 0 && todivide._z != 0);
		_x /= todivide._x;
		_y /= todivide._y;
		_z /= todivide._z;
		return *this;
	}

	/**@}*/
    
    /** @name Vector utilities */
    
	/**
     Computes the length of the vector.
     
     Calculation is sqrt((_x * _x) + (_y * _y) + (_z * _z)))
     
     @return Length of vector value
     */
	inline float length() {
		return(sqrt((_x * _x) + (_y * _y) + (_z * _z)));
	}

    /**
     Computes dot product of two vectors.
     @return Value of dot product
     */
	inline float dotProduct(const IND_Vector3 &vec) const {
		return _x * vec._x + _y * vec._y + _z * vec._z;
	}

    /**
     Computes cross product of two vectors.
     @param other The vector to make cross product with
     @return Vector result of cross product
     */
	inline IND_Vector3 crossProduct(const IND_Vector3 &other) const {
		return IND_Vector3(
		           _y * other._z - _z * other._y,
		           _z * other._x - _x * other._z,
		           _x * other._y - _y * other._x);
	}

	/**
     Normalizes the vector.
     
     Operation computes length of vector, and divides every vector component by it.
     @return Length of vector (before being normalized)
     */
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
    
	/**
     Computes the distance to another vector
     
     Calculates difference of components, (x2-x1,y2-y1,z2-z1) and gets the length of it.
     
     @param to The vector to where distance is to be computed (that is x2) from (x2-x1,y2-y1,z2-z1)
     @return Length of vector differenct between this and the other one
     */
	inline float distance(const IND_Vector3 &to) const {
		return (*this - to).length();
	}

	/**
     Computes the angle between two vectors. 
     Angle between vectors:
     alpha = arcos((vec1*vec2)/|vec1|*|vec2|) in a general way
     alpha = arcos(vec1*vec2) in unit vectors
     
     @param dest Vector  in
     @return value of angle. This will be signed angle
     */
	inline float angleBetweenUnitVecs(const IND_Vector3 &dest) {
		float dotp = dotProduct(dest);

		return (acos(dotp));
	}
    
    /**@}*/
    
    /** @name Public Values */
    /**@{*/
	float _x, _y, _z; ///< Values of vector
    /**@}*/
};

/**@}*/

#endif   //_IND_VECTOR3_
