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

#ifndef _IND_CAMERA2D_
#define _IND_CAMERA2D_


// ----- Includes -----
#include "Defines.h"

#ifdef INDIERENDER_DIRECTX
#include <d3d9.h>
#include <d3dx9.h>
#endif

// --------------------------------------------------------------------------------
//									   IND_Camera
// --------------------------------------------------------------------------------

/**
@defgroup Camera2d Camera2d
@ingroup Cameras
*/
/**@{*/
/**
 @brief This is the camera for 2d scenarios. 
 Using this class you can control all the parameters of a 2d camera. You
 can have as many cameras as you want. Use ::IND_Render::setCamera2d(IND_Camera2d *pCamera2d) method before drawing in order
 to choose the camera you want to use.
 */
class LIB_EXP IND_Camera2d { //ñ
public:

	// ----- Init/End -----

	IND_Camera2d(float pX, float pY);
	~IND_Camera2d();

	// ----- Public methods -----

	//! Sets the position of the camera without taking into account the orientations
	void setPosition(float pX, float pY);

	//! Sets the position of the camera taking into account the orientation (x value)
	void moveStrafe(float pStrafe);
	
	//! Sets the position of the camera taking into account the orientation (y value)
	void moveFly(float pFly);
	
	//! Sets the zoom value of the camera. This is independent to orientation or position
	void setZoom(float pZoom);

	//! Sets the angle value of the camera. Will affect to IND_Camera2d#moveStrafe(float) and IND_Camera2d#moveFly(float) calls
	void setAngle(float pAngle);

	// ----- Public gets -----

	//! Returns the x position of the camera
	float getPosX()  {
		return _pos._x;
	}
	//! Returns the y position of the camera
	float getPosY()  {
		return _pos._y;
	}
	//! Returns the zoom of the camera
	float getZoom()  {
		return _zoom;
	}
	//! Returns the angle of the camera
	float getAngle()  {
		return _angle;
	}

private:

    /** @cond DOCUMENT_PRIVATEAPI */
	// ----- Private -----

	bool _ok;

	// ----- Vars -----

	IND_Vector3 _pos;
	IND_Vector3 _look;
	IND_Vector3 _up;
	IND_Vector3 _right;

	float _zoom;
	float _angle;
	float _prevAngle;
	// ----- Private methods -----

	void                initVars();
	void                freeVars();

	// ----- Friends -----

	friend class DirectXRender;
	friend class OpenGLRender;
    /** @endcond */
};

/**@}*/

#endif // _IND_CAMERA_
