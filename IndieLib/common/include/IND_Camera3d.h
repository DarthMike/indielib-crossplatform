/*****************************************************************************************
 * File: IND_Camera3d.h
 * Desc: Camera for 3d scenes
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


#ifndef _IND_CAMERA3D_
#define _IND_CAMERA3D_


// ----- Includes -----

#include "IndiePlatforms.h"
#include "IND_Vector3.h"

// --------------------------------------------------------------------------------
//									   IND_Camera
// --------------------------------------------------------------------------------

/**
@defgroup Camera3d Camera3d
@ingroup Cameras
*/
/**@{*/
/**
 @brief Camera for 3d scenarios. 
 Using this class you can control all the parameters of a 3d camera. You
 can have as many cameras as you want. Use ::IND_Render::setCamera3d(IND_Camera3d *pCamera2d) method before drawing in order
 to choose the camera you want to use.
 */
class LIB_EXP IND_Camera3d { //ñ
public:

	// ----- Init/End -----

	IND_Camera3d(float pX, float pY, float pZ);
	~IND_Camera3d();

	// ----- Public methods -----

	// View
	void setPosition(float pX, float pY, float pZ);
	void setPitch(float pAngle);
	void setYaw(float pAngle);
	void setRoll(float pAngle);
	void setZoom(float pZoom);
	void moveWalk(float pWalk);
	void moveFly(float pFly);
	void moveStrafe(float pStrafe);

	// Projection
	void setFov(float pFov);
	void setAspect(float pAspect);
	void setNearClippingPlane(float pNearPlane);
	void setFarClippingPlane(float pFarPlane);
	void toggleOrthoProjection(bool pOrtho);
	void setOrthoWidth(float pOrthoWidth);
	void setOrthoHeight(float pOrthoHeight);

	// ----- Public gets -----

	//! Returns the x position of the camera
	float getPosX();

	//! Returns the y position of the camera
	float getPosY();

	//! Returns the z position of the camera
	float getPosZ();

	//! Returns the first component of the lookat camera vector
	float GetLookX();

	//! Returns the second component of the lookat camera vector
	float GetLookY();

	//! Returns the third component of the lookat camera vector
	float GetLookZ();

	//! Returns the first component of the up camera vector
	float getUpX();

	//! Returns the second component of the up camera vector
	float getUpY();

	//! Returns the third component of the up camera vector
	float getUpZ();

	//! Returns the first component of the right camera vector
	float getRightX();

	//! Returns the second component of the right camera vector
	float getRightY();

	//! Returns the third component of the right camera vector
	float getRightZ();

	//! Returns the zoom factor  of the camera
	float getZoom()  {
		return _zoom;
	}
	//! Returns the fov of the camera
	float getFov()  {
		return _fov;
	}
	//! Returns the aspect of the camera
	float getAspect()  {
		return _aspect;
	}
	//! Returns the near clipping plane of the camera
	float getNearClippingPlane()  {
		return _nearClippingPlane;
	}
	//! Returns the far clipping plane of the camera
	float getFarClippingPlane()  {
		return _farClippingPlane;
	}
	//! Returns true if the camera has an ortho projection assigned
	bool isOrthoProjection()  {
		return _ortho;
	}
	//! Returns the width of the camera ortho projection
	float getOrthoWidth()  {
		return _orthoWidth;
	}
	//! Returns the height of the camera ortho projection
	float getOrthoHeight()  {
		return _orthoHeight;
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

	float _pitch;
	float _yaw;
	float _roll;

	float _zoom;
	float _fov;
	float _aspect;
	float _nearClippingPlane;
	float _farClippingPlane;
	bool _ortho;
	float _orthoWidth, _orthoHeight;

	// ----- Private methods -----

	void                initVars();
	void                freeVars();

	// ----- Friends -----

	friend class DirectXRender;
	friend class OpenGLRender;

    /** @endcond */
};
/**@}*/

#endif // _IND_CAMERA3D_
