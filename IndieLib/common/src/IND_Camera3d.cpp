/*****************************************************************************************
 * File: IND_Camera3d.h
 * Dess: Camera for 3d scenes
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

// ----- Includes -----

#include "Global.h"
#include "IND_Camera3D.h"
#include "IND_Math.h"

#ifdef INDIERENDER_DIRECTX
#include <d3d9.h>
#include <d3dx9.h>
#endif

// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
 * @brief Initializes a 3d camera and sets it in a position.
 * @param pX				X Position of the camera.
 * @param pY				Y Position of the camera.
 * @param pZ				Z Position of the camera.
 */
IND_Camera3d::IND_Camera3d(float pX, float pY, float pZ) {
	// Position
	_pos._x = pX;
	_pos._y = pY;
	_pos._z = pZ;

	// Orientation
	_yaw = 0;
	_pitch = 0;
	_roll = 0;

	// Rest of values
	_zoom = 1.0f;
#ifdef INDIERENDER_DIRECTX
	_fov = D3DX_PI / 4;
#else
	_fov = PI / 4;
#endif
	_aspect = (float) 800 / (float) 600;
	_nearClippingPlane = 0.01f;
	_farClippingPlane = 100.0f;
	_ortho = 0;
	_orthoWidth = _orthoHeight = 0;
}


IND_Camera3d::~IND_Camera3d() {

}


// --------------------------------------------------------------------------------
//										 Public methods
// --------------------------------------------------------------------------------

float IND_Camera3d::getPosX()  {
	return _pos._x;
}

float IND_Camera3d::getPosY()  {
	return _pos._y;
}

float IND_Camera3d::getPosZ()  {
	return _pos._z;
}

float IND_Camera3d::GetLookX()  {
	return _look._x;
}

float IND_Camera3d::GetLookY()  {
	return _look._y;
}

float IND_Camera3d::GetLookZ()  {
	return _look._z;
}

float IND_Camera3d::getUpX()  {
	return _up._x;
}

float IND_Camera3d::getUpY()  {
	return _up._y;
}

float IND_Camera3d::getUpZ()  {
	return _up._z;
}

float IND_Camera3d::getRightX()  {
	return _right._x;
}

float IND_Camera3d::getRightY()  {
	return _right._y;
}

float IND_Camera3d::getRightZ()  {
	return _right._z;
}

/**
 * @brief Sets the position of the camera.
 * @param pX				X Position of the camera.
 * @param pY				Y Position of the camera.
 * @param pZ				Z Position of the camera.	
 */
void IND_Camera3d::setPosition(float pX, float pY, float pZ) {
	_pos._x = pX;
	_pos._y = pY;
	_pos._z = pZ;
}

/**
 * @brief Zooms the camera. Use this if you want to zoom in or out the scenery.
 * Default: 1.0f
 * @param pZoom				Zoom factor.
 */
void IND_Camera3d::setZoom(float pZoom) {
	if (pZoom < 0.0f) {
		pZoom = (-1.0f)*pZoom;
	} 
	_zoom = pZoom;
}

/**
 * @brief Sets the angle of camera rotation around the roll axis.
 * Default: 0.0f
 * @param pRoll				Roll angle in degrees.
 */
void IND_Camera3d::setRoll(float pRoll) {
	_roll = pRoll;
}

/**
 * @brief Sets the angle of camera rotation around the pitch axis.
 * Default: 0.0f
 * @param pPitch			Pitch angle in degrees.
 */
void IND_Camera3d::setPitch(float pPitch) {
	_pitch = pPitch;
}

/**
 * @brief Sets the angle of camera rotation around the yaw axis.
 * Default: 0.0f
 * @param pYaw				Yaw angle in degrees.
 */
void IND_Camera3d::setYaw(float pYaw) {
	_yaw = pYaw;
}

/**
 * @brief Moves the camera from it's current position towards the direction it is facing.
 * @param pValue			Distancy to "walk" the camera.
 */
void IND_Camera3d::moveWalk(float pValue) {
	// Advance
	_pos._y += _look._y * pValue;
	_pos._x += _look._x * pValue;
	_pos._z += _look._z * pValue;
}

/**
 * @brief Moves the camera from it's current position in right / left directions.
 * @param pStrafe			Distancy to "strafe" the camera.
 */
void IND_Camera3d::moveStrafe(float pStrafe) {
	_pos._y += _right._y * pStrafe;
	_pos._x += _right._x * pStrafe;
	_pos._z += _right._z * pStrafe;
}

/**
 * @brief Moves the camera from it's current position in up / left right directions.
 * @param pFly				Distancy to "fly" the camera.
 */
void IND_Camera3d::moveFly(float pFly) {
	_pos._x += _up._x * pFly;
	_pos._y += _up._y * pFly;
	_pos._z += _up._z * pFly;
}

/**
 * @brief Sets the field of view of the camera.
 * Default: ( DirectX : D3DX_PI / 4 ) ( OpenGl : PI / 4 )
 * @param pFov				Fov value.
 */
void IND_Camera3d::setFov(float pFov) {
	_fov = pFov;
}

/**
 * @brief Sets the aspect of camera.
 * Default: 800 / 600
 * @param pAspect			Aspect value.
 */
void IND_Camera3d::setAspect(float pAspect) {
	_aspect = pAspect;
}

/**
 * @brief Sets the distancy to the near plane.
 * Default: 0.01f
 * @param pNearPlane			Distancy to the near plane.
 */
void IND_Camera3d::setNearClippingPlane(float pNearPlane) {
	_nearClippingPlane = pNearPlane;
}

/**
 * @brief Sets the distancy to the far plane.
 * Default: 100.0f
 * @param pFarPlane			Distancy to the far plane.
 */
void IND_Camera3d::setFarClippingPlane(float pFarPlane) {
	_farClippingPlane = pFarPlane;
}

/**
 * @brief Activates / deactivates the orthonormal projection.
 * Default: 0
 * @param pOrtho			Orhot projection 0 / 1 = off / on.
 */
void IND_Camera3d::toggleOrthoProjection(bool pOrtho) {
	_ortho = pOrtho;
}

/**
 * @brief Sets the width of the ortho projection. 
 * This value should usually be the same
 * as the viewport width, meaning you can set this value to IND_Render::getViewPortWidth()
 * Default: 0
 * @param pOrthoWidth			Width value for the ortho projection.
 */
void IND_Camera3d::setOrthoWidth(float pOrthoWidth) {
	_orthoWidth = pOrthoWidth;
}

/**
 * @brief Sets the height of the ortho projection. 
 * This value should usually be the same
 * as the viewport height, meaning you can set this value to IND_Render::getViewPortHeight()
 * Default: 0
 * @param pOrthoHeight			Hight value for the ortho projection.
 */
void IND_Camera3d::setOrthoHeight(float pOrthoHeight) {
	_orthoHeight = pOrthoHeight;
}
