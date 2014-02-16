/*****************************************************************************************
 * File: IND_Camera2d.h
 * Desc: Camera for 2d scenes
 *****************************************************************************************/

/*********************************** The zlib License ************************************
 *
 * Copyright (c) 2013 Indielib-crossplatform Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 *****************************************************************************************/


#ifndef _IND_CAMERA2D_
#define _IND_CAMERA2D_


// ----- Includes -----

#include "IndiePlatforms.h"
#include "IND_Vector3.h"

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
    friend class OpenGLES2Render;

    /** @endcond */
};

/**@}*/

#endif // _IND_CAMERA_
