/*****************************************************************************************
 * File: IND_Light.h
 * Desc: Light object
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
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

#ifndef _IND_LIGHT_
#define _IND_LIGHT_

// ----- Forward declarations -----
struct LIGHT;

// ----- Includes -----

#include "Defines.h"

#ifdef INDIERENDER_DIRECTX
#include <d3d9.h>
#include <d3dx9.h>
#endif

#ifdef INDIERENDER_GLES_IOS
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#endif

// --------------------------------------------------------------------------------
//									 IND_Light
// --------------------------------------------------------------------------------

/**
@defgroup IND_Light IND_Light
@ingroup Objects
IND_Light class managed by IND_LightManager for managing lights, click in IND_Light to see all the methods of this class.
*/
/**@{*/

/**
@b IND_Image is an image object from the class ::IND_ImageManager. Read the explanation in ::IND_ImageManager for having more details.
*/
class LIB_EXP IND_Light {
public:
	// ----- Init/End -----
	IND_Light();
	~IND_Light();
	// ----- Public sets------

	void setColor(float pR, float pG, float pB, float pA);
	void setPosition(float pX, float pY, float pZ);
	void setDirection(float pD1, float pD2, float pD3);
	void setAttenuation(float pAttenuation);
	void setRange(float pRange);
	void setTheta(float pTheta);
	void setPhi(float pPhi);
	void setFalloff(float pFalloff);

	// ----- Public gets ------

	//! Returns true if the light is enabled, false otherwise.
	bool isEnable();
	//! Returns red color component of the light.
	float getColorR();
	//! Returns green color component of the light.
	float getColorG();
	//! Returns blue color component    of the light.
	float getColorB();
	//! Returns alpha  component    of the light.
	float getColorA();
	//! Returns x position of the light.
	float getPosX();
	//! Returns y position of the light.
	float getPosY();
	//! Returns z position of the light.
	float getPosZ();
	//! Returns the first component of the vector which defines the light direction.
	float getDirP1();
	//! Returns the second component of the vector which defines the light direction.
	float getDirP2();
	//! Returns the third component of the vector which defines the light direction.
	float getDirP3();
	//! Returns the attenuation factor of the light.
	float getAttenuation();
	//! Returns the range factor of the light.
	float getRange();
	//! Returns the range theta of the light.
	float getTheta();
	//! Returns the phi factor of the light.
	float getPhi();
	//! Returns the falloff factor of the light.
	float getFalloff();

private:

	// ----- Structures ------
	LIGHT *_light;

	// ----- Friends -----

	friend class IND_LightManager;

};
/**@}*/

#endif // _IND_LIGHT_
