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

#ifndef _IND_CAMERA3D_
#define _IND_CAMERA3D_


// ----- Includes -----

#ifdef INDIERENDER_DIRECTX
#include <d3d9.h>
#include <d3dx9.h>
#endif

#ifdef INDIERENDER_OPENGL
#include "Defines.h"
#endif

#ifdef INDIERENDER_GLES_IOS
#include "Defines.h"
#endif

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
