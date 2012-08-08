/*****************************************************************************************
 * File: IND_Camera2d.h
 * Dess: Camera for 2d scenes
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
#include "Defines.h"
#include "IND_Camera2d.h"

//TODO: THE CAMERA SHOULD MAINTAIN IT'S OWN TRANSFORMATION MATRIX. RENDERER CODE IS ACCESSING
//PRIVATE IVARS OF THIS CLASS UNNECESSARILY
// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
 * Initializes a 2d camera and sets it in a position. In order to display on all of the
 * screen's area this position is usually defined as the middle of the screen.
 * @param pX				X Position of the camera.
 * @param pY				Y Position of the camera.
 */
IND_Camera2d::IND_Camera2d(float pX, float pY) {
	_pos._x = pX;
	_pos._y = pY;
	_pos._z = 0.5f;

	_look   = IND_Vector3(0.0f, 0.0f, -1.0f); 
	_up     = IND_Vector3(0.0f, -1.0f, 0.0f); 
	_right  = IND_Vector3(1.0f, 0.0f, 0.0f);
	
	_zoom = 1.0f;
	_angle = 0.0f;
	_prevAngle = 0.0f;
}


IND_Camera2d::~IND_Camera2d() {

}


// --------------------------------------------------------------------------------
//										 Public methods
// --------------------------------------------------------------------------------


/**
 * Sets the position of the camera. In order to display on all of the
 * screen's area this position is usually defined as the middle of the screen.
 * Please note that sets the position not using orientation, so this method is independent of the 
 * current orientation value of the camera. Generally use this to setup the camera initially to some spot
 * when setting up a scene.
 * @param pX				X Position of the camera.
 * @param pY				Y Position of the camera.
 */
void IND_Camera2d::setPosition(float pX, float pY) {
	_pos._x = pX;
	_pos._y = pY;
}

/**
 * Moves the camera a desired amount using the 'right' axis. The 'right' axis will be generally
 * x axis if camera is not rotated. This movement, thus, takes into account camera rotation.
 * @param pStrafe			Amount to move.
 */
void IND_Camera2d::moveStrafe(float pStrafe) {
	_pos._x += _right._x * pStrafe;
	_pos._y += _right._y * pStrafe;
	_pos._z += _right._z * pStrafe;
}


/**
 * Moves the camera a desired amount using the 'up' axis. The 'up' axis will be generally
 * y axis if camera is not rotated. This movement, thus, takes into account camera rotation.
 * @param pFly				Amount to move.
 */
void IND_Camera2d::moveFly(float pFly) {
	_pos._x += _up._x * pFly;
	_pos._y += _up._y * pFly;
	_pos._z += _up._z * pFly;
}

/**
 * Zooms the camera. Use this in order to zoom in or out of the scenery.
 * Default: 1.0f.
 * @param pZoom				Zoom factor.
 */
void IND_Camera2d::setZoom(float pZoom) {
	if (pZoom < 0.0f) {
		pZoom = (-1.0f)*pZoom;
	} 
	_zoom = pZoom;
}


/**
 * Sets the angle of the camera. Use this in order to rotate the scene.
 * It will affect successive calls to moveStrafe and moveFly methods.
 * Default: 0.0f
 * @param pAngle			Camera angle in degrees.
 */
void IND_Camera2d::setAngle(float pAngle) {
	//Transformation of axes is done by renderer class.
	//this caches the previous angle to know how much the angle has changed.
	_prevAngle = _angle;
	_angle = pAngle;
}
